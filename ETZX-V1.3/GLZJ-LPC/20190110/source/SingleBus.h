
#ifndef _SINGLE_BUS_H
#define _SINGLE_BUS_H

#define RELEASE	 1

//总线0宏定义
#define 	BUS0_FREQ_SIZE		7							//单总线0数据帧长度
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//单总线0数据队列长度
#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//单总线0数据队列长度(接收)

#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	(3<<10)
#define 	BUS0_ON_WORK		0x23


//添加呼叫记录时可能返回的值
#define		SAVE_FAIL			0		//存储失败
#define		SAVE_ONLY			1		//存储成功,唯一一条呼叫记录
#define		SAVE_NORMAL			2		//存储成功,正常添加呼叫记录
#define		SAVE_EXIST			3		//该条记录已经存在
#define		SAVE_PRIO_HI		4		//存储成功,有高优先级呼叫存在
#define		STATE_DEL			0		//没有查找到该记录
#define		STATE_EXIST			1		//查找到该记录 
#define		STATE_HI			2		//查找记录，发现有高优先级呼叫存在			



extern uint16_t			uiDevState; 						//设备当前所处的状态
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



#define SetDevState(x) (uiDevState|=(x))			//置位操作
#define ClrDevState(x) (uiDevState&=~(x))		//清零操作
#define GetDevState(x) (uiDevState&(x))			//获取位状态



//#define 	PC_ADDR1					0X00				//PC机地址1
#define 	PC_ADDR2					0X00				//PC机地址2
#define 	PC_ADDR3					0X00				//PC机地址3

#define 	GLZJ_ADDR2					0xfe				//管理主机地址2
#define     GLZJ_ADDR3					0X00				//管理主机地址3

#define 	ADD_BED_FJ			250							//加床分机房号地址	//0xFA
#define 	MOVE_FJ				251							//移动分机房号地址	//0xFB


#define 	YHFJ_ADDR2				0XFD					//医护分机房号地址
#define 	ZLDIS_ADDR2				0XF0					//走廊显示屏房号地址


#define 	RE_SND_TIMES	3



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

#define CMD_VOLUME_SET			0X5C	//音量设置命令
#define CMD_SYS_MOD_SET			0X5D	//系统运行模式设置(后门)
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



//发送和接收引脚定义

#define Bus0RecPin (1<<1)	//P0.1									//单总线0接收引脚定义
#define GetBus0RecPin() (FIO0PIN&Bus0RecPin)


#define Bus0SendPin	(1<<13)  //P2.13									//单总线0发送引脚定义
#define SetBus0SendPin() (FIO2SET = Bus0SendPin)
#define ClrBus0SendPin() (FIO2CLR = Bus0SendPin)



#define DisableBus0RecInt()	{IO0IntClr = Bus0RecPin; IO0IntEnF &= ~Bus0RecPin;} 					//单总线0禁止总线中断宏定义
#define EnableBus0RecInt()	{IO0IntClr = Bus0RecPin; IO0IntEnF |= Bus0RecPin;}			//单总线0恢复总线中断宏定义


#define SD_LOW_STATE  	(GetBus0RecPin()==0)
#define SD_HIGH_STATE	(GetBus0RecPin())

#define ACT_SD_LOW	   	(ClrBus0SendPin())
#define ACT_SD_HIGH	   	(SetBus0SendPin())


extern uint16_t State;							//R485相关标志
#define  bRS485DataTreat 	(1<<0)				//RS485数据处理标志
#define  bOriginMusic		(1<<1)				//背景音乐初始状态
#define  bPCOpen			(1<<2)				//上位机打开状态
#define  bTimerOutDeal		(1<<3)				//TimerOutDeal()函数处理标志
#define  bLedState			(1<<4)				//LED灯状态
#define  bPowerEnState		(1<<5)				//信号线电源状态
#define  bHostTalk			(1<<6)				//主机对讲
#define  bHostTalk1			(1<<7)				//主机对讲通道1
#define  bReadTime			(1<<8)				//读取时间标志
#define  bUsart2Rec			(1<<9)				//串口2接收到数据

