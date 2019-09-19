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
#define KEY_UP			0x80								//按键弹起
#define TIMER_OUT		0xa0								//超时命令	
//按键定义	
#define NO_KEY			0xff								//无键按下 	
//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 
#include "SingleBus.h"
#include "Key.h"
#include "IAP.h"


#ifndef _IN_MAIN_
extern void Send_Data(uint8 *Databuf,uint8 l);
extern void delay_nus(unsigned char i);
extern void Delayms(uint16 ms);
#endif


extern uint8 code MyNumber[];


	 
//设备配置字定义
typedef struct
{
	uint8	byInitFlag; 									//初始化参数标志
	uint8  bySelfSecAddr;									//本机区地址
	uint8  bySelfRoomAddr;									//本机房地址
	uint8  bySelfBedAddr;									//本机床地址 
	uint8  byReserve1;										//保留字节1
	uint8  byReserve2;										//保留字节2
	uint8  byReserve3;										//保留字节3
	uint8  byVersionHi;										//版本号高字节
	uint8  byVersionLo;										//版本号低字节   	
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
	uint16				uiShortType;						//短整型数据
	uint8				byChar1;							//字节数据1			
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

