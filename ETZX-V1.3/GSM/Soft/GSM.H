
#define AFFIRM_UART 			0X01			//确认GSM串口
#define LOOP_DISP_OFF 			0X02			//回显关闭
#define PIN1_STATE_QUERY 		0X03			//SIM卡在位和PIN1码状态查询
#define SIGNAL_INTENSION_QUERY 	0X04			//查询信号强度
#define COPS_QUERY				0X05			//网络运营商注册查询
#define GPRS_LOGIN_QUERY		0X06			//GPRS网络注册查询


#define ATI_QUERY				0X07			//模块信息查询


#define MEM_STATE_QUERY			0X10			//查询存储器状态


#define COMM_OK					0X01			//普通OK
#define TEL_NO_HANDLE			0X02			//电话未接听
#define TEL_HAND_UP				0X03			//电话挂断
#define TEL_HANDLE				0X04			//电话接听
#define GSM_CALL_OK				0X05			//GSM主动呼叫成功
#define COPS_OK					0X06			//网络注册成功
#define CPIN_READY				0X07			//SIM卡插入
#define TEL_IGNORE				0X08			//电话忽略
#define TEL_BUSY				0X09			//电话忙



#ifndef _IN_GSM_
extern void AffirmUartCom(void);
extern void LoopDispOff(void);
extern void LoopDispOn(void);
extern void SIMStateQuery(void);
extern void SignalIntensionQuery(void);
extern void NetOPSQuery(void);
extern void GSMModuleInfoQuery(void);
extern void GSMUartAutoBrtSet(void);
extern void GSMUartBrtSet(uint16 UartBrt);
extern void GSMPathQuery(void);
extern void EchoRestrainQuery(void);
extern void EchoRestrainSet(uint8 Parameter);
extern void GSMSWPathSet(uint8 path);
extern void GSMSPKVoiceSet(uint8 Voice);
extern void GSMMICGainSet(uint8 Gain);
extern void GSMCall(void);
extern void GSMHandUp(void);
extern void GSMInit(void);
extern void GSMRestar(void);
extern uint8 GSMCallTreat(void);
extern uint8 GSMHandUpTreat(void);
#endif