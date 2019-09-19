/**********************************************************************************************************************/
//一特之星单总线收发程序
/**********************************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "singlebus.h"
#include "hzxsp.h"
#include "led.h"

//总线0变量定义
INT16U byBus0State 	= 0;							//单总线0状态字
#define bBus0StartRec		(1<<0)				//单总线0开始接收起始位标志
#define bBus0OnRec			(1<<1)				//单总线0开始接收数据位标志
#define  bBus0Enable		(1<<2)				//单总线0允许发送标志
#define  bBus0ReqSend		(1<<3)				//单总线0请求发送标志
#define  bBus0OnSendBit		(1<<4)				//单总线0正在发送一个数据位标志
#define  bBus0OnSendFreq	(1<<5)				//单总线0正在发送一个数据帧标志
#define  bBus0SendError		(1<<6)				//单总线0发送错误标志
#define  bBus0Error			(1<<7)				//单总线0故障标志
#define  bBus0RecFinish		(1<<8)				//单总线0接收完成标志
#define  bBus0RecBit9		(1<<9)				//单总线0接收字节数据的第9位
#define  bBus0SendFinish	(1<<10)				//单总线0发送完成标志
#define  bBus0Disable		(1<<11)				//单总线0禁止发送标志
#define  bBus0SendBit		(1<<12)				//单总线0正在发送的数据位
#define  bBus0RecBit		(1<<13) 			//单总线0正在接收的数据位
#define nbBus0SendBit 12
#define SetBus0State(x) (byBus0State|=x)		//置位操作
#define ClrBus0State(x) (byBus0State&=~x)		//清零操作
#define GetBus0State(x) (byBus0State&x)			//获取位状态
INT8U byBus0RecCount = 0;						//高4位是总线0接收定时中断计数，低4位是接收到的位计数
INT8U byBus0RecBuf;								//总线0接收缓冲单元
INT8U byBus0SendBuf;							//总线0发送缓冲单元	
INT8U byBus0RecData[BUS0_FREQ_SIZE];			//总线0接收缓冲区
INT8U byBus0SendData[BUS0_FREQ_SIZE];			//总线0发送缓冲区
INT8U byBus0SendCount = 0;						//高4位是总线0发送定时中断计数，低4位是发送的位计数
INT8U byBus0RecSendCount = 0;					//高4位是总线0接收到的字节计数，低4位是总线0发送完的字节计数
INT8U byBus0SendStopCount;						//总线0停止时间计数
INT8U byBus0RecTimeOut = 0;						//总线0接收超时计数
INT8U byBus0DisableCount = 0;					//总线0禁止计数	 

INT8U byBus0TxQ[BUS0_TX_Q_ZISE];				//总线0发送队列							
INT8U byBus0TxHead = 0;							//单总线0发送队列头指针
INT8U byBus0TxTail = 0; 						//单总线0发送队列尾指针
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 

INT8U byBus0RxQ[BUS0_RX_Q_ZISE];				//总线0接收队列							
INT8U byBus0RxHead = 0;							//单总线0接收队列头指针
INT8U byBus0RxTail = 0; 						//单总线0接收队列尾指针
#define IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE) 

//变量定义
STBusFreq stBusDealFreq;									//总线数据临时变量

//---------------------------------
//----------------------------------
/**********************************************************
*函数名称			:SingleBusInit	
*函数描述        	:单总线初始化
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
void SingleBusInit(void)
{
	//初始化发送接收IO
    LPC_IOCON->PIO1_4&=~0X07;
#ifdef RELEASE
	LPC_IOCON->PIO1_11&=~0X07;
	LPC_GPIO1->DIR  &= ~Bus0RecPin;
#else
	LPC_IOCON->PIO2_6&=~0X07;
	LPC_GPIO2->DIR  &= ~Bus0RecPin;
#endif
	LPC_GPIO1->DATA &= ~Bus0SendPin;
	LPC_GPIO1->DIR  |= Bus0SendPin;
	//初始化隔离控制IO
	LPC_IOCON->PIO2_8&=~0x07;
	ClrProtectPin();
	LPC_GPIO2->DIR |= ProtectPin;
	ClrProtectPin();
	//初始化定时器作为接收使用
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);                              /* 打开定时器模块               */

    LPC_TMR32B0->IR      = 1;
    LPC_TMR32B0->PR      = 1000;                                        /* 设置分频系数                 */
    LPC_TMR32B0->MCR     = 3;                                           /* 设置MR0匹配后复位TC并产生中断*/
    LPC_TMR32B0->MR0     = SystemFrequency/10000000;                       /* 设置100uS中断时间             */
    LPC_TMR32B0->TCR     = 0x01;                                        /* 启动定时器                   */
    NVIC_EnableIRQ(TIMER_32_0_IRQn);                                    /* 设置中断并使能               */
    NVIC_SetPriority(TIMER_32_0_IRQn, 1);
	byBus0SendStopCount = 230;
	//使能接收中断(上升沿有效)
