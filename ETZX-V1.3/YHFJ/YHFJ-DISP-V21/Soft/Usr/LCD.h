/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:TFT.h
*文件描述    		:TFT彩屏驱动程序头文件
*创建人     		:尹运同
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
#ifndef _LCD_H_
#define _LCD_H_

union union_type
{
	int8  ASCII_BCD[10];  		//字符型
	int16  uiOperData[5]; 		//整型
	int32  ulOperData[2];		//长整型
};


#define SSD1963_DATA	P0

#define DataPortL	P0
#define DataPortH	P2

//---------------------------------
//color
/*#define	 BLACK   0x000000
#define	 RED     0xFF0000
#define	 GREEN   0x00FF00
#define	 BLUE    0x0000FF
#define	 YELLOW  0xFFFF00
#define	 PURPLE  0xFF00FF	//紫色
#define	 CYAN    0x00FFFF	//蓝绿色
#define	 WHITE   0xFFFFFF*/


//蓝绿红格式
#define	 BLACK   0x0000
#define	 RED     0x001f
#define	 GREEN   0x07E0
#define  POWDER_BLUE  0X3800	//浅蓝色
#define	 BLUE    0xf800
#define	 YELLOW  0x07ff
#define	 PURPLE  0xF81F	//紫色
#define	 CYAN    0xffe0	//蓝绿色
#define	 WHITE   0xFFFF


#define	 BLACK_CODE   0x00
#define	 RED_CODE     0x01
#define	 GREEN_CODE   0x02
#define  POWDER_BLUE_CODE 0X03
#define	 BLUE_CODE    0x04
#define	 YELLOW_CODE  0x05
#define	 PURPLE_CODE  0x06	//紫色
#define	 CYAN_CODE    0x07	//蓝绿色
#define	 WHITE_CODE   0x08


//---------------------------------

#define  LCD_ORIGIN_TOUCH_X  0XE5		//TFT屏原点在触摸芯片输出的X值
#define	 LCD_ORIGIN_TOUCH_Y  0XE0       //TFT屏原点在触摸芯片输出的Y值


#define  LCD_PIX_X	480					//TFT屏为480*272
#define  LCD_PIX_Y	272



