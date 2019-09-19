/*
************************Copyright(c)************************
*	  			   湖南一特股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.c
*文件描述    		:听筒监控程序
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

sbit 	HandlerKey = P4^3;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];  
extern STLocalControl 	xdata stLocalControl;
 
/**********************************************************
*函数名称			:KeyManager	
*函数描述        	:听筒监控线程
*输入参数   		:
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
void KeyManager(void)
{
	HandlerKey = 1;											//听筒设置成弱上拉
	while(TRUE)
	{
		WDT_CONTR = 0x3d;	//喂狗
		OSTimeDly(OS_TICKS_PER_SEC/40);
		if(HandlerKey == 0)									//听筒放下为低电平
		{
			continue;
		}
		OSTimeDly(OS_TICKS_PER_SEC/40);
		if(HandlerKey == 0)
		{
			continue;
		}
		OSQPost(byMainCmdQ, KEY_UP);						//听筒摘机了
		fled0=led0=1;
		while(HandlerKey == 1)
		{
			WDT_CONTR = 0x3d;	//喂狗
			OSTimeDly(OS_TICKS_PER_SEC/50);
		}
		OSQPost(byMainCmdQ, KEY_DOWN);						//听筒挂机了
		fled0=led0=0;
	}
}
/**********************************************************
*函数名称			:KeyDown	
*函数描述        	:听筒挂机处理函数
*输入参数   		:
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
void KeyDown(void)
{
	bHandleDown = 1; 										//保存听筒当前状态	
	VoiceChannelCtx();										//语音通道切换到免提上
	if((bChannel1Talk&bHandAnswer))
	{	//确实是用听筒主动呼叫的,缩短超时时间,超时后自动发送通道切换命令		
		MakeCH1TimerOut(5, 0);
		
	}	
	else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad)&&bHandAnswer)
	{	//确实是用听筒主动呼叫的,缩短超时时间,超时后自动发送复位命令		
		MakeCH0TimerOut(5, 0);
		
	}
	else if((!bHandAnswer)&&(!bKeyAnswer)&&(!bIndicatingOther))
	{				
		InitKeyCallAddr();
	}	
}
/**********************************************************
*函数名称			:KeyUp	
*函数描述        	:听筒摘机处理函数
*输入参数   		:
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
void KeyUp(void)
{
	bHandleDown = 0;											//保存听筒当前状态
	VoiceChannelCtx(); 											//语音切换到听筒上
	if(bIndicatingOther&&(!bHandAnswer)&&(!bKeyAnswer))
	{	//如果正在报号,发送处理呼叫命令(紧急呼叫发送清除命令)		
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
		stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
		stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;				
		if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
		}
		else
		{	//不是紧急呼叫,置听筒接听标志,发送处理命令									
			stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
			stLocalControl.stBusDealFreq.byCmd += 0x06; 
		}		
		if(TRUE == Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
		{
			bCallDealSending = 1;
		}		
	}
	else
	{
		stLocalControl.stBusDealFreq.bySndSecAddr = KEY_CLEAR;
		stLocalControl.stBusDealFreq.byCmd = CMD_KEY_DOWN;
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
		stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
		bRoomAddr = 0;
		bSetPrio = 0;
	}
}