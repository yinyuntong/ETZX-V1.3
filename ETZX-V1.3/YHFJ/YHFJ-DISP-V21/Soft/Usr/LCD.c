/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:TFT.c
*�ļ�����    		:TFT������������
*������     		:����ͬ
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     							
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_LCD_
#include "config.h"


union union_type xdata unOperData;

uint8 xdata fontbuf1,fontbuf2,fontbuf3;


uint8 	xdata WardDataBuff[WARD_FACE_DATA_LEN];


uint8 code ColonCode16x32[64]={	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x03,0xC0,0x03,0xC0,
								0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,
						   		0x03,0xC0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   		0x00,0x00,0x00,0x00,0x00,0x00,};/*":",0*/
/* (16 X 32 , ���� )*/


void Send_Ssd1963_Command(uint8 ssd1963_command);
void Send_Ssd1963_Data(uint8 ssd1963_data);
void InitSSD1963(void);
void Set_Display_Area(uint16 start_x,uint16 end_x,uint16 start_y,uint16 end_y);
void Display_Dot(uint16 x,uint16 y,uint16  color,uint8 page);
void Display_HLine(uint16 x,uint16  y,uint16  width,uint16  color,uint8 page);
void Display_VLine(uint16  x,uint16  y,uint16  height,uint16  color,uint8 page);
void Lcd_Clear(uint16  color,uint8 page);
void Display_ASCII(uint16 x,uint16 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);
void Display_Chinese(uint16 x,uint16 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);


void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf,uint8 page);
void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr,uint8 page);


void ShowStartInfusionCall(uint8 page);
void ShowStartServiceCall(uint8 page);
void ShowStartEmergencyCall(uint8 page);
void ShowStartHelpCall(uint8 page);
void ShowCallSuccess(uint8 page);
void lcd_fangda_dis_text(uint16 x, uint16 y, uint8 type_x_wide, uint8 type_y_height, 
						 uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p,uint8 page);



#define WriteData8(sdata) {DataPortL=(sdata); SSD1963_WR=0; SSD1963_WR=1;}  
	
#define WriteData16(sdata) {DataPortL=(sdata); DataPortH=(sdata)>>8;SSD1963_WR=0; SSD1963_WR=1;} 

/*---------------------------------------------------------------------------
����ԭ��: void Send_Ssd1963_Command(uint8 xdata ssd1963_command)
����˵��: ssd1963_command:��Ҫд������
�� �� ֵ: ��
��������: д����
----------------------------------------------------------------------------*/
void Send_Ssd1963_Command(uint8 ssd1963_command)
{
 	SSD1963_CD=0;
	SSD1963_CS=0;
	DataPortL=ssd1963_command;
	SSD1963_WR=0; 
	SSD1963_WR=1;
	SSD1963_CS=1;
	SSD1963_CD=1;  
  
}
/*---------------------------------------------------------------------------
����ԭ��: void Send_Ssd1963_Data(uint8 xdata ssd1963_data)
����˵��: ssd1963_data:��Ҫд������
�� �� ֵ: ��
��������: д����
----------------------------------------------------------------------------*/
void Send_Ssd1963_Data(uint8 ssd1963_data)
{
	SSD1963_CS=0;
	DataPortL=ssd1963_data;
	SSD1963_WR=0;
	SSD1963_WR=1;
	SSD1963_CS=1;

}


void SSD1963Data_16(uint sdata)

{
	SSD1963_CS=0;
	DataPortL=sdata;//temp.UByte[1];
	DataPortH=sdata>>8;//temp.UByte[0];
	SSD1963_WR=0;
	SSD1963_WR=1;
	SSD1963_CS=1;
}


void SetScrollArea(uint16 top, uint16 scroll, uint16 bottom)
{

	Send_Ssd1963_Command(cmd_set_scroll_area);
	Send_Ssd1963_Data(top>>8);
	Send_Ssd1963_Data(top);
	Send_Ssd1963_Data(scroll>>8);
	Send_Ssd1963_Data(scroll);
	Send_Ssd1963_Data(bottom>>8);
	Send_Ssd1963_Data(bottom);
}

void SetScrollStart(uint16 line)
{
	Send_Ssd1963_Command(cmd_set_scroll_start);
	Send_Ssd1963_Data(line>>8);
	Send_Ssd1963_Data(line); 
}

void Set_VisualPage(uint8 VisualPage)
{
	SetScrollStart(VisualPage*272);
}

/*--------------------------------------------------------------------------------------------
����ԭ��: void Set_Display_Area(uint16 xdata start_x,uint16 xdata end_x,uint16 xdata start_y,uint16 xdata end_y) 
����˵��: start_x:���X��ʼ����
          end_x:���X��������
		  start_y:���Y��ʼ����
          end_y:���Y��������
�� �� ֵ: ��
��������: ������Ļ����
----------------------------------------------------------------------------------------------*/
void Set_Display_Area(uint16  start_x,uint16  end_x,uint16 start_y,uint16  end_y)
{
  uint8 xdata shigh_x1,slow_x1,shigh_x2,slow_x2;
  uint8 xdata shigh_y1,slow_y1,shigh_y2,slow_y2;
  shigh_x1=start_x/256;
  slow_x1=start_x%256;
  shigh_x2=end_x/256;
  slow_x2=end_x%256;
  shigh_y1=start_y/256;
  slow_y1=start_y%256;
  shigh_y2=end_y/256;
  slow_y2=end_y%256;
  Send_Ssd1963_Command(0x2A);
  Send_Ssd1963_Data(shigh_x1);
  Send_Ssd1963_Data(slow_x1);
  Send_Ssd1963_Data(shigh_x2);
  Send_Ssd1963_Data(slow_x2);
  Send_Ssd1963_Command(0x2B);
  Send_Ssd1963_Data(shigh_y1);
  Send_Ssd1963_Data(slow_y1);
  Send_Ssd1963_Data(shigh_y2);
  Send_Ssd1963_Data(slow_y2);
}


uint16 GetColor(uint8 srgb)
{
	if(srgb==BLACK_CODE)	return(BLACK);
	else if(srgb==RED_CODE)	return(RED);
	else if(srgb==GREEN_CODE)	return(GREEN);	
	else if(srgb==POWDER_BLUE_CODE)	return(POWDER_BLUE);
	else if(srgb==BLUE_CODE)	return(BLUE);
	else if(srgb==YELLOW_CODE)	return(YELLOW);
	else if(srgb==PURPLE_CODE)	return(PURPLE);
	else if(srgb==CYAN_CODE)	return(CYAN);		
	else if(srgb==WHITE_CODE)	return(WHITE);
	else return(WHITE); 
}
/*--------------------------------------------------------------------------------------------
����ԭ��: void Lcd_Clear(uint32 xdata color)
����˵��: color:��������ɫ
�� �� ֵ: ��
��������: ����
----------------------------------------------------------------------------------------------*/
void Lcd_Clear(uint16  color,uint8 page)
{
  uint16 xdata k,j;

  Set_Display_Area(0,479,page*LCD_PIX_Y,page*LCD_PIX_Y+271);
  Send_Ssd1963_Command(0x2C);
  
  WDT_CONTR = 0x3d; 
  SSD1963_CS=0;
  for(k=0;k<480;k++)
    {
     for(j=0;j<272;j++)
     {
		WriteData16(color);
	 }
    }
  SSD1963_CS=1;
}

