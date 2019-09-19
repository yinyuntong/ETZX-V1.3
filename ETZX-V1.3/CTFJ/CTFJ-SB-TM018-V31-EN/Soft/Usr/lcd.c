#define _IN_LCD_
#include "config.h"


uchar xdata fontbuf1,fontbuf2;
uint8 xdata ASCII_BCD[3];
//uint16 xdata DispPageAddr[10];

uint8 xdata DispDataBuff[160];
uint8 xdata CurrentPage;

uint8 code ColonCode16x32[64]={	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x03,0xC0,0x03,0xC0,
								0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,
						   		0x03,0xC0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   		0x00,0x00,0x00,0x00,0x00,0x00,};/*":",0*/


bit bSecondDisState;	//��̬�������״̬


extern STLocalControl xdata stLocalControl;

uint16 xdata DisplayedLen=0;



void Delayms(uint ms);
void Send_ST7735_Command(uchar st7735_command);
void Send_ST7735_Data(uchar st7735_data);
void ST7735_Init(void);
void Set_Display_Area(uchar start_x,uchar end_x,uchar start_y,uchar end_y);
void Lcd_Clear(uint color);
void Display_Dot(uchar x,uchar y,uint color);  
void Display_HLine(uchar x,uchar y,uchar width,uint color);
void Display_VLine(uchar x,uchar y,uchar height,uint color);
void Display_ASCII(uint8 x,uint8 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size);
void Display_Chinese(uint8 x,uint8 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size);
void Lcd_Fill(uchar start_x,uchar start_y,uchar width,uchar height,uint color);




void Delayms(uint ms)	  
{
   uint i;
   for(;ms!=0;ms--)
   	{
		for(i=900;i!=0;i--);
		WDT_CONTR = 0x3d;	//ι��
   	}
}


void Send_ST7735_Command(uchar st7735_command)
{
 //  SpiLcdMode();
   LCD_CS7=0;
   LCD_RS=0;

   
 /*
   LCD_RD=1;
   LCD_WR=0;
   LCD_DATA=st7735_command;
   LCD_WR=1;
  */
   SPI_WriteByte(st7735_command);
 
   LCD_CS7=1;
   LCD_RS=1;
//   SpiNormalMode();
}



void Send_ST7735_Data(uchar st7735_data)
{
//   SpiLcdMode();
   LCD_CS7=0;
   LCD_RS=1;

   
/* LCD_RD=1;
   LCD_WR=0;
   LCD_DATA=st7735_data;
   LCD_WR=1;
 */
    SPI_WriteByte(st7735_data);

   LCD_CS7=1;
   LCD_RS=0;
//   SpiNormalMode();   
}




//����1.7��Һ����TM018ʹ��ILI9163C����оƬ

