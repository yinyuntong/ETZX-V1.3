/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "main.h"
#include "cc1101.h"
#include "wtvxxx.h"

#define USE_WOR 1

#define F_SAMPLING 1000     //1kHz sampling


unsigned char TxBuf[8],RxBuf[24],RxBufBak[4],num_1,num_2,calling,inc;
unsigned short power_rec[4];  //电池采样序列 
unsigned char sbed,ebed;

bool rfirq =0;
bool audio_irq=0;

extern INT8U play_start,play_end;//播放声音开始与结束

#define SetBell(x) {if(x)GPIOD->ODR|=IO_BEEP;else GPIOD->ODR&=~IO_BEEP;}
#define SetIRTX(x) {if(x)GPIOC->ODR|=IO_IRTX;else GPIOC->ODR&=~IO_IRTX;}

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
void ConfigIOPort(void)
{
	GPIOA->ODR &= ~(IO_SW3); //置低
	GPIOA->ODR &= ~IO_IRCNT; //置高
	GPIOA->DDR |= IO_RFCS|IO_IRCNT; //输出模式
	GPIOA->CR1 |= (IO_RFCS|IO_SW3); //上拉
	GPIOA->CR2 |= (IO_SW3); //允许按键中断
	GPIOB->ODR |= IO_LED; //置高
	GPIOB->DDR |= IO_LED; //输出模式
	GPIOB->CR2 |= (IO_INTAUD); 
	GPIOC->ODR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
	GPIOC->DDR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
	GPIOC->CR1 |= IO_SPICLK|IO_SPIMOSI|IO_SPIMISO|IO_AUDRST|IO_AUDCS;
	GPIOD->ODR |= IO_AUDCS;
	GPIOD->DDR |= IO_AUDCS|IO_RFCS|IO_IR;
	GPIOD->CR1 |= IO_SW4|IO_SW2|IO_IR;
	GPIOD->CR2 |= (IO_SW4|IO_SW2); //允许按键中断
}

#define EEPROM_BASE_ADDRESS 0x4000
#define EEPROM_MAX_ADDRESS (EEPROM_BASE_ADDRESS+640)
#define EEPROM_PROTECT_DISABLE()    do{     FLASH->DUKR = 0xAE;          \
                                            FLASH->DUKR = 0x56;          \
                                    }while(0)                           //  写保护关闭

#define EEPROM_PROTECT_ENABLE()     do{ FLASH->IAPSR &= 0xF7; } while(0) //  写保护开启

INT8U EEPROM_write(INT16U uiAddress, INT8U ucData)
{
    INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress; 
    if (eeprom_addr1 > EEPROM_MAX_ADDRESS)                              // 地址错误
    {
        return 1;                                            // 
    }
    EEPROM_PROTECT_DISABLE();                                           // 关闭写保护
    *((@far INT8U*) eeprom_addr1) = ucData;                             // 
    EEPROM_PROTECT_ENABLE();                                            // 开启写保护
    return 0;
}
INT8U EEPROM_read(INT16U uiAddress)
{
    INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress;             //
    return  *((@far INT8U*) eeprom_addr1);                              // 读取数据 
}



/*----------------------------
Get ADC result
----------------------------*/
INT16U GetADCResult(INT8U ch)
{
	INT16U value,i;
	ADC1_DeInit();
	ADC1->CSR|=ch; //选择通道
  ADC1->CR2|=0x08; // 数据排列,右对齐
	ADC1->CR2&=~0x02; //SCAN DISABLE
	ADC1->CR3&=~0x80;  //DATE BUFFER DISABLE
	ADC1->TDRL=(1<<ch);
	ADC1->CR1&=~0x02;
	ADC1->CR1|=0x01;
	for(i=0;i<200;i++); // 延时一段时间，至少100uS，保证ADC模块的上电完成
  ADC1->CSR&=(~0x80);
	ADC1->CR1|=0x01; // 再次将CR1寄存器的最低位置1,开启AD转换
  while((ADC1->CSR & 0x80)==0); // 等待转换结束
  value=ADC1->DRH;
	value<<=8;
  value|=ADC1->DRL;
	ADC1->CSR&=(~0x80);
	ADC1->CR1&=~0x01;	
	ADC1_DeInit();
  return value;                 //Return ADC result
}
unsigned char pa_sel,mode,voml;
void SaveConfig(void)
{
	EEPROM_write(0,voml);
	EEPROM_write(1,mode);
	EEPROM_write(2,pa_sel);
}
void ReadConfig(void)
{
	voml = EEPROM_read(0);
	mode = EEPROM_read(1);
	pa_sel = EEPROM_read(2);
}