/*------------------------------------------------------------------------------
����ԭ��: void Display_Dot(uint16 xdata x,uint16 xdata y,uint32 xdata color)   
����˵��: x:���X����,y:���Y����,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ��
-------------------------------------------------------------------------------*/
void Display_Dot(uint16  x,uint16  y,uint16  color,uint8 page)  
{  

  Set_Display_Area(x,x,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  WriteData16(color);
  SSD1963_CS=1;
}


/*void Display_Dot1(uint16  x,uint16  y,uint8 srgb,uint8 page)  
{  
  uint16 xdata color;
  color= GetColor(srgb);
 
  Set_Display_Area(x,x,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  WriteData16(color);
  SSD1963_CS=1;

}*/


//������Ŵ���ʾ
void Display_Dot_Mul(uint16  x,uint16  y,uint16  color,uint8 page)  
{  
  uint8 xdata i;
  Set_Display_Area(x-1,x+1,y-1+page*LCD_PIX_Y,y+1+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  WDT_CONTR = 0x3d;   
  SSD1963_CS=0;
  for(i=0;i<9;i++)
  	WriteData16(color);
  SSD1963_CS=1;

}

/*-------------------------------------------------------------------------------------------------
����ԭ��: void Display_HLine(uint16 xdata x,uint16 xdata y,uint16 xdata width,uint32 xdata color) 
����˵��: x:���X����,y:���Y����,width:ˮƽֱ�ߵĳ���,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾˮƽֱ��
--------------------------------------------------------------------------------------------------*/
void Display_HLine(uint16  x,uint16  y,uint16  width,uint16  color,uint8 page)
{

  Set_Display_Area(x,x+width-1,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  WDT_CONTR = 0x3d; 	//ι��
  SSD1963_CS=0;
  while(width--)
  {
	  WriteData16(color);
  }
   SSD1963_CS=1;
}


/*--------------------------------------------------------------------------------------------------
����ԭ��: void Display_VLine(uint16 xdata x,uint16 xdata y,uint16 xdata height,uint32 xdata color)
����˵��: x:���X����,y:���Y����,height:��ֱֱ�ߵĳ���,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ��ֱֱ��
----------------------------------------------------------------------------------------------------*/
void Display_VLine(uint16  x,uint16  y,uint16  height,uint16  color,uint8 page)
{

  Set_Display_Area(x,x,y+page*LCD_PIX_Y,y+height-1+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  WDT_CONTR = 0x3d; 
  SSD1963_CS=0;
  while(height--)
  {
    WriteData16(color);
  }
  SSD1963_CS=1;
}  
 




//��ʾһ������
void DisplayFrame(uint16 start_x,uint16 start_y,uint16 end_x,uint16 end_y, uint16 color,uint8 page)
{
	Display_HLine(start_x,start_y,end_x-start_x+1,color,page);
	Display_HLine(start_x,end_y,end_x-start_x+1,color,page);

	Display_VLine(start_x,start_y,end_y-start_y+1,color,page);
	Display_VLine(end_x,start_y,end_y-start_y+1,color,page);
}






//��ʾһ����ɫ��
void DisplayBlock(uint16 start_x,uint16 start_y,uint16 end_x,uint16 end_y, uint16 color,uint8 page)
{
	uint16 xdata width,height;
	uint16 i,j;

	Set_Display_Area(start_x,end_x,start_y+page*LCD_PIX_Y,end_y+page*LCD_PIX_Y);
	Send_Ssd1963_Command(0x2C);


	width=end_x-start_x+1;

	height=end_y-start_y+1;

	WDT_CONTR = 0x3d; 

	SSD1963_CS=0;
	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			WriteData16(color);
		}
	} 
	SSD1963_CS=1;
}




 /*--------------------------------------------------------------------------------------------------------------------------------------
 ����ԭ��: void Display_ASCII(uint16 xdata x,uint16 xdata y,uint8 xdata *p,uint32 xdata fcolor,uint32 xdata bcolor)
 ����˵��: x:�ַ���X����,y:�ַ���Y����,frcolor:�ַ�����ɫ��brcolor:�ַ��ı�����ɫ
 �� �� ֵ: ��
 ��������: ����Ļ����ʾ�ַ�
 ----------------------------------------------------------------------------------------------------------------------------------------*/
  void Display_ASCII(uint16 x,uint16 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
 {
 
	uint8 xdata i,j;
	switch(size)
	{
	   case 1: //8*16
		   Set_Display_Area(x,x+7,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+15);
		   i=16;
		   break;
	   case 2: //12*24
		   Set_Display_Area(x,x+15,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+23);
		   i=48;
		   break;
	   case 3: //16*32
		   Set_Display_Area(x,x+15,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+31);
		   i=64;
		   break;
	   default://Ĭ��Ϊ12*24
		   Set_Display_Area(x,x+15,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+23);
		   i=48;
		   break;
	}

	Send_Ssd1963_Command(0x2C);
	WDT_CONTR = 0x3d; 

	SSD1963_CS=0;
	for(;i>0;i--)
	{
	   for(j=0;j<8;j++)
	   {
			if(*p&(0x80>>j))
			{
				WriteData16(fcolor);
			}
			else
			{
				WriteData16(bcolor);

			}

	   }
	   p++;
	}
	SSD1963_CS=1;
 }


/*-----------------------------------------------------------------------------
����ԭ��: void Display_Chinese(uint x,uint y,uchar *p,ulong fcolor,ulong bcolor)
����˵��: x:���ֵ�X����,y:���ֵ�Y����,fcolor:���ֵ���ɫ��bcolor�����ֵı�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ����
----------------------------------------------------------------------------*/
void Display_Chinese(uint16 x,uint16 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	uchar xdata i,j;
	switch(size)
	{
	case 1: //15*16
	   Set_Display_Area(x,x+15,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+15);
	   i=32;	//32���ֽڵ�������
	   break;
	case 2: //24*24
	   Set_Display_Area(x,x+23,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+23);
	   i=72;
	   break;
	case 3: //32*32
	   Set_Display_Area(x,x+31,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+31);
	   i=128;
	   break;

	default://Ĭ��Ϊ24*24
	   Set_Display_Area(x,x+23,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+23);
	   i=72;
	   break;		   
	}

	Send_Ssd1963_Command(0x2C);
	 WDT_CONTR = 0x3d; 
	SSD1963_CS=0;
	for(;i>0;i--)
	{
	   for(j=0;j<8;j++)
	   {
			if(*p&(0x80>>j))
			{
				WriteData16(fcolor);
			}
			else
			{
				WriteData16(bcolor);

			}
	   }
	   p++;
	}
	SSD1963_CS=1;
 }



void ASCIIXDirectionIncrease(uint8  size,uint16 *px,uint16 *py)
{
	uint16 xdata x,y;
	x=*px;
	y=*py;
	switch(size)
	{
		case 1://8*16
		
			if(x+8<LCD_PIX_X) x+=8;
			else 
			{
				if(y+16<LCD_PIX_Y) {y+=16;x=0;}
				else {x=0;y=0;}
			}
			break;
		case 2://12*24
		default:
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



void ChineseXDirectionIncrease(uint8 size,uint16 *px,uint16 *py)
{
	uint16 xdata x,y;
	x=*px;
	y=*py;
	switch(size)
	{
	case 1: //15*16
	
		if(x+15<LCD_PIX_X) x+=15;
		else 
		{
			if(y+16<LCD_PIX_Y) {y+=16;x=0;}
			else {x=0;y=0;}
		}
		break;
	case 2://24*24
	default:
		if(x+24<LCD_PIX_X) x+=24;
		else 
		{
			if(y+24<LCD_PIX_Y) {y+=24;x=0;}
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



//��ʾһ������
void DisplayDigitString(uint16 *px,uint16 *py,uint8 *pDigit,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	uint16 xdata x,y;
	x=*px;
	y=*py;	
	
  	WDT_CONTR = 0x3d; 	
	for(;Length>0;Length--)
	{	
		if(*pDigit<10) Get_GT23_Data(0x00,*pDigit+48,size);	
		else Get_GT23_Data(0x00,*pDigit+55,size);	//ʮ�������е�ABC....
		Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);

		ASCIIXDirectionIncrease(size,&x,&y);
		pDigit++;
	}
	
	*px=x;
	*py=y;
}




//д��������
void DisplayDigit(uint16 x,uint16 y,uint8 Digit,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	
	if(Digit<10) Get_GT23_Data(0x00,Digit+48,size);	
	else Get_GT23_Data(0x00,Digit+55,size);	//ʮ�������е�ABC....
	Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);	
}




//��ʾASCII�ַ���
void DisplayASCIIString(uint16 *px,uint16 *py,uint8 *pASCII,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	uint16 xdata x,y;
	x=*px;
	y=*py;
	
	WDT_CONTR = 0x3d; 

	for(;Length>0;Length--)
	{	
		Get_GT23_Data(0x00,*pASCII,size);	
		Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);

		ASCIIXDirectionIncrease(size,&x,&y);
		pASCII++;
	}
	*px=x;
	*py=y;
}



//д����ASCII�ַ�
void DisplayASCII(uint16 x,uint16 y,uint8 ASCII,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	
	Get_GT23_Data(0x00,ASCII,size);	
	Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);

}





//��ʾһ������
void DisplayChineseString(uint16 *px,uint16 *py,uint8 *pChinese,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	uint16 xdata x,y;
	x=*px;
	y=*py;
	
	WDT_CONTR = 0x3d; 

	for(;Length>0;Length--)
	{	
		Get_GT23_Data1(pChinese,size);		
		Display_Chinese(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);

		ChineseXDirectionIncrease(size,&x,&y);
		pChinese=pChinese+2;	//һ����������ռ�����ֽ�
	}
	*px=x;
	*py=y;
}




void DisplayString(uint16 *px,uint16 *py,uint8 *pDispData,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page)
{
	uint16 xdata x,y;
	x=*px;
	y=*py;

	WDT_CONTR = 0x3d; 

	for(;Length>0;)
	{	
		if((*pDispData>=0xa1)&&(*pDispData<=0xf7))	//���ֽڷ��Ϻ�����������
		{
			pDispData++;
			if(*pDispData>=0xa1)
			{
				pDispData--;
				Get_GT23_Data1(pDispData,size);	

				//�Ƚ��пռ��ж�
				switch(size)
				{
					case 1:
						if(x+16> LCD_PIX_X)
						{
							x=0;
							if(y+16>LCD_PIX_Y) y=0;
							else y +=16;
						}
						break;
					case 2:
						if(x+24> LCD_PIX_X)
						{
							x=0;
							if(y+24>LCD_PIX_Y) y=0;
							else y +=24;
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
				Display_Chinese(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);

				ChineseXDirectionIncrease(size,&x,&y);
				pDispData=pDispData+2;	//һ����������ռ�����ֽ�
				Length -=2;
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
			Display_ASCII(x,y,GT23_Rec_Data,fcolor,bcolor,size,page);

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



/*void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCallType,uint8 page)
{
	uint16 xdata x,y;

	if(stLocalControl.byDisplayFace==INFO_INDICATION_FACE) return;

	
	Lcd_Clear(BLUE,page);
	byRoomAddr &= 0x7f;
	switch(byCallType)
	{
		case CMD_INFUSION_CALL:								//�������		
			x=144;
			y=120;
			(unOperData.ASCII_BCD)[0]=byRoomAddr/100;
			(unOperData.ASCII_BCD)[1]=(byRoomAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byRoomAddr%100)%10;
			DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
			
			(unOperData.ASCII_BCD)[0]=byBedAddr/100;
			(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
			DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			DisplayChineseString(&x,&y,"������",3,WHITE,BLUE,3,page);
			break;

		case CMD_SERVICE_CALL: 							//��Һ��Ϻ���
			x=112;
			y=120;
			(unOperData.ASCII_BCD)[0]=byRoomAddr/100;
			(unOperData.ASCII_BCD)[1]=(byRoomAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byRoomAddr%100)%10;
			DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
			
			(unOperData.ASCII_BCD)[0]=byBedAddr/100;
			(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
			DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			DisplayChineseString(&x,&y,"����Һ���",5,WHITE,BLUE,3,page);
			break;
			
		case CMD_EMERGENCY_CALL:   							//��������
			x=108;
			y=120;
			(unOperData.ASCII_BCD)[0]=byRoomAddr/100;
			(unOperData.ASCII_BCD)[1]=(byRoomAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byRoomAddr%100)%10;
			DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			DisplayChineseString(&x,&y,"�����������",6,WHITE,BLUE,3,page);
			break;
	}	

	Set_VisualPage(page);
	
	stLocalControl.byDisplayFace=INFO_INDICATION_FACE;

}
*/


////������Ϣָʾ����֮�����Ϣָʾ������ʾ
void ShowCallFace0(uint8 page)
{
	uint16 xdata x,y;
	uint8 xdata i;
	
	x=32;y=0;
	Lcd_Clear(BLUE,page);

	
	WDT_CONTR = 0x3d; 

    for(i=0;i<stLocalControl.byCallCount;i++)
    {
    	x=32; y += 42;
		if(y>=240) break;	//����һ����ʾ��Χ
		
    	switch(stLocalControl.byCallArray[i][3])	//ԭ����
		{
			case CMD_INFUSION_CALL:     			//�������
				if(stLocalControl.byCallArray[i][1] == 250)
				{
					DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				}
				else 
				{
					(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
					(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
					(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;

					DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
					DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				}
				
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][2])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][2])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][2])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"������",3,WHITE,BLUE,3,page);
				break;
				
			case CMD_SERVICE_CALL: 							//��Һ��Ϻ���
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][2])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][2])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][2])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"����Һ���",5,WHITE,BLUE,3,page);
				break;
				
			case CMD_EMERGENCY_CALL:   							//��������
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"�����������",6,WHITE,BLUE,3,page);
				break;				

		}

    }


	Set_VisualPage(page);	
	stLocalControl.byDisplayFace=INFO_INDICATION_FACE;

}




