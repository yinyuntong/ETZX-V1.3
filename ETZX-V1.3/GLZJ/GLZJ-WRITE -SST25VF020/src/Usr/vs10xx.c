/** \file vs10xx.c
 * Functions for interfacing with the mp3 player chip.
 * \todo safe rewind
 * \todo VS1003 WMA "wma-bytes-left" variable adjustment at ff/rew
 */
#define _IN_VS10XX_
#include "config.h"


/** Write VS10xx register*/
void Mp3WriteRegister(uint8 addr,uint8 cmd_H,uint8 cmd_L)
{ 
	MP3_DREQ = 1;				  //51单片机IO作输入时先置为1，vs1003进行数据请求
	while(!MP3_DREQ);			  //VS1003的DREQ为高电平时才接收数据
	MP3_XCS	= 0;				  //打开片选，SCI有效，这样才能对功能寄存器进行读写
	SPI_WriteByte(0x02);		  //写入操作码0x02	 00000010 （功能寄存器写操作）
	SPI_WriteByte(addr);		  //写入寄存器地址
	SPI_WriteByte(cmd_H);		  //写入高字节
	SPI_WriteByte(cmd_L);		  //写入低字节 
	MP3_XCS	= 1;				  //当最后一位被移入且最后的时钟已发送，必须将XCS拉高以完成写操作。
}

/** Set VS10xx Volume Register */
void Mp3SetVolume(uint8 leftchannel,uint8 rightchannel)
{
 	Mp3WriteRegister(11,(leftchannel),(rightchannel));
}


/** Read the 16-bit value of a VS10xx register */
unsigned int Mp3ReadRegister (unsigned char addr)
{
	uint16 temp = 0;
	MP3_DREQ = 1;
	while(!MP3_DREQ);			 //VS1003的DREQ为高电平时才接收数据
	MP3_XCS	= 0;				 //打开片选，SCI有效
	SPI_WriteByte(0x03);		 //读出操作码0x03	00000011（功能寄存器读操作）
	SPI_WriteByte(addr);		 //写入寄存器地址
	temp   = SPI_WriteByte(0xff);	 //读高字节
	temp <<= 8;
	temp  |= SPI_WriteByte(0xff);	 //读取低字节，与高字节拼成一个字
	MP3_XCS	= 1;				 //关闭片选，SCI无效
	return temp;				 //返回读到的值

}

  
/*---------------------------------------------------------*
* 函 数 名：VS1003Init()
* 函数功能：vs1003初始化
* 输入变量：无
* 返 回 值：无
* 调用函数：VS1003WriteRegistor(),VS1003SPIWrite()
* 说    明：vs1003硬件复位，低电平有效
*---------------------------------------------------------*/
void VS1003Init(void)
{
  MP3_XCS =1;
  MP3_XDCS =1;
  MP3_DREQ = 1;
  
  MP3_XRESET = 1;	  
  delay_nms(100);
  MP3_XRESET = 0;						  //硬复位，XRESET低电平有效,reset = 1
  delay_nms(100);
  MP3_XRESET = 1;
  delay_nms(100);

  Mp3WriteRegister(0x00,0x08,0x04);   //软件复位，0号寄存器写0x0804SPI_MODE=0x0804
  Mp3WriteRegister(0x02,0x00,0x55);   //重低音设置，2号寄存器0-3bit最低频限，10Hz步进,4-7bit,低音加重，1db步进，8-11，最低频限1000Hz步进12-15，高音控制
  Mp3WriteRegister(0x03,0x98,0x00);   //时钟设置，3号寄存器写0x9800，

  Mp3WriteRegister(0x05,0xbb,0x81); 	  //立体声设置，5号寄存器写0xbb81，采样率48k，立体声
  //VS1003WriteRegistor(0x0b,0x25,0x25);  //音量设置，高八位设置左声道，低八位设置右声道，最大为0x00,最小为0xff
  Mp3WriteRegister(0x0b,volumn,volumn);

  
  MP3_XDCS = 0; 						  //打开数据片选，注意此时XCS（片选）为高电平，SDI有效
  SPI_WriteByte(0); 					  //写入数据，这里写入4个0，是无关数据，用来启动数据传输
  SPI_WriteByte(0);
  SPI_WriteByte(0);
  SPI_WriteByte(0);
  MP3_XDCS = 1; 						  //关闭数据片选


//	ConsoleWrite("vs1003b reset finish\r");							 
}
/*---------------------------------------------------------*

/*---------------------------------------------------------*
* 函 数 名：VS1003SendDat()
* 函数功能：将数据送到vs1003中进行解码播放
* 输入变量：dat，送到vs1003数据缓冲区的数据
* 返 回 值：无
* 调用函数：VS1003SPIWrite()
* 说    明：往vs1003写入一个字节的数据
*---------------------------------------------------------*/
void VS1003SendDat(uint8 dat)
{
//  MP3_DREQ = 1;
  while(!MP3_DREQ);
  SPI_WriteByte(dat);

}
/*---------------------------------------------------------*/

