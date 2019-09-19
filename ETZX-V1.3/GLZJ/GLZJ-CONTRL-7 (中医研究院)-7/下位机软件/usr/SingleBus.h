/*
**********************************Copyright (c) ****************************
*	  						cwg
*			       All Rights Reserved
*			     		QQ:7084394
*
*---------------------------------File Info---------------------------------
*File Name     		:SingleBus.h
*Descriptor    		:单总线头文件
*Create By     		:尹运同
*Create Date   		:2008-8-8
*Version       		:V1.0
*Note	     		:
*
*---------------------------------------------------------------------------
*Modified By  		:
*Modified Date  	:
*Version        	:
*Note	     		:
*
****************************************************************************
*/
#define		BUS0_STOP_TIME		200
#define		BUS0_TMP_STOP_TIME	10
//总线0宏定义
#define 	BUS0_FREQ_SIZE		7
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)
//#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*20 + 1)
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)

#define 	DisableBus0RecInt()	{IE0=0;EX0=0;} 
#define 	EnableBus0RecInt()	{IE0=0;EX0=1;}

#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23



//总线1宏定义
#define 	BUS1_FREQ_SIZE		7							//单总线1数据帧长度
#define 	BUS1_FREQ_SIZE_HI	(BUS1_FREQ_SIZE << 4)
#define 	BUS1_TX_Q_ZISE		(BUS1_FREQ_SIZE*5 + 1)		//单总线1数据队列长度
#define 	DisableBus1RecInt()	{EX1 = 0;} 					//单总线1禁止总线中断宏定义
#define 	EnableBus1RecInt()	{IE1 = 0;EX1 = 1;}			//单总线1恢复总线中断宏定义
#define 	BUS1_SEND_CON		0x3c
#define 	BUS1_CAN_SEND 		0x0c 
#define 	BUS1_ON_REC			0x03
#define		BUS1_REQ_SEND		0x28
#define		BUS1_SEND_FINISH	0x0c
#define 	BUS1_ON_WORK		0x23


//添加呼叫记录时可能返回的值
#define		SAVE_FAIL			0		//存储失败
#define		SAVE_ONLY			1		//存储成功,唯一一条呼叫记录
#define		SAVE_NORMAL			2		//存储成功,正常添加呼叫记录
#define		SAVE_EXIST			3		//该条记录已经存在
#define		SAVE_PRIO_HI		4		//存储成功,有高优先级呼叫存在
#define		STATE_DEL			0		//没有查找到该记录
#define		STATE_EXIST			1		//查找到该记录 
#define		STATE_HI			2		//查找记录，发现有高优先级呼叫存在			

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34


//100US
#define TIMER0_H  0xff
#define TIMER0_L  0x67


//uint8 DTimeState=0;
#define bCH0TimerOutDeal	0x01
#define bCH1TimerOutDeal	0x02
#define bWaitCommAckDTime	0x04
#define bWaitAckTimeLand	0x08
#define bWaitRS485AckDTime	0x10
#define bDetectPcTime		0x20
#define bUsart1SdResultDTime 0x40
#define bDataTimeUpdate     0x80