void CheckRecv(void)
{
	unsigned char i,k,leng;
	INT16U cont=0,cont1=0;
	DisableRFIrq();
  leng=24;
  if(halRfReceivePacket(RxBuf,&leng))     //读取接收到的无线数据
	{ 
		if (leng>7)
		{
			halSpiStrobe(CCxxx0_SPWD);//进入掉电状态
  		SetLEDMode(1);
	  	if(RxBuf[7]==0xaa)
	  	{ 
	  		switch(RxBuf[3])                    //判别接收命令
				{  
				case 0x5a:                       // 编号
		     	if(RxBuf[1]==251)
					{
			      cont=0;cont1=0;
						while(1)
						{ 
							cont++;
							if (cont==500)
							{
								SetLEDMode(0);
							}
              if(cont>1000)
							{
								cont=0;
								SetLEDMode(1);
								cont1++;
							}
						  if(cont1>10){break;}
						  if(GetSW4()==0)
						  { 
								EEPROM_write(3,RxBuf[0]);
								EEPROM_write(4,RxBuf[2]);
								EEPROM_write(5,RxBuf[4]);
								EEPROM_write(6,RxBuf[5]);
							  WTVxxxInit(voml);      //语音芯片初始化
							  if((EEPROM_read(3)==RxBuf[0])&&(EEPROM_read(4)==RxBuf[2]))
							  { 
							  	num_1=RxBuf[0]; num_2=RxBuf[2];
									sbed = RxBuf[4];
									ebed = RxBuf[5];
							    PlaySound(19);
							  }
							  else{PlaySound(20);}
							  while(GetSW4()==0);
							  break;
							}
							Delayms(1);
					 	}
					}
		      break;
				case 0x06:                 	//床头呼叫
					if (((RxBuf[2]<sbed)||(RxBuf[2]>ebed))&&RxBuf[1]!=250) break;
				case 0x07:					//输液
					if (((RxBuf[2]<sbed)||(RxBuf[2]>ebed))&&RxBuf[1]!=250) break;
				case 0x08:                 	//卫生间呼叫
					if (RxBuf[1]==251) break;
			  		if(num_1==RxBuf[0])
					{
						for (i=0;i<4;i++)
						{						
							if (RxBufBak[i]!=RxBuf[i]) break;		//相同的数据就不再语音报号
						}
						if (play_end==play_start||i<4)
						{
							calling=1;
							if (play_end==play_start) WTVxxxInit(voml);      //语音芯片初始化
							else StopPlay();
							for (i=0;i<4;i++) RxBufBak[i] = RxBuf[i];
							for(k=0;k<3;k++)
							{
								PlaySpeak(RxBuf[1],RxBuf[2],RxBuf[3]);
								SetLEDMode(0);
							}
						}
					}
          break;
				default: 
					break;
				}
			}
			SetLEDMode(0);
		}
		else
		{
			CC1101_Init();              //cc1101初始化
			#ifdef USE_WOR
			Init_WOR();
			#endif
			inc=0;
		}
		halSpiStrobe(CCxxx0_SIDLE);
 	}//end if halRfReceivePacket(RxBuf,&leng)			 
	#ifndef USE_WOR
	halSpiStrobe(CCxxx0_SRX);
	#else
	StartRxWor();
	#endif
	rfirq=0;
	EnableRFIrq();
}
extern void IRCmd(FunctionalState NewState);
extern void IrDi_init(void);
void main(void)
{
  unsigned char i,j,k,len;
  unsigned long buf;
	unsigned char index;
	unsigned long tmp;
	unsigned char rxbuf[8];
	bool low_bat=0,send_ir;

	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);
	ConfigIOPort();
	
	//disable ADC interrupts
	ADC1_ITConfig(ADC1_IT_EOC,DISABLE);
	
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD,EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_RISE_ONLY);
	SPI_Init(SPI_FIRSTBIT_MSB,SPI_BAUDRATEPRESCALER_8,SPI_MODE_MASTER,SPI_CLOCKPOLARITY_LOW,SPI_CLOCKPHASE_1EDGE,SPI_DATADIRECTION_2LINES_FULLDUPLEX,SPI_NSS_SOFT,0);
	SPI_Cmd(ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, ENABLE);
	AWU_Init(AWU_TIMEBASE_2S);  //1秒唤醒一次

  FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN); //停止时FLASH 掉电
   
  CLK_SlowActiveHaltWakeUpCmd(ENABLE);  //停止时主电源关闭
	
  //CLK_FastHaltWakeUpCmd(ENABLE); //快速唤醒
	enableInterrupts();
	//disableInterrupts();
	power_rec[0] = GetADCResult(5);
	
	SetLEDMode(1);
	CC1101_Init();              //cc1101初始化
#ifdef USE_WOR
	Init_WOR();
