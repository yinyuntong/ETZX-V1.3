/****************************************Copyright (c)****************************************************
程序说明:
此程序为适用于一特之星系统，具双面32*128单色点阵显示功能，可以通过无线或有线方式设置本机信息
可显示时间,呼叫信息，服务信息等
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "spi.h"
#include "E2PROM.h"
#include "led.h"
#include "uart.h"
#include "singlebus.h"
#include "CC1101.h"
#include "Util.h"
#include "string.h"
#include "stdio.h"
#include "main.h"


#define EnableIRQ()  __enable_irq()
#define DisableIRQ() __disable_irq()

//extern INT8U DispColor;
uint8_t curDispNusre=0;
/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
unsigned long    GulSysClock     =  12000000UL;                         /* 记录处理器时钟频率           */

SYSTEM_INFO SystemInfo;


INT8U daysOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};   //一年中月份的天数

INT8U KeyState=0;	//设置区号按键状态
INT8U MySec=1;

INT8U ASCII_BCD[4]={0,0,0,0};

INT8U 	KeyDownDelay	=0x00;

INT8U	KeyAlwaysDownDelay = 0x00;	

INT8U  SaveSecAddrDelay =0X00;



INT16U Num;
INT8U buf[256];


extern STBusFreq stBusDealFreq;

//插入护士信息到护士到位信息表中
void InsertNurse(INT8U NO)
{
	INT8U i,j,buf[20];
	memset((void*)buf,0,20);
	j=0;
	buf[j++] = NO;
	for (i=0;SystemInfo.DisplayInfo.NurseBuf[i]!=0&&i<19;i++)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[i]!=NO)
		{
			buf[j++]=SystemInfo.DisplayInfo.NurseBuf[i];
		}
	}
	memcpy((void*)SystemInfo.DisplayInfo.NurseBuf,(void*)buf,20);
	if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)
	{
/*		if (SystemInfo.DisplayInfo.NurseShowLen>LED_WIDTH); //需要移动
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;*/
	}
	else
	{
		curDispNusre =0;
		SystemInfo.DisplayInfo.state |= DISPLAY_STATE_NURSE;
		SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
	}
}
//删除护士到位信息
void DeleteNurse(INT8U NO)
{
	INT8U i,j,buf[20];
	memset((void*)buf,0,20);
	j=0;
	for (i=0;SystemInfo.DisplayInfo.NurseBuf[i]!=0&&i<19;i++)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[i]!=NO)
		{
			buf[j++]=SystemInfo.DisplayInfo.NurseBuf[i];
		}
	}
	memcpy((void*)SystemInfo.DisplayInfo.NurseBuf,(void*)buf,20);
	if (buf[0]==0)
	{
		SystemInfo.DisplayInfo.state &= ~DISPLAY_STATE_NURSE;
		SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
	}
	else
	{
/*		if (SystemInfo.DisplayInfo.NurseShowLen>LED_WIDTH); //需要移动
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;*/
	}
}

void intMasterEnable(void);                                             /* 打开总中断                   */

/*********************************************************************************************************
** Function name:        WDTInit
** Descriptions:        WDT初始化函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void WDTInit (void)
{
    LPC_SYSCON->WDTCLKSEL      = 0x01;                                  /* 选择WDT时钟源                */
    LPC_SYSCON->WDTCLKUEN      = 0x00;
    LPC_SYSCON->WDTCLKUEN      = 0x01;                                  /* 更新使能                     */
    LPC_SYSCON->WDTCLKDIV      = 0x01;                                  /* 分频为1                      */
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<15);                               /* 打开WDT模块                  */
    LPC_WDT->TC          = 0xFFFFFF;                                      /* 定时时间                     */
    LPC_WDT->MOD         = (0x01 << 0)                                  /* 使能WDT                      */
                          |(0x01 << 1);                                 /* 使能WDT中断                  */
    LPC_WDT->FEED        = 0xAA;                                        /* 喂狗                         */
    LPC_WDT->FEED        = 0x55;
}

/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void KeyGPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
	//测试模式端口初始化
	LPC_IOCON->PIO0_1 &= ~0x07;		   //P0.1
	//初始化输入
	LPC_GPIO0->DIR    &= ~(1<<1);                       	/* 初始化为输入功能     */
}

