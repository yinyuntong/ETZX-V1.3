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
unsigned short power_rec[4];  //��ز������� 


bool rfirq =0;
bool audio_irq=0;

INT16U RFStatusCnt= 0;
INT8U RFClearState = 0,RFStatus=0;


char CC1101SigValue=0;


STEepromCfgData stEepromCfgData;


bool low_bat=0;
bool para_error=0;



extern INT8U play_start,play_end;//����������ʼ�����


void seng_tx_para(void);


//IO����
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
//PD5  --  BATT (INPUT)ģ�� 
//PD6  --  SW2(INPUT IRQ)
void ConfigIOPort(void)
{
	GPIOA->ODR &= ~(IO_SW3); //�õ�	Ĭ��Ϊ����ڣ�����Ϊ�����ʱ��������
	GPIOA->ODR &= ~IO_IRCNT; //�õ͡����ı䷽��ʱ��ֱ�ӹر�һ�������
	GPIOA->DDR |= IO_RFCS|IO_IRCNT; //���ģʽ
	GPIOA->CR1 |= (IO_RFCS|IO_SW3); //����
	GPIOA->CR2 |= (IO_SW3); //�������ж�
	GPIOB->ODR |= IO_LED; //�ø�
	GPIOB->DDR |= IO_LED; //���ģʽ
	GPIOB->CR2 |= (IO_INTAUD); 
	GPIOC->ODR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
	GPIOC->DDR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
	GPIOC->CR1 |= IO_SPICLK|IO_SPIMOSI|IO_SPIMISO|IO_AUDRST|IO_AUDCS;
	GPIOD->ODR |= IO_AUDCS;
	GPIOD->DDR |= IO_AUDCS|IO_RFCS|IO_IR;
	GPIOD->CR1 |= IO_SW4|IO_SW2|IO_IR;
	GPIOD->CR2 |= (IO_SW4|IO_SW2); //�������ж�

#if CHECK_560US==1
	GPIOD->ODR |= IO_RFTS;		//���Ϊ�ߵ�ƽ,����Ϊ�����
	GPIOD->DDR |= IO_RFTS;
#endif
}

#define EEPROM_BASE_ADDRESS 0x4000
#define EEPROM_MAX_ADDRESS (EEPROM_BASE_ADDRESS+640)
#define EEPROM_PROTECT_DISABLE()    do{     FLASH->DUKR = 0xAE;          \
                                            FLASH->DUKR = 0x56;          \
                                    }while(0)                           //  д�����ر�

#define EEPROM_PROTECT_ENABLE()     do{ FLASH->IAPSR &= 0xF7; } while(0) //  д��������





//��ַ0:����
//��ַ1:����尴�������ź�ģʽѡ��0:����433��1:����(��ͳһΪ����,�����ݺ���)
//��ַ2:��尴������433���ʣ��ִ����ݲ��ú��ԡ�
//��ַ3: ����
//��ַ4:�ƶ��ֻ���
//��ַ5:�ƶ��ֻ�����Ĵ�ͷ�ֻ���ʼ�ţ�Ĭ��Ϊ1
//��ַ6:�ƶ��ֻ�����Ĵ�ͷ�ֻ������ţ�Ĭ��Ϊ255


INT8U EEPROM_write(INT16U uiAddress, INT8U ucData)
{
    INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress; 
    if (eeprom_addr1 > EEPROM_MAX_ADDRESS)                              // ��ַ����
    {
        return 1;                                            // 
    }

#if USE_UART1==0		//û��ʹ�ô��ڵ��ԣ�����������̬��

	if(low_bat )
	{//��ѹ����
		return 1;
	}
#endif

    EEPROM_PROTECT_DISABLE();                                           // �ر�д����
    *((@far INT8U*) eeprom_addr1) = ucData;                             // 
    EEPROM_PROTECT_ENABLE();                                            // ����д����
    return 0;
}
INT8U EEPROM_read(INT16U uiAddress)
{
    INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress;             //
    return  *((@far INT8U*) eeprom_addr1);                              // ��ȡ���� 
}



