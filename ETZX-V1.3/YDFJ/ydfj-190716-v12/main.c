/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "main.h"
#include "cc1101.h"

#define F_SAMPLING 1000     //1kHz sampling


unsigned char TxBuf[8],RxBuf[24],RxBufBak[4],calling;
INT16U ir_inc=0;
INT16U check_bat_cnt=0;
unsigned short power_rec[4];  //电池采样序列 


bool rfirq =0;
bool audio_irq=0;

INT16U RFStatusCnt= 0;
INT8U RFClearState = 0,RFStatus=0;


char CC1101SigValue=0;


STEepromCfgData stEepromCfgData;


bool low_bat=0;
bool para_error=0;



extern INT8U play_start,play_end;//播放声音开始与结束


void seng_tx_para(void);


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
	GPIOA->ODR &= ~(IO_SW3); //置低	默认为输入口，当改为输出口时，蓝灯亮
	GPIOA->ODR &= ~IO_IRCNT; //置低　当改变方向时，直接关闭一个红外管
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

#if CHECK_560US==1
	GPIOD->ODR |= IO_RFTS;		//输出为高电平,否则为输入口
	GPIOD->DDR |= IO_RFTS;
#endif
}

#define EEPROM_BASE_ADDRESS 0x4000
#define EEPROM_MAX_ADDRESS (EEPROM_BASE_ADDRESS+640)
#define EEPROM_PROTECT_DISABLE()    do{     FLASH->DUKR = 0xAE;          \
                                            FLASH->DUKR = 0x56;          \
                                    }while(0)                           //  写保护关闭

#define EEPROM_PROTECT_ENABLE()     do{ FLASH->IAPSR &= 0xF7; } while(0) //  写保护开启





//地址0:音量
//地址1:按面板按键发送信号模式选择0:无线433；1:红外(现统一为红外,此数据忽略)
//地址2:面板按键无线433功率，现此数据不用忽略。
//地址3: 区号
//地址4:移动分机号
//地址5:移动分机管理的床头分机起始号，默认为1
//地址6:移动分机管理的床头分机结束号，默认为255


INT8U EEPROM_write(INT16U uiAddress, INT8U ucData)
{
    INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress; 
    if (eeprom_addr1 > EEPROM_MAX_ADDRESS)                              // 地址错误
    {
        return 1;                                            // 
    }

#if USE_UART1==0		//没有使用串口调试，正常工作况态下

	if(low_bat )
	{//电压过低
		return 1;
	}
#endif

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



uint8 ParaCrcCheck(uint16 addr)
{
	uint8 crcResult=0;
	uint8 crcData =0;
	uint8 i;
	uint8 paraData;

	

	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		paraData = EEPROM_read(addr);
		crcResult +=paraData; 
		addr++;
	}
	crcData = EEPROM_read(addr);

	
	if(crcData == crcResult) return 1;
	else return 0;
}



//计算stLocalControl.stEepromCfgData中的数据校验和
uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stEepromCfgData.byInitFlag);


	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}

	return crcResult;
}


void ReadParameter(uint16 addr)
{

	uint8 i=0;
	uint8 *pbyData = &(stEepromCfgData.byInitFlag);
	

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		*pbyData = EEPROM_read(addr);
		pbyData++;
		addr++;
	}	
}


