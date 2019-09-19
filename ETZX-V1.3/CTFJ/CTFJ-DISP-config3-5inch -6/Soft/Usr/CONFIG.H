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
//ϵͳͷ�ļ�
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "STC12C5A.H"
#define const code

#define DEBUG 1


#define MAX_COM	 1
 
#ifndef TRUE
#define TRUE  1
#endif 
#ifndef FALSE
#define FALSE 0
#endif


#ifndef true
#define true  1
#endif 
#ifndef false
#define false 0
#endif

#ifndef STC12C5A
#define STC12C5A 1
#endif

#ifndef STC90
#define STC90 0
#endif


#define CONFIG_CC1101 0

#define YHFJ_ROOM_ADDR 253	//ҽ��ֻ��κ�


#define AVAIL_DATA_LEN 48

#define MONTH_AGE 150		//150���ϵ���ֵ���ǰ����ۼƵ�����

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long 

//���߳�����
#define	BUS0_REC		0x10 								//����0�������
#define BUS0_SND		0x20								//����0�������
#define	BUS1_REC		0x30 								//����0�������
#define BUS1_SND		0x40								//����0�������
#define KEY_DOWN		0x50								//��������
#define KEY_UP			0x60								//��������
#define	KEY_ALWAYS		0x70								//��������
#define TIMER_OUT		0x80								//��ʱ����	


#define BED_CARD_TYPE			0						//��ͷ����Ϣ����
#define BED_TYPE				1						//��ͷ��Ϣ����
#define WARD_SEC_TYPE			3						//������Ϣ������Ϣ����
#define DOOR_SEC_TYPE			4						//������Ϣ������Ϣ����
#define AISLE_SEC_TYPE			5						//������ʾ����Ϣ����

#define DIET_TYPE				6						//��ʳ��Ϣ����
#define PRESCRIPTION_TYPE		8					//ҩ��
#define PROMPT_INFO_TYPE		9					//��ʾ��Ϣ


#define POWER_UP_FACE		1									//�ϵ����
#define MAIN_MENU_FACE	2
#define CALL_SUCCESS_FACE		3
#define TALK_FACE				4
#define INFO_INDICATION_FACE	5

#define DIET_FACE			6						//��ʳ����
#define PRESCRIPTION_FACE		8
#define PROMPT_INFO_FACE		9
#define SEC_INFO_FACE			10
#define BROAD_FACE				11
#define INFUSION_CALL_FACE		12
#define SERVICE_CALL_FACE		13
#define EMERGENCY_CALL_FACE		14
#define HELP_CALL_FACE			15
#define BED_INFO_FACE			16

#define START_NUM_SET_FACE		17
#define NUM_SET_OK_FACE			18

#define START_RF_NUM_SET_FACE	19 
#define RF_NUM_SET_OK_FACE		20




//�洢оƬ25VF032  ��4M  0x400000�ռ�
//�ֿ�ʹ�õ�0x3b06b0

//��ͷ����Ϣʹ�ô�0x3c0000��ʼ  ��ʣ��256K�ռ�
//�õ�116K

#define DIET_DATA_START_ADDR	        0x3c0000			//��ʳ��Ϣ������ʼ��ַ
												             //ÿ������Ϣռ��32K
												
#define PRESCRIPTION_DATA_START_ADDR	0x3c8000			//ҽ����Ϣ������ʼ��ַ


#define PROMPT_DATA_START_ADDR	   		0x3d0000				//��ʾ��Ϣ������ʼ��ַ


#define DIET_LENGTH_ADDR      			0x3d8000					//��ʳ��Ϣ�ܼƳ��ȴ洢��ַ

#define PRESCRIPTION_LENGTH_ADDR      	0x3d9000					//ҽ����Ϣ�ܼƳ��ȴ洢��ַ
											
#define PROMPT_LENGTH_ADDR      		0x3da000					//��ʾ��Ϣ�ܼƳ��ȴ洢��ַ


#define BED_FACE_DATA_LENGTH_ADDR		0x3db000					//��ͷ�����е���ʾ���ݳ�����ʼ��ַ

#define BED_FACE_DATA_START_ADDR		(BED_FACE_DATA_LENGTH_ADDR+2)	//��ͷ�����е��������ڴ洢���е���ʼλ��
																	     //���ֻ��4K
																	     
#define BED_FACE_FORM_LENGTH_ADDR		0x3dc000					//��ͷ�����еĸ�ʽ���ݳ�����ʼ��ַ

