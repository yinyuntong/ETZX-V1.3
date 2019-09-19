/**********************************************************************************************
本模声负责将显示缓冲区的显示数据发送到LED点阵模上，以实现动态扫描显示的功能
发送采用两个SSP口同步发送。
显示功能全部在中断中完成
显示方式：常态下显示时间（小字号显示日期及状态，大字号显示时间；或小字号显示日期时间，大字号显示公布的信息）,有呼叫时(含普通呼叫、紧急呼叫、输液报警)，显示呼叫信息
(小字号显示日期及时间和状态，大字号显示呼叫信息)，有护士到位时，显示到位信息（小字号显示日期及时间，大字号显示到位信息）
***********************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "led.h"
#include "spi.h"
#include "uart.h"
#include "string.h"

//显示区域
SCREEN screen; //屏幕定义

#define SetDispLineA() (LPC_GPIO0->DATA|=(1<<7))
#define SetDispLineB() (LPC_GPIO2->DATA|=(1<<9))
#define SetDispLineC() (LPC_GPIO2->DATA|=(1<<10))
#define SetDispLineD() (LPC_GPIO2->DATA|=(1<<2))
#define SetDispRCLK() (LPC_GPIO0->DATA|=(1<<6))

#define ClrDispLineA() (LPC_GPIO0->DATA&=~(1<<7))
#define ClrDispLineB() (LPC_GPIO2->DATA&=~(1<<9))
#define ClrDispLineC() (LPC_GPIO2->DATA&=~(1<<10))
#define ClrDispLineD() (LPC_GPIO2->DATA&=~(1<<2))
#define ClrDispRCLK()  (LPC_GPIO0->DATA&=~(1<<6))

//LOGO
const INT8U Eter_logo[128/8*32]	= {
/* 图像     128x32    */
0xFF,0xFC,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xC0,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFC,0x00,0x00,0x78,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xF8,0x00,0x00,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xF0,0x00,0x00,0x71,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xF0,0x00,0x00,0x77,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xE0,0x07,0xF0,0x7F,0x9F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xC0,0x1F,0xFC,0x7F,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xC0,0x3F,0xF8,0x7C,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x80,0x7F,0xE0,0x70,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x80,0xFF,0xC0,0x70,0x3F,0x80,0x00,0x80,0x00,0x80,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,
0x80,0xFF,0x00,0x70,0x7F,0x00,0x01,0x00,0x01,0x00,0x03,0xFF,0xFF,0xFF,0xE7,0xCF,
0x01,0xFE,0x00,0x71,0xFF,0x00,0x03,0x00,0x02,0x00,0x01,0xFF,0xFF,0xFF,0x05,0x87,
0x01,0xFE,0x00,0x73,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x81,0xFF,0xFF,0xFF,0x00,0x01,
0x01,0xFE,0x00,0xFF,0xFF,0x83,0xFF,0xF0,0x3F,0xFF,0xC1,0xFF,0xFF,0xFF,0x00,0x01,
0x01,0xFE,0x03,0xFF,0xFF,0x83,0xFF,0xF0,0x3F,0xFF,0xC1,0xFF,0xFF,0xFF,0x07,0xCF,
0x01,0xFE,0x07,0xFF,0xFF,0x83,0xFF,0xF0,0x3F,0xFF,0xC1,0xFF,0xFF,0xFF,0x20,0x01,
0x01,0xFE,0x1F,0xFE,0xFF,0x80,0x03,0xF0,0x3F,0xFF,0x83,0xF8,0x00,0x07,0x20,0x01,
0x80,0xFE,0x7F,0x80,0xFF,0x80,0x03,0xF0,0x3F,0x80,0x03,0xF8,0x00,0x0F,0xE3,0xE3,
0x80,0xFE,0xFF,0x80,0xFF,0x80,0x03,0xF0,0x3F,0xC0,0x0F,0xF8,0x00,0x0F,0xC7,0xE1,
0x80,0x7F,0xFF,0x01,0xFF,0x83,0xFF,0xF0,0x3F,0xF0,0x3F,0xFF,0xFF,0xFF,0xC4,0x01,
0xC0,0x3F,0xFE,0x01,0xFF,0x83,0xFF,0xF0,0x3F,0xF8,0x1F,0xFF,0xFF,0xFF,0x84,0x01,
0xC0,0x1F,0xFC,0x03,0xFF,0x83,0xFF,0xF0,0x3F,0xFC,0x0F,0xFF,0xFF,0xFF,0x06,0x63,
0xE0,0x07,0xF0,0x03,0xFF,0x80,0x00,0xF0,0x3F,0xFE,0x03,0xFF,0xFF,0xFF,0x26,0x23,
0xE0,0x00,0x00,0x07,0xFF,0x80,0x00,0xF0,0x3F,0xFF,0x01,0xFF,0xFF,0xFF,0xE4,0x23,
0xF0,0x00,0x00,0x0F,0xFF,0x80,0x00,0xF0,0x3F,0xFF,0x80,0xFF,0xFF,0xFF,0xE6,0x43,
0xFC,0x00,0x00,0x0F,0xFF,0x80,0x00,0xF0,0x3F,0xFF,0xE0,0x7F,0xFF,0xFF,0xEF,0xC7,
0xFE,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xC0,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFC,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};
//正常图标
const INT8U Icon_ok[8]={
/* 图像       8x8     */
//0xC3,0xBD,0x7E,0x5A,0x7E,0x66,0xBD,0xC3,/*"未命名文件",0*/
0xC3,0x0D,0x7E,0x5A,0x7E,0x66,0xBD,0xC3
};
//错误图标
const INT8U Icon_error[8]={
/* 图像       8x8     */
0xC3,0xBD,0x7E,0x42,0x42,0x7E,0xBD,0xC3

};
//本机故障
const INT8U Icon_failure[8]={
0xC3,0xBD,0x5A,0x66,0x66,0x5A,0xBD,0xC3,/*"未命名文件",0*/
};
//总线错误
const INT8U Icon_buserror[8]={
0xBF,0xDF,0x81,0x34,0x81,0xFB,0xFD,0xFF,/*"未命名文件",0*/
};


