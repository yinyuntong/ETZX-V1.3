/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SingleBus.h
*�ļ�����    		:�����߳���ͷ�ļ�
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
//����0�궨��
#define 	BUS0_FREQ_SIZE		7							//������0����֡����
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*4 + 1)		//������0���ݶ��г���
#define 	DisableBus0RecInt()	{EX0 = 0;PW=1;} 			//������0��ֹ�����жϺ궨��
#define 	EnableBus0RecInt()	{IE0 = 0;EX0 = 1;}			//������0�ָ������жϺ궨��
#define 	BUS0_SEND_CON		0x3C
#define 	BUS0_CAN_SEND 		0x0C 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0C
#define 	BUS0_ON_WORK		0x23
#define     TIMER0_H  0xFF
#define     TIMER0_L  0x67

 
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
	uint8 bySndSecAddr;			/*�������ͷ�����ַ*/
	uint8 bySndRoomAddr;		/*�������ͷ�����ַ*/
	uint8 bySndBedAddr;			/*�������ͷ�����ַ*/
	uint8 byCmd;				/*����*/
	uint8 byRecSecAddr;			/*���շ�����ַ*/
	uint8 byRecRoomAddr;		/*���շ�����ַ*/
	uint8 byRecBedAddr;			/*���շ�����ַ*/	
} STBusFreq, *pSTBusFreq;	
/*************************�����**********************/
#define CMD_QUEST				0x01	/*��ѯ����*/
#define CMD_ANSWER				0x02	/*Ӧ������*/

#define CMD_BUS_ANSWER			0x02	//�����߻�Ӧ������

