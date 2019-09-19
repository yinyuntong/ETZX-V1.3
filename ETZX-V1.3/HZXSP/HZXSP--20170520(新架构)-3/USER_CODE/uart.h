#ifndef _IN_UART_
#define _IN_UART_

#define UART0_BUF_SIZE		(MESG_BUF_LEN+7+12)//串口缓冲区大小定义
typedef struct
{
	INT8U 	byState;
	INT8U	byRxHead;
	INT8U	byDealHead;										//一帧待处理数据的头部
	INT8U 	byRxTail;
	INT8U   byCrc;
	INT8U   byTimerout;		
	INT8U 	uiLen;
} STUartControl, *pSTUartControl;



typedef struct
{
	INT8U byDataType;										//数据类型，0x01：国标码
	INT8U byInfoType;										//数据屏的类型,0x01:床头卡,0x02:费用清单,0x03:药品清单
	INT8U byFrameID;										//帧号
	INT8U byEndFlag;										//页结束标志，0x00:没有结束，0x01:结束
} STLcdCtlData, *pSTLcdCtlData;


typedef struct
{
	INT8U	byStartByte1;					//数据帧起始字符1
	INT8U 	bySndSecAddr;					//主动发送方区地址
	INT8U 	bySndRoomAddr;					//主动发送方房地址
	INT8U 	bySndBedAddr;					//主动发送方床地址	
	INT8U 	byRecSecAddr;					//收方区地址
	INT8U 	byRecRoomAddr;					//收方房地址
	INT8U 	byRecBedAddr;					//收方床地址
	INT8U 	byCmd;							//命令
	//INT16U  uiLen;							//数据长度，2个字节
	INT8U   byLenHigh;						//长度高字节
	INT8U	byLenLow;						//长度低字节
	INT8U   byLcdSec;						//走廊显示屏区号
	INT8U   byLcdRoom;						//走廊显示屏房号
	INT8U   byNull;							//未用
	STLcdCtlData stLcdCtlData;			//走廊显示屏页面控制数据
	INT8U   byData[1];						//链路数据	
} STUartFreq, *pSTUartFreq;


#define FREQ_DATA_LEN			19//16						//整个链路数据长度
#define FREQ_HEAD_SIZE			10							//数据帧包头长度
#define START_BYTE1				0x5a
#define	START_BYTE2				0xa5
#define END_BYTE				0x16  


extern STUartControl stUartControl;								//串口控制结构体

extern void UartInit(INT8U baud);
extern void UartReInit(INT8U baud); 
extern void UartManager(void);
extern void uartSendByte (INT8U ucDat);
extern INT8U CheckPsk(pSTUartFreq pstDataFreq);
#endif

