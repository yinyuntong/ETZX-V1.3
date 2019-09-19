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
#define _IN_MAIN_
#include "config.h"

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//���߳���Ϣ����
STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��


//��̬�����ڴ��С����
#define MALLOC_MEM_SIZE		100
//uint8 			xdata 			byMallocMem[MALLOC_MEM_SIZE];

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
//	CMOD	&=	!ECF;			//��ֹ PCA����������ж�   
//	CCAPM0	&=	!PCA0_ECCF;		//��ֹCCF0�ж�
//	CCAPM1	&=	!PCA1_ECCF;		//��ֹCCF1�ж�	
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
	CCAP1L = (uint8)uiIsrTimerCount;
	CCAP1H = (uint8)(uiIsrTimerCount>>8);
	CCF1=0;
#endif

	if(stUsartCfg.byRecTimeout!=0) stUsartCfg.byRecTimeout--;
}


void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
#if DEBUG==1
    uint8 xdata i;
	WDT_CONTR = 0x3d; 

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
}


void Send_Data_Byte(uint8 SendData)
{ 
#if DEBUG==1
    ES=0;
    SBUF=SendData;
    while(!TI);
	TI=0;
	ES=1;
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
		WDT_CONTR = 0x3d; 
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

	CCAPM0 = 0x11;	//�½����ж�,ʹ��
	
	
	CCAP1L = (uint8)uiIsrTimerCount;
	CCAP1H = (uint8)(uiIsrTimerCount>>8);
	//����PCAģ��1Ϊ16λ�����ʱ��,ECCF1=1����PCAģ��1�ж�
	CCAPM1 = 0x49;
 	 
	//����PCA����������
	CR = 1;	

	
	//��λ���Ź�
	WDT_CONTR = 0x3d; 
	//IO������
	//STC12C52/5Aϵ��:M0=0,M1=0:׼˫��M0=0,M1=1:���裬M0=1,M1=0:���죬M0=1,M1=1:��©
	
	//�ر�ע��:�������������Χδ�����������裬�������óɸ������룬ֻ�����ó�׼˫�򣬷���ý��ڸߵ�ƽʱ��������״̬

	P4SW=P46EN|P45EN|P44EN;	//P4�����ó�IO��

	
	//IO��ʼ��
	SSD1963_CS	= 1;
	SST25VF_CS = 1;
	GT23L_CS = 1;
	ADS7843_CS = 1;
	EPH1660_CS = 0;




	//ȫ�ֱ�����ʼ��Ϊ0
	memset(&(stLocalControl.byChar1),0x00,sizeof(STLocalControl));
	
	Delayms(200);

	
	//��ȡϵͳ������
	InitParameter();

  	//���ڳ�ʼ��
  	UsartInit();


	//�����߳�ʼ�� 	
	SingleBusInit();


	//��λEPH1660
//	ResetEPH1660();


	//����MCUΪSPI����
	MCUMasterSPI();


	//�洢����ʼ��
	SST25VF_Init();

//	FRAM_TEST();

	//RGB����оƬ��ʼ��
	InitSSD1963();	

		
	//�����ϵ�ǼǱ�־,����˸ʱ���ʼ��,��ʱ����(��ʱ���Զ������ϵ�Ǽ�����)	
	bLanding = 1;	
	MakeCH0TimerOut(50, 0);	

	ShowPowerUpFace(DIS_FRAM_MEM1);	//���ڵ�1ҳ
	
	stLocalControl.stTime.byYear=0x08;
	stLocalControl.stTime.byMonth=0x08;
	stLocalControl.stTime.byDay=0x08;
	stLocalControl.stTime.byHour=0x08;
	stLocalControl.stTime.byMin=0x08;
	stLocalControl.byDisplayFace=0;

	stLocalControl.bySecondFlashTime=25;
	stLocalControl.byDispNumSetOkTime=0;

	Read_Cont(WARD_FACE_DATA_START_ADDR,WARD_FACE_DATA_LEN,WardDataBuff);
	StorSecInfo(DIS_FRAM_MEM0);
//	OSMemInit(byMallocMem,MALLOC_MEM_SIZE);
	
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
//			bBus0RecFinish = 0;
//			Bus0RecData(byBus0RecData);
//			Bus0RecDeal();
			break;
		case BUS0_SND:	 									//����0���ݷ������
			Bus0SendDeal();
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
	OSTaskCreate(TimerOutManager, NULL, 2);
//	OSTaskCreate(EPH1660Manager, NULL, 3);
	OSTaskCreate(DispManager, NULL, 3);	
	//������Ϣѭ��	

	while(TRUE)
	{
		WDT_CONTR = 0x3d; 
		GetMessage(&Msg);
		DispatchMessage(Msg);
		if(bUsart0RecFinish)									//����0�յ�һ֡����
		{ 	
			bUsart0RecFinish=0;
			Usart0RecDeal();	
		}	
		
/*		if(byBus0FreqNum>0)
		{
			Bus0RecDeal();
		}
*/		


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