bool SaveParameter(uint16 addr)
{  
	uint8 i;
	uint16 addr_temp=addr;
	uint8 *pbyData;
	uint8 readtimes;
	uint8 writetimes;
	uint8 crc_temp;

	writetimes =3;
	do
	{
		addr = addr_temp;
		
		pbyData=&(stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			EEPROM_write(addr,*pbyData);
			pbyData++;
			addr++;
		}


		readtimes=3;
		do
		{
			pbyData=&(stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<sizeof(STEepromCfgData);i++)
			{
				if(*pbyData != EEPROM_read(addr)) break;
				
				pbyData++;
				addr++;
			}	

			if(i>= sizeof(STEepromCfgData))
			{//写进去的数据与读出来的数据相同

				//break;
				return 1;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			//break;	//相同
			return 1;
		}
	}while(--writetimes);
	
	return 0;
}



void InitParameter(void)
{
	uint8 flagData;
	uint8 readtimes=3;

	uint8 result=0;
CheckIapSec0:	

	do
	{

		if(ParaCrcCheck(IAP0_ADDR) ==1)	 break;
			

	}while(--readtimes);


	if(readtimes)
	{//第一块校验和正确
		if(EEPROM_read(IAP0_ADDR+INIT_INDEX)== 0X5A)
		{
			ReadParameter(IAP0_ADDR);
			//return;
			result=1;
		}
		else result =0;

	}
	else result =0;


	readtimes=3;
CheckIapSec1:	
	do
	{

		if(ParaCrcCheck(IAP1_ADDR) ==1)  break;
			

	}while(--readtimes);


	if(readtimes)
	{//第二块校验和正确
		if(EEPROM_read(IAP1_ADDR+INIT_INDEX)== 0X5A)
		{	
			if(result==0)
			{//0区数据不正确的情况下
				ReadParameter(IAP1_ADDR);
			}
			//return;
			result |= 0x02;
		}
		else result &= (~0x02);

	}
	else result &= (~0x02);
	

//	UART1_SendData8(result);
	
	switch(result)
	{
		case 0:
/*			stEepromCfgData.byInitFlag = 0x5a;
			stEepromCfgData.bySec = 254;
			stEepromCfgData.byNum = 0x02;
			stEepromCfgData.bySBed =0x00;
			stEepromCfgData.byEBed =255;
			stEepromCfgData.byVol =8;
			stEepromCfgData.byCrc = CalcParaCrc();
*/
			para_error =1;
			stEepromCfgData.byVol =8;
			WTVxxxInit(15);      //语音芯片初始化
			PlaySound(20);		 //失败			
			break;
			
		case 1://0区数据正确，1区数据不正确
			SaveParameter(IAP1_ADDR);
			break;
		
		case 2://1区数据正确，0区数据不正确	
			SaveParameter(IAP0_ADDR);
			break;

		case 3:	//两区数据都正确
			break;

		default:
			break;
			
					
	}

}


void CheckRecv(void)
{
	unsigned char i,k,leng;
	INT16U cont=0,cont1=0;
	DisableRFIrq();
  	leng=24;
  	if(halRfReceivePacket(RxBuf,&leng))     //读取接收到的无线数据
	{ 
		UART1_SendData8(leng);
		for(i=0;i<leng;i++)
		{
			UART1_SendData8(RxBuf[i]);
		}	

		//一条特殊的测试数据

		if(leng ==5 )
		{
			if(stEepromCfgData.bySec ==254)
			{
				
				if((RxBuf[0]==0xff) && (RxBuf[1]==0xff) && (RxBuf[2]==0x01) &&  (RxBuf[3]==0x32) && (RxBuf[4]==0x00))
				{
	  				SetLEDMode(1);			//输出模式，蓝灯亮


					TxBuf[0]=0xff;
					TxBuf[1]=0xff;  		 
					TxBuf[2]=0x01;
					TxBuf[3]=0x32;
					TxBuf[4]= CC1101SigValue;

					halSpiStrobe(CCxxx0_SIDLE);
					halRfSendPacket(TxBuf,5);//发送回复命令数据
					Delayms(100);
					halRfSendPacket(TxBuf,5);//发送回复命令数据

					WTVxxxInit(stEepromCfgData.byVol);	   //语音芯片初始化
					PlaySound(0x0e);

					SetLEDMode(0);			//输入模式，蓝灯灭					
				}
			}
		}
				
		else if (leng>7)
		{
			halSpiStrobe(CCxxx0_SPWD);	//进入掉电状态
		
  			SetLEDMode(1);			//输出模式，蓝灯亮

			i=0;
	  		if(RxBuf[7]==0xaa)
	  		{ 
	  			switch(RxBuf[3])                    //判别接收命令
				{  
					case 0x5a:                       // 编号
		     		if(RxBuf[1]==251)
					{//加床分机
						SetLEDMode(0);			//sw3输入模式
						Delayms(1);
						if(GetSW2()==0 || GetSW3()==0)
						{
							//只有底下键按下才进入编号状态
							SetLEDMode(1);			//输出模式，蓝灯亮
					
				      		cont=0;cont1=0;
							while(1)
							{ 
								cont++;
								if (cont==500)		//0.5秒
								{
									SetLEDMode(0);	//输入模式，蓝灯灭
								}
	              				if(cont>1000)
								{
									cont=0;
									SetLEDMode(1);	//输出模式，蓝灯亮	
									cont1++;
								}
							  	if(cont1>10){break;}	//5秒时间到，退出编号状态
							  	if(GetSW4()==0)
							  	{ //面板按键按下
									stEepromCfgData.byInitFlag =0x5a;
									stEepromCfgData.bySec = RxBuf[0];	//区号
									stEepromCfgData.byNum = RxBuf[2];	//移动分机号
									stEepromCfgData.bySBed = RxBuf[4];	//起始床号
									stEepromCfgData.byEBed = RxBuf[5];	//结束床号
									stEepromCfgData.byCrc = CalcParaCrc();

									i=SaveParameter(IAP0_ADDR);
									i=SaveParameter(IAP1_ADDR);
									WTVxxxInit(stEepromCfgData.byVol);      //语音芯片初始化

									if(i)
									{
				    					PlaySound(19);//成功 

										seng_tx_answer(0x5a);
										halSpiStrobe(CCxxx0_SIDLE);
										halSpiStrobe(CCxxx0_SFTX);										
									}

								  	else
									{//失败
										PlaySound(20);
									}
									i=0;
								  	while(GetSW4()==0) {Delayms(20); i++; if(i>=200) break;};	//等待按键松开
								  	break;
								}
								Delayms(1);
						 	}
						}
					}
		      		break;
				case 0x5d:
					seng_tx_para();
					break;
					
				case 0x06:                 	//床头呼叫
					if (((RxBuf[2]<stEepromCfgData.bySBed)||(RxBuf[2]>stEepromCfgData.byEBed))&&RxBuf[1]!=250) break;		//加床分机没有界限全部处理
				case 0x07:					//输液
					if (((RxBuf[2]<stEepromCfgData.bySBed)||(RxBuf[2]>stEepromCfgData.byEBed))&&RxBuf[1]!=250) break;
				case 0x08:                 	//卫生间呼叫
					if (RxBuf[1]==251) break;	//移动分机不处理
			  		if(stEepromCfgData.bySec==RxBuf[0])
					{//是本区信号
						for (i=0;i<4;i++)
						{						
							if (RxBufBak[i]!=RxBuf[i]) break;
						}
						if (play_end==play_start||i<4)
						{//接收的信号与之前信号不同　或　没有在语音报号
							calling=1;
							if (play_end==play_start) WTVxxxInit(stEepromCfgData.byVol);      //语音芯片初始化
							else StopPlay();
							for (i=0;i<4;i++) RxBufBak[i] = RxBuf[i];		//将数据备份
							for(k=0;k<3;k++)
							{//报号3遍
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
			SetLEDMode(0);//输入模式，蓝灯灭
		}

 	}//end if halRfReceivePacket(RxBuf,&leng)	


/*	halSpiStrobe(CCxxx0_SIDLE);
	halSpiStrobe(CCxxx0_SFRX);
	halSpiStrobe(CCxxx0_SFTX);*/ 	
	#if USE_WOR==1
	StartRxWor();
	#else
	halSpiStrobe(CCxxx0_SRX);
	#endif
	rfirq=0;
	EnableRFIrq();
}
extern void IRCmd(FunctionalState NewState);
extern void IrDi_init(void);


void timer4_init(void)
{
	TIM4_DeInit();		//恢复相关寄存器默认值
// 1ms定时，用的4M　HSI频率,1MS定时
//	TIM4_TimeBaseInit(TIM4_PRESCALER_32, 0x7e);//初始化预分频器及自动装载值

// 1ms定时，用的16M　HSI频率,1MS定时
//	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0x7e);//初始化预分频器及自动装载值

// 560us定时，用的16M　HSI频率
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 69);//初始化预分频器及自动装载值


// 600us定时，用的16M　HSI频率
//	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 74);//初始化预分频器及自动装载值


	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	//中断配置:允许
	TIM4_UpdateDisableConfig(DISABLE);		//允许更新事件
	TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);	//溢出产生更新中断
	TIM4->EGR=0x01;//允许产生更新事件

	TIM4_Cmd(ENABLE);		//启用定时器4
//	TIM4->CR1 = 0x81; // 预装载T4，开计数器


}





void main(void)
{
	INT8U *pbyData;
  	unsigned char i=0,k=0;
	unsigned char index=0;
	bool send_ir;

	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);	//Fhsi = Fhsi rc/1=16M
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);	//Fcpu = Fhsi/4 =4M

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);

	#if USE_UART1==0	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
	#else
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
	#endif

	//定时器T2用于PWM,定时器T4用于普通定时

	ConfigIOPort();

	#if USE_TIMER4 == 1
	timer4_init();
	#endif



	#if USE_UART1==0
	//disable ADC interrupts
	ADC1_ITConfig(ADC1_IT_EOC,DISABLE);
	#endif

	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD,EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_RISE_ONLY);
	SPI_Init(SPI_FIRSTBIT_MSB,SPI_BAUDRATEPRESCALER_8,SPI_MODE_MASTER,SPI_CLOCKPOLARITY_LOW,SPI_CLOCKPHASE_1EDGE,SPI_DATADIRECTION_2LINES_FULLDUPLEX,SPI_NSS_SOFT,0);
	SPI_Cmd(ENABLE);


	#if USE_AWU==1	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, ENABLE);
	AWU_Init(AWU_TIMEBASE_2S);  //2秒唤醒一次

  	FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN); //停止时FLASH 掉电
  	CLK_SlowActiveHaltWakeUpCmd(ENABLE);  //停止时主电源关闭
  //CLK_FastHaltWakeUpCmd(ENABLE); //快速唤醒
	#endif


