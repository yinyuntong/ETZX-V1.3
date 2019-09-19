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
#include "STC12C5A.H"
#define const code

#define DEBUG 0

#define MAX_COM	 1

#define MAX_KEY_SIZE 3

#define MAX_CALL_COUNT 20

#define NO_CHANGE_DATA 0xfe
 
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


#define DIS_FRAM_MEM0  	0x00
#define DIS_FRAM_MEM1	0x01
#define DIS_FRAM_MEM2	0x02


#define YHFJ_ROOM_ADDR 253

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long 

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
#define EPH1660_POSITION_CMD 0xb0							//EPH1660触摸点坐标位置命令

//计算器操作类型宏定义
#define ADD_OPER	1	//+
#define SUB_OPER	2 	//-
#define MUL_OPER    3	//*
#define DIV_OPER	4	// /


//键值定义
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


#define MAX_VOLUMN_VALUE 8
#define MIN_VOLUMN_VALUE 0


#define BED_CARD_TYPE			0						//床头卡信息类型
#define BED_TYPE				1						//床头信息类型
#define WARD_SEC_TYPE			3						//病区信息界面信息类型
#define DOOR_SEC_TYPE			4						//门区信息界面信息类型
#define AISLE_SEC_TYPE			5						//走廊显示屏信息类型
#define COST_LIST_TYPE			6						//费用清单信息类型
#define MEDICAL_LIST_TYPE		7						//药品清单信息类型
#define PRESCRIPTION_TYPE		8						//药嘱
#define PROMPT_INFO_TYPE		9						//提示信息

#define POWER_UP_FACE		1							//上电界面
#define MAIN_MENU_FACE	2
#define CALL_SUCCESS_FACE		3
#define TALK_FACE				4
#define INFO_INDICATION_FACE	5
#define COST_LIST_FACE			6
#define MEDICAL_LIST_FACE		7
#define PRESCRIPTION_FACE		8
#define PROMPT_INFO_FACE		9
#define SEC_INFO_FACE			10
#define BROAD_FACE				11
#define FUSION_CALL_FACE		12
#define SERVICE_CALL_FACE		13
#define EMERGENCY_CALL_FACE		14
#define HELP_CALL_FACE			15

#define START_NUM_SET_FACE		17
#define NUM_SET_OK_FACE			18

#define TIME_SET_FACE			20						//时间设置界面
#define NUMBER_SET_FACE			21						//设备编号界面
#define VOICE_TIMER_SET_FACE    23						//小音量时段设置界面
#define CALCULATE_FACE			24						//计算器界面
#define BACK_LIGHT_TIMER_SET_FACE 25					//背关开关时段设置界面
#define SEC_INFO_SET_FACE 		26						//病区信息设置界面
#define SELF_NUMBER_SET_FACE	27						//本机号码设置界面
#define TOUCH_ADJUST_FACE		28						//触摸屏校准界面

#define BED_FJ_NUMBER_SET_FACE  30
#define WC_FJ_NUMBER_SET_FACE 	31						//卫浴分机编号界面
#define MOVE_FJ_NUMBER_SET_FACE 32						//移动分机编号界面
#define ADD_FJ_NUMBER_SET_FACE  33						//加床分机编号界面

#define NUMBER_PW_INPUT_FACE    34						//编号密码输入界面

#define MAX_VOL_SET_FACE		35						//最大音量设置界面

//时间设置输入框宏定义
#define INPUT_YEAR				01
#define INPUT_MONTH				02
#define INPUT_DAY				03
#define INPUT_HOUR				04
#define INPUT_MIN				05
#define INPUT_SEC				06

//在线编号输入框宏定义
#define INPUT_SECTION			01
#define INPUT_ROOM				02
#define INPUT_BED				03
#define INPUT_BED_START			04
#define INPUT_BED_END			05


