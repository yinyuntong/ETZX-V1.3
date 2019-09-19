#define _IN_MAIN_

#include <config.h>

uint xdata TimerCount10S;	 //10S��ʱ��������Һ����ʾ������ļ�ʱ


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



void Delay_1ms(uint ms)   //��ʱ1mS����	  
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
*��������			:delay_nms	
*��������        	:���뼶��ʱ����
*�������   		:i:��ʱ��ms��
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
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
*��������			:delay_nus	
*��������        	:���뼶��ʱ����
*�������   		:i:��ʱ��us��
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
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
����ԭ��: void InitiRegister(void)
����˵��: ��
�� �� ֵ: ��
�������ܣ���ʼ��Ӳ��
----------------------------------------------------------------------------*/
void InitiRegister(void)
{

    P4SW=0x70;   
	P3M0=0x30;//P3.4,P3.5����Ϊ�������(���߷��Ͷˣ�Һ�������),P3.3�����߽��նˣ�����Ϊ��������
	P3M1=0x08;
	P4M0=0x42;//P4.1,P4.6����Ϊ�������
	P4M1=0x00;
	P5M0=0x0C;//P5.2,P5.3����Ϊ�������
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

	P3M0 |= 0x80;	//irǿ������� 
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
	if(irflag==1)   ir=~ir;		//�ߵ�ƽʱ38K����
	else ir=0;					//�͵�ƽΪ0
	EA=1;
}



void timer1() interrupt 3 	   //40ms��ʱ
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
	{// 1 ��
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
	  SCON=0X50;	   //������ʽ1
	   
/*	   TMOD&=0X0F;	   //TO��ʽ����
	   TMOD|=0X20;	   //;t1 ��ʽ2�������ʷ�����
	
	
	   TL1 =   0XF6;	   //������4800  ��18.432M����������
	   TH1 =   TL1;
	   PCON    &=  ~SMOD;	//	;�����ʱ���ѡ�� 	   smod=0
	// AUXR    &=  0xBf;	   //��ͳ12��Ƶ�ٶ�
	   AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:4800*12=57.6K
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



void Timer0Init(void)		//T0����38K�ز�
{
	TMOD	&=	0xf0;	   //T1��ʽ����
	TMOD	|=	0x02;	   //T0 ��ʽ2   ����8λ

/*	TL0 	=   236;	  	 //13us��ʱ
	TH0 	=   TL0;
	AUXR 	&= ~T0x12;	   //12Tģʽ*/

	TL0 	=   16;	  	 //13us��ʱ
	TH0 	=   TL0;
	AUXR 	|= T0x12;	   //1Tģʽ

	TF0		=0;
	TR0		=0;
	ET0 	=1;
}


void Timer1Init(void)
{
    TMOD		&=	0x0f;	   //T0��ʽ����
    TMOD		|=	0x10;	   //T1 ��ʽ1    ����16λ


	TH1		=	0x10;//40ms��ʱ
	TL1		=	0x00;

	AUXR 	&= ~T1x12;	   //12Tģʽ
	
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
			{//��������ʱ��ʼ����
				if(TimerCount10S == 0)
				{//����״̬
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

								//��ʼ���ⷢ��
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
						case KEY_ESC_UP:	//�����ſ�ʼ����
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
					TimerCount10S=DISP_ON_TIME;	//���¸�ֵ��ʹ����5��
				}
			}
			uart_send_byte(KeyValue);
			KeyValue = NO_KEY;
		}

   }
}



void ir_sendbyte(uchar ircode)  //���ͺ�������
{

	uchar i;
	 
	set_count=64;		//��ʼλ   //����833us���η���   ���շ��յ�Ϊ0 
	irflag=1;
	count=0;
	TR0=1;
	while(count<set_count);
	TR0=0;
	TL0 = TH0;

	
	for(i=0;i<8;i++)
	{
		if(ircode&0x01)
		{//����λΪ1
			irflag =1;   //���η���   ���շ��յ�Ϊ0 
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
	set_count=64;		//����λ   //����833us�ߵ�ƽ 
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
	if(fKeyUpDown ==0)	//����һһ�����ڰ���״̬
	{

		//����һ��
		Row1 =0;
		Row2 =1;
		Row3 =1;
		if(fKey0 == 0) //"0"������״̬��־
		{
			if(Col1==1)	//��1��
			{
				delay_nms(20);
				if(Col1 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey0 =1;			// "0"���õ���״̬
					KeyValue=KEY0_UP;
				}
			}
		}
		
		if(fKey1 == 0) //"1"������״̬��־
		{
			if(Col2==1)	//��2��
			{
				delay_nms(20);
				if(Col2 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey1 =1;			// "1"���õ���״̬
					KeyValue=KEY1_UP;
				}
			}
		}
		if(fKey2 == 0) //"2"������״̬��־
		{
			if(Col3==1)	//��3��
			{
				delay_nms(20);
				if(Col3 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey2 =1;			// "2"���õ���״̬
					KeyValue=KEY2_UP;
				}
			}
		}

		if(fKey3 == 0) //"3"������״̬��־
		{
			if(Col4==1)	//��4��
			{
				delay_nms(20);
				if(Col4 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey3 =1;			// "3"���õ���״̬
					KeyValue=KEY3_UP;
				}
			}
		}

		//���ڶ���
		Row1 =1;
		Row2 =0;

		if(fKey4 == 0) //"4"������״̬��־
		{
			if(Col1==1)	//��1��
			{
				delay_nms(20);
				if(Col1 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey4 =1;			// "4"���õ���״̬
					KeyValue=KEY4_UP;
				}
			}
		}
		if(fKey5 == 0) //"5"������״̬��־
		{
			if(Col2==1)	//��2��
			{
				delay_nms(20);
				if(Col2 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey5 =1;			// "5"���õ���״̬
					KeyValue=KEY5_UP;
				}
			}
		}

		if(fKey6 == 0) //"6"������״̬��־
		{
			if(Col3==1)	//��3��
			{
				delay_nms(20);
				if(Col3 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
///					LED = 1;		//����
					fKey6 =1;			// "6"���õ���״̬
					KeyValue=KEY6_UP;
				}
			}
		}

		if(fKey7 == 0) //"7"������״̬��־
		{
			if(Col4==1)	//��4��
			{
				delay_nms(20);
				if(Col4 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey7 =1;			// "7"���õ���״̬
					KeyValue=KEY7_UP;
				}
			}
		}
		

		//��������
		Row2 =1;
		Row3 =0;

		if(fKey8 == 0) //"8"������״̬��־
		{
			if(Col1==1)	//��1��
			{
				delay_nms(20);
				if(Col1 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey8 =1;			// "8"���õ���״̬
					KeyValue=KEY8_UP;
				}
			}
		}

		if(fKey9 == 0) //"9"������״̬��־
		{
			if(Col2==1)	//��2��
			{
				delay_nms(20);
				if(Col2 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKey9 =1;			// "9"���õ���״̬
					KeyValue=KEY9_UP;
				}
			}
		}
		
		if(fKeyEnter== 0) //"ȷ��"������״̬��־
		{
			if(Col3==1)	//��3��
			{
				delay_nms(20);
				if(Col3 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKeyEnter =1;			// "ȷ��"���õ���״̬
					KeyValue=KEY_ENTER_UP;
				}
			}
		}


		if(fKeyEsc== 0) //"���"������״̬��־
		{
			if(Col4==1)	//��4��
			{
				delay_nms(20);
				if(Col4 ==1)
				{
					fKeyUpDown = 1;	//����״ָ̬ʾ
//					LED = 1;		//����
					fKeyEsc =1;			// "���"���õ���״̬
					KeyValue=KEY_ESC_UP;
				}
			}
		}
		

		//��������
		Row3 =1;

	}


	if(fKeyUpDown == 1)	//���а������ǵ���״̬	
	{
	
		//����һ��
		Row1 =0;
		Row2 =1;
		Row3 =1;

		if(fKey0 == 1) //"0"������״̬��־
		{
			if(Col1==0)	//��1��
			{
				delay_nms(20);
				if(Col1 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey0 =0;			// "0"���ð���״̬
					KeyValue=KEY0_DOWN;
				}
			}
		}


		if(fKey1 == 1) //"1"������״̬��־
		{
			if(Col2==0)	//��2��
			{
				delay_nms(20);
				if(Col2 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey1 =0;			// "1"���ð���״̬
					KeyValue=KEY1_DOWN;
				}
			}
		}

		if(fKey2 == 1) //"2"������״̬��־
		{
			if(Col3==0)	//��3��
			{
				delay_nms(20);
				if(Col3 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey2 =0;			// "2"���ð���״̬
					KeyValue=KEY2_DOWN;
				}
			}
		}

		if(fKey3 == 1) //"3"������״̬��־
		{
			if(Col4==0)	//��4��
			{
				delay_nms(20);
				if(Col4 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey3 =0;			// "3"���ð���״̬
					KeyValue=KEY3_DOWN;
				}
			}
		}

		//���ڶ���
		Row1 =1;
		Row2 =0;
		if(fKey4 == 1) //"4"������״̬��־
		{
			if(Col1==0)	//��1��
			{
				delay_nms(20);
				if(Col1 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey4 =0;			// "4"���ð���״̬
					KeyValue=KEY4_DOWN;
				}
			}
		}

		if(fKey5 == 1) //"5"������״̬��־
		{
			if(Col2==0)	//��2��
			{
				delay_nms(20);
				if(Col2 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey5 =0;			// "5"���ð���״̬
					KeyValue=KEY5_DOWN;
				}
			}
		}

		if(fKey6 == 1) //"6"������״̬��־
		{
			if(Col3==0)	//��3��
			{
				delay_nms(20);
				if(Col3 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey6 =0;			// "6"���ð���״̬
					KeyValue=KEY6_DOWN;
				}
			}
		}

		if(fKey7 == 1) //"7"������״̬��־
		{
			if(Col4==0)	//��1��
			{
				delay_nms(20);
				if(Col4 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey7 =0;			// "7"���ð���״̬
					KeyValue=KEY7_DOWN;
				}
			}
		}


		//��������
		Row2 =1;
		Row3 =0;

		if(fKey8 == 1) //"8"������״̬��־
		{
			if(Col1==0)	//��1��
			{
				delay_nms(20);
				if(Col1 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey8 =0;			// "8"���ð���״̬
					KeyValue=KEY8_DOWN;
				}
			}
		}

		if(fKey9 == 1) //"9"������״̬��־
		{
			if(Col2==0)	//��2��
			{
				delay_nms(20);
				if(Col2 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKey9 =0;			// "9"���ð���״̬
					KeyValue=KEY9_DOWN;
				}
			}
		}

		if(fKeyEnter == 1) //"ȷ��"������״̬��־
		{
			if(Col3==0)	//��3��
			{
				delay_nms(20);
				if(Col3 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKeyEnter =0;			// "ȷ��"���ð���״̬
					KeyValue=KEY_ENTER_DOWN;
				}
			}
		}

		if(fKeyEsc == 1) //"ȡ��"������״̬��־
		{
			if(Col4==0)	//��4��
			{
				delay_nms(20);
				if(Col4 ==0)
				{
					fKeyUpDown = 0;	//����״ָ̬ʾ
//					LED = 0;		//����
					fKeyEsc =0;			// "ȡ��"���ð���״̬
					KeyValue=KEY_ESC_DOWN;
				}
			}
		}

		Row3 =1;

	}
}


