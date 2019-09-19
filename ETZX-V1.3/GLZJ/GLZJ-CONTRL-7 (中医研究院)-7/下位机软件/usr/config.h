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
#define DEBUG 1

#ifndef STC12C5A
#define STC12C5A 1
#endif

#ifndef STC90
#define STC90	0
#endif

//#define SPK_ROOM 1			//语音报号时报房号
#define SPK_ROOM 0			//语音报号时不报房号


/*操作系统头文件*/
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <math.h>                                                                                                          
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STC12C5A.h"


#define int8 char
#define uint8 unsigned char
#define int16 int
#define uint16 unsigned int
#define int32 long int
#define uint32 unsigned long int 

/*应用头文件*/
#include "SingleBus.h"
#include "I2C.h"
#include "PCF8563.h"
#include "Uart.h"
#include "key.h"
#include "iap.h"
#include "avrx.h"

//各端口定义
sbit POWER_EN	=	P1^7;	// OUT  1:  24V电源打开

sbit POWER_SW	=	P4^3;	//IN:    机箱上的电源开关

sbit INT6	=	P3^3;

sbit KBC	=	P5^0;
sbit KBD	=	P5^1;
sbit KGB	=	P2^5;



sbit MP3_RST	=P4^6;

sbit LED_STATE	=P0^7;		//状态指示灯OUT   0:灯亮
sbit LED_POWER	=P0^6;		//电源指示灯OUT   0:灯亮

sbit SDA	=	P0^5;
sbit SCL	=	P0^4;

sbit RE0	=	P0^1;
sbit DE0	=	P0^2;

sbit KV		=	P1^1;		//单片机发出蜂鸣声
sbit SS		=	P1^4;

sbit GB_FZ	=   P5^3;		//用于广播时SD线上的负载平衡


#define const code 


/*系统定义*/
#ifndef 	true
#define   	true      			1
#endif
#ifndef		false
#define   	false 				0
#endif


#ifndef 	TRUE
#define   	TRUE      			1
#endif
#ifndef		FALSE
#define   	FALSE 				0
#endif


#define		ISD_TIME			200


#define 	RE_SND_TIMES	3

#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))



extern void LedStateFlash(void);


//本机配置字结构体
typedef struct _STEepromCfgData
{
	uint8  byInitFlag;										//初始化标志
    uint8  bySelfSecAddr;									//区地址
    uint8  bySelfRoomAddr;									//房地址  
    uint8  bySelfBedAddr;									//床地址
    uint16 uiRs485Brt;										//RS485波特率
	uint8  byCrc;											//校验和
} STEepromCfgData, *pSTEepromCfgData;
#define		VOICE_DATA_SIZE		16
//语音报号控制体
typedef struct _STVoicePlay 
{
    uint8  byVoiceData[VOICE_DATA_SIZE];					//需要播放的语音数据
    uint8  byVoicePosition;									//播放语音位置
    uint8  byVoiceSize;										//需要播放的语音长度
    uint8  byISD1700State;									//语音芯片状态   
} STVoicePlay, *pSTVoicePlay;
//双向链表节点
typedef struct _STNod
{
    uint8  byCallSecAddr;									//呼叫方区地址
    uint8  byCallRoomAddr;									//呼叫方房地址
    uint8  byCallBedAddr;									//呼叫方床地址
    uint8  byCallPriority;									//呼叫优先级
    struct _STNod xdata *pstPrev;
    struct _STNod xdata *pstNext;    
} STNod, *pSTNod;
//双向链表结构体
typedef struct _STPointer
{
    struct _STNod xdata *pstHead;									//链表头节点
    struct _STNod xdata *pstTail;									//链表尾节点    
} STPointer, *pSTPointer;
//信息指示方结构体
typedef struct _STIndicationData
{
    uint8  byIndicationSecAddr;								//指示方区地址
    uint8  byIndicationRoomAddr;							//指示方房地址  
    uint8  byIndicationBedAddr;								//指示方床地址 
    uint8  byCallCmd;										//指示的命令
    uint8  byState;											//该呼叫信息的状态
    struct _STNod xdata *pstNext;    								//下一条呼叫信息指针
} STIndicationData, *pSTIndicationData;
//超时管理结构体
typedef struct _STTimerout
{	
	uint8  byTimerOutSet;									//一次超时时间设置(20ms单位)
	uint8  byTimerOut;										//一次超时时间(20ms单位)
	uint8  byTimerOutCount;									//允许超时次数	
} STTimerout, *pSTTimerout;
//地址结构体
typedef struct
{
	uint8  bySndSecAddr;										//发送方区地址
    uint8  bySndRoomAddr;										//发送方房地址  
    uint8  bySndBedAddr;										//发送方床地址
    uint8  byRecSecAddr;
	uint8  byRecRoomAddr;
	uint8  byRecBedAddr;

    uint8  byCmd;											//原命令
} STAddr, *pSTAddr;


//地址结构体
typedef struct
{
	uint8  bySecAddr;										//区地址
    uint8  byRoomAddr;										//房地址  
    uint8  byBedAddr;										//床地址									
} STAddr_S, *pSTAddr_S;


typedef struct queue
{
	uint16 queuesize;		//数组的大小
	uint16 head,tail;		//队列的头和尾下标
	uint8 *q;				//数组头指针
	
}Queue;

