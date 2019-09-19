/*
**********************************Copyright (c) ****************************
*	  						cwg
*			       All Rights Reserved
*			     		QQ:7084394
*
*---------------------------------File Info---------------------------------
*File Name     	:uart.h
*Descriptor    	:��������ͷ�ļ�
*Create By     	:ChenWeiGuo
*Create Date   	:2008-6-18
*Version       	:V1.0
*Note	     	:
*
*---------------------------------------------------------------------------
*Modified By  	:
*Modified Date  :
*Version        :
*Note	     	:
*
****************************************************************************
*/ 
#define UART0_BUF_SIZE		255
typedef struct
{
	uint8 	byState;	
	uint8 	byRxTail;
	uint8   byCrc; 	
	uint8 	byLen;
	uint16  uiRecLen;	//����֡�е�������ĳ���
} STUartControl, *pSTUartControl;
typedef struct
{	
    uint8 	bySndAreaAddr;		//���ͷ�����ַ
	uint8	bySndRoomAddr;		//���ͷ�����ַ
	uint8   bySndBedAddr;		//���ͷ�����ַ 
	uint8 	byRecAreaAddr;		//���շ�����ַ
	uint8	byRecRoomAddr;		//���շ�����ַ
	uint8   byRecBedAddr;		//���շ�����ַ
	uint8	byCmd;				//�������
	uint16	byLen;		  	    //���ݳ���
	uint8	byData[1];			//��·����
} STUartFreq, *pSTUartFreq;
typedef struct
{
	uint8 byDataType;			//�������ͣ�0x01��������
	uint8 byPagePosition;		//����λ��
	uint8 byPageType;			//�����������ͣ�0x01:��ͷ����0x02:�����嵥��0x03:ҩƷ�嵥
	uint8 byLinePosition;		//�е�λ��
	uint8 byEndFlag;			//ҳ������־��0x26:û�н�����0x28:����
} STLcdPageData, *pSTLcdPageData;

#define MAX_FREQ_DATA_LEN		100    //47//31//28 
#define FREQ_HEAD_SIZE			10
#define START_BYTE1				0x5a
#define	START_BYTE2				0xa5
#define END_BYTE				0x16  

#ifndef _IN_UART_
#define _IN_UART_
void UartInit(void);
void UartDeal(void); 
//void UartManager(void);
void Send_Data(uint8 *Databuf,uint8 xdata l);
void Send_Data_Byte(uint8 SendData);
#endif