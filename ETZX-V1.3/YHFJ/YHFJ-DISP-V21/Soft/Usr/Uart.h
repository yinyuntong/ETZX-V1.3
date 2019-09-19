/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Uart.c
*文件描述    		:串口驱动程序
*创建人     		:尹运同
*创建日期   		:2009-4-6
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
//定义串口数目
#define MAX_COM		1

#define	UART0_RX_BUF_SIZE	(128+EXT_SIZE)

typedef struct
{
	uint8  xdata *pbyRecBuf;		/*接收缓冲区*/
	uint8  byRecState;		/*接收过程中所处的阶段*/
	uint16 uiRecPosition;	/*数据存放位置*/
	uint16 uiRecLen;		/*数据域的数据个数*/
	uint8  byRecCount;		/*处所阶段应接收该阶段数据的个数*/
	uint8  byRecTimeout;	/*接收过程中的超时计数*/
} STUsartCfg, pSTUsartCfg;



typedef struct
{
	uint8	byStartByte1;					//数据帧起始字符1
	uint8 	bySndSecAddr;					//收方区地址
	uint8 	bySndRoomAddr;					//接收方房地址
	uint8 	bySndBedAddr;					//接收方床地址	
	uint8 	byRecSecAddr;					//主动发送方区地址
	uint8 	byRecRoomAddr;					//主动发送方房地址
	uint8 	byRecBedAddr;					//主动发送方床地址
	uint8 	byCmd;							//命令
	uint16  uiLen;							//数据长度，2个字节
	uint8   byData[1];						//链路数据	
} STUartFreq, *pSTUartFreq;


//标示符定义
#define		START_BYTE1			0x5a
#define		END_BYTE			0x16

#define 	DOWN_START_BYTE1	0x46
#define 	DOWN_START_BYTE2	0XB9
#define		DOWN_ID1			0X6A
#define		DOWN_ID2			0X68
#define 	DOWN_END_BYTE		0X16

#define 	CMD_POSITION		0X07
#define 	LEN_HIGH_POSITION	0x08
#define 	LEN_LOW_POSITION	0x09
#define 	DATA_POSITION0		0x0a
#define 	DATA_POSITION1		0x0B
#define 	DATA_POSITION2		0x0C
#define 	DATA_POSITION3		0x0D
#define 	DATA_POSITION4		0x0E
#define 	DATA_POSITION5		0x0F
//串口1控制引脚
#define		RE_485				0
#define		DE_485				1
//长度标示符定义
#define		HEAD_SIZE			9
#define		EXT_SIZE			12
#define		MAX_UART0_DATA_LEN	(UART0_RX_BUF_SIZE-EXT_SIZE)


#ifndef _IN_UART_
#define _IN_UART_
extern STUsartCfg  xdata stUsartCfg;
extern uint8	xdata byUsart0RecBuf[];

extern bit bUsart0RecFinish;
extern bit bRS485DataError;


extern void UsartInit(void);
extern void Usart0RecDeal(void);
extern void UsartManager(void);
extern uint8 GetPskCrc(STUartFreq *pstDataFreq);
extern uint8 CheckPsk(pSTUartFreq pstDataFreq);
#endif