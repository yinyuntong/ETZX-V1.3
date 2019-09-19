/*
**********************************Copyright (c) ****************************
*	  						cwg
*			       All Rights Reserved
*			     		QQ:7084394
*
*---------------------------------File Info---------------------------------
*File Name     		:SingleBus.h
*Descriptor    		:������ͷ�ļ�
*Create By     		:����ͬ
*Create Date   		:2008-8-8
*Version       		:V1.0
*Note	     		:
*
*---------------------------------------------------------------------------
*Modified By  		:
*Modified Date  	:
*Version        	:
*Note	     		:
*
****************************************************************************
*/
#define		BUS0_STOP_TIME		200
#define		BUS0_TMP_STOP_TIME	10
//����0�궨��
#define 	BUS0_FREQ_SIZE		7
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)
//#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*20 + 1)
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)

#define 	DisableBus0RecInt()	{IE0=0;EX0=0;} 
#define 	EnableBus0RecInt()	{IE0=0;EX0=1;}

#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23



//����1�궨��
#define 	BUS1_FREQ_SIZE		7							//������1����֡����
#define 	BUS1_FREQ_SIZE_HI	(BUS1_FREQ_SIZE << 4)
#define 	BUS1_TX_Q_ZISE		(BUS1_FREQ_SIZE*5 + 1)		//������1���ݶ��г���
#define 	DisableBus1RecInt()	{EX1 = 0;} 					//������1��ֹ�����жϺ궨��
#define 	EnableBus1RecInt()	{IE1 = 0;EX1 = 1;}			//������1�ָ������жϺ궨��
#define 	BUS1_SEND_CON		0x3c
#define 	BUS1_CAN_SEND 		0x0c 
#define 	BUS1_ON_REC			0x03
#define		BUS1_REQ_SEND		0x28
#define		BUS1_SEND_FINISH	0x0c
#define 	BUS1_ON_WORK		0x23


//��Ӻ��м�¼ʱ���ܷ��ص�ֵ
#define		SAVE_FAIL			0		//�洢ʧ��
#define		SAVE_ONLY			1		//�洢�ɹ�,Ψһһ�����м�¼
#define		SAVE_NORMAL			2		//�洢�ɹ�,������Ӻ��м�¼
#define		SAVE_EXIST			3		//������¼�Ѿ�����
#define		SAVE_PRIO_HI		4		//�洢�ɹ�,�и����ȼ����д���
#define		STATE_DEL			0		//û�в��ҵ��ü�¼
#define		STATE_EXIST			1		//���ҵ��ü�¼ 
#define		STATE_HI			2		//���Ҽ�¼�������и����ȼ����д���			

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34


//100US
#define TIMER0_H  0xff
#define TIMER0_L  0x67


//uint8 DTimeState=0;
#define bCH0TimerOutDeal	0x01
#define bCH1TimerOutDeal	0x02
#define bWaitCommAckDTime	0x04
#define bWaitAckTimeLand	0x08
#define bWaitRS485AckDTime	0x10
#define bDetectPcTime		0x20
#define bUsart1SdResultDTime 0x40
#define bDataTimeUpdate     0x80


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
	unsigned char bySndSecAddr;			/*�������ͷ�����ַ*/
	unsigned char bySndRoomAddr;		/*�������ͷ�����ַ*/
	unsigned char bySndBedAddr;			/*�������ͷ�����ַ*/
	unsigned char byCmd;				/*����*/
	unsigned char byRecSecAddr;			/*���շ�����ַ*/
	unsigned char byRecRoomAddr;		/*���շ�����ַ*/
	unsigned char byRecBedAddr;			/*���շ�����ַ*/	
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
#define CMD_INFUSION_ANSWER		0x0c	/*������Һ����*/		//������ַ��������з���ַ
#define CMD_SERVICE_ANSWER		0x0d	/*�����������*/
#define CMD_EMERGENCY_ANSWER	0x0e	/*�����������*/
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
#define CMD_DISP_MODE			0x17	//��ͷ�ֻ���ʾģʽ��������
#define CMD_SYSTERM_RESET		0x18	/*ϵͳ��λ����*/
/*****************************************************************/
#define CMD_DATE_SEND			0x19	/*���ڷ�������*/		/*ǰ�����ֽڣ��꣺4λBCD�룬���������ֽڣ��º����ڣ�4λBCD��*/
#define CMD_COLOR_CLEAR			0X1a	//���ɫ������
#define CMD_SCHEDULE_SEND		0x1b	/*һ����������*/
#define CMD_BED_VOICE_PLAY		0x1c	//������������ʱ����ͷ�ֻ��Ƿ���������
/*****************************************************************/
#define CMD_BROADCAST3			0x20	/*3��㲥��������*/
#define CMD_INFUSION_ENTER		0x21	/*ȷ����Һ����*/
#define CMD_SERVICE_ENTER		0x22	/*ȷ�Ϸ�������*/
#define CMD_EMERGENCY_ENTER		0x23	/*ȷ�Ͻ�������*/
#define CMD_HELP_ENTER			0x24	/*ȷ����Ԯ����*/
/*****************************************************************/

#define CMD_START_VOICE			0x26	/*������������*/
#define CMD_STOP_VOICE			0x27	/*ֹͣ��������*/
#define CMD_MUSIC_PLAY			0x28	/*������������*/
/*****************************************************************/
#define CMD_CHANNEL_CHANGE		0x2a	/*�л�ͨ��ͨ��*/
#define CMD_CHANNEL_CLOSE		0x2b	/*�ر�ͨ��ͨ��*/
#define CMD_POWER_ON			0x2c	/*�ϵ縴λ����*/
#define CMD_INSPERCTOR_ENTER	0x2d	/*ȷ�ϲ鷿����*/
#define CMD_STOP_INDICATION		0x2e	/*��ֹ������Ϣ����*/
#define CMD_INFO_INDICATION		0x2f	/*��Ϣָʾ����*/
/*�������з���ַ��ǰ�����ֽڣ����������ֽ�Ϊ�������ͣ���3λ���ȼ�����5λΪ���*/
/*****************************************************************/
#define CMD_INCREACE_VOL		0x31	/*������������*/
#define CMD_DECREACE_VOL		0x32	/*������С����*/

#define CMD_RS485_BRT_SET		0x38	//RS485����������
#define CMD_RS485_INFO_SEC_ROOM	0x39	//��ͷ�ֻ�����RS485��Ϣʱ�Ƿ���Ҫ�ж����š�����
#define CMD_VOICE_PLAY_ROOM		0x3a	//���������Ƿ񲥱�����

#define CMD_OPEN_LCD			0x3e	/*����ʾ����*/
#define CMD_CLOSE_LCD			0x3f	/*�ر���ʾ����*/
/*****************************************************************/
#define CMD_OPEN_485BUS_IN		0x41	/*��485��������*/
#define CMD_CLOSE_485BUS		0x42	/*�ر�485����*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE_LCD	0x43    /*����ֱ�Һ����Ϣ����*/
#define CMD_CLEAR_LCD			0x49	/*���Һ����Ϣ����*/

#define CMD_CLEAR_MK_LCD		0x54	//����ſڷֻ���ʾ��
/*****************************************************************/
#define CMD_NUMBER_SET		    0x5a	/*�������*/
#define CMD_SYSTEM_SEC_SET		0x5b	//ϵͳ��������
#define CMD_VOICE_TIMES			0x5c	//�������ű�������
/*****************************************************************/
#define CMD_GET_BUS				0x71	/*ռ����������*/

#define CMD_QUEST				0x73	/*��ѯ����*/
#define CMD_QUEST_ANSWER		0x74	/*��ѯӦ������*/
#define CMD_TALK_CLEAR			0X75	/*����ͨ������*/

#define CMD_DISPLAY_DATA		0x80	/*����һ֡Һ��������*/
#define CMD_HANDLE_DISPLAY_DATA 0x82	/*����һ֡�ֱ� Һ��������*/
#define CMD_DATA_ERROR			0x81	/*һ֡���ݳ�������*/
#define CMD_DISPLAY_FORM		0X83	//����һ֡��ͷ�ֻ���ʽ

#define CMD_NURSE_IN			0X83	//��ʿ��λ����
#define CMD_NURSE_OUT			0X84	//��ʿ�뿪����

#define CMD_SUPPLY_OX_START		0X85	//������ʼ
#define CMD_SUPPLY_OX_END		0X86	//��������

#define CMD_RF_NUMSET_OK		0x88	//��Һ����������ɹ�

#define CMD_VOICE_TIMER_SET		0x89	//С����ʱ������
#define CMD_BL_TIMER_SET		0x8a	//�����ʱ������

#define CMD_PC_OPEN				0x8b	//��λ��������
#define CMD_PC_CLOSE			0x8C	//��λ���ر�����
#define CMD_SUPPLY_OX_TOTAL		0x8d	//������ʱ�ܼ�����

#define  CMD_STOP_VOICE_PLAY	0x90	//�������Ž�������
#define  CMD_SYSTEM_STATE		0x91	//����ϵͳ״̬

/*****************************************************************/
#define CMD_ONLINE_WRITE		0xf3	/*����д������*/
#define CMD_ONLINE_READ			0xf4	/*���߶�������*/
#define CMD_ONLINE_OUTPUT		0xf5	/*�����������*/
/*****************************************************************/
#define CMD_LED_OFFLINE_WRITE	0xf0	/*ledeeprom����д������*/
#define CMD_LED_OFFLINE_READ	0xf1	/*ledeeprom���߶�������*/
#define CMD_LED_OFFLINE_OUTPUT	0xf2	/*ledeeprom�����������*/
/*****************************************************************/
#define CMD_EEPROM_OFFLINE_WR	0xfe	/*eeprom����д������*/
#define CMD_EEPROM_OFFLINE_RD	0xf1	/*eeprom���߶�������*/
#define CMD_EEPROM_OFFLINE_OUTT	0xf2	/*eeprom�����������*/ 

#ifndef _IN_SINGLE_BUS_
#define _IN_SINGLE_BUS_
void SingleBusInit(void); 	
void Bus0OutputData(uint8* pbyData); 
void Bus1OutputData(uint8* pbyData);

void Bus0RecDeal(void);
void Bus1RecDeal(void);

void Bus0SendFinishDeal(void); 
void Bus1SendFinishDeal(void);

void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);
	

void TimerOutDeal(void);
//void MakeVoiceData();
//void StartVoicePlay();

extern uint8 AddCallNod(pSTBusFreq pstBusFreq);
extern uint8 byReSend;

extern bit  bBus0RecFinish;				//������0������ɱ�־
extern bit  bBus0SendFinish;				//������0������ɱ�־

extern bit  bBus1RecFinish;				//������0������ɱ�־
extern bit  bBus1SendFinish;				//������0������ɱ�־


extern  bit flag_origin_music;			//���ű�����ֵ�ԭʼ״̬
extern uint8 DTimeState;

#endif
