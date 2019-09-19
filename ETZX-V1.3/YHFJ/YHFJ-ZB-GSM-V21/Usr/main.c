/*
************************Copyright(c)************************
*	  			   湖南一特股份有限公司
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
#define _IN_MAIN_
#include "config.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//主线程消息队列
STLocalControl 	xdata stLocalControl;						//全局变量结构体

void TIMER1Int(void) interrupt T1_INTNO		//3号中断
{
	ET1=0;
}

void UsartInt(void) interrupt UART_INTNO	//4 //4号中断
{	
	ES=0;
}

#if STC12C5A==true
void AdcInt(void)	interrupt ADC_INTNO		//5号中断
{
	EADC=0;
}

void LvdInt(void) interrupt LVD_INTNO		//6号中断
{
	ELVD=0;
}

//void PcaInt(void) interrupt PCA_INTNO		//7号中断
//{
//	CMOD	&=	!ECF;			//禁止 PCA计数器溢出中断   
//	CCAPM0	&=	!PCA0_ECCF;		//禁止CCF0中断
//	CCAPM1	&=	!PCA1_ECCF;		//禁止CCF1中断	
//}

void SpiInt(void) interrupt SPI_INTNO		//9号中断
{
	IE2 &= ~ESPI;
}

#endif


/***********************************************************/

void UsartInit(void)
{
	   SCON=0X50;	   //工作方式1
	   TMOD&=0X0F;	   //TO方式不变
	   TMOD|=0X20;	   //;t1 方式2作波特率发生器
	
	
	   TL1 =   0XF6;	   //波特率4800  在18.432M晶振的情况下
	   TH1 =   TL1;
	   PCON    &=  ~SMOD;	//	;波特率倍增选择 	   smod=0
	// AUXR    &=  0xBf;	   //传统12分频速度
	   AUXR |= T1x12;	   //1T模式,最后波特率为:4800*12=57.6K
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=1;
	   ES=1;   


}
/***********************************************************/

void uart_send(unsigned char *SDATA,unsigned char length)
{
#if DEBUG==1
	WDT_CONTR = 0x3d;	//喂狗

	EA=0;
	ES=0;
	CR=0;
	TI=0;
	for(;length>0;length--)
	{
		SBUF=*SDATA;
		while(!TI);
		TI=0;
		SDATA++;
	}
	CR=1;
	ES=1;
	EA=1;
#endif
}



