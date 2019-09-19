#include "main.h"
#include "cc1101.h"
#define LENGTH 32
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
INT8U PaTabel[8] = {0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0 ,0xc0};  //0dBm





typedef struct S_RF_SETTINGS
{
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
//    INT8U MCSM1;     //MainRadio Cntrl State Machine
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
	INT8U MCSM2;       // 主通信控制状态机配置*********** 
    INT8U WOREVT1;   // 高字节时间0暂停***************** // High uchar Event 0 timeout
    INT8U WOREVT0;   // 低字节时间0暂停***************** // Low uchar Event 0 timeout
    INT8U WORCTRL;   // 电磁波激活控制****************** // Wake On Radio control

} RF_SETTINGS;


/////////////////////////////////////////////////////////////////////////////////////////
const RF_SETTINGS rfSettings = 
{

    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.

    0x10,   // FREQ2     Frequency control word, high byte.   FOSC/65536*FREQ=433MHZ
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.

    0x58,   // MDMCFG4   Modem configuration.
			//[3:0] DRATE_E[3:0]
    0x43,   // MDMCFG3   Modem configuration.
			//DRATE_M[7:0]       数据速率 = (0X43+256)*2^8/268435456*26M=323*256/268435456*26m=8k
			//DRATE_M[7:0]       数据速率 = (0X43+256)*2^11/268435456*26M=323*256/268435456*26m=64k
    0x03,   // MDMCFG2   Modem configuration.
			//[7]DEM_DCFILT_OFF 0--较高灵敏度
			//[6:4]MOD_FORMAT[2:0] 0--2FSK 1--GFSK
			//[3] MANCHESTER_EN  开启曼彻斯特编码/解码
			//[2:0] SYNC_MODE[2:0] 复合同步字限定符模式 0--无前导与同步 1--15/16同步 2--16/16同步 3--30/32同步
    0x22,   // MDMCFG1   Modem configuration.
			//[7] FEC_EN
			//[6:4] NUM_PREAMBLE[2:0] 设置将要传输的最低前导字节数 0--2 1--3 2--4 3--6 4--8 5--12 6--16 7--24
			//[1:0] CHANSPC_E[1:0] 2位指数信道间隔
    0xF8,   // MDMCFG0   Modem configuration. //信道间隔
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
    0x10,   // FREND0    Front end TX configuration.
//    0x0F,   //MCSM1    
            // [3:0] 1111 接收发送完成后保持在RX状态
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
			//[5:4] FS_AUTOCAL[1:0] 3--4次校准
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
			//[7:5] PQT 同步字质量阀值 0
			//[3] CRC失败时，自动刷新RXFIFO[禁用]CRC过滤
			//[2] 当开启时，2 个状态字节将附加在数据包的有效负载上。状态字节包含RSSI  和 LQI  值，以及 CRC OK标记。
			//[1:0] 无地址校验	
  //  0x4b,   // PKTCTRL1  Packet automation control  ,,,,wor
    0x05,   // PKTCTRL0  Packet automation control.
			//[2]CRC校验控制 1--启用
			//[1:0]数据包长度模式 0--固定  1--可变长  2--无限制
    0x00,   // ADDR      Device address.
    LENGTH,   // PKTLEN    Packet length.

///////////////////////////////////////////////////////////////////////////////////////////
/*
	0x18,  //MCSM2     // 主通信控制状态机配置************
    0x01, // WOREVT1   // 高字节时间0暂停***************** High uchar Event 0 timeout
    0xF1, // WOREVT0   // 低字节时间0暂停***************** Low uchar Event 0 timeout
    0x38, // WORCTRL	 // 电磁波激活控制****************** Wake On Radio control
*/
/*    0x09,  //MCSM2     // 主通信控制状态机配置************ 
			//[4] RX_TIME_RSSI 根据 RSSI 测量（载波监听）执行 RX 终止。就 ASK/OOK 调制而言，如果第一个 8 符号周期内没有载波监听，则 RX 超时。
			//[3] RX_TIME_QUAL RX_TIME  定时器超时的情况下，RX_TIME_QUAL=0 时，芯片检查是否找到同步字，或在 RX_TIME_QUAL=1  时检查是找到了同步字还是设置了 PQI。
			//[2:0] RX_TIME[2:0] 在 WOR 模式和标准 RX 运行时，RX 中的同步字搜索时限。该时限与经编程的事件 0 有关。
			//	0--
    0x02, // WOREVT1   // 高字节时间0暂停***************** High uchar Event 0 timeout
    0x08, // WOREVT0   // 低字节时间0暂停***************** Low uchar Event 0 timeout
    0x38, // WORCTRL	 // 电磁波激活控制****************** Wake On Radio control
			//[7] RC_PD 向 RC 振荡器发送断电模式信号。当写入 0 时，执行自动初始化校准
			//[6:4] EVENT1[2:0] 寄存器模块的超时设置。事件 1 超时解码。
			//[3] RC_CAL 开启（1）或关闭（0）RC 振荡器校准。
			//[1:0] WOR_RES [0]
			//经计算发送一帧最长的数据要33ms
*/
};



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

