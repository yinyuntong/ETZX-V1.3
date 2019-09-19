/*********************************************************************************************************
MASTER SPI 驱动
*********************************************************************************************************/

/*********************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "spi.h"
#include "util.h"
#include "led.h"
uint8_t manID,devID;

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
//	ClrSpiCS1();
}
void Disable_GT23L32S4W(void)
{
	SetSpiCS0();
//	SetSpiCS1();
}
void Enable_Flash(void)
{
	ClrSpiCS0();
//	ClrSpiCS1();
}
void Disable_Flash(void)
{
	SetSpiCS0();
//	SetSpiCS1();
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


/************************************************************************/
/* PROCEDURE: Chip_Erase												*/
/*																		*/
/* This procedure erases the entire Chip.								*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Chip_Erase()
{	
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x60);			/* send Chip Erase command (60h or C7h) */
	Disable_Flash();					/* disable device */
}

/************************************************************************/
/* PROCEDURE: Sector_Erase												*/
/*																		*/
/* This procedure Sector Erases the Chip.								*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Sector_Erase(unsigned long Dst)
{


	Enable_Flash();					/* enable device */
	SPI_PutByte(0x20);			/* send Sector Erase command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	Disable_Flash();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: Block_Erase_32K											*/
/*																		*/
/* This procedure Block Erases 32 KByte of the Chip.					*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Block_Erase_32K(unsigned long Dst)
{
	Enable_Flash();					/* enable device */
	SPI_PutByte(0x52);			/* send 32 KByte Block Erase command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	Disable_Flash();					/* disable device */
}	


/************************************************************************/
/* PROCEDURE: Block_Erase_64K											*/
/*																		*/
/* This procedure Block Erases 64 KByte of the Chip.					*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Block_Erase_64K(unsigned long Dst)
{
	Enable_Flash();					/* enable device */
	SPI_PutByte(0xD8);			/* send 64KByte Block Erase command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	Disable_Flash();					/* disable device */
}

/************************************************************************/
/* PROCEDURE: Read_Status_Register										*/
/*																		*/
/* This procedure read the status register and returns the byte.		*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/************************************************************************/
unsigned char Read_Status_Register()
{
	unsigned char byte = 0;
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x05);		/* send RDSR command */
	byte = SPI_GetByte();		/* receive byte */
	Disable_Flash();				/* disable device */
	return byte;
}


/************************************************************************/
/* PROCEDURE: Wait_Busy													*/
/*																		*/
/* This procedure waits until device is no longer busy (can be used by	*/
/* Byte-Program, Sector-Erase, Block-Erase, Chip-Erase).				*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Wait_Busy()
{
	INT16U i;
	i = 30000;

	while ((Read_Status_Register()&0x01) == 0x01)	/* waste time until not busy */
	{
		Delay_Us(10);
		if(--i ==0) break;		//等待最长时间300ms		
	}

}



/************************************************************************/
/* PROCEDURE: WREN														*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WREN()
{
	Enable_Flash();	/* enable device */
	SPI_PutByte(0x06);		/* send WREN command */
	Disable_Flash();	/* disable device */
	Wait_Busy();
}


/************************************************************************/
/* PROCEDURE: WRDI														*/
/*																		*/
/* This procedure disables the Write Enable Latch.						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WRDI()
{
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x04);		/* send WRDI command */
	Disable_Flash();				/* disable device */
}
/************************************************************************/
/* PROCEDURE: EBSY														*/
/*																		*/
/* This procedure enable SO to output RY/BY# status during AAI 			*/
/* programming.															*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void EBSY()
{
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x70);		/* send EBSY command */
	Disable_Flash();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: DBSY														*/
/*																		*/
/* This procedure disable SO as output RY/BY# status signal during AAI	*/
/* programming.															*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void DBSY()
{
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x80);		/* send DBSY command */
	Disable_Flash();				/* disable device */
}	

/************************************************************************/
/* PROCEDURE: EWSR														*/
/*																		*/
/* This procedure Enables Write Status Register.  						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void EWSR()
{
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x50);		/* enable writing to the status register */
	Disable_Flash();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: WRSR														*/
/*																		*/
/* This procedure writes a byte to the Status Register.					*/
/*																		*/
/* Input:																*/
/*		byte															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WRSR(unsigned char byte)
{
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x01);		/* select write to status register */
	SPI_PutByte(byte);		/* data that will change the status of BPx 
							   or BPL (only bits 2,3,4,5,7 can be written) */
	Disable_Flash();				/* disable the device */
}



