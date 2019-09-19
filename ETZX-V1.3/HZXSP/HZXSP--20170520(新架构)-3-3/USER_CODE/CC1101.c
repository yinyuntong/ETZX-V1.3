/*************************************************************************************************/

/************************************************************************************************/


#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "cc1101.h"
#include "uart.h"
#include "SingleBus.h"
#include "E2PROM.h"
#include "led.h"
#include "spi.h"
#include "string.h"

extern void InsertNurse(INT8U NO);
extern void DeleteNurse(INT8U NO);


INT8U RecFrameID=0;
INT8U ScreenNum=0;
//INT8U DispColor=0;

#define LENGTH 32
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
INT8U PaTabel[8] = {0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0};  //0dBm

//ģ��SPI�ڶ���
#define GDO0 (1<<1) //P1.1	   RF_TS
#define GDO2 (1<<2)	//P1.2	   RF_IRQ
#define MISO (1<<0)	//P1.0	  RF_MISO
#define MOSI (1<<11) //P2.11  RF_MOSI
#define SCK	 (1<<11) //P0.11  RF_SCLK
#define CSN	(1<<0) //P3.0	  RF_CS
//ģ��SPI�ڲ�������
#define GetGD0() (LPC_GPIO1->DATA&GDO0)
#define GetGD2() (LPC_GPIO1->DATA&GDO2)
#define GetMISO() (LPC_GPIO1->DATA&MISO)
#define SetMOSI() (LPC_GPIO2->DATA|=MOSI)
#define ClrMOSI() (LPC_GPIO2->DATA&=~MOSI)
#define SetSCK() (LPC_GPIO0->DATA|=SCK)
#define ClrSCK() (LPC_GPIO0->DATA&=~SCK)
#define SetCSN() (LPC_GPIO3->DATA|=CSN)
#define ClrCSN() (LPC_GPIO3->DATA&=~CSN)

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
//	INT8U MCSM2;       // ��ͨ�ſ���״̬������*********** 
    INT8U WOREVT1;   // ���ֽ�ʱ��0��ͣ***************** // High uchar Event 0 timeout
    INT8U WOREVT0;   // ���ֽ�ʱ��0��ͣ***************** // Low uchar Event 0 timeout
    INT8U WORCTRL;   // ��Ų��������****************** // Wake On Radio control

} RF_SETTINGS;

