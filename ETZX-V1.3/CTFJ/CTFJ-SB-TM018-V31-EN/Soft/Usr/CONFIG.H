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
#define KEY_DOWN		0x50								//按键按下
#define KEY_UP			0x60								//按键弹起
#define	KEY_ALWAYS		0x70								//键常按下
#define TIMER_OUT		0x80								//超时命令	
#define	UART0_CMD		0x90				                //串口0数据帧命令
#define	UART1_CMD		0xa0				                //串口1数据帧命令


//按键值定义
/*#define	UP_KEY_VAL			0x00								
#define DOWN_KEY_VAL		0x01							
#define ENTER_KEY_VAL		0x02							
#define ESC_KEY_VAL			0x03								
#define CALL_KEY_VAL		0x04						
#define NO_KEY_VAL			0xff								//无键按下*/

#define	DIET_KEY_VAL			0x00								
#define PRESCRIPTION_KEY_VAL		0x01							
#define PROMPT_KEY_VAL		0x02							
#define ESC_KEY_VAL			0x03								
#define CALL_KEY_VAL		0x04						
#define NO_KEY_VAL			0xff								//无键按下

#define LED_ON			0									//灯亮
#define LED_OFF			1									//灯灭
#define	LED_FLASH		2									//灯闪烁


#define FLAG_LED_ON			0									//灯亮标志
#define FLAG_LED_OFF			1									//灯灭标志

#define DIET_TYPE			6						//饮食信息类型
#define PRESCRIPTION_TYPE		8					//药嘱
#define PROMPT_INFO_TYPE		9					//提示信息

#define LAND_FACE		1							//常态界面
#define MAIN_MENU_FACE	2
#define CALL_SUCCESS_FACE		3
#define TALK_FACE				4

#define DIET_FACE			6						//饮食界面
#define PRESCRIPTION_FACE		8
#define PROMPT_INFO_FACE		9

#define BROAD_FACE				10
#define LCD_DISPLAY_OFF_FACE	11


#define LINE_FEED				10				//换行符
#define FORM_FEED				12				//换页符


#define MAX_PAGE_NUMBER			10				//最大页数


//存储芯片25VF032  共4M  0x400000空间
//字库使用到0x3b06b0

//床头卡信息使用从0x3c0000开始  还剩下256K空间
//用掉116K

#define DIET_DATA_START_ADDR	        0x3c0000			//饮食信息数据起始地址
												             //每类型信息占用32K
												

#define PRESCRIPTION_DATA_START_ADDR	0x3c8000			//医嘱信息数据起始地址


#define PROMPT_DATA_START_ADDR	   		0x3d0000				//提示信息数据起始地址


#define BED_FACE_DATA_START_ADDR		0x3d8000					//床头界面中的数据在在存储器中的起始位置
																	//最多只有4K

#define DIET_LENGTH_ADDR      			0x3d9000					//饮食信息总计长度存储地址

#define PRESCRIPTION_LENGTH_ADDR      	0x3da000					//医嘱信息总计长度存储地址
											
#define PROMPT_LENGTH_ADDR      		0x3db000					//揭示信息总计长度存储地址

#define BED_FACE_FORM_START_ADDR		0x3dc000					//床头界面中的格式数据在在存储器中的起始位置
																	//最多只有4K
#define BED_FACE_FORM_LENGTH_START_ADDR		0x3dd000					//床头界面中的格式数据长度起始地址

#define BED_FACE_DATA_LENGTH_START_ADDR		0x3de000					//床头界面中的显示数据长度起始地址




										
#define MALLOC_MEM_SIZE	50


#define LCD_DISPLAY_INFO_TIME	3000

//控制引脚定义
sbit	MOSI		=	P1^5;								//SPI 主出从入口
sbit	MISO		=	P1^6;								//SPI 从出主入口
sbit	SCLK		=	P1^7;								//SPI  时钟线


sbit	ESC_KEY  	= 	P2^0;								//返回键
sbit	UP_KEY 		= 	P2^1;								//上翻
sbit	ENTER_KEY 	= 	P2^2;								//确认键
sbit 	DOWN_KEY		=	P2^3;								//下翻

sbit    CALL_KEY	=   P2^4;								//呼叫键

sbit    SCL		=	P2^5;
sbit 	SDA         =	P2^6;

sbit	LED			=	P5^0;								//LED指示