#if USE_UART1==1
	UART1_Init((u32)57600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);
	UART1_Cmd(ENABLE);
	Delayms(2);
	UART1_SendData8(0x55);
#endif


	IrDi_init();



	enableInterrupts();
	//disableInterrupts();


	#if USE_UART1==0
	power_rec[0] = GetADCResult(5);
	#endif



	SetLEDMode(1);				//输出模式，蓝灯亮
	CC1101_Init();              //cc1101初始化
	#if USE_WOR==1
	Init_WOR();					//可唤醒
	#endif
	halSpiStrobe(CCxxx0_SPWD);	//进入掉电状态

	InitParameter();
	

/*	if ((stEepromCfgData.bySBed==0&&stEepromCfgData.byEBed==0)||
		(stEepromCfgData.bySBed==255&&stEepromCfgData.byEBed==255)||
		(stEepromCfgData.bySBed>stEepromCfgData.byEBed)) 
		{stEepromCfgData.bySBed=0;stEepromCfgData.byEBed=255;}
*/	
//	WTVxxxInit(stEepromCfgData.byVol);      		//语音芯片初始化

	#if USE_WOR==1
	StartRxWor();
	#else
	halSpiStrobe(CCxxx0_SRX);
	#endif

	SetLEDMode(0);				//输入模式，蓝灯亮
	EnableRFIrq();

	#if USE_UART1==0
	index=0;
	power_rec[index] = GetADCResult(5);            
	index++;
	power_rec[3]=power_rec[2]=power_rec[1]=power_rec[0];
	#endif


	seng_tx_para();


	while(1)
	{ 

		if(GetIrState(IR_REPEAT))
		{
			ClrIrState(IR_REPEAT);
			Ir_send_repeat();
		}	

	
		if (audio_irq)
		{
			CheckPlay();
			audio_irq=0;
		}


		#if USE_UART1==0
		if(++check_bat_cnt == 2000)
		{
			power_rec[index] = GetADCResult(5);
			index = (index+1)%3;
			power_rec[3] = (power_rec[2]+power_rec[1]+power_rec[0])/3;		// 3次电压的平均值


/*
			TxBuf[0]=power_rec[3]>>8;  //高位
			TxBuf[1]=power_rec[3];  	//低位		 
			halSpiStrobe(CCxxx0_SIDLE);
			halRfSendPacket(TxBuf,2);//发送电量数据
//			halSpiStrobe(CCxxx0_SIDLE);
			halSpiStrobe(CCxxx0_SFTX);					
#if USE_WOR==1
			StartRxWor();					
#else
			halSpiStrobe(CCxxx0_SRX);
#endif
*/

//3.6v 0x24e   3.5v 0x23e   3.4v 0x22e  3.3v   0x222    3.2v 0c214       3.1v 0x20a         3.0v 0x1ff
			
//			if (power_rec[3]<563) //<3.3v  	
			if (power_rec[3]<0x24e) //<3.6v	
			{
				low_bat=1;
			}
			else 
			{
				low_bat=0;
			}
			check_bat_cnt =0;
		}
		#endif

		
		if (rfirq!=0)
		{//无线数据接收 
			CheckRecv();
		}
		
		if ((i=key_in())!=0)
		{
			switch(i)
			{
			case 3: 
				if(stEepromCfgData.byVol>2)               //音量-
				{ 
					stEepromCfgData.byVol-=2;
					if (stEepromCfgData.byVol>15) stEepromCfgData.byVol=0;
					stEepromCfgData.byCrc = CalcParaCrc();
					//SaveParameter(IAP0_ADDR);
					EEPROM_write(IAP0_ADDR+VOL_INDEX,stEepromCfgData.byVol);
					EEPROM_write(IAP1_ADDR+VOL_INDEX,stEepromCfgData.byVol);
					EEPROM_write(IAP0_ADDR+CRC_INDEX,stEepromCfgData.byCrc);
					EEPROM_write(IAP1_ADDR+CRC_INDEX,stEepromCfgData.byCrc);					
				}

				WTVxxxInit(stEepromCfgData.byVol);	   //语音芯片初始化
				PlaySound(22);//音量调节成功
				break;
			case 2: //音量+
				if(stEepromCfgData.byVol<15)
				{
					stEepromCfgData.byVol+=2;
					if (stEepromCfgData.byVol>15) stEepromCfgData.byVol=15;
					stEepromCfgData.byCrc = CalcParaCrc();					
					//SaveParameter(IAP0_ADDR);
					EEPROM_write(IAP0_ADDR+VOL_INDEX,stEepromCfgData.byVol);
					EEPROM_write(IAP1_ADDR+VOL_INDEX,stEepromCfgData.byVol);
					EEPROM_write(IAP0_ADDR+CRC_INDEX,stEepromCfgData.byCrc);
					EEPROM_write(IAP1_ADDR+CRC_INDEX,stEepromCfgData.byCrc);					
			
				}

				WTVxxxInit(stEepromCfgData.byVol);	   //语音芯片初始化
				PlaySound(22);
				break;
			case 4: 						//挂断键/查房
				if (calling)
				{
					StopPlay();
					seng_tx();
					halSpiStrobe(CCxxx0_SIDLE);
					halSpiStrobe(CCxxx0_SFTX);						
					#if USE_WOR==1
					StartRxWor();					
					#else
					halSpiStrobe(CCxxx0_SRX);
					#endif
				}
				else
				{
					seng_tx();
					halSpiStrobe(CCxxx0_SIDLE);
					halSpiStrobe(CCxxx0_SFTX);						
					#if USE_WOR==1
					StartRxWor();					
					#else
					halSpiStrobe(CCxxx0_SRX);
					#endif
	
					TxBuf[0]=0x05;
					TxBuf[1]=stEepromCfgData.byNum;
					TxBuf[2]=0x02;
					k=0x05+stEepromCfgData.byNum+0x02;
					TxBuf[3]=k;


					#if IR_UART ==1					
					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
					Delayms(100);
					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
					#endif



					#if IR_NEC ==1				
//					Ir_repeat_timer = 197;	//110ms

					SetIRCNT(0);		//禁止按键旁红外发射管发射,因为功率大以免旁边分机接收到
					Ir_send_start();

					Ir_send_data(USER_CODE2);		//用户代码高字节
					Ir_send_data(USER_CODE1);		//用户代码低字节
					Ir_send_data(~OX_KEYVALUE); 	//键值
					Ir_send_data(OX_KEYVALUE);		//键值
					Ir_send_end();

					SetIRCNT(1);
					#endif

				}
				rfirq=0;
				break;
			}
		}



		ir_inc++;
		
		#if USE_UART1==0
		if (low_bat)
		{//电量低红灯闪烁
			if ((ir_inc%1000)==50)	//灭的时间长，主要也是为了省电 
			{
				SetLed(1);		//灭
			} 
			else if ((ir_inc%1000)==999) 
			{
				SetLed(0);		//亮
			}
		}
		else SetLed(1);	//电量正常，红灯灭
		#endif
			

		if(ir_inc %2000 == 0)	//	2S1次
		{
//			UART1_SendData8(0x13);

			TxBuf[0]=0x05;
			TxBuf[1]=stEepromCfgData.byNum;
			TxBuf[2]=0x01;
			k=0x05+stEepromCfgData.byNum+0x01;
			TxBuf[3]=k;
			
			#if IR_UART ==1				
			IrDi_send_data(TxBuf,4,IR_MODE_NORMAL);
			#endif	



			#if IR_NEC ==1
			SetIRCNT(1);		//用作输入,允许按键旁红外发射管发射

			Ir_send_start();
			
			Ir_send_data(USER_CODE2);		//用户代码高字节
			Ir_send_data(USER_CODE1);		//用户代码低字节
			Ir_send_data(~stEepromCfgData.byNum);			//键值
			Ir_send_data(stEepromCfgData.byNum);			//键值
			Ir_send_end();
			#endif

		}



		if(ir_inc %4000 == 0)	//	4S   1次
		{
		
			ir_inc =0;			//轮到最大数据时将数据清0

//			UART1_SendData8(0x14);			

			//必须加上这一段，否则CC1101模块会被无线电波唤醒不进入休眠状态
			#if USE_WOR==1
			if (rfirq==0)
			{
				StartRxWor();
			}
			#else		
			halSpiStrobe(CCxxx0_SRX);
			#endif			
		}


		if(RFStatusCnt)
		{
			if(--RFStatusCnt ==0)
			{
				UART1_SendData8(0x11);
				halSpiStrobe(CCxxx0_SIDLE);
				halSpiStrobe(CCxxx0_SFRX);
				halSpiStrobe(CCxxx0_SFTX);

				
				#if USE_WOR==1
				StartRxWor();
				#else		
				halSpiStrobe(CCxxx0_SRX);
				#endif			
			}
		}
		

		SetLEDMode(0);



		if (GetGDO0())
		{
			if(RFStatus==0) 
			{
				RFStatus = 1;  
				RFStatusCnt = 6000; 	//6秒				
			}
		}
		else 
		{ 
			RFStatus = 0;
			RFStatusCnt = 0;
		} 

		Delayms(1);


	}
}

