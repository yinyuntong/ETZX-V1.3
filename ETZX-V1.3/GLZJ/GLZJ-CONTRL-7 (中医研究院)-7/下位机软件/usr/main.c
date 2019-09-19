/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:main.c
*�ļ�����    		:������
*������     		:����ͬ
*��������   		:2009-4-9
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_MAIN_
#include "config.h"

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ

//��̬�����ڴ��С����
#define MALLOC_MEM_SIZE		300
uint8 			xdata 			byMallocMem[MALLOC_MEM_SIZE];
//��������
STLocalControl  xdata 			stLocalControl;
Queue DataQueue;//����һ�����ݶ���

void LedInit(void);
void Delayms(uint16 ms);


bit bLedState=0;
bit bPowerEnState;
bit	bPCState=0;
bit PowerSwPrePress=0;
bit PowerSwPressed=0;
bit PowerSwPreUp=1;
bit PowerSwUped=1;

bit bKeyDownTreat=0;


uint8 mSecond=0;
uint8 Second=0;

void TIMER1Int(void) interrupt T1_INTNO		//3���ж�
{
	ET1=0;
}

#if STC12C5A==true
void AdcInt(void)	interrupt ADC_INTNO		//5���ж�
{
	EADC=0;
}

void LvdInt(void) interrupt LVD_INTNO		//6���ж�
{
	ELVD=0;
}

//void PcaInt(void) interrupt PCA_INTNO		//7���ж�
//{
//	CMOD	&=	!ECF;			//��ֹ PCA����������ж�   
//	CCAPM0	&=	!PCA0_ECCF;		//��ֹCCF0�ж�
//	CCAPM1	&=	!PCA1_ECCF;		//��ֹCCF1�ж�	
//}

void SpiInt(void) interrupt SPI_INTNO		//9���ж�
{
	IE2 &= ~ESPI;
}

#endif


void Mp3Rst(void)
{
	MP3_RST =1;		//�ߵ�ƽ��λ
	Delayms(1000);
	MP3_RST =0;	
}


/*---------------------------------------------------------------------------
����ԭ��: void Delayms(uint16 ms)
����˵��: ms--��Ҫ��ʱ��ֵ
�� �� ֵ: ��
�������ܣ���ʱ����(����18.432M����ָ��������ʱ1mS)
----------------------------------------------------------------------------*/
void Delayms(uint16 ms)	  
{
   uint16 xdata i;
   for(;ms!=0;ms--)
		for(i=900;i!=0;i--);
}


uint8 ParaCrcCheck(uint16 addr)
{
	uint8 crcResult=0;
	uint8 crcData =0;
	uint8 i;
	uint8 paraData;
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		paraData = IapReadByte(addr);
		crcResult +=paraData; /*IapReadByte(addr)*/;
		addr++;
	}
	crcData = IapReadByte(addr);
	DisableIAP();
	
	if(crcData == crcResult) return 1;
	else return 0;
}



uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}
	DisableIAP();
	return crcResult;
}