void W25QXX_WRSR(unsigned int status)
{
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x01);		/* select write to status register */
	SPI_PutByte(status);		// data that will change the status of BPx 
	SPI_PutByte(status>>8);		// or BPL (only bits 2,3,4,5,7 can be written) */
	Disable_Flash();		   /* disable the device */
}

/************************************************************************/
/* PROCEDURE: Poll_SO													*/
/*																		*/
/* This procedure polls for the SO line during AAI programming			*/
/* waiting for SO to transition to 1 which will indicate AAI programming*/
/* is completed															*/
/*																		*/
/* Input:																*/
/*		SO																*/
/*																		*/
/* Output:																*/
/*		None															*/
/************************************************************************/
void Poll_SO()
{
	uint16_t i =0xffff;
	
	unsigned char temp = 0;
	Enable_Flash();
	while (temp == 0x00)	/* waste time until not busy */
	{
		if (GetSpiMISO()) temp=1;
		else temp = 0;
		Delay_Us(100);
		if( --i == 0x00) break;
	}
	Disable_Flash();
}

/************************************************************************/
/* PROCEDURE: Read_ID													*/
/*																		*/
/* This procedure Reads the manufacturer's ID and device ID.  It will 	*/
/* use 90h or ABh as the command to read the ID (90h in this sample).   */
/* It is up to the user to give the last byte ID_addr to determine      */
/* whether the device outputs manufacturer's ID first, or device ID 	*/
/* first.  Please see the product datasheet for details.  Returns ID in */
/* variable byte.														*/
/*																		*/
/* Input:																*/
/*		ID_addr															*/
/*																		*/
/* Returns:																*/
/*		byte:	SST25VF032  ID1(Manufacture's ID = BFh or Device ID = 4Ah)			*/
/*		byte:	W25Q32         ID1(Manufacture's ID = eFh or Device ID = 15h)			*/
/************************************************************************/
unsigned char Read_ID(uint8_t ID_addr)
{
	unsigned char byte;
	Enable_Flash();				/* enable device */
	SPI_PutByte(0x90);		/* send read ID command (90h or ABh) */
    SPI_PutByte(0x00);		/* send address */
	SPI_PutByte(0x00);		/* send address */
	SPI_PutByte(ID_addr);		/* send address - either 00H or 01H */
	byte = SPI_GetByte();		/* receive byte */
	Disable_Flash();				/* disable device */
	return byte;
}


/************************************************************************/
/* PROCEDURE: Jedec_ID_Read												*/
/*																		*/
/* This procedure Reads the manufacturer's ID (BFh), memory type (25h)  */
/* and device ID (8Dh).  It will use 9Fh as the JEDEC ID command.    	*/
/* Please see the product datasheet for details.						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		IDs_Read:  SST25VF032  ID1(Manufacture's ID = BFh, Memory Type (25h),			*/
/*		 and Device ID (4Ah)											*/
/*																		*/
/*            W25Q64:      0XEF4016
/************************************************************************/
unsigned long Jedec_ID_Read() 
{
	unsigned long temp;
	
	temp = 0;

	Enable_Flash();				 /* enable device */
	SPI_PutByte(0x9F);		 /* send JEDEC ID command (9Fh) */
    temp = (temp | SPI_GetByte()) << 8;	 /* receive byte */
	temp = (temp | SPI_GetByte()) << 8;	
	temp = (temp | SPI_GetByte()); 	 /* temp value = 0xBF258D */
	Disable_Flash();				/* disable device */

	return temp;
}



/************************************************************************/
/* PROCEDURE:	Auto_Add_IncA											*/
/*																		*/
/* This procedure programs consecutive addresses of 2 bytes of data into*/
/* the device:  1st data byte will be programmed into the initial		*/
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be be		*/
/* programmed into initial address [A23-A1] and with A0  = 1.  This		*/
/* is used to to start the AAI process.  It should be followed by 		*/
/* Auto_Add_IncB.														*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/*																		*/
/* Note: Only RDSR command can be executed once in AAI mode with SO  	*/
/* 	 disable to output RY/BY# status.  Use WRDI to exit AAI mode 		*/
/*	 unless AAI is programming the last address or last address of		*/
/* 	 unprotected block, which automatically exits AAI mode.				*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*          SST25VF020:Dst:	Destination Address 000000H - 03FFFFH              	*/
/*		byte1:		1st byte to be programmed							*/
/*      byte1:		2nd byte to be programmed							*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Auto_Add_IncA(unsigned long Dst, unsigned char byte1, unsigned char byte2)
{
	Enable_Flash();					/* enable device */
	SPI_PutByte(0xAD);			/* send AAI command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	SPI_PutByte(byte1);			/* send 1st byte to be programmed */	
	SPI_PutByte(byte2);			/* send 2nd byte to be programmed */
	Disable_Flash();					/* disable device */
}

