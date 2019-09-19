/*
************************Copyright(c)************************
*	  			   ºşÄÏÒ»ÌØµç×Ó¹É·İÓĞÏŞ¹«Ë¾
*			       	All Rights Reserved
*			     		 
*
*-----------------------ÎÄ¼şĞÅÏ¢---------------------------
*ÎÄ¼şÃû³Æ     		:config.h
*ÎÄ¼şÃèÊö    		:ÅäÖÃÍ·ÎÄ¼ş
*´´½¨ÈË     		:ÒüÔËÍ¬
*´´½¨ÈÕÆÚ   		:2008-9-22
*°æ±¾ºÅ       		:V1.0
*×¢ÊÍ	     		:					
*----------------------------------------------------------
*ĞŞ¸ÄÈË  			:
*ĞŞ¸ÄÈÕÆÚ  			:
*°æ±¾ºÅ        		:
*×¢ÊÍ	     		:
***********************************************************
*/
//ÏµÍ³Í·ÎÄ¼ş
#pragma REGPARMS	  
#include <intrins.h>
#include <absacc.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include "STC12C5A.H"
#define const code

#define DEBUG 0


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

#define YHFJ_ROOM_ADDR 253	//Ò½Îñ·Ö»ú¶ÎºÅ


#define AVAIL_DATA_LEN 48

#define MONTH_AGE 150		//150ÒÔÉÏµÄÊıÖµ°´ÊÇ°´ÔÂÀÛ¼ÆµÄÄêÁä

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long 

//Ö÷Ïß³ÌÃüÁî
#define	BUS0_REC		0x10 								//×ÜÏß0½ÓÊÕÍê³É
#define BUS0_SND		0x20								//×ÜÏß0·¢ËÍÍê³É
#define	BUS1_REC		0x30 								//×ÜÏß0½ÓÊÕÍê³É
#define BUS1_SND		0x40								//×ÜÏß0·¢ËÍÍê³É
#define KEY_DOWN		0x50								//°´¼ü°´ÏÂ
#define KEY_UP			0x60								//°´¼üµ¯Æğ
#define	KEY_ALWAYS		0x70								//¼ü³£°´ÏÂ
#define TIMER_OUT		0x80								//³¬Ê±ÃüÁî	


#define BED_CARD_TYPE			0						//´²Í·¿¨ĞÅÏ¢ÀàĞÍ
#define BED_TYPE				1						//´²Í·ĞÅÏ¢ÀàĞÍ
#define WARD_SEC_TYPE			3						//²¡ÇøĞÅÏ¢½çÃæĞÅÏ¢ÀàĞÍ
#define DOOR_SEC_TYPE			4						//ÃÅÇøĞÅÏ¢½çÃæĞÅÏ¢ÀàĞÍ
#define AISLE_SEC_TYPE			5						//×ßÀÈÏÔÊ¾ÆÁĞÅÏ¢ÀàĞÍ

#define DIET_TYPE				6						//ÒûÊ³ĞÅÏ¢ÀàĞÍ
#define PRESCRIPTION_TYPE		8					//Ò©Öö
#define PROMPT_INFO_TYPE		9					//ÌáÊ¾ĞÅÏ¢


#define POWER_UP_FACE		1									//ÉÏµç½çÃæ
#define MAIN_MENU_FACE	2
#define CALL_SUCCESS_FACE		3
#define TALK_FACE				4
#define INFO_INDICATION_FACE	5

#define DIET_FACE			6						//ÒûÊ³½çÃæ
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


//´æ´¢Ğ¾Æ¬25VF032  ¹²4M  0x400000¿Õ¼ä
//×Ö¿âÊ¹ÓÃµ½0x3b06b0

//´²Í·¿¨ĞÅÏ¢Ê¹ÓÃ´Ó0x3c0000¿ªÊ¼  »¹Ê£ÏÂ256K¿Õ¼ä
//ÓÃµô116K

#define DIET_DATA_START_ADDR	        0x3c0000			//ÒûÊ³ĞÅÏ¢Êı¾İÆğÊ¼µØÖ·
												             //Ã¿ÀàĞÍĞÅÏ¢Õ¼ÓÃ32K
												
