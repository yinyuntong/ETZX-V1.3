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
#include "math.h"


uint8 xdata fontbuf1,fontbuf2,fontbuf3;
uint8 xdata BedDataBuff[SEGMENT_DATA_LEN];
uint8 xdata ASCIIBuff[5];


uint16 code ColorArray[8]={BLACK,RED,GREEN,BLUE,YELLOW,PURPLE,CYAN,WHITE};

uint16 x;
uint16 y;
uint8 size;
uint8 mul_row;
uint8 mul_col;
uint16 fcolor;
uint16 bcolor;
uint8 *pDispData;
uint8 page=0;
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



STItemForm code default_bed_form[14]= 
{	
			{0x01,{4,10}, {164,170},0x09,0x08,0},
			{0x0a,{4,178},{164,231},0x09,0x08,0},
			{0x02,{4,239},{164,292},RED_CODE,BLACK_CODE,0},
			{0x03,{4,300},{164,353},GREEN_CODE,BLACK_CODE,0},
			{0x04,{4,361},{164,414},PURPLE_CODE,BLACK_CODE,0},
			{0x05,{4,422},{164,470},BLUE3_CODE,WHITE_CODE,0},
			{0x07,{172,10},{790,170},0x09,0x08,0},
			{0x08,{172,178},{468,231},0x09,0x08,0},
			{0x09,{476,178},{790,231},0x09,0x08,0},
			{0x0f,{172,239},{468,292},0x09,0x08,0x28},
			{0x10,{476,239},{790,292},0x09,0x08,0x28},
			{0x0e,{172,300},{790,353},0x09,0x08,0},
			{0x0d,{172,361},{790,414},0x09,0x08,0},
			{0x1c,{172,422},{790,470},0x09,0x08,0}
			
};

STItemData code default_bed_data[13]=
{
//{0x01,2,"20"},
{0x0a,8,"16-05-18"},
{0x02,4,"病重"},
{0x03,6,"新农合"},
{0x04,8,"一级护理"},
{0x05,4,"流食"},
{0x07,6,"岺雪强"},
{0x08,2,"女"},
{0x09,4,"65岁"},
{0x0f,4,"周诚"},
{0x10,4,"徐薇"},
{0x0e,10,"青霉素过敏"},
{0x0d,12,"防坠床防跌落"},
{0x1c,0,""}
};




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
void Set_VisualPage(void);
void ShowBedFaceForm(void);


void ShowItem(uint8 ItemCode);
void Display_ASCII(uint8 code *p);
uint8 GetTitleLength(uint8 ItemCode);
uint8* GetTitle(uint8 ItemCode);

void Display_ASCII_2(uint8 *p);

void get_dot_number();
void get_basesize_mul();

int8 get_size_mul_start_xy(uint8 item_code,uint8 disp_mode);



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