/************************************************************************/
/* PROCEDURE:	Auto_Add_IncB											*/
/*																		*/
/* This procedure programs consecutive addresses of 2 bytes of data into*/
/* the device:  1st data byte will be programmed into the initial 		*/
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be be 		*/
/* programmed into initial address [A23-A1] and with A0  = 1.    This  	*/
/* is used after Auto_Address_IncA.										*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/* Note: Only WRDI and AAI command can be executed once in AAI mode 	*/
/*	 with SO enabled as RY/BY# status.  When the device is busy 		*/
/*	 asserting CE# will output the status of RY/BY# on SO.  Use WRDI	*/
/* 	 to exit AAI mode unless AAI is programming the last address or		*/
/*	 last address of unprotected block, which automatically exits 		*/
/*	 AAI mode.															*/
/*																		*/
/* Input:																*/
/*																		*/
/*		byte1:		1st byte to be programmed							*/
/*		byte2:		2nd byte to be programmed							*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Auto_Add_IncB(unsigned char byte1, unsigned char byte2)
{
	Enable_Flash();					/* enable device */
	SPI_PutByte(0xAD);			/* send AAI command */
	SPI_PutByte(byte1);			/* send 1st byte to be programmed */
	SPI_PutByte(byte2);			/* send 2nd byte to be programmed */
	Disable_Flash();					/* disable device */
}	



/************************************************************************/
/* PROCEDURE: Wait_Busy_AAI												*/
/*																		*/
/* This procedure waits until device is no longer busy for AAI mode.	*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Wait_Busy_AAI()
{
	uint16_t i;
	i = 10000;

	while ((Read_Status_Register()&0x40) == 0x40)	/* waste time until not busy */
	{
		Delay_Ms(1);
		if(--i ==0) break;		//等待最长时间1s

	}
}

/************************************************************************/
/* PROCEDURE: WREN_Check												*/
/*																		*/
/* This procedure checks to see if WEL bit set before program/erase.	*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WREN_Check()
{
	uint16_t i;
	i = 0xffff;

	while ((Read_Status_Register() & 0x02)!= 0x02)				/* verify that WEL bit is set */
	{
		Delay_Us(10);
		if(--i ==0) break;		//等待最长时间655.36ms

	}
}

/************************************************************************/
/* PROCEDURE: WREN_AAI_Check											*/
/*																		*/
/* This procedure checks for AAI and WEL bit once in AAI mode.			*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WREN_AAI_Check()
{
	uint16_t i;
	i = 1000;

	while((Read_Status_Register()&0x42) != 0x42)				/* verify that AAI and WEL bit is set */
	{	
		Delay_Ms(1);
		if(--i ==0) break;		//等待最长时间1s

	}
}





/************************************************************************/
/* PROCEDURE:	Auto_Add_IncA_EBSY										*/
/*																		*/
/* This procedure is the same as procedure Auto_Add_IncA except that it */
/* uses EBSY and Poll_SO functions to check for RY/BY. It programs		*/
/* consecutive addresses of the device.  The 1st data byte will be		*/
/* programmed into the initial address [A23-A1] and with A0 = 0.  The 	*/
/* 2nd data byte will be programmed into initial address [A23-A1] and 	*/
/* with A0  = 1.  This is used to to start the AAI process.  It should  */
/* be followed by Auto_Add_IncB_EBSY.									*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/*																		*/
/* Note: Only WRDI and AAI command can be executed once in AAI mode 	*/
/*	 with SO enabled as RY/BY# status.  When the device is busy			*/
/*	 asserting CE# will output the status of RY/BY# on SO.  Use WRDI	*/
/* 	 to exit AAI mode unless AAI is programming the last address or		*/
/*	 last address of unprotected block, which automatically exits		*/
/*	 AAI mode.															*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*		byte1:		1st byte to be programmed							*/
/*      byte1:		2nd byte to be programmed							*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Auto_Add_IncA_EBSY(unsigned long Dst, unsigned char byte1, unsigned char byte2)
{
	EBSY();					/* enable RY/BY# status for SO in AAI */	

	Enable_Flash();				/* enable device */
	SPI_PutByte(0xAD);			/* send AAI command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	SPI_PutByte(byte1);			/* send 1st byte to be programmed */	
	SPI_PutByte(byte2);			/* send 2nd byte to be programmed */
	Disable_Flash();				/* disable device */
	
	Poll_SO();				/* polls RY/BY# using SO line */

}