/*---------------------------------------------------------*
* 函 数 名：VS1003FlushBuffer()
* 函数功能：清vs1003的数据缓冲区
* 调用函数：VS1003SendDat
* 输入变量：无
* 返 回 值：无
* 说    明：
*---------------------------------------------------------*/
void VS1003FlushBuffer(void)
{
  uint16 i;
  MP3_XDCS = 0; 				//打开数据片选，即开启SDI传输
  for(i = 0; i < 2048; i++)
  {
	  VS1003SendDat(0); 		//将数据缓冲区的数据清除
  }
  MP3_XDCS = 1; 				 //关闭数据片选
}
/*---------------------------------------------------------*/

// void sub_volumn() interrupt 0
//add the volume size of vol 
void vsl003_volumn_modify(uint16 vol)
{
  if(vol)
  {   
	  //add the volumn
	  if(volumn+vol > 0)
	  {
		  volumn += vol;
		  MP3_XDCS = 1;
		  Mp3WriteRegister(0x0b,volumn,volumn);
		  MP3_XDCS = 0;
	  }
  }
  else
  {
	  //sub the volume
	  if(volumn+vol < 0xff)
	  {
		  volumn += vol;
		  MP3_XDCS = 1;
		  Mp3WriteRegister(0x0b,volumn,volumn);
		  MP3_XDCS = 0;
	  }
  }
}

void sub_volumn()
{
  if(volumn+2 < 0xff)
  {
	  volumn += 2;
	  MP3_XDCS = 1;
	  Mp3WriteRegister(0x0b,volumn,volumn);
	  MP3_XDCS = 0;
  }
}
//add the volumn
//void add_volumn() interrupt 2
void add_volumn()
{
  if(volumn-2 > 0)
  {
	  volumn -= 2;
	  MP3_XDCS = 1;
	  Mp3WriteRegister(0x0b,volumn,volumn);
	  MP3_XDCS = 0;
  }
}
  



/** VS10xx Sine Test Function - Good getting started example */ 
void VsSineTest()
{
	MP3_XCS =1;
	MP3_XDCS =1;
	
	MP3_XRESET = 1; 	
	delay_nms(100);
	MP3_XRESET = 0; 						//硬复位，XRESET低电平有效,reset = 1
	delay_nms(100);
	MP3_XRESET = 1;
	delay_nms(100);



	/* VS10xx Application Notes, chapter 4.8 ---------------------------------*/
	/* As an example, let's write value 0x0820 to register 00 byte by byte    */
	Mp3WriteRegister(0x00,0x08,0x20);
  	PRG_STATE	= 0;

	/* Send a Sine Test Header to Data port                                   */
	Mp3SelectData();       /* Now SPI writes go to SDI port                   */

	SPI_WriteByte(0x53);      /* - This is a special VLSI Solution test header - */
	SPI_WriteByte(0xef);      /* - that starts a sine sound. It's good for     - */
	SPI_WriteByte(0x6e);      /* - testing your code, the chip and also for    - */
	SPI_WriteByte(0x44);      /* - seeing if your MP3 decoder was manufactured - */
	SPI_WriteByte(0x00);      /* - by VLSI Solution oy. ------------------------ */
	SPI_WriteByte(0x00);		//2K频率
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x00);
	Mp3DeselectData();
	delay_nms(500);           /* 500 ms delay */


	while(1);

	/* Stop the sine test sound */
	Mp3SelectData();
	SPI_WriteByte(0x45);
	SPI_WriteByte(0x78);
	SPI_WriteByte(0x69);
	SPI_WriteByte(0x74);
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x00);
	SPI_WriteByte(0x00);
	Mp3DeselectData();

  	delay_nms(500);           /* 500 ms delay */
}  