/**********************************************************
*��������			:ReadParameter	
*��������        	:��Flash�е��������ݶ��뵽�ڴ���
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:EnableIAP,IapReadByte,DisableIAP
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void ReadParameter(uint16 addr)
{

	uint8 i=0;
	uint8 *pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	
	EnableIAP(IAP_READ); 
	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		*pbyData = IapReadByte(addr);
		pbyData++;
		addr++;
	}
	DisableIAP();

}


void SaveParameter(uint16 addr)
{
	uint8 i;
	uint16 addr_temp=addr;
	uint8 *pbyData;
	uint8 readtimes;
	uint8 writetimes;
	uint8 crc_temp;

	stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
	WDT_CONTR = 0x3d;	//ι��
	writetimes =3;
	do
	{
		EnableIAP(IAP_ERASE);
		IapErase(addr); 

		
		EnableIAP(IAP_WRITE); 
		addr = addr_temp;
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}

		//��ַ����ǰһ������
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}
		

		EnableIAP(IAP_READ);
		readtimes=3;
		do
		{
			pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<(sizeof(STEepromCfgData))*2;)
			{
				if(*pbyData != IapReadByte(addr)) break;
				
				pbyData++;
				addr++;
				i++;
				if(i== sizeof(STEepromCfgData)) pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			}	

			if(i>= (sizeof(STEepromCfgData)*2))
			{//д��ȥ���������������������ͬ
				break;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			break;	//��ͬ
		}
	}while(--writetimes);

	
	DisableIAP();
}



/**********************************************************
*��������			:ParameterInit	
*��������        	:������ʼ��
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void ParameterInit(void)
{
	uint8 flagData;
	uint8 readtimes=3;
CheckIapSec0:	

	do
	{

		if(ParaCrcCheck(IAP0_ADDR) ==1)  break;
			

	}while(--readtimes);


	if(readtimes)
	{//��һ��У�����ȷ

		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == INIT_FLAG)
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���
			ReadParameter(IAP0_ADDR);
			return;
		}
		else	goto CheckIapSec1;	
	}

	else
	{
CheckIapSec1:	
		readtimes=3;	
		do
		{
			if(ParaCrcCheck(IAP0_ADDR+sizeof(STEepromCfgData)) ==1)  break;
				
		}while(--readtimes);
	
	
		if(readtimes)
		{//�ڶ���У�����ȷ
			EnableIAP(IAP_READ);
			flagData = IapReadByte(IAP0_ADDR+sizeof(STEepromCfgData));
			DisableIAP();
			
			if(flagData == INIT_FLAG)
			{//�����Ѿ���ʼ�������,��ȡ���ݲ���
				ReadParameter(IAP0_ADDR+sizeof(STEepromCfgData));
				SaveParameter(IAP0_ADDR);
				return;
			}
			else	goto UseInitValue;	
		}
		else
		{//��һ��͵ڶ���洢��3�ζ�ȡ��ʧ�ܻ���δ��ʼ����Ӧ�ó�ʼֵ���������浽�ڲ�EEPROM��ȥ
UseInitValue:
			stLocalControl.stEepromCfgData.byInitFlag = INIT_FLAG;
			stLocalControl.stEepromCfgData.bySelfSecAddr = 0x01;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = GLZJ_ADDR2;
			stLocalControl.stEepromCfgData.bySelfBedAddr = GLZJ_ADDR3;
			stLocalControl.stEepromCfgData.uiRs485Brt	 = 57600;		//Ĭ�ϲ�����57600
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

		}	
	}
	DisableIAP();
}


void PCATimerInt(void) interrupt 7
{
	EA=0;

#if STC12C5A==1
		uiIsrTimerCount+=ISR_INC_COUNT;
		CCAP0L = (uint8)uiIsrTimerCount;
		CCAP0H = (uint8)(uiIsrTimerCount>>8);
		CCF0=0;
#endif
	
	
#if USE_COM0==1
		if(stUsartCfg[0].byRecTimeout) 
		{
			stUsartCfg[0].byRecTimeout--;
		}
#endif
	
#if USE_COM1==1
		if(stUsartCfg[1].byRecTimeout) 
		{
			stUsartCfg[1].byRecTimeout--;
			if(stUsartCfg[1].byRecTimeout ==0)
			{
				stUsartCfg[1].byRecState=0;
				stUsartCfg[1].uiRecPosition=0;		
			}
		}
#endif


	TimerOutDeal();

	if(POWER_SW ==0)
	{
		if(PowerSwPrePress==0)   PowerSwPrePress=1;//������������
		else 
		{//�ѳ�������
			if(PowerSwPressed==0)
			{
				PowerSwPressed=1;
				bKeyDownTreat=1;    //������������

				PowerSwPreUp=0;
				PowerSwUped=0;
			}
			else
			{//�����Ѱ���
			}
		}
	}


	else
	{
		if(PowerSwPreUp==0)  PowerSwPreUp=1;
		else 
		{//�����ѳ�������
			if(PowerSwUped==0)
			{
				PowerSwUped=1;
				
				PowerSwPrePress=0;
				PowerSwPressed=0;				
			}
			else 
			{//�����������ѵ���

			}
		}
	}

	if(++mSecond == 50)
	{
		mSecond=0;
		if(++Second ==30)
		{
			Second=0;
			DTimeState |=bDataTimeUpdate;
						
		}
	}
	EA=1;
}

/**********************************************************
*��������			:IoInit	
*��������        	:ϵͳIO�ڳ�ʼ��
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void IoInit(void)
{
	//IO������
	//STC12C52/5Aϵ��:M0=0,M1=0:׼˫��M0=0,M1=1:���裬M0=1,M1=0:���죬M0=1,M1=1:��©
	
//KBC,KBD �������
	P5M0 |= Bin(0,0,0,0,0,0,1,1);
	P5M1 &= (~Bin(0,0,0,0,0,0,1,1));

//KGB�������
	P2M0 |= Bin(0,0,1,0,0,0,0,0);
	P2M1 &= (~Bin(0,0,1,0,0,0,0,0));

	
	KBC	=0;
	KBD =0;
	KGB =0;
	RE0 =1;			//��ֹRS485����
//	DE0 =0;			//��ֹRS485����
	DE0 =1;
	SS	=1;

//P5.3�������		���ڿ�������ʱ�ĸ���
	P5M0 |=   Bin(0,0,0,0,1,0,0,0);
	P5M1 &= (~Bin(0,0,0,0,1,0,0,0));
	GB_FZ=0;

//p1.7 �������            ����24V��Դ����
	P1M0 |=   Bin(1,0,0,0,0,0,0,0);
	P1M1 &= (~Bin(1,0,0,0,0,0,0,0));
	POWER_EN=1;			//24v��Դ��
	bPowerEnState=1;		//24V��Դ״̬��־��1	


	POWER_SW = 1;											//�������ó�������
}
/**********************************************************
*��������			:Init	
*��������        	:������ݳ�ʼ������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Init(void)
{	
	//״̬��ʼ��
	memset(&(stLocalControl.byWaitAckTimeLanding),0x00,sizeof(STLocalControl));
	
	stLocalControl.uiDevState = FLAG_LANDING;	
	stLocalControl.stIndicationData.pstNext = NULL;
	stLocalControl.stVoicePlay.byISD1700State = 0x00;

#if	STC12C5A==1
//	CMOD = 0x02;	//PCA ��ʱFOSC/2 
	CMOD = 0x00;	//PCA ��ʱFOSC/12 
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//����PCAģ��0Ϊ16λ�����ʱ��,ECCF0=1����PCAģ��0�ж�
	CCAPM0 = 0x49;
	
	//����PCA����������
	CR = 1;	

	//��λ���Ź�                                                                                                                                                                                                                                                                                     
//	WDT_CONTR = 0x3e; 		//���Ź���ʱ2.5s@20M
#endif
	//IO�ڳ�ʼ��
	IoInit();

	//mp3_rst mp3����оƬ��ʼ��
	Mp3Rst();
				
	//��Դ�ơ�״̬�Ƴ�ʼ��
	LedInit();
	//�����߳�ʼ��		
	SingleBusInit();
	EA=1;
	Delayms(200);
	//����������ʼ��
	ParameterInit();

	//���ڳ�ʼ��
	Usart0Init(stLocalControl.stEepromCfgData.uiRs485Brt);
	Usart1Init();

	//OSMemInit(byMallocMem,MALLOC_MEM_SIZE);
	init_mempool(byMallocMem,MALLOC_MEM_SIZE);  	//��ʼ���ڴ��
	//�����ʼ��
	stLocalControl.stCallPointer.pstHead = 
	stLocalControl.stCallPointer.pstTail = NULL;

//	AddCallNod((pSTBusFreq)(&(stLocalControl.stBusFreq.bySndSecAddr)));


	MakeCH0TimerOut(1, 0);

	stLocalControl.byChar =0x00;	//ϵͳæ
	Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//����ϵͳæ״̬



 //��λ���Ź�																																																																					  
 	WDT_CONTR = 0x3e;		 //���Ź���ʱ2.5s@20M

}


/**********************************************************
*��������			:main	
*��������        	:ϵͳ������,������������
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-1-4
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
int main(void)
{	
	P4SW = (P44EN|P45EN|P46EN);	//����IO ��
	Delayms(100);

	//ϵͳӲ����ʼ��
	Init();	

	P8563_Read();
	byMinute=stTime.byMinute;
	stLocalControl.stBusFreq.bySndSecAddr = stTime.bySecond;
	stLocalControl.stBusFreq.bySndRoomAddr= stTime.byMinute;
	stLocalControl.stBusFreq.bySndBedAddr =	stTime.byHour;
	stLocalControl.stBusFreq.byCmd		=CMD_DATE_SEND;
	stLocalControl.stBusFreq.byRecSecAddr = stTime.byDay;
	stLocalControl.stBusFreq.byRecRoomAddr= stTime.byMonth;
	stLocalControl.stBusFreq.byRecBedAddr =	stTime.byYear;
	Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

	while(1)
	{
		//��λ���Ź�                                                                                                                                                                                                                                                                                     
		WDT_CONTR = 0x3e; 		//���Ź���ʱ2.5s@20M       
		if(bUsart1RecFinish) Usart1RecDeal();

		if(bBus0RecFinish)									//����0�յ�����
		{ 	
			Bus0RecDeal();
		}
		
		if(bBus0SendFinish)									//����0����������֡
		{
			Bus0SendFinishDeal();		
		} 
		if(bBus1RecFinish)									//����1�յ�����
		{ 	
			Bus1RecDeal();	
		}
		if(bBus1SendFinish)									//����1����������֡
		{
			Bus1SendFinishDeal();		
		} 		

		if(bKeyDownTreat)
		{
			bKeyDownTreat=0;
			KeyDown();
		}

		if(DTimeState&bCH0TimerOutDeal)
		{
			DTimeState &= ~bCH0TimerOutDeal;
			if(stLocalControl.uiDevState & FLAG_LANDING)
			{	//�ϵ�Ǽ�״̬����λ���зֻ�						
				stLocalControl.stBusFreq.bySndSecAddr = 0x00;
				stLocalControl.stBusFreq.bySndRoomAddr = 0x00;
				stLocalControl.stBusFreq.bySndBedAddr = 0x00;
				stLocalControl.stBusFreq.byCmd = CMD_POWER_ON;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				Usart1SndCmd(CMD_LANDING,0x00,NULL);
				stLocalControl.byWaitAckTimeLanding = 250;	//�ȴ�5S
				
			}
			else if(stLocalControl.uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN|
				FLAG_TALK|FLAG_BROADCAST))
			{	//�ȴ�Ӧ��ʱ��
				if((stLocalControl.uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN))&&
					((byUsart1RecBuf[CMD_POSITION] ==CMD_INFUSION_ANSWER)||(byUsart1RecBuf[CMD_POSITION] ==CMD_COMM_CALL)) )
				{//����ڵȴ�Ӧ����ߵȴ�����״̬��,�Ҵ����е�����Ϊ������Һ���л���ͨ����,�����������ɹ���
					Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);
					byUsart1RecBuf[CMD_POSITION] =0x00;		//��մ�����
					
				}				
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
				stLocalControl.stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
				stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
				stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;;					
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}
			else if(stLocalControl.uiDevState & FLAG_INDICATION)
			{
/*					//������ģ�鷢��ֹͣ��������
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				if(0x00 == (stLocalControl.uiDevState & FLAG_STOP_EXIST))
				{	//û�����ڷ���ֹͣ����
//						stLocalControl.stVoicePlay.byISD1700State |= ISD_TIME_OVER;

					stLocalControl.stBusFreq.byCmd = CMD_STOP_INDICATION;						
					if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
					{
						stLocalControl.uiDevState |= FLAG_STOP_EXIST;
					}						
				}

				*/
			}				
		}
		if(DTimeState &bCH1TimerOutDeal)
		{
		    DTimeState &= ~bCH1TimerOutDeal;
			stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stCH1CallAddr.bySndSecAddr;
			stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stCH1CallAddr.bySndRoomAddr;
			stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stCH1CallAddr.bySndBedAddr;
			stLocalControl.stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
			stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stCH1CallAddr.byRecSecAddr;
			stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stCH1CallAddr.byRecRoomAddr;
			stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stCH1CallAddr.byRecBedAddr;			
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));		
		}

		if(DTimeState & bDataTimeUpdate)
		{
			DTimeState &= ~bDataTimeUpdate;
		 	P8563_Read();
			if(byMinute!=stTime.byMinute)
			{
				stTime.bySecond &= 0x7f;	//bit7
				if(stTime.bySecond < 0x60)  
				{
					stTime.byMinute &= 0x7f;	//bit7
					if(stTime.byMinute < 0x60)  
					{
						stTime.byHour &= 0x3f;		//bit7 bit6
						if(stTime.byHour < 0x24)    
						{
							stTime.byDay &= 0x3f;		//bit7 bit6
							if(stTime.byDay < 0x32)   	
							{							
								stTime.byMonth &= 0x1f;		//bit7... bit5
								if(stTime.byMonth < 0x13)  		
								{
									if(stTime.byYear <= 0x99)  
									{
										byMinute=stTime.byMinute;
										stLocalControl.stBusFreq.bySndSecAddr = stTime.bySecond;
										stLocalControl.stBusFreq.bySndRoomAddr= stTime.byMinute;
										stLocalControl.stBusFreq.bySndBedAddr = stTime.byHour;
										stLocalControl.stBusFreq.byCmd		=CMD_DATE_SEND;
										stLocalControl.stBusFreq.byRecSecAddr = stTime.byDay;
										stLocalControl.stBusFreq.byRecRoomAddr= stTime.byMonth;
										stLocalControl.stBusFreq.byRecBedAddr = stTime.byYear;
										Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)); 
									}
								}
							}
						}
					}
				}
			}		
		
		}

		if(DTimeState &bWaitCommAckDTime)
		{
			DTimeState &= ~bWaitCommAckDTime;
			stLocalControl.uiDevState &= ~(FLAG_WAIT_COMM_ACK);
			Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);
	   	
		}

		if(DTimeState & bWaitRS485AckDTime)
		{
			DTimeState &= ~bWaitRS485AckDTime;
			Usart1SndRS485Result(byUsart0SndBuf[CMD_POSITION],FALSE);
		}

	   	if(DTimeState &bWaitAckTimeLand)
		{	
			DTimeState &= ~bWaitAckTimeLand;
			stLocalControl.uiDetectPcTime = 30000;	//����PC�����10����
			stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusFreq.byCmd = CMD_PC_CLOSE;
			stLocalControl.stBusFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusFreq.byRecRoomAddr =0xff;
			stLocalControl.stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)); 		
		}

		if(DTimeState & bDetectPcTime)
		{
			DTimeState &= ~bDetectPcTime;
			Usart1SndCmd(CMD_LANDING,0x00,NULL);
			stLocalControl.byWaitAckTimeLanding = 250;	//�ȴ�5S	
		}

		
		if(DTimeState & bUsart1SdResultDTime)
		{
			DTimeState &= ~bUsart1SdResultDTime;
			Usart1SndRS485Result(byUsart0SndBuf[CMD_POSITION],TRUE);	//���ͽ����
		}	


		if(bRS485DataTreat)
		{
			if(stUsartCfg[0].uiSndLen==0) 
			{//RS485û���������ڷ���
				pSTUartFreq pstDataFreq=byUsart1RecBuf;
				if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_DISPLAY_FORM))
				{
					DE0=1;
//					Delayms(1);
					EA=0;
					memcpy(byUsart0SndBuf,byUsart1RecBuf,pstDataFreq->uiLen+EXT_SIZE);
					EA=1;

					pstDataFreq=byUsart0SndBuf;	
					stUsartCfg[0].pbySndBuf = byUsart0SndBuf;
					stUsartCfg[0].uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg[0].uiSndPos = 0; 	//������֡ͷ����ʼ��������	
					TI=0;
					SBUF= stUsartCfg[0].pbySndBuf[stUsartCfg[0].uiSndPos];

					if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA))
					{
						//����ֻ���ַ					
						stLocalControl.stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
						stLocalControl.stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
						stLocalControl.stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

						//���õȴ�Ӧ��ʱ2S  �ڲ�����Ϊ57.6K�������,100�ֽ�����ֻҪ17.3ms
						stLocalControl.byWaitRS485AckDTime = 95;		//�ȴ�1.9S	

					}
				}
				bRS485DataTreat =0;
			}
		}
	}	
}
	
	
	
void LedStateFlash(void)
{
	if(bLedState==0)
	{
		LED_STATE=1;
		bLedState=1;
	}
	else
	{
		LED_STATE=0;
		bLedState=0;
	}
}

 void LedInit(void)
{
 
	 LED_STATE=0;	//״ָ̬ʾ�ƴ�
	 bLedState=0;

	 LED_POWER=0;	//24v��Դָ�ƴ�
}