////����ȷ������֮�����Ϣָʾ������ʾ
void ShowCallFace1(uint8 page)
{
	uint16 xdata x,y;
	uint8 xdata i;


	if(stLocalControl.byCallCount<=5)
	{
		y=stLocalControl.byCallCount*42;
		
		if(stLocalControl.byCallCount==1)
		{//��һ����¼������		
			Lcd_Clear(BLUE,page);
		}
		else 
		{//���������¼
			DisplayBlock(0,y,479,y+31,BLUE,page);
		}

		
		x=32;
		y=stLocalControl.byCallCount*42;
		i=stLocalControl.byCallCount-1;
	   	switch(stLocalControl.byCallArray[i][3])	//ԭ����
		{
			case CMD_INFUSION_CALL:     			//�������
				if(stLocalControl.byCallArray[i][1] == 250)
				{
					DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				}
				else 
				{			
					(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
					(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
					(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;

					DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
					DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				}
				
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][2])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][2])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][2])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"������",3,WHITE,BLUE,3,page);
				break;
				
			case CMD_SERVICE_CALL: 							//��Һ��Ϻ���
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][2])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][2])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][2])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"����Һ���",5,WHITE,BLUE,3,page);
				break;
				
			case CMD_EMERGENCY_CALL:   							//��������
				(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
				(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
				(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"�����������",6,WHITE,BLUE,3,page);
				break;				

		}

		Set_VisualPage(page);	
		stLocalControl.byDisplayFace=INFO_INDICATION_FACE;
		
	}
}



