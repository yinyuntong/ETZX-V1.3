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
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_MAIN
#include "config.h"

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//���߳���Ϣ����
STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��

bit bWireLessRec=0;
bit bEnableOxTimer=0;				//���û�յ�������ʼ�������ʱ

uint8 xdata byWireLessRecBuff[10];
uint8 xdata byWireLessRecLen;


uint8 xdata byWireLessSndBuff[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};

uint8 code  	run[12]={31,29,31,30,31,30,31,31,30,31,30,31};   
uint8 code   notrun[12]={31,28,31,30,31,30,31,31,30,31,30,31};


bit bOXSupplyState=0;		//����״̬��־



void Ex0Int(void)	interrupt X0_INTNO		//0���ж�
{
	EX0=0;
}

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
//	CMOD	&=	~ECF;			//��ֹ PCA����������ж�   
//	CCAPM0	&=	~PCA0_ECCF;		//��ֹCCF0�ж�
//	CCAPM1	&=	~PCA1_ECCF;		//��ֹCCF1�ж�	
//}

#endif
/***********************************************************/
void UserTickTimer(void)   /* ϵͳ��ʱ�ж��е��õ��û��� */
{ 
	#if STC90 == 1  
		TF2=0;
	#endif
	#if STC12C5A==1
		uiIsrTimerCount+=ISR_INC_COUNT;
		CCAP0L = (uint8)uiIsrTimerCount;
		CCAP0H = (uint8)(uiIsrTimerCount>>8);
		CCF0=0;
	#endif
	if(stUsartCfg.byRecTimeout!=0) stUsartCfg.byRecTimeout--;
}



void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
    uint8 xdata i;
	Databuf=Databuf;
	l=l;
	i=i;
#if DEBUG==1
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	WDT_CONTR = 0x3e;	//ι��
    ES=0;
	for(i=0;i<l;i++)
     {
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;
	 }
	ES=1;
#endif

/*#if DEBUG==1
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	stUsartCfg.uiSndLen=l;
	stUsartCfg.uiSndPos=0;
	stUsartCfg.pbySndBuf=Databuf;
	SBUF=stUsartCfg.pbySndBuf[0];
#endif
*/
}


void Send_Data_Byte(uint8 SendData)
{ 
	uint8 xdata i;
	SendData=SendData;
	i=i;
#if DEBUG ==1
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	WDT_CONTR = 0x3e;	//ι��
	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
#endif

/*#if DEBUG==1	
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	stUsartCfg.uiSndLen=1;
	stUsartCfg.uiSndPos=0; 
	SBUF=SendData;	
#endif
*/
}


void RD_ID(void)
{
	uint8 idata *p;
	p=0xf1;
	Send_Data(p,7);
}

bit DirRFSerialCompare()	//���к����λ����
{
	if(stLocalControl.stEepromCfgData.byRFSerialNum1!= byWireLessRecBuff[1])
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum2!= byWireLessRecBuff[2])
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum3!= byWireLessRecBuff[3])
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum4!= byWireLessRecBuff[4])
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum5!= byWireLessRecBuff[5])
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum6!= byWireLessRecBuff[6])
	{	
		return(0);
	}
	
	return(1); 	
}


uint16   allday(STTime   a)  	//���ض����� 
{    
	uint16 xdata  x; 
	uint8 xdata i;
	x=(a.byYear)*365+a.byYear/4;   
	if((a.byYear%4)==0)  
	{   
		for(i=0;i<a.byMonth;i++)   
		{   
			if(i>0)   
			{   
				 x=x+run[i-1];   
			}   
		}   
	}   
	else   
	{   
		for(i=0;i<a.byMonth;i++)   
		{   
			if(i>0)   
			{   
				x=x+notrun[i-1];   
			}   
		}   
	}   
	x=x+a.byDay;   
	return   x;   
}  

