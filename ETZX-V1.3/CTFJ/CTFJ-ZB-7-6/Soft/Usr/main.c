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
#include "config.h"
#include "absacc.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//���߳���Ϣ����
STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��

extern uint8 data byBus0RecData[];
uint8 xdata UartRecData;
uint8 xdata IrDataPosit=0;
uint8 xdata RFSerial[6];		//������Һ��������������е����к�

uint8 code  	run[12]={31,29,31,30,31,30,31,31,30,31,30,31};   
uint8 code   notrun[12]={31,28,31,30,31,30,31,31,30,31,30,31};

uint8 code byVersionHi=1;
uint8 code byVersionLo=2;


uint8 xdata BusLowDTime=0;
uint8 xdata BusHighDTime=0;


bit bFourByteRec=0;					//���ڽ��յ�4�ֽ�����
bit bNumSeting=0;
bit bRFNumSeting=0;
bit bOxTimerKeyState=1;
bit bEnableOxTimer=0;				//���û�յ�ʱ�����ݲ������ʱ
bit bWillSndOxSupplyStart=0;		//�ϵ紦�ڹ���״̬
bit bWillSndOxSupplyEnd=0;			//�����͹�����������
bit bWillSndTotalOx;						//�����͹�����ʱ��Ϣ
bit bCloseCGB=0;					//�㲥ʱ����״̬��־


bit bBus1Answer =0;					//���յ�����1��Ӧ���־
bit bOxSupplyState=0;				//�����ڹ�����ʱ״̬
bit bSixByteRec=0;					//���ڽ��յ�6�ֽ�����


void TIMER1Int(void) interrupt T1_INTNO		//3���ж�
{
	ET1=0;
}

/*---------------------------------------------------------------------------
����ԭ��: void Delayms(uint ms)
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
  if(a.byMin<b.byMin)   
  {   
  	sum.byMin=a.byMin+60-b.byMin;
	sum.uiHour=sum.uiHour-1;   
  }
  else  
  {   
	sum.byMin=a.byMin-b.byMin;   
  }   
  return   sum;   
 } 

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

}



void UsartInit(void)
{

	RXD=1;
	TXD=1;
	SCON = 0X50;		//������ʽ1

#if 0
 //   TMOD= 0x21;   //  ;t1�������ʷ�������t0����ʽ1��16λ)��ʱ��
	TMOD &= 0X0F;		//TO��ʽ����
	TMOD |= 0X20;		//T1 8λ�Զ�����
	TL1	=	0XF6;		//������4800  ��18.432M����������
	TH1	=	0XF6;
    PCON &=	(~SMOD);   //  ;�����ʱ���ѡ��		smod=0
  	AUXR |= T1x12;		//1Tģʽ,�������Ϊ:4800*12=57.6K
    TF1=0;
    TR1=1;
#endif


#if 0
	TMOD &= 0X0F;	   //TO��ʽ����
	TMOD |= 0X20;	   //T1 8λ�Զ�����
	TL1 =   0Xec;	   //������2400  ��18.432M����������
	TH1 =   TL1;
	PCON &= (~SMOD);   //  ;�����ʱ���ѡ��	   smod=0
	//	   AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:4800*12=57.6K
	TF1=0;
	TR1=1;
#endif

	TMOD &= 0X0F;	   //TO��ʽ����
	TMOD |= 0X20;	   //T1 8λ�Զ�����
	TL1 =   0Xd8;	   //������1200  ��18.432M����������
	TH1 =   TL1;
	PCON &= (~SMOD);   //  ;�����ʱ���ѡ��	   smod=0
	//	   AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:4800*12=57.6K
	TF1=0;
	TR1=1;


	
	RI=0;
	TI=0;
	REN=1;
	ES=1;	
}



void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
	uint8 xdata i;
    ES=0;
	for(i=0;i<l;i++)
	{
		SBUF=*Databuf;
		while(!TI);
		TI=0;
		Databuf++;
	}
	ES=1;
 }

void Send_Data_Byte(uint8 SendData)
{
 	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
}


/*void RD_ID(void)
{
	uint8 idata *p;
	p=0xf1;
	Send_Data(p,7);
} */


