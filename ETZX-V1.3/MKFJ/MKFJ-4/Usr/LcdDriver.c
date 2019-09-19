/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:LcdDriver.c
*文件描述    		:液晶驱动程序
*创建人     		:熊坚强
*创建日期   		:2008-9-22
*版本号       		:V1.1
*注释	     		:
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_LCD_
#include "config.h"
sbit    SSD1963_CS     = P1^0;
sbit    SSD1963_RST    = P1^1;
sbit    SSD1963_CD     = P5^3;
sbit    SSD1963_TE     = P4^2;
sbit    SSD1963_WR     = P3^6;
sbit    SSD1963_RD     = P3^7;
#define SSD1963_DATAL   P0
#define SSD1963_DATAH   P2
uint16 xdata fontbuf;
extern bit   Page_Flag;
extern uint8 xdata  SerchRoom;
extern uint8 xdata  GT23_Rec_Data[128];
extern uint8 xdata  Page_Color[5];
extern uint8 xdata  Modify_Room_Number;
extern uint8 xdata  Modify_SEC_Name;
extern STLocalControl 	xdata stLocalControl;
bit    Broadcasting_Flag;
bit    First_Serchroom_Flag;
extern bit  BusLowRenewOK_Flag;
extern void Delayms(uint16 ms);

extern bit bLcdOn;



const uint8 byAsciiTable[] = 
{
  0x00,0x30,0x00,0x31,0x00,0x32,0x00,0x33,0x00,0x34,0x00,0x35,0x00,0x36,0x00,0x37,0x00,0x38,0x00,0x39
};
const uint8 byWordTable[] = 
{
  0xB7,0xBF,0xB4,0xB2,0xBA,0xF4,0xBD,0xD0,0xCE,0xC0,0xC9,0xFA,0xBC,0xE4,0xCA,0xE4,0xD2,0xBA,0xCD,0xEA,0xB1,0xCF,0xBC,0xD3
}; 
const uint8  byLandingWord[] = 
{
  0xCF,0xB5,0xCD,0xB3,0xB3,0xF5,0xCA,0xBC,0xBB,0xAF
};

const uint16 ColorIndex[9]={0x0000,0xF800,0x07E0,0x0007,0x001F,0xFFE0,0xF81F,0x07FF,0xFFFF};

/*#define	 BLACK   0x0000	   //0
#define	 RED     0xF800	   //1
#define	 GREEN   0x07E0	   //2
#define	 BLUE1   0x0007	   //3
#define	 BLUE    0x001F	   //4
#define	 YELLOW  0xFFE0	   //5
#define	 PURPLE  0xF81F	   //6
#define	 CYAN    0x07FF	   //7
#define	 WHITE   0xFFFF	   //8*/


