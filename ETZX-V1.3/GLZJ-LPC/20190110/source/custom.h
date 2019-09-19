#include "LPC17xx.h"                                                    /* LPC17xx����Ĵ���            */
#include "lpc1700_reg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
//---------------------------------------------------------------
#define MALLOC_BUF	0


#define INIT_FLAG 0x5a

#define PARA_ADDR	0x3ff000	//4M �ֽ����һ������
#define FLASH_TEST_ADDR	0x3ff000


//���������ֽṹ��
typedef struct _STEepromCfgData
{
	uint8_t  byInitFlag;									//��ʼ����־
    uint8_t  bySelfSecAddr;									//����ַ
    uint8_t  bySelfRoomAddr;								//����ַ  
    uint8_t  bySelfBedAddr;									//����ַ
    uint16_t uiRs485Brt;									//RS485������

    uint8_t  byBLOffStartHour;								//�������ʼСʱ
    uint8_t  byBLOffStartMin;								//�������ʼ����
    uint8_t  byBLOffEndHour;								//����ؽ���Сʱ
    uint8_t  byBLOffEndMin;									//����ؽ�������
    uint8_t  byLVoiceStartHour;								//С����ʱ����ʼСʱ
    uint8_t  byLVoiceStartMin;								//С����ʱ����ʼ����
    uint8_t  byLVoiceEndHour;								//С����ʱ�ν���Сʱ
    uint8_t  byLVoiceEndMin;								//С����ʱ�ν�������
    uint8_t	 byLVoiceVal;									//С����ֵ

    uint8_t  bPlayRoom;										//���ŷ��ű�־

	uint8_t  byHostPlayVol;									//������������
	uint8_t  bySlaPlayVol;									//�ֻ���������

	uint8_t	 byHostPlayLittleVol;							//��������С����
	uint8_t  bySlaPlayLittleVol;							//�ֻ�����С����

	
	uint8_t  byHostTalkVol;									//����ͨ������
	uint8_t  bySlaTalkVol;									//�ֻ�ͨ������
    
    uint8_t  byVersionH;									//�汾��λ
    uint8_t  byVersionL;									//�汾��λ

	uint8_t	 bySysMode;										//ϵͳģʽ
	//��4λWIFI��أ���4λ�������
/*	#define USE_WIFI_MODE		0x80						//ʹ��WIFI��ʽ���д�ͷ����Ϣ����
	#define USE_WIFI_DEBUG		0x40						//��WIFI���ӹ����м��������е���Ϣ��ӡ����,������USE_WIFI_MODE=1
	#define USE_WIFI_REC_DEBUG	0x20						//��WIFI���մ�ͷ������Ϣ��ӡ����,������USE_WIFI_MODE=1

	#define USE_USART2_MODE		0x08						//ʹ�ô��ڽ��մ�ͷ����Ϣ(������������)
	#define USE_USART2_SD_DATA	0x04						//ʹ�ô��ڷ���Ӧ�𡢽������
	#define USE_TIME_DEBUG			0X01						//����������ʱ������������ȷ

*/
	
} STEepromCfgData, *pSTEepromCfgData;




/*����֡��ʽ
�ֽ�1�����ͷ�����ַ��һ���ֽڣ�
�ֽ�2�����ͷ�����ַ��һ���ֽڣ�
�ֽ�3�����ͷ�����ַ��һ���ֽڣ�
�ֽ�4��������루һ���ֽڣ�
�ֽ�5�����շ�����ַ��һ���ֽڣ�
�ֽ�6�����շ�����ַ��һ���ֽڣ�
�ֽ�7�����շ�����ַ��һ���ֽڣ�
ע:�������ڲ�ͬ�������п����в�ͬ����
*/

/*������Э��
  �������:һ�ֽ�
  ���ݴ���:6�ֽ�(��������Ĳ�ͬ���ݸ�ʽ)
*/
typedef struct
{
	uint8_t bySndSecAddr;			/*�������ͷ�����ַ*/
	uint8_t bySndRoomAddr;		/*�������ͷ�����ַ*/
	uint8_t bySndBedAddr;			/*�������ͷ�����ַ*/
	uint8_t byCmd;				/*����*/
	uint8_t byRecSecAddr;			/*���շ�����ַ*/
	uint8_t byRecRoomAddr;		/*���շ�����ַ*/
	uint8_t byRecBedAddr;			/*���շ�����ַ*/	
} STBusFreq, *pSTBusFreq;	



