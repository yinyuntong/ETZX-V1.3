/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:config.h
*文件描述    		:配置头文件
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
//系统头文件
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "STC12C56.H"

#define const code 
#ifndef TRUE
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif


#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))

//主线程命令
#define	BUS0_REC		0x20 								//总线0接收完成
#define BUS0_SND		0x40								//总线0发送完成	 
#define KEY_DOWN		0x60								//按键按下
#define KEY_ALWAYS		0x70
#define KEY_UP			0x80								//按键弹起
#define TIMER_OUT		0xa0								//超时命令	
//按键定义	
#define NO_KEY			0xff								//无键按下 	

sbit  	MP3_DREQ	=	P3^2;
sbit	KEY_COPY	=	P3^5;
sbit 	MP3_XDCS	=	P3^7;

sbit 	PRG_STATE	=	P1^0;
sbit	MP3_XRESET	=	P1^1;
sbit	MP3_XCS		=	P1^2;
sbit	TER_CS		=	P1^3;
sbit	SST25VF_CS		=	P1^4;
sbit	MOSI		=	P1^5;
sbit	MISO		=	P1^6;
sbit	SCLK		=	P1^7;


//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 
#include "SingleBus.h"
#include "Key.h"
#include "IAP.h"
#include "VS10XX.h"
#include "SPI.h"
#include "SST25VF.h"

	 
//设备配置字定义
typedef struct
{
	uint8	byInitFlag; 									//初始化参数标志
	
} STEepromCfgData, *pSTEepromCfgData; 
//超时控制结构体
typedef struct
{
	uint8 				byTimerOutSet;						//一次超时时间设置(20ms单位)
	uint8 				byTimerOut;							//一次超时时间(20ms单位)
	uint8 				byTimerOutCount;					//允许超时次数 
} STTimerOut, *pSTTimerOut;	 	

//全局控制结构体
typedef struct
{
	uint8				byChar1;							//字节数据1	
	uint16				uiShortType;						//短整型数据	
	uint8 				xdata* pbyData;						//数据指针
	STBusFreq 			stBusDealFreq;						//单总线处理结构体
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体
	STEepromCfgData		stEepromCfgData;					//系统配置字	  		
} STLocalControl, *pSTLocalControl;	  	

//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 
#define	Fosc				18432000
#define Fbus				184320
	
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			0x0000		//STC12C5624内的E2只能是数据E2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN_
extern void Send_Data(uint8 *Databuf,uint8 l);
extern void Send_Data_Byte(uint8 SndData);
extern void delay_nus(uint16 i);
extern void delay_nms(uint16 ms);


extern uint16	xdata uiIsrTimerCount ;					//系统时钟定时值
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];						//主线程消息队列
extern STLocalControl 	xdata stLocalControl;						//全局变量结构体
extern uint8 	data volumn;

extern void ConsoleWrite(char code *str);


#endif