void uart_send_byte(unsigned char SDATA)
{
#if DEBUG==1
	EA=0;
	ES=0;
	CR=0;
	TI=0;
	SBUF=SDATA;
	while(!TI);
	TI=0;
	CR=1;
	ES=1;
	EA=1;
#endif
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
		WDT_CONTR = 0x3d;	//喂狗
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

	//参数初始化
	byDevState1 = 0;
	byDevState2 = 0;
	byDevState3 = 0;
	byDevState4 = 0;
	

	//IO初始化
	MUS = 0;
	CloseCGB();
	CloseCBD();



	P1M0 |=   Bin(0,0,0,1,0,0,0,0);
	P1M1 &= (~Bin(0,0,0,1,0,0,0,0));	//XTA配置成推挽输出

	
	P1M0 |=   Bin(0,0,0,0,1,0,0,0);
	P1M1 &= (~Bin(0,0,0,0,1,0,0,0));	//XTD配置成推挽输出	
	XTA = 0;
	XTD = 0;


//	KDR = 1;  	
	KDR =0;

	//MC_CNT配置成强推挽输出
 	P1M0 |=   Bin(0,0,1,0,0,0,0,0); P1M1 &= (~Bin(0,0,1,0,0,0,0,0));
	
	P1M0 |=   Bin(1,0,0,0,0,0,0,0);  P1M1 |=  Bin(1,0,0,0,0,0,0,0);	//MC_CTRL配置成开漏输出

	//MCN开漏输出    悬空
	P1M0 |= Bin(0,1,0,0,0,0,0,0);P1M1 |= Bin(0,1,0,0,0,0,0,0);MCN= 1;
	
	CloseHandleMIC();
	ClosePanelMIC();
	P2M0=0xff;
	P2M1=0xff;	//喇叭的音量控制配置成开漏输出
	SetSPKVol0();	//喇叭静音


	//PW强推挽输出
	P0M0 |=   Bin(0,0,0,0,1,0,0,0);
	P0M1 &= (~Bin(0,0,0,0,1,0,0,0));	
	bPWState=PW=1;		//禁止34018
	bPW2State=PW2=1;	//禁止TDA7233
	bVl0State=VL0=0;	//关闭34018的喇叭

		
	Delayms(1000);		//延时1S
	
	//读取系统配置字
	InitParameter();

  	//串口初始化
  	UsartInit();
	uart_send_byte(0X88);

	//单总线初始化 
	SingleBusInit(); 
	
	//初始化呼叫地址
	InitKeyCallAddr();
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
	memset(stLocalControl.byKeyDownState, 0x00, 4);	
	//状态初始化  	
	bLanding = 1;
	//初始化听筒状态为挂机状态
	bHandleDown = 1;	
	MakeCH0TimerOut(50, 0);	


//	测试声音通道
/*	bPWState=PW=0;				//使能34018
	OpenPanelMIC();
	XTD=1;
	OpenCGB();
	SetSPKVol4();
	bVl0State=1;*/

/*	bPW2State=PW2=0;				//使能TDA7233
	OpenHandleMIC();
	XTD=1;
	OpenCGB();*/	

//	while(1);

	
	led0init();
	stLocalControl.byNumSetTime=0;
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
		case KEY_DOWN: 										//听筒挂机处理   				
			KeyDown();
			break;		
		case KEY_UP:  										//听筒摘机处理
			KeyUp();
			break;			
		case TIMER_OUT:	 									//超时处理
			TimerOutDeal();
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
	OSTaskCreate(Bus0Manage, NULL, 1);
	OSTaskCreate(Bus1Manage, NULL, 2);
	OSTaskCreate(KeyManager, NULL, 3);
	OSTaskCreate(TimerOutManager, NULL, 4);		
	//进入消息循环		
	while(TRUE)
	{
		WDT_CONTR = 0x3d;	//喂狗
		GetMessage(&Msg);
		DispatchMessage(Msg);

		if(Bus0RxBuffLen() >= BUS0_FREQ_SIZE)
		{ 	//有一帧完整的数据在接收队列中 
			OS_ENTER_CRITICAL();
			stLocalControl.stBusDealFreq.bySndSecAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.bySndRoomAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.bySndBedAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byCmd= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byRecSecAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byRecRoomAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			stLocalControl.stBusDealFreq.byRecBedAddr= byBus0RxQ[byBus0RxHead];
			IncBus0RxPtr(byBus0RxHead);
			OS_EXIT_CRITICAL();	
			Bus0RecDeal();
		}
		else
		{	//没有一帧完整的数据在发送队列中了
			byBus0RxHead = byBus0RxTail = 0;
		}

		
		if(Bus1RxBuffLen() >= BUS1_FREQ_SIZE)
		{ 	//有一帧完整的数据在接收队列中 
			OS_ENTER_CRITICAL();
			stLocalControl.stBusDealFreq.bySndSecAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.bySndRoomAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.bySndBedAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byCmd= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byRecSecAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byRecRoomAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			stLocalControl.stBusDealFreq.byRecBedAddr= byBus1RxQ[byBus1RxHead];
			IncBus1RxPtr(byBus1RxHead);
			OS_EXIT_CRITICAL();	
			Bus1RecDeal();
		}
		else
		{	//没有一帧完整的数据在发送队列中了
			byBus1RxHead = byBus1RxTail = 0;
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
       PCON = PCON | 0x01;                    //CPU进入休眠状态	    
    }	 
}
