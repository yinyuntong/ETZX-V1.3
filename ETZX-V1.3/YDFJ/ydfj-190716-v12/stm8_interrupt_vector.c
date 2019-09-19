/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
#include "stm8s_awu.h"
#include "main.h"
typedef void @far (*interrupt_handler_t)(void);


struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}


/*Interrupt routine for the AWU unit*/
@far @interrupt void AWU_IT (void){
  /*Clear the AWU flag*/
  (void) AWU->CSR;
  return;
}

@far @interrupt void PortAInterrupt (void){
	return;
}

@far @interrupt void PortCInterrupt (void){
	return;
}
@far @interrupt void PortDInterrupt (void){
	if(GetGDO0()==0)
	{	
		UART1_SendData8(0X23);
		rfirq=1; //�����ж�
	}
	return;
}
extern bool audio_play; //���ڷ���
extern bool audio_irq; 
@far @interrupt void PortBInterrupt (void){
	audio_irq=1;
	audio_play=0;//�������Ž���
}

//��ʱ��2�жϣ�100US�ж�һ�Σ�
@far @interrupt void TIM2UpdateInterrupt(void)
{
}
//��ʱ��4�жϣ�1MS�ж�һ�Σ�
@far @interrupt void TIM4UpdateInterrupt(void)
{

	TIM4->SR1=0x00;//?������±�־

#if CHECK_560US ==1	
	if(sm_state ==0)
	{
		SetGDO2(1);	//������
		sm_state =1;
	}
	else
	{
		SetGDO2(0);	//������
		sm_state =0;
	}
#endif

	if(Ir_timer)
	{
		Ir_timer--;
	}


	if(Ir_repeat_timer)
	{
		if(--Ir_repeat_timer == 0)
		{
			SetIrState(IR_REPEAT);
		}
	}
}


extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, AWU_IT}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, PortAInterrupt}, /* irq3  */
	{0x82, PortBInterrupt}, /* irq4  */
	{0x82, PortCInterrupt}, /* irq5  */
	{0x82, PortDInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /*�˿�E�ⲿ�ж�  */
	{0x82, NonHandledInterrupt}, /*CAN RX  */
	{0x82, NonHandledInterrupt}, /*CAN TX/ER/SC  */
	{0x82, NonHandledInterrupt}, /*SPI������� */
	{0x82, NonHandledInterrupt}, /*TIM1 ����/����� */
	{0x82, NonHandledInterrupt}, /* TIM1 ����/�Ƚ� */
	{0x82, TIM2UpdateInterrupt}, /* TIM2 ����/�����  */
	{0x82, NonHandledInterrupt}, /*TIM2 ����/�Ƚ�*/
	{0x82, NonHandledInterrupt}, /*TIM3 ����/����� */
	{0x82, NonHandledInterrupt}, /* TIM3����/�Ƚ� */
	{0x82, NonHandledInterrupt}, /* UART1������� */
	{0x82, NonHandledInterrupt}, /* UART1���ռĴ�����*/
	{0x82, NonHandledInterrupt}, /* I2C�ж� */
	{0x82, NonHandledInterrupt}, /* UART2/3������� */
	{0x82, NonHandledInterrupt}, /* UART2/3���ռĴ����� */
	{0x82, NonHandledInterrupt}, /* A/Dת������ */
	{0x82, TIM4UpdateInterrupt}, /* TIM4 ����/�����*/
	{0x82, NonHandledInterrupt}, /* FLASH��̽���*/
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */

};