#define cmd_soft_reset 0x01  //Software Reset
#define cmd_get_power_mode 0x0A //Get the current power mode
#define cmd_get_address_mode 0x0B //Get the frame memory to the display panel read order
#define cmd_get_pixel_format 0x0C  //Gett the current pixel format
#define cmd_get_display_mode 0x0D  //The display module returns the Display Signal Mode.
#define cmd_get_signal_mode 0x0E //Get the current display mode from the peripheral
#define cmd_enter_sleep_mode 0x10  //Turn off the panel. This command will pull low the GPIO0. If GPIO0 is configured as normal GPIO or LCD miscellaneous signal with command set_gpio_conf, this command will be ignored.
#define cmd_exit_sleep_mode 0x11  //Turn on the panel. This command will pull high the GPIO0. If GPIO0 is configured as normal GPIO or LCD miscellaneous signal with command set_gpio_conf, this command will be ignored.
#define cmd_enter_partial_mode 0x12 //Part of the display area is used for image display.
#define cmd_enter_normal_mode 0x13 //The whole display area is used for image display.
#define cmd_exit_invert_mode 0x20 //Displayed image colors are not inverted.
#define cmd_enter_invert_mode 0x21 //Displayed image colors are inverted.
#define cmd_set_gamma_curve 0x26 //Selects the gamma curve used by the display device.
#define cmd_set_display_off 0x28 //Blanks the display device
#define cmd_set_display_on 0x29 //Show the image on the display device
#define cmd_set_column_address 0x2A //Set the column extent
#define cmd_set_page_address 0x2B //Set the page extent
#define cmd_write_memory_start 0x2C //Transfer image information from the host processor interface to the peripheral starting at the location provided by set_column_address and set_page_address
#define cmd_read_memory_start 0x2E //Transfer image data from the peripheral to the host processor interface starting at the location provided by set_column_address and set_page_address
#define cmd_set_partial_area 0x30 //Defines the partial display area on the display device
#define cmd_set_scroll_area 0x33 //Defines the vertical scrolling and fixed area on display area
#define cmd_set_tear_off 0x34 //Synchronization information is not sent from the display module to the host processor
#define cmd_set_tear_on 0x35 //Synchronization information is sent from the display module to the host processor at the start of VFP
#define cmd_set_address_mode 0x36 //Set the read order from frame buffer to the display panel
#define cmd_set_scroll_start 0x37 //Defines the vertical scrolling starting point
#define cmd_exit_idle_mode 0x38 //Full color depth is used for the display panel
#define cmd_enter_idle_mode 0x39  //Reduce color depth is used on the display panel.
#define cmd_set_pixel_format 0x3A  //Defines how many bits per pixel are used in the interface
#define cmd_write_memory_continue 0x3C //Transfer image information from the host processor interface to the peripheral from the last written location 
#define cmd_read_memory_continue 0x3E //Read image data from the peripheral continuing after the last read_memory_continue or read_memory_start
#define cmd_set_tear_scanline 0x44 //Synchronization information is sent from the display module to the host processor when the display device refresh reaches the provided scanline 
#define cmd_get_scanline 0x45 //Get the current scan line
#define cmd_read_ddb 0xA1 //Read the DDB from the provided location
#define cmd_set_lcd_mode 0xB0  //Set the LCD panel mode (RGB TFT or TTL)
#define cmd_get_lcd_mode 0xB1 //Get the current LCD panel mode, pad strength and resolution
#define cmd_set_hori_period 0xB4 //Set front porch
#define cmd_get_hori_period 0xB5 //Get current front porch settings
#define cmd_set_vert_period 0xB6 //Set the vertical blanking interval between last scan line and next LFRAME pulse
#define cmd_get_vert_period 0xB7 //Set the vertical blanking interval between last scan line and next LFRAME pulse
#define cmd_set_gpio_conf 0xB8 //Set the GPIO configuration. If the GPIO is not used for LCD,set the direction. Otherwise, they are toggled with LCD signals.
#define cmd_get_gpio_conf 0xB9 //Get the current GPIO configuration
#define cmd_set_gpio_value 0xBA //Set GPIO value for GPIO configured as output
#define cmd_get_gpio_status 0xBB //Read current GPIO status. If the individual GPIO was configured as input, the value is the status of the corresponding pin. Otherwise, it is the programmed value.
#define cmd_set_post_proc 0xBC  //Set the image post processor
#define cmd_get_post_proc 0xBD //Set the image post processor
#define cmd_set_pwm_conf 0xBE //Set the image post processor
#define cmd_get_pwm_conf 0xBF //Set the image post processor
#define cmd_set_lcd_gen0 0xC0 //Set the rise, fall, period and toggling properties of LCD signal generator 0
#define cmd_get_lcd_gen0 0xC1 //Get the current settings of LCD signal generator 0
#define cmd_set_lcd_gen1 0xC2 //Set the rise, fall, period and toggling properties of LCD signal generator 1
#define cmd_get_lcd_gen1 0xC3 //Get the current settings of LCD signal generator 1
#define cmd_set_lcd_gen2 0xC4 //Set the rise, fall, period and toggling properties of LCD signal generator 2
#define cmd_get_lcd_gen2 0xC5 //Get the current settings of LCD signal generator 2
#define cmd_set_lcd_gen3 0xC6 //Set the rise, fall, period and toggling properties of LCD signal generator 3
#define cmd_get_lcd_gen3 0xC7 //Get the current settings of LCD signal generator 3
#define cmd_set_gpio0_rop 0xC8 //Set the GPIO0 with respect to the LCD signal generators using ROP3 operation. No effect if the GPIO0 is configured as general GPIO. 
#define cmd_get_gpio0_rop 0xC9 //Get the GPIO0 properties with respect to the LCD signal generators.
#define cmd_set_gpio1_rop 0xCA //Set the GPIO1 with respect to the LCD signal generators using ROP3 operation. No effect if the GPIO1 is configured as general GPIO.
#define cmd_get_gpio1_rop 0xCB  //Get the GPIO1 properties with respect to the LCD signal generators.
#define cmd_set_gpio2_rop 0xCC //Set the GPIO2 with respect to the LCD signal generators using ROP3 operation. No effect if the GPIO2 is configured as general GPIO.
#define cmd_get_gpio2_rop 0xCD //Get the GPIO2 properties with respect to the LCD signal generators.
#define cmd_set_gpio3_rop 0xCE //Set the GPIO3 with respect to the LCD signal generators using ROP3 operation. No effect if the GPIO3 is configured as general GPIO.
#define cmd_get_gpio3_rop 0xCF //Get the GPIO3 properties with respect to the LCD signal generators.
#define cmd_set_dbc_conf 0xD0 //Set the dynamic back light configuration
#define cmd_get_dbc_conf 0xD1 //Get the current dynamic back light configuration
#define cmd_set_dbc_th 0xD4 //Set the threshold for each level of power saving
#define cmd_get_dbc_th 0xD5 //Get the threshold for each level of power saving
#define cmd_set_pll 0xE0 //Start the PLL. Before the start, the system was operated with the crystal oscillator or clock input
#define cmd_set_pll_mn 0xE2 //Set the PLL
#define cmd_get_pll_mn 0xE3 //Get the PLL settings
#define cmd_get_pll_status 0xE4 //Get the current PLL status
#define cmd_set_deep_sleep 0xE5  //Set deep sleep mode
#define cmd_set_lshift_freq 0xE6 //Set the LSHIFT (pixel clock) frequency
#define cmd_get_lshift_freq 0xE7 //Get current LSHIFT (pixel clock) frequency setting
#define cmd_set_pixel_data_interface 0xF0 //Set the pixel data format of the parallel host processor interface
#define cmd_get_pixel_data_interface 0xF1 //Get the current pixel data format settings