#ifdef RELEASE
		LPC_GPIO1->IS &= ~Bus0RecPin; //边沿中断
		LPC_GPIO1->IEV |= Bus0RecPin; //上升沿中断
		LPC_GPIO1->IE |= Bus0RecPin; //允许中断
		NVIC_EnableIRQ(EINT1_IRQn);                                    /* 设置中断并使能               */
    	NVIC_SetPriority(EINT1_IRQn, 2);
#else
		LPC_GPIO2->IS &= ~Bus0RecPin; //边沿中断
		LPC_GPIO2->IEV |= Bus0RecPin; //上升沿中断
		LPC_GPIO2->IE |= Bus0RecPin; //允许中断
		NVIC_EnableIRQ(EINT2_IRQn);                                    /* 设置中断并使能               */
    	NVIC_SetPriority(EINT2_IRQn, 2);
#endif	
}
/**********************************************************
*函数名称			:Bus0RecInt	
*函数描述        	:外部中断0函数,单总线0接收中断
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
#ifdef RELEASE
void PIOINT1_IRQHandler(void)
{
	LPC_GPIO1->IC |= Bus0RecPin;
#else
void PIOINT2_IRQHandler(void)
{
	LPC_GPIO2->IC |= Bus0RecPin;
#endif								          
	DisableBus0RecInt();					//禁止再次下降沿中断
	SetBus0State(bBus0StartRec);			//启动起始位沿检测
	ClrBus0State(bBus0Enable);				//禁止总线发送							
	ClrBus0State(byBus0RecCount);						//清接收寄存器 
	uartSendByte(GetBus0RecPin());
}
/**********************************************************
*函数名称			:Timer0Int	
*函数描述        	:定时器0溢出中断,定时器每93.75us中断一次
					 程序间隔检查总线0与总线1的接收和发送
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
void TIMER32_0_IRQHandler (void)
{ 
	static INT16U display_tick=0;
	INT8U  byTemp;
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
		if (screen.DISP_first==1&&(LPC_SSP0->SR&0x10)==0)
		{
			PutDataToSSP();
		}
	}
	if ((SystemInfo.state&(SYSTEM_STATE_TESTMODE|SYSTEM_STATE_LOCAL_ERROR))==0) //非测试模式及本机错误
	{
		if (GetBus0RecPin()) 
		{
			ClrBus0State(bBus0RecBit);
		}
		else 
		{
			SetBus0State(bBus0RecBit);
			SystemInfo.error=0;
		}
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------	           
		/*******************************总线0接收处理***********************************/
		if(GetBus0State(bBus0StartRec))							//判断总线所处的状态，接收到起始位
		{	
					
			byBus0RecCount += 0x10;					//增加定时中断计数次数
			if(0x50 == (byBus0RecCount & 0xf0))		//到总线起始位检测时间
			{			
				ClrBus0State(bBus0StartRec);
				byBus0RecCount = 0x00;				//重新开始计数			
				if(GetBus0State(bBus0RecBit))			
				{ 	//无效起始位																		
					if((!GetBus0State(bBus0SendError))&&GetBus0State(bBus0OnSendFreq))
					{	//没有发生过总线发送错误，且有一帧数据正在发送，停止帧数据发送，置发送错误标志						
						byBus0State &= (~BUS0_SEND_CON);
						SetBus0State(bBus0SendError);
						ClrBus0SendPin();			//释放总线	 				
					}				
					byBus0SendStopCount = 240;					
					byBus0RecSendCount = 0x00;		//接收出错，重置接收发送计数值
					EnableBus0RecInt();
				}
				else								
				{	//有效起始位
					SetBus0State(bBus0OnRec);					//开始接收数据位 											
				}
			}
		}
		else if(GetBus0State(bBus0OnRec))
		{	
			byBus0RecCount += 0x10;					//增加定时中断计数次数	 		
			if(0xa0 == (byBus0RecCount & 0xf0))
			{
				byBus0RecCount &= 0x0f;				//清除定时中断计数次数
				byBus0RecCount ++;
				if(0x0a == (byBus0RecCount & 0x0f))	
				{	//收到第10位,结束位	  				
					ClrBus0State(bBus0OnRec);					//停止数据接收					
					if(GetBus0State(bBus0RecBit))
					{	//有效的结束位						
						if(((byBus0RecSendCount & 0xf0)==0&&GetBus0State(bBus0RecBit9)==0)||((byBus0RecSendCount & 0xf0)!=0&&GetBus0State(bBus0RecBit9)!=0)) 
						{	//数据桢错误
							byBus0RecTimeOut = 0;
							byBus0RecSendCount &= 0x0f;											
						}
						else 
						{	//数据桢正确
											
							byBus0RecTimeOut = 230;	//设置下一个字节数据接收超时时间
							byBus0RecData[byBus0RecSendCount>>4] = byBus0RecBuf;
							byBus0RecSendCount += 0x10;
															
							if((byBus0RecSendCount & 0xf0) >= BUS0_FREQ_SIZE_HI)
							{																						
								byBus0RecSendCount &= 0x0f;															
								//如果接收到的这帧数据是自己发送的不置标志
								if(!((GetBus0State(bBus0OnSendFreq) != 0)&&(GetBus0State(bBus0ReqSend) == 0)))
								{
																						  																						
									if(Bus0RxBuffLen() >= (BUS0_RX_Q_ZISE - 1))
									{	//没有空间存储了,失败	
										SetBus0State(bBus0RecFinish);
									}	
									else
									{
										for (byTemp=0; byTemp<BUS0_FREQ_SIZE; byTemp++)
										{	//数据入接收队列
											byBus0RxQ[byBus0RxTail] = byBus0RecData[byTemp];
											IncBus0RxPtr(byBus0RxTail);
										}
									}
								} 																		
								byBus0RecTimeOut = 0;
								byBus0DisableCount = 10;
								SetBus0State(bBus0Disable);	//禁止总线使用						
							}	
						}				
						byBus0SendStopCount = 240;
						EnableBus0RecInt();						
					}
					else							//无效结束位
					{
						SetBus0State(bBus0Error);										
						if((!GetBus0State(bBus0SendError)) && GetBus0State(bBus0OnSendFreq))
						{	//没有发生过总线发送错误，且有一帧数据正在发送，停止帧数据发送，置发送错误标志
							byBus0State &= (~BUS0_SEND_CON);
							SetBus0State(bBus0SendError);							
							ClrBus0SendPin();		//释放总线
						}
						byBus0RecSendCount = 0x00;	//接收出错，重置接收发送计数值
					}
				}
				else if(0x09 == (byBus0RecCount & 0x0f))
				{	//第9位数据
					if (GetBus0State(bBus0RecBit)) SetBus0State(bBus0RecBit9); else ClrBus0State(bBus0RecBit9);					
				}
				else 								//有效数据位
				{ 
					byBus0RecBuf >>= 1;
					if(GetBus0State(bBus0RecBit))
					{	//为高电平
						byBus0RecBuf |= 0x80;
					}	
				}
			}
		}
		/*******************************总线0发送处理***********************************/
		if((byBus0State & BUS0_CAN_SEND) == BUS0_CAN_SEND)
		{	//总线0上有数据发送请求，且总线允许发送		
			if(GetBus0State(bBus0RecBit))
			{	//总线正常,可以发送
				SetBus0SendPin();
				ClrBus0State(bBus0SendBit);					//发送起始位数据 			
				byBus0SendCount = 0;
				byBus0State &= (~BUS0_CAN_SEND);
				byBus0SendBuf = byBus0SendData[byBus0RecSendCount & 0x0f];
				SetBus0State(bBus0OnSendBit);					//取出待发送的数据并置正在发送标志		
			}
			else
			{	//总线不正常,停止发送
				byBus0State &= (~BUS0_SEND_CON);
				SetBus0State(bBus0SendError);			
				byBus0RecSendCount &= 0xf0;
				ClrBus0SendPin();
				byBus0SendStopCount = 240;
			}
		}
		else if(GetBus0State(bBus0OnSendBit))
		{ 	//有数据位正在发送,首先发送的是起始位			
			if((GetBus0State(bBus0SendBit)==0&&GetBus0State(bBus0RecBit)==0)||(GetBus0State(bBus0SendBit)!=0&&GetBus0State(bBus0RecBit)!=0))   		
			{	//发送的数据和接收的数据相同
				byBus0SendCount += 0x10;
				if(0xa0 == (byBus0SendCount & 0xf0))
				{	//一位数据发送完毕,首先发送的是起始位
					byBus0SendCount &= 0x0f;
					byBus0SendCount += 0x01;				
					if(0x09 == (byBus0SendCount & 0x0f))
					{	//发送到第9位了
						if ((byBus0RecSendCount&0x0f)==0) 
						{
							SetBus0State(bBus0SendBit);
							ClrBus0SendPin();
						}
						else
						{
							ClrBus0State(bBus0SendBit);
							SetBus0SendPin();
						}					
					}
					else if(0x0a == (byBus0SendCount & 0x0f))
					{	//发送到结束位了
						SetBus0State(bBus0SendBit);
						ClrBus0SendPin();	
					}
					else if(0x0b == (byBus0SendCount & 0x0f))
					{	//已经发送完结束位了
						ClrBus0State(bBus0OnSendBit);						
						byBus0RecSendCount += 0x01;									
						if((byBus0RecSendCount & 0x0f) >= BUS0_FREQ_SIZE)
						{	//发送完一帧数据												
							byBus0RecSendCount &= 0xf0;		//重新进入数据帧的发送阶段
							byBus0State &= (~BUS0_SEND_CON);
							byBus0SendStopCount = 240;
							byBus0State |= BUS0_SEND_FINISH;								
							byBus0DisableCount = 10;								
						}
						else
						{  						
							byBus0SendStopCount = 10;
							SetBus0State(bBus0ReqSend);
						}
						EnableBus0RecInt();			//再次使能接收中断
					}
					else
					{
						if(byBus0SendBuf & 0x01)
						{	//发送高电平
							SetBus0State(bBus0SendBit);
							ClrBus0SendPin();					
						}
						else
						{	//发送低电平
							ClrBus0State(bBus0SendBit);
							SetBus0SendPin();
						}
						byBus0SendBuf >>= 1;
					}
				}
			}
			else
			{	//不相同,发送失败								
				byBus0State &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				SetBus0State(bBus0SendError);					
				ClrBus0SendPin(); 
				byBus0SendStopCount = 240;
			}
		} 	
		/*******************************总线0控制处理***********************************/
		if(0 == GetBus0State(BUS0_ON_REC))
		{
			if(byBus0SendStopCount != 0)
			{
				if((--byBus0SendStopCount) == 0)
				{				
					SetBus0State(bBus0Enable);								
				}
			}		
			if(GetBus0State(bBus0Error))
			{								
				ClrBus0State(bBus0Enable);			
				if(GetBus0State(bBus0RecBit))
				{				
					ClrBus0State(bBus0Error);
					EnableBus0RecInt();
					byBus0SendStopCount = 240;
				}
			}
		} 	
		/*******************************总线0超时处理***********************************/
		if(byBus0RecTimeOut != 0)
		{
			if(--byBus0RecTimeOut == 0)				//接收超时到
			{
				byBus0RecSendCount &= 0x0f;
			}
		}
		if(byBus0DisableCount != 0)
		{
			if(--byBus0DisableCount == 0)			//禁止超时到
			{			
				ClrBus0State(bBus0Disable);
			}
		} 
		/***********总线0自动发送管理**********/	 
		if((byBus0State & BUS0_ON_WORK) == 0x00)	//总线0没有工作
		{				
			if(GetBus0State(bBus0SendError))		//总线0有发送错误
			{	//产生了发送错误,自动重发								
				ClrBus0State(bBus0SendError);				
				byBus0State |= BUS0_REQ_SEND;		
			}
			else									//总线0无发送错误
			{		
				if(GetBus0State(bBus0Disable)==0)
				{	//总线0没有禁止使用,且发送结束处理已经完成
					if(Bus0TxBuffLen() >= BUS0_FREQ_SIZE)
					{  																		
						byBus0SendData[0] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);
						byBus0SendData[1] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);
						byBus0SendData[2] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);
						byBus0SendData[3] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);
						byBus0SendData[4] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);
						byBus0SendData[5] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);
						byBus0SendData[6] = byBus0TxQ[byBus0TxHead];
						IncBus0TxPtr(byBus0TxHead);	
						
						SetBus0State(BUS0_REQ_SEND);						
					}
					else
					{
						byBus0TxHead = byBus0TxTail = 0;
					}								
				}
			}
		}
	} 
} 
/**********************************************************
*函数名称			:Bus0OutputData	
*函数描述        	:单总线0将待发送数据放入缓冲区
*输入参数   		:pbyData:待发送的数据指针
*返回值				:TRUE:发送成功,FALSE:队列满,发送失败 	
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
INT8U Bus0OutputData(INT8U * pbyData)
{
	INT8U byTemp = BUS0_FREQ_SIZE;

	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入发送队列
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	return(TRUE);	
}


/**********************************************************
*函数名称			:BcdToHex	
*函数描述        	:BCD转换成十六进制
*输入参数   		:byData:待转换的BCD码
*返回值				:byRet:转化后的16进制数据
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
INT8U BcdToHex(INT8U byData)
{
	INT8U byRet;
	byRet = byData >> 4;
	byRet *= 10;
	byData &= 0x0f;
	byRet += byData;
	return(byRet);
}
/**********************************************************
*函数名称			:Bus0RecDeal	
*函数描述        	:单总线0收到一帧数据处理函数,该函数首先
					 取出收到的数据,针对每条命令执行对应的控
					 制动作
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void Bus0RecDeal(void)
{
	switch(stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//收到确认命令
			SystemInfo.state |= SYSTEM_STATE_ONLINE; //置系统在线状态			
			break; 
		case CMD_QUEST:										//查询命令
			stBusDealFreq.bySndSecAddr = SystemInfo.LOCAL_ADDR;
			stBusDealFreq.bySndRoomAddr = 0;
			stBusDealFreq.bySndBedAddr = 0;
			stBusDealFreq.byCmd = CMD_ANSWER; 			
			Bus0OutputData(&(stBusDealFreq.bySndSecAddr));
			break;
		//2012/4/9设置系统波特率
		case CMD_SET_BAUD:
			/*uartSendByte(stBusDealFreq.byRecSecAddr);
			uartSendByte(stBusDealFreq.byRecRoomAddr);
			uartSendByte(stBusDealFreq.byRecBedAddr); */
		    SystemInfo.UartBaud=stBusDealFreq.byRecSecAddr;
			eepromWrite((INT8U*)&SystemInfo); //保存参数
	        UartReInit(SystemInfo.UartBaud); //启动新的波特率
		    break;
		//------------------------------------------------------
		//------------------------------------------------------
		case CMD_DATA_SEND:									//收到校时命令
			SystemInfo.sTime.second = BcdToHex(stBusDealFreq.bySndSecAddr & 0x7f);
			SystemInfo.sTime.minute = BcdToHex(stBusDealFreq.bySndRoomAddr & 0x7f);
			SystemInfo.sTime.hour   = BcdToHex(stBusDealFreq.bySndBedAddr & 0x3f);
			SystemInfo.sTime.day    = BcdToHex(stBusDealFreq.byRecSecAddr & 0x3f);
			SystemInfo.sTime.month  = BcdToHex(stBusDealFreq.byRecRoomAddr & 0x1f);
			SystemInfo.sTime.year   = BcdToHex(stBusDealFreq.byRecBedAddr);			
			break;
		case CMD_INFO_INDICATION:  							//信息指示命令		
			SystemInfo.DisplayInfo.CallBuf[0] =  stBusDealFreq.byRecSecAddr&0x1f;  //呼叫类型
			SystemInfo.DisplayInfo.CallBuf[1] =  stBusDealFreq.bySndSecAddr;		//区号
			SystemInfo.LOCAL_ADDR = stBusDealFreq.bySndSecAddr; //存储为本机区号
			SystemInfo.DisplayInfo.CallBuf[2] =  stBusDealFreq.bySndRoomAddr;		//房号
			SystemInfo.DisplayInfo.CallBuf[3] =  stBusDealFreq.bySndBedAddr;		//床号
			SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CALL;
			SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
			break;
		case CMD_NURSE_COME:		//护士到位命令
			InsertNurse(stBusDealFreq.byRecRoomAddr);
			break;	
		case CMD_NURSE_BACK:	   //护士离开命令
			DeleteNurse(stBusDealFreq.byRecRoomAddr);
			break;
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
		case CMD_INFUSION_CLEAR:
		case CMD_SERVICE_CLEAR:
		case CMD_EMERGENCY_CLEAR:
		case CMD_HELP_CLEAR:								//处理呼叫或者清除呼叫指令
			//比较正在指示的地址与处理的地址
			if((stBusDealFreq.byRecSecAddr != 0xff) && (stBusDealFreq.byRecSecAddr != SystemInfo.DisplayInfo.CallBuf[1]))
			{	//区地址不正确
				break;
			}
			if((stBusDealFreq.byRecRoomAddr != 0xff) && (stBusDealFreq.byRecRoomAddr != SystemInfo.DisplayInfo.CallBuf[2]))
			{	//房地址不正确
				break;
			}
			if((stBusDealFreq.byRecBedAddr != 0xff) && (stBusDealFreq.byRecBedAddr != SystemInfo.DisplayInfo.CallBuf[3]))
			{	//床地址不正确
				break;
			} 
		case CMD_STOP_INDICATION:
			SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CALL;
			SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
			break;
		case CMD_POWER_ON:	  								//本机重新热启动
			//if(AddrCompare(&stBusDealFreq))
			//{	//发送到本机的命令，重启 				
				while(1);			
			//}
			break;			
	}
	SystemInfo.state|=SYSTEM_STATE_ONLINE; //标识在线状态		
}


//单总线数据接收事物处理
void Bus0Manager(void)
{
	INT8U tx_buf[7];
	//有完成接收信号
	if (GetBus0State(bBus0RecFinish))
	{
		memcpy((void*)tx_buf,(void*)byBus0RecData,sizeof(STBusFreq));
		ClrBus0State(bBus0RecFinish);
		memcpy((void*)&stBusDealFreq, tx_buf, sizeof(STBusFreq));
		Bus0RecDeal();

	}
	//接收队列中有数据未处理
	if(Bus0RxBuffLen() >= BUS0_FREQ_SIZE)
	{  																		
		tx_buf[0] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[1] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[2] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[3] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[4] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[5] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[6] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		if(Bus0RxBuffLen() < BUS0_FREQ_SIZE) byBus0TxHead = byBus0TxTail = 0;
		memcpy((void*)&stBusDealFreq, tx_buf, sizeof(STBusFreq));
		Bus0RecDeal();
	}
}

