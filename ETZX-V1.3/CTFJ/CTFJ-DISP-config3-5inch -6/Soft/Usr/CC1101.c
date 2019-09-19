#define _IN_CC1101_
#include "config.h"
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
uint8 xdata PaTabel[8] = {0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0};  //0dBm


typedef struct S_RF_SETTINGS
{
	uint8 FSCTRL2;		//自已加的
    uint8 FSCTRL1;   // Frequency synthesizer control.
    uint8 FSCTRL0;   // Frequency synthesizer control.
    uint8 FREQ2;     // Frequency control word, high uint8.
    uint8 FREQ1;     // Frequency control word, middle uint8.
    uint8 FREQ0;     // Frequency control word, low uint8.
    uint8 MDMCFG4;   // Modem configuration.
    uint8 MDMCFG3;   // Modem configuration.
    uint8 MDMCFG2;   // Modem configuration.
    uint8 MDMCFG1;   // Modem configuration.
    uint8 MDMCFG0;   // Modem configuration.
    uint8 CHANNR;    // Channel number.
    uint8 DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    uint8 FREND1;    // Front end RX configuration.
    uint8 FREND0;    // Front end RX configuration.
    uint8 MCSM0;     // Main Radio Control State Machine configuration.
    uint8 FOCCFG;    // Frequency Offset Compensation Configuration.
    uint8 BSCFG;     // Bit synchronization Configuration.
    uint8 AGCCTRL2;  // AGC control.
	uint8 AGCCTRL1;  // AGC control.
    uint8 AGCCTRL0;  // AGC control.
    uint8 FSCAL3;    // Frequency synthesizer calibration.
    uint8 FSCAL2;    // Frequency synthesizer calibration.
	uint8 FSCAL1;    // Frequency synthesizer calibration.
    uint8 FSCAL0;    // Frequency synthesizer calibration.
    uint8 FSTEST;    // Frequency synthesizer calibration control
    uint8 TEST2;     // Various test settings.
    uint8 TEST1;     // Various test settings.
    uint8 TEST0;     // Various test settings.
    uint8 IOCFG2;    // GDO2 output pin configuration
    uint8 IOCFG0;    // GDO0 output pin configuration
    uint8 PKTCTRL1;  // Packet automation control.
    uint8 PKTCTRL0;  // Packet automation control.
    uint8 ADDR;      // Device address.
    uint8 PKTLEN;    // Packet length.
///////////////////////////////////////////////////////////////////////////////////////
//	uint8 MCSM2;     // 主通信控制状态机配置************/
    uint8 WOREVT1;   // 高字节时间0暂停*****************/ // High uchar Event 0 timeout
    uint8 WOREVT0;   // 低字节时间0暂停*****************/ // Low uchar Event 0 timeout
    uint8 WORCTRL;   // 电磁波激活控制******************/ // Wake On Radio control
} RF_SETTINGS;

/////////////////////////////////////////////////////////////////
/*const  RF_SETTINGS rfSettings = 
{
	0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0x5b,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x0c,   // PKTLEN    Packet length.
///////////////////////////////////////////////////////////////////////////////////////
	//0x19,  //MCSM2     // 主通信控制状态机配置 
    0x11, // WOREVT1   // 高字节时间0暂停 High uchar Event 0 timeout
    0xf1, // WOREVT0   // 低字节时间0暂停 Low uchar Event 0 timeout
    0x78  // WORCTRL	 // 电磁波激活控制 Wake On Radio control
};
*/