/*数据帧格式
字节1：发送方区地址（一个字节）
字节2：发送方房地址（一个字节）
字节3：发送方床地址（一个字节）
字节4：命令代码（一个字节）
字节5：接收方区地址（一个字节）
字节6：接收方房地址（一个字节）
字节7：接收方床地址（一个字节）
注:数据桢在不同的命令中可能有不同含义
*/
/*单总线协议
  命令代码:一字节
  数据代码:6字节(根据命令的不同数据格式)
*/
typedef struct
{
	unsigned char bySndSecAddr;			/*主动发送方区地址*/
	unsigned char bySndRoomAddr;		/*主动发送方房地址*/
	unsigned char bySndBedAddr;			/*主动发送方床地址*/
	unsigned char byCmd;				/*命令*/
	unsigned char byRecSecAddr;			/*接收方区地址*/
	unsigned char byRecRoomAddr;		/*接收方房地址*/
	unsigned char byRecBedAddr;			/*接收方床地址*/	
} STBusFreq, *pSTBusFreq;	
/*************************命令定义**********************/
#define CMD_ENTER_PC			0x01	/*对集成主机(PC机)的确认命令*/
#define CMD_BUS_ANSWER			0x02	/*单总线上的通用应答命令*/
#define CMD_RESULT_PC			0x02	/*对集成主机(PC机)的结果命令*/
#define CMD_LANDING				0x03	/*登记命令*/			/*后面两个字节为优先级和0x00*/
#define CMD_ENTER				0x04	/*确认命令*/			/*只能为主机对从机发送*/
/*****************************************************************/
#define CMD_COMM_CALL			0x05	/*普通呼叫命令*/		/*后面两个字节为优先级和主方状态*/
#define CMD_INFUSION_CALL		0x06	/*输液呼叫命令*/
#define CMD_SERVICE_CALL		0x07	/*服务呼叫命令*/
#define CMD_EMERGENCY_CALL		0x08	/*紧急呼叫命令*/
#define CMD_HELP_CALL			0x09	/*求援呼叫命令*/		/* 必须得到主机的确认命令*/
#define CMD_INSPECTOR_CALL		0x0a	/*查房呼叫命令*/		/* 必须得到主机的确认命令*/
/*****************************************************************/
#define CMD_COMM_ANSWER			0x0b	/*普通应答命令*/		
#define CMD_INFUSION_ANSWER		0x0c	/*处理输液命令*/		//处理方地址，命令，呼叫方地址
#define CMD_SERVICE_ANSWER		0x0d	/*处理服务命令*/
#define CMD_EMERGENCY_ANSWER	0x0e	/*处理紧急命令*/
#define CMD_HELP_ANSWER			0x0f	/*处理求援命令*/
/*****************************************************************/
#define CMD_INFUSION_CLEAR		0x10	/*清除输液命令*/		/* 对应的清除命令*/
#define CMD_SERVICE_CLEAR		0x11	/*清除服务命令*/
#define CMD_EMERGENCY_CLEAR		0x12	/*清除紧急命令*/
#define CMD_HELP_CLEAR			0x13	/*清除求援命令*/
/*****************************************************************/
#define CMD_BROADCAST1			0x14	/*1类广播呼叫命令*/
#define CMD_BROADCAST2			0x15	/*2类广播呼叫命令*/
#define CMD_CALL_LISTEN			0x16	/*接听呼叫命令*/
#define CMD_DISP_MODE			0x17	//床头分机显示模式设置命令
#define CMD_SYSTERM_RESET		0x18	/*系统复位命令*/
/*****************************************************************/
#define CMD_DATE_SEND			0x19	/*日期发送命令*/		/*前两个字节，年：4位BCD码，后面两个字节：月和日期：4位BCD码*/
#define CMD_COLOR_CLEAR			0X1a	//清除色块命令
#define CMD_SCHEDULE_SEND		0x1b	/*一览表发送命令*/
#define CMD_BED_VOICE_PLAY		0x1c	//管理主机报号时，床头分机是否语音报号
/*****************************************************************/
#define CMD_BROADCAST3			0x20	/*3类广播呼叫命令*/
#define CMD_INFUSION_ENTER		0x21	/*确认输液命令*/
#define CMD_SERVICE_ENTER		0x22	/*确认服务命令*/
#define CMD_EMERGENCY_ENTER		0x23	/*确认紧急命令*/
#define CMD_HELP_ENTER			0x24	/*确认求援命令*/
/*****************************************************************/

#define CMD_START_VOICE			0x26	/*启动播音命令*/
#define CMD_STOP_VOICE			0x27	/*停止播音命令*/
#define CMD_MUSIC_PLAY			0x28	/*背景音乐命令*/
/*****************************************************************/
#define CMD_CHANNEL_CHANGE		0x2a	/*切换通话通道*/
#define CMD_CHANNEL_CLOSE		0x2b	/*关闭通话通道*/
#define CMD_POWER_ON			0x2c	/*上电复位命令*/
#define CMD_INSPERCTOR_ENTER	0x2d	/*确认查房命令*/
#define CMD_STOP_INDICATION		0x2e	/*终止呼叫信息命令*/
#define CMD_INFO_INDICATION		0x2f	/*信息指示命令*/
/*主动呼叫方地址在前两个字节，后面两个字节为呼叫类型：高3位优先级，低5位为类别*/
/*****************************************************************/
#define CMD_INCREACE_VOL		0x31	/*音量增加命令*/
#define CMD_DECREACE_VOL		0x32	/*音量减小命令*/

#define CMD_RS485_BRT_SET		0x38	//RS485波特率设置
#define CMD_RS485_INFO_SEC_ROOM	0x39	//床头分机接收RS485信息时是否需要判断区号、房号
#define CMD_VOICE_PLAY_ROOM		0x3a	//语音报号是否播报房号