#define BED_FACE_FORM_START_ADDR		(BED_FACE_FORM_LENGTH_ADDR+2)					//��ͷ�����еĸ�ʽ�������ڴ洢���е���ʼλ��
																	//���ֻ��4K

#define BED_FACE_DATA_BAK				0x3dd000					//�Դ�ͷ�����ݵ�һ�α���( ��������,����, У���)

#define BED_FACE_FORM_BAK				0x3de000					//�Դ�ͷ����ʽ��һ�α���( ��������,����, У���)


//����Ϊ������Ϣ��Ԥ��ռ�

#define BED_FACE_DATA_START_ADDR_2		0x3e0000					//��ͷ�����е��������ڴ洢���е���ʼλ��
																	//���ֻ��4K

#define BED_FACE_FORM_START_ADDR_2		0x3e1000					//��ͷ�����еĸ�ʽ�������ڴ洢���е���ʼλ��
																	//���ֻ��4K

#define FREE_START_ADDR					0X3ff000
#define FRAME_TEST_ADD					0X3ff000


#define LENGTH_32K		32512
#define LENGTH_4K		3840

#define FIRST_DATA_LEN		41			//�׶����ݳ���
#define SEGMENT_DATA_LEN    60			//ÿ�����������


#define SHORT_NAME_MODE			0
#define LONG_NAME_MODE			1

#define NAME_LEN_POSITION		41


#define	CC1101_REREAD_TIME		15000						//ÿ��5���Ӷ�ȡ1��CC1101


//��Ŀ����
#define BED_NUMBER				1		//����
#define ILLNESS_LEVEL			2		//����
#define MEDICAL_LEVEL			3		//ҽ��
#define NURSING_LEVEL			4		//����
#define DIETARY_LEVEL			5		//��ʳ
#define HOSPITALIZED_NO			6		//סԺ��
#define NAME					7		//����
#define SEX						8		//�Ա�
#define AGE						9		//����
#define ADMISSION_DATE			10		//��Ժ
#define DIAGNOSIS               11		//���
#define BLOOD					12		//Ѫ��
#define CAUTION					13		//��ȫ��ʾ
#define ALERGY_DRUGS			14		//����ʷ
#define DOCTOR					15		//ҽ��
#define NURSE					16		//��ʿ
#define ILLNESS_CHANGE			17		//����仯
#define PHARMACY 				18		//��ҩ
#define MESSAGE_BOARD			19		//���԰�
#define DIET_INFO				20		//��ʳ��Ϣ
#define DOCTOR_ADVICE			21		//ҽ��
#define HINT					22		//��ʾ
#define OX_START				23		//��ʼ
#define OX_END					24		//����
#define OX_SUBTOTAL				25		//С��
#define OX_TOTAL				26		//�ۼ�
#define OX_SUPPLY				27		//����

#define MESSAGE2				28		//��Ϣ��2
#define MESSAGE3				39		//��Ϣ��3
#define MESSAGE4				30		//��Ϣ��4
#define MESSAGE5				31		//��Ϣ��5
#define MESSAGE6				32
#define MESSAGE7				33
#define MESSAGE8				34
#define MESSAGE9				35
#define MESSAGE10				36
#define MESSAGE11				37
#define MESSAGE12				38
#define MESSAGE13				39


#define MAX_ITEM_CODE 			40





//�������Ŷ���
//�������Ŷ���
sbit	MOSI		=	P1^5;								//SPI ���������
sbit	MISO		=	P1^6;								//SPI �ӳ������
sbit	SCLK		=	P1^7;								//SPI  ʱ����
sbit 	SCK			=	P1^7;

sbit  	EPH1660_CS		= 	P1^4;
sbit  	RESETB  	= 	P4^7;
sbit  	SPI_CMD 	= 	P5^2;
sbit  	WKICOM  	= 	P1^3;
sbit  	WKO	  		= 	P3^2;

sbit 	ADS7843_CS	=	P4^4;								//����оƬƬѡ
sbit	TPBUSY		=	P1^2;
sbit 	TPIRQ		=   P1^3;



 //ԭʹ��GT23L�ֿ�оƬ
//sbit 	GT23L_CS	=	P4^5;								//�ֿ�оƬƬѡ
//sbit 	SST25VF_CS	=	P4^1;								//�洢��Ƭѡ

