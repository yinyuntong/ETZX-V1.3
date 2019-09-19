/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:I2C.h
*文件描述    		:I2C驱动程序头文件
*创建人     		:尹运同
*创建日期   		:2009-4-6
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#ifndef _I2C_H_
#define _I2C_H_


#define uchar unsigned char 
#define uint  unsigned int


#ifndef _IN_I2C_
#define _IN_I2C_
extern void Delay_us(uchar cnt);
extern void Start();
extern void Stop();
extern void WriteACK(uchar ack);
extern void WaitACK();
extern void writebyte(uchar wdata);
extern uchar Readbyte();
extern void writeData(uchar address,uchar mdata);
extern void writeDataStr(uchar address,uchar count,uchar * buf);
extern uchar ReadData(uchar address);
extern void ReadDataStr(uchar address,uchar count,uchar * buff);
#endif


#endif