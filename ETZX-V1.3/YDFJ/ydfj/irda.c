#include "main.h"

void IrDi_800us(void)
{
	unsigned char i=240;
	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
}

#define FPWM 38000 //38KHZ
void IrDi_init(void) //初始化PWM输出用作数据发送，FSK 为38KHZ
{
	TIM2->CR1 &= ~1; //关闭TIM1
	TIM2->PSCR = 0;    //不分频
	TIM2->ARRH = (F_OSC/FPWM)>>8; 
	TIM2->ARRL = (F_OSC/FPWM)&0xff;
	TIM2->CCR2H = (F_OSC/FPWM/2)>>8;
	TIM2->CCR2L = (F_OSC/FPWM/2)&0xff;  //占空比50%
	TIM2->CCMR2 = 0x60;  //PWM模式1
	//TIM1->CCER2 |= BIT(4); //使能OC4
	SetIR(0);
	//TIM1_CR1 |= BIT(0);    //使能TIM1 
	//TIM2->BKR |= (1<<7);    //禁止刹车 
}
void IRCmd(FunctionalState NewState)
{
	//TIM1_Cmd(NewState);
	if (NewState==DISABLE) 
	{
		TIM2->CCER1 &= ~(1<<4); //禁止OC4
		TIM2->CR1 &= ~(1<<0);    //禁止TIM1
		SetIR(0);
	}
	else 
	{
		TIM2->CCER1 |= 1<<4; //使能OC4
		TIM2->CR1 |= 1;    //使能TIM1
		SetIR(1);
	}
}


void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,bool mode)
{ 
	unsigned char i,j,k;
	
	SetIRCNT(mode)
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