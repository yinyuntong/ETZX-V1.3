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
	MP3_DREQ = 1;				  //51��Ƭ��IO������ʱ����Ϊ1��vs1003������������
	while(!MP3_DREQ);			  //VS1003��DREQΪ�ߵ�ƽʱ�Ž�������
	MP3_XCS	= 0;				  //��Ƭѡ��SCI��Ч���������ܶԹ��ܼĴ������ж�д
	SPI_WriteByte(0x02);		  //д�������0x02	 00000010 �����ܼĴ���д������
	SPI_WriteByte(addr);		  //д��Ĵ�����ַ
	SPI_WriteByte(cmd_H);		  //д����ֽ�
	SPI_WriteByte(cmd_L);		  //д����ֽ� 
	MP3_XCS	= 1;				  //�����һλ������������ʱ���ѷ��ͣ����뽫XCS���������д������
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
	while(!MP3_DREQ);			 //VS1003��DREQΪ�ߵ�ƽʱ�Ž�������
	MP3_XCS	= 0;				 //��Ƭѡ��SCI��Ч
	SPI_WriteByte(0x03);		 //����������0x03	00000011�����ܼĴ�����������
	SPI_WriteByte(addr);		 //д��Ĵ�����ַ
	temp   = SPI_WriteByte(0xff);	 //�����ֽ�
	temp <<= 8;
	temp  |= SPI_WriteByte(0xff);	 //��ȡ���ֽڣ�����ֽ�ƴ��һ����
	MP3_XCS	= 1;				 //�ر�Ƭѡ��SCI��Ч
	return temp;				 //���ض�����ֵ

}

  
/*---------------------------------------------------------*
* �� �� ����VS1003Init()
* �������ܣ�vs1003��ʼ��
* �����������
* �� �� ֵ����
* ���ú�����VS1003WriteRegistor(),VS1003SPIWrite()
* ˵    ����vs1003Ӳ����λ���͵�ƽ��Ч
*---------------------------------------------------------*/
void VS1003Init(void)
{
  MP3_XCS =1;
  MP3_XDCS =1;
  MP3_DREQ = 1;
  
  MP3_XRESET = 1;	  
  delay_nms(100);
  MP3_XRESET = 0;						  //Ӳ��λ��XRESET�͵�ƽ��Ч,reset = 1
  delay_nms(100);
  MP3_XRESET = 1;
  delay_nms(100);

  Mp3WriteRegister(0x00,0x08,0x04);   //�����λ��0�żĴ���д0x0804SPI_MODE=0x0804
  Mp3WriteRegister(0x02,0x00,0x55);   //�ص������ã�2�żĴ���0-3bit���Ƶ�ޣ�10Hz����,4-7bit,�������أ�1db������8-11�����Ƶ��1000Hz����12-15����������
  Mp3WriteRegister(0x03,0x98,0x00);   //ʱ�����ã�3�żĴ���д0x9800��

  Mp3WriteRegister(0x05,0xbb,0x81); 	  //���������ã�5�żĴ���д0xbb81��������48k��������
  //VS1003WriteRegistor(0x0b,0x25,0x25);  //�������ã��߰�λ�������������Ͱ�λ���������������Ϊ0x00,��СΪ0xff
  Mp3WriteRegister(0x0b,volumn,volumn);

  
  MP3_XDCS = 0; 						  //������Ƭѡ��ע���ʱXCS��Ƭѡ��Ϊ�ߵ�ƽ��SDI��Ч
  SPI_WriteByte(0); 					  //д�����ݣ�����д��4��0�����޹����ݣ������������ݴ���
  SPI_WriteByte(0);
  SPI_WriteByte(0);
  SPI_WriteByte(0);
  MP3_XDCS = 1; 						  //�ر�����Ƭѡ


//	ConsoleWrite("vs1003b reset finish\r");							 
}
/*---------------------------------------------------------*

/*---------------------------------------------------------*
* �� �� ����VS1003SendDat()
* �������ܣ��������͵�vs1003�н��н��벥��
* ���������dat���͵�vs1003���ݻ�����������
* �� �� ֵ����
* ���ú�����VS1003SPIWrite()
* ˵    ������vs1003д��һ���ֽڵ�����
*---------------------------------------------------------*/
void VS1003SendDat(uint8 dat)
{
//  MP3_DREQ = 1;
  while(!MP3_DREQ);
  SPI_WriteByte(dat);

}
/*---------------------------------------------------------*/

/*---------------------------------------------------------*
* �� �� ����VS1003FlushBuffer()
* �������ܣ���vs1003�����ݻ�����
* ���ú�����VS1003SendDat
* �����������
* �� �� ֵ����
* ˵    ����
*---------------------------------------------------------*/
void VS1003FlushBuffer(void)
{
  uint16 i;
  MP3_XDCS = 0; 				//������Ƭѡ��������SDI����
  for(i = 0; i < 2048; i++)
  {
	  VS1003SendDat(0); 		//�����ݻ��������������
  }
  MP3_XDCS = 1; 				 //�ر�����Ƭѡ
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
	MP3_XRESET = 0; 						//Ӳ��λ��XRESET�͵�ƽ��Ч,reset = 1
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
	SPI_WriteByte(0x00);		//2KƵ��
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



