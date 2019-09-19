/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:I2C.c
*文件描述    		:I2C驱动程序
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
#define _IN_I2C_
#include "config.h"

void Delay_us(uchar cnt);
void Start();
void Stop();
void WriteACK(uchar ack);
void WaitACK();
void writebyte(uchar wdata);
uchar Readbyte();
void writeData(uchar address,uchar mdata);
void writeDataStr(uchar address,uchar count,uchar * buf);
uchar ReadData(uchar address);
void ReadDataStr(uchar address,uchar count,uchar * buff);

/********************************************
内部函数，延时1us
********************************************/
void Delay_us(uchar cnt)
{ /*根据晶振频率制定延时时间*///18.432M
	uchar i;
	for(i=0; i<cnt; i++)
	{
		_nop_();
 		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
 		_nop_();
		_nop_();
		_nop_();


		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();

	}

	return;
}

/********************************************
内部函数，I2C开始
********************************************/
void Start()
{  
   SCL=0;
   Delay_us(10);

   SDA=1;
   Delay_us(10);
   SCL=1;
   Delay_us(10);
   SDA=0;
   Delay_us(10);
   SCL=0;
   Delay_us(10);
}

/********************************************
内部函数，I2C结束
********************************************/
void Stop()
{
   SCL=0;
   Delay_us(10);
   SDA=0;

   Delay_us(10);
   SCL=1;
   Delay_us(10);
   SDA=1;
   Delay_us(10);
   SCL=0;

}
/********************************************
内部函数，输出ACK ,每个字节传输完成，输出ack=0,结束读书据，ack=1;
********************************************/
void WriteACK(uchar ack)
{

   SCL=0;
   Delay_us(10);
   SDA=ack;
   Delay_us(10);
   SCL=1;
   Delay_us(10);
   SCL=0;
   Delay_us(10);
}
/********************************************
内部函数，等待ACK
********************************************/
void WaitACK()
{  
   uint errtime=2550;
   SDA=1;
   SCL=0;
   Delay_us(10); /*读ACK*/
   SCL=1;
   Delay_us(10);
   while(SDA)
   {  errtime--;
      if(!errtime) 
	  {
	  	Stop();
 		SCL=0;
		return;
	  }
   }
   SCL=0;
   Delay_us(10);
   return;
}
/********************************************
内部函数.输出数据字节
入口:B=数据
********************************************/
void writebyte(uchar wdata)
{
   uchar i;
   SCL=0;
   for(i=0;i<8;i++)
   {
   	   Delay_us(10);
       if(wdata&0x80) SDA=1;
       else SDA=0;
       wdata<<=1;
	   Delay_us(10);
       SCL=1;
       Delay_us(10);
       SCL=0;
   }
   SDA=1;
   WaitACK();     //I2C器件或通讯出错，将会退出I2C通讯
}
/********************************************
内部函数.输入数据
出口:B
********************************************/
uchar Readbyte()
{
   uchar i,bytedata;
   SDA=1;
   for(i=0;i<8;i++)
   {
      SCL=1; 
      bytedata<<=1;
      bytedata|=SDA;
      SCL=0;
      Delay_us(10);
   }
   return(bytedata);
}
/********************************************
输出数据->pcf8563
********************************************/
void writeData(uchar address,uchar mdata)
{
   Start();
   writebyte(0xa2); /*写命令*/
   writebyte(address); /*写地址*/
   writebyte(mdata); /*写数据*/
   Stop();
}

/********************************************
输出数据->pcf8563
********************************************/
void writeDataStr(uchar address,uchar count,uchar * buff) /*多字节*/
{
   uchar i;
   Start();
   writebyte(0xa2); /*写命令*/
   writebyte(address); /*写地址*/
   for(i=0;i<count;i++)
   	{
   		writebyte(*buff); /*写数据*/
		buff++;
   	}
   
   Stop();
}


/********************************************
输入数据<-pcf8563
********************************************/
uchar ReadData(uchar address) /*单字节*/
{  uchar rdata;
   Start();
   writebyte(0xa2); /*写命令*/
   writebyte(address); /*写地址*/
   Start();
   writebyte(0xa3); /*读命令*/
   rdata=Readbyte();
   WriteACK(1);
   Stop();
   return(rdata);
}

void ReadDataStr(uchar address,uchar count,uchar * buff) /*多字节*/
{  uchar i;
   Start();
   writebyte(0xa2); /*写命令*/
   writebyte(address); /*写地址*/
   Start();
   writebyte(0xa3); /*读命令*/
   for(i=0;i<count;i++)
   {
        buff[i]=Readbyte();
        if(i<count-1) WriteACK(0);
   }
  WriteACK(1);
   Stop();
}