//原来的
//sbit 	GT23L_CS	=	P1^3;								//字库芯片片选
//sbit 	SST25VF_CS	=	P1^4;								////存储器片选

//现在的
sbit 	GT23L_CS	=	P1^4;								//字库芯片片选
sbit 	SST25VF_CS	=	P1^3;								////存储器片选


sbit  LCD_WR   = P3^6;								//TFT屏相关口线
sbit  LCD_RD   = P3^7;
sbit  LCD_CS7  = P1^1;
sbit  LCD_RS   = P1^0;
sbit  LCD_RST  = P4^4;


sbit 	DATA0		=	P0^0;
sbit	DATA1		=	P0^1;
sbit 	DATA2		=	P0^2;
sbit	DATA3		=	P0^3;
sbit 	DATA4		=	P0^4;
sbit 	DATA5		=	P0^5;
sbit 	DATA6		=	P0^6;
sbit 	DATA7		=	P0^7;
#define LCD_DATA	P0


//sbit  	MIC_CTRL	=	P3^1;
sbit  	MIC_CTRL	=	P3^2;

sbit 	BL_SW		=	P5^1;

#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))

//晶振宏定义
#define	Fosc				18432000
#define Fbus				184320


//包含头文件
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 	
#include "SingleBus.h"	
#include "IAP.h"
#include "Usart.h"
#include "SPI.h"
#include "lcd.h"
#include "PCT25VF.h"
#include "Key.h"
#include "GT23.h"
#include "qnio.h"


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
	uint16  uiRs485Brt;										//RS485总线波特率
	uint8	byCrc;											//校验和
} STEepromCfgData, *pSTEepromCfgData;


//超时控制结构体
typedef struct
{
	uint8 				byTimerOutSet;						//一次超时时间设置(20ms单位)
	uint8 				byTimerOut;							//一次超时时间(20ms单位)
	uint8 				byTimerOutCount;					//允许超时次数 
} STTimerOut, *pSTTimerOut;

//时间结构体
typedef struct
{
uint8 bySecond;
uint8 byMinute;
uint8 byHour;
uint8 byDay;
uint8 byMonth;
uint8 byYear;
}STTime;

//双向链表节点
typedef struct _STNod
{
    struct _STNod xdata *pstPrev;
    struct _STNod xdata *pstNext;    
} STNod, *pSTNod;

//全局控制结构体
typedef struct
{
	uint8				byLedState;							//灯状态 0:灭,1.亮,2:闪烁
	uint8				byLedFlashTime;						//灯闪烁时间控制
	uint8				byLedTimeSet;						//闪烁时间设置值
	uint8 				bySecondFlashTime;					//显示界面上秒钟闪烁时间	//500us
	uint8 				byDisplayFace;						//显示界面是哪个界面
	STNod				stDispPagePointer;					//显示页指针链
	uint16				uiShortType;						//短整型数据
	uint8				byChar1;							//字节数据1			
	uint8 				xdata* pbyData;						//数据指针
	STTime				stTime;								//时间结构体
	STBusFreq 			stBusDealFreq;						//单总线处理结构体
	STTimerOut			stCH0TimerOut;						//设备当前状态超时控制结构体
	uint16				uiDietDataLen;						//饮食信息数据长度
	uint16				uiPrescriptionDataLen;				// 医嘱信息数据长度
	uint16 				uiPromptDataLen;					//提示信息数据长度
	STEepromCfgData		stEepromCfgData;					//系统配置字
	uint8 				byPatientDischarge;					//1==病人出院标志
	uint16 				uiLcdDisplayOnTime;					//LCD显示屏亮的时间
	uint16 				uiLcdDisplayInfoTime;				//LCD屏显示信息的时间
} STLocalControl, *pSTLocalControl;	

//5msPCA中断计时次数，Fosc=18.432MHz,分频系数为2 			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			0x0000		//STC12LE5A60S2内的E2只能是数据E2

//只有2个扇区，每盲区512字节
#define	IAP0_ADDR			0x0000		//STC12C5A60S2内的E2
#define IAP1_ADDR			0x0200
#define INIT_FLAG           0x5a


#define VERSION_HI			0x03
#define VERSION_LO			0x01

#ifndef _IN_MAIN_
#define _IN_MAIN_
extern STLocalControl xdata stLocalControl;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];
extern uint8 xdata MicCtrlState;

extern bit bBL_SW_State;

extern void UserTickTimer(void);
#endif
