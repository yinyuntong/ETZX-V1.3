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


uint8 xdata fontbuf1,fontbuf2,fontbuf3;
uint8 xdata BedDataBuff[SEGMENT_DATA_LEN];


uint16 code ColorArray[8]={BLACK,RED,GREEN,BLUE,YELLOW,PURPLE,CYAN,WHITE};

uint16 x;
uint16 y;
uint8 size;
uint8 mul_row;
uint8 mul_col;
uint16 fcolor;
uint16 bcolor;
uint8 *pDispData;
uint8 page;
uint16 start_x;
uint16 start_y;
uint16 end_x;
uint16 end_y;
uint8 Length;
uint8 DotWidth;

bit fTitle_TextAtLine=0;	//�����������ͬһ�б�־

uint8 dot_number=0;
uint8  current_size=0;	//��ǰ�ַ��ߴ�
uint8  current_vert_spac=0;//��ǰ�м��


void DispManager(void);

void Send_Ssd1963_Command(uint8 ssd1963_command);
void Send_Ssd1963_Data(uint8 ssd1963_data);
void InitSSD1963(void);
void Set_Display_Area(uint16 start_x,uint16 end_x,uint16 start_y,uint16 end_y);
void Display_Dot(void);
void Display_HLine(uint16  width);
void Display_VLine(uint16  height);
void Lcd_Clear(void);



void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf);
void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCallType);
void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr);
void ShowBedFace(void);

void ShowStartInfusionCall();
void ShowStartServiceCall();
void ShowStartEmergencyCall();
void ShowStartHelpCall();
void ShowCallSuccess();

void lcd_fangda_dis_text(uint8 type_x_wide, uint8 type_y_height);
void DisplayDigitString(void);
void DisplayString(void);
void DisplayStringLen(void);
void DisplayOneChar(uint8 byChar);



void ShowPatientBaseInfo(void);


void BL_OFF(void);
void BL_ON(void);
void ShowPowerUpFace(void);	//д�ڵ�0ҳ
void Set_VisualPage(uint8 VisualPage);
void ShowBedFaceForm(void);


void ShowItem(uint8 ItemCode);


uint8 GetTitleLength(uint8 ItemCode);
uint8* GetTitle(uint8 ItemCode);


void get_dot_number();
void get_basesize_mul();



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


void SSD1963Data_16(uint16 sdata)

{
	SSD1963_CS=0;
	DataPortL=sdata;//temp.UByte[1];
	DataPortH=sdata>>8;//temp.UByte[0];
	SSD1963_WR=0;
	SSD1963_WR=1;
	SSD1963_CS=1;
}


