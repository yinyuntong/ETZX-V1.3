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
	

unsigned char xdata FRAM_BUFF[20];



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
void Read_Cont(unsigned long Dst, unsigned long no_bytes,uint8 *pBuffer);
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
void AAI_Write(unsigned long Dst, uint8 *p,uint8 no_bytes);
void delay_nus(unsigned char i);
void delay_nms(unsigned char i);



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
	unsigned char temp = 0;
	CE_Low();
	while (temp == 0x00)	/* waste time until not busy */
	{
		temp = MISO;
		WDT_CONTR = 0x3d; 
	}
	CE_High();
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
//	WREN_Check();
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
void Read_Cont(unsigned long Dst, unsigned long no_bytes,uint8 *pBuffer)
{
	unsigned long i = 0;
  	WDT_CONTR = 0x3d; 	
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
  	WDT_CONTR = 0x3d; 	
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
	uint16 xdata i=0xffff;
	while ((Read_Status_Register()&0x01) == 0x01)	/* waste time until not busy */
	{
		WDT_CONTR = 0x3d; 
		delay_nus(10);
		if( --i == 0) break;
	}

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
	uint16 xdata i=0xffff;
	while ((Read_Status_Register()&0X40) == 0x40)	/* waste time until not busy */
	{
		WDT_CONTR = 0x3d; 
		Delayms(1);
		if( --i ==0) break;
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
	uint16 xdata i=0xffff;
	while ((Read_Status_Register()&0x02) != 0x02)	/* verify that WEL bit is set */
	{
		WDT_CONTR = 0x3d; 
		delay_nus(10);
		if( --i == 0) break;
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
	uint16 xdata i=0xffff;
	while ((Read_Status_Register()&0x42) != 0x42)	/* verify that AAI and WEL bit is set */
	{
		WDT_CONTR = 0x3d; 
		delay_nus(10);
		if( --i == 0) break;
	}
}




void Byte_Write(unsigned long Dst, unsigned char byte)
{
	WREN();
	Sector_Erase(Dst);
	Wait_Busy();
//	delay_nms(25);

	WREN();
	Byte_Program(Dst,byte);
	Wait_Busy();
//	delay_nus(20);
}




	


//为了PCT25VF020    SST25VF020兼容
//将程序修改成单字节写循环
void AAI_Write(unsigned long Dst, uint8 *p,uint8 no_bytes)
{
	WREN();
	Sector_Erase(Dst);
//	delay_nms(25);
	Wait_Busy();

	WDT_CONTR = 0x3d; 
	for(;no_bytes>0;no_bytes--)
	{
		WREN();
		Byte_Program(Dst,*p);
		Wait_Busy();
		Dst++;
		p++;
	}
		
}




//原已整个扇区擦除,不再探除
void Byte_Write1(unsigned long Dst, unsigned char byte)
{
	WREN();
	Byte_Program(Dst,byte);
	Wait_Busy();
//	delay_nus(20);
}








//为了PCT25VF020    SST25VF020兼容
//将程序修改成单字节写循环
void AAI_Write1(unsigned long Dst, uint8 *p,uint8 no_bytes)
{
	WDT_CONTR = 0x3d; 

	for(;no_bytes>0;no_bytes--)
	{
		WREN();
		Byte_Program(Dst,*p);
		Wait_Busy();
		Dst++;
		p++;
	}
		
}




/**********************************************************
*函数名称			:FRAM_TEST	
*函数描述        	:存储芯片测试操作
*输入参数   		:
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
void FRAM_TEST(void)
{
 	 unsigned char i;

 	 FRAM_BUFF[0]=0x11;
	 FRAM_BUFF[1]=0x22;
	 FRAM_BUFF[2]=0x33;
	 FRAM_BUFF[3]=0x44;
	 FRAM_BUFF[4]=0x55;
	 FRAM_BUFF[5]=0x66;	

	 FRAM_BUFF[6]=0x77;
	 FRAM_BUFF[7]=0x88;
	 FRAM_BUFF[8]=0x99;
	 FRAM_BUFF[9]=0xaa;
	 FRAM_BUFF[10]=0xbb;

	 
	 Byte_Write(0x008000,FRAM_BUFF[0]);
	 AAI_Write1(0x008001,&(FRAM_BUFF[1]),10);
	 
	 for(i=0;i<20;i++) FRAM_BUFF[i]=0;
	 delay_nms(10);

	 FRAM_BUFF[0]=Read(0x008000);
	 Read_Cont(0x008001,10,&(FRAM_BUFF[1]));
	 delay_nms(10);

	 Send_Data(FRAM_BUFF,11);

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
void delay_nms(unsigned char i)
{
 	unsigned int j;

 	for(;i>0;i--)
 	{
 		for(j=900;j>0;j--);
		WDT_CONTR = 0x3d; 
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
void delay_nus(unsigned char i)
{
 	unsigned char j;

 	for(;i>0;i--)
 	{
 		for(j=2;j>0;j--);
 	}
	 
}