//�����������֮�����Ϣָʾ������ʾ
void ShowCallFace2(uint8 page)
{
	uint16 xdata x,y;
	uint8 xdata i;

	if(stLocalControl.byCallCount<5)
	{//���������ĩβһ�м�¼
		x=32;y=(stLocalControl.byCallCount+1)*42;
		DisplayBlock(x,y,479,y+31,BLUE,page);
	}

	if(stLocalControl.byUpdataRecNum < 5)
	{
		if(stLocalControl.byUpdataRecNum != stLocalControl.byCallCount)
		{//���µļ�¼�������һ����¼
			x=32;
			y=(stLocalControl.byUpdataRecNum+1)*42;
			i=stLocalControl.byUpdataRecNum;
		   	switch(stLocalControl.byCallArray[i][3])	//ԭ����
			{
				case CMD_INFUSION_CALL:     			//�������
					if(stLocalControl.byCallArray[i][1] == 250)
					{
						DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
					}
					else 
					{
						(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
						(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
						(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;

						DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
						DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
					}
					
					(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][2])/100;
					(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][2])%100)/10;
					(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][2])%100)%10;
					DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
					DisplayChineseString(&x,&y,"������",3,WHITE,BLUE,3,page);
					break;
					
				case CMD_SERVICE_CALL: 							//��Һ��Ϻ���
					(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
					(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
					(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;
					DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
					DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
					
					(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][2])/100;
					(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][2])%100)/10;
					(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][2])%100)%10;
					DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
					DisplayChineseString(&x,&y,"����Һ���",5,WHITE,BLUE,3,page);
					break;
					
				case CMD_EMERGENCY_CALL:   							//��������
					(unOperData.ASCII_BCD)[0] = (stLocalControl.byCallArray[i][1])/100;
					(unOperData.ASCII_BCD)[1] = ((stLocalControl.byCallArray[i][1])%100)/10;
					(unOperData.ASCII_BCD)[2] = ((stLocalControl.byCallArray[i][1])%100)%10;
					DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
					DisplayChineseString(&x,&y,"�����������",6,WHITE,BLUE,3,page);
					break;				

			}
			DisplayBlock(x,y,479,y+31,BLUE,page);	//���ü�¼ĩβ�޹ز������
		}
	}
	Set_VisualPage(page);	
	stLocalControl.byDisplayFace=INFO_INDICATION_FACE;
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
void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr,uint8 page)
{ 

	uint16 xdata x,y;

	if(stLocalControl.byDisplayFace==TALK_FACE) return;

	
	Lcd_Clear(BLUE,page);
	x=160;
	y=120;
	
	if(0x00 == byRoomAddr)
	{	//����ַΪ0,����Ϊֱ�ӱ�ŷֻ�
		x=(479-32*8)/2;
		y=120;	
		DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
		(unOperData.ASCII_BCD)[0]=byBedAddr/100;
		(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
		(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
		if((unOperData.ASCII_BCD)[0])
		{
			DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
		}
		else 
		{
			if((unOperData.ASCII_BCD)[1])
			{
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
			}
			else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
		}
		DisplayChineseString(&x,&y,"���ֻ�ͨ����",6,WHITE,BLUE,3,page);

	}

	else 
	{	
		x=(479-32*10)/2;
		y=120;
		if(byRoomAddr == 253)
		{//ҽ���ֻ�
			DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
			(unOperData.ASCII_BCD)[0]=byBedAddr/100;
			(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
			if((unOperData.ASCII_BCD)[0])
			{
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			}
			else 
			{
				if((unOperData.ASCII_BCD)[1])
				{
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
				}
				else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
			}
			DisplayChineseString(&x,&y,"��ҽ���ֻ�ͨ����",8,WHITE,BLUE,3,page);			
		}
		else 
		{
			DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
			(unOperData.ASCII_BCD)[0]=byRoomAddr/100;
			(unOperData.ASCII_BCD)[1]=(byRoomAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byRoomAddr%100)%10;
			if((unOperData.ASCII_BCD)[0])
			{
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			}
			else 
			{
				if((unOperData.ASCII_BCD)[1])
				{
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
				}
				else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
			}
			DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
			(unOperData.ASCII_BCD)[0]=byBedAddr/100;
			(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
			if((unOperData.ASCII_BCD)[0])
			{
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			}
			else 
			{
				if((unOperData.ASCII_BCD)[1])
				{
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
				}
				else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
			}
			DisplayChineseString(&x,&y,"���ֻ�ͨ����",6,WHITE,BLUE,3,page);		
		}
	}
	Set_VisualPage(page);

	stLocalControl.byDisplayFace=TALK_FACE;

}


void ShowKeyFace(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byKeyInTimes==0)
	{
		DisplayBlock(0,240+page*LCD_PIX_Y,479,271+page*LCD_PIX_Y,BLUE,page);
	}
	x=stLocalControl.byKeyInTimes*16;
	y=240;
	DisplayDigitString(&x,&y,&(stLocalControl.stBusDealFreq.bySndSecAddr),1,WHITE,BLUE,3,page);

}


void StorSecInfo(uint8 page)
{
	uint16 xdata x,y;

	Lcd_Clear(WHITE,page);

	DisplayBlock(0,0,479,33,BLUE,page);

	x=96;
	y=0;
	DisplayASCIIString(&x,&y,"20",2,WHITE,BLUE,3,page);
	DisplayDigit(128,0,stLocalControl.stTime.byYear>>4,WHITE,BLUE,3,page);
	DisplayDigit(144,0,stLocalControl.stTime.byYear&0x0f,WHITE,BLUE,3,page);

	DisplayASCII(160,0,'/',WHITE,BLUE,3,page);

	DisplayDigit(176,0,stLocalControl.stTime.byMonth>>4,WHITE,BLUE,3,page);
	DisplayDigit(192,0,stLocalControl.stTime.byMonth&0x0f,WHITE,BLUE,3,page);	


	DisplayASCII(208,0,'/',WHITE,BLUE,3,page);

	DisplayDigit(224,0,stLocalControl.stTime.byDay>>4,WHITE,BLUE,3,page);
	DisplayDigit(240,0,stLocalControl.stTime.byDay&0x0f,WHITE,BLUE,3,page);

	
	//���������ּ��
	DisplayDigit(304,0,stLocalControl.stTime.byHour>>4,WHITE,BLUE,3,page);
	DisplayDigit(320,0,stLocalControl.stTime.byHour&0x0f,WHITE,BLUE,3,page);
	
	Display_ASCII(336,0,ColonCode16x32,WHITE,BLUE,3,page);	//":"

	DisplayDigit(352,0,stLocalControl.stTime.byMin>>4,WHITE,BLUE,3,page);
	DisplayDigit(368,0,stLocalControl.stTime.byMin&0x0f,WHITE,BLUE,3,page);	


	DisplayBlock(0,34,479,67,GREEN,page);


	if(WardDataBuff[6])
	{
		x=64;
		y=37;	
		(unOperData.ASCII_BCD)[0] = WardDataBuff[6]/100;
		(unOperData.ASCII_BCD)[1] = (WardDataBuff[6]%100)/10;
		(unOperData.ASCII_BCD)[2] = (WardDataBuff[6]%100)%10;
		if((unOperData.ASCII_BCD)[0])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLACK,GREEN,3,page);
			
		}
		else if((unOperData.ASCII_BCD)[1])
		{
			x +=16;
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,BLACK,GREEN,3,page);
			
		}
		else
		{
			x +=32;
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,BLACK,GREEN,3,page);
		}
	}

	
	x=(479-32*7)/2;	//128
	y=35;
	DisplayChineseString(&x,&y,"��������̬",6,BLACK,GREEN,3,page);

	x=0;y=72;
	DisplayChineseString(&x,&y,"��������",4,BLACK,PURPLE,2,page);

	x=120;y=72;
	DisplayBlock(120,72,120+35,72+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[7]/100;
	(unOperData.ASCII_BCD)[1]=WardDataBuff[7]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=107;
	DisplayChineseString(&x,&y,"��Σ",2,BLACK,YELLOW,2,page);
	
	x=72;y=107;
	DisplayBlock(72,107,72+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[8]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[8]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=160;
	DisplayChineseString(&x,&y,"��λ",2,BLACK,YELLOW,2,page);
	
	x=232;y=107;
	DisplayBlock(232,107,232+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[9]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[9]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=320;
	DisplayChineseString(&x,&y,"�մ�",2,BLACK,YELLOW,2,page);
	
	x=392;y=107;
	DisplayBlock(392,107,392+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[10]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[10]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=142;
	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2,page);
	
	x=72;y=142;
	DisplayBlock(72,142,72+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[11]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[11]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	x=240;
	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2,page);	
	
	x=312;y=142;
	DisplayBlock(312,142,312+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[12]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[12]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	

/*	x=0;y=175;
	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2,page);
	
	x=72;y=175;
	DisplayBlock(72,175,72+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[13]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[13]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=240;
	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2,page);
	
	x=312;y=175;
	DisplayBlock(312,175,312+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[14]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[14]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}*/
	
	
	
	x=0;y=175;  //209;
	DisplayChineseString(&x,&y,"һ������",4,BLACK,PURPLE,2,page);
	
	x=120;y=175;  //209;
	DisplayBlock(120,175/*209*/,120+35,175/*209*/+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[15]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[15]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	
	x=240;
	DisplayChineseString(&x,&y,"����",2,BLACK,YELLOW,2,page);
	x=312;y=175;   //209;
	DisplayBlock(312,175/*209*/,312+35,175/*209*/+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[16]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}						
	
	DisplayBlock(0,240,479,271,BLUE,page);
}



/*void ShowSecInfo(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==SEC_INFO_FACE) return;
	
	Lcd_Clear(WHITE,page);

	DisplayBlock(0,0,479,33,BLUE,page);

	x=96;
	y=0;
	DisplayASCIIString(&x,&y,"20",2,WHITE,BLUE,3,page);
	DisplayDigit(128,0,stLocalControl.stTime.byYear>>4,WHITE,BLUE,3,page);
	DisplayDigit(144,0,stLocalControl.stTime.byYear&0x0f,WHITE,BLUE,3,page);

	DisplayASCII(160,0,'/',WHITE,BLUE,3,page);

	DisplayDigit(176,0,stLocalControl.stTime.byMonth>>4,WHITE,BLUE,3,page);
	DisplayDigit(192,0,stLocalControl.stTime.byMonth&0x0f,WHITE,BLUE,3,page);	


	DisplayASCII(208,0,'/',WHITE,BLUE,3,page);

	DisplayDigit(224,0,stLocalControl.stTime.byDay>>4,WHITE,BLUE,3,page);
	DisplayDigit(240,0,stLocalControl.stTime.byDay&0x0f,WHITE,BLUE,3,page);

	
	//���������ּ��
	DisplayDigit(304,0,stLocalControl.stTime.byHour>>4,WHITE,BLUE,3,page);
	DisplayDigit(320,0,stLocalControl.stTime.byHour&0x0f,WHITE,BLUE,3,page);
	
	Display_ASCII(336,0,ColonCode16x32,WHITE,BLUE,3,page);	//":"

	DisplayDigit(352,0,stLocalControl.stTime.byMin>>4,WHITE,BLUE,3,page);
	DisplayDigit(368,0,stLocalControl.stTime.byMin&0x0f,WHITE,BLUE,3,page);	


	DisplayBlock(0,34,479,67,GREEN,page);


	if(WardDataBuff[6])
	{
		x=64;
		y=37;	
		(unOperData.ASCII_BCD)[0] = WardDataBuff[6]/100;
		(unOperData.ASCII_BCD)[1] = (WardDataBuff[6]%100)/10;
		(unOperData.ASCII_BCD)[2] = (WardDataBuff[6]%100)%10;
		if((unOperData.ASCII_BCD)[0])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLACK,GREEN,3,page);
			
		}
		else if((unOperData.ASCII_BCD)[1])
		{
			x +=16;
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,BLACK,GREEN,3,page);
			
		}
		else
		{
			x +=32;
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,BLACK,GREEN,3,page);
		}
	}

	
	x=(480-32*7)/2;	//128
	y=35;
	DisplayChineseString(&x,&y,"��������̬",6,BLACK,GREEN,3,page);

	x=0;y=72;
	DisplayChineseString(&x,&y,"��������",4,BLACK,PURPLE,2,page);

	x=120;y=72;
	DisplayBlock(120,72,120+35,72+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[7]/100;
	(unOperData.ASCII_BCD)[1]=WardDataBuff[7]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=107;
	DisplayChineseString(&x,&y,"��Σ",2,BLACK,YELLOW,2,page);
	
	x=72;y=107;
	DisplayBlock(72,107,72+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[8]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[8]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=160;
	DisplayChineseString(&x,&y,"��λ",2,BLACK,YELLOW,2,page);
	
	x=232;y=107;
	DisplayBlock(232,107,232+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[9]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[9]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=320;
	DisplayChineseString(&x,&y,"�մ�",2,BLACK,YELLOW,2,page);
	
	x=392;y=107;
	DisplayBlock(392,107,392+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[10]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[10]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=142;
	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2,page);
	
	x=72;y=142;
	DisplayBlock(72,142,72+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[11]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[11]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	x=240;
	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2,page);	
	
	x=312;y=142;
	DisplayBlock(312,142,312+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[12]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[12]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=175;
	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2,page);
	
	x=72;y=175;
	DisplayBlock(72,175,72+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[13]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[13]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=240;
	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2,page);
	
	x=312;y=175;
	DisplayBlock(312,175,312+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[14]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[14]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	x=0;y=209;
	DisplayChineseString(&x,&y,"һ������",4,BLACK,PURPLE,2,page);
	
	x=120;y=209;
	DisplayBlock(120,209,120+35,209+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[15]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[15]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	
	x=240;
	DisplayChineseString(&x,&y,"����",2,BLACK,YELLOW,2,page);
	x=312;y=209;
	DisplayBlock(312,209,312+35,209+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[16]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}						
	
	DisplayBlock(0,240,480,272,BLUE,page);

	Set_VisualPage(page);
	
	stLocalControl.byDisplayFace=SEC_INFO_FACE; 

}
*/


void ShowStartNumSet(uint8 page)
{

	uint16 xdata x,y;

	Lcd_Clear(BLUE,page);

	x=(479-32*10)/2;
	y=120;

	(unOperData.ASCII_BCD)[0]=stLocalControl.stBusDealFreq.bySndSecAddr/100;
	(unOperData.ASCII_BCD)[1]=(stLocalControl.stBusDealFreq.bySndSecAddr%100)/10;
	(unOperData.ASCII_BCD)[2]=(stLocalControl.stBusDealFreq.bySndSecAddr%100)%10;

	if((unOperData.ASCII_BCD)[0])
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,3,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
		}
		else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
	}
	DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);

	(unOperData.ASCII_BCD)[0]=stLocalControl.stBusDealFreq.bySndRoomAddr/100;
	(unOperData.ASCII_BCD)[1]=(stLocalControl.stBusDealFreq.bySndRoomAddr%100)/10;
	(unOperData.ASCII_BCD)[2]=(stLocalControl.stBusDealFreq.bySndRoomAddr%100)%10;

	if((unOperData.ASCII_BCD)[0])
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,3,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
		}
		else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
	}
	DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);

	(unOperData.ASCII_BCD)[0]=stLocalControl.stBusDealFreq.bySndBedAddr/100;
	(unOperData.ASCII_BCD)[1]=(stLocalControl.stBusDealFreq.bySndBedAddr%100)/10;
	(unOperData.ASCII_BCD)[2]=(stLocalControl.stBusDealFreq.bySndBedAddr%100)%10;

	if((unOperData.ASCII_BCD)[0])
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,3,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
		}
		else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
	}
	DisplayChineseString(&x,&y,"����ʼ���",5,WHITE,BLUE,3,page);

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=START_NUM_SET_FACE;	
	
}


void ShowNumSetOk(uint8 page)
{
	uint16 xdata x,y;	
	Lcd_Clear(BLUE,page);
	
	x=(479-32*4)/2;
	y=(271-32)/2;
	DisplayChineseString(&x,&y,"��ųɹ�",4,WHITE,BLUE,3,page);

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=NUM_SET_OK_FACE;	
}


