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
#ifndef _SPI_H_
#define _SPI_H_

#ifndef _IN_SPI_
#define _IN_SPI_
extern void MCUMasterSPI(void);
extern uint8 SPI_WriteByte(uint8 dat);
extern void EPH1660MasterSPI(void);
#endif

#endif

