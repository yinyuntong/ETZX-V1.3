/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:I2C.c
*�ļ�����    		:I2C��������
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
�ڲ���������ʱ1us
********************************************/
void Delay_us(uchar cnt)
{ /*���ݾ���Ƶ���ƶ���ʱʱ��*///18.432M
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
�ڲ�������I2C��ʼ
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
�ڲ�������I2C����
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
�ڲ����������ACK ,ÿ���ֽڴ�����ɣ����ack=0,��������ݣ�ack=1;
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
�ڲ��������ȴ�ACK
********************************************/
void WaitACK()
{  
   uint errtime=2550;
   SDA=1;
   SCL=0;
   Delay_us(10); /*��ACK*/
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
�ڲ�����.��������ֽ�
���:B=����
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
   WaitACK();     //I2C������ͨѶ���������˳�I2CͨѶ
}
/********************************************
�ڲ�����.��������
����:B
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
�������->pcf8563
********************************************/
void writeData(uchar address,uchar mdata)
{
   Start();
   writebyte(0xa2); /*д����*/
   writebyte(address); /*д��ַ*/
   writebyte(mdata); /*д����*/
   Stop();
}

/********************************************
�������->pcf8563
********************************************/
void writeDataStr(uchar address,uchar count,uchar * buff) /*���ֽ�*/
{
   uchar i;
   Start();
   writebyte(0xa2); /*д����*/
   writebyte(address); /*д��ַ*/
   for(i=0;i<count;i++)
   	{
   		writebyte(*buff); /*д����*/
		buff++;
   	}
   
   Stop();
}


/********************************************
��������<-pcf8563
********************************************/
uchar ReadData(uchar address) /*���ֽ�*/
{  uchar rdata;
   Start();
   writebyte(0xa2); /*д����*/
   writebyte(address); /*д��ַ*/
   Start();
   writebyte(0xa3); /*������*/
   rdata=Readbyte();
   WriteACK(1);
   Stop();
   return(rdata);
}

void ReadDataStr(uchar address,uchar count,uchar * buff) /*���ֽ�*/
{  uchar i;
   Start();
   writebyte(0xa2); /*д����*/
   writebyte(address); /*д��ַ*/
   Start();
   writebyte(0xa3); /*������*/
   for(i=0;i<count;i++)
   {
        buff[i]=Readbyte();
        if(i<count-1) WriteACK(0);
   }
  WriteACK(1);
   Stop();
}