/////////////////////////////////////////////////////////////////////////////////////////
const RF_SETTINGS rfSettings = 
{
	0x00,   //FSCTRL2
        0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.

    0x10,   // FREQ2     Frequency control word, high byte.   FOSC/65536*FREQ=433MHZ
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.

    0x58,   // MDMCFG4   Modem configuration.
			//[3:0] DRATE_E[3:0]
    0x43,   // MDMCFG3   Modem configuration.
			//DRATE_M[7:0]       �������� = (0X43+256)*2^8/268435456*26M=323*256/268435456*26m=8k
			//DRATE_M[7:0]       �������� = (0X43+256)*2^11/268435456*26M=323*256/268435456*26m=64k
    0x03,   // MDMCFG2   Modem configuration.
			//[7]DEM_DCFILT_OFF 0--�ϸ�������
			//[6:4]MOD_FORMAT[2:0] 0--2FSK 1--GFSK
			//[3] MANCHESTER_EN  ��������˹�ر���/����
			//[2:0] SYNC_MODE[2:0] ����ͬ�����޶���ģʽ 0--��ǰ����ͬ�� 1--15/16ͬ�� 2--16/16ͬ�� 3--30/32ͬ��
    0x22,   // MDMCFG1   Modem configuration.
			//[7] FEC_EN
			//[6:4] NUM_PREAMBLE[2:0] ���ý�Ҫ��������ǰ���ֽ��� 0--2 1--3 2--4 3--6 4--8 5--12 6--16 7--24
			//[1:0] CHANSPC_E[1:0] 2λָ���ŵ����
    0xF8,   // MDMCFG0   Modem configuration. //�ŵ����
			//[7:0] CHANSPC_M[7:0]
/*
	//
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
			//[5:4] FS_AUTOCAL[1:0] 3--4��У׼
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
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

   0x04,   // PKTCTRL1  Packet automation control.
			//[7:5] PQT ͬ����������ֵ 0
			//[3] CRCʧ��ʱ���Զ�ˢ��RXFIFO[����]
			//[2] ������ʱ��2 ��״̬�ֽڽ����������ݰ�����Ч�����ϡ�״̬�ֽڰ���RSSI  �� LQI  ֵ���Լ� CRC OK��ǡ�
			//[1:0] �޵�ַУ��	
  //  0x4b,   // PKTCTRL1  Packet automation control  ,,,,wor
    0x05,   // PKTCTRL0  Packet automation control.
			//[2]CRCУ����� 1--����
			//[1:0]���ݰ�����ģʽ 0--�̶�  1--�ɱ䳤  2--������
    0x00,   // ADDR      Device address.
    LENGTH,   // PKTLEN    Packet length.

///////////////////////////////////////////////////////////////////////////////////////////
/*
	0x18,  //MCSM2     // ��ͨ�ſ���״̬������************
    0x01, // WOREVT1   // ���ֽ�ʱ��0��ͣ***************** High uchar Event 0 timeout
    0xF1, // WOREVT0   // ���ֽ�ʱ��0��ͣ***************** Low uchar Event 0 timeout
    0x38, // WORCTRL	 // ��Ų��������****************** Wake On Radio control
*/
/*    0x09,  //MCSM2     // ��ͨ�ſ���״̬������************ 
			//[4] RX_TIME_RSSI ���� RSSI �������ز�������ִ�� RX ��ֹ���� ASK/OOK ���ƶ��ԣ������һ�� 8 ����������û���ز��������� RX ��ʱ��
			//[3] RX_TIME_QUAL RX_TIME  ��ʱ����ʱ������£�RX_TIME_QUAL=0 ʱ��оƬ����Ƿ��ҵ�ͬ���֣����� RX_TIME_QUAL=1  ʱ������ҵ���ͬ���ֻ��������� PQI��
			//[2:0] RX_TIME[2:0] �� WOR ģʽ�ͱ�׼ RX ����ʱ��RX �е�ͬ��������ʱ�ޡ���ʱ���뾭��̵��¼� 0 �йء�
			//	0--
    0x02, // WOREVT1   // ���ֽ�ʱ��0��ͣ***************** High uchar Event 0 timeout
    0x08, // WOREVT0   // ���ֽ�ʱ��0��ͣ***************** Low uchar Event 0 timeout
    0x38, // WORCTRL	 // ��Ų��������****************** Wake On Radio control
			//[7] RC_PD �� RC �������Ͷϵ�ģʽ�źš���д�� 0 ʱ��ִ���Զ���ʼ��У׼
			//[6:4] EVENT1[2:0] �Ĵ���ģ��ĳ�ʱ���á��¼� 1 ��ʱ���롣
			//[3] RC_CAL ������1����رգ�0��RC ����У׼��
			//[1:0] WOR_RES [0]
			//�����㷢��һ֡�������Ҫ33ms
*/
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
	INT8U i;
    do {
		i=10;
		while(i--);
    } while (--timeout);
}



void cc1101Pin_init(void)

