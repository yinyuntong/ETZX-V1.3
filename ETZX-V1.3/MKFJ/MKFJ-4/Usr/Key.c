/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.c
*文件描述    		:按键驱动程序
*创建人     		:陈卫国
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
sbit	DealKey = P3^3;									//处置键	
bit		bLedRedState;									//门灯红色灯状态
bit		bLedGreenState;									//门灯绿色灯状态
bit		bLedBlueState;									//门灯蓝色灯状态		
bit		bLedDealState;									//处理灯状态 
  
extern STLocalControl 	xdata stLocalControl;
extern bit   SET_BY_SERIAL_FLAG;
extern bit   SerchRoomFlag;
extern bit   SerchRoomKeyFlag;
extern bit   Broadcasting_Flag;
extern uint16 xdata  SerchRoomCount;
extern uint8  xdata  Modify_Room_Number;
extern uint8  xdata  Modify_SEC_Name;
extern void SaveParameter(void);


extern uint8 xdata byDealKeyErrorDownTime;
extern uint8 xdata byDealKeyShortDownTime;
extern uint8 xdata byDealKeyAlwaysDownTime;

bit   SET_BY_SERIAL_OK_FLAG;
bit   Key_SerchRoom_Flag2;
bit   Key_SerchRoom_Flag3;
/**********************************************************
***********************************************************
*函数名称			:SetMDLedState	
*函数描述        	:设置门灯三色灯状态
*输入参数   		:byState:bit0-1:红灯状态,bit2-3:绿灯状态
					 bit4-5:蓝灯状态
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SetMDLedState(uint8 byState)
{
	stLocalControl.byMDLedState = byState;					//保存所有门灯的状态
	#ifdef DL_SWAP
	bLedRedState   = (bit)(byState & 0x03);
	MDR = !bLedRedState;
	bLedGreenState = (bit)(byState & 0x0C);
	MDG = !bLedGreenState;	
	bLedBlueState  = (bit)(byState & 0x30);			//设置所有门灯的状态
	MDB = !bLedBlueState;
	#else
	MDR = bLedRedState   = (bit)(byState & 0x03);
	MDG = bLedGreenState = (bit)(byState & 0x0C);	
	MDB = bLedBlueState  = (bit)(byState & 0x30);			//设置所有门灯的状态
	#endif
}
/**********************************************************
*函数名称			:SetLedDealState	
*函数描述        	:设置红色Led灯状态
*输入参数   		:byState:灯状态
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
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
	stLocalControl.byLedState &= 0xF0;
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 KeyScan(void)
{
	EA=0;  	
	P3M1 &= 0xF7; 			                                //设置成弱上拉模式
	P3M0 &= 0xF7;
    DealKey = 1;									        //置高电平
	_nop_();
	_nop_();
	_nop_();  
	_nop_();						
	if(0 == DealKey)
	{ 												
		P3M0 |= 0x08;                                      //还原成强上拉输出模式	
		LED_DEAL = bLedDealState; 						   //恢复指示灯的状态 

		if((byDealKeyErrorDownTime==0)&&(byDealKeyShortDownTime==0)&&(byDealKeyAlwaysDownTime==0))						//没有进入消抖，短按，长按状态
			byDealKeyErrorDownTime=4;							//20ms消抖
		EA=1;
		return(DEAL_KEY);
	}  	
	P3M0 |= 0x08;											//还原成强上拉输出模式	
	LED_DEAL = bLedDealState; 							   //恢复指示灯的状态
	EA=1;
	return(NO_KEY);	
} 

/**********************************************************
*函数名称			:KeyDownDeal	
*函数描述        	:按键按下处理函数
*输入参数   		:byKey:按下键的键值
*返回值				: 	
*全局变量			:stLocalControl
*调用模块  			:Bus0OutputData
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyDownDeal(uint8 byKey)
{ 
    if(Broadcasting_Flag)
	{
	  return;   //广播状态，不处理
	}
	if(bLanding)
	{	//登记状态,不处理	
		return;
	}
    if(SET_BY_SERIAL_FLAG)
     {
		SET_BY_SERIAL_FLAG=0;
		stLocalControl.stEepromCfgData.bySelfSecAddr  = stLocalControl.stBusDealFreq.bySndSecAddr;
	    stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	    stLocalControl.stEepromCfgData.bySelfBedAddr  = stLocalControl.stBusDealFreq.bySndBedAddr;
		if(FmReadByte(Close_EnSoundNote_ADDR)==0x01)
		stLocalControl.stEepromCfgData.byEnable1=0xF7;
		else
		stLocalControl.stEepromCfgData.byEnable1=CBYTE[0x2803];
		stLocalControl.stEepromCfgData.byEnable2=CBYTE[0x2804];
		stLocalControl.stEepromCfgData.byPrio=CBYTE[0x2805];
		stLocalControl.stEepromCfgData.byRingTime=CBYTE[0x2806];
		stLocalControl.stEepromCfgData.byTalkTime=CBYTE[0x2807];
		stLocalControl.stEepromCfgData.byBroadTime=CBYTE[0x2808];
		stLocalControl.stEepromCfgData.byListenDelay=CBYTE[0x2809];
		stLocalControl.stEepromCfgData.byVolumStep=CBYTE[0x280A];
		stLocalControl.stEepromCfgData.byRingVol=CBYTE[0x280B];
		stLocalControl.stEepromCfgData.byCH0TalkedVol=CBYTE[0x280C];
		stLocalControl.stEepromCfgData.byCH1TalkedVol=CBYTE[0x280D];
		stLocalControl.stEepromCfgData.byBroadVol=CBYTE[0x280E];
		stLocalControl.stEepromCfgData.byNoteVol=CBYTE[0x280F];
		stLocalControl.stEepromCfgData.byMusicVol=CBYTE[0x2810];
		stLocalControl.stEepromCfgData.byCH0TalkVol=CBYTE[0x2811];
		stLocalControl.stEepromCfgData.byCH1TalkVol=CBYTE[0x2812];
		stLocalControl.stEepromCfgData.bySelfRingVol=CBYTE[0x2813];
		stLocalControl.stEepromCfgData.byMaxVol=CBYTE[0x2814];
		stLocalControl.stEepromCfgData.byMinVol=CBYTE[0x2815];
		stLocalControl.stEepromCfgData.byReserve1=CBYTE[0x2816];
		stLocalControl.stEepromCfgData.byReserve2=CBYTE[0x2817];
		stLocalControl.stEepromCfgData.byReserve3=CBYTE[0x2818];
		stLocalControl.stEepromCfgData.bySerialNum1=CBYTE[0x2819];
		stLocalControl.stEepromCfgData.bySerialNum2=CBYTE[0x281A];
		stLocalControl.stEepromCfgData.bySerialNum3=CBYTE[0x281A];
		stLocalControl.stEepromCfgData.byVersionHi=CBYTE[0x281C];
		stLocalControl.stEepromCfgData.byVersionLo=CBYTE[0x281D];
		byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	    byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	    SaveParameter();
	    SetLedDealState(LED_OFF);
		Modify_Room_Number = 0x01;
		FmWriteByte(Modify_Room_Number_ADDR,0x01);
		ShowNumberPageOK();	 		//编号成功后提示
		SET_BY_SERIAL_OK_FLAG=1;
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
			bDealKeyDown = 1;								//保存处置键的状态			
			if((bConfusionNoting|bServiceNoting|bHelpNoting|bEmergencyNoting))
			{	//有本房呼叫存在,清除本房呼叫
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
				stLocalControl.stBusDealFreq.byCmd += 0x0A; 
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.byRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.byBedAddr;						
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Key_SerchRoom_Flag2=0;
		        Key_SerchRoom_Flag3=0;				
			} 			
			else if(bIndicatingOther)
			{	//如果正在指示其他分机的呼叫,处理其他分机呼叫
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.byRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.byBedAddr;
				if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
				{	//卫生间呼叫,不需要处理,直接清除
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				}
				else
				{	//其他类型呼叫,处理
					stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
					stLocalControl.stBusDealFreq.byCmd += 0x0A;
				} 				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Key_SerchRoom_Flag2=0;
		        Key_SerchRoom_Flag3=0;				
			} 
		   else if(Key_SerchRoom_Flag3)	//第三屏查房记录显示,显示时间为5S
		   {
		      Key_SerchRoom_Flag2=0;
		      Key_SerchRoom_Flag3=0;
			  SerchRoomKeyFlag=1;
			  SerchRoomCount=0;
			  SerchRoomFlag=0;
			  ShowSerchRommPage3();
		   }
		   else if(Key_SerchRoom_Flag2)  //第二屏查房记录显示，显示时间为5S
		   {
		      Key_SerchRoom_Flag3=1;
			  Key_SerchRoom_Flag2=0;	  
			  SerchRoomKeyFlag=1;
			  SerchRoomCount=0;
			  SerchRoomFlag=0;
		      ShowSerchRommPage2();
		   }
		   else                        //第一屏查房记录显示，显示时间为5S
		   {
		      Key_SerchRoom_Flag2=1;	  
			  Key_SerchRoom_Flag3=0;
			  SerchRoomKeyFlag=1;
			  SerchRoomCount=0;
			  SerchRoomFlag=0;
		      ShowSerchRommPage1();
		   } 
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyUpDeal(uint8 byKey)
{
    if(Broadcasting_Flag)
	{
	  return;   //广播状态，不处理
	}
	if(bLanding)
	{	//登记状态,不处理	
		return;
	} 
	switch(byKey)
	{
		case DEAL_KEY:			
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void KeyAlwaysDeal(uint8 byKey)
{
    if(Broadcasting_Flag)
	{
	  return;   //广播状态，不处理
	}
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
		if(bNurseIn) 		//长按键表示全区广播
		{
		  stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		  stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		  stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		  stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
		  stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
		  stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
		  stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST3;  				
		  Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
		  ShowCalling();	
		  SerchRoomKeyFlag=0;
		}		
	}		
}