#define SetState(x) (State|=(x))			//置位操作
#define ClrState(x) (State&=~(x))		//清零操作
#define GetState(x) (State&(x))			//获取位状态




extern uint16_t BusErrorState;					//单总线出错状态
#define  bStartBitError 	(1<<0)				//起始位出错
#define  bEndBitError		(1<<1)				//结束位出错
#define  bNightBitError		(1<<2)				//第9位出错


#define SetBusErrorState(x) (BusErrorState|=(x))			//置位操作
#define ClrBusErrorState(x) (BusErrorState&=~(x))		//清零操作
#define GetBusErrorState(x) (BusErrorState&(x))			//获取位状态



#ifndef _IN_SINGLE_BUS_
extern uint8_t TimerOutCount;			//20ms单元定时
extern uint8_t BusLowDTime;

//变量定义
extern uint8_t 				byChar;
extern uint8_t				byWaitAckTimeLanding;				//上电登记时等待PC机的应答时间
extern uint16_t				uiDevState; 						//设备当前所处的状态
extern uint16_t				uiDetectPcTime;						//侦听PC机间隔时间	
extern uint8_t				byVoicePlayTime;					//报号定时器
extern STBusFreq			stBusFreq;							//单总线数据处理结构体	
extern STTimerout			stCH0TimerOut;						//超时管理结构体
extern STTimerout			stCH1TimerOut;						//通道1通话超时管理结构体	
extern uint8_t				byWaitCommAckDTime;					//通用等待定时时间
extern uint8_t 				byUsart2HandingDTime;				//串口2正在处理定时时间
extern uint8_t 				byUsart0SdResultDTime;				//串口2发送结果码延时时间
extern STPointer			stCallPointer;						//呼叫队列双向链表
extern STIndicationData		stIndicationData;					//信息指示数据结构体
//extern STVoicePlay 			stVoicePlay;						//语音播放控制
extern STAddr				stCallAddr; 						//通道0主动呼叫方地址
extern STAddr				stCH1CallAddr; 						//通道1主动呼叫方地址
extern STAddr_S				stRS485Addr;						//RS485数据接收方地址
extern uint8_t				byWaitRS485AckDTime;				//通用等待定时时间
extern STEepromCfgData 		stEepromCfgData;					//本机配置字结构体
extern uint8_t				byReSndBrtTimes;					//重复发送设置波特率的次数
extern uint8_t				byReSndRs485RoomTimes;				//重复发送床头分机接收RS485信息是否需要判断房号的次数
extern uint8_t 				byReSndBedVoiPlaTimes;				//重复发送语音报号时床头分机是否语音报号的次数
extern STAddr_S				stCommAddr;							//通用地址

//---------------------------------


extern uint8_t TimerOutCount;			//20ms单元定时
extern uint8_t bBus0SndBitOk;



extern STTime	stTime;
extern uint8_t byYearH;		//年高字节;

extern void SingleBusInit(void);
extern void TIMER1_IRQHandler (void);			 //100us定时
extern uint8_t Bus0OutputData(uint8_t * pbyData);
extern void  eint3Isr (void);
extern void init_parameter(void);
extern void save_parameter(void);
extern void Bus0Manager(void);
extern void SaveCallAddr(pSTBusFreq pstBusFreq);
extern uint8_t AddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
extern uint8_t DirAddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
extern void MakeCH0TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
extern void MakeCH1TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
extern uint8_t AddCallNod(pSTBusFreq pstBusFreq);
extern void RemoveCallNod(pSTBusFreq pstBusFreq);
extern void SaveIndicationData(pSTBusFreq pstBusFreq, uint8_t byFirstNod);
extern void VoiceChannelCtx(void);
extern void RestartIndication(void);
extern void LedStateFlash(void);
extern void Bus0RecDeal(void);
extern void Bus0SendFinishDeal(void);
extern void TimerOutDeal(void);
extern void reset_parameter(void);

#endif

#endif

