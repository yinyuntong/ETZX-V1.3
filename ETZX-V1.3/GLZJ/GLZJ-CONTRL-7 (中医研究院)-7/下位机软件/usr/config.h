/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:config.h
*�ļ�����    		:����ͷ�ļ�
*������     		:����ͬ
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define DEBUG 1

#ifndef STC12C5A
#define STC12C5A 1
#endif

#ifndef STC90
#define STC90	0
#endif

//#define SPK_ROOM 1			//��������ʱ������
#define SPK_ROOM 0			//��������ʱ��������


/*����ϵͳͷ�ļ�*/
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <math.h>                                                                                                          
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STC12C5A.h"


#define int8 char
#define uint8 unsigned char
#define int16 int
#define uint16 unsigned int
#define int32 long int
#define uint32 unsigned long int 

/*Ӧ��ͷ�ļ�*/
#include "SingleBus.h"
#include "I2C.h"
#include "PCF8563.h"
#include "Uart.h"
#include "key.h"
#include "iap.h"
#include "avrx.h"

//���˿ڶ���
sbit POWER_EN	=	P1^7;	// OUT  1:  24V��Դ��

sbit POWER_SW	=	P4^3;	//IN:    �����ϵĵ�Դ����

sbit INT6	=	P3^3;

sbit KBC	=	P5^0;
sbit KBD	=	P5^1;
sbit KGB	=	P2^5;



sbit MP3_RST	=P4^6;

sbit LED_STATE	=P0^7;		//״ָ̬ʾ��OUT   0:����
sbit LED_POWER	=P0^6;		//��Դָʾ��OUT   0:����

sbit SDA	=	P0^5;
sbit SCL	=	P0^4;

sbit RE0	=	P0^1;
sbit DE0	=	P0^2;

sbit KV		=	P1^1;		//��Ƭ������������
sbit SS		=	P1^4;

sbit GB_FZ	=   P5^3;		//���ڹ㲥ʱSD���ϵĸ���ƽ��


#define const code 


/*ϵͳ����*/
#ifndef 	true
#define   	true      			1
#endif
#ifndef		false
#define   	false 				0
#endif


#ifndef 	TRUE
#define   	TRUE      			1
#endif
#ifndef		FALSE
#define   	FALSE 				0
#endif


#define		ISD_TIME			200


#define 	RE_SND_TIMES	3

#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))



extern void LedStateFlash(void);


//���������ֽṹ��
typedef struct _STEepromCfgData
{
	uint8  byInitFlag;										//��ʼ����־
    uint8  bySelfSecAddr;									//����ַ
    uint8  bySelfRoomAddr;									//����ַ  
    uint8  bySelfBedAddr;									//����ַ
    uint16 uiRs485Brt;										//RS485������
	uint8  byCrc;											//У���
} STEepromCfgData, *pSTEepromCfgData;
#define		VOICE_DATA_SIZE		16
//�������ſ�����
typedef struct _STVoicePlay 
{
    uint8  byVoiceData[VOICE_DATA_SIZE];					//��Ҫ���ŵ���������
    uint8  byVoicePosition;									//��������λ��
    uint8  byVoiceSize;										//��Ҫ���ŵ���������
    uint8  byISD1700State;									//����оƬ״̬   
} STVoicePlay, *pSTVoicePlay;
//˫������ڵ�
typedef struct _STNod
{
    uint8  byCallSecAddr;									//���з�����ַ
    uint8  byCallRoomAddr;									//���з�����ַ
    uint8  byCallBedAddr;									//���з�����ַ
    uint8  byCallPriority;									//�������ȼ�
    struct _STNod xdata *pstPrev;
    struct _STNod xdata *pstNext;    
} STNod, *pSTNod;
//˫������ṹ��
typedef struct _STPointer
{
    struct _STNod xdata *pstHead;									//����ͷ�ڵ�
    struct _STNod xdata *pstTail;									//����β�ڵ�    
} STPointer, *pSTPointer;
//��Ϣָʾ���ṹ��
typedef struct _STIndicationData
{
    uint8  byIndicationSecAddr;								//ָʾ������ַ
    uint8  byIndicationRoomAddr;							//ָʾ������ַ  
    uint8  byIndicationBedAddr;								//ָʾ������ַ 
    uint8  byCallCmd;										//ָʾ������
    uint8  byState;											//�ú�����Ϣ��״̬
    struct _STNod xdata *pstNext;    								//��һ��������Ϣָ��
} STIndicationData, *pSTIndicationData;
//��ʱ����ṹ��
typedef struct _STTimerout
{	
	uint8  byTimerOutSet;									//һ�γ�ʱʱ������(20ms��λ)
	uint8  byTimerOut;										//һ�γ�ʱʱ��(20ms��λ)
	uint8  byTimerOutCount;									//����ʱ����	
} STTimerout, *pSTTimerout;
//��ַ�ṹ��
typedef struct
{
	uint8  bySndSecAddr;										//���ͷ�����ַ
    uint8  bySndRoomAddr;										//���ͷ�����ַ  
    uint8  bySndBedAddr;										//���ͷ�����ַ
    uint8  byRecSecAddr;
	uint8  byRecRoomAddr;
	uint8  byRecBedAddr;

    uint8  byCmd;											//ԭ����
} STAddr, *pSTAddr;