void UartInt(void) interrupt UART_INTNO
{
	
	if(TI==1)
		TI=0;
	if(RI==1)
	{
		RI=0;
		UartRecData = ~SBUF;	//��λȡ��
		stLocalControl.byIrDataIntervalTimer=5;		//ÿ�ֽڼ��������100ms
		if(IrDataPosit==0)
		{	
			if(UartRecData==0x05)	//��ʼ��
				byIrDARecBuff[IrDataPosit++]=	UartRecData;
		}
		
		else 
		{
			byIrDARecBuff[IrDataPosit++]=	UartRecData;
			if(IrDataPosit >=7)   IrDataPosit =0;
/*			if(IrDataPosit==4)  
			{
				if(byIrDARecBuff[3]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]))
				{
					bFourByteRec=1;
					IrDataPosit=0;
				}
			}

			else if(IrDataPosit==6)
			{
				if(byIrDARecBuff[5]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]+byIrDARecBuff[3]+byIrDARecBuff[4]))
				{

					bSixByteRec =1;
				}
				IrDataPosit=0;	
			}*/
		}	
	}
}

void OXTimerKeyInit(void)
{

/*
//OX_TIMER_KEY׼˫���
//	P3M0 &= 0xDF;
//	P3M1 &= 0xDF;	

	//OX_TIMER_KEY��������
	P3M0 |= 0x20;
	P3M1 &= 0xDF;	

	OX_TIMER_KEY=1;
	bOxTimerKeyState=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
	if(OX_TIMER_KEY==0)
	{
		OSWait(K_TMO,4);
		if(OX_TIMER_KEY==0)  
		{
			bOxTimerKeyState=0;
			bWillSndOxSupplyStart=1;		//�ϵ�ʱ�ʹ��ڹ���״̬
		}
		else 
		{
			bOxTimerKeyState=1;

		}
	}
	*/

	if(stLocalControl.stEepromCfgData.byBedFjFlag & OX_SUPPLY_STATE)	///�ϵ�ʱ�ʹ��ڹ���״̬
	{
		bWillSndOxSupplyStart=1;
	}

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

	CR = 1;	
	EPCA_LVD =1;
	//��λ���Ź�                                                                                                                                                                                                                                                                                     
	//WDT_CONTR = 0x3a; 	//@20MHz,157.3ms
	WDT_CONTR = 0x3d;		//@20MHz,1.25s	



	//������ʼ��
	byDevState1 = 0;
	byDevState2 = 0;
	byDevState3 = 0;

	//�ڴ����
	memset(&(stLocalControl.byLedState),0x00,sizeof(STLocalControl));

//STC12C56ϵ��::M0=0,M1=0:׼˫��M0=0,M1=1:���죬M0=1,M1=0:���裬M0=1,M1=1:��©

//�ر�ע��:�������������Χδ�����������裬�������óɸ������룬ֻ�����ó�׼˫�򣬷���ý��ڸߵ�ƽʱ��������״̬
	
//IOC��©���,�ø�:��,�õ�:�ر�
//,LED���������趨Ϊ�������,RS485������Ϊ׼˫���


	//IO��ʼ��
	MUS = 0;	//��ʾ���˿���0

	//MUT�������
	P1M0  &= (~Bin(0,1,0,0,0,0,0,0));
	P1M1  |=   Bin(0,1,0,0,0,0,0,0);
	bMUTState= MUT = 1;		//�ر�MC34119

	P1M0 &= (~Bin(0,0,0,0,1,0,0,0));
	P1M1 |=   Bin(0,0,0,0,1,0,0,0);
	XTA = 0;	//�ر�MIC��SA������ͨ��


	P1M0 &= (~Bin(0,0,0,1,0,0,0,0));
	P1M1 |=   Bin(0,0,0,1,0,0,0,0);
	XTD = 0;	//�ر�MIC��SD������ͨ��	

	KDR = 0;	//����485����

	//MUS�������
	P1M0  &= (~Bin(0,0,0,0,0,0,1,0));
	P1M1  |=   Bin(0,0,0,0,0,0,1,0);

	MUS = 0;	//ʹSD��·��ͨ

	CloseCBD();
	CloseCGB();

	bNumSeting=0;
	bRFNumSeting=0;
	bEnableOxTimer=0;				//���û�յ�ʱ�����ݲ������ʱ
	bWillSndOxSupplyStart=0;					//�ϵ紦�ڹ���״̬
	bWillSndTotalOx=0;						//�����͹�����ʱ�ܼ���Ϣ
	bCloseCGB=0;					//�㲥ʱ����״̬��־

	bBus1Answer =0;					//���յ�����1��Ӧ���־
	bOxSupplyState=0;				//�����ڹ�����ʱ״̬	


  	//���ڳ�ʼ��
  	UsartInit();


	Delayms(200);
	//��ȡϵͳ������
	InitParameter();



   //	RD_ID();	//56ϵͳû��ID
	
	//�����߳�ʼ�� 	
	SingleBusInit();

	//�����ϵ�ָʾ��״̬ 	
	SetLedDealState(LED_ON);
	
	stLocalControl.byLedTimeSet	= 25;
	stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;

	//��ʼ��������������ֽڼ��ʱ��
	stLocalControl.byIrDataIntervalTimer=0;	

	//��ʼ���Ƽ�״̬��־
	bLedDealState = LED_DEAL =0;

	OXTimerKeyInit();

	
	memset(&(stLocalControl.stTime.byYear),0x00,6);	//�嵱ǰʱ������	

	stLocalControl.uiNurseInTime=0;

	stLocalControl.byNumSetTime=0;

	stLocalControl.uiRFNumSetTime=0;
	

//	Send_Data(&(stLocalControl.stEepromCfgData.byRFSerialNum1),6);
	stLocalControl.uiBus1TestTime = BUS1_TEST_TIME;	//�뵥����1�ϵ�ģ��ÿ��1����1��

	//�����ϵ�ǼǱ�־,����˸ʱ���ʼ��,��ʱ����(��ʱ���Զ������ϵ�Ǽ�����)	
//	bLanding = 1;
	bLanding=0;
	
//	MakeCH0TimerOut(50, 0);


	BusLowDTime=100;
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
		case BUS1_REC:										//����1�յ�����֡
			Bus1RecDeal();
			break;
		case BUS1_SND:	 									//����1���ݷ������
			Bus1SendDeal();
			break;		
		case TIMER_OUT:	 									//��ʱ����
			TimerOutDeal();
			break;
		case KEY_DOWN: 										//�����´���   				
			KeyDownDeal(Msg&0x0f);
			break;
		case KEY_ALWAYS:									//�������´���
			KeyAlwaysDeal(Msg&0x0f);
			break;
		case KEY_UP:  										//��������
			KeyUpDeal(Msg&0x0f);
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
	
	//ϵͳӲ����ʼ��
	Init();		
	//�������߳���Ϣ����	
	OSQCreate(byMainCmdQ, 16);
	//����Ϊ���������߳�	
/*	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(Bus1Manage, NULL, 2);
	OSTaskCreate(TimerOutManager, NULL, 3);
	OSTaskCreate(KeyManager, NULL, 4);
	OSTaskCreate(OXManager, NULL, 5);
*/
	OSTaskCreate(BusManage, NULL, 1);	
	OSTaskCreate(TimerOutManager, NULL, 2);
	OSTaskCreate(KeyManager, NULL, 3);
	//OSTaskCreate(OXManager, NULL, 4);

	//������Ϣѭ��		
	while(TRUE)
	{
		GetMessage(&Msg);
		WDT_CONTR = 0x3d;		//@20MHz,1.25s
		DispatchMessage(Msg);
		if(bFourByteRec==1)
		{
			bFourByteRec=0;
			IrDATreat();			
		}

		if(bSixByteRec ==1)
		{
			bSixByteRec =0;
			IrDANumberSet();
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
	//����ϵͳ��ʼ��	
	OSInit();
	//����ϵͳ���߳�
	OSTaskCreate(MainTask, NULL, 0); 


	while(1)
    {
		_nop_();
 //      PCON = PCON | 0x01;                    //CPU��������״̬
    }	 
}