//��ʹ���Լ������ֿ�SST25VF032,����ԭ�ֿ�λ��
sbit 	GT23L_CS	=	P4^1;								//�ֿ�оƬƬѡ
sbit 	SST25VF_CS	=	P4^5;								//�洢��Ƭѡ



sbit  BL_SW			=P3^5;
sbit  SSD1963_CS  = P1^0;
sbit  SSD1963_RST = P1^1;
sbit  SSD1963_CD  = P5^3;
sbit  SSD1963_TE  = P4^2;
sbit  SSD1963_WR  = P3^6;
sbit  SSD1963_RD  = P3^7;

sbit 	DATA0		=	P0^0;
sbit	DATA1		=	P0^1;
sbit 	DATA2		=	P0^2;
sbit	DATA3		=	P0^3;
sbit 	DATA4		=	P0^4;
sbit 	DATA5		=	P0^5;
sbit 	DATA6		=	P0^6;
sbit 	DATA7		=	P0^7;

sbit 	DATA8		=	P2^0;
sbit	DATA9		=	P2^1;
sbit 	DATA10		=	P2^2;
sbit	DATA11		=	P2^3;
sbit 	DATA12		=	P2^4;
sbit 	DATA13		=	P2^5;
sbit 	DATA14		=	P2^6;
sbit 	DATA15		=	P2^7;

sbit 	led0        =   P4^0;

sbit	RF_IRQ		=	P4^3;
sbit 	RF_TS		=	P5^0;
sbit 	RF_CS		=	P5^1;

sbit	    CSN		=	P5^1;


#define Bin(b7,b6,b5,b4,b3,b2,b1,b0)	((b7<<7)|(b6<<6)|(b5<<5)|(b4<<4)|(b3<<3)|(b2<<2)|(b1<<1)|(b0<<0))


//����ͷ�ļ�
#include "OS_CFG.H"
#include "OS_CPU.H"
#include "..\os\OS.H"
#include "..\os\OS_Q.h"
#include "..\os\OS_SEM.h" 	
#include "SingleBus.h"
#include "GT23.h"	
#include "IAP.h"
#include "Uart.h"
#include "SPI.h"
#include "LCD.h"
#include "PCT25VF.h"
#include "EPH1660.H"
#if CONFIG_CC1101==1
#include "CC1101.H"
#endif

#include "128x80Arial.h"

typedef struct
{
	uint16  uiHour;
	uint8  	byMin;
	uint8   bySec;
} STOXTime, *pSTOXTime;


//�豸�����ֶ���
typedef struct
{
	uint8   byInitFlag;										//��ʼ��������־
	uint8  	bySelfSecAddr;									//��������ַ
	uint8  	bySelfRoomAddr;									//��������ַ
	uint8  	bySelfBedAddr;									//��������ַ
	uint8	bySerialNum1;									//���к��ֽ�1(���к����λ��һ���ֽ�)
	uint8	bySerialNum2;									//���к��ֽ�2
	uint8	bySerialNum3;									//���к��ֽ�3(���к����λ��һ���ֽ�)
	uint8	byVersionHi;									//�汾�Ÿ��ֽ�
	uint8	byVersionLo;									//�汾�ŵ��ֽ� 
	STOXTime stSupplyOxTotalTime;							//�����ܼ�ʱ��
	uint8   byRFSerialNum1;									//��Һ���������к�1
	uint8   byRFSerialNum2;									//��Һ���������к�2
	uint8   byRFSerialNum3;									//��Һ���������к�3
	uint8   byRFSerialNum4;									//��Һ���������к�4
	uint8   byRFSerialNum5;									//��Һ���������к�5
	uint8   byRFSerialNum6;									//��Һ���������к�6
	uint16 	uiRs485Brt;										//RS485���߲�����
	uint8	byRs485SecRoom;									//����RS485����ʱ�Ƿ���Ҫ�ж����š�����
	uint8	byCrc;									//��ͷ�ֻ���ʾ������������

} STEepromCfgData, *pSTEepromCfgData;
//ʱ��ṹ��
typedef struct
{	
	uint16 x;
	uint16 y;
}STCoordinate,*pSTCoordinate;


typedef struct
{	
	uint8 ItemCode;	//��Ŀ
	STCoordinate StartPoint;	//�����������
	STCoordinate EndPoint;		//�����������
	uint8 bcolor;				//������ɫ
	uint8 fcolor;				//������ɫ
	uint8 DispMode;				//��ʾ��ʽ
}STItemForm,*pSTItemForm;	//��Ŀ��ʽ


