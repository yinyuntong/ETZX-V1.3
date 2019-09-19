#define _IN_MAIN_

#include <config.h>

uint xdata TimerCount10S;	 //10S计时器，用于液晶显示屏背光的计时


uchar tms;
uchar SecAddr,RoomAddr,BedAddr;

uchar bdata flag0,flag1,flag2;
uchar KeyValue;

uchar count,set_count,sum;
uchar LedFlashTime;

uchar BCD[3];


sbit fled		=	flag0^0;
sbit fKeyUpDown	=	flag0^1;
sbit fKeyCall	=	flag0^2;
sbit fKey1		=	flag0^3;
sbit fKey2		=	flag0^4;
sbit fKey3		=	flag0^5;
sbit fKey4		=	flag0^6;
sbit fKey5		=	flag0^7;


sbit fKey6		=	flag1^0;
sbit fKey7		=	flag1^1;
sbit fKey8		=	flag1^2;
sbit fKey9		=	flag1^3;
sbit fKeyEnter	=	flag1^4;
sbit fKey0		=	flag1^5;
sbit fKeyEsc	=	flag1^6;

sbit fSecAddr	= 	flag2^0;
sbit fRoomAddr	=   flag2^1;
sbit fBedAddr	=	flag2^3;
sbit irflag		=	flag2^4;
sbit Timer10SFinish_Flag = flag2^5;



void  KeyScan(void);
void Delay_1ms(uint ms);
void  Delay_1us(uint us);
void delay_nms(unsigned int i);
void delay_nus(unsigned int i);
void uart_send(unsigned char *SDATA,unsigned char length);
void uart_send_byte(unsigned char SDATA);
void ir_sendbyte(uchar ircode);



void Delay_1ms(uint ms)   //延时1mS函数	  
{
    uint  i;
    while(ms--)
     {
       for(i=0;i<800;i++)
		 _nop_();
     }
}

 void  Delay_1us(uint us)
{
   while(us--)
    {
      _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	  _nop_();
	}
}


/**********************************************************
*函数名称			:delay_nms	
*函数描述        	:毫秒级延时操作
*输入参数   		:i:延时的ms数
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void delay_nms(unsigned int i)
{
 	unsigned int j;

 	for(;i>0;i--)
 	{
 		for(j=900;j>0;j--);
 	}
	 
}


/**********************************************************
*函数名称			:delay_nus	
*函数描述        	:纳秒级延时操作
*输入参数   		:i:延时的us数
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void delay_nus(unsigned int i)
{

 	for(;i>0;i--)
 	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();

 	}
	 
}



void ClearRAM(void)
{

}
 /*---------------------------------------------------------------------------
函数原型: void InitiRegister(void)
参数说明: 无
返 回 值: 无
函数功能：初始化硬件
----------------------------------------------------------------------------*/
void InitiRegister(void)
{

    P4SW=0x70;   
	P3M0=0x30;//P3.4,P3.5设置为推挽输出(总线发送端，液晶背光端),P3.3（总线接收端）设置为高阻输入
	P3M1=0x08;
	P4M0=0x42;//P4.1,P4.6设置为推挽输出
	P4M1=0x00;
	P5M0=0x0C;//P5.2,P5.3设置为推挽输出
	P5M1=0x00;
    WDT_CONTR=0x3A; 

	fKeyUpDown 	=	1;
	fKeyCall	=	1;
	fKey1		=	1;
	fKey2		=	1;
	fKey3		=	1;
	fKey4		=	1;
	fKey5		=	1;
	
	
	fKey6		=	1;
	fKey7		=	1;
	fKey8		=	1;
	fKey9		=	1;
	fKeyEnter	=	1;
	fKey0		=	1;
	fKeyEsc		=	1;

	fSecAddr	=	1;
	fRoomAddr	=	0;
	fBedAddr	=	0;

	SecAddr		=0x00;
	RoomAddr	=0x00;
	BedAddr		=0x00;

	BCD[0]=0;
	BCD[1]=0;
	BCD[2]=0;
	
	count		=0;
	irflag		=0;
	ir 			=0;

	P3M0 |= 0x80;	//ir强推挽输出 
	P3M1 &= 0x7f;

	tms=0;
	TimerCount10S=0;

	KeyValue =NO_KEY;
    EA=1;
}


void intt0() interrupt 0 
{ 

	EX0 =0;

}


void timer0() interrupt 1 
{	
	EA=0;
	count++;
	if(irflag==1)   ir=~ir;		//高电平时38K方波
	else ir=0;					//低电平为0
	EA=1;
}



void timer1() interrupt 3 	   //40ms定时
{  
	EA=0;
	TR1=0;
	TH1=0X10;
	TL1=0X00;
	TF1=0;
	TR1=1;

	WDT_CONTR=0x3A;

	tms++;
	if(	tms >= 25)
	{// 1 秒
		tms =0;
		if(TimerCount10S)
		{
			if(--TimerCount10S ==0)
			{
				CloseLCDBLK();
				clear_screen();
			}
		}

	}
	EA=1;
}


void  Usart0Int() interrupt  4
{
	ES =0;
}
/***********************************************************/

void UsartInit(void)
{
	  SCON=0X50;	   //工作方式1
	   
/*	   TMOD&=0X0F;	   //TO方式不变
	   TMOD|=0X20;	   //;t1 方式2作波特率发生器
	
	
	   TL1 =   0XF6;	   //波特率4800  在18.432M晶振的情况下
	   TH1 =   TL1;
	   PCON    &=  ~SMOD;	//	;波特率倍增选择 	   smod=0
	// AUXR    &=  0xBf;	   //传统12分频速度
	   AUXR |= T1x12;	   //1T模式,最后波特率为:4800*12=57.6K
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=0;
	   ES=0;   
*/

	 BRT = 246;
	 AUXR |= BRTR|BRTx12|S1BRS;
	 RI=0;
	 TI=0;
	 REN=0;
	 ES=0;	


}



void Timer0Init(void)		//T0用作38K载波
{
	TMOD	&=	0xf0;	   //T1方式不变
	TMOD	|=	0x02;	   //T0 方式2   数据8位

/*	TL0 	=   236;	  	 //13us定时
	TH0 	=   TL0;
	AUXR 	&= ~T0x12;	   //12T模式*/

	TL0 	=   16;	  	 //13us定时
	TH0 	=   TL0;
	AUXR 	|= T0x12;	   //1T模式

	TF0		=0;
	TR0		=0;
	ET0 	=1;
}


void Timer1Init(void)
{
    TMOD		&=	0x0f;	   //T0方式不变
    TMOD		|=	0x10;	   //T1 方式1    数据16位


	TH1		=	0x10;//40ms定时
	TL1		=	0x00;

	AUXR 	&= ~T1x12;	   //12T模式
	
	ET1		=1;
	TF1		=0;
	TR1		=1;

}


void uart_send(unsigned char *SDATA,unsigned char length)
{
#if DEBUG==1
	EA=0;
	ES=0;
	TI=0;
	for(;length>0;length--)
	{
		SBUF=*SDATA;
		while(!TI);
		TI=0;
		SDATA++;
	}
	ES=0;
	EA=1;
#endif
}



void uart_send_byte(unsigned char SDATA)
{
#if DEBUG==1
	EA=0;
	ES=0; 
	TI=0;
	SBUF=SDATA;
	while(!TI);
	TI=0; 
	ES=0;
	EA=1;
#endif
}


void main(void)
{
	ClearRAM();
	InitiRegister();

	Timer0Init();
	Timer1Init();

	UsartInit();


	InitLCD();

	
	OpenLCDBLK();
	delay_nms(2000);

	DisplayInputSec();
	DisplayInputNumber();
	TimerCount10S=DISP_ON_TIME;


   while(1)
   {
		KeyScan();
		if(KeyValue != NO_KEY)
		{
			
			if((KeyValue & 0xf0) ==0)
			{//按键弹起时开始处理
				if(TimerCount10S == 0)
				{//黑屏状态
					fSecAddr =1;
					fRoomAddr =0;
					fBedAddr =0;
					SecAddr =0;
					RoomAddr =0;
					BedAddr =0;
					BCD[0] = 0x00;
					BCD[1] = 0x00;
					BCD[2] = 0x00;	

					DisplayInputSec();
					DisplayInputNumber();
					OpenLCDBLK();
					TimerCount10S=DISP_ON_TIME;
					
				}
				else
				{	
					switch(KeyValue)
					{
						case KEY0_UP:
						case KEY1_UP:
						case KEY2_UP:
						case KEY3_UP:
						case KEY4_UP:
						case KEY5_UP:
						case KEY6_UP:
						case KEY7_UP:
						case KEY8_UP:
						case KEY9_UP:
							BCD[0] = BCD[1];
							BCD[1] = BCD[2];
							BCD[2] = KeyValue;
							DisplayInputNumber();
							break;
						case KEY_ENTER_UP:
							if(fSecAddr)
							{
								SecAddr = BCD[0]*100 + BCD[1]*10 + BCD[2];
								fSecAddr =0;
								fRoomAddr =1;
								BCD[0] = 0x00;
								BCD[1] = 0x00;
								BCD[2] = 0x00;
								DisplayInputRoom();
								DisplayInputNumber();
							}
							else if(fRoomAddr)
							{
								RoomAddr = BCD[0]*100 + BCD[1]*10 + BCD[2];
								fRoomAddr =0;
								fBedAddr  =1;
								BCD[0] = 0x00;
								BCD[1] = 0x00;
								BCD[2] = 0x00;	
								DisplayInputBed();
								DisplayInputNumber();


							}
							else if(fBedAddr)
							{	

								BedAddr = BCD[0]*100 + BCD[1]*10 + BCD[2];
								fBedAddr = 0;
								fSecAddr =1;
								BCD[0] = 0x00;
								BCD[1] = 0x00;
								BCD[2] = 0x00;							

								//开始红外发送
								ir_sendbyte(0x05);
								ir_sendbyte(SecAddr);
								ir_sendbyte(RoomAddr);
								ir_sendbyte(BedAddr);
								ir_sendbyte(0x5a);

								sum=0x05;
								sum += SecAddr;
								sum += RoomAddr;
								sum += BedAddr;
								sum += 0x5a;
								ir_sendbyte(sum);

								
								DisplayInputSec();
								DisplayInputNumber();																						
							}
							break;
						case KEY_ESC_UP:	//从区号开始再来
							fSecAddr =1;
							fRoomAddr =0;
							fBedAddr =0;						
							SecAddr =0;
							RoomAddr =0;
							BedAddr =0;
							
							BCD[0] = 0x00;
							BCD[1] = 0x00;
							BCD[2] = 0x00;
							DisplayInputSec();
							DisplayInputNumber();							
							break;
							
						default:
							break;
					}
					TimerCount10S=DISP_ON_TIME;	//重新赋值，使灯亮5秒
				}
			}
			uart_send_byte(KeyValue);
			KeyValue = NO_KEY;
		}

   }
}



void ir_sendbyte(uchar ircode)  //发送红外数据
{

	uchar i;
	 
	set_count=64;		//起始位   //发送833us矩形方波   接收方收到为0 
	irflag=1;
	count=0;
	TR0=1;
	while(count<set_count);
	TR0=0;
	TL0 = TH0;

	
	for(i=0;i<8;i++)
	{
		if(ircode&0x01)
		{//数据位为1
			irflag =1;   //矩形方波   接收方收到为0 
		}
		else irflag =0;
		count=0;
		TR0=1;
		while(count<set_count);
		TR0=0;
		TL0 = TH0;
		ircode=ircode>>1;
	}


	ir=0;
	set_count=64;		//结束位   //发送833us高电平 
	irflag=0;
	count=0;
	TR0=1;
	while(count<set_count);
	TR0=0;
	TL0 = TH0;

	ir=0;
	irflag =0;	
}




