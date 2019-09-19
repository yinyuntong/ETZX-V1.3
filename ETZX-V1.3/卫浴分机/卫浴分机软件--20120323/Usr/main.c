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
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//主线程消息队列
STLocalControl 	xdata stLocalControl;						//全局变量结构体


extern uint8 xdata BusLowDTime;



void UsartInit(void)
{

	RXD=1;
	TXD=1;
	SCON = 0X50;		//工作方式1

	TMOD &= 0X0F;		//TO方式不变
	TMOD |= 0X20;		//T1 8位自动重载
	TL1	=	0XF6;		//波特率4800  在18.432M晶振的情况下
	TH1	=	0XF6;
    PCON &=	(~0x80/*SMOD*/);   //  ;波特率倍增选择		smod=0
  	AUXR |= 0x40/*T1x12*/;		//1T模式,最后波特率为:4800*12=57.6K
    TF1=0;
    TR1=1;
	
	RI=0;
	TI=0;
	REN=1;
	//ES=1;	
}



void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
	uint8 xdata i;
   // ES=0;
	for(i=0;i<l;i++)
	{
		SBUF=*Databuf;
		while(!TI);
		TI=0;
		Databuf++;
	}
	//ES=1;
 }

void Send_Data_Byte(uint8 SendData)
{
 //	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
//	ES=1;
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
	CMOD = 0x02;
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块0为16位软件定时器，ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;	
	//开PCA中断和LVD(低压检测)中断共享的总中断控制位
	EPCA_LVD = 1;
	//启动PCA计数器计数
	CR = 1;	
	//复位看门狗
	WDT_CONTR = 0x3d; 
	//P1M1 |= 0xC0;
	//P1M0 &= 0x3F;
	P1M1 |= 0xC8;
	P1M0 &= 0x37;	
	//读取系统配置字
	//ReadParameter(); 
	//--------------------------------------------------
	//--------------------------------------------------
	//继电器控制端复位
	RST_BUS=0;
	//--------------------------------------------------
	//--------------------------------------------------
	EnableIAP(IAP_READ);
	if(IapReadByte(0x0800)==INIT_FLAG)
	 {
        ReadParameter();
	 }
	else
	 {
	    stLocalControl.stEepromCfgData.bySelfSecAddr=CBYTE[0x2800];
		stLocalControl.stEepromCfgData.bySelfRoomAddr=CBYTE[0x2801];
		stLocalControl.stEepromCfgData.bySelfBedAddr=CBYTE[0x2802];
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
	 }
	//-------------------------------------------------------
	//-------------------------------------------------------
	//单总线初始化 
	SingleBusInit();
	//红外接收初始化 
	IRInit();
	//状态初始化
	bLanding = 0;
	bCalling = 0;
	//设置灯的状态和闪烁时间,设置上电超时	
	stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;	  
	if(stLocalControl.stEepromCfgData.byFlag&0x01)
    {	//设定为灯长亮
		SetLedRedState(LED_ON);	
	}
	else
	{	//设定为灯灭
		SetLedRedState(LED_OFF);	
	}				
	MakeCH0TimerOut(0, 0); 
	BusLowDTime=100;


	UsartInit();
	Send_Data_Byte(0x55);
}
/**********************************************************
*函数名称			:GetMessage	
*函数描述        	:获取主线程消息队列中的消息(无消息则挂起自身)
*输入参数   		:Msg:存储消息指针
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
*创建人	      		:陈卫国
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
		case KEY_DOWN: 										//键按下处理 
			KeyDownDeal(Msg&0x0f);
			break;				
		case TIMER_OUT:	 									//超时处理
			TimerOutDeal();
			break;
		case IR_REC:	 									//红外数据处理
			IRRecDeal();
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
*创建人	      		:陈卫国
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

	WDT_CONTR = 0x3d;	
	//以下为创建任务线程 
	OSTaskCreate(Bus0Manage, NULL, 1);
	OSTaskCreate(IRRecManage, NULL, 2);		
	OSTaskCreate(KeyManager, NULL, 3);
	OSTaskCreate(TimerOutManager, NULL, 4);	
	//进入消息循环
	WDT_CONTR = 0x3d;	
	while(TRUE)
	{
		GetMessage(&Msg);
		DispatchMessage(Msg);
		WDT_CONTR = 0x3d;

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
*创建人	      		:陈卫国
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
       PCON = PCON | 0x01;                    //CPU进入休眠状态   
    }	 
}
