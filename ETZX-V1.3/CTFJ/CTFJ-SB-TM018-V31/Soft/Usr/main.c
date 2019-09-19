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
STLocalControl 	xdata stLocalControl;//ȫ�ֱ����ṹ��



uint8  xdata MicCtrlState;

uint8 code *DispData="  ";

bit bBL_SW_State=1;


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

#if STC12C5A==1
	uiIsrTimerCount+=ISR_INC_COUNT;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	CCF0=0;
#endif


	if(stUsartCfg[0].byRecTimeout!=0) stUsartCfg[0].byRecTimeout--;
}

void BL_SW_Init()
{
	//BL_SW�������
	P5M0 |= 	Bin(0,0,0,0,0,0,1,0);
	P5M1 &=   (~Bin(0,0,0,0,0,0,1,0));
	BL_SW =1;
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
	memset(&(stLocalControl.byLedState),0x00,sizeof(STLocalControl));
	
	CMOD = 0x02;
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//����PCAģ��0Ϊ16λ�����ʱ��,ECCF0=1����PCAģ��0�ж�
	CCAPM0 = 0x49;
 	 
	//����PCA����������
	CR = 1;	
//	//��λ���Ź�
	WDT_CONTR = 0x3d; 
	//IO������
	//STC12C52/5Aϵ��:M0=0,M1=0:׼˫��M0=0,M1=1:���裬M0=1,M1=0:���죬M0=1,M1=1:��©
	//�ر�ע��:�������������Χδ�����������裬�������óɸ������룬ֻ�����ó�׼˫�򣬷���ý��ڸߵ�ƽʱ��������״̬
	
	//P0:����SSD1963������,׼˫��
	P0M0 = 0x00;
	P0M1 = 0x00;

	//P1��:P1^0(RS)����P1^1(cs)����P1^2δ��1^3(spi_cs2)����P1^4(SPI_CS1)����P1^5(MOSI)����P1^6(MISO)׼˫��P1^7(SCLK)����
	//P1M0:  1  0  1  1  1  0  1  1
	//P1M1:  0  0  0  0  0  0  0  0
	P1M0 = 0XBB;
	P1M1 = 0X00;

	//P2��::P2^0(SW1)׼˫��P2^1(SW2)׼˫��P2^2(SW3)׼˫��P2^3(SW4)׼˫��P2^4(SW5)׼˫��P2^5:δ��P2^6:δ��P2^7δ��
	//P2M0 = 0  0  0  0  0  0  0  0
	//P2M1 = 0  0  0  0  0  0  0  0
	P2M0 = 0X00;
	P2M1 = 0X00;

/*	//P3��:P3^0(RXD)׼˫��P3^1(TXD)����P3^2δ��P3^3(RECD):׼˫��P3^4(SND)����P3^5δ��P3^6(WR)����P3^7(RD)���� 
	//P3M0 =  1  1  0  1  0  0  1  0
	//P3M1 =  0  0  0  0  0  0  0  0
	P3M0 = 0Xd2;
	P3M1 = 0X00;
*/
	//P3��:P3^0(RXD)׼˫��P3^1(TXD)δ��P3^2����P3^3(RECD):׼˫��P3^4(SND)����P3^5δ��P3^6(WR)����P3^7(RD)���� 
	//P3M0 =  1  1  0  1  0  1  0  0
	//P3M1 =  0  0  0  0  0  0  0  0
	P3M0 = 0Xd4;
	P3M1 = 0X00;

	
	//P4��:P4^0δ��P4^1δ��P4^2δ��P4^3δ��P4^4(RESET)����P4^5δ��P4^δ��P4^7δ�� 
	//P4M0 =  0  0  0  1  0  0  0  0
	//P4M1 =  0  0  0  0  0  0  0  0
	P4M0 = 0X10;
	P4M1 = 0X00;
	P4SW=P46EN|P45EN|P44EN;

	//P5��:P5^0(LED)����P5^1δ��P5^2δ��P5^3δ��
	//P5M0 = 0000  0  0  0  1
	//P5M1 = 0000  0  0  0  0
	P5M0 = 0X01;
	P5M1 = 0X00;

	
	//IO��ʼ��
	LCD_CS7	= 1;
	SST25VF_CS = 1;
	GT23L_CS = 1;

	//�ر�MIC
	MicCtrlState=MIC_CTRL=0;

	Delayms(1000);


	//��ȡϵͳ������
	InitParameter();

	//�����߳�ʼ�� 	
	SingleBusInit();

  	//���ڳ�ʼ��
  	UsartInit();
	Send_Data_Byte(0x33);


	SpiLcdMode();
    ST7735_Init();	//Һ������ʼ��
//	������Ƴ�ʼ��
	BL_SW_Init();

 	//�洢����ʼ��
 	SST25VF_Init();

//	FRAM_TEST();





	//�����ϵ�ָʾ��״̬ 	
	SetLedState(LED_OFF);	

	//���ó�ʼʱ��
	stLocalControl.stTime.byYear=0x11;
	stLocalControl.stTime.byMonth=0x01;
	stLocalControl.stTime.byDay=0x28;
	stLocalControl.stTime.byHour=0x16;
	stLocalControl.stTime.byMinute=0x28;
	
	//�����ϵ�ǼǱ�־,��ʱ����(��ʱ���Զ������ϵ�Ǽ�����)	
	bLanding = 1;	
	MakeCH0TimerOut(50, 0);	

	//,����˸ʱ���ʼ��
	stLocalControl.byLedTimeSet	= 25;
	stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;	

	stLocalControl.bySecondFlashTime=25;



	Read_Cont((uint8 *)(&(stLocalControl.uiDietDataLen)),DIET_LENGTH_ADDR,2);
	Read_Cont((uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),PRESCRIPTION_LENGTH_ADDR,2);
	Read_Cont((uint8 *)(&(stLocalControl.uiPromptDataLen)),PROMPT_LENGTH_ADDR,2);

	//�ϵ�ʱʹ����
	ShowPowerUpFace();
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
		case KEY_DOWN: 										//�����´���   				
			KeyDownDeal(Msg);
			break;
		case KEY_ALWAYS:									//�������´���
			KeyAlwaysDeal(Msg);
			break;
		case KEY_UP:  										//��������
			KeyUpDeal(Msg);
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
	OSTaskCreate(KeyManager, NULL, 2);
	OSTaskCreate(DispManager, NULL, 3);	
	OSTaskCreate(TimerOutManager, NULL, 4);

	//������Ϣѭ��	

	while(TRUE)
	{
		WDT_CONTR = 0x3d;	//ι��
		GetMessage(&Msg);
		DispatchMessage(Msg);
		if(bUsart0RecFinish==1)
		{
			bUsart0RecFinish=0;
			Usart0RecDeal();
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