const unsigned char FontDots16[3][34] =    {
   {0xCA, 0XB1, //GBK内码
/*   	0x00, 0x00, 0x00, 0x0C, 0x7E, 0x0C, 0x66, 0x0C, 
	0x66, 0x0C, 0x67, 0xFF, 0x66, 0x0C, 0x7E, 0xCC, 
	0x66, 0xEC, 0x66, 0x6C, 0x66, 0x3C, 0x7E, 0x0C, 
	0x66, 0x0C, 0x60, 0x0C, 0x00, 0x0C, 0x00, 0x78},	//时*/
	0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x3C, 0x08, 
	 0x25, 0xFE, 0x24, 0x08, 0x24, 0x08, 0x3C, 0x88, 
	 0x24, 0x48, 0x24, 0x48, 0x24, 0x08, 0x3C, 0x08, 
	 0x24, 0x08, 0x00, 0x08, 0x00, 0x38, 0x00, 0x00},	//时	

{   0xB7, 0XD6,  //GBK内码

/* 	0x00, 0x00, 0x00, 0x60, 0x03, 0x30, 0x06, 0x30, 
  	0x0E, 0x18, 0x0C, 0x1C, 0x18, 0x0C, 0x3F, 0xFF, 
  	0x73, 0x0F, 0x03, 0x0C, 0x03, 0x0C, 0x06, 0x0C, 
  	0x0E, 0x0C, 0x1C, 0x18, 0x38, 0x78, 0x00, 0x00},  //分*/
    0x00, 0x00, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 
	0x04, 0x10, 0x08, 0x08, 0x10, 0x04, 0x2F, 0xF2, 
	0x02, 0x10, 0x02, 0x10, 0x02, 0x10, 0x04, 0x10, 
	0x08, 0x10, 0x10, 0x10, 0x20, 0x60, 0x00, 0x00},  //分

{   0xC3, 0xEB,   //GBK内码
/*	0x00, 0x00, 0x03, 0x18, 0x3E, 0x18, 0x0C, 0x18, 
	0x0C, 0xDE, 0x7F, 0xDE, 0x1C, 0xDB, 0x1F, 0x9B, 
	0x1F, 0x98, 0x3F, 0x1E, 0x3C, 0x1E, 0x6C, 0x0C, 
	0x0C, 0x18, 0x0C, 0x70, 0x0F, 0xC0, 0x00, 0x00} 	//秒*/
	 0x00, 0x00, 0x03, 0x10, 0x1C, 0x10, 0x04, 0x10, 
	 0x04, 0x54, 0x3F, 0x52, 0x04, 0x52, 0x0C, 0x90, 
	 0x0E, 0x14, 0x15, 0x14, 0x14, 0x08, 0x24, 0x08, 
	 0x04, 0x10, 0x04, 0x20, 0x04, 0x40, 0x00, 0x00}	//秒   
};




INT8U first=1;