//��ַ�ṹ��
typedef struct
{
	uint8  bySecAddr;										//����ַ
    uint8  byRoomAddr;										//����ַ  
    uint8  byBedAddr;										//����ַ									
} STAddr_S, *pSTAddr_S;


typedef struct queue
{
	uint16 queuesize;		//����Ĵ�С
	uint16 head,tail;		//���е�ͷ��β�±�
	uint8 *q;				//����ͷָ��
	
}Queue;

/*ȫ�ֿ��ƽṹ��*/
typedef struct
{
uint8 				byChar;
uint8				byWaitAckTimeLanding;				//�ϵ�Ǽ�ʱ�ȴ�PC����Ӧ��ʱ��
uint16				uiDevState; 						//�豸��ǰ������״̬
uint16				uiDetectPcTime;						//����PC�����ʱ��	
uint8				byVoicePlayTime;					//���Ŷ�ʱ��
STBusFreq			stBusFreq;							//���������ݴ���ṹ��	
STTimerout			stCH0TimerOut;						//��ʱ����ṹ��
STTimerout			stCH1TimerOut;						//ͨ��1ͨ����ʱ����ṹ��	
uint8				byWaitCommAckDTime;						//ͨ�õȴ���ʱʱ��
uint8 				byUsart1HandingDTime;				//����1���ڴ���ʱʱ��
uint8 				byUsart1SdResultDTime;				//����1���ͽ������ʱʱ��
STPointer			stCallPointer;						//���ж���˫������
STIndicationData	stIndicationData;					//��Ϣָʾ���ݽṹ��
STVoicePlay 		stVoicePlay;						//�������ſ���
STAddr				stCallAddr; 						//ͨ��0�������з���ַ
STAddr				stCH1CallAddr; 						//ͨ��1�������з���ַ
STAddr_S			stRS485Addr;						//RS485���ݽ��շ���ַ
uint8				byWaitRS485AckDTime;				//ͨ�õȴ���ʱʱ��
STEepromCfgData 	stEepromCfgData;					//���������ֽṹ��
uint8				byReSndBrtTimes;					//�ظ��������ò����ʵĴ���
uint8				byReSndRs485RoomTimes;				//�ظ����ʹ�ͷ�ֻ�����RS485��Ϣ�Ƿ���Ҫ�жϷ��ŵĴ���
uint8 				byReSndBedVoiPlaTimes;				//�ظ�������������ʱ��ͷ�ֻ��Ƿ��������ŵĴ���
STAddr_S			stCommAddr;							//ͨ�õ�ַ
} STLocalControl, *pSTLocalControl;


