/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:main.c
*�ļ�����    		:������
*������     		:������
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

STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��
extern bit   NUMBER_SET_STOP_FLAG;
extern bit   SerchRoomFlag;
extern bit   SET_BY_SERIAL_OK_FLAG;
extern bit   BusLow_Flag;
extern bit   Self_BusLow_Flag;
extern bit   BusLowRenewOK_Flag;

extern bit bBus0RecFinish;
extern bit  bBus0SendFinish;

bit   CheckBusLow_Flag;
bit   Display_BusLow_Flag;
bit   bUsartRecFrame=0;
bit   bTimer0OutDeal=0;


bit   bDealKeyShortDown=0;
bit   bDealKeyAlwaysDown=0;
bit   bDealKeyUp=0;
//extern xdata Self_BusLow_Count;

bit  bLcdOn=0;

uint8 xdata msTimer=0;
uint8 xdata byDealKeyErrorDownTime=0;
uint8 xdata byDealKeyShortDownTime=0;
uint8 xdata byDealKeyAlwaysDownTime=0;


uint8 xdata byKeyValue;


#define OS_TIME_ISR     7                                       /* ϵͳ��ʱ��ʹ�õ��ж�                         */


#define UserTickTimer() 	{uiIsrTimerCount+=ISR_INC_COUNT;CCAP0L = (uint8)uiIsrTimerCount;CCAP0H = (uint8)(uiIsrTimerCount>>8);CCF0=0;/*WDT_CONTR = 0x3a;*/}   /* ϵͳ��ʱ�ж��е��õ��û���								 */