/**********************************************************
*��������			:ShowBroadFace	
*��������        	:��ʾͨ������
*�������   		:byRoomAddr:����ַ,byBedAddr:����ַ,bSelf:�Ƿ�����㲥��
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2009-4-14
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf,uint8 page)
{ 
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==BROAD_FACE) return;

	
	Lcd_Clear(BLUE,page);	
	if(bSelf)
	{
		x=160;
		y=120;
		DisplayChineseString(&x,&y,"���ڹ㲥��",5,WHITE,BLUE,3,page);		
		
	}
	else
	{
		if(byRoomAddr==YHFJ_ROOM_ADDR)
		{
			x=(479-32*8)/2;
			y=120;
			(unOperData.ASCII_BCD)[0]=byBedAddr/100;
			(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
			(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
			if((unOperData.ASCII_BCD)[0])
			{
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
			}
			else
			{
				if((unOperData.ASCII_BCD)[1])
				{
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);
				}
				else
				{
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);				
				}
			}
			x+=16;
			DisplayChineseString(&x,&y,"��ҽ���ֻ��㲥",7,WHITE,BLUE,3,page);		
		}
		else
		{
			x=128;
			y=120;
			if(byBedAddr ==0x00)
			{
				(unOperData.ASCII_BCD)[0]=byRoomAddr/100;
				(unOperData.ASCII_BCD)[1]=(byRoomAddr%100)/10;
				(unOperData.ASCII_BCD)[2]=(byRoomAddr%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"���ſڷֻ��㲥",7,WHITE,BLUE,3,page);
				
			}
			else
			{
				(unOperData.ASCII_BCD)[0]=byRoomAddr/100;
				(unOperData.ASCII_BCD)[1]=(byRoomAddr%100)/10;
				(unOperData.ASCII_BCD)[2]=(byRoomAddr%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"��",1,WHITE,BLUE,3,page);
				(unOperData.ASCII_BCD)[0]=byBedAddr/100;
				(unOperData.ASCII_BCD)[1]=(byBedAddr%100)/10;
				(unOperData.ASCII_BCD)[2]=(byBedAddr%100)%10;
				DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,3,page);
				DisplayChineseString(&x,&y,"���㲥",3,WHITE,BLUE,3,page);
			}
		}
	}	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=BROAD_FACE;
 }


/*
void ShowStartInfusionCall(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==FUSION_CALL_FACE) return;

	Lcd_Clear(BLUE,page);	
	x=144;
	y=120;
	DisplayChineseString(&x,&y,"��ʼ�������",6,WHITE,BLUE,3,page);

	Set_VisualPage(page);

	stLocalControl.byDisplayFace=FUSION_CALL_FACE;

}

void ShowStartServiceCall(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==SERVICE_CALL_FACE) return;	
	Lcd_Clear(BLUE,page);
	
	x=144;
	y=120;
	DisplayChineseString(&x,&y,"��ʼ�������",6,WHITE,BLUE,3,page);
	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=SERVICE_CALL_FACE;

}

void ShowStartEmergencyCall(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==EMERGENCY_CALL_FACE) return;

	Lcd_Clear(BLUE,page);
	
	x=144;
	y=120;
	DisplayChineseString(&x,&y,"��ʼ��������",6,WHITE,BLUE,3,page);

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=EMERGENCY_CALL_FACE;

}

void ShowStartHelpCall(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==HELP_CALL_FACE) return;	
	Lcd_Clear(BLUE,page);
	
	x=144;
	y=120;
	DisplayChineseString(&x,&y,"��ʼ��Ԯ����",6,WHITE,BLUE,3,page);

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=HELP_CALL_FACE;
}

void ShowCallSuccess(uint8 page)
{
	uint16 xdata x,y;
	if(stLocalControl.byDisplayFace==CALL_SUCCESS_FACE) return;	 
	Lcd_Clear(BLUE,page);
	
	x=120;
	y=120;
	DisplayChineseString(&x,&y,"���гɹ�",4,WHITE,BLUE,3,page);
	DisplayASCIIString(&x,&y,",",1,WHITE,BLUE,2,page);
	DisplayChineseString(&x,&y,"��ȴ�",3,WHITE,BLUE,3,page);

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=CALL_SUCCESS_FACE;
}

*/


//x,y:��ʼ�������
//type_x_wide:ԭ����X��Ŀ��, type_y_height:ԭ����Y��ĸ߶�
//mul_row:����Ŵ���,   mul_col:����Ŵ�һ��
//cbc_filter:���˲��� 1:���˱���ɫ
//p:Ҫ��ʾ��ͼƬ����ָ��

void lcd_fangda_dis_text(uint16 x, uint16 y, uint8 type_x_wide, uint8 type_y_height, 
						 uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p,uint8 page)
{

	unsigned char xdata i,j,k,m,n;

	Set_Display_Area(x,x+type_x_wide*mul_col-1,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+type_y_height*mul_row-1);
	Send_Ssd1963_Command(0x2C);

	WDT_CONTR = 0x3d; 

	SSD1963_CS=0;
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
								WriteData16(fcolor);
							}

							else
							{
								WriteData16(bcolor);									
							}

						//}
					}
				}
			}
		}
	}
	SSD1963_CS=1;
}


void ShowPowerUpFace(uint8 page)	
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);
	
	x=(479-32*6)/2;
	y=(271-32*4)/2;
	DisplayChineseString(&x,&y,"ϵͳ��ʼ��",5,WHITE,BLUE,3,page);
	DisplayASCIIString(&x,&y,"...",3,WHITE,BLUE,3,page);

	y += 32;
	x=(479-32*4)/2;
	DisplayChineseString(&x,&y,"�汾",2,WHITE,BLUE,3,page);
	DisplayASCIIString(&x,&y,"V",1,WHITE,BLUE,3,page);
	(unOperData.ASCII_BCD)[0]=VERSION_HI/100;
	(unOperData.ASCII_BCD)[1]=(VERSION_HI%100)/10;
	(unOperData.ASCII_BCD)[2]=(VERSION_HI%100)%10;	
	if((unOperData.ASCII_BCD)[0])
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,3,page);
	}
	else
	{	
		if((unOperData.ASCII_BCD)[1])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);	
		}
		else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
	}
	DisplayASCIIString(&x,&y,".",1,WHITE,BLUE,3,page);
	
	(unOperData.ASCII_BCD)[0]=VERSION_LO/100;
	(unOperData.ASCII_BCD)[1]=(VERSION_LO%100)/10;
	(unOperData.ASCII_BCD)[2]=(VERSION_LO%100)%10;	
	if((unOperData.ASCII_BCD)[0])
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,3,page);
	}
	else
	{	
		if((unOperData.ASCII_BCD)[1])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,3,page);	
		}
		else DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,3,page);
	}	

/*	y += 32;
	x =(479-32*10)/2;
	DisplayChineseString(&x,&y,"����һ�ص���ҽ�ù���",10,WHITE,BLUE,3,page);

	y += 32;
	x =(479-32*6)/2;	
	DisplayChineseString(&x,&y,"�ɷ����޹�˾",6,WHITE,BLUE,3,page);*/

	y += 32;
	x =(479-32*10)/2;
	DisplayChineseString(&x,&y,"��������ϵͳҽ���ֻ�",10,WHITE,BLUE,3,page);

	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=POWER_UP_FACE;
}


//���˵���ʾ����
void ShowMainMenuFace(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);

	//����������
	DisplayBlock(0,0,479,1,WHITE,page);
	DisplayBlock(0,90,479,91,WHITE,page);
	DisplayBlock(0,180,479,181,WHITE,page);
	DisplayBlock(0,270,479,271,WHITE,page);

	//����������
	DisplayBlock(0,0,1,271,WHITE,page);
	DisplayBlock(159,0,160,271,WHITE,page);
	DisplayBlock(318,0,319,271,WHITE,page);
	DisplayBlock(478,0,479,271,WHITE,page);

	//1.ʱ������
	x=2+(157-32*3)/2;
	y=2+(88-32*2)/2;
	DisplayString(&x,&y,"1.ʱ��",6,WHITE,BLUE,2,page);
	x=2+(157-32*2)/2;
	y+= 32+4;
	DisplayString(&x,&y,"����",4,WHITE,BLUE,2,page);

	//2.���߱��
	x=161+(157-32*3)/2;
	y=2+(88-32*2)/2;
	DisplayString(&x,&y,"2.�豸",6,WHITE,BLUE,2,page);
	x=161+(157-32*2)/2;
	y+= 32+4;
	DisplayString(&x,&y,"���",4,WHITE,BLUE,2,page);	

	//3.����ʱ�������
	x=320+(157-32*4)/2;
	y=2+(88-32*2)/2;
	DisplayString(&x,&y,"3.����ʱ",8,WHITE,BLUE,2,page);
	x=320+(157-32*4)/2;
	y+= 32+4;
	DisplayString(&x,&y,"�������",8,WHITE,BLUE,2,page);

	//4.����ʱ�������
	x=2+(157-32*4)/2;
	y=92+(88-32*2)/2;
	DisplayString(&x,&y,"4.����ʱ",8,WHITE,BLUE,2,page);
	x=2+(157-32*4)/2;
	y+= 32+4;
	DisplayString(&x,&y,"�������",8,WHITE,BLUE,2,page);

	//5.ͳ������
	x=161+(157-32*3)/2;
	y=92+(88-32*2)/2;
	DisplayString(&x,&y,"5.ͳ��",6,WHITE,BLUE,2,page);
	x=161+(157-32*2)/2;
	y+= 32+4;
	DisplayString(&x,&y,"����",4,WHITE,BLUE,2,page);

	//6.����
	x=320+(157-32*3)/2;
	y=92+(88-32)/2;
	DisplayString(&x,&y,"6.����",6,WHITE,BLUE,2,page);


	//7.�����������
	x=2+(157-32*4)/2;
	y=182+(88-32*2)/2;
	DisplayString(&x,&y,"7.�����",8,WHITE,BLUE,2,page);
	x=2+(157-32*3)/2;
	y+= 32+4;
	DisplayString(&x,&y,"������",6,WHITE,BLUE,2,page);


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=MAIN_MENU_FACE;	

	stLocalControl.byKeyInTimes = 0;
}


