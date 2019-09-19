
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
#define MAX_COM		2

#define USE_COM0   1
#define USE_COM1   1

#define BRT1	1200
#define BRT2	2400
#define BRT3	4800
#define BRT4	9600
#define BRT5	14400
#define BRT6	19200
#define BRT7	28800
#define BRT8	38400
#define BRT9	57600
#define BRT10	115200


#define	UART0_TX_BUF_SIZE	(128+EXT_SIZE)		//串口0只要发送



#define UART1_RX_BUF_SIZE   	(128+EXT_SIZE)
#define	UART1_TX_BUF_SIZE		(8+EXT_SIZE)
#define	UART1_RX_DEAL_BUF_SIZE	(128+EXT_SIZE)


#define IncRxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiRecBufSize)
#define IncTxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiSndBufSize)

//串口结构体定义
typedef struct
{
	uint8  xdata *pbyRecBuf;		/*接收缓冲区*/
	uint8  xdata *pbySndBuf;		/*发送缓冲区*/
	uint8  byRecState;		/*接收过程中所处的阶段*/
	uint16 uiRecPosition;	/*数据存放位置*/
	uint16 uiRecLen;		/*数据域的数据个数*/
	uint8  byRecCount;		/*处所阶段应接收该阶段数据的个数*/
	uint8  byRecTimeout;	/*接收过程中的超时计数*/

	uint16 uiSndLen;		/*发送数据长度*/
	uint16 uiSndPos;		/*发送数据位置*/	
} STUsartCfg, pSTUsartCfg;



typedef struct
{
	uint8	byStartByte1;					//数据帧起始字符1
	uint8 	bySndSecAddr;					//主动发送方区地址
	uint8 	bySndRoomAddr;					//主动发送方房地址
	uint8 	bySndBedAddr;					//主动发送方床地址	
	uint8 	byRecSecAddr;					//收方区地址
	uint8 	byRecRoomAddr;					//收方房地址
	uint8 	byRecBedAddr;					//收方床地址
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


#define 	START_POSITION		0X00
#define 	SND_SEC_POSITION	0X01
#define 	SND_ROOM_POSITION   0X02
#define 	SND_BED_POSITION	0X03
#define 	REC_SEC_POSITION	0X04
#define 	REC_ROOM_POSITION   0X05
#define 	REC_BED_POSITION	0X06
#define 	CMD_POSITION		0X07
#define 	LEN_HIGH_POSITION	0X08
#define 	LEN_LOW_POSITION	0X09
#define		DATA0_POSITION		0X0A
#define		DATA1_POSITION		0X0B
#define		DATA2_POSITION		0X0c
#define		DATA3_POSITION		0X0d
#define		DATA4_POSITION		0X0e
#define		DATA5_POSITION		0X0f

#define		CMD_ERROR_PC		0x00

//串口错误代码
#define 	USART_ENTER_CODE0   0X00	//校验和错
#define 	USART_ENTER_CODE1   0X01	//一帧数据正确
#define 	USART_ENTER_CODE2   0X02	//长度超限
#define 	USART_ENTER_CODE3   0X03	//长度到了但最后一字符不是帧尾字符
#define 	USART_ENTER_CODE4   0X04	//前一帧数据处理还未完成


//串口1控制引脚
#define		RE_485				0
#define		DE_485				1
//长度标示符定义
//#define		HEAD_SIZE			10
//#define		EXT_SIZE			13
//#define		MAX_UART0_DATA_LEN	(UART0_RX_BUF_SIZE-14)
#define		HEAD_SIZE			9
#define		EXT_SIZE			12
#define		MAX_UART1_DATA_LEN	(UART1_RX_BUF_SIZE-12)

#define 	RELOAD_COUNT		0xfd	//fosc=11.0592M,12T,smod=0x00;9600bps


#ifndef _IN_UART_
#define _IN_UART_
extern STUsartCfg  xdata stUsartCfg[];

//串口0配置   与下位机通迅
extern uint8	xdata byUsart0SndBuf[];

//串口1配置	   与上位机通迅
extern uint8	xdata byUsart1RecBuf[];
extern uint8	xdata byUsart1SndBuf[];
//extern uint8   xdata byUsart1RecDealBuf[];


void Usart0Init(uint16 uiRs485Brt);
void Usart1Init(void);
void Usart1RecDeal(void);
void Usart1SndCmd(uint8 Cmd,uint8 byLen,uint8 *pData);
void Usart1SndEnter(uint8 byOriginalCmd,uint8 byData);
void Usart1SndResult(uint8 byOriginalCmd,uint8 byData);
void Usart1SndRS485Result(uint8 byOriginalCmd,uint8 byData);
void Usart1SndResult1(uint8 byOriginalCmd,uint8 byData0,uint8 byData1);
void Usart1SndResult3(uint8 byOriginalCmd,uint8 byData0,uint8 *byData,uint8 byLen);
void Usart1SndError(uint8 byData);
uint8 GetPskCrc(pSTUartFreq pstDataFreq);
uint8 CheckPsk(pSTUartFreq pstDataFreq);
void Usart1SndData(pSTBusFreq pstBusFreq);

void Usart1Sendbyte(uint8 dat);
void uart1_send_byte(unsigned char SDATA);
void uart0_send_byte(unsigned char SDATA);

extern bit bUsart1RecFinish;
extern bit bRS485DataTreat;

extern uint8 errtime;
#endif