/*----------------------------
Get ADC result
----------------------------*/
INT16U GetADCResult(INT8U ch)
{
	INT16U value,i;
	ADC1_DeInit();
	ADC1->CSR|=ch; //ѡ��ͨ��
  	ADC1->CR2|=0x08; // ��������,�Ҷ���
	ADC1->CR2&=~0x02; //SCAN DISABLE
	ADC1->CR3&=~0x80;  //DATE BUFFER DISABLE
	ADC1->TDRL=(1<<ch);
	ADC1->CR1&=~0x02;
	ADC1->CR1|=0x01;
	for(i=0;i<200;i++); // ��ʱһ��ʱ�䣬����100uS����֤ADCģ����ϵ����
  	ADC1->CSR&=(~0x80);
	ADC1->CR1|=0x01; // �ٴν�CR1�Ĵ��������λ��1,����ADת��
  	while((ADC1->CSR & 0x80)==0); // �ȴ�ת������
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



//����stLocalControl.stEepromCfgData�е�����У���
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
			{//д��ȥ���������������������ͬ

				//break;
				return 1;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			//break;	//��ͬ
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
	{//��һ��У�����ȷ
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
	{//�ڶ���У�����ȷ
		if(EEPROM_read(IAP1_ADDR+INIT_INDEX)== 0X5A)
		{	
			if(result==0)
			{//0�����ݲ���ȷ�������
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
			WTVxxxInit(15);      //����оƬ��ʼ��
			PlaySound(20);		 //ʧ��			
			break;
			
		case 1://0��������ȷ��1�����ݲ���ȷ
			SaveParameter(IAP1_ADDR);
			break;
		
		case 2://1��������ȷ��0�����ݲ���ȷ	
			SaveParameter(IAP0_ADDR);
			break;

		case 3:	//�������ݶ���ȷ
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
  	if(halRfReceivePacket(RxBuf,&leng))     //��ȡ���յ�����������
	{ 
		UART1_SendData8(leng);
		for(i=0;i<leng;i++)
		{
			UART1_SendData8(RxBuf[i]);
		}	

		//һ������Ĳ�������

		if(leng ==5 )
		{
			if(stEepromCfgData.bySec ==254)
			{
				
				if((RxBuf[0]==0xff) && (RxBuf[1]==0xff) && (RxBuf[2]==0x01) &&  (RxBuf[3]==0x32) && (RxBuf[4]==0x00))
				{
	  				SetLEDMode(1);			//���ģʽ��������


					TxBuf[0]=0xff;
					TxBuf[1]=0xff;  		 
					TxBuf[2]=0x01;
					TxBuf[3]=0x32;
					TxBuf[4]= CC1101SigValue;

					halSpiStrobe(CCxxx0_SIDLE);
					halRfSendPacket(TxBuf,5);//���ͻظ���������
					Delayms(100);
					halRfSendPacket(TxBuf,5);//���ͻظ���������

					WTVxxxInit(stEepromCfgData.byVol);	   //����оƬ��ʼ��
					PlaySound(0x0e);

					SetLEDMode(0);			//����ģʽ��������					
				}
			}
		}
				
		else if (leng>7)
		{
			halSpiStrobe(CCxxx0_SPWD);	//�������״̬
		
  			SetLEDMode(1);			//���ģʽ��������

			i=0;
	  		if(RxBuf[7]==0xaa)
	  		{ 
	  			switch(RxBuf[3])                    //�б��������
				{  
					case 0x5a:                       // ���
		     		if(RxBuf[1]==251)
					{//�Ӵ��ֻ�
						SetLEDMode(0);			//sw3����ģʽ
						Delayms(1);
						if(GetSW2()==0 || GetSW3()==0)
						{
							//ֻ�е��¼����²Ž�����״̬
							SetLEDMode(1);			//���ģʽ��������
					
				      		cont=0;cont1=0;
							while(1)
							{ 
								cont++;
								if (cont==500)		//0.5��
								{
									SetLEDMode(0);	//����ģʽ��������
								}
	              				if(cont>1000)
								{
									cont=0;
									SetLEDMode(1);	//���ģʽ��������	
									cont1++;
								}
							  	if(cont1>10){break;}	//5��ʱ�䵽���˳����״̬
							  	if(GetSW4()==0)
							  	{ //��尴������
									stEepromCfgData.byInitFlag =0x5a;
									stEepromCfgData.bySec = RxBuf[0];	//����
									stEepromCfgData.byNum = RxBuf[2];	//�ƶ��ֻ���
									stEepromCfgData.bySBed = RxBuf[4];	//��ʼ����
									stEepromCfgData.byEBed = RxBuf[5];	//��������
									stEepromCfgData.byCrc = CalcParaCrc();

									i=SaveParameter(IAP0_ADDR);
									i=SaveParameter(IAP1_ADDR);
									WTVxxxInit(stEepromCfgData.byVol);      //����оƬ��ʼ��

									if(i)
									{
				    					PlaySound(19);//�ɹ� 

										seng_tx_answer(0x5a);
										halSpiStrobe(CCxxx0_SIDLE);
										halSpiStrobe(CCxxx0_SFTX);										
									}

								  	else
									{//ʧ��
										PlaySound(20);
									}
									i=0;
								  	while(GetSW4()==0) {Delayms(20); i++; if(i>=200) break;};	//�ȴ������ɿ�
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
					
				case 0x06:                 	//��ͷ����
					if (((RxBuf[2]<stEepromCfgData.bySBed)||(RxBuf[2]>stEepromCfgData.byEBed))&&RxBuf[1]!=250) break;		//�Ӵ��ֻ�û�н���ȫ������
				case 0x07:					//��Һ
					if (((RxBuf[2]<stEepromCfgData.bySBed)||(RxBuf[2]>stEepromCfgData.byEBed))&&RxBuf[1]!=250) break;
				case 0x08:                 	//���������
					if (RxBuf[1]==251) break;	//�ƶ��ֻ�������
			  		if(stEepromCfgData.bySec==RxBuf[0])
					{//�Ǳ����ź�
						for (i=0;i<4;i++)
						{						
							if (RxBufBak[i]!=RxBuf[i]) break;
						}
						if (play_end==play_start||i<4)
						{//���յ��ź���֮ǰ�źŲ�ͬ����û������������
							calling=1;
							if (play_end==play_start) WTVxxxInit(stEepromCfgData.byVol);      //����оƬ��ʼ��
							else StopPlay();
							for (i=0;i<4;i++) RxBufBak[i] = RxBuf[i];		//�����ݱ���
							for(k=0;k<3;k++)
							{//����3��
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
			SetLEDMode(0);//����ģʽ��������
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
	TIM4_DeInit();		//�ָ���ؼĴ���Ĭ��ֵ
// 1ms��ʱ���õ�4M��HSIƵ��,1MS��ʱ
//	TIM4_TimeBaseInit(TIM4_PRESCALER_32, 0x7e);//��ʼ��Ԥ��Ƶ�����Զ�װ��ֵ

// 1ms��ʱ���õ�16M��HSIƵ��,1MS��ʱ
//	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0x7e);//��ʼ��Ԥ��Ƶ�����Զ�װ��ֵ

// 560us��ʱ���õ�16M��HSIƵ��
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 69);//��ʼ��Ԥ��Ƶ�����Զ�װ��ֵ


// 600us��ʱ���õ�16M��HSIƵ��
//	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 74);//��ʼ��Ԥ��Ƶ�����Զ�װ��ֵ


	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	//�ж�����:����
	TIM4_UpdateDisableConfig(DISABLE);		//��������¼�
	TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);	//������������ж�
	TIM4->EGR=0x01;//������������¼�

	TIM4_Cmd(ENABLE);		//���ö�ʱ��4
//	TIM4->CR1 = 0x81; // Ԥװ��T4����������


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

	//��ʱ��T2����PWM,��ʱ��T4������ͨ��ʱ

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
	AWU_Init(AWU_TIMEBASE_2S);  //2�뻽��һ��

  	FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN); //ֹͣʱFLASH ����
  	CLK_SlowActiveHaltWakeUpCmd(ENABLE);  //ֹͣʱ����Դ�ر�
  //CLK_FastHaltWakeUpCmd(ENABLE); //���ٻ���
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



	SetLEDMode(1);				//���ģʽ��������
	CC1101_Init();              //cc1101��ʼ��
	#if USE_WOR==1
	Init_WOR();					//�ɻ���
	#endif
	halSpiStrobe(CCxxx0_SPWD);	//�������״̬

	InitParameter();
	

/*	if ((stEepromCfgData.bySBed==0&&stEepromCfgData.byEBed==0)||
		(stEepromCfgData.bySBed==255&&stEepromCfgData.byEBed==255)||
		(stEepromCfgData.bySBed>stEepromCfgData.byEBed)) 
		{stEepromCfgData.bySBed=0;stEepromCfgData.byEBed=255;}
*/	
//	WTVxxxInit(stEepromCfgData.byVol);      		//����оƬ��ʼ��

	#if USE_WOR==1
	StartRxWor();
	#else
	halSpiStrobe(CCxxx0_SRX);
	#endif

	SetLEDMode(0);				//����ģʽ��������
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
			power_rec[3] = (power_rec[2]+power_rec[1]+power_rec[0])/3;		// 3�ε�ѹ��ƽ��ֵ


/*
			TxBuf[0]=power_rec[3]>>8;  //��λ
			TxBuf[1]=power_rec[3];  	//��λ		 
			halSpiStrobe(CCxxx0_SIDLE);
			halRfSendPacket(TxBuf,2);//���͵�������
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
		{//�������ݽ��� 
			CheckRecv();
		}
		
		if ((i=key_in())!=0)
		{
			switch(i)
			{
			case 3: 
				if(stEepromCfgData.byVol>2)               //����-
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

				WTVxxxInit(stEepromCfgData.byVol);	   //����оƬ��ʼ��
				PlaySound(22);//�������ڳɹ�
				break;
			case 2: //����+
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

				WTVxxxInit(stEepromCfgData.byVol);	   //����оƬ��ʼ��
				PlaySound(22);
				break;
			case 4: 						//�Ҷϼ�/�鷿
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

					SetIRCNT(0);		//��ֹ�����Ժ��ⷢ��ܷ���,��Ϊ���ʴ������Ա߷ֻ����յ�
					Ir_send_start();

					Ir_send_data(USER_CODE2);		//�û�������ֽ�
					Ir_send_data(USER_CODE1);		//�û�������ֽ�
					Ir_send_data(~OX_KEYVALUE); 	//��ֵ
					Ir_send_data(OX_KEYVALUE);		//��ֵ
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
		{//�����ͺ����˸
			if ((ir_inc%1000)==50)	//���ʱ�䳤����ҪҲ��Ϊ��ʡ�� 
			{
				SetLed(1);		//��
			} 
			else if ((ir_inc%1000)==999) 
			{
				SetLed(0);		//��
			}
		}
		else SetLed(1);	//���������������
		#endif
			

		if(ir_inc %2000 == 0)	//	2S1��
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
			SetIRCNT(1);		//��������,�������Ժ��ⷢ��ܷ���

			Ir_send_start();
			
			Ir_send_data(USER_CODE2);		//�û�������ֽ�
			Ir_send_data(USER_CODE1);		//�û�������ֽ�
			Ir_send_data(~stEepromCfgData.byNum);			//��ֵ
			Ir_send_data(stEepromCfgData.byNum);			//��ֵ
			Ir_send_end();
			#endif

		}



		if(ir_inc %4000 == 0)	//	4S   1��
		{
		
			ir_inc =0;			//�ֵ��������ʱ��������0

//			UART1_SendData8(0x14);			

			//���������һ�Σ�����CC1101ģ��ᱻ���ߵ粨���Ѳ���������״̬
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
				RFStatusCnt = 6000; 	//6��				
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
  		SetLEDMode(1);	//��尴�����ģʽ�����ڵ�ָʾ
  		while(GetSW2()==0) {Delayms(20); x++; if(x>=120) break;}

		if(x>=120) return 5;	//δ��
		else return 2;//����+
  	}


  	if(GetSW3()==0)
  	{//sw3����LEDָʾ������
  		x=0;
		SetLEDMode(0);
  		while(GetSW3()==0) {Delayms(20); x++; if(x>=120) break;}
		SetLEDMode(1);
		
		if(x>=120) return 6;	//δ��
		else return 3;//����-
  	}
	
  	if(GetSW4()==0)
  	{
  		x=0;
  		SetLEDMode(1);
		while(GetSW4()==0){Delayms(20); x++; if(x>=120) break;} 

		if(x>=120) return 7;	//δ��
		else return 4;

	}//�Ҷϼ�
  	return 0;
}

void seng_tx(void)
{  
	INT8U i;
 	calling=0;
	
 	TxBuf[0]=stEepromCfgData.bySec;
    if(TxBuf[0]==RxBufBak[0])
	{ //������������մ�ͷ�ֻ��������
		TxBuf[4]= RxBufBak[0];
	}
	else
	{
		TxBuf[4]=0;		//����������Ϊ0
	}
	TxBuf[1]=251;  //�ƶ��ֻ��ķ��Ź̶�251           
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
	halRfSendPacket(TxBuf,8);//���͹Ҷ���������
	Delayms(100);
	halRfSendPacket(TxBuf,8);//���͹Ҷ���������
}


void seng_tx_answer(INT8U cmd_type)
{  

 	TxBuf[0]=stEepromCfgData.bySec;	//����
 	
	TxBuf[1]=251;  //�ƶ��ֻ��ķ��Ź̶�251           
	TxBuf[2]=stEepromCfgData.byNum;	//�ֻ���
	TxBuf[3]=0x02;

	TxBuf[4]=cmd_type;	//ԭ����
	
	TxBuf[5]= 0x01;		//�ɹ�
	TxBuf[6]= 0x00;
	TxBuf[7]= 0x55;
	halSpiStrobe(CCxxx0_SIDLE);
	halRfSendPacket(TxBuf,8);//����Ӧ����������
	Delayms(100);
	halRfSendPacket(TxBuf,8);//����Ӧ����������
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
	 	RxBuf[3]=stEepromCfgData.bySec;	//����      
		RxBuf[4]=stEepromCfgData.byNum;	//�ֻ���
		RxBuf[5]= stEepromCfgData.bySBed;	
		RxBuf[6]= stEepromCfgData.byEBed;		
		RxBuf[7]= stEepromCfgData.byVol;
		RxBuf[8]= stEepromCfgData.byCrc;
		RxBuf[9]= 0x55;
		halSpiStrobe(CCxxx0_SIDLE);
		halRfSendPacket(RxBuf,10);//����Ӧ����������
//		Delayms(100);
//		halRfSendPacket(RxBuf,10);//����Ӧ����������

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
		halRfSendPacket(RxBuf,17);//����Ӧ����������



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


 