/************************************************************************/
/* PROCEDURE:	Auto_Add_IncB_EBSY										*/
/*																		*/
/* This procedure is the same as Auto_Add_IncB excpet that it uses 		*/
/* Poll_SO to poll for RY/BY#.  It demonstrate on how to use DBSY after	*/
/* AAI programmming is completed.  It programs consecutive addresses of */
/* the device.  The 1st data byte will be programmed into the initial   */
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be 		*/
/* programmed into initial address [A23-A1] and with A0  = 1.  This is 	*/
/* used after Auto_Address_IncA.										*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/* Note: Only WRDI and AAI command can be executed once in AAI mode 	*/
/*	 with SO enabled as RY/BY# status.  When the device is busy 		*/
/*	 asserting CE# will output the status of RY/BY# on SO.  Use WRDI	*/
/* 	 to exit AAI mode unless AAI is programming the last address or		*/
/*	 last address of unprotected block, which automatically exits 		*/
/*	 AAI mode.															*/
/*																		*/
/* Input:																*/
/*																		*/
/*		byte1:		1st byte to be programmed							*/
/*		byte2:		2nd byte to be programmed							*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Auto_Add_IncB_EBSY(unsigned char byte1, unsigned char byte2)
{
	Enable_Flash();					/* enable device */
	SPI_PutByte(0xAD);			/* send AAI command */
	SPI_PutByte(byte1);			/* send 1st byte to be programmed */
	SPI_PutByte(byte2);			/* send 2nd byte to be programmed */
	Disable_Flash();					/* disable device */

	Poll_SO();					/* polls RY/BY# using SO line */
	
	WRDI(); 					/* Exit AAI before executing DBSY */
	DBSY();						/* disable SO as RY/BY# output if in AAI */
}	


/************************************************************************/
/* PROCEDURE: init														*/
/*																		*/
/* This procedure initializes the SCK to low. Must be called prior to 	*/
/* setting up mode 0.													*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		SCK																*/
/************************************************************************/
void W25QXX_Lock(void)
{
	EWSR();
	Wait_Busy();
//	W25QXX_WRSR(0x38);		//保护底部1/2(   字库空间)
	W25QXX_WRSR(0x3c);		//保护全部空间
	Wait_Busy();
}


void SST25VF_Lock(void)
{
	EWSR();
	Delay_Ms(1);
	WRSR(0x1c);		//全部保护
	Delay_Ms(1);
}


void W25QXX_UnLock(void)
{
	EWSR();
	Wait_Busy();
	W25QXX_WRSR(0x00);		//不保护全部空间
	Wait_Busy();
}


void SST25VF_UnLock(void)
{
	EWSR();
	Delay_Ms(1);
	WRSR(0x00);		//全部保护	
	Delay_Ms(1);
}


void Flash_Lock(void)
{
	if(manID == WINBOND_FLASH)
	{
		W25QXX_Lock();
	}
	else if(manID == SST_FLASH)
	{
		SST25VF_Lock();
	}

}


void Flash_Unlock(void)
{

	if(manID == WINBOND_FLASH)
	{
		W25QXX_UnLock();
	}
	else if(manID == SST_FLASH)
	{
		SST25VF_UnLock();
	}
}

/************************************************************************/
/* PROCEDURE:	Byte_Program											*/
/*																		*/
/* This procedure programs one address of the device.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*				block protected.										*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*		byte:		byte to be programmed								*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Byte_Program(unsigned long Dst, unsigned char byte)
{
	Enable_Flash();					/* enable device */
	SPI_PutByte(0x02); 			/* send Byte Program command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	SPI_PutByte(byte);			/* send byte to be programmed */
	Disable_Flash();					/* disable device */
}



