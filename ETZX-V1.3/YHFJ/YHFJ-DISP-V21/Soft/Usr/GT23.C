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
	GT23L_CS=0;
	//SPI_CS1=0;
	WDT_CONTR = 0x3d; 
	if(highbyte >=0xA1&&highbyte<=0xA9&&lowbyte>=0xA1)	  //如果是全角字符,数字 
	{
	   if(word_size==1) //15*16
		{
		  address1 = (highbyte-0xa1)*94; 
		  address2 = (lowbyte-0xA1)+address1;
		  address3 = address2*32;
		  address4 = address3+C16BaseAdd;
		}
	  else if(word_size==2)//24*24
	   {
		  address1 = (highbyte-0xa1)*94; 
		  address2 = (lowbyte-0xA1)+address1;
		  address3 = address2*72;
		  address4 = address3+C24BaseAdd;
	   }
	   else if(word_size==3)//32*32
	   {
		   address1 = (highbyte-0xa1)*94; 
		   address2 = (lowbyte-0xA1)+address1;
		   address3 = address2*128;
		   address4 = address3+C32BaseAdd; 
	   }
	 }

	
    else if(highbyte >=0xB0&&highbyte<=0xF7&&lowbyte>=0xA1)    //如果是汉字 
	 {
		if(word_size==1) //15*16
		 {
		   address1 = (highbyte-0xB0)*94; 
		   address2 = lowbyte-0xA1+846+address1;
		   address3 = address2*32;
		   address4 = address3+C16BaseAdd;
		 }
	   else if(word_size==2)//24*24
	    {
	      address1 = (highbyte-0xB0)*94; 
	      address2 = lowbyte-0xA1+846+address1;
	      address3 = address2*72;
	      address4 = address3+C24BaseAdd;
		}
		else if(word_size==3)//32*32
		{
		  address1 = (highbyte-0xB0)*94; 
	      address2 = lowbyte-0xA1+846+address1;
	      address3 = address2*128;
	      address4 = address3+C32BaseAdd; 
		}
	 }
	//-------------------------------------------------------------
	else if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <= 0x7E)	  //如果是ASCII
	{ 
	  if(word_size==1)		//8*16
	    {
	      address1 = (lowbyte-0x20)*16; 
	      address4 = address1+A8BaseAdd;
		}
	  else if(word_size==2)//12*24
	    {
	      address1 = (lowbyte-0x20)*48; 
	      address4 = address1+A12BaseAdd;	    
		}
	  else if(word_size==3)//16*32
	    {
	      address1 = (lowbyte-0x20)*64; 
	      address4 = address1+A16BaseAdd;

		}
	}
	//-------------------------------------------------------------
    GT23_32Address=address4;
    GT23_16HAddress=GT23_32Address/65536;
    GT23_16LAddress=GT23_32Address%65536;
    GT23L_CS=0;
    SPI_WriteByte(0x03);
    SPI_WriteByte(GT23_16HAddress%256);
    SPI_WriteByte(GT23_16LAddress/256);
    SPI_WriteByte(GT23_16LAddress%256);
	
	if(highbyte >=0xa1&&highbyte<=0xF7&&lowbyte>=0xA1)
	 {
      if(word_size==1)//15*16
       {
         for(GT23_Rec_Length=0;GT23_Rec_Length<32;GT23_Rec_Length++)
         GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
       }
      if(word_size==2)//24*24
       {
         for(GT23_Rec_Length=0;GT23_Rec_Length<72;GT23_Rec_Length++)
         GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
       }	  
      else if(word_size==3)//32*32
      {
        for(GT23_Rec_Length=0;GT23_Rec_Length<128;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
      }
     }
	else if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <= 0x7E)	
	{
	 if(word_size==1)//8*16
       {
         for(GT23_Rec_Length=0;GT23_Rec_Length<16;GT23_Rec_Length++)
         GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
       }
      else if(word_size==2)//12*24
      {
        for(GT23_Rec_Length=0;GT23_Rec_Length<48;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
      }
	  else if(word_size==3)//16*32
      {
        for(GT23_Rec_Length=0;GT23_Rec_Length<64;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
      }
	}   
    GT23L_CS=1;
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
	GT23L_CS=0;
//	Send_Data((uint8 *)word,2);
	highbyte=*word;
	word++;
	lowbyte= *word;

	WDT_CONTR = 0x3d; 

	if(highbyte<0xa1) //是ASCII字符
	{
		lowbyte=highbyte;
		highbyte=0x00;	//为了跟下面一致
	}
	
	if(highbyte >=0xA1&&highbyte<=0xA9&&lowbyte>=0xA1)	  //如果是全角字符,数字 
	{
		 if(word_size==1) //15*16
		  {
			address1 = (highbyte-0xa1)*94; 
			address2 = (lowbyte-0xA1)+address1;
			address3 = address2*32;
			address4 = address3+C16BaseAdd;
		  }
		else if(word_size==2)//24*24
		 {
			address1 = (highbyte-0xa1)*94; 
			address2 = (lowbyte-0xA1)+address1;
			address3 = address2*72;
			address4 = address3+C24BaseAdd;
		 }
		 else if(word_size==3)//32*32
		 {
			 address1 = (highbyte-0xa1)*94; 
			 address2 = (lowbyte-0xA1)+address1;
			 address3 = address2*128;
			 address4 = address3+C32BaseAdd; 
		 }

	 }

	
     else if(highbyte >=0xB0&&highbyte<=0xF7&&lowbyte>=0xA1)    //如果是汉字 
	 {
		if(word_size==1) //15*16
		 {
		   address1 = (highbyte-0xB0)*94; 
		   address2 = lowbyte-0xA1+846+address1;
		   address3 = address2*32;
		   address4 = address3+C16BaseAdd;
		 }
	   else if(word_size==2)//24*24
	    {
	      address1 = (highbyte-0xB0)*94; 
	      address2 = lowbyte-0xA1+846+address1;
	      address3 = address2*72;
	      address4 = address3+C24BaseAdd;
		}
		else if(word_size==3)//32*32
		{
		  address1 = (highbyte-0xB0)*94; 
	      address2 = lowbyte-0xA1+846+address1;
	      address3 = address2*128;
	      address4 = address3+C32BaseAdd; 
		}
	  }
	//-------------------------------------------------------------
	else if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <= 0x7E)	  //如果是ASCII
	{ 
	  if(word_size==1)		//8*16
	    {
	      address1 = (lowbyte-0x20)*16; 
	      address4 = address1+A8BaseAdd;
		}
	  else if(word_size==2)//12*24
	    {
	      address1 = (lowbyte-0x20)*48; 
	      address4 = address1+A12BaseAdd;	    
		}
	  else if(word_size==3)//16*32
	    {
	      address1 = (lowbyte-0x20)*64; 
	      address4 = address1+A16BaseAdd;

		}
	}
	//-------------------------------------------------------------
    GT23_32Address=address4;
    GT23_16HAddress=GT23_32Address/65536;
    GT23_16LAddress=GT23_32Address%65536;
    GT23L_CS=0;
    SPI_WriteByte(0x03);
    SPI_WriteByte(GT23_16HAddress%256);
    SPI_WriteByte(GT23_16LAddress/256);
    SPI_WriteByte(GT23_16LAddress%256);
	
	if(highbyte >=0xa1&&highbyte<=0xF7&&lowbyte>=0xA1)
	 {
      if(word_size==1)//15*16
       {
         for(GT23_Rec_Length=0;GT23_Rec_Length<32;GT23_Rec_Length++)
         GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
       }
      if(word_size==2)//24*24
       {
         for(GT23_Rec_Length=0;GT23_Rec_Length<72;GT23_Rec_Length++)
         GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
       }	  
      else if(word_size==3)//32*32
      {
        for(GT23_Rec_Length=0;GT23_Rec_Length<128;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
      }
     }
	else if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <= 0x7E)	
	{
	 if(word_size==1)//8*16
       {
         for(GT23_Rec_Length=0;GT23_Rec_Length<16;GT23_Rec_Length++)
         GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
       }
      else if(word_size==2)//12*24
      {
        for(GT23_Rec_Length=0;GT23_Rec_Length<48;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
      }
	  else if(word_size==3)//16*32
      {
        for(GT23_Rec_Length=0;GT23_Rec_Length<64;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=Rec_GT23_Data();
      }
	}   

    GT23L_CS=1;
}
