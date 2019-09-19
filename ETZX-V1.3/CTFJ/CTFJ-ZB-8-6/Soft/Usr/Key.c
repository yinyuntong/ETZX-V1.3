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


bit		bLedDealState;										//处理灯状态				
					
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;

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
void SetLedDealState(uint8 byState)
{
	LED_DEAL = (bit)byState;
	bLedDealState = (bit)byState;
	stLocalControl.byLedState &= 0xf0;
	stLocalControl.byLedState |= byState;
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
	OS_ENTER_CRITICAL();  	
	P1M1 &= 0xfb;											//设置成弱上拉模式
	DealKey = 1;									//置高电平
	_nop_();
	_nop_();
	_nop_(); 
	_nop_(); 						
	
	if(0 == DealKey)
	{ 		
		P1M1 |= 0x04;										//还原成强上拉输出模式
		LED_DEAL = bLedDealState; 	 							//恢复指示灯的状态  
		OS_EXIT_CRITICAL();
		return(DEAL_KEY);
	} 
	
	P1M1 |= 0x04;											//还原成强上拉输出模式
	LED_DEAL = bLedDealState; 						//恢复指示灯的状态
	OS_EXIT_CRITICAL();

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
	static uint8 byKeyDelay;
	
    while(TRUE)
    {

//		OxTimerTreat();

		
        OSWait(K_TMO, OS_TICKS_PER_SEC/50); 				//20ms延时          					   
        byKeyValue = KeyScan();              			
        if(NO_KEY == byKeyValue)
        {			       	
        	continue;
        }        
        OSWait(K_TMO, OS_TICKS_PER_SEC/50);    				//20ms延时消抖       					
        if(byKeyValue != KeyScan())
        { 			
            continue;
        }
        OSQPost(byMainCmdQ, KEY_DOWN|byKeyValue);
//		Send_Data_Byte(KEY_DOWN|byKeyValue);
		byKeyDelay = KEY_DELAY;
		while(byKeyDelay--)
		{
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
			OSWait(K_TMO, OS_TICKS_PER_SEC/20);  			       
        }
KeyUp:
		OSQPost(byMainCmdQ, KEY_UP|byKeyValue);	
//		Send_Data_Byte(KEY_UP|byKeyValue);
		WDT_CONTR = 0x3d;		//@20MHz,1.25s

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

		case DEAL_KEY:
			bLedDealState = LED_DEAL =1;
			bDealKeyDown = 1;								//保存处置键的状态
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;					
			if(bNumSeting)	//编号状态
			{
				bNumSeting=0;
				stLocalControl.byNumSetTime=0;
				stLocalControl.stBusDealFreq.bySndSecAddr=stLocalControl.stCallAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr=stLocalControl.stCallAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr=stLocalControl.stCallAddr.bySndBedAddr;
				stLocalControl.stBusDealFreq.byCmd=	CMD_NUMBER_SET;
				stLocalControl.stBusDealFreq.byRecSecAddr=0x02;		//成功
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			
				//保存新的地址数据						
				stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;					

//				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
//				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_NUMBER_SET;			
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}

			if(bRFNumSeting)	//输液报警器对码状态
			{
				bRFNumSeting=0;
				stLocalControl.uiRFNumSetTime=0;
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),RFSerial,3);
				memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(RFSerial[3]),3);
				stLocalControl.stBusDealFreq.byCmd= CMD_RF_NUMSET_OK;	//成功 	
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum1),RFSerial,6);
				//保存输液报警器序列号
				WDT_CONTR = 0x3d; 	//喂狗
				EA=0;
				SaveParameter(IAP0_ADDR);
				EA=1;
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),RFSerial,3);
				stLocalControl.stBusDealFreq.byCmd = CMD_RF_NUMSET_OK;	//成功 
				memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(RFSerial[3]),3);			
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
				return;
			}


			//如果存在本机的呼叫,按下处置键,清除本机的呼叫	
			if(bConfusionNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			if(bServiceNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			if(bHelpNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_HELP_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			if(bEmergencyNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			//如果正在指示其他分机的呼叫,处理其他分机呼叫
			if((bNurseIn&bIndicatingOther))
			{
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
				if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
				{	//卫生间呼叫,不需要处理,直接清除
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				}
				else
				{	//其他类型呼叫,处理
					if(/*stLocalControl.stIndicationData.stAddr.bySecAddr==WIRELESS_SEC&&*/
					   stLocalControl.stIndicationData.stAddr.bySndRoomAddr==ADD_BED_FJ)
					{//加床分机呼叫,不需要处理,直接清除
						stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
					}
					else
					{
						stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
						stLocalControl.stBusDealFreq.byCmd += 0x06;
					}
				} 				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}

			if(bVoiceNoting || bMusicPlaying)
			{//语音提示状态或者背景音乐播放状态
				if(bCloseCGB)
				{//广播打开状态,接下来应关闭广播
					bCloseCGB=0;
					CloseCGB();
					bMUTState = MUT =OFF;
				}
				else
				{
					bCloseCGB=1;
					OpenCGB();
					bMUTState = MUT =ON;					
				}
			}
			break;

		default:
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
	if(bLanding)
	{	//登记状态,不处理	
		return;
	} 
	switch(byKey)
	{
		case DEAL_KEY:
			bLedDealState = LED_DEAL =0;
			bDealKeyDown = 0; 								//保存处置键的状态	  			
			if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
			{	//通道0主动呼叫或者通话状态,减少该通道超时时间				
				MakeCH0TimerOut(5, 0);
			}
			else if(bChannel1Talk)
			{	//通道1主动通话状态,减少该通道超时时间
				MakeCH1TimerOut(5, 0);
			}			
			break;
		default:
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
	if(bLanding)
	{	//登记状态,不处理	
		return;
	} 
	switch(byKey)
	{		
		case DEAL_KEY:
			if((bBusy|bChannel1Talk|bChannel1Talked))
			{	//本机不是处于空闲状态,不作任何处理
				break;
			} 			
			if(bNurseIn)
			{	//护士到位,发送办公区广播命令
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST2;  				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 			
			}			
			break; 	
		default:
			break;
	}		
}
