/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:config.h
*文件描述    		:配置头文件
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
//系统头文件
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <stdlib.h>
#include "STC12C56.H"
#define const code


#define DEBUG 1


#define ON 1
#define OFF	0



#ifndef TRUE
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif


#ifndef true
#define true  1
#endif 
#ifndef false
#define false 0
#endif

#ifndef STC12C5A
#define STC12C5A 1
#endif

#ifndef STC90
#define STC90 0
#endif

//主线程命令
#define	BUS0_REC		0x10 								//总线0接收完成
#define BUS0_SND		0x20								//总线0发送完成
#define	BUS1_REC		0x30 								//总线0接收完成
#define BUS1_SND		0x40								//总线0发送完成
#define KEY_DOWN		0x50								//按键按下
#define KEY_UP			0x60								//按键弹起
#define	KEY_ALWAYS		0x70								//键常按下
#define TIMER_OUT		0x80								//超时命令	
#define	UART0_CMD		0x90				                //串口0数据帧命令
#define	UART1_CMD		0xa0				                //串口1数据帧命令


/*#define	SET_KEY_DOWN		0x50							//设置键按下
#define	SET_KEY_ALWAYS		0x70							//设置键长按
#define	SET_KEY_UP			0x60							//设置键弹起

#define DEAL_KEY_DOWN		0x51							//处理键按下
#define DEAL_KEY_ALWAYS		0x71							//处理键长按
#define DEAL_KEY_UP			0x61							//处理键弹起*/


//用于总线1
#define CALL_KEY_DOWN		0x54							//呼叫键按下
#define CALL_KEY_ALWAYS     0X74							//呼叫键长按
#define CALL_KEY_UP    		0X64							//呼叫键弹起


//按键定义
//#define	SET_KEY			0x00								//设置键
#define DEAL_KEY		0x01								//处理键
//define CALL_KEY		0x02								//呼叫键
#define OX_KEY		0X03								//供氧键
#define NO_KEY			0xff								//无键按下


#define NURSE_IN_TIME	350								//护士到位信号有效时间



//灯定义
#define	LED_OFF			0									//灯关闭
#define	LED_ON			1									//灯常亮
#define	LED_FLASH		2									//灯闪烁
#define HAND_LED_ON		0									//手柄灯亮
#define HAND_LED_OFF	1									//手柄灯灭
#define HAND_LED_FLASH	2									//手柄灯闪烁 


#define WIRELESS_SEC		99									//无线区
#define MOVE_FJ				251								//移动分机
#define ADD_BED_FJ			250								//加床分机

#define IrDA_CMD_NURSE_IN   0x01							//红外接口护士到位命令
#define IrDA_CMD_INSPECTION	0x02							//红外接口查房命令

//控制引脚定义
sbit	CGB			= 	P3^7;								//SD控制脚
sbit	CBD			= 	P1^0;								//SA控制脚 

sbit	XTA			= 	P1^3;								//MIC的SA控制
sbit	XTD			=	P1^4;								//MIC的SD控制


sbit    IrDA		=	P3^0;								//红外接收模块输入
sbit	MUS			=	P1^1;								//单总线发送继电器开关，0：正常运行；1：发送关断

sbit	DealKey 	= 	P1^2;								//处置键
sbit	LED_DEAL	= 	P1^2;								//处置键LED灯


sbit 	OX_TIMER_KEY=	P3^5;

sbit  	KDR 		= 	P1^5;								//485输入控制引脚 
sbit    MUT			= 	P1^6;								//功放控制引脚

#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))


#define OpenCGB()		{P3M0 |=   Bin(1,0,0,0,0,0,0,0); P3M1 |=   Bin(1,0,0,0,0,0,0,0); CGB = 1;}	//开漏
#define CloseCGB()		{P3M0 &= (~Bin(1,0,0,0,0,0,0,0));P3M1 &= (~Bin(1,0,0,0,0,0,0,0));CGB = 1;}	//准双向输入
#define OpenCBD()		{P1M0 |=   Bin(0,0,0,0,0,0,0,1); P1M1 |=   Bin(0,0,0,0,0,0,0,1); CBD = 1;}	
#define CloseCBD()		{P1M0 &= (~Bin(0,0,0,0,0,0,0,1));P1M1 &= (~Bin(0,0,0,0,0,0,0,1));CBD = 1;}	

//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 	
#include "SingleBus.h"	
#include "IAP.h"
#include "key.h"
//设备配置字定义
typedef struct
{
	uint8   byInitFlag;										//初始化参数标志
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
	uint8 	byRFSerialNum1;									//输液报警器序列号1
	uint8 	byRFSerialNum2;									//输液报警器序列号2
	uint8 	byRFSerialNum3;									//输液报警器序列号3
	uint8 	byRFSerialNum4;									//输液报警器序列号4
	uint8 	byRFSerialNum5;									//输液报警器序列号5
	uint8 	byRFSerialNum6;									//输液报警器序列号6
	uint8   byBedFjFlag;									//床头分机相关标志
/*#define PatientDischarge	0x01			//bit0:  代表病人出院标志  0:该床位有病人  1:病人已出院
    #define BacklightOn			0x02			//bit1:  代表背光显示状态标志  0:背光关       1:背关开
    #define OxSupply			0x04			//bit2:  代表是否处于供氧计时状态  0：未供氧计时  1：处于供氧计时状态
    #define BedVoicePlay		0x08			//bit3:代表床头分机是否语音报号0:不语音报号　　1:语语报号(默认不报号)							
    #define	bDispMode		0x10			//bit4:床头分机显示模式  0:供氧信息显示模式，1，色块模式
    #define bSecRoom			0x20			//bit5:接收床头卡、清除床头卡是否需要判断区号房号0:不需要判断，1:需要判断
*/
   uint8	byCrc;											//数据校验和
} STEepromCfgData, *pSTEepromCfgData;