/*********************************************************************************************************
** Function name:       SSP 0,1 PinInit
** Descriptions:        SPI0\SSP1引脚初始化函数 只要 SSP0-MOSI|CLK SSP1-MOSI
** input parameters:     无
** output parameters:    无
*********************************************************************************************************/
void LED_Init(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                              /* 初始化GPIO AHB时钟           */

	//行选择输出(ABCD)(P0.7 P2.9 P2.10 P2.2)    (RCLK) P0.6
    LPC_IOCON->PIO0_7 &= ~0x07;
	LPC_IOCON->PIO2_9 &= ~0x07;
	LPC_IOCON->PIO2_10 &= ~0x07;
	LPC_IOCON->PIO2_2 &= ~0x07;
	LPC_IOCON->PIO0_6 &= ~0x07;
	//初始化输出
	LPC_GPIO0->DIR    |= (1<<6)|(1<<7);                       	/* 初始化为输出功能     */
	LPC_GPIO2->DIR    |= ((1<<2)|(1<<9)|(1<<10));          /* 初始化为输出功能     */


	//列数据输出(SSP0 MOSI SCK)(SSP1 MOSI)
    LPC_IOCON->SWCLK_PIO0_10 |= 0x02;  //SCK0
    LPC_IOCON->PIO0_9 |= 0x01;   //MOSI0
	LPC_IOCON->PIO2_3 |= 0X02; //MOSI1

	//初始化SSP0 MASTER
	LPC_SYSCON->PRESETCTRL      |= 0x01;                                /* 禁止SPI0复位                 */
    LPC_SYSCON->SYSAHBCLKCTRL   |= (1 << 11);                           /* 打开SPI0外设                 */
    LPC_IOCON->SCK_LOC         = 0x00;                                  /*  P0.10配置为SCK              */
    LPC_SYSCON->SSP0CLKDIV       = 0x09;                                /* SSP时钟分频                  */

    LPC_SSP0->CR0 = (1 << 8) |                                       /* SCR  设置SPI时钟分频         */
                    (0x01 << 7) |                                       /* CPHA 时钟输出相位,           */
                                                                        /* 仅SPI模式有效                */
                    (0x01 << 6) |                                       /* CPOL 时钟输出极性,           */
                                                                        /* 仅SPI模式有效                */
                    (0x00 << 4) |                                       /* FRF  帧格式 00=SPI,01=SSI,   */
                                                                        /* 10=Microwire,11=保留         */
                    //(0x0F << 0);                                        /* DSS  数据长度,0000-0010=保留,*/
                    (0x07 << 0);                                        /* DSS  数据长度,0111:8位*/
                                                                        /* 0011=4位,0111=8位,1111=16位  */
    LPC_SSP0->CR1 = (0x00 << 3) |                                       /* SOD  从机输出禁能,1=禁止     */
                    (0x00 << 2) |                                       /* MS   主从选择,0=主机,1=从机  */
                    (0x01 << 1) |                                       /* SSE  SSP使能                 */
                    (0x00 << 0);                                        /* LBM  回写模式                */

 //   LPC_SSP0->CPSR = 2;                                                 /* PCLK分频值                   */
	 LPC_SSP0->CPSR = 4; 
	 LPC_SSP0->ICR  = 0x03;                                              /* 中断清除寄存器               */


	//初始化SSP1 MASTER
	LPC_SYSCON->PRESETCTRL      |= 0x04;                                /* 禁止SPI1复位                 */
    LPC_SYSCON->SYSAHBCLKCTRL   |= (1 << 18);                           /* 打开SPI1外设                 */
    LPC_SYSCON->SSP1CLKDIV       = 0x09;                                /* SSP时钟分频                  */

    LPC_SSP1->CR0 = (1 << 8) |                                       /* SCR  设置SPI时钟分频         */
                    (0x01 << 7) |                                       /* CPHA 时钟输出相位,           */
                                                                        /* 仅SPI模式有效                */
                    (0x01 << 6) |                                       /* CPOL 时钟输出极性,           */
                                                                        /* 仅SPI模式有效                */
                    (0x00 << 4) |                                       /* FRF  帧格式 00=SPI,01=SSI,   */
                                                                        /* 10=Microwire,11=保留         */
                    //(0x0F << 0);                                        /* DSS  数据长度,0000-0010=保留,*/
                      (0x07 << 0);                                        /* DSS  数据长度,0111:8位*/
                                                                        /* 0011=4位,0111=8位,1111=16位  */

    LPC_SSP1->CR1 = (0x00 << 3) |                                       /* SOD  从机输出禁能,1=禁止     */
                    (0x00 << 2) |                                       /* MS   主从选择,0=主机,1=从机  */
                    (0x01 << 1) |                                       /* SSE  SSP使能                 */
                    (0x00 << 0);                                        /* LBM  回写模式                */

//    LPC_SSP1->CPSR = 2;                                                 /* PCLK分频值                   */
    LPC_SSP1->CPSR = 4;                                                 /* PCLK分频值                   */
    LPC_SSP1->ICR  = 0x03;                                              /* 中断清除寄存器               */


    //开启SSP0中断
	//NVIC_EnableIRQ(SSP0_IRQn);
    //NVIC_SetPriority(SSP0_IRQn, 2);                                     /*  中断使能并设置优先级        */
    //LPC_SSP0->IMSC |= 0x03;                                        /*  中断使能寄存器              */

	//初始化显示控制参数
	screen.DISP_area.p1.x=0;
	screen.DISP_area.p1.y=0;
	screen.DISP_area.p2.x=LED_WIDTH-1;
	screen.DISP_area.p2.x=LED_HEIGHT-1;
	screen.DISP_x=0;
	screen.DISP_y=0;
	screen.OP_area.p1.x=0;
	screen.OP_area.p1.y=0;
	screen.OP_area.p2.x=BUF_WIDTH-1;
	screen.OP_area.p2.y=BUF_HEIGHT-1;
	screen.OP_point.x=screen.OP_area.p1.x;
	screen.OP_point.y=screen.OP_area.p1.y;
	screen.DISP_frame = 0;
	screen.OP_frame = 0;
	screen.RunTime_frame = 0;
	screen.DISP_first=0;
}
//清屏操作
void ClsScreen(INT8U color)
{
	if(color == RED)
		memset((void*)screen.frame[screen.OP_frame].buf,0xff,BYTES_SCREEN);
	else if(color == GREEN)
		memset((void*)(screen.frame[screen.OP_frame].buf+BYTES_SCREEN),0xff,BYTES_SCREEN);
	else if(color == YELLOW)
		memset((void*)screen.frame[screen.OP_frame].buf,0xff,BYTES_SCREEN*2);
}


