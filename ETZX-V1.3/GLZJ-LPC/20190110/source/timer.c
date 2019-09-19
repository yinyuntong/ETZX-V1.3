#define _IN_TIMER_

#include"custom.h"

void init_timer0 (void);
void init_timer1 (void);
void reset_start_timer1(void);
void stop_timer1(void);
void init_timer2 (void);
/*********************************************************************************************************
** Function name:      init_timer0
** Descriptions:      定时器0初始化程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void init_timer0 (void)
{
    LPC_TIM0->TCR  = 0x02;	//复位
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;		//定时器模式,每个PCLK的上升沿增加
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;		//预分频寄存器
    //LPC_TIM0->MR0  = SystemFrequency/4/2;       // 0.5S中断1次                
	//LPC_TIM0->MR0  = SystemFrequency/200/20;      //1ms中断1次 
	LPC_TIM0->MR0  = SystemFrequency/4/2000;      //0.5ms中断1次   	
	//LPC_TIM0->MR0  = SystemFrequency/2500;      //1.6ms                         
    LPC_TIM0->MCR  = 0x03;                        //匹配后产生中断并使TC复位            
    
    NVIC_EnableIRQ(TIMER0_IRQn);                  //设置中断并使能            
    NVIC_SetPriority(TIMER0_IRQn, 6);
	LPC_TIM0->TCR  = 0x01;                        // 启动定时器                 
}
/*********************************************************************************************************
** Function name:      init_timer1
** Descriptions:      定时器1初始化程序
** input parameters:    无
** output parameters:   无			    
** Returned value:      无
*********************************************************************************************************/
void init_timer1 (void)
{
    LPC_TIM1->TCR  = 0x02;	//复位	//定时器控制寄存器
    LPC_TIM1->IR   = 1;		//定时器中断寄存器
    LPC_TIM1->CTCR = 0;		//定时器模式,每个PCLK的上升沿增加
    LPC_TIM1->TC   = 0;		//保存定时器计数器的计数值。32位的TC每隔（PR+1）个PCLK周期递增一次。
    LPC_TIM1->PR   = 0;		//预分频寄存器
    //LPC_TIM0->MR0  = SystemFrequency/4/2;        // 0.5S中断1次               
//	LPC_TIM1->MR0  = SystemFrequency/4/10000;       //100us   	
	LPC_TIM1->MR0  = SystemFrequency/4/9615;       //104us  
    LPC_TIM1->MCR  = 0x03;                         // 匹配后产生中断并使TC复位           
    
    NVIC_EnableIRQ(TIMER1_IRQn);                   //设置中断并使能            
    NVIC_SetPriority(TIMER1_IRQn, 2);
    LPC_TIM1->TCR  = 0x01;                         // 启动定时器                 
}


void reset_start_timer1(void)
{
	LPC_TIM1->TCR  = 0x02;  //复位
	LPC_TIM1->IR         = 0x01; 	   //清除中断
	__NOP();
	LPC_TIM1->TCR  = 0x01;                         // 启动定时器 
}

void stop_timer1(void)
{
	 LPC_TIM1->TCR  = 0x02;	//复位	 
}


/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_timer2 (void)
{
    LPC_SC->PCONP |= (1<<22);// 开启TIMER2的功率寄存器控制位   
    LPC_TIM2->TCR  = 0x02;	 //复位	bit0	为1时，定时器计数器和预分频计数器使能计数,为0时,计数器被禁止
							 //bit1	为1时，定时器计数器和预分频计数器在PCLK的下一个上升沿同步复位。计数器在TCR[1]恢复为0之前保持复位状态
    LPC_TIM2->IR   = 1;
    LPC_TIM2->CTCR = 0;		 //定时器模式,每个PCLK的上升沿增加
    LPC_TIM2->TC   = 0;		 //tc每pr+1个pclk周期递增1
    LPC_TIM2->PR   = 0;		 //预分频寄存器
    LPC_TIM2->MR0  = SystemFrequency/4/8000; //125us中断1次 ,可通过MCR设定为在匹配时复位TC,停止TC,                        
    LPC_TIM2->MCR  = 0x03;     // 匹配后产生中断并使TC复位,控制匹配出现时是否复位中断或是否复位TC         
    
    NVIC_EnableIRQ(TIMER2_IRQn);    //设置中断并使能  
    NVIC_SetPriority(TIMER2_IRQn, 5);
	LPC_TIM2->TCR  = 0x01;      // 启动定时器   
}

