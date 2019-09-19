
#ifndef _IN_SINGLE_BUS_
#define _IN_SINGLE_BUS_

#define RELEASE	 1

//总线0宏定义
#define 	BUS0_FREQ_SIZE		7							//单总线0数据帧长度
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*4 + 1)		//单总线0数据队列长度
#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*4 + 1)		//单总线0数据队列长度(接收)

#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c0000
#define 	BUS0_ON_WORK		0x23
//单总线自动重载时间
//#define		TIMER0_COUNT		(100us)((256-(Fbus*9375/12/1000000))) 
//---------------------------------------------------------------
//---------------------------------------------------------------
//#define     TIMER0_H  0xFE
//#define     TIMER0_L  0x34
#define     TIMER0_H  0xFF
#define     TIMER0_L  0x67
//---------------------------------------------------------------
 
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
	INT8U bySndSecAddr;			/*主动发送方区地址*/
	INT8U bySndRoomAddr;		/*主动发送方房地址*/
	INT8U bySndBedAddr;			/*主动发送方床地址*/
	INT8U byCmd;				/*命令*/
	INT8U byRecSecAddr;			/*接收方区地址*/
	INT8U byRecRoomAddr;		/*接收方房地址*/
	INT8U byRecBedAddr;			/*接收方床地址*/	
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
#define CMD_LCD_SEND			0x1a	/*屏数据发送命令*/
#define CMD_SCHEDULE_SEND		0x1b	/*一览表发送命令*/
#define CMD_NUMBER_SET			0x1c	/*修改编号命令*/		/*新的地址在前2个字节，被修改地址在后两个字节*/
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
#define	CMD_OPEN_485BUS_IN_ACK	0x34	//从机打开485输入命令
#define	CMD_OPEN_485BUS_OUT_ACK	0x35	//从机打开485输出命令
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
#define CMD_RECEIVE_OK			0x4a	/*收到信息命令*/
#define CMD_RECEIVE_ERROR		0x4b	/*选择信息命令*/
#define CMD_NO_BUG				0x4c	/*请勿打扰命令*/
#define CMD_NO_BUG_AVALID		0x4d	/*请勿打扰有效命令*/
#define CMD_BUG_END				0x4e	/*结束打扰命令*/

#define CMD_WRITE_SERIAL		0x55	/*写入序列号*/
#define CMD_READ_SERIAL			0x56	/*读序列号*/
#define CMD_OUT_SERIAL			0x57	/*序列号输出*/
#define CMD_SET_BY_SERIAL		0x5a	/*根据序列号写设备地址*/
#define CMD_READ_BY_SERIAL		0x5b	/*根据序列号读设备地址*/
#define CMD_OUT_BY_SERIAL		0x5c	/*根据序列号读设备地址输出*/

#define	CMD_LED_INDICATE		0x70	/*灯指示命令,发送方地址无效，区地址为灯状态*/
#define CMD_GET_BUS				0x71	/*占用总线命令*/
//--------------------------------------------------------------
#define CMD_DISPLAY_DATA		0x80	/*发送一帧液晶屏数据*/
#define CMD_DATA_ERROR			0x81	/*一帧数据出错命令*/
#define CMD_NURSE_COME		    0x83	/*护士到位命令*/
#define CMD_NURSE_BACK		    0x84	/*护士离开命令*/
//--------------------------------------------------------------
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
#define CMD_SET_BAUD	        0x38	/*设置系统波特率命令*/ 



//发送和接收引脚定义

#ifdef RELEASE
#define Bus0RecPin (1<<11)	//P1.11									//单总线0接收引脚定义
#define GetBus0RecPin() (LPC_GPIO1->DATA&Bus0RecPin)
#else
#define Bus0RecPin (1<<6)	//P2.6									//单总线0接收引脚定义
#define GetBus0RecPin() (LPC_GPIO2->DATA&Bus0RecPin)
#endif
#define Bus0SendPin	(1<<4)  //P1.4									//单总线0发送引脚定义
#define SetBus0SendPin() (LPC_GPIO1->DATA|=Bus0SendPin)
#define ClrBus0SendPin() (LPC_GPIO1->DATA&=~Bus0SendPin)

#ifdef RELEASE
#define 	DisableBus0RecInt()	{LPC_GPIO1->IE&=~Bus0RecPin;} 					//单总线0禁止总线中断宏定义
#define 	EnableBus0RecInt()	{LPC_GPIO1->IC|=Bus0RecPin;LPC_GPIO1->IE|=Bus0RecPin;}			//单总线0恢复总线中断宏定义
#else
#define 	DisableBus0RecInt()	{LPC_GPIO2->IE&=~Bus0RecPin;} 					//单总线0禁止总线中断宏定义
#define 	EnableBus0RecInt()	{LPC_GPIO2->IC|=Bus0RecPin;LPC_GPIO2->IE|=Bus0RecPin;}			//单总线0恢复总线中断宏定义
#endif

//隔离控制引脚定义
#define ProtectPin (1<<8)  //P2.8
#define SetProtectPin() (LPC_GPIO2->DATA|=ProtectPin);
#define ClrProtectPin() (LPC_GPIO2->DATA&=~ProtectPin);



extern void SingleBusInit(void); 	
extern INT8U Bus0OutputData(INT8U* pbyData);  
extern void UartDeal(void);
extern void MakeCH0TimerOut(INT8U byTimerOut, INT8U byTimerOutCount);
extern void Bus0RecDeal(void);
extern void Bus0SendDeal(void);
extern void Bus0Manage(void);
extern void TimerOutDeal(void);
extern void TimerOutManager(void);

#endif

