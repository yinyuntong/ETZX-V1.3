/*Software Driver

SST25VF040B 4 Mbit(512K x 8) Serial Flash Memory

November 4th, 2005, Rev. 1.0

ABOUT THE SOFTWARE
This application note provides software driver examples for SST25VF040B,
Serial Flash. Extensive comments are included in each routine to describe 
the function of each routine.  The interface coding uses polling method 
rather than the SPI protocol to interface with these serial devices.  The
functions are differentiated below in terms of the communication protocols
(uses Mode 0) and specific device operation instructions. This code has been 
designed to compile using the Keil compiler.


ABOUT THE SST25VF040B

Companion product datasheets for the SST25VF040B should be reviewed in 
conjunction with this application note for a complete understanding 
of the device.


Device Communication Protocol(pinout related) functions:

Functions                    		Function
------------------------------------------------------------------
init					Initializes clock to set up mode 0.
Send_Byte				Sends one byte using SI pin to send and 
						shift out 1-bit per clock rising edge
Get_Byte				Receives one byte using SO pin to receive and shift 
						in 1-bit per clock falling edge
Poll_SO					Used in the polling for RY/BY# of SO during AAI programming
CE_High					Sets Chip Enable pin of the serial flash to high
CE_Low					Clears Chip Enable of the serial flash to low
Hold_Low				Clears Hold pin to make serial flash hold
Unhold					Unholds the serial flash
WP_Low					Clears WP pin to make serial flash write protected
UnWP					Disables write protection pin

Note:  The pin names of the SST25VF040B are used in this application note. The associated test code
will not compile unless these pinouts (SCK, SI, SO, SO, CE, WP, Hold) are pre-defined on your
software which should reflect your hardware interfaced. 	 


Device Operation Instruction functions:

Functions                    		Function
------------------------------------------------------------------
Read_Status_Register	Reads the status register of the serial flash
EWSR					Enables the Write Status Register
WRSR					Performs a write to the status register
WREN					Write enables the serial flash
WRDI					Write disables the serial flash
EBSY					Enable SO to output RY/BY# status during AAI programming
DBSY					Disable SO to output RY/BY# status during AAI programming
Read_ID					Reads the manufacturer ID and device ID
Jedec_ID_Read			Reads the Jedec ID
Read					Reads one byte from the serial flash and returns byte(max of 25 MHz CLK frequency)
Read_Cont				Reads multiple bytes(max of 25 MHz CLK frequency)
HighSpeed_Read			Reads one byte from the serial flash and returns byte(max of 50 MHz CLK frequency)
HighSpeed_Read_Cont		Reads multiple bytes(max of 50 MHz CLK frequency)
Byte_Program			Program one byte to the serial flash
Auto_Add_IncA			Initial Auto Address Increment process
Auto_Add_IncB			Successive Auto_Address_Increment process after AAI initiation
Auto_Add_IncA_EBSY		Initial Auto Address Increment process with EBSY
Auto_Add_IncB_EBSY		Successive Auto_Address_Increment process after AAI initiation with EBSY and WRDI/DBSY
Chip_Erase				Erases entire serial flash
Sector_Erase			Erases one sector (4 KB) of the serial flash
Block_Erase_32K			Erases 32 KByte block memory of the serial flash
Block_Erase_64K			Erases 64 KByte block memory of the serial flash
Wait_Busy				Polls status register until busy bit is low
Wait_Busy_AAI			Polls status register until busy bit is low for AAI programming
WREN_Check				Checks to see if WEL is set
WREN_AAI_Check			Checks to see if WEL and AAI mode is set

*/

                                                                     
/*"C" LANGUAGE DRIVERS */

/********************************************************************/
/* Copyright Silicon Storage Technology, Inc. (SST), 1994-2005	    */
/* Example "C" language Driver of SST25VF040B Serial Flash			*/
/* Conrado Canio, Silicon Storage Technology, Inc.                  */
/*                                                                  */
/* Revision 1.0, November 4th, 2005			  						*/   
/*                                                                  */
/*																	*/
/********************************************************************/

#define _IN_SST25VF_
#include "config.h"
	

uint8 xdata manID,devID;

/* Function Prototypes */