{
	LPC_IOCON->PIO3_0&=~0X07;		//IO��
	LPC_GPIO3->DIR	|= CSN;
	SetCSN();

	LPC_IOCON->R_PIO1_1	|=	0X01;		//IO��
	LPC_GPIO1->DIR	&= ~GDO0;			//����


	LPC_IOCON->R_PIO1_0	|=	0X01;		//IO��
	LPC_GPIO1->DIR	&= ~MISO;			//����


	LPC_IOCON->R_PIO0_11	|=0X01;		//IO��
	LPC_GPIO0->DIR	|= SCK;
	SetSCK();

	
	LPC_IOCON->PIO2_11&=~0X07;		//IO��
	LPC_GPIO2->DIR	|= MOSI;
	SetMOSI();


	LPC_IOCON->R_PIO1_2	|=	0X01;		//IO��
	LPC_GPIO1->DIR	&= ~cc1101RecPin;

	LPC_GPIO1->IS &= ~cc1101RecPin; //�����ж�
//	LPC_GPIO1->IEV |= cc1101RecPin; //�½����ж�
	LPC_GPIO1->IEV &= ~cc1101RecPin; //�������ж�

#if 0
	LPC_GPIO1->IE |= cc1101RecPin; //�����ж�
	NVIC_EnableIRQ(EINT1_IRQn);                                    /* �����жϲ�ʹ��               */
	NVIC_SetPriority(EINT1_IRQn, 2);
#endif
}


INT8U bWireLessRec=0;	
INT8U byWireLessRecLen=0;	//���ݳ���
INT8U byWireLessRecBuff[LENGTH];
INT16S DataLenTotal=0;			//  1�ι㲥��Ϣ���ܳ���
INT8U RecedDataLen=0;				//�ѽ��յ����ݳ���
INT8U byRecError=0;				//���ճ���
INT8U byRecStarted=0;			//�����˵�һ֡
/**********************************************************
*��������			:PIOINT1_IRQHandler	
*��������        	:cc1101���ڵĶ˿�1�ж�
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:
*��������	 		:
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
/*void PIOINT1_IRQHandler(void)
{
	LPC_GPIO1->IC |= cc1101RecPin;		//��λ�жϱ�־
#if 0
	uartSendByte(LPC_GPIO1->RIS>>24);
	uartSendByte(LPC_GPIO1->RIS>>16);
	uartSendByte(LPC_GPIO1->RIS>>8);
	uartSendByte(LPC_GPIO1->RIS);
#endif	
	bWireLessRec=1;	
	NVIC_DisableIRQ(EINT1_IRQn); 								 // ��ֹ�ж�			   

}*/