void Set_VisualPage(void)
{
	SetScrollStart(page*LCD_PIX_Y);
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
	//屏幕为4.3时(928x525)，帧频为60FPS时   像素时钟为29.232M
	//根据公式： FPR = 2^20*29.232/120-1 = 255432 = 0x3e5c8
	Send_Ssd1963_Command(cmd_set_lshift_freq);			
	Send_Ssd1963_Data(0x03); 
	Send_Ssd1963_Data(0xe5); 
	Send_Ssd1963_Data(0xc8); 

	
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

	SetScrollArea(0,480,0);


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

/*	Send_Ssd1963_Command(0xBF); //gET_PWM_CONF	
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
	while(stUsartCfg.uiSndLen);
	Send_Data_Byte(Read_Ssd1963_Data());
*/


	
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

  start_y += page*LCD_PIX_Y;
  end_y +=  page *LCD_PIX_Y;
  
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
void Lcd_Clear(void)
{
  uint16 xdata k,j;

  Set_Display_Area(0,LCD_PIX_X-1,0,LCD_PIX_Y-1);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  for(k=0;k<LCD_PIX_X;k++)
    {
     for(j=0;j<LCD_PIX_Y;j++)
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
void Display_Dot(void)	
{  

  Set_Display_Area(x,x,y,y);
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
void Display_HLine(uint16  width)
{

  Set_Display_Area(x,x+width-1,y,y);
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
void Display_VLine(uint16  height)
{

  Set_Display_Area(x,x,y,y+height-1);
  Send_Ssd1963_Command(0x2C);
  SSD1963_CS=0;
  while(height--)
  {
	WriteData16(fcolor);
  }
  SSD1963_CS=1;
} 


//显示一个方框
void DisplayFrame(void)
{
	
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
void DisplayBlock(void)
{
	uint16 xdata width,height;
	uint16 i,j;

	if(end_x >= LCD_PIX_X || end_y >=LCD_PIX_Y) return;
	if(start_x>=end_x || start_y>= end_y) return;

	Set_Display_Area(start_x,end_x,start_y,end_y);
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
void Display_ASCII_Mul(void)
{

	switch(size)
	{	   	
	   case 1: //8*16
			lcd_fangda_dis_text(8,16);	   
		   break;
	   case 2: //12*24
	   default:
		   lcd_fangda_dis_text(16,24);
		   break;
	   case 3: //16*32
		   lcd_fangda_dis_text(16,32);
		   break;

	   case A128x80Aria:
	   		Display_ASCII( &(FontDots[*pDispData-0x30][0]));
	   		break;

	}

}




 /*--------------------------------------------------------------------------------------------------------------------------------------
 函数原型: void Display_ASCII(uint16 xdata x,uint16 xdata y,uint8 xdata *p,uint32 xdata fcolor,uint32 xdata bcolor)
 参数说明: x:字符的X坐标,y:字符的Y坐标,frcolor:字符的颜色，brcolor:字符的背景颜色
 返 回 值: 无
 函数功能: 在屏幕上显示字符
 ----------------------------------------------------------------------------------------------------------------------------------------*/
void Display_ASCII(uint8 code *p)   //　128x80的字体存储在ROM中
{

	uint16 xdata i;
	uint8 xdata j;

	switch(size)
	{

	   case A128x80Aria:
	   	   Set_Display_Area(x,x+79,y,y+127);
		   i=1280;	   		
	   		break;

	   default:
	   		return;
			
	}

	Send_Ssd1963_Command(0x2C);

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



/*--------------------------------------------------------------------------------------------------------------------------------------
 函数原型: void Display_ASCII_2(uint8 *p)
 参数说明: x:字符的X坐标,y:字符的Y坐标,frcolor:字符的颜色，brcolor:字符的背景颜色
 返 回 值: 无
 函数功能: 在屏幕上显示字符
 ----------------------------------------------------------------------------------------------------------------------------------------*/
void Display_ASCII_2(uint8 *p)
{

	uint16 xdata i;
	uint8 xdata j;

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

	   default:
	   	   return;
			
	}

	Send_Ssd1963_Command(0x2C);

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
函数原型: void Display_Chinese(uint x,uint y,uchar *p,ulong fcolor,ulong bcolor)
参数说明: x:汉字的X坐标,y:汉字的Y坐标,fcolor:汉字的颜色，bcolor：汉字的背景颜色
返 回 值: 无
函数功能: 在屏幕上显示汉字
----------------------------------------------------------------------------*/
void Display_Chinese_Mul(void)
{
	switch(current_size)
	{
		
	case 1: //16*16
	case 3:	//48*48
	case 5: //80*80
	default:
		lcd_fangda_dis_text(16,16);
	   	break; 
		
	case 2:	//32*32
	case 4: //64*64
	case 6:	//96*96
	case 7:	//128*128
	case 8: //160*160
		lcd_fangda_dis_text(32,32);
	   	break;
		   
	}

 }




//写前所写区域处理
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
		else return 1;

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

void lcd_fangda_dis_text(uint8 type_x_wide, uint8 type_y_height)
{

	unsigned char  i,j,k,m,n;


	Set_Display_Area(x,x+type_x_wide*mul_col-1,y,y+type_y_height*mul_row-1);
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

				x += dot_number;
				
				pDispData=pDispData+2;	//一个汉字内码占两个字节

			}
			else
			{//低字节不符合汉字条件
				break;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e))	//符合ASCII字符条件
		{
			Get_GT23_Data(pDispData,size);
						
			//先进行写空间处理
			if(WriteAreaHandlerMul(0)==0)   break;	//超出区域退出
			
			Display_ASCII_Mul();

			x += dot_number/2;

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

		x += dot_number;
		
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

					x += dot_number;
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
				break;
			}
		}
		else if((*pDispData>=0x20)&&(*pDispData<=0x7e)) //符合ASCII字符条件
		{
			Get_GT23_Data(pDispData,size);
			//先进行写空间处理
			if(WriteAreaHandlerMul(0)==0)   break;	//超出区域退出
			
			Display_ASCII_Mul();

			x += dot_number/2;
			pDispData++;
			Length--;
		}
		else	//其它不相关数据
		{
			pDispData++;
			Length--;		
		}		

	}
	WDT_CONTR = 0x3e;
	
}
				

uchar * Digit2ASCII(uint8 byChar)		
{
	ASCIIBuff[0]=byChar/100;
	ASCIIBuff[1]=(byChar%100)/10;
	ASCIIBuff[2]=(byChar%100)%10;
	ASCIIBuff[3]=0;

	if(ASCIIBuff[0])
	{
		ASCIIBuff[0] += 0X30;
		ASCIIBuff[1] += 0X30;
		ASCIIBuff[2] += 0x30;
		return(ASCIIBuff);
	
	}
	else 
	{
		if(ASCIIBuff[1])
		{
			ASCIIBuff[1] += 0X30;
			ASCIIBuff[2] += 0x30;
			return(&(ASCIIBuff[1]));		
		}
		else 
		{
			ASCIIBuff[2] += 0x30;
			return(&(ASCIIBuff[2]));

		}
	}
			
}



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



void ShowAddBedCallFace(uint8 byBedAddr)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"加");
	strcat(BedDataBuff,Digit2ASCII(byBedAddr));
	strcat(BedDataBuff, "床呼叫");
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);

}



void DisSendPinBugFace(void)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"本机故障!");
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
	ShowItem(MESSAGE2);