void SST25VF_Init();
void Send_Byte(unsigned char out);
unsigned char Get_Byte();
void Poll_SO();
void CE_High();
void CE_Low();
void Hold_Low();
void Unhold();
void WP_Low();
void UnWP();
unsigned char Read_Status_Register();
void EWSR();
void WRSR(byte);
void WREN();
void WRDI();
void EBSY();
void DBSY();
unsigned char Read_ID(ID_addr);
unsigned long Jedec_ID_Read(); 
unsigned char Read(unsigned long Dst);
void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes);
unsigned char HighSpeed_Read(unsigned long Dst); 
void HighSpeed_Read_Cont(unsigned long Dst, unsigned long no_bytes,uint8 *pBuffer);
void Byte_Program(unsigned long Dst, unsigned char byte);
void Auto_Add_IncA(unsigned long Dst, unsigned char byte1, unsigned char byte2);
void Auto_Add_IncB(unsigned char byte1, unsigned char byte2);
void Auto_Add_IncA_EBSY(unsigned long Dst, unsigned char byte1, unsigned char byte2);
void Auto_Add_IncB_EBSY(unsigned char byte1, unsigned char byte2);
void Chip_Erase();
void Sector_Erase(unsigned long Dst);
void Block_Erase_32K(unsigned long Dst);
void Block_Erase_64K(unsigned long Dst);
void Wait_Busy();
void Wait_Busy_AAI();
void WREN_Check();
void WREN_AAI_Check();

void Verify(unsigned char byte, unsigned char cor_byte);
void Byte_Write(unsigned long Dst, unsigned char byte);
void AAI_Write(unsigned long Dst, uint8 *p,uint16 no_bytes);
void delay_nus(unsigned int i);
void delay_nms(unsigned int i);



//unsigned char idata upper_128[128];		/* global array to store read data */
										/* to upper RAM area from 80H - FFH */

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
void SST25VF_Init()
{
//	SCLK = 0;	/* set clock to low initial state */
	
	Send_Data_Byte(Read_Status_Register());

	EWSR();
	Delayms(1);
	WRSR(0x00);		//不写保护
	Delayms(1);

	Send_Data_Byte(Read_Status_Register());
}

/************************************************************************/
/* PROCEDURE: Send_Byte													*/
/*																		*/
/* This procedure outputs a byte shifting out 1-bit per clock rising	*/
/* edge on the the SI pin(LSB 1st).										*/
/*																		*/
/* Input:																*/
/*		out																*/
/*																		*/
/* Output:																*/
/*		SI																*/
/************************************************************************/
void Send_Byte(unsigned char out)
{
	
	//unsigned char i = 0;
	//for (i = 0; i < 8; i++)
	//{
	//	
	//	if ((out & 0x80) == 0x80)	/* check if MSB is high */
	//		MOSI = 1;
	//	else
	//		MOSI = 0;					/* if not, set to low */
	//	SCLK = 1;					/* toggle clock high */
	//	out = (out << 1);			/* shift 1 place for next bit */
	//	SCLK = 0;					/* toggle clock low */
	//}
	SPI_WriteByte(out);
}

/************************************************************************/
/* PROCEDURE: Get_Byte													*/
/*																		*/
/* This procedure inputs a byte shifting in 1-bit per clock falling		*/
/* edge on the SO pin(LSB 1st).											*/
/*																		*/
/* Input:																*/
/*		SO																*/
/*																		*/
/* Output:																*/
/*		None															*/
/************************************************************************/
unsigned char Get_Byte()
{
	//unsigned char i = 0, in = 0, temp = 0;
	//for (i = 0; i < 8; i++)
	//{
	//	in = (in << 1);		/* shift 1 place to the left or shift in 0 */
	//	temp = MISO;			/* save input */
	//	SCLK = 1;			/* toggle clock high */
	//	if (temp == 1)			/* check to see if bit is high */
	//		in = in | 0x01;		/* if high, make bit high */

	//	SCLK = 0;			/* toggle clock low */

	//}
	//return in;

	uint8 xdata in;
	in=SPI_WriteByte(0x00);	//输入8个时钟
	return in;
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
	uint16 xdata i =0xffff;
	
	unsigned char temp = 0;
	CE_Low();
	while (temp == 0x00)	/* waste time until not busy */
	{
		temp = MISO;
		delay_nus(1);
		if( --i == 0x00) break;
	}
	CE_High();
	WDT_CONTR = 0x3e;	//喂狗
}

/************************************************************************/
/* PROCEDURE: CE_High													*/
/*																		*/
/* This procedure set CE = High.										*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		CE																*/
/*																		*/
/************************************************************************/
void CE_High() 
{
	SST25VF_CS = 1;				/* set CE high */
}

/************************************************************************/
/* PROCEDURE: CE_Low													*/
/*																		*/
/* This procedure drives the CE of the device to low.  					*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		CE																*/
/*																		*/
/************************************************************************/
void CE_Low() 
{	
	SST25VF_CS = 0;				/* clear CE low */
}