typedef struct
{
	uint8 ItemCode;	//��Ŀ
	uint8 DataLength;	//�������ݳ���
	uint8 *pData;		//����Ҫ��ʾ������
}STItemData,*pSTItemData;		//��Ŀ����


typedef struct
{
	uint8  byYear;
	uint8  byMonth;
	uint8  byDay;
	uint8  byHour;
	uint8  byMin;
	uint8  bySec;
} STTime, *pSTTime;



//��ʱ���ƽṹ��
typedef struct
{
	uint8 				byTimerOutSet;						//һ�γ�ʱʱ������(20ms��λ)
	uint8 				byTimerOut;							//һ�γ�ʱʱ��(20ms��λ)
	uint8 				byTimerOutCount;					//������ʱ���� 
} STTimerOut, *pSTTimerOut;


#define PATIENT_DISCHARGE	0x01		//1:���˳�Ժ
#define LCD_ON				0x02		// 1 :�����


//ȫ�ֿ��ƽṹ��
typedef struct
{						//�ַ�����ָ��
	STBusFreq 			stBusDealFreq;						//�������ݴ����洢��
	STTimerOut			stCH0TimerOut;						//�豸��ǰ״̬��ʱ���ƽṹ�� 
	STEepromCfgData		stEepromCfgData;					//ϵͳ������
	STTime				stTime;								//ʱ��
	uint8 				byDisplayFace;						//��ǰ��ʾ����
	uint8				byDispNumSetOkTime;					//��ʾ��ųɹ�����ʱ��
	uint8 				byMSecond;							//���뵥λ��ʱ
	STTime				stSupplyOxStartTime;				//������ʱ��ʼʱ��
	STTime				stSupplyOxEndTime;					//������ʱ����ʱ��
	STOXTime 			stSupplyOxSubtotalTime;				//��������ʱ��
	STOXTime			stSupplyOxTotalTime;				//�����۴�ʱ��
	int16				uiBedFaceFormLen;					//����һ�����ݳ���
	int16               uiBedFaceDataLen;					//�����ִ�ͷ����һ�����ݳ���
	uint16				uiDietDataLen;						//��ʳ���ݳ���
	uint16				uiPrescriptionDataLen;				//ҽ�����ݳ���
	uint16              uiPromptDataLen;					//��ʾ��Ϣ���ݳ���
	uint16 				uiSendedDataLen;					//�ѷ����ݳ���
	uint16 				uiSendSBTotalDataLen;				//�����ֱ��ܼ����ݳ���
	uint8				bySendSBTimes;						//�����ֱ�����
	
	uint8				byUsart1SdDelayTime;				//����1���������ӳ�ʱ��
	uint8				bySlaveState;						//�ֻ�״̬
	//#define PATIENT_DISCHARGE 0x01		//1:���˳�Ժ
	//#define LCD_ON				0x02		// 1 :�����	�
	uint16 				uiLcdDisplayTime;					//��ʾ����ʾʱ��
	uint16 				uiCC1101ReReadTime;					//cc1101�ض����ʱ��
	STItemForm          stItemForm;							//��Ŀ��ʽ
	STItemData		    stItemData;							//��Ŀ����
} STLocalControl, *pSTLocalControl;
//5msPCA�жϼ�ʱ������Fosc=18.432MHz,��Ƶϵ��Ϊ2 
#define	Fosc				18432000L
#define Fbus				184320
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			0x0000		//STC12LE5A60S2�ڵ�E2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN
#define _IN_MAIN
extern uint16	xdata uiIsrTimerCount ;					//ϵͳʱ�Ӷ�ʱֵ
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];						//���߳���Ϣ����
extern STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��
extern bit bWireLessRec;
//extern bit bOXSupplyState;
extern bit bEnableOxTimer;
extern bit	bBL_SW_State;


extern uint8 xdata byWireLessRecLen;
extern uint8 xdata byWireLessRecBuff[];
extern uint8 xdata byWireLessSndBuff[];

extern void UserTickTimer(void);
extern void Send_Data(uint8 *Databuf,uint8 l);
extern void Send_Data_Byte(uint8 SendData);
extern STOXTime   timeInterval(STTime a,STTime   b);
extern uint16   allday(STTime   a);

#endif