/*全局控制结构体*/
typedef struct
{
uint8 				byChar;
uint8				byWaitAckTimeLanding;				//上电登记时等待PC机的应答时间
uint16				uiDevState; 						//设备当前所处的状态
uint16				uiDetectPcTime;						//侦听PC机间隔时间	
uint8				byVoicePlayTime;					//报号定时器
STBusFreq			stBusFreq;							//单总线数据处理结构体	
STTimerout			stCH0TimerOut;						//超时管理结构体
STTimerout			stCH1TimerOut;						//通道1通话超时管理结构体	
uint8				byWaitCommAckDTime;						//通用等待定时时间
uint8 				byUsart1HandingDTime;				//串口1正在处理定时时间
uint8 				byUsart1SdResultDTime;				//串口1发送结果码延时时间
STPointer			stCallPointer;						//呼叫队列双向链表
STIndicationData	stIndicationData;					//信息指示数据结构体
STVoicePlay 		stVoicePlay;						//语音播放控制
STAddr				stCallAddr; 						//通道0主动呼叫方地址
STAddr				stCH1CallAddr; 						//通道1主动呼叫方地址
STAddr_S			stRS485Addr;						//RS485数据接收方地址
uint8				byWaitRS485AckDTime;				//通用等待定时时间
STEepromCfgData 	stEepromCfgData;					//本机配置字结构体
uint8				byReSndBrtTimes;					//重复发送设置波特率的次数
uint8				byReSndRs485RoomTimes;				//重复发送床头分机接收RS485信息是否需要判断房号的次数
uint8 				byReSndBedVoiPlaTimes;				//重复发送语音报号时床头分机是否语音报号的次数
STAddr_S			stCommAddr;							//通用地址
} STLocalControl, *pSTLocalControl;


//主线程消息类型定义
#define		MAIN_CMD_BUS0_REC			0x10				//总线0接收数据命令
#define 	MAIN_CMD_BUS0_SND			0x20				//总线0发送完数据命令
#define		MAIN_CMD_BUS1_REC			0x30 				//总线1接收完成
#define 	MAIN_CMD_BUS1_SND			0x40				//总线1发送完成	 
#define 	MAIN_CMD_TIMEROUT			0x50				//超时命令	
#define 	KEY_DOWN					0x60				//键按下命令
#define 	KEY_UP						0x70				//键弹起命令



//设备状态定义
#define		FLAG_LANDING				0x8000 				//上电登记状态
#define     FLAG_CHANNEL_CHANGE_EXIST    0x4000				//有通道切换在总线发送队列中
#define 	FLAG_WAIT_COMM_ACK			0x2000				//正在等待单总线上的通用应答标志
#define		FLAG_STOP_EXIST				0x1000				//有停止呼叫指示在总线发送队列中
#define		FLAG_TALK1					0x0800				//通道1通话标志
#define		FLAG_NOTE					0x0400				//语音提示状态
#define		FLAG_MUSIC					0x0200				//背音乐播放状态
#define		FLAG_INDICATION_EXIST		0x0100				//有呼叫指示在总线发送队列中
#define		FLAG_BUSY					0x0080				//系统忙标志
#define		FLAG_WAIT_ACK				0x0040				//等待应答标志
#define		FLAG_WAIT_LISTEN			0x0020				//等待接听标志
#define		FLAG_TALK					0x0010				//通道0通话标志
#define		FLAG_BROADCAST				0x0008				//广播标志
#define		FLAG_INDICATION				0x0004				//分机呼叫指示标志
#define 	FLAG_WAIT_RS485_ACK			0x0002				//等待RS485 总线应答


//#define 	PC_ADDR1					0X00				//PC机地址1
#define 	PC_ADDR2					0X00				//PC机地址2
#define 	PC_ADDR3					0X00				//PC机地址3

#define 	GLZJ_ADDR2					0xfe				//管理主机地址2
#define     GLZJ_ADDR3					0X00				//管理主机地址3

#define 	ADD_BED_FJ			250							//加床分机房号地址	//0xFA
#define 	MOVE_FJ				251							//移动分机房号地址	//0xFB


#define 	YHFJ_ADDR2				0XFD					//医护分机房号地址
#define 	ZLDIS_ADDR2				0XF0					//走廊显示屏房号地址

/*定义CPU时钟(HZ)*/
#define 	CPU_CLK 		18432000L 
#define OS_TICKS_PER_SEC        50        /* 声明1秒系统系统节拍数                                    */


/*定义定时器初始化值*/
#if STC90==TRUE
#define ISR_INC_COUNT		(65536-(CPU_CLK/12/OS_TICKS_PER_SEC))	//使用T2时用
#else
#define ISR_INC_COUNT		(CPU_CLK/12/OS_TICKS_PER_SEC)		//使用PCA时用 12分频  //每20ms中断1次
#endif

#define	IAP0_ADDR			0x0000		//STC12C5A60S2内的E2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN_
extern STLocalControl xdata	 stLocalControl; 
extern uint8 			xdata 	byMallocMem[];

extern bit bPCState;
extern bit bLedState;
extern bit bPowerEnState;


extern void Delayms(uint16 ms);
extern void SaveParameter(uint16 addr);


#endif