//˫������ڵ�
typedef struct _STNod
{
    uint8_t  byCallSecAddr;									//���з�����ַ
    uint8_t  byCallRoomAddr;									//���з�����ַ
    uint8_t  byCallBedAddr;									//���з�����ַ
    uint8_t  byCallPriority;									//�������ȼ�
    struct _STNod *pstPrev;
    struct _STNod *pstNext;    
} STNod, *pSTNod;
//˫������ṹ��
typedef struct _STPointer
{
    struct _STNod *pstHead;									//����ͷ�ڵ�
    struct _STNod *pstTail;									//����β�ڵ�    
} STPointer, *pSTPointer;
//��Ϣָʾ���ṹ��
typedef struct _STIndicationData
{
    uint8_t  byIndicationSecAddr;								//ָʾ������ַ
    uint8_t  byIndicationRoomAddr;							//ָʾ������ַ  
    uint8_t  byIndicationBedAddr;								//ָʾ������ַ 
    uint8_t  byCallCmd;										//ָʾ������
    uint8_t  byState;											//�ú�����Ϣ��״̬
    struct _STNod *pstNext;    								//��һ��������Ϣָ��
} STIndicationData, *pSTIndicationData;
//��ʱ����ṹ��
typedef struct _STTimerout
{	
	uint8_t  byTimerOutSet;									//һ�γ�ʱʱ������(20ms��λ)
	uint8_t  byTimerOut;										//һ�γ�ʱʱ��(20ms��λ)
	uint8_t  byTimerOutCount;									//����ʱ����	
} STTimerout, *pSTTimerout;
//��ַ�ṹ��
typedef struct
{
	uint8_t  bySndSecAddr;										//���ͷ�����ַ
    uint8_t  bySndRoomAddr;										//���ͷ�����ַ  
    uint8_t  bySndBedAddr;										//���ͷ�����ַ
    uint8_t  byRecSecAddr;
	uint8_t  byRecRoomAddr;
	uint8_t  byRecBedAddr;

    uint8_t  byCmd;											//ԭ����
} STAddr, *pSTAddr;


//��ַ�ṹ��
typedef struct
{
	uint8_t  bySecAddr;										//����ַ
    uint8_t  byRoomAddr;									//����ַ  
    uint8_t  byBedAddr;										//����ַ									
} STAddr_S, *pSTAddr_S;


typedef struct queue
{
	uint16_t queuesize;		//����Ĵ�С
	uint16_t head,tail;		//���е�ͷ��β�±�
	uint8_t *q;				//����ͷָ��
	
}Queue;



//ʱ��ṹ��
typedef struct
{
uint8_t bySecond;
uint8_t byMinute;
uint8_t byHour;
//uint8_t byWeek;
uint8_t byDay;
uint8_t byMonth;
uint8_t byYearL;
//uint8_t byYearH;
}STTime;



typedef struct{
  uint16_t		Year;
  uint8_t 		Month;
  uint8_t		Date;
  uint8_t		Week;
  uint8_t		Hour;
  uint8_t		Min;
  uint8_t		Sec;
}Rtc_struct;


//���ڽṹ�嶨��
typedef struct
{
	uint16_t uiSndLen;		/*�������ݳ���*/
	uint16_t uiSndPos;		/*��������λ��*/	
} STUsartCfg, pSTUsartCfg;



typedef struct
{
	uint8_t	byStartByte1;					//����֡��ʼ�ַ�1
	uint8_t 	bySndSecAddr;					//�������ͷ�����ַ
	uint8_t 	bySndRoomAddr;					//�������ͷ�����ַ
	uint8_t 	bySndBedAddr;					//�������ͷ�����ַ	
	uint8_t 	byRecSecAddr;					//�շ�����ַ
	uint8_t 	byRecRoomAddr;					//�շ�����ַ
	uint8_t 	byRecBedAddr;					//�շ�����ַ
	uint8_t 	byCmd;							//����
	uint8_t		byLenH;							//���ȸ��ֽ�
	uint8_t 	byLenL;							//���ȵ��ֽ�
	uint8_t   byData[1];						//��·����	
} STUartFreq, *pSTUartFreq;


#include "uart.h"
//#include "sst25vfxxx.h"
#include "pct25vf.h"
#include "global.h"
#include "wdt.h"
#include "voice_play.h"
#include "timer.h"
#include "interrupt.h"
#include "Extint.h"
#include "SingleBus.h"
#include "tel.h"
#include "ds3231.h"
#include "wpa_cli.h"
#include "fm62429.h"


#define	enable_wdt				