void ST7735_Init(void)
{
  LCD_RST=1;             
  Delayms(1);
  LCD_RST=0;
  Delayms(1);
  LCD_RST=1;
  Delayms(2); 


  Send_ST7735_Command(0x11); //Exit Sleep
  Delayms(120);
  Send_ST7735_Command(0x26); //Set Default Gamma
  Send_ST7735_Data(0x04);
  Send_ST7735_Command(0xF2); //E0h & E1h Enable/Disable
  Send_ST7735_Data(0x00);
  Send_ST7735_Command(0xB1);//Set Frame Rate
  Send_ST7735_Data(0x0C);
  Send_ST7735_Data(0x14);
  Send_ST7735_Command(0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
  Send_ST7735_Data(0x0C);
  Send_ST7735_Data(0x05);
  Send_ST7735_Command(0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
  Send_ST7735_Data(0x02);
  Send_ST7735_Command(0xC5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
  Send_ST7735_Data(0x44);
  Send_ST7735_Data(0x52);
  Send_ST7735_Command(0xC7);
  Send_ST7735_Data(0xB7);
  //************* Scan direction **********//
  Send_ST7735_Command(0x2A); //Set Column Address
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(0x7F);
  Send_ST7735_Command(0x2B); //Set Page Address
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(0x9F);
  Send_ST7735_Command(0x36); //Set Scanning Direction & BGR
  Send_ST7735_Data(0xC8);
  Send_ST7735_Command(0x3a); //Set Pixel
  Send_ST7735_Data(0x55);
  //************* Gamma Setting **********//
  Send_ST7735_Command(0xf2); //Enable Gamma bit
  Send_ST7735_Data(0x01);
  Send_ST7735_Command(0xE0);
  Send_ST7735_Data(0x3F);//p1
  Send_ST7735_Data(0x1B);//p2
  Send_ST7735_Data(0x19);//p3
  Send_ST7735_Data(0x27);//p4
  Send_ST7735_Data(0x1F);//p5
  Send_ST7735_Data(0x0D);//p6
  Send_ST7735_Data(0x45);//p7
  Send_ST7735_Data(0xB8);//p8
  Send_ST7735_Data(0x30);//p9
  Send_ST7735_Data(0x17);//p10
  Send_ST7735_Data(0x0F);//p11
  
  Send_ST7735_Data(0x05);//p12
  Send_ST7735_Data(0x13);//p13
  Send_ST7735_Data(0x02);//p14
  Send_ST7735_Data(0x00);//p15
  Send_ST7735_Command(0xE1);
  Send_ST7735_Data(0x00);//p1
  Send_ST7735_Data(0x24);//p2
  Send_ST7735_Data(0x26);//p3
  Send_ST7735_Data(0x08);//p4
  Send_ST7735_Data(0x10);//p5
  Send_ST7735_Data(0x12);//p6
  Send_ST7735_Data(0x3A);//p7
  Send_ST7735_Data(0x74);//p8
  Send_ST7735_Data(0x4F);//p9
  Send_ST7735_Data(0x08);//p10
  Send_ST7735_Data(0x20);//p11
  Send_ST7735_Data(0x2A);//p12
  Send_ST7735_Data(0x3C);//p13
  Send_ST7735_Data(0x3D);//p14
  Send_ST7735_Data(0x3F);//p15
  Send_ST7735_Command(0x29); // Display On
  Send_ST7735_Command(0x2C);//Memory Write
  Delayms(100);
}
/*--------------------------------------------------------------------------------------------
����ԭ��: void Set_Display_Area(uint start_x,uint end_x,uint start_y,uint end_y) 
����˵��: start_x:���X��ʼ����
          end_x:���X��������
		  start_y:���Y��ʼ����
          end_y:���Y��������
�� �� ֵ: ��
��������: ������Ļ����
----------------------------------------------------------------------------------------------*/
//SH-BL17
/*void Set_Display_Area(uchar start_x,uchar end_x,uchar start_y,uchar end_y)
{
  Send_ST7735_Command(0x2A);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(start_x+2);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(end_x+2);
  Send_ST7735_Command(0x2B);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(start_y+1);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(end_y+1);
}*/


//p-ts7017��ʾ��
void Set_Display_Area(uchar start_x,uchar end_x,uchar start_y,uchar end_y)
{
  Send_ST7735_Command(0x2A);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(start_x);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(end_x);
  Send_ST7735_Command(0x2B);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(start_y);
  Send_ST7735_Data(0x00);
  Send_ST7735_Data(end_y);
}
/*--------------------------------------------------------------------------------------------
����ԭ��: void Lcd_Clear(uint color)
����˵��: color:��������ɫ
�� �� ֵ: ��
��������: ����
----------------------------------------------------------------------------------------------*/
void Lcd_Clear(uint color)
{
  uchar xdata i,j;
  Set_Display_Area(0x00,0x7F,0x00,0x9F);
  Send_ST7735_Command(0x2C);
  for(i=0;i<160;i++)
    {
     for(j=0;j<128;j++)
     {
      Send_ST7735_Data(color/256);
	  Send_ST7735_Data(color%256);
	 }
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------
����ԭ��: void Lcd_Fill(uchar start_x,uchar start_y,uchar width,uchar height,uint color)
����˵��: start_x--x�����ʼ����ֵ
          start_y--y�����ʼ����ֵ
		  width  --���Ŀ��
		  height --���ĸ߶�
		  color  --��������ɫ
�� �� ֵ: ��
�������ܣ������
-----------------------------------------------------------------------------------------------------------------------------------------------*/ 
void Lcd_Fill(uchar start_x,uchar start_y,uchar width,uchar height,uint color)
{
  uchar xdata i,j;
  Set_Display_Area(start_x,start_x+width-1,start_y,start_y+height-1);
  Send_ST7735_Command(0x2C);
   for(i=0;i<height;i++)
    {
     for(j=0;j<width;j++)
     {
      Send_ST7735_Data(color/256);
	  Send_ST7735_Data(color%256);
	 }
   }
}

void DisplayBlock(uint8 start_x,uint8 start_y,uint8 end_x,uint8 end_y, uint8 srgb)
{
	uint16 xdata color;
	uint8 xdata width,height;
	uint8 i,j;
	switch(srgb)
	{
		case BLACK_CODE:
			color=BLACK;
			break;
		case BLUE_CODE:
			color=BLUE;
			break;	
		case GREEN_CODE:
			color=GREEN;
			break;	
		case CYAN_CODE:
			color=CYAN;
			break;
		case RED_CODE:
			color=RED;
			break;
		case PURPLE_CODE:
			color=PURPLE;
			break;
		case YELLOW_CODE:
			color=YELLOW;
			break;
		default:
		case WHITE_CODE:
			color=WHITE;
			break;		
	}

  Set_Display_Area(start_x,end_x,start_y,end_y);
  Send_ST7735_Command(0x2C);
  width=end_x-start_x+1;
  height=end_y-start_y+1;
  for(j=0;j<height;j++)
  {
  	for(i=0;i<width;i++)
  	{
		Send_ST7735_Data(color/256);
		Send_ST7735_Data(color%256);
  	}
  } 
  
}

/*------------------------------------------------------------------------------
����ԭ��: void Display_Dot(uchar x,uchar y,uint color)    
����˵��: x:���X����,y:���Y����,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ��
-------------------------------------------------------------------------------*/
void Display_Dot(uchar x,uchar y,uint color)  
{  
  Set_Display_Area(x,x,y,y);
  Send_ST7735_Command(0x2C);
  Send_ST7735_Data(color/256);
  Send_ST7735_Data(color%256);
}
/*-------------------------------------------------------------------------------------------------
����ԭ��: void Display_HLine(uchar x,uchar y,uchar width,uint color)
����˵��: x:���X����,y:���Y����,width:ˮƽֱ�ߵĳ���,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾˮƽֱ��
--------------------------------------------------------------------------------------------------*/
void Display_HLine(uchar x,uchar y,uchar width,uint color)
{
  Set_Display_Area(x,x+width-1,y,y);
  Send_ST7735_Command(0x2C);
  while(width--)
  {
  	WDT_CONTR = 0x3d;	//ι��
    Send_ST7735_Data(color/256);
	Send_ST7735_Data(color%256);
  }
}
/*--------------------------------------------------------------------------------------------------
����ԭ��: void Display_VLine(uchar x,uchar y,uchar height,uint color)
����˵��: x:���X����,y:���Y����,height:��ֱֱ�ߵĳ���,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ��ֱֱ��
----------------------------------------------------------------------------------------------------*/
void Display_VLine(uchar x,uchar y,uchar height,uint color)
{
  Set_Display_Area(x,x,y,y+height-1);
  Send_ST7735_Command(0x2C);
  while(height--)
  {
  	WDT_CONTR = 0x3d;	//ι��
    Send_ST7735_Data(color/256);
	Send_ST7735_Data(color%256);
  }
} 

/*--------------------------------------------------------------------------------------------------------------------------------------
����ԭ��: void Display_ASCII(uint16 xdata x,uint16 xdata y,uint8 xdata *p,uint32 xdata fcolor,uint32 xdata bcolor)
����˵��: x:�ַ���X����,y:�ַ���Y����,frcolor:�ַ�����ɫ��brcolor:�ַ��ı�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ�ַ�
----------------------------------------------------------------------------------------------------------------------------------------*/
 void Display_ASCII(uint8 x,uint8 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size)
{

  uint8 xdata i,j;
  switch(size)
   {
   case 1: //8*16
	   Set_Display_Area(x,x+7,y,y+15);
	   i=16;
	   break;
   case 2: //12*24
	   Set_Display_Area(x,x+15,y,y+23);
	   i=48;
	   break;
   case 3: //16*32
	   Set_Display_Area(x,x+15,y,y+31);
	   i=64;
	   break;
   default://Ĭ��Ϊ8*16
	   Set_Display_Area(x,x+7,y,y+15);
	   i=16;
	   break;	   
   }

  Send_ST7735_Command(0x2C);
  
  for(;i>0;i--)
   {
	  for(j=0;j<8;j++)
	  {
		   if(*p&(0x80>>j))
		   {
			   fontbuf1=fcolor/256;
			   fontbuf2=fcolor%256;
		   }
		   else
		   {
			   fontbuf1=bcolor/256;
			   fontbuf2=bcolor%256;
		   }
		   Send_ST7735_Data(fontbuf1);
		   Send_ST7735_Data(fontbuf2);
	  }
	  p++;
  }
}





void Display_Chinese(uint8 x,uint8 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size)
{
  uint8 xdata i,j;
  switch(size)
   {
   case 1: //15*16
   default:
	   Set_Display_Area(x,x+15,y,y+15);
	   i=32;	//32���ֽڵ�������
	   break;

   case 3: //32*32
	   Set_Display_Area(x,x+31,y,y+31);
	   i=128;
	   break;
   
   }

   Send_ST7735_Command(0x2C);
   for(;i>0;i--)
	{
	   for(j=0;j<8;j++)
	   {
			if(*p&(0x80>>j))
			{
				fontbuf1=fcolor/256;
				fontbuf2=fcolor%256;
			}
			else
			{
				fontbuf1=bcolor/256;
				fontbuf2=bcolor%256;

			}
			Send_ST7735_Data(fontbuf1);
			Send_ST7735_Data(fontbuf2);
	   }
	   p++;
   }
 }


void ASCIIXDirectionIncrease(uint8  size,uint8 *px,uint8 *py)
{
	uint8 xdata x,y;
	x=*px;
	y=*py;
	switch(size)
	{
	case 1://8*16
	default:
		if(x+8<LCD_PIX_X) x+=8;
		else 
		{
			if(y+16<LCD_PIX_Y) {y+=16;x=0;}
			else {x=0;y=0;}
		}
		break;
	case 2://12*24
		if(x+12<LCD_PIX_X) x+=12;
		else 
		{
			if(y+24<LCD_PIX_Y) {y+=24;x=0;}
			else {x=0;y=0;}
		}
		break;
	case 3://16*32
		if(x+16<LCD_PIX_X) x+=16;
		else 
		{
			if(y+32<LCD_PIX_Y) {y+=32;x=0;}
			else {x=0;y=0;}
		}
		break;	
	}
	*px=x;
	*py=y;
}


void ChineseXDirectionIncrease(uint8 size,uint8 *px,uint8 *py)
{
	uint8 xdata x,y;
	x=*px;
	y=*py;
	switch(size)
	{
	case 1:	//15*16
	default:
		if(x+16<LCD_PIX_X) x+=16;
		else 
		{
			if(y+16<LCD_PIX_Y) {y+=16;x=0;}
			else {x=0;y=0;}
		}
		break;

	case 3://32*32
		if(x+32<LCD_PIX_X) x+=32;
		else 
		{
			if(y+32<LCD_PIX_Y) {y+=32;x=0;}
			else {x=0;y=0;}
		}
		break;

	}
	*px=x;
	*py=y;
}



void DisplayDigitString(uint8 *px,uint8 *py,uint8 *pDigit,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size)
{
	uint8 xdata x,y;
	x=*px;
	y=*py;
	for(;Length>0;Length--)
	{
		CCAPM0=0X00;	//��ֹ����	
		if(*pDigit<10) Get_GT23_Data(0x00,*pDigit+48,size);
		else Get_GT23_Data(0x00,*pDigit+55,size);
		CCAPM0=0X49;	//���ŵ���		
		Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size);

		ASCIIXDirectionIncrease(size,&x,&y);
		pDigit++;
	}
	*px=x;
	*py=y;
}


void DisplayDigit(uint8 x,uint8 y,uint8 Digit,uint16 fcolor,uint16 bcolor,uint8 size)
{
	CCAPM0=0X00;	//��ֹ����	
	if(Digit<10) Get_GT23_Data(0x00,Digit+48,size);
	else Get_GT23_Data(0x00,Digit+55,size);
	CCAPM0=0X49;	//���ŵ���		
	Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size);
}


void DisplayASCIIString(uint8 *px,uint8 *py,uint8 *pASCII,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size)
{
	uint8 xdata x,y;
	x=*px;
	y=*py;
	for(;Length>0;Length--)
	{
		CCAPM0=0X00;	//��ֹ����	
		Get_GT23_Data(0x00,*pASCII,size);
		CCAPM0=0X49;	//���ŵ���		
		Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size);

		ASCIIXDirectionIncrease(size,&x,&y);
		pASCII++;
	}
	*px=x;
	*py=y;
}

void DisplayASCII(uint8 x,uint8 y,uint8 ASCII,uint16 fcolor,uint16 bcolor,uint8 size)
{
	CCAPM0=0X00;	//��ֹ����	
	Get_GT23_Data(0x00,ASCII,size);
	CCAPM0=0X49;	//���ŵ���		
	Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size);
}


void DisplayChineseString(uint8 *px,uint8 *py,uint8 *pChinese,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size)
{
	uint8 xdata x,y;
	x=*px;
	y=*py;
	for(;Length>0;Length--)
	{
		CCAPM0=0X00;	//��ֹ����	
		Get_GT23_Data1(pChinese,size);
		CCAPM0=0X49;	//���ŵ���		
		Display_Chinese(x,y,GT23_Rec_Data,fcolor,bcolor,size);

		ChineseXDirectionIncrease(size,&x,&y);
		pChinese=pChinese+2;	//һ����������ռ�����ֽ�
	}
	*px=x;
	*py=y;
}


void DisplayString(uint8 *px,uint8 *py,uint8 *pDispData,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size)
{
	uint8 xdata x,y;
	x=*px;
	y=*py;


	for(;Length>0;)
	{	
		if((*pDispData>=0x81)&&(*pDispData<=0xfe))	//���ֽڷ��Ϻ�����������
		{
			pDispData++;
			if(*pDispData>=0x40)
			{
				if(Length>=2)
				{//��ֹ�������ݴ�ʱ���ݳ�����0  ���
					pDispData--;
					Get_GT23_Data1(pDispData,size);	

					//�Ƚ��пռ��ж�
					switch(size)
					{
						case 1:
						default:
							if(x+16> LCD_PIX_X)
							{							
								x=0;
								if(y+16>LCD_PIX_Y) y=0;
								else y +=16;
							}
							break;
						case 3:
							if(x+32> LCD_PIX_X)
							{
								x=0;
								if(y+32>LCD_PIX_Y) y=0;
								else y +=32;
							}
							break;							
					}
					Display_Chinese(x,y,GT23_Rec_Data,fcolor,bcolor,size);

					ChineseXDirectionIncrease(size,&x,&y);
					pDispData=pDispData+2;	//һ����������ռ�����ֽ�
					Length -=2;
				}
				else
				{//���Ȳ���2
					Length--;
				}
			}
			else
			{//���ֽڲ����Ϻ�������
				Length--;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e))	//����ASCII�ַ�����
		{
			Get_GT23_Data(0x00,*pDispData,size);
			//�Ƚ��пռ��ж�
			switch(size)
			{
				case 1:
				default:
					if(x+8> LCD_PIX_X)
					{
						x=0;
						if(y+16>LCD_PIX_Y) y=0;
						else y +=16;
					}
					break;
				case 2:
					if(x+12> LCD_PIX_X)
					{
						x=0;
						if(y+24>LCD_PIX_Y) y=0;
						else y +=24;
					}
					break;	
				case 3:
					if(x+16> LCD_PIX_X)
					{
						x=0;
						if(y+32>LCD_PIX_Y) y=0;
						else y +=32;
					}
					break;							
			}			
			Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size);

			ASCIIXDirectionIncrease(size,&x,&y);
			pDispData++;
			Length--;
		}
		else 	//�������������
		{
			pDispData++;
			Length--;		
		}
	}
	
	*px=x;
	*py=y;
}




