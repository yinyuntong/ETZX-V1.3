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
#include "STC12C56.H"
#define const code 
#ifndef TRUE
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif
//主线程命令
#define	BUS0_REC		0x10 								//总线0接收完成
#define BUS0_SND		0x20								//总线0发送完成	 
#define KEY_DOWN		0x30								//按键按下
#define TIMER_OUT		0x40								//超时命令	
//------------------------------------------------------------------------
#define IR_REC		    0x50								//红外数据处理	
//-------------------------------------------------------------------------
//按键定义
#define	SET_KEY			0x00								//设置键
#define NO_KEY			0xff								//无键按下	
//灯定义
sbit	LED_RED		= 	P1^6;								//红色灯
sbit	LED_GREEN	=	P1^7;								//绿色灯
sbit    RST_BUS     =   P1^3;
#define LED_ON			0
#define LED_OFF			1
#define LED_FLASH		2
//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h"
#include "SingleBus.h"
#include "Key.h"	
#include "IAP.h"
#include "IR.h"
//设备配置字定义
typedef struct
{
	uint8   byInitFlag;
	uint8  	bySelfSecAddr;									//本机区地址
	uint8  	bySelfRoomAddr;									//本机房地址
	uint8  	bySelfBedAddr;									//本机床地址
	uint8	byFlag;											//上电状态标志
	uint8   byPrio;											//优先级
	uint8	byReserve1;										//保留字节1
	uint8	byReserve2;										//保留字节2
	uint8	byReserve3;										//保留字节3
	uint8	bySerialNum1;									//序列号字节1(序列号最高位的一个字节)
	uint8	bySerialNum2;									//序列号字节2
	uint8	bySerialNum3;									//序列号字节3(序列号最低位的一个字节)
	uint8	byVersionHi;									//版本号高字节
	uint8	byVersionLo;									//版本号低字节 
	uint8 	byCrc;											//校验和
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
	uint8				byLedState;							//灯状态
	uint8				byLedDelay;							//灯延时
	uint8				byLedTime;							//时间设定
	uint16				uiShortType;						//短整型数据
	uint8				byChar;								//字符型数据
	uint8*				pbyData;							//字符数据指针
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体 
	STEepromCfgData		stEepromCfgData;					//系统配置字
	STBusFreq 			stBusDealFreq;						//总线数据处理存储器	  		
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


extern unsigned char code ParameterData[];


#ifndef _IN_MAIN_
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[]; 
extern STLocalControl 	xdata stLocalControl; 
extern void Send_Data_Byte(uint8 SendData);
extern void Send_Data(uint8 *Databuf,uint8 xdata l);

#endif
