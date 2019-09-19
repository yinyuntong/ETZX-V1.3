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

#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "STC12C5A.H"
#define const code 
#ifndef TRUE
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif 

//#define DL_SWAP 1  //因灯带的错误需要将极性翻转,并将顺序合倒顺

typedef unsigned char  uint8;                                   /* 定义可移植的无符号8位整数关键字            */
typedef signed   char  int8;                                    /* 定义可移植的有符号8位整数关键字            */
typedef unsigned int   uint16;                                  /* 定义可移植的无符号16位整数关键字           */
typedef signed   int   int16;                                   /* 定义可移植的有符号16位整数关键字           */
typedef unsigned long  uint32;                                  /* 定义可移植的无符号32位整数关键字           */
typedef signed   long  int32;                                   /* 定义可移植的有符号32位整数关键字           */

//主线程命令定义  
#define	BUS0_REC		0x10 								//总线接收完成
#define BUS0_SND		0x20								//总线发送完成
#define KEY_DOWN		0x30								//按键按下
#define KEY_UP			0x40								//按键弹起
#define	KEY_ALWAYS		0x50								//键常按下
#define USART_REC		0x60 								//串口接收
#define TIMER_OUT		0x70 								//超时命令
//按键定义
#define DEAL_KEY		0x01								//处理键
#define NO_KEY			0xFF								//无键按下	
//灯定义
#define	LED_OFF			0									//灯关闭
#define	LED_ON			1									//灯常亮
#define	LED_FLASH		2									//灯闪烁
//控制引脚定义
sbit	LED_DEAL	=	P3^3;								//处理灯
sbit	CGB			= 	P1^2;								//SD控制脚
sbit	CBD			= 	P1^3;								//SA控制脚 
sbit	VOL			= 	P1^4;								//音量控制脚
sbit	CTA			= 	P5^2;								//MIC的SA控制
sbit	CTD			=	P4^0;								//MIC的SD控制
sbit  	KDR 		= 	P3^4;								//485控制引脚 
sbit    PW			= 	P4^6;								//功放控制引脚
#ifdef DL_SWAP
sbit 	MDR			=	P5^0;								//门灯红
sbit 	MDB			=	P4^4;								//门灯蓝
sbit    MDG			=	P5^1;								//门灯绿
#else
sbit 	MDR			=	P4^4;								//门灯红
sbit 	MDB			=	P5^0;								//门灯蓝
sbit    MDG			=	P5^1;								//门灯绿
#endif
//-------------------------------------------------------------------
//-------------------------------------------------------------------
sbit    RST_BUS	   =    P4^7;                               //总线复位端              
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#define OpenCGB()		{P1M1 |= 0x04; P1M0 |= 0x04; CGB = 1;}	//开漏
#define CloseCGB()		{P1M1 &= 0xFB; P1M0 &= 0xFB; CGB = 1;}	//准双向输入
#define OpenCBD()		{P1M1 |= 0x08; P1M0 |= 0x08; CBD = 1;}	//开漏
#define CloseCBD()		{P1M0 &= 0xF7; P1M1 &= 0xF7; CBD = 1;}	//准双向输入
//液晶背光控制端											//液晶屏背光控制
sbit	LCD_LED	    = 	P3^5;
#define SetLcdState(a)	{LCD_LED = a;bLcdState = a;}
//---------------------------------------------------------------------------
//包含头文件//
//#include "OS_CFG.H"
//#include "OS_CPU.H"
//#include "..\os\OS.H"
//#include "..\os\OS_Q.h"
//#include "..\os\OS_SEM.h"	
#include "uart.h"
#include "LcdDriver.h"
#include "SingleBus.h"
#include "SPI.h"
#include "IAP.h"
#include "Key.h"
typedef struct
{
	uint8  	bySelfSecAddr;									//本机区地址
	uint8  	bySelfRoomAddr;									//本机房地址
	uint8  	bySelfBedAddr;									//本机床地址
	uint8  	byEnable1;										//允许标志1
	uint8  	byEnable2;										//允许标志2	
	uint8   byPrio;											//优先级
	uint8	byRingTime;										//振铃时间
	uint8	byTalkTime;										//通话时间
	uint8	byBroadTime;									//广播时间
	uint8	byListenDelay;									//接听延时时间
	uint8	byVolumStep;									//音量调节步长
	uint8	byRingVol;										//振铃提示音量
	uint8	byCH0TalkedVol;									//通道0被呼通话音量
	uint8	byCH1TalkedVol;									//通道1被呼通话音量
	uint8	byBroadVol;										//广播音量
	uint8	byNoteVol;										//提示音量
	uint8	byMusicVol;										//背景音乐音量
	uint8	byCH0TalkVol;									//通道0主动通话音量
	uint8	byCH1TalkVol;									//通道1主动通话音量
	uint8	bySelfRingVol;									//本机振铃提示音量
	uint8	byMaxVol;										//最大音量值
	uint8	byMinVol;										//最小音量值
	uint8	byReserve1;										//保留字节1
	uint8	byReserve2;										//保留字节2
	uint8	byReserve3;										//保留字节3
	uint8	bySerialNum1;									//序列号字节1(序列号最高位的一个字节)
	uint8	bySerialNum2;									//序列号字节2
	uint8	bySerialNum3;									//序列号字节3(序列号最低位的一个字节)
	uint8	byVersionHi;									//版本号高字节
	uint8	byVersionLo;									//版本号低字节 
} STEepromCfgData, *pSTEepromCfgData;
//地址结构体
typedef struct
{
	uint8  	bySecAddr;										//区地址
    uint8  	byRoomAddr;										//房地址  
    uint8  	byBedAddr;										//床地址
} STAddr, *pSTAddr;
//信息指示方地址结构体
typedef struct 
{	    
	STAddr	stAddr;											//指示方地址 
    uint8  	byCallCmd;										//指示的命令
} STIndicationData, *pSTIndicationData;	
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
	uint8				byLedState;							//高4位为红灯状态,低4位为绿灯状态
	uint8				byLedFlashTime;						//灯闪烁时间控制
	uint8				byLedFlashDelay;					//灯闪烁延时
	uint8               byMDLedState;					    //门灯状态	
	uint8				byLcdMsk;							//液晶屏掩码
	uint8				byLcdSecMsk;						//液晶科室掩码
	uint32				uiShortType;						//短整型数据
	uint8				byChar1;							//字符型数据
	uint8				byChar2;							//字符型数据2
	uint8 xdata 		*pbyData;							//字符数据指针
	STBusFreq 			stBusDealFreq;						//总线数据处理存储器
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体 
	STTimerOut			stCH1TimerOut;						//通道1通话超时管理结构体	
	STIndicationData	stIndicationData;					//信息指示数据结构体
	STAddr				stCallAddr;							//主动呼叫方地址 	
	STEepromCfgData		stEepromCfgData;					//系统配置字	 		
} STLocalControl, *pSTLocalControl;