//���߳���Ϣ���Ͷ���
#define		MAIN_CMD_BUS0_REC			0x10				//����0������������
#define 	MAIN_CMD_BUS0_SND			0x20				//����0��������������
#define		MAIN_CMD_BUS1_REC			0x30 				//����1�������
#define 	MAIN_CMD_BUS1_SND			0x40				//����1�������	 
#define 	MAIN_CMD_TIMEROUT			0x50				//��ʱ����	
#define 	KEY_DOWN					0x60				//����������
#define 	KEY_UP						0x70				//����������



//�豸״̬����
#define		FLAG_LANDING				0x8000 				//�ϵ�Ǽ�״̬
#define     FLAG_CHANNEL_CHANGE_EXIST    0x4000				//��ͨ���л������߷��Ͷ�����
#define 	FLAG_WAIT_COMM_ACK			0x2000				//���ڵȴ��������ϵ�ͨ��Ӧ���־
#define		FLAG_STOP_EXIST				0x1000				//��ֹͣ����ָʾ�����߷��Ͷ�����
#define		FLAG_TALK1					0x0800				//ͨ��1ͨ����־
#define		FLAG_NOTE					0x0400				//������ʾ״̬
#define		FLAG_MUSIC					0x0200				//�����ֲ���״̬
#define		FLAG_INDICATION_EXIST		0x0100				//�к���ָʾ�����߷��Ͷ�����
#define		FLAG_BUSY					0x0080				//ϵͳæ��־
#define		FLAG_WAIT_ACK				0x0040				//�ȴ�Ӧ���־
#define		FLAG_WAIT_LISTEN			0x0020				//�ȴ�������־
#define		FLAG_TALK					0x0010				//ͨ��0ͨ����־
#define		FLAG_BROADCAST				0x0008				//�㲥��־
#define		FLAG_INDICATION				0x0004				//�ֻ�����ָʾ��־
#define 	FLAG_WAIT_RS485_ACK			0x0002				//�ȴ�RS485 ����Ӧ��


//#define 	PC_ADDR1					0X00				//PC����ַ1
#define 	PC_ADDR2					0X00				//PC����ַ2
#define 	PC_ADDR3					0X00				//PC����ַ3

#define 	GLZJ_ADDR2					0xfe				//����������ַ2
#define     GLZJ_ADDR3					0X00				//����������ַ3

#define 	ADD_BED_FJ			250							//�Ӵ��ֻ����ŵ�ַ	//0xFA
#define 	MOVE_FJ				251							//�ƶ��ֻ����ŵ�ַ	//0xFB


#define 	YHFJ_ADDR2				0XFD					//ҽ���ֻ����ŵ�ַ
#define 	ZLDIS_ADDR2				0XF0					//������ʾ�����ŵ�ַ

/*����CPUʱ��(HZ)*/
#define 	CPU_CLK 		18432000L 
#define OS_TICKS_PER_SEC        50        /* ����1��ϵͳϵͳ������                                    */


/*���嶨ʱ����ʼ��ֵ*/
#if STC90==TRUE
#define ISR_INC_COUNT		(65536-(CPU_CLK/12/OS_TICKS_PER_SEC))	//ʹ��T2ʱ��
#else
#define ISR_INC_COUNT		(CPU_CLK/12/OS_TICKS_PER_SEC)		//ʹ��PCAʱ�� 12��Ƶ  //ÿ20ms�ж�1��
#endif

#define	IAP0_ADDR			0x0000		//STC12C5A60S2�ڵ�E2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN_
extern STLocalControl xdata	 stLocalControl; 
extern uint8 			xdata 	byMallocMem[];

extern bit bPCState;
extern bit bLedState;
extern bit bPowerEnState;


extern void Delayms(uint16 ms);
extern void SaveParameter(uint16 addr);


#endif


