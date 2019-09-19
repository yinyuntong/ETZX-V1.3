/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:main.c
*文件描述    		:主函数
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
#include "config.h"
#include "absacc.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//主线程消息队列
STLocalControl 	xdata stLocalControl;						//全局变量结构体

extern uint8 data byBus0RecData[];
uint8 xdata UartRecData;
uint8 xdata IrDataPosit=0;
uint8 xdata RFSerial[6];		//保存输液报警器对码过程中的序列号

uint8 code  	run[12]={31,29,31,30,31,30,31,31,30,31,30,31};   
uint8 code   notrun[12]={31,28,31,30,31,30,31,31,30,31,30,31};

uint8 code byVersionHi=1;
uint8 code byVersionLo=2;


uint8 xdata BusLowDTime=0;
uint8 xdata BusHighDTime=0;


bit bFourByteRec=0;					//串口接收到4字节命令
bit bNumSeting=0;
bit bRFNumSeting=0;
bit bOxTimerKeyState=1;
bit bEnableOxTimer=0;				//如果没收到时间数据不充许计时
bit bWillSndOxSupplyStart=0;		//上电处于供氧状态
bit bWillSndOxSupplyEnd=0;			//将发送供氧结束命令
bit bWillSndTotalOx;						//将发送供氧计时信息
bit bCloseCGB=0;					//广播时喇叭状态标志


bit bBus1Answer =0;					//接收单总线1的应答标志
bit bOxSupplyState=0;				//正处于供氧计时状态
bit bSixByteRec=0;					//串口接收到6字节命令


void TIMER1Int(void) interrupt T1_INTNO		//3号中断
{
	ET1=0;
}

/*---------------------------------------------------------------------------
函数原型: void Delayms(uint ms)
参数说明: ms--需要延时的值
返 回 值: 无
函数功能：延时程序(对于18.432M晶振单指令周期延时1mS)
----------------------------------------------------------------------------*/
void Delayms(uint16 ms)	  
{
   uint16 xdata i;
   for(;ms!=0;ms--)
		for(i=900;i!=0;i--);
}


uint16   allday(STTime   a)  	//返回多少天 
{    
	uint16 xdata  x; 
	uint8 xdata i;
	x=(a.byYear)*365+a.byYear/4;   
	if((a.byYear%4)==0)  
	{   
		for(i=0;i<a.byMonth;i++)   
		{   
			if(i>0)   
			{   
				 x=x+run[i-1];   
			}   
		}   
	}   
	else   
	{   
		for(i=0;i<a.byMonth;i++)   
		{   
			if(i>0)   
			{   
				x=x+notrun[i-1];   
			}   
		}   
	}   
	x=x+a.byDay;   
	return   x;   
}  

STOXTime   timeInterval(STTime a,STTime   b)  	//a:结束时间  b:起始时间  
{   
  uint16   xdata x,y;   
  STOXTime   xdata sum;   
  x=allday(a);   
  y=allday(b);   
  sum.uiHour=(x-y)*24+a.byHour-b.byHour;   
  if(a.byMin<b.byMin)   
  {   
  	sum.byMin=a.byMin+60-b.byMin;
	sum.uiHour=sum.uiHour-1;   
  }
  else  
  {   
	sum.byMin=a.byMin-b.byMin;   
  }   
  return   sum;   
 } 

/***********************************************************/
void UserTickTimer(void)   /* 系统定时中断中调用的用户函 */
{ 
#if STC90 == 1  
	TF2=0;
#endif
#if STC12C5A==1
	uiIsrTimerCount+=ISR_INC_COUNT;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	CCF0=0;
#endif

}



void UsartInit(void)
{

	RXD=1;
	TXD=1;
	SCON = 0X50;		//工作方式1

#if 0
 //   TMOD= 0x21;   //  ;t1作波特率发生器，t0作方式1（16位)定时器
	TMOD &= 0X0F;		//TO方式不变
	TMOD |= 0X20;		//T1 8位自动重载
	TL1	=	0XF6;		//波特率4800  在18.432M晶振的情况下
	TH1	=	0XF6;
    PCON &=	(~SMOD);   //  ;波特率倍增选择		smod=0
  	AUXR |= T1x12;		//1T模式,最后波特率为:4800*12=57.6K
    TF1=0;
    TR1=1;
#endif


#if 0
	TMOD &= 0X0F;	   //TO方式不变
	TMOD |= 0X20;	   //T1 8位自动重载
	TL1 =   0Xec;	   //波特率2400  在18.432M晶振的情况下
	TH1 =   TL1;
	PCON &= (~SMOD);   //  ;波特率倍增选择	   smod=0
	//	   AUXR |= T1x12;	   //1T模式,最后波特率为:4800*12=57.6K
	TF1=0;
	TR1=1;
#endif

	TMOD &= 0X0F;	   //TO方式不变
	TMOD |= 0X20;	   //T1 8位自动重载
	TL1 =   0Xd8;	   //波特率1200  在18.432M晶振的情况下
	TH1 =   TL1;
	PCON &= (~SMOD);   //  ;波特率倍增选择	   smod=0
	//	   AUXR |= T1x12;	   //1T模式,最后波特率为:4800*12=57.6K
	TF1=0;
	TR1=1;


	
	RI=0;
	TI=0;
	REN=1;
	ES=1;	
}



