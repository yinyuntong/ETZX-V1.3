/*********************************************************************************************************
MASTER SPI ����
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
** Descriptions:        SPI���ų�ʼ������
** input parameters:     ��
** output parameters:    ��
*********************************************************************************************************/
void SPI_Init(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��           */
	LPC_IOCON->PIO2_4 &= ~0x07;		// �ֿ�Ƭѡ�� P2.4
	LPC_IOCON->PIO3_5 &= ~0x07;		// �洢��Ƭѡ�� P3.5
	LPC_IOCON->PIO3_4 &= ~0x07;		//MOSI
	LPC_IOCON->PIO1_9 &= ~0x07;		//SCK
	//��ʼ�����
	LPC_GPIO1->DIR    |= (1<<9);                       	/* ��ʼ��Ϊ�������     */
	LPC_GPIO2->DIR    |= (1<<4);          /* ��ʼ��Ϊ�������     */
	LPC_GPIO3->DIR    |= (1<<4)|(1<<5);          /* ��ʼ��Ϊ�������     */

	LPC_IOCON->PIO2_5 &= ~0x07;		//MISO
	//��ʼ������
	LPC_GPIO2->DIR    &= ~(SPI_MISO);
	//��IOΪ��ʼ״̬
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
//����һ���ֽ����ݸ�SPI����
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
//���Ͷ��ֽ����ݸ�SPI����
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
//��ȡһ���ֽ����ݴ�SPI����
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
//��ȡ���ֽ����ݴ�SPI����
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

//��������˵��----��ȡ�ֿ�����
//ADD����Ҫ������ģ����ʼ��ַ
//ReadByteNum����Ҫ��ȡ����ģ�ĳ���
//DataBuffer������������ģ��ŵĵ�ַ
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
