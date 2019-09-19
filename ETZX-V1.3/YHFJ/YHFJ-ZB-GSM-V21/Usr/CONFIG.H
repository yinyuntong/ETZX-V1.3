/*
************************Copyright(c)************************
*	  			   湖南一特股份有限公司
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
#include "STC12C5A.H"
#define const code 


#define DEBUG	1

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

#define 	ADD_BED_FJ			250
#define 	MOVE_FJ				251
#define 	GSM_MODULE_ADDR 	252
#define 	YHFJ_ROOM_ADDR		253


//主线程命令
#define	BUS0_REC		0x10 								//总线0接收完成
#define BUS0_SND		0x20								//总线0发送完成
#define	BUS1_REC		0x30 								//总线0接收完成
#define BUS1_SND		0x40								//总线0发送完成	 
#define TIMER_OUT		0x50								//超时命令	
#define KEY_DOWN		0x60								//听筒放下
#define KEY_UP			0x70								//听筒拿起
#define	UART0_CMD		0x80				                //串口0数据帧命令

//控制引脚定义
sbit	led0		= 	P5^3;								//听筒LED灯控制脚
sbit	MC_CNT			= 	P1^5;								//听筒MIC控制脚
sbit	MCN			= 	P1^6;								//面板	MIC控制脚
sbit	MC_CTRL		= 	P1^7;								//面板	MIC开关

sbit	XTA			= 	P1^4;								//MIC的SA控制
sbit	XTD			=	P1^3;								//MIC的SD控制
sbit	HOOK		= 	P4^3;								//听筒摘起信号
sbit	MUS			=	P3^5;								//提示音	


sbit 	VL0			=	P2^0;
sbit 	VL1			=	P2^1;
sbit 	VL2			=	P2^2;
sbit 	VL3			=	P2^3;
sbit 	VL4			=	P2^4;
sbit 	VL5			=	P2^5;
sbit 	VL6			=	P2^6;
sbit 	VL7			=	P2^7;


sbit	PW			= 	P0^3;								//34018片选脚
sbit	CGB			= 	P0^5;								//SD控制脚
sbit	CBD			= 	P0^4;								//SA控制脚 
sbit	KM			= 	P0^6;								
sbit  	KDR 		= 	P0^7;								//485控制引脚 

sbit 	PW2			=	P5^2;								//高电平禁止

#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))

#define OpenCGB()		{P0M0 |= Bin(0,0,1,0,0,0,0,0);P0M1 |= Bin(0,0,1,0,0,0,0,0);CGB = 1;}	
#define CloseCGB()		{P0M0 &= (~Bin(0,0,1,0,0,0,0,0));P0M1 &= (~Bin(0,0,1,0,0,0,0,0));CGB = 0;}	
#define OpenCBD()		{P0M0 |= Bin(0,0,0,1,0,0,0,0);P0M1 |= Bin(0,0,0,1,0,0,0,0);CBD = 1;}	
#define CloseCBD()		{P0M0 &= (~Bin(0,0,0,1,0,0,0,0));P0M1 &= (~Bin(0,0,0,1,0,0,0,0));CBD = 0;}


#define OpenHandleMIC()	{ MC_CNT=0; }

#define CloseHandleMIC(){ MC_CNT=1;}


#define OpenPanelMIC()	{ MC_CNT=1;  /*P1M0 |= Bin(0,1,0,0,0,0,0,0);P1M1 |= Bin(0,1,0,0,0,0,0,0);MCN= 1;   MCN悬空*/	MC_CTRL= 1; /*MC_CTRL开漏输出1*/}
						
#define ClosePanelMIC()	{ /*P1M0 &= (~Bin(0,1,0,0,0,0,0,0));P1M1 &= (~Bin(0,1,0,0,0,0,0,0));MCN= 0;  MCN准双向口,输出0*/   MC_CTRL= 0; /*MC_CTRL拉到地*/}


#define Vol0 	0x00
#define Vol1   	0x01
#define Vol2   	0x03
#define Vol3  	0x07
#define Vol4	0x0f
#define Vol5   	0x1f
#define Vol6   	0x3f
#define Vol7   	0x7f
#define VolMax  0xff


