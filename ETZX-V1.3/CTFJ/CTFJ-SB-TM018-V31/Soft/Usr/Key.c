/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.c
*文件描述    		:按键驱动程序
*创建人     		:尹运同
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:采用按键与灯显示控制共用一个I/O口的方式					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_KEY_
#include "config.h"
					
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;

uint8 xdata MainMenuSelect;

/**********************************************************
*函数名称			:SetLedDealState	
*函数描述        	:设置处理灯状态
*输入参数   		:byState:灯状态
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SetLedState(uint8 byState)
{
	LED = (bit)byState;
	FLED = (bit)byState;
	stLocalControl.byLedState = byState;
}


/**********************************************************
*函数名称			:KeyScan	
*函数描述        	:按键扫描函数
*输入参数   		:
*返回值				:NO_KEY:无按键按下,SET_KEY:设置键按下
					 DEAL_KEY:处置键按下   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
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
*函数名称			:KeyManager	
*函数描述        	:按键管理线程
*输入参数   		:
*返回值				: 	
*全局变量			:byMainCmdQ
*调用模块  			:KeyScan,OSQPost
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyManager(void)
{ 	
	static uint8 byKeyValue;
	static uint8 byKeyDelay;
	
    while(TRUE)
    {
    	WDT_CONTR = 0x3d;	//喂狗
        OSWait(K_TMO, OS_TICKS_PER_SEC/50); 				//20ms延时          					   
        byKeyValue = KeyScan();              			
        if(NO_KEY_VAL== byKeyValue)
        {			       	
        	continue;
        }        
        OSWait(K_TMO, OS_TICKS_PER_SEC/50);    				//20ms延时消抖       					
        if(byKeyValue != KeyScan())
        { 			
            continue;
        }
        OSQPost(byMainCmdQ, KEY_DOWN|byKeyValue);

		byKeyDelay = KEY_DELAY;
		while(byKeyDelay--)
		{
			WDT_CONTR = 0x3d;	//喂狗
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
        	WDT_CONTR = 0x3d;	//喂狗
			OSWait(K_TMO, OS_TICKS_PER_SEC/20);  			       
        }
KeyUp:
		OSQPost(byMainCmdQ, KEY_UP|byKeyValue);			
    }
}
/**********************************************************
*函数名称			:KeyDownDeal	
*函数描述        	:按键按下处理函数
*输入参数   		:byKey:按下键的键值
*返回值				: 	
*全局变量			:stLocalControl
*调用模块  			:Bus0OutputData
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyDownDeal(uint8 byKey)
{ 
	byKey&= 0x0f;
	

	stLocalControl.uiLcdDisplayOnTime=250;
	BL_SW =1;
	
	if(bLanding)
	{	//登记状态,不处理	
		return;
	}


	switch(byKey)
	{
		case DIET_KEY_VAL:	//饮食键
			if(stLocalControl.byDisplayFace!= DIET_FACE)  DisplayedLen=0;  //之前是别的界面		
			ShowFace(DIET_FACE);
			stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
			break;

		case PRESCRIPTION_KEY_VAL:	//医嘱键
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
*函数名称			:KeyUpDeal	
*函数描述        	:按键弹起处理函数
*输入参数   		:byKey:弹起键的键值
*返回值				: 	
*全局变量			:
*调用模块  			:MakeCH0TimerOut,MakeCH1TimerOut
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyUpDeal(uint8 byKey)
{
	byKey&= 0x0f;

	if(bLanding)
	{	//登记状态,不处理	
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
*函数名称			:KeyAlwaysDeal	
*函数描述        	:按键长按下处理函数
*输入参数   		:byKey:长按键的键值
*返回值				: 	
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyAlwaysDeal(uint8 byKey)
{
	byKey&= 0x0f;

#if DEBUG ==0	
	if(bLanding)
	{	//登记状态,不处理	
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
