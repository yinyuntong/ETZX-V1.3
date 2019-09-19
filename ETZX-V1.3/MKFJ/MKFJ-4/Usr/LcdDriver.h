/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:LcdDriver.h
*文件描述    		:液晶驱动程序头文件
*创建人     		:陈卫国
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define	 BLACK   0x0000	   //0
#define	 RED     0xF800	   //1
#define	 GREEN   0x07E0	   //2
#define	 BLUE1   0x0007	   //3
#define	 BLUE    0x001F	   //4
#define	 YELLOW  0xFFE0	   //5
#define	 PURPLE  0xF81F	   //6
#define	 CYAN    0x07FF	   //7
#define	 WHITE   0xFFFF	   //8

#ifndef _IN_LCD_
#define _IN_LCD_ 
void LcmInit(void);
void Lcd_Clear(uint16 color,uint8 VisualPage);
void Set_Display_Area(uint16 start_x,uint16 end_x,uint16 start_y,uint16 end_y);
void Send_Ssd1963_Data(uint16 ssd1963_data);
void Send_Ssd1963_Command(uint8 ssd1963_command);
void lcd_fangda_dis_text(uint16 x,uint16 y, uint8 type_x_wide, uint8 type_y_height,uint8 mul_col,uint8 mul_row,uint16 fcolor,uint16 bcolor,uint8 *p);
void ShowLandingFace(void);
void ShowLandingWord(uint16 xdata byRow, uint16 xdata byCol, uint8 xdata byWord,uint16 xdata fcolor,uint16 xdata bcolor);
void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCmd);
void ShowSecPage(void);
void ShowSerchRommPage1(void);
void ShowSerchRommPage2(void);
void ShowSerchRommPage3(void);
void ShowNumberPage(void);
void ShowNumberPageOK(void);
void ShowBedPage(void);
void ShowPage(void);
void Lcd_Fill(uint16 start_x,uint16 start_y, uint16 width, uint16 height,uint16 color);
void Set_VisualPage(uint8 VisualPage);
void SetScrollStart(uint16 line);
void SetScrollArea(uint16 top, uint16 scroll, uint16 bottom);
void ShowVersion(uint8 xdata hversion,uint8 xdata lversion,uint8 page);
void ShowCalling(void);
void ShowBROADCASTing(void);
void ShowBusError(void);
void ShowSelfError(void);
void ShowNormal(void);
#endif