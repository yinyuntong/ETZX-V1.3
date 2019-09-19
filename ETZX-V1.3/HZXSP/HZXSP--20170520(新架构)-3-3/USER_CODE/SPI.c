/*********************************************************************************************************
MASTER SPI ����
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
		if(--i ==0) break;		//�ȴ��ʱ��300ms		
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
		if(--i ==0) break;		//�ȴ��ʱ��1s

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
		if(--i ==0) break;		//�ȴ��ʱ��655.36ms

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
		if(--i ==0) break;		//�ȴ��ʱ��1s

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
//	W25QXX_WRSR(0x38);		//�����ײ�1/2(   �ֿ�ռ�)
	W25QXX_WRSR(0x3c);		//����ȫ���ռ�
	Wait_Busy();
}


void SST25VF_Lock(void)
{
	EWSR();
	Delay_Ms(1);
	WRSR(0x1c);		//ȫ������
	Delay_Ms(1);
}


void W25QXX_UnLock(void)
{
	EWSR();
	Wait_Busy();
	W25QXX_WRSR(0x00);		//������ȫ���ռ�
	Wait_Busy();
}


void SST25VF_UnLock(void)
{
	EWSR();
	Delay_Ms(1);
	WRSR(0x00);		//ȫ������	
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
*��������FLASH_PageWrite()
* ������u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite ����ָ�룬д���ַ��д��ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASHҳд�����ݺ������ⲿ����
*********************************************************************************************/
void FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{

	WREN();//ʹ��д��
	Enable_Flash(); 

	SPI_PutByte(W25X_PageProgram);
	SPI_PutByte((WriteAddr & 0xFF0000) >> 16);
	SPI_PutByte((WriteAddr & 0xFF00) >> 8);
	SPI_PutByte(WriteAddr & 0xFF);

	//���д��������Ƿ񳬳�ҳ��������С
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

	/* �ȴ�д���*/
	Wait_Busy();
}




/******************************************************************************************
*��������FLASH_BufferWrite()
* ������u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite ����ָ�룬д���ַ��д��ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASH������ݺ������ⲿ����
*********************************************************************************************/
void FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % FLASH_PageSize;                           //����д���ҳ�Ķ�Ӧ��ʼ��ַ
	count = FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / FLASH_PageSize;                //�����ܹ�Ҫд��ҳ��
	NumOfSingle = NumByteToWrite % FLASH_PageSize;               //����ʣ�൥��ҳд�����ݸ���
	if (Addr == 0) // ���Ҫд���ҳ��ַΪ0��˵��������ҳд���ݣ�û��ƫ��
	{
		if (NumOfPage == 0) // ��������д��ҳ��Ϊ0��˵����������һ��ҳ�ķ�Χ�ڣ���ֱ�ӽ���ҳ��д
		{
			FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);      //����ҳд����
		}
		else // ���Ҫд��ҳ������0
		{ 
			//�Ƚ���ͷ���ݽ�����ҳд��
			while (NumOfPage--)
			{ 	
				FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);//��ҳд��		
				WriteAddr +=  FLASH_PageSize;  //��ַƫ��	
				pBuffer += FLASH_PageSize;    //����ָ��ƫ��
			}
			//��ʣ�����ݸ���д��
			FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else //���д��ĵ�ַ����ҳ�Ŀ�ͷλ��
	{
		if (NumOfPage == 0) //���д������ҳ�ĸ���Ϊ0��������С��һҳ����
		{
			if (NumOfSingle > count) //���ʣ�����ݴ��ڵ�ǰҳ��ʣ������
			{
				temp = NumOfSingle - count;     //���㳬�������ݸ���
				FLASH_PageWrite(pBuffer, WriteAddr, count); //д����ǰҳ
				WriteAddr +=  count;    //���õ�ַƫ��
				pBuffer += count;       //��������ָ��ƫ��
				FLASH_PageWrite(pBuffer, WriteAddr, temp); //��ʣ����д���µ�ҳ
			}
			else  //���ʣ������С�ڵ�ǰҳ��ʣ������
			{
				FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite); //ֱ��д�뵱ǰҳ
			}
		}
		else //���д������ҳ�ĸ�������0�������ݴ���һҳ����
		{
			NumByteToWrite -= count;         //�����ݼ�ȥ��ǰҳʣ�������
			NumOfPage =  NumByteToWrite / FLASH_PageSize;  //����Ҫд����ҳ����
			NumOfSingle = NumByteToWrite % FLASH_PageSize; //����ʣ�����ݸ���
			//����ͷ����д�뵱ǰҳʣ���ֽڸ���
			FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;   //���õ�ַƫ��
			pBuffer += count;   //��������ָ��ƫ��
			//��ʼʣ�����ݵ���ҳд��
			while (NumOfPage--)
			{
				//д��һ��ҳ���ֽ���
				FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);
				//���õ�ַƫ��
				WriteAddr +=  FLASH_PageSize;
				//����ָ��ƫ��
				pBuffer += FLASH_PageSize;
			}
			//���ʣ�����ݴ���0����ʣ��ĸ���д����һ��ҳ
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


//Dst:����Ϊż��ַ
void AAI_Write(unsigned long Dst, uint8_t *p,uint16_t no_bytes)
{
	if(no_bytes%2==1) 
	{//����������
		*(p+no_bytes) = 0xff;	//����ԭ�洢�ռ�����κ��ƻ��	
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
	Auto_Add_IncB_EBSY(*p,*(p+1));		//�˳�AAIģʽ

}




void Write_Cont(unsigned long Dst, INT8U *p,INT16U no_bytes)
{
//	uint8_t i;
	Flash_Unlock();

	//ֻ�д�������ʼ��ַд���ݲ�ִ��̽�ղ���
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
	//ִ����ԭʼ�������ĵ��ֽ�д�����
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

	Flash_Lock();	//�洢�ռ䱣��
}

extern SYSTEM_INFO SystemInfo;	 //ϵͳ��Ϣ

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