void halWait(INT16U timeout) 
{
	INT8U i;
    do 
	{
		i=10;
		while(i--);
    } while (--timeout);

}
//专用于语音芯片
unsigned char SPITransferByte(unsigned char byData)
{
    uint8 i;
	uint8 RecData=0;
	for(i=0; i<8; i++)
	{   
		SetSCK(0);
		if(byData&0x01){ SetMOSI(1);}
		else {SetMOSI(0);}
		Delayus(150);
		SetSCK(1);
		Delayus(150);
		if(GetMISO()!=0) RecData |=0x80;
		else RecData &=0x7f;
		byData>>= 1;
		RecData>>=1;
	} 
	return(RecData);
}

unsigned char SPI_SendByte(unsigned char Data)
{
	INT16U x=0;

	SPI->DR = Data;
	while(SPI->SR&SPI_SR_BSY)  {if(x>1000)break;x++;};
	return SPI->DR;
}



void Init_WOR(void)
{
	halSpiStrobe(CCxxx0_SIDLE);
	halSpiWriteReg(CCxxx0_WORCTRL, 0x78); // 
	halSpiWriteReg(CCxxx0_WOREVT1, 0x02);//43B5 500ms, 28A0 300ms 1B15 200ms
	halSpiWriteReg(CCxxx0_WOREVT0, 0xB5);// 876B 1S
	halSpiWriteReg(CCxxx0_MCSM2, 0x1f); // RX_TIME // RX_TIME_RSSI= 1 RX_TIME_QUAL = 0
	// 0:12.5%, 1:6.25%, 2:3.125%, 3:1.563%, 4:0.781%
	//halSpiWriteReg(CCxxx0_MCSM0, 0x38); // 频率校准方式，每4次从IDLE模式到RX模式进行一次频率合成器校准
}
void StartRxWor(void)
{
	halSpiStrobe(CCxxx0_SIDLE);
	halSpiStrobe(CCxxx0_SWORRST);
	halSpiStrobe(CCxxx0_SWOR);
}



/*****************************************************************************************
//函数名：CpuInit()
//输入：无
//输出：无
//功能描述：SPI初始化程序
/*****************************************************************************************/
unsigned char CC1101_Init(void)
{
	SetCSN(1);
	delay(5000);
	POWER_UP_RESET_CC1100();
	halRfWriteRfSettings();
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);

//	halSpiStrobe(CCxxx0_SRX);//进入接收状态
//	halSpiStrobe(CCxxx0_SWOR);//电磁波唤醒状态
	return 0;
}
	

//*****************************************************************************************
//函数名：void RESET_CC1100(void)
//输入：无
//输出：无
//功能描述：复位CC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
	INT16U x=0;
	SetCSN(0); 
	while (GetMISO()){if(x>10000)break;x++;};
  	SPI_SendByte(CCxxx0_SRES); 		//写入复位命令
	x=0;
	while (GetMISO()){if(x>10000)break;x++;}; 
  	SetCSN(1); 
}

//*****************************************************************************************
//函数名：void POWER_UP_RESET_CC1100(void) 
//输入：无
//输出：无
//功能描述：上电复位CC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
	SetCSN(1); 
	halWait(1); 
	SetCSN(0); 
	halWait(1); 
	SetCSN(1); 
	halWait(41); 
	RESET_CC1100();   		//复位CC1100
}

//*****************************************************************************************
//函数名：void halSpiWriteReg(INT8U addr, INT8U value)
//输入：地址和配置字
//输出：无
//功能描述：SPI写寄存器
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value) 
{
	INT16U x=0;

    SetCSN(0);
    while (GetMISO()){if(x>10000)break;x++;};
    SPI_SendByte(addr);		//写地址
    SPI_SendByte(value);		//写入配置
    SetCSN(1);
}

