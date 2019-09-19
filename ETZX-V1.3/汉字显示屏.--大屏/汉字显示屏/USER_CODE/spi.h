#ifndef _SPI_H_
#define _SPI_H_

//SPI�ӿڶ���
#define SPI_CS0	(1<<4)   // �ֿ�Ƭѡ�� P2.4
#define SPI_CS1	(1<<5)   // �洢��Ƭѡ�� P3.5
#define SPI_MISO (1<<5)	 //P2.5
#define SPI_MOSI (1<<4)	 //P3.4
#define SPI_SCK	(1<<9)   //P1.9
//SPI�ӿڲ�������
#define SetSpiCS0() (LPC_GPIO2->DATA|=SPI_CS0)
#define ClrSpiCS0() (LPC_GPIO2->DATA&=~SPI_CS0)
#define SetSpiCS1() (LPC_GPIO3->DATA|=SPI_CS1)
#define ClrSpiCS1() (LPC_GPIO3->DATA&=~SPI_CS1)
#define SetSpiMOSI() (LPC_GPIO3->DATA|=SPI_MOSI)
#define ClrSpiMOSI() (LPC_GPIO3->DATA&=~SPI_MOSI)
#define SetSpiSCK() (LPC_GPIO1->DATA|=SPI_SCK)
#define ClrSpiSCK() (LPC_GPIO1->DATA&=~SPI_SCK)
#define GetSpiMISO() (LPC_GPIO2->DATA&SPI_MISO)

//SPI������������
extern void SPI_Init(void);
extern void Enable_GT23L32S4W(void);
extern void Disable_GT23L32S4W(void);
extern void Enable_25VF020(void);
extern void Disable_25VF020(void);
extern void SPI_PutByte(INT8U data);
extern void SPI_PutBytes(INT8U * data,INT8U len);
extern INT8U SPI_GetByte(void);
extern void SPI_GetBytes(INT8U * buf,INT8U len);
extern void Read_GL23L32S4W(INT32U ADD,INT8U ReadByteNum,INT8U *DataBuffer);

#endif


