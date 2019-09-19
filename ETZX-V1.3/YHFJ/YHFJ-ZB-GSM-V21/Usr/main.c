/*
************************Copyright(c)************************
*	  			   ����һ�عɷ����޹�˾
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
#define _IN_MAIN_
#include "config.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//���߳���Ϣ����
STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��

void TIMER1Int(void) interrupt T1_INTNO		//3���ж�
{
	ET1=0;
}

void UsartInt(void) interrupt UART_INTNO	//4 //4���ж�
{	
	ES=0;
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


/***********************************************************/

void UsartInit(void)
{
	   SCON=0X50;	   //������ʽ1
	   TMOD&=0X0F;	   //TO��ʽ����
	   TMOD|=0X20;	   //;t1 ��ʽ2�������ʷ�����
	
	
	   TL1 =   0XF6;	   //������4800  ��18.432M����������
	   TH1 =   TL1;
	   PCON    &=  ~SMOD;	//	;�����ʱ���ѡ�� 	   smod=0
	// AUXR    &=  0xBf;	   //��ͳ12��Ƶ�ٶ�
	   AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:4800*12=57.6K
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=1;
	   ES=1;   


}
/***********************************************************/

void uart_send(unsigned char *SDATA,unsigned char length)
{
#if DEBUG==1
	WDT_CONTR = 0x3d;	//ι��

	EA=0;
	ES=0;
	CR=0;
	TI=0;
	for(;length>0;length--)
	{
		SBUF=*SDATA;
		while(!TI);
		TI=0;
		SDATA++;
	}
	CR=1;
	ES=1;
	EA=1;
#endif
}



void uart_send_byte(unsigned char SDATA)
{
#if DEBUG==1
	EA=0;
	ES=0;
	CR=0;
	TI=0;
	SBUF=SDATA;
	while(!TI);
	TI=0;
	CR=1;
	ES=1;
	EA=1;
#endif
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
   	{
		for(i=900;i!=0;i--);
		WDT_CONTR = 0x3d;	//ι��
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

	//������ʼ��
	byDevState1 = 0;
	byDevState2 = 0;
	byDevState3 = 0;
	byDevState4 = 0;
	

	//IO��ʼ��
	MUS = 0;
	CloseCGB();
	CloseCBD();



	P1M0 |=   Bin(0,0,0,1,0,0,0,0);
	P1M1 &= (~Bin(0,0,0,1,0,0,0,0));	//XTA���ó��������

	
	P1M0 |=   Bin(0,0,0,0,1,0,0,0);
	P1M1 &= (~Bin(0,0,0,0,1,0,0,0));	//XTD���ó��������	
	XTA = 0;
	XTD = 0;


//	KDR = 1;  	
	KDR =0;

	//MC_CNT���ó�ǿ�������
 	P1M0 |=   Bin(0,0,1,0,0,0,0,0); P1M1 &= (~Bin(0,0,1,0,0,0,0,0));
	
	P1M0 |=   Bin(1,0,0,0,0,0,0,0);  P1M1 |=  Bin(1,0,0,0,0,0,0,0);	//MC_CTRL���óɿ�©���

	//MCN��©���    ����
	P1M0 |= Bin(0,1,0,0,0,0,0,0);P1M1 |= Bin(0,1,0,0,0,0,0,0);MCN= 1;
	
	CloseHandleMIC();
	ClosePanelMIC();
	P2M0=0xff;
	P2M1=0xff;	//���ȵ������������óɿ�©���
	SetSPKVol0();	//���Ⱦ���


	//PWǿ�������
	P0M0 |=   Bin(0,0,0,0,1,0,0,0);
	P0M1 &= (~Bin(0,0,0,0,1,0,0,0));	
	bPWState=PW=1;		//��ֹ34018
	bPW2State=PW2=1;	//��ֹTDA7233
	bVl0State=VL0=0;	//�ر�34018������

		
	Delayms(1000);		//��ʱ1S
	
	//��ȡϵͳ������
	InitParameter();

  	//���ڳ�ʼ��
  	UsartInit();
	uart_send_byte(0X88);

	//�����߳�ʼ�� 
	SingleBusInit(); 
	
	//��ʼ�����е�ַ
	InitKeyCallAddr();
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
	memset(stLocalControl.byKeyDownState, 0x00, 4);	
	//״̬��ʼ��  	
	bLanding = 1;
	//��ʼ����Ͳ״̬Ϊ�һ�״̬
	bHandleDown = 1;	
	MakeCH0TimerOut(50, 0);	


//	��������ͨ��
/*	bPWState=PW=0;				//ʹ��34018
	OpenPanelMIC();
	XTD=1;
	OpenCGB();
	SetSPKVol4();
	bVl0State=1;*/

/*	bPW2State=PW2=0;				//ʹ��TDA7233
	OpenHandleMIC();
	XTD=1;
	OpenCGB();*/	

//	while(1);

	
	led0init();
	stLocalControl.byNumSetTime=0;
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
		case KEY_DOWN: 										//��Ͳ�һ�����   				
			KeyDown();
			break;		
		case KEY_UP:  										//��Ͳժ������
			KeyUp();
			break;			
		case TIMER_OUT:	 									//��ʱ����
			TimerOutDeal();
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
	OSTaskCreate(Bus0Manage, NULL, 1);
	OSTaskCreate(Bus1Manage, NULL, 2);
	OSTaskCreate(KeyManager, NULL, 3);
	OSTaskCreate(TimerOutManager, NULL, 4);		
	//������Ϣѭ��		
	while(TRUE)
	{
		WDT_CONTR = 0x3d;	//ι��
		GetMessage(&Msg);
		DispatchMessage(Msg);

		if(Bus0RxBuffLen() >= BUS0_FREQ_SIZE)
		{ 	//��һ֡�����������ڽ��ն����� 
			OS_ENTER_CRITICAL();
			stLocalControl.stBusDealFreq.bySndSecAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.bySndRoomAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.bySndBedAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byCmd= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byRecSecAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byRecRoomAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byRecBedAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			OS_EXIT_CRITICAL();	
			Bus0RecDeal();
		}
		else
		{	//û��һ֡�����������ڷ��Ͷ�������
			byBus0RxHead = byBus0RxTail = 0;
		}

		
		if(Bus1RxBuffLen() >= BUS1_FREQ_SIZE)
		{ 	//��һ֡�����������ڽ��ն����� 
			OS_ENTER_CRITICAL();
			stLocalControl.stBusDealFreq.bySndSecAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.bySndRoomAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.bySndBedAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byCmd= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byRecSecAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byRecRoomAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byRecBedAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			OS_EXIT_CRITICAL();	
			Bus1RecDeal();
		}
		else
		{	//û��һ֡�����������ڷ��Ͷ�������
			byBus1RxHead = byBus1RxTail = 0;
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
       PCON = PCON | 0x01;                    //CPU��������״̬	    
    }	 
}