//	BL_SW=1;	//背关打开
	BL_ON();
	
}


void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCallType)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
    byRoomAddr &= 0x7f;
	switch(byCallType)
	{
		case CMD_INFUSION_CALL:								//护理呼叫
			strcat(BedDataBuff,Digit2ASCII(byRoomAddr));		
			strcat(BedDataBuff,"房");
			strcat(BedDataBuff,Digit2ASCII(byBedAddr));
			strcat(BedDataBuff,"床呼叫");
			break;

		case CMD_SERVICE_CALL:								//输液完毕呼叫
			strcat(BedDataBuff,Digit2ASCII(byRoomAddr));		
			strcat(BedDataBuff,"房");
			strcat(BedDataBuff,Digit2ASCII(byBedAddr));
			strcat(BedDataBuff,"输液完毕");
			break;
			
		case CMD_EMERGENCY_CALL:   							//紧急呼叫
			strcat(BedDataBuff,Digit2ASCII(byRoomAddr));		
			strcat(BedDataBuff,"房卫生间呼叫");					
			break;

		default:
			break;
	}		
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
	
	ShowItem(MESSAGE2);
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
void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr)
{ 
	uint8 i;
	byRoomAddr=byRoomAddr;
	byBedAddr=byBedAddr;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"本机正在通话中");
	
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);

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
void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf)
{ 
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;

	if(bSelf)
	{		
		strcat(BedDataBuff,"本机正在广播中");
		

	}
	else
	{
		if(byRoomAddr==YHFJ_ROOM_ADDR)
		{
			strcat(BedDataBuff,Digit2ASCII(byBedAddr));		
			strcat(BedDataBuff,"号医护分机正在广播");
					
		}
		else
		{	
			strcat(BedDataBuff,Digit2ASCII(byRoomAddr));		
			strcat(BedDataBuff,"房");
			strcat(BedDataBuff,Digit2ASCII(byBedAddr));		
			strcat(BedDataBuff,"床正在广播");			
		}
	}
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
	ShowItem(MESSAGE2);	
 }



void ShowStartInfusionCall(void)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"开始护理呼叫");
	
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);
}


void ShowStartServiceCall(void)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"开始服务呼叫");
	
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);

}


void ShowStartEmergencyCall(void)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"开始紧急呼叫");
	
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);
}

void ShowStartHelpCall(void)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,"开始求援呼叫");
	
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);
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


