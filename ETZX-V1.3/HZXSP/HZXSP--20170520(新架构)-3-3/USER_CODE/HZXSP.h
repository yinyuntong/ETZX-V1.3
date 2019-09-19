#ifndef _KTKZQ_H_
#define _KTKZQ_H_

#define DISPLAY_STATE_CALL 0x01 //有呼叫信息
#define DISPLAY_STATE_NURSE 0x02  //有到位信息
#define DISPLAY_STATE_MESSAGE 0x04 //有信息公布
#define DISPLAY_STATE_TIME 0x08 //可以显示时间
#define DISPLAY_STATE_CHANGEING 0x10 //切换显示	//实质上就是时间秒钟的变化,用于时间显示
#define DISPLAY_STATE_CHANGED 0x20 //有变换显示请求
#define DISPLAY_STATE_TIME_CHANGED 0x40 //显示变化


#define MESG_BUF_LEN 252


typedef struct {
	INT8U MessageBuf[MESG_BUF_LEN];
	INT8U color;
	INT8U  byCrc;	//以上数据保存在EEPROM中共256字节

	
	INT16U MessageIndex;
	INT16U MessageShowLen;
	INT8U state;
	INT8U CallBuf[6];	//保存单个呼叫信息
	INT8U  CallMessageBuf[MESG_BUF_LEN];	//呼叫显示信息缓冲区
	INT16U CallShowLen;
	INT16U CallIndex;
	INT8U NurseBuf[20];
	INT16U NurseShowLen;
	INT16U NurseIndex;
	INT16U ErrorShowLen;
	INT16U ErrorIndex;
	INT8U ChangeDelay; //切换延时
}DISPLAYINFO,*pDISPLAYINFO;


typedef struct {
	INT8U CallType;
	INT8U CallValue;
}SLAVECALL;


#define BEDTYPE	1
#define WCTYPE	2
#define ADDBEDTYPE	3

#define MESSAGE_CHANGED_DELAY 30 //30S

//系统状态位定义

#define SYSTEM_STATE_LOCAL_ERROR 0x01	   //本机错误
#define SYSTEM_STATE_DISPINFO 0x02  //是否显示信息
#define SYSTEM_STATE_ONLINE 0x04 //是否处于联机状态
#define SYSTEM_STATE_MOVING 0x10 //可以执行移动
#define SYSTEM_STATE_TESTMODE 0x20 //执行测试模式
#define SYSTEM_STATE_BUS_ERROR 0x40 //总线错误


#define SYSTEM_SAVE_SIZE 256 //系统信息存储长度
typedef struct {
	INT8U year;
	INT8U month;
	INT8U day;
	INT8U hour;
	INT8U minute;
	INT8U second;
}DATETIME, *pDATETIME; 

typedef struct system_info
{
	INT8U UartBaud;/*通信速率*/
	INT8U LOCAL_ADDR;   //本机区码
	DISPLAYINFO DisplayInfo; //显示信息
	INT16U state; //系统状态
	INT32U error; //系统错误计时
	DATETIME sTime; //系统时间
}SYSTEM_INFO;

extern SYSTEM_INFO SystemInfo;	 //系统信息
#endif

