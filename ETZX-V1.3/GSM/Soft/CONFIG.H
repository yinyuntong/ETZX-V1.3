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


#define uint8 unsigned char
#define uint16  unsigned int

#define uchar unsigned char
#define uint  unsigned int

#define LED_ON	0
#define	LED_OFF 1

#define DEBUG 1

#define TEL_NUM_LEN 11
#define UART0_REC_LEN 100


#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))

#define WIRELESS_SEC				99								//无线区
#define ADD_BED_FJ_ADDR				250								//加床分机
#define MOVE_FJ_ADDR				251								//移动分机
#define GSM_MODUL_ADDR				252
#define YHFJ_ADDR					253
#define GLZJ_ADDR					254

//包含头文件

#include "SingleBus.h"
#include "IAP.h"
#include "GSM.h"

sbit SIM_STATE			=	P3^3;			//sim卡状态
sbit SYS_STATE_LED		=	P3^4;
sbit GSM_STATE_LED		=	P3^5;			
sbit GSM_PWR_SW	=	P1^1;
sbit GSM_ON_OFF	=	P1^2;
sbit GSM_RST	=	P1^3;

sbit XTA		=	P1^6;
sbit XTD		=	P1^7;


//时间结构体
typedef struct
{
	uint8  byYear;
	uint8  byMonth;
	uint8  byDay;
	uint8  byHour;
	uint8  byMin;
	uint8  bySec;
}STTime, *pSTTime;

	 
//设备配置字定义
typedef struct
{
	uint8	byInitFlag; 									//初始化参数标志
	uint8  	bySelfSecAddr;									//本机区地址
	uint8  	bySelfRoomAddr;									//本机房地址
	uint8  	bySelfBedAddr;									//本机床地址 
	uint8	bySerialNum1;									//序列号字节1(序列号最高位的一个字节)
	uint8	bySerialNum2;									//序列号字节2
	uint8	bySerialNum3;									//序列号字节3(序列号最低位的一个字节)
	uint8	byVersionHi;									//版本号高字节
	uint8	byVersionLo;									//版本号低字节 
	uint8   byTelNum[TEL_NUM_LEN+1];						//电话号码	第1位在byTelNum[0]
	uint8   byTelNumValidLen;								//电话号码有效长度
	uint8	byTelFuncState;									//移动电话功能状态
	uint8	byRingTime;										//振铃时间
	uint8	byTalkTime;										//通话时间
	uint8	byBroadTime;									//广播时间	
}STEepromCfgData, *pSTEepromCfgData; 


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
	STBusFreq			stIndicationData;					//信息指示
	STBusFreq           stCallAddr;							//
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体
	STTimerOut			stCH1TimerOut;
	STEepromCfgData		stEepromCfgData;					//系统配置字
	STTime				stTime;
	uint8 				byLedFlashTime;
	uint8				byLedTimeSet;
} STLocalControl, *pSTLocalControl;	  	
//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为PCACLK0   12分频 
#define	Fosc				18432000
#define Fbus				184320

#define OS_TICKS_PER_SEC        50         /* 声明1秒系统系统节拍数                                    */


#define ISR_INC_COUNT		(Fosc/12/OS_TICKS_PER_SEC)	//定时时间20ms		
#define	IAP_ADDR			0x0000		//STC12C5624内的E2只能是数据E2
#define TEL_NUM_ADDR			0x0200
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN_
extern void Send_Data(uint8 *Databuf,uint8 l);
extern void UartSendByte(uint8 Data);
extern void UartSendString(uint8 * Databuf);
extern void delay_nus(unsigned char i);
extern void Delayms(uint16 ms);
extern uint8 UartRecTreat(uint8* pCmd,uint8 byCmdLen);

extern STLocalControl xdata stLocalControl;
extern uint8 xdata TelNum[];
extern bit bUartRecFinished;
extern uint8 xdata RePeat;

#endif