void ShowCallSuccess(void)
{
	
	pDispData = "呼叫成功,请等待";
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);
}


void ShowItem(uint8 ItemCode)
{

	uint8 xdata position=0;
	uint8 num=0;
	uint8 i=0;

	page =0;
	
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
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1) DisplayStringLen();
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
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1) DisplayStringLen();
					
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
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1) DisplayStringLen();
				break;
				
			case OX_TOTAL:
				//DisplayBlock();//把原有的消息清空	
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
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1) DisplayStringLen();
				break;
				
			case BED_NUMBER:
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
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1)  DisplayStringLen();		
				break;	
	
			case OX_SUPPLY:
				pDispData =NULL;
				Length = 0;					
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1)  DisplayStringLen();
				break;
				
			case ILLNESS_LEVEL:
			case MEDICAL_LEVEL:
			case NURSING_LEVEL:
			case DIETARY_LEVEL:
				pDispData = stLocalControl.stItemData.pData+2;
				Length = stLocalControl.stItemData.DataLength-2;
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode) ==1) DisplayStringLen();	
				break;	
				
			case MESSAGE2:
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1) DisplayStringLen();					
				break;
				
			default:				
				pDispData = stLocalControl.stItemData.pData;
				Length = stLocalControl.stItemData.DataLength;
				if(get_size_mul_start_xy(ItemCode,stLocalControl.stItemForm.DispMode)==1) DisplayStringLen();	
				break;
				
		}
	}
	Set_VisualPage();
	stLocalControl.byDisplayFace = BED_INFO_FACE;
}


void ShowPatientBaseInfo(void)
{	
	uint8 xdata position=0;
	uint8 num=0;
	uint8 i=0;
	uint16 uiDataLen;
	
	page=0;
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

		if(stLocalControl.stItemData.ItemCode== BED_NUMBER)		//不显示床号信息项
		{
			position +=stLocalControl.stItemData.DataLength;
			uiDataLen -= stLocalControl.stItemData.DataLength;	
			continue;
		}
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
						//Send_Data(stLocalControl.stItemData.pData,stLocalControl.stItemData.DataLength);
						//Send_Data_Byte('\n');


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
						if(get_size_mul_start_xy(stLocalControl.stItemForm.ItemCode,stLocalControl.stItemForm.DispMode)==1)  DisplayStringLen();
					}
				}
			}
			position +=stLocalControl.stItemData.DataLength;
			uiDataLen -= stLocalControl.stItemData.DataLength;			
		}  //有数据的末尾

	}

	Set_VisualPage();
	stLocalControl.byDisplayFace= BED_INFO_FACE;
}



void ShowBedFace(void)
{	
	uint8 i,j;
	uint8 crc;
	uint8 *p;
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
		if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2))
		{//备份区正确
			while(1)
			{
				SectorDataCopy(BED_FACE_FORM_LENGTH_ADDR,BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+3);	//拷贝至主格式区				
				if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2)==1)  break;					
			}
			ShowBedFaceForm();
		}
		else 		//主格式区不正确，备份格式区也不正确。
		{//使之显示默认格式
			stLocalControl.uiBedFaceFormLen = 14*sizeof(STItemForm);
			Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);
			Write_Cont(BED_FACE_FORM_START_ADDR,&(default_bed_form[0].ItemCode),stLocalControl.uiBedFaceFormLen);

			crc= stLocalControl.uiBedFaceFormLen>>8;		//校验和对长度与数据求和
			crc+= stLocalControl.uiBedFaceFormLen;
			p= &(default_bed_form[0].ItemCode);
			for(i=0;i<stLocalControl.uiBedFaceFormLen;i++) 
			{
				crc += *p;
				p++;
			}
			Write_Cont(BED_FACE_FORM_START_ADDR+stLocalControl.uiBedFaceFormLen,&crc,1);
			SectorDataCopy(BED_FACE_FORM_BAK,BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+3);	
			ShowBedFaceForm();
		}
	}
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
	{//主数据区不正确
		if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2))
		{//备份区正确
			while(1)
			{
				SectorDataCopy(BED_FACE_DATA_LENGTH_ADDR,BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+3);	//拷贝至主数据区				
				if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)  break;					
			}
			ShowPatientBaseInfo();
		}
		else 		//主数据区不正确，备份数据区也不正确。
		{

			stLocalControl.uiBedFaceDataLen =0;
			Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&(stLocalControl.uiBedFaceDataLen),2);
			Write_Cont(BED_FACE_DATA_BAK,&(stLocalControl.uiBedFaceDataLen),2);