/************************************************************************/
/* PROCEDURE: Hold()													*/
/*																		*/
/* This procedure clears the Hold pin to low.							*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		Hold															*/
/************************************************************************/
void Hold_Low()
{

//	Hold = 0;			/* clear Hold pin */
	
}

/************************************************************************/
/* PROCEDURE: Unhold()													*/
/*																		*/
/* This procedure sets the Hold pin to high.							*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		Hold															*/
/************************************************************************/
void Unhold()
{

//	Hold = 1;			/* set Hold pin */
	
}

/************************************************************************/
/* PROCEDURE: WP()														*/
/*																		*/
/* This procedure clears the WP pin to low.								*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		WP																*/
/************************************************************************/
void WP_Low()
{

//	WP = 0;				/* clear WP pin */

}

/************************************************************************/
/* PROCEDURE: UnWP()													*/
/*																		*/
/* This procedure sets the WP pin to high.								*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Output:																*/
/*		WP																*/
/************************************************************************/
void UnWP()
{

//	WP = 1;				/* set WP pin */

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
	CE_Low();				/* enable device */
	Send_Byte(0x05);		/* send RDSR command */
	byte = Get_Byte();		/* receive byte */
	CE_High();				/* disable device */
	return byte;
}



/************************************************************************/


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
	CE_Low();				/* enable device */
	Send_Byte(0x50);		/* enable writing to the status register */
	CE_High();				/* disable device */
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
void WRSR(byte)
{
	CE_Low();				/* enable device */
	Send_Byte(0x01);		/* select write to status register */
	Send_Byte(byte);		/* data that will change the status of BPx 
							   or BPL (only bits 2,3,4,5,7 can be written) */
	CE_High();				/* disable the device */
}