#define CMD_OPEN_LCD			0x3e	/*打开显示命令*/
#define CMD_CLOSE_LCD			0x3f	/*关闭显示命令*/
/*****************************************************************/
#define CMD_OPEN_485BUS_IN		0x41	/*打开485输入命令*/
#define CMD_CLOSE_485BUS		0x42	/*关闭485命令*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE_LCD	0x43    /*清除手柄液晶信息命令*/
#define CMD_CLEAR_LCD			0x49	/*清除液晶信息命令*/

#define CMD_CLEAR_MK_LCD		0x54	//清除门口分机显示屏
/*****************************************************************/
#define CMD_NUMBER_SET		    0x5a	/*编号命令*/
#define CMD_SYSTEM_SEC_SET		0x5b	//系统区号设置
#define CMD_VOICE_TIMES			0x5c	//语音报号遍数设置
/*****************************************************************/
#define CMD_GET_BUS				0x71	/*占用总线命令*/

#define CMD_QUEST				0x73	/*查询命令*/
#define CMD_QUEST_ANSWER		0x74	/*查询应答命令*/
#define CMD_TALK_CLEAR			0X75	/*结束通话命令*/

#define CMD_DISPLAY_DATA		0x80	/*发送一帧液晶屏数据*/
#define CMD_HANDLE_DISPLAY_DATA 0x82	/*发送一帧手柄 液晶屏数据*/
#define CMD_DATA_ERROR			0x81	/*一帧数据出错命令*/
#define CMD_DISPLAY_FORM		0X83	//发送一帧床头分机格式

#define CMD_NURSE_IN			0X83	//护士到位命令
#define CMD_NURSE_OUT			0X84	//护士离开命令

#define CMD_SUPPLY_OX_START		0X85	//供氧开始
#define CMD_SUPPLY_OX_END		0X86	//供氧结束

#define CMD_RF_NUMSET_OK		0x88	//输液报警器对码成功

#define CMD_VOICE_TIMER_SET		0x89	//小音段时段设置
#define CMD_BL_TIMER_SET		0x8a	//背光关时段设置

#define CMD_PC_OPEN				0x8b	//上位机打开命令
#define CMD_PC_CLOSE			0x8C	//上位机关闭命令
#define CMD_SUPPLY_OX_TOTAL		0x8d	//供氧计时总计命令

#define  CMD_STOP_VOICE_PLAY	0x90	//语音报号结束命令
#define  CMD_SYSTEM_STATE		0x91	//报告系统状态

/*****************************************************************/
#define CMD_ONLINE_WRITE		0xf3	/*在线写入命令*/
#define CMD_ONLINE_READ			0xf4	/*在线读出命令*/
#define CMD_ONLINE_OUTPUT		0xf5	/*在线输出命令*/
/*****************************************************************/
#define CMD_LED_OFFLINE_WRITE	0xf0	/*ledeeprom离线写入命令*/
#define CMD_LED_OFFLINE_READ	0xf1	/*ledeeprom离线读出命令*/
#define CMD_LED_OFFLINE_OUTPUT	0xf2	/*ledeeprom离线输出命令*/
/*****************************************************************/
#define CMD_EEPROM_OFFLINE_WR	0xfe	/*eeprom离线写入命令*/
#define CMD_EEPROM_OFFLINE_RD	0xf1	/*eeprom离线读出命令*/
#define CMD_EEPROM_OFFLINE_OUTT	0xf2	/*eeprom离线输出命令*/ 

#ifndef _IN_SINGLE_BUS_
#define _IN_SINGLE_BUS_
void SingleBusInit(void); 	
void Bus0OutputData(uint8* pbyData); 
void Bus1OutputData(uint8* pbyData);

void Bus0RecDeal(void);
void Bus1RecDeal(void);

void Bus0SendFinishDeal(void); 
void Bus1SendFinishDeal(void);

void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);
	

void TimerOutDeal(void);
//void MakeVoiceData();
//void StartVoicePlay();

extern uint8 AddCallNod(pSTBusFreq pstBusFreq);
extern uint8 byReSend;

extern bit  bBus0RecFinish;				//单总线0接收完成标志
extern bit  bBus0SendFinish;				//单总线0发送完成标志

extern bit  bBus1RecFinish;				//单总线0接收完成标志
extern bit  bBus1SendFinish;				//单总线0发送完成标志


extern  bit flag_origin_music;			//播放背蠊音乐的原始状态
extern uint8 DTimeState;

#endif
