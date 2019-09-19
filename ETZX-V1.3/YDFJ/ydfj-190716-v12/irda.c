#include "main.h"

INT8U  sm_state=0;					//sm״̬

INT8U  IrState=0; 							//����״̬


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



void IrDi_init(void) //��ʼ��PWM����������ݷ��ͣ�FSK Ϊ38KHZ
{
	TIM2->CR1 &= ~1; //�ر�TIM2
	TIM2->PSCR = 0;    //����Ƶ
	TIM2->ARRH = (F_OSC/FPWM)>>8; 
	TIM2->ARRL = (F_OSC/FPWM)&0xff;
//	TIM2->CCR2H = (F_OSC/FPWM/2)>>8;
//	TIM2->CCR2L = (F_OSC/FPWM/2)&0xff;  //ռ�ձ�50%

//	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
//	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;  //ռ�ձ�1/3


	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;	//ռ�ձ�1/4


	TIM2->CCMR2 = 0x60;  //PWMģʽ1


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
		TIM2->CCER1 &= ~(1<<4); //��ֹOC4
		TIM2->CR1 &= ~(1<<0);    //��ֹTIM2

#if IR_IDLE_HIGH==1		
		SetIR(1);
#else 
		SetIR(0);
#endif

	}
	else 
	{
		TIM2->CCER1 |= 1<<4; //ʹ��OC4
		TIM2->CR1 |= 1;    //ʹ��TIM2
		SetIR(1);
	}
}


void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,bool mode)
{ 
	unsigned char i,j,k;
	
	SetIRCNT(mode)		//����ʱ���0:��ֹ�������Ʒ��䣻��λ�Զ����ʹ˶˿�Ϊ���룬�����������������
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



void Ir_send_start(void)
{
	uint8_t i;
	uint16_t x=0;
		
#if USE_TIMER4==1

	IRCmd(ENABLE);
	TIM4->SR1=0x00;//?������±�־�
	Ir_timer = TIME_9MS;
	while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);

	x=0;
	TIM4->SR1=0x00;//?������±�־
	Ir_timer = TIME_4MS5;	//4.5ms����
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
		TIM4->SR1=0x00;//?������±�־
		TIM4->CNTR = 0x00; // ������ֵ
		Ir_timer =1;
		IRCmd(ENABLE);
		while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};;
		IRCmd(DISABLE);

		x=0;
		TIM4->SR1=0x00;//?������±�־
		TIM4->CNTR = 0x00; // ������ֵ
		
		if(send_data &0x01)
		{
			Ir_timer = TIME_2MS25-1;	//����
			while(Ir_timer) {Delayus(1);x++;if(x>=65530) break;};			
		}
		else
		{
			Ir_timer =1;	//����
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
	TIM4->SR1=0x00;//?������±�־
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
	TIM4->SR1=0x00;//?������±�־
	Ir_timer = TIME_9MS;
	IRCmd(ENABLE);
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};
	IRCmd(DISABLE);

	x=0;
	TIM4->SR1=0x00;//?������±�־
	Ir_timer = TIME_2MS25;	//2.25ms����
	while(Ir_timer){Delayus(1);x++;if(x>=65530) break;};

	x=0;
	TIM4->SR1=0x00;//?������±�־
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