/*			j=0;
			for(i=0;i<13;i++)
			{
				GT23_Rec_Data[j++] = default_bed_data[i].ItemCode;
				if(default_bed_data[i].ItemCode==2 ||default_bed_data[i].ItemCode==3||default_bed_data[i].ItemCode==4||default_bed_data[i].ItemCode==5)
				{
					GT23_Rec_Data[j++] = default_bed_data[i].DataLength+2;
					GT23_Rec_Data[j++] = default_bed_form[i].fcolor;
					GT23_Rec_Data[j++] = default_bed_form[i].bcolor;
				}
				else GT23_Rec_Data[j++] = default_bed_data[i].DataLength;
				
				p= default_bed_data[i].pData;
				while(*p)
				{
					GT23_Rec_Data[j++] = *p;
					p++;
				}
			}

			stLocalControl.uiBedFaceDataLen =j;			
			crc=j;
			Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&(stLocalControl.uiBedFaceDataLen),2);
			for(i=0;i<stLocalControl.uiBedFaceDataLen;i++) 
			{
				crc += GT23_Rec_Data[i];
			}
			Write_Cont(BED_FACE_DATA_START_ADDR,GT23_Rec_Data,stLocalControl.uiBedFaceDataLen);
			Write_Cont(BED_FACE_DATA_START_ADDR+stLocalControl.uiBedFaceDataLen,&crc,1);

			SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);
			
			ShowPatientBaseInfo();*/
		}
	}
	ShowItem(OX_START);
	ShowItem(OX_END);
	ShowItem(OX_SUBTOTAL);	
	ShowItem(OX_TOTAL);

	Set_VisualPage();
	stLocalControl.byDisplayFace= BED_INFO_FACE;

}



void ShowPowerUpFace(void)	//写在第0页
{
	page=0;
	fcolor=BLUE;
	Lcd_Clear();

	//测试用
/*	x=0;y=0;
	size =A128x80Aria;
	mul_col=1;
	mul_row=1;	
	pDispData="129";
	DisplayString();*/


	current_size = 4;	//64*64
	current_vert_spac = VERT_SPAC_64;
	get_dot_number();
	get_basesize_mul();
	
	x=(LCD_PIX_X-dot_number*6)/2;		
	y=(LCD_PIX_Y-(dot_number+current_vert_spac)*4)/2;		

	fcolor=WHITE;
	bcolor=BLUE;
	pDispData="系统初始化...";
	start_x =0;
	start_y =0;
	end_x = 799;
	end_y =479;		
	DisplayString();
	

	y += (dot_number+current_vert_spac);			//106
	x=(LCD_PIX_X-dot_number*4)/2;		//176
	pDispData="版本V";
	DisplayString();

	stLocalControl.stEepromCfgData.byVersionHi =0x04;
	stLocalControl.stEepromCfgData.byVersionLo =0x06;
	
	DisplayOneChar(stLocalControl.stEepromCfgData.byVersionHi);
	pDispData=".";
    DisplayString();
	DisplayOneChar(stLocalControl.stEepromCfgData.byVersionLo);
	
	

/*	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*10)/2;
	pDispData="湖南一特电子医用工程";
	DisplayString();	


	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*6)/2;
	pDispData="股份有限公司";
	DisplayString();*/


	y += (dot_number+current_vert_spac);
	x =(LCD_PIX_X-dot_number*10)/2;
	pDispData="病房护理系统床头分机";
	DisplayString();	

	
	Set_VisualPage();
	stLocalControl.byDisplayFace=POWER_UP_FACE;
}