//*****************************************************************************************
//函数名：void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，写入缓冲区，写入个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
	INT16U x=0;
    INT8U i, temp;
	temp = addr | WRITE_BURST;
    SetCSN(0);
    while (GetMISO()){if(x>60000)break;x++;};;
    SPI_SendData(temp);
    for (i = 0; i < count; i++)
	{
		SPI_SendByte(buffer[i]);
    }
    SetCSN(1);
}

//*****************************************************************************************
//函数名：void halSpiStrobe(INT8U strobe)
//输入：命令
//输出：无
//功能描述：SPI写命令
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{
	INT16U x=0;
    SetCSN(0);
    while (GetMISO()){if(x>10000)break;x++;};
    SPI_SendByte(strobe);		//写入命令
    SetCSN(1);
}

//*****************************************************************************************
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该寄存器的配置字
//功能描述：SPI读寄存器
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
	INT16U x=0;
	INT8U temp, value;
  	temp = addr|READ_SINGLE;//读寄存器命令
	SetCSN(0);
	while (GetMISO()){if(x>10000)break;x++;};;
	SPI_SendByte(temp);
	value = SPI_SendByte(0);
	SetCSN(1);
	return value;
}


//*****************************************************************************************
//函数名：void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，读出数据后暂存的缓冲区，读出配置个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
	INT16U x=0;
    INT8U i,temp;
	temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
    SetCSN(0);
    while (GetMISO()){if(x>10000)break;x++;};;
	SPI_SendByte(temp);   
    for (i = 0; i < count; i++) 
	{
        buffer[i] = SPI_SendByte(0);
    }
    SetCSN(1);
}


//*****************************************************************************************
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该状态寄存器当前值
//功能描述：SPI读状态寄存器
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{
	INT16U x=0;
    INT8U value,temp;
	temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
    SetCSN(0);
    while (GetMISO()){if(x>10000)break;x++;};;
    SPI_SendByte(temp);
	value = SPI_SendByte(0);
	SetCSN(1);
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
//    halSpiWriteReg(CCxxx0_MCSM1 ,   rfSettings.MCSM1 );//
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
/*
//	halSpiWriteReg(CCxxx0_MCSM2 ,   rfSettings.MCSM2 );
	halSpiWriteReg(CCxxx0_WOREVT1,  rfSettings.WOREVT1);
    halSpiWriteReg(CCxxx0_WOREVT0,  rfSettings.WOREVT0);
    halSpiWriteReg(CCxxx0_WORCTRL,  rfSettings.WORCTRL);
*/    
}



//*****************************************************************************************
//函数名：void halRfSendPacket(INT8U *txBuffer, INT8U size)
//输入：发送的缓冲区，发送数据个数
//输出：无
//功能描述：CC1100发送一组数据
//*****************************************************************************************

void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
	INT16U x=0;
	halSpiStrobe(CCxxx0_SIDLE);

	halSpiWriteReg(CCxxx0_TXFIFO, size);
  	halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//写入要发送的数据
	
  	halSpiStrobe(CCxxx0_STX);		//进入发送模式发送数据

  // Wait for GDO0 to be set -> sync transmitted
  	while (GetGDO0()==0) {halWait(1);if(x>60000)break;x++;}
  // Wait for GDO0 to be cleared -> end of packet
	x=0;
	while (GetGDO0()){halWait(1);if(x>60000)break;x++;}
	halSpiStrobe(CCxxx0_SFTX);
}

INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
	INT8U i=(*length)*4;  // 具体多少要根据datarate和length来决定
    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
	{
      	packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
      	if (packetLength <= *length) 		//如果所要的有效数据长度小于等于接收到的数据包的长度
		{
        	halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
        	*length = packetLength;				//把接收数据长度的修改为当前数据的长度
        
        	// Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
        	halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位


			if(status[0] >= 128) 
			{
				CC1101SigValue = (status[0] - 256)/2 - 74;
			}
			else 
			{
				CC1101SigValue = (status[0])/2 - 74;
			}


			//CC1101SigValue = status[0];
			UART1_SendData8(status[0]); 
			
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
	else	return 0;
}
