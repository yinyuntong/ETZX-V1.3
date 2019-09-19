#ifndef _GLOBAL_H_
#define _GLOBAL_H_

extern uint8_t KeyState;			//LED�밴����״̬

#define SOFT_RST_KEY_PRESS	(1<<0)
#define PARA_RST_KEY_PRESS	(1<<1)

#define SetKeyState(x) (KeyState |=(x))			//��λ����
#define ClrKeyState(x) (KeyState&=~(x))			//�������
#define GetKeyState(x) (KeyState&(x))				//��ȡλ״̬


#define PARA_RST_KEY_TIME 	5000			//�ָ�Ĭ�ϲ�������ʱ��
#define SOFT_RST_KEY_TIME	20				//��λ����ʱ��


#ifndef _IN_GLOBAL_
extern volatile uint8_t		little_voice_effective;	  //С������Ч
extern uint8_t				blight_off_state;		 	 //�����״̬


extern uint16_t 			para_rst_key_time;
extern uint8_t  			soft_rst_key_time;



extern volatile uint16_t	ring_val;
extern volatile uint8_t		ring_fre_val;
extern volatile uint8_t		ring_tlp_chk;
extern volatile uint8_t		ring_tlp_picked;
extern volatile uint8_t		ring_tlp_release;

extern volatile uint8_t		mt8870_received;

extern volatile uint8_t 	voice_change_flag;


extern void short_soft_delay(uint16_t ch);
extern void soft_delay_half_ms(uint16_t ch);
extern void short_delay(uint16_t i);


extern void init_my_val(void);
extern void init_scon1_rcvbuf(uint8_t ch);
extern void init_scon2_rcvbuf(uint8_t ch);
extern void init_scon3_rcvbuf(uint8_t ch);

extern void close_speak(void);
extern void open_speak(void);
extern void channel_pin_config(void);

#endif



#endif
