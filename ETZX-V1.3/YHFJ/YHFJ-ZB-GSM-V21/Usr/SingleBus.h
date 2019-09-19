/*
************************Copyright(c)************************
*	  			   湖南一特股份有限公司
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
#define 	BUS0_FREQ_SIZE		7							//单总线0数据帧长度
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//单总线0数据队列长度

#define 	DisableBus0RecInt()	{EX0 = 0;PW=1;PW2=1;VL0=0;} 		//单总线0禁止总线中断宏定义
#define 	EnableBus0RecInt()	{IE0 = 0;EX0 = 1;}			//单总线0恢复总线中断宏定义
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23
//总线1宏定义
#define 	BUS1_FREQ_SIZE		7							//单总线1数据帧长度
#define 	BUS1_FREQ_SIZE_HI	(BUS1_FREQ_SIZE << 4)
#define 	BUS1_TX_Q_ZISE		(BUS1_FREQ_SIZE*12 + 1)		//单总线1数据队列长度
#define 	DisableBus1RecInt()	{EX1 = 0;} 					//单总线1禁止总线中断宏定义
#define 	EnableBus1RecInt()	{IE1 = 0;EX1 = 1;}			//单总线1恢复总线中断宏定义
#define 	BUS1_SEND_CON		0x3c
#define 	BUS1_CAN_SEND 		0x0c 
#define 	BUS1_ON_REC			0x03
#define		BUS1_REQ_SEND		0x28
#define		BUS1_SEND_FINISH	0x0c
#define 	BUS1_ON_WORK		0x23


#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*5 + 1)		//单总线0数据接收队列长度
#define 	BUS1_RX_Q_ZISE		(BUS1_FREQ_SIZE*5 + 1)		//单总线0数据接收队列长度

#define 	IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define 	Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE)

#define 	IncBus1RxPtr(addr)    {addr=(addr+1)%BUS1_RX_Q_ZISE;}
#define 	Bus1RxBuffLen()	      ((byBus1RxTail+BUS1_RX_Q_ZISE-byBus1RxHead)%BUS1_RX_Q_ZISE)

//单总线自动重载时间
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000)))

//300us
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
#define CMD_RESULT_PC			0x02	/*对集成主机(PC机)的结果命令*/

#define CMD_BUS_ANSWER			0x02	/*单总线上的通用应答命令*/
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
#define CMD_BROADCAST1			0x14	/*1类广播呼叫命令*/
#define CMD_BROADCAST2			0x15	/*2类广播呼叫命令*/
#define CMD_CALL_LISTEN			0x16	/*接听呼叫命令*/
#define CMD_PRIORITY_SET		0x17	/*优先级指定命令*/		/*系统命令：第一个字节高3位为优先级，接收方地址*/
#define CMD_SYSTERM_RESET		0x18	/*系统复位命令*/
/*****************************************************************/
#define CMD_DATA_SEND			0x19	/*日期发送命令*/		/*前两个字节，年：4位BCD码，后面两个字节：月和日期：4位BCD码*/
#define CMD_LCD_SEND			0x1a	/*屏数据发送命令*/
#define CMD_SCHEDULE_SEND		0x1b	/*一览表发送命令*/
#define CMD_HANDLER_SEND		0x1d	/*发送手柄液晶命令*/
/*****************************************************************/
#define CMD_CONTROL_SEND		0x1e	/*控制字传递命令*/
#define CMD_FLOOR_SET			0x1f	/*修改楼层号命令*/
#define CMD_BROADCAST3			0x20	/*3类广播呼叫命令*/
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
#define CMD_MUSIC				0x30	/*音乐命令*/
#define CMD_INCREACE_VOL		0x31	/*音量增加命令*/
#define CMD_DECREACE_VOL		0x32	/*音量减小命令*/


#define CMD_GSM_NUM_SEARCH		0x33	//查询GSM模块中设置的呼叫手机号码
#define CMD_GSM_NUM_SET			0X34	//设置GSM模块中的呼叫手机号码
#define CMD_GSM_NUM_DEL			0X35	//删除GSM模块中设置的呼叫手机号码
#define CMD_GSM_FUNC_SEARCH		0X36	//GSM模块功能状态查询
#define CMD_GSM_FUNC_SET		0X37	//GSM模块功能设置
#define CMD_RS485_BRT_SET		0X38	//RS485总线波特率设置

#define CMD_OPEN_LCD			0x3e	/*打开显示命令*/
#define CMD_CLOSE_LCD			0x3f	/*关闭显示命令*/
/*****************************************************************/
#define CMD_OPEN_485BUS_IN		0x41	/*打开485输入命令*/
#define CMD_CLOSE_485BUS		0x42	/*关闭485命令*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE		0x43	/*清除手柄液晶信息命令*/
#define CMD_KEY_AVALID			0x44	/*按键有效命令*/
#define CMD_KEY_INVALID			0x45	/*按键失效命令*/
#define CMD_KEY_DOWN			0x46	/*按键压下命令*/
#define CMD_KEY_UP				0x47	/*按键释放命令*/
#define CMD_ERROR_REPORT		0x48	/*故障报告命令*/
#define CMD_CLEAR_LCD			0x49	/*清除液晶信息命令*/
#define CMD_RECEIVE_OK			0x4a	/*收到信息命令*/
#define CMD_RECEIVE_ERROR		0x4b	/*选择信息命令*/