#define MIN_VOLUMN_VALUE 0x00
#define MAX_VOLUMN_VALUE 0xff



#define SetSPKVol0()   {VL0=0;}

#define SetSPKVol1()   {P2=0X01;}

#define SetSPKVol2()   {P2=0X03;}

#define SetSPKVol3()   {P2=0X07;}

#define SetSPKVol4()   {P2=0X0f;}

#define SetSPKVol5()   {P2=0X1f;}

#define SetSPKVol6()   {P2=0X3f;}

#define SetSPKVol7()   {P2=0X7f;}


#define SetSPKVolMax() {P2=0XFF;} 	//喇叭最大音量


#define KEY_0   0
#define KEY_1   1
#define KEY_2   2
#define KEY_3   3
#define KEY_4   4
#define KEY_5   5
#define KEY_6   6
#define KEY_7   7
#define KEY_8   8
#define KEY_9   9
#define KEY_CLEAR   10
#define KEY_ENTER   11
#define KEY_VOICEINC   12
#define KEY_VOICEDEC   13
#define KEY_ADDBED     14
#define KEY_SEC   	  15
#define KEY_ROOM		  16
#define KEY_MS		  17
#define KEY_MR    	18
#define KEY_SEARCH   19
#define KEY_CALCULATE  20
#define KEY_MODE		  21
#define KEY_BROAD	22
#define KEY_MUSIC	23
#define KEY_RESERVE 24

//键值缓存区大小
#define MAX_KEY_SIZE	20
#define GET_SEC_ADDR	0
#define GET_ROOM_ADDR	1
#define GET_BED_ADDR	2 
//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 	
#include "SingleBus.h"	 
#include "IAP.h"
#include "Key.h" 

//时间段结构体
typedef struct
{
	uint8 	byStartHour;
	uint8   byStartMin;
	uint8	byEndHour;
	uint8 	byEndMin;
}STTimerZone;

