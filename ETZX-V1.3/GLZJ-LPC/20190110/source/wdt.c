#define _IN_WDT_


#include"custom.h"

void init_wdt(void);
void feed_wdt(void);
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_wdt()
{
  LPC_WDT -> WDMOD = 0;
  //feed_wdt();

  LPC_WDT -> WDCLKSEL = 0x1;
  //WDCLKSEL		bit1	bit0
  //				  0		  0		  ѡ���ڲ�RC����(irc_clk)��Ϊ���Ź���ʱ��Դ,Ĭ��
  //				  0		  1		  ѡ��APB����ʱ��(���Ź�PCLK)��Ϊ���Ź���ʱ��Դ
  //				  1		  0		  ѡ��RTC����(rtc_clk)��Ϊ���Ź���ʱ��Դ
  //				  1		  1		  ����,��ʹ�ø�����
  //WDCLKSEL		bit31	  0		  ���⸴λ��������λ�ͱ�Ϊ0,��λ����ͨ����������
  //WDCLKSEL		bit31	  1		  �����ɽ���λ��1,ֻҪWDLOCK��λ,�üĴ����е�λ�Ͳ��ܱ��޸�
  LPC_WDT -> WDTC= 0x112a880;	  //3s		 6000 per 1ms
  LPC_WDT -> WDMOD = 3;
  feed_wdt();
  //NVIC_SetPriority(WDT_IRQn, 1);
  //NVIC_EnableIRQ(WDT_IRQn);         // �����жϲ�ʹ�� 
//  wdt_tst_cnt=0;
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void feed_wdt()
{
  //NVIC_DisableIRQ(TIMER0_IRQn);                  //�����жϲ�ʹ�� 
  LPC_WDT -> WDFEED = 0xaa;
  LPC_WDT -> WDFEED = 0x55;
  //NVIC_EnableIRQ(TIMER0_IRQn);                  //�����жϲ�ʹ�� 
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/



