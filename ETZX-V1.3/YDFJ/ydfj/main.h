#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm8s_gpio.h" 
#include "stm8s_spi.h"

#define F_OSC 16000000L  //16MHZ
#define F_CPU 4000000L //4MHz

#define		INT8U		unsigned char
#define		INT16U		unsigned short
#define		INT32U		unsigned int
#define uint16 INT16U
#define uint8 INT8U
#define uint32 INT32U
//IO定义
//PA1  --  RFCS (OUTPUT) 
//PA2  --  SW3(INPUT IRQ)
//PA3  --  IRCNT   (OUTPUT) 
//PB4  --  LED(OUTPUT)
//PB5  --  INTAUD (INPUT IRQ)
//PC3  --  AUDCS (OUTPUT) 
//PC4  --  AUDRST (OUTPUT) 
//PC5  --  SPICLK
//PC6  --  SPIMOSI
//PC7  --  SPIMISO
//PD1  --  RFTS (INPUT) 
//PD2  --  SW4   (INPUT IRQ)
//PD3  --  IR(OUTPUT)
//PD4  --  RFIRQ (INPUT IRQ)
//PD5  --  BATT (INPUT)模拟 
//PD6  --  SW2(INPUT IRQ)

//PA
#define IO_RFCS (1<<1)
#define IO_SW3 (1<<2)
#define IO_IRCNT (1<<3)
//PB
#define IO_LED (1<<4)
#define IO_INTAUD (1<<5)
//PC
#define IO_AUDCS (1<<3)
#define IO_AUDRST (1<<4)
#define IO_SPICLK (1<<5)
#define IO_SPIMOSI (1<<6)
#define IO_SPIMISO (1<<7)
//PD
#define IO_RFTS (1<<1)
#define IO_SW4 (1<<2)
#define IO_IR (1<<3)
#define IO_RFIRQ (1<<4)
#define IO_BATT (1<<5)
#define IO_SW2 (1<<6)

#define IR_MODE_NORMAL 1
#define IR_MODE_LITTLE 0

extern bool rfirq; //无线中断标志

#define EnableRFIrq() GPIOD->CR2|=IO_RFIRQ
#define DisableRFIrq() GPIOD->CR2&=~(IO_RFIRQ)

#define SetLEDMode(x) {if(x)GPIOA->DDR|=IO_SW3;else GPIOA->DDR&=~IO_SW3;}
#define SetLed(x) {if(x)GPIOB->ODR|=IO_LED;else GPIOB->ODR&=~IO_LED;}
#define SetAUDCS(x) {if(x)GPIOC->ODR|=IO_AUDCS;else GPIOC->ODR&=~IO_AUDCS;}
#define SetRfcs(x) {if(x)GPIOA->ODR|=IO_RFCS;else GPIOA->ODR&=~IO_RFCS;}
#define SetAUDRST(x) {if(x)GPIOC->ODR|=IO_AUDRST;else GPIOC->ODR&=~IO_AUDRST;}
#define SetIRCNT(x) {if(x)GPIOA->DDR&=~IO_IRCNT;else GPIOA->DDR|=IO_IRCNT;}
#define SetIR(x) {if(x)GPIOD->ODR|=IO_IR;else GPIOD->ODR&=~IO_IR;}
#define GetSW2() (GPIOD->IDR&IO_SW2)
#define GetSW3() (GPIOA->IDR&IO_SW3)
#define GetSW4() (GPIOD->IDR&IO_SW4)
#define GetINTAUD() (GPIOB->IDR&IO_INTAUD)

#define SetGDO0(x) {if(x)GPIOD->ODR|=IO_RFIRQ;else GPIOD->ODR&=~IO_RFIRQ;} 
#define SetGDO2(x) {if(x)GPIOD->ODR|=IO_RFTS;else GPIOD->ODR&=~IO_RFTS;} 
#define SetMOSI(x) {if(x)GPIOC->ODR|=IO_SPIMOSI;else GPIOC->ODR&=~IO_SPIMOSI;} 
#define SetMISO(x) {if(x)GPIOC->ODR|=IO_SPIMISO;else GPIOC->ODR&=~IO_SPIMISO;} 
#define SetSCK(x) {if(x)GPIOC->ODR|=IO_SPICLK;else GPIOC->ODR&=~IO_SPICLK;}
#define SetCSN(x) {if(x)GPIOA->ODR|=IO_RFCS;else GPIOA->ODR&=~IO_RFCS;} 

#define GetGDO0() (GPIOD->IDR&IO_RFIRQ) 
#define GetGDO2() (GPIOD->IDR&IO_RFTS)
#define GetMISO() (GPIOC->IDR&IO_SPIMISO) 

extern void Delayms(uint16 ms);
extern void Delayus(uint16 us);
extern unsigned char SPITransferByte(unsigned char byData);
extern unsigned char SPI_SendByte(unsigned char Data);
extern void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2);
extern void WTVxxxInit(unsigned char voml);
extern void PlaySound(uint8 byPos);
extern void CheckPlay(void);
extern void StopPlay(void);
extern void seng_tx(void);
extern unsigned char key_in(void);
extern void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,unsigned char mode);

#endif