void ShowNumberPWInputFace(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);
	for(x=0;x<10;x++)
	{
		(unOperData.ASCII_BCD)[x]=0x00;
	}
	x=10;
	y=30;
	DisplayString(&x,&y,"������������:",15,WHITE,BLUE,3,page);
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=NUMBER_PW_INPUT_FACE;	
}



//�豸��Ų˵���ʾ����
void ShowNumberSetFace(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);

	//����������
	DisplayBlock(0,0,479,1,WHITE,page);
	DisplayBlock(0,90,479,91,WHITE,page);
	DisplayBlock(0,180,479,181,WHITE,page);
	DisplayBlock(0,270,479,271,WHITE,page);

	//����������
	DisplayBlock(0,0,1,271,WHITE,page);
	DisplayBlock(159,0,160,271,WHITE,page);
	DisplayBlock(318,0,319,271,WHITE,page);
	DisplayBlock(478,0,479,271,WHITE,page);

	//1��ͷ�ֻ����
	x=2+(157-24*4)/2;
	y=17;		//y=2+(88-10-24*2)/2;	  ���Ϊ10
	DisplayString(&x,&y,"1.��ҽ��",8,WHITE,BLUE,2,page);
	x=2+(157-24*4)/2;
	y+= 24+10;
	DisplayString(&x,&y,"�ֻ����",8,WHITE,BLUE,2,page);

	//2.��ԡ�ſڷֻ����
	x=161+(157-24*5)/2;
	y=17;		//y=2+(88-10-24*2)/2;	  ���Ϊ10
	DisplayString(&x,&y,"2.��ԡ�ſ�",10,WHITE,BLUE,2,page);
	x=161+(157-24*4)/2;
	y+= 24+10;
	DisplayString(&x,&y,"�ֻ����",8,WHITE,BLUE,2,page);	

	//3.�ƶ��ֻ����
	x=320+(157-24*3)/2;
	y=17;		//y=2+(88-10-24*2)/2;	  ���Ϊ10
	DisplayString(&x,&y,"3.�ƶ�",6,WHITE,BLUE,2,page);
	x=320+(157-24*4)/2;
	y+= 24+10;
	DisplayString(&x,&y,"�ֻ����",8,WHITE,BLUE,2,page);

	//4.�Ӵ��ֻ����
	x=2+(157-24*3)/2;
	y=107;		//y=92+(88-10-24*2)/2;	  ���Ϊ10	
	DisplayString(&x,&y,"4.�Ӵ�",6,WHITE,BLUE,2,page);
	x=2+(157-24*4)/2;
	y+= 24+10;
	DisplayString(&x,&y,"�ֻ����",8,WHITE,BLUE,2,page);

	//4.��ҽ���ֻ����
	x=161+(157-24*3)/2;
	y=107;		//y=92+(88-10-24*2)/2;	  ���Ϊ10	
	DisplayString(&x,&y,"5.��ͷ",6,WHITE,BLUE,2,page);
	x=161+(157-24*4)/2;
	y+= 24+10;
	DisplayString(&x,&y,"�ֻ����",8,WHITE,BLUE,2,page);
	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=NUMBER_SET_FACE;	
}

void ShowTimeSet(uint8 page)
{
	uint16 xdata x,y;
	Lcd_Clear(BLUE,page);

	x= (480-24*11)/2;	//108
	y= (272-32*5-10)/2;	//ÿһ��������

	DisplayString(&x,&y,"���������:",11,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+31,y+23,WHITE,page);
	(unOperData.ASCII_BCD)[0]=stLocalControl.stTime.byYear>>4;
	(unOperData.ASCII_BCD)[1]=stLocalControl.stTime.byYear&0x0f;
	x +=4;
	DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLUE,WHITE,2,page);
	x +=32;
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);

	
	x= (480-24*11)/2;
	y += 34;
	DisplayString(&x,&y,"�������·�:",11,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+31,y+23,WHITE,page);
	(unOperData.ASCII_BCD)[2]=stLocalControl.stTime.byMonth>>4;
	(unOperData.ASCII_BCD)[3]=stLocalControl.stTime.byMonth&0x0f;
	x +=4;
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),2,BLUE,WHITE,2,page);	
	

	x= (480-24*11)/2;
	y += 34;
	DisplayString(&x,&y,"����������:",11,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+31,y+23,WHITE,page);
	(unOperData.ASCII_BCD)[4]=stLocalControl.stTime.byDay>>4;
	(unOperData.ASCII_BCD)[5]=stLocalControl.stTime.byDay&0x0f;
	x +=4;
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[4]),2,BLUE,WHITE,2,page);	
	

	x= (480-24*11)/2;
	y += 34;
	DisplayString(&x,&y,"������Сʱ:",11,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+31,y+23,WHITE,page);
	(unOperData.ASCII_BCD)[6]=stLocalControl.stTime.byHour>>4;
	(unOperData.ASCII_BCD)[7]=stLocalControl.stTime.byHour&0x0f;
	x +=4;
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),2,BLUE,WHITE,2,page);	
	

	x= (480-24*11)/2;
	y += 34;
	DisplayString(&x,&y,"���������:",11,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+31,y+23,WHITE,page);
	(unOperData.ASCII_BCD)[8]=stLocalControl.stTime.byMin>>4;
	(unOperData.ASCII_BCD)[9]=stLocalControl.stTime.byMin&0x0f;
	x +=4;
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[8]),2,BLUE,WHITE,2,page);	
	

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=TIME_SET_FACE;	

}


/*void ShowNumberSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);

	x= (480-24*8)/2;
	y= (272-24*4-8)/2; //ÿһ��������

	DisplayString(&x,&y,"�ֻ����,������",15,WHITE,BLUE,2,page);

	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�
	x += 68;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�

	

	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�



	Set_VisualPage(page);
	stLocalControl.byDisplayFace=NUMBER_SET_FACE; 
}
*/

void ShowBedFjNumberSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);
	memset((unOperData.ASCII_BCD),0x00,10);

	x= (480-24*10)/2;
	y= (272-24*4-8)/2; //ÿһ��������

	DisplayString(&x,&y,"��ͷ�ֻ����,������",19,WHITE,BLUE,2,page);

	x= (480-24*8)/2;
	y += 34;
	
	/*DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,page);	
//	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,page);	
//	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�

	

	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);		
//	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�
*/

	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,page); 
//	DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,page); 
//	DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�
	

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=BED_FJ_NUMBER_SET_FACE; 
}


void ShowWcDoorFjNumberSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);
	memset((unOperData.ASCII_BCD),0x00,10);

	x= (480-24*12)/2;
	y= (272-24*4-8)/2; //ÿһ��������

	DisplayString(&x,&y,"��ԡ�ſڷֻ����,������",23,WHITE,BLUE,2,page);
	
/*	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,page);
//	DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	x= (480-24*8)/2;
	x -=24;
	y += 34;
	DisplayString(&x,&y,"�ֻ���:",7,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);
//	DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�	
*/

	x= (480-24*8)/2;
	x -=24;
	y += 34;
	DisplayString(&x,&y,"�ֻ���:",7,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);
//	DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=WC_FJ_NUMBER_SET_FACE; 
}


void ShowMoveFjNumberSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);
	memset((unOperData.ASCII_BCD),0x00,10);
	unOperData.ASCII_BCD[2] = 1;  //�ƶ��ֻ���Ŵ�1�ſ�ʼ
	unOperData.ASCII_BCD[5] = 1;	//��ʼ����Ĭ��Ϊ1��
	unOperData.ASCII_BCD[6] = 2;	//��������Ĭ��Ϊ255��
	unOperData.ASCII_BCD[7] = 5;
	unOperData.ASCII_BCD[8] = 5;


	x= (480-24*10)/2;  //120
	y= (272-34*4)/2; //ÿһ����10��   //68
	DisplayString(&x,&y,"�ƶ��ֻ����,������",19,WHITE,BLUE,2,page);


	x= (480-24*10)/2; //120
	y += 34;     //102
	DisplayString(&x,&y,"�ֻ���:",7,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,page);  //120+12*7=204
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);	//204+12*3+32= 272

	x= (480-24*10)/2;	//120
	y += 34;   //136
	DisplayString(&x,&y,"��ʼ����:",9,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,page); //120+12*9 = 228

	x= (480-24*10)/2;
	y += 34;
	DisplayString(&x,&y,"��������:",9,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);	

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=MOVE_FJ_NUMBER_SET_FACE; 
}


void ShowAddFjNumberSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);
	memset((unOperData.ASCII_BCD),0x00,10);

	x= (480-24*10)/2;
	y= (272-24*4-8)/2; //ÿһ��������

	DisplayString(&x,&y,"�Ӵ��ֻ����,������",19,WHITE,BLUE,2,page);

/*	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,page);
	//DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�	
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	x= (480-24*8)/2;
	x -=24;
	y += 34;
	DisplayString(&x,&y,"�ֻ���:",7,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);
	//DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�
*/
	x= (480-24*8)/2;
	x -=24;
	y += 34;
	DisplayString(&x,&y,"�ֻ���:",7,WHITE,BLUE,2,page);
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);
	//DisplayBlock(x,y,x+36,y+23,WHITE,page); // 3 ���ռ�
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=ADD_FJ_NUMBER_SET_FACE; 
}




void ShowSelfNumberSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(BLUE,page);

	(unOperData.ASCII_BCD)[0] = stLocalControl.stEepromCfgData.bySelfSecAddr/100;
	(unOperData.ASCII_BCD)[1] = (stLocalControl.stEepromCfgData.bySelfSecAddr%100)/10;
	(unOperData.ASCII_BCD)[2] = (stLocalControl.stEepromCfgData.bySelfSecAddr%100)%10;	

	x= (480-24*8)/2;
	y= (272-24*4-8)/2; //ÿһ��������

	DisplayString(&x,&y,"�������,������",15,WHITE,BLUE,2,page);

	x= (480-24*8)/2;
	y += 34;
	DisplayString(&x,&y,"����:",5,WHITE,BLUE,2,page);
//	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,page);
	x += 32;	//�����������ֿ�32���
	DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,page);



	(unOperData.ASCII_BCD)[6] = stLocalControl.stEepromCfgData.bySelfBedAddr/100;
	(unOperData.ASCII_BCD)[7] = (stLocalControl.stEepromCfgData.bySelfBedAddr%100)/10;
	(unOperData.ASCII_BCD)[8] = (stLocalControl.stEepromCfgData.bySelfBedAddr%100)%10;

	x= (480-24*8)/2;
	x -=24;
	y += 34;
	DisplayString(&x,&y,"�ֻ���:",7,WHITE,BLUE,2,page);
//	DisplayBlock(x,y,x+36,y+23,WHITE,page);	// 3 ���ռ�
	DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,page);
	

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=SELF_NUMBER_SET_FACE; 	
}


void ShowVoiceTimerSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(YELLOW,page);

	x= 32;
	y= (272-24*7-70)/2; //ÿһ����10��

	DisplayChineseString(&x,&y,"С����ʱ����ʼʱ��",9,BLACK,YELLOW,2,page);

	x= 64;
	y += 34;
	DisplayString(&x,&y,"������Сʱ:",11,BLACK,YELLOW,2,page);
//	DisplayBlock(x,y,x+23,y+23,CYAN,page); // 2 ���ռ�
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[5]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[5]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,RED,CYAN,2,page);
	
	x += 32;
	DisplayString(&x,&y,"��[ȷ��]��",10,BLUE,YELLOW,2,page);

	x= 64+24*3;
	y += 34;
	DisplayString(&x,&y,"����:",5,BLACK,YELLOW,2,page);
//	DisplayBlock(x,y,x+23,y+23,CYAN,page); // 2 ���ռ�
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[6]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[6]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,RED,CYAN,2,page);



	x= 32;
	y += 34;
	DisplayChineseString(&x,&y,"����ʱ��",4,BLACK,YELLOW,2,page);

	x= 64;
	y += 34;
	DisplayString(&x,&y,"������Сʱ:",11,BLACK,YELLOW,2,page);
//	DisplayBlock(x,y,x+23,y+23,CYAN,page); // 2 ���ռ�
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[7]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[7]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,RED,CYAN,2,page);


	x= 64+24*3;
	y += 34;
	DisplayString(&x,&y,"����:",5,BLACK,YELLOW,2,page);
//	DisplayBlock(x,y,x+23,y+23,CYAN,page); // 2 ���ռ�
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[8]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[8]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,RED,CYAN,2,page);


	x=64;
	y +=34;
	DisplayString(&x,&y,"����������:",11,BLACK,YELLOW,2,page);
//	DisplayBlock(x,y,x+23,y+23,CYAN,page); // 2 ���ռ�	
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[9]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[9]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,RED,CYAN,2,page);


	unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[5]>>4;
	unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[5]&0x0f;

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=VOICE_TIMER_SET_FACE; 

}


void ShowMaxVolSetFace(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(PURPLE,page);


	x= 32;
	y= 60;

	DisplayString(&x,&y,"1.���������������:",19,GREEN,PURPLE,2,page);

	x=272;
	(unOperData.ASCII_BCD)[0] = stLocalControl.stEepromCfgData.byMaxPlayVolVal;
	
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,1,RED,CYAN,2,page);

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=MAX_VOL_SET_FACE;	
}




void ShowBackLightTimerSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(CYAN,page);

	x= 32;
	y= (272-24*7-70)/2; //ÿһ����10��

	DisplayChineseString(&x,&y,"�������ʼʱ��",7,BLUE,CYAN,2,page);

	x= 64;
	y += 34;
	DisplayString(&x,&y,"������Сʱ:",11,BLUE,CYAN,2,page);
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[5]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[5]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLUE,RED,2,page);


	x += 32;
	DisplayString(&x,&y,"��<ȷ��>��",10,YELLOW,CYAN,2,page);

	x= 64+24*3;
	y += 34;
	DisplayString(&x,&y,"����:",5,BLUE,CYAN,2,page);
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[6]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[6]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLUE,RED,2,page);



	x= 32;
	y += 68;
	DisplayChineseString(&x,&y,"����ʱ��",4,BLUE,CYAN,2,page);

	x= 64;
	y += 34;
	DisplayString(&x,&y,"������Сʱ:",11,BLUE,CYAN,2,page);
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[7]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[7]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLUE,RED,2,page);



	x= 64+24*3;
	y += 34;
	DisplayString(&x,&y,"����:",5,BLUE,CYAN,2,page);
	(unOperData.ASCII_BCD)[0] = (unOperData.ASCII_BCD)[8]>>4;
	(unOperData.ASCII_BCD)[1] = (unOperData.ASCII_BCD)[8]&0x0f;
	DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLUE,RED,2,page);

	unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[5]>>4;
	unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[5]&0x0f;

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=BACK_LIGHT_TIMER_SET_FACE; 

}


void ShowSecInfoSet(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(WHITE,page);

	Read_Cont(WARD_FACE_DATA_START_ADDR,WARD_FACE_DATA_LEN,WardDataBuff);


	x=64;y=30;
	DisplayChineseString(&x,&y,"������̬��Ϣ����",8,BLUE,WHITE,3,page);

	x=0;y=72;
	DisplayChineseString(&x,&y,"����",2,BLACK,GREEN,2,page);

	x=72;y=72;
	DisplayBlock(72,72,72+35,72+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[6]/100;
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[6]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[6]%100)%10;
	
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}

	
	x=240;y=72;
	DisplayChineseString(&x,&y,"��������",4,BLACK,PURPLE,2,page);

	x=360;y=72;
	DisplayBlock(360,72,360+35,72+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[7]/100;
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[7]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[7]%100)%10;
	
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=107;
	DisplayChineseString(&x,&y,"��Σ",2,BLACK,YELLOW,2,page);
	
	x=72;y=107;
	DisplayBlock(72,107,72+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[8]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[8]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[8]%100)%10;	
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=160;
	DisplayChineseString(&x,&y,"��λ",2,BLACK,YELLOW,2,page);
	
	x=232;y=107;
	DisplayBlock(232,107,232+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[9]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[9]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[9]%100)%10;	
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=320;
	DisplayChineseString(&x,&y,"�մ�",2,BLACK,YELLOW,2,page);
	
	x=392;y=107;
	DisplayBlock(392,107,392+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[10]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[10]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[10]%100)%10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=142;
	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2,page);
	
	x=72;y=142;
	DisplayBlock(72,142,72+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[11]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[11]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[11]%100)%10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	x=240;
	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2,page);	
	
	x=312;y=142;
	DisplayBlock(312,142,312+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[12]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[12]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[12]%100)%10;

	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=0;y=175;
	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2,page);
	
	x=72;y=175;
	DisplayBlock(72,175,72+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[13]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[13]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[13]%100)%10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	x=240;
	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2,page);
	
	x=312;y=175;
	DisplayBlock(312,175,312+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[14]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[14]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[14]%100)%10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	x=0;y=209;
	DisplayChineseString(&x,&y,"һ������",4,BLACK,PURPLE,2,page);
	
	x=120;y=209;
	DisplayBlock(120,209,120+35,209+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[15]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[15]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[15]%100)%10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	
	
	
	
	x=240;
	DisplayChineseString(&x,&y,"����",2,BLACK,YELLOW,2,page);
	x=312;y=209;
	DisplayBlock(312,209,312+35,209+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
	
	(unOperData.ASCII_BCD)[1]=(WardDataBuff[16]%100)/10;	
	(unOperData.ASCII_BCD)[2]=(WardDataBuff[16]%100)%10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}						

	x=48; y=240;
	DisplayString(&x,&y,"ÿ������һ����Ϣ�밴[ȷ��]��",28,BLUE,WHITE,2,page);


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=SEC_INFO_SET_FACE; 
	stLocalControl.bySecondFlashTime=25;

}


void ShowCalculateFace(uint8 page)
{
	uint16 x,y;
	Lcd_Clear(PURPLE,page);


	x= 32;
	y= 60;

	DisplayChineseString(&x,&y,"���ü�����������",8,GREEN,PURPLE,2,page);

	DisplayBlock(100,100,480-100,100+31,WHITE,page);
	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=CALCULATE_FACE; 	
}






void ShowTouchAdjust(uint8 page)
{
	Lcd_Clear(BLUE,page);		
	Display_HLine(15,30,30,WHITE,page);
	Display_VLine(30,15,30,WHITE,page);
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=TOUCH_ADJUST_FACE;

}


void StoreSecInfo(uint8 page)	//�洢������̬��Ϣ����ʾ�����ڴ�
{
	uint16 x,y;

	DisplayBlock(64,35,127,66,GREEN,page);
	if(WardDataBuff[6])
	{
		x=64;
		y=37;	
		(unOperData.ASCII_BCD)[0] = WardDataBuff[6]/100;
		(unOperData.ASCII_BCD)[1] = (WardDataBuff[6]%100)/10;
		(unOperData.ASCII_BCD)[2] = (WardDataBuff[6]%100)%10;
		if((unOperData.ASCII_BCD)[0])
		{
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLACK,GREEN,3,page);
			
		}
		else if((unOperData.ASCII_BCD)[1])
		{
			x +=16;
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,BLACK,GREEN,3,page);
			
		}
		else
		{
			x +=32;
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,BLACK,GREEN,3,page);
		}
	}

	
	x=120;y=72;
	DisplayBlock(120,72,120+35,72+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[7]/100;
	(unOperData.ASCII_BCD)[1]=WardDataBuff[7]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}