//��4λWIFI��أ���4λ�������
#define USE_WIFI_MODE		0x80						//ʹ��WIFI��ʽ���д�ͷ����Ϣ����
#define USE_WIFI_DEBUG		0x40						//��WIFI���ӹ����м��������е���Ϣ��ӡ����,������USE_WIFI_MODE=1
#define USE_WIFI_REC_DEBUG	0x20						//��WIFI���մ�ͷ������Ϣ��ӡ����,������USE_WIFI_MODE=1

#define USE_USART2_MODE 	0x08						//ʹ�ô��ڽ��մ�ͷ����Ϣ(������������)

#define USE_TIME_DEBUG		0X01						//����������ʱ������������ȷ

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


#define PLAY_HOST_CH	0
#define PLAY_SLAVE_CH	1

#define TALK_HOST_CH	0
#define TALK_SLAVE_CH	1

/////////////////////////////////////////////////////////////////////////////
#define		sst_cs			(1ul<<16)		//p0.16
#define		sst_sck			(1ul<<15)		//p0.15
#define		sst_miso		(1ul<<17)		//p0.17
#define		sst_mosi		(1ul<<18)		//p0.18

#define		sst_cs_ctrl(x)			(x?(FIO0SET = sst_cs):(FIO0CLR = sst_cs))
#define 	sst_sck_ctrl(x)			(x?(FIO0SET = sst_sck):(FIO0CLR = sst_sck))
#define 	sst_mosi_ctrl(x)		(x?(FIO0SET = sst_mosi):(FIO0CLR = sst_mosi))
#define 	sst_miso_status			(FIO0PIN&sst_miso)		
/////////////////////////////////////////////////////////////////////////////
#define		int0_status				(FIO2PIN&(1ul<<12))
#define		MTT_TD_pin				(1ul<<12)		//p2.12
#define		MTT_TD_pin_status		(FIO2PIN&MTT_TD_pin)
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#define		sd_rx_pin				(1ul<<1)		//p0.1
#define		sd_rx_pin_status		(FIO0PIN&sd_rx_pin)

#define 	sd_tx_pin				(1ul<<13)		//p2.13
#define		sd_tx_ctrl(x)			(x?(FIO2SET = sd_tx_pin):(FIO2CLR = sd_tx_pin))	

/////////////////////////////////////////////////////////////////////////////
#define 	P0Int    1					//PORT0 GPIO�жϹ���״̬
#define 	P2Int    (1ul<<2)			//PORT2 GPIO�жϹ���״̬
/////////////////////////////////////////////////////////////////////////////
#define     tlp_irq_pin				(1ul<<23)
#define 	phone_tlp				(1ul<<23)		//p0.23 �绰ժ��
#define 	phone_tlp_status		(FIO0PIN&phone_tlp)

#define		phone_ring				(1ul<<31)		//p1.31
#define		phone_ring_ctrl(x)		(x?(FIO1SET = phone_ring):(FIO1CLR = phone_ring))


#define		phone_tlq				(1ul<<28)	   	//p1.28 �绰����
#define		phone_tlq_ctrl(x)		(x?(FIO1SET = phone_tlq):(FIO1CLR = phone_tlq))	


#define 	tel_ctr_pin				(1ul<<5)		//p0.5   ��ѹѡ��
#define 	AC36V					1				//��������
#define 	DC60V					0				//������ͨ��ֱ����ѹ
#define		tel_ctrl(x)				(x?(FIO0SET = tel_ctr_pin):(FIO0CLR = tel_ctr_pin))	

#define 	AM79R70_c1_pin			(1ul<<18)		//p1.18
#define 	AM79R70_c1_ctrl(x)		(x?(FIO1SET = AM79R70_c1_pin):(FIO1CLR = AM79R70_c1_pin))


#define 	AM79R70_c2_pin			(1ul<<19)		//p1.19
#define 	AM79R70_c2_ctrl(x)		(x?(FIO1SET = AM79R70_c2_pin):(FIO1CLR = AM79R70_c2_pin))


#define 	AM79R70_c3_pin			(1ul<<22)		//p1.22
#define 	AM79R70_c3_ctrl(x)		(x?(FIO1SET = AM79R70_c3_pin):(FIO1CLR = AM79R70_c3_pin))

/////////////////////////////////////////////////////////////////////////////
#define mt8870_dq1		(FIO0PIN&(1<<19))
#define mt8870_dq2		(FIO0PIN&(1<<20))
#define mt8870_dq3		(FIO0PIN&(1<<21))
#define mt8870_dq4		(FIO0PIN&(1<<22))
/////////////////////////////////////////////////////////////////////////////
//ds3231оƬ
#define DS3231_SCL			(1<<3)	//p2.3
#define DS3231_SDA			(1<<2)	//p2.2
#define DS3231_RST          (1<<4)  //P2.4