#define CMD_LANDING				0x03	/*�Ǽ�����*/			/*���������ֽ�Ϊ���ȼ���0x00*/
#define CMD_ENTER				0x04	/*ȷ������*/			/*ֻ��Ϊ�����Դӻ�����*/
/*****************************************************************/
#define CMD_COMM_CALL			0x05	/*��ͨ��������*/		/*���������ֽ�Ϊ���ȼ�������״̬*/
#define CMD_INFUSION_CALL		0x06	/*��Һ��������*/
#define CMD_SERVICE_CALL		0x07	/*�����������*/
#define CMD_EMERGENCY_CALL		0x08	/*������������*/
#define CMD_HELP_CALL			0x09	/*��Ԯ��������*/		/* ����õ�������ȷ������*/
#define CMD_INSPECTOR_CALL		0x0a	/*�鷿��������*/		/* ����õ�������ȷ������*/
/*****************************************************************/
#define CMD_COMM_ANSWER			0x0b	/*��ͨӦ������*/		
#define CMD_INFUSION_ANSWER		0x0c	/*������Һ����*/
#define CMD_SERVICE_ANSWER		0x0d	/*������������*/
#define CMD_EMERGENCY_ANSWER	0x0e	/*������������*/
#define CMD_HELP_ANSWER			0x0f	/*������Ԯ����*/
/*****************************************************************/
#define CMD_INFUSION_CLEAR		0x10	/*�����Һ����*/		/* ��Ӧ���������*/
#define CMD_SERVICE_CLEAR		0x11	/*�����������*/
#define CMD_EMERGENCY_CLEAR		0x12	/*�����������*/
#define CMD_HELP_CLEAR			0x13	/*�����Ԯ����*/
/*****************************************************************/
#define CMD_BROADCAST1			0x14	/*1��㲥��������*/
#define CMD_BROADCAST2			0x15	/*2��㲥��������*/
#define CMD_CALL_LISTEN			0x16	/*������������*/
#define CMD_PRIORITY_SET		0x17	/*���ȼ�ָ������*/		/*ϵͳ�����һ���ֽڸ�3λΪ���ȼ������շ���ַ*/
#define CMD_SYSTERM_RESET		0x18	/*ϵͳ��λ����*/
/*****************************************************************/
#define CMD_DATA_SEND			0x19	/*���ڷ�������*/		/*ǰ�����ֽڣ��꣺4λBCD�룬���������ֽڣ��º����ڣ�4λBCD��*/
#define CMD_LCD_SEND			0x1a	/*�����ݷ�������*/
#define CMD_SCHEDULE_SEND		0x1b	/*һ������������*/
#define CMD_NUMBER_SET			0x1c	/*�޸ı������*/		/*�µĵ�ַ��ǰ2���ֽڣ����޸ĵ�ַ�ں������ֽ�*/
#define CMD_HANDLER_SEND		0x1d	/*�����ֱ�Һ������*/
/*****************************************************************/
#define CMD_CONTROL_SEND		0x1e	/*�����ִ�������*/
#define CMD_FLOOR_SET			0x1f	/*�޸�¥�������*/
#define CMD_BROADCAST3			0x20	/*3��㲥��������*/
#define CMD_INFUSION_ENTER		0x21	/*ȷ����Һ����*/
#define CMD_SERVICE_ENTER		0x22	/*ȷ�Ϸ�������*/
#define CMD_HELP_ENTER			0x23	/*ȷ����Ԯ����*/
#define CMD_EMERGENCY_ENTER		0x24	/*ȷ�Ͻ�������*/
/*****************************************************************/
#define CMD_CLEAR_INSPERCTOR	0x25	/*����鷿����*/
#define CMD_START_VOICE			0x26	/*������������*/
#define CMD_STOP_VOICE			0x27	/*ֹͣ��������*/
#define CMD_MUSIC_PLAY			0x28	/*������������*/
#define CMD_REQUEST_INFO		0x29	/*��������Ϣ����*/
/*****************************************************************/
#define CMD_CHANNEL_CHANGE		0x2a	/*�л�ͨ��ͨ��*/
#define CMD_CHANNEL_CLOSE		0x2b	/*�ر�ͨ��ͨ��*/
#define CMD_POWER_ON			0x2c	/*�ϵ縴λ����*/
#define CMD_INSPERCTOR_ENTER	0x2d	/*ȷ�ϲ鷿����*/
#define CMD_STOP_INDICATION		0x2e	/*��ֹ������Ϣ����*/
#define CMD_INFO_INDICATION		0x2f	/*��Ϣָʾ����*/
/*�������з���ַ��ǰ�����ֽڣ����������ֽ�Ϊ�������ͣ���3λ���ȼ�����5λΪ���*/
/*****************************************************************/
#define CMD_MUSIC				0x30	/*��������*/
#define CMD_INCREACE_VOL		0x31	/*������������*/
#define CMD_DECREACE_VOL		0x32	/*������С����*/
#define	CMD_OPEN_485BUS_IN_ACK	0x34	/*�ӻ���485��������*/
#define	CMD_OPEN_485BUS_OUT_ACK	0x35	/*�ӻ���485�������*/
#define CMD_OPEN_LCD			0x3e	/*����ʾ����*/
#define CMD_CLOSE_LCD			0x3f	/*�ر���ʾ����*/
/*****************************************************************/
#define CMD_OPEN_485BUS_OUT		0x40	/*��485���*/
#define CMD_OPEN_485BUS_IN		0x41	/*��485��������*/
#define CMD_CLOSE_485BUS		0x42	/*�ر�485����*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE		0x43	/*����ֱ�Һ����Ϣ����*/
#define CMD_KEY_AVALID			0x44	/*������Ч����*/
#define CMD_KEY_INVALID			0x45	 /*����ʧЧ����*/
#define CMD_KEY_DOWN			0x46	/*����ѹ������*/
#define CMD_KEY_UP				0x47	/*�����ͷ�����*/
#define CMD_ERROR_REPORT		0x48	/*���ϱ�������*/
#define CMD_CLEAR_LCD			0x49	/*���Һ����Ϣ����*/
#define CMD_NO_BUG				0x4c	/*�����������*/
#define CMD_NO_BUG_AVALID		0x4d	/*���������Ч����*/
#define CMD_BUG_END				0x4e	/*������������*/

#define CMD_SICK_SEND			0x51	/*������ʶ����*/
#define CMD_CLEAR_SICK			0x52	/*���������ʶ��Ϣ*/
#define CMD_SEC_SET				0x53	/*�ſڿ���������Ϣ����*/
#define CMD_SEC_CLR				0x54	/*�ſڿ���������Ϣ���*/
#define CMD_WRITE_SERIAL		0x55	/*д�����к�*/
#define CMD_READ_SERIAL			0x56	/*�����к�*/
#define CMD_OUT_SERIAL			0x57	/*���к����*/
#define CMD_SET_BY_SERIAL		0x5a	/*�������к�д�豸��ַ*/
#define CMD_SET_SEC_SERIAL		0x5b	/*����ϵͳ����*/
#define CMD_OUT_BY_SERIAL		0x5c	/*�������кŶ��豸��ַ���*/
//--------------------------------------------------------------------
#define CMD_ENABLE_SOUND		0x61	/*������������*/
#define CMD_DISABLE_SOUND		0x62	/*��ֹ��������*/
//--------------------------------------------------------------------
#define	CMD_LED_INDICATE		0x70	/*��ָʾ����,���ͷ���ַ��Ч������ַΪ��״̬*/
#define CMD_GET_BUS				0x71	/*ռ����������*/

