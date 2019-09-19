#ifndef _MAIN_H_
#define _MAIN_H_


extern INT8U KeyState;	//设置区号按键状态
extern INT8U MySec;
extern INT8U ASCII_BCD[];
extern INT8U 	KeyDownDelay;
extern INT8U	KeyAlwaysDownDelay;	
extern INT8U  SaveSecAddrDelay;
extern INT16U Num;
extern INT8U buf[256];



#define KEY_DOWN_ACT 	0x01	//键按下动作，但还未经过消抖过程
#define KEY_DOWNED 		0x02	//按键经过消抖过程确已按下
#define KEY_ALWAYS_DOWN	0x04	//按键长时间按下(2秒以上)
#define KEY_UP			0x08	//按键弹起

#define KEY_DOWNED_HANDLER	0x10			//键按下程序处理
#define KEY_ALWAYS_DOWN_HANDLER		0x20	//键弹起程序处理

#define FIRST_KEY_HANDLED	0X40			//第一次键按下，只作本区号显示，不进行加1，在之后的1s内有键按下，才进行加1
 										//如果在1秒后无按键，退出第一次按键状态，并进行区号保存
#define KEY_DOWN_DELAY	200		//20MS
#define KEY_ALWAYS_DOWN_DELAY  50 //1S
#define SAVE_SEC_ADDR_DELAY	30;	// 1秒钟


#define GetTestMode() (LPC_GPIO0->DATA&(1<<1))
#define GetKeyValue() (LPC_GPIO0->DATA&(1<<1))	


#define KeyPin	(1<<1)	//p0.1



extern void InsertNurse(INT8U NO);
extern void DeleteNurse(INT8U NO);


#endif