#define DS3231_SDA_MOD		4

#define Set_DS3231_SDA      FIO2SET = DS3231_SDA
#define Clr_DS3231_SDA	    FIO2CLR = DS3231_SDA

#define Set_DS3231_SCL  	FIO2SET = DS3231_SCL
#define Clr_DS3231_SCL  	FIO2CLR = DS3231_SCL

#define Set_DS3231_RST  	FIO2SET = DS3231_RST
#define Clr_DS3231_RST      FIO2CLR = DS3231_RST


#define DS3231_SDA_status	(FIO2PIN&DS3231_SDA)		//�����Ϊ0��IO״̬Ϊ�ߵ�ƽ


#define DS3231_DELAY   5

#define DS3231_SDA_INPUT()    LPC_GPIO2->FIODIR &= ~DS3231_SDA		//��λΪ0����������
#define DS3231_SDA_OUTPUT()   LPC_GPIO2->FIODIR |= DS3231_SDA

/////////////////////////////////////////////////////////////////////////////
#define gb_fz_pin					(1ul<<26)		//P3.26 	�㲥����
#define	gb_fz_ctrl(x)				(x?(FIO3SET = gb_fz_pin):(FIO3CLR = gb_fz_pin))	

#define xtd_pin						(1ul<<25)		//p3.25	�Խ�ʱSDͨ������
#define	xtd_ctrl(x)					(x?(FIO3SET = xtd_pin):(FIO3CLR = xtd_pin))

#define xta_pin						(1ul<<20)		//p1.20	�Խ�ʱSAͨ������
#define	xta_ctrl(x)					(x?(FIO1SET = xta_pin):(FIO1CLR = xta_pin))


#define kbd_pin						(1ul<<21)		//p1.21	���ű�������SAͨ������
#define	kbd_ctrl(x)					(x?(FIO1SET = kbd_pin):(FIO1CLR = kbd_pin))

#define kgb_pin						(1ul<<23)		//p1.23	��������ʱSDͨ������
#define	kgb_ctrl(x)					(x?(FIO1SET = kgb_pin):(FIO1CLR = kgb_pin))

#define kbc_pin						(1ul<<24)		//p1.24	��������ʱSAͨ������
#define	kbc_ctrl(x)					(x?(FIO1SET = kbc_pin):(FIO1CLR = kbc_pin))


#define cgb_pin						(1ul<<29)		//p1.29	ͨ��ʱ�绰��SD����
#define	cgb_ctrl(x)					(x?(FIO1SET = cgb_pin):(FIO1CLR = cgb_pin))

#define cbd_pin						(1ul<<0)		//p0.0	ͨ��ʱ�绰��SA����
#define	cbd_ctrl(x)					(x?(FIO0SET = cbd_pin):(FIO0CLR = cbd_pin))


/////////////////////////////////////////////////////////////////////////////

#define power_en_pin				(1ul<<25)		//p1.25  lm317��Դ����
#define	power_en_ctrl(x)			(x?(FIO1SET = power_en_pin):(FIO1CLR = power_en_pin))

#define para_reset_key				(1ul<<26)		//p1.26   para_rst _key  ������λ���� 
#define	para_reset_key_status		(FIO1PIN&para_reset_key)

#define soft_reset_key				(1ul<<8)		//p0.8	����ϵͳ������
#define soft_reset_key_status		(FIO0PIN&soft_reset_key)

#define wifi_ap_led_pin				(1ul<<7)		//p0.7	power led ��ʾ
#define	wifi_ap_led_ctrl(x)			(x?(FIO0SET = wifi_ap_led_pin):(FIO0CLR = wifi_ap_led_pin))

#define wifi_server_led_pin				(1ul<<6)		//p0.6	wifi  led ��ʾ
#define	wifi_server_led_ctrl(x)			(x?(FIO0SET = wifi_server_led_pin):(FIO0CLR = wifi_server_led_pin))
/////////////////////////////////////////////////////////////////////////////

#define msm_txe_pin					(1ul<<4)		//p0.4	������ʾģ�鷢������
#define	msm_txe_ctrl(x)				(x?(FIO0SET = msm_txe_pin):(FIO0CLR = msm_txe_pin))


