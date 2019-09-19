#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#ifndef _IN_INTERRUPT_
extern uint16_t ms_unit;							//毫秒单位，30 000 ms = 30秒
extern uint16_t sec_unit;						//秒单位
extern uint8_t flag_readtime;					//读时间标志

extern void UART0_IRQHandler (void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void UART2_IRQHandler (void);
extern void TIMER0_IRQHandler (void);
extern void EINT0_IRQHandler (void);

extern void WDT_IRQHandler(void);


#endif


#endif

