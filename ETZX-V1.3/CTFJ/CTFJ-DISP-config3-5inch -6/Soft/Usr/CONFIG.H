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

#define DEBUG 1


#define MAX_COM	 1
 
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


#define CONFIG_CC1101 0

#define YHFJ_ROOM_ADDR 253	//医务分机段号


#define AVAIL_DATA_LEN 48

#define MONTH_AGE 150		//150以上的数值按是按月累计的年龄

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


#define BED_CARD_TYPE			0						//床头卡信息类型
#define BED_TYPE				1						//床头信息类型
#define WARD_SEC_TYPE			3						//病区信息界面信息类型
#define DOOR_SEC_TYPE			4						//门区信息界面信息类型
#define AISLE_SEC_TYPE			5						//走廊显示屏信息类型

#define DIET_TYPE				6						//饮食信息类型
#define PRESCRIPTION_TYPE		8					//药嘱
#define PROMPT_INFO_TYPE		9					//提示信息


#define POWER_UP_FACE		1									//上电界面
#define MAIN_MENU_FACE	2
#define CALL_SUCCESS_FACE		3
#define TALK_FACE				4
#define INFO_INDICATION_FACE	5

#define DIET_FACE			6						//饮食界面
#define PRESCRIPTION_FACE		8
#define PROMPT_INFO_FACE		9
#define SEC_INFO_FACE			10
#define BROAD_FACE				11
#define INFUSION_CALL_FACE		12
#define SERVICE_CALL_FACE		13
#define EMERGENCY_CALL_FACE		14
#define HELP_CALL_FACE			15
#define BED_INFO_FACE			16

#define START_NUM_SET_FACE		17
#define NUM_SET_OK_FACE			18

#define START_RF_NUM_SET_FACE	19 
#define RF_NUM_SET_OK_FACE		20




//存储芯片25VF032  共4M  0x400000空间
//字库使用到0x3b06b0

//床头卡信息使用从0x3c0000开始  还剩下256K空间
//用掉116K

#define DIET_DATA_START_ADDR	        0x3c0000			//饮食信息数据起始地址
												             //每类型信息占用32K
												
#define PRESCRIPTION_DATA_START_ADDR	0x3c8000			//医嘱信息数据起始地址


#define PROMPT_DATA_START_ADDR	   		0x3d0000				//提示信息数据起始地址


#define DIET_LENGTH_ADDR      			0x3d8000					//饮食信息总计长度存储地址

#define PRESCRIPTION_LENGTH_ADDR      	0x3d9000					//医嘱信息总计长度存储地址
											
#define PROMPT_LENGTH_ADDR      		0x3da000					//揭示信息总计长度存储地址


#define BED_FACE_DATA_LENGTH_ADDR		0x3db000					//床头界面中的显示数据长度起始地址

#define BED_FACE_DATA_START_ADDR		(BED_FACE_DATA_LENGTH_ADDR+2)	//床头界面中的数据在在存储器中的起始位置
																	     //最多只有4K
																	     
#define BED_FACE_FORM_LENGTH_ADDR		0x3dc000					//床头界面中的格式数据长度起始地址

#define BED_FACE_FORM_START_ADDR		(BED_FACE_FORM_LENGTH_ADDR+2)					//床头界面中的格式数据在在存储器中的起始位置
																	//最多只有4K

#define BED_FACE_DATA_BAK				0x3dd000					//对床头卡数据的一次备份( 包括长度,数据, 校验和)

#define BED_FACE_FORM_BAK				0x3de000					//对床头卡格式的一次备份( 包括长度,数据, 校验和)


//以下为接收信息的预存空间

#define BED_FACE_DATA_START_ADDR_2		0x3e0000					//床头界面中的数据在在存储器中的起始位置
																	//最多只有4K

#define BED_FACE_FORM_START_ADDR_2		0x3e1000					//床头界面中的格式数据在在存储器中的起始位置
																	//最多只有4K

#define FREE_START_ADDR					0X3ff000
#define FRAME_TEST_ADD					0X3ff000


#define LENGTH_32K		32512
#define LENGTH_4K		3840

