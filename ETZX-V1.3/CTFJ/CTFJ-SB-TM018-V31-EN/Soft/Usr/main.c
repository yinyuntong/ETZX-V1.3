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
STLocalControl 	xdata stLocalControl;//全局变量结构体



uint8  xdata MicCtrlState;

uint8 code *DispData="  ";

bit bBL_SW_State=1;


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

#if STC12C5A==1
	uiIsrTimerCount+=ISR_INC_COUNT;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	CCF0=0;
#endif


	if(stUsartCfg[0].byRecTimeout!=0) stUsartCfg[0].byRecTimeout--;
}

void BL_SW_Init()
{
	//BL_SW推挽输出
	P5M0 |= 	Bin(0,0,0,0,0,0,1,0);
	P5M1 &=   (~Bin(0,0,0,0,0,0,1,0));
	BL_SW =1;
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
	memset(&(stLocalControl.byLedState),0x00,sizeof(STLocalControl));
	
	CMOD = 0x02;
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块0为16位软件定时器,ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;
 	 
	//启动PCA计数器计数
	CR = 1;	
//	//复位看门狗
	WDT_CONTR = 0x3d; 
	//IO口配置
	//STC12C52/5A系列:M0=0,M1=0:准双向，M0=0,M1=1:高阻，M0=1,M1=0:推挽，M0=1,M1=1:开漏
	//特别注意:因输入口线上外围未配置上拉电阻，不能配置成高阻输入，只能配置成准双向，否则该脚在高电平时处于悬浮状态
	
	//P0:用作SSD1963数据线,准双向
	P0M0 = 0x00;
	P0M1 = 0x00;

	//P1口:P1^0(RS)推挽P1^1(cs)推挽P1^2未用1^3(spi_cs2)推挽P1^4(SPI_CS1)推挽P1^5(MOSI)推挽P1^6(MISO)准双向P1^7(SCLK)推挽
	//P1M0:  1  0  1  1  1  0  1  1
	//P1M1:  0  0  0  0  0  0  0  0
	P1M0 = 0XBB;
	P1M1 = 0X00;

	//P2口::P2^0(SW1)准双向P2^1(SW2)准双向P2^2(SW3)准双向P2^3(SW4)准双向P2^4(SW5)准双向P2^5:未用P2^6:未用P2^7未用
	//P2M0 = 0  0  0  0  0  0  0  0
	//P2M1 = 0  0  0  0  0  0  0  0
	P2M0 = 0X00;
	P2M1 = 0X00;

/*	//P3口:P3^0(RXD)准双向P3^1(TXD)推挽P3^2未用P3^3(RECD):准双向P3^4(SND)推挽P3^5未用P3^6(WR)推挽P3^7(RD)推挽 
	//P3M0 =  1  1  0  1  0  0  1  0
	//P3M1 =  0  0  0  0  0  0  0  0
	P3M0 = 0Xd2;
	P3M1 = 0X00;
*/
	//P3口:P3^0(RXD)准双向P3^1(TXD)未用P3^2推挽P3^3(RECD):准双向P3^4(SND)推挽P3^5未用P3^6(WR)推挽P3^7(RD)推挽 
	//P3M0 =  1  1  0  1  0  1  0  0
	//P3M1 =  0  0  0  0  0  0  0  0
	P3M0 = 0Xd4;
	P3M1 = 0X00;

	
	//P4口:P4^0未用P4^1未用P4^2未用P4^3未用P4^4(RESET)推挽P4^5未用P4^未用P4^7未用 
	//P4M0 =  0  0  0  1  0  0  0  0
	//P4M1 =  0  0  0  0  0  0  0  0
	P4M0 = 0X10;
	P4M1 = 0X00;
	P4SW=P46EN|P45EN|P44EN;

	//P5口:P5^0(LED)推挽P5^1未用P5^2未用P5^3未用
	//P5M0 = 0000  0  0  0  1
	//P5M1 = 0000  0  0  0  0
	P5M0 = 0X01;
	P5M1 = 0X00;

	
	//IO初始化
	LCD_CS7	= 1;
	SST25VF_CS = 1;
	GT23L_CS = 1;

	//关闭MIC
	MicCtrlState=MIC_CTRL=0;

	Delayms(1000);


	//读取系统配置字
	InitParameter();

	//单总线初始化 	
	SingleBusInit();

  	//串口初始化
  	UsartInit();
	Send_Data_Byte(0x33);


	SpiLcdMode();
    ST7735_Init();	//液晶屏初始化
//	背光控制初始化
	BL_SW_Init();

 	//存储器初始化
 	SST25VF_Init();

//	FRAM_TEST();





	//设置上电指示灯状态 	
	SetLedState(LED_OFF);	

	//设置初始时间
	stLocalControl.stTime.byYear=0x11;
	stLocalControl.stTime.byMonth=0x01;
	stLocalControl.stTime.byDay=0x28;
	stLocalControl.stTime.byHour=0x16;
	stLocalControl.stTime.byMinute=0x28;
	
	//设置上电登记标志,超时设置(超时后自动发送上电登记命令)	
	bLanding = 1;	
	MakeCH0TimerOut(50, 0);	

	//,灯闪烁时间初始化
	stLocalControl.byLedTimeSet	= 25;
	stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;	

	stLocalControl.bySecondFlashTime=25;



	Read_Cont((uint8 *)(&(stLocalControl.uiDietDataLen)),DIET_LENGTH_ADDR,2);
	Read_Cont((uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),PRESCRIPTION_LENGTH_ADDR,2);
	Read_Cont((uint8 *)(&(stLocalControl.uiPromptDataLen)),PROMPT_LENGTH_ADDR,2);

	//上电时使屏亮
	ShowPowerUpFace();
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
		case KEY_DOWN: 										//键按下处理   				
			KeyDownDeal(Msg);
			break;
		case KEY_ALWAYS:									//键长按下处理
			KeyAlwaysDeal(Msg);
			break;
		case KEY_UP:  										//键弹起处理
			KeyUpDeal(Msg);
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
	OSTaskCreate(KeyManager, NULL, 2);
	OSTaskCreate(DispManager, NULL, 3);	
	OSTaskCreate(TimerOutManager, NULL, 4);

	//进入消息循环	

	while(TRUE)
	{
		WDT_CONTR = 0x3d;	//喂狗
		GetMessage(&Msg);
		DispatchMessage(Msg);
		if(bUsart0RecFinish==1)
		{
			bUsart0RecFinish=0;
			Usart0RecDeal();
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