const  RF_SETTINGS rfSettings = 
{
/*
	0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0x5b,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x0c,   // PKTLEN    Packet length.
///////////////////////////////////////////////////////////////////////////////////////
	//0x19,  //MCSM2     // 主通信控制状态机配置/ 
    0x11, // WOREVT1   // 高字节时间0暂停/ High uchar Event 0 timeout
    0xf1, // WOREVT0   // 低字节时间0暂停/ Low uchar Event 0 timeout
    0x78  // WORCTRL	 // 电磁波激活控制/ Wake On Radio control
*/


	0x00,
	0x08, // FSCTRL1 Frequency synthesizer control.
	0x00, // FSCTRL0 Frequency synthesizer control.
	0x10, // FREQ2 Frequency control word, high byte.
	0xA7, // FREQ1 Frequency control word, middle byte.
	0x62, // FREQ0 Frequency control word, low byte.
	0x58, // MDMCFG4 Modem configuration.
	0x43, // MDMCFG3 Modem configuration.
	0x03, // MDMCFG2 Modem configuration.
	0x22, // MDMCFG1 Modem configuration.
	0xF8, // MDMCFG0 Modem configuration.
	/* //
	0xc8, // MDMCFG4 Modem configuration.wor,,
	0x93, // MDMCFG3 Modem configuration.wor,,
	0x03, // MDMCFG2 Modem configuration.wor,,
	0xf2, // MDMCFG1 Modem configuration.wor,,
	0xF8, // MDMCFG0 Modem configuration.wor,,
	*/
	0x00, // CHANNR Channel number.
	0x47, // DEVIATN Modem deviation setting (when FSK modulation is enabled).
	0x56, // FREND1 Front end RX configuration.
	0x10, // FREND0 Front end RX configuration.
	0x18, // MCSM0 Main Radio Control State Machine configuration.
	0x1D, // FOCCFG Frequency Offset Compensation Configuration.
	0x1C, // BSCFG Bit synchronization Configuration.
	0x43, // AGCCTRL2 AGC control.
	0x40, // AGCCTRL1 AGC control.
	0x91, // AGCCTRL0 AGC control.
	0xEA, // FSCAL3 Frequency synthesizer calibration.
	0x2A, // FSCAL2 Frequency synthesizer calibration.
	0x00, // FSCAL1 Frequency synthesizer calibration.
	0x11, // FSCAL0 Frequency synthesizer calibration.
	0x59, // FSTEST Frequency synthesizer calibration.
	0x81, // TEST2 Various test settings.
	0x35, // TEST1 Various test settings.
	0x09, // TEST0 Various test settings.
	0x0B, // IOCFG2 GDO2 output pin configuration.
	0x06, // IOCFG0D GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
	0x04, // PKTCTRL1 Packet automation control.
	// 0x4b, // PKTCTRL1 Packet automation control ,,,,wor
	0x05, // PKTCTRL0 Packet automation control.
	0x00, // ADDR Device address.
	0x0c, // PKTLEN Packet length.
	///////////////////////////////////////////////////////////////////////////////////////////
	// 0x18, //MCSM2 // 主通信控制状态机配置/ 
	// 0x01, // WOREVT1 // 高字节时间0暂停/ High uchar Event 0 timeout
	// 0xF1, // WOREVT0 // 低字节时间0暂停/ Low uchar Event 0 timeout
	// 0x38, // WORCTRL // 电磁波激活控制/ Wake On Radio control
	/*
	0x09, //MCSM2 // 主通信控制状态机配置 / 
	0x02, // WOREVT1 // 高字节时间0暂停		 / High uchar Event 0 timeout
	0x08, // WOREVT0 // 低字节时间0暂停		 / Low uchar Event 0 timeout
	0x38, // WORCTRL // 电磁波激活控制		 / Wake On Radio control
	*/

};


void CC1101Init(void);
void delay(unsigned int s);
void RESET_CC1100(void);
void POWER_UP_RESET_CC1100(void);
void halSpiWriteReg(uint8 addr, uint8 value);
void halSpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count);
void halSpiStrobe(uint8 strobe);
uint8 halSpiReadReg(uint8 addr);
void halSpiReadBurstReg(uint8 addr, uint8 *buffer, uint8 count);
uint8 halSpiReadStatus(uint8 addr);
void halRfWriteRfSettings(void);
void halRfSendPacket(uint8 *txBuffer, uint8 size); 
uint8 halRfReceivePacket(uint8 *rxBuffer, uint8 *length); 

//*****************************************************************************************
//函数名：delay(unsigned int s)
//输入：时间
//输出：无
//功能描述：普通廷时,内部用
//*****************************************************************************************		
void delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}


void halWait(uint16 timeout) {
    do {
        _nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_(); 
		WDT_CONTR = 0x3e;		
    } while (--timeout);
}


/*void SpiInit(void)
{
	CSN=0;
	SCK=0;
	CSN=1;
}*/

/*****************************************************************************************
//函数名：CpuInit()
//输入：无
//输出：无
//功能描述：SPI初始化程序
/*****************************************************************************************/
void CC1101Init(void)
{
//	SpiInit();
	delay(5000);
	POWER_UP_RESET_CC1100();
	halRfWriteRfSettings();
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
	halSpiStrobe(CCxxx0_SRX);//进入接收状态
}
	

//*****************************************************************************************
//函数名：SpisendByte(uint8 dat)
//输入：发送的数据
//输出：无
//功能描述：SPI发送一个字节
//*****************************************************************************************
#define SpiTxRxByte(dat) SPI_WriteByte(dat)