void OSTickISR(void) interrupt OS_TIME_ISR
{	

    UserTickTimer();                                    /* �û�����                                                 */

	 
	
	if(++msTimer==4)	//20ms��ʱ��
	{
		msTimer=0;
		bTimer0OutDeal=1;
	}
	
	if(byDealKeyErrorDownTime)	//��������״̬
	{
		byKeyValue=KeyScan();
		if(DEAL_KEY != byKeyValue)
		{//�����,�˳�����״̬
			byDealKeyErrorDownTime=0;
		}
		
		else if(--byDealKeyErrorDownTime==0)	//20ms����ʱ�䵽
		{
			bDealKeyShortDown=1;
			byDealKeyShortDownTime =200;			//����1Sʱ�����̰�

		}
	}

	else if(byDealKeyShortDownTime)	//���ڶ̰�״̬
	{
		byKeyValue=KeyScan();
		if(DEAL_KEY != byKeyValue)
		{//�����,�˳��̰�״̬
			byDealKeyShortDownTime=0;
			bDealKeyUp=1;
		}
		else if(--byDealKeyShortDownTime==0)	//1s�̰�ʱ�䵽
		{
			bDealKeyShortDown=0;
			bDealKeyAlwaysDown=1;
			byDealKeyAlwaysDownTime =1;			//Ϊһ����ֵ��ʾ���볤��״̬
		}		
	}

	else if(byDealKeyAlwaysDownTime)	//���ڳ���״̬
	{
		byKeyValue=KeyScan();
		if(DEAL_KEY != byKeyValue)
		{//����ȣ��˳�����״̬
			byDealKeyAlwaysDownTime=0;
			bDealKeyUp =1;
		}
	}
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
		WDT_CONTR =0x3d;
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
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Init(void)
{ 		
	memset(&(stLocalControl.byLedState),0x00,sizeof(STLocalControl));		//������������0
	NUMBER_SET_STOP_FLAG=0;
	SerchRoomFlag=0;
	SET_BY_SERIAL_OK_FLAG=0;
	BusLow_Flag=0;
	Self_BusLow_Flag=0;
	BusLowRenewOK_Flag=0;
	
	bBus0RecFinish=0;
	bBus0SendFinish=0;
	
	CheckBusLow_Flag=0;
	Display_BusLow_Flag=0;
	bUsartRecFrame=0;
	bTimer0OutDeal=0;
	
	
	bDealKeyShortDown=0;
	bDealKeyAlwaysDown=0;
	bDealKeyUp=0;
	//extern xdata Self_BusLow_Count;
	
	
	
	msTimer=0;
	byDealKeyErrorDownTime=0;
	byDealKeyShortDownTime=0;
	byDealKeyAlwaysDownTime=0;
	
	
	byKeyValue=0;


	
	CMOD = 0x02;
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;	
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//����PCAģ��0Ϊ16λ�����ʱ����ECCF0=1����PCAģ��0�ж�
	CCAPM0 = 0x49;	
	//��PCA�жϺ�LVD(��ѹ���)�жϹ�������жϿ���λ
	CR = 1;	

	//��λ���Ź�
	WDT_CONTR = 0x3d; 
	EA=1;


	P4SW=0x70;	
	//P4M1 &= 0xAE; //XTD��MDR��MUT����Ϊ�������
    //P4M0 |= 0x51;
	//--------------------------------------------------
	//--------------------------------------------------
#ifdef DL_SWAP
	P4M1 &= 0x3E; //XTD��MUT��RESETB����Ϊ������� MDR ��©
    P4M0 |= 0xD1;
#else
	P4M1 &= 0x2E; //XTD��MDR��MUT��RESETB����Ϊ�������
    P4M0 |= 0xD1;
#endif
	//--------------------------------------------------
	//--------------------------------------------------
#ifdef DL_SWAP
	P5M1 &= 0xFb; //XTA����Ϊ������� MDB��MDG ��©
    P5M0 |= 0x07;
#else
	P5M1 &= 0xF8; //XTA��MDB��MDG����Ϊ�������
    P5M0 |= 0x07;
#endif



	P3M0 |= 0x08;									   //LED_DEAL�����ó�ǿ�������ģʽ	
	P3M1 &= 0xF7;


	//--------------------------------------------------
	//--------------------------------------------------
	//�̵������ƶ˸�λ
	RST_BUS=0;
	//--------------------------------------------------
	//--------------------------------------------------
	SingleBusInit(); 
	SpiInit();	
	//UartInit();	
	SetLedDealState(LED_ON); 	
	SetMDLedState((LED_ON<<4)|(LED_ON<<2)|LED_ON); //��������ŵƵ����


	ParameterInit();  

	
	//-----------------------
	//-----------------------
	//2012/4/9��Ӳ����ʿɱ�
	UartInit();	 
	Send_Data_Byte(0x55);

	
	//-----------------------
	//-----------------------
	LcmInit();	

//	FRAM_TEST();
	
	PW = 1;
	VOL= 1;
	CGB = 0; 	
	CBD = 0;
	CTA = 0;
	CTD = 0;		
	//KDR = 1;  
	//--------------------------------------------------
	//--------------------------------------------------
	//RS485����һֱ���ڽ���״̬
	KDR = 0;
	//--------------------------------------------------
	//--------------------------------------------------		
	//������ʼ��
	byDevState1 = 0;
	byDevState2 = 0;
	byDevState3 = 0;
	byDevState4 = 0;
	bPWState=1;
	//�����ϵ�ָʾ��״̬ 	
	//SetLedDealState(LED_ON);
	//SetMDLedState((LED_ON<<4)|(LED_ON<<2)|LED_ON);
	//�����ϵ�ǼǱ�־,����˸ʱ���ʼ��,��ʱ����(��ʱ���Զ������ϵ�Ǽ�����)
	stLocalControl.byLedFlashDelay = 25;	
	stLocalControl.byLedFlashTime = 50;
	bLanding = 0;
	byDevState1 = stLocalControl.stEepromCfgData.bySelfSecAddr & 0x80;	
	MakeCH0TimerOut(0, 0);
}



/**********************************************************
*��������			:main	
*��������        	:ϵͳ������,������������
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Main(void)
{  	
	unsigned short iii=0;
	//ϵͳӲ����ʼ��
	Init();		 
	while(1)
    {

		
	    //��λ���Ź�
		WDT_CONTR = 0x3d;
	    if(SerchRoomFlag)
		{
		    SerchRoomFlag=0;
			SetLcdState(1);
		    ShowPage();			
		}
		if(NUMBER_SET_STOP_FLAG)
		{								
		   NUMBER_SET_STOP_FLAG=0;
		   SetLcdState(1);
		   ShowPage();
		}
		else if(SET_BY_SERIAL_OK_FLAG)
		{
		   SET_BY_SERIAL_OK_FLAG=0;
		   SetLcdState(1);
		   ReadPageColour();
		   ShowPage();
		}


	   if(bBus0RecFinish)
	   	{
	   		Bus0RecDeal();
	   	}
	   if(bBus0SendFinish)									//����0����������֡
	   	{
	   		Bus0SendDeal();
	   	}

	   if(bUsartRecFrame)
	   	{
	   		bUsartRecFrame=0;
	   		UartDeal();
	   	}
	   if(bTimer0OutDeal)
	   	{
	   		bTimer0OutDeal=0;
			TimerOutDeal();
	   	}


		if(bDealKeyShortDown)
		{
			bDealKeyShortDown=0;
			KeyDownDeal(DEAL_KEY);
		}

		if(bDealKeyAlwaysDown)
		{
			bDealKeyAlwaysDown=0;
			KeyAlwaysDeal(DEAL_KEY);
		}

		if(bDealKeyUp)
		{
			bDealKeyUp=0;
			KeyUpDeal(DEAL_KEY);
		}

		if((byDealKeyErrorDownTime==0)&&(byDealKeyShortDownTime==0)&&(byDealKeyAlwaysDownTime==0))
		{
			KeyScan();
		}
	   	//���ڱ��������п��ܳ����󱣻������
    }	 
}