#define FIRST_DATA_LEN		41			//首段数据长度
#define SEGMENT_DATA_LEN    60			//每段数据最长长度


#define SHORT_NAME_MODE			0
#define LONG_NAME_MODE			1

#define NAME_LEN_POSITION		41


#define	CC1101_REREAD_TIME		15000						//每隔5分钟读取1次CC1101


//项目代码
#define BED_NUMBER				1		//床号
#define ILLNESS_LEVEL			2		//病情
#define MEDICAL_LEVEL			3		//医保
#define NURSING_LEVEL			4		//护理
#define DIETARY_LEVEL			5		//饮食
#define HOSPITALIZED_NO			6		//住院号
#define NAME					7		//姓名
#define SEX						8		//性别
#define AGE						9		//年龄
#define ADMISSION_DATE			10		//入院
#define DIAGNOSIS               11		//诊断
#define BLOOD					12		//血型
#define CAUTION					13		//安全警示
#define ALERGY_DRUGS			14		//过敏史
#define DOCTOR					15		//医生
#define NURSE					16		//护士
#define ILLNESS_CHANGE			17		//病情变化
#define PHARMACY 				18		//用药
#define MESSAGE_BOARD			19		//留言板
#define DIET_INFO				20		//饮食信息
#define DOCTOR_ADVICE			21		//医嘱
#define HINT					22		//提示
#define OX_START				23		//开始
#define OX_END					24		//结束
#define OX_SUBTOTAL				25		//小计
#define OX_TOTAL				26		//累计
#define OX_SUPPLY				27		//供氧

#define MESSAGE2				28		//消息栏2
#define MESSAGE3				39		//消息栏3
#define MESSAGE4				30		//消息栏4
#define MESSAGE5				31		//消息栏5
#define MESSAGE6				32
#define MESSAGE7				33
#define MESSAGE8				34
#define MESSAGE9				35
#define MESSAGE10				36
#define MESSAGE11				37
#define MESSAGE12				38
#define MESSAGE13				39


#define MAX_ITEM_CODE 			40





//控制引脚定义
//控制引脚定义
sbit	MOSI		=	P1^5;								//SPI 主出从入口
sbit	MISO		=	P1^6;								//SPI 从出主入口
sbit	SCLK		=	P1^7;								//SPI  时钟线
sbit 	SCK			=	P1^7;

sbit  	EPH1660_CS		= 	P1^4;
sbit  	RESETB  	= 	P4^7;
sbit  	SPI_CMD 	= 	P5^2;
sbit  	WKICOM  	= 	P1^3;
sbit  	WKO	  		= 	P3^2;

sbit 	ADS7843_CS	=	P4^4;								//触摸芯片片选
sbit	TPBUSY		=	P1^2;
sbit 	TPIRQ		=   P1^3;



 //原使用GT23L字库芯片
//sbit 	GT23L_CS	=	P4^5;								//字库芯片片选
//sbit 	SST25VF_CS	=	P4^1;								//存储器片选

//现使用自己做的字库SST25VF032,放在原字库位置
sbit 	GT23L_CS	=	P4^1;								//字库芯片片选
sbit 	SST25VF_CS	=	P4^5;								//存储器片选



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

sbit	    CSN		=	P5^1;


#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))


//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 	
#include "SingleBus.h"
#include "GT23.h"	
#include "IAP.h"
#include "Uart.h"
#include "SPI.h"
#include "LCD.h"
#include "PCT25VF.h"
#include "EPH1660.H"
#if CONFIG_CC1101==1
#include "CC1101.H"
#endif

#include "128x80Arial.h"

