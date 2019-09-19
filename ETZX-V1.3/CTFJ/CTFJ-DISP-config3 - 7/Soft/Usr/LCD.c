/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:TFT.c
*文件描述    		:TFT彩屏驱动程序
*创建人     		:尹运同
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     							
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
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

bit fTitle_TextAtLine=0;	//正文与标题在同一行标志

uint8 dot_number=0;
uint8  current_size=0;	//当前字符尺寸
uint8  current_vert_spac=0;//当前行间距


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
void ShowPowerUpFace(void);	//写在第0页
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
函数原型: void Send_Ssd1963_Command(uint8 xdata ssd1963_command)
参数说明: ssd1963_command:需要写的命令
返 回 值: 无
函数功能: 写命令
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
函数原型: void Send_Ssd1963_Data(uint8 xdata ssd1963_data)
参数说明: ssd1963_data:需要写的数据
返 回 值: 无
函数功能: 写数据
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
函数原型: void Read_Ssd1963_Data()
参数说明: 
返 回 值: 无
函数功能: 读参数
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
函数原型: void InitSSD1963(void)
参数说明: 无
返 回 值: 无
函数功能: 初始化LCD控制器
----------------------------------------------------------------------------*/
void InitSSD1963(void)
{
	//-------------------------
	SSD1963_TE=1;
	SSD1963_RD=1;
	SSD1963_CS=0;
	//-------------------------
	SSD1963_RST=1;			   //硬件复位
	Delayms(50);
	SSD1963_RST=0;
	Delayms(200);
	SSD1963_RST=1;
	Delayms(50);

	
	//设定PLL分频系数
	//pll: n= 34 m= 3
	Send_Ssd1963_Command(cmd_set_pll_mn);		   
	Send_Ssd1963_Data(0x3B); //倍频系数为60 晶振为6M 倍频后为360M VCO
	Send_Ssd1963_Data(0x02); //分频系数为3 工作频率为360M/3=42M
	Send_Ssd1963_Data(0x54); //固定值	

	Send_Ssd1963_Command(cmd_set_pll); //允许PLL
	Send_Ssd1963_Data(0x01);   
	//延时1Ms
	Delayms(1);
	Send_Ssd1963_Command(cmd_set_pll);		 //锁定PLL
	Send_Ssd1963_Data(0x03);


	//software reset		

	Send_Ssd1963_Command(cmd_soft_reset);
	
	//设定像素时钟	
	//屏幕为4.3时(480x272)，帧频为30FPS 像素时钟为3.9168M	FPR=32596		 60FPS时 FPR= 65192
	Send_Ssd1963_Command(cmd_set_lshift_freq);			
	Send_Ssd1963_Data(0x01); //00
	Send_Ssd1963_Data(0x33); //63
	Send_Ssd1963_Data(0x32); //32
	
	//面板参数设定		   
	Send_Ssd1963_Command(cmd_set_lcd_mode);
	Send_Ssd1963_Data(0x18);  //显示18bit总线 允许FRC 下降沿及低电平有效
	Send_Ssd1963_Data(0x20);  //TFT RGB总线 允许控制信号
	Send_Ssd1963_Data((DISP_HOR_RESOLUTION-1)>>8);
	Send_Ssd1963_Data(DISP_HOR_RESOLUTION-1);  //水平像素480
	Send_Ssd1963_Data((DISP_VER_RESOLUTION-1)>>8); 
	Send_Ssd1963_Data(DISP_VER_RESOLUTION-1);  //Hor 像素272
	Send_Ssd1963_Data(0x00);  //像素序列RGB


		
	//水平同步信号设定
	Send_Ssd1963_Command(cmd_set_hori_period);	   
		 
#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
	Send_Ssd1963_Data((HT-1)>>8);
	Send_Ssd1963_Data(HT-1); //水平周期 503个时钟周期 (包含显示周期与非显示周期)
#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
	Send_Ssd1963_Data((HPS-1)>>8); 
	Send_Ssd1963_Data(HPS-1); //水平非显示周期时钟个数为42
	Send_Ssd1963_Data(DISP_HOR_PULSE_WIDTH-1); //水平同步信号宽度时钟个数为3
	Send_Ssd1963_Data(0x00); 
	Send_Ssd1963_Data(0x00); //水平同步信号开始位置
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

		//显示像素格式设定
	Send_Ssd1963_Command(cmd_set_pixel_format); 	  
#ifndef DATA_BUS8
	Send_Ssd1963_Data(0x50); //16bit
#else
	Send_Ssd1963_Data(RGB_DATA_FORMAT_18); //24bit
#endif

	//数据总线接口设定
	
	Send_Ssd1963_Command(cmd_set_pixel_data_interface);
#ifndef DATA_BUS8	     
	Send_Ssd1963_Data(0x03);   //16BIT RGB565
#else
	Send_Ssd1963_Data(0x00);   //8BIT
#endif

	//设置RGB颜色顺序
	Send_Ssd1963_Command(0x36);
	Send_Ssd1963_Data(0x08);   //RGB 

#ifdef USE_SSD1963_PWM	
	Send_Ssd1963_Command(0xbe);//设置PWM 
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
	Send_Ssd1963_Data(0x40);//对比度值
	Send_Ssd1963_Data(0x10);//亮度值  
	Send_Ssd1963_Data(0x40);//饱和度值 
	Send_Ssd1963_Data(0x01);//处理机允许
*/




// set pwm enable by host
	P3M0 |= Bin(0,0,1,0,0,0,0,0);	//开漏输出
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

	
	//开显示
	Send_Ssd1963_Command(cmd_set_display_on);


/*
	//强推挽输出
	P3M0 |= (Bin(0,0,1,0,0,0,0,0));
	P3M1 &= (~Bin(0,0,1,0,0,0,0,0));
	//打开背光
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
函数原型: void Set_Display_Area(uint16 xdata start_x,uint16 xdata end_x,uint16 xdata start_y,uint16 xdata end_y) 
参数说明: start_x:点的X起始坐标
          end_x:点的X结束坐标
		  start_y:点的Y起始坐标
          end_y:点的Y结束坐标
返 回 值: 无
函数功能: 设置屏幕区域
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
函数原型: void Lcd_Clear(uint32 xdata color)
参数说明: color:清屏的颜色
返 回 值: 无
函数功能: 清屏
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
函数原型: void Display_Dot(uint16 xdata x,uint16 xdata y,uint32 xdata color)   
参数说明: x:点的X坐标,y:点的Y坐标,color:点的颜色
返 回 值: 无
函数功能: 在屏幕上显示点
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
函数原型: void Display_HLine(uint16 xdata x,uint16 xdata y,uint16 xdata width,uint32 xdata color) 
参数说明: x:点的X坐标,y:点的Y坐标,width:水平直线的长度,color:点的颜色
返 回 值: 无
函数功能: 在屏幕上显示水平直线
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
函数原型: void Display_VLine(uint16 xdata x,uint16 xdata y,uint16 xdata height,uint32 xdata color)
参数说明: x:点的X坐标,y:点的Y坐标,height:垂直直线的长度,color:点的颜色
返 回 值: 无
函数功能: 在屏幕上显示垂直直线
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


//显示一个方框
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




//显示一个颜色块
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
 函数原型: void Display_ASCII(uint16 xdata x,uint16 xdata y,uint8 xdata *p,uint32 xdata fcolor,uint32 xdata bcolor)
 参数说明: x:字符的X坐标,y:字符的Y坐标,frcolor:字符的颜色，brcolor:字符的背景颜色
 返 回 值: 无
 函数功能: 在屏幕上显示字符
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
	   case 4://48*96   在这种字体下，不存在放大  4号字体特殊处理
			//因GT23_REC_DATA[128]存储空间有限，只能按如下处理

		   if( (*pDispData>=0x30)&&(*pDispData<=0x39)) 
		   	{//数字内码
				Set_Display_Area(x,x+47,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+95);
				Send_Ssd1963_Command(0x2C);
		
				
				for(i=0;i<6;i++)
				{//分8次读取点阵数据到GT23_REC_DATA
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
函数原型: void Display_Chinese(uint x,uint y,uchar *p,ulong fcolor,ulong bcolor)
参数说明: x:汉字的X坐标,y:汉字的Y坐标,fcolor:汉字的颜色，bcolor：汉字的背景颜色
返 回 值: 无
函数功能: 在屏幕上显示汉字
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


//写前所写区域处理
// uint8 WriteAreaHandlerMul(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint8 type,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y)
uint8 WriteAreaHandlerMul(uint8 type)
{
	uint16 old_x =x;
	uint16 old_y =y;
	
	if(type==0) //半角数据
	{
		if(x+dot_number/2-1 > end_x)
		{//超过右边界,从头开始
			x=start_x;
			if(y+2*dot_number+current_vert_spac-1>end_y)    goto fail;	//超出所写区域
			else
			{
				y += (dot_number+current_vert_spac);	//换到下一行
				if(x+dot_number/2-1 > end_x)  goto fail;		//从新行开始，还是1个字符都放不下
				else return 1;
			}
		}
		else 
		{
			return 1;
		}

	}
	else if(type==1)	//全角数据
	{
	
		if(x+dot_number-1 >end_x)
		{//超过右边界,从头开始
			x=start_x;
			if(y+2*dot_number+current_vert_spac-1>end_y)    goto fail;	//超出所写区域
			else
			{
				y += (dot_number+current_vert_spac);	//换到下一行
				if(x+dot_number-1 > end_x)  goto fail;		//从新行开始，还是1个汉字都放不下
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


//x,y:起始点的坐标
//type_x_wide:原子体X轴的宽度, type_y_height:原子体Y轴的高度
//mul_row:纵向放大倍数,   mul_col:横向放大一倍
//cbc_filter:过滤参数 1:过滤背景色
//p:要显示的图片数据指针

//void lcd_fangda_dis_text(uint16 x, uint16 y, uint8 type_x_wide, uint8 type_y_height, 
//						 uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *p,uint8 page)
void lcd_fangda_dis_text(uint8 type_x_wide, uint8 type_y_height)
{

	unsigned char  i,j,k,m,n;


	Set_Display_Area(x,x+type_x_wide*mul_col-1,y+page*LCD_PIX_Y,y+page*LCD_PIX_Y+type_y_height*mul_row-1);
	Send_Ssd1963_Command(0x2C);


	SSD1963_CS=0;	

	for (i=0;i<type_y_height;i++) //原字体有多少行
	{

		for(n=0;n<mul_row;n++)	//纵坐标的放大倍数
		{

			for(j=0;j<type_x_wide/8;j++)	//一横有几个字节,例16*16 行向2个字节
			{

				for(k=0;k<8;k++)	//1个字节8点
				{

					for(m=0;m<mul_col;m++)	//横坐标放大倍数
					{
						//if(((x+8*j*mul_col+k*mul_col+m)<LCD_PIX_X)&&((y+i*mul_row+n)<LCD_PIX_Y))	//在区域范围内
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





//px,py  写入字符串的起始x,y坐标
//size:  字体大小
//mul_row,mul_col:行列放大倍数
//fcolor,bcolor:前景色，背景色
//pDigit:要写入的字符串
//page: 写入的页面
//s_x,s_y:写入区域的起始点坐标
//end_x,end_y:写入区域的结束点坐标


//void DisplayString(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *pDispData,uint8 page,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y)
void DisplayString(void)
{
	
	while(*pDispData)
	{	
		if((*pDispData>=0x81)&&(*pDispData<=0xfe))	//高字节符合汉字内码条件
		{
			pDispData++;
			if(*pDispData>=0x40)
			{
			
				pDispData--;
				Get_GT23_Data(pDispData,size);	

				//先进行写空间处理
				if(WriteAreaHandlerMul(1)==0)   break;	//超出区域退出

				Display_Chinese_Mul();

				x+=dot_number;
				pDispData=pDispData+2;	//一个汉字内码占两个字节

			}
			else
			{//低字节不符合汉字条件
				if(*pDispData==0x00) break;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e))	//符合ASCII字符条件
		{
			Get_GT23_Data(pDispData,size);
						
			//先进行写空间处理
			if(WriteAreaHandlerMul(0)==0)   break;	//超出区域退出
			
			Display_ASCII_Mul();

			 x+= dot_number/2;
			pDispData++;
		}
		else //非法字符
		{
			pDispData++;
		}

	}
	WDT_CONTR = 0x3e;	//喂狗
}
		

//px,py  写入字符串的起始x,y坐标
//size:  字体大小
//mul_row,mul_col:行列放大倍数
//fcolor,bcolor:前景色，背景色
//pDigit:要写入的字符串
//page: 写入的页面
//s_x,s_y:写入区域的起始点坐标
//end_x,end_y:写入区域的结束点坐标
//lenth:数字长度

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
			Get_GT23_Data(&Digit,size);	//十六进制中的ABC....
		}
		if(WriteAreaHandlerMul(0)==0) return;
		Display_ASCII_Mul();

		x+= dot_number/2;
		pDispData++;
	}	
} 


//根据长度写入字符
//px,py  写入字符串的起始x,y坐标
//size:  字体大小
//mul_row,mul_col:行列放大倍数
//fcolor,bcolor:前景色，背景色
//pDigit:要写入的字符串
//page: 写入的页面
//s_x,s_y:写入区域的起始点坐标
//end_x,end_y:写入区域的结束点坐标


//void DisplayStringLen(uint16 *px,uint16 *py,uint8 size,uint8 mul_row,uint8 mul_col,uint16 fcolor,uint16 bcolor,uint8 *pDispData,uint8 page,uint16 s_x,uint16 s_y,uint16 end_x,uint16 end_y,uint8 Length)
void DisplayStringLen(void)
{
	for(;Length>0;)
	{	
		if((*pDispData>=0x81)&&(*pDispData<=0xfe))	//高字节符合汉字内码条件
		{
			pDispData++;
			if(*pDispData>=0x40)
			{
				if(Length>=2)
				{//防止混乱数据串时数据长度向0  溢出			
					pDispData--;
					Get_GT23_Data(pDispData,size);	

					//先进行写空间处理
					if(WriteAreaHandlerMul(1)==0)   break;	//超出区域退出

					Display_Chinese_Mul();

					x+= dot_number;
					pDispData=pDispData+2;	//一个汉字内码占两个字节
					Length -=2;
				}
				else
				{//长度不足2
					Length--;				
				}
			}
			else
			{//低字节不符合汉字条件
				Length--;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e)) //符合ASCII字符条件
		{
			Get_GT23_Data(pDispData,size);
			//先进行写空间处理
			if(WriteAreaHandlerMul(0)==0)   break;	//超出区域退出
			
			Display_ASCII_Mul();

			x +=dot_number/2;
			pDispData++;
			Length--;
		}
		else	//其它不相关数据
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
	pDispData= "加";
	start_x=0;
	start_y=0;
	end_x=480;
	end_y=272;
	DisplayString();

	DisplayOneChar(byBedAddr);

	pDispData= "床呼叫";	
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
	pDispData= "本机故障!";
	start_x=357;
	start_y=246;
	end_x=480;
	end_y=272;

	DisplayString();
		
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=INFO_INDICATION_FACE;

//	BL_SW=1;	//背关打开
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
//	CCAPM0=0X48;	//禁止调度
//	MCUMasterSPI(); //单片机为主模式
	switch(byCallType)
	{
		case CMD_INFUSION_CALL:								//护理呼叫
			x=(480-32*7)/2;			//128
			y=(272-32)/2;			//120

			fcolor=WHITE;
			bcolor=BLUE;
			start_x=0;
			start_y=0;
			end_x=480;
			end_y=272;
			DisplayOneChar(byRoomAddr);

			pDispData= "房";
			DisplayString();

			DisplayOneChar(byBedAddr);

			pDispData= "床呼叫";
			DisplayString();			
			break;

		case CMD_SERVICE_CALL:								//输液完毕呼叫
			x=(480-32*8)/2;
			y=(272-32)/2;

			fcolor=WHITE;
			bcolor=BLUE;
			start_x=0;
			start_y=0;
			end_x=480;
			end_y=272;

			DisplayOneChar(byRoomAddr);
			pDispData= "房";
			DisplayString();

			DisplayOneChar(byBedAddr);
			pDispData= "床输液完毕";
			DisplayString();
			break;			
		case CMD_EMERGENCY_CALL:   							//紧急呼叫
			
			x=(480-32*7)/2;
			y=(272-32)/2;
			fcolor=WHITE;
			bcolor=BLUE;
			start_x=0;
			start_y=0;
			end_x=480;
			end_y=272;

			DisplayOneChar(byRoomAddr);
			pDispData= "房卫生间呼叫";
			DisplayString();						
			break;
	}	

//	EPH1660MasterSPI(); //
//	CCAPM0=0X49;	//开启调度

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=INFO_INDICATION_FACE;	
}


/**********************************************************
*函数名称			:ShowTalkFace	
*函数描述        	:显示通话界面
*输入参数   		:byRoomAddr:房地址,byBedAddr:床地址
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:
*创建日期	 		:2009-3-16
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
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
	pDispData= "本机正在通话中";
	DisplayString();	


	Set_VisualPage(page);
	stLocalControl.byDisplayFace=TALK_FACE;
}




/*********************************************************
函数名称			:ShowBroadFace	
*函数描述        	:显示通话界面
*输入参数   		:byRoomAddr:房地址,byBedAddr:床地址,bSelf:是否自身广播中
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2009-4-14
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
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

//	CCAPM0=0X48;	//禁止调度
//	MCUMasterSPI(); //单片机为主模式	

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
		pDispData= "本机正在广播中";
		DisplayString();
	}
	else
	{
		if(byRoomAddr==YHFJ_ROOM_ADDR)
		{

			x=(480-32*10)/2;
			y=(272-32)/2;

			DisplayOneChar(byBedAddr);
			
			pDispData= "号医护分机正在广播";
			DisplayString();
					
		}
		else
		{			
			x=(480-32*8)/2;
			y=(272-32)/2;

			DisplayOneChar(byRoomAddr);
			
			pDispData= "房";
			DisplayString();

			DisplayOneChar(byBedAddr);
			
			pDispData= "床正在广播";
			DisplayString();			

		
		}
	}

//	EPH1660MasterSPI();	//
//	CCAPM0=0X49;	//开启调度	

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
	pDispData= "开始护理呼叫";
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
	pDispData= "开始服务呼叫";	
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
	pDispData= "开始紧急呼叫";	
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
	pDispData= "开始求援呼叫";
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

	uint8 title_length=0;	//标题长度
	uint8 LengthBack=Length;	//备份要显示的原始数据长茺
	uint8 *pdispdata2 = pDispData;	//备份原始要显示的数据


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
	{//高度太小，连最小尺寸的字符都显示不下，直接跳过
		fTitle_TextAtLine =0;
		return;
	}

	if(fTitle_TextAtLine)
	{
		title_length = GetTitleLength(stLocalControl.stItemForm.ItemCode);	//获取标题长度进入到总长度中
		if(stLocalControl.stItemForm.DispMode &0x08)  title_length ++;    //bit3:标题是带“：”
		Length += title_length;
								
	}

	do
	{
		get_dot_number();

		RowCharNum= area_height/(dot_number+current_vert_spac);

		ColumnCharNum= area_width/(dot_number/2);
		
		if(Length<=RowCharNum*ColumnCharNum)
		{//个数符合要求
			get_basesize_mul();

			x=start_x;  // 横向顶格显示
			if(area_width >= Length * (dot_number/2)) y=start_y + (area_height - dot_number)/2; 	// 1行就能显示�
			else y= start_y +(area_height-(dot_number+current_vert_spac)*(RowCharNum-1)-dot_number)/2;   //竖向居中显示
			
			if(fTitle_TextAtLine)
			{//标题与正文同行

				//if(area_width >= Length * (dot_number/2)) y=start_y + (area_height - dot_number)/2; 	// 1行就能显示，
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
			{//只能显示一行
				if(fTitle_TextAtLine)
				{//标题与正文同行
					if(area_width>= title_length*8+LengthBack*(dot_number/2))	//标题用16字体
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
	
	WDT_CONTR = 0x3e;	//喂狗
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
	pDispData= "呼叫成功,请等待";
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

	//读取界面格式总长度
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_ADDR,2);	


	//从格式段中去找开始与结束点
	num= stLocalControl.uiBedFaceFormLen/sizeof(STItemForm);
	if(num>=MAX_ITEM_CODE)
	{//超限
		stLocalControl.uiBedFaceFormLen=0;
		Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);
		Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);
		return;
	}

	i=0;
	for(;num>0;num--)
	{
		if(Read(BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm))== ItemCode) break; //找到项目代码
		else i++;
		
	}
	if(num!=0)
	{//找到对应的格式项
		Read_Cont(&(stLocalControl.stItemForm.ItemCode),BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm),sizeof(STItemForm));//读取整项格式数据
		start_x=stLocalControl.stItemForm.StartPoint.x;
		start_y=stLocalControl.stItemForm.StartPoint.y;

		end_x= stLocalControl.stItemForm.EndPoint.x;
		end_y= stLocalControl.stItemForm.EndPoint.y;


		if(end_x >= LCD_PIX_X || end_y >=LCD_PIX_Y) return;
		if(start_x>=end_x || start_y>= end_y) return;

		if((ItemCode == ILLNESS_LEVEL) ||(ItemCode == MEDICAL_LEVEL)||(ItemCode == NURSING_LEVEL)||(ItemCode == DIETARY_LEVEL))
		{//特例,背景字体颜色从数据项中取
			fcolor= GetColor(*(stLocalControl.stItemData.pData));	//字体
			bcolor= GetColor(*(stLocalControl.stItemData.pData+1)); 					
		}
		else
		{
			fcolor= GetColor(stLocalControl.stItemForm.fcolor);
			bcolor= GetColor(stLocalControl.stItemForm.bcolor);
		}
		if(ItemCode!=BED_NUMBER)
		{//床号没有标题，不受标题格式影响
			if(stLocalControl.stItemForm.DispMode & 0x20)	//bit5:是否显示标题
			{//显示标题
				if(stLocalControl.stItemForm.DispMode &0x10)//bit4:标题显示方向
				{//竖向显示
					fTitle_TextAtLine =0;
					if(stLocalControl.stItemForm.DispMode &0x08)//bit3:标题是否带“：”
					{//标题带“：”
						start_x +=24;
						
					}
					else
					{//标题不带":"
						start_x +=16;
					}
					
				}
				else
				{//横向显示
					//if((ItemCode == OX_START) ||(ItemCode == OX_END)||(ItemCode == OX_SUBTOTAL)||(ItemCode ==OX_TOTAL))
					if(stLocalControl.stItemForm.DispMode &0x01)//bit0:判断正文与标题是否另起一行
					{//另起一起
						start_y +=16;
						fTitle_TextAtLine =0;
					}

					else 
					{//在一行显示
						fTitle_TextAtLine =1;
									
					}

				}
			
			}
		}
		DisplayBlock();//把原有的消息清空	
	switch(ItemCode)
		{
			case OX_START:
				BedDataBuff[0]=(stLocalControl.stSupplyOxStartTime.byHour/10)+0x30;	//转换成ASCII数字
				BedDataBuff[1]=(stLocalControl.stSupplyOxStartTime.byHour%10)+0x30;
				BedDataBuff[2]= 58;		//":"号
				BedDataBuff[3]=(stLocalControl.stSupplyOxStartTime.byMin/10)+0x30;
				BedDataBuff[4]=(stLocalControl.stSupplyOxStartTime.byMin%10)+0x30;
				BedDataBuff[5]=0x00;

				pDispData = BedDataBuff;
				Length = 5;				
				ShowInfoBlockLen();				
				break;
			case OX_END:
				BedDataBuff[0]=(stLocalControl.stSupplyOxEndTime.byHour/10)+0x30;	//转换成ASCII数字
				BedDataBuff[1]=(stLocalControl.stSupplyOxEndTime.byHour%10)+0x30;
				BedDataBuff[2]= 58;		//":"号
				BedDataBuff[3]=(stLocalControl.stSupplyOxEndTime.byMin/10)+0x30;
				BedDataBuff[4]=(stLocalControl.stSupplyOxEndTime.byMin%10)+0x30;
				BedDataBuff[5]=0x00;

				pDispData = BedDataBuff;
				Length = 5;				
				ShowInfoBlockLen();	
				break;
			case OX_SUBTOTAL:
				x=stLocalControl.stSupplyOxSubtotalTime.uiHour%10000;	//得千位余数
				//BedDataBuff[0]=(x/1000)+0x30;   //得千位数  转换成ASCII数字
				y=x%1000;						//得百位余数
				//BedDataBuff[1]=(y/100)+0x30;		//得百位数
				BedDataBuff[0]=((y%100)/10)+0x30;
				BedDataBuff[1]=((y%100)%10)+0x30;		
				BedDataBuff[2]= 58;		//":"号
				BedDataBuff[3]=(stLocalControl.stSupplyOxSubtotalTime.byMin/10)+0x30;
				BedDataBuff[4]=(stLocalControl.stSupplyOxSubtotalTime.byMin%10)+0x30;		
				BedDataBuff[5]=0x00;
				

				pDispData = BedDataBuff;
				Length = 5;				
				ShowInfoBlockLen();	
				break;
				
			case OX_TOTAL:
				DisplayBlock();//把原有的消息清空	
				x=stLocalControl.stSupplyOxTotalTime.uiHour%10000;	//千位余数
				BedDataBuff[0]=(x/1000)+0x30;       //转换成ASCII数字
				y=x%1000;
				BedDataBuff[1]=(y/100)+0x30;
				BedDataBuff[2]=((y%100)/10)+0x30;
				BedDataBuff[3]=((y%100)%10)+0x30;		
				BedDataBuff[4]= 58;		//":"号
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
					Length=2;	//只显示两个数字
				}
				else
				{
					pDispData =  BedDataBuff;
					Length=3;	//显示3个数字				
				}
				if((end_y - start_y+1 >=96)&&(end_y - start_y+1 <152))
				{//y空间满足特殊字体要求
					if(end_x - start_x +1 >= (48*Length))
					{//x空间也满足写下床号的要求
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

	//读取界面格式总长度
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_ADDR,2);	
	//读取界面数据总长度
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_LENGTH_ADDR,2);


	uiDataLen = stLocalControl.uiBedFaceDataLen;
	//从格式段中去找开始与结束点
	num= stLocalControl.uiBedFaceFormLen/sizeof(STItemForm);
	if(num>=MAX_ITEM_CODE)
	{//超限
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
		{//有数据
			if((stLocalControl.stItemData.DataLength<=SEGMENT_DATA_LEN)&&(stLocalControl.stItemData.ItemCode < MAX_ITEM_CODE))
			{//数据段数据不超长或者功能码相符
				i=0;
				num= stLocalControl.uiBedFaceFormLen/sizeof(STItemForm);
				for(;num>0;num--)
				{
					if(Read(BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm))== stLocalControl.stItemData.ItemCode) break; //找到项目代码
					else i++;
					
				}
				if(num!=0)
				{//找到对应的格式项
					Read_Cont(&(stLocalControl.stItemForm.ItemCode),BED_FACE_FORM_START_ADDR+i*sizeof(STItemForm),sizeof(STItemForm));//读取整项格式数据
					if((stLocalControl.stItemForm.EndPoint.x > stLocalControl.stItemForm.StartPoint.x)&&
					   (stLocalControl.stItemForm.EndPoint.y > stLocalControl.stItemForm.StartPoint.y))

					{//起始点X，Y坐标符合要求
						start_x=stLocalControl.stItemForm.StartPoint.x;
						start_y=stLocalControl.stItemForm.StartPoint.y;

						end_x= stLocalControl.stItemForm.EndPoint.x;
						end_y= stLocalControl.stItemForm.EndPoint.y;


						stLocalControl.stItemData.pData = BedDataBuff;
						Read_Cont(stLocalControl.stItemData.pData ,BED_FACE_DATA_START_ADDR+position,stLocalControl.stItemData.DataLength);


						if((stLocalControl.stItemData.ItemCode == ILLNESS_LEVEL) ||(stLocalControl.stItemData.ItemCode == MEDICAL_LEVEL)||(stLocalControl.stItemData.ItemCode == NURSING_LEVEL)||(stLocalControl.stItemData.ItemCode == DIETARY_LEVEL))
						{//特例,背景字体颜色从数据项中取
							fcolor= GetColor(*(stLocalControl.stItemData.pData));	//字体
							bcolor= GetColor(*(stLocalControl.stItemData.pData+1));						
						}
						else
						{
							fcolor= GetColor(stLocalControl.stItemForm.fcolor);
							bcolor= GetColor(stLocalControl.stItemForm.bcolor);
						}

						if(stLocalControl.stItemForm.DispMode & 0x20)	//bit5:是否显示标题
						{//显示标题
							if(stLocalControl.stItemForm.DispMode &0x10)//bit4:标题显示方向
							{//竖向显示
								fTitle_TextAtLine=0;
								if(stLocalControl.stItemForm.DispMode &0x08)//bit3:标题是否带“：”
								{//标题带“：”
									start_x +=24;
									
								}
								else
								{//标题不带":"
									start_x +=16;
								}
								
							}
							else
							{//横向显示
								if(stLocalControl.stItemForm.DispMode &0x01)//bit0:判断正文与标题是否另起一行
								{//另起一行
									start_y +=16;
									fTitle_TextAtLine=0;
								}
								else
								{//在同一行
									fTitle_TextAtLine =1;
															
								}
							}
							
						}
						
						DisplayBlock();//把原有的消息清空	
						if((stLocalControl.stItemData.ItemCode == ILLNESS_LEVEL) ||(stLocalControl.stItemData.ItemCode == MEDICAL_LEVEL)||(stLocalControl.stItemData.ItemCode == NURSING_LEVEL)||(stLocalControl.stItemData.ItemCode == DIETARY_LEVEL))
						{//特例,真正的数据从第三个字节开始
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
		}  //有数据的末尾

	}

}



void ShowBedFace(void)
{	
	uint8 i;
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_ADDR,2);	//读格式数据总个数

	if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2)) //将数据长度也包含进去
	{
		ShowBedFaceForm();	
		if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2)!=1)
		{//如果备份区不正确
			for(i=0;i<5;i++)
			{
				SectorDataCopy(BED_FACE_FORM_BAK,BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+3);	//拷贝至另一备份区				
				if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2)==1)  break;	
			}		
		}
	}
	else
	{//主格式不正确
		Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_BAK,2);	//读备份区格式数据总个数	
	
		if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2))
		{//备份区正确
			while(1)
			{
				SectorDataCopy(BED_FACE_FORM_LENGTH_ADDR,BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+3);	//拷贝至主格式区				
				if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2)==1)  break;					
			}
			ShowBedFaceForm();
		}
		else return;		//主格式区不正确，备份格式区也不正确。
	}


//	ShowBedFaceForm();
	WDT_CONTR = 0x3e; 
	ShowItem(BED_NUMBER);
	Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_LENGTH_ADDR,2);	//读床头卡数据总个数

	if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)) //将数据长度也包含进去
	{
		
		
		ShowPatientBaseInfo();			
		if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)!=1)
		{//如果备份区不正确
			for(i=0;i<5;i++)
			{
				SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);	//拷贝至另一备份区				
				if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)==1)  break;	
			}		
		}
	}
	else
	{//主格式不正确
		Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_BAK,2);	//读备份区床头卡数据总个数
		
		if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2))
		{//备份区正确
			while(1)
			{
				SectorDataCopy(BED_FACE_DATA_LENGTH_ADDR,BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+3);	//拷贝至主数据区				
				if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)  break;					
			}
			ShowPatientBaseInfo();
		}
		else return;		//主数据区不正确，备份数据区也不正确。
	}
//    ShowPatientBaseInfo();
	ShowItem(OX_START);
	ShowItem(OX_END);
	ShowItem(OX_SUBTOTAL);	
	ShowItem(OX_TOTAL);

}



void ShowPowerUpFace(void)	//写在第0页
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
	pDispData="系统初始化...";
	start_x =0;
	start_y =0;
	end_x = 480;
	end_y =272;		
	DisplayString();
	

	y += (dot_number+current_vert_spac);			//106
	x=(LCD_PIX_X-dot_number*4)/2;		//176
	pDispData="版本V";
	DisplayString();

	stLocalControl.stEepromCfgData.byVersionHi=3;
	DisplayOneChar(stLocalControl.stEepromCfgData.byVersionHi);
	pDispData=".";
    DisplayString();
	stLocalControl.stEepromCfgData.byVersionLo=7;
	DisplayOneChar(stLocalControl.stEepromCfgData.byVersionLo);
	
	

	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*10)/2;
	pDispData="病房护理系统床头分机";
	DisplayString();	


/*	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*4)/2;
	pDispData="床头分机";
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
	pDispData="区";
	DisplayString();


    DisplayOneChar(stLocalControl.stBusDealFreq.bySndRoomAddr);
	pDispData ="房";
	DisplayString();

	
    DisplayOneChar(stLocalControl.stBusDealFreq.bySndBedAddr);
	pDispData ="床开始编号";
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
	pDispData="编号成功";
	DisplayString();
	
	Set_VisualPage(page);
	stLocalControl.byDisplayFace=NUM_SET_OK_FACE;

	page=1;
	ShowItem(BED_NUMBER);		//写在第1页
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
	pDispData="输液报警器开始对码";	
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
	pDispData="输液报警器对码成功";		
 	DisplayString();

	Set_VisualPage(page);
	stLocalControl.byDisplayFace=RF_NUM_SET_OK_FACE;		
}




uint8* GetTitle(uint8 ItemCode)
{
	switch(ItemCode)
	{
		case BED_NUMBER:
			return("床号");
			break;

		case ILLNESS_LEVEL:
			return("病情");
			break;

		case MEDICAL_LEVEL:
			return("医保");
			break;

		case NURSING_LEVEL:
			return("护理");
			break;

		case DIETARY_LEVEL:
			return("饮食");
			break;

		case HOSPITALIZED_NO:
			return("入院号");
			break;

		case NAME:
			return("姓名");
			break;

		case SEX:
			return("性别");
			break;

		case AGE:
			return("年龄");
			break;
		
		case ADMISSION_DATE:
			return("入院");
			break;

		case DIAGNOSIS:
			return("诊断");
			break;

		case BLOOD:
			return("血型");
			break;

        case CAUTION:
			return("安全警示");
			break;

		case ALERGY_DRUGS:
			return("过敏史");
			break;

			
		case DOCTOR:
			return("医生");
			break;

		case NURSE:
			return("护士");
			break;

		case ILLNESS_CHANGE:
			return("病情变化");
			break;

		case PHARMACY:
			return("用药");
			break;

		case MESSAGE_BOARD:
			return("留言板");
			break;

		case DIET_INFO:
			return("饮食信息");
			break;

		case DOCTOR_ADVICE:
			return("医嘱");
			break;

		case HINT:
			return("提示");
			break;
			
			
		case OX_SUPPLY:
			return("供氧");
			break;

		case OX_START:
			return("开始");
			break;

		case OX_END:
			return("结束");
			break;

		case OX_SUBTOTAL:
			return("小计");
			break;
		case OX_TOTAL:
			return("累计");
			break;

		case MESSAGE2:
			return("消息栏2");
			break;

		case MESSAGE3:
			return("消息栏3");
			break;	

		case MESSAGE4:
			return("消息栏4");
			break;	

		case MESSAGE5:
			return("消息栏5");
			break;	

		case MESSAGE6:
			return("消息栏6");
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
			 title_length= 4;			//都是2个汉字的
			 break;



		case HOSPITALIZED_NO:
		case ALERGY_DRUGS:
		case MESSAGE_BOARD:
			 title_length= 6;			//都是3个汉字的
			 break;			


		case MESSAGE2:
		case MESSAGE3:
		case MESSAGE4:
		case MESSAGE5:
		case MESSAGE6:
			 title_length= 7;			//都是3.5个汉字的
			 break;				
			

        case CAUTION:
		case ILLNESS_CHANGE:
		case DIET_INFO:
			 title_length= 8;			//都是4个汉字的
			 break;		

		default:
			title_length= 0;
			break;		
	}
	return title_length;
}


//显示床头分机界面格式
void ShowBedFaceForm(void)
{
	uint16 position=0;
	uint16 uiFormLen;
	page=1;
	fcolor=WHITE;
	Lcd_Clear();

//	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_START_ADDR,2);	//读格式数据总个数
	uiFormLen= stLocalControl.uiBedFaceFormLen;

	if(((stLocalControl.uiBedFaceFormLen/sizeof(STItemForm))>=MAX_ITEM_CODE) ||(stLocalControl.uiBedFaceFormLen ==0xffff))
	{//项目超限
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
		{//显示标题		
			if(stLocalControl.stItemForm.DispMode & 0x10)	//bit4
			{//竖向显示
			
				pDispData=GetTitle(stLocalControl.stItemForm.ItemCode);
				Length = GetTitleLength(stLocalControl.stItemForm.ItemCode)/2;

				end_x = start_x +16;
				
				
				if(pDispData!=NULL)
				{//竖向标题用16点阵，都居中显示
					x=start_x;
					if( (end_y-start_y)/16>= Length)
					{//空间足够
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
					DisplayString();//显示标题
					

					if(stLocalControl.stItemForm.DispMode & 0x08)	//bit3
					{//显示“：”号
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
			{//横向显示
				pDispData=GetTitle(stLocalControl.stItemForm.ItemCode);
				if(pDispData!=NULL)
				{
					x=start_x;
					y=start_y;
					if(stLocalControl.stItemForm.DispMode &0x01)//bit0:判断正文与标题是否另起一行
					{//另起一起 16点阵

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
					DisplayString();//显示标题

					if(stLocalControl.stItemForm.DispMode & 0x08)	//bit3
					{//显示“：”号
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
