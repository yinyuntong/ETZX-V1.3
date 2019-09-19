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
#define _IN_MAIN_
#include "config.h"

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//主线程消息队列
STLocalControl 	xdata stLocalControl;						//全局变量结构体


//动态管理内存大小定义
#define MALLOC_MEM_SIZE		100
//uint8 			xdata 			byMallocMem[MALLOC_MEM_SIZE];

void Ex0Int(void)	interrupt X0_INTNO		//0号中断
{
	EX0=0;
}

void TIMER1Int(void) interrupt T1_INTNO		//3号中断
{
	ET1=0;
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

#endif
/***********************************************************/
void UserTickTimer(void)   /* 系统定时中断中调用的用户函 */
{ 
#if STC90 == 1  
	TF2=0;
#endif
#if STC12C5A==1
	uiIsrTimerCount+=ISR_INC_COUNT;
	CCAP1L = (uint8)uiIsrTimerCount;
	CCAP1H = (uint8)(uiIsrTimerCount>>8);
	CCF1=0;
#endif

	if(stUsartCfg.byRecTimeout!=0) stUsartCfg.byRecTimeout--;
}


void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
#if DEBUG==1
    uint8 xdata i;
	WDT_CONTR = 0x3d; 

    ES=0;
	for(i=0;i<l;i++)
     {
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;
	 }
	ES=1;
#endif
}


void Send_Data_Byte(uint8 SendData)
{ 
#if DEBUG==1
    ES=0;
    SBUF=SendData;
    while(!TI);
	TI=0;
	ES=1;
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
		WDT_CONTR = 0x3d; 
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

	CCAPM0 = 0x11;	//下降沿中断,使能
	
	
	CCAP1L = (uint8)uiIsrTimerCount;
	CCAP1H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块1为16位软件定时器,ECCF1=1允许PCA模块1中断
	CCAPM1 = 0x49;
 	 
	//启动PCA计数器计数
	CR = 1;	

	
	//复位看门狗
	WDT_CONTR = 0x3d; 
	//IO口配置
	//STC12C52/5A系列:M0=0,M1=0:准双向，M0=0,M1=1:高阻，M0=1,M1=0:推挽，M0=1,M1=1:开漏
	
	//特别注意:因输入口线上外围未配置上拉电阻，不能配置成高阻输入，只能配置成准双向，否则该脚在高电平时处于悬浮状态

	P4SW=P46EN|P45EN|P44EN;	//P4口设置成IO口

	
	//IO初始化
	SSD1963_CS	= 1;
	SST25VF_CS = 1;
	GT23L_CS = 1;
	ADS7843_CS = 1;
	EPH1660_CS = 0;




	//全局变量初始化为0
	memset(&(stLocalControl.byChar1),0x00,sizeof(STLocalControl));
	
	Delayms(200);

	
	//读取系统配置字
	InitParameter();

  	//串口初始化
  	UsartInit();


	//单总线初始化 	
	SingleBusInit();


	//复位EPH1660
//	ResetEPH1660();


	//设置MCU为SPI主方
	MCUMasterSPI();


	//存储器初始化
	SST25VF_Init();

//	FRAM_TEST();

	//RGB驱动芯片初始化
	InitSSD1963();	

		
	//设置上电登记标志,灯闪烁时间初始化,超时设置(超时后自动发送上电登记命令)	
	bLanding = 1;	
	MakeCH0TimerOut(50, 0);	

	ShowPowerUpFace(DIS_FRAM_MEM1);	//放在第1页
	
	stLocalControl.stTime.byYear=0x08;
	stLocalControl.stTime.byMonth=0x08;
	stLocalControl.stTime.byDay=0x08;
	stLocalControl.stTime.byHour=0x08;
	stLocalControl.stTime.byMin=0x08;
	stLocalControl.byDisplayFace=0;

	stLocalControl.bySecondFlashTime=25;
	stLocalControl.byDispNumSetOkTime=0;

	Read_Cont(WARD_FACE_DATA_START_ADDR,WARD_FACE_DATA_LEN,WardDataBuff);
	StorSecInfo(DIS_FRAM_MEM0);
//	OSMemInit(byMallocMem,MALLOC_MEM_SIZE);
	
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
//			bBus0RecFinish = 0;
//			Bus0RecData(byBus0RecData);
//			Bus0RecDeal();
			break;
		case BUS0_SND:	 									//总线0数据发送完成
			Bus0SendDeal();
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
	OSTaskCreate(TimerOutManager, NULL, 2);
//	OSTaskCreate(EPH1660Manager, NULL, 3);
	OSTaskCreate(DispManager, NULL, 3);	
	//进入消息循环	

	while(TRUE)
	{
		WDT_CONTR = 0x3d; 
		GetMessage(&Msg);
		DispatchMessage(Msg);
		if(bUsart0RecFinish)									//串口0收到一帧数据
		{ 	
			bUsart0RecFinish=0;
			Usart0RecDeal();	
		}	
		
/*		if(byBus0FreqNum>0)
		{
			Bus0RecDeal();
		}
*/		


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
	EPH1660_CS=0;	//禁止
	Delayms(200);
	//操作系统初始化	
	OSInit();

	led0=1;
	//创建系统主线程
	OSTaskCreate(MainTask, NULL, 0); 
	
	while(1)
    {
		_nop_();
 //      PCON = PCON | 0x01;                    //CPU进入休眠状态	    
    }	 
}
