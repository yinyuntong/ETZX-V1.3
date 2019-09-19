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
#define MAX_COM		1

#define	UART0_RX_BUF_SIZE	(128+EXT_SIZE)

typedef struct
{
	uint8  xdata *pbyRecBuf;		/*���ջ�����*/
	uint8  byRecState;		/*���չ����������Ľ׶�*/
	uint16 uiRecPosition;	/*���ݴ��λ��*/
	uint16 uiRecLen;		/*����������ݸ���*/
	uint8  byRecCount;		/*�����׶�Ӧ���ոý׶����ݵĸ���*/
	uint8  byRecTimeout;	/*���չ����еĳ�ʱ����*/
} STUsartCfg, pSTUsartCfg;



typedef struct
{
	uint8	byStartByte1;					//����֡��ʼ�ַ�1
	uint8 	bySndSecAddr;					//�շ�����ַ
	uint8 	bySndRoomAddr;					//���շ�����ַ
	uint8 	bySndBedAddr;					//���շ�����ַ	
	uint8 	byRecSecAddr;					//�������ͷ�����ַ
	uint8 	byRecRoomAddr;					//�������ͷ�����ַ
	uint8 	byRecBedAddr;					//�������ͷ�����ַ
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

#define 	CMD_POSITION		0X07
#define 	LEN_HIGH_POSITION	0x08
#define 	LEN_LOW_POSITION	0x09
#define 	DATA_POSITION0		0x0a
#define 	DATA_POSITION1		0x0B
#define 	DATA_POSITION2		0x0C
#define 	DATA_POSITION3		0x0D
#define 	DATA_POSITION4		0x0E
#define 	DATA_POSITION5		0x0F
//����1��������
#define		RE_485				0
#define		DE_485				1
//���ȱ�ʾ������
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