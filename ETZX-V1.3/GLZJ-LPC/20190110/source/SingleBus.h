
#ifndef _SINGLE_BUS_H
#define _SINGLE_BUS_H

#define RELEASE	 1

//����0�궨��
#define 	BUS0_FREQ_SIZE		7							//������0����֡����
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//������0���ݶ��г���
#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*10 + 1)		//������0���ݶ��г���(����)

#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	(3<<10)
#define 	BUS0_ON_WORK		0x23


//��Ӻ��м�¼ʱ���ܷ��ص�ֵ
#define		SAVE_FAIL			0		//�洢ʧ��
#define		SAVE_ONLY			1		//�洢�ɹ�,Ψһһ�����м�¼
#define		SAVE_NORMAL			2		//�洢�ɹ�,������Ӻ��м�¼
#define		SAVE_EXIST			3		//������¼�Ѿ�����
#define		SAVE_PRIO_HI		4		//�洢�ɹ�,�и����ȼ����д���
#define		STATE_DEL			0		//û�в��ҵ��ü�¼
#define		STATE_EXIST			1		//���ҵ��ü�¼ 
#define		STATE_HI			2		//���Ҽ�¼�������и����ȼ����д���			



extern uint16_t			uiDevState; 						//�豸��ǰ������״̬
//�豸״̬����
#define		FLAG_LANDING				0x8000 				//�ϵ�Ǽ�״̬
#define     FLAG_CHANNEL_CHANGE_EXIST    0x4000				//��ͨ���л������߷��Ͷ�����
#define 	FLAG_WAIT_COMM_ACK			0x2000				//���ڵȴ��������ϵ�ͨ��Ӧ���־
#define		FLAG_STOP_EXIST				0x1000				//��ֹͣ����ָʾ�����߷��Ͷ�����
#define		FLAG_TALK1					0x0800				//ͨ��1ͨ����־
#define		FLAG_NOTE					0x0400				//������ʾ״̬
#define		FLAG_MUSIC					0x0200				//�����ֲ���״̬
#define		FLAG_INDICATION_EXIST		0x0100				//�к���ָʾ�����߷��Ͷ�����
#define		FLAG_BUSY					0x0080				//ϵͳæ��־
#define		FLAG_WAIT_ACK				0x0040				//�ȴ�Ӧ���־
#define		FLAG_WAIT_LISTEN			0x0020				//�ȴ�������־
#define		FLAG_TALK					0x0010				//ͨ��0ͨ����־
#define		FLAG_BROADCAST				0x0008				//�㲥��־
#define		FLAG_INDICATION				0x0004				//�ֻ�����ָʾ��־
#define 	FLAG_WAIT_RS485_ACK			0x0002				//�ȴ�RS485 ����Ӧ��



#define SetDevState(x) (uiDevState|=(x))			//��λ����
#define ClrDevState(x) (uiDevState&=~(x))		//�������
#define GetDevState(x) (uiDevState&(x))			//��ȡλ״̬



//#define 	PC_ADDR1					0X00				//PC����ַ1
#define 	PC_ADDR2					0X00				//PC����ַ2
#define 	PC_ADDR3					0X00				//PC����ַ3

#define 	GLZJ_ADDR2					0xfe				//����������ַ2
#define     GLZJ_ADDR3					0X00				//����������ַ3

#define 	ADD_BED_FJ			250							//�Ӵ��ֻ����ŵ�ַ	//0xFA
#define 	MOVE_FJ				251							//�ƶ��ֻ����ŵ�ַ	//0xFB


#define 	YHFJ_ADDR2				0XFD					//ҽ���ֻ����ŵ�ַ
#define 	ZLDIS_ADDR2				0XF0					//������ʾ�����ŵ�ַ


#define 	RE_SND_TIMES	3



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

#define CMD_VOLUME_SET			0X5C	//������������
#define CMD_SYS_MOD_SET			0X5D	//ϵͳ����ģʽ����(����)
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



//���ͺͽ������Ŷ���

#define Bus0RecPin (1<<1)	//P0.1									//������0�������Ŷ���
#define GetBus0RecPin() (FIO0PIN&Bus0RecPin)


#define Bus0SendPin	(1<<13)  //P2.13									//������0�������Ŷ���
#define SetBus0SendPin() (FIO2SET = Bus0SendPin)
#define ClrBus0SendPin() (FIO2CLR = Bus0SendPin)



#define DisableBus0RecInt()	{IO0IntClr = Bus0RecPin; IO0IntEnF &= ~Bus0RecPin;} 					//������0��ֹ�����жϺ궨��
#define EnableBus0RecInt()	{IO0IntClr = Bus0RecPin; IO0IntEnF |= Bus0RecPin;}			//������0�ָ������жϺ궨��


#define SD_LOW_STATE  	(GetBus0RecPin()==0)
#define SD_HIGH_STATE	(GetBus0RecPin())

#define ACT_SD_LOW	   	(ClrBus0SendPin())
#define ACT_SD_HIGH	   	(SetBus0SendPin())


extern uint16_t State;							//R485��ر�־
#define  bRS485DataTreat 	(1<<0)				//RS485���ݴ����־
#define  bOriginMusic		(1<<1)				//�������ֳ�ʼ״̬
#define  bPCOpen			(1<<2)				//��λ����״̬
#define  bTimerOutDeal		(1<<3)				//TimerOutDeal()���������־
#define  bLedState			(1<<4)				//LED��״̬
#define  bPowerEnState		(1<<5)				//�ź��ߵ�Դ״̬
#define  bHostTalk			(1<<6)				//�����Խ�
#define  bHostTalk1			(1<<7)				//�����Խ�ͨ��1
#define  bReadTime			(1<<8)				//��ȡʱ���־
#define  bUsart2Rec			(1<<9)				//����2���յ�����

#define SetState(x) (State|=(x))			//��λ����
#define ClrState(x) (State&=~(x))		//�������
#define GetState(x) (State&(x))			//��ȡλ״̬




extern uint16_t BusErrorState;					//�����߳���״̬
#define  bStartBitError 	(1<<0)				//��ʼλ����
#define  bEndBitError		(1<<1)				//����λ����
#define  bNightBitError		(1<<2)				//��9λ����


#define SetBusErrorState(x) (BusErrorState|=(x))			//��λ����
#define ClrBusErrorState(x) (BusErrorState&=~(x))		//�������
#define GetBusErrorState(x) (BusErrorState&(x))			//��ȡλ״̬



#ifndef _IN_SINGLE_BUS_
extern uint8_t TimerOutCount;			//20ms��Ԫ��ʱ
extern uint8_t BusLowDTime;

//��������
extern uint8_t 				byChar;
extern uint8_t				byWaitAckTimeLanding;				//�ϵ�Ǽ�ʱ�ȴ�PC����Ӧ��ʱ��
extern uint16_t				uiDevState; 						//�豸��ǰ������״̬
extern uint16_t				uiDetectPcTime;						//����PC�����ʱ��	
extern uint8_t				byVoicePlayTime;					//���Ŷ�ʱ��
extern STBusFreq			stBusFreq;							//���������ݴ���ṹ��	
extern STTimerout			stCH0TimerOut;						//��ʱ����ṹ��
extern STTimerout			stCH1TimerOut;						//ͨ��1ͨ����ʱ����ṹ��	
extern uint8_t				byWaitCommAckDTime;					//ͨ�õȴ���ʱʱ��
extern uint8_t 				byUsart2HandingDTime;				//����2���ڴ���ʱʱ��
extern uint8_t 				byUsart0SdResultDTime;				//����2���ͽ������ʱʱ��
extern STPointer			stCallPointer;						//���ж���˫������
extern STIndicationData		stIndicationData;					//��Ϣָʾ���ݽṹ��
//extern STVoicePlay 			stVoicePlay;						//�������ſ���
extern STAddr				stCallAddr; 						//ͨ��0�������з���ַ
extern STAddr				stCH1CallAddr; 						//ͨ��1�������з���ַ
extern STAddr_S				stRS485Addr;						//RS485���ݽ��շ���ַ
extern uint8_t				byWaitRS485AckDTime;				//ͨ�õȴ���ʱʱ��
extern STEepromCfgData 		stEepromCfgData;					//���������ֽṹ��
extern uint8_t				byReSndBrtTimes;					//�ظ��������ò����ʵĴ���
extern uint8_t				byReSndRs485RoomTimes;				//�ظ����ʹ�ͷ�ֻ�����RS485��Ϣ�Ƿ���Ҫ�жϷ��ŵĴ���
extern uint8_t 				byReSndBedVoiPlaTimes;				//�ظ�������������ʱ��ͷ�ֻ��Ƿ��������ŵĴ���
extern STAddr_S				stCommAddr;							//ͨ�õ�ַ

//---------------------------------


extern uint8_t TimerOutCount;			//20ms��Ԫ��ʱ
extern uint8_t bBus0SndBitOk;



extern STTime	stTime;
extern uint8_t byYearH;		//����ֽ�;

extern void SingleBusInit(void);
extern void TIMER1_IRQHandler (void);			 //100us��ʱ
extern uint8_t Bus0OutputData(uint8_t * pbyData);
extern void  eint3Isr (void);
extern void init_parameter(void);
extern void save_parameter(void);
extern void Bus0Manager(void);
extern void SaveCallAddr(pSTBusFreq pstBusFreq);
extern uint8_t AddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
extern uint8_t DirAddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
extern void MakeCH0TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
extern void MakeCH1TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
extern uint8_t AddCallNod(pSTBusFreq pstBusFreq);
extern void RemoveCallNod(pSTBusFreq pstBusFreq);
extern void SaveIndicationData(pSTBusFreq pstBusFreq, uint8_t byFirstNod);
extern void VoiceChannelCtx(void);
extern void RestartIndication(void);
extern void LedStateFlash(void);
extern void Bus0RecDeal(void);
extern void Bus0SendFinishDeal(void);
extern void TimerOutDeal(void);
extern void reset_parameter(void);

#endif

#endif