#define OS_TICKS_PER_SEC 200

//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 
#define	Fosc				     18432000		
#define ISR_INC_COUNT		     (Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			     0x0200
#define	INIT_FLAG		         0x5A


/*#define LCD_INIT_ADDR		     0x00000
#define LCD_SEC_MSK_ADDR	     0x01000
#define LCD_SEC_ADDR		     0x02000
#define Serch_Room_ADDR		     0x03000
#define Serch_Room_Page_ADDR     0x14000
#define Modify_Room_Number_ADDR  0x15000
#define Close_EnSoundNote_ADDR	 0x16000 //禁止语音报号地址
#define Serch_Room_TIME_ADDR     0x20000 //查房时间的地址
#define Set_Baud_ADDR            0x3E000 //系统波特率
#define System_Baud_ADDR         0x3F000 //系统波特率*/


//存储芯片25VF032  共4M  0x400000空间
//字库使用到0x3b06b0

//门牌信息使用从0x3c0000开始  还剩下256K空间
#define LCD_INIT_ADDR		     0x3c0000
#define LCD_SEC_MSK_ADDR	     0x3c1000
#define LCD_SEC_ADDR		     0x3c2000
#define Serch_Room_ADDR		     0x3c3000
#define Serch_Room_Page_ADDR     0x3d4000
#define Modify_Room_Number_ADDR  0x3d5000
#define Close_EnSoundNote_ADDR	 0x3d6000 //禁止语音报号地址
#define Serch_Room_TIME_ADDR     0x3d7000 //查房时间的地址
#define Set_Baud_ADDR            0x3e7000 //系统波特率
#define System_Baud_ADDR         0x3e8000 //系统波特率


