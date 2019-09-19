/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SingleBus.h
*�ļ�����    		:�����߳���ͷ�ļ�
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
//����0�궨��
#define 	BUS0_FREQ_SIZE		7							//������0����֡����
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//������0���ݶ��г���
//#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*5 + 1)		//������0���ݶ��г���
#define 	DisableBus0RecInt()	{EX0 = 0;} 					//������0��ֹ�����жϺ궨��
#define 	EnableBus0RecInt()	{IE0 = 0;EX0 = 1;}			//������0�ָ������жϺ궨��
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23 



//�������Զ�����ʱ��
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000)))
//#define		TIMER0_COUNT		((256-(Fbus*15000/12/1000000))) 	//150us

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34

//100US
#define TIMER0_H  0xff
#define TIMER0_L  0x67


#define BUS_FREE_TIME 1000	//1000*01ms=100ms

#define BUS1_TEST_TIME 3000	//���1����
 
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
#define CMD_ENTER_PC			0x01	/*�Լ�������(PC��)��ȷ������*/
#define CMD_BUS_ANSWER			0x02	/*�������ϵ�ͨ��Ӧ������*/
#define CMD_RESULT_PC			0x02	/*�Լ�������(PC��)�Ľ������*/
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
#define CMD_SERVICE_ANSWER		0x0d	/*�����������*/
#define CMD_EMERGENCY_ANSWER	0x0e	/*�����������*/
#define CMD_HELP_ANSWER			0x0f	/*������Ԯ����*/
/*****************************************************************/
#define CMD_INFUSION_CLEAR		0x10	/*�����Һ����*/		/* ��Ӧ���������*/
#define CMD_SERVICE_CLEAR		0x11	/*�����������*/
#define CMD_EMERGENCY_CLEAR		0x12	/*�����������*/
#define CMD_HELP_CLEAR			0x13	/*�����Ԯ����*/
/*****************************************************************/
#define CMD_BROADCAST1			0x14	/*�����㲥��������*/
#define CMD_BROADCAST2			0x15	//�칫���㲥��������*/
#define CMD_CALL_LISTEN			0x16	/*������������*/
#define CMD_PRIORITY_SET		0x17	/*���ȼ�ָ������*/		/*ϵͳ�����һ���ֽڸ�3λΪ���ȼ������շ���ַ*/
#define CMD_SYSTERM_RESET		0x18	/*ϵͳ��λ����*/
/*****************************************************************/
#define CMD_DATE_SEND			0x19	/*���ڷ�������*/		/*ǰ�����ֽڣ��꣺4λBCD�룬���������ֽڣ��º����ڣ�4λBCD��*/
#define CMD_LCD_SEND			0x1a	/*�����ݷ�������*/
#define CMD_SCHEDULE_SEND		0x1b	/*һ����������*/
#define CMD_NUMBER_SET			0x1c	/*�޸ı������*/		/*�µĵ�ַ��ǰ2���ֽڣ����޸ĵ�ַ�ں������ֽ�*/
#define CMD_HANDLER_SEND		0x1d	/*�����ֱ�Һ������*/
/*****************************************************************/
#define CMD_CONTROL_SEND		0x1e	/*�����ִ�������*/
#define CMD_FLOOR_SET			0x1f	/*�޸�¥�������*/
#define CMD_BROADCAST3			0x20	/*ȫ���㲥��������*/
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

#define CMD_GSM_NUM_SEARCH		0x33	//��ѯGSMģ�������õĺ����ֻ�����
#define CMD_GSM_NUM_SET			0X34	//����GSMģ���еĺ����ֻ�����
#define CMD_GSM_NUM_DEL			0X35	//ɾ��GSMģ�������õĺ����ֻ�����
#define CMD_GSM_FUNC_SEARCH		0X36	//GSMģ�鹦��״̬��ѯ
#define CMD_GSM_FUNC_SET		0X37	//GSMģ�鹦������



#define CMD_OPEN_LCD			0x3e	/*����ʾ����*/
#define CMD_CLOSE_LCD			0x3f	/*�ر���ʾ����*/
/*****************************************************************/
#define CMD_OPEN_485BUS_OUT		0x40	/*��485���*/
#define CMD_OPEN_485BUS_IN		0x41	/*��485��������*/
#define CMD_CLOSE_485BUS		0x42	/*�ر�485����*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE_LCD	0x43	/*����ֱ�Һ����*/
#define CMD_KEY_AVALID			0x44	/*������Ч����*/
#define CMD_KEY_INVALID			0x45	/*����ʧЧ����*/
#define CMD_KEY_DOWN			0x46	/*����ѹ������*/
#define CMD_KEY_UP				0x47	/*�����ͷ�����*/
#define CMD_ERROR_REPORT		0x48	/*���ϱ�������*/
#define CMD_CLEAR_LCD			0x49	/*���Һ����Ϣ����*/
#define CMD_RECEIVE_OK			0x4a	/*�յ���Ϣ����*/
#define CMD_RECEIVE_ERROR		0x4b	/*ѡ����Ϣ����*/
#define CMD_NO_BUG				0x4c	/*�����������*/
#define CMD_NO_BUG_AVALID		0x4d	/*���������Ч����*/
#define CMD_BUG_END				0x4e	/*������������*/

#define CMD_WRITE_SERIAL		0x55	/*д�����к�*/
#define CMD_READ_SERIAL			0x56	/*�����к�*/
#define CMD_OUT_SERIAL			0x57	/*���к����*/
#define CMD_SET_BY_SERIAL		0x5a	/*�������к�д�豸��ַ*/
#define CMD_READ_BY_SERIAL		0x5b	/*�������кŶ��豸��ַ*/
#define CMD_OUT_BY_SERIAL		0x5c	/*�������кŶ��豸��ַ���*/

#define	CMD_LED_INDICATE		0x70	/*��ָʾ����,���ͷ���ַ��Ч������ַΪ��״̬*/
#define CMD_GET_BUS				0x71	/*ռ����������*/
#define CMD_QUEST				0x73	/*��ѯ����*/
#define CMD_QUEST_ANSWER		0x74	/*��ѯӦ������*/
#define CMD_TALK_CLEAR			0X75	/*����ͨ������*/


/*****************************************************************/
#define CMD_DISPLAY_DATA		0x80	/*����һ֡Һ��������*/
#define CMD_DATA_ERROR			0x81	/*һ֡���ݳ�������*/
#define CMD_SB_DISPLAY_DATA		0x82	/*����һ֡Һ��������*/

#define CMD_NURSE_IN			0X83	//��ʿ��λ����
#define CMD_NURSE_OUT			0X84	//��ʿ�뿪����


#define CMD_SUPPLY_OX_START		0X85	//������ʼ
#define CMD_SUPPLY_OX_END		0X86	//��������

#define CMD_RF_NUMSET_START		0x87	//��Һ���������뿪ʼ
#define CMD_RF_NUMSET_OK		0x88	//��Һ����������ɹ�
#define CMD_RF_NUMSET_END		0x89	//��Һ�������������
#define CMD_TELL_RF_NUM		    0x8A	//������ʾģ����Һ�����������

#define CMD_PC_OPEN				0x8b	//��λ��������
#define CMD_PC_CLOSE			0x8C	//��λ���ر�����
#define CMD_SUPPLY_OX_TOTAL		0x8d	//������ʱ�ܼ�����
#define CMD_BUS1_TEST			0x8e	
#define CMD_BUS1_TEST_ANSWER	0x8f

#define CMD_VOICE_TIMER_GET		0x90	//��ȡС����ʱ��
#define CMD_BL_TIMER_GET		0x91	//��ȡ�����ʱ���
#define CMD_WARD_SEC_INFO_SET	0x92	//������Ϣ����

#define CMD_SYSTERM_RESTART		0x93	//ϵͳ���¿�ʼ����
#define CMD_SEND_PIN_BUG		0x94	//������0���Ͷ˹���
#define CMD_SB_DATA_ANSWER		0X95	//�ֱ����ݻ�Ӧ��


/*****************************************************************/

#define CMD_SHORT_CHECK			0xa0	//��λ����·����
#define CMD_VOLTAGE5_CHECK		0xa1	//��λ��5V��ѹ����
#define CMD_VOLTAGE3_CHECK      0xa2	//��λ��3.3V��ѹ����
#define CMD_VOLTAGE1_CHECK      0xa3	//��λ��1.2V��ѹ����
#define CMD_BUS0_CHECK			0xa4	//������1��������
#define CMD_BUS1_CHECK			0xa5	//������1��������

#define CMD_SD_NOR_VOL_CHECK	0xa6	//��̬��SD���ϵ�ѹ����
#define CMD_SA_NOR_VOL_CHECK    0xa7	//��̬��SA���ϵ�ѹ����

#define CMD_SD_TAL_VOL_CHECK	0xa8	//ͨ��������SD���ϵ�ѹ����
#define CMD_SA_TAL_VOL_CHECK    0xa9	//ͨ��������SA���ϵ�ѹ����

#define CMD_SD_TAL_VOL_CHECK_END 0xaA	//ͨ��������SD���ϵ�ѹ���Խ���
#define CMD_SA_TAL_VOL_CHECK_END 0xaB	//ͨ��������SA���ϵ�ѹ���Խ���
/*****************************************************************/


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


extern uint8 Bus0OutputData(uint8* pbyData);  
extern bit Bus0SendPin;


#ifdef _IN_SINGLE_BUS_
//�豸״̬����
uint8 	bdata 	byDevState1;
sbit  	bBusy 				= 	byDevState1^7;				//ϵͳæ��־
sbit	bChannel1Talk		=	byDevState1^2;				//ͨ��1����ͨ����־
sbit	bIndicatingOther	=	byDevState1^1;				//ָʾ�����ֻ����б�־
sbit	bLanding			=	byDevState1^0;				//�ϵ�Ǽ�״̬

uint8 	bdata 	byDevState2;
sbit  	bWaitAck 			= 	byDevState2^7;				//�ȴ�Ӧ���־
sbit  	bWaitListen			= 	byDevState2^6;				//�ȴ�������־
sbit 	bDealKeyDown		=   byDevState2^5;
sbit	bChannel0Talk		=	byDevState2^4;				//ͨ��0����ͨ����־
sbit	bSickRoomBroad		=	byDevState2^3;				//�����㲥��־
sbit	bOfficeBroad		=	byDevState2^2;				//�칫���㲥��־
sbit	bAllBroad			=	byDevState2^1;				//ȫ���㲥��־

uint8 	bdata 	byDevState3;
sbit	bVoiceNoting		=	byDevState3^6;				//������ʾ��־
sbit	bMusicPlaying		=	byDevState3^5;				//�������ֲ��ű�־
sbit	bConfusionNoting	=	byDevState3^4;				//��Һ������ʾ��־
sbit	bServiceNoting		=	byDevState3^3;				//���������ʾ��־
sbit	bHelpNoting			=	byDevState3^2;				//��Ԯ������ʾ��־
sbit	bEmergencyNoting	=	byDevState3^1;				//����������ʾ��־ 






#else
extern	uint8 	bdata byDevState1;
extern	uint8 	bdata byDevState2;
extern	uint8 	bdata byDevState3;



extern	bit  	bBusy;
extern	bit		bChannel1Talk;
extern	bit		bIndicatingOther;
extern	bit		bLanding;

extern	bit  	bWaitAck;
extern	bit  	bWaitListen;
extern  bit  	bDealKeyDown;
extern	bit		bChannel0Talk;
extern	bit		bSickRoomBroad;
extern	bit		bOfficeBroad;
extern	bit		bAllBroad;


extern	bit		bVoiceNoting;
extern	bit		bMusicPlaying;
extern	bit		bConfusionNoting;
extern	bit		bServiceNoting;
extern	bit		bHelpNoting;
extern	bit		bEmergencyNoting;


extern 	bit 	bBus0RecFinish;
extern  bit     bBus0SendFinish;
/********************************************/
void SingleBusInit(void); 	 
void ReadParameter(void);
void InitParameter(void);
void VoiceChannelCtx(void);
void LedControl(void);
void TimerOutDeal(void);	
void Bus0RecDeal(void);
void Bus0SendDeal(void);
void MakeCH0TimerOut(uint8 byTimerOut,uint8 byTimerOutCount);
void MakeCH1TimerOut(uint8 byTimerOut,uint8 byTimerOutCount);
extern void SaveParameter(void);
#endif