/*---------------------------------------------------------------------------
����ԭ��: void Read_Ssd1963_Data()
����˵��: 
�� �� ֵ: ��
��������: ������
----------------------------------------------------------------------------*/
uint8 Read_Ssd1963_Data()
{
	uint8 RdData;
	SSD1963_CS=0;
	SSD1963_RD=0;
	SSD1963_RD=1;	
	RdData=DataPortL;
	SSD1963_CS=1;
	return(RdData);

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

/*---------------------------------------------------------------------------
����ԭ��: void InitSSD1963(void)
����˵��: ��
�� �� ֵ: ��
��������: ��ʼ��LCD������
----------------------------------------------------------------------------*/
void InitSSD1963(void)
{
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

	//����RGB��ɫ˳��
	Send_Ssd1963_Command(0x36);
	Send_Ssd1963_Data(0x08);   //RGB 

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


/*	Send_Ssd1963_Command(0xBC);
	Send_Ssd1963_Data(0x40);//�Աȶ�ֵ
	Send_Ssd1963_Data(0x10);//����ֵ  
	Send_Ssd1963_Data(0x40);//���Ͷ�ֵ 
	Send_Ssd1963_Data(0x01);//���������
*/




// set pwm enable by host
	P3M0 |= Bin(0,0,1,0,0,0,0,0);	//��©���
	P3M1 |= Bin(0,0,1,0,0,0,0,0);
	BL_SW=1;
	Send_Ssd1963_Command(0xBE); //SET_PWM_CONF
	Send_Ssd1963_Data(0x0e);//set pwm freq
//	Send_Ssd1963_Data(0x3f);//set pwm duty cycle  
	Send_Ssd1963_Data(0xf0);//set pwm duty cycle  

	Send_Ssd1963_Data(0x01);//enable pwm by host
	Send_Ssd1963_Data(0xff);//DBC manu brightness	
	Send_Ssd1963_Data(0x00);//DBC mini brightness
	Send_Ssd1963_Data(0x00);//brightness prescaler	

	
	//����ʾ
	Send_Ssd1963_Command(cmd_set_display_on);


/*
	//ǿ�������
	P3M0 |= (Bin(0,0,1,0,0,0,0,0));
	P3M1 &= (~Bin(0,0,1,0,0,0,0,0));
	//�򿪱���
	BL_SW=1;
*/
}


void BL_OFF(void)
{
		Send_Ssd1963_Command(0xBE); //SET_PWM_CONF
		Send_Ssd1963_Data(0x0e);//set pwm freq
	//	Send_Ssd1963_Data(0x3f);//set pwm duty cycle  
		Send_Ssd1963_Data(0x00);//set pwm duty cycle  
	
		Send_Ssd1963_Data(0x01);//enable pwm by host
		Send_Ssd1963_Data(0xff);//DBC manu brightness	
		Send_Ssd1963_Data(0x00);//DBC mini brightness
		Send_Ssd1963_Data(0x00);//brightness prescaler	

		Send_Ssd1963_Command(cmd_set_display_off);

}


void BL_ON(void)
{
		Send_Ssd1963_Command(0xBE); //SET_PWM_CONF
		Send_Ssd1963_Data(0x0e);//set pwm freq
	//	Send_Ssd1963_Data(0x3f);//set pwm duty cycle  
		Send_Ssd1963_Data(0xf0);//set pwm duty cycle  
	
		Send_Ssd1963_Data(0x01);//enable pwm by host
		Send_Ssd1963_Data(0xff);//DBC manu brightness	
		Send_Ssd1963_Data(0x00);//DBC mini brightness
		Send_Ssd1963_Data(0x00);//brightness prescaler

		Send_Ssd1963_Command(cmd_set_display_on);
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
	else if(srgb==BLUE3_CODE) return(BLUE3_VALUE);
	else if(srgb==BLUE4_CODE) return(BLUE4_VALUE);	
	else return(WHITE); 
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





/*--------------------------------------------------------------------------------------------
����ԭ��: void Lcd_Clear(uint32 xdata color)
����˵��: color:��������ɫ
�� �� ֵ: ��
��������: ����
----------------------------------------------------------------------------------------------*/
//void Lcd_Clear(uint16  color,uint8 page)
void Lcd_Clear(void)
{
  uint16 xdata k,j;

  Set_Display_Area(0,479,page*LCD_PIX_Y,page*LCD_PIX_Y+271);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  for(k=0;k<480;k++)
    {
     for(j=0;j<272;j++)
     {
		WriteData16(fcolor);
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
//void Display_Dot(uint16  x,uint16  y,uint16  color,uint8 page)
void Display_Dot(void)	
{  

  Set_Display_Area(x,x,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  WriteData16(fcolor);
  SSD1963_CS=1;
}


/*-------------------------------------------------------------------------------------------------
����ԭ��: void Display_HLine(uint16 xdata x,uint16 xdata y,uint16 xdata width,uint32 xdata color) 
����˵��: x:���X����,y:���Y����,width:ˮƽֱ�ߵĳ���,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾˮƽֱ��
--------------------------------------------------------------------------------------------------*/
//void Display_HLine(uint16  x,uint16  y,uint16  width,uint16  color,uint8 page)
void Display_HLine(uint16  width)
{

  Set_Display_Area(x,x+width-1,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  while(width--)
  {
	  WriteData16(fcolor);
  }
   SSD1963_CS=1;
}

/*--------------------------------------------------------------------------------------------------
����ԭ��: void Display_VLine(uint16 xdata x,uint16 xdata y,uint16 xdata height,uint32 xdata color)
����˵��: x:���X����,y:���Y����,height:��ֱֱ�ߵĳ���,color:�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ��ֱֱ��
----------------------------------------------------------------------------------------------------*/
//void Display_VLine(uint16  x,uint16  y,uint16  height,uint16  color,uint8 page)
void Display_VLine(uint16  height)
{

  Set_Display_Area(x,x,y+page*LCD_PIX_Y,y+height-1+page*LCD_PIX_Y);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  while(height--)
  {
	WriteData16(fcolor);
  }
  SSD1963_CS=1;
} 


//��ʾһ������
//void DisplayFrame(uint16 start_x,uint16 start_y,uint16 end_x,uint16 end_y, uint16 color,uint8 page)
void DisplayFrame(void)
{
	//Display_HLine(start_x,start_y,end_x-start_x+1,fcolor,page);
	x=start_x;
	y=start_y;
	Display_HLine(end_x-start_x+1);

	x=start_x;
	y=end_y;
	Display_HLine(end_x-start_x+1);


	x=start_x;
	y=start_y;	
	Display_VLine(end_y-start_y+1);

	x=end_x;
	y=start_y;	
	Display_VLine(end_y-start_y+1);	
}




//��ʾһ����ɫ��
//void DisplayBlock(uint16 start_x,uint16 start_y,uint16 end_x,uint16 end_y, uint16 color,uint8 page)
void DisplayBlock(void)
{
	uint16 xdata width,height;
	uint16 i,j;

	if(end_x >= LCD_PIX_X || end_y >=LCD_PIX_Y) return;
	if(start_x>=end_x || start_y>= end_y) return;


	Set_Display_Area(start_x,end_x,start_y+page*LCD_PIX_Y,end_y+page*LCD_PIX_Y);
	Send_Ssd1963_Command(0x2C);


	width=end_x-start_x+1;

	height=end_y-start_y+1;

	SSD1963_CS=0;
	for(j=0;j<height;j++)
	{
		for(i=0;i<width;i++)
		{
			WriteData16(bcolor);
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
//void Display_ASCII_Mul(uint16 x,uint16 y,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p,uint8 page)
void Display_ASCII_Mul(void)
{
	uint8 xdata i,j,k;
	uint8 xdata *p;
	switch(size)
	{
   	
	   case 1: //8*16
	   default:
			lcd_fangda_dis_text(8,16);	   
		   break;
	   case 2: //12*24
		   lcd_fangda_dis_text(16,24);
		   break;
	   case 3: //16*32
		   lcd_fangda_dis_text(16,32);
		   break;
	   case 4://48*96   �����������£������ڷŴ�  4���������⴦��
			//��GT23_REC_DATA[128]�洢�ռ����ޣ�ֻ�ܰ����´���

		   if( (*pDispData>=0x30)&&(*pDispData<=0x39)) 
		   	{//��������
				Set_Display_Area(x,x+47,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+95);
				Send_Ssd1963_Command(0x2C);
		
				
				for(i=0;i<6;i++)
				{//��8�ζ�ȡ�������ݵ�GT23_REC_DATA
					Read_Cont(GT23_Rec_Data,576*(*pDispData-0x30)+D48BaseAdd+i*96,96);
					k=96;
					p=GT23_Rec_Data;
					SSD1963_CS=0;
					for(;k>0;k--)
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
		   	}
		   break;
	}

}



/*-----------------------------------------------------------------------------
����ԭ��: void Display_Chinese(uint x,uint y,uchar *p,ulong fcolor,ulong bcolor)
����˵��: x:���ֵ�X����,y:���ֵ�Y����,fcolor:���ֵ���ɫ��bcolor�����ֵı�����ɫ
�� �� ֵ: ��
��������: ����Ļ����ʾ����
----------------------------------------------------------------------------*/
//void Display_Chinese_Mul(uint16 x,uint16 y,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p,uint8 page)

void Display_Chinese_Mul(void)
{
	switch(size)
	{
		
	case 1: //16*16
	default:
		lcd_fangda_dis_text(16,16);
	   	break; 


	case 3: //32*32
		lcd_fangda_dis_text(32,32);
	   	break;
	   
	}

 }


//дǰ��д������
// uint8 WriteAreaHandlerMul(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint8 type,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y)
uint8 WriteAreaHandlerMul(uint8 type)
{
	uint16 old_x =x;
	uint16 old_y =y;
	
	if(type==0) //�������
	{
		if(x+dot_number/2-1 > end_x)
		{//�����ұ߽�,��ͷ��ʼ
			x=start_x;
			if(y+2*dot_number+current_vert_spac-1>end_y)    goto fail;	//������д����
			else
			{
				y += (dot_number+current_vert_spac);	//������һ��
				if(x+dot_number/2-1 > end_x)  goto fail;		//�����п�ʼ������1���ַ����Ų���
				else return 1;
			}
		}
		else 
		{
			return 1;
		}

	}
	else if(type==1)	//ȫ������
	{
	
		if(x+dot_number-1 >end_x)
		{//�����ұ߽�,��ͷ��ʼ
			x=start_x;
			if(y+2*dot_number+current_vert_spac-1>end_y)    goto fail;	//������д����
			else
			{
				y += (dot_number+current_vert_spac);	//������һ��
				if(x+dot_number-1 > end_x)  goto fail;		//�����п�ʼ������1�����ֶ��Ų���
				else return 1;
			}
		}
		else return 1;
	}

	fail:
		x=old_x;
		y=old_y;
		return 0;

}	


//x,y:��ʼ�������
//type_x_wide:ԭ����X��Ŀ��, type_y_height:ԭ����Y��ĸ߶�
//mul_row:����Ŵ���,   mul_col:����Ŵ�һ��
//cbc_filter:���˲��� 1:���˱���ɫ
//p:Ҫ��ʾ��ͼƬ����ָ��

//void lcd_fangda_dis_text(uint16 x, uint16 y, uint8 type_x_wide, uint8 type_y_height, 
//						 uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p,uint8 page)
void lcd_fangda_dis_text(uint8 type_x_wide, uint8 type_y_height)
{

	unsigned char  i,j,k,m,n;


	Set_Display_Area(x,x+type_x_wide*mul_col-1,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+type_y_height*mul_row-1);
	Send_Ssd1963_Command(0x2C);


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
							if ((GT23_Rec_Data[i*(type_x_wide/8)+j]>>(7-k))&0x1)
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





//px,py  д���ַ�������ʼx,y����
//size:  �����С
//mul_row,mul_col:���зŴ���
//fcolor,bcolor:ǰ��ɫ������ɫ
//pDigit:Ҫд����ַ���
//page: д���ҳ��
//s_x,s_y:д���������ʼ������
//end_x,end_y:д������Ľ���������


//void DisplayString(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *pDispData,uint8 page,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y)
void DisplayString(void)
{
	
	while(*pDispData)
	{	
		if((*pDispData>=0x81)&&(*pDispData<=0xfe))	//���ֽڷ��Ϻ�����������
		{
			pDispData++;
			if(*pDispData>=0x40)
			{
			
				pDispData--;
				Get_GT23_Data(pDispData,size);	

				//�Ƚ���д�ռ䴦��
				if(WriteAreaHandlerMul(1)==0)   break;	//���������˳�

				Display_Chinese_Mul();

				x+=dot_number;
				pDispData=pDispData+2;	//һ����������ռ�����ֽ�

			}
			else
			{//���ֽڲ����Ϻ�������
				if(*pDispData==0x00) break;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e))	//����ASCII�ַ�����
		{
			Get_GT23_Data(pDispData,size);
						
			//�Ƚ���д�ռ䴦��
			if(WriteAreaHandlerMul(0)==0)   break;	//���������˳�
			
			Display_ASCII_Mul();

			 x+= dot_number/2;
			pDispData++;
		}
		else //�Ƿ��ַ�
		{
			pDispData++;
		}

	}
	WDT_CONTR = 0x3e;	//ι��
}
		

//px,py  д���ַ�������ʼx,y����
//size:  �����С
//mul_row,mul_col:���зŴ���
//fcolor,bcolor:ǰ��ɫ������ɫ
//pDigit:Ҫд����ַ���
//page: д���ҳ��
//s_x,s_y:д���������ʼ������
//end_x,end_y:д������Ľ���������
//lenth:���ֳ���

//void DisplayDigitString(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *pDigit,uint8 page,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y,uint8 lenth)
void DisplayDigitString(void)
{
	uint8 Digit;
	for(;Length>0;Length--)
	{	
		Digit = *pDispData;
		if(*pDispData<10)
		{
			Digit += 0x30;
			Get_GT23_Data(&Digit,size);
		}
		else 
		{
			Digit += 55;
			Get_GT23_Data(&Digit,size);	//ʮ�������е�ABC....
		}
		if(WriteAreaHandlerMul(0)==0) return;
		Display_ASCII_Mul();

		x+= dot_number/2;
		pDispData++;
	}	
} 


//���ݳ���д���ַ�
//px,py  д���ַ�������ʼx,y����
//size:  �����С
//mul_row,mul_col:���зŴ���
//fcolor,bcolor:ǰ��ɫ������ɫ
//pDigit:Ҫд����ַ���
//page: д���ҳ��
//s_x,s_y:д���������ʼ������
//end_x,end_y:д������Ľ���������


//void DisplayStringLen(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *pDispData,uint8 page,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y,uint8 Length)
void DisplayStringLen(void)
{
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
					Get_GT23_Data(pDispData,size);	

					//�Ƚ���д�ռ䴦��
					if(WriteAreaHandlerMul(1)==0)   break;	//���������˳�

					Display_Chinese_Mul();

					x+= dot_number;
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
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e)) //����ASCII�ַ�����
		{
			Get_GT23_Data(pDispData,size);
			//�Ƚ���д�ռ䴦��
			if(WriteAreaHandlerMul(0)==0)   break;	//���������˳�
			
			Display_ASCII_Mul();

			x +=dot_number/2;
			pDispData++;
			Length--;
		}
		else	//�������������
		{
			pDispData++;
			Length--;		
		}		

	}
	
}
				

//void DisplayOneChar(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 byChar,uint8 page,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y)		
void DisplayOneChar(uint8 byChar)		
{
	BedDataBuff[0]=byChar/100;
	BedDataBuff[1]=(byChar%100)/10;
	BedDataBuff[2]=(byChar%100)%10;
	BedDataBuff[3]=0;

	if(BedDataBuff[0])
	{
		pDispData=BedDataBuff;
		Length =3;
		DisplayDigitString();
	
	}
	else 
	{
		if(BedDataBuff[1])
		{
			pDispData=&(BedDataBuff[1]);
			Length =2;
			DisplayDigitString();
		}
		else 
		{
			pDispData=&(BedDataBuff[2]);
			Length =1;				
			DisplayDigitString();
		}
	}
			
}




//void ShowAddBedCallFace(uint8 byBedAddr,uint8 page)

void ShowAddBedCallFace(uint8 byBedAddr){
	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	bcolor =BLUE;
	DisplayBlock();	

	x=(480-32*7)/2;			//128			
	y=(272-32)/2;			//120
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	pDispData= "��";
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;
	DisplayString();

	DisplayOneChar(byBedAddr);

	pDispData= "������";	
	DisplayString();

	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=BED_INFO_FACE;
}



void DisSendPinBugFace(void)
//void DisSendPinBugFace(uint8 page)
{

	start_x = 357;
	start_y = 242;
	end_x= 479;
	end_y= 271;
	bcolor =BLACK;
	DisplayBlock();	



	x=357;
	y=246;
	current_size=1;
	get_dot_number();
	get_basesize_mul();

	fcolor=BLUE;
	bcolor=BLACK;
	pDispData= "��������!";
	start_x=357;
	start_y=246;
	end_x=480;
	end_y=272;

	DisplayString();
		
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=INFO_INDICATION_FACE;

//	BL_SW=1;	//���ش�
	BL_ON();
	
}


//void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCallType,uint8 page)
void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCallType)
{
	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 271;
	bcolor =BLUE;
	DisplayBlock();	
	
	byRoomAddr &= 0x7f;

	current_size=2;
	get_dot_number();
	get_basesize_mul();	
//	CCAPM0=0X48;	//��ֹ����
//	MCUMasterSPI(); //��Ƭ��Ϊ��ģʽ
	switch(byCallType)
	{
		case CMD_INFUSION_CALL:								//�������
			x=(480-32*7)/2;			//128
			y=(272-32)/2;			//120

			fcolor=WHITE;
			bcolor=BLUE;
			start_x=0;
			start_y=0;
			end_x=480;
			end_y=272;
			DisplayOneChar(byRoomAddr);

			pDispData= "��";
			DisplayString();

			DisplayOneChar(byBedAddr);

			pDispData= "������";
			DisplayString();			
			break;

		case CMD_SERVICE_CALL:								//��Һ��Ϻ���
			x=(480-32*8)/2;
			y=(272-32)/2;

			fcolor=WHITE;
			bcolor=BLUE;
			start_x=0;
			start_y=0;
			end_x=480;
			end_y=272;

			DisplayOneChar(byRoomAddr);
			pDispData= "��";
			DisplayString();

			DisplayOneChar(byBedAddr);
			pDispData= "����Һ���";
			DisplayString();
			break;			
		case CMD_EMERGENCY_CALL:   							//��������
			
			x=(480-32*7)/2;
			y=(272-32)/2;
			fcolor=WHITE;
			bcolor=BLUE;
			start_x=0;
			start_y=0;
			end_x=480;
			end_y=272;

			DisplayOneChar(byRoomAddr);
			pDispData= "�����������";
			DisplayString();						
			break;
	}	

//	EPH1660MasterSPI(); //
//	CCAPM0=0X49;	//��������

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
//void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr,uint8 page)
void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr)
{ 
	byRoomAddr=byRoomAddr;
	byBedAddr=byBedAddr;
	start_x = 357;
	start_y = 242;
	end_x= 479;
	end_y= 271;
	bcolor =BLUE;	

	DisplayBlock();


	x=(480-32*7)/2;			//128
	y=(272-32)/2;			//120
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;
	pDispData= "��������ͨ����";
	DisplayString();	


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=TALK_FACE;
}




/*********************************************************
��������			:ShowBroadFace	
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
//void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf,uint8 page)
void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf)
{ 

	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	bcolor =BLUE;	
	DisplayBlock();

//	CCAPM0=0X48;	//��ֹ����
//	MCUMasterSPI(); //��Ƭ��Ϊ��ģʽ	

	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;

	if(bSelf)
	{

		x=(480-32*7)/2;
		y=(272-32)/2;
		pDispData= "�������ڹ㲥��";
		DisplayString();
	}
	else
	{
		if(byRoomAddr==YHFJ_ROOM_ADDR)
		{

			x=(480-32*10)/2;
			y=(272-32)/2;

			DisplayOneChar(byBedAddr);
			
			pDispData= "��ҽ���ֻ����ڹ㲥";
			DisplayString();
					
		}
		else
		{			
			x=(480-32*8)/2;
			y=(272-32)/2;

			DisplayOneChar(byRoomAddr);
			
			pDispData= "��";
			DisplayString();

			DisplayOneChar(byBedAddr);
			
			pDispData= "�����ڹ㲥";
			DisplayString();			

		
		}
	}

//	EPH1660MasterSPI();	//
//	CCAPM0=0X49;	//��������	

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=BROAD_FACE;	
 }




//void ShowStartInfusionCall(uint8 page)
void ShowStartInfusionCall(void)
{
	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	fcolor =BLUE;	
	DisplayBlock();

	x=144;
	y=120;
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;
	pDispData= "��ʼ�������";
	DisplayString();	


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=INFUSION_CALL_FACE;
}


//void ShowStartServiceCall(uint8 page)
void ShowStartServiceCall(void)
{
	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	bcolor =BLUE;
	DisplayBlock();

	
	x=144;
	y=120;
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;
	pDispData= "��ʼ�������";	
	DisplayString();

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=SERVICE_CALL_FACE;

}


//void ShowStartEmergencyCall(uint8 page)
void ShowStartEmergencyCall(void)
{

	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	bcolor =BLUE;
	DisplayBlock();
	
	x=144;
	y=120;
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;	
	pDispData= "��ʼ��������";	
	DisplayString();
	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=EMERGENCY_CALL_FACE;

}

//void ShowStartHelpCall(uint8 page)
void ShowStartHelpCall(void)
{
	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	bcolor =BLUE;
	DisplayBlock();

	
	x=144;
	y=120;
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;	
	pDispData= "��ʼ��Ԯ����";
	DisplayString();

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=HELP_CALL_FACE;

}


void get_basesize_mul()
{
	switch(current_size)
	{
		default:
		case 1:  //16x16
			size =1;
			mul_col=1;
			mul_row=1;
			break;
		case 2: //32x32
			size =3;
			mul_col=1;
			mul_row=1;
			break;
		case 3: //48x48
			size=1;
			mul_col=3;
			mul_row=3;
			break;
		case 4:	//64x64
			size=3;
			mul_col=2;
			mul_row=2;
			break;
		case 5: //80x80
			size=1;
			mul_col=5;
			mul_row=5;
			break;
		case 6:	//96x96
			size=3;
			mul_col=3;
			mul_row=3;
			break;
		case 7:	//128x128
			size=3;
			mul_col=4;
			mul_row=4;
			break;

		case 8:	//160x160
			size=3;
			mul_col=5;
			mul_row=5;
			break;			
			
	}
}


void get_dot_number()
{
	switch(current_size)
	{
		case 1: 
				dot_number=16;
			    current_vert_spac = VERT_SPAC_16;
				break;
		case 2:
				dot_number=32;
				current_vert_spac = VERT_SPAC_32;
				break;
		case 3:
				dot_number=48;
				current_vert_spac = VERT_SPAC_48;
				break;
		case 4: 
				dot_number=64;
				current_vert_spac = VERT_SPAC_64;
				break;
		case 5:
				dot_number=80;
				current_vert_spac = VERT_SPAC_80;
				break;
		case 6:
				dot_number=96;
				current_vert_spac = VERT_SPAC_96;
				break;
				
		case 7:
				dot_number=128;
				current_vert_spac = VERT_SPAC_128;
				break;
		case 8:
				dot_number=160;
				current_vert_spac = VERT_SPAC_160;
				break;
				
		default:
				dot_number=16;
			    current_vert_spac = VERT_SPAC_16;			
				break;
			
	}

}

void ShowInfoBlockLen(void)
{
	uint8 RowCharNum;
	uint8 ColumnCharNum;
	

	uint16 area_width=0;
	uint16 area_height=0;

	uint8 title_length=0;	//���ⳤ��
	uint8 LengthBack=Length;	//����Ҫ��ʾ��ԭʼ���ݳ���
	uint8 *pdispdata2 = pDispData;	//����ԭʼҪ��ʾ������


	area_width = end_x-start_x+1;
	area_height = end_y-start_y+1;

	if(area_height-VERT_SPAC_160 >=160)  		{current_size = DOT160X160;current_vert_spac = VERT_SPAC_160;}
	else if(area_height-VERT_SPAC_128 >=128)  	{current_size = DOT128X128;current_vert_spac = VERT_SPAC_128;}	
	else if(area_height-VERT_SPAC_96 >=96)  	{current_size = DOT96X96;current_vert_spac = VERT_SPAC_96;}
	else if(area_height-VERT_SPAC_80 >=80)		{current_size = DOT80X80;current_vert_spac = VERT_SPAC_80;}
	else if(area_height-VERT_SPAC_64 >=64)		{current_size = DOT64X64;current_vert_spac = VERT_SPAC_64;}
	else if(area_height-VERT_SPAC_48 >=48)		{current_size = DOT48X48;current_vert_spac = VERT_SPAC_48;}
	else if(area_height-VERT_SPAC_32 >=32)		{current_size = DOT32X32;current_vert_spac = VERT_SPAC_32;}
	else if(area_height-VERT_SPAC_16 >=16)		{current_size = DOT16X16;current_vert_spac = VERT_SPAC_16;}
	else
	{//�߶�̫С������С�ߴ���ַ�����ʾ���£�ֱ������
		fTitle_TextAtLine =0;
		return;
	}

	if(fTitle_TextAtLine)
	{
		title_length = GetTitleLength(stLocalControl.stItemForm.ItemCode);	//��ȡ���ⳤ�Ƚ��뵽�ܳ�����
		if(stLocalControl.stItemForm.DispMode &0x08)  title_length ++;    //bit3:�����Ǵ�������
		Length += title_length;
								
	}

	do
	{
		get_dot_number();

		RowCharNum= area_height/(dot_number+current_vert_spac);

		ColumnCharNum= area_width/(dot_number/2);
		
		if(Length<=RowCharNum*ColumnCharNum)
		{//��������Ҫ��
			get_basesize_mul();

			x=start_x;  // ���򶥸���ʾ
			if(area_width >= Length * (dot_number/2)) y=start_y + (area_height - dot_number)/2; 	// 1�о�����ʾ�
			else y= start_y +(area_height-(dot_number+current_vert_spac)*(RowCharNum-1)-dot_number)/2;   //���������ʾ
			
			if(fTitle_TextAtLine)
			{//����������ͬ��

				//if(area_width >= Length * (dot_number/2)) y=start_y + (area_height - dot_number)/2; 	// 1�о�����ʾ��
				pDispData = GetTitle(stLocalControl.stItemForm.ItemCode);
				if(pDispData!=NULL)
				{
					DisplayString();
					if(stLocalControl.stItemForm.DispMode &0x08)
					{
						
						pDispData=":";
						DisplayString();

					}
					
				}
				pDispData = pdispdata2;
				Length 	  = LengthBack;
			}
			get_basesize_mul();
			DisplayStringLen();
			break;
			
		}
		else 
		{	if(RowCharNum ==1)
			{//ֻ����ʾһ��
				if(fTitle_TextAtLine)
				{//����������ͬ��
					if(area_width>= title_length*8+LengthBack*(dot_number/2))	//������16����
					{
						x=start_x;
						y=start_y+(area_height-16)/2;
						pDispData = GetTitle(stLocalControl.stItemForm.ItemCode);
						if(pDispData!=NULL)
						{
							dot_number=16;
							size=1;
							mul_row =1;
							mul_col=1;
							DisplayString();
							if(stLocalControl.stItemForm.DispMode &0x08)
							{
								
								pDispData=":";
								DisplayString();

							}
							
						}
						get_dot_number();
						y=start_y+(area_height-dot_number)/2;
						pDispData = pdispdata2;
						Length 	  = LengthBack;	

						get_basesize_mul();
						DisplayStringLen();
						break;							
					}
					//else current_size--;
				}
				//else current_size--;
			}

			//else current_size--;
			current_size--;
		}
	}while(current_size);
	
	WDT_CONTR = 0x3e;	//ι��
	fTitle_TextAtLine =0;

}




//void ShowCallSuccess(uint8 page)
void ShowCallSuccess(void)
{
	start_x = 0;
	start_y = 120;
	end_x= 479;
	end_y= 152;
	bcolor =BLUE;
	DisplayBlock();

	
	x=120;
	y=120;
	current_size=2;
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;	
	pDispData= "���гɹ�,��ȴ�";
	DisplayString();

	Set_VisualPage(page);	
	stLocalControl.byDisplayFace=CALL_SUCCESS_FACE;
}


void ShowItem(uint8 ItemCode)
{

	uint8 xdata position=0;
	uint8 num=0;
	uint8 i=0;
//	uint8 j,k;
	
	page=1;
	mul_row=1;
	mul_col=1;

	//��ȡ�����ʽ�ܳ���
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_ADDR,2);	


	//�Ӹ�ʽ����ȥ�ҿ�ʼ�������
	num= stLocalControl.uiBedFaceFormLen/sizeof(STItemForm);
	if(num>=MAX_ITEM_CODE)
	{//����
		stLocalControl.uiBedFaceFormLen=0;
		Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);
		Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);
		return;
	}

	i=0;
	for(;num>0;num--)
	{
		if(Read(BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm))== ItemCode) break; //�ҵ���Ŀ����
		else i++;
		
	}
	if(num!=0)
	{//�ҵ���Ӧ�ĸ�ʽ��
		Read_Cont(&(stLocalControl.stItemForm.ItemCode),BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm),sizeof(STItemForm));//��ȡ�����ʽ����
		start_x=stLocalControl.stItemForm.StartPoint.x;
		start_y=stLocalControl.stItemForm.StartPoint.y;

		end_x= stLocalControl.stItemForm.EndPoint.x;
		end_y= stLocalControl.stItemForm.EndPoint.y;


		if(end_x >= LCD_PIX_X || end_y >=LCD_PIX_Y) return;
		if(start_x>=end_x || start_y>= end_y) return;

		if((ItemCode == ILLNESS_LEVEL) ||(ItemCode == MEDICAL_LEVEL)||(ItemCode == NURSING_LEVEL)||(ItemCode == DIETARY_LEVEL))
		{//����,����������ɫ����������ȡ
			fcolor= GetColor(*(stLocalControl.stItemData.pData));	//����
			bcolor= GetColor(*(stLocalControl.stItemData.pData+1)); 					
		}
		else
		{
			fcolor= GetColor(stLocalControl.stItemForm.fcolor);
			bcolor= GetColor(stLocalControl.stItemForm.bcolor);
		}
		if(ItemCode!=BED_NUMBER)
		{//����û�б��⣬���ܱ����ʽӰ��
			if(stLocalControl.stItemForm.DispMode & 0x20)	//bit5:�Ƿ���ʾ����
			{//��ʾ����
				if(stLocalControl.stItemForm.DispMode &0x10)//bit4:������ʾ����
				{//������ʾ
					fTitle_TextAtLine =0;
					if(stLocalControl.stItemForm.DispMode &0x08)//bit3:�����Ƿ��������
					{//�����������
						start_x +=24;
						
					}
					else
					{//���ⲻ��":"
						start_x +=16;
					}
					
				}
				else
				{//������ʾ
					//if((ItemCode == OX_START) ||(ItemCode == OX_END)||(ItemCode == OX_SUBTOTAL)||(ItemCode ==OX_TOTAL))
					if(stLocalControl.stItemForm.DispMode &0x01)//bit0:�ж�����������Ƿ�����һ��
					{//����һ��
						start_y +=16;
						fTitle_TextAtLine =0;
					}

					else 
					{//��һ����ʾ
						fTitle_TextAtLine =1;
									
					}

				}
			
			}
		}
		DisplayBlock();//��ԭ�е���Ϣ���	
	switch(ItemCode)
		{
			case OX_START:
				BedDataBuff[0]=(stLocalControl.stSupplyOxStartTime.byHour/10)+0x30;	//ת����ASCII����
				BedDataBuff[1]=(stLocalControl.stSupplyOxStartTime.byHour%10)+0x30;
				BedDataBuff[2]= 58;		//":"��
				BedDataBuff[3]=(stLocalControl.stSupplyOxStartTime.byMin/10)+0x30;
				BedDataBuff[4]=(stLocalControl.stSupplyOxStartTime.byMin%10)+0x30;
				BedDataBuff[5]=0x00;

				pDispData = BedDataBuff;
				Length = 5;				
				ShowInfoBlockLen();				
				break;
			case OX_END:
				BedDataBuff[0]=(stLocalControl.stSupplyOxEndTime.byHour/10)+0x30;	//ת����ASCII����
				BedDataBuff[1]=(stLocalControl.stSupplyOxEndTime.byHour%10)+0x30;
				BedDataBuff[2]= 58;		//":"��
				BedDataBuff[3]=(stLocalControl.stSupplyOxEndTime.byMin/10)+0x30;
				BedDataBuff[4]=(stLocalControl.stSupplyOxEndTime.byMin%10)+0x30;
				BedDataBuff[5]=0x00;

				pDispData = BedDataBuff;
				Length = 5;				
				ShowInfoBlockLen();	
				break;
			case OX_SUBTOTAL:
				x=stLocalControl.stSupplyOxSubtotalTime.uiHour%10000;	//��ǧλ����
				//BedDataBuff[0]=(x/1000)+0x30;   //��ǧλ��  ת����ASCII����
				y=x%1000;						//�ð�λ����
				//BedDataBuff[1]=(y/100)+0x30;		//�ð�λ��
				BedDataBuff[0]=((y%100)/10)+0x30;
				BedDataBuff[1]=((y%100)%10)+0x30;		
				BedDataBuff[2]= 58;		//":"��
				BedDataBuff[3]=(stLocalControl.stSupplyOxSubtotalTime.byMin/10)+0x30;
				BedDataBuff[4]=(stLocalControl.stSupplyOxSubtotalTime.byMin%10)+0x30;		
				BedDataBuff[5]=0x00;
				

				pDispData = BedDataBuff;
				Length = 5;				
				ShowInfoBlockLen();	
				break;
				
			case OX_TOTAL:
				DisplayBlock();//��ԭ�е���Ϣ���	
				x=stLocalControl.stSupplyOxTotalTime.uiHour%10000;	//ǧλ����
				BedDataBuff[0]=(x/1000)+0x30;       //ת����ASCII����
				y=x%1000;
				BedDataBuff[1]=(y/100)+0x30;
				BedDataBuff[2]=((y%100)/10)+0x30;
				BedDataBuff[3]=((y%100)%10)+0x30;		
				BedDataBuff[4]= 58;		//":"��
				BedDataBuff[5]=(stLocalControl.stSupplyOxTotalTime.byMin/10)+0x30;
				BedDataBuff[6]=(stLocalControl.stSupplyOxTotalTime.byMin%10)+0x30;		
				BedDataBuff[7]=0x00;
				

				pDispData = BedDataBuff;
				Length = 7;				
				ShowInfoBlockLen();	
				break;
				
			case BED_NUMBER:
    			size=4;	
				current_vert_spac=0;
				mul_row = 1;
				mul_col = 1;	
				x=start_x;y=start_y;
				BedDataBuff[0]=stLocalControl.stEepromCfgData.bySelfBedAddr/100+0x30;
				BedDataBuff[1]=(stLocalControl.stEepromCfgData.bySelfBedAddr%100)/10+0x30;
				BedDataBuff[2]=(stLocalControl.stEepromCfgData.bySelfBedAddr%100)%10+0x30;
				BedDataBuff[3]=0;		

				if(BedDataBuff[0] ==0x30)
				{
					pDispData =  &(BedDataBuff[1]);
					Length=2;	//ֻ��ʾ��������
				}
				else
				{
					pDispData =  BedDataBuff;
					Length=3;	//��ʾ3������				
				}
				if((end_y - start_y+1 >=96)&&(end_y - start_y+1 <152))
				{//y�ռ�������������Ҫ��
					if(end_x - start_x +1 >= (48*Length))
					{//x�ռ�Ҳ����д�´��ŵ�Ҫ��
						x=start_x+ (end_x-start_x-(48*Length))/2;
						y= start_y +(end_y-start_y-96)/2;
						size =4;
						mul_col=1;
						mul_row=1;
						dot_number =96;	
						DisplayString();
						return;
					}
				}

				ShowInfoBlockLen();				
				break;	

				
			case OX_SUPPLY:
				break;
			case ILLNESS_LEVEL:
			case MEDICAL_LEVEL:
			case NURSING_LEVEL:
			case DIETARY_LEVEL:
				pDispData = stLocalControl.stItemData.pData+2;
				Length = stLocalControl.stItemData.DataLength-2;
				ShowInfoBlockLen();		
				break;				
			default:				
				pDispData = stLocalControl.stItemData.pData;
				Length = stLocalControl.stItemData.DataLength;
				ShowInfoBlockLen();		
				break;
				
		}
	}

}


void ShowPatientBaseInfo(void)
{	
	uint8 xdata position=0;
	uint8 num=0;
	uint8 i=0;
	uint16 uiDataLen;
	
	page=1;
	mul_row=1;
	mul_col=1;

	//��ȡ�����ʽ�ܳ���
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_ADDR,2);	
	//��ȡ���������ܳ���
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_LENGTH_ADDR,2);


	uiDataLen = stLocalControl.uiBedFaceDataLen;
	//�Ӹ�ʽ����ȥ�ҿ�ʼ�������
	num= stLocalControl.uiBedFaceFormLen/sizeof(STItemForm);
	if(num>=MAX_ITEM_CODE)
	{//����
		stLocalControl.uiBedFaceFormLen=0;
		Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);
		Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);
		return;
	}

	for(;uiDataLen>0;)
	{

		Read_Cont(&(stLocalControl.stItemData.ItemCode),BED_FACE_DATA_START_ADDR+position,2);
		position +=2;
		uiDataLen -=2;

		if(stLocalControl.stItemData.DataLength)
		{//������
			if((stLocalControl.stItemData.DataLength<=SEGMENT_DATA_LEN)&&(stLocalControl.stItemData.ItemCode < MAX_ITEM_CODE))
			{//���ݶ����ݲ��������߹��������
				i=0;
				num= stLocalControl.uiBedFaceFormLen/sizeof(STItemForm);
				for(;num>0;num--)
				{
					if(Read(BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm))== stLocalControl.stItemData.ItemCode) break; //�ҵ���Ŀ����
					else i++;
					
				}
				if(num!=0)
				{//�ҵ���Ӧ�ĸ�ʽ��
					Read_Cont(&(stLocalControl.stItemForm.ItemCode),BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm),sizeof(STItemForm));//��ȡ�����ʽ����
					if((stLocalControl.stItemForm.EndPoint.x > stLocalControl.stItemForm.StartPoint.x)&&
					   (stLocalControl.stItemForm.EndPoint.y > stLocalControl.stItemForm.StartPoint.y))

					{//��ʼ��X��Y�������Ҫ��
						start_x=stLocalControl.stItemForm.StartPoint.x;
						start_y=stLocalControl.stItemForm.StartPoint.y;

						end_x= stLocalControl.stItemForm.EndPoint.x;
						end_y= stLocalControl.stItemForm.EndPoint.y;


						stLocalControl.stItemData.pData = BedDataBuff;
						Read_Cont(stLocalControl.stItemData.pData ,BED_FACE_DATA_START_ADDR+position,stLocalControl.stItemData.DataLength);


						if((stLocalControl.stItemData.ItemCode == ILLNESS_LEVEL) ||(stLocalControl.stItemData.ItemCode == MEDICAL_LEVEL)||(stLocalControl.stItemData.ItemCode == NURSING_LEVEL)||(stLocalControl.stItemData.ItemCode == DIETARY_LEVEL))
						{//����,����������ɫ����������ȡ
							fcolor= GetColor(*(stLocalControl.stItemData.pData));	//����
							bcolor= GetColor(*(stLocalControl.stItemData.pData+1));						
						}
						else
						{
							fcolor= GetColor(stLocalControl.stItemForm.fcolor);
							bcolor= GetColor(stLocalControl.stItemForm.bcolor);
						}

						if(stLocalControl.stItemForm.DispMode & 0x20)	//bit5:�Ƿ���ʾ����
						{//��ʾ����
							if(stLocalControl.stItemForm.DispMode &0x10)//bit4:������ʾ����
							{//������ʾ
								fTitle_TextAtLine=0;
								if(stLocalControl.stItemForm.DispMode &0x08)//bit3:�����Ƿ��������
								{//�����������
									start_x +=24;
									
								}
								else
								{//���ⲻ��":"
									start_x +=16;
								}
								
							}
							else
							{//������ʾ
								if(stLocalControl.stItemForm.DispMode &0x01)//bit0:�ж�����������Ƿ�����һ��
								{//����һ��
									start_y +=16;
									fTitle_TextAtLine=0;
								}
								else
								{//��ͬһ��
									fTitle_TextAtLine =1;
															
								}
							}
							
						}
						
						DisplayBlock();//��ԭ�е���Ϣ���	
						if((stLocalControl.stItemData.ItemCode == ILLNESS_LEVEL) ||(stLocalControl.stItemData.ItemCode == MEDICAL_LEVEL)||(stLocalControl.stItemData.ItemCode == NURSING_LEVEL)||(stLocalControl.stItemData.ItemCode == DIETARY_LEVEL))
						{//����,���������ݴӵ������ֽڿ�ʼ
							pDispData = stLocalControl.stItemData.pData+2;
							Length = stLocalControl.stItemData.DataLength-2;						
						}
						else
						{
							pDispData = stLocalControl.stItemData.pData;
							Length = stLocalControl.stItemData.DataLength;
						}
						ShowInfoBlockLen();
					}
				}
			}
			position +=stLocalControl.stItemData.DataLength;
			uiDataLen -= stLocalControl.stItemData.DataLength;			
		}  //�����ݵ�ĩβ

	}

}



void ShowBedFace(void)
{	
	uint8 i;
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_ADDR,2);	//����ʽ�����ܸ���

	if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2)) //�����ݳ���Ҳ������ȥ
	{
		ShowBedFaceForm();	
		if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2)!=1)
		{//�������������ȷ
			for(i=0;i<5;i++)
			{
				SectorDataCopy(BED_FACE_FORM_BAK,BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+3);	//��������һ������				
				if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2)==1)  break;	
			}		
		}
	}
	else
	{//����ʽ����ȷ
		Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_BAK,2);	//����������ʽ�����ܸ���	
	
		if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2))
		{//��������ȷ
			while(1)
			{
				SectorDataCopy(BED_FACE_FORM_LENGTH_ADDR,BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+3);	//����������ʽ��				
				if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2)==1)  break;					
			}
			ShowBedFaceForm();
		}
		else return;		//����ʽ������ȷ�����ݸ�ʽ��Ҳ����ȷ��
	}


//	ShowBedFaceForm();
	WDT_CONTR = 0x3e; 
	ShowItem(BED_NUMBER);
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_LENGTH_ADDR,2);	//����ͷ�������ܸ���

	if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)) //�����ݳ���Ҳ������ȥ
	{
		
		
		ShowPatientBaseInfo();			
		if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)!=1)
		{//�������������ȷ
			for(i=0;i<5;i++)
			{
				SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);	//��������һ������				
				if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)==1)  break;	
			}		
		}
	}
	else
	{//����ʽ����ȷ
		Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_BAK,2);	//����������ͷ�������ܸ���
		
		if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2))
		{//��������ȷ
			while(1)
			{
				SectorDataCopy(BED_FACE_DATA_LENGTH_ADDR,BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+3);	//��������������				
				if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)  break;					
			}
			ShowPatientBaseInfo();
		}
		else return;		//������������ȷ������������Ҳ����ȷ��
	}
//    ShowPatientBaseInfo();
	ShowItem(OX_START);
	ShowItem(OX_END);
	ShowItem(OX_SUBTOTAL);	
	ShowItem(OX_TOTAL);

}



void ShowPowerUpFace(void)	//д�ڵ�0ҳ
{
	fcolor=BLUE;
	Lcd_Clear();

	current_size = 2;	//32*32
	get_dot_number();
	get_basesize_mul();

	x=(LCD_PIX_X-dot_number*6)/2;		
	y=(LCD_PIX_Y-(dot_number+current_vert_spac)*4)/2;		
	
	fcolor=WHITE;
	bcolor=BLUE;
	pDispData="ϵͳ��ʼ��...";
	start_x =0;
	start_y =0;
	end_x = 480;
	end_y =272;		
	DisplayString();
	

	y += (dot_number+current_vert_spac);			//106
	x=(LCD_PIX_X-dot_number*4)/2;		//176
	pDispData="�汾V";
	DisplayString();

	stLocalControl.stEepromCfgData.byVersionHi=3;
	DisplayOneChar(stLocalControl.stEepromCfgData.byVersionHi);
	pDispData=".";
    DisplayString();
	stLocalControl.stEepromCfgData.byVersionLo=7;
	DisplayOneChar(stLocalControl.stEepromCfgData.byVersionLo);
	
	

	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*10)/2;
	pDispData="��������ϵͳ��ͷ�ֻ�";
	DisplayString();	


/*	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*4)/2;
	pDispData="��ͷ�ֻ�";
	DisplayString();	*/

	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=POWER_UP_FACE;
}


void ShowStartNumSet(void)
{

	start_x=0;
	start_y=120;
	end_x = 479;
	end_y =152;	
	bcolor =BLUE;
	DisplayBlock();

	x=(480-32*10)/2;		//60
	y=120;
	current_size = 2;	//32*32
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	
	start_x =0;
	start_y =0;
	end_x = 480;
	end_y =272;	
    DisplayOneChar(stLocalControl.stBusDealFreq.bySndSecAddr);
	pDispData="��";
	DisplayString();


    DisplayOneChar(stLocalControl.stBusDealFreq.bySndRoomAddr);
	pDispData ="��";
	DisplayString();

	
    DisplayOneChar(stLocalControl.stBusDealFreq.bySndBedAddr);
	pDispData ="����ʼ���";
	DisplayString();


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=START_NUM_SET_FACE;	
}


void ShowNumSetOk(void)
{
	start_x=0;
	start_y=120;
	end_x = 479;
	end_y =152;	
	bcolor =BLUE;
	DisplayBlock();

	
	x=(480-32*4)/2;
	y=(272-32)/2;
	current_size = 2;	//32*32
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	
	start_x =0;
	start_y =0;
	end_x = 480;
	end_y =272;	
	pDispData="��ųɹ�";
	DisplayString();
	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=NUM_SET_OK_FACE;

	page=1;
	ShowItem(BED_NUMBER);		//д�ڵ�1ҳ
}


void ShowStartRfNumSet(void)			
{
	start_x=0;
	start_y=120;
	end_x = 479;
	end_y =152;	
	bcolor =BLUE;
	DisplayBlock();

	
	x=(480-32*9)/2;
	y=(272-32)/2;
	current_size = 2;	//32*32
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	
	start_x =0;
	start_y =0;
	end_x = 480;
	end_y =272;	
	pDispData="��Һ��������ʼ����";	
	DisplayString();


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=START_RF_NUM_SET_FACE;		
}

void ShowRfNumSetOk(void)			
{
	start_x=0;
	start_y=120;
	end_x = 479;
	end_y =152;	
	bcolor =BLUE;
	DisplayBlock();

	
	x=(480-32*9)/2;
	y=(272-32)/2;
	
	current_size = 2;	//32*32
	get_dot_number();
	get_basesize_mul();

	fcolor=WHITE;
	bcolor=BLUE;
	
	start_x =0;
	start_y =0;
	end_x = 480;
	end_y =272;	
	pDispData="��Һ����������ɹ�";		
 	DisplayString();

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=RF_NUM_SET_OK_FACE;		
}




uint8* GetTitle(uint8 ItemCode)
{
	switch(ItemCode)
	{
		case BED_NUMBER:
			return("����");
			break;

		case ILLNESS_LEVEL:
			return("����");
			break;

		case MEDICAL_LEVEL:
			return("ҽ��");
			break;

		case NURSING_LEVEL:
			return("����");
			break;

		case DIETARY_LEVEL:
			return("��ʳ");
			break;

		case HOSPITALIZED_NO:
			return("��Ժ��");
			break;

		case NAME:
			return("����");
			break;

		case SEX:
			return("�Ա�");
			break;

		case AGE:
			return("����");
			break;
		
		case ADMISSION_DATE:
			return("��Ժ");
			break;

		case DIAGNOSIS:
			return("���");
			break;

		case BLOOD:
			return("Ѫ��");
			break;

        case CAUTION:
			return("��ȫ��ʾ");
			break;

		case ALERGY_DRUGS:
			return("����ʷ");
			break;

			
		case DOCTOR:
			return("ҽ��");
			break;

		case NURSE:
			return("��ʿ");
			break;

		case ILLNESS_CHANGE:
			return("����仯");
			break;

		case PHARMACY:
			return("��ҩ");
			break;

		case MESSAGE_BOARD:
			return("���԰�");
			break;

		case DIET_INFO:
			return("��ʳ��Ϣ");
			break;

		case DOCTOR_ADVICE:
			return("ҽ��");
			break;

		case HINT:
			return("��ʾ");
			break;
			
			
		case OX_SUPPLY:
			return("����");
			break;

		case OX_START:
			return("��ʼ");
			break;

		case OX_END:
			return("����");
			break;

		case OX_SUBTOTAL:
			return("С��");
			break;
		case OX_TOTAL:
			return("�ۼ�");
			break;

		case MESSAGE2:
			return("��Ϣ��2");
			break;

		case MESSAGE3:
			return("��Ϣ��3");
			break;	

		case MESSAGE4:
			return("��Ϣ��4");
			break;	

		case MESSAGE5:
			return("��Ϣ��5");
			break;	

		case MESSAGE6:
			return("��Ϣ��6");
			break;	
			

		default:
			return(NULL);
			break;		
	}
}


uint8 GetTitleLength(uint8 ItemCode)
{
	uint8 title_length=0;
	switch(ItemCode)
	{
		case BED_NUMBER:
		case ILLNESS_LEVEL:
		case MEDICAL_LEVEL:
		case DIETARY_LEVEL:
		case NAME:
		case SEX:
		case AGE:
		case ADMISSION_DATE:
		case DIAGNOSIS:
		case BLOOD:
		case DOCTOR:
		case NURSE:
		case PHARMACY:
		case DOCTOR_ADVICE:
		case HINT:
		case OX_SUPPLY:	
		case OX_START:
		case OX_END:
		case OX_SUBTOTAL:
		case OX_TOTAL:
			 title_length= 4;			//����2�����ֵ�
			 break;



		case HOSPITALIZED_NO:
		case ALERGY_DRUGS:
		case MESSAGE_BOARD:
			 title_length= 6;			//����3�����ֵ�
			 break;			


		case MESSAGE2:
		case MESSAGE3:
		case MESSAGE4:
		case MESSAGE5:
		case MESSAGE6:
			 title_length= 7;			//����3.5�����ֵ�
			 break;				
			

        case CAUTION:
		case ILLNESS_CHANGE:
		case DIET_INFO:
			 title_length= 8;			//����4�����ֵ�
			 break;		

		default:
			title_length= 0;
			break;		
	}
	return title_length;
}


//��ʾ��ͷ�ֻ������ʽ
void ShowBedFaceForm(void)
{
	uint16 position=0;
	uint16 uiFormLen;
	page=1;
	fcolor=WHITE;
	Lcd_Clear();

//	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_START_ADDR,2);	//����ʽ�����ܸ���
	uiFormLen= stLocalControl.uiBedFaceFormLen;

	if(((stLocalControl.uiBedFaceFormLen/sizeof(STItemForm))>=MAX_ITEM_CODE) ||(stLocalControl.uiBedFaceFormLen ==0xffff))
	{//��Ŀ����
		stLocalControl.uiBedFaceFormLen=0;
		Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);
		Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);
		return;
	}

	for(;uiFormLen>=sizeof(STItemForm);)
	{
		Read_Cont(&(stLocalControl.stItemForm.ItemCode),BED_FACE_FORM_START_ADDR+position,sizeof(STItemForm));
		start_x = stLocalControl.stItemForm.StartPoint.x;
		start_y = stLocalControl.stItemForm.StartPoint.y;
		end_x = stLocalControl.stItemForm.EndPoint.x;
		end_y = stLocalControl.stItemForm.EndPoint.y;

		fcolor=GetColor(stLocalControl.stItemForm.fcolor);
		bcolor=GetColor(stLocalControl.stItemForm.bcolor);

		mul_col=1;
		mul_row=1;	
		DisplayBlock();

		if(stLocalControl.stItemForm.DispMode & 0x20)	//bit5
		{//��ʾ����		
			if(stLocalControl.stItemForm.DispMode & 0x10)	//bit4
			{//������ʾ
			
				pDispData=GetTitle(stLocalControl.stItemForm.ItemCode);
				Length = GetTitleLength(stLocalControl.stItemForm.ItemCode)/2;

				end_x = start_x +16;
				
				
				if(pDispData!=NULL)
				{//���������16���󣬶�������ʾ
					x=start_x;
					if( (end_y-start_y)/16>= Length)
					{//�ռ��㹻
						y= start_y + (end_y-start_y- 16*Length)/2;
						
					}
					else 
					{
						Length = (end_y-start_y)/16;
						y= start_y + (end_y-start_y- 16*Length)/2;
						
					}
					size=1;
					dot_number =16;
					current_vert_spac=0;
					mul_row=1;
					mul_col=1;
					DisplayString();//��ʾ����
					

					if(stLocalControl.stItemForm.DispMode & 0x08)	//bit3
					{//��ʾ��������
						x=end_x +1;
						size=1;
						mul_row=1;
						mul_col=1;
						Get_GT23_Data(":",1);
						Display_ASCII_Mul();					
					}

				}
			}
			else
			{//������ʾ
				pDispData=GetTitle(stLocalControl.stItemForm.ItemCode);
				if(pDispData!=NULL)
				{
					x=start_x;
					y=start_y;
					if(stLocalControl.stItemForm.DispMode &0x01)//bit0:�ж�����������Ƿ�����һ��
					{//����һ�� 16����

						y=start_y;
							
					}

					else 
					{
						if(end_y-start_y>16)
						{
							y=start_y +(end_y-start_y-16)/2;
						}
					}
					current_size=1;  //3
					get_dot_number();
					get_basesize_mul();
					DisplayString();//��ʾ����

					if(stLocalControl.stItemForm.DispMode & 0x08)	//bit3
					{//��ʾ��������
						size=1;
						mul_col=1;
						mul_row=1;
						Get_GT23_Data(":",1);
						Display_ASCII_Mul();							
				
					}	
				}
			}
		}
		
		uiFormLen	-=sizeof(STItemForm);
		position	+=sizeof(STItemForm);
		
	}
	Set_VisualPage(page);
	stLocalControl.byDisplayFace = BED_INFO_FACE;	
}