typedef struct
{
	uint16  uiHour;
	uint8  	byMin;
	uint8   bySec;
} STOXTime, *pSTOXTime;


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
	STOXTime stSupplyOxTotalTime;							//供氧总计时间
	uint8   byRFSerialNum1;									//输液报警器序列号1
	uint8   byRFSerialNum2;									//输液报警器序列号2
	uint8   byRFSerialNum3;									//输液报警器序列号3
	uint8   byRFSerialNum4;									//输液报警器序列号4
	uint8   byRFSerialNum5;									//输液报警器序列号5
	uint8   byRFSerialNum6;									//输液报警器序列号6
	uint16 	uiRs485Brt;										//RS485总线波特率
	uint8	byRs485SecRoom;									//接收RS485数据时是否需要判断区号、房号
	uint8	byCrc;									//床头分机显示数据所在扇区

} STEepromCfgData, *pSTEepromCfgData;
//时间结构体
typedef struct
{	
	uint16 x;
	uint16 y;
}STCoordinate,*pSTCoordinate;


typedef struct
{	
	uint8 ItemCode;	//项目
	STCoordinate StartPoint;	//块自起点坐标
	STCoordinate EndPoint;		//块结束点坐标
	uint8 bcolor;				//背景颜色
	uint8 fcolor;				//字体颜色
	uint8 DispMode;				//显示方式
}STItemForm,*pSTItemForm;	//项目格式


typedef struct
{
	uint8 ItemCode;	//项目
	uint8 DataLength;	//该项数据长度
	uint8 *pData;		//具体要显示的数据
}STItemData,*pSTItemData;		//项目数据


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


#define PATIENT_DISCHARGE	0x01		//1:病人出院
#define LCD_ON				0x02		// 1 :背光打开


//全局控制结构体
typedef struct
{						//字符数据指针
	STBusFreq 			stBusDealFreq;						//总线数据处理存储器
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体 
	STEepromCfgData		stEepromCfgData;					//系统配置字
	STTime				stTime;								//时间
	uint8 				byDisplayFace;						//当前显示界面
	uint8				byDispNumSetOkTime;					//显示编号成功界面时间
	uint8 				byMSecond;							//毫秒单位定时
	STTime				stSupplyOxStartTime;				//供氧计时开始时间
	STTime				stSupplyOxEndTime;					//供氧计时结束时间
	STOXTime 			stSupplyOxSubtotalTime;				//供氧本次时间
	STOXTime			stSupplyOxTotalTime;				//供氧累次时间
	int16				uiBedFaceFormLen;					//整个一屏数据长度
	int16               uiBedFaceDataLen;					//长名字床头卡的一屏数据长度
	uint16				uiDietDataLen;						//饮食数据长度
	uint16				uiPrescriptionDataLen;				//医嘱数据长茺
	uint16              uiPromptDataLen;					//提示信息数据长度
	uint16 				uiSendedDataLen;					//已发数据长度
	uint16 				uiSendSBTotalDataLen;				//发送手柄总计数据长度
	uint8				bySendSBTimes;						//发送手柄次数
	
	uint8				byUsart1SdDelayTime;				//串口1发送数据延迟时间
	uint8				bySlaveState;						//分机状态
	//#define PATIENT_DISCHARGE 0x01		//1:病人出院
	//#define LCD_ON				0x02		// 1 :背光打开	�
	uint16 				uiLcdDisplayTime;					//显示屏显示时间
	uint16 				uiCC1101ReReadTime;					//cc1101重读间隔时间
	STItemForm          stItemForm;							//项目格式
	STItemData		    stItemData;							//项目数据
} STLocalControl, *pSTLocalControl;
//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 
#define	Fosc				18432000L
#define Fbus				184320
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			0x0000		//STC12LE5A60S2内的E2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN
#define _IN_MAIN
extern uint16	xdata uiIsrTimerCount ;					//系统时钟定时值
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];						//主线程消息队列
extern STLocalControl 	xdata stLocalControl;						//全局变量结构体
extern bit bWireLessRec;
//extern bit bOXSupplyState;
extern bit bEnableOxTimer;
extern bit	bBL_SW_State;


extern uint8 xdata byWireLessRecLen;
extern uint8 xdata byWireLessRecBuff[];
extern uint8 xdata byWireLessSndBuff[];

extern void UserTickTimer(void);
extern void Send_Data(uint8 *Databuf,uint8 l);
extern void Send_Data_Byte(uint8 SendData);
extern STOXTime   timeInterval(STTime a,STTime   b);
extern uint16   allday(STTime   a);

#endif