//uint8   byBedFjFlag;									//床头分机相关标志
#define PatientDischarge 	0x01			//bit0:  代表病人出院标志  0:该床位有病人  1:病人已出院
#define BacklightOn			0x02			//bit1:  代表背光显示状态标志  0:背光关       1:背关开
#define OxSupply			0x04			//bit2:  代表是否处于供氧计时状态  0：未供氧计时  1：处于供氧计时状态
#define BedVoicePlay		0x08			//bit3:代表床头分机是否语音报号0:不语音报号　　1:语语报号(默认不报号)							
#define	bDispMode			0x10			//bit4:床头分机显示模式  0:供氧信息显示模式，1，色块模式
#define bSecRoom			0x20			//bit5:接收床头卡、清除床头卡是否需要判断区号房号0:不需要判断，1:需要判断



//地址结构体
typedef struct
{
	uint8  	bySndSecAddr;										//主呼方区地址
    uint8  	bySndRoomAddr;										//主呼方房地址  
    uint8  	bySndBedAddr;										//主呼方床地址

	uint8 	byRecSecAddr;									//被呼方区地址
	uint8	byRecRoomAddr;									//被呼方房地址
	uint8 	byRecBedAddr;									//被呼方床地址
} STAddr, *pSTAddr;
//信息指示方地址结构体
typedef struct 
{	    
	STAddr	stAddr;											//指示方地址 
    uint8  	byCallCmd;										//指示的命令
} STIndicationData, *pSTIndicationData;	

//时间结构体
typedef struct
{
	uint8  byYear;
	uint8  byMonth;
	uint8  byDay;
	uint8  byHour;
	uint8  byMin;
	uint8  bySec;
} STTime, *pSTTime;


typedef struct
{
	uint16  uiHour;
	uint8  	byMin;
} STOXTime, *pSTOXTime;


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
	uint8				byLedTimeSet;						//闪烁时间设置值
	uint16  			uiNurseInTime;						//检测护士到位定时时间
	uint8 				byIrDataIntervalTimer;				//红外数据接收过程中字节之间间隔时间
	uint8 				byNumSetTime;						//编号时间
	uint16				uiRFNumSetTime;						//输液报警器对码时间
	uint16				uiShortType;						//短整型数据
	uint8				byChar1;							//字符型数据
	uint8	xdata 		*pbyData;							//字符数据指针
	STBusFreq 			stBusDealFreq;						//总线数据处理存储器
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体 
	STTimerOut			stCH1Timerout;						//通道1通话超时管理结构体	
	STIndicationData	stIndicationData;					//信息指示数据结构体
	STAddr				stCallAddr;							//主动呼叫方地址 	
	STEepromCfgData		stEepromCfgData;					//系统配置字
	STTime 				stTime;
	uint16 				uiWaitAckTimeSupplyOxEnd;				//供氧计时结束等待应答信号时间
	uint16				uiWaitAckTimeSupplyOxTotal;
	uint16 				uiBus1TestTime;						//单总线1测试时间 6000(2分钟)
	uint8 				byBus1AnswerTime;					//等待单总线1应答时间 250(5秒钟)
} STLocalControl, *pSTLocalControl;
//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 
#define	Fosc				18432000
#define Fbus				184320
	
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
			
#define	IAP0_ADDR			0x0000	//第0  扇区:每扇区512字节
#define	IAP1_ADDR			0x0200	//第1  扇区:每扇区512字节
#define	IAP2_ADDR			0x0400	//第2  扇区:每扇区512字节
#define	IAP3_ADDR			0x0600	//第3  扇区:每扇区512字节

#define INIT_FLAG           0x5A


#ifndef _IN_MAIN_

extern bit bNumSeting;
extern bit bRFNumSeting;
extern bit bOxTimerKeyState;

extern bit bEnableOxTimer;				
extern bit bWillSndOxSupplyStart;
extern bit bWillSndTotalOx;						//将发送供氧计时信息
extern bit bCloseCGB;

extern bit bBus1Answer;					//接收单总线1的应答标志
extern bit bOxSupplyState;				//正处于供氧计时状态

extern bit bFourByteRec;
extern bit bSixByteRec;



extern uint8 xdata IrDataPosit;
extern uint8 xdata RFSerial[6];

extern uint8 xdata BusLowDTime;
extern uint8 xdata BusHighDTime;


extern uint8 code byVersionHi;
extern uint8 code byVersionLo;

extern void Send_Data(uint8 *Databuf,uint8 xdata l);
extern void Send_Data_Byte(uint8 SendData);
extern void UserTickTimer(void);
extern void OxTimerTreat(void);
extern void Delayms(uint16 ms);


#endif
