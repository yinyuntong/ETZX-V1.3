#ifndef _IN_UART_
#define _IN_UART_

#define UART0_BUF_SIZE		100								//串口缓冲区大小定义
typedef struct
{
	INT8U 	byState;
	INT8U	byRxHead;
	INT8U 	byRxTail;
	INT8U   byCrc;
	INT8U   byTimerout;		
	INT8U 	uiLen;
} STUartControl, *pSTUartControl;
typedef struct
{
	INT8U	byStartByte1;	 								//起始字符1
	INT8U 	bySndAreaAddr;									//发送方区地址
	INT8U	bySndRoomAddr;	 								//发送方房地址
	INT8U   bySndBedAddr;									//发送房床地址 
	INT8U 	byRecAreaAddr;									//接收方区地址
	INT8U	byRecRoomAddr;									//接收方房地址
	INT8U   byRecBedAddr;									//接收房床地址
	INT8U	byCmd;											//命令代码
	INT8U	byLenh;										    //数据长度
	INT8U	byLen;										    //数据长度
	INT8U	byData[1];										//链路数据
} STUartFreq, *pSTUartFreq;
typedef struct
{
	INT8U byDataType;										//数据类型，0x01：国标码
	INT8U byPagePosition;									//屏的位置
	INT8U byPageType;										//数据屏的类型,0x01:床头卡,0x02:费用清单,0x03:药品清单
	INT8U byLinePosition;									//行的位置
	INT8U byEndFlag;										//页结束标志，0x26:没有结束，0x28:结束
} STLcdPageData, *pSTLcdPageData;
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
#endif

