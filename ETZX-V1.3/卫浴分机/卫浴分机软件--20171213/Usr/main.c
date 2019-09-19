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
#include "absacc.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//���߳���Ϣ����
STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��



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
	//����PCAģ��0Ϊ16λ�����ʱ����ECCF0=1����PCAģ��0�ж�
	CCAPM0 = 0x49;	
	//��PCA�жϺ�LVD(��ѹ���)�жϹ�������жϿ���λ
	EPCA_LVD = 1;
	//����PCA����������
	CR = 1;	
	//��λ���Ź�
	WDT_CONTR = 0x3d; 
	//P1M1 |= 0xC0;
	//P1M0 &= 0x3F;
	P1M1 |= 0xC8;
	P1M0 &= 0x37;	
	//��ȡϵͳ������
	//ReadParameter(); 
	//--------------------------------------------------
	//--------------------------------------------------
	//�̵������ƶ˸�λ
	RST_BUS=0;
	//--------------------------------------------------
	InitParameter();
	
	//�����߳�ʼ�� 
	SingleBusInit();
	//������ճ�ʼ�� 
	IRInit();
	//״̬��ʼ��
	bLanding = 0;
	bCalling = 0;
	//���õƵ�״̬����˸ʱ��,�����ϵ糬ʱ	
	stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;	  
	if(stLocalControl.stEepromCfgData.byFlag&0x01)
    {	//�趨Ϊ�Ƴ���
		SetLedRedState(LED_ON);	
	}
	else
	{	//�趨Ϊ����
		SetLedRedState(LED_OFF);	
	}				
	MakeCH0TimerOut(0, 0); 
	BusLowDTime=100;

	Send_Data_Byte(stLocalControl.stEepromCfgData.byVersionHi);
	Send_Data_Byte(stLocalControl.stEepromCfgData.byVersionLo);


/*	Send_Data((uint8 *)(&stLocalControl.stEepromCfgData),sizeof(STEepromCfgData));
	
 	ReadParameter(IAP0_ADDR);
	Send_Data((uint8 *)(&stLocalControl.stEepromCfgData),sizeof(STEepromCfgData));

	ReadParameter(IAP1_ADDR);
	Send_Data((uint8 *)(&stLocalControl.stEepromCfgData),sizeof(STEepromCfgData));

 	ReadParameter(IAP2_ADDR);
	Send_Data((uint8 *)(&stLocalControl.stEepromCfgData),sizeof(STEepromCfgData));*/	
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
			KeyDownDeal(Msg&0x0f);
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
	
	//ϵͳӲ����ʼ��
	Init();
	//�������߳���Ϣ����
	OSQCreate(byMainCmdQ, 16);

	WDT_CONTR = 0x3d;	
	//����Ϊ���������߳� 
	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(KeyManager, NULL, 2);
	OSTaskCreate(TimerOutManager, NULL, 3);	
	//������Ϣѭ��
	WDT_CONTR = 0x3d;	
	while(TRUE)
	{
		GetMessage(&Msg);
		DispatchMessage(Msg);
		WDT_CONTR = 0x3d;
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
       PCON = PCON | 0x01;                    //CPU��������״̬   
    }	 
}
