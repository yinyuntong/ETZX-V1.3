/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:main.c
*文件描述    		:主函数
*创建人     		:陈卫国
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
#include "config.h"
#include "absacc.h"	

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值

STLocalControl 	xdata stLocalControl;						//全局变量结构体
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


#define OS_TIME_ISR     7                                       /* 系统定时器使用的中断                         */


#define UserTickTimer() 	{uiIsrTimerCount+=ISR_INC_COUNT;CCAP0L = (uint8)uiIsrTimerCount;CCAP0H = (uint8)(uiIsrTimerCount>>8);CCF0=0;/*WDT_CONTR = 0x3a;*/}   /* 系统定时中断中调用的用户函								 */

void OSTickISR(void) interrupt OS_TIME_ISR
{	

    UserTickTimer();                                    /* 用户函数                                                 */

	 
	
	if(++msTimer==4)	//20ms定时到
	{
		msTimer=0;
		bTimer0OutDeal=1;
	}
	
	if(byDealKeyErrorDownTime)	//处于消抖状态
	{
		byKeyValue=KeyScan();
		if(DEAL_KEY != byKeyValue)
		{//不相等,退出消抖状态
			byDealKeyErrorDownTime=0;
		}
		
		else if(--byDealKeyErrorDownTime==0)	//20ms消抖时间到
		{
			bDealKeyShortDown=1;
			byDealKeyShortDownTime =200;			//低于1S时间属短按

		}
	}

	else if(byDealKeyShortDownTime)	//处于短按状态
	{
		byKeyValue=KeyScan();
		if(DEAL_KEY != byKeyValue)
		{//不相等,退出短按状态
			byDealKeyShortDownTime=0;
			bDealKeyUp=1;
		}
		else if(--byDealKeyShortDownTime==0)	//1s短按时间到
		{
			bDealKeyShortDown=0;
			bDealKeyAlwaysDown=1;
			byDealKeyAlwaysDownTime =1;			//为一个真值表示进入长按状态
		}		
	}

	else if(byDealKeyAlwaysDownTime)	//处于长按状态
	{
		byKeyValue=KeyScan();
		if(DEAL_KEY != byKeyValue)
		{//不相等，退出长按状态
			byDealKeyAlwaysDownTime=0;
			bDealKeyUp =1;
		}
	}
}



/*---------------------------------------------------------------------------
函数原型: void Delayms(uint16 ms)
参数说明: ms--需要延时的值
返 回 值: 无
函数功能：延时程序(对于18.432M晶振单指令周期延时1mS)
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
*函数名称			:Init	
*函数描述        	:硬件初始化操作
*输入参数   		:
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
void Init(void)
{ 		
	memset(&(stLocalControl.byLedState),0x00,sizeof(STLocalControl));		//将变量数据清0
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
	//设置PCA模块0为16位软件定时器，ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;	
	//开PCA中断和LVD(低压检测)中断共享的总中断控制位
	CR = 1;	

	//复位看门狗
	WDT_CONTR = 0x3d; 
	EA=1;


	P4SW=0x70;	
	//P4M1 &= 0xAE; //XTD、MDR、MUT设置为推挽输出
    //P4M0 |= 0x51;
	//--------------------------------------------------
	//--------------------------------------------------
#ifdef DL_SWAP
	P4M1 &= 0x3E; //XTD、MUT、RESETB设置为推挽输出 MDR 开漏
    P4M0 |= 0xD1;
#else
	P4M1 &= 0x2E; //XTD、MDR、MUT、RESETB设置为推挽输出
    P4M0 |= 0xD1;
#endif
	//--------------------------------------------------
	//--------------------------------------------------
#ifdef DL_SWAP
	P5M1 &= 0xFb; //XTA设置为推挽输出 MDB、MDG 开漏
    P5M0 |= 0x07;
#else
	P5M1 &= 0xF8; //XTA、MDB、MDG设置为推挽输出
    P5M0 |= 0x07;
#endif



	P3M0 |= 0x08;									   //LED_DEAL灯设置成强上拉输出模式	
	P3M1 &= 0xF7;


	//--------------------------------------------------
	//--------------------------------------------------
	//继电器控制端复位
	RST_BUS=0;
	//--------------------------------------------------
	//--------------------------------------------------
	SingleBusInit(); 
	SpiInit();	
	//UartInit();	
	SetLedDealState(LED_ON); 	
	SetMDLedState((LED_ON<<4)|(LED_ON<<2)|LED_ON); //检查所有门灯的情况


	ParameterInit();  

	
	//-----------------------
	//-----------------------
	//2012/4/9添加波特率可变
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
	//RS485总线一直处于接收状态
	KDR = 0;
	//--------------------------------------------------
	//--------------------------------------------------		
	//参数初始化
	byDevState1 = 0;
	byDevState2 = 0;
	byDevState3 = 0;
	byDevState4 = 0;
	bPWState=1;
	//设置上电指示灯状态 	
	//SetLedDealState(LED_ON);
	//SetMDLedState((LED_ON<<4)|(LED_ON<<2)|LED_ON);
	//设置上电登记标志,灯闪烁时间初始化,超时设置(超时后自动发送上电登记命令)
	stLocalControl.byLedFlashDelay = 25;	
	stLocalControl.byLedFlashTime = 50;
	bLanding = 0;
	byDevState1 = stLocalControl.stEepromCfgData.bySelfSecAddr & 0x80;	
	MakeCH0TimerOut(0, 0);
}



/**********************************************************
*函数名称			:main	
*函数描述        	:系统主函数,整个软件的入口
*输入参数   		:
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
void Main(void)
{  	
	unsigned short iii=0;
	//系统硬件初始化
	Init();		 
	while(1)
    {

		
	    //复位看门狗
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
	   if(bBus0SendFinish)									//总线0发送完数据帧
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
	   	//由于背光驱动有可能出现误保护的情况
    }	 
}