#define PRESCRIPTION_DATA_START_ADDR	0x3c8000			//Ò½ÖöĞÅÏ¢Êı¾İÆğÊ¼µØÖ·


#define PROMPT_DATA_START_ADDR	   		0x3d0000				//ÌáÊ¾ĞÅÏ¢Êı¾İÆğÊ¼µØÖ·


#define DIET_LENGTH_ADDR      			0x3d8000					//ÒûÊ³ĞÅÏ¢×Ü¼Æ³¤¶È´æ´¢µØÖ·

#define PRESCRIPTION_LENGTH_ADDR      	0x3d9000					//Ò½ÖöĞÅÏ¢×Ü¼Æ³¤¶È´æ´¢µØÖ·
											
#define PROMPT_LENGTH_ADDR      		0x3da000					//½ÒÊ¾ĞÅÏ¢×Ü¼Æ³¤¶È´æ´¢µØÖ·


#define BED_FACE_DATA_LENGTH_ADDR		0x3db000					//´²Í·½çÃæÖĞµÄÏÔÊ¾Êı¾İ³¤¶ÈÆğÊ¼µØÖ·

#define BED_FACE_DATA_START_ADDR		(BED_FACE_DATA_LENGTH_ADDR+2)	//´²Í·½çÃæÖĞµÄÊı¾İÔÚÔÚ´æ´¢Æ÷ÖĞµÄÆğÊ¼Î»ÖÃ
																	     //×î¶àÖ»ÓĞ4K
																	     
#define BED_FACE_FORM_LENGTH_ADDR		0x3dc000					//´²Í·½çÃæÖĞµÄ¸ñÊ½Êı¾İ³¤¶ÈÆğÊ¼µØÖ·

#define BED_FACE_FORM_START_ADDR		(BED_FACE_FORM_LENGTH_ADDR+2)					//´²Í·½çÃæÖĞµÄ¸ñÊ½Êı¾İÔÚÔÚ´æ´¢Æ÷ÖĞµÄÆğÊ¼Î»ÖÃ
																	//×î¶àÖ»ÓĞ4K

#define BED_FACE_DATA_BAK				0x3dd000					//¶Ô´²Í·¿¨Êı¾İµÄÒ»´Î±¸·İ( °üÀ¨³¤¶È,Êı¾İ, Ğ£ÑéºÍ)

#define BED_FACE_FORM_BAK				0x3de000					//¶Ô´²Í·¿¨¸ñÊ½µÄÒ»´Î±¸·İ( °üÀ¨³¤¶È,Êı¾İ, Ğ£ÑéºÍ)


//ÒÔÏÂÎª½ÓÊÕĞÅÏ¢µÄÔ¤´æ¿Õ¼ä

#define BED_FACE_DATA_START_ADDR_2		0x3e0000					//´²Í·½çÃæÖĞµÄÊı¾İÔÚÔÚ´æ´¢Æ÷ÖĞµÄÆğÊ¼Î»ÖÃ
																	//×î¶àÖ»ÓĞ4K

#define BED_FACE_FORM_START_ADDR_2		0x3e1000					//´²Í·½çÃæÖĞµÄ¸ñÊ½Êı¾İÔÚÔÚ´æ´¢Æ÷ÖĞµÄÆğÊ¼Î»ÖÃ
																	//×î¶àÖ»ÓĞ4K

#define FREE_START_ADDR					0X3ff000
#define FRAME_TEST_ADD					0X3ff000




#define LENGTH_32K		32512
#define LENGTH_4K		3840


#define FIRST_DATA_LEN		41			//Ê×¶ÎÊı¾İ³¤¶È
#define SEGMENT_DATA_LEN    60			//Ã¿¶ÎÊı¾İ×î³¤³¤¶È


#define SHORT_NAME_MODE			0
#define LONG_NAME_MODE			1

#define NAME_LEN_POSITION		41


#define	CC1101_REREAD_TIME		15000						//Ã¿¸ô5·ÖÖÓ¶ÁÈ¡1´ÎCC1101