void Init_WOR(void)
{
#if 1
	halSpiStrobe(CCxxx0_SIDLE);
	halSpiWriteReg(CCxxx0_WORCTRL, 0x78); // 
	halSpiWriteReg(CCxxx0_WOREVT1, 0x02);//43B5 500ms, 28A0 300ms 1B15 200ms
	halSpiWriteReg(CCxxx0_WOREVT0, 0xB5);// 876B 1S
	halSpiWriteReg(CCxxx0_MCSM2, 0x1f); // RX_TIME // RX_TIME_RSSI= 1 RX_TIME_QUAL = 0
	// 0:12.5%, 1:6.25%, 2:3.125%, 3:1.563%, 4:0.781%
	//halSpiWriteReg(CCxxx0_MCSM0, 0x38); // Ƶ��У׼��ʽ��ÿ4�δ�IDLEģʽ��RXģʽ����һ��Ƶ�ʺϳ���У׼
#else
	halSpiStrobe(CCxxx0_SIDLE);
	/* PQT���� 0--һֱ���� */
	reg = halSpiReadReg(CCxxx0_PKTCTRL1);
	reg |= 0x20;
	halSpiWriteReg(CCxxx0_PKTCTRL1, reg);

	//Change the values of WOREVT1 and WOREVT0 to adjust the WOR interval
	//FFFA = 1884ms
	//43B5 500ms, 28A0 300ms 1B15 200ms  2b5 20ms
	// 876B 1S
	halSpiWriteReg(CCxxx0_WOREVT1, 0x0);
	halSpiWriteReg(CCxxx0_WOREVT0, 0x35);
	/* choose EVENT1[WORCTRL.6-4] == 7=0b111 */
	reg = halSpiReadReg(CCxxx0_WORCTRL);
	reg |= 0x70;
	halSpiWriteReg(CCxxx0_WORCTRL, reg);
	/* ena����e frequence calibration RC_CAL[WORCTRL.3] ==
	reg = halSpiReadReg(CCxxx0_WORCTRL);
	reg |= 0x80;
	halSpiWriteReg(CCxxx0_WORCTRL, reg);
	/* choose WOR_RES[WORCTRL.1-0] == 00 */
	reg = halSpiReadReg(CCxxx0_WORCTRL);
	reg &= ~0x03;
	halSpiWriteReg(CCxxx0_WORCTRL, reg);
	/* choose when to calibrate FS_AUTOCAL[MCSM0.5-4] == 01 */
	reg = halSpiReadReg(CCxxx0_MCSM0);
	reg &= ~0x20;
	reg |= 0x10;
	halSpiWriteReg(CCxxx0_MCSM0, reg);	/* 
	/* initial filter by rssi RX_TIME_RSSI[MCSM2.4] == 0 */
	reg = halSpiReadReg(CCxxx0_MCSM2);
	reg &= ~0x10;
	halSpiWriteReg(CCxxx0_MCSM2, reg);
	/* stay in Rx after pream����e RX_TIME_QUAL[MCSM2.3] == 0 */
	reg = halSpiReadReg(CCxxx0_MCSM2);
	reg &= ~0x08;
	halSpiWriteReg(CCxxx0_MCSM2, reg);
	/* 0:12.5%, 1:6.25%, 2:3.125%, 3:1.563%, 4:0.781% 0 */
	reg = halSpiReadReg(CCxxx0_MCSM2);
	//reg &=0xf8;//&= ~0x00;
	//reg |=0x08;//&= ~0x00;
	reg=0x1f;
	//SendData(reg);
	halSpiWriteReg(CCxxx0_MCSM2, reg);
	/* ena����e RC oscilator RC_PD[WORCTRL.7] == 0 */
	reg = halSpiReadReg(CCxxx0_WORCTRL);
	reg &= ~0x80;
	halSpiWriteReg(CCxxx0_WORCTRL, reg);
#endif	
}
void StartRxWor(void)
{
	halSpiStrobe(CCxxx0_SIDLE);
	halSpiStrobe(CCxxx0_SWORRST);
	halSpiStrobe(CCxxx0_SWOR);
}

//*****************************************************************************************
//��������CC1101_Init()
//���룺��
//�������
//����������SPI��ʼ������
//*****************************************************************************************/
void CC1101_Init(void)
{
//	ClrCSN();
//	ClrSCK();
//	SetCSN();
	delay(5000);
	POWER_UP_RESET_CC1100();
	halRfWriteRfSettings();
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
	halSpiStrobe(CCxxx0_SRX );//�������״̬
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
	
	ClrSCK();
	for(i=0; i<8; i++)
	{
		if(dat & 0x80)
		{
			SetMOSI();
		}
		else ClrMOSI();
		dat <<= 1;

		SetSCK(); 
		__NOP();
		__NOP();

		temp <<= 1;
		if(GetMISO())temp++; 
		ClrSCK();
		__NOP();
		__NOP();	
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
{   INT16U x=0;
	ClrCSN(); 
	while (GetMISO()){if(x>10000)break;x++;};
    SpiTxRxByte(CCxxx0_SRES); 		//д�븴λ����
	x=0;
	while (GetMISO()){if(x>10000)break;x++;}; 
    SetCSN(); 
}

//*****************************************************************************************
//��������void POWER_UP_RESET_CC1100(void) 
//���룺��
//�������
//�����������ϵ縴λCC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
	SetCSN(); 
	halWait(1); 
	ClrCSN(); 
	halWait(1); 
	SetCSN(); 
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
{   INT16U x=0;
    ClrCSN();
    while (GetMISO()){if(x>10000)break;x++;};
    SpiTxRxByte(addr);		//д��ַ
    SpiTxRxByte(value);		//д������
    SetCSN();
}

//*****************************************************************************************
//��������void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{   INT16U x=0;
    INT8U i, temp;
	temp = addr | WRITE_BURST;
    ClrCSN();
    while (GetMISO()){if(x>10000)break;x++;};;
    SpiTxRxByte(temp);
    for (i = 0; i < count; i++)
 	{
        SpiTxRxByte(buffer[i]);
    }
    SetCSN();
}

//*****************************************************************************************
//��������void halSpiStrobe(INT8U strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{   INT16U x=0;
    ClrCSN();
    while (GetMISO()){if(x>10000)break;x++;};
    SpiTxRxByte(strobe);		//д������
    SetCSN();
}

