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
#define 	DisableBus0RecInt()	{EX1 = 0;} 			        //������0��ֹ�����жϺ궨��
#define 	EnableBus0RecInt()	{IE1 = 0;EX1 = 1;}			//������0�ָ������жϺ궨��
#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c
#define 	BUS0_ON_WORK		0x23 

#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//������0���ݽ��ն��г���
#define 	MAX_FREQ_NUMBER		10

//��Ӻ��м�¼ʱ���ܷ��ص�ֵ
#define		SAVE_FAIL			0		//�洢ʧ��
#define		SAVE_ONLY			1		//�洢�ɹ�,Ψһһ�����м�¼
#define		SAVE_NORMAL			2		//�洢�ɹ�,������Ӻ��м�¼
#define		SAVE_EXIST			3		//������¼�Ѿ�����
#define		SAVE_PRIO_HI		4		//�洢�ɹ�,�и����ȼ����д���
#define		STATE_DEL			0		//û�в��ҵ��ü�¼
#define		STATE_EXIST			1		//���ҵ��ü�¼ 
#define		STATE_HI			2		//���Ҽ�¼�������и����ȼ����д���	

//�������Զ�����ʱ��
//#define		TIMER0_COUNT		((256-(Fbus*9375/12/1000000)))
//#define		TIMER0_COUNT		((256-(Fbus*15000/12/1000000))) 	//150us

//300us
//#define TIMER0_H  0xfe
//#define TIMER0_L  0x34

//100us
#define TIMER0_H  0xff
#define TIMER0_L  0x67


//100us
#define TIMER0_COUNT 0x67
 
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
#define CMD_BROADCAST1			0x14	/*1��㲥��������*/
#define CMD_BROADCAST2			0x15	/*2��㲥��������*/
#define CMD_CALL_LISTEN			0x16	/*������������*/
#define CMD_PRIORITY_SET		0x17	/*���ȼ�ָ������*/		/*ϵͳ�����һ���ֽڸ�3λΪ���ȼ������շ���ַ*/
#define CMD_SYSTERM_RESET		0x18	/*ϵͳ��λ����*/
/*****************************************************************/
#define CMD_DATE_SEND			0x19	/*���ڷ�������*/		/*ǰ�����ֽڣ��꣺4λBCD�룬���������ֽڣ��º����ڣ�4λBCD��*/

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

#define CMD_RS485_BRT_SET		0x38	//RS485���߲���������

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

#define CMD_SET_NUMBER			0x5a	//�������
#define CMD_SELF_SET_NUMBER		0x5b	//�Լ��������
#define CMD_SYSTEM_SEC_SET		0x5b	//ϵͳ��������

#define CMD_GET_BUS				0x71	/*ռ����������*/
#define CMD_QUEST				0x73	/*��ѯ����*/
#define CMD_QUEST_ANSWER		0x74	/*��ѯӦ������*/
#define CMD_TALK_CLEAR			0X75	/*����ͨ������*/


/*****************************************************************/
#define CMD_DISPLAY_DATA		0x80	/*����һ֡Һ��������*/
#define CMD_DATA_ERROR			0x81	/*һ֡���ݳ�������*/
#define CMD_SB_LCD_DISPLAY_DATA 0X82	//�ֱ���һ֡����

#define CMD_NURSE_IN			0X83	//��ʿ��λ����
#define CMD_NURSE_OUT			0X84	//��ʿ�뿪����

#define CMD_SUPPLY_OX_START		0X85	//������ʼ
#define CMD_SUPPLY_OX_END		0X86	//��������

#define	CMD_MAIN_MENU_SET		0x87	//ϵͳ�˵�����
#define CMD_MAIN_MENU_ESC		0x88	//�˳�ϵͳ�˵�����

#define CMD_VOICE_TIMER_SET		0x89	//С����ʱ������
#define CMD_BL_TIMER_SET		0x8a	//�����ʱ������
#define CMD_MAX_VOL_SET			0x8b	

#define CMD_VOICE_TIMER_GET		0x90	//��ȡС����ʱ��
#define CMD_BL_TIMER_GET		0x91	//��ȡ�����ʱ���
#define CMD_WARD_SEC_INFO_SET	0x92	//������Ϣ����
#define CMD_MAX_VOL_GET			0x93	//�����������

/*****************************************************************/
#define IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE) 

#ifdef _IN_SINGLE_BUS_
//�豸״̬����
uint8 	bdata 	byDevState1;
sbit  	bBusy 				= 	byDevState1^7;				//ϵͳæ��־
sbit	bSetKeyDown			=	byDevState1^6;				//���ü�����״̬
sbit	bDealKeyDown		=	byDevState1^5;				//���������״̬
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
sbit	bPWState			=	byDevState3^0;				//PW���ƽ�״̬
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
sbit	bEnHandDown			=	byEnable2^6;				//����ֱ��ҶϺ���
#else
extern	uint8 	byDevState1;
extern	uint8 	byDevState2;
extern	uint8 	byDevState3;
extern 	uint8 	byEnable1;
extern	uint8 	byEnable2;
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
extern  bit		bPWState;
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
extern	bit		bEnHandDown;


extern bit bSecondDisState;										//������˸��־
extern bit bSignflag;											//�������ű�־
extern bit bBus0RecFinish;
extern bit Bus0SendPin;




extern uint8 xdata byBus0FreqNum;
extern uint8 data byBus0RecData[];


//extern uint8 data byBus0RecData[];					//����0���ջ�����
extern uint8 xdata byBus0RxQ[];									//����0���ն���
extern uint8 xdata byBus0RxHead;								//������0���ն���ͷָ��
extern uint8 xdata byBus0RxTail; 								//������0���ն���βָ��


 
extern void SingleBusInit(void); 	 
extern void ReadParameter(uint16 addr);
extern void SaveParameter(uint16 addr);
extern void InitParameter(void);
extern void VoiceChannelCtx(void);
extern void LedControl(void);
extern void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);	
extern void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount);		
extern void Bus0RecDeal(void);
extern void Bus0SendDeal(void);
extern void Bus0Manage(void);
extern void TimerOutDeal(void);
extern void TimerOutManager(void);
extern uint8 Bus0OutputData(uint8* pbyData); 
extern int8 Bus0RecData(uint8* pbyData);
extern bit AddrCompare(pSTBusFreq pstBusFreq);

#endif
