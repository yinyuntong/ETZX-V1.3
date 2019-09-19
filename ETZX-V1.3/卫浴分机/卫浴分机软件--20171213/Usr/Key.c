/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.c
*�ļ�����    		:������������
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
#define _IN_KEY_
#include "config.h"

//�������Ŷ���
sbit	SetKey 		= 	P3^4;								//���ü�
bit		bLedRedState;										//���״̬	
/**********************************************************
*��������			:SetLedRedState	
*��������        	:���ú�ɫLed��״̬
*�������   		:byState:��״̬
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
*������	      		:����ͬ
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
*������	      		:����ͬ
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
*������	      		:����ͬ
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
			if(bNumSeting)
			{
				bNumSeting =0;
				stLocalControl.stEepromCfgData.bySelfSecAddr = bySec;	
				stLocalControl.stEepromCfgData.bySelfRoomAddr = byRoom;	
				stLocalControl.stEepromCfgData.bySelfBedAddr =0x00;

				byNumSetTime =0;
			    //stLocalControl.byLedTime = stLocalControl.byLedDelay = 0;
		        SetLedRedState(LED_ON);				
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			
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
			break;
			
		default:
			break;
	}		
} 