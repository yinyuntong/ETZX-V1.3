/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:HZ.c
*文件描述    		:汉字识别、汉字字库驱动
*创建人     		:尹运同
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		     :
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_GT23_
#include "config.h"

uint8	xdata  GT23_Rec_Data[128];
uint8	xdata  GT23_Rec_Length=0;
uint16	xdata  GT23_16LAddress;
uint16	xdata  GT23_16HAddress;
uint32	xdata  GT23_32Address; 

void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size);
void Get_GT23_Data1(uint8 *word,uint8  word_size);
void GT23_TEST(void);
uint8 Rec_GT23_Data(void);

/*---------------------------------------------------------------------------
函数原型: uint8 Rec_GT23_Data(void)
参数说明: 无
返 回 值: 字库芯片的返回值
函数功能：获取字库芯片的点阵数据
----------------------------------------------------------------------------*/
/*uint8 Rec_GT23_Data(void)
 {
    uint8 i,j;
	for(i=0;i<8;i++)
	  { 
	    j <<= 1;
        SCLK=1;
	   	_nop_();
       if(MISO)
       j |= 0x01;	 
       SCLK=0;
	   	_nop_();
	  }
	return j;
 }*/

 uint8 Rec_GT23_Data(void)
  {
	SPI_WriteByte(0xf0);
	return SPDAT;
  }
 /*-----------------------------------------------------------------------------------------------------------------------------------
函数原型: void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size)
参数说明: highbyte--汉字国标码的高字节,lowbyte--汉字国标码的低字节,word_size--获取点阵汉字的字体大小 

1为15*16点阵汉字或者8*16点阵ASCII
2为24*24点阵汉字或者16*24点阵ASCII
3为32*32点阵汉字或者16*32点阵ASCII

15*16
BaseAdd=0x2C9D0;
if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32+ BaseAdd;
else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;

24*24
BaseAdd=0x68190;
if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*72+ BaseAdd;	
else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*72+ BaseAdd;

32*32
BaseAdd=0XEDF00;
if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*128+ BaseAdd;
else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*128+ BaseAdd;

返 回 值: 无
函数功能：通过发送汉字的国标码来获取该汉字的点阵数据
--------------------------------------------------------------------------------------------------------------------------------------*/
void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size)
{
    uint32  xdata address1,address2,address3,address4;
	SpiNormalMode();

	if(highbyte==0x00&&lowbyte<0x7f&&lowbyte>=0x20) // 0x21~0x7e  是ASCII字符
	{
		switch(word_size)
		{

		 case 1: //16*8
		 default:
			 address1 = (lowbyte-0x20)*16; 
			 address3 = address1+A8BaseAdd; 
			 Read_Cont(GT23_Rec_Data,address3,16);
			 break; 
			 
		 case 2: //24x12
			 address1 = (lowbyte-0x20)*48; 
			 address3 = address1+A12BaseAdd; 
			 Read_Cont(GT23_Rec_Data,address3,48);
			 break; 				 
		 
		 case 3:	//32X16
			 address1 = (lowbyte-0x20)*64; 
			 address3 = address1+A16BaseAdd; 
			 Read_Cont(GT23_Rec_Data,address3,64);
			 break; 				 
			
		}		
	}
	else if(highbyte >=0x81&&highbyte<=0xfe&&lowbyte>=0x40&&lowbyte<=0xfe)	  //GBK内码 32X32
	{
		switch(word_size)
		{
			case 1:
			default:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*32;				 //每个汉字占用32字节
				 address4 = address3+C16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,32);
				 break;
			case 3:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*128;				 //每个汉字占用128字节
				 address4 = address3+C32BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,128);
				 break; 			 
		}

	}

	SpiLcdMode();
}



 /*-----------------------------------------------------------------------------------------------------------------------------------
函数原型: void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size)
参数说明: highbyte--汉字国标码的高字节,lowbyte--汉字国标码的低字节,word_size--获取点阵汉字的字体大小 
		  1为15*16点阵汉字或者8*16点阵ASCII
		  2为24*24点阵汉字或者16*24点阵ASCII
		  3为32*32点阵汉字或者16*32点阵ASCII

返 回 值: 无
函数功能：通过发送汉字的国标码来获取该汉字的点阵数据
--------------------------------------------------------------------------------------------------------------------------------------*/
void Get_GT23_Data1(uint8 *word,uint8 word_size)
{
    uint32  xdata address1,address2,address3,address4;
	uint8 xdata highbyte,lowbyte;
	SpiNormalMode();

	
	highbyte=*word;
	word++;
	lowbyte= *word;
	if(highbyte<0x7f&&highbyte>=0x20) //   0x21~0x7e  是ASCII字符
	{
		switch(word_size)
		{

		 case 1: //16*8
		 default:
			 address1 = (highbyte-0x20)*16; 
			 address3 = address1+A8BaseAdd; 
			 Read_Cont(GT23_Rec_Data,address3,16);
			 break; 
			 
		 case 2: //24x12
			 address1 = (highbyte-0x20)*48; 
			 address3 = address1+A12BaseAdd; 
			 Read_Cont(GT23_Rec_Data,address3,48);
			 break; 				 
		 
		 case 3:	//32X16
			 address1 = (highbyte-0x20)*64; 
			 address3 = address1+A16BaseAdd; 
			 Read_Cont(GT23_Rec_Data,address3,64);
			 break; 				 
				
		}		
	}
	else if(highbyte >=0x81&&highbyte<=0xfe&&lowbyte>=0x40&&lowbyte<=0xfe)	  //GBK内码 32X32
	{
		switch(word_size)
		{
			case 1:
			default:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*32;				 //每个汉字占用32字节
				 address4 = address3+C16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,32);
				 break;
			case 3:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*128;				 //每个汉字占用128字节
				 address4 = address3+C32BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,128);
				 break; 			 
		}

	}

    
	SpiLcdMode();
}