void  KeyScan(void)
{
	if(fKeyUpDown ==0)	//有任一一键处于按下状态
	{

		//检测第一排
		Row1 =0;
		Row2 =1;
		Row3 =1;
		if(fKey0 == 0) //"0"键按下状态标志
		{
			if(Col1==1)	//第1列
			{
				delay_nms(20);
				if(Col1 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey0 =1;			// "0"键置弹起状态
					KeyValue=KEY0_UP;
				}
			}
		}
		
		if(fKey1 == 0) //"1"键按下状态标志
		{
			if(Col2==1)	//第2列
			{
				delay_nms(20);
				if(Col2 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey1 =1;			// "1"键置弹起状态
					KeyValue=KEY1_UP;
				}
			}
		}
		if(fKey2 == 0) //"2"键按下状态标志
		{
			if(Col3==1)	//第3列
			{
				delay_nms(20);
				if(Col3 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey2 =1;			// "2"键置弹起状态
					KeyValue=KEY2_UP;
				}
			}
		}

		if(fKey3 == 0) //"3"键按下状态标志
		{
			if(Col4==1)	//第4列
			{
				delay_nms(20);
				if(Col4 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey3 =1;			// "3"键置弹起状态
					KeyValue=KEY3_UP;
				}
			}
		}

		//检测第二排
		Row1 =1;
		Row2 =0;

		if(fKey4 == 0) //"4"键按下状态标志
		{
			if(Col1==1)	//第1列
			{
				delay_nms(20);
				if(Col1 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey4 =1;			// "4"键置弹起状态
					KeyValue=KEY4_UP;
				}
			}
		}
		if(fKey5 == 0) //"5"键按下状态标志
		{
			if(Col2==1)	//第2列
			{
				delay_nms(20);
				if(Col2 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey5 =1;			// "5"键置弹起状态
					KeyValue=KEY5_UP;
				}
			}
		}

		if(fKey6 == 0) //"6"键按下状态标志
		{
			if(Col3==1)	//第3列
			{
				delay_nms(20);
				if(Col3 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
///					LED = 1;		//灯灭
					fKey6 =1;			// "6"键置弹起状态
					KeyValue=KEY6_UP;
				}
			}
		}

		if(fKey7 == 0) //"7"键按下状态标志
		{
			if(Col4==1)	//第4列
			{
				delay_nms(20);
				if(Col4 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey7 =1;			// "7"键置弹起状态
					KeyValue=KEY7_UP;
				}
			}
		}
		

		//检测第三排
		Row2 =1;
		Row3 =0;

		if(fKey8 == 0) //"8"键按下状态标志
		{
			if(Col1==1)	//第1列
			{
				delay_nms(20);
				if(Col1 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey8 =1;			// "8"键置弹起状态
					KeyValue=KEY8_UP;
				}
			}
		}

		if(fKey9 == 0) //"9"键按下状态标志
		{
			if(Col2==1)	//第2列
			{
				delay_nms(20);
				if(Col2 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKey9 =1;			// "9"键置弹起状态
					KeyValue=KEY9_UP;
				}
			}
		}
		
		if(fKeyEnter== 0) //"确认"键按下状态标志
		{
			if(Col3==1)	//第3列
			{
				delay_nms(20);
				if(Col3 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKeyEnter =1;			// "确认"键置弹起状态
					KeyValue=KEY_ENTER_UP;
				}
			}
		}


		if(fKeyEsc== 0) //"清除"键按下状态标志
		{
			if(Col4==1)	//第4列
			{
				delay_nms(20);
				if(Col4 ==1)
				{
					fKeyUpDown = 1;	//按键状态指示
//					LED = 1;		//灯灭
					fKeyEsc =1;			// "清除"键置弹起状态
					KeyValue=KEY_ESC_UP;
				}
			}
		}
		

		//检测第四排
		Row3 =1;

	}


	if(fKeyUpDown == 1)	//所有按键都是弹起状态	
	{
	
		//检测第一排
		Row1 =0;
		Row2 =1;
		Row3 =1;

		if(fKey0 == 1) //"0"键弹起状态标志
		{
			if(Col1==0)	//第1列
			{
				delay_nms(20);
				if(Col1 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey0 =0;			// "0"键置按下状态
					KeyValue=KEY0_DOWN;
				}
			}
		}


		if(fKey1 == 1) //"1"键弹起状态标志
		{
			if(Col2==0)	//第2列
			{
				delay_nms(20);
				if(Col2 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey1 =0;			// "1"键置按下状态
					KeyValue=KEY1_DOWN;
				}
			}
		}

		if(fKey2 == 1) //"2"键弹起状态标志
		{
			if(Col3==0)	//第3列
			{
				delay_nms(20);
				if(Col3 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey2 =0;			// "2"键置按下状态
					KeyValue=KEY2_DOWN;
				}
			}
		}

		if(fKey3 == 1) //"3"键弹起状态标志
		{
			if(Col4==0)	//第4列
			{
				delay_nms(20);
				if(Col4 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey3 =0;			// "3"键置按下状态
					KeyValue=KEY3_DOWN;
				}
			}
		}

		//检测第二排
		Row1 =1;
		Row2 =0;
		if(fKey4 == 1) //"4"键弹起状态标志
		{
			if(Col1==0)	//第1列
			{
				delay_nms(20);
				if(Col1 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey4 =0;			// "4"键置按下状态
					KeyValue=KEY4_DOWN;
				}
			}
		}

		if(fKey5 == 1) //"5"键弹起状态标志
		{
			if(Col2==0)	//第2列
			{
				delay_nms(20);
				if(Col2 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey5 =0;			// "5"键置按下状态
					KeyValue=KEY5_DOWN;
				}
			}
		}

		if(fKey6 == 1) //"6"键弹起状态标志
		{
			if(Col3==0)	//第3列
			{
				delay_nms(20);
				if(Col3 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey6 =0;			// "6"键置按下状态
					KeyValue=KEY6_DOWN;
				}
			}
		}

		if(fKey7 == 1) //"7"键弹起状态标志
		{
			if(Col4==0)	//第1列
			{
				delay_nms(20);
				if(Col4 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey7 =0;			// "7"键置按下状态
					KeyValue=KEY7_DOWN;
				}
			}
		}


		//检测第三排
		Row2 =1;
		Row3 =0;

		if(fKey8 == 1) //"8"键弹起状态标志
		{
			if(Col1==0)	//第1列
			{
				delay_nms(20);
				if(Col1 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey8 =0;			// "8"键置按下状态
					KeyValue=KEY8_DOWN;
				}
			}
		}

		if(fKey9 == 1) //"9"键弹起状态标志
		{
			if(Col2==0)	//第2列
			{
				delay_nms(20);
				if(Col2 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKey9 =0;			// "9"键置按下状态
					KeyValue=KEY9_DOWN;
				}
			}
		}

		if(fKeyEnter == 1) //"确认"键弹起状态标志
		{
			if(Col3==0)	//第3列
			{
				delay_nms(20);
				if(Col3 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKeyEnter =0;			// "确认"键置按下状态
					KeyValue=KEY_ENTER_DOWN;
				}
			}
		}

		if(fKeyEsc == 1) //"取消"键弹起状态标志
		{
			if(Col4==0)	//第4列
			{
				delay_nms(20);
				if(Col4 ==0)
				{
					fKeyUpDown = 0;	//按键状态指示
//					LED = 0;		//灯亮
					fKeyEsc =0;			// "取消"键置按下状态
					KeyValue=KEY_ESC_DOWN;
				}
			}
		}

		Row3 =1;

	}
}