/******************************************************************************************
*函数名：FLASH_PageWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH页写入数据函数，外部调用
*********************************************************************************************/
void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{

	WREN();//使能写入
	Enable_Flash(); 

	SPI_PutByte(W25X_PageProgram);
	SPI_PutByte((WriteAddr & 0xFF0000) >> 16);
	SPI_PutByte((WriteAddr & 0xFF00) >> 8);
	SPI_PutByte(WriteAddr & 0xFF);

	//检测写入的数据是否超出页的容量大小
	if(NumByteToWrite > FLASH_PerWritePageSize)
	{
	 NumByteToWrite = FLASH_PerWritePageSize;
	}
	
	while (NumByteToWrite--)
	{
		SPI_PutByte(*pBuffer);
		pBuffer++;
	}
	Disable_Flash();					  /* disable device */

	/* 等待写完成*/
	Wait_Busy();
}




/******************************************************************************************
*函数名：FLASH_BufferWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH多个数据函数，外部调用
*********************************************************************************************/
void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % FLASH_PageSize;                           //计算写入的页的对应初始地址
	count = FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / FLASH_PageSize;                //计算总共要写的页数
	NumOfSingle = NumByteToWrite % FLASH_PageSize;               //计算剩余单个页写的数据个数
	if (Addr == 0) // 如果要写入的页地址为0，说明正好整页写数据，没有偏移
	{
		if (NumOfPage == 0) // 如果计算的写入页数为0，说明数据量在一个页的范围内，可直接进行页的写
		{
			FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);      //进行页写数据
		}
		else // 如果要写的页数大于0
		{ 
			//先将开头数据进行整页写入
			while (NumOfPage--)
			{ 	
				FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);//整页写入		
				WriteAddr +=  FLASH_PageSize;  //地址偏移	
				pBuffer += FLASH_PageSize;    //数据指针偏移
			}
			//将剩余数据个数写入
			FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else //如果写入的地址不在页的开头位置
	{
		if (NumOfPage == 0) //如果写入数据页的个数为0，即数据小于一页容量
		{
			if (NumOfSingle > count) //如果剩余数据大于当前页的剩余容量
			{
				temp = NumOfSingle - count;     //计算超出的数据个数
				FLASH_PageWrite(pBuffer, WriteAddr, count); //写满当前页
				WriteAddr +=  count;    //设置地址偏移
				pBuffer += count;       //设置数据指针偏移
				FLASH_PageWrite(pBuffer, WriteAddr, temp); //将剩余量写入新的页
			}
			else  //如果剩余数据小于当前页的剩余容量
			{
				FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite); //直接写入当前页
			}
		}
		else //如果写入数据页的个数大于0，即数据大于一页容量
		{
			NumByteToWrite -= count;         //总数据减去当前页剩余的容量
			NumOfPage =  NumByteToWrite / FLASH_PageSize;  //计算要写的整页个数
			NumOfSingle = NumByteToWrite % FLASH_PageSize; //计算剩余数据个数
			//将开头数据写入当前页剩余字节个数
			FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;   //设置地址偏移
			pBuffer += count;   //设置数据指针偏移
			//开始剩下数据的整页写入
			while (NumOfPage--)
			{
				//写入一个页的字节数
				FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);
				//设置地址偏移
				WriteAddr +=  FLASH_PageSize;
				//设置指针偏移
				pBuffer += FLASH_PageSize;
			}
			//如果剩余数据大于0，将剩余的个数写入下一个页
			if (NumOfSingle != 0)
			{
				FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}


/************************************************************************/
/* PROCEDURE:	Read													*/
/*																		*/		
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.											*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		Dst:	Destination Address 000000H - 07FFFFH					*/
/*          SST25VF020:Dst:	Destination Address 000000H - 03FFFFH              	*/           
/*      																*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/*																		*/
/************************************************************************/
unsigned char Read(unsigned long Dst) 
{
	unsigned char byte = 0;	

	Enable_Flash();				/* enable device */
	SPI_PutByte(0x03); 		/* read command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	byte = SPI_GetByte();
	Disable_Flash();				/* disable device */
	return byte;			/* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	Read_Cont												*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*          SST25VF020:Dst:	Destination Address 000000H - 03FFFFH              	*/
/*      no_bytes	Number of bytes to read	(max = 128)					*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Read_Cont(INT8U *pBuffer,unsigned long Dst, unsigned long no_bytes)
{
	unsigned long i = 0;
	Enable_Flash();					/* enable device */
	SPI_PutByte(0x03); 			/* read command */
	SPI_PutByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI_PutByte(((Dst & 0xFFFF) >> 8));
	SPI_PutByte(Dst & 0xFF);
	for (i = 0; i < no_bytes; i++)		/* read until no_bytes is reached */
	{
		pBuffer[i] = SPI_GetByte();	/* receive byte and store at address 80H - FFH */
	}
	Disable_Flash();					/* disable device */

}