unsigned char key_in(void)
{
  	unsigned char i=0;
	unsigned int x=0;
	
  	if(GetSW2()==0)
  	{
  		SetLEDMode(1);	//面板按键输出模式，用于灯指示
  		while(GetSW2()==0) {Delayms(20); x++; if(x>=120) break;}

		if(x>=120) return 5;	//未用
		else return 2;//音量+
  	}


  	if(GetSW3()==0)
  	{//sw3与蓝LED指示灯重用
  		x=0;
		SetLEDMode(0);
  		while(GetSW3()==0) {Delayms(20); x++; if(x>=120) break;}
		SetLEDMode(1);
		
		if(x>=120) return 6;	//未用
		else return 3;//音量-
  	}
	
  	if(GetSW4()==0)
  	{
  		x=0;
  		SetLEDMode(1);
		while(GetSW4()==0){Delayms(20); x++; if(x>=120) break;} 

		if(x>=120) return 7;	//未用
		else return 4;

	}//挂断键
  	return 0;
}

void seng_tx(void)
{  
	INT8U i;
 	calling=0;
	
 	TxBuf[0]=stEepromCfgData.bySec;
    if(TxBuf[0]==RxBufBak[0])
	{ //本机区号与接收床头分机区号相等
		TxBuf[4]= RxBufBak[0];
	}
	else
	{
		TxBuf[4]=0;		//否则发送区号为0
	}
	TxBuf[1]=251;  //移动分机的房号固定251           
	TxBuf[2]=stEepromCfgData.byNum;
	TxBuf[3]=0x10;
	if (RxBufBak[3]==7) TxBuf[3]=0x11;
	else if(RxBufBak[3]==8){TxBuf[3]=0x12;}
	TxBuf[5]= RxBufBak[1];
	TxBuf[6]= RxBufBak[2];
	TxBuf[7]= 0x55;


	for(i=0;i<8;i++)
	{
		UART1_SendData8(TxBuf[i]);
	}
	
	halSpiStrobe(CCxxx0_SIDLE);
	halRfSendPacket(TxBuf,8);//发送挂断命令数据
	Delayms(100);
	halRfSendPacket(TxBuf,8);//发送挂断命令数据
}


