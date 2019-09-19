//////////////////////
//总线0变量定义

#define 	BUS0_FREQ_SIZE		7						   	//单总线0数据帧长度
typedef     unsigned char   uint8;
extern uint8 bdata byBus0State1 ;//	= 0;  							//单总线0状态字2
extern bit  bBus0RecFinish;//		= byBus0State1^0;				//单总线0接收完成标志
//sbit  bBus0RecBit9			= byBus0State1^1;				//单总线0接收字节数据的第9位
extern bit  bBus0SendFinish;		//= byBus0State1^2;				//单总线0发送完成标志
//sbit  bBus0Disable			= byBus0State1^3;				//单总线0禁止发送标志
//sbit  bBus0SendBit			= byBus0State1^4;				//单总线0正在发送的数据位
//sbit  bBus0RecBit			= byBus0State1^5; 				//单总线0正在接收的数据位
extern uint8 xdata byBus0RecData[BUS0_FREQ_SIZE];					//总线0接收缓冲区
extern void SingleBusInit(void);
extern uint8 Bus0OutputData(uint8* pbyData);
//////////////////////