STOXTime   timeInterval(STTime a,STTime   b)  	//a:����ʱ��  b:��ʼʱ��  
{   
  uint16   xdata x,y;   
  STOXTime   xdata sum;   
  x=allday(a);   
  y=allday(b);   
  sum.uiHour=(x-y)*24+a.byHour-b.byHour;   
  if((a.byMin)<b.byMin)   
  {   
  	sum.byMin=a.byMin+60-b.byMin;
	sum.uiHour=sum.uiHour-1;   
  }
  else  
  {   
	sum.byMin=a.byMin-b.byMin;   
  } 

  if((a.bySec)<b.bySec)   
  {   
  	sum.bySec=a.bySec+60-b.bySec;
	sum.byMin=sum.byMin-1;   
  }
  else  
  {   
	sum.bySec=a.bySec-b.bySec;   
  }
  return   sum;   
 }   

/**********************************************************
*��������			:Init	
*��������        	:Ӳ����ʼ������
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Init(void)
{
	CMOD = 0x02;
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//����PCAģ��0Ϊ16λ�����ʱ��,ECCF0=1����PCAģ��0�ж�
	CCAPM0 = 0x49;
 	 
//	TF1=0;
//	CCAPM1 = (PCA1_CAPN|PCA1_ECCF);		//PCA1�����ж�
//	AUXR1 |= PCA_P4;		//ӳ�䵽P4��

	//����PCA����������
	CR = 1;	
	

	//IO������
	//STC12C52/5Aϵ��:M0=0,M1=0:׼˫��M0=0,M1=1:���裬M0=1,M1=0:���죬M0=1,M1=1:��©
	
	//�ر�ע��:�������������Χδ�����������裬�������óɸ������룬ֻ�����ó�׼˫�򣬷���ý��ڸߵ�ƽʱ��������״̬


	P4SW=P46EN|P45EN|P44EN;	//P4�����ó�IO��


	memset(&(stLocalControl.stBusDealFreq.bySndSecAddr),0x00,sizeof(STLocalControl));		//������������0

	//IO��ʼ��
	SSD1963_CS	= 1;
	SST25VF_CS = 1;
	GT23L_CS = 1;
	ADS7843_CS = 1;
	EPH1660_CS = 0;

//	UsartInitConfig(57600);
//	Send_Data_Byte(0x33);
	Delayms(200);

	//��ȡϵͳ������
	InitParameter();

  	//���ڳ�ʼ��
  	UsartInit();
	Send_Data_Byte(0x88);	


	//��λEPH1660
//	ResetEPH1660();

	//����MCUΪSPI����
	MCUMasterSPI();
	

//	RD_ID();

	//�洢����ʼ��

    manID=Read_ID(0x00);
	devID=Read_ID(0X01);

	Flash_Lock();

	//���Դ洢��
//	FRAM_TEST();


	//�����߳�ʼ�� 	
	SingleBusInit();
	
	//RGB����оƬ��ʼ��
	InitSSD1963();
	page=0;
	ShowPowerUpFace();


#if CONFIG_CC1101==1	
	CC1101Init();
#endif
		
	//�����ϵ�ǼǱ�־,����˸ʱ���ʼ��,��ʱ����(��ʱ���Զ������ϵ�Ǽ�����)	
	bLanding = 1;	
	MakeCH0TimerOut(50, 0);	


	stLocalControl.stSupplyOxTotalTime.uiHour=stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour;
	stLocalControl.stSupplyOxTotalTime.byMin=stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin;

	memset(&(stLocalControl.stTime.byYear),0x00,6);	//�嵱ǰʱ������

	stLocalControl.byDispNumSetOkTime=0;


#if CONFIG_CC1101==1
//�Ƶ����,����cc1101��ʼ��ʱ�����ж���������ɶ��ռ����
	TF1=0;
	CCAPM1 = (PCA1_CAPN|PCA1_ECCF);		//PCA1�����ж�
	AUXR1 |= PCA_P4;	     	//ӳ�䵽P4��	
	halSpiStrobe(CCxxx0_SRX);	//�������
#endif

	//��ͷ����洢����ʾ�ڴ���
	page=1;
	ShowBedFace();

	memset(BedDataBuff,0x00,SEGMENT_DATA_LEN);
	memset(byUsart0SndBuf,0x00,UART0_TX_BUF_SIZE);

	stLocalControl.uiCC1101ReReadTime = CC1101_REREAD_TIME;

	//��λ���Ź�
	WDT_CONTR = 0x3e; 	

    Set_VisualPage(1);	
	stLocalControl.byDisplayFace=BED_INFO_FACE;

	fcolor=BLUE;
	page=0;
	Lcd_Clear();		//����0ҳ���������ں�����Ϣ��ʾ)		
}
/**********************************************************
*��������			:GetMessage	
*��������        	:��ȡ���߳���Ϣ�����е���Ϣ(����Ϣ���������)
*�������   		:Msg:�洢��Ϣָ��
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void GetMessage(uint8 data *Msg)
{
	OSQPend(Msg, byMainCmdQ, 0);
}
/**********************************************************
*��������			:DispatchMessage	
*��������        	:�ַ������ȡ�����߳���Ϣ
*�������   		:Msg:��Ϣ,��4λ����������,��4λ����������
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void DispatchMessage(uint8 Msg)
{
	switch(Msg&0xf0)
	{
		case BUS0_REC:										//����0�յ�����֡
			Bus0RecDeal();
			break;
		case BUS0_SND:	 									//����0���ݷ������
			Bus0SendDeal();
			break;			
		case TIMER_OUT:	 									//��ʱ����
			TimerOutDeal();
			break;	
		default:
			break;
	}	
}
/**********************************************************
*��������			:MainTask	
*��������        	:ϵͳ���߳�,��������ϵͳ����Ϣ����
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void MainTask(void)
{
	static uint8 data Msg;  
	uint8 i;
	
	//ϵͳӲ����ʼ��
	Init();	
	//�������߳���Ϣ����	
	OSQCreate(byMainCmdQ, 16);
	//����Ϊ���������߳�	
	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(TimerOutManager, NULL, 2);	
	//������Ϣѭ��	

//	RD_ID();

	while(TRUE)
	{
		//��λ���Ź�
		WDT_CONTR = 0x3e; 
		GetMessage(&Msg);
		DispatchMessage(Msg);
		if(bUsart0RecFinish)									//����0�յ�һ֡����
		{ 	
			bUsart0RecFinish=0;
			Usart0RecDeal();	
		}

#if CONFIG_CC1101==1
		if(bWireLessRec==1)
		{
			//Send_Data(byWireLessRecBuff,byWireLessRecLen);
			bWireLessRec=0;
			byWireLessRecLen=8;	//Ҫ��ȡ���ݵ���󳤶�
			if(halRfReceivePacket(byWireLessRecBuff,&byWireLessRecLen)==CRC_OK)
			{
				if(byWireLessRecLen==8)
				{//��Һ�������������ź�
					if(byWireLessRecBuff[7]==CMD_NUMBER_SET)	
					{//�Ƕ�������
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(byWireLessRecBuff[1]),3);
						stLocalControl.stBusDealFreq.byCmd = CMD_RF_NUMSET_START;
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(byWireLessRecBuff[4]),3);
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					else 
					{
						if(byWireLessRecBuff[7]==CMD_INFUSION_CALL)
						{
							if(DirRFSerialCompare())	//����Ӧ����Һ������
							{
								memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
								stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CALL;
								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							}						
						}
					
					}
				}
				else if(byWireLessRecLen==6)
				{//�ƶ��ֻ��������ź�
					if(byWireLessRecBuff[0]==0x05)
					{//��ʼλ��ȷ
						if(byWireLessRecBuff[5]==(uint8)(byWireLessRecBuff[0]+byWireLessRecBuff[1]+byWireLessRecBuff[2]+byWireLessRecBuff[3]+byWireLessRecBuff[4]))
						{//У������
							if((byWireLessRecBuff[1]== stLocalControl.stEepromCfgData.bySelfSecAddr) && (byWireLessRecBuff[2]== 251))
							{//���������Ϊ�ƶ��ֻ�
								if(byWireLessRecBuff[4] ==0x01)
								{//�����ǻ�ʿ��λ����
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
									stLocalControl.stBusDealFreq.bySndBedAddr = byWireLessRecBuff[3];	//�ƶ��ֻ���
									stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_IN;
									memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState &PATIENT_DISCHARGE)) //�����������
									{
										BL_ON();//����ʾ��
									}								
									
								}
								else if(byWireLessRecBuff[4] ==0x02)
								{//�����ǲ鷿����
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
									stLocalControl.stBusDealFreq.bySndBedAddr = byWireLessRecBuff[3];	//�ƶ��ֻ���
									stLocalControl.stBusDealFreq.byCmd = CMD_INSPECTOR_CALL;
									memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState &PATIENT_DISCHARGE)) //�����������
									{
										BL_ON();//����ʾ��
									}								
									
								}
							}
						}
					}
				}								
			}
//			Send_Data(byWireLessRecBuff,byWireLessRecLen);
			halSpiStrobe(CCxxx0_SRX);	//�������
			//CCF1=0;     //������жϣ������ڴ�ʱ��������жϲ��������ٴδ����ж�
			CCAPM1 |= PCA1_ECCF;	//�����ж�
		
		}
#endif
		
		if(bDispBedFace==1)
		{
			bDispBedFace =0;
			if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)
			{				
				page =1;
				ShowItem(BED_NUMBER);	//��ʾ����
				ShowPatientBaseInfo();		//��ʾ���˻�����Ϣ
				ShowItem(OX_START);
				ShowItem(OX_END);
				ShowItem(OX_SUBTOTAL);	
				ShowItem(OX_TOTAL);
				Set_VisualPage(1);
				stLocalControl.byDisplayFace = BED_INFO_FACE;


				for(i=0;i<5;i++)
				{
					SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);	//��������һ������				
					if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)==1)  break;
						
				}
				
			}
			else
			{
				stLocalControl.uiBedFaceDataLen =0;			
				Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&(stLocalControl.uiBedFaceDataLen),2);	//�����ݳ�����0
				Write_Cont(BED_FACE_DATA_BAK,&(stLocalControl.uiBedFaceDataLen),2);
				
			}
		}

		if(bDispBedForm ==1)
		{
			bDispBedForm =0;
			if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2))	//�����ݳ���Ҳ������ȥ
			{
				ShowBedFaceForm();	
				ShowItem(BED_NUMBER);
				Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_LENGTH_ADDR,2);	//����ͷ�������ܸ���
				if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)) //�����ݳ���Ҳ������ȥ
				{
					ShowPatientBaseInfo();	

					ShowItem(OX_START);
					ShowItem(OX_END);
					ShowItem(OX_SUBTOTAL);	
					ShowItem(OX_TOTAL);					
					if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)!=1)
					{//�����������������ȷ
						for(i=0;i<5;i++)
						{
							SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);	//��������һ������				
							if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)==1)	break;	
						}		
					}
				}
				else
				{//������������ȷ
					if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2))
					{//��������ȷ
						while(1)
						{
							SectorDataCopy(BED_FACE_DATA_LENGTH_ADDR,BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+3);	//��������������				
							if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)  break; 				
						}
						ShowPatientBaseInfo();
						ShowItem(OX_START);
						ShowItem(OX_END);
						ShowItem(OX_SUBTOTAL);	
						ShowItem(OX_TOTAL);						
					}
					else 	//������������ȷ������������Ҳ����ȷ��
					{
					}
				}
		
				Set_VisualPage(1);

				for(i=0;i<5;i++)
				{
					SectorDataCopy(BED_FACE_FORM_BAK,BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+3);	//��������һ������				
					if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2)==1)  break;
						
				}				
			}
			else
			{
				stLocalControl.uiBedFaceFormLen =0;			
				Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);	//�����ݳ�����0
				Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);	//�����ݳ�����0
			}
				
		}
	}
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
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void main(void)
{  	
	//RD_ID();
	P4SW=0X70;
	EPH1660_CS=0;	//��ֹ
	Delayms(200);
	//����ϵͳ��ʼ��	
	OSInit();

	led0=1;
	//����ϵͳ���߳�
	OSTaskCreate(MainTask, NULL, 0); 
	
	while(1)
    {
		_nop_();
 //      PCON = PCON | 0x01;                    //CPU��������״̬	    
    }	 
}