#endif
	halSpiStrobe(CCxxx0_SPWD);//进入掉电状态
	
	ReadConfig();
	if(voml>15)              //如果音量没有被设置，默认为最大
	{ 
		voml=15;
		mode=1; //默认为红外
		pa_sel=4;//默认无线查房功率为4
		SaveConfig();

	}
	num_1= EEPROM_read(3);
	num_2= EEPROM_read(4);
	sbed= EEPROM_read(5);
	ebed= EEPROM_read(6);
	if ((sbed==0&&ebed==0)||(sbed==255&&ebed==255)) {sbed=0;ebed=255;}
	WTVxxxInit(voml);      //语音芯片初始化
	
#ifndef USE_WOR
	halSpiStrobe(CCxxx0_SRX);
#else
	StartRxWor();
#endif

	SetLEDMode(0);
	EnableRFIrq();
	
	index=0;
	power_rec[index] = GetADCResult(5);            
	index++;
	power_rec[3]=power_rec[2]=power_rec[1]=power_rec[0];


//	playtest(0,22);
	
	while(1)
	{ 
		if (audio_irq)
		{
			CheckPlay();
			audio_irq=0;
		}
		power_rec[index] = GetADCResult(5);
		index = (index+1)%3;
		power_rec[3] = (power_rec[2]+power_rec[1]+power_rec[0])/3;
		if (power_rec[3]<563) //<3.3v
		{
			low_bat=1;
		}else low_bat=0;
		send_ir=1;
	if (rfirq!=0){CheckRecv();send_ir=0;}//无线数据接收 
		if ((i=key_in())!=0)
		{
			switch(i)
			{
			case 3: 
				if(voml>0)               //音量-
				{ 
					voml-=2;
					if (voml>15) voml=0;
					SaveConfig();
				}
				WTVxxxInit(voml);      //语音芯片初始化
				PlaySound(22);//音量调节成功
				break;
			case 2: //音量+
				if(voml<15)
				{
					voml+=2;
					if (voml>15) voml=15;
					SaveConfig();
				}
				WTVxxxInit(voml);      //语音芯片初始化
				PlaySound(22);
				break;
			case 4:                         //挂断键/查房
				if (calling)
				{
					StopPlay();
					
					seng_tx();
					#ifndef USE_WOR
					halSpiStrobe(CCxxx0_SRX);
					#else
					StartRxWor();
					#endif
				}
				else
				{
					seng_tx();
					#ifndef USE_WOR
					halSpiStrobe(CCxxx0_SRX);
					#else
					StartRxWor();
					#endif
	
					TxBuf[0]=0x05;
					TxBuf[1]=num_2;
					TxBuf[2]=0x02;
					k=0x05+num_2+0x02;
					TxBuf[3]=k;
					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
					Delayms(100);
					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
				}
				rfirq=0;
				break;
			}
		}
		else if (send_ir)
		{
			inc++;
			TxBuf[0]=0x05;
			TxBuf[1]=num_2;
			TxBuf[2]=0x01;
			k=0x05+num_2+0x01;
			TxBuf[3]=k;
			IrDi_send_data(TxBuf,4,IR_MODE_NORMAL);
			if (low_bat)
			{
			if ((inc%2)==0) {SetLed(1);} else {SetLed(0);}
			}else SetLed(1);
		}
		
		#ifndef USE_WOR
		halSpiStrobe(CCxxx0_SRX);
		#else
		StartRxWor();
		#endif
		SetLEDMode(0);
		halt();	
	}
}

unsigned char key_in(void)
{
  unsigned char i=0;
  if(GetSW2()==0)
  {
  	SetLEDMode(1);
		while(GetSW2()==0)
    {
			Delayms(20);
			i++;
		if (i>120) {i--;SetLEDMode(0);}
		}
		if (i==120) return 5;//PA设置
		else return 2;   //音量+
  }	 
  if(GetSW3()==0)
  {
  	while(GetSW3()==0);
		SetLEDMode(1);
		return 3;//音量-
  }
  
  if(GetSW4()==0)
  {//挂断键
  	SetLEDMode(1);
	while(GetSW4()==0); 
	return 4;
  }

  
  return 0;
}

void seng_tx(void)
{  
 	calling=0;
 	TxBuf[0]=num_1;
    if(TxBuf[0]==RxBufBak[0])
	{ 
		TxBuf[4]= RxBufBak[0];
	}
	else
	{
		TxBuf[4]=0;
	}
	TxBuf[1]=251;  //移动分机的房号固定251           
	TxBuf[2]=num_2;
	TxBuf[3]=0x10;
	if (RxBufBak[3]==7) TxBuf[3]=0x11;
	else if(RxBufBak[3]==8){TxBuf[3]=0x12;}
	TxBuf[5]= RxBufBak[1];
	TxBuf[6]= RxBufBak[2];
	TxBuf[7]= 0x55;
	halSpiStrobe(CCxxx0_SIDLE);
	halRfSendPacket(TxBuf,8);//发送挂断命令数据
	Delayms(100);
	halRfSendPacket(TxBuf,8);//发送挂断命令数据
}
 