//ÏîÄ¿´úÂë
#define BED_NUMBER				1
#define ILLNESS_LEVEL			2
#define MEDICAL_LEVEL			3
#define NURSING_LEVEL			4
#define DIETARY_LEVEL			5
#define HOSPITALIZED_NO			6
#define NAME					7
#define SEX						8
#define AGE						9
#define ADMISSION_DATE			10
#define DIAGNOSIS               11
#define BLOOD					12
#define CAUTION					13
#define ALERGY_DRUGS			14
#define DOCTOR					15
#define NURSE					16
#define ILLNESS_CHANGE			17
#define PHARMACY 				18
#define MESSAGE_BOARD			19		//Ò½»¤ÁôÑÔ°å
#define DIET_INFO				20		//ÒûÊ³ĞÅÏ¢
#define DOCTOR_ADVICE			21		//Ò½ÖöĞÅÏ¢
#define HINT					22		//ÌáÊ¾ĞÅÏ¢
#define OX_START				23
#define OX_END					24
#define OX_SUBTOTAL				25
#define OX_TOTAL				26
#define OX_SUPPLY				27

#define MESSAGE2				28
#define MESSAGE3				39
#define MESSAGE4				30
#define MESSAGE5				31
#define MESSAGE6				32
#define MESSAGE7				33
#define MESSAGE8				34
#define MESSAGE9				35
#define MESSAGE10				36
#define MESSAGE11				37
#define MESSAGE12				38
#define MESSAGE13				39


#define MAX_ITEM_CODE 			40





//¿ØÖÆÒı½Å¶¨Òå
//¿ØÖÆÒı½Å¶¨Òå
sbit	MOSI		=	P1^5;								//SPI Ö÷³ö´ÓÈë¿Ú
sbit	MISO		=	P1^6;								//SPI ´Ó³öÖ÷Èë¿Ú
sbit	SCLK		=	P1^7;								//SPI  Ê±ÖÓÏß
sbit 	SCK			=	P1^7;

sbit  	EPH1660_CS		= 	P1^4;
sbit  	RESETB  	= 	P4^7;
sbit  	SPI_CMD 	= 	P5^2;
sbit  	WKICOM  	= 	P1^3;
sbit  	WKO	  		= 	P3^2;

sbit 	ADS7843_CS	=	P4^4;								//´¥ÃşĞ¾Æ¬Æ¬Ñ¡
sbit	TPBUSY		=	P1^2;
sbit 	TPIRQ		=   P1^3;


//Ô­Ê¼µÄ
//sbit 	GT23L_CS	=	P4^5;								//×Ö¿âĞ¾Æ¬Æ¬Ñ¡
//sbit 	SST25VF_CS	=	P4^1;								//´æ´¢Æ÷Æ¬Ñ¡


//ºóÃæ´æ´¢Ğ¾Æ¬ÒÆµ½×Ö¿âĞ¾Æ¬Î»ÖÃ
sbit 	GT23L_CS	=	P4^1;	
sbit 	SST25VF_CS	=	P4^5;   //´æ´¢Æ÷Æ¬Ñ¡



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


//°üº¬Í·ÎÄ¼ş
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
#include "CC1101.H"

typedef struct
{
	uint16  uiHour;
	uint8  	byMin;
	uint8   bySec;
} STOXTime, *pSTOXTime;