void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
	uint8 xdata i;
    ES=0;
	for(i=0;i<l;i++)
	{
		SBUF=*Databuf;
		while(!TI);
		TI=0;
		Databuf++;
	}
	ES=1;
 }

void Send_Data_Byte(uint8 SendData)
{
 	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
}


/*void RD_ID(void)
{
	uint8 idata *p;
	p=0xf1;
	Send_Data(p,7);
} */


void UartInt(void) interrupt UART_INTNO
{
	
	if(TI==1)
		TI=0;
	if(RI==1)
	{
		RI=0;
		UartRecData = ~SBUF;	//按位取反
		stLocalControl.byIrDataIntervalTimer=5;		//每字节间隔不超过100ms
		if(IrDataPosit==0)
		{	
			if(UartRecData==0x05)	//起始码
				byIrDARecBuff[IrDataPosit++]=	UartRecData;
		}
		
		else 
		{
			byIrDARecBuff[IrDataPosit++]=	UartRecData;
			if(IrDataPosit >=7)   IrDataPosit =0;
/*			if(IrDataPosit==4)  
			{
				if(byIrDARecBuff[3]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]))
				{
					bFourByteRec=1;
					IrDataPosit=0;
				}
			}

			else if(IrDataPosit==6)
			{
				if(byIrDARecBuff[5]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]+byIrDARecBuff[3]+byIrDARecBuff[4]))
				{

					bSixByteRec =1;
				}
				IrDataPosit=0;	
			}*/
		}	
	}
}