//设备配置字定义
typedef struct
{
	uint8   byInitFlag;										//初始化标志
	uint8  	bySelfSecAddr;									//本机区地址
	uint8  	bySelfRoomAddr;									//本机房地址
	uint8  	bySelfBedAddr;									//本机床地址
	uint8  	byEnable1;										//允许标志1
	uint8  	byEnable2;										//允许标志2	
	uint8   byPrio;											//优先级
	uint8	byRingTime;										//振铃时间
	uint8	byTalkTime;										//通话时间
	uint8	byBroadTime;									//广播时间
	uint8	byListenDelay;									//接听延时时间(40ms的基本单位)
	uint8	byVolumStep;									//音量调节步长
	uint8	byHDwnRingVol;									//免提振铃提示音量 			//0
	uint8	byHUpRingVol;									//摘机振铃提示音量			//1
	uint8	byCH0HDwnTalkedVol;								//通道0免提被呼通话音量		//2
	uint8	byCH1HDwnTalkedVol;								//通道1免提被呼通话音量		//3
	uint8	byCH0HUpTalkedVol;								//通道0摘机被呼通话音量		//4
	uint8	byCH1HUpTalkedVol;								//通道1摘机被呼通话音量		//5
	uint8	byHDwnBroadVol;									//免提广播音量				//6
	uint8	byHUpBroadVol;									//摘机广播音量				//7
	uint8	byHDwnNoteVol;									//免提提示音量 				//8
	uint8	byHUpNoteVol;									//摘机提示音量 				//9
	uint8	byHDwnMusicVol;									//免提背景音乐音量			//10
	uint8	byHUpMusicVol;									//摘机背景音乐音量 			//11
	uint8	byCH0HDwnTalkVol;								//通道0免提主动通话音量		//12
	uint8	byCH1HDwnTalkVol;								//通道1免提主动通话音量		//13
	uint8	byCH0HUpTalkVol;								//通道0摘机主动通话音量		//14
	uint8	byCH1HUpTalkVol;								//通道1摘机主动通话音量		//15
	uint8	byHDwnSelfRingVol;								//本机免提振铃提示音量		//16
	uint8	byHUpSelfRingVol;								//本机摘机振铃提示音量		//17
	uint8	byMaxVol;										//最大音量值
	uint8	byMinVol;										//最小音量值
	uint8	byReserve1;										//保留字节1
	uint8	byReserve2;										//保留字节2
	uint8	byReserve3;										//保留字节3
	uint8	bySerialNum1;									//序列号字节1(序列号最高位的一个字节)
	uint8	bySerialNum2;									//序列号字节2
	uint8	bySerialNum3;									//序列号字节3(序列号最低位的一个字节)
	STTimerZone 		stBLTimeZone;						//背光关时间段
	STTimerZone			stVoiceTimeZone;					// 小音量时段时间段
	uint8 	byMicroVoiceVal;								// 小音量值
	uint8	byVersionHi;									//版本号高字节
	uint8	byVersionLo;									//版本号低字节 

	uint8   byCrc;											//校验和
} STEepromCfgData, *pSTEepromCfgData;
//许可标志掩码
#define		EN_SICKROOM_BROADCAST		0x0001				//许可病区广播
#define		EN_OFFICE_BROADCAST			0x0002				//许可办公区广播
#define		EN_ALL_BROADCAST			0x0004				//许可全区广播
#define		EN_SOUND_REMIND				0x0008				//许可语音提示
#define		EN_SYS_RING					0x0010				//许可外部振铃
#define		EN_AUTO_LISTEN				0x0020				//许可自动接听
#define		EN_VOICE_REMIND				0x0040				//许可声音提示
#define		EN_HANDLER_DEAL				0x0080				//许可手柄处理
#define		EN_INFUSION_TREAT			0x0100				//许可输液呼叫处理
#define		EN_SERVICE_TREAT			0x0200				//许可服务呼叫处理
#define		EN_EMERGENCY_TREAT			0x0400				//许可紧急呼叫处理
#define		EN_HELP_TREAT				0x0800				//许可求援呼叫处理
#define		EN_CALL_TREAT				0x1000				//许可呼叫处理
#define		EN_MUSIC					0x2000				//许可背景音乐播放
//地址结构体
typedef struct
{
	uint8  	bySndSecAddr;										//主呼方区地址
    uint8  	bySndRoomAddr;										//主呼方房地址  
    uint8  	bySndBedAddr;										//主呼方床地址
	uint8  	byRecSecAddr;										//被呼方区地址
    uint8  	byRecRoomAddr;										//被呼方房地址  
    uint8  	byRecBedAddr;										//被呼方床地址    
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
	uint16				uiShortType;						//短整形数据
	uint8				byChar1;							//字符型形数据
	uint8*				pbyData;							//数据指针
	uint8				byKeyDownState[4];					//按键按下状态	
	uint8				byKeyValue[MAX_KEY_SIZE];			//记录的按键数字					
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体 
	STTimerOut			stCH1Timerout;						//通道1通话超时管理结构体	
	STIndicationData	stIndicationData;					//信息指示数据结构体
	STAddr				stCallAddr;							//主动呼叫方地址 	
	STEepromCfgData		stEepromCfgData;					//系统配置字
	STBusFreq			stKeyCallAddr;						//键盘呼叫的地址
	STBusFreq 			stBusDealFreq;						//总线数据处理存储器 
	//uint8				byCallDealTimerout;					//总线处理发送超时
	uint8				byNumSetTime;
	STTime				stTime;								//当前时间
	
} STLocalControl, *pSTLocalControl;
//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 
#define	Fosc				18432000L
#define Fbus				184320
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP0_ADDR			0x0000		//STC12C5A60S2内的E2只能是数据E2
#define IAP1_ADDR			0x0200
#define INIT_FLAG           0x5a

#ifndef _IN_MAIN_
extern void uart_send(unsigned char *SDATA,unsigned char length);
extern void uart_send_byte(unsigned char SDATA);
#endif

