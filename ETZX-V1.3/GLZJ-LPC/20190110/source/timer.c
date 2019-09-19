#define _IN_TIMER_

#include"custom.h"

void init_timer0 (void);
void init_timer1 (void);
void reset_start_timer1(void);
void stop_timer1(void);
void init_timer2 (void);
/*********************************************************************************************************
** Function name:      init_timer0
** Descriptions:      ��ʱ��0��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void init_timer0 (void)
{
    LPC_TIM0->TCR  = 0x02;	//��λ
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;		//��ʱ��ģʽ,ÿ��PCLK������������
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;		//Ԥ��Ƶ�Ĵ���
    //LPC_TIM0->MR0  = SystemFrequency/4/2;       // 0.5S�ж�1��                
	//LPC_TIM0->MR0  = SystemFrequency/200/20;      //1ms�ж�1�� 
	LPC_TIM0->MR0  = SystemFrequency/4/2000;      //0.5ms�ж�1��   	
	//LPC_TIM0->MR0  = SystemFrequency/2500;      //1.6ms                         
    LPC_TIM0->MCR  = 0x03;                        //ƥ�������жϲ�ʹTC��λ            
    
    NVIC_EnableIRQ(TIMER0_IRQn);                  //�����жϲ�ʹ��            
    NVIC_SetPriority(TIMER0_IRQn, 6);
	LPC_TIM0->TCR  = 0x01;                        // ������ʱ��                 
}
/*********************************************************************************************************
** Function name:      init_timer1
** Descriptions:      ��ʱ��1��ʼ������
** input parameters:    ��
** output parameters:   ��			    
** Returned value:      ��
*********************************************************************************************************/
void init_timer1 (void)
{
    LPC_TIM1->TCR  = 0x02;	//��λ	//��ʱ�����ƼĴ���
    LPC_TIM1->IR   = 1;		//��ʱ���жϼĴ���
    LPC_TIM1->CTCR = 0;		//��ʱ��ģʽ,ÿ��PCLK������������
    LPC_TIM1->TC   = 0;		//���涨ʱ���������ļ���ֵ��32λ��TCÿ����PR+1����PCLK���ڵ���һ�Ρ�
    LPC_TIM1->PR   = 0;		//Ԥ��Ƶ�Ĵ���
    //LPC_TIM0->MR0  = SystemFrequency/4/2;        // 0.5S�ж�1��               
//	LPC_TIM1->MR0  = SystemFrequency/4/10000;       //100us   	
	LPC_TIM1->MR0  = SystemFrequency/4/9615;       //104us  
    LPC_TIM1->MCR  = 0x03;                         // ƥ�������жϲ�ʹTC��λ           
    
    NVIC_EnableIRQ(TIMER1_IRQn);                   //�����жϲ�ʹ��            
    NVIC_SetPriority(TIMER1_IRQn, 2);
    LPC_TIM1->TCR  = 0x01;                         // ������ʱ��                 
}


void reset_start_timer1(void)
{
	LPC_TIM1->TCR  = 0x02;  //��λ
	LPC_TIM1->IR         = 0x01; 	   //����ж�
	__NOP();
	LPC_TIM1->TCR  = 0x01;                         // ������ʱ�� 
}

void stop_timer1(void)
{
	 LPC_TIM1->TCR  = 0x02;	//��λ	 
}


/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_timer2 (void)
{
    LPC_SC->PCONP |= (1<<22);// ����TIMER2�Ĺ��ʼĴ�������λ   
    LPC_TIM2->TCR  = 0x02;	 //��λ	bit0	Ϊ1ʱ����ʱ����������Ԥ��Ƶ������ʹ�ܼ���,Ϊ0ʱ,����������ֹ
							 //bit1	Ϊ1ʱ����ʱ����������Ԥ��Ƶ��������PCLK����һ��������ͬ����λ����������TCR[1]�ָ�Ϊ0֮ǰ���ָ�λ״̬
    LPC_TIM2->IR   = 1;
    LPC_TIM2->CTCR = 0;		 //��ʱ��ģʽ,ÿ��PCLK������������
    LPC_TIM2->TC   = 0;		 //tcÿpr+1��pclk���ڵ���1
    LPC_TIM2->PR   = 0;		 //Ԥ��Ƶ�Ĵ���
    LPC_TIM2->MR0  = SystemFrequency/4/8000; //125us�ж�1�� ,��ͨ��MCR�趨Ϊ��ƥ��ʱ��λTC,ֹͣTC,                        
    LPC_TIM2->MCR  = 0x03;     // ƥ�������жϲ�ʹTC��λ,����ƥ�����ʱ�Ƿ�λ�жϻ��Ƿ�λTC         
    
    NVIC_EnableIRQ(TIMER2_IRQn);    //�����жϲ�ʹ��  
    NVIC_SetPriority(TIMER2_IRQn, 5);
	LPC_TIM2->TCR  = 0x01;      // ������ʱ��   
}

