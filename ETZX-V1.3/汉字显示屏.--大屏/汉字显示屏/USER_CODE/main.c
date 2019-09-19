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


#define EnableIRQ()  __enable_irq()
#define DisableIRQ() __disable_irq()
/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
unsigned long    GulSysClock     =  12000000UL;                         /* 记录处理器时钟频率           */

SYSTEM_INFO SystemInfo;


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
		if (SystemInfo.DisplayInfo.NurseShowLen>screen.width); //需要移动
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
	}
	else
	{
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
		if (SystemInfo.DisplayInfo.NurseShowLen>screen.width); //需要移动
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
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

#define GetTestMode() (LPC_GPIO0->DATA&(1<<1))
/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void GPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
	//测试模式端口初始化
	LPC_IOCON->PIO0_1 &= ~0x07;		   //P0.1
	//初始化输入
	LPC_GPIO0->DIR    &= ~(1<<1);                       	/* 初始化为输入功能     */
}

INT8U daysOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};   //一年中月份的天数

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
	//100ms延时处理

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
	SystemInfo.state|=SYSTEM_STATE_MOVING; //通知执行移动
	if (stUartControl.byTimerout>1) stUartControl.byTimerout--;	
}

/*********************************************************************************************************
** Function name:        timer0Init
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
    LPC_TMR32B1->MR0     = SystemFrequency/30000;                       /* 设置1S中断时间             */
    LPC_TMR32B1->TCR     = 0x01;                                        /* 启动定时器                   */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);                                    /* 设置中断并使能               */
    NVIC_SetPriority(TIMER_32_1_IRQn, 3);
}

