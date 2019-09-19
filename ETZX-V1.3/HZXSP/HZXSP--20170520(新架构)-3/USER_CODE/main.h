#ifndef _MAIN_H_
#define _MAIN_H_


extern INT8U KeyState;	//�������Ű���״̬
extern INT8U MySec;
extern INT8U ASCII_BCD[];
extern INT8U 	KeyDownDelay;
extern INT8U	KeyAlwaysDownDelay;	
extern INT8U  SaveSecAddrDelay;
extern INT16U Num;
extern INT8U buf[256];



#define KEY_DOWN_ACT 	0x01	//�����¶���������δ������������
#define KEY_DOWNED 		0x02	//����������������ȷ�Ѱ���
#define KEY_ALWAYS_DOWN	0x04	//������ʱ�䰴��(2������)
#define KEY_UP			0x08	//��������

#define KEY_DOWNED_HANDLER	0x10			//�����³�����
#define KEY_ALWAYS_DOWN_HANDLER		0x20	//�����������

#define FIRST_KEY_HANDLED	0X40			//��һ�μ����£�ֻ����������ʾ�������м�1����֮���1s���м����£��Ž��м�1
 										//�����1����ް������˳���һ�ΰ���״̬�����������ű���
#define KEY_DOWN_DELAY	200		//20MS
#define KEY_ALWAYS_DOWN_DELAY  50 //1S
#define SAVE_SEC_ADDR_DELAY	30;	// 1����


#define GetTestMode() (LPC_GPIO0->DATA&(1<<1))
#define GetKeyValue() (LPC_GPIO0->DATA&(1<<1))	


#define KeyPin	(1<<1)	//p0.1



extern void InsertNurse(INT8U NO);
extern void DeleteNurse(INT8U NO);


#endif

