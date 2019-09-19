
/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Uart.c
*�ļ�����    		:������������
*������     		:����ͬ
*��������   		:2009-4-6
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
//���崮����Ŀ
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


#define	UART0_TX_BUF_SIZE	(128+EXT_SIZE)		//����0ֻҪ����



#define UART1_RX_BUF_SIZE   	(128+EXT_SIZE)
#define	UART1_TX_BUF_SIZE		(8+EXT_SIZE)
#define	UART1_RX_DEAL_BUF_SIZE	(128+EXT_SIZE)


#define IncRxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiRecBufSize)
#define IncTxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiSndBufSize)

//���ڽṹ�嶨��
typedef struct
{
	uint8  xdata *pbyRecBuf;		/*���ջ�����*/
	uint8  xdata *pbySndBuf;		/*���ͻ�����*/
	uint8  byRecState;		/*���չ����������Ľ׶�*/
	uint16 uiRecPosition;	/*���ݴ��λ��*/
	uint16 uiRecLen;		/*����������ݸ���*/
	uint8  byRecCount;		/*�����׶�Ӧ���ոý׶����ݵĸ���*/
	uint8  byRecTimeout;	/*���չ����еĳ�ʱ����*/

	uint16 uiSndLen;		/*�������ݳ���*/
	uint16 uiSndPos;		/*��������λ��*/	
} STUsartCfg, pSTUsartCfg;



typedef struct
{
	uint8	byStartByte1;					//����֡��ʼ�ַ�1
	uint8 	bySndSecAddr;					//�������ͷ�����ַ
	uint8 	bySndRoomAddr;					//�������ͷ�����ַ
	uint8 	bySndBedAddr;					//�������ͷ�����ַ	
	uint8 	byRecSecAddr;					//�շ�����ַ
	uint8 	byRecRoomAddr;					//�շ�����ַ
	uint8 	byRecBedAddr;					//�շ�����ַ
	uint8 	byCmd;							//����
	uint16  uiLen;							//���ݳ��ȣ�2���ֽ�
	uint8   byData[1];						//��·����	
} STUartFreq, *pSTUartFreq;
//��ʾ������
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

//���ڴ������
#define 	USART_ENTER_CODE0   0X00	//У��ʹ�
#define 	USART_ENTER_CODE1   0X01	//һ֡������ȷ
#define 	USART_ENTER_CODE2   0X02	//���ȳ���
#define 	USART_ENTER_CODE3   0X03	//���ȵ��˵����һ�ַ�����֡β�ַ�
#define 	USART_ENTER_CODE4   0X04	//ǰһ֡���ݴ���δ���


//����1��������
#define		RE_485				0
#define		DE_485				1
//���ȱ�ʾ������
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

//����0����   ����λ��ͨѸ
extern uint8	xdata byUsart0SndBuf[];

//����1����	   ����λ��ͨѸ
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