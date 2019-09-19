#include "main.h"

INT8U  sm_state=0;					//sm状态

INT8U  IrState=0; 							//红外状态


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



void IrDi_init(void) //初始化PWM输出用作数据发送，FSK 为38KHZ
{
	TIM2->CR1 &= ~1; //关闭TIM2
	TIM2->PSCR = 0;    //不分频
	TIM2->ARRH = (F_OSC/FPWM)>>8; 
	TIM2->ARRL = (F_OSC/FPWM)&0xff;
//	TIM2->CCR2H = (F_OSC/FPWM/2)>>8;
//	TIM2->CCR2L = (F_OSC/FPWM/2)&0xff;  //占空比50%

//	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
//	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;  //占空比1/3


	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;	//占空比1/4


	TIM2->CCMR2 = 0x60;  //PWM模式1


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
		TIM2->CCER1 &= ~(1<<4); //禁止OC4
		TIM2->CR1 &= ~(1<<0);    //禁止TIM2

#if IR_IDLE_HIGH==1		
		SetIR(1);
#else 
		SetIR(0);
#endif

	}
	else 
	{
		TIM2->CCER1 |= 1<<4; //使能OC4
		TIM2->CR1 |= 1;    //使能TIM2
		SetIR(1);
	}
}


void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,bool mode)
{ 
	unsigned char i,j,k;
	
	SetIRCNT(mode)		//按键时输出0:禁止正面红外灯发射；定位自动发送此端口为输入，允许两个红外管外射
	IrDi_init();
	for (k=0;k<size;k++)
	{
		//发送起始位
		IRCmd(ENABLE);
	  IrDi_800us();
		//发送8位数据
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
	  //发送停止位
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
	TIM4->SR1=0x00;//?清除更新标志�
	Ir_timer = TIME_9MS;
	while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);

	x=0;
	TIM4->SR1=0x00;//?清除更新标志
	Ir_timer = TIME_4MS5;	//4.5ms空码
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
		TIM4->SR1=0x00;//?清除更新标志
		TIM4->CNTR = 0x00; // 计数器值
		Ir_timer =1;
		IRCmd(ENABLE);
		while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};;
		IRCmd(DISABLE);

		x=0;
		TIM4->SR1=0x00;//?清除更新标志
		TIM4->CNTR = 0x00; // 计数器值
		
		if(send_data &0x01)
		{
			Ir_timer = TIME_2MS25-1;	//空码
			while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};			
		}
		else
		{
			Ir_timer =1;	//空码
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
	TIM4->SR1=0x00;//?清除更新标志
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
	TIM4->SR1=0x00;//?清除更新标志
	Ir_timer = TIME_9MS;
	IRCmd(ENABLE);
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);

	x=0;
	TIM4->SR1=0x00;//?清除更新标志
	Ir_timer = TIME_2MS25;	//2.25ms空码
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};

	x=0;
	TIM4->SR1=0x00;//?清除更新标志
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


