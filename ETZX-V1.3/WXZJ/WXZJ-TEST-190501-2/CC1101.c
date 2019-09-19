#include "cc1101.h"
#define LENGTH 32
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
INT8U PaTabel[8] = {0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0};  //0dBm


#if 0
typedef struct S_RF_SETTINGS
{
	INT8U FSCTRL2;		//���Ѽӵ�
    INT8U FSCTRL1;   // Frequency synthesizer control.
    INT8U FSCTRL0;   // Frequency synthesizer control.
    INT8U FREQ2;     // Frequency control word, high INT8U.
    INT8U FREQ1;     // Frequency control word, middle INT8U.
    INT8U FREQ0;     // Frequency control word, low INT8U.
    INT8U MDMCFG4;   // Modem configuration.
    INT8U MDMCFG3;   // Modem configuration.
    INT8U MDMCFG2;   // Modem configuration.
    INT8U MDMCFG1;   // Modem configuration.
    INT8U MDMCFG0;   // Modem configuration.
    INT8U CHANNR;    // Channel number.
    INT8U DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    INT8U FREND1;    // Front end RX configuration.
    INT8U FREND0;    // Front end RX configuration.
    INT8U MCSM0;     // Main Radio Control State Machine configuration.
    INT8U FOCCFG;    // Frequency Offset Compensation Configuration.
    INT8U BSCFG;     // Bit synchronization Configuration.
    INT8U AGCCTRL2;  // AGC control.
	INT8U AGCCTRL1;  // AGC control.
    INT8U AGCCTRL0;  // AGC control.
    INT8U FSCAL3;    // Frequency synthesizer calibration.
    INT8U FSCAL2;    // Frequency synthesizer calibration.
	INT8U FSCAL1;    // Frequency synthesizer calibration.
    INT8U FSCAL0;    // Frequency synthesizer calibration.
    INT8U FSTEST;    // Frequency synthesizer calibration control
    INT8U TEST2;     // Various test settings.
    INT8U TEST1;     // Various test settings.
    INT8U TEST0;     // Various test settings.
    INT8U IOCFG2;    // GDO2 output pin configuration
    INT8U IOCFG0;    // GDO0 output pin configuration
    INT8U PKTCTRL1;  // Packet automation control.
    INT8U PKTCTRL0;  // Packet automation control.
    INT8U ADDR;      // Device address.
    INT8U PKTLEN;    // Packet length.
/*
	INT8U MCSM2;       // ��ͨ�ſ���״̬������*********** 
    INT8U WOREVT1;   // ���ֽ�ʱ��0��ͣ***************** // High uchar Event 0 timeout
    INT8U WOREVT0;   // ���ֽ�ʱ��0��ͣ***************** // Low uchar Event 0 timeout
    INT8U WORCTRL;   // ��Ų��������****************** // Wake On Radio control
*/
} RF_SETTINGS;

/////////////////////////////////////////////////////////////////
const RF_SETTINGS rfSettings = 
{
	0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.

    0x58,   // MDMCFG4   Modem configuration.
    0x43,   // MDMCFG3   Modem configuration.
//    0x03,   // MDMCFG2   Modem configuration.
//    0x00,   // MDMCFG2   Modem configuration.  û��ͬ��/����  1������Ҳ�ղ���
	0x01,	// MDMCFG2	 Modem configuration.����15λ��ǰ���֣�16λ��ͬ����

    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

/*	//
    0xc8,   // MDMCFG4   Modem configuration.wor,,
    0x93,   // MDMCFG3   Modem configuration.wor,,
    0x03,   // MDMCFG2   Modem configuration.wor,,
    0xf2,   // MDMCFG1   Modem configuration.wor,,
    0xF8,   // MDMCFG0   Modem configuration.wor,,
*/
    0x00,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0x43,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0E,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

   0x04,   // PKTCTRL1  Packet automation control.
  //  0x4b,   // PKTCTRL1  Packet automation control  ,,,,wor
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    LENGTH,   // PKTLEN    Packet length.
///////////////////////////////////////////////////////////////////////////////////////////
//	0x18,  //MCSM2     // ��ͨ�ſ���״̬������************/ 
 //   0x01, // WOREVT1   // ���ֽ�ʱ��0��ͣ*****************/ High uchar Event 0 timeout
 //   0xF1, // WOREVT0   // ���ֽ�ʱ��0��ͣ*****************/ Low uchar Event 0 timeout
 //   0x38, // WORCTRL	 // ��Ų��������******************/ Wake On Radio control
/*
    0x09,  //MCSM2     // ��ͨ�ſ���״̬������************ / 
    0x02, // WOREVT1   // ���ֽ�ʱ��0��ͣ***************** / High uchar Event 0 timeout
    0x08, // WOREVT0   // ���ֽ�ʱ��0��ͣ***************** / Low uchar Event 0 timeout
    0x38, // WORCTRL	 // ��Ų��������****************** / Wake On Radio control
	*/
};
#endif



