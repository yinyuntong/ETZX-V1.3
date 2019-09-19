/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SingleBus.h
*文件描述    		:单总线程序头文件
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
//总线0宏定义
#define 	BUS0_FREQ_SIZE		7							//单总线0数据帧长度
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*5 + 1)		//单总线0数据队列长度
#define 	DisableBus0RecInt()	{EX1 = 0;} 			        //单总线0禁止总线中断宏定义
#define 	EnableBus0RecInt()	{IE1 = 0;EX1 = 1;}			//单总线0恢复总线中断宏定义
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23 
//单总线自动重载时间
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000)))
//#define		TIMER0_COUNT		((256-(Fbus*15000/12/1000000))) 	//150us

//30us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34

//100us
#define TIMER0_H  0xff
#define TIMER0_L  0x67


 
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
	uint8 bySndSecAddr;			/*主动发送方区地址*/
	uint8 bySndRoomAddr;		/*主动发送方房地址*/
	uint8 bySndBedAddr;			/*主动发送方床地址*/
	uint8 byCmd;				/*命令*/
	uint8 byRecSecAddr;			/*接收方区地址*/
	uint8 byRecRoomAddr;		/*接收方房地址*/
	uint8 byRecBedAddr;			/*接收方床地址*/	
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
#define CMD_INFUSION_ANSWER		0x0c	/*处理输液命令*/
#define CMD_SERVICE_ANSWER		0x0d	/*处理服务命令*/
#define CMD_EMERGENCY_ANSWER	0x0e	/*处理紧急命令*/
#define CMD_HELP_ANSWER			0x0f	/*处理求援命令*/
/*****************************************************************/
#define CMD_INFUSION_CLEAR		0x10	/*清除输液命令*/		/* 对应的清除命令*/
#define CMD_SERVICE_CLEAR		0x11	/*清除服务命令*/
#define CMD_EMERGENCY_CLEAR		0x12	/*清除紧急命令*/
#define CMD_HELP_CLEAR			0x13	/*清除求援命令*/
/*****************************************************************/
#define CMD_BROADCAST1			0x14	/*病区广播呼叫命令*/
#define CMD_BROADCAST2			0x15	//办公区广播呼叫命令*/
#define CMD_CALL_LISTEN			0x16	/*接听呼叫命令*/
#define CMD_DISP_MODE			0x17	//床头分机显示模式设置命令
#define CMD_SYSTERM_RESET		0x18	/*系统复位命令*/
/*****************************************************************/
#define CMD_DATE_SEND			0x19	/*日期发送命令*/		/*前两个字节，年：4位BCD码，后面两个字节：月和日期：4位BCD码*/
#define CMD_COLOR_CLEAR			0X1a	//清除色块命令
#define CMD_SCHEDULE_SEND		0x1b	/*一览表发送命令*/

/*****************************************************************/
#define CMD_BROADCAST3			0x20	/*全区广播呼叫命令*/
#define CMD_INFUSION_ENTER		0x21	/*确认输液命令*/
#define CMD_SERVICE_ENTER		0x22	/*确认服务命令*/
#define CMD_HELP_ENTER			0x23	/*确认求援命令*/
#define CMD_EMERGENCY_ENTER		0x24	/*确认紧急命令*/
/*****************************************************************/
#define CMD_CLEAR_INSPERCTOR	0x25	/*清除查房命令*/
#define CMD_START_VOICE			0x26	/*启动播音命令*/
#define CMD_STOP_VOICE			0x27	/*停止播音命令*/
#define CMD_MUSIC_PLAY			0x28	/*背景音乐命令*/
#define CMD_REQUEST_INFO		0x29	/*请求发送信息命令*/
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
#define CMD_RS485_INFO_SEC_ROOM	0x39	//床头分机接收RS485信息时是否判决区号、房号参数设置命令

#define CMD_OPEN_LCD			0x3e	/*打开显示命令*/
#define CMD_CLOSE_LCD			0x3f	/*关闭显示命令*/
/*****************************************************************/
#define CMD_OPEN_485BUS_IN		0x41	/*打开485输入命令*/
#define CMD_CLOSE_485BUS		0x42	/*关闭485命令*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE_LCD	0x43	/*清除手柄液晶屏*/
#define CMD_CLEAR_LCD			0x49	/*清除液晶信息命令*/


#define CMD_NUMBER_SET			0x5a	/*编号命令*/
#define CMD_SYSTEM_SEC_SET		0x5B	//系统区号设置命令