//start: 取值0----15;
//end:取值   1－16
//清屏操作
void ClsScreenForLine(INT8U start, INT8U end,INT8U color) 
{
	if((color ==RED)|| (color == YELLOW))
		memset((void*)(screen.frame[screen.OP_frame].buf+(INT32U)(start*BYTES_LINE)),0xff,BYTES_LINE*(end-start));
	if((color == GREEN)||(color == YELLOW))
		memset((void*)(screen.frame[screen.OP_frame].buf+BYTES_SCREEN+start*BYTES_LINE),0xff,BYTES_LINE*(end-start));
}



//满屏操作
void FullScreen(INT8U color)
{
	if((color ==RED)|| (color == YELLOW))
		memset((void*)screen.frame[screen.OP_frame].buf,0x00,BYTES_SCREEN);
	if((color ==GREEN)|| (color == YELLOW))
		memset((void*)(screen.frame[screen.OP_frame].buf+BYTES_SCREEN),0x00,BYTES_SCREEN);
}


//显示当前行内容
void ShowNextLine(void)
{
	INT8U  line = screen.DISP_y;
	SetDispRCLK();
	if ((line&0x01)==0) ClrDispLineA();
	else SetDispLineA();
	if ((line&0x02)==0) ClrDispLineB();
	else SetDispLineB();
	if ((line&0x04)==0) ClrDispLineC();
	else SetDispLineC();
	if ((line&0x08)==0) ClrDispLineD();
	else SetDispLineD();
	ClrDispRCLK();
	screen.DISP_y++;
	if (screen.DISP_y>=LED_HEIGHT )
	{
		screen.DISP_y=0;
		//表示重新开始一屏的显示
		//更新执行的显示区
		screen.RunTime_frame = screen.DISP_frame;
	} 
}


#if 0
//送显示数据给SSP控制器
void PutDataToSSP(void)
{
	static INT16U * data0,*data1,*data2,*data3;
	INT16U tmp,tmp1;
	INT8U i;
	if (screen.DISP_first==0) //第一屏
	{
		//if (screen.DISP_x==0)
		//{
			data2 = data0 = (INT16U*)(screen.frame[screen.RunTime_frame].buf+(INT32U)((screen.DISP_y+1)*BYTES_LINE+screen.DISP_x/8)-2);
//			data2 = data0 = (INT16U*)(screen.frame[screen.RunTime_frame].buf+(INT32U)((screen.DISP_y)*BYTES_LINE+screen.DISP_x/8));

			data3 = data1 = (INT16U*)(data0+(BYTES_LINE*LED_HEIGHT)/2);	//绿色数据
		//}
		while(LPC_SSP0->SR&0x14) {tmp = LPC_SSP0->DR; tmp = LPC_SSP1->DR;} //清空接收FIFO
		//写入数据
		//while(LPC_SSP0->SR&0x02)
		for (i=0;i<6;i++)	// 1行共12字节数据
		{
			tmp = *data0--;
			tmp1 = *data1--;
//			tmp = *data0++;
//			tmp1 = *data1++;
			LPC_SSP1->DR = tmp1;
			LPC_SSP0->DR = tmp;
					                                               
		}
		//一行点阵显示已完成
		screen.DISP_x=0;
		screen.DISP_first=1;
	}
	else   //第二屏(反面屏)
	{
		while(LPC_SSP0->SR&0x14) {tmp = LPC_SSP0->DR; tmp = LPC_SSP1->DR;} //清空接收FIFO
		//写入数据
		for (i=0;i<6;i++)
		{
			tmp = *data2--;
			tmp1 = *data3--;
//			tmp = *data2++;
//			tmp1 = *data3++;
			LPC_SSP1->DR = tmp1;
			LPC_SSP0->DR = tmp;		                                               
		}
		screen.DISP_first = 0;
	}
 }
#endif