#define CMD_DISPLAY_DATA		0x80	/*����һ֡Һ��������*/
#define CMD_DATA_ERROR			0x81	/*һ֡���ݳ�������*/
#define CMD_NURSE_COME		    0x83	/*��ʿ��λ����*/
#define CMD_NURSE_BACK		    0x84	/*��ʿ�뿪����*/
/*****************************************************************/
#define CMD_ONLINE_WRITE		0xf3	/*����д������*/
#define CMD_ONLIEN_READ			0xf4	/*���߶�������*/
#define CMD_ONLINE_OUTPUT		0xf5	/*�����������*/
/*****************************************************************/
#define CMD_LED_OFFLINE_WRITE	0xf0	/*ledeeprom����д������*/
#define CMD_LED_OFFLINE_READ	0xf1	/*ledeeprom���߶�������*/
#define CMD_LED_OFFLINE_OUTPUT	0xf2	/*ledeeprom�����������*/
/*****************************************************************/
#define CMD_EEPROM_OFFLINE_WR	0xfe	/*eeprom����д������*/
#define CMD_EEPROM_OFFLINE_RD	0xf1	/*eeprom���߶�������*/
#define CMD_EEPROM_OFFLINE_OUTT	0xf2	/*eeprom�����������*/ 
/*****************************************************************/
#define CMD_BUS0_CHECK			  0xA4	/*�����߼������*/
#define CMD_SD_TAL_VOL_CHECK	  0xA8	/*ͨ�������SD���ϵ�ѹ���*/
#define CMD_SA_TAL_VOL_CHECK      0xA9	/*ͨ�������SA���ϵ�ѹ���*/
#define CMD_SD_TAL_VOL_CHECK_END  0xAA	/*ͨ�������SD���ϵ�ѹ������*/
#define CMD_SA_TAL_VOL_CHECK_END  0xAB	/*ͨ�������SA���ϵ�ѹ������*/
#define CMD_RS485_CHECK			  0xAC	/*RS485ͨѸ���*/
#define CMD_SET_BAUD	          0x38	/*����ϵͳ����������*/ 
/*****************************************************************/
#ifdef _IN_SINGLE_BUS_