uint8 DisplayOneScreen(uint8 *pDispData,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size)
{
	uint8 xdata x=0;
	uint8 xdata y=0;

	uint8 xdata DispedLen=0;

	for(;Length>0;)
	{	
		if((*pDispData>=0x81)&&(*pDispData<=0xfe))	//���ֽڷ��Ϻ�����������
		{
			pDispData++;
			if(*pDispData>=0x40)
			{
				if(Length>=2)
				{//��ֹ�������ݴ�ʱ���ݳ�����0  ���
					pDispData--;
					Get_GT23_Data1(pDispData,size);	

					//�Ƚ��пռ��жϣ������пռ�:x,y ���겻��
											//   �����޿ռ�:x,y����ĵ���һ����ʼ
											//   �������һ���ռ䣬�˳�
					switch(size)
					{
						case 1:
						default:
							if(x+16> LCD_PIX_X)
							{							
								x=0;
								if(y+18>LCD_PIX_Y) return DispedLen;		//һ���ռ���д��������
								else y +=18;			//��2���м��
							}
							break;
						case 3:
							if(x+32> LCD_PIX_X)
							{
								x=0;
								if(y+34>LCD_PIX_Y) return DispedLen;
								else y +=34;			//��2���м��
							}
							break;							
					}
					Display_Chinese(x,y,GT23_Rec_Data,fcolor,bcolor,size);

					//ChineseXDirectionIncrease(size,&x,&y);
					if(size==3) x+= 32;
					else x+= 16;
					pDispData=pDispData+2;	//һ����������ռ�����ֽ�
					Length -=2;
					DispedLen +=2;
					
				}
				else
				{//���Ȳ���2
					Length--;
					return DispedLen;
				}
			}
			else
			{//���ֽڲ����Ϻ�������
				Length--;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e))	//����ASCII�ַ�����
		{
			Get_GT23_Data(0x00,*pDispData,size);
			//�Ƚ��пռ��жϣ������пռ�:x,y ���겻��
									//	 �����޿ռ�:x,y����ĵ���һ����ʼ
									//	 �������һ���ռ䣬�˳�

			switch(size)
			{
				case 1:
				default:
					if(x+8> LCD_PIX_X)
					{
						x=0;
						if(y+18>LCD_PIX_Y) return DispedLen;
						else y +=18;		//��2���м��
					}
					break;
				case 2:
					if(x+12> LCD_PIX_X)
					{
						x=0;
						if(y+26>LCD_PIX_Y) return DispedLen;
						else y +=26;		//��2���м��
					}
					break;	
				case 3:
					if(x+16> LCD_PIX_X)
					{
						x=0;
						if(y+34>LCD_PIX_Y) return DispedLen;
						else y +=34;		//��2���м��
					}
					break;							
			}			
			Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size);

			//ASCIIXDirectionIncrease(size,&x,&y);
			if(size ==3) x+=16;
			else if(size ==2) x+=12;
			else x+=8;
			pDispData++;
			Length--;
			DispedLen++;
		}
		else 	//�������������
		{
			if(*pDispData == 10)//���з�
			{
				switch(size)
				{
					case 1:
					default:
						if(y+18>LCD_PIX_Y) return DispedLen;
						else {y +=18;x=0;}
						break;
						
					case 2:
						if(y+26>LCD_PIX_Y) return DispedLen;
						else {y +=26;x=0;}
						break;	
						
					case 3:
						if(y+34>LCD_PIX_Y) return DispedLen;
						else {y +=34;x=0;}
						break;							
				}
				
			}
			
			pDispData++;
			Length--;
			DispedLen ++;
		}
	}
	return DispedLen;
}