//小音段时段输入框宏定义
#define INPUT_VOICE_START_HOUR  01
#define INPUT_VOICE_START_MIN	02
#define INPUT_VOICE_END_HOUR	03
#define INPUT_VOICE_END_MIN		04
#define INPUT_VOICE_VALUE		05

//背光关时段输入框宏定义
#define INPUT_BL_START_HOUR  	01
#define INPUT_BL_START_MIN		02
#define INPUT_BL_END_HOUR		03
#define INPUT_BL_END_MIN		04


//最大音量设置时输入框宏定义
#define INPUT_PLAY_VOL_MAX  	01
#define INPUT_PLAY_VOL_MIN		02
#define INPUT_TALK_VOL_MAX		03
#define INPUT_TALK_VOL_MIN		04


//计算器界面输入框宏定义
#define INPUT_FIRST_OPER		01
#define INPUT_SECOND_OPER		02
#define INPUT_RESULT			03

//病区信息统计界面输入框宏定义
#define INPUT_WARD_SEC			00
#define INPUT_PATIENT_SUM		01
#define INPUT_TERMINAL_PATIENT_SUM 02
#define INPUT_BED_SUM			03		//床位数
#define INPUT_EMPTY_BED_SUM		04		//空床数
#define INPUT_ADMISSION_SUM		05		//入院人数
#define INPUT_DISCHARGE_SUM		06		//出院人数
#define INPUT_SWITCH_IN_SUM		7		//转入人数
#define INPUT_SWITCH_OUT_SUM	8		//转出人数
#define INPUT_PRIMARY_CARE_SUM	9		//一级护理人数
#define INPUT_OXYGEN_SUPPLY_SUM 10		//吸供人数


//触摸屏校准
#define INPUT_TOPLEFT 			01
#define INPUT_TOPRIGHT			02
#define INPUT_BOTTONRIGHT		03
#define INPUT_BOTTONLEFT		04


#define WARD_FACE_DATA_START_ADDR		0x000000					//医护分机界面中的数据在在铁电中的起始位置

#define WARD_FACE_DATA_LEN 17


#define LINE_FEED				10				//换行符
#define FORM_FEED				12				//换页符


#define MAX_PAGE_NUMBER			10				//最大页数




//控制引脚定义
sbit	MOSI		=	P1^5;								//SPI 主出从入口
sbit	MISO		=	P1^6;								//SPI 从出主入口
sbit	SCLK		=	P1^7;								//SPI  时钟线


sbit  	EPH1660_CS		= 	P1^4;
sbit  	RESETB  	= 	P4^7;
sbit  	SPI_CMD 	= 	P5^2;
sbit  	WKICOM  	= 	P1^3;
sbit  	WKO	  		= 	P3^2;

sbit 	ADS7843_CS	=	P4^4;								//触摸芯片片选
sbit	TPBUSY		=	P1^2;
sbit 	TPIRQ		=   P1^3;


sbit 	GT23L_CS	=	P4^5;								//字库芯片片选


sbit 	SST25VF_CS	=	P4^1;								//铁电存储器片选

sbit  BL_SW			=P3^5;
sbit  SSD1963_CS  = P1^0;
sbit  SSD1963_RST = P1^1;
sbit  SSD1963_CD  = P5^3;
sbit  SSD1963_TE  = P4^2;
sbit  SSD1963_WR  = P3^6;
sbit  SSD1963_RD  = P3^7;

sbit 	DATA0		=	P0^0;
sbit	DATA1		=	P0^1;
sbit 	DATA2		=	P0^2;
sbit	DATA3		=	P0^3;
sbit 	DATA4		=	P0^4;
sbit 	DATA5		=	P0^5;
sbit 	DATA6		=	P0^6;
sbit 	DATA7		=	P0^7;