uint8 	bdata 	byDevState1;
sbit  	bBusy 				= 	byDevState1^7;				//ϵͳæ��־
sbit	bSetKeyDown			=	byDevState1^6;				//���ü�����״̬
sbit	bDealKeyDown		=	byDevState1^5;				//����������״̬
sbit	bNurseIn			=	byDevState1^4;	 			//��ʿ��λ��־
sbit	bChannel1Talked		=	byDevState1^3;				//ͨ��1����ͨ����־
sbit	bChannel1Talk		=	byDevState1^2;				//ͨ��1����ͨ����־
sbit	bIndicatingOther	=	byDevState1^1;				//ָʾ�����ֻ����б�־
sbit	bLanding			=	byDevState1^0;				//�ϵ�Ǽ�״̬
uint8 	bdata 	byDevState2;
sbit  	bWaitAck 			= 	byDevState2^7;				//�ȴ�Ӧ���־
sbit  	bWaitListen			= 	byDevState2^6;				//�ȴ�������־
sbit	bChannel0Talked		=	byDevState2^5;				//ͨ��0����ͨ����־
sbit	bChannel0Talk		=	byDevState2^4;				//ͨ��0����ͨ����־
sbit	bSickRoomBroad		=	byDevState2^3;				//�����㲥��־
sbit	bOfficeBroad		=	byDevState2^2;				//�칫���㲥��־
sbit	bAllBroad			=	byDevState2^1;				//ȫ���㲥��־
sbit	bSelfBroad			=	byDevState2^0;				//�����㲥��־
uint8 	bdata 	byDevState3;
sbit	bCalledRing			=	byDevState3^7;				//���������־
sbit	bVoiceNoting		=	byDevState3^6;				//������ʾ��־
sbit	bMusicPlaying		=	byDevState3^5;				//�������ֲ��ű�־
sbit	bConfusionNoting	=	byDevState3^4;				//��Һ������ʾ��־
sbit	bServiceNoting		=	byDevState3^3;				//���������ʾ��־
sbit	bHelpNoting			=	byDevState3^2;				//��Ԯ������ʾ��־
sbit	bEmergencyNoting	=	byDevState3^1;				//����������ʾ��־ 
sbit	bPWState			= 	byDevState3^0;
uint8 	bdata 	byDevState4;
sbit	bIndication         =	byDevState4^0;				//������Ϣָʾ
sbit	bLcdState	        =	byDevState4^1;				//Һ������״̬
//���ɱ�־����
uint8 	bdata byEnable1;
sbit	bEnSickRoomBroad	= 	byEnable1^0;				//���ɲ����㲥
sbit	bEnOfficeBroad		=	byEnable1^1;				//���ɰ칫���㲥
sbit	bEnAllBroad			=	byEnable1^2;				//����ȫ���㲥
sbit	bEnSoundNote		=	byEnable1^3;				//����������ʾ
sbit	bEnOutRing			=	byEnable1^4;				//�����ⲿ����
sbit	bEnAutoListen		=	byEnable1^5;				//�����Զ�����
sbit	bEnVoiceNote		=	byEnable1^6;				//����������ʾ
sbit	bEnKeyVoice			=	byEnable1^7;				//���ɰ�����ʾ��
uint8 	bdata byEnable2;
sbit	bEnInfusionDeal		=	byEnable2^0;				//������Һ���д���
sbit	bEnServiceDeal		=	byEnable2^1;				//���ɷ�����д���
sbit	bEnHelpDeal			=	byEnable2^2;				//������Ԯ���д���
sbit	bEnEmergencyDeal	=	byEnable2^3;				//������Ԯ���д���
sbit	bEnCallDeal			=	byEnable2^4;				//���ɺ��д���
sbit	bEnMusicplay		=	byEnable2^5;				//���ɱ������ֲ���
#else
extern	uint8 	byDevState1;
extern	uint8 	byDevState2;
extern	uint8 	byDevState3;
extern	uint8 	byDevState4;
extern 	uint8 	byEnable1;
extern	uint8 	byEnable2;
extern	bit  	bIndication;
extern	bit  	bLcdState;
extern	bit  	bBusy;
extern 	bit		bSetKeyDown;
extern	bit		bDealKeyDown;
extern	bit		bNurseIn;
extern	bit		bChannel1Talked;
extern	bit		bChannel1Talk;
extern	bit		bIndicatingOther;
extern	bit		bLanding;
extern	bit  	bWaitAck;
extern	bit  	bWaitListen;
extern	bit		bChannel0Talked;
extern	bit		bChannel0Talk;
extern	bit		bSickRoomBroad;
extern	bit		bOfficeBroad;
extern	bit		bAllBroad;
extern	bit		bSelfBroad;	
extern	bit		bCalledRing;
extern	bit		bVoiceNoting;
extern	bit		bMusicPlaying;
extern	bit		bConfusionNoting;
extern	bit		bServiceNoting;
extern	bit		bHelpNoting;
extern	bit		bEmergencyNoting;
extern	bit		bPWState;
/********************************************/
extern	bit		bEnSickRoomBroad;
extern	bit		bEnOfficeBroad;
extern	bit		bEnAllBroad;
extern	bit		bEnSoundNote;
extern	bit		bEnOutRing;
extern	bit		bEnAutoListen;
extern	bit		bEnVoiceNote;
extern	bit		bEnKeyVoice;
extern	bit		bEnInfusionDeal;
extern	bit		bEnServiceDeal;
extern	bit		bEnHelpDeal;
extern	bit		bEnEmergencyDeal;
extern	bit		bEnCallDeal;
extern	bit		bEnMusicplay;
void SingleBusInit(void); 	
uint8 Bus0OutputData(uint8* pbyData);   
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void Bus0RecDeal(void);
void Bus0SendDeal(void);
//void Bus0Manage(void);
void TimerOutDeal(void);
//void TimerOutManager(void);
bit  AddrCompare(pSTBusFreq pstBusFreq);
bit  AddrCompareCheck(pSTBusFreq pstBusFreq);
void ReadPageColour(void);

#endif