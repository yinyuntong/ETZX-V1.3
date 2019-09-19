#ifndef _UART_H_
#define _UART_H_


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


#define	UART0_TX_BUF_SIZE		(256+EXT_SIZE)		//串口0只要发送RS485数据
#define UART0_RX_BUF_SIZE		64					//调试用

#define	UART1_RX_BUF_SIZE		1024				//与WIFI模块通迅
#define UART1_TX_BUF_SIZE		30


#define UART2_RX_BUF_SIZE   	(200+EXT_SIZE)	//串口2用于上位机通迅
#define	UART2_TX_BUF_SIZE		(8+EXT_SIZE)

#define UART3_RX_BUF_SIZE   	10					//与来电显示芯片通迅
#define UART3_TX_BUF_SIZE		30


#define IncRxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiRecBufSize)
#define IncTxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiSndBufSize)


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
#define		HEAD_SIZE			9
#define		EXT_SIZE			12
#define		MAX_UART2_DATA_LEN	(UART2_RX_BUF_SIZE-12)

#define 	RELOAD_COUNT		0xfd	//fosc=11.0592M,12T,smod=0x00;9600bps


#ifndef _IN_UART_

//串口变量定义
extern STUsartCfg  stUsartCfg[];

//串口0配置   与下位机通迅
extern uint8_t	byUsart0SndBuf[];

extern uint8_t	byUsart1SndBuf[];

//串口2配置	   与上位机通迅
extern uint8_t	byUsart2SndBuf[];

extern uint8_t	byUsart3SndBuf[];


extern volatile uint8_t		scon0_Rcvbuf[];
extern volatile uint8_t		scon1_Rcvbuf[];
extern volatile uint8_t		scon2_Rcvbuf[];
extern volatile uint8_t		scon3_Rcvbuf[];


extern volatile uint8_t		scon0_time;
extern volatile uint8_t		scon0_tov;
extern volatile uint16_t	scon0_received;
extern volatile uint16_t	scon0_len;
extern volatile uint8_t		*scon0_ptr;
extern volatile uint8_t		scon0_GucRcvNew;


extern volatile uint8_t		scon1_time;
extern volatile uint8_t		scon1_tov;
extern volatile uint16_t		scon1_received;
extern volatile uint16_t		scon1_len;
extern volatile uint8_t		*scon1_ptr;
extern volatile uint8_t		scon1_GucRcvNew;



extern volatile uint8_t		scon2_time;
extern volatile uint8_t		scon2_tov;
extern volatile uint16_t		scon2_received;
extern volatile uint16_t		scon2_len;
extern volatile uint8_t		*scon2_ptr;
extern volatile uint8_t		scon2_GucRcvNew;


extern volatile uint8_t		scon3_time;
extern volatile uint8_t		scon3_tov;
extern volatile uint16_t		scon3_received;
extern volatile uint16_t		scon3_len;
extern volatile uint8_t		*scon3_ptr;
//extern volatile uint8_t		uart3_auto_send;
extern volatile uint8_t		uart3_auto_time;



extern void init_uart0(uint32_t);
extern void init_uart1(uint32_t);
extern void init_uart2(uint32_t);
extern void init_uart3(uint32_t);
extern void uart0_send_byte (uint8_t);
extern void uart1_send_byte (uint8_t);
extern void uart2_send_byte (uint8_t);
extern void uart3_send_byte (uint8_t);
extern void uart0_send_str (uint8_t *);
extern void uart1_send_str (uint8_t *);
extern void uart1_send_str_len (uint8_t *pucStr,uint16_t len);
extern void uart2_send_str (uint8_t *);
extern void uart3_send_str (uint8_t *);
extern void init_scon0_Rcvbuf(void);
extern void Usart2RecDeal(void);
extern void Usart2SndCmd(STAddr_S sur_addr,uint8_t Cmd,uint8_t byLen,uint8_t *pData);
extern void uart0_send_str2 (uint8_t *pucStr,uint16_t len);
extern void uart2_send_str2 (uint8_t *pucStr,uint16_t num);
extern uint8_t GetPskCrc(pSTUartFreq pstDataFreq);
extern uint8_t CheckPsk(pSTUartFreq pstDataFreq);

#endif


#endif