//	x=0;y=107;
//	DisplayChineseString(&x,&y,"��Σ",2,BLACK,YELLOW,2);

	x=72;y=107;
	DisplayBlock(72,107,72+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[8]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[8]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}


//	x=160;
//	DisplayChineseString(&x,&y,"��λ",2,BLACK,YELLOW,2);
	
	x=232;y=107;
	DisplayBlock(232,107,232+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[9]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[9]%100;	//ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
	

//	x=320;
//	DisplayChineseString(&x,&y,"�մ�",2,BLACK,YELLOW,2);
	
	x=392;y=107;
	DisplayBlock(392,107,392+35,107+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[10]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[10]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}

	
//	x=0;y=142;
//	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2);

	x=72;y=142;
	DisplayBlock(72,142,72+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[11]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[11]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}

	

//	x=240;
//	DisplayChineseString(&x,&y,"��Ժ",2,BLACK,YELLOW,2);	

	x=312;y=142;
	DisplayBlock(312,142,312+35,142+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[12]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[12]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}



/*
//	x=0;y=175;
//	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2);
	
	x=72;y=175;
	DisplayBlock(72,175,72+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[13]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[13]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}


//	x=240;
//	DisplayChineseString(&x,&y,"ת��",2,BLACK,YELLOW,2);
	
	x=312;y=175;
	DisplayBlock(312,175,312+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[14]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[14]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}
*/
	

//	x=0;y=209;
//	DisplayChineseString(&x,&y,"һ������",4,BLACK,PURPLE,2);
	
	x=120;y=175;//209;
	DisplayBlock(120,175,120+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[15]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[15]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}




//	x=240;
//	DisplayChineseString(&x,&y,"����",2,BLACK,YELLOW,2);
	x=312;y=175;//209;
	DisplayBlock(312,175,312+35,175+23,BLUE,page);
	(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
	
	(unOperData.ASCII_BCD)[1]=WardDataBuff[16]%100; //ʮ��λ
	(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[1]%10;
	(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[1]/10;
	
	if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
	{
		DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,page);
	}
	else
	{
		if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
			DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,page);
		else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,page);
	}						


	if(stLocalControl.byDisplayFace==SEC_INFO_FACE) Set_VisualPage(page);	

}


/*---------------------------------------------------------------------------
����ԭ��: void InitSSD1963(void)
����˵��: ��
�� �� ֵ: ��
��������: ��ʼ��LCD������
----------------------------------------------------------------------------*/
void InitSSD1963(void)
{
//	uint8 i;
//	uint16 xdata x,y;
	//-------------------------
	SSD1963_TE=1;
	SSD1963_RD=1;
	SSD1963_CS=0;
	//-------------------------
	SSD1963_RST=1;			   //Ӳ����λ
	Delayms(50);
	SSD1963_RST=0;
	Delayms(200);
	SSD1963_RST=1;
	Delayms(50);

	
	//�趨PLL��Ƶϵ��
	//pll: n= 34 m= 3
	Send_Ssd1963_Command(cmd_set_pll_mn);		   
	Send_Ssd1963_Data(0x3B); //��Ƶϵ��Ϊ60 ����Ϊ6M ��Ƶ��Ϊ360M VCO
	Send_Ssd1963_Data(0x02); //��Ƶϵ��Ϊ3 ����Ƶ��Ϊ360M/3=42M
	Send_Ssd1963_Data(0x54); //�̶�ֵ	

	Send_Ssd1963_Command(cmd_set_pll); //����PLL
	Send_Ssd1963_Data(0x01);   
	//��ʱ1Ms
	Delayms(1);
	Send_Ssd1963_Command(cmd_set_pll);		 //����PLL
	Send_Ssd1963_Data(0x03);

	//software reset		

	Send_Ssd1963_Command(cmd_soft_reset);
	
	//�趨����ʱ��	
	//��ĻΪ4.3ʱ(480x272)��֡ƵΪ30FPS ����ʱ��Ϊ3.9168M	FPR=32596		 60FPSʱ FPR= 65192
	Send_Ssd1963_Command(cmd_set_lshift_freq);			
	Send_Ssd1963_Data(0x01); //00
	Send_Ssd1963_Data(0x33); //63
	Send_Ssd1963_Data(0x32); //32
	
	//�������趨		   
	Send_Ssd1963_Command(cmd_set_lcd_mode);
	Send_Ssd1963_Data(0x18);  //��ʾ18bit���� ����FRC �½��ؼ��͵�ƽ��Ч
	Send_Ssd1963_Data(0x20);  //TFT RGB���� ��������ź�
	Send_Ssd1963_Data((DISP_HOR_RESOLUTION-1)>>8);
	Send_Ssd1963_Data(DISP_HOR_RESOLUTION-1);  //ˮƽ����480
	Send_Ssd1963_Data((DISP_VER_RESOLUTION-1)>>8); 
	Send_Ssd1963_Data(DISP_VER_RESOLUTION-1);  //Hor ����272
	Send_Ssd1963_Data(0x00);  //��������RGB


		
	//ˮƽͬ���ź��趨
	Send_Ssd1963_Command(cmd_set_hori_period);	   
		 
#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
	Send_Ssd1963_Data((HT-1)>>8);
	Send_Ssd1963_Data(HT-1); //ˮƽ���� 503��ʱ������ (������ʾ���������ʾ����)
#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
	Send_Ssd1963_Data((HPS-1)>>8); 
	Send_Ssd1963_Data(HPS-1); //ˮƽ����ʾ����ʱ�Ӹ���Ϊ42
	Send_Ssd1963_Data(DISP_HOR_PULSE_WIDTH-1); //ˮƽͬ���źſ��ʱ�Ӹ���Ϊ3
	Send_Ssd1963_Data(0x00); 
	Send_Ssd1963_Data(0x00); //ˮƽͬ���źſ�ʼλ��
	Send_Ssd1963_Data(0x00);
	
	//vsync setting
	Send_Ssd1963_Command(cmd_set_vert_period); 
	
#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)    
	Send_Ssd1963_Data((VT-1)>>8);  //
	Send_Ssd1963_Data(VT-1);  //preiod width=  295
#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)
	Send_Ssd1963_Data((VSP-1)>>8);
	Send_Ssd1963_Data(VSP-1);  //no-preiod width=10
	Send_Ssd1963_Data(DISP_VER_PULSE_WIDTH-1);	//plus width=3
	Send_Ssd1963_Data(0x00);
	Send_Ssd1963_Data(0x00);

		//��ʾ���ظ�ʽ�趨
	Send_Ssd1963_Command(cmd_set_pixel_format); 	  
#ifndef DATA_BUS8
	Send_Ssd1963_Data(0x50); //16bit
#else
	Send_Ssd1963_Data(RGB_DATA_FORMAT_18); //24bit
#endif

	//�������߽ӿ��趨
	
	Send_Ssd1963_Command(cmd_set_pixel_data_interface);
#ifndef DATA_BUS8	     
	Send_Ssd1963_Data(0x03);   //16BIT RGB565
#else
	Send_Ssd1963_Data(0x00);   //8BIT
#endif
	
#ifdef USE_SSD1963_PWM	
	Send_Ssd1963_Command(0xbe);//����PWM 
	Send_Ssd1963_Data(0X0E);
	Send_Ssd1963_Data(0XFF);
	Send_Ssd1963_Data(0X09);
	Send_Ssd1963_Data(0XFF);
	Send_Ssd1963_Data(0X00);
	Send_Ssd1963_Data(0X00); 
	//(Define the threshold value)
	Send_Ssd1963_Command(cmd_set_dbc_th); 
	Send_Ssd1963_Data(0X00);
	Send_Ssd1963_Data(0X09);
	Send_Ssd1963_Data(0X90);
	Send_Ssd1963_Data(0X00);
	Send_Ssd1963_Data(0X17);
	Send_Ssd1963_Data(0XE8);
	Send_Ssd1963_Data(0X00); 
	Send_Ssd1963_Data(0X39);
	Send_Ssd1963_Data(0X60); 
	
	Send_Ssd1963_Command(cmd_set_dbc_conf);
	Send_Ssd1963_Data(0x0d);
#endif
	Send_Ssd1963_Command(0xB8); 			// Set all GPIOs to output, controlled by host
	Send_Ssd1963_Data(0x01);				// Set GPIO0 as output
	Send_Ssd1963_Data(0x01);				// GPIO[3:0] used as normal GPIOs
	Send_Ssd1963_Command(0xBA); 			// Set all GPIOs Value
	Send_Ssd1963_Data(0x01);				// Set GPIO0 to high

	SetScrollArea(0,272,0);

	//����ʾ
	Send_Ssd1963_Command(cmd_set_display_on);



	Lcd_Clear(BLUE,0);
	Set_VisualPage(0);

	P3M0 |= (Bin(0,0,1,0,0,0,0,0));
	P3M1 &= (~Bin(0,0,1,0,0,0,0,0));
	BL_SW=1;

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

	while(bLanding)
	{		
		WDT_CONTR = 0x3d; 
		OSWait(K_TMO, 10);					
	}
	
	Lcd_Clear(BLUE,DIS_FRAM_MEM1);

	

	Set_VisualPage(DIS_FRAM_MEM0);
	stLocalControl.byDisplayFace=SEC_INFO_FACE; 

	
	while(TRUE)
	{		
		WDT_CONTR = 0x3d; 
		OSWait(K_TMO, 10);			 		
	}	

}

