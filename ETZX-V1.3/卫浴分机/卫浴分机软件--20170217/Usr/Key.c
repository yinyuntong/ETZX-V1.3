/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.c
*�ļ�����    		:������������
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
#define _IN_KEY_
#include "config.h"

//�������Ŷ���
sbit	SetKey 		= 	P3^4;								//���ü�
bit		bLedRedState;										//���״̬	
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;
extern uint8 xdata IRByteCount;
extern bit   SET_BY_SERIAL_FLAG;
bit   SET_BY_SERIAL_OK_FLAG;
extern void SaveParameter(void);
extern bit IR_SET_BY_SERIAL_FLAG;
extern uint8 xdata TEMPIRDATA[6];
/**********************************************************
*��������			:SetLedRedState	
*��������        	:���ú�ɫLed��״̬
*�������   		:byState:��״̬
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
void SetLedRedState(uint8 byState)
{
	LED_RED = LED_GREEN = (bit)byState;
	bLedRedState = (bit)byState;
	stLocalControl.byLedState = byState;	
}
/**********************************************************
*��������			:KeyScan	
*��������        	:����ɨ�躯��
*�������   		:
*����ֵ				:NO_KEY:�ް�������,SET_KEY:���ü�����  	
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
uint8 KeyScan(void)
{
	if(0 == SetKey)
	{
		return(SET_KEY);
	}
	return(NO_KEY);	
} 
/**********************************************************
*��������			:KeyManager	
*��������        	:���������߳�
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:byMainCmdQ
*����ģ��  			:KeyScan,OSQPost
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyManager(void)
{ 	
	static uint8 byKeyValue;  	
	SetKey  = 1;
    while(TRUE)
    {
        OSWait(K_TMO, OS_TICKS_PER_SEC/50); 				//20ms��ʱ           					  
        byKeyValue = KeyScan();              			
        if(byKeyValue == NO_KEY)
        {			       	
           continue;
        }        
        OSWait(K_TMO, OS_TICKS_PER_SEC/50);         					
        if(byKeyValue != KeyScan())
        { 			
            continue;
        }
        OSQPost(byMainCmdQ, KEY_DOWN|byKeyValue);	 		                                                 
        while(byKeyValue == KeyScan())
        {	
			OSWait(K_TMO, 2);  			       
        } 
		WDT_CONTR = 0x3d;
    }
}
/**********************************************************
*��������			:KeyDownDeal	
*��������        	:�������´�����
*�������   		:byKey:���¼��ļ�ֵ
*����ֵ				: 	
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:Bus0OutputData
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyDownDeal(uint8 byKey)
{ 
	if(bLanding)
	{	//�Ǽ�״̬,������	
	    IRByteCount=0; 
		return;
	} 
	if(SET_BY_SERIAL_FLAG)
	{
		SET_BY_SERIAL_FLAG=0;
		//-----------------------------------------------------
		//-----------------------------------------------------
		if(IR_SET_BY_SERIAL_FLAG)
		{
		   IR_SET_BY_SERIAL_FLAG=0;	   
		   stLocalControl.stEepromCfgData.bySelfSecAddr  = TEMPIRDATA[1];
		   stLocalControl.stEepromCfgData.bySelfRoomAddr = TEMPIRDATA[2];
	       stLocalControl.stEepromCfgData.bySelfBedAddr  = TEMPIRDATA[3];
		   memset(TEMPIRDATA,0,6);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		else
		{
		  stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
		  stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	      stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
		}
		stLocalControl.stEepromCfgData.byFlag=CBYTE[0x2803];
		stLocalControl.stEepromCfgData.byPrio=CBYTE[0x2804];
		stLocalControl.stEepromCfgData.byReserve1=CBYTE[0x2805];
		stLocalControl.stEepromCfgData.byReserve2=CBYTE[0x2806];
		stLocalControl.stEepromCfgData.byReserve3=CBYTE[0x2807];
		stLocalControl.stEepromCfgData.bySerialNum1=CBYTE[0x2808];
		stLocalControl.stEepromCfgData.bySerialNum2=CBYTE[0x2809];
		stLocalControl.stEepromCfgData.bySerialNum3=CBYTE[0x280A];
		stLocalControl.stEepromCfgData.byVersionHi=CBYTE[0x280B];
		stLocalControl.stEepromCfgData.byVersionLo=CBYTE[0x280C];
		SaveParameter();
		////��ųɹ���ʼ����1S
		SetLedRedState(LED_FLASH);
		stLocalControl.byLedTime = stLocalControl.byLedDelay =10;
		SET_BY_SERIAL_OK_FLAG =1;
		return;
	}
	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;  
	switch(byKey)
	{
		case SET_KEY:
		 if(!bCalling)
			{
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;								
				stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CALL;   				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}	  						
			else
			{				
				stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;   				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			} 
		IRByteCount=0; 
	    break;
	}		
} 