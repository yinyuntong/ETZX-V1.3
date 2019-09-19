#ifndef _LCD_H_
#define _LCD_H_

#define uchar unsigned char
#define uint unsigned int
//---------------------------------
//color
#define	 BLACK   0x0000
#define	 RED     0xF800
#define	 GREEN   0x07E0
#define	 BLUE    0x0017
#define	 YELLOW  0xFFE0
#define	 PURPLE  0xF817
#define	 CYAN    0x07FF
#define	 WHITE   0xFFFF


#define	 BLACK_CODE   0x00
#define	 RED_CODE     0x01
#define	 GREEN_CODE   0x02
#define	 BLUE_CODE    0x03
#define	 YELLOW_CODE  0x04
#define	 PURPLE_CODE  0x05
#define	 CYAN_CODE    0x06
#define	 WHITE_CODE   0x07

#define LCD_PIX_X 128
#define LCD_PIX_Y 160

#define DISP_FONT_SIZE 1
#define DISP_FONT_Y 16
#define DISP_FONT_X 16
//---------------------------------

#ifndef _IN_LCD_
extern bit bSecondDisState;

extern uint16 xdata DispPageAddr[];
extern uint8 xdata CurrentPage;

extern uint8 code ColonCode16x32[];

extern uint16 xdata DisplayedLen;


extern void Delayms(uint ms);
extern void Send_ST7735_Command(uchar st7735_command);
extern void Send_ST7735_Data(uchar st7735_data);
extern void ST7735_Init(void);
extern void Set_Display_Area(uchar start_x,uchar end_x,uchar start_y,uchar end_y);
extern void Lcd_Clear(uint color);
extern void Display_Dot(uchar x,uchar y,uint color);  
extern void Display_HLine(uchar x,uchar y,uchar width,uint color);
extern void Display_VLine(uchar x,uchar y,uchar height,uint color);

extern void Lcd_Fill(uchar start_x,uchar start_y,uchar width,uchar height,uint color);
extern void DispManager(void);
extern void ShowLandingFace(void);
extern void ShowTalkFace(void);
extern void ShowBroadFace(void);
extern void ShowCallSuccess(void);


extern void DisplayDigitString(uint8 *px,uint8 *py,uint8 *pDigit,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size);
extern void DisplayASCIIString(uint8 *px,uint8 *py,uint8 *pASCII,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size);
extern void DisplayBlock(uint8 start_x,uint8 start_y,uint8 end_x,uint8 end_y, uint8 srgb);
extern void DisplayChineseString(uint8 *px,uint8 *py,uint8 *pChinese,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size);
extern void FindDispPageAddr(uint8 DisplayFace);
extern void ShowCostListFace();
extern void ShowMedicalListFace();
extern void ShowPrescriptionFace();
extern void ShowPromptFace();
extern void ShowFace(uint8 DispFaceType);
extern void DisplayDigit(uint8 x,uint8 y,uint8 Digit,uint16 fcolor,uint16 bcolor,uint8 size);
extern void DisplayASCII(uint8 x,uint8 y,uint8 ASCII,uint16 fcolor,uint16 bcolor,uint8 size);

extern void Display_ASCII(uint8 x,uint8 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size);
extern void Display_Chinese(uint8 x,uint8 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size);
extern void ShowPowerUpFace(void);
extern void ShowMainMenuFace(void);
extern uint8 DisplayOneScreen(uint8 *pDispData,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size);
extern void SecondFlash(void);
extern void ShowTime(void);
#endif

#endif