//Éè±¸ÅäÖÃ×Ö¶¨Òå
typedef struct
{
	uint8   byInitFlag;										//³õÊ¼»¯²ÎÊı±êÖ¾
	uint8  	bySelfSecAddr;									//±¾»úÇøµØÖ·
	uint8  	bySelfRoomAddr;									//±¾»ú·¿µØÖ·
	uint8  	bySelfBedAddr;									//±¾»ú´²µØÖ·
	uint8	bySerialNum1;									//ĞòÁĞºÅ×Ö½Ú1(ĞòÁĞºÅ×î¸ßÎ»µÄÒ»¸ö×Ö½Ú)
	uint8	bySerialNum2;									//ĞòÁĞºÅ×Ö½Ú2
	uint8	bySerialNum3;									//ĞòÁĞºÅ×Ö½Ú3(ĞòÁĞºÅ×îµÍÎ»µÄÒ»¸ö×Ö½Ú)
	uint8	byVersionHi;									//°æ±¾ºÅ¸ß×Ö½Ú
	uint8	byVersionLo;									//°æ±¾ºÅµÍ×Ö½Ú 
	STOXTime stSupplyOxTotalTime;							//¹©Ñõ×Ü¼ÆÊ±¼ä
	uint8   byRFSerialNum1;									//ÊäÒº±¨¾¯Æ÷ĞòÁĞºÅ1
	uint8   byRFSerialNum2;									//ÊäÒº±¨¾¯Æ÷ĞòÁĞºÅ2
	uint8   byRFSerialNum3;									//ÊäÒº±¨¾¯Æ÷ĞòÁĞºÅ3
	uint8   byRFSerialNum4;									//ÊäÒº±¨¾¯Æ÷ĞòÁĞºÅ4
	uint8   byRFSerialNum5;									//ÊäÒº±¨¾¯Æ÷ĞòÁĞºÅ5
	uint8   byRFSerialNum6;									//ÊäÒº±¨¾¯Æ÷ĞòÁĞºÅ6
	uint16 	uiRs485Brt;										//RS485×ÜÏß²¨ÌØÂÊ
	uint8	byRs485SecRoom;									//½ÓÊÕRS485Êı¾İÊ±ÊÇ·ñĞèÒªÅĞ¶ÏÇøºÅ¡¢·¿ºÅ
	uint8 	byCrc;											//ÒÔÉÏÊı¾İµÄCRCĞ£Ñé
} STEepromCfgData, *pSTEepromCfgData;
//Ê±¼ä½á¹¹Ìå
typedef struct
{	
	uint16 x;
	uint16 y;
}STCoordinate,*pSTCoordinate;


typedef struct
{	
	uint8 ItemCode;	//ÏîÄ¿
	STCoordinate StartPoint;	//¿é×ÔÆğµã×ø±ê
	STCoordinate EndPoint;		//¿é½áÊøµã×ø±ê
	uint8 bcolor;				//±³¾°ÑÕÉ«
	uint8 fcolor;				//×ÖÌåÑÕÉ«
	uint8 DispMode;				//ÏÔÊ¾·½Ê½
}STItemForm,*pSTItemForm;	//ÏîÄ¿¸ñÊ½


typedef struct
{
	uint8 ItemCode;	//ÏîÄ¿
	uint8 DataLength;	//¸ÃÏîÊı¾İ³¤¶È
	uint8 *pData;		//¾ßÌåÒªÏÔÊ¾µÄÊı¾İ
}STItemData,*pSTItemData;		//ÏîÄ¿Êı¾İ


typedef struct
{
	uint8  byYear;
	uint8  byMonth;
	uint8  byDay;
	uint8  byHour;
	uint8  byMin;
	uint8  bySec;
} STTime, *pSTTime;



//³¬Ê±¿ØÖÆ½á¹¹Ìå
typedef struct
{
	uint8 				byTimerOutSet;						//Ò»´Î³¬Ê±Ê±¼äÉèÖÃ(20msµ¥Î»)
	uint8 				byTimerOut;							//Ò»´Î³¬Ê±Ê±¼ä(20msµ¥Î»)
	uint8 				byTimerOutCount;					//ÔÊĞí³¬Ê±´ÎÊı 
} STTimerOut, *pSTTimerOut;

#define PATIENT_DISCHARGE	0x01		//1:²¡ÈË³öÔº
#define LCD_ON				0x02		// 1 :±³¹â´ò¿ª