#define CMD_NUMBER_SET		    0x5a	//编号命令
#define CMD_SELF_SET_NUMBER		0x5b	//自己编号命令
#define CMD_SYSTEM_SEC_SET		0X5B	//系统区号设置

#define CMD_GET_BUS				0x71	/*占用总线命令*/
#define CMD_QUEST				0x73	/*查询命令*/
#define CMD_QUEST_ANSWER		0x74	/*查询应答命令*/
#define CMD_TALK_CLEAR			0X75	/*结束通话命令*/
/*****************************************************************/
#define CMD_DISPLAY_DATA		0x80	/*发送一帧液晶屏数据*/
#define CMD_DATA_ERROR			0x81	/*一帧数据出错命令*/

#define CMD_NURSE_IN			0X83	//护士到位命令
#define CMD_NURSE_OUT			0X84	//护士离开命令

#define CMD_SUPPLY_OX_START		0X85	//供氧开始
#define CMD_SUPPLY_OX_END		0X86	//供氧结束

#define	CMD_MAIN_MENU_SET		0x87	//系统菜单操作
#define CMD_MAIN_MENU_ESC		0x88	//退出系统菜单操作

#define CMD_VOICE_TIMER_SET		0x89	//小音段时段设置
#define CMD_BL_TIMER_SET		0x8a	//背光关时段设置
#define CMD_VOL_VAL_SET		    0x8b	//音量信息设置

#define CMD_VOICE_TIMER_GET		0x90	//获取小音段时间
#define CMD_BL_TIMER_GET		0x91	//获取背光关时间段
#define CMD_WARD_SEC_INFO_SET	0x92	//病区信息设置
#define CMD_VOL_VAL_GET			0X93    //获取音量信息
 
/*****************************************************************/

#ifdef _IN_SINGLE_BUS_
//设备状态定义
uint8 	bdata 	byDevState1;
sbit  	bBusy 				= 	byDevState1^7;				//系统忙标志
sbit	bSetKeyDown			=	byDevState1^6;				//设置键按下状态
sbit	bDealKeyDown		=	byDevState1^5;				//处理键按下状态
sbit	bHandleDown			=	byDevState1^4;	 			//护士到位标志
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
sbit	bKeyAnswer			=	byDevState3^4;				//按确认键接听
sbit	bRoomAddr			=	byDevState3^3;				//按键呼叫房地址更新了
sbit	bHandAnswer			=	byDevState3^2;				//听筒接听
sbit	bPWState			=	byDevState3^1;				//免提功放控制
sbit	bPW2State			=	byDevState3^0;			   	//听筒功放控制
uint8 	bdata 	byDevState4;
sbit	bSetPrio			=	byDevState4^7;				//设置护理优先级
sbit	bCallDealSending	=	byDevState4^6;				//呼叫处理正在发送	 
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
#else
extern	uint8 	byDevState1;
extern	uint8 	byDevState2;
extern	uint8 	byDevState3;
extern	uint8 	byDevState4;
extern 	uint8 	byEnable1;
extern	uint8 	byEnable2;
extern	bit  	bBusy;
extern 	bit		bSetKeyDown;
extern	bit		bDealKeyDown;
extern	bit		bHandleDown;
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
extern	bit		bKeyAnswer;
extern	bit		bRoomAddr;
extern	bit		bHandAnswer;
extern	bit		bPWState;
extern	bit		bPW2State;
extern  bit		bSetPrio;
extern  bit		bCallDealSending;	
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


extern bit  fled0;
extern bit  bKeyEnterDown;
extern bit  bKeyBroadDown;
extern bit  bVl0State;


extern uint8 xdata byBus0RxQ[];						//总线0接收队列
extern uint8 xdata byBus0RxHead ;								//单总线0接收队列头指针
extern uint8 xdata byBus0RxTail ; 								//单总线0接收队列尾指针


extern uint8 xdata byBus1RxQ[];						//总线1接收队列
extern uint8 xdata byBus1RxHead ;								//单总线1接收队列头指针
extern uint8 xdata byBus1RxTail ; 								//单总线1接收队列尾指针


void SingleBusInit(void); 	
uint8 Bus0OutputData(uint8* pbyData);  
uint8 Bus1OutputData(uint8* pbyData); 


void VoiceChannelCtx(void);
void InitKeyCallAddr(void);
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void Bus0RecDeal(void);
void Bus0SendDeal(void);
void Bus0Manage(void);
void Bus1RecDeal(void);
void Bus1SendDeal(void);
void Bus1Manage(void);
void TimerOutDeal(void);
void TimerOutManager(void);
void IdleTask(void);
void SaveParameter(uint16 addr);
void InitParameter(void);


extern void led0flash(void);
extern void led0init(void);

extern uint8 Bus0RecData(uint8* pbyData);
extern uint8 Bus1RecData(uint8* pbyData);
extern void AddPlayVoiceVol(uint8 xdata *pbyData);


#endif
