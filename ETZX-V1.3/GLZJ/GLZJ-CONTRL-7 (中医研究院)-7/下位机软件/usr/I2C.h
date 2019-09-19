/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:I2C.h
*�ļ�����    		:I2C��������ͷ�ļ�
*������     		:����ͬ
*��������   		:2009-4-6
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
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