/**********************************************************
*函数名称			:PIOINT0_IRQHandler	
*函数描述        	:KEY按键所在的端口0中断
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:
*创建日期	 		:
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void PIOINT0_IRQHandler(void)
{
	LPC_GPIO0->IC |= KeyPin;		//复位中断标志
					          
}



/**********************************************************
*函数名称			:Timer0Int	
*函数描述        	:定时器0溢出中断,定时器每93.75us中断一次
					 
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#if 0
void TIMER32_0_IRQHandler (void)
{ 
	static INT16U display_tick=0;
    LPC_TMR32B0->IR = 0x01;                                             /* 清除中断标志                 */
	//显示刷新延时控制
	display_tick++;
	if (display_tick>=9) //完成了1ms 的定时
	{
		display_tick=0;
	   //启动显示程序
	   ShowNextLine();
	   PutDataToSSP();
	}
	else
	{
		if (first ==0 &&(LPC_SSP0->SR&0x10)==0)
			PutDataToSSP();
		
		else if (screen.DISP_first==1&&(LPC_SSP0->SR&0x10)==0)		//tx FIFO为空时，busy位为0
		{
			PutDataToSSP();
		}
	}


	if(KeyState & KEY_UP)
	{
		if(GetKeyValue() ==0)
		{
			KeyState &= ~KEY_UP;
			KeyState |= KEY_DOWN_ACT;
			KeyDownDelay = KEY_DOWN_DELAY;
		}
	}

	else if(KeyState & KEY_DOWN_ACT)
	{
		if(KeyDownDelay)
		{
			KeyDownDelay--;
			if(KeyDownDelay==0)
			{
				KeyState &= ~KEY_DOWN_ACT;
				if(GetKeyValue()==0)
				{
					KeyState |= KEY_DOWNED;

					KeyState |= KEY_DOWNED_HANDLER;
					
					KeyAlwaysDownDelay= KEY_ALWAYS_DOWN_DELAY;
					KeyDownDelay  =  KEY_DOWN_DELAY;

					ClsScreen(YELLOW);

				}
				else
				{
					KeyState &= ~(KEY_DOWN_ACT|KEY_DOWNED|KEY_ALWAYS_DOWN);
					KeyState |= KEY_UP;
				}
				#if 0   //未用中断方式
				NVIC_EnableIRQ(EINT0_IRQn);                                    /* 设置中断并使能               */
				NVIC_SetPriority(EINT0_IRQn, 2);	
				#endif
			}
		}
	}
	else if(KeyState & KEY_DOWNED)
	{
		if(KeyDownDelay)
		{
			KeyDownDelay --;
			if(KeyDownDelay==0)
			{
				
				if(GetKeyValue()==0)
				{
					KeyDownDelay = KEY_DOWN_DELAY;
					if(KeyAlwaysDownDelay)
					{
						KeyAlwaysDownDelay--;
						if(KeyAlwaysDownDelay==0)
						{
							KeyState &= ~ KEY_DOWNED;
							KeyState |= KEY_ALWAYS_DOWN;
							KeyDownDelay = 200;
							KeyAlwaysDownDelay = 25;	//　0.5秒间隔
							KeyState |= KEY_ALWAYS_DOWN_HANDLER;

							ClsScreen(YELLOW);
						}
					}
				}
				else
				{
					KeyState &= ~(KEY_DOWN_ACT|KEY_DOWNED|KEY_ALWAYS_DOWN);
					KeyState  |=  KEY_UP;
					
					ClsScreen(YELLOW);
				}
			}
		}
	}

	else if(KeyState & KEY_ALWAYS_DOWN)
	{
		if(KeyDownDelay)
		{
			KeyDownDelay --;
			if(KeyDownDelay==0)
			{
				if(GetKeyValue()==0)
				{
					KeyDownDelay = 200;
					if(KeyAlwaysDownDelay)
					{
						KeyAlwaysDownDelay--;
						if(KeyAlwaysDownDelay==0)
						{

							KeyAlwaysDownDelay = 25;	//　0.5秒间隔
							KeyState |= KEY_ALWAYS_DOWN_HANDLER;
							
						}
					}
				}
				else
				{
					KeyState &= ~(KEY_DOWN_ACT|KEY_DOWNED|KEY_ALWAYS_DOWN);
					KeyState  |=  KEY_UP;
					
					ClsScreen(YELLOW);
				}				
			}
		}
	}


}