void Send_Ssd1963_Command(uint8 ssd1963_command)
{
   SSD1963_CD=0; 
   //SSD1963_CS=0;
   SSD1963_WR=0;
   SSD1963_DATAH=0x00;
   SSD1963_DATAL=ssd1963_command;
   //SSD1963_CS=1;
   SSD1963_WR=1;
   SSD1963_CD=1;
}
void Send_Ssd1963_Data(uint16 ssd1963_data)
{
   //SSD1963_CD=1; 
   //SSD1963_CS=0;
   SSD1963_WR=0;
   SSD1963_DATAH=ssd1963_data/256;
   SSD1963_DATAL=ssd1963_data%256;
   //SSD1963_CS=1;
   SSD1963_WR=1;
   //SSD1963_CD=0;
}
void Set_Display_Area(uint16 start_x,uint16 end_x,uint16 start_y,uint16 end_y)
{
  uint8 xdata shigh_x1,slow_x1,shigh_x2,slow_x2;
  uint8 xdata shigh_y1,slow_y1,shigh_y2,slow_y2;
  shigh_x1=start_x>>8;
  slow_x1=start_x&0xff;
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
void SetScrollArea(uint16 top, uint16 scroll, uint16 bottom)
{
  Send_Ssd1963_Command(0x33);
  Send_Ssd1963_Data(top>>8);
  Send_Ssd1963_Data(top);
  Send_Ssd1963_Data(scroll>>8);
  Send_Ssd1963_Data(scroll);
  Send_Ssd1963_Data(bottom>>8);
  Send_Ssd1963_Data(bottom);
}
void SetScrollStart(uint16 line)
{
  Send_Ssd1963_Command(0x37);
  Send_Ssd1963_Data(line>>8);
  Send_Ssd1963_Data(line); 
}
void Set_VisualPage(uint8 VisualPage)
{
  SetScrollStart(VisualPage*272);
}
void Lcd_Fill(uint16 start_x,uint16 start_y, uint16 width, uint16 height,uint16 color)
{
  uint16 xdata  i,j;
  Set_Display_Area(start_x,start_x+width-1,start_y,start_y+height-1);
  Send_Ssd1963_Command(0x2C);
  SSD1963_DATAH=color/256;
  SSD1963_DATAL=color%256;
   for(i=0;i<height;i++)
    {
     for(j=0;j<width;j++)
     {
      //Send_Ssd1963_Data(color);
	  	SSD1963_WR=0;
   		SSD1963_WR=1;
	 }
   }
}
void Lcd_Clear(uint16 color,uint8 VisualPage)
{
  uint16 xdata k,j;
  Set_Display_Area(0x00,0x1DF,VisualPage*272,0x10F+VisualPage*272);
  Send_Ssd1963_Command(0x2C);
  SSD1963_DATAH=color/256;
  SSD1963_DATAL=color%256;
  for(k=0;k<480;k++)
    {
	  for(j=0;j<272;j++)
	  {
	  	SSD1963_WR=0;
   		SSD1963_WR=1;
	  }
      //Send_Ssd1963_Data(color);
    }
  WDT_CONTR = 0x3d;
}
void lcd_fangda_dis_text(uint16 x,uint16 y, uint8 type_x_wide, uint8 type_y_height,uint8 mul_col,uint8 mul_row,uint16 fcolor,uint16 bcolor,uint8 *p)
{
    uint8 xdata i,j,k,m,n;
	Set_Display_Area(x,x+type_x_wide*mul_col-1,y,y+type_y_height*mul_row-1);

	
	Send_Ssd1963_Command(0x2C);
	for (i=0;i<type_y_height;i++) 
	{
	  for(n=0;n<mul_row;n++)	
		{
    	for(j=0;j<type_x_wide/8;j++)	
		 {
		 for(k=0;k<8;k++)	
		  {
			for(m=0;m<mul_col;m++)	
			{
			  if ((p[i*(type_x_wide/8)+j]>>(7-k))&0x1)
			   {
				  fontbuf=fcolor;						
				}

			 else
			  {
				 fontbuf=bcolor;								
			  }
			  Send_Ssd1963_Data(fontbuf);
			}
		}
	   }
	}
  }
  WDT_CONTR =0x3d;
}
void ShowWord(uint16 xdata byRow, uint16 xdata byCol, uint8 xdata byWord,uint16 xdata fcolor,uint16 xdata bcolor,uint8 xdata size)
{
	uint8 xdata uiHPosition,uiLPosition;
	uiHPosition = byWordTable[byWord * 2];
	uiLPosition	= byWordTable[byWord *2 +1];
	Get_GT23_Data(uiHPosition,uiLPosition,3);
	lcd_fangda_dis_text(byRow,byCol,32,32,size,size,fcolor,bcolor,GT23_Rec_Data);
}
void ShowAscii(uint16 xdata byRow, uint16 xdata byCol, uint8 xdata byAscii,uint16 xdata fcolor,uint16 xdata bcolor,uint8 xdata size)
{
	uint8 xdata uiHPosition,uiLPosition;
	uiHPosition = byAsciiTable[byAscii * 2];
	uiLPosition	= byAsciiTable[byAscii *2 +1];
	Get_GT23_Data(uiHPosition,uiLPosition,3);	
	lcd_fangda_dis_text(byRow,byCol,16,32,size,size,fcolor,bcolor,GT23_Rec_Data);	
} 
/**********************************************************
*函数名称			:ShowCallFace	
*函数描述        	:显示呼叫界面
*输入参数   		:byRoomAddr:房地址,byBedAddr:床地址,byCmd:呼叫命令					 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	: 呼叫信息放在第3屏显示
**********************************************************/ 
void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCmd)
{
    uint16 xdata fcolor,bcolor;
	fcolor=WHITE;//字体颜色
	bcolor=BLUE; //字体背景色,也是清屏色
	Lcd_Fill(0,644,480,80,bcolor);
	switch(byCmd)
	{
		case CMD_INFUSION_CALL:								      //服务呼叫命令
			 byBedAddr &= 0x7F;
			 byBedAddr %= 100;
		    //---------------------------------------------------------------
		    //---------------------------------------------------------------
		    //加床分机呼叫
			if(byRoomAddr==250)
			{
			   ShowWord(70,648,11,fcolor,bcolor,2);				    //加
			   ShowAscii(144,653,byBedAddr/10,fcolor,bcolor,2);
			   ShowAscii(176,653,byBedAddr%10,fcolor,bcolor,2);
			   ShowWord(218,648,1,fcolor,bcolor,2);				   //床
			   ShowWord(282,648,2,fcolor,bcolor,2);				   //呼
			   ShowWord(346,648,3,fcolor,bcolor,2);				   //叫
			}
		   //---------------------------------------------------------------
		   //---------------------------------------------------------------
		   //非加床分机呼叫
			else
			{
			   byRoomAddr &= 0x7F;
			   byRoomAddr %= 100;
			   ShowAscii(23,653,byRoomAddr/10,fcolor,bcolor,2);
			   ShowAscii(55,653,byRoomAddr%10,fcolor,bcolor,2);
			   ShowWord(97,648,0,fcolor,bcolor,2);				   //房
			   ShowAscii(171,653,byBedAddr/10,fcolor,bcolor,2);
			   ShowAscii(203,653,byBedAddr%10,fcolor,bcolor,2);
			   ShowWord(245,648,1,fcolor,bcolor,2);				   //床
			   ShowWord(319,648,2,fcolor,bcolor,2);				   //呼
			   ShowWord(393,648,3,fcolor,bcolor,2);				   //叫
			 }
			break; 
		case CMD_SERVICE_CALL:								       //输液呼叫命令
			byRoomAddr &= 0x7F;
			byRoomAddr %= 100;
			byBedAddr &= 0x7F;
			byBedAddr %= 100;
			ShowAscii(94,660,byRoomAddr/10,fcolor,bcolor,1);
			ShowAscii(110,660,byRoomAddr%10,fcolor,bcolor,1);
			ShowWord(131,656,0,fcolor,bcolor,1);				   //房
			ShowAscii(168,660,byBedAddr/10,fcolor,bcolor,1);
			ShowAscii(184,660,byBedAddr%10,fcolor,bcolor,1);
			ShowWord(205,656,1,fcolor,bcolor,1);				   //床
			ShowWord(242,656,7,fcolor,bcolor,1);				   //输
			ShowWord(279,656,8,fcolor,bcolor,1);				   //液
			ShowWord(316,656,9,fcolor,bcolor,1);                   //完
			ShowWord(353,656,10,fcolor,bcolor,1);		           //毕
			break; 
		case CMD_EMERGENCY_CALL:							       //紧急呼叫命令
			byRoomAddr &= 0x7F;
			byRoomAddr %= 100;
			byBedAddr &= 0x7F;
			byBedAddr %= 100;
			ShowAscii(16,653, byRoomAddr/10,fcolor,bcolor,2);
			ShowAscii(48,653, byRoomAddr%10,fcolor,bcolor,2);
			ShowWord(80,648,0,fcolor,bcolor,2);					   //房
			ShowWord(144,648,4,fcolor,bcolor,2);				   //卫
			ShowWord(208,648,5,fcolor,bcolor,2);		           //生
			ShowWord(272,648,6,fcolor,bcolor,2);                   //间
			ShowWord(336,648,2,fcolor,bcolor,2);                   //呼
			ShowWord(400,648,3,fcolor,bcolor,2);                   //叫
			break;
	} 
	Set_VisualPage(2); 
}
/**********************************************************
*函数名称			:void ShowVersion(uint8 xdata hversion,uint8 xdata lversion,uint8 page)
*函数描述        	:显示软件版本号
*输入参数   		:hversion--软件版本高位,lversion--软件版本低位					 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:软件版本号放在第1屏显示
**********************************************************/ 
void ShowVersion(uint8 xdata hversion,uint8 xdata lversion,uint8 page)
{
    uint16 xdata fcolor,bcolor;
    fcolor=WHITE;             //字体颜色
    bcolor=BLUE;    		  //字体背景色,也是清屏色         
	Get_GT23_Data(0xC8,0xED,3);
	if(page==0)
    lcd_fangda_dis_text(0,112,32,32,2,2,fcolor,bcolor,GT23_Rec_Data); //软
	else
	lcd_fangda_dis_text(0,112+272,32,32,2,2,fcolor,bcolor,GT23_Rec_Data); //软

	
    Get_GT23_Data(0xBC,0xFE,3);
	if(page==0)
    lcd_fangda_dis_text(64,112,32,32,2,2,fcolor,bcolor,GT23_Rec_Data); //件
	else
	lcd_fangda_dis_text(64,112+272,32,32,2,2,fcolor,bcolor,GT23_Rec_Data); //件
	
    Get_GT23_Data(0xB0,0xE6,3);
	if(page==0)
    lcd_fangda_dis_text(128,112,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);//版
	else
	lcd_fangda_dis_text(128,112+272,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);//版

	
    Get_GT23_Data(0xB1,0xBE,3);
	if(page==0)
    lcd_fangda_dis_text(192,112,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);//本
	else
    lcd_fangda_dis_text(192,112+272,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);//本

	
	Get_GT23_Data(0xBA,0xC5,3);
	if(page==0)
    lcd_fangda_dis_text(256,112,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);//号
	else
	lcd_fangda_dis_text(256,112+272,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);//号

	
	Get_GT23_Data(0x00,0x3A,3);
	if(page==0)
	lcd_fangda_dis_text(320,112,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//:
	else
	lcd_fangda_dis_text(320,112+272,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//:
	
	Get_GT23_Data(0x00,0x56,3);
	if(page==0)
	lcd_fangda_dis_text(352,116,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//V
	else
	lcd_fangda_dis_text(352,116+272,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//V

	
	Get_GT23_Data(0x00,0x30+hversion,3);
	if(page==0)
	lcd_fangda_dis_text(384,112,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//1
	else
	lcd_fangda_dis_text(384,112+272,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//1

	
	Get_GT23_Data(0x00,0x2E,3);
	if(page==0)
	lcd_fangda_dis_text(416,112,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//.
	else
	lcd_fangda_dis_text(416,112+272,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//.
	
	Get_GT23_Data(0x00,0x30+lversion,3);
	if(page==0)
	lcd_fangda_dis_text(448,112,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//1
	else
	lcd_fangda_dis_text(448,112+272,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);//1
	if(page==0)
	Set_VisualPage(0);
	else
	Set_VisualPage(1);
} 
/**********************************************************
*函数名称			:ShowLandingWord	
*函数描述        	:显示登陆汉字字符
*输入参数   		:byRow:行地址,byCol:列地址,byWord:汉字					 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void ShowLandingWord(uint16 xdata byRow, uint16 xdata byCol, uint8 xdata byWord,uint16 xdata fcolor,uint16 xdata bcolor)
{
	uint8 xdata byHPosition,byLPosition;
	byHPosition = byLandingWord[byWord * 2];
	byLPosition	= byLandingWord[byWord *2 +1];
	Get_GT23_Data(byHPosition,byLPosition,3);
	lcd_fangda_dis_text(byRow,byCol,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
}
/**********************************************************
*函数名称			:ShowLandingFace	
*函数描述        	:显示登陆界面
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:系统初始化放在第3屏显示
**********************************************************/ 
void ShowLandingFace(void)
{	
    uint16 xdata fcolor,bcolor;
	fcolor=WHITE;//字体颜色
	bcolor=BLUE; //字体背景色,也是清屏色
    ShowLandingWord(32,112+544,0,fcolor,bcolor);
	ShowLandingWord(100,112+544,1,fcolor,bcolor);
	ShowLandingWord(168,112+544,2,fcolor,bcolor);
	ShowLandingWord(236,112+544,3,fcolor,bcolor);
	ShowLandingWord(304,112+544,4,fcolor,bcolor);
	Get_GT23_Data(0x00,0x2E,3);
	lcd_fangda_dis_text(352,100+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	lcd_fangda_dis_text(384,100+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	lcd_fangda_dis_text(416,100+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Set_VisualPage(2);
}
/**********************************************************
*函数名称			:ShowBedPage	
*函数描述        	:显示多少床到多少床数据
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-23
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void ShowBedPage(void)
{	
	uint8  xdata  byData1,byData2;
	uint8  xdata i;
	uint8  *G2312_code;
	uint8  xdata  byData[8];
	uint16 xdata  fcolor,bcolor;
	uint16 xdata  DisplayAddr;
	uint8  xdata  DispalyCount;
    if(Modify_SEC_Name==0x01)
    {
      byData1=Page_Color[3];//字体颜色 
	  byData2=Page_Color[4];//字体背景色,也是清屏色
	  fcolor = ColorIndex[byData1];
	  bcolor = ColorIndex[byData2];
    }
	else
	{
	  fcolor=BLUE1;//字体颜色
	  bcolor=WHITE; //字体背景色,也是清屏色
	}
	//-----------------------------------------------------------------
	Lcd_Fill(0,178,480,94,bcolor);

	stLocalControl.uiShortType  = LCD_SEC_ADDR;	
	stLocalControl.uiShortType  = stLocalControl.uiShortType+23;  //责任医生
	for(i=0;i<8;i++)
	{
		byData[i]= FmReadByte(stLocalControl.uiShortType++);
	}
	
	G2312_code="责";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(0,193,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	
	G2312_code="任";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(16,193,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);	
	
	G2312_code="医";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(32,193,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	
	G2312_code="生";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(48,193,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);

	Get_GT23_Data(0x00,0x3a,1);
	lcd_fangda_dis_text(64,193,8,16,1,1,fcolor,bcolor,GT23_Rec_Data);


    DisplayAddr=72;	                                     //初始显示地址x
	DispalyCount=0;										 //初始显示
	for(i=0;i<8;i++)
	{
	  if(byData[DispalyCount]>=0x20&&byData[DispalyCount]<=0x7E)
	  {
	    Get_GT23_Data(0x00,byData[DispalyCount],3);
	    lcd_fangda_dis_text(DisplayAddr,193,16,32,1,1,fcolor,bcolor,GT23_Rec_Data); 
	    DisplayAddr=DisplayAddr+16;
		DispalyCount++;
	  }
	  else if(byData[DispalyCount]>=0x81&&byData[DispalyCount]<=0xFe)
	  {
	    Get_GT23_Data(byData[DispalyCount],byData[DispalyCount+1],3);
	    lcd_fangda_dis_text(DisplayAddr,193,32,32,1,1,fcolor,bcolor,GT23_Rec_Data); 
	    DisplayAddr=DisplayAddr+32;
		DispalyCount+=2;
	  }
	  if(DispalyCount>7)
	  break;
	}

	
	G2312_code="责";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(0,232,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	
	G2312_code="任";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(16,232,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);	
	
	G2312_code="护";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(32,232,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	
	G2312_code="士";
	Get_GT23_Data(*G2312_code,*(G2312_code+1),1);
	lcd_fangda_dis_text(48,232,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);

	Get_GT23_Data(0x00,0x3a,1);
	lcd_fangda_dis_text(64,232,8,16,1,1,fcolor,bcolor,GT23_Rec_Data);

	stLocalControl.uiShortType  = LCD_SEC_ADDR;	
	stLocalControl.uiShortType  = stLocalControl.uiShortType+31;  //责任护士
	for(i=0;i<8;i++)
	{
		byData[i]= FmReadByte(stLocalControl.uiShortType++);
	}


    DisplayAddr=72;	                                     //初始显示地址x
	DispalyCount=0;										 //初始显示
	for(i=0;i<8;i++)
	{
	  if(byData[DispalyCount]>=0x20&&byData[DispalyCount]<=0x7E)
	  {
	    Get_GT23_Data(0x00,byData[DispalyCount],3);
	    lcd_fangda_dis_text(DisplayAddr,232,16,32,1,1,fcolor,bcolor,GT23_Rec_Data); 
	    DisplayAddr=DisplayAddr+16;
		DispalyCount++;
	  }
	  else if(byData[DispalyCount]>=0x81&&byData[DispalyCount]<=0xFe)
	  {
	    Get_GT23_Data(byData[DispalyCount],byData[DispalyCount+1],3);
	    lcd_fangda_dis_text(DisplayAddr,232,32,32,1,1,fcolor,bcolor,GT23_Rec_Data); 
	    DisplayAddr=DisplayAddr+32;
		DispalyCount+=2;
	  }
	  if(DispalyCount>7)
	  break;
	}
	

	DisplayAddr = 192;
	stLocalControl.uiShortType  = LCD_SEC_ADDR;	
	stLocalControl.uiShortType  += 39; 	//定位到床1的最高位
	byData1=FmReadByte(stLocalControl.uiShortType);
	if((byData1>0x30)&&(byData1<0x39))
	{
		Get_GT23_Data(0x00,byData1,3);
		lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
		DisplayAddr += 32;
		
	}
	else
	{//不要显示
		goto ShowBedPage1;
	}
	
/*	if((byData1 == 0x20)||(byData1 ==0x30))
	{//不要显示
		goto ShowBedPage1;
	}
	else 
	{
		Get_GT23_Data(0x00,byData1,2);
		lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
		DisplayAddr += 32;
		
	}*/


ShowBedPage1:
	stLocalControl.uiShortType  = LCD_SEC_ADDR;	
	stLocalControl.uiShortType  += 14;  //开始显示多少床到多少床
	byData1=FmReadByte(stLocalControl.uiShortType++); 
	byData2=FmReadByte(stLocalControl.uiShortType++);
	Get_GT23_Data(0x00,byData1,3);
	lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	DisplayAddr +=32;
	Get_GT23_Data(0x00,byData2,3);
	lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	DisplayAddr +=32;
	
	Get_GT23_Data(0x00,0x2D,3);    //   -
	lcd_fangda_dis_text(DisplayAddr,188,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	DisplayAddr +=32;



	stLocalControl.uiShortType  = LCD_SEC_ADDR;	
	stLocalControl.uiShortType  += 40; 	//定位到床2的最高位
	byData1=FmReadByte(stLocalControl.uiShortType);

	if((byData1>0x30)&&(byData1<0x39))
	{
		Get_GT23_Data(0x00,byData1,3);
		lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
		DisplayAddr += 32;
		
	}	
	else 
	{//不要显示
		goto ShowBedPage2;
	}	

/*	if((byData1 == 0x20)||(byData1 ==0x30))
	{//不要显示
		goto ShowBedPage2;
	}
	else 
	{
		Get_GT23_Data(0x00,byData1,2);
		lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
		DisplayAddr += 32;
		
	}*/	

ShowBedPage2:	
	stLocalControl.uiShortType  = LCD_SEC_ADDR;	
	stLocalControl.uiShortType  += 16;  //定位到床2的后两位
	byData1=FmReadByte(stLocalControl.uiShortType++); 
	byData2=FmReadByte(stLocalControl.uiShortType++);
	Get_GT23_Data(0x00,byData1,3);
	lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	DisplayAddr += 32;
	Get_GT23_Data(0x00,byData2,3);
	lcd_fangda_dis_text(DisplayAddr,192,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	DisplayAddr += 32;
	Get_GT23_Data(0xB4,0xB2,3);    //   床
	lcd_fangda_dis_text(DisplayAddr,188,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);
}
/**********************************************************
*函数名称			:ShowSecPage	
*函数描述        	:显示科室名称数据
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-23
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void ShowSecPage(void)
{	
    uint8  xdata i;
	uint8  xdata  byData1,byData2;
	uint16 xdata  fcolor,bcolor;
	uint8  xdata  byData[14];
	uint16 xdata  DisplayAddr;
	uint8  xdata  DispalyCount;
    if(Modify_SEC_Name==0x01)
    {
      byData1=Page_Color[0];//字体颜色 
	  byData2=Page_Color[1];//字体背景色,也是清屏色
	  fcolor = ColorIndex[byData1];
	  bcolor = ColorIndex[byData2]; 
    }
	else
	{
	  fcolor=WHITE; //字体颜色
	  bcolor=BLUE1; //字体背景色,也是清屏色
	}
	stLocalControl.uiShortType = LCD_SEC_ADDR;	         //第一帧开始显示科室名称
	for(i=0;i<14;i++)
	byData[i]=FmReadByte(stLocalControl.uiShortType++);	 //得到发送过来的数据
    DisplayAddr=16;	                                     //初始显示地址
	DispalyCount=0;										 //初始显示
	for(i=0;i<14;i++)
	{
	  if(byData[DispalyCount]>=0x20&&byData[DispalyCount]<=0x7E)
	  {
	    Get_GT23_Data(0x00,byData[DispalyCount],3);
	    lcd_fangda_dis_text(DisplayAddr,24,16,32,2,2,fcolor,bcolor,GT23_Rec_Data); 
	    DisplayAddr=DisplayAddr+32;
		DispalyCount++;
	  }
	  else if(byData[DispalyCount]>=0x81&&byData[DispalyCount]<=0xFe)
	  {
	    Get_GT23_Data(byData[DispalyCount],byData[DispalyCount+1],3);
	    lcd_fangda_dis_text(DisplayAddr,20,32,32,2,2,fcolor,bcolor,GT23_Rec_Data); 
	    DisplayAddr=DisplayAddr+64;
		DispalyCount+=2;
	  }
	  if(DispalyCount>13)
	  break;
	}
}
void ShowSerchRoomCom(uint8 xdata  page)
{
   	uint16 xdata fcolor,bcolor;
	fcolor=WHITE;//字体颜色
	bcolor=BLUE; //字体背景色,也是清屏色
  	Lcd_Fill(25,277,430,1,fcolor);
	Lcd_Fill(25,537,430,1,fcolor);
	Lcd_Fill(25,277,1,260,fcolor);
	Lcd_Fill(455,277,1,261,fcolor);
	Lcd_Fill(136,277,1,260,fcolor);
	Lcd_Fill(25,67+272,430,1,fcolor);
	Lcd_Fill(25,107+272,430,1,fcolor);
	Lcd_Fill(25,147+272,430,1,fcolor);
	Lcd_Fill(25,187+272,430,1,fcolor);
	Lcd_Fill(25,227+272,430,1,fcolor);
	//护士号
	Get_GT23_Data(0xBB,0xA4,3);
	lcd_fangda_dis_text(30,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data(0xCA,0xBF,3);
	lcd_fangda_dis_text(65,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data(0xBA,0xC5,3);
	lcd_fangda_dis_text(100,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);	 
	//查房时间
	Get_GT23_Data(0xB2,0xE9,3);
	lcd_fangda_dis_text(149,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data(0xB7,0xBF,3);
	lcd_fangda_dis_text(218,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data(0xCA,0xB1,3);
	lcd_fangda_dis_text(287,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data(0xBC,0xE4,3);
	lcd_fangda_dis_text(357,272+24,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);
    Get_GT23_Data(0x00,0x28,3);
	lcd_fangda_dis_text(394,24+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
    Get_GT23_Data(0x00,0x30+page,3);
	lcd_fangda_dis_text(414,25+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data(0x00,0x29,3);
	lcd_fangda_dis_text(434,24+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	Lcd_Fill(63,73+272,40,32,bcolor);
    Lcd_Fill(146,70+272,305,32,bcolor);
	Lcd_Fill(63,113+272,40,32,bcolor);
	Lcd_Fill(146,110+272,305,32,bcolor);
	Lcd_Fill(63,153+272,40,32,bcolor);
	Lcd_Fill(146,150+272,305,32,bcolor);
	Lcd_Fill(63,193+272,40,32,bcolor);
	Lcd_Fill(146,190+272,305,32,bcolor);
	Lcd_Fill(63,233+272,40,32,bcolor);
	Lcd_Fill(146,230+272,305,32,bcolor);
}
/**********************************************************
*函数名称			:ShowSerchRommPage1	
*函数描述        	:显示查房界面1数据
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-3-10
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	: 查房信息存储在第2屏
**********************************************************/ 
void ShowSerchRommPage1(void)
{
    uint16 xdata fcolor,bcolor;
    uint8  xdata serchroomcount=0;
	uint8  xdata nursenumber=0,nursenumber1,nursenumber2;
	uint8  xdata minute=0,minute1,minute2;
	uint8  xdata hour=0,hour1,hour2;	
	uint8  xdata day=0,day1,day2;
	uint8  xdata month=0,month1,month2;
	uint8  xdata year=0,year1,year2;
	fcolor=WHITE;//字体颜色
	bcolor=BLUE; //字体背景色,也是清屏色
	if(bIndication)
	{	
		return;	  //有呼叫信息存在,退出
	}
	if(First_Serchroom_Flag)
	{
	   First_Serchroom_Flag=0;
	   Lcd_Fill(0,110+272,480,82,bcolor);
	}
	ShowSerchRoomCom(1);
	serchroomcount=SerchRoom;	//读取护士查房次数
	if(Page_Flag==0)	 //第0页的查房记录显示，即开始的5条查房记录
	 {
	    if(serchroomcount>0)
	    {
		  //显示第一行护士号及查房时间
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x1000);
		  nursenumber=nursenumber&0x7F;
		  nursenumber=nursenumber%100;
		  nursenumber1=nursenumber%10+0x30;//个位
		  nursenumber2=nursenumber/10+0x30;//十位
	      //第一行护士编号
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
          //第一行查房时间
		  minute=FmReadByte(Serch_Room_TIME_ADDR+0x1000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x1001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x1002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x1003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x1004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
		 //----------------------------------------------------------------------
		 //----------------------------------------------------------------------
		  Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,68+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
		 //---------------------------------------------------------------------
	    //第二行护士编号
		if(serchroomcount>1)
		{
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x2000);
		  nursenumber=nursenumber&0x7F;
		  nursenumber=nursenumber%100;
		  nursenumber1=nursenumber%10+0x30;//个位
		  nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第二行查房时间
		  minute=FmReadByte(Serch_Room_TIME_ADDR+0x2000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x2001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x2002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x2003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x2004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,108+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		}	
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
    	//第三行护士编号
		if(serchroomcount>2)
		{
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x3000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第三行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x3000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x3001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x3002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x3003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x3004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,148+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		}	
	    //第四行护士编号
	    if(serchroomcount>3)
	    {
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x4000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第四行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x4000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x4001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x4002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x4003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x4004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,188+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
		if(serchroomcount>4)
		{
	     //第五行护士编号
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x5000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第五行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x5000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x5001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x5002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x5003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x5004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,year2,3);
	      lcd_fangda_dis_text(180,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,228+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
	}
  else
  {		 //显示第一行护士号及查房时间
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x1000);
		 nursenumber=nursenumber&0x7F;
		 nursenumber=nursenumber%100;
		 nursenumber1=nursenumber%10+0x30;//个位
		 nursenumber2=nursenumber/10+0x30;//十位
		 //Lcd_Fill(63,73+272,40,32,bcolor);
         //Lcd_Fill(146,70+272,305,32,bcolor);
	     //第一行护士编号
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
         //第一行查房时间
		 minute=FmReadByte(Serch_Room_TIME_ADDR+0x1000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0x1001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0x1002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0x1003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0x1004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
		 //----------------------------------------------------------------------
		 //----------------------------------------------------------------------
		 Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,68+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		 //---------------------------------------------------------------------
	    //第二行护士编号
		// Lcd_Fill(63,113+272,40,32,bcolor);
	    // Lcd_Fill(146,110+272,305,32,bcolor);
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x2000);
		 nursenumber=nursenumber&0x7F;
		 nursenumber=nursenumber%100;
		 nursenumber1=nursenumber%10+0x30;//个位
		 nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	    //第二行查房时间
		 minute=FmReadByte(Serch_Room_TIME_ADDR+0x2000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0x2001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0x2002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0x2003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0x2004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
	     Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,108+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
    	//第三行护士编号
		// Lcd_Fill(63,153+272,40,32,bcolor);
	    // Lcd_Fill(146,150+272,305,32,bcolor);
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x3000);
	     nursenumber=nursenumber&0x7F;
	     nursenumber=nursenumber%100;
	     nursenumber1=nursenumber%10+0x30;//个位
	     nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	   //第三行查房时间
	     minute=FmReadByte(Serch_Room_TIME_ADDR+0x3000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0x3001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0x3002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0x3003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0x3004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
	     Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,148+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	   //第四行护士编号
	   //  Lcd_Fill(63,193+272,40,32,bcolor);
	   //  Lcd_Fill(146,190+272,305,32,bcolor);
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x4000);
	     nursenumber=nursenumber&0x7F;
	     nursenumber=nursenumber%100;
	     nursenumber1=nursenumber%10+0x30;//个位
	     nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	   //第四行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x4000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x4001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x4002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x4003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x4004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,188+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      //第五行护士编号
		 // Lcd_Fill(63,233+272,40,32,bcolor);
	    //  Lcd_Fill(146,230+272,305,32,bcolor);
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x5000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第五行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x5000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x5001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x5002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x5003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x5004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,year2,3);
	      lcd_fangda_dis_text(180,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,228+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	}
  Set_VisualPage(1);
}
/**********************************************************
*函数名称			:ShowSerchRommPage2	
*函数描述        	:显示查房界面2数据
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-11-09
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	: 查房信息存储在第2屏
**********************************************************/ 
void ShowSerchRommPage2(void)
{
    uint16 xdata fcolor,bcolor;
    uint8  xdata serchroomcount=0;
	uint8  xdata nursenumber=0,nursenumber1,nursenumber2;
	uint8  xdata minute=0,minute1,minute2;
	uint8  xdata hour=0,hour1,hour2;	
	uint8  xdata day=0,day1,day2;
	uint8  xdata month=0,month1,month2;
	uint8  xdata year=0,year1,year2;
	fcolor=WHITE;//字体颜色
	bcolor=BLUE; //字体背景色,也是清屏色
	if(bIndication)
	{	
		return;	  //有呼叫信息存在,退出
	}
	ShowSerchRoomCom(2);
	serchroomcount=SerchRoom;	//读取护士查房次数
	if(Page_Flag==0)	 //第0页的查房记录显示，即开始的5条查房记录
	  {
	    if(serchroomcount>5)
	    {
		  //显示第一行护士号及查房时间
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x6000);
		  nursenumber=nursenumber&0x7F;
		  nursenumber=nursenumber%100;
		  nursenumber1=nursenumber%10+0x30;//个位
		  nursenumber2=nursenumber/10+0x30;//十位
	      //第一行护士编号
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
          //第一行查房时间
		  minute=FmReadByte(Serch_Room_TIME_ADDR+0x6000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x6001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x6002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x6003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x6004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
		 //----------------------------------------------------------------------
		 //----------------------------------------------------------------------
		  Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,68+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
		 //---------------------------------------------------------------------
	    //第二行护士编号
		if(serchroomcount>6)
		{
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x7000);
		  nursenumber=nursenumber&0x7F;
		  nursenumber=nursenumber%100;
		  nursenumber1=nursenumber%10+0x30;//个位
		  nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第二行查房时间
		  minute=FmReadByte(Serch_Room_TIME_ADDR+0x7000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x7001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x7002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x7003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x7004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,108+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		}	
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
    	//第三行护士编号
		if(serchroomcount>7)
		{
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x8000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第三行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x8000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x8001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x8002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x8003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x8004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,148+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		}	
	    //第四行护士编号
	    if(serchroomcount>8)
	    {
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x9000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第四行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x9000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x9001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x9002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x9003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x9004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,188+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
		if(serchroomcount>9)
		{
	     //第五行护士编号
	      nursenumber=FmReadByte(Serch_Room_ADDR+0xA000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第五行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xA000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xA001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xA002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xA003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xA004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,year2,3);
	      lcd_fangda_dis_text(180,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,228+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
	}
  else
  {		 //显示第一行护士号及查房时间
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x6000);
		 nursenumber=nursenumber&0x7F;
		 nursenumber=nursenumber%100;
		 nursenumber1=nursenumber%10+0x30;//个位
		 nursenumber2=nursenumber/10+0x30;//十位
	     //第一行护士编号
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
         //第一行查房时间
		 minute=FmReadByte(Serch_Room_TIME_ADDR+0x6000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0x6001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0x6002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0x6003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0x6004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
		 //----------------------------------------------------------------------
		 //----------------------------------------------------------------------
		 Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,68+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		 //---------------------------------------------------------------------
	    //第二行护士编号
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x7000);
		 nursenumber=nursenumber&0x7F;
		 nursenumber=nursenumber%100;
		 nursenumber1=nursenumber%10+0x30;//个位
		 nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	    //第二行查房时间
		 minute=FmReadByte(Serch_Room_TIME_ADDR+0x7000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0x7001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0x7002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0x7003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0x7004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
	     Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,108+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
    	//第三行护士编号
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x8000);
	     nursenumber=nursenumber&0x7F;
	     nursenumber=nursenumber%100;
	     nursenumber1=nursenumber%10+0x30;//个位
	     nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	   //第三行查房时间
	     minute=FmReadByte(Serch_Room_TIME_ADDR+0x8000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0x8001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0x8002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0x8003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0x8004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
	     Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,148+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	   //第四行护士编号
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x9000);
	     nursenumber=nursenumber&0x7F;
	     nursenumber=nursenumber%100;
	     nursenumber1=nursenumber%10+0x30;//个位
	     nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	   //第四行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0x9000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0x9001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0x9002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0x9003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0x9004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,188+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     //第五行护士编号
	      nursenumber=FmReadByte(Serch_Room_ADDR+0xA000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第五行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xA000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xA001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xA002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xA003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xA004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,year2,3);
	      lcd_fangda_dis_text(180,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,228+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	}
  }
/**********************************************************
*函数名称			:ShowSerchRommPage3	
*函数描述        	:显示查房界面3数据
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-11-09
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	: 查房信息存储在第3屏
**********************************************************/ 
void ShowSerchRommPage3(void)
{
    uint16 xdata fcolor,bcolor;
    uint8  xdata serchroomcount=0;
	uint8  xdata nursenumber=0,nursenumber1,nursenumber2;
	uint8  xdata minute=0,minute1,minute2;
	uint8  xdata hour=0,hour1,hour2;	
	uint8  xdata day=0,day1,day2;
	uint8  xdata month=0,month1,month2;
	uint8  xdata year=0,year1,year2;
	fcolor=WHITE;//字体颜色
	bcolor=BLUE; //字体背景色,也是清屏色
	if(bIndication)
	{	
		return;	  //有呼叫信息存在,退出
	}
	ShowSerchRoomCom(3);
	serchroomcount=SerchRoom;	//读取护士查房次数
	if(Page_Flag==0)	 //第0页的查房记录显示，即开始的5条查房记录
	  {
	    if(serchroomcount>10)
	    {
		  //显示第一行护士号及查房时间
	      nursenumber=FmReadByte(Serch_Room_ADDR+0xB000);
		  nursenumber=nursenumber&0x7F;
		  nursenumber=nursenumber%100;
		  nursenumber1=nursenumber%10+0x30;//个位
		  nursenumber2=nursenumber/10+0x30;//十位
	      //第一行护士编号
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
          //第一行查房时间
		  minute=FmReadByte(Serch_Room_TIME_ADDR+0xB000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xB001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xB002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xB003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xB004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
		 //----------------------------------------------------------------------
		 //----------------------------------------------------------------------
		  Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,68+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
		 //---------------------------------------------------------------------
	    //第二行护士编号
		if(serchroomcount>11)
		{
	      nursenumber=FmReadByte(Serch_Room_ADDR+0xC000);
		  nursenumber=nursenumber&0x7F;
		  nursenumber=nursenumber%100;
		  nursenumber1=nursenumber%10+0x30;//个位
		  nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第二行查房时间
		  minute=FmReadByte(Serch_Room_TIME_ADDR+0xC000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xC001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xC002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xC003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xC004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,108+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		}	
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
    	//第三行护士编号
		if(serchroomcount>12)
		{
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x2D000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第三行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xD000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xD001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xD002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xD003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xD004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,148+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		}	
	    //第四行护士编号
	    if(serchroomcount>13)
	    {
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x2E000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第四行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xE000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xE001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xE002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xE003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xE004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,188+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
		if(serchroomcount>14)
		{
	     //第五行护士编号
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x2F000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第五行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xF000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xF001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xF002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xF003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xF004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,year2,3);
	      lcd_fangda_dis_text(180,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,228+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		}
	}
  else
  {		 //显示第一行护士号及查房时间
	     nursenumber=FmReadByte(Serch_Room_ADDR+0xB000);
		 nursenumber=nursenumber&0x7F;
		 nursenumber=nursenumber%100;
		 nursenumber1=nursenumber%10+0x30;//个位
		 nursenumber2=nursenumber/10+0x30;//十位
	     //第一行护士编号
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
         //第一行查房时间
		 minute=FmReadByte(Serch_Room_TIME_ADDR+0xB000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0xB001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0xB002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0xB003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0xB004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
		 //----------------------------------------------------------------------
		 //----------------------------------------------------------------------
		 Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,71+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,68+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,73+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		 //---------------------------------------------------------------------
	    //第二行护士编号
	     nursenumber=FmReadByte(Serch_Room_ADDR+0xC000);
		 nursenumber=nursenumber&0x7F;
		 nursenumber=nursenumber%100;
		 nursenumber1=nursenumber%10+0x30;//个位
		 nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	    //第二行查房时间
		 minute=FmReadByte(Serch_Room_TIME_ADDR+0xC000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0xC001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0xC002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0xC003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0xC004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------
	     Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,111+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,108+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,113+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
    	//第三行护士编号
	     nursenumber=FmReadByte(Serch_Room_ADDR+0x2D000);
	     nursenumber=nursenumber&0x7F;
	     nursenumber=nursenumber%100;
	     nursenumber1=nursenumber%10+0x30;//个位
	     nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	   //第三行查房时间
	     minute=FmReadByte(Serch_Room_TIME_ADDR+0xD000);
		 hour=FmReadByte(Serch_Room_TIME_ADDR+0xD001);
		 day=FmReadByte(Serch_Room_TIME_ADDR+0xD002);
		 month=FmReadByte(Serch_Room_TIME_ADDR+0xD003);
		 year=FmReadByte(Serch_Room_TIME_ADDR+0xD004);
		 hour1=hour%10+0x30;//个位
		 hour2=hour/10+0x30;//十位
		 minute1=minute%10+0x30;//个位
		 minute2=minute/10+0x30;//十位
		 day1=day%10+0x30;//个位
		 day2=day/10+0x30;//十位
		 month1=month%10+0x30;//个位
		 month2=month/10+0x30;//十位
		 year1=year%10+0x30;//个位
		 year2=year/10+0x30;//十位
	     Get_GT23_Data(0x00,0x32,3);
	     lcd_fangda_dis_text(146,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x30,3);
	     lcd_fangda_dis_text(163,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		 Get_GT23_Data(0x00,year2,3);
		 lcd_fangda_dis_text(180,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,year1,3);
	     lcd_fangda_dis_text(197,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(214,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	     Get_GT23_Data(0x00,month2,3);
	     lcd_fangda_dis_text(231,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,month1,3);
	     lcd_fangda_dis_text(248,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,0x2F,3);
	     lcd_fangda_dis_text(265,151+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		 Get_GT23_Data(0x00,day2,3);
	     lcd_fangda_dis_text(282,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,day1,3);
	     lcd_fangda_dis_text(299,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour2,3);
	     lcd_fangda_dis_text(356,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,hour1,3);
	     lcd_fangda_dis_text(373,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	     Get_GT23_Data(0x00,0x3A,3);
	     lcd_fangda_dis_text(395,148+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	     Get_GT23_Data(0x00,minute2,3);
	     lcd_fangda_dis_text(407,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,minute1,3);
	     lcd_fangda_dis_text(424,153+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	   //第四行护士编号
		 nursenumber=FmReadByte(Serch_Room_ADDR+0x2E000);
	     nursenumber=nursenumber&0x7F;
	     nursenumber=nursenumber%100;
	     nursenumber1=nursenumber%10+0x30;//个位
	     nursenumber2=nursenumber/10+0x30;//十位
	     Get_GT23_Data(0x00,nursenumber2,3);
	     lcd_fangda_dis_text(63,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     Get_GT23_Data(0x00,nursenumber1,3);
	     lcd_fangda_dis_text(83,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	   //第四行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xE000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xE001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xE002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xE003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xE004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
		  Get_GT23_Data(0x00,year2,3);
		  lcd_fangda_dis_text(180,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,191+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
		  Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,188+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,193+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	     //第五行护士编号
	      nursenumber=FmReadByte(Serch_Room_ADDR+0x2F000);
	      nursenumber=nursenumber&0x7F;
	      nursenumber=nursenumber%100;
	      nursenumber1=nursenumber%10+0x30;//个位
	      nursenumber2=nursenumber/10+0x30;//十位
	      Get_GT23_Data(0x00,nursenumber2,3);
	      lcd_fangda_dis_text(63,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,nursenumber1,3);
	      lcd_fangda_dis_text(83,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      //第五行查房时间
	      minute=FmReadByte(Serch_Room_TIME_ADDR+0xF000);
		  hour=FmReadByte(Serch_Room_TIME_ADDR+0xF001);
		  day=FmReadByte(Serch_Room_TIME_ADDR+0xF002);
		  month=FmReadByte(Serch_Room_TIME_ADDR+0xF003);
		  year=FmReadByte(Serch_Room_TIME_ADDR+0xF004);
		  hour1=hour%10+0x30;//个位
		  hour2=hour/10+0x30;//十位
		  minute1=minute%10+0x30;//个位
		  minute2=minute/10+0x30;//十位
		  day1=day%10+0x30;//个位
		  day2=day/10+0x30;//十位
		  month1=month%10+0x30;//个位
		  month2=month/10+0x30;//十位
		  year1=year%10+0x30;//个位
		  year2=year/10+0x30;//十位
	      Get_GT23_Data(0x00,0x32,3);
	      lcd_fangda_dis_text(146,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x30,3);
	      lcd_fangda_dis_text(163,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,year2,3);
	      lcd_fangda_dis_text(180,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,year1,3);
	      lcd_fangda_dis_text(197,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(214,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,month2,3);
	      lcd_fangda_dis_text(231,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,month1,3);
	      lcd_fangda_dis_text(248,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,0x2F,3);
	      lcd_fangda_dis_text(265,230+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);// "/"
	      Get_GT23_Data(0x00,day2,3);
	      lcd_fangda_dis_text(282,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,day1,3);
	      lcd_fangda_dis_text(299,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour2,3);
	      lcd_fangda_dis_text(356,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,hour1,3);
	      lcd_fangda_dis_text(373,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
	      Get_GT23_Data(0x00,0x3A,3);
	      lcd_fangda_dis_text(395,228+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);//":"
	      Get_GT23_Data(0x00,minute2,3);
	      lcd_fangda_dis_text(407,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	      Get_GT23_Data(0x00,minute1,3);
	      lcd_fangda_dis_text(424,232+272,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);	
	}
  }
void ShowNumberPage(void)
{
   uint8 sec,sec1,sec2;
   uint8 room,room1,room2;
   uint16 xdata fcolor,bcolor;
   fcolor=WHITE;//字体颜色
   bcolor=BLUE; //字体背景色,也是清屏色
   sec=stLocalControl.stBusDealFreq.bySndSecAddr&0x7F;
   sec %=100;
   room=stLocalControl.stBusDealFreq.bySndRoomAddr&0x7F;
   room %=100;
   sec1=sec/10+0x30;
   sec2=sec%10+0x30;
   room1=room/10+0x30;
   room2=room%10+0x30;
   SetLcdState(1);
   Lcd_Fill(0,100+544,480,80,bcolor);
   Get_GT23_Data(0x00,sec1,3);
   lcd_fangda_dis_text(120,120+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   Get_GT23_Data(0x00,sec2,3);
   lcd_fangda_dis_text(136,120+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   Get_GT23_Data(0xC7,0xF8,3);
   lcd_fangda_dis_text(152,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//区
   Get_GT23_Data(0x00,room1,3);
   lcd_fangda_dis_text(184,120+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   Get_GT23_Data(0x00,room2,3);
   lcd_fangda_dis_text(200,120+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   Get_GT23_Data(0xB7,0xBF,3);
   lcd_fangda_dis_text(216,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//房
   Get_GT23_Data(0xB1,0xE0,3);
   lcd_fangda_dis_text(248,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//编
   Get_GT23_Data(0xBA,0xC5,3);
   lcd_fangda_dis_text(280,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//号
   Get_GT23_Data(0x00,0x2E,3);
   lcd_fangda_dis_text(312,110+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   lcd_fangda_dis_text(328,110+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   lcd_fangda_dis_text(344,110+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
   Set_VisualPage(2);
}
void ShowNumberPageOK(void)
{
   uint16 xdata fcolor,bcolor;
   fcolor=WHITE;//字体颜色
   bcolor=BLUE; //字体背景色,也是清屏色
   Lcd_Fill(0,100+544,480,80,bcolor);
   Get_GT23_Data(0xB1,0xE0,3);
   lcd_fangda_dis_text(176,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//编
   Get_GT23_Data(0xBA,0xC5,3);
   lcd_fangda_dis_text(208,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//号
   Get_GT23_Data(0xB3,0xC9,3);
   lcd_fangda_dis_text(240,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//成
   Get_GT23_Data(0xB9,0xA6,3);
   lcd_fangda_dis_text(272,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//功
   Set_VisualPage(2);
}
void ShowCalling(void)
{
   uint16 xdata fcolor,bcolor;
   fcolor=WHITE;//字体颜色
   bcolor=BLUE; //字体背景色,也是清屏色
   Lcd_Fill(0,644,480,80,bcolor);
   Get_GT23_Data(0xB1,0xBE,3);
   lcd_fangda_dis_text(128,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//本
   Get_GT23_Data(0xBB,0xFA,3);
   lcd_fangda_dis_text(160,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//机
   Get_GT23_Data(0xD5,0xFD,3);
   lcd_fangda_dis_text(192,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//正
   Get_GT23_Data(0xD4,0xDA,3);
   lcd_fangda_dis_text(224,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//在
   Get_GT23_Data(0xB9,0xE3,3);
   lcd_fangda_dis_text(256,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//广
   Get_GT23_Data(0xB2,0xA5,3);
   lcd_fangda_dis_text(288,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//播
   Get_GT23_Data(0xD6,0xD0,3);
   lcd_fangda_dis_text(320,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//中
   Set_VisualPage(2);
}
void ShowBROADCASTing(void)
{
   uint8  xdata tempbed,temproom;
   uint8  xdata tempbedh,tempbedl;
   uint16 xdata fcolor,bcolor;
   fcolor=WHITE;//字体颜色
   bcolor=BLUE; //字体背景色,也是清屏色
   Lcd_Fill(0,100+544,480,80,bcolor);
   temproom=stLocalControl.stBusDealFreq.bySndRoomAddr;
   tempbed=stLocalControl.stBusDealFreq.bySndBedAddr;
   if(temproom==253&&tempbed!=0)    //医护分机正在广播
   {
     Broadcasting_Flag=1;
     tempbed=tempbed&0x7F;
	 tempbed=tempbed%100;
	 tempbedh=tempbed/10+0x30;
     tempbedl=tempbed%10+0x30;
	 Get_GT23_Data(0x00,tempbedh,3);
     lcd_fangda_dis_text(80,122+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
     Get_GT23_Data(0x00,tempbedl,3);
     lcd_fangda_dis_text(96,122+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);  
     Get_GT23_Data(0xBA,0xC5,3);
     lcd_fangda_dis_text(112,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//号
     Get_GT23_Data(0xD2,0xBD,3);
     lcd_fangda_dis_text(144,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//医
     Get_GT23_Data(0xBB,0xA4,3);
     lcd_fangda_dis_text(176,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//护
     Get_GT23_Data(0xB7,0xD6,3);
     lcd_fangda_dis_text(208,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//分
     Get_GT23_Data(0xBB,0xFA,3);
     lcd_fangda_dis_text(240,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//机
     Get_GT23_Data(0xD5,0xFD,3);
     lcd_fangda_dis_text(272,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//正
     Get_GT23_Data(0xD4,0xDA,3);
     lcd_fangda_dis_text(304,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//在
     Get_GT23_Data(0xB9,0xE3,3);
     lcd_fangda_dis_text(336,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//广
     Get_GT23_Data(0xB2,0xA5,3);
     lcd_fangda_dis_text(368,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//播
     Set_VisualPage(2);
   }
  else if(temproom!=253&&tempbed==0)   //门口分机正在广播
  {
     Broadcasting_Flag=1;
     temproom=temproom&0x7F; 
	 temproom=temproom%100;
     tempbedh=temproom/10+0x30;
     tempbedl=temproom%10+0x30;
	 Get_GT23_Data(0x00,tempbedh,3);
     lcd_fangda_dis_text(80,122+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);
     Get_GT23_Data(0x00,tempbedl,3);
     lcd_fangda_dis_text(96,122+544,16,32,1,1,fcolor,bcolor,GT23_Rec_Data);  
     Get_GT23_Data(0xB7,0xBF,3);
     lcd_fangda_dis_text(112,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//房
     Get_GT23_Data(0xC3,0xC5,3);
     lcd_fangda_dis_text(144,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//门
     Get_GT23_Data(0xBF,0xDA,3);
     lcd_fangda_dis_text(176,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//口
     Get_GT23_Data(0xB7,0xD6,3);
     lcd_fangda_dis_text(208,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//分
     Get_GT23_Data(0xBB,0xFA,3);
     lcd_fangda_dis_text(240,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//机
     Get_GT23_Data(0xD5,0xFD,3);
     lcd_fangda_dis_text(272,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//正
     Get_GT23_Data(0xD4,0xDA,3);
     lcd_fangda_dis_text(304,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//在
     Get_GT23_Data(0xB9,0xE3,3);
     lcd_fangda_dis_text(336,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//广
     Get_GT23_Data(0xB2,0xA5,3);
     lcd_fangda_dis_text(368,120+544,32,32,1,1,fcolor,bcolor,GT23_Rec_Data);//播
     Set_VisualPage(2); 
  } 
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------



void ShowBusError(void)
{
	uint16 xdata  fcolor,bcolor;	

    if(Modify_SEC_Name==0x01)
    {
		if(Page_Color[3]>=9 ||Page_Color[4]>=9)
		{
			fcolor=BLUE1;  //字体颜色 
			bcolor=WHITE; //字体背景色,也是清屏色
		}
		else
		{
			fcolor = ColorIndex[Page_Color[3]]; //字体颜色 
			bcolor = ColorIndex[Page_Color[4]]; //字体背景色,也是清屏色
		}
    }
	else
	{
	  fcolor=BLUE1;//字体颜色
	  bcolor=WHITE; //字体背景色,也是清屏色
	}

	Get_GT23_Data1("总",1);										  //如果有总线故障则显示“总线故障”
	lcd_fangda_dis_text(416,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data1("线",1);
	lcd_fangda_dis_text(432,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	Get_GT23_Data1("故",1);
	lcd_fangda_dis_text(448,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);//故
	Get_GT23_Data1("障",1);
	lcd_fangda_dis_text(464,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);//障
}




void ShowSelfError(void)
{
	uint16 xdata  fcolor,bcolor;	
 
	fcolor=BLUE1;//字体颜色
	bcolor=WHITE; //字体背景色,也是清屏色

	if(Modify_SEC_Name==0x01)
	 {
		 if(Page_Color[3]>=9||Page_Color[4]>=9)
		 {
			 fcolor=BLUE1;	//字体颜色 
			 bcolor=WHITE; //字体背景色,也是清屏色
		 }
		 else
		 {
			 fcolor = ColorIndex[Page_Color[3]]; //字体颜色 
			 bcolor = ColorIndex[Page_Color[4]]; //字体背景色,也是清屏色
		 }
	 }
	 else
	 {
	   fcolor=BLUE1;//字体颜色
	   bcolor=WHITE; //字体背景色,也是清屏色
	 }
	
	 Get_GT23_Data1("本",1);									   //如果本机故障则显示“本机故障”
	 lcd_fangda_dis_text(416,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	 Get_GT23_Data1("机",1);
	 lcd_fangda_dis_text(432,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);
	 Get_GT23_Data1("故",1);
	 lcd_fangda_dis_text(448,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);//故
	 Get_GT23_Data1("障",1);
	 lcd_fangda_dis_text(464,256,16,16,1,1,fcolor,bcolor,GT23_Rec_Data);//障

}



void ShowNormal(void)
{
	 Lcd_Fill(416,256,64,16,ColorIndex[Page_Color[4]]);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
/**********************************************************
*函数名称			:ShowPage	
*函数描述        	:显示整页数据
*输入参数   		:				 
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-23
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void ShowPage(void)
{	
	uint8  xdata byData1,byData2,byData3;	
	uint16 xdata fcolor,bcolor,ccolor;
	if(bIndication)
	{	
	  return;	  //有呼叫信息存在,退出
	}
    if(Modify_SEC_Name==0x01)
    {
      byData1=Page_Color[0];//字体颜色 
	  byData2=Page_Color[1];//字体背景色,也是清屏色
	  byData3=Page_Color[2];//中间填充色

	  fcolor = ColorIndex[byData1];
	  bcolor = ColorIndex[byData2];
	  ccolor = ColorIndex[byData3];
    }
	else
	{
	  fcolor=WHITE;//字体颜色
	  bcolor=BLUE1; //字体背景色,也是清屏色
	  ccolor=YELLOW;//房号与几床到几床的中间填充颜色
	}
	//科室号
	if(Modify_SEC_Name==0x01)
	{
	    ShowSecPage(); 
    }
	//-------------------------------------------------------------------
	//房号
	if(Modify_Room_Number==0x01) //如果已经编号，则显示房号
	{
	    byData1=stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7F;
	    byData1=byData1%100;
	    byData2=stLocalControl.stEepromCfgData.bySelfSecAddr&0x7F;
	    byData2=byData2%100;
		if(byData2/10>0)
		{
		   Get_GT23_Data(0x00,byData2/10+0x30,3);
	       lcd_fangda_dis_text(142,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
		   Get_GT23_Data(0x00,byData2%10+0x30,3);
	       lcd_fangda_dis_text(174,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	       Get_GT23_Data(0x00,byData1/10+0x30,3);
	       lcd_fangda_dis_text(206,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	       Get_GT23_Data(0x00,byData1%10+0x30,3);
	       lcd_fangda_dis_text(238,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data); 
	       Get_GT23_Data(0xB7,0xBF,3);
	       lcd_fangda_dis_text(270,94,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	   }
	   else
	   {
	       Get_GT23_Data(0x00,byData2%10+0x30,3);
	       lcd_fangda_dis_text(160,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	       Get_GT23_Data(0x00,byData1/10+0x30,3);
	       lcd_fangda_dis_text(192,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	       Get_GT23_Data(0x00,byData1%10+0x30,3);
	       lcd_fangda_dis_text(224,96,16,32,2,2,fcolor,bcolor,GT23_Rec_Data); 
	       Get_GT23_Data(0xB7,0xBF,3);
	       lcd_fangda_dis_text(256,94,32,32,2,2,fcolor,bcolor,GT23_Rec_Data);
	   }
	}
	//-------------------------------------------------------------------
	//几床到几床
	if(Modify_SEC_Name==0x01)
    {
       byData1=Page_Color[3];//字体颜色 
	   byData2=Page_Color[4];//字体背景色,也是清屏色
	   fcolor = ColorIndex[byData1];
	   bcolor = ColorIndex[byData2];
    }
	else
	{
	   fcolor=BLUE1;//字体颜色
	   bcolor=WHITE;//字体背景色,也是清屏色
	}
	if(Modify_SEC_Name==0x01)
	{
	   ShowBedPage();
	}
	Set_VisualPage(0);
}
void LcmInit(void)
{

   //背光控制端(P3.5)设置为推挽输出
    P3M1 &= 0xDF;
    P3M0 |= 0x20;
    LCD_LED=0;
	SSD1963_RD=1;
    SSD1963_CS=0;
    SSD1963_RST=1;            
    //Delayms(5);
	
    SSD1963_RST=0;
    Delayms(10);
    SSD1963_RST=1;
    Delayms(5);
    Send_Ssd1963_Command(0x01); 
    Delayms(5);
    Send_Ssd1963_Command(0xE0);
    Delayms(1);
    Send_Ssd1963_Data(0x01);
    Delayms(1);
    Send_Ssd1963_Command(0xE0);
    Delayms(1);
    Send_Ssd1963_Data(0x03);
    Delayms(1);
    Send_Ssd1963_Command(0xE2); 
    Send_Ssd1963_Data(0x3B);
    Send_Ssd1963_Data(0x02);
    Send_Ssd1963_Data(0x04);
    Send_Ssd1963_Command(0xE6);
    Delayms(1);
    Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Data(0x55);
    Send_Ssd1963_Data(0x54);
    Send_Ssd1963_Command(0xB0);
    Send_Ssd1963_Data(0x00);	
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Data(0xDF);
    Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Data(0x0F);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Command(0x3A);
    Send_Ssd1963_Data(0x20);
    Send_Ssd1963_Command(0xB4);
    Send_Ssd1963_Data(0x02);
    Send_Ssd1963_Data(0x12);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Data(0x40);
    Send_Ssd1963_Data(0x07);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Command(0xB6);
    Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Data(0x1F);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Data(0x04);
    Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Data(0x00);
    Send_Ssd1963_Command(0x2A);
	Send_Ssd1963_Data(0x00);
	Send_Ssd1963_Data(0x00);
	Send_Ssd1963_Data(0xDF);
	Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Command(0x2B);
	Send_Ssd1963_Data(0x00);
	Send_Ssd1963_Data(0x00);
	Send_Ssd1963_Data(0x0F);
	Send_Ssd1963_Data(0x01);
	SetScrollArea(0,272,0);
    Send_Ssd1963_Command(0x36);
    Send_Ssd1963_Data(0x08);
    Send_Ssd1963_Command(0xF0);
	Send_Ssd1963_Data(0x03);
    Delayms(1);
    Send_Ssd1963_Command(0x29);
    Send_Ssd1963_Command(0xB8);
    Send_Ssd1963_Data(0x0F);
    Send_Ssd1963_Data(0x01);
    Send_Ssd1963_Command(0xBA);
    Send_Ssd1963_Data(0x01);
    //SSD1963_CS=1;
	Lcd_Clear(BLUE,0);
	bLcdOn =1;
	LCD_LED=1;
	ShowVersion(3,0,0);  //“软件版本号”、主界面放在第一屏
	Lcd_Clear(BLUE,1);	  
	//ShowVersion(1,3,1);	 //“护士查房”放在第二屏
	First_Serchroom_Flag=1;
	Lcd_Clear(BLUE,2);
	//ShowLandingFace();   //“系统初始化”、呼叫信息放在第三屏
	SetLedDealState(LED_OFF);
	SetMDLedState(LED_OFF);
	SetLcdState(1);
	ReadPageColour();	
	ShowPage();		
} 
