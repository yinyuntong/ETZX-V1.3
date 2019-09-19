/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SPI.h
*文件描述    		:SPI总线驱动程序头文件
*创建人     		:陈卫国
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
//总线片选信号
sbit SST_CS  = P1^3;											//SST存储器片选信号线
sbit GT23_CS = P1^4;									       	//字库芯片片选信号线
#define   SST_WRSR          0x01								//写状态寄存器
#define   SST_WRITE		    0x02 								//写操作
#define   SST_READ			0x03								//读操作
#define   SST_WRDI			0x04								//写禁止
#define   SST_WREN			0x06								//写允许
#define   SST_SECTOR_EARSE  0x20 								//扇区拆除操作 ,每个扇区为4KB字节空间
#define   SST_EWSR          0x50								//允许写状态寄存器
#define   SST_AAI_S         0xAF 								//SST地址自动增量
#define   SST_AAI_P         0xAD 								//PCT地址自动增量
#define   SST_READ_ID       0x90 								//读写存储芯片ID号
#define   C16BaseAdd  		0x2C9D0;  							//16*16 汉字
#define   C24BaseAdd  		0x68190;  							//24*24 汉字
#define   C32BaseAdd  		0xEDF00;  							//32*32 汉字
#define   A8BaseAdd   		0x1DD780;  							//8*16 ASCII
#define   A12BaseAdd  		0x1DFF00;  							//16*24 ASCII
#define   A16BaseAdd  		0x1E5A50;  							//16*32 ASCII
#ifndef _IN_SPI_
#define _IN_SPI_
void SpiInit(void);
uint8 SpiTransferByte(uint8 byData);
uint8 FmReadByte(uint32 uiAddr);
uint8 Read_Status_Register(void);
void Wait_Busy(void);
void FmWriteByte(uint32 uiAddr, uint8 byData);
void FmWriteBytes(uint32 uiAddr, uint16 uiLen, uint8* pbyData);
void ParameterInit(void);
void Get_GT23_Data(uint8 highbyte,uint8 lowbyte);
void Delay_nus(uint8 xdata i);
void FmWriteConstBytes(uint32 uiAddr, uint16 uiLen, uint8 const* pbyData);
uint8 READ_ID(void);
#endif