typedef struct S_RF_SETTINGS
{
	INT8U FSCTRL2;		//���Ѽӵ�
    INT8U FSCTRL1;   // Frequency synthesizer control.
    INT8U FSCTRL0;   // Frequency synthesizer control.
    INT8U FREQ2;     // Frequency control word, high INT8U.
    INT8U FREQ1;     // Frequency control word, middle INT8U.
    INT8U FREQ0;     // Frequency control word, low INT8U.
    INT8U MDMCFG4;   // Modem configuration.
    INT8U MDMCFG3;   // Modem configuration.
    INT8U MDMCFG2;   // Modem configuration.
    INT8U MDMCFG1;   // Modem configuration.
    INT8U MDMCFG0;   // Modem configuration.
    INT8U CHANNR;    // Channel number.
    INT8U DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    INT8U FREND1;    // Front end RX configuration.
    INT8U FREND0;    // Front end RX configuration.

    INT8U MCSM0;     // Main Radio Control State Machine configuration.
    INT8U FOCCFG;    // Frequency Offset Compensation Configuration.
    INT8U BSCFG;     // Bit synchronization Configuration.
    INT8U AGCCTRL2;  // AGC control.
	INT8U AGCCTRL1;  // AGC control.
    INT8U AGCCTRL0;  // AGC control.
    INT8U FSCAL3;    // Frequency synthesizer calibration.
    INT8U FSCAL2;    // Frequency synthesizer calibration.
	INT8U FSCAL1;    // Frequency synthesizer calibration.
    INT8U FSCAL0;    // Frequency synthesizer calibration.
    INT8U FSTEST;    // Frequency synthesizer calibration control
    INT8U TEST2;     // Various test settings.
    INT8U TEST1;     // Various test settings.
    INT8U TEST0;     // Various test settings.
    INT8U IOCFG2;    // GDO2 output pin configuration
    INT8U IOCFG0;    // GDO0 output pin configuration
    INT8U PKTCTRL1;  // Packet automation control.
    INT8U PKTCTRL0;  // Packet automation control.
    INT8U ADDR;      // Device address.
    INT8U PKTLEN;    // Packet length.
/////////////////////////////////////////////////////////////////////////////////////////
//		INT8U MCSM2;       // ��ͨ�ſ���״̬������*********** 
    INT8U WOREVT1;   // ���ֽ�ʱ��0��ͣ***************** // High uchar Event 0 timeout
    INT8U WOREVT0;   // ���ֽ�ʱ��0��ͣ***************** // Low uchar Event 0 timeout
    INT8U WORCTRL;   // ��Ų��������****************** // Wake On Radio control

} RF_SETTINGS;