//TFT-LCD面板参数定义
#define DISP_HOR_RESOLUTION 480
#define DISP_HOR_PULSE_WIDTH 41
#define DISP_HOR_BACK_PORCH 2
#define DISP_HOR_FRONT_PORCH 2 
#define DISP_VER_PULSE_WIDTH 10
#define DISP_VER_BACK_PORCH 2
#define DISP_VER_FRONT_PORCH 2
#define DISP_VER_RESOLUTION 272
#define GetMaxX() 	DISP_HOR_RESOLUTION
#define GetMaxY() DISP_VER_RESOLUTION

//显示数据格式定义
#define RGB_DATA_FORMAT_16 0x50  //565
#define RGB_DATA_FORMAT_18 0x60  //666
#define RGB_DATA_FORMAT_24 0x70  //888

//---------------------------------
#ifndef _IN_TFT_

extern uint8 	xdata WardDataBuff[];
extern union union_type xdata unOperData;
extern uint8 code ColonCode16x32[64];

extern void Send_Ssd1963_Command(uint8  ssd1963_command);
extern void Send_Ssd1963_Data(uint8  ssd1963_data);
extern void InitSSD1963(void);
extern void Set_Display_Area(uint16  start_x,uint16  end_x,uint16  start_y,uint16  end_y);

extern void Display_Dot(uint16 x,uint16 y,uint16 color,uint8 page);
//extern void Display_Dot1(uint16  x,uint16  y,uint8 srgb,uint8 page);
extern void Display_HLine(uint16 x,uint16 y,uint16 width,uint16 color,uint8 page);
extern void Display_VLine(uint16 x,uint16 y,uint16 height,uint16 color,uint8 page);
extern void Lcd_Clear(uint16 color,uint8 page);
extern void DisplayBlock(uint16 start_x,uint16 start_y,uint16 end_x,uint16 end_y, uint16 color,uint8 page);
extern void DisplayFrame(uint16 start_x,uint16 start_y,uint16 end_x,uint16 end_y, uint16 color,uint8 page);


extern void DisplayDigitString(uint16 *px,uint16 *py,uint8 *pDigit,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);
extern void DisplayASCIIString(uint16 *px,uint16 *py,uint8 *pASCII,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);

extern void Display_Chinese(uint16 x,uint16 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);
extern void Display_ASCII(uint16 x,uint16 y,uint8 *p,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);



extern void ShowBroadFace(uint8 byRoomAddr, uint8 byBedAddr, bit bSelf,uint8 page);
//extern void ShowCallFace(uint8 byRoomAddr, uint8 byBedAddr, uint8 byCallType,uint8 page);

extern void ShowTalkFace(uint8 byRoomAddr, uint8 byBedAddr,uint8 page);

extern void ShowStartInfusionCall(uint8 page);
extern void ShowStartServiceCall(uint8 page);
extern void ShowStartEmergencyCall(uint8 page);
extern void ShowStartHelpCall(uint8 page);
extern void ShowCallSuccess(uint8 page);

extern void ShowKeyFace(uint8 page);
extern void StorSecInfo(uint8 page);

extern void DisplayDigit(uint16 x,uint16 y,uint8 Digit,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);
extern void DisplayASCII(uint16 x,uint16 y,uint8 ASCII,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);
extern void DisplayChineseString(uint16 *px,uint16 *py,uint8 *pChinese,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);
extern void DisplayString(uint16 *px,uint16 *py,uint8 *pDispData,uint8 Length,uint16 fcolor,uint16 bcolor,uint8 size,uint8 page);


extern void Set_VisualPage(uint8 VisualPage);
extern void ShowStartNumSet(uint8 page);
extern void ShowNumSetOk(uint8 page);
extern void ShowPowerUpFace(uint8 page);
extern void ShowMainMenuFace(uint8 page);

extern void ShowTimeSet(uint8 page);
extern void ShowNumberSet(uint8 page);
extern void ShowSelfNumberSet(uint8 page);
extern void ShowVoiceTimerSet(uint8 page);
extern void ShowBackLightTimerSet(uint8 page);
extern void ShowCalculateFace(uint8 page);
extern void ShowSecInfoSet(uint8 page);
extern void StoreSecInfo(uint8 page);

extern void ShowTouchAdjust(uint8 page);


extern void DispManager(void);
extern void ShowCallFace0(uint8 page);
extern void ShowCallFace1(uint8 page);
extern void ShowCallFace2(uint8 page);

extern void ShowNumberSetFace(uint8 page);
extern void ShowBedFjNumberSet(uint8 page);
extern void ShowWcDoorFjNumberSet(uint8 page);
extern void ShowMoveFjNumberSet(uint8 page);
extern void ShowAddFjNumberSet(uint8 page);
extern void ShowNumberPWInputFace(uint8 page);

extern void ShowMaxVolSetFace(uint8 page);
#endif


#endif