/*uint8 SpiTxRxByte(uint8 dat)
{
	uint8 i,temp;
	temp = 0;
	
	SCK = 0;
	for(i=0; i<8; i++)
	{
		if(dat & 0x80)
		{
			MOSI = 1;
		}
		else MOSI = 0;
		dat <<= 1;

		SCK = 1; 
		_nop_();
		_nop_();

		temp <<= 1;
		if(MISO)temp++; 
		SCK = 0;
		_nop_();
		_nop_();	
	}
	return temp;
}
*/
//*****************************************************************************************
//函数名：void RESET_CC1100(void)
//输入：无
//输出：无
//功能描述：复位CC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
	uint16 xdata i=0xffff;
	CSN = 0; 
	while (MISO)
	{
		if(--i==0) break;
	}
    SpiTxRxByte(CCxxx0_SRES); 		//写入复位命令
    i=0xffff;
	while (MISO)
	{
		if(--i==0) break;	
	}
    CSN = 1; 
}

//*****************************************************************************************
//函数名：void POWER_UP_RESET_CC1100(void) 
//输入：无
//输出：无
//功能描述：上电复位CC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
	CSN = 1; 
	halWait(1); 
	CSN = 0; 
	halWait(1); 
	CSN = 1; 
	halWait(41); 
	RESET_CC1100();   		//复位CC1100
}

//*****************************************************************************************
//函数名：void halSpiWriteReg(uint8 addr, uint8 value)
//输入：地址和配置字
//输出：无
//功能描述：SPI写寄存器
//*****************************************************************************************
void halSpiWriteReg(uint8 addr, uint8 value) 
{
	uint16 xdata i=0xffff;
    CSN = 0;
    while (MISO)
	{
		if(--i==0) break;	
	}
	WDT_CONTR = 0x3e;	//喂狗
    SpiTxRxByte(addr);		//写地址
    SpiTxRxByte(value);		//写入配置
    CSN = 1;
}

//*****************************************************************************************
//函数名：void halSpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count)
//输入：地址，写入缓冲区，写入个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count) 
{
    uint8 i, temp;
	uint16 xdata j=0xffff;
	temp = addr | WRITE_BURST;
    CSN = 0;
    while (MISO)
	{
		if(--j==0) break;	
	}	
	WDT_CONTR = 0x3e;	//喂狗	
    SpiTxRxByte(temp);
    for (i = 0; i < count; i++)
 	{
        SpiTxRxByte(buffer[i]);
    }
    CSN = 1;
}

//*****************************************************************************************
//函数名：void halSpiStrobe(uint8 strobe)
//输入：命令
//输出：无
//功能描述：SPI写命令
//*****************************************************************************************
void halSpiStrobe(uint8 strobe) 
{
	uint16 xdata i=0xffff;
    CSN = 0;
    while (MISO)
	{
		if(--i==0) break;	
	}	
	WDT_CONTR = 0x3e;	//喂狗	
    SpiTxRxByte(strobe);		//写入命令
    CSN = 1;
}

//*****************************************************************************************
//函数名：uint8 halSpiReadReg(uint8 addr)
//输入：地址
//输出：该寄存器的配置字
//功能描述：SPI读寄存器
//*****************************************************************************************
uint8 halSpiReadReg(uint8 addr) 
{
	uint8 temp, value;
	uint16 xdata i=0xffff;
    temp = addr|READ_SINGLE;//读寄存器命令
	CSN = 0;
	while (MISO)
	{
		if(--i==0) break;	
	}
	WDT_CONTR = 0x3e;	//喂狗	
	SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN = 1;
	return value;
}


//*****************************************************************************************
//函数名：void halSpiReadBurstReg(uint8 addr, uint8 *buffer, uint8 count)
//输入：地址，读出数据后暂存的缓冲区，读出配置个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiReadBurstReg(uint8 addr, uint8 *buffer, uint8 count) 
{
    uint8 i,temp;
	uint16 xdata j=0xffff;
	temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
    CSN = 0;
    while (MISO)
	{
		if(--j==0) break;	
	}
	WDT_CONTR = 0x3e;	//喂狗
	SpiTxRxByte(temp);   
    for (i = 0; i < count; i++) 
	{
        buffer[i] = SpiTxRxByte(0);
    }
    CSN = 1;
}