void SaveSystemInfo()
{
	
}
//显示时间
void ShowTime(INT8U *buf)
{
	if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)!=0
		||(SystemInfo.DisplayInfo.state&0x1f)==0)
	{
		sprintf(buf,"%02d:%02d:%02d",SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOut(15,8,buf,FONT_ARIAL,24);

		sprintf(buf,"20%02d-%02d-%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day);
		TextOut(32,1,buf,FONT_DEFAULT,7);

		//显示系统状态
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
		//显示错误信息
		
		if (SystemInfo.state&SYSTEM_STATE_BUS_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_buserror,8);
		}
		else if (SystemInfo.state&SYSTEM_STATE_LOCAL_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_failure,8);
		}
	}
	else
	{
		sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day,SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOut(10,1,buf,FONT_DEFAULT,7);
		//显示系统状态
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
	}
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
			sprintf(buf,"加%d床呼叫",SystemInfo.DisplayInfo.CallBuf[3]);
		else
			sprintf(buf,"%d床呼叫",SystemInfo.DisplayInfo.CallBuf[3]);
		break;
	case 0x07: //输液呼叫
		sprintf(buf,"%d床输液完毕",SystemInfo.DisplayInfo.CallBuf[3]);
		break;
	case 0x08: //卫生间呼叫
		sprintf(buf,"%d房卫生间呼叫",SystemInfo.DisplayInfo.CallBuf[2]);
		break;	
	}
	len = GetTextShowLength(buf,16);
	size=16;	
	
	if (len>screen.width)
	{
		  SystemInfo.DisplayInfo.CallShowLen = len;
		  if (SystemInfo.DisplayInfo.CallIndex==0)ClsScreenForLine(8,31);
		  TextOut(screen.width-SystemInfo.DisplayInfo.CallIndex,(24-size)/2+8,buf,FONT_ARIAL,size);
		  SystemInfo.DisplayInfo.CallIndex = (SystemInfo.DisplayInfo.CallIndex+1)%(SystemInfo.DisplayInfo.CallShowLen+screen.width);
	}
	else TextOut((screen.width-len)/2,(24-size)/2+8,buf,FONT_ARIAL,size);
}
//清除矩形区域
extern void ClearRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
//显示护士到信息
void ShowNurseInfo(INT8U * buf)
{
	INT16U len;
	INT8U size,i,*tmp;
	strcpy(buf,"护士在:");
	tmp = buf;
	while(*tmp++);
	tmp--;
	//uartSendByte(*(tmp-2));
	for (i=0;i<20;i++)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[i]!=0)
		{
			sprintf(tmp,"%d,",SystemInfo.DisplayInfo.NurseBuf[i]);
			while(*tmp++);
			tmp--;
		}
		else 
			break;
	}
	
	if (SystemInfo.DisplayInfo.NurseBuf[0]==0) return;
	tmp--;
	sprintf(tmp,"%s","房");
	while(*tmp++);
	tmp--;
	size=16;
	len = 	GetTextShowLength(buf,16);
	
	if (len>screen.width)
	{
		SystemInfo.DisplayInfo.NurseShowLen = len;
		if (SystemInfo.DisplayInfo.NurseIndex==0)ClsScreenForLine(8,31);
		if (screen.width-SystemInfo.DisplayInfo.NurseIndex>0)	ClearRect(0,(24-size)/2+8,screen.width-SystemInfo.DisplayInfo.NurseIndex,(24-size)/2+8+size);
		if (screen.width+len-SystemInfo.DisplayInfo.NurseIndex<screen.width-1)	ClearRect(screen.width+len-SystemInfo.DisplayInfo.NurseIndex,(24-size)/2+8,screen.width-1,(24-size)/2+8+size);
		TextOut(screen.width-SystemInfo.DisplayInfo.NurseIndex,(24-size)/2+8,buf,FONT_ARIAL,size);
		SystemInfo.DisplayInfo.NurseIndex = (SystemInfo.DisplayInfo.NurseIndex+1)%(SystemInfo.DisplayInfo.NurseShowLen+screen.width);
	}
	else
	{ 
		ClearRect(0,(24-size)/2+8,(screen.width-len)/2,(24-size)/2+8+size);
		ClearRect(screen.width-(screen.width-len)/2,(24-size)/2+8,screen.width-1,(24-size)/2+8+size);
		TextOut((screen.width-len)/2,(24-size)/2+8,buf,FONT_ARIAL,size);	
	}
}
//显示服务信息
void ShowMessageInfo(INT8U * buf)
{
	if (SystemInfo.DisplayInfo.MessageShowLen==0)
	{
		 SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16);
	}
	if (SystemInfo.DisplayInfo.MessageShowLen>screen.width)
	{
		TextOut(screen.width-SystemInfo.DisplayInfo.MessageIndex,13,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
		SystemInfo.DisplayInfo.MessageIndex = (SystemInfo.DisplayInfo.MessageIndex+1)%(SystemInfo.DisplayInfo.MessageShowLen+screen.width);
	}
	else
	{
		TextOut((screen.width-SystemInfo.DisplayInfo.MessageShowLen)/2,13,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
	}
}

int main(void)
{
	INT8U i,test;
	INT8U buf[256];
  SystemInit();	                                                    /* 系统时钟初始化               */
	GPIOInit();
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
  intMasterEnable();                                                  /* 使能中断总开关               */
	SingleBusInit();

	//读取系统设置信息
	eepromRead(0x0, (INT8U*)&SystemInfo, SYSTEM_SAVE_SIZE);                                         /* 从EEPROM 0x100地址读出数据 */
	if (SystemInfo.UartBaud>10)																                                                              
	{
		//初始化为默认值
		SystemInfo.UartBaud = 9; //默认为57600
		SystemInfo.LOCAL_ADDR=0;
		SystemInfo.DisplayInfo.MessageBuf[0]=0;
		eepromWrite((INT8U*)&SystemInfo); //保存参数
		eepromRead(0x0, (INT8U*)&SystemInfo, SYSTEM_SAVE_SIZE);
	}
	//初始化串口 
	UartInit(SystemInfo.UartBaud);

	if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
	else
	{
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
	}
	i=0;
	while(SystemInfo.DisplayInfo.MessageBuf[i])
	{
		uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i++]);
	}
	
	//初始化模拟SPI口
	SPI_Init();
	//初始化显示输出接口
	LED_Init();
	  
	//strcpy(SystemInfo.DisplayInfo.MessageBuf,"今天是2009年以来值得庆幸的日子，全国股民集体向2000点遗体告别啦！略微有些悲壮！");
	//SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16)+screen.width;
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

	SetArea(0,0,127,31);
	PutPixelDatas((INT8U*)Eter_logo,128*32/8);
	Delay_Ms(5000);
	WDTInit();
	ClsScreen();
	SetOperateFrame(1);
	ClsScreen();
	SetOperateFrame(0);
	while (1) {
		//DisableIRQ();                                                   /* 关中断                       */
    LPC_WDT->FEED  = 0xAA;                                          /* 喂狗                         */
    LPC_WDT->FEED  = 0x55;
    //EnableIRQ();   
		//查询总线接收状态
		Bus0Manager();
		//查询串行数据接收
		UartManager();                                                 /* 开中断                       */
		
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
				if (test==0) ClsScreen();
				DrawRect(test,test,screen.width-test-1,screen.height-test-1);
				test = (test+1)%(screen.height/2);
			}
		}
		else
		{
			if (SystemInfo.state&SYSTEM_STATE_TESTMODE)
			{
				SystemInfo.state&= ~SYSTEM_STATE_TESTMODE;
				ClsScreen();	
			}

			//切换控制
			if (SystemInfo.DisplayInfo.ChangeDelay>30)
			{
				if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL)==0&&(SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)==0) //在呼叫和位置信息时不切换
				{
					if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)
					{
						SystemInfo.DisplayInfo.ChangeDelay=0;
						SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //清除切换状态
						SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;						
					}
					else
					{
						if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)!=0&&SystemInfo.DisplayInfo.MessageIndex==0)
						{
							SystemInfo.DisplayInfo.ChangeDelay=0;
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGEING; //清除切换状态
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
						}
					}
				}
			}

			//检查总线错误
			if (SystemInfo.error>4)
			{
				if ((SystemInfo.state&SYSTEM_STATE_BUS_ERROR)==0)
				{
					//测试是否为本机错误
					SystemInfo.state|= SYSTEM_STATE_TESTMODE; //置为测试模式，以禁止总线接收数据
					ClrBus0SendPin();
					SetProtectPin();
					Delay_Ms(10);
					if (GetBus0RecPin()==0) //低电平表示本机正常
					{
						SystemInfo.state|=SYSTEM_STATE_BUS_ERROR;  //置错误状态
						SystemInfo.state&=~SYSTEM_STATE_ONLINE; //清在线状态
						ClrProtectPin();
						Delay_Ms(10);
					}
					else
					{
						SystemInfo.state|=SYSTEM_STATE_BUS_ERROR;  //置错误状态
						SystemInfo.state&=~SYSTEM_STATE_ONLINE; //清在线状态
						//本机故障
						SystemInfo.state|=SYSTEM_STATE_LOCAL_ERROR;  //置错误状态
					}
					SystemInfo.state&= ~SYSTEM_STATE_TESTMODE;
				}
			}
			else
			{
				if (SystemInfo.state&SYSTEM_STATE_BUS_ERROR)
				{
					SystemInfo.state&=~SYSTEM_STATE_BUS_ERROR;  //清错误状态
					SystemInfo.state&=~SYSTEM_STATE_LOCAL_ERROR;  //清错误状态
					ClrProtectPin();
					//SystemInfo.state&=~SYSTEM_STATE_ONLINE; //清在线状态
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
					 i = GetNextDisplayFrame();
					 SetOperateFrame(i);
					 ClsScreen();
					 ShowCallInfo(buf);
					 ShowTime(buf);
					 SetDisplayFrame(i);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //有护士到位信息
				{
					SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //清除切换状态
					SystemInfo.DisplayInfo.NurseIndex=0;
					SystemInfo.DisplayInfo.NurseShowLen=0;
					 i = GetNextDisplayFrame();
					 SetOperateFrame(i);
					 ClsScreen();
					 ShowNurseInfo(buf);
					 ShowTime(buf);
					 SetDisplayFrame(i);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //切换显示为时间信息
				{
					i = GetNextDisplayFrame();
					SetOperateFrame(i);
					ClsScreen();
					ShowTime(buf);
					SetDisplayFrame(i);	
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//有信息显示
				{
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					i = GetNextDisplayFrame();
					SetOperateFrame(i);
					ClsScreen();
					ShowMessageInfo(buf);
					ShowTime(buf);
					SetDisplayFrame(i);
				}
				else
				{
					i = GetNextDisplayFrame();
					SetOperateFrame(i);
					ClsScreen();
					ShowTime(buf);
					SetDisplayFrame(i);
				}
			}
			//检查无线接收状态
	
			//处理文字移动
			if (SystemInfo.state&SYSTEM_STATE_MOVING)
			{
				SystemInfo.state&=~SYSTEM_STATE_MOVING;
				if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL) //有呼叫信息
				{
					if (SystemInfo.DisplayInfo.CallShowLen>screen.width) //需要移动
					{
						ShowCallInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //有护士到位信息
				{
					if (SystemInfo.DisplayInfo.NurseShowLen>screen.width) //需要移动
					{
						ShowNurseInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //切换显示，不动作
				{
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//有信息显示
				{
					if (SystemInfo.DisplayInfo.MessageShowLen>screen.width) //需要移动
					{
						ShowMessageInfo(buf);
					}
				}
			}
		
			if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_TIME_CHANGED)
			{
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_TIME_CHANGED;
				ShowTime(buf);
			}
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
