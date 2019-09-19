#ifndef _MAIN_H_
#define _MAIN_H_


#ifndef _IN_MAIN_
extern uint xdata TimerCount10S;	 //10S计时器，用于液晶显示屏背光的计时
extern uchar tms;
extern uchar SecAddr,RoomAddr,BedAddr;
extern uchar bdata flag0,flag1,flag2;
extern uchar KeyValue;
extern uchar count,set_count,sum;
extern uchar LedFlashTime;
extern uchar BCD[3];

extern bit fled;
extern bit fKeyUpDown;
extern bit fKeyCall;
extern bit fKey1;
extern bit fKey2;
extern bit fKey3;
extern bit fKey4;
extern bit fKey5;


extern bit fKey6;
extern bit fKey7;
extern bit fKey8;
extern bit fKey9;
extern bit fKeyEnter;
extern bit fKey0;
extern bit fKeyEsc;

extern bit fSecAddr;
extern bit fRoomAddr;
extern bit fBedAddr;
extern bit irflag;
extern bit Timer10SFinish_Flag;


extern void  KeyScan(void);
extern void Delay_1ms(uint ms);
extern void  Delay_1us(uint us);
extern void delay_nms(unsigned int i);
extern void delay_nus(unsigned int i);
extern void uart_send(unsigned char *SDATA,unsigned char length);
extern void uart_send_byte(unsigned char SDATA);
extern void ir_sendbyte(uchar ircode);

#endif


#endif