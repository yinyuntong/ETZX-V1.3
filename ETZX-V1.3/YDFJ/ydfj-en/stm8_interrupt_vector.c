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
}
@far @interrupt void PortDInterrupt (void){
	rfirq=1; //无线中断
	return;
}
extern bool audio_play; //正在放音
extern bool audio_irq; 
@far @interrupt void PortBInterrupt (void){
	audio_irq=1;
	audio_play=0;//声音播放结束
}

//定时器2中断（100US中断一次）
@far @interrupt void TIM2UpdateInterrupt(void)
{
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
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, TIM2UpdateInterrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
