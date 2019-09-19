/*
**********************************Copyright (c) ****************************
*	  						cwg
*			       All Rights Reserved
*			     		QQ:7084394
*
*---------------------------------File Info---------------------------------
*File Name     		:SingleBus.h
*Descriptor    		:单总线头文件
*Create By     		:陈卫国
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
//总线0宏定义
#define 	BUS0_FREQ_SIZE		7
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*20 + 1)
#define 	DisableBus0RecInt()	{EX1 = 0;} 
#define 	EnableBus0RecInt()	{IE1 = 0;EX1 = 1;}
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c 
#define 	BUS0_ON_WORK		0x23
//单总线自动重载时间
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000))) 

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34

//100uS
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
#define CMD_QUEST				0x01	/*查询命令*/
#define CMD_ANSWER				0x02	/*应答命令*/
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
#define CMD_TIME_SEND			0x1a	/*时间发送命令*/
#define CMD_SCHEDULE_SEND		0x1b	/*一览表发送命令*/
#define CMD_NUMBER_SET			0x1c	/*修改编号命令*/		/*新的地址在前2个字节，被修改地址在后两个字节*/
#define CMD_LAMP_CONTROL		0x1d	/*灯控命令*/
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
#define CMD_STOP_INDICATION		0x2e	/*终止信息指示*/
#define CMD_INFO_INDICATION		0x2f	/*信息指示命令*/
/*主动呼叫方地址在前两个字节，后面两个字节为呼叫类型：高3位优先级，低5位为类别*/
/*****************************************************************/
#define CMD_MUSIC				0x30	/*音乐命令*/
#define CMD_INCREACE_VOL		0x31	/*音量增加命令*/
#define CMD_DECREACE_VOL		0x32	/*音量减小命令*/

#define CMD_VOICE_PLAY_ROOM		0x3a	//语音报号是否播报房号


#define CMD_OPEN_LCD			0x3e	/*打开显示命令*/
#define CMD_CLOSE_LCD			0x3f	/*关闭显示命令*/
/*****************************************************************/
#define CMD_OPEN_485BUS_OUT		0x40	/*打开485输出*/
#define CMD_OPEN_485BUS_IN		0x41	/*打开485输入命令*/
#define CMD_CLOSE_485BUS		0x42	/*关闭485命令*/
/*****************************************************************/
#define CMD_KEY_AVALID			0x44	/*按键有效命令*/
#define CMD_KEY_INVALID			0x45	/*按键失效命令*/
#define CMD_KEY_DOWN			0x46	/*按键压下命令*/
#define CMD_KEY_UP				0x47	/*按键释放命令*/
#define CMD_ERROR_REPORT		0x48	/*故障报告命令*/
#define CMD_CLEAR_LCD			0x49	/*清除液晶信息命令*/
#define CMD_RECEIVE_INFO		0x4a	/*收到信息命令*/
#define CMD_SELECT_INFO			0x4b	/*选择信息命令*/
#define CMD_NO_BUG				0x4c	/*请勿打扰命令*/
#define CMD_NO_BUG_AVALID		0x4d	/*请勿打扰有效命令*/
#define CMD_BUG_END				0x4e	/*结束打扰命令*/


#define  CMD_STOP_VOICE_PLAY	0x90	//语音报号结束命令

/*****************************************************************/  
#ifdef _IN_SINGLE_BUS_
//设备状态定义
uint8 	bdata 	byDevState1;
sbit	bLanding 	= 	byDevState1^0;				//上电登记状态	
#else
extern	uint8	byDevState1;
extern	bit		bLanding;
void SingleBusInit(void); 
void ReadParameter(void);	
void InitParameter(void);
void Bus0OutputData(uint8* pbyData);  
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);
void Bus0RecDeal(void);
void Bus0SendDeal(void);
void Bus0Manage(void); 
void TimerOutDeal(void);
void TimerOutManager(void);
#endif