void PutDataToSSP(void)
{
	static INT8U * data0,*data1,*data2,*data3;
	INT16U tmp,tmp1;
	INT8U i;
	if (screen.DISP_first==0) //第一屏
	{
		if(first==1)
		{
			//if (screen.DISP_x==0)
			//{
				data2 = data0 = (screen.frame[screen.RunTime_frame].buf+(INT32U)((screen.DISP_y)*BYTES_LINE+screen.DISP_x/8));

				data3 = data1 = (data0+(BYTES_LINE*LED_HEIGHT));	//绿色数据
			//}
			while(LPC_SSP0->SR&0x14) {tmp = LPC_SSP0->DR; tmp = LPC_SSP1->DR;} //清空接收FIFO
			//写入数据
			//while(LPC_SSP0->SR&0x02)
			for (i=0;i<8;i++)	// 1行共12字节数据
			{

				tmp = *data0++;
				tmp1 = *data1++;
				LPC_SSP1->DR = tmp1;
				LPC_SSP0->DR = tmp;
						                                               
			}
			
			first=0;
		}
		else 
		{
			while(LPC_SSP0->SR&0x14) {tmp = LPC_SSP0->DR; tmp = LPC_SSP1->DR;} //清空接收FIFO
			//写入数据
			for (i=0;i<4;i++)
			{

				tmp = *data0++;
				tmp1 = *data1++;
				LPC_SSP1->DR = tmp1;
				LPC_SSP0->DR = tmp;		                                               
			}
			first=1;
			screen.DISP_x=0;
			screen.DISP_first=1;
			//一行点阵显示已完
		}

	}
	else   //第二屏(反面屏)
	{
		if(first==1)
		{
			while(LPC_SSP0->SR&0x14) {tmp = LPC_SSP0->DR; tmp = LPC_SSP1->DR;} //清空接收FIFO
			//写入数据
			for (i=0;i<8;i++)
			{

				tmp = *data2++;
				tmp1 = *data3++;
				LPC_SSP1->DR = tmp1;
				LPC_SSP0->DR = tmp;		                                               
			}
			first=0;
		}
		else
		{
			while(LPC_SSP0->SR&0x14) {tmp = LPC_SSP0->DR; tmp = LPC_SSP1->DR;} //清空接收FIFO
			//写入数据
			for (i=0;i<4;i++)
			{

				tmp = *data2++;
				tmp1 = *data3++;
				LPC_SSP1->DR = tmp1;
				LPC_SSP0->DR = tmp;		                                               
			}
			first=1;			
			screen.DISP_first = 0;
		}
	}
 }

/*********************************************************************************************************
** Function name:       SSP0_IRQHandler
** Descriptions:        SPI0中断服务函数
** input parameters:    无
** output parameters:   无
*********************************************************************************************************/
#if 0
void SSP0_IRQHandler (void)
{
//	static INT8U i=0;
	LPC_SSP0->ICR = 0x03;                                               /*  清除标志位                  */
    if (screen.DISP_x!=0) PutDataToSSP();
	//uartSendByte(i++);
}
#endif

//设置点阵操作区域
void SetArea(INT8U x1,INT8U y1,INT8U x2,INT8U y2)
{
	 screen.OP_area.p1.x = x1;
	 screen.OP_area.p1.y = y1;
	 screen.OP_area.p2.x = x2;
	 screen.OP_area.p2.y = y2;
	 screen.OP_point.x = x1;
	 screen.OP_point.y = y1;
}



//画直线
void DrawLine(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U color)
{
	if (x1==x2) //是直线
	{
		if (y1<=y2)
		{
		 	while (y1<=y2)
			{
				SetPixel(x1,y1++,1,color);
			}
		}
		else
		{
			while (y2<=y1)
			{
				SetPixel(x1,y2++,1,color);
			}
		}	
	}
	else if(y1==y2)
	{
		if (x1<=x2)
		{
			while (x1<=x2)
			{
				SetPixel(x1++,y1,1,color);
			}
		}
		else
		{
			while (x2<=x1)
			{
				SetPixel(x2++,y1,1,color);
			}
		}
	}
}




//画矩形
void DrawRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U color)
{
	DrawLine(x1,y1,x2,y1,color);
	DrawLine(x2,y1,x2,y2,color);
	DrawLine(x1,y2,x2,y2,color);
	DrawLine(x1,y1,x1,y2,color);
}



//清除矩形区域
void ClearRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2,INT8U color)
{
	INT8U i;
	while (y1<=y2)
	{
		for (i=x1;i<=x2;i++)  SetPixel(i,y1,0,color);
		y1++;
	}
}



//操作显示像素
void SetPixel(INT8U x,INT8U y, INT8U data,INT8U color)
{
	INT8U *tmp,index;
	if( (color == RED) || (color == YELLOW) )
	{	
		tmp = screen.frame[screen.OP_frame].buf+(INT32U)(y*BYTES_LINE+x/8);
		index = x%8;
		if (data) *tmp&=~(1<<index);
		else *tmp|=(1<<index);
	}
	if( (color == GREEN) || (color == YELLOW) )
	{
		tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(INT32U)((y*BYTES_LINE)+x/8);
		index = x%8;
		if (data) *tmp&=~(1<<index);
		else *tmp|=(1<<index);	
	}
}