#define	CMD_LED_INDICATE		0x70	/*灯指示命令,发送方地址无效，区地址为灯状态*/
#define CMD_GET_BUS				0x71	/*占用总线命令*/
#define CMD_QUEST				0x73	/*查询命令*/
#define CMD_QUEST_ANSWER		0x74	/*查询应答命令*/
#define CMD_TALK_CLEAR			0X75	/*结束通话命令*/


/*****************************************************************/
#define CMD_DISPLAY_DATA		0x80	/*发送一帧液晶屏数据*/
#define CMD_DATA_ERROR			0x81	/*一帧数据出错命令*/
#define CMD_SB_LCD_DISPLAY_DATA		0x82	/*发送一帧液晶屏数据*/
#define CMD_DISPLAY_FORM		0X83	//界面显示格式数据

#define CMD_NURSE_IN			0X83	//护士到位命令
#define CMD_NURSE_OUT			0X84	//护士离开命令


#define CMD_SUPPLY_OX_START		0X85	//供氧开始
#define CMD_SUPPLY_OX_END		0X86	//供氧结束

#define CMD_RF_NUMSET_START		0x87	//输液报警器对码开始
#define CMD_RF_NUMSET_OK		0x88	//输液报警器对码成功
#define CMD_RF_NUMSET_END		0x89	//输液报警器对码结束
#define CMD_TELL_RF_NUM		    0x8A	//告诉显示模块输液报警器的序号

#define CMD_PC_OPEN				0x8b	//上位机打开命令
#define CMD_PC_CLOSE			0x8C	//上位机关闭命令
#define CMD_SUPPLY_OX_TOTAL		0x8d	//供氧计时总计命令
#define CMD_BUS1_TEST			0x8e	
#define CMD_BUS1_TEST_ANSWER	0x8f

#define CMD_VOICE_TIMER_GET		0x90	//获取小音段时间
#define CMD_BL_TIMER_GET		0x91	//获取背光关时间段
#define CMD_WARD_SEC_INFO_SET	0x92	//病区信息设置

#define CMD_SYSTERM_RESTART		0x93	//系统重新开始命令
#define CMD_SEND_PIN_BUG		0x94	//单总线0发送端故障
#define CMD_SB_DATA_ANSWER		0X95	//手柄数据回应答
/*****************************************************************/


#define CMD_ONLINE_WRITE		0xf3	/*在线写入命令*/
#define CMD_ONLIEN_READ			0xf4	/*在线读出命令*/
#define CMD_ONLINE_OUTPUT		0xf5	/*在线输出命令*/
/*****************************************************************/
#define CMD_LED_OFFLINE_WRITE	0xf0	/*ledeeprom离线写入命令*/
#define CMD_LED_OFFLINE_READ	0xf1	/*ledeeprom离线读出命令*/
#define CMD_LED_OFFLINE_OUTPUT	0xf2	/*ledeeprom离线输出命令*/
/*****************************************************************/
#define CMD_EEPROM_OFFLINE_WR	0xfe	/*eeprom离线写入命令*/
#define CMD_EEPROM_OFFLINE_RD	0xf1	/*eeprom离线读出命令*/
#define CMD_EEPROM_OFFLINE_OUTT	0xf2	/*eeprom离线输出命令*/ 

extern uint8 Bus0OutputData(uint8* pbyData);  

extern bit Bus0SendPin;

