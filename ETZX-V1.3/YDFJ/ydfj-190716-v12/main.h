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


#define IR_UART 1			//使用串口模式发送红外信号

#define IR_NEC	0			//按NEC协议发送红外信号

#define CHECK_560US 0				//调试模式，可在RF_TS脚上检测560us定时方波

#define IR_IDLE_HIGH 0		//红外空闲时低电平


#define USE_TIMER4 0		//使用定时器T4定时发送红外


#define USE_AWU 0			//使用自动唤醒单元
#define USE_HALT 0			//MCU 使用停止模式


#define USE_WOR 1			//使用无线唤醒

#define USE_UART1 0			//PD5在TX1与AIN5之间切换


#define USER_CODE1	0X87
#define USER_CODE2	0X65

#define OX_KEYVALUE	250 //原码




#define VERSION_HI 	0X01
#define VERSION_LO	0X02

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
#define IO_RFCS (1<<1)		//cc1101片选
#define IO_SW3 (1<<2)		//音量减
#define IO_IRCNT (1<<3)		//控制面板红外管不发射信号，当常态时按面板按键进行供氧计时开始/结束操作时，因功率较大，面板红外管不发射信号
//PB
#define IO_LED (1<<4)
#define IO_INTAUD (1<<5)	//语音报号芯片中断
//PC
#define IO_AUDCS (1<<3)		//语音报号芯片片选
#define IO_AUDRST (1<<4)	//语音报号芯片复位
#define IO_SPICLK (1<<5)
#define IO_SPIMOSI (1<<6)
#define IO_SPIMISO (1<<7)
//PD
#define IO_RFTS (1<<1)
#define IO_SW4 (1<<2)		//面板按键
#define IO_IR (1<<3)		//红外信号发射脚
#define IO_RFIRQ (1<<4)		//cc1101接收信号中断
#define IO_BATT (1<<5)		//电池检测脚
#define IO_SW2 (1<<6)		//音量加

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



#define TIME_9MS	16
#define TIME_4MS5	8
#define TIME_2MS25	4
#define TIME_1MS12	2





extern INT8U			IrState; 							//红外状态
//设备状态定义
#define		IR_BOOT				0x01 				//引导码
#define 	IR_ADDR				0X02				//地址
#define 	IR_DATA				0X04				//数据
#define 	IR_REPEAT			0X08				//重复




#define SetIrState(x) (IrState|=(x))			//置位操作
#define ClrIrState(x) (IrState&=~(x))		//清零操作
#define GetIrState(x) (IrState&(x))			//获取位状态


extern INT8U  sm_state;					//sm状态
extern INT8U  Ir_timer;
extern INT8U  Ir_repeat_timer;
extern char   CC1101SigValue;



typedef struct
{
	INT8U byInitFlag;
	INT8U bySec;	//区号
	INT8U byNum;	//分机号
	INT8U bySBed;	//起始床号
	INT8U byEBed;	//结束床号
	INT8U byVol;	//音量值	
	INT8U byCrc;	// 校验和
}STEepromCfgData,*pSTEepromCfgData;



#define IAP0_ADDR		0x02

#define IAP1_ADDR 		0X22

#define INIT_INDEX 0X00
#define SEC_INDEX	0X01
#define NUM_INDEX 	0X02
#define SBED_INDEX	0X03
#define EBED_INDEX	0X04
#define VOL_INDEX	0X05
#define CRC_INDEX	0X06



extern STEepromCfgData stEepromCfgData;


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

extern void Ir_send_start(void);
extern void Ir_send_data(uint8_t send_data);
extern void Ir_send_repeat(void);
extern void Ir_send_end(void);

extern void IrDi_600us(void);

extern void seng_tx_answer(INT8U cmd_type);


#endif
