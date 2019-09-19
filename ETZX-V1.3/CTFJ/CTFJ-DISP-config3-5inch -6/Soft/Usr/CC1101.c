#define _IN_CC1101_
#include "config.h"
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
uint8 xdata PaTabel[8] = {0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0};  //0dBm


typedef struct S_RF_SETTINGS
{
	uint8 FSCTRL2;		//���Ѽӵ�
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
//	uint8 MCSM2;     // ��ͨ�ſ���״̬������************/
    uint8 WOREVT1;   // ���ֽ�ʱ��0��ͣ*****************/ // High uchar Event 0 timeout
    uint8 WOREVT0;   // ���ֽ�ʱ��0��ͣ*****************/ // Low uchar Event 0 timeout
    uint8 WORCTRL;   // ��Ų��������******************/ // Wake On Radio control
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
	//0x19,  //MCSM2     // ��ͨ�ſ���״̬������ 
    0x11, // WOREVT1   // ���ֽ�ʱ��0��ͣ High uchar Event 0 timeout
    0xf1, // WOREVT0   // ���ֽ�ʱ��0��ͣ Low uchar Event 0 timeout
    0x78  // WORCTRL	 // ��Ų�������� Wake On Radio control
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
	//0x19,  //MCSM2     // ��ͨ�ſ���״̬������/ 
    0x11, // WOREVT1   // ���ֽ�ʱ��0��ͣ/ High uchar Event 0 timeout
    0xf1, // WOREVT0   // ���ֽ�ʱ��0��ͣ/ Low uchar Event 0 timeout
    0x78  // WORCTRL	 // ��Ų��������/ Wake On Radio control
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
	// 0x18, //MCSM2 // ��ͨ�ſ���״̬������/ 
	// 0x01, // WOREVT1 // ���ֽ�ʱ��0��ͣ/ High uchar Event 0 timeout
	// 0xF1, // WOREVT0 // ���ֽ�ʱ��0��ͣ/ Low uchar Event 0 timeout
	// 0x38, // WORCTRL // ��Ų��������/ Wake On Radio control
	/*
	0x09, //MCSM2 // ��ͨ�ſ���״̬������ / 
	0x02, // WOREVT1 // ���ֽ�ʱ��0��ͣ		 / High uchar Event 0 timeout
	0x08, // WOREVT0 // ���ֽ�ʱ��0��ͣ		 / Low uchar Event 0 timeout
	0x38, // WORCTRL // ��Ų��������		 / Wake On Radio control
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
//��������delay(unsigned int s)
//���룺ʱ��
//�������
//������������ͨ͢ʱ,�ڲ���
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
//��������CpuInit()
//���룺��
//�������
//����������SPI��ʼ������
/*****************************************************************************************/
void CC1101Init(void)
{
//	SpiInit();
	delay(5000);
	POWER_UP_RESET_CC1100();
	halRfWriteRfSettings();
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
	halSpiStrobe(CCxxx0_SRX);//�������״̬
}
	

//*****************************************************************************************
//��������SpisendByte(uint8 dat)
//���룺���͵�����
//�������
//����������SPI����һ���ֽ�
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
//��������void RESET_CC1100(void)
//���룺��
//�������
//������������λCC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
	uint16 xdata i=0xffff;
	CSN = 0; 
	while (MISO)
	{
		if(--i==0) break;
	}
    SpiTxRxByte(CCxxx0_SRES); 		//д�븴λ����
    i=0xffff;
	while (MISO)
	{
		if(--i==0) break;	
	}
    CSN = 1; 
}

//*****************************************************************************************
//��������void POWER_UP_RESET_CC1100(void) 
//���룺��
//�������
//�����������ϵ縴λCC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
	CSN = 1; 
	halWait(1); 
	CSN = 0; 
	halWait(1); 
	CSN = 1; 
	halWait(41); 
	RESET_CC1100();   		//��λCC1100
}

//*****************************************************************************************
//��������void halSpiWriteReg(uint8 addr, uint8 value)
//���룺��ַ��������
//�������
//����������SPIд�Ĵ���
//*****************************************************************************************
void halSpiWriteReg(uint8 addr, uint8 value) 
{
	uint16 xdata i=0xffff;
    CSN = 0;
    while (MISO)
	{
		if(--i==0) break;	
	}
	WDT_CONTR = 0x3e;	//ι��
    SpiTxRxByte(addr);		//д��ַ
    SpiTxRxByte(value);		//д������
    CSN = 1;
}

