/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SPI.h
*文件描述    		:SPI 驱动程序头文件
*创建人     		:尹运同
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
/* SPI macros */
	
	/** switch to fast SPI Clock */
#define SPISetFastClock(){SPCTL=0x71;}  /* 0x73 is much slower*/
	
	
	/** SPI data return register */
#define SPI_RESULT_BYTE SPDAT

#ifndef _IN_SPI_
#define _IN_SPI_

extern void SpiInit(void);
extern uint8 SPI_WriteByte(unsigned char dat);
extern unsigned char SPIGetChar();
extern void SPI8Clocks(unsigned char nClocks);


#endif