//È«¾Ö¿ØÖÆ½á¹¹Ìå
typedef struct
{
	STBusFreq 			stBusDealFreq;						//×ÜÏßÊı¾İ´¦Àí´æ´¢Æ÷
	STTimerOut			stCH0TimerOut;						//Éè±¸µ±Ç°×´Ì¬³¬Ê±¿ØÖÆ½á¹¹Ìå 
	STEepromCfgData		stEepromCfgData;					//ÏµÍ³ÅäÖÃ×Ö
	STTime				stTime;								//Ê±¼ä
	uint8 				byDisplayFace;						//µ±Ç°ÏÔÊ¾½çÃæ
	uint8				byDispNumSetOkTime;					//ÏÔÊ¾±àºÅ³É¹¦½çÃæÊ±¼ä
	uint8 				byMSecond;							//ºÁÃëµ¥Î»¶¨Ê±
	STTime				stSupplyOxStartTime;				//¹©Ñõ¼ÆÊ±¿ªÊ¼Ê±¼ä
	STTime				stSupplyOxEndTime;					//¹©Ñõ¼ÆÊ±½áÊøÊ±¼ä
	STOXTime 			stSupplyOxSubtotalTime;				//¹©Ñõ±¾´ÎÊ±¼ä
	STOXTime			stSupplyOxTotalTime;				//¹©ÑõÀÛ´ÎÊ±¼ä
	int16				uiBedFaceFormLen;					//Õû¸öÒ»ÆÁÊı¾İ³¤¶È
	int16               uiBedFaceDataLen;					//³¤Ãû×Ö´²Í·¿¨µÄÒ»ÆÁÊı¾İ³¤¶È
	uint16				uiDietDataLen;						//ÒûÊ³Êı¾İ³¤¶È
	uint16				uiPrescriptionDataLen;				//Ò½ÖöÊı¾İ³¤Üû
	uint16              uiPromptDataLen;					//ÌáÊ¾ĞÅÏ¢Êı¾İ³¤¶È
	uint16 				uiSendedDataLen;					//ÒÑ·¢Êı¾İ³¤¶È
	uint16 				uiSendSBTotalDataLen;				//·¢ËÍÊÖ±ú×Ü¼ÆÊı¾İ³¤¶È
	uint8				bySendSBTimes;						//·¢ËÍÊÖ±ú´ÎÊı
	
	uint8				byUsart1SdDelayTime;				//´®¿Ú1·¢ËÍÊı¾İÑÓ³ÙÊ±¼ä
	uint8				bySlaveState;						//·Ö»ú×´Ì¬
//#define PATIENT_DISCHARGE	0x01		//1:²¡ÈË³öÔº
//#define LCD_ON				0x02		// 1 :±³¹â´ò¿ª	¾
	uint16 				uiLcdDisplayTime;					//ÏÔÊ¾ÆÁÏÔÊ¾Ê±¼ä
	uint16 				uiCC1101ReReadTime;					//cc1101ÖØ¶Á¼ä¸ôÊ±¼ä
	STItemForm          stItemForm;							//ÏîÄ¿¸ñÊ½
	STItemData		    stItemData;							//ÏîÄ¿Êı¾İ
} STLocalControl, *pSTLocalControl;
//5msPCAÖĞ¶Ï¼ÆÊ±´ÎÊı£¬Fosc=18.432MHz,·ÖÆµÏµÊıÎª2 
#define	Fosc				18432000L
#define Fbus				184320
			
#define ISR_INC_COUNT		(Fosc/2/OS_TICKS_PER_SEC)
#define	IAP_ADDR			0x0000		//STC12LE5A60S2ÄÚµÄE2
#define INIT_FLAG           0x5a


#ifndef _IN_MAIN
#define _IN_MAIN
extern uint16	xdata uiIsrTimerCount ;					//ÏµÍ³Ê±ÖÓ¶¨Ê±Öµ
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];						//Ö÷Ïß³ÌÏûÏ¢¶ÓÁĞ
extern STLocalControl 	xdata stLocalControl;						//È«¾Ö±äÁ¿½á¹¹Ìå
extern bit bWireLessRec;
//extern bit bOXSupplyState;
extern bit bEnableOxTimer;





extern uint8 xdata byWireLessRecLen;
extern uint8 xdata byWireLessRecBuff[];
extern uint8 xdata byWireLessSndBuff[];

extern void UserTickTimer(void);
extern void Send_Data(uint8 *Databuf,uint8 xdata l);
extern void Send_Data_Byte(uint8 SendData);
extern STOXTime   timeInterval(STTime a,STTime   b);
extern uint16   allday(STTime   a);

#endif
