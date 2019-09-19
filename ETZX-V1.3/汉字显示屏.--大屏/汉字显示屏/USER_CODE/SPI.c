/*********************************************************************************************************
MASTER SPI 驱动
*********************************************************************************************************/

/*********************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "spi.h"
#include "util.h"

/********************************************************************************************************/


/*********************************************************************************************************
** Function name:       SPI_Init
** Descriptions:        SPI引脚初始化函数
** input parameters:     无
** output parameters:    无
*********************************************************************************************************/
void SPI_Init(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
	LPC_IOCON->PIO2_4 &= ~0x07;		// 字库片选脚 P2.4
	LPC_IOCON->PIO3_5 &= ~0x07;		// 存储器片选脚 P3.5
	LPC_IOCON->PIO3_4 &= ~0x07;		//MOSI
	LPC_IOCON->PIO1_9 &= ~0x07;		//SCK
	//初始化输出
	LPC_GPIO1->DIR    |= (1<<9);                       	/* 初始化为输出功能     */
	LPC_GPIO2->DIR    |= (1<<4);          /* 初始化为输出功能     */
	LPC_GPIO3->DIR    |= (1<<4)|(1<<5);          /* 初始化为输出功能     */

	LPC_IOCON->PIO2_5 &= ~0x07;		//MISO
	//初始化输入
	LPC_GPIO2->DIR    &= ~(SPI_MISO);
	//置IO为初始状态
	SetSpiCS0();
	SetSpiCS1();
	SetSpiMOSI();
	SetSpiSCK();
}
void Enable_GT23L32S4W(void)
{
	ClrSpiCS0();
}
void Disable_GT23L32S4W(void)
{
	SetSpiCS0();
}
void Enable_25VF020(void)
{
	ClrSpiCS1();
}
void Disable_25VF020(void)
{
	SetSpiCS1();
} 
/***********************************************************************/
//传送一个字节数据给SPI总线
/***********************************************************************/
void SPI_PutByte(INT8U dat)
{
	INT8U i;
	for (i=0;i<8;i++)
	{
		ClrSpiSCK();
		if ((dat&0x80)==0) ClrSpiMOSI();
		else SetSpiMOSI();
		dat<<=1;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		SetSpiSCK();
	}
}
/***********************************************************************/
//传送多字节数据给SPI总线
/***********************************************************************/
void SPI_PutBytes(INT8U * data,INT8U len)
{
	INT8U i,dat;
	while (len--)
	{
		dat = *data++;
		for (i=0;i<8;i++)
		{
			ClrSpiSCK();
			if ((dat&0x80)==0)ClrSpiMOSI();
			else SetSpiMOSI();
			dat<<=1;
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			SetSpiSCK();
		}
	}
}
/***********************************************************************/
//获取一个字节数据从SPI总线
/***********************************************************************/
INT8U SPI_GetByte(void)
{
	INT8U val,i;
	val = 0;
	SetSpiMOSI();
	for (i=0;i<8;i++)
	{
		ClrSpiSCK();
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		val<<=1;
		if (GetSpiMISO()) val |= 1;
		SetSpiSCK();
	}
	return val;
}
/***********************************************************************/
//获取多字节数据从SPI总线
/***********************************************************************/
void SPI_GetBytes(INT8U * buf,INT8U len)
{
	INT8U val,i;
	SetSpiMOSI();
	while (len--)
	{
		val = 0;
		for (i=0;i<8;i++)
		{
			ClrSpiSCK();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			val<<=1;
			if (GetSpiMISO()) val |= 1;
			SetSpiSCK();
		}
		*buf++ = val;
	}
}

//函数参数说明----读取字库数据
//ADD——要读的字模的起始地址
//ReadByteNum——要读取的字模的长度
//DataBuffer——读出的字模存放的地址
void Read_GL23L32S4W(INT32U ADDR,INT8U ReadByteNum,INT8U *DataBuffer)
{
   Enable_GT23L32S4W();                          // select EEPROM
   SPI_PutByte(0x03); //the first is normal Read operate
   SPI_PutByte(ADDR>>16);
   SPI_PutByte(ADDR>>8);
   SPI_PutByte(ADDR);
   SPI_GetBytes(DataBuffer,ReadByteNum);
   Disable_GT23L32S4W();
}