//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{   INT16U x=0;
	INT8U temp, value;
    temp = addr|READ_SINGLE;//���Ĵ�������
	ClrCSN();
	while (GetMISO()){if(x>10000)break;x++;};;
	SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	SetCSN();
	return value;
}


//*****************************************************************************************
//��������void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{   INT16U x=0;
    INT8U i,temp;
	temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
    ClrCSN();
    while (GetMISO()){if(x>10000)break;x++;};;
	SpiTxRxByte(temp);   
    for (i = 0; i < count; i++) 
	{
        buffer[i] = SpiTxRxByte(0);
    }
    SetCSN();
}


//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{   INT16U x=0;
    INT8U value,temp;
	temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    ClrCSN();
    while (GetMISO()){if(x>10000)break;x++;};;
    SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	SetCSN();
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
//��������void halRfSendPacket(INT8U *txBuffer, INT8U size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************

void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{   INT16U x=0;
	halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����

    halSpiStrobe(CCxxx0_STX);		//���뷢��ģʽ��������

    // Wait for GDO0 to be set -> sync transmitted
    while (!GetGD0()){if(x>10000)break;x++;};;
    // Wait for GDO0 to be cleared -> end of packet
	x=0;
    while (GetGD0()){if(x>10000)break;x++;};;
	halSpiStrobe(CCxxx0_SFTX);
}

INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
//	INT8U i=(*length)*4;  // �������Ҫ����datarate��length������
	 
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