#endif
/*********************************************************************************************************
* Function Name:        TIMER32_0_IRQHandler
* Description:          TIMER0 中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void TIMER32_1_IRQHandler (void)
{
	static INT8U stick=0;
    LPC_TMR32B1->IR = 0x01;                                             /* 清除中断标志                 */
	stick++;
	//33.3ms延时处理

	if (stick>=30)	  //秒级定时
	{
		SystemInfo.error++;
		SystemInfo.DisplayInfo.ChangeDelay++;
		stick=0;
		//时间处理 
		SystemInfo.sTime.second++;
		if(SystemInfo.sTime.second >= 60)
		{
			SystemInfo.sTime.second = 0;
			//分钟增加
			SystemInfo.sTime.minute++;
			if(SystemInfo.sTime.minute >= 60)
			{
				SystemInfo.sTime.minute = 0;
				//小时增加
				SystemInfo.sTime.hour++;							
				if(SystemInfo.sTime.hour >= 24)
				{							
					SystemInfo.sTime.hour = 0;
					SystemInfo.sTime.day++;
					if ((SystemInfo.sTime.month==2&&SystemInfo.sTime.year%4==0&&SystemInfo.sTime.day>(daysOfMonth[SystemInfo.sTime.month]+1))||
						(SystemInfo.sTime.day>daysOfMonth[SystemInfo.sTime.month])) 
					{
						SystemInfo.sTime.day = 1;
						SystemInfo.sTime.month++;
						if (SystemInfo.sTime.month>12)
						{
							SystemInfo.sTime.month = 1;
							SystemInfo.sTime.year = (SystemInfo.sTime.year+1)%100;
						}
					}
	
				}
			}
		}
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_TIME_CHANGED; //时间已改变
	}
	
//	if(stick%2==0)
	{
		SystemInfo.state|=SYSTEM_STATE_MOVING; //通知执行移动
	}
	
	if (stUartControl.byTimerout>1) stUartControl.byTimerout--;	

	if(SaveSecAddrDelay)
	{
		SaveSecAddrDelay --;
		if(SaveSecAddrDelay ==0)
		{
			KeyState &= ~FIRST_KEY_HANDLED;
			if(MySec != SystemInfo.LOCAL_ADDR)
			{
				SystemInfo.LOCAL_ADDR = MySec;
				//eepromWrite((INT8U*)&SystemInfo); //区号有更改，保存区号地址
				Write_Cont(SYS_INFO_ADD,(INT8U*)&SystemInfo,sizeof(SystemInfo));
			}
			ClsScreen(YELLOW);		
		}
	}	
}

/*********************************************************************************************************
** Function name:        timer0Init
** Descriptions:        32位定时器0初始化函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void timer0Init (void)
{
	//初始化定时器T0 作为显示扫描使用
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);								/* 打开定时器模块				*/

	LPC_TMR32B0->IR 	 = 1;
	LPC_TMR32B0->PR 	 = 1000;										/* 设置分频系数 				*/
	LPC_TMR32B0->MCR	 = 3;											/* 设置MR0匹配后复位TC并产生中断*/
	LPC_TMR32B0->MR0	 = SystemFrequency/10000000;					   /* 设置100uS中断时间 			*/
//	LPC_TMR32B0->MR0	 = SystemFrequency/1000000;					    /* 设置1mS中断时间 			*/
	LPC_TMR32B0->TCR	 = 0x01;										/* 启动定时器					*/
	NVIC_EnableIRQ(TIMER_32_0_IRQn);									/* 设置中断并使能				*/
	NVIC_SetPriority(TIMER_32_0_IRQn, 1);
}


/*********************************************************************************************************
** Function name:        timer1Init
** Descriptions:        32位定时器1初始化函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void timer1Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);                              /* 打开定时器模块               */

    LPC_TMR32B1->IR      = 1;
    LPC_TMR32B1->PR      = 1000;                                        /* 设置分频系数                 */
    LPC_TMR32B1->MCR     = 3;                                           /* 设置MR0匹配后复位TC并产生中断*/
    LPC_TMR32B1->MR0     = SystemFrequency/30000;                       /* 设置33.3mS中断时间             */
    LPC_TMR32B1->TCR     = 0x01;                                        /* 启动定时器                   */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);                                    /* 设置中断并使能               */
    NVIC_SetPriority(TIMER_32_1_IRQn, 3);
}

void SaveSystemInfo()
{
	
}

void ShowVersion(INT8U *buf)
{
	sprintf(buf,"V%d.%d",VERSION_HI,VERSION_LO);

	TextOutRed(16,0,buf,FONT_BOLD,16);

}