//送数据至显示
void PutPixelDatas(INT8U *buf,INT16U len,INT8U color)
{
	INT8U i,*tmp,index;
	INT8U *TempBuf = buf;
	INT16U TempLen = len;
	if((color == RED)||(color== YELLOW))
	{
		tmp = screen.frame[screen.OP_frame].buf+(BYTES_LINE*screen.OP_point.y)+screen.OP_point.x/8;
		index = screen.OP_point.x%8;
		while(len--)
		{
			for (i=0; i<8; i++)
			{
					if (*buf&(0x80>>i)) *tmp|=(1<<index);
					else *tmp&=~(1<<index);
					screen.OP_point.x++;
					if (screen.OP_point.x>screen.OP_area.p2.x)
					{
//						uartSendByte(*tmp);
						screen.OP_point.x=screen.OP_area.p1.x;
						screen.OP_point.y++;
						if (screen.OP_point.y>screen.OP_area.p2.y) screen.OP_point.y = screen.OP_area.p1.y;
						tmp = screen.frame[screen.OP_frame].buf+(BYTES_LINE*screen.OP_point.y)+screen.OP_point.x/8;
						index = screen.OP_point.x%8;
					}
					else
					{
						index = (index+1)%8;
						if (index==0) tmp++;
					}
			}
			buf++;
		}
	}

	buf = TempBuf;
	len =  TempLen;
	if((color == GREEN)||(color== YELLOW))
	{
		tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(BYTES_LINE*screen.OP_point.y)+screen.OP_point.x/8;
		index = screen.OP_point.x%8;
		while(len--)
		{
			for (i=0; i<8; i++)
			{
					if (*buf&(0x80>>i)) *tmp|=(1<<index);
					else *tmp&=~(1<<index);
					screen.OP_point.x++;
					if (screen.OP_point.x>screen.OP_area.p2.x)
					{
						screen.OP_point.x=screen.OP_area.p1.x;
						screen.OP_point.y++;
						if (screen.OP_point.y>screen.OP_area.p2.y) screen.OP_point.y = screen.OP_area.p1.y;
						tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(BYTES_LINE*screen.OP_point.y)+screen.OP_point.x/8;
						index = screen.OP_point.x%8;
					}
					else
					{
						index = (index+1)%8;
						if (index==0) tmp++;
					}
			}
			buf++;
		}	
	}
}



INT16U GetTextShowLength(INT8U * txt,INT8U size)	//横向有多少点
{
	INT8U width=8;
	INT16U length=0,len=0;
	while(*txt)
	{
		len=2;
		if ((*txt<=0x7e)&&(*txt>=0x20)) //ASCII码
		{
			len=1;
			switch(size)
			{				
				case 16:
				default:
					width=8;
					break;
			}
		}
		else if ((*txt==0xaa) &&(*(txt+1)>= 0xA1) && (*(txt+1)<=0xFE))//国标扩展字符
		{
			switch(size)
			{				
				case 16:
				default:
					width=8;
					break;
			}
		}


		
		else if ((*txt>=0x81) && (*txt<=0xfe)&& (*(txt+1)>=0x40) &&(*(txt+1)<=0xfe))  //GBK内码 32X32
		{

			switch(size)
			{
				case 16:
				default:
					width=16;
					break;					
			}
		}

		txt+=len;
		length+=width;		
	}
	return length;
}



