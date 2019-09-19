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
*ע��	     		:���ð��������ʾ���ƹ���һ��I/O�ڵķ�ʽ					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_KEY_
#include "config.h"
					
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;

uint8 xdata MainMenuSelect;

/**********************************************************
*��������			:SetLedDealState	
*��������        	:���ô����״̬
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
void SetLedState(uint8 byState)
{
	LED = (bit)byState;
	FLED = (bit)byState;
	stLocalControl.byLedState = byState;
}


/**********************************************************
*��������			:KeyScan	
*��������        	:����ɨ�躯��
*�������   		:
*����ֵ				:NO_KEY:�ް�������,SET_KEY:���ü�����
					 DEAL_KEY:���ü�����   	
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
	if(0 == CALL_KEY)
	{
		return(CALL_KEY_VAL);
	}
 	

	_nop_(); 
	_nop_(); 						
	if(0 == UP_KEY)
	{ 	

		return(DIET_KEY_VAL);
	}

	_nop_();
	_nop_();
	if(0 == DOWN_KEY)
	{ 		  
		return(PRESCRIPTION_KEY_VAL);
	}  	

	_nop_();
	_nop_();	
	if(0 == ENTER_KEY)
	{ 		  
		return(PROMPT_KEY_VAL);
	}

	_nop_();
	_nop_();	
	if(0 == ESC_KEY)
	{ 		  
		return(ESC_KEY_VAL);
	} 

	return(NO_KEY_VAL);	
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
	static uint8 byKeyDelay;
	
    while(TRUE)
    {
    	WDT_CONTR = 0x3d;	//ι��
        OSWait(K_TMO, OS_TICKS_PER_SEC/50); 				//20ms��ʱ          					   
        byKeyValue = KeyScan();              			
        if(NO_KEY_VAL== byKeyValue)
        {			       	
        	continue;
        }        
        OSWait(K_TMO, OS_TICKS_PER_SEC/50);    				//20ms��ʱ����       					
        if(byKeyValue != KeyScan())
        { 			
            continue;
        }
        OSQPost(byMainCmdQ, KEY_DOWN|byKeyValue);

		byKeyDelay = KEY_DELAY;
		while(byKeyDelay--)
		{
			WDT_CONTR = 0x3d;	//ι��
			if(byKeyValue == KeyScan())
			{	
				OSWait(K_TMO, OS_TICKS_PER_SEC/50); 
			}
			else
			{ 	
				goto KeyUp;
			}
		}		
		OSQPost(byMainCmdQ, KEY_ALWAYS|byKeyValue);				                                              
        while(byKeyValue == KeyScan())
        {	
        	WDT_CONTR = 0x3d;	//ι��
			OSWait(K_TMO, OS_TICKS_PER_SEC/20);  			       
        }
KeyUp:
		OSQPost(byMainCmdQ, KEY_UP|byKeyValue);			
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
	byKey&= 0x0f;
	

	stLocalControl.uiLcdDisplayOnTime=250;
	BL_SW =1;
	
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	}


	switch(byKey)
	{
		case DIET_KEY_VAL:	//��ʳ��
			if(stLocalControl.byDisplayFace!= DIET_FACE)  DisplayedLen=0;  //֮ǰ�Ǳ�Ľ���		
			ShowFace(DIET_FACE);
			stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
			break;

		case PRESCRIPTION_KEY_VAL:	//ҽ����
			if(stLocalControl.byDisplayFace!= PRESCRIPTION_FACE) DisplayedLen=0;
			ShowFace(PRESCRIPTION_FACE);
			stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
			break;	

		case PROMPT_KEY_VAL:
			if(stLocalControl.byDisplayFace!= PROMPT_INFO_FACE) DisplayedLen=0;
			ShowFace(PROMPT_INFO_FACE);
			stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
			break;	

		case ESC_KEY_VAL:
			if(stLocalControl.byDisplayFace != LAND_FACE)
			{
				CurrentPage =0;
				stLocalControl.uiLcdDisplayInfoTime =0;
				ShowLandingFace();
			}
			break;
			
		case CALL_KEY_VAL:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = byKey|KEY_DOWN;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;			
	
	}		
}
/**********************************************************
*��������			:KeyUpDeal	
*��������        	:������������
*�������   		:byKey:������ļ�ֵ
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:MakeCH0TimerOut,MakeCH1TimerOut
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyUpDeal(uint8 byKey)
{
	byKey&= 0x0f;

	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	}

	switch(byKey)
	{
		case DIET_KEY_VAL:
		case PRESCRIPTION_KEY_VAL:
		case PROMPT_KEY_VAL:
		case ESC_KEY_VAL:
			break;
		case CALL_KEY_VAL:	
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = byKey|KEY_UP;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		}

}
/**********************************************************
*��������			:KeyAlwaysDeal	
*��������        	:���������´�����
*�������   		:byKey:�������ļ�ֵ
*����ֵ				: 	
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyAlwaysDeal(uint8 byKey)
{
	byKey&= 0x0f;

#if DEBUG ==0	
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	} 
#endif	
	switch(byKey)
	{
		case DIET_KEY_VAL:
		case PRESCRIPTION_KEY_VAL:
		case PROMPT_KEY_VAL:
		case ESC_KEY_VAL:
			break;
		case CALL_KEY_VAL:	
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = byKey|KEY_ALWAYS;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
	}

}