void ShowStartNumSet(void)
{
	uint8 i;
	for(i=0;i<SEGMENT_DATA_LEN;i++)   BedDataBuff[i]=0;
	
	strcat(BedDataBuff,Digit2ASCII(stLocalControl.stBusDealFreq.bySndSecAddr));
	strcat(BedDataBuff,"区");
	strcat(BedDataBuff,Digit2ASCII(stLocalControl.stBusDealFreq.bySndRoomAddr));
	strcat(BedDataBuff,"房");
	strcat(BedDataBuff,Digit2ASCII(stLocalControl.stBusDealFreq.bySndBedAddr));
	strcat(BedDataBuff,"床开始编号");	
	pDispData = BedDataBuff;
	Length=strlen(pDispData);
		
	ShowItem(MESSAGE2);
}


void ShowNumSetOk(void)
{

	
	pDispData="编号成功";
	Length=strlen(pDispData);
	ShowItem(MESSAGE2);
	
	ShowItem(BED_NUMBER);		//写在第0页,过5秒再返回第0页
}


void ShowStartRfNumSet(void)			
{


	pDispData="输液报警器开始对码";	
	Length=strlen(pDispData);
	ShowItem(MESSAGE2);	
}

void ShowRfNumSetOk(void)			
{

	
	pDispData="输液报警器对码成功";	
	Length= strlen(pDispData);
	ShowItem(MESSAGE2);	
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
		case NURSING_LEVEL:
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

//由一块区域的起始点坐标、结束点坐标、项目代码、要显示的字符,字符个数=======>
//得到要显示字符的基本尺寸、
//字符的放大倍数、
//显示字符块的区域(考虑到竖向标题)
//显示字符的起始坐标
//
int8 get_size_mul_start_xy(uint8 item_code,uint8 disp_mode)
{
	uint16 area_width,area_height;
	uint8 RowCharNum;
	uint8 ColumnCharNum;	
	if(end_x >= LCD_PIX_X || end_y >=LCD_PIX_Y) return 0;
	if(start_x>=end_x || start_y>= end_y) return 0;
	area_width = end_x-start_x+1;
	area_height = end_y-start_y+1;

	if(item_code == BED_NUMBER)
	{
		if((area_width>= 80*Length) &&(area_height>=128))
		{
			size =A128x80Aria;
			mul_col=1;
			mul_row=1;
			dot_number =160;
			x = start_x + (area_width-80*Length)/2;
			y = start_y + (area_height-128)/2;
			return 1;
			
		}
		else goto  get_size_mul_start_xy_1;
	}
	else
	{
		if(disp_mode & TITLE_WHETHER_DIS)
		{//显示标题,只有一种方式:横向同行且带帽号
			byUsart0SndBuf[0]=0;
			//标题横向显示
				
			strcat(byUsart0SndBuf,GetTitle(item_code));
			Length += GetTitleLength(item_code);
			strcat(byUsart0SndBuf,":");
			Length++;
			strcat(byUsart0SndBuf,pDispData);
			pDispData = byUsart0SndBuf;
		}
		
	}
	

get_size_mul_start_xy_1:

	if(area_height-VERT_SPAC_160 >=160) 		{current_size = DOT160X160;current_vert_spac = VERT_SPAC_160;}
	else if(area_height-VERT_SPAC_128 >=128)	{current_size = DOT128X128;current_vert_spac = VERT_SPAC_128;}	
	else if(area_height-VERT_SPAC_96 >=96)		{current_size = DOT96X96;current_vert_spac = VERT_SPAC_96;}
	else if(area_height-VERT_SPAC_80 >=80)		{current_size = DOT80X80;current_vert_spac = VERT_SPAC_80;}
	else if(area_height-VERT_SPAC_64 >=64)		{current_size = DOT64X64;current_vert_spac = VERT_SPAC_64;}
	else if(area_height-VERT_SPAC_48 >=48)		{current_size = DOT48X48;current_vert_spac = VERT_SPAC_48;}
	else if(area_height-VERT_SPAC_32 >=32)		{current_size = DOT32X32;current_vert_spac = VERT_SPAC_32;}
	else if(area_height-VERT_SPAC_16 >=16)		{current_size = DOT16X16;current_vert_spac = VERT_SPAC_16;}
	else
	{//高度太小，连最小尺寸的字符都显示不下，直接跳过
		return 0;
	}


	do
	{
		get_dot_number();

		RowCharNum= area_height/(dot_number+current_vert_spac);
		ColumnCharNum= area_width/(dot_number/2);
		if(Length<=RowCharNum*ColumnCharNum)
		{//个数符合要求
	
			if(RowCharNum==1)
			{//只有一行，行列都居中显示
				x= start_x+(area_width-Length*(dot_number/2))/2;
			
			}

			else x=start_x;	// 横向顶格显示
			
			y= start_y +(area_height-(dot_number+current_vert_spac)*(RowCharNum-1)-dot_number)/2;	//竖向居中显示

			get_basesize_mul();  //显示基本尺寸与放大倍数
			break;
			
		}
		else current_size--;
	}while(current_size);
	if(current_size ==0) return 0;
	else return 1;
}


//显示床头分机界面格式
void ShowBedFaceForm(void)
{
	uint16 position=0;
	uint16 uiFormLen;
	uint8 i,j;
	
	uint16 area_height,area_width;
	uint8 RowCharNum,ColumnCharNum;	


	page=0;
	fcolor = WHITE;//BLUE4_VALUE;
	Lcd_Clear();

//	Read_Cont((uint8*)&(stLocalControl.uiBedFaceFormLen),BED_FACE_FORM_LENGTH_START_ADDR,2);	//读格式数据总个数
	uiFormLen= stLocalControl.uiBedFaceFormLen;

	if(((stLocalControl.uiBedFaceFormLen/sizeof(STItemForm))>=MAX_ITEM_CODE) ||(stLocalControl.uiBedFaceFormLen ==0xffff)||(stLocalControl.uiBedFaceFormLen ==0))
	{//项目超限
		stLocalControl.uiBedFaceFormLen=0;
		Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);
		Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);

		for(i=0;i<14;i++)
		{
			start_x = default_bed_form[i].StartPoint.x;
			start_y = default_bed_form[i].StartPoint.y;
			end_x 	= default_bed_form[i].EndPoint.x;
			end_y 	= default_bed_form[i].EndPoint.y;
			
			bcolor 	= GetColor(default_bed_form[i].bcolor);
			fcolor  = GetColor(default_bed_form[i].fcolor);
			DisplayBlock();

			start_x = default_bed_form[i].EndPoint.x;
			start_y = default_bed_form[i].StartPoint.y+4;
			end_x 	= start_x+4;
			end_y 	= default_bed_form[i].EndPoint.y+4;
			bcolor  = BLACK;
			DisplayBlock();	

			start_x = default_bed_form[i].StartPoint.x+4;
			start_y = default_bed_form[i].EndPoint.y;
			end_x 	= default_bed_form[i].EndPoint.x;
			end_y 	= start_y+4;
			bcolor  = BLACK;
			DisplayBlock();	

			
			start_x = default_bed_form[i].StartPoint.x;
			start_y = default_bed_form[i].StartPoint.y;
			end_x 	= default_bed_form[i].EndPoint.x;
			end_y 	= default_bed_form[i].EndPoint.y;	




			for(j=0;j<14;j++)
			{
				if(default_bed_data[j].ItemCode ==default_bed_form[i].ItemCode) break;

			}
			if(j<14)
			{//找到缺省数据
				if(default_bed_form[i].ItemCode == BED_NUMBER)
				{
					ASCIIBuff[0]=stLocalControl.stEepromCfgData.bySelfBedAddr/100+0x30;
					ASCIIBuff[4]= stLocalControl.stEepromCfgData.bySelfBedAddr%100;
					ASCIIBuff[1]=ASCIIBuff[4]/10+0x30;
					ASCIIBuff[2]=ASCIIBuff[4]%10+0x30;
					ASCIIBuff[3]=0;
					if(ASCIIBuff[0]!=0x30) 
					{
						pDispData = ASCIIBuff;
						Length =3;
					}
					else
					{
						pDispData = &(ASCIIBuff[1]);
						Length =2;					
					}
					
				}
				else
				{
					pDispData = default_bed_data[j].pData;
					Length = default_bed_data[j].DataLength;
				}


				
				bcolor 	= GetColor(default_bed_form[i].bcolor);
				fcolor  = GetColor(default_bed_form[i].fcolor);	

				if(get_size_mul_start_xy(default_bed_form[i].ItemCode,default_bed_form[i].DispMode)==1)  DisplayStringLen();
										
			}
			WDT_CONTR = 0x3e;
		}		
		return;
	}

	
	for(;uiFormLen>=sizeof(STItemForm);uiFormLen-=sizeof(STItemForm),position+=sizeof(STItemForm))
	{
		Read_Cont(&(stLocalControl.stItemForm.ItemCode),BED_FACE_FORM_START_ADDR+position,sizeof(STItemForm));
		start_x = stLocalControl.stItemForm.StartPoint.x;
		start_y = stLocalControl.stItemForm.StartPoint.y;
		end_x = stLocalControl.stItemForm.EndPoint.x;
		end_y = stLocalControl.stItemForm.EndPoint.y;

		bcolor=GetColor(stLocalControl.stItemForm.bcolor);
		//bcolor = BLUE3_VALUE;
		DisplayBlock();

		start_x = stLocalControl.stItemForm.EndPoint.x;
		start_y = stLocalControl.stItemForm.StartPoint.y+4;
		end_x	= start_x+4;
		end_y	= stLocalControl.stItemForm.EndPoint.y+4;
		bcolor	= BLACK;
		DisplayBlock(); 
		
		start_x = stLocalControl.stItemForm.StartPoint.x+4;
		start_y = stLocalControl.stItemForm.EndPoint.y;
		end_x	= stLocalControl.stItemForm.EndPoint.x;
		end_y	= start_y+4;
		bcolor	= BLACK;
		DisplayBlock(); 
		
		
		start_x = stLocalControl.stItemForm.StartPoint.x;
		start_y = stLocalControl.stItemForm.StartPoint.y;
		end_x	= stLocalControl.stItemForm.EndPoint.x;
		end_y	= stLocalControl.stItemForm.EndPoint.y;

		fcolor=GetColor(stLocalControl.stItemForm.fcolor);
		bcolor=GetColor(stLocalControl.stItemForm.bcolor);
		//bcolor = BLUE3_VALUE;
		
		pDispData=GetTitle(stLocalControl.stItemForm.ItemCode);
		Length = GetTitleLength(stLocalControl.stItemForm.ItemCode);	

		if((pDispData ==NULL)|| (Length==0)) continue;

		
		area_height = end_y-start_y +1;
		area_width  = end_x-start_x +1;

		
		if(area_height-VERT_SPAC_160 >=160) 		{current_size = DOT160X160;current_vert_spac = VERT_SPAC_160;}
		else if(area_height-VERT_SPAC_128 >=128)	{current_size = DOT128X128;current_vert_spac = VERT_SPAC_128;}	
		else if(area_height-VERT_SPAC_96 >=96)		{current_size = DOT96X96;current_vert_spac = VERT_SPAC_96;}
		else if(area_height-VERT_SPAC_80 >=80)		{current_size = DOT80X80;current_vert_spac = VERT_SPAC_80;}
		else if(area_height-VERT_SPAC_64 >=64)		{current_size = DOT64X64;current_vert_spac = VERT_SPAC_64;}
		else if(area_height-VERT_SPAC_48 >=48)		{current_size = DOT48X48;current_vert_spac = VERT_SPAC_48;}
		else if(area_height-VERT_SPAC_32 >=32)		{current_size = DOT32X32;current_vert_spac = VERT_SPAC_32;}
		else if(area_height-VERT_SPAC_16 >=16)		{current_size = DOT16X16;current_vert_spac = VERT_SPAC_16;}
		else
		{//高度太小，连最小尺寸的字符都显示不下，直接跳过
			continue;
		}
		
		
		do
		{
			get_dot_number();
		
			RowCharNum= area_height/(dot_number+current_vert_spac);
			ColumnCharNum= area_width/(dot_number/2);
			if(Length<=RowCharNum*ColumnCharNum)
			{//个数符合要求

				if(RowCharNum==1)
				{//只有一行，行列都居中显示
					x= start_x+(area_width-Length*(dot_number/2))/2;
				
				}
		
				else x=start_x; // 横向顶格显示
				
				y= start_y +(area_height-(dot_number+current_vert_spac)*(RowCharNum-1)-dot_number)/2;	//竖向居中显示
		
				get_basesize_mul();  //显示基本尺寸与放大倍数
				break;
				
			}
			else current_size--;
		}while(current_size);
		DisplayStringLen();

	}
}