void cc1101Hander(void)
{
	INT8U i;
	STBusFreq *pstBusDealFreq ;
	INT8U  *pbyData;
	pSTC1101Freq pstC1101DealFreq;	
	if(bWireLessRec)
	{
		bWireLessRec=0;
		byWireLessRecLen=32;	//Ҫ��ȡ���ݵ���󳤶�
		if(halRfReceivePacket(byWireLessRecBuff,&byWireLessRecLen)==CRC_OK)
		{

			//������
		/*	uartSendByte(byWireLessRecLen);
			for(i=0;i<byWireLessRecLen;i++)
			{
				uartSendByte(byWireLessRecBuff[i]);
			}*/

			
			if(byWireLessRecLen< 9)  goto cc1101HanderRet;		//һ֡��������

			
		   pstBusDealFreq = (pSTBusFreq)(byWireLessRecBuff);

			if(SystemInfo.LOCAL_ADDR == pstBusDealFreq->bySndSecAddr)
			{//�Ǳ�����Ϣ
				if(byWireLessRecLen ==9) 
				{
					if(pstBusDealFreq->byCmd != CMD_DISPLAY_DATA) goto cc1101HanderRet;		//���ǽ��չ����Ϣ����

					if((pstBusDealFreq->bySndRoomAddr==1) || (pstBusDealFreq->bySndRoomAddr== RecFrameID+1)) goto message_treat;
					//����֡9���ֽڵĹ����Ϣ
				}

				else if(byWireLessRecLen ==10)
				{
					switch(pstBusDealFreq->byCmd)
					{
					case CMD_ENTER: 									//�յ�ȷ������
						SystemInfo.state |= SYSTEM_STATE_ONLINE; //��ϵͳ����״̬			
						break; 
					case CMD_QUEST: 									//��ѯ����
						pstBusDealFreq->bySndSecAddr = SystemInfo.LOCAL_ADDR;
						pstBusDealFreq->bySndRoomAddr = 0;
						pstBusDealFreq->bySndBedAddr = 0;
						pstBusDealFreq->byCmd = CMD_ANSWER;			
						halRfSendPacket(&(pstBusDealFreq->bySndSecAddr),7);
						break;
					//------------------------------------------------------
					case CMD_DATA_SEND: 								//�յ�Уʱ����
						//SystemInfo.sTime.second = BcdToHex(pstBusDealFreq->bySndSecAddr & 0x7f);
						SystemInfo.sTime.minute = BcdToHex(pstBusDealFreq->bySndRoomAddr & 0x7f);
						SystemInfo.sTime.hour	= BcdToHex(pstBusDealFreq->bySndBedAddr & 0x3f);
						SystemInfo.sTime.day	= BcdToHex(pstBusDealFreq->byRecSecAddr & 0x3f);
						SystemInfo.sTime.month	= BcdToHex(pstBusDealFreq->byRecRoomAddr & 0x1f);
						SystemInfo.sTime.year	= BcdToHex(pstBusDealFreq->byRecBedAddr); 		
						break;
					case CMD_INFO_INDICATION:							//��Ϣָʾ����		
						SystemInfo.DisplayInfo.CallBuf[0] =  pstBusDealFreq->byRecSecAddr&0x1f;  //��������
						SystemInfo.DisplayInfo.CallBuf[1] =  pstBusDealFreq->bySndSecAddr;		//����
						//SystemInfo.LOCAL_ADDR = pstBusDealFreq->bySndSecAddr; //�洢Ϊ��������
						SystemInfo.DisplayInfo.CallBuf[2] =  pstBusDealFreq->bySndRoomAddr;		//����
						SystemInfo.DisplayInfo.CallBuf[3] =  pstBusDealFreq->bySndBedAddr;		//����
						SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CALL;
						SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
						break;
					case CMD_NURSE_COME:		//��ʿ��λ����
						InsertNurse(pstBusDealFreq->byRecRoomAddr);
						break;	
					case CMD_NURSE_BACK:	   //��ʿ�뿪����
						DeleteNurse(pstBusDealFreq->byRecRoomAddr);
						break;
					case CMD_INFUSION_ANSWER:
					case CMD_SERVICE_ANSWER:
					case CMD_EMERGENCY_ANSWER:
					case CMD_HELP_ANSWER:
					case CMD_INFUSION_CLEAR:
					case CMD_SERVICE_CLEAR:
					case CMD_EMERGENCY_CLEAR:
					case CMD_HELP_CLEAR:								//������л����������ָ��
						//�Ƚ�����ָʾ�ĵ�ַ�봦��ĵ�ַ
						if((pstBusDealFreq->byRecSecAddr != 0xff) && (pstBusDealFreq->byRecSecAddr != SystemInfo.DisplayInfo.CallBuf[1]))
						{	//����ַ����ȷ
							break;
						}
						if((pstBusDealFreq->byRecRoomAddr != 0xff) && (pstBusDealFreq->byRecRoomAddr != SystemInfo.DisplayInfo.CallBuf[2]))
						{	//����ַ����ȷ
							break;
						}
						if((pstBusDealFreq->byRecBedAddr != 0xff) && (pstBusDealFreq->byRecBedAddr != SystemInfo.DisplayInfo.CallBuf[3]))
						{	//����ַ����ȷ
							break;
						} 
					case CMD_STOP_INDICATION:
						SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CALL;
						SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
						break;
					case CMD_POWER_ON:	  								//��������������
						//if(AddrCompare(&stBusDealFreq))
						//{	//���͵�������������� 				
	//						while(1);			
						//}
						break;	
					case CMD_CLEAR_LCD:
					   	memset((void*)SystemInfo.DisplayInfo.MessageBuf,0,MESG_BUF_LEN);	
						SaveParameter(SYS_INFO_ADD);
						SaveParameter(SYS_INFO_ADD_BAK);
						SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
						SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
						break;
						
					}
					SystemInfo.state|=SYSTEM_STATE_ONLINE; //��ʶ����״̬						
				}
				else if(pstBusDealFreq->byCmd == CMD_DISPLAY_DATA)
				{//�ǹ����Ϣ
message_treat:				
					pstC1101DealFreq = (pSTC1101Freq)(byWireLessRecBuff);
					pbyData = pstC1101DealFreq->byData;
					
					if(pstC1101DealFreq->byFrameID==0x01)
					{//��1֡
						DataLenTotal = *pbyData;		//��ȡ�����ݳ���
						DataLenTotal -= (pstC1101DealFreq->byLen-1);	//��һ���������а������ܳ��ȡ����š���ʾ��ɫ
						if(DataLenTotal<0) {byRecError =1;goto RecError;}//return;	


						ScreenNum = *(pbyData+1);
						SystemInfo.DisplayInfo.color = *(pbyData+2);
						RecFrameID= 0x01;
						RecedDataLen = pstC1101DealFreq->byLen-3;	//ʵ����Ч���ݵĳ���
						memset((void*)SystemInfo.DisplayInfo.MessageBuf,0,MESG_BUF_LEN);
						memcpy((void*)SystemInfo.DisplayInfo.MessageBuf,(void*)(pbyData+3),RecedDataLen);

						if(DataLenTotal==0)
						{//���ݽ������
 
							 SaveParameter(SYS_INFO_ADD);
							 SaveParameter(SYS_INFO_ADD_BAK);

							if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
							else
							{
								SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
								SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
								SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING;
								SystemInfo.DisplayInfo.ChangeDelay=MESSAGE_CHANGED_DELAY;  /*31;*/
							}
							
							RecFrameID =0;							
						}						
					}
					else if(RecFrameID+1 == pstC1101DealFreq->byFrameID)
					{//����֡������

						DataLenTotal -= pstC1101DealFreq->byLen;

						if(DataLenTotal<0) {byRecError=1;goto RecError;}
						
						if((RecedDataLen +pstC1101DealFreq->byLen)>=MESG_BUF_LEN) {byRecError=1;goto RecError;}

						RecFrameID = pstC1101DealFreq->byFrameID;
						memcpy((void*)(SystemInfo.DisplayInfo.MessageBuf+RecedDataLen),(void*)(pbyData),pstC1101DealFreq->byLen);
						RecedDataLen += pstC1101DealFreq->byLen;
						if(DataLenTotal==0)
						{//���ݽ������
							SaveParameter(SYS_INFO_ADD);
							SaveParameter(SYS_INFO_ADD_BAK);

							if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
							else
							{
								SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
								SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
								SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING;
								SystemInfo.DisplayInfo.ChangeDelay=MESSAGE_CHANGED_DELAY; /*31;*/
							}
							
							RecFrameID =0;							
						}						
				
					}
					
					if(byRecError==1)
					{
RecError:
						byRecError =0;
						DataLenTotal=0;
						RecedDataLen =0;
						byRecStarted=0;
						RecFrameID =0;
						memset((void*)SystemInfo.DisplayInfo.MessageBuf,0,MESG_BUF_LEN);
					}
										
				}
			}

		}	
cc1101HanderRet:
		halSpiStrobe(CCxxx0_SRX);	//�������
		LPC_GPIO1->IC |= cc1101RecPin;		//��λ�жϱ�־
		LPC_GPIO1->IE |=  cc1101RecPin;		// ��ֹ�ж�

	}

}