void OXTimerKeyInit(void)
{

/*
//OX_TIMER_KEY准双向口
//	P3M0 &= 0xDF;
//	P3M1 &= 0xDF;	

	//OX_TIMER_KEY高阻输入
	P3M0 |= 0x20;
	P3M1 &= 0xDF;	

	OX_TIMER_KEY=1;
	bOxTimerKeyState=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();	
	if(OX_TIMER_KEY==0)
	{
		OSWait(K_TMO,4);
		if(OX_TIMER_KEY==0)  
		{
			bOxTimerKeyState=0;
			bWillSndOxSupplyStart=1;		//上电时就处于供氧状态
		}
		else 
		{
			bOxTimerKeyState=1;

		}
	}
	*/

	if(stLocalControl.stEepromCfgData.byBedFjFlag & OX_SUPPLY_STATE)	///上电时就处于供氧状态
	{
		bWillSndOxSupplyStart=1;
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
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Init(void)
{
	CMOD = 0x02;    
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块0为16位软件定时器,ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;

	CR = 1;	
	EPCA_LVD =1;
	//复位看门狗                                                                                                                                                                                                                                                                                     
	//WDT_CONTR = 0x3a; 	//@20MHz,157.3ms
	WDT_CONTR = 0x3d;		//@20MHz,1.25s	



	//参数初始化
	byDevState1 = 0;
	byDevState2 = 0;
	byDevState3 = 0;

	//内存清空
	memset(&(stLocalControl.byLedState),0x00,sizeof(STLocalControl));

//STC12C56系列::M0=0,M1=0:准双向，M0=0,M1=1:推挽，M0=1,M1=0:高阻，M0=1,M1=1:开漏

//特别注意:因输入口线上外围未配置上拉电阻，不能配置成高阻输入，只能配置成准双向，否则该脚在高电平时处于悬浮状态
	
//IOC开漏输出,置高:打开,置低:关闭
//,LED控制引脚设定为推挽输出,RS485控制线为准双向口


	//IO初始化
	MUS = 0;	//提示音端口置0

	//MUT推挽输出
	P1M0  &= (~Bin(0,1,0,0,0,0,0,0));
	P1M1  |=   Bin(0,1,0,0,0,0,0,0);
	bMUTState= MUT = 1;		//关闭MC34119

	P1M0 &= (~Bin(0,0,0,0,1,0,0,0));
	P1M1 |=   Bin(0,0,0,0,1,0,0,0);
	XTA = 0;	//关闭MIC到SA的声音通道


	P1M0 &= (~Bin(0,0,0,1,0,0,0,0));
	P1M1 |=   Bin(0,0,0,1,0,0,0,0);
	XTD = 0;	//关闭MIC到SD的声音通道	

	KDR = 0;	//允许485接收

	//MUS推挽输出
	P1M0  &= (~Bin(0,0,0,0,0,0,1,0));
	P1M1  |=   Bin(0,0,0,0,0,0,1,0);

	MUS = 0;	//使SD线路连通

	CloseCBD();
	CloseCGB();

	bNumSeting=0;
	bRFNumSeting=0;
	bEnableOxTimer=0;				//如果没收到时间数据不充许计时
	bWillSndOxSupplyStart=0;					//上电处于供氧状态
	bWillSndTotalOx=0;						//将发送供氧计时总计信息
	bCloseCGB=0;					//广播时喇叭状态标志

	bBus1Answer =0;					//接收单总线1的应答标志
	bOxSupplyState=0;				//正处于供氧计时状态	


  	//串口初始化
  	UsartInit();


	Delayms(200);
	//读取系统配置字
	InitParameter();



   //	RD_ID();	//56系统没有ID
	
	//单总线初始化 	
	SingleBusInit();

	//设置上电指示灯状态 	
	SetLedDealState(LED_ON);
	
	stLocalControl.byLedTimeSet	= 25;
	stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;

	//初始化红外接收数据字节间隔时间
	stLocalControl.byIrDataIntervalTimer=0;	

	//初始化灯及状态标志
	bLedDealState = LED_DEAL =0;

	OXTimerKeyInit();

	
	memset(&(stLocalControl.stTime.byYear),0x00,6);	//清当前时间数据	

	stLocalControl.uiNurseInTime=0;

	stLocalControl.byNumSetTime=0;

	stLocalControl.uiRFNumSetTime=0;
	

//	Send_Data(&(stLocalControl.stEepromCfgData.byRFSerialNum1),6);
	stLocalControl.uiBus1TestTime = BUS1_TEST_TIME;	//与单总线1上的模块每隔1秒检测1次

	//设置上电登记标志,灯闪烁时间初始化,超时设置(超时后自动发送上电登记命令)	
//	bLanding = 1;
	bLanding=0;
	
//	MakeCH0TimerOut(50, 0);


	BusLowDTime=100;
}
/**********************************************************
*函数名称			:GetMessage	
*函数描述        	:获取主线程消息队列中的消息(无消息则挂起自身)
*输入参数   		:Msg:存储消息指针
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
void GetMessage(uint8 data *Msg)
{
	OSQPend(Msg, byMainCmdQ, 0);
}
/**********************************************************
*函数名称			:DispatchMessage	
*函数描述        	:分发处理获取的主线程消息
*输入参数   		:Msg:消息,高4位是命令类型,低4位是命令数据
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
void DispatchMessage(uint8 Msg)
{
	switch(Msg&0xf0)
	{
		case BUS0_REC:										//总线0收到数据帧
			Bus0RecDeal();
			break;
		case BUS0_SND:	 									//总线0数据发送完成
			Bus0SendDeal();
			break;	
		case BUS1_REC:										//总线1收到数据帧
			Bus1RecDeal();
			break;
		case BUS1_SND:	 									//总线1数据发送完成
			Bus1SendDeal();
			break;		
		case TIMER_OUT:	 									//超时处理
			TimerOutDeal();
			break;
		case KEY_DOWN: 										//键按下处理   				
			KeyDownDeal(Msg&0x0f);
			break;
		case KEY_ALWAYS:									//键长按下处理
			KeyAlwaysDeal(Msg&0x0f);
			break;
		case KEY_UP:  										//键弹起处理
			KeyUpDeal(Msg&0x0f);
			break;								
	}	
}
/**********************************************************
*函数名称			:MainTask	
*函数描述        	:系统主线程,负责整个系统的消息处理
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
void MainTask(void)
{
	static uint8 data Msg;  
	
	//系统硬件初始化
	Init();		
	//创建主线程消息队列	
	OSQCreate(byMainCmdQ, 16);
	//以下为创建任务线程	
/*	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(Bus1Manage, NULL, 2);
	OSTaskCreate(TimerOutManager, NULL, 3);
	OSTaskCreate(KeyManager, NULL, 4);
	OSTaskCreate(OXManager, NULL, 5);
*/
	OSTaskCreate(BusManage, NULL, 1);	
	OSTaskCreate(TimerOutManager, NULL, 2);
	OSTaskCreate(KeyManager, NULL, 3);
	//OSTaskCreate(OXManager, NULL, 4);

	//进入消息循环		
	while(TRUE)
	{
		GetMessage(&Msg);
		WDT_CONTR = 0x3d;		//@20MHz,1.25s
		DispatchMessage(Msg);
		if(bFourByteRec==1)
		{
			bFourByteRec=0;
			IrDATreat();			
		}

		if(bSixByteRec ==1)
		{
			bSixByteRec =0;
			IrDANumberSet();
		}
		
	}
} 
/**********************************************************
*函数名称			:main	
*函数描述        	:系统主函数,整个软件的入口
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
void main(void)
{  		
	//操作系统初始化	
	OSInit();
	//创建系统主线程
	OSTaskCreate(MainTask, NULL, 0); 


	while(1)
    {
		_nop_();
 //      PCON = PCON | 0x01;                    //CPU进入休眠状态
    }	 
}