//显示时间
void ShowTime(INT8U *buf)
{
	if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)!=0
		||(SystemInfo.DisplayInfo.state&0x1f)==0)
	{
		if((KeyState&(KEY_DOWNED|KEY_ALWAYS_DOWN))  || SaveSecAddrDelay)
		{// 有键按下状态，屏上不显示


		}
		else
		{
			sprintf(buf,"%02d",SystemInfo.sTime.hour);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"时");
			TextOutGreen_DotData(16,0,buf,FONT_DEFAULT,16);	

			sprintf(buf,"%02d",SystemInfo.sTime.minute);
			TextOutRed(32,0,buf,FONT_BOLD,16);
			sprintf(buf,"分");
			TextOutGreen_DotData(48,0,buf,FONT_DEFAULT,16);			

			sprintf(buf,"%02d",SystemInfo.sTime.second);
			TextOutRed(64,0,buf,FONT_BOLD,16);
			sprintf(buf,"秒");
			TextOutGreen_DotData(80,0,buf,FONT_DEFAULT,16);	
			

			
/*			sprintf(buf,"%02d  %02d  %02d  ",SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
			ClearRect(0,0,15,1,RED);	//将残余的显示清掉
			TextOutRed(0,2,buf,FONT_DEFAULT,16);

			sprintf(buf,"  时  分  秒");
			TextOutGreen(0,0,buf,FONT_DEFAULT,16);	*/		
		}
//		sprintf(buf,"20%02d-%02d-%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day);
//		TextOut(32,1,buf,FONT_DEFAULT,7);

#if 0
		//显示系统状态
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
		//显示错误信息
#endif


#if 0
		if (SystemInfo.state&SYSTEM_STATE_BUS_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_buserror,8);
		}
		else if (SystemInfo.state&SYSTEM_STATE_LOCAL_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_failure,8);
#endif

	}
#if 0		
	else
	{
		sprintf(buf,"%02d:%02d:%02d",SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOutRed(16,2,buf,FONT_DEFAULT,16);
	
		sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day,SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOut(10,1,buf,FONT_DEFAULT,7);
		//显示系统状态
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
	
	}
#endif		
}


	
//显示呼叫信息
void ShowCallInfo(INT8U * buf)
{
	INT16U len;INT8U size;
	buf[0]=0;


	switch(SystemInfo.DisplayInfo.CallBuf[0])
	{
	case 0x06: //床头呼叫
		if (SystemInfo.DisplayInfo.CallBuf[2]==250)//加床呼叫
		{		
			sprintf(buf,"加");
			TextOutGreen(0,0,buf,FONT_DEFAULT,16);
			if(SystemInfo.DisplayInfo.CallBuf[3]<100)
			{
	            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
				TextOutRed(16,0,buf,FONT_BOLD,16);
				sprintf(buf,"床呼叫");
				TextOutGreen(32,0,buf,FONT_DEFAULT,16);
			}
			else
			{
	            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
				TextOutRed(16,0,buf,FONT_BOLD,16);
				sprintf(buf,"床呼叫");
				TextOutGreen(40,0,buf,FONT_DEFAULT,16);			
			}
		}
		else if(SystemInfo.DisplayInfo.CallBuf[3] == 0x00)
		{
			//sprintf(buf,"%d房门口呼叫",SystemInfo.DisplayInfo.CallBuf[2]);
			if(SystemInfo.DisplayInfo.CallBuf[2]<100)
			{
				sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
				TextOutRed(0,0,buf,FONT_BOLD,16);
				sprintf(buf,"房门口呼叫");
				TextOutGreen(16,0,buf,FONT_DEFAULT,16);
			}
			else
			{
	            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
				TextOutRed(0,0,buf,FONT_BOLD,16);
				sprintf(buf,"门口呼叫");
				TextOutGreen(24,0,buf,FONT_DEFAULT,16);				
			}
		}
		
		else 
		{
			//sprintf(buf,"%d床呼叫",SystemInfo.DisplayInfo.CallBuf[3]);
			if(SystemInfo.DisplayInfo.CallBuf[2]>=100)
			{
				if(SystemInfo.DisplayInfo.CallBuf[3]>=100)
				{//XXX房xxx床呼
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"房");
					TextOutGreen(24,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(40,0,buf,FONT_BOLD,16);
					sprintf(buf,"床呼");
					TextOutGreen(64,0,buf,FONT_DEFAULT,16);						
				}
				else
				{//xxx-xx床呼叫
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"-");
					TextOutGreen(24,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(32,0,buf,FONT_BOLD,16);
					sprintf(buf,"床呼叫");
					TextOutGreen(48,0,buf,FONT_DEFAULT,16);					
				}
			}
			else
			{
				if(SystemInfo.DisplayInfo.CallBuf[3]>=100)
				{//XX-xxx床呼
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"-");
					TextOutGreen(16,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(24,0,buf,FONT_BOLD,16);
					sprintf(buf,"床呼叫");
					TextOutGreen(48,0,buf,FONT_DEFAULT,16);						
				}
				else
				{//xx房xx床呼叫
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"房");
					TextOutGreen(16,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(32,0,buf,FONT_BOLD,16);
					sprintf(buf,"床呼叫");
					TextOutGreen(48,0,buf,FONT_DEFAULT,16);					
				}				
			}
		}
		break;
	case 0x07: //输液呼叫
		//sprintf(buf,"%d床输液完毕",SystemInfo.DisplayInfo.CallBuf[3]);
		if(SystemInfo.DisplayInfo.CallBuf[3]<100)
		{
			sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"床输液完毕");
			TextOutGreen(16,0,buf,FONT_DEFAULT,16);
		}
		else
		{
            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"床输液完");
			TextOutGreen(24,0,buf,FONT_DEFAULT,16);				
		}
		break;
	case 0x08: //卫生间呼叫
		//sprintf(buf,"%d房呼叫",SystemInfo.DisplayInfo.CallBuf[2]);
		if(SystemInfo.DisplayInfo.CallBuf[2]<100)
		{
			sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"房呼叫 ");
			TextOutGreen(16,0,buf,FONT_DEFAULT,16);
		}
		else
		{
            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"房呼叫");
			TextOutGreen(32,0,buf,FONT_DEFAULT,16);				
		}		
		break;	
	}	
}