//x,y:��ʼ�������
//type_x_wide:ԭ����X��Ŀ��, type_y_height:ԭ����Y��ĸ߶�
//mul_row:����Ŵ���,   mul_col:����Ŵ�һ��
//cbc_filter:���˲��� 1:���˱���ɫ
//p:Ҫ��ʾ��ͼƬ����ָ��

void lcd_fangda_dis_text(uint8 x, uint8 y, uint8 type_x_wide, uint8 type_y_height, 
						 uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p)
{

	uint8 xdata i,j,k,m,n;


	Set_Display_Area(x,x+type_x_wide*mul_col-1,y,y+type_y_height*mul_row-1);
	Send_ST7735_Command(0X2C);
	for (i=0;i<type_y_height;i++) //ԭ�����ж�����
	{

		for(n=0;n<mul_row;n++)	//������ķŴ���
		{

			for(j=0;j<type_x_wide/8;j++)	//һ���м����ֽ�,��16*16 ����2���ֽ�
			{

				for(k=0;k<8;k++)	//1���ֽ�8��
				{

					for(m=0;m<mul_col;m++)	//������Ŵ���
					{
						//if(((x+8*j*mul_col+k*mul_col+m)<LCD_PIX_X)&&((y+i*mul_row+n)<LCD_PIX_Y))	//������Χ��
						//{
							if ((p[i*(type_x_wide/8)+j]>>(7-k))&0x1)
							{
								fontbuf1=fcolor%256;
								fontbuf2=fcolor/256;						
							}

							else
							{
								fontbuf1=bcolor%256;
								fontbuf2=bcolor/256;								
							}

							Send_ST7735_Data(fontbuf1);
							Send_ST7735_Data(fontbuf2);
						//}
					}
				}
			}
		}
	}
}



