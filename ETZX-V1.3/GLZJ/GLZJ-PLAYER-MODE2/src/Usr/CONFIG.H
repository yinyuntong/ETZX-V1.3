/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:config.h
*�ļ�����    		:����ͷ�ļ�
*������     		:������
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
//ϵͳͷ�ļ�
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "STC12C56.H"

#define const code 
#ifndef TRUE
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif

#define true 1

#define SPK_ROOM 0

#define ADD_FJ_ADDR 250
#define MOVE_FJ_ADDR 251


#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))

//���߳�����
#define	BUS0_REC		0x20 								//����0�������
#define BUS0_SND		0x40								//����0�������	 
#define KEY_DOWN		0x60								//��������
#define KEY_ALWAYS		0x70
#define KEY_UP			0x80								//��������
#define TIMER_OUT		0xa0								//��ʱ����	
//��������	
#define NO_KEY			0xff								//�޼����� 	

sbit  	MP3_DREQ	=	P3^2;
sbit	KEY_COPY	=	P3^5;
sbit 	MP3_XDCS	=	P3^7;

sbit 	PRG_STATE	=	P1^0;
sbit	MP3_XRESET	=	P1^1;
sbit	MP3_XCS		=	P1^2;
sbit	TER_CS		=	P1^3;
sbit	SST25VF_CS		=	P1^4;
sbit	MOSI		=	P1^5;
sbit	MISO		=	P1^6;
sbit	SCLK		=	P1^7;


//����ͷ�ļ�
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 
#include "SingleBus.h"
#include "Key.h"
#include "IAP.h"
#include "VS10XX.h"
#include "SPI.h"
#include "SST25VF.h"
#include "player.h"

	 
//�豸�����ֶ���
typedef struct
{
	uint8	byInitFlag; 									//��ʼ��������־
	
} STEepromCfgData, *pSTEepromCfgData;


#define		VOICE_DATA_SIZE		16

//�������ſ�����
typedef struct _STVoicePlay 
{
    uint8  byVoiceData[VOICE_DATA_SIZE];					//��Ҫ���ŵ���������
    uint8  byVoicePosition;									//��������λ��
    uint8  byVoiceSize;										//��Ҫ���ŵ���������   
    uint8  byVS1003State;
	uint8  byVoiceTimes;									//�������ŵı���
} STVoicePlay, *pSTVoicePlay;

//��ʱ���ƽṹ��
typedef struct
{
	uint8 				byTimerOutSet;						//һ�γ�ʱʱ������(20ms��λ)
	uint8 				byTimerOut;							//һ�γ�ʱʱ��(20ms��λ)
	uint8 				byTimerOutCount;					//������ʱ���� 
} STTimerOut, *pSTTimerOut;	 	

//��Ϣָʾ���ṹ��
typedef struct _STIndicationData
{
    uint8  byIndicationSecAddr;								//ָʾ������ַ
    uint8  byIndicationRoomAddr;							//ָʾ������ַ  
    uint8  byIndicationBedAddr;								//ָʾ������ַ 
    uint8  byCallCmd;										//ָʾ������
    uint8  byState;											//�ú�����Ϣ��״̬
} STIndicationData, *pSTIndicationData;


//ȫ�ֿ��ƽṹ��
typedef struct
{
	uint8				byChar1;							//�ֽ�����1	
	uint16				uiShortType;						//����������	
	uint8 				xdata* pbyData;						//����ָ��
	STBusFreq 			stBusDealFreq;						//�����ߴ����ṹ��
	STVoicePlay 		stVoicePlay;						//�������ſ���	
	STTimerOut			stCH0TimerOut;						//�豸��ǰ״̬��ʱ���ƽṹ��
	STEepromCfgData		stEepromCfgData;					//ϵͳ������	  		
} STLocalControl, *pSTLocalControl;	  	

//5msPCA�жϼ�ʱ������Fosc=18.432MHz,��Ƶϵ��Ϊ2 
#define	Fosc				18432000
#define Fbus				184320
	
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			0x0000		//STC12C5624�ڵ�E2ֻ��������E2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN_
extern void Send_Data(uint8 *Databuf,uint8 l);
extern void Send_Data_Byte(uint8 SndData);
extern void delay_nus(uint16 i);
extern void delay_nms(uint16 ms);


extern uint16	xdata uiIsrTimerCount ;					//ϵͳʱ�Ӷ�ʱֵ
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];						//���߳���Ϣ����
extern STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��
extern uint8 	data volumn;
extern uint32	xdata SST25VFAddr;
extern int32  xdata VoiceDataLen;
extern uint8 xdata i;

extern void ConsoleWrite(char code *str);


#endif