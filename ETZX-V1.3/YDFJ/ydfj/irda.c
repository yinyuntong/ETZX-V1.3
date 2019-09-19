#include "main.h"

void IrDi_800us(void)
{
	unsigned char i=240;
	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
}

#define FPWM 38000 //38KHZ
void IrDi_init(void) //��ʼ��PWM����������ݷ��ͣ�FSK Ϊ38KHZ
{
	TIM2->CR1 &= ~1; //�ر�TIM1
	TIM2->PSCR = 0;    //����Ƶ
	TIM2->ARRH = (F_OSC/FPWM)>>8; 
	TIM2->ARRL = (F_OSC/FPWM)&0xff;
	TIM2->CCR2H = (F_OSC/FPWM/2)>>8;
	TIM2->CCR2L = (F_OSC/FPWM/2)&0xff;  //ռ�ձ�50%
	TIM2->CCMR2 = 0x60;  //PWMģʽ1
	//TIM1->CCER2 |= BIT(4); //ʹ��OC4
	SetIR(0);
	//TIM1_CR1 |= BIT(0);    //ʹ��TIM1 
	//TIM2->BKR |= (1<<7);    //��ֹɲ�� 
}
void IRCmd(FunctionalState NewState)
{
	//TIM1_Cmd(NewState);
	if (NewState==DISABLE) 
	{
		TIM2->CCER1 &= ~(1<<4); //��ֹOC4
		TIM2->CR1 &= ~(1<<0);    //��ֹTIM1
		SetIR(0);
	}
	else 
	{
		TIM2->CCER1 |= 1<<4; //ʹ��OC4
		TIM2->CR1 |= 1;    //ʹ��TIM1
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
		//������ʼλ
		IRCmd(ENABLE);
	  IrDi_800us();
		//����8λ����
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
	  //����ֹͣλ
		IRCmd(DISABLE);
	  IrDi_800us();
		IrDi_800us();
	}
	SetIRCNT(1);
}