
#ifndef _IN_SINGLE_BUS_
#define _IN_SINGLE_BUS_

#define RELEASE	 1

//����0�궨��
#define 	BUS0_FREQ_SIZE		7							//������0����֡����
#define 	BUS0_FREQ_SIZE_HI	(BUS0_FREQ_SIZE << 4)		
#define 	BUS0_TX_Q_ZISE		(BUS0_FREQ_SIZE*4 + 1)		//������0���ݶ��г���
#define 	BUS0_RX_Q_ZISE		(BUS0_FREQ_SIZE*4 + 1)		//������0���ݶ��г���(����)

#define 	BUS0_SEND_CON		0x3c
#define 	BUS0_CAN_SEND 		0x0c 
#define 	BUS0_ON_REC			0x03
#define		BUS0_REQ_SEND		0x28
#define		BUS0_SEND_FINISH	0x0c0000
#define 	BUS0_ON_WORK		0x23
//�������Զ�����ʱ��
//#define		TIMER0_COUNT		(100us)((256-(Fbus*9375/12/1000000))) 
//---------------------------------------------------------------
//---------------------------------------------------------------
//#define     TIMER0_H  0xFE
//#define     TIMER0_L  0x34
#define     TIMER0_H  0xFF
#define     TIMER0_L  0x67
//---------------------------------------------------------------
 
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
	INT8U bySndSecAddr;			/*�������ͷ�����ַ*/
	INT8U bySndRoomAddr;		/*�������ͷ�����ַ*/
	INT8U bySndBedAddr;			/*�������ͷ�����ַ*/
	INT8U byCmd;				/*����*/
	INT8U byRecSecAddr;			/*���շ�����ַ*/
	INT8U byRecRoomAddr;		/*���շ�����ַ*/
	INT8U byRecBedAddr;			/*���շ�����ַ*/	
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
#define CMD_LCD_SEND			0x1a	/*�����ݷ�������*/
#define CMD_SCHEDULE_SEND		0x1b	/*һ����������*/
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
#define	CMD_OPEN_485BUS_IN_ACK	0x34	//�ӻ���485��������
#define	CMD_OPEN_485BUS_OUT_ACK	0x35	//�ӻ���485�������
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
//--------------------------------------------------------------
#define CMD_DISPLAY_DATA		0x80	/*����һ֡Һ��������*/
#define CMD_DATA_ERROR			0x81	/*һ֡���ݳ�������*/
#define CMD_NURSE_COME		    0x83	/*��ʿ��λ����*/
#define CMD_NURSE_BACK		    0x84	/*��ʿ�뿪����*/
//--------------------------------------------------------------
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
#define CMD_SET_BAUD	        0x38	/*����ϵͳ����������*/ 



//���ͺͽ������Ŷ���

#ifdef RELEASE
#define Bus0RecPin (1<<11)	//P1.11									//������0�������Ŷ���
#define GetBus0RecPin() (LPC_GPIO1->DATA&Bus0RecPin)
#else
#define Bus0RecPin (1<<6)	//P2.6									//������0�������Ŷ���
#define GetBus0RecPin() (LPC_GPIO2->DATA&Bus0RecPin)
#endif
#define Bus0SendPin	(1<<4)  //P1.4									//������0�������Ŷ���
#define SetBus0SendPin() (LPC_GPIO1->DATA|=Bus0SendPin)
#define ClrBus0SendPin() (LPC_GPIO1->DATA&=~Bus0SendPin)

#ifdef RELEASE
#define 	DisableBus0RecInt()	{LPC_GPIO1->IE&=~Bus0RecPin;} 					//������0��ֹ�����жϺ궨��
#define 	EnableBus0RecInt()	{LPC_GPIO1->IC|=Bus0RecPin;LPC_GPIO1->IE|=Bus0RecPin;}			//������0�ָ������жϺ궨��
#else
#define 	DisableBus0RecInt()	{LPC_GPIO2->IE&=~Bus0RecPin;} 					//������0��ֹ�����жϺ궨��
#define 	EnableBus0RecInt()	{LPC_GPIO2->IC|=Bus0RecPin;LPC_GPIO2->IE|=Bus0RecPin;}			//������0�ָ������жϺ궨��
#endif

//����������Ŷ���
#define ProtectPin (1<<8)  //P2.8
#define SetProtectPin() (LPC_GPIO2->DATA|=ProtectPin);
#define ClrProtectPin() (LPC_GPIO2->DATA&=~ProtectPin);



extern void SingleBusInit(void); 	
extern INT8U Bus0OutputData(INT8U* pbyData);  
extern void UartDeal(void);
extern void MakeCH0TimerOut(INT8U byTimerOut, INT8U byTimerOutCount);
extern void Bus0RecDeal(void);
extern void Bus0SendDeal(void);
extern void Bus0Manage(void);
extern void TimerOutDeal(void);
extern void TimerOutManager(void);

#endif

