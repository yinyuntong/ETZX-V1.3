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
*注释	     		:
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_KEY_
#include "config.h"

//按键引脚定义
sbit	SetKey 		= 	P3^4;								//设置键
bit		bLedRedState;										//红灯状态	
/**********************************************************
*函数名称			:SetLedRedState	
*函数描述        	:设置红色Led灯状态
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
void SetLedRedState(uint8 byState)
{
	LED_RED = LED_GREEN = (bit)byState;
	bLedRedState = (bit)byState;
	stLocalControl.byLedState = byState;	
}
/**********************************************************
*函数名称			:KeyScan	
*函数描述        	:按键扫描函数
*输入参数   		:
*返回值				:NO_KEY:无按键按下,SET_KEY:设置键按下  	
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
	if(0 == SetKey)
	{
		return(SET_KEY);
	}
	return(NO_KEY);	
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
	SetKey  = 1;
    while(TRUE)
    {
        OSWait(K_TMO, OS_TICKS_PER_SEC/50); 				//20ms延时           					  
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
	if(bLanding)
	{	//登记状态,不处理	
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