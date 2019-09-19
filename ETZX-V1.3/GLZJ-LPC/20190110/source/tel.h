#ifndef _TEL_H_
#define _TEL_H_

extern uint16_t TelState;								//电话处理相关标志
#define  bTelRing			(1<<1)				//电话机振铃状态
#define  bTelRingOut		(1<<2)				//电话振铃输出高低电平
#define  bTelTlpEnChk		(1<<3)				//电话机TLP允许检测
#define  bTelHungDowned		(1<<4)				//电话机挂机动作
#define  bTelPickUped		(1<<5)				//电话机摘机动作
#define  bOriTelTlp			(1<<6)				//原tlp状态
#define  bCID				(1<<7)				//来电显示状态
#define  bCID_CMD			(1<<8)				//发送CID命令
#define  bCIDOccupy			(1<<9)				//CID　信通占用信号
#define  bCIDMarks			(1<<10)				//CID	标志信号
#define  bCIDData			(1<<11)				//CID 数据:包括消息类型、长度、消息体、校验
#define  bPhoneRingState		(1<<12)				//芯片TRING 口线状态 

#define SetTelState(x) (TelState|=(x))			//置位操作
#define ClrTelState(x) (TelState&=~(x))			//清零操作
#define GetTelState(x) (TelState&(x))			//获取位状态




#define TEL_PICKUP_TIME		20000		//10秒提示

#ifndef _IN_TEL_
extern uint16_t tel_ring_time;			//电话振铃时音
extern uint16_t tel_tlp_dis_chk;			//禁止检测时间
extern uint16_t tel_tlp_chk_delay;		//电话检测延时时间
extern uint16_t CID_marks_time ;			//标志信号延时时间
extern uint16_t CID_txe_delay;			//发送允许延时，时间到发送禁止
extern uint8_t  cid_data[];
extern uint8_t		phone_key_buf[];
extern uint16_t    tel_pickup_time;


extern void start_tel_ring(void);
extern void stop_tel_ring(void);
extern void init_tel(void);
extern void init_msm7512(void);
extern send_msm7512_test(void);
extern uint8_t make_msm7512_data(uint8_t room,uint8_t bed);
extern void init_mt8870(void);
extern void init_phone_key_buf(void);
extern void mt8870_received_treat(void);
extern void tel_pickup_treat(void);
extern void tel_hungdown_treat(void);

extern void AM79R70_pincfg(void);
extern void AM79R70_active(void);
extern void AM79R70_standby(void);
extern void AM79R70_ringing(void);


#endif





#endif