//4.8K
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = -
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (41) CHIP_RDY
const RF_SETTINGS rfSettings= {
	0x00,
    0x06,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0xC7,   // MDMCFG4   Modem configuration.
    0x83,   // MDMCFG3   Modem configuration.
//    0x03,   // MDMCFG2   Modem configuration.
	0x01,	// MDMCFG2	 [2:0] SYNC_MODE[2:0] ����ͬ�����޶���ģʽ 0--��ǰ����ͬ�� 1--15/16ͬ�� 2--16/16ͬ�� 3--30/32ͬ��

    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x40,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end TX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x43,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
    0xE9,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x1F,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.

    0x29,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xFF    // PKTLEN    Packet length.
};


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


void halWait(INT16U timeout) {
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
    } while (--timeout);
}


void SpiInit(void)
{
	CSN=0;
	SCK=0;
	CSN=1;
}

/*****************************************************************************************
//��������CpuInit()
//���룺��
//�������
//����������SPI��ʼ������
/*****************************************************************************************/
void CpuInit(void)
{
	SpiInit();
	delay(5000);
	POWER_UP_RESET_CC1100();
	halRfWriteRfSettings();
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
}
	

//*****************************************************************************************
//��������SpisendByte(INT8U dat)
//���룺���͵�����
//�������
//����������SPI����һ���ֽ�
//*****************************************************************************************
INT8U SpiTxRxByte(INT8U dat)
{
	INT8U i,temp;
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

//*****************************************************************************************
//��������void RESET_CC1100(void)
//���룺��
//�������
//������������λCC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
	CSN = 0; 
	while (MISO);
    SpiTxRxByte(CCxxx0_SRES); 		//д�븴λ����
	while (MISO); 
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
//��������void halSpiWriteReg(INT8U addr, INT8U value)
//���룺��ַ��������
//�������
//����������SPIд�Ĵ���
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value) 
{
    CSN = 0;
    while (MISO);
    SpiTxRxByte(addr);		//д��ַ
    SpiTxRxByte(value);		//д������
    CSN = 1;
}

//*****************************************************************************************
//��������void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i, temp;
	temp = addr | WRITE_BURST;
    CSN = 0;
    while (MISO);
    SpiTxRxByte(temp);
    for (i = 0; i < count; i++)
 	{
        SpiTxRxByte(buffer[i]);
    }
    CSN = 1;
}

//*****************************************************************************************
//��������void halSpiStrobe(INT8U strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{
    CSN = 0;
    while (MISO);
    SpiTxRxByte(strobe);		//д������
    CSN = 1;
}





//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
	INT8U temp, value;
    temp = addr|READ_SINGLE;//���Ĵ�������
	CSN = 0;
	while (MISO);
	SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN = 1;
	return value;
}


//*****************************************************************************************
//��������void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i,temp;
	temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
    CSN = 0;
    while (MISO);
	SpiTxRxByte(temp);   
    for (i = 0; i < count; i++) 
	{
        buffer[i] = SpiTxRxByte(0);
    }
    CSN = 1;
}


//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{
    INT8U value,temp;
	temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    CSN = 0;
    while (MISO);
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

/*
	halSpiWriteReg(CCxxx0_MCSM2 ,   rfSettings.MCSM2 );
    halSpiWriteReg(CCxxx0_WOREVT1,  rfSettings.WOREVT1);
    halSpiWriteReg(CCxxx0_WOREVT0,  rfSettings.WOREVT0);
    halSpiWriteReg(CCxxx0_WORCTRL,  rfSettings.WORCTRL);
*/
}

//*****************************************************************************************
//��������void halRfSendPacket(INT8U *txBuffer, INT8U size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************

void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
	
	halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����
	while(GDO2);
    halSpiStrobe(CCxxx0_STX);		//���뷢��ģʽ��������

    // Wait for GDO0 to be set -> sync transmitted
    while (!GDO0);
    // Wait for GDO0 to be cleared -> end of packet
    while (GDO0);
	halSpiStrobe(CCxxx0_SFTX);
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

static INT8U RfGetRxStatus(void)
{
	INT8U temp, spiRxStatus1,spiRxStatus2;
	INT8U i=4;// ѭ�����Դ���
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
INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
	INT8U i=(*length)*4;  // �������Ҫ����datarate��length������

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
/*
void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}*/