void ShowNurseInfo(INT8U *buf)
{
	INT16U len;
	INT8U size,*tmp;

	strcpy(buf,"护士位置");
	tmp = buf;
	while(*tmp++);
	tmp--;	
	
	if(curDispNusre==0)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[curDispNusre]!=0)
		{
			sprintf(tmp,"%02d",SystemInfo.DisplayInfo.NurseBuf[curDispNusre]);
			while(*tmp++);
			tmp--;

			sprintf(tmp,"%s","房");
			while(*tmp++);
			tmp--;

			Num = GetNextDisplayFrame();
			SetOperateFrame(Num);
			ClsScreen(YELLOW);
			TextOutRed(0,0,buf,FONT_DEFAULT,16);
			SetDisplayFrame(Num);	

			curDispNusre++;
		}
		else  return;		
	}

	else
	{
		if (SystemInfo.DisplayInfo.NurseBuf[curDispNusre]!=0)
		{
			sprintf(tmp,"%02d",SystemInfo.DisplayInfo.NurseBuf[curDispNusre]);
			while(*tmp++);
			tmp--;

			sprintf(tmp,"%s","房");
			while(*tmp++);
			tmp--;

			Num = GetNextDisplayFrame();
			SetOperateFrame(Num);
			ClsScreen(YELLOW);
			TextOutRed(0,0,buf,FONT_DEFAULT,16);
			SetDisplayFrame(Num);	

			curDispNusre++;

		}
		else
		{
			curDispNusre=0;
			
			if (SystemInfo.DisplayInfo.NurseBuf[curDispNusre]!=0)
			{
				sprintf(tmp,"%02d",SystemInfo.DisplayInfo.NurseBuf[curDispNusre]);
				while(*tmp++);
				tmp--;
				
				sprintf(tmp,"%s","房");
				while(*tmp++);
				tmp--;
				
				Num = GetNextDisplayFrame();
				SetOperateFrame(Num);
				ClsScreen(YELLOW);
				TextOutRed(0,0,buf,FONT_DEFAULT,16);
				SetDisplayFrame(Num); 
				
				curDispNusre++;

			}
			else return;
		}
	}
	
}



//显示服务信息
void ShowMessageInfo(INT8U * buf)
{
	if (SystemInfo.DisplayInfo.MessageShowLen==0)
	{
		 SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16);

	}
	if (SystemInfo.DisplayInfo.MessageShowLen>LED_WIDTH)
	{	
		switch(SystemInfo.DisplayInfo.color)
		{
			case COLOR_RED:
				default:
					TextOutRed(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					break;
			case COLOR_GREEN:
					TextOutGreen(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					break;				
			case COLOR_ORANGE:
					TextOutRed(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					TextOutGreen(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					break;				
					
		}
		SystemInfo.DisplayInfo.MessageIndex = (SystemInfo.DisplayInfo.MessageIndex+1)%(SystemInfo.DisplayInfo.MessageShowLen+LED_WIDTH);

	}
	else
	{
		switch(SystemInfo.DisplayInfo.color)
		{
			case COLOR_RED:
			default:
				TextOutRed((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				break;
			case COLOR_GREEN:
				TextOutGreen((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				break;
			case COLOR_ORANGE:
				TextOutRed((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				TextOutGreen((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				break;				
		}
	}
}


void CC1101_Exint1_Enable(void)
{
	LPC_GPIO1->IE |= cc1101RecPin; //允许中断
	NVIC_EnableIRQ(EINT1_IRQn); 								   /* 设置中断并使能			   */
	NVIC_SetPriority(EINT1_IRQn, 2);
}



void hex_convert(INT8U HexData)
{
	INT8U i;

	ASCII_BCD[0]=HexData/100 +0x30;
	ASCII_BCD[1]=(HexData%100)/10 +0x30;
	ASCII_BCD[2]=(HexData%100)%10 +0x30;
	ASCII_BCD[3]=0;



	if(ASCII_BCD[0]==0x30)
	{
		ASCII_BCD[0]= ASCII_BCD[1];
		ASCII_BCD[1]= ASCII_BCD[2];
		ASCII_BCD[2]=0;	
		if(ASCII_BCD[0]==0x30)
		{
			ASCII_BCD[0]= ASCII_BCD[1];
			ASCII_BCD[1]= 0;			
		}		
	}

}

int main(void)
{

	uint8_t i;
	uint32_t addr;
  	SystemInit();	                                                    /* 系统时钟初始化               */
	KeyGPIOInit();
	KeyState |= KEY_UP;
	memset((void*)&SystemInfo,0,sizeof(SYSTEM_INFO));
	SystemInfo.sTime.year = 12;
	SystemInfo.sTime.month =12;
	SystemInfo.sTime.day = 25;
	SystemInfo.sTime.hour = 14;
	SystemInfo.sTime.minute = 36;
	SystemInfo.sTime.second = 0;
	SystemInfo.DisplayInfo.state =0;
	SystemInfo.DisplayInfo.state |= DISPLAY_STATE_TIME_CHANGED;

	timer1Init();

	//初始化模拟SPI口
	SPI_Init();
    manID=Read_ID(0x00);
	devID=Read_ID(0X01);

	Flash_Lock();


	//读取系统设置信息
	Read_Cont((INT8U*)&SystemInfo,SYS_INFO_ADD,SYSTEM_SAVE_SIZE); 
	
	if (SystemInfo.UartBaud>10)																                                                              
	{
		//初始化为默认值
		SystemInfo.UartBaud = 9; //默认为57600
		SystemInfo.LOCAL_ADDR=1;
		SystemInfo.DisplayInfo.MessageBuf[0]=0;
		Write_Cont(SYS_INFO_ADD,(INT8U*)&SystemInfo,sizeof(SystemInfo));
		Read_Cont((INT8U*)&SystemInfo,SYS_INFO_ADD,SYSTEM_SAVE_SIZE);
	}
	MySec = SystemInfo.LOCAL_ADDR;
	//初始化串口
//	SystemInfo.UartBaud = 9; //默认为57600
	UartInit(SystemInfo.UartBaud);
	uartSendByte(0X55);

/*	uartSendByte(manID);
	uartSendByte(devID);
	FRAM_TEST();*/

	if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
	else
	{
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
	}

	
	cc1101Pin_init();
	CC1101_Init();
	

	//初始化显示输出接口
	LED_Init();
                                         
	//strcpy(SystemInfo.DisplayInfo.MessageBuf,"今天是2009年以来值得庆幸的日子，全国股民集体向2000点遗体告别啦！略微有些悲壮！");
	//SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16)+LED_WIDTH;
	//SystemInfo.DisplayInfo.state |= DISPLAY_STATE_MESSAGE;
	/*SystemInfo.DisplayInfo.CallBuf[0]=8;
	SystemInfo.DisplayInfo.CallBuf[1]=1;
	SystemInfo.DisplayInfo.CallBuf[2]=250;
	SystemInfo.DisplayInfo.CallBuf[3]=153;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CALL;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;*/
	//SystemInfo.DisplayInfo.state=0;

	/*SystemInfo.DisplayInfo.NurseBuf[0]=100;
	SystemInfo.DisplayInfo.NurseBuf[1]=10;
	SystemInfo.DisplayInfo.NurseBuf[2]=3;
	SystemInfo.DisplayInfo.NurseBuf[3]=14;
	SystemInfo.DisplayInfo.NurseBuf[4]=0;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_NURSE;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
	*/

	SetArea(0,0,95,15);
//	PutPixelDatas((INT8U*)Eter_logo,128*32/8);
	
	SetOperateFrame(0);
	ClsScreen(YELLOW);	
	SetOperateFrame(1);	
	ClsScreen(YELLOW);
	SetOperateFrame(0);

	intMasterEnable();        /* 使能中断总开关               */  
	Delay_Ms(3000);
	SingleBusInit();


	SetOperateFrame(0);
	FullScreen(RED);
	SetDisplayFrame(0);
	Delay_Ms(3000);
	ClsScreen(RED);

	
	SetOperateFrame(0);
	FullScreen(GREEN);
	SetDisplayFrame(0);	
	Delay_Ms(3000);
	ClsScreen(GREEN);


	SetOperateFrame(0);
	ShowVersion(buf);
	SetDisplayFrame(0); 
	Delay_Ms(3000);
	ClsScreen(RED);
/*	SystemInfo.DisplayInfo.MessageBuf[0]=0xaa;
	Write_Cont(SYS_INFO_ADD,SystemInfo.DisplayInfo.MessageBuf,1);
	uartSendByte(Read(SYS_INFO_ADD));*/


#if 0
	SetOperateFrame(0);
	FullScreen(YELLOW);
	SetDisplayFrame(0);	
	Delay_Ms(3000);
	ClsScreen(YELLOW);
#endif


	first=1;


#if 0
	SetOperateFrame(0);
	DrawLine(3,0,46,0,RED);
	SetDisplayFrame(0);	
	Delay_Ms(2000);	
	ClsScreenForLine(0,1,RED);

	SetOperateFrame(0);
	DrawRect(0,0,39,8,GREEN);
	SetDisplayFrame(0);	
	Delay_Ms(2000);	
	ClearRect(0,0,39,8,GREEN);


	SetArea(3,0,10,7);
	SetOperateFrame(0);
	PutPixelDatas((INT8U*)Icon_ok,8,RED);

	SetArea(12,0,19,7);
	SetOperateFrame(0);
	PutPixelDatas((INT8U*)Icon_ok,8,GREEN);	
	SetDisplayFrame(0);
	Delay_Ms(2000);
	ClearRect(3,0,19,7,YELLOW);
#endif


	CC1101_Exint1_Enable();
	halSpiStrobe(CCxxx0_SRX);	//允许接收

//测试
/*	SystemInfo.DisplayInfo.color = GREEN;
	SystemInfo.DisplayInfo.state |=DISPLAY_STATE_MESSAGE;
	sprintf(SystemInfo.DisplayInfo.MessageBuf,"恭祝大家猴年新春快乐，万事如意，事事顺心!");
	SystemInfo.DisplayInfo.ChangeDelay=31;*/


	WDTInit();
	while (1) 
	{
		//DisableIRQ();                                                   /* 关中断                       */
	    LPC_WDT->FEED  = 0xAA;                                         // 喂狗                     
	    LPC_WDT->FEED  = 0x55;
	    //EnableIRQ();   

		#if 0
		//查询串行数据接收
		UartManager();                                                
		#endif


		#if 0
		if (!GetTestMode()) //是否要进入测试模式
		{
			if ((SystemInfo.state&SYSTEM_STATE_TESTMODE)==0)
			{
				SystemInfo.state|= SYSTEM_STATE_TESTMODE;
				test=0;
			}
			if (SystemInfo.state&SYSTEM_STATE_MOVING)
			{
				SystemInfo.state&=~SYSTEM_STATE_MOVING;
				if (test==0) ClsScreen(YELLOW);
				DrawRect(test,test,LED_WIDTH-test-1,LED_HEIGHT-test-1,RED);
				test = (test+1)%(LED_HEIGHT/2);
			}
		}
		else
		#endif
		
		{
			if (SystemInfo.state&SYSTEM_STATE_TESTMODE)
			{
				SystemInfo.state&= ~SYSTEM_STATE_TESTMODE;
				ClsScreen(YELLOW);	
			}

			//切换控制:常态时在时间与广告消息间进行切换。
			if (SystemInfo.DisplayInfo.ChangeDelay>MESSAGE_CHANGED_DELAY /*5*/)	//5秒
			{
				if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL)==0/*&&(SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)==0*/) //在呼叫和位置信息时不切换
				{
					if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)
					{
						SystemInfo.DisplayInfo.ChangeDelay=0;
						SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //清除切换状态
						SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;						
					}
					else
					{
						if((SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)!=0)
						{
							SystemInfo.DisplayInfo.ChangeDelay=MESSAGE_CHANGED_DELAY-1;
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;							
						}
						else if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)!=0&&SystemInfo.DisplayInfo.MessageIndex==0)
						{
							SystemInfo.DisplayInfo.ChangeDelay=0;
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGEING; //置切换显示状态
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
						}
					}
				}
			}

	

			if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGED) //有内容发生变化
			{
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGED;
				if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL) //有呼叫信息
				{
					SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //清除切换状态
					SystemInfo.DisplayInfo.CallIndex=0;
					SystemInfo.DisplayInfo.CallShowLen=0;
					 Num = GetNextDisplayFrame();
					 SetOperateFrame(Num);
					 ClsScreen(YELLOW);
					 ShowCallInfo(buf);
					 SetDisplayFrame(Num);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //有护士到位信息
				{
					SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //清除切换状态
					SystemInfo.DisplayInfo.NurseIndex=0;
					SystemInfo.DisplayInfo.NurseShowLen=0;
					 ShowNurseInfo(buf);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //切换显示为时间信息
				{
/*					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowTime(buf);
					SetDisplayFrame(Num);	
*/
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowMessageInfo(buf);
					SetDisplayFrame(Num);

				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//有信息显示
				{
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowMessageInfo(buf);
					SetDisplayFrame(Num);
					
				}
				else
				{
/*					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowTime(buf);
					SetDisplayFrame(Num);
*/
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowMessageInfo(buf);
					SetDisplayFrame(Num);

				}
			}


			
			//检查无线接收状态
			cc1101Hander();
			Bus0Manager();
			UartManager();

			if(TimerOutDealFlag==1)
			{
				TimerOutDealFlag=0;
				TimerOutDeal();
			}

			if(KeyState	&	KEY_DOWNED_HANDLER)
			{
				KeyState &= ~(KEY_DOWNED_HANDLER);
				SaveSecAddrDelay = SAVE_SEC_ADDR_DELAY;		//键按下已处理延时变量，如果1S后再无按键操作，则保存区号
				uartSendByte(KeyState);
				if(KeyState & FIRST_KEY_HANDLED)
				{//有第一次键按下
					MySec ++;
					if(MySec==16) MySec=0;
					
				}
				else KeyState |= FIRST_KEY_HANDLED;			//第1次键按下操作已处理

				uartSendByte(KeyState);
				hex_convert(MySec);
				ClearRect(0,0,15,15,RED);
				TextOutRed(0,0,ASCII_BCD,FONT_DEFAULT,8);
			}

			else if(KeyState&(KEY_ALWAYS_DOWN_HANDLER))
			{
				KeyState &= ~(KEY_ALWAYS_DOWN_HANDLER);

				SaveSecAddrDelay = SAVE_SEC_ADDR_DELAY;
				MySec ++;
				if(MySec==16) MySec=0;
				hex_convert(MySec);
				ClearRect(0,0,15,15,RED);
				TextOutRed(0,0,ASCII_BCD,FONT_DEFAULT,8);
			}

			//处理文字移动
			if (SystemInfo.state&SYSTEM_STATE_MOVING)
			{
				SystemInfo.state&=~SYSTEM_STATE_MOVING;
				if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL) //有呼叫信息
				{
					if (SystemInfo.DisplayInfo.CallShowLen>LED_WIDTH) //需要移动
					{
						ShowCallInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //有护士到位信息
				{
					if (SystemInfo.DisplayInfo.NurseShowLen>LED_WIDTH) //需要移动
					{
						ShowNurseInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //显示时间状态下，不动作
				{
					if (SystemInfo.DisplayInfo.MessageShowLen>LED_WIDTH) //需要移动 //新增加
					{
						ShowMessageInfo(buf);
					}				
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//有信息显示
				{
					if (SystemInfo.DisplayInfo.MessageShowLen>LED_WIDTH) //需要移动
					{
						ShowMessageInfo(buf);
					}
				}
			}

		
/*			if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_TIME_CHANGED)
			{
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_TIME_CHANGED;
				ShowTime(buf);
			
			}
*/
		}
    }                                                                   /* End while (1)                */
}

__asm void intMasterEnable() {
    CPSIE   I                                                           /* 打开总中断                   */
    BX      LR
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
