#include "main.h"

INT8U  sm_state=0;					//sm×´Ì¬

INT8U  IrState=0; 							//ºìÍâ×´Ì¬


INT8U Ir_timer=0;

INT8U Ir_repeat_timer=0;


void IrDi_800us(void)
{
	unsigned char i=240;
	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
}


void IrDi_600us(void)
{
	unsigned char i=168;
	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
}



#define FPWM 38000 //38KHZ



void IrDi_init(void) //³õÊ¼»¯PWMÊä³öÓÃ×÷Êı¾İ·¢ËÍ£¬FSK Îª38KHZ
{
	TIM2->CR1 &= ~1; //¹Ø±ÕTIM2
	TIM2->PSCR = 0;    //²»·ÖÆµ
	TIM2->ARRH = (F_OSC/FPWM)>>8; 
	TIM2->ARRL = (F_OSC/FPWM)&0xff;
//	TIM2->CCR2H = (F_OSC/FPWM/2)>>8;
//	TIM2->CCR2L = (F_OSC/FPWM/2)&0xff;  //Õ¼¿Õ±È50%

//	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
//	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;  //Õ¼¿Õ±È1/3


	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;	//Õ¼¿Õ±È1/4


	TIM2->CCMR2 = 0x60;  //PWMÄ£Ê½1


#if IR_IDLE_HIGH==1
	SetIR(1);
#else
	SetIR(0);

#endif

}
void IRCmd(FunctionalState NewState)
{

	if (NewState==DISABLE) 
	{
		TIM2->CCER1 &= ~(1<<4); //½ûÖ¹OC4
		TIM2->CR1 &= ~(1<<0);    //½ûÖ¹TIM2

#if IR_IDLE_HIGH==1		
		SetIR(1);
#else 
		SetIR(0);
#endif

	}
	else 
	{
		TIM2->CCER1 |= 1<<4; //Ê¹ÄÜOC4
		TIM2->CR1 |= 1;    //Ê¹ÄÜTIM2
		SetIR(1);
	}
}


void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,bool mode)
{ 
	unsigned char i,j,k;
	
	SetIRCNT(mode)		//°´¼üÊ±Êä³ö0:½ûÖ¹ÕıÃæºìÍâµÆ·¢Éä£»¶¨Î»×Ô¶¯·¢ËÍ´Ë¶Ë¿ÚÎªÊäÈë£¬ÔÊĞíÁ½¸öºìÍâ¹ÜÍâÉä
	IrDi_init();
	for (k=0;k<size;k++)
	{
		//·¢ËÍÆğÊ¼Î»
		IRCmd(ENABLE);
	  IrDi_800us();
		//·¢ËÍ8Î»Êı¾İ
	  for(i=0;i<8;i++)
	  { 
			if((IrDa_data[k]&0x01)==0x01)
			{
				IRCmd(ENABLE);
				IrDi_800us();
			}
			else
			{
				IRCmd(DISABLE);
				IrDi_800us();
			}
			IrDa_data[k]>>=1;
	  }
	  //·¢ËÍÍ£Ö¹Î»
		IRCmd(DISABLE);
	  IrDi_800us();
		IrDi_800us();
	}
	SetIRCNT(1);
}



void Ir_send_start(void)
{
	uint8_t i;
	uint16_t x=0;
		
#if USE_TIMER4==1

	IRCmd(ENABLE);
	TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾Ø
	Ir_timer = TIME_9MS;
	while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);

	x=0;
	TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
	Ir_timer = TIME_4MS5;	//4.5ms¿ÕÂë
	while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};

	
#else



	IRCmd(ENABLE);
	for(i=0;i<15;i++)
	{
		IrDi_600us();
	}
	IRCmd(DISABLE);

	for(i=0;i<8;i++)
	{
		IrDi_600us();
	}
#endif



}




void Ir_send_data(uint8_t send_data)	
{
	uint8_t i,j;
	uint16_t x=0;
	
#if USE_TIMER4 ==1
	for(i=0;i<8;i++)
	{
		x=0;
		TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
		TIM4->CNTR = 0x00; // ¼ÆÊıÆ÷Öµ
		Ir_timer =1;
		IRCmd(ENABLE);
		while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};;
		IRCmd(DISABLE);

		x=0;
		TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
		TIM4->CNTR = 0x00; // ¼ÆÊıÆ÷Öµ
		
		if(send_data &0x01)
		{
			Ir_timer = TIME_2MS25-1;	//¿ÕÂë
			while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};			
		}
		else
		{
			Ir_timer =1;	//¿ÕÂë
			while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};		
		}

		send_data >>=1;
	}
#else
	for(i=0;i<8;i++)
	{
		IRCmd(ENABLE);
		IrDi_600us();
		IRCmd(DISABLE);


		if(send_data &0x01)
		{
			for(j=0;j<3;j++)
			{
				IrDi_600us();
			}
			
		}
		else
		{
			IrDi_600us();		
		}

		send_data >>=1;
	}
#endif
}



void Ir_send_end(void)
{
	uint16_t x=0;
#if USE_TIMER4 ==1
	TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
	Ir_timer =1;
	IRCmd(ENABLE);
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE); 

#else
	IRCmd(ENABLE);
	IrDi_600us();	
	IRCmd(DISABLE); 
#endif
}


void Ir_send_repeat(void)
{
	uint16_t x=0;
#if USE_TIMER4==1
	TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
	Ir_timer = TIME_9MS;
	IRCmd(ENABLE);
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);

	x=0;
	TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
	Ir_timer = TIME_2MS25;	//2.25ms¿ÕÂë
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};

	x=0;
	TIM4->SR1=0x00;//?Çå³ı¸üĞÂ±êÖ¾
	Ir_timer = 1;
	IRCmd(ENABLE);
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);
#else
	uint8_t i;
	IRCmd(ENABLE);
	for(i=0;i<15;i++) IrDi_600us();
	IRCmd(DISABLE);	

	for(i=0;i<4;i++) IrDi_600us();

	IRCmd(ENABLE);
	IrDi_600us();
	IRCmd(DISABLE);		
#endif
	
}