void W25QXX_WRSR(unsigned int status)
{
	CE_Low();				/* enable device */
	Send_Byte(0x01);		/* select write to status register */
	Send_Byte(status);		// data that will change the status of BPx 
	Send_Byte(status>>8);		// or BPL (only bits 2,3,4,5,7 can be written) */
	CE_High();		   /* disable the device */
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
	CE_Low();				/* enable device */
	Send_Byte(0x06);		/* send WREN command */
	CE_High();				/* disable device */
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
	CE_Low();				/* enable device */
	Send_Byte(0x04);		/* send WRDI command */
	CE_High();				/* disable device */
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
	CE_Low();				/* enable device */
	Send_Byte(0x70);		/* send EBSY command */
	CE_High();				/* disable device */
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
	CE_Low();				/* enable device */
	Send_Byte(0x80);		/* send DBSY command */
	CE_High();				/* disable device */
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
	Delayms(1);
	WRSR(0x1c);		//全部保护
	Delayms(1);
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
	Delayms(1);
	WRSR(0x00);		//全部不保护	
	Delayms(1);
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
/*		byte:	ID1(Manufacture's ID = BFh or Device ID = 8Dh)			*/
/*																		*/
/************************************************************************/
unsigned char Read_ID(ID_addr)
{
	unsigned char byte;
	CE_Low();				/* enable device */
	Send_Byte(0x90);		/* send read ID command (90h or ABh) */
    Send_Byte(0x00);		/* send address */
	Send_Byte(0x00);		/* send address */
	Send_Byte(ID_addr);		/* send address - either 00H or 01H */
	byte = Get_Byte();		/* receive byte */
	CE_High();				/* disable device */
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
/*		IDs_Read:ID1(Manufacture's ID = BFh, Memory Type (25h),			*/
/*		 and Device ID (8Dh)											*/
/*																		*/
/************************************************************************/
unsigned long Jedec_ID_Read() 
{
	unsigned long temp;
	
	temp = 0;

	CE_Low();				 /* enable device */
	Send_Byte(0x9F);		 /* send JEDEC ID command (9Fh) */
    temp = (temp | Get_Byte()) << 8;	 /* receive byte */
	temp = (temp | Get_Byte()) << 8;	
	temp = (temp | Get_Byte()); 	 /* temp value = 0xBF258D */
	CE_High();				/* disable device */

	return temp;
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

	CE_Low();				/* enable device */
	Send_Byte(0x03); 		/* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	byte = Get_Byte();
	CE_High();				/* disable device */
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
void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes)
{
	unsigned long i = 0;
	CE_Low();					/* enable device */
	Send_Byte(0x03); 			/* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	for (i = 0; i < no_bytes; i++)		/* read until no_bytes is reached */
	{
		pBuffer[i] = Get_Byte();	/* receive byte and store at address 80H - FFH */
	}
	CE_High();					/* disable device */

}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read											*/
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
unsigned char HighSpeed_Read(unsigned long Dst) 
{
	unsigned char byte = 0;	

	CE_Low();				/* enable device */
	Send_Byte(0x0B); 		/* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF);		/*dummy byte*/
	byte = Get_Byte();
	CE_High();				/* disable device */
	return byte;			/* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read_Cont										*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*      no_bytes	Number of bytes to read	(max = 128)					*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void HighSpeed_Read_Cont(unsigned long Dst, unsigned long no_bytes,uint8 *pBuffer)
{
	unsigned long i = 0;
	CE_Low();					/* enable device */
	Send_Byte(0x0B); 			/* read command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(0xFF);			/*dummy byte*/
	for (i = 0; i < no_bytes; i++)		/* read until no_bytes is reached */
	{
		pBuffer[i] = Get_Byte();	/* receive byte and store at address 80H - FFH */
	}
	CE_High();					/* disable device */
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
	CE_Low();					/* enable device */
	Send_Byte(0x02); 			/* send Byte Program command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(byte);			/* send byte to be programmed */
	CE_High();					/* disable device */
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
	CE_Low();					/* enable device */
	Send_Byte(0xAD);			/* send AAI command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(byte1);			/* send 1st byte to be programmed */	
	Send_Byte(byte2);			/* send 2nd byte to be programmed */
	CE_High();					/* disable device */
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
	CE_Low();					/* enable device */
	Send_Byte(0xAD);			/* send AAI command */
	Send_Byte(byte1);			/* send 1st byte to be programmed */
	Send_Byte(byte2);			/* send 2nd byte to be programmed */
	CE_High();					/* disable device */
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

	CE_Low();				/* enable device */
	Send_Byte(0xAD);			/* send AAI command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(byte1);			/* send 1st byte to be programmed */	
	Send_Byte(byte2);			/* send 2nd byte to be programmed */
	CE_High();				/* disable device */
	
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
	CE_Low();					/* enable device */
	Send_Byte(0xAD);			/* send AAI command */
	Send_Byte(byte1);			/* send 1st byte to be programmed */
	Send_Byte(byte2);			/* send 2nd byte to be programmed */
	CE_High();					/* disable device */

	Poll_SO();					/* polls RY/BY# using SO line */
	
	WRDI(); 					/* Exit AAI before executing DBSY */
	DBSY();						/* disable SO as RY/BY# output if in AAI */
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
	CE_Low();					/* enable device */
	Send_Byte(0x60);			/* send Chip Erase command (60h or C7h) */
	CE_High();					/* disable device */
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


	CE_Low();					/* enable device */
	Send_Byte(0x20);			/* send Sector Erase command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High();					/* disable device */
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
	CE_Low();					/* enable device */
	Send_Byte(0x52);			/* send 32 KByte Block Erase command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High();					/* disable device */
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
	CE_Low();					/* enable device */
	Send_Byte(0xD8);			/* send 64KByte Block Erase command */
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High();					/* disable device */
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
	uint16 xdata i;
	i = 60000;

	while ((Read_Status_Register()&0x01) == 0x01)	/* waste time until not busy */
	{

		delay_nus(1);
		if(--i ==0) break;		//等待最长时间10ms		
	}
	WDT_CONTR = 0x3e;	//喂狗
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
	uint16 xdata i;
	i = 1000;

	while ((Read_Status_Register()&0x40) == 0x40)	/* waste time until not busy */
	{
		Delayms(1);
		if(--i ==0) break;		//等待最长时间1s

	}
	WDT_CONTR = 0x3e;	//喂狗
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
	uint16 xdata i;
	i = 0xffff;

	while ((Read_Status_Register() & 0x02)!= 0x02)				/* verify that WEL bit is set */
	{
		delay_nus(10);
		if(--i ==0) break;		//等待最长时间655.36ms

	}
	WDT_CONTR = 0x3e;	//喂狗
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
	uint16 xdata i;
	i = 1000;

	while((Read_Status_Register()&0x42) != 0x42)				/* verify that AAI and WEL bit is set */
	{	
		Delayms(1);
		if(--i ==0) break;		//等待最长时间1s

	}
	WDT_CONTR = 0x3e;	//喂狗
}




void Byte_Write(unsigned long Dst, unsigned char byte)
{
	
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



	WREN();
	Byte_Program(Dst,byte);
	Wait_Busy();

	Flash_Lock();
}





/******************************************************************************************
*函数名：FLASH_PageWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH页写入数据函数，外部调用
*********************************************************************************************/
void FLASH_PageWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{

	WREN();//使能写入
	CE_Low(); 

	Send_Byte(W25X_PageProgram);
	Send_Byte((WriteAddr & 0xFF0000) >> 16);
	Send_Byte((WriteAddr & 0xFF00) >> 8);
	Send_Byte(WriteAddr & 0xFF);

	//检测写入的数据是否超出页的容量大小
	if(NumByteToWrite > FLASH_PerWritePageSize)
	{
	 	NumByteToWrite = FLASH_PerWritePageSize;
	}
	
	while (NumByteToWrite--)
	{
		Send_Byte(*pBuffer);
		pBuffer++;
	}
	CE_High();					  /* disable device */
	WDT_CONTR = 0x3e;	//喂狗

	/* 等待写完成*/
	Wait_Busy();
}




/******************************************************************************************
*函数名：FLASH_BufferWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH多个数据函数，外部调用
*********************************************************************************************/
void FLASH_BufferWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{
	uint8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
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
			WDT_CONTR = 0x3e;	//喂狗
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
			WDT_CONTR = 0x3e;	//喂狗
			//如果剩余数据大于0，将剩余的个数写入下一个页
			if (NumOfSingle != 0)
			{
				FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}




//Dst:必须为偶地址
void AAI_Write(unsigned long Dst, uint8 *p,uint16 no_bytes)
{
	if(no_bytes%2==1) 
	{//奇数个数据
		*(p+no_bytes) = 0xff;	//不对原存储空间产生任何破坏。	
		no_bytes++;	

	}
	WREN();
	Auto_Add_IncA_EBSY(Dst,*p,*(p+1));

	no_bytes -=2;
	p +=2;

	for(;no_bytes>2;)
	{
		CE_Low();					
		Send_Byte(0xAD);			
		Send_Byte(*p);			
		Send_Byte(*(p+1));			
		CE_High();					
		
		Poll_SO();					
		no_bytes -=2;
		p +=2;
	}
	Auto_Add_IncB_EBSY(*p,*(p+1));		//退出AAI模式

}




void Write_Cont(unsigned long Dst, uint8 *p,uint16 no_bytes)
{
	uint8 i;
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


void FRAM_TEST(void)
{
	 unsigned int i;

	 Flash_Unlock();
	 WREN();
	 Sector_Erase(FRAME_TEST_ADD);
	 Wait_Busy();
	 Flash_Lock();
	
	 for(i=0;i<101;i++)  GT23_Rec_Data[i]=i;
	 Write_Cont(FRAME_TEST_ADD,GT23_Rec_Data,101);

	 Read_Cont(GT23_Rec_Data,FRAME_TEST_ADD,102);
	 Send_Data(GT23_Rec_Data,102);

	 
  	 for(i=0;i<99;i++)  GT23_Rec_Data[i]=99-i;
	 Write_Cont(FRAME_TEST_ADD+101,GT23_Rec_Data,99);
	 Read_Cont(GT23_Rec_Data,FRAME_TEST_ADD+101,99);
	 Send_Data(GT23_Rec_Data,99);

	 for(i=0;i<30;i++)  GT23_Rec_Data[i]=20+i;
	 FLASH_BufferWrite(GT23_Rec_Data,FRAME_TEST_ADD+200,30);
	 Read_Cont(GT23_Rec_Data,FRAME_TEST_ADD+200,30);
	 Send_Data(GT23_Rec_Data,30);

	 
	for(i=0;i<5;i++)  GT23_Rec_Data[i]=20+i;
	
/*	for(i=0;i<5;i++)
	{
		WREN();
		Byte_Program(FRAME_TEST_ADD+200+i,GT23_Rec_Data[i]);
		Wait_Busy();
	}*/
	 Write_Cont(FRAME_TEST_ADD+200,GT23_Rec_Data,5);
	 Read_Cont(GT23_Rec_Data,FRAME_TEST_ADD+200,5);
	 Send_Data(GT23_Rec_Data,5);

}

/**********************************************************
*函数名称			:delay_nms	
*函数描述        	:毫秒级延时操作
*输入参数   		:i:延时的ms数
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void delay_nms(unsigned int i)
{
 	unsigned int j;

 	for(;i>0;i--)
 	{
 		for(j=900;j>0;j--);
 	}
	 
}


/**********************************************************
*函数名称			:delay_nus	
*函数描述        	:纳秒级延时操作
*输入参数   		:i:延时的us数
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void delay_nus(unsigned int i)
{
 	unsigned char j;

 	for(;i>0;i--)
 	{
 		for(j=2;j>0;j--);
 	}
	 
}