//*****************************************************************************************
//��������void halSpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
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
	WDT_CONTR = 0x3e;	//ι��	
    SpiTxRxByte(temp);
    for (i = 0; i < count; i++)
 	{
        SpiTxRxByte(buffer[i]);
    }
    CSN = 1;
}

//*****************************************************************************************
//��������void halSpiStrobe(uint8 strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void halSpiStrobe(uint8 strobe) 
{
	uint16 xdata i=0xffff;
    CSN = 0;
    while (MISO)
	{
		if(--i==0) break;	
	}	
	WDT_CONTR = 0x3e;	//ι��	
    SpiTxRxByte(strobe);		//д������
    CSN = 1;
}

//*****************************************************************************************
//��������uint8 halSpiReadReg(uint8 addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
uint8 halSpiReadReg(uint8 addr) 
{
	uint8 temp, value;
	uint16 xdata i=0xffff;
    temp = addr|READ_SINGLE;//���Ĵ�������
	CSN = 0;
	while (MISO)
	{
		if(--i==0) break;	
	}
	WDT_CONTR = 0x3e;	//ι��	
	SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN = 1;
	return value;
}


//*****************************************************************************************
//��������void halSpiReadBurstReg(uint8 addr, uint8 *buffer, uint8 count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiReadBurstReg(uint8 addr, uint8 *buffer, uint8 count) 
{
    uint8 i,temp;
	uint16 xdata j=0xffff;
	temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
    CSN = 0;
    while (MISO)
	{
		if(--j==0) break;	
	}
	WDT_CONTR = 0x3e;	//ι��
	SpiTxRxByte(temp);   
    for (i = 0; i < count; i++) 
	{
        buffer[i] = SpiTxRxByte(0);
    }
    CSN = 1;
}


//*****************************************************************************************
//��������uint8 halSpiReadReg(uint8 addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
uint8 halSpiReadStatus(uint8 addr) 
{
    uint8 value,temp;
	uint16 xdata i=0xffff;
	temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    CSN = 0;
    while (MISO)
	{
		if(--i==0) break;	
	}
	WDT_CONTR = 0x3e;	//ι��
    SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN = 1;
	return value;
}
//*****************************************************************************************
//��������void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//���룺��
//�������
//��������������CC1100�ļĴ���
//*****************************************************************************************
void halRfWriteRfSettings(void) 
{

	halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//���Ѽӵ�
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
//��������void halRfSendPacket(uint8 *txBuffer, uint8 size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************

void halRfSendPacket(uint8 *txBuffer, uint8 size) 
{
	uint16 xdata i=0xffff;
 	CCAPM0 &= (~PCA0_ECCF);	//�رյ���
	CCAPM1 &= (~PCA1_ECCF);	//�ر��ж�	
	halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����

    halSpiStrobe(CCxxx0_STX);		//���뷢��ģʽ��������

    // Wait for GDO0 to be set -> sync transmitted
/*    while (!RF_IRQ)
	{ 
		delay(100);
		WDT_CONTR = 0x3e;	//ι��		
     	if(--i==0) break;

	}
    // Wait for GDO0 to be cleared -> end of packet
    i=0xffff;
    while (RF_IRQ)
	{
		delay(100);
		WDT_CONTR = 0x3e;	//ι��		
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
    halSpiStrobe(CCxxx0_SRX);		//�������״̬
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
	uint8 i=4;// ѭ�����Դ���
    temp = CCxxx0_SNOP|READ_SINGLE;//���Ĵ�������
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
//	uint8 i=(*length)*4;  // �������Ҫ����datarate��length������

   // halSpiStrobe(CCxxx0_SRX);		//�������״̬
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
  if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //����ӵ��ֽ�����Ϊ0
	{
       packetLength = halSpiReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
        if (packetLength <= *length) 		//�����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
		{
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
            *length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
			halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return (status[1] & CRC_OK);			//���У��ɹ����ؽ��ճɹ�
        }
		 else 
		{
            *length = packetLength;
            halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return 0;
        }
    } 
	else
 	return 0;  
}