//*****************************************************************************************
//函数名：uint8 halSpiReadReg(uint8 addr)
//输入：地址
//输出：该状态寄存器当前值
//功能描述：SPI读状态寄存器
//*****************************************************************************************
uint8 halSpiReadStatus(uint8 addr) 
{
    uint8 value,temp;
	uint16 xdata i=0xffff;
	temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
    CSN = 0;
    while (MISO)
	{
		if(--i==0) break;	
	}
	WDT_CONTR = 0x3e;	//喂狗
    SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN = 1;
	return value;
}
//*****************************************************************************************
//函数名：void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//输入：无
//输出：无
//功能描述：配置CC1100的寄存器
//*****************************************************************************************
void halRfWriteRfSettings(void) 
{

	halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//自已加的
    // Write register settings
    halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    halSpiWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
    halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
	halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
	halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
	halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
    halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
    halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
    halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
//////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////
//	halSpiWriteReg(CCxxx0_MCSM2 ,   rfSettings.MCSM2 );
	halSpiWriteReg(CCxxx0_WOREVT1,  rfSettings.WOREVT1);
    halSpiWriteReg(CCxxx0_WOREVT0,  rfSettings.WOREVT0);
    halSpiWriteReg(CCxxx0_WORCTRL,  rfSettings.WORCTRL);
}

//*****************************************************************************************
//函数名：void halRfSendPacket(uint8 *txBuffer, uint8 size)
//输入：发送的缓冲区，发送数据个数
//输出：无
//功能描述：CC1100发送一组数据
//*****************************************************************************************

void halRfSendPacket(uint8 *txBuffer, uint8 size) 
{
	uint16 xdata i=0xffff;
 	CCAPM0 &= (~PCA0_ECCF);	//关闭调度
	CCAPM1 &= (~PCA1_ECCF);	//关闭中断	
	halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//写入要发送的数据

    halSpiStrobe(CCxxx0_STX);		//进入发送模式发送数据

    // Wait for GDO0 to be set -> sync transmitted
/*    while (!RF_IRQ)
	{ 
		delay(100);
		WDT_CONTR = 0x3e;	//喂狗		
     	if(--i==0) break;

	}
    // Wait for GDO0 to be cleared -> end of packet
    i=0xffff;
    while (RF_IRQ)
	{
		delay(100);
		WDT_CONTR = 0x3e;	//喂狗		
	  	if(--i==0) break;
	
	}
*/
	Delayms(200);
	
	halSpiStrobe(CCxxx0_SFTX);
	CCF0=0;
  	CCAPM0 |= PCA0_ECCF;
	CCF1=0;
	CCAPM1 |= PCA1_ECCF;	
}

/*
void setRxMode(void)
{
    halSpiStrobe(CCxxx0_SRX);		//进入接收状态
}
*/
/*
// Bit masks corresponding to STATE[2:0] in the status byte returned on MISO
#define CCxx00_STATE_BM                 0x70
#define CCxx00_FIFO_BYTES_AVAILABLE_BM  0x0F
#define CCxx00_STATE_TX_BM              0x20
#define CCxx00_STATE_TX_UNDERFLOW_BM    0x70
#define CCxx00_STATE_RX_BM              0x10
#define CCxx00_STATE_RX_OVERFLOW_BM     0x60
#define CCxx00_STATE_IDLE_BM            0x00

static uint8 RfGetRxStatus(void)
{
	uint8 temp, spiRxStatus1,spiRxStatus2;
	uint8 i=4;// 循环测试次数
    temp = CCxxx0_SNOP|READ_SINGLE;//读寄存器命令
	CSN = 0;
	while (MISO);
	SpiTxRxByte(temp);
	spiRxStatus1 = SpiTxRxByte(0);
	do
	{
		SpiTxRxByte(temp);
		spiRxStatus2 = SpiTxRxByte(0);
		if(spiRxStatus1 == spiRxStatus2)
		{
			if( (spiRxStatus1 & CCxx00_STATE_BM) == CCxx00_STATE_RX_OVERFLOW_BM)
			{
               halSpiStrobe(CCxxx0_SFRX);
			   return 0;
			}
		    return 1;
		}
		 spiRxStatus1=spiRxStatus2;
	}
	while(i--);
	CSN = 1;
    return 0;	
}
 */
 uint8 halRfReceivePacket(uint8 *rxBuffer, uint8 *length) 
{
    uint8 status[2];
    uint8 packetLength;
//	uint8 i=(*length)*4;  // 具体多少要根据datarate和length来决定

   // halSpiStrobe(CCxxx0_SRX);		//进入接收状态
	//delay(5);
    //while (!GDO1);
    //while (GDO1);
	//delay(2);
//	while (GDO0)
//	{
	//	delay(2);
	//	--i;
	//	if(i<1)
	//	   return 0; 	    
//	}	 
  if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
	{
       packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
        if (packetLength <= *length) 		//如果所要的有效数据长度小于等于接收到的数据包的长度
		{
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
            *length = packetLength;				//把接收数据长度的修改为当前数据的长度
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
			halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
            return (status[1] & CRC_OK);			//如果校验成功返回接收成功
        }
		 else 
		{
            *length = packetLength;
            halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
            return 0;
        }
    } 
	else
 	return 0;  
}
