/*
**********************************Copyright (c) ****************************
*	  						cwg
*			       All Rights Reserved
*			     		QQ:7084394
*
*---------------------------------File Info---------------------------------
*File Name     		:SingleBus.h
*Descriptor    		:������ͷ�ļ�
*Create By     		:������
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
//����0�궨��
#define 	BUS0_FREQ_SIZE		7
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*20 + 1)
#define 	DisableBus0RecInt()	{EX1 = 0;} 
#define 	EnableBus0RecInt()	{IE1 = 0;EX1 = 1;}
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c 
#define 	BUS0_ON_WORK		0x23
//�������Զ�����ʱ��
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000))) 

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34

//100uS
#define TIMER0_H  0xff
#define TIMER0_L  0x67


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
#define CMD_BROADCAST1			0x14	/*1��㲥��������*/
#define CMD_BROADCAST2			0x15	/*2��㲥��������*/
#define CMD_CALL_LISTEN			0x16	/*������������*/
#define CMD_PRIORITY_SET		0x17	/*���ȼ�ָ������*/		/*ϵͳ�����һ���ֽڸ�3λΪ���ȼ������շ���ַ*/
#define CMD_SYSTERM_RESET		0x18	/*ϵͳ��λ����*/
/*****************************************************************/
#define CMD_DATA_SEND			0x19	/*���ڷ�������*/		/*ǰ�����ֽڣ��꣺4λBCD�룬���������ֽڣ��º����ڣ�4λBCD��*/
#define CMD_TIME_SEND			0x1a	/*ʱ�䷢������*/
#define CMD_SCHEDULE_SEND		0x1b	/*һ����������*/
#define CMD_NUMBER_SET			0x1c	/*�޸ı������*/		/*�µĵ�ַ��ǰ2���ֽڣ����޸ĵ�ַ�ں������ֽ�*/
#define CMD_LAMP_CONTROL		0x1d	/*�ƿ�����*/
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
#define CMD_STOP_INDICATION		0x2e	/*��ֹ��Ϣָʾ*/
#define CMD_INFO_INDICATION		0x2f	/*��Ϣָʾ����*/
/*�������з���ַ��ǰ�����ֽڣ����������ֽ�Ϊ�������ͣ���3λ���ȼ�����5λΪ���*/
/*****************************************************************/
#define CMD_MUSIC				0x30	/*��������*/
#define CMD_INCREACE_VOL		0x31	/*������������*/
#define CMD_DECREACE_VOL		0x32	/*������С����*/

#define CMD_VOICE_PLAY_ROOM		0x3a	//���������Ƿ񲥱�����


#define CMD_OPEN_LCD			0x3e	/*����ʾ����*/
#define CMD_CLOSE_LCD			0x3f	/*�ر���ʾ����*/
/*****************************************************************/
#define CMD_OPEN_485BUS_OUT		0x40	/*��485���*/
#define CMD_OPEN_485BUS_IN		0x41	/*��485��������*/
#define CMD_CLOSE_485BUS		0x42	/*�ر�485����*/
/*****************************************************************/
#define CMD_KEY_AVALID			0x44	/*������Ч����*/
#define CMD_KEY_INVALID			0x45	/*����ʧЧ����*/
#define CMD_KEY_DOWN			0x46	/*����ѹ������*/
#define CMD_KEY_UP				0x47	/*�����ͷ�����*/
#define CMD_ERROR_REPORT		0x48	/*���ϱ�������*/
#define CMD_CLEAR_LCD			0x49	/*���Һ����Ϣ����*/
#define CMD_RECEIVE_INFO		0x4a	/*�յ���Ϣ����*/
#define CMD_SELECT_INFO			0x4b	/*ѡ����Ϣ����*/
#define CMD_NO_BUG				0x4c	/*�����������*/
#define CMD_NO_BUG_AVALID		0x4d	/*���������Ч����*/
#define CMD_BUG_END				0x4e	/*������������*/


#define  CMD_STOP_VOICE_PLAY	0x90	//�������Ž�������

/*****************************************************************/  
#ifdef _IN_SINGLE_BUS_
//�豸״̬����
uint8 	bdata 	byDevState1;
sbit	bLanding 	= 	byDevState1^0;				//�ϵ�Ǽ�״̬	
#else
extern	uint8	byDevState1;
extern	bit		bLanding;
void SingleBusInit(void); 
void ReadParameter(void);	
void InitParameter(void);
void Bus0OutputData(uint8* pbyData);  
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);
void Bus0RecDeal(void);
void Bus0SendDeal(void);
void Bus0Manage(void); 
void TimerOutDeal(void);
void TimerOutManager(void);
#endif