#ifdef _IN_SINGLE_BUS_
//设备状态定义
uint8 	bdata 	byDevState1;
sbit  	bBusy 				= 	byDevState1^7;				//系统忙标志
sbit	bSetKeyDown			=	byDevState1^6;				//设置键按下状态
sbit	bDealKeyDown		=	byDevState1^5;				//处理键按下状态
sbit	bNurseIn			=	byDevState1^4;	 			//护士到位标志
sbit	bChannel1Talked		=	byDevState1^3;				//通道1被动通话标志
sbit	bChannel1Talk		=	byDevState1^2;				//通道1主动通话标志
sbit	bIndicatingOther	=	byDevState1^1;				//指示其它分机呼叫标志
sbit	bLanding			=	byDevState1^0;				//上电登记状态
uint8 	bdata 	byDevState2;
sbit  	bWaitAck 			= 	byDevState2^7;				//等待应答标志
sbit  	bWaitListen			= 	byDevState2^6;				//等待接听标志
sbit	bChannel0Talked		=	byDevState2^5;				//通道0被动通话标志
sbit	bChannel0Talk		=	byDevState2^4;				//通道0主动通话标志
sbit	bSickRoomBroad		=	byDevState2^3;				//病区广播标志
sbit	bOfficeBroad		=	byDevState2^2;				//办公区广播标志
sbit	bAllBroad			=	byDevState2^1;				//全区广播标志
sbit	bSelfBroad			=	byDevState2^0;				//主动广播标志
uint8 	bdata 	byDevState3;
sbit	bCalledRing			=	byDevState3^7;				//被叫振铃标志
sbit	bVoiceNoting		=	byDevState3^6;				//语音提示标志
sbit	bMusicPlaying		=	byDevState3^5;				//背景音乐播放标志
sbit	bConfusionNoting	=	byDevState3^4;				//输液呼叫提示标志
sbit	bServiceNoting		=	byDevState3^3;				//服务呼叫提示标志
sbit	bHelpNoting			=	byDevState3^2;				//求援呼叫提示标志
sbit	bEmergencyNoting	=	byDevState3^1;				//紧急呼叫提示标志 
sbit	bPWState			=	byDevState3^0;				//PW控制脚状态
//许可标志定义
uint8 	bdata byEnable1;
sbit	bEnSickRoomBroad	= 	byEnable1^0;				//许可病区广播
sbit	bEnOfficeBroad		=	byEnable1^1;				//许可办公区广播
sbit	bEnAllBroad			=	byEnable1^2;				//许可全区广播
sbit	bEnSoundNote		=	byEnable1^3;				//许可语音提示
sbit	bEnOutRing			=	byEnable1^4;				//许可外部振铃
sbit	bEnAutoListen		=	byEnable1^5;				//许可自动接听
sbit	bEnVoiceNote		=	byEnable1^6;				//许可声音提示
sbit	bEnHandleDeal		=	byEnable1^7;				//许可手柄处理
uint8 	bdata byEnable2;
sbit	bEnInfusionDeal		=	byEnable2^0;				//许可输液呼叫处理
sbit	bEnServiceDeal		=	byEnable2^1;				//许可服务呼叫处理
sbit	bEnHelpDeal			=	byEnable2^2;				//许可求援呼叫处理
sbit	bEnEmergencyDeal	=	byEnable2^3;				//许可求援呼叫处理
sbit	bEnCallDeal			=	byEnable2^4;				//许可呼叫处理
sbit	bEnMusicplay		=	byEnable2^5;				//许可背景音乐播放
sbit	bEnHandDown			=	byEnable2^6;				//许可手柄挂断呼叫
#else
extern	uint8 	byDevState1;
extern	uint8 	byDevState2;
extern	uint8 	byDevState3;
extern 	uint8 	byEnable1;
extern	uint8 	byEnable2;
extern	bit  	bBusy;
extern 	bit		bSetKeyDown;
extern	bit		bDealKeyDown;
extern	bit		bNurseIn;
extern	bit		bChannel1Talked;
extern	bit		bChannel1Talk;
extern	bit		bIndicatingOther;
extern	bit		bLanding;
extern	bit  	bWaitAck;
extern	bit  	bWaitListen;
extern	bit		bChannel0Talked;
extern	bit		bChannel0Talk;
extern	bit		bSickRoomBroad;
extern	bit		bOfficeBroad;
extern	bit		bAllBroad;
extern	bit		bSelfBroad;	
extern	bit		bCalledRing;
extern	bit		bVoiceNoting;
extern	bit		bMusicPlaying;
extern	bit		bConfusionNoting;
extern	bit		bServiceNoting;
extern	bit		bHelpNoting;
extern	bit		bEmergencyNoting;
extern  bit		bPWState;
/********************************************/
extern	bit		bEnSickRoomBroad;
extern	bit		bEnOfficeBroad;
extern	bit		bEnAllBroad;
extern	bit		bEnSoundNote;
extern	bit		bEnOutRing;
extern	bit		bEnAutoListen;
extern	bit		bEnVoiceNote;
extern	bit		bEnHandleDeal;
extern	bit		bEnInfusionDeal;
extern	bit		bEnServiceDeal;
extern	bit		bEnHelpDeal;
extern	bit		bEnEmergencyDeal;
extern	bit		bEnCallDeal;
extern	bit		bEnMusicplay;
extern	bit		bEnHandDown;

void SingleBusInit(void); 	 
void ReadParameter(uint16 addr);
void SaveParameter(void);
void InitParameter(void);
void VoiceChannelCtx(void);
void LedControl(void);
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);		
void Bus0RecDeal(void);
void Bus0SendDeal(void);
void Bus0Manage(void);
void TimerOutDeal(void);
void TimerOutManager(void);
bit AddrCompare(pSTBusFreq pstBusFreq);

#endif