//在指定位输出一个文字
void TextOutRed(INT16S x,INT16S y,INT8U * txt,INT8U font,INT8U size)
{
	INT8U buf[130];
	INT8U dat;
	INT16U *code;
	INT32U BaseAddr=0;
	INT8U bufLen=0,len,width=8,act_width,i,j,index,*tmp,begin;
	while(*txt)
	{
	    LPC_WDT->FEED  = 0xAA;                                         // 喂狗                     
	    LPC_WDT->FEED  = 0x55;	
		len=2;
		if ((*txt<=0x7e)&&(*txt>=0x20)) //ASCII码
		{
			len=1;
			if ((*txt<=0x39)&&(*txt>=0x30)) //数字
			{
				len=1;
			
				switch(size)
				{
					case 16:	//8*16
					default:
						switch(font)
						{
						case FONT_DEFAULT:
						default:
							BaseAddr = A8BaseAdd;
							bufLen = 16;
							width=8;
							BaseAddr+=(*txt-0x20)*bufLen;
							break;
						case FONT_BOLD:
							BaseAddr = D8BaseAdd;			//数字粗体
							bufLen =16;
							width =8;
							BaseAddr+=(*txt-0x30)*bufLen;
							break;
						}
						break;
				}
				
			}
			else
			{	//非数字的ASCII码
				switch(size)
				{
					case 16:	//8*16
					default:
						switch(font)
						{
						case FONT_DEFAULT:
						default:
							BaseAddr = A8BaseAdd;
							bufLen = 16;
							width=8;
							break;
						}
						break;
				}
				BaseAddr+=(*txt-0x20)*bufLen;
			}
		}
		else if ((*txt==0xaa) &&(*(txt+1)>= 0xA1) && (*(txt+1)<=0xFE))//国标扩展字符
		{
			code = (INT16U*)txt;
			switch(size)
			{
				case 16: //8*16
				default:
					BaseAddr = A8BaseAdd;
					bufLen = 16;
					width=8;
					break;
			}
			BaseAddr+=(*code-0xaaa1)*bufLen;	
		}

		
		else if ((*txt>=0x81) && (*txt<=0xfe)&& (*(txt+1)>=0x40) &&(*(txt+1)<=0xfe))  //GBK内码 
		{

			switch(size)
			{

				case 16:
				default:
					BaseAddr= C16BaseAdd;
					bufLen=32;
					width=16;
					break;		
			}
			BaseAddr +=( (*txt - 0x81) * 191 + (*(txt+1) - 0x40))*bufLen;
		}

		else 
		{
			txt++;
		}

		if (x+width<0) {txt+=len; x+=width; continue;}


		Read_GL23L32S4W(BaseAddr,bufLen,buf);
	
		act_width = width;
		

		txt+=len; len = 0;

		begin=1;  //设置列开始
		
		if (x<0)
		{
			begin = -x+1;
			SetArea(0,y,x+width-1,y+size-1);
		}
		else
		{
			if (width+x>=LED_WIDTH) width = LED_WIDTH-x;
			SetArea(x,y,x+width-1,y+size-1);
		}
		//送数据至显示
		len +=(begin-1)/8;
		dat = buf[len++];
		tmp = screen.frame[screen.OP_frame].buf+(screen.OP_point.y*BYTES_LINE)+screen.OP_point.x/8;
		index = screen.OP_point.x%8;
		for (i=0;i<size;i++)
		{
			for(j=begin;j<=width;j++)
			{
				if (dat&(0x80>>((j-1)%8))) *tmp&=~(1<<index);
				else *tmp|=(1<<index);
				screen.OP_point.x++;
				if (screen.OP_point.x>screen.OP_area.p2.x)
				{
					screen.OP_point.x=screen.OP_area.p1.x;
					screen.OP_point.y++;
					if (screen.OP_point.y>screen.OP_area.p2.y) screen.OP_point.y = screen.OP_area.p1.y;
					tmp = screen.frame[screen.OP_frame].buf+(screen.OP_point.y*BYTES_LINE)+screen.OP_point.x/8;
					index = screen.OP_point.x%8;
				}
				else
				{
					index = (index+1)%8;
					if (index==0) tmp++;
				}
				//dat<<=1;
				if ((j%8)==0) dat = buf[len++];
			}
			len += (act_width-width)/8;
			len += (begin-1)/8;
			dat = buf[len-1];
			if (((j-1)%8)!=0) dat = buf[len++];
		}
		x+=width;
		if (x>=LED_WIDTH) break;   //超出显示区域		
	}
}


void TextOutGreen(INT16S x,INT16S y,INT8U * txt,INT8U font,INT8U size)
{
	INT8U buf[130];
	INT8U dat;
	INT16U *code;
	INT32U BaseAddr=0;
	INT8U bufLen=0,len,width=8,act_width,i,j,index,*tmp,begin;
	while(*txt)
	{
		len=2;
		if ((*txt<=0x7e)&&(*txt>=0x20)) //ASCII码
		{
			len=1;
			if ((*txt<=0x39)&&(*txt>=0x30)) //数字
			{
				len=1;
			
				switch(size)
				{
					case 16:	//8*16
					default:
						switch(font)
						{
						case FONT_DEFAULT:
						default:
							BaseAddr = A8BaseAdd;
							bufLen = 16;
							width=8;
							BaseAddr+=(*txt-0x20)*bufLen;
							break;
						case FONT_BOLD:
							BaseAddr = D8BaseAdd;			//数字粗体
							bufLen =16;
							width =8;
							BaseAddr+=(*txt-0x30)*bufLen;
							break;
						}
						break;
				}
				
			}	
			else
			{//非数字的ASCII码
				switch(size)
				{
					case 16: //8*16
					default:
						switch(font)
						{
						case FONT_DEFAULT:
						default:
							BaseAddr = A8BaseAdd;
							bufLen = 16;
							width=8;
							break;
						}
						break;
				}
				BaseAddr+=(*txt-0x20)*bufLen;
			}
		}
		else if ((*txt==0xaa) &&(*(txt+1)>= 0xA1) && (*(txt+1)<=0xFE))//国标扩展字符
		{
			code = (INT16U*)txt;
			switch(size)
			{
				case 16: //8*16
				default:
					BaseAddr = A8BaseAdd;
					bufLen = 16;
					width=8;
					break;
			}

			BaseAddr+=(*code-0xaaa1)*bufLen;	
		}

		else if ((*txt>=0x81) && (*txt<=0xfe)&& (*(txt+1)>=0x40) &&(*(txt+1)<=0xfe))  //GBK内码
		{

			switch(size)
			{

				case 16:
				default:
					BaseAddr= C16BaseAdd;;//
					bufLen=32;
					width=16;
					break;		
			}
			BaseAddr +=( (*txt - 0x81) * 191 + (*(txt+1) - 0x40))*bufLen;
		}

		

//		if ((font!=FONT_DEFAULT)&&(*txt<=0x7e)&&(*txt>=0x20)&&(size>=12));   //非等宽字体
//		else
		{
			if (x+width<0) {txt+=len; x+=width; continue;}
		}

		Read_GL23L32S4W(BaseAddr,bufLen,buf);
		
		act_width = width;
/*		if ((font!=FONT_DEFAULT)&&(*txt<=0x7e)&&(*txt>=0x20)&&(size>=12)) //非等宽字体
		{
			width = buf[1];//+size/3;
			txt+=len;
			if (x+width<0) {x+=width; continue;}
			len=2;
			if (size<32) memset((void*)(buf+bufLen),0,act_width*2/8);
			len+= act_width/4;
		}
		else */
		{
			txt+=len; len = 0;
		}
		begin=1;  //设置列开始
		
		if (x<0)
		{
			begin = -x+1;
			SetArea(0,y,x+width-1,y+size-1);
		}
		else
		{
			if (width+x>=LED_WIDTH) width = LED_WIDTH-x;
			SetArea(x,y,x+width-1,y+size-1);
		}
		//送数据至显示
		len +=(begin-1)/8;
		dat = buf[len++];
		tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(screen.OP_point.y*BYTES_LINE)+screen.OP_point.x/8;
		index = screen.OP_point.x%8;
		for (i=0;i<size;i++)
		{
			for(j=begin;j<=width;j++)
			{
				if (dat&(0x80>>((j-1)%8))) *tmp&=~(1<<index);
				else *tmp|=(1<<index);
				screen.OP_point.x++;
				if (screen.OP_point.x>screen.OP_area.p2.x)
				{
					screen.OP_point.x=screen.OP_area.p1.x;
					screen.OP_point.y++;
					if (screen.OP_point.y>screen.OP_area.p2.y) screen.OP_point.y = screen.OP_area.p1.y;
					tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(screen.OP_point.y*BYTES_LINE)+screen.OP_point.x/8;
					index = screen.OP_point.x%8;
				}
				else
				{
					index = (index+1)%8;
					if (index==0) tmp++;
				}
				//dat<<=1;
				if ((j%8)==0) dat = buf[len++];
			}
			len += (act_width-width)/8;
			len += (begin-1)/8;
			dat = buf[len-1];
			if (((j-1)%8)!=0) dat = buf[len++];
		}
		x+=width;
		if (x>=LED_WIDTH) break;   //超出显示区域		
	}
}