sbit 	DATA8		=	P2^0;
sbit	DATA9		=	P2^1;
sbit 	DATA10		=	P2^2;
sbit	DATA11		=	P2^3;
sbit 	DATA12		=	P2^4;
sbit 	DATA13		=	P2^5;
sbit 	DATA14		=	P2^6;
sbit 	DATA15		=	P2^7;

sbit 	led0        =   P4^0;

sbit	RF_IRQ		=	P4^3;
sbit 	RF_TS		=	P5^0;
sbit 	RF_CS		=	P5^1;

#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))


//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_MEM.h"
#include "..\os\OS_SEM.h" 	
#include "SingleBus.h"
#include "GT23.h"	
#include "IAP.h"
#include "Uart.h"
#include "SPI.h"
#include "LCD.h"
#include "SST25VF.h"
//#include "ADS7843.H"
//设备配置字定义
typedef struct
{
	uint8   byInitFlag;										//初始化参数标志
	uint8  	bySelfSecAddr;									//本机区地址
	uint8  	bySelfRoomAddr;									//本机房地址
	uint8  	bySelfBedAddr;									//本机床地址
	uint8	bySerialNum1;									//序列号字节1(序列号最高位的一个字节)
	uint8	bySerialNum2;									//序列号字节2
	uint8	bySerialNum3;									//序列号字节3(序列号最低位的一个字节)
	uint8	byVersionHi;									//版本号高字节
	uint8	byVersionLo;									//版本号低字节 
	uint8   byXCoorPara;									//坐标X参数
	uint8   byYCoorPara;									//坐标Y参数
	uint8   byXTopLeftADValue;								//坐上角AD转换的X值
	uint8   byYTopLeftADValue;								//坐上角AD转换的Y值
	uint16  uiRs485Brt;										//RS485总线波特率
	uint8	byMaxPlayVolVal;								//语音报号的最大音量值
	uint8 	byCrc;											//校验和
	
} STEepromCfgData, *pSTEepromCfgData;

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
	uint8				byChar1;							//字符型数据
	uint16				uiShortType;						//短整型数据
	uint8	xdata 		*pbyData;							//字符数据指针
	STBusFreq 			stBusDealFreq;						//总线数据处理存储器
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体 
	STEepromCfgData		stEepromCfgData;					//系统配置字
	STTime				stTime;								//时间
	uint8 				byDisplayFace;						//显示界面
	uint8				byInputBox;							//输入框
	uint16				uiCursorX;							//光标坐标X
	uint16				uiCursorY;							//光标坐标Y	
	uint8 				bySecondFlashTime;					//秒灯闪烁时间
	uint8 				byDispNumSetOkTime;					//编号成功显示时间
//	STPointer			stCallPointer;						//呼叫队列双向链表
//	STIndicationData	stIndicationData;					//信息指示数据结构体	
	uint8 				byKeyInTimes;						//按键输入次数
	uint8 				byOperType;							//加/减/乘/除  操作类型
	uint16 				uiWardFaceDataLen;					//病区信息数据长度	
	uint8 				byCallCount;						//呼叫队列中节点个数
	uint8 				byUpdataRecNum;						//更新记录的序号	
	uint8 				byCallArray[MAX_CALL_COUNT][4];		//呼叫队列
} STLocalControl, *pSTLocalControl;
//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 
#define	Fosc				18432000L
#define Fbus				184320
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP0_ADDR			0x0000		//STC12LE5A60S2内的E2只能是数据E2
#define IAP1_ADDR			0x0200
#define INIT_FLAG           0x5a



#define VERSION_HI			0x02
#define VERSION_LO			0x01

#ifndef _IN_MAIN_
extern uint16	xdata uiIsrTimerCount;					//系统时钟定时值
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];						//主线程消息队列
extern STLocalControl 	xdata stLocalControl;						//全局变量结构体
extern void UserTickTimer(void);
extern void Send_Data(uint8 *Databuf,uint8 xdata l);
extern void Send_Data_Byte(uint8 SendData);
extern void Delayms(uint16 ms);

#endif
