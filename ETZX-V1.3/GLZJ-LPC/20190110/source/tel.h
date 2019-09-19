#ifndef _TEL_H_
#define _TEL_H_

extern uint16_t TelState;								//�绰������ر�־
#define  bTelRing			(1<<1)				//�绰������״̬
#define  bTelRingOut		(1<<2)				//�绰��������ߵ͵�ƽ
#define  bTelTlpEnChk		(1<<3)				//�绰��TLP������
#define  bTelHungDowned		(1<<4)				//�绰���һ�����
#define  bTelPickUped		(1<<5)				//�绰��ժ������
#define  bOriTelTlp			(1<<6)				//ԭtlp״̬
#define  bCID				(1<<7)				//������ʾ״̬
#define  bCID_CMD			(1<<8)				//����CID����
#define  bCIDOccupy			(1<<9)				//CID����ͨռ���ź�
#define  bCIDMarks			(1<<10)				//CID	��־�ź�
#define  bCIDData			(1<<11)				//CID ����:������Ϣ���͡����ȡ���Ϣ�塢У��
#define  bPhoneRingState		(1<<12)				//оƬTRING ����״̬ 

#define SetTelState(x) (TelState|=(x))			//��λ����
#define ClrTelState(x) (TelState&=~(x))			//�������
#define GetTelState(x) (TelState&(x))			//��ȡλ״̬




#define TEL_PICKUP_TIME		20000		//10����ʾ

#ifndef _IN_TEL_
extern uint16_t tel_ring_time;			//�绰����ʱ��
extern uint16_t tel_tlp_dis_chk;			//��ֹ���ʱ��
extern uint16_t tel_tlp_chk_delay;		//�绰�����ʱʱ��
extern uint16_t CID_marks_time ;			//��־�ź���ʱʱ��
extern uint16_t CID_txe_delay;			//����������ʱ��ʱ�䵽���ͽ�ֹ
extern uint8_t  cid_data[];
extern uint8_t		phone_key_buf[];
extern uint16_t    tel_pickup_time;


extern void start_tel_ring(void);
extern void stop_tel_ring(void);
extern void init_tel(void);
extern void init_msm7512(void);
extern send_msm7512_test(void);
extern uint8_t make_msm7512_data(uint8_t room,uint8_t bed);
extern void init_mt8870(void);
extern void init_phone_key_buf(void);
extern void mt8870_received_treat(void);
extern void tel_pickup_treat(void);
extern void tel_hungdown_treat(void);

extern void AM79R70_pincfg(void);
extern void AM79R70_active(void);
extern void AM79R70_standby(void);
extern void AM79R70_ringing(void);


#endif





#endif