void seng_tx_answer(INT8U cmd_type)
{  

 	TxBuf[0]=stEepromCfgData.bySec;	//区号
 	
	TxBuf[1]=251;  //移动分机的房号固定251           
	TxBuf[2]=stEepromCfgData.byNum;	//分机号
	TxBuf[3]=0x02;

	TxBuf[4]=cmd_type;	//原命令
	
	TxBuf[5]= 0x01;		//成功
	TxBuf[6]= 0x00;
	TxBuf[7]= 0x55;
	halSpiStrobe(CCxxx0_SIDLE);
	halRfSendPacket(TxBuf,8);//发送应答命令数据
	Delayms(100);
	halRfSendPacket(TxBuf,8);//发送应答命令数据
}


void seng_tx_para(void)
{  

	uint8 *pbyData;
	uint8 i;
	
	if(para_error ==0)
	{

#if USE_UART1==1
		UART1_SendData8(VERSION_HI);
		UART1_SendData8(VERSION_LO);
		
		pbyData = &(stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			UART1_SendData8(*pbyData);
			pbyData++;
		}
#else
		Delayms(10);
#endif

	
		RxBuf[0]= VERSION_HI;
		RxBuf[1]= VERSION_LO;

		RxBuf[2]= stEepromCfgData.byInitFlag;
	 	RxBuf[3]=stEepromCfgData.bySec;	//区号      
		RxBuf[4]=stEepromCfgData.byNum;	//分机号
		RxBuf[5]= stEepromCfgData.bySBed;	
		RxBuf[6]= stEepromCfgData.byEBed;		
		RxBuf[7]= stEepromCfgData.byVol;
		RxBuf[8]= stEepromCfgData.byCrc;
		RxBuf[9]= 0x55;
		halSpiStrobe(CCxxx0_SIDLE);
		halRfSendPacket(RxBuf,10);//发送应答命令数据
//		Delayms(100);
//		halRfSendPacket(RxBuf,10);//发送应答命令数据

		for(i=0;i<10;i++) RxBuf[i] =0;



	}

	else
	{

		Delayms(10);

		RxBuf[0]= VERSION_HI;
		RxBuf[1]= VERSION_LO;
	 	RxBuf[2]=EEPROM_read(IAP0_ADDR+INIT_INDEX);
	 	RxBuf[3]=EEPROM_read(IAP0_ADDR+SEC_INDEX);
		RxBuf[4]=EEPROM_read(IAP0_ADDR+NUM_INDEX);
	 	RxBuf[5]=EEPROM_read(IAP0_ADDR+SBED_INDEX);
	 	RxBuf[6]=EEPROM_read(IAP0_ADDR+EBED_INDEX);
		RxBuf[7]=EEPROM_read(IAP0_ADDR+VOL_INDEX);	
		RxBuf[8]=EEPROM_read(IAP0_ADDR+CRC_INDEX);	

	 	RxBuf[9] =EEPROM_read(IAP1_ADDR+INIT_INDEX);
	 	RxBuf[10]=EEPROM_read(IAP1_ADDR+SEC_INDEX);
		RxBuf[11]=EEPROM_read(IAP1_ADDR+NUM_INDEX);
	 	RxBuf[12]=EEPROM_read(IAP1_ADDR+SBED_INDEX);
	 	RxBuf[13]=EEPROM_read(IAP1_ADDR+EBED_INDEX);
		RxBuf[14]=EEPROM_read(IAP1_ADDR+VOL_INDEX);	
		RxBuf[15]=EEPROM_read(IAP1_ADDR+CRC_INDEX);	

		RxBuf[16]= 0x55;
		halSpiStrobe(CCxxx0_SIDLE);
		halRfSendPacket(RxBuf,17);//发送应答命令数据



#if USE_UART1==1
		for(i=0;i<17;i++)
		{
			UART1_SendData8(RxBuf[i]);
		}

#endif


		for(i=0;i<17;i++) RxBuf[i] =0;


		para_error =0;
	}
}


 
