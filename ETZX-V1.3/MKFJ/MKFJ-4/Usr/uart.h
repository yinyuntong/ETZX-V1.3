/*
**********************************Copyright (c) ****************************
*	  						cwg
*			       All Rights Reserved
*			     		QQ:7084394
*
*---------------------------------File Info---------------------------------
*File Name     	:uart.h
*Descriptor    	:串口驱动头文件
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
	uint16  uiRecLen;	//数据帧中的数据域的长度
} STUartControl, *pSTUartControl;
typedef struct
{	
    uint8 	bySndAreaAddr;		//发送方区地址
	uint8	bySndRoomAddr;		//发送方房地址
	uint8   bySndBedAddr;		//发送房床地址 
	uint8 	byRecAreaAddr;		//接收方区地址
	uint8	byRecRoomAddr;		//接收方房地址
	uint8   byRecBedAddr;		//接收房床地址
	uint8	byCmd;				//命令代码
	uint16	byLen;		  	    //数据长度
	uint8	byData[1];			//链路数据
} STUartFreq, *pSTUartFreq;
typedef struct
{
	uint8 byDataType;			//数据类型，0x01：国标码
	uint8 byPagePosition;		//屏的位置
	uint8 byPageType;			//数据屏的类型，0x01:床头卡，0x02:费用清单，0x03:药品清单
	uint8 byLinePosition;		//行的位置
	uint8 byEndFlag;			//页结束标志，0x26:没有结束，0x28:结束
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