/*
************************Copyright(c)************************
*	  			   ����һ�عɷ����޹�˾
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
#define 	BUS0_FREQ_SIZE		7							//������0����֡����
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//������0���ݶ��г���

#define 	DisableBus0RecInt()	{EX0 = 0;PW=1;PW2=1;VL0=0;} 		//������0��ֹ�����жϺ궨��
#define 	EnableBus0RecInt()	{IE0 = 0;EX0 = 1;}			//������0�ָ������жϺ궨��
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23
//����1�궨��
#define 	BUS1_FREQ_SIZE		7							//������1����֡����
#define 	BUS1_FREQ_SIZE_HI	(BUS1_FREQ_SIZE << 4)
#define 	BUS1_TX_Q_ZISE		(BUS1_FREQ_SIZE*12 + 1)		//������1���ݶ��г���
#define 	DisableBus1RecInt()	{EX1 = 0;} 					//������1��ֹ�����жϺ궨��
#define 	EnableBus1RecInt()	{IE1 = 0;EX1 = 1;}			//������1�ָ������жϺ궨��
#define 	BUS1_SEND_CON		0x3c
#define 	BUS1_CAN_SEND 		0x0c 
#define 	BUS1_ON_REC			0x03
#define		BUS1_REQ_SEND		0x28
#define		BUS1_SEND_FINISH	0x0c
#define 	BUS1_ON_WORK		0x23


#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*5 + 1)		//������0���ݽ��ն��г���
#define 	BUS1_RX_Q_ZISE		(BUS1_FREQ_SIZE*5 + 1)		//������0���ݽ��ն��г���

#define 	IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define 	Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE)

#define 	IncBus1RxPtr(addr)    {addr=(addr+1)%BUS1_RX_Q_ZISE;}
#define 	Bus1RxBuffLen()	      ((byBus1RxTail+BUS1_RX_Q_ZISE-byBus1RxHead)%BUS1_RX_Q_ZISE)

//�������Զ�����ʱ��
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000)))

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34

//100us
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
#define CMD_ENTER_PC			0x01	/*�Լ�������(PC��)��ȷ������*/
#define CMD_RESULT_PC			0x02	/*�Լ�������(PC��)�Ľ������*/

#define CMD_BUS_ANSWER			0x02	/*�������ϵ�ͨ��Ӧ������*/
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
#define CMD_LCD_SEND			0x1a	/*�����ݷ�������*/
#define CMD_SCHEDULE_SEND		0x1b	/*һ����������*/
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


#define CMD_GSM_NUM_SEARCH		0x33	//��ѯGSMģ�������õĺ����ֻ�����
#define CMD_GSM_NUM_SET			0X34	//����GSMģ���еĺ����ֻ�����
#define CMD_GSM_NUM_DEL			0X35	//ɾ��GSMģ�������õĺ����ֻ�����
#define CMD_GSM_FUNC_SEARCH		0X36	//GSMģ�鹦��״̬��ѯ
#define CMD_GSM_FUNC_SET		0X37	//GSMģ�鹦������
#define CMD_RS485_BRT_SET		0X38	//RS485���߲���������

#define CMD_OPEN_LCD			0x3e	/*����ʾ����*/
#define CMD_CLOSE_LCD			0x3f	/*�ر���ʾ����*/
/*****************************************************************/
#define CMD_OPEN_485BUS_IN		0x41	/*��485��������*/
#define CMD_CLOSE_485BUS		0x42	/*�ر�485����*/
/*****************************************************************/
#define CMD_CLEAR_HANDLE		0x43	/*����ֱ�Һ����Ϣ����*/
#define CMD_KEY_AVALID			0x44	/*������Ч����*/
#define CMD_KEY_INVALID			0x45	/*����ʧЧ����*/
#define CMD_KEY_DOWN			0x46	/*����ѹ������*/
#define CMD_KEY_UP				0x47	/*�����ͷ�����*/
#define CMD_ERROR_REPORT		0x48	/*���ϱ�������*/
#define CMD_CLEAR_LCD			0x49	/*���Һ����Ϣ����*/
#define CMD_RECEIVE_OK			0x4a	/*�յ���Ϣ����*/
#define CMD_RECEIVE_ERROR		0x4b	/*ѡ����Ϣ����*/

#define CMD_NUMBER_SET		    0x5a	//�������
#define CMD_SELF_SET_NUMBER		0x5b	//�Լ��������
#define CMD_SYSTEM_SEC_SET		0X5B	//ϵͳ��������

#define CMD_GET_BUS				0x71	/*ռ����������*/
#define CMD_QUEST				0x73	/*��ѯ����*/
#define CMD_QUEST_ANSWER		0x74	/*��ѯӦ������*/
#define CMD_TALK_CLEAR			0X75	/*����ͨ������*/
/*****************************************************************/
#define CMD_DISPLAY_DATA		0x80	/*����һ֡Һ��������*/
#define CMD_DATA_ERROR			0x81	/*һ֡���ݳ�������*/

#define CMD_NURSE_IN			0X83	//��ʿ��λ����
#define CMD_NURSE_OUT			0X84	//��ʿ�뿪����

#define CMD_SUPPLY_OX_START		0X85	//������ʼ
#define CMD_SUPPLY_OX_END		0X86	//��������

#define	CMD_MAIN_MENU_SET		0x87	//ϵͳ�˵�����
#define CMD_MAIN_MENU_ESC		0x88	//�˳�ϵͳ�˵�����

#define CMD_VOICE_TIMER_SET		0x89	//С����ʱ������
#define CMD_BL_TIMER_SET		0x8a	//�����ʱ������
#define CMD_VOL_VAL_SET		    0x8b	//������Ϣ����

#define CMD_VOICE_TIMER_GET		0x90	//��ȡС����ʱ��
#define CMD_BL_TIMER_GET		0x91	//��ȡ�����ʱ���
#define CMD_WARD_SEC_INFO_SET	0x92	//������Ϣ����
#define CMD_VOL_VAL_GET			0X93    //��ȡ������Ϣ
 
/*****************************************************************/

#ifdef _IN_SINGLE_BUS_
//�豸״̬����
uint8 	bdata 	byDevState1;
sbit  	bBusy 				= 	byDevState1^7;				//ϵͳæ��־
sbit	bSetKeyDown			=	byDevState1^6;				//���ü�����״̬
sbit	bDealKeyDown		=	byDevState1^5;				//���������״̬
sbit	bHandleDown			=	byDevState1^4;	 			//��ʿ��λ��־
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
sbit	bKeyAnswer			=	byDevState3^4;				//��ȷ�ϼ�����
sbit	bRoomAddr			=	byDevState3^3;				//�������з���ַ������
sbit	bHandAnswer			=	byDevState3^2;				//��Ͳ����
sbit	bPWState			=	byDevState3^1;				//���Ṧ�ſ���
sbit	bPW2State			=	byDevState3^0;			   	//��Ͳ���ſ���
uint8 	bdata 	byDevState4;
sbit	bSetPrio			=	byDevState4^7;				//���û������ȼ�
sbit	bCallDealSending	=	byDevState4^6;				//���д������ڷ���	 
//��ɱ�־����
uint8 	bdata byEnable1;
sbit	bEnSickRoomBroad	= 	byEnable1^0;				//��ɲ����㲥
sbit	bEnOfficeBroad		=	byEnable1^1;				//��ɰ칫���㲥
sbit	bEnAllBroad			=	byEnable1^2;				//���ȫ���㲥
sbit	bEnSoundNote		=	byEnable1^3;				//���������ʾ
sbit	bEnOutRing			=	byEnable1^4;				//����ⲿ����
sbit	bEnAutoListen		=	byEnable1^5;				//����Զ�����
sbit	bEnVoiceNote		=	byEnable1^6;				//���������ʾ
sbit	bEnHandleDeal		=	byEnable1^7;				//����ֱ�����
uint8 	bdata byEnable2;
sbit	bEnInfusionDeal		=	byEnable2^0;				//�����Һ���д���
sbit	bEnServiceDeal		=	byEnable2^1;				//��ɷ�����д���
sbit	bEnHelpDeal			=	byEnable2^2;				//�����Ԯ���д���
sbit	bEnEmergencyDeal	=	byEnable2^3;				//�����Ԯ���д���
sbit	bEnCallDeal			=	byEnable2^4;				//��ɺ��д���
sbit	bEnMusicplay		=	byEnable2^5;				//��ɱ������ֲ���
#else
extern	uint8 	byDevState1;
extern	uint8 	byDevState2;
extern	uint8 	byDevState3;
extern	uint8 	byDevState4;
extern 	uint8 	byEnable1;
extern	uint8 	byEnable2;
extern	bit  	bBusy;
extern 	bit		bSetKeyDown;
extern	bit		bDealKeyDown;
extern	bit		bHandleDown;
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
extern	bit		bKeyAnswer;
extern	bit		bRoomAddr;
extern	bit		bHandAnswer;
extern	bit		bPWState;
extern	bit		bPW2State;
extern  bit		bSetPrio;
extern  bit		bCallDealSending;	
/********************************************/
extern	bit		bEnSickRoomBroad;
extern	bit		bEnOfficeBroad;
extern	bit		bEnAllBroad;
extern	bit		bEnSoundNote;
extern	bit		bEnOutRing;
extern	bit		bEnAutoListen;
extern	bit		bEnVoiceNote;
extern	bit		bEnHandleDeal;
extern	bit		bEnInfusionDeal;
extern	bit		bEnServiceDeal;
extern	bit		bEnHelpDeal;
extern	bit		bEnEmergencyDeal;
extern	bit		bEnCallDeal;
extern	bit		bEnMusicplay;


extern bit  fled0;
extern bit  bKeyEnterDown;
extern bit  bKeyBroadDown;
extern bit  bVl0State;


extern uint8 xdata byBus0RxQ[];						//����0���ն���
extern uint8 xdata byBus0RxHead ;								//������0���ն���ͷָ��
extern uint8 xdata byBus0RxTail ; 								//������0���ն���βָ��


extern uint8 xdata byBus1RxQ[];						//����1���ն���
extern uint8 xdata byBus1RxHead ;								//������1���ն���ͷָ��
extern uint8 xdata byBus1RxTail ; 								//������1���ն���βָ��


void SingleBusInit(void); 	
uint8 Bus0OutputData(uint8* pbyData);  
uint8 Bus1OutputData(uint8* pbyData); 


void VoiceChannelCtx(void);
void InitKeyCallAddr(void);
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
void Bus0RecDeal(void);
void Bus0SendDeal(void);
void Bus0Manage(void);
void Bus1RecDeal(void);
void Bus1SendDeal(void);
void Bus1Manage(void);
void TimerOutDeal(void);
void TimerOutManager(void);
void IdleTask(void);
void SaveParameter(uint16 addr);
void InitParameter(void);


extern void led0flash(void);
extern void led0init(void);

extern uint8 Bus0RecData(uint8* pbyData);
extern uint8 Bus1RecData(uint8* pbyData);
extern void AddPlayVoiceVol(uint8 xdata *pbyData);


#endif