void ShowPowerUpFace(void)
{
	uint8 xdata x,y;


	Lcd_Clear(BLUE);
	x=(128-8*6)/2;
	y=(160-20*3)/2;
	DisplayString(&x,&y,"System",6,WHITE,BLUE,1);

	x=(128-8*14)/2;
	y+=20;
	DisplayString(&x,&y,"Initialization",14,WHITE,BLUE,1);
		
	x=(128-8*12)/2;
	y +=20;
	DisplayString(&x,&y,"Version V",9,WHITE,BLUE,1);


	ASCII_BCD[0]=VERSION_HI/100;
	ASCII_BCD[1]=(VERSION_HI%100)/10;
	ASCII_BCD[2]=(VERSION_HI%100)%10;	
	if(ASCII_BCD[0])
	{
		DisplayDigitString(&x,&y,&(ASCII_BCD[0]),3,WHITE,BLUE,1);
	}
	else
	{	
		if(ASCII_BCD[1])
		{
			DisplayDigitString(&x,&y,&(ASCII_BCD[1]),2,WHITE,BLUE,1);	
		}
		else DisplayDigitString(&x,&y,&(ASCII_BCD[2]),1,WHITE,BLUE,1);
	}
	DisplayASCIIString(&x,&y,".",1,WHITE,BLUE,1);
	
	ASCII_BCD[0]=VERSION_LO/100;
	ASCII_BCD[1]=(VERSION_LO%100)/10;
	ASCII_BCD[2]=(VERSION_LO%100)%10;	
	if(ASCII_BCD[0])
	{
		DisplayDigitString(&x,&y,&(ASCII_BCD[0]),3,WHITE,BLUE,1);
	}
	else
	{	
		if(ASCII_BCD[1])
		{
			DisplayDigitString(&x,&y,&(ASCII_BCD[1]),2,WHITE,BLUE,1);	
		}
		else DisplayDigitString(&x,&y,&(ASCII_BCD[2]),1,WHITE,BLUE,1);
	}		

}