//Dst:必须为偶地址
void AAI_Write(unsigned long Dst, uint8_t *p,uint16_t no_bytes)
{
	if(no_bytes%2==1) 
	{//奇数个数据
		*(p+no_bytes) = 0xff;	//不对原存储空间产生任何破坏�	
		no_bytes++;	
	}
	WREN();
	Auto_Add_IncA_EBSY(Dst,*p,*(p+1));

	no_bytes -=2;
	p +=2;

	for(;no_bytes>2;)
	{
		Enable_Flash();					
		SPI_PutByte(0xAD);			
		SPI_PutByte(*p);			
		SPI_PutByte(*(p+1));			
		Disable_Flash();					
		
		Poll_SO();					
		no_bytes -=2;
		p +=2;
	}
	Auto_Add_IncB_EBSY(*p,*(p+1));		//退出AAI模式

}




void Write_Cont(unsigned long Dst, INT8U *p,INT16U no_bytes)
{
//	uint8_t i;
	Flash_Unlock();

	//只有从扇区起始地址写数据才执行探险操作
	if((DATA_TYPE_SIZE == 0x1000) &&(Dst%DATA_TYPE_SIZE ==0))
	{
		WREN();
		Sector_Erase(Dst);
		Wait_Busy();
	}
	else  if((DATA_TYPE_SIZE == 0x8000) &&(Dst%DATA_TYPE_SIZE ==0))
	{
		WREN();	
		Block_Erase_32K(Dst);
		Wait_Busy();		
	}
	else  if((DATA_TYPE_SIZE == 0x10000) &&(Dst%DATA_TYPE_SIZE ==0))  
	{
		WREN();
		Block_Erase_64K(Dst);
		Wait_Busy();		
	}
	

		
	if(manID == SST_FLASH && devID == SST25VF032)
	{
		if((Dst%2 ==0)&&(no_bytes>=6))
		{
			AAI_Write(Dst,p,no_bytes);
		}
		else goto slow_write;
	}
	else if(manID == WINBOND_FLASH)
	{
		FLASH_BufferWrite(p,Dst,no_bytes);
	}
	
	else
	{
	//执行最原始、最慢的单字节写入操作
slow_write:
		for(;no_bytes>0;no_bytes--)
		{
			WREN();
			Byte_Program(Dst,*p);
			Wait_Busy();
			Dst++;
			p++;
		}
	}

	Flash_Lock();	//存储空间保护
}

extern SYSTEM_INFO SystemInfo;	 //系统信息

void FRAM_TEST(void)
{
	 unsigned int i;

	 Flash_Unlock();
	 WREN();
	 Sector_Erase(FRAME_TEST_ADD);
	 Wait_Busy();
	 Flash_Lock();
	
	 for(i=0;i<300;i++)  SystemInfo.DisplayInfo.MessageBuf[i]=i;
	 Write_Cont(FRAME_TEST_ADD,SystemInfo.DisplayInfo.MessageBuf,300);

	 Read_Cont(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD,200);
	 for(i=0;i<200;i++)
	 	uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i]);

	Read_Cont(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD+200,100);
	for(i=0;i<100;i++)
	   uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i]);


/*  	 for(i=0;i<249;i++)  SystemInfo.DisplayInfo.MessageBuf[i]=249-i;
	 Write_Cont(FRAME_TEST_ADD+251,SystemInfo.DisplayInfo.MessageBuf,249);
	

	 Read_Cont(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD,250);
	 for(i=0;i<250;i++)
	 	uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i]);

	Read_Cont(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD+250,250);
	for(i=0;i<250;i++)
	   uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i]);


	 for(i=0;i<30;i++)  SystemInfo.DisplayInfo.MessageBuf[i]=20+i;
	 FLASH_BufferWrite(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD+500,30);
	 Read_Cont(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD+500,30);
	 for(i=0;i<30;i++)
		uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i]);


	 
	for(i=0;i<5;i++)  SystemInfo.DisplayInfo.MessageBuf[i]=20+i;
	for(i=0;i<5;i++)
	{
		WREN();
		Byte_Program(FRAME_TEST_ADD+500+i,SystemInfo.DisplayInfo.MessageBuf[i]);
		Wait_Busy();
	}
	 Read_Cont(SystemInfo.DisplayInfo.MessageBuf,FRAME_TEST_ADD+500,5);
	 for(i=0;i<5;i++)
		uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i]);*/


}