#define msm_xd_pin					(1ul<<28)		//p4.28	������ʾģ�鷢��
#define	msm_xd_ctrl(x)				(x?(FIO4SET = msm_xd_pin):(FIO4CLR = msm_xd_pin))

#define msm_rx_pin					(1ul<<29)		//p4.29	������ʾģ�����
#define	msm_rx_ctrl(x)				(FIO4PIN&msm_rx_pin)

/////////////////////////////////////////////////////////////////////////////

#define esp_rst_pin					(1ul<<9)		//p0.9	WIFIģ�鸴λ��
#define	esp_rst_ctrl(x)				(x?(FIO0SET = esp_rst_pin):(FIO0CLR = esp_rst_pin))


/////////////////////////////////////////////////////////////////////////////
#define fm624_dat1_pin				(1ul<<9)		//p2.9	�������� ������λ��
#define	fm624_dat1_ctrl(x)			(x?(FIO2SET = fm624_dat1_pin):(FIO2CLR = fm624_dat1_pin))


#define fm624_clk1_pin				(1ul<<8)		//p2.8	�������� ������λ��
#define	fm624_clk1_ctrl(x)			(x?(FIO2SET = fm624_clk1_pin):(FIO2CLR = fm624_clk1_pin))



#define fm624_dat2_pin				(1ul<<27)		//p0.27	ͨ��������λ��
#define	fm624_dat2_ctrl(x)			(x?(FIO0SET = fm624_dat2_pin):(FIO0CLR = fm624_dat2_pin))


#define fm624_clk2_pin				(1ul<<28)		//p0.28	ͨ��������λ��
#define	fm624_clk2_ctrl(x)			(x?(FIO0SET = fm624_clk2_pin):(FIO0CLR = fm624_clk2_pin))



/////////////////////////////////////////////////////////////////////////////
#define NULL			0


#define	add			1
#define delete		0
#define MAX_VICS                    51                                  /*  �쳣�����������Ŀ          */

/*********************************************************************************************************
  ����ֵ����
*********************************************************************************************************/
#define     ZY_OK               0                                       /*  �����ɹ�                    */
#define     ZY_NOT_OK           1                                       /*  ����ʧ��                    */
#define     ZY_PARAMETER_ERR    2                                       /*  ��������                    */
#define     ZY_NO_FIND_FUNCTION 6                                       /*  û�з���ָ������            */
#define     ZY_NO_MEMORY        12                                      /*  �ڴ治��                    */
#define     ZY_TIME_OUT         13                                      /*  ��ʱ                        */
#define     ZY_NO_FIND_OBJECT   16                                      /*  û�з��ֶ���                */


#define IAP_ENTER_ADR   0x1FFF1FF1                                      // IAP��ڵ�ַ����              /

/* 
*  ����IAP������
*/                                     
#define IAP_Prepare             50                                      // ѡ������                     /
                                                                        // ����ʼ�����š����������š�   /                    
#define IAP_RAMTOFLASH          51                                      // �������� FLASHĿ���ַ       /
                                                                        // RAMԴ��ַ    ����д���ֽ���  /
                                                                        // ϵͳʱ��Ƶ�ʡ�               /
#define IAP_ERASESECTOR         52                                      // ��������    ����ʼ������     /
                                                                        // ���������š�ϵͳʱ��Ƶ�ʡ�   /
#define IAP_BLAND_CHK            53                                      // �������    ����ʼ�����š�   /
                                                                        // ���������š�                 
#define IAP_READPARTID          54                                      // ������ID    ���ޡ�           /
#define IAP_BOOTCODEID          55  									// ��Boot�汾�š��ޡ�           /
#define IAP_COMPARE             56 										// �Ƚ�����    ��Flash��ʼ��ַRAM��ʼ��ַ����Ҫ�Ƚϵ��ֽ����� 

#define Read_chip_serial		58

//����IAP����״̬��
#define CMD_SUCCESS                                0
#define INVALID_COMMAND                            1
#define SRC_ADDR_ERROR                             2 
#define DST_ADDR_ERROR                             3
#define SRC_ADDR_NOT_MAPPED                        4
#define DST_ADDR_NOT_MAPPED                        5
#define COUNT_ERROR                                6
#define INVALID_SECTOR                             7
#define SECTOR_NOT_BLANK                           8
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION    9
#define COMPARE_ERROR                              10
#define BUSY                                       11


//����CCLKֵ��С����λΪKHz 
#define IAP_FCCLK            (25000)

//���庯��ָ��  
extern void (*IAP_Entry) (uint32_t ulParam_tab[], uint32_t ulPesult_tab[]);	