/**********************************************************
*��������			:ShowLandingFace	
*��������        	:��ʾ��½����
*�������   		:
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/ 
void ShowLandingFace(void)
{
	uint8 xdata temp1,temp2,temp3;
	uint8 xdata x,y;


	temp1=stLocalControl.stEepromCfgData.bySelfBedAddr/100; ////��λ
	temp2=(stLocalControl.stEepromCfgData.bySelfBedAddr%100)/10;	//ʮλ
	temp3=(stLocalControl.stEepromCfgData.bySelfBedAddr%100)%10;	//��λ

//	x=0;y=0;
	DisplayBlock(0,0,127,63,BLUE_CODE); 


	x=16;y=4;
	DisplayString(&x,&y,"Bed: ",5,BLACK,BLUE,2);
	DisplayDigitString(&x,&y,&temp1,1,WHITE,BLUE,3);
	DisplayDigitString(&x,&y,&temp2,1,WHITE,BLUE,3);
	DisplayDigitString(&x,&y,&temp3,1,WHITE,BLUE,3);


	x=16;y=32;
	temp1=stLocalControl.stEepromCfgData.bySelfRoomAddr%100;
	temp2=temp1%10; //��λ
	temp1=temp1/10; //ʮλ
	DisplayString(&x,&y,"Room:",5,BLACK,BLUE,2);		
	DisplayDigitString(&x,&y,&temp1,1,WHITE,BLUE,3);
	DisplayDigitString(&x,&y,&temp2,1,WHITE,BLUE,3);	
	

	x=0;y=64;
	temp1=stLocalControl.stTime.byYear; //BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
	DisplayASCIIString(&x,&y,"-",1,BLACK,CYAN,3);	

	temp1=stLocalControl.stTime.byMonth;	//BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
	DisplayASCIIString(&x,&y,"-",1,BLACK,CYAN,3);

	temp1=stLocalControl.stTime.byDay;	//BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);


	DisplayBlock(0,96,23,127,CYAN_CODE);
	x=24;y=96;
	temp1=stLocalControl.stTime.byHour; //BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
	DisplayASCIIString(&x,&y,":",1,BLACK,CYAN,3);	

	temp1=stLocalControl.stTime.byMinute; //BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
	DisplayBlock(x,y,127,127,CYAN_CODE);


	x=0;y=128;
	DisplayString(&x,&y,"There is a need, please call!",(uint8)strlen("There is a need, please call!"),BLUE,GREEN,1);
	DisplayBlock(x,y,127,159,GREEN_CODE);

	stLocalControl.byDisplayFace=LAND_FACE;
	if(stLocalControl.byPatientDischarge==0)
	{//�ô�λ�в�����
		if(bBL_SW_State)	//���⿪ʱ��
			BL_SW = 1;	//����ʾ������
		else BL_SW = 0;
	}
	else 
	{
		BL_SW=0;	//�ر���ʾ������
	}	
}



void ShowMainMenuFace(void)
{
	uint8 x,y;
	Lcd_Clear(BLUE);
	x=0;y=0;	
	if(MainMenuSelect==1)
	{//��ѡ��
		DisplayString(&x,&y,"1.DietMode",10,BLUE,WHITE,2);

	}
	else
	{
		DisplayString(&x,&y,"1.DietMode",10,WHITE,BLUE,2);

	}	

	
	
	
	x=0;y +=26;
	if(MainMenuSelect==2)
	{//��ѡ��
		DisplayString(&x,&y,"2.Doctor's Advice",(uint8)strlen("2.Doctor's Advice"),BLUE,WHITE,2);

	}
	else 
	{
		DisplayString(&x,&y,"2.Doctor's Advice",(uint8)strlen("2.Doctor's Advice"),WHITE,BLUE,2);
	}

	
	x=0;y+=26;
	if(MainMenuSelect==3)
	{//��ѡ��
		DisplayString(&x,&y,"3.Prompting Message",(uint8)strlen("3.Prompting Message"),BLUE,WHITE,2);

	}
	else 
	{//��ѡ��
		DisplayString(&x,&y,"3.Prompting Message",(uint8)strlen("3.Prompting Message"),WHITE,BLUE,2);
	}
	
	stLocalControl.byDisplayFace=MAIN_MENU_FACE;


}
/**********************************************************
*��������			:ShowTalkFace	
*��������        	:��ʾͨ������
*�������   		:byRoomAddr:����ַ,byBedAddr:����ַ
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:
*��������	 		:2009-3-16
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void ShowTalkFace(void)
{ 
	uint8 xdata x,y;
	Lcd_Clear(BLUE);
	x=0;
	y=(160-24*2)/2;

	DisplayString(&x,&y,"This extension is busy",(uint8)strlen("This extension is busy"),WHITE,BLUE,2);
 	stLocalControl.byDisplayFace=TALK_FACE;

}


void ShowBroadFace(void)
{ 
	uint8 xdata x,y;
	Lcd_Clear(BLUE);
	x=0;
	y=(160-24)/2;

	DisplayString(&x,&y,"On the Air",10,WHITE,BLUE,2);
 	stLocalControl.byDisplayFace=BROAD_FACE;

}


void ShowCallSuccess(void)
{
	uint8 xdata x,y;
	Lcd_Clear(BLUE);
	x=0;
	y=(160-24*2)/2;

	DisplayString(&x,&y,"Call success,Please wait!",(uint8)strlen("Call success,Please wait!"),WHITE,BLUE,2);

	stLocalControl.byDisplayFace=CALL_SUCCESS_FACE;

}


void ShowTime(void)
{
	uint8 xdata x,y;
	uint8 xdata temp1,temp2;

	x=0;y=64;
	temp1=stLocalControl.stTime.byYear; //BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);

	
	x+=16;
	temp1=stLocalControl.stTime.byMonth;	//BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);

	
	x+=16;
	temp1=stLocalControl.stTime.byDay;	//BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
	

	x=24;y=96;
	temp1=stLocalControl.stTime.byHour; //BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);

	
	x+=16;
	temp1=stLocalControl.stTime.byMinute; //BCD��
	temp2=temp1%16; //��λ
	temp1=temp1/16; //ʮλ
	DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
	DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);

}


void SecondFlash(void)
{
	uint8 xdata x,y;

	x=56;y=96;
	bSecondDisState=!bSecondDisState;
	if(bSecondDisState)
	{
		Display_ASCII(56,96,ColonCode16x32,BLACK,CYAN,3);	//":"				
	}
	else
	{
		DisplayBlock(56,96,56+15,96+31,CYAN_CODE);
	}

}

void ShowFace(uint8 DispFaceType)
{

	uint16 xdata TotalDataLen;
	uint8  xdata DispDataLen;
    uint8  xdata x,y;
	uint32 xdata MRReadAddr;


//������Ϣ�����ݳ���
	switch(DispFaceType)
	{
		case DIET_FACE:
			TotalDataLen = stLocalControl.uiDietDataLen;
			break;
		case PRESCRIPTION_FACE:
			TotalDataLen = stLocalControl.uiPrescriptionDataLen;
			break;		
		case PROMPT_INFO_FACE:
			TotalDataLen = stLocalControl.uiPromptDataLen;
			break;
		default:
			TotalDataLen=0;
			break;
	}

	if(TotalDataLen == 0)	//û������
	{
		Lcd_Clear(BLUE);	//����
		x=0;y=64;
		DisplayString(&x,&y,"No Date",7,WHITE,BLUE,3);
		//return;
		goto SHOW_FACE_RETURN;
	}

	if(DisplayedLen>=TotalDataLen) DisplayedLen =0;			//����������ʾ���ˣ���ͷ��ʼ
	if(DisplayedLen>=32768) DisplayedLen =0;

	//��ʾ�������������е���ʼ��ַ
	switch(DispFaceType)
	{
		case DIET_FACE:
			MRReadAddr = DIET_DATA_START_ADDR+DisplayedLen;  //һ��80������(160���ֽ�)
			break;
		case PRESCRIPTION_FACE:
			MRReadAddr = PRESCRIPTION_DATA_START_ADDR+DisplayedLen;
			break;		
		case PROMPT_INFO_FACE:
			MRReadAddr = PROMPT_DATA_START_ADDR+DisplayedLen;
			break;
		default:
			MRReadAddr = 0;
			break;
	}	
	

	
	if(DisplayedLen +160>=TotalDataLen)
	{//����������һ����ʾ����
		DispDataLen = TotalDataLen - DisplayedLen;
	}
	else
	{
		DispDataLen=160;
	}
	Read_Cont(DispDataBuff,MRReadAddr,DispDataLen);//��Ҫ��ʾ�����ݴӴ洢���ж�ȡ����ʾ��������
	Send_Data(DispDataBuff,DispDataLen);

	Lcd_Clear(BLUE);	//����	
	DisplayedLen += DisplayOneScreen(DispDataBuff,DispDataLen,WHITE,BLUE,1);


SHOW_FACE_RETURN:
	
	switch(DispFaceType)
	{
		case DIET_TYPE:
			stLocalControl.byDisplayFace=DIET_FACE;
			break;

		case PRESCRIPTION_TYPE:
			stLocalControl.byDisplayFace=PRESCRIPTION_FACE;
			break;			
		case PROMPT_INFO_TYPE:
			stLocalControl.byDisplayFace=PROMPT_INFO_FACE;
			break;	
		default:
			break;
	}

}



void ShowDietFace()
{
	stLocalControl.byDisplayFace=DIET_FACE;

}	

void ShowPrescriptionFace()
{
	stLocalControl.byDisplayFace=PRESCRIPTION_FACE;
}


void ShowPromptFace()
{
	stLocalControl.byDisplayFace=PROMPT_INFO_FACE;

}
/**********************************************************
*��������			:DispManager	
*��������        	:���������߳�
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:byMainCmdQ
*����ģ��  			:OSQPost
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void DispManager(void)
{ 	


	//��ʾ��ʼ����
	ShowLandingFace();
	while(TRUE)
	{		
		WDT_CONTR = 0x3d;	//ι��
		OSWait(K_TMO, 10);			 		
	}	

}

 