//只有显示"时，分，秒"汉字时才能使用这个函数
void TextOutGreen_DotData(INT16S x,INT16S y,INT8U * txt,INT8U font,INT8U size)
{
	INT8U buf[130];
	INT8U dat;
//	INT16U *code;
//	INT32U BaseAddr=0;
//	INT8U bufLen=0;
	INT8U len,width=8,act_width,i,j,index,*tmp,begin;
	while(*txt)
	{
		for(i=0;i<3;i++)
		{
			if((*txt == FontDots16[i][0])&& (*(txt+1)== FontDots16[i][1])) break;
			
		}

		txt += 2;
		
		if(i<3)
		{
			memcpy(buf,&(FontDots16[i][2]),32);
		}
		else  continue;

		width = 16;
		

	

		if (x+width<0) {x+=width; continue;}

		
		act_width =16;
		size =16;
		
		len = 0;
		begin=1;  //设置列开始
		
		if (x<0)
		{
			begin = -x+1;
			SetArea(0,y,x+width-1,y+size-1);
		}
		else
		{
			if (width+x>=LED_WIDTH) width = LED_WIDTH-x;
			SetArea(x,y,x+width-1,y+size-1);
		}
		//送数据至显示
		len +=(begin-1)/8;
		dat = buf[len++];
		tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(screen.OP_point.y*BYTES_LINE)+screen.OP_point.x/8;
		index = screen.OP_point.x%8;
		for (i=0;i<size;i++)
		{
			for(j=begin;j<=width;j++)
			{
				if (dat&(0x80>>((j-1)%8))) *tmp&=~(1<<index);
				else *tmp|=(1<<index);
				screen.OP_point.x++;
				if (screen.OP_point.x>screen.OP_area.p2.x)
				{
					screen.OP_point.x=screen.OP_area.p1.x;
					screen.OP_point.y++;
					if (screen.OP_point.y>screen.OP_area.p2.y) screen.OP_point.y = screen.OP_area.p1.y;
					tmp = screen.frame[screen.OP_frame].buf+BYTES_SCREEN+(screen.OP_point.y*BYTES_LINE)+screen.OP_point.x/8;
					index = screen.OP_point.x%8;
				}
				else
				{
					index = (index+1)%8;
					if (index==0) tmp++;
				}
				//dat<<=1;
				if ((j%8)==0) dat = buf[len++];
			}
			len += (act_width-width)/8;
			len += (begin-1)/8;
			dat = buf[len-1];
			if (((j-1)%8)!=0) dat = buf[len++];
		}
		x+=width;
		if (x>=LED_WIDTH) break;   //超出显示区域		
	}
}





void SetDisplayFrame(INT8U index)
{
	screen.DISP_frame = index;
}

void SetOperateFrame(INT8U index)
{
	screen.OP_frame = index;
}

INT8U GetNextDisplayFrame(void)
{
	return ((screen.DISP_frame+1)%2);
}


