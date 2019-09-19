/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SingleBus.c
*文件描述    		:单总线驱动程序
*创建人     		:尹运同
*创建日期   		:2009-4-6
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/

#define _IN_SINGLE_BUS_
#include "config.h"
//总线0变量定义
uint8 bdata byBus0State0 	= 0;							//单总线0状态字1
sbit  bBus0StartRec			= byBus0State0^0;				//单总线0开始接收起始位标志
sbit  bBus0OnRec			= byBus0State0^1;				//单总线0开始接收数据位标志
sbit  bBus0Enable			= byBus0State0^2;				//单总线0允许发送标志
sbit  bBus0ReqSend			= byBus0State0^3;				//单总线0请求发送标志
sbit  bBus0OnSendBit		= byBus0State0^4;				//单总线0正在发送一个数据位标志
sbit  bBus0OnSendFreq		= byBus0State0^5;				//单总线0正在发送一个数据帧标志
sbit  bBus0SendError		= byBus0State0^6;				//单总线0发送错误标志
sbit  bBus0Error			= byBus0State0^7;				//单总线0故障标志
uint8 bdata byBus0State1 	= 0;  							//单总线0状态字2
sbit  bBus0RecFinish		= byBus0State1^0;				//单总线0接收完成标志
sbit  bBus0RecBit9			= byBus0State1^1;				//单总线0接收字节数据的第9位
sbit  bBus0SendFinish		= byBus0State1^2;				//单总线0发送完成标志
sbit  bBus0Disable			= byBus0State1^3;				//单总线0禁止发送标志
sbit  bBus0SendBit			= byBus0State1^4;				//单总线0正在发送的数据位
sbit  bBus0RecBit			= byBus0State1^5; 
 
uint8 data byBus0RecCount = 0;						//高4位是总线0接收定时中断计数，低4位是接收到的位计数
uint8 data byBus0RecBuf;								//总线0接收缓冲单元
uint8 data byBus0SendBuf;							//总线0发送缓冲单元	
uint8 data byBus0RecData[BUS0_FREQ_SIZE];			//总线0接收缓冲区
uint8 data byBus0SendData[BUS0_FREQ_SIZE];			//总线0发送缓冲区
uint8 data byBus0SendCount = 0;						//高4位是总线0发送定时中断计数，低4位是发送的位计数
uint8 data byBus0RecSendCount = 0;					//高4位是总线0接收到的字节计数，低4位是总线0发送完的字节计数
uint8 data byBus0SendStopCount;						//总线停止时间计数
uint8 data byBus0RecTimeOut = 0;						//总线0接收超时计数
uint8 data byBus0DisableCount = 0;					//总线0禁止计数
uint8 xdata byBus0TxQ[BUS0_TX_Q_ZISE];				//总线0发送队列							
uint8 data byBus0TxHead = 0;
uint8 data byBus0TxTail = 0;
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 

sbit  Bus0RecPin	= P3^2;									//单总线0接收引脚定义
sbit  Bus0SendPin	= P3^5;									//单总线0发送引脚定义






//总线1变量定义
uint8 bdata byBus1State0 	= 0;							//单总线1状态字1
sbit  bBus1StartRec			= byBus1State0^0;				//单总线1开始接收起始位标志
sbit  bBus1OnRec			= byBus1State0^1;				//单总线1开始接收数据位标志
sbit  bBus1Enable			= byBus1State0^2;				//单总线1允许发送标志
sbit  bBus1ReqSend			= byBus1State0^3;				//单总线1请求发送标志
sbit  bBus1OnSendBit		= byBus1State0^4;				//单总线1正在发送一个数据位标志
sbit  bBus1OnSendFreq		= byBus1State0^5;				//单总线1正在发送一个数据帧标志
sbit  bBus1SendError		= byBus1State0^6;				//单总线1发送错误标志
sbit  bBus1Error			= byBus1State0^7;				//单总线1故障标志
uint8 bdata byBus1State1 	= 0;  							//单总线1状态字2
sbit  bBus1RecFinish		= byBus1State1^0;				//单总线1接收完成标志
sbit  bBus1RecBit9			= byBus1State1^1;				//单总线1接收字节数据的第9位
sbit  bBus1SendFinish		= byBus1State1^2;				//单总线1发送完成标志
sbit  bBus1Disable			= byBus1State1^3;				//单总线1禁止发送标志
sbit  bBus1SendBit			= byBus1State1^4;				//单总线1正在发送的数据位
sbit  bBus1RecBit			= byBus1State1^5; 				//单总线1正在接收的数据位
uint8 xdata byBus1RecCount = 0;								//高4位是总线1接收定时中断计数，低4位是接收到的位计数
uint8 xdata byBus1RecBuf;									//总线1接收缓冲单元
uint8 xdata byBus1SendBuf;									//总线1发送缓冲单元	
uint8 xdata byBus1RecData[BUS1_FREQ_SIZE];					//总线1接收缓冲区
uint8 xdata byBus1SendData[BUS1_FREQ_SIZE];					//总线1发送缓冲区
uint8 xdata byBus1SendCount = 0;							//高4位是总线1发送定时中断计数，低4位是发送的位计数
uint8 xdata byBus1RecSendCount = 0;							//高4位是总线1接收到的字节计数，低4位是总线1发送完的字节计数
uint8 xdata byBus1SendStopCount;								//总线1停止时间计数
uint8 xdata byBus1RecTimeOut = 0;							//总线1接收超时计数
uint8 xdata byBus1DisableCount = 0;							//总线1禁止计数	 
uint8 xdata byBus1TxQ[BUS1_TX_Q_ZISE];						//总线1发送队列							
uint8 xdata byBus1TxHead = 0;								//单总线1发送队列头指针
uint8 xdata byBus1TxTail = 0; 								//单总线1发送队列尾指针
#define IncBus1TxPtr(addr)    {addr=(addr+1)%BUS1_TX_Q_ZISE;}
#define Bus1TxBuffLen()	      ((byBus1TxTail+BUS1_TX_Q_ZISE-byBus1TxHead)%BUS1_TX_Q_ZISE)  
sbit  Bus1RecPin	= P3^3;									//单总线1接收引脚定义
sbit  Bus1SendPin	= P3^4;	   								//单总线1发送引脚定义


bit flag_origin_music=0;			//播放背蠊音乐的原始状态
bit bBus0SndBitOk=0;


uint8 DTimeState=0;
/*
#define bCH0TimerOutDeal	0x01
#define bCH1TimerOutDeal	0x02
#define bWaitCommAckDTime	0x04
#define bWaitAckTimeLand	0x08
#define bWaitRS485AckDTime	0x10
#define bDetectPcTime		0x20
#define bUsart1SdResultDTime 0x40
#define bDataTimeUpdate     0x80
*/

/**********************************************************
*函数名称			:SingleBusInit	
*函数描述        	:单总线初始化
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SingleBusInit(void)
{
//	   单总线1
	//P3^3,高阻输入
	P3M0 &= (~(Bin(0,0,0,0,1,0,0,0)));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);


	//P3^4,开漏输出 
	P3M0 |=  (Bin(0,0,0,1,0,0,0,0));
	P3M1 |=  (Bin(0,0,0,1,0,0,0,0));


		
	//总线电平设置
 	Bus0SendPin = 1;
	Bus0RecPin = 1;  
	Bus1SendPin = 1;
	Bus1RecPin = 1;		
	
	TMOD &= 0xf0;	 //  ;t1作波特率发生器(不变)，
	TMOD |= 0X01;	//t0作方式1（16位)定时器

	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	TF0 = 0;												//清除中断标志
	AUXR &= (~T0x12);										//传统12分频速度  
	ET0 = 1;												//允许定时器0中断 
	TR0 = 1;												//启动定时器
	//其它控制设置
	byBus0SendStopCount = 240;								//上电总线0禁止发送时间设置 	
	byBus1SendStopCount = 240;								//上电总线1禁止发送时间设置

	IE0 = 0;												//清除中断标志
	IT0 = 1;												//外部中断0为下降沿触发模式 				
	PT0=1;
	IE1 = 0;												//清除外部中断1标志
	IT1 = 1;												//外部中断1为下降沿触发模式 			
	
	if(Bus0RecPin)
	{	//如果总线正常,开中断	
		EX0 = 1;
	}
	else
	{	//如果总线不正常,置总线故障标志 				
		bBus0Error = 1;
	}	
	if(Bus1RecPin)
	{	//如果总线正常,开中断		
		EX1 = 1;
	}
	else
	{	//如果总线不正常,置总线故障标志				
		bBus1Error = 1;
	} 

}

/**********************************************************
*函数名称			:Bus0RecInt	
*函数描述        	:外部中断0函数,单总线0接收中断
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
//#pragma disable
void Bus0RecInt(void) interrupt X0_INTNO
{ 	
	EA=0;
	DisableBus0RecInt();									//禁止再次下降沿中断
	bBus0StartRec = 1;										//启动起始位沿检测
	bBus0Enable = 0;										//禁止总线发送							
	byBus0RecCount = 0;										//清接收寄存器
	EA=1;
}
/**********************************************************
*函数名称			:Bus1RecInt	
*函数描述        	:外部中断1函数,单总线1接收中断
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
//#pragma disable
void Bus1RecInt(void) interrupt 2
{ 	
	EA=0;
	DisableBus1RecInt();									//禁止再次下降沿中断
	bBus1StartRec = 1;										//启动起始位沿检测
	bBus1Enable = 0;										//禁止总线发送							
	byBus1RecCount = 0;										//清接收寄存器	
	EA=1;
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
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
//#pragma disable
void Timer0Int(void) interrupt T0_INTNO
{ 

	EA=0;
	TR0=0;
	TH0=TIMER0_H;
	TL0=TIMER0_L;
	TR0=1;	
	
	bBus0RecBit = Bus0RecPin;	
	bBus1RecBit = Bus1RecPin;
	/*******************************总线0接收处理***********************************/
	if(bBus0StartRec)										//判断总线所处的状态，接收到起始位
	{			
		byBus0RecCount += 0x10; 							//增加定时中断计数次数
		if(0x50 == (byBus0RecCount & 0xf0)) 				//到总线起始位检测时间
		{			
			bBus0StartRec = 0;
			byBus0RecCount = 0x00;							//重新开始计数			
			if(bBus0RecBit) 		
			{	//无效起始位																		
				if((!bBus0SendError)&&bBus0OnSendFreq)
				{	//没有发生过总线发送错误,且有一帧数据正在发送,停止帧数据发送,置发送错误标志 					
					byBus0State0 &= (~BUS0_SEND_CON);
					bBus0SendError = 1;
					Bus0SendPin = 1;						//释放总线					
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;					//接收出错,重置接收发送计数值
				EnableBus0RecInt(); 			
			}
			else								
			{	//有效起始位
				bBus0OnRec = 1; 							//开始接收数据位											
			}
		}
	}
	else if(bBus0OnRec)
	{		
		byBus0RecCount += 0x10; 							//增加定时中断计数次数			
		if(0xa0 == (byBus0RecCount & 0xf0))
		{
			byBus0RecCount &= 0x0f; 						//清除定时中断计数次数
			byBus0RecCount += 0x01;
			if(0x0a == (byBus0RecCount & 0x0f)) 
			{	//收到第10位,结束位 				
				bBus0OnRec = 0; 							//停止数据接收					
				if(bBus0RecBit)
				{	//有效的结束位							
					if(((bit)(byBus0RecSendCount & 0xf0) == bBus0RecBit9)) 
					{	//数据桢错误
						byBus0RecTimeOut = 0;
						byBus0RecSendCount &= 0x0f; 										
					}
					else 
					{	//数据桢正确					
						byBus0RecTimeOut = 230; 			//设置下一个字节数据接收超时时间
						byBus0RecData[byBus0RecSendCount>>4] = byBus0RecBuf;
						byBus0RecSendCount += 0x10; 																				
						if((byBus0RecSendCount & 0xf0) >= BUS0_FREQ_SIZE_HI)
						{																						
							byBus0RecSendCount &= 0x0f; 						
							if(!((bBus0OnSendFreq == 1)&&(bBus0ReqSend == 0)))
							{	//如果接收到的这帧数据是自己发送的不置标志							
								bBus0RecFinish = 1; 																																					
							}																		
							byBus0RecTimeOut = 0;
							byBus0DisableCount = 10;
							bBus0Disable = 1;				//禁止总线使用							
						}	
					}				
					byBus0SendStopCount = 240;
					EnableBus0RecInt(); 					
				}
				else							
				{	//无效结束位
					bBus0Error = 1; 									
					if((!bBus0SendError) && bBus0OnSendFreq)
					{	//没有发生过总线发送错误,且有一帧数据正在发送,停止帧数据发送,置发送错误标志
						byBus0State0 &= (~BUS0_SEND_CON);
						bBus0SendError = 1; 						
						Bus0SendPin = 1;					//释放总线
					}
					byBus0RecSendCount = 0x00;				//接收出错,重置接收发送计数值					
				}
			}
			else if(0x09 == (byBus0RecCount & 0x0f))
			{	//第9位数据
				bBus0RecBit9 = bBus0RecBit; 				
			}
			else											
			{	//有效数据位
				byBus0RecBuf >>= 1;
				if(bBus0RecBit)
				{	//为高电平
					byBus0RecBuf |= 0x80;
				}	
			}
		}
	}
	/*******************************总线1接收处理***********************************/
	if(bBus1StartRec)										//判断总线所处的状态,接收到起始位
	{			
		byBus1RecCount += 0x10;								//增加定时中断计数次数
		if(0x50 == (byBus1RecCount & 0xf0))					//到总线起始位检测时间
		{			
			bBus1StartRec = 0;
			byBus1RecCount = 0x00;							//重新开始计数			
			if(bBus1RecBit)			
			{ 	//无效起始位																		
				if((!bBus1SendError)&&bBus1OnSendFreq)
				{	//没有发生过总线发送错误,且有一帧数据正在发送,停止帧数据发送,置发送错误标志					
					byBus1State0 &= (~BUS1_SEND_CON);
					bBus1SendError = 1;
					Bus1SendPin = 1;						//释放总线	 				
				}				
				byBus1SendStopCount = 240;					
				byBus1RecSendCount = 0x00;					//接收出错，重置接收发送计数值
				EnableBus1RecInt();
			}
			else											//有效起始位
			{
				bBus1OnRec = 1;								//开始接收数据位 											
			}
		}
	}
	else if(bBus1OnRec)
	{		
		byBus1RecCount += 0x10;								//增加定时中断计数次数	 		
		if(0xa0 == (byBus1RecCount & 0xf0))
		{
			byBus1RecCount &= 0x0f;							//清除定时中断计数次数
			byBus1RecCount += 0x01;
			if(0x0a == (byBus1RecCount & 0x0f))	
			{	//收到第10位,结束位	  				
				bBus1OnRec = 0;								//停止数据接收					
				if(bBus1RecBit)
				{	//有效的结束位							
					if(((bit)(byBus1RecSendCount & 0xf0) == bBus1RecBit9)) 
					{	//数据桢错误
						byBus1RecTimeOut = 0;
						byBus1RecSendCount &= 0x0f;											
					}
					else 
					{	//数据桢正确					
						byBus1RecTimeOut = 230;
						byBus1RecData[byBus1RecSendCount>>4] = byBus1RecBuf;
						byBus1RecSendCount += 0x10;																					
						if((byBus1RecSendCount & 0xf0) >= BUS1_FREQ_SIZE_HI)
						{																						
							byBus1RecSendCount &= 0x0f;	
							if(!((bBus1OnSendFreq == 1)&&(bBus1ReqSend==0)))
							{	//如果接收到的这帧数据是自己发送的不置标志						
								bBus1RecFinish = 1;															  																						
							} 																		
							byBus1RecTimeOut = 0;
							byBus1DisableCount = 10;
							bBus1Disable = 1;				//禁止总线使用							
						}	
					}				
					byBus1SendStopCount = 240;
					EnableBus1RecInt();						
				}
				else							
				{	//无效结束位
					bBus1Error = 1;										
					if((!bBus1SendError) && bBus1OnSendFreq)
					{	//没有发生过总线发送错误,且有一帧数据正在发送,停止帧数据发送,置发送错误标志
						byBus1State0 &= (~BUS1_SEND_CON);
						bBus1SendError = 1;							
						Bus1SendPin = 1;					//释放总线
					}
					byBus1RecSendCount = 0x00;				//接收出错,重置接收发送计数值
				}
			}
			else if(0x09 == (byBus1RecCount & 0x0f))
			{	//第9位数据
				bBus1RecBit9 = bBus1RecBit;					
			}
			else 											//有效数据位
			{ 
				byBus1RecBuf >>= 1;
				if(bBus1RecBit)
				{	//为高电平
					byBus1RecBuf |= 0x80;
				}	
			}
		}
	}	
	
	/*******************************总线0发送处理***********************************/
	if((byBus0State0 & BUS0_CAN_SEND) == BUS0_CAN_SEND)
	{	//总线0上有数据发送请求,且总线允许发送		
		if(bBus0RecBit)
		{	//总线正常,可以发送
			Bus0SendPin = 0;
			bBus0SendBit = 0;								//发送起始位数据			
			byBus0SendCount = 0;
			byBus0State0 &= (~BUS0_CAN_SEND);
			byBus0SendBuf = byBus0SendData[byBus0RecSendCount & 0x0f];
			bBus0OnSendBit = 1; 							//取出待发送的数据并置正在发送标志		
		}
		else
		{	//总线不正常,停止发送
			byBus0State0 &= (~BUS0_SEND_CON);
			bBus0SendError = 1; 		
			byBus0RecSendCount &= 0xf0;
			Bus0SendPin = 1;
			byBus0SendStopCount = 240;
		}
	}
	else if(bBus0OnSendBit)
	{	//有数据位正在发送,首先发送的是起始位
		byBus0SendCount += 0x10;
		if(0x50 == (byBus0SendCount & 0xf0))
		{
			if(bBus0SendBit == bBus0RecBit) bBus0SndBitOk=1;
			else
			{	//不相同,发送失败			
				byBus0State0 &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				bBus0SendError = 1; 				
				Bus0SendPin = 1; 
				byBus0SendStopCount = 240;
				bBus0SndBitOk =0;
			}

		}

			
		//if(bBus0SendBit == bBus0RecBit) 	
		else if(bBus0SndBitOk==1)
		{	//发送的数据和接收的数据相同	
			//byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//一位数据发送完毕,首先发送的是起始位
				bBus0SndBitOk =0;

				byBus0SendCount &= 0x0f;
				byBus0SendCount += 0x01;				
				if(0x09 == (byBus0SendCount & 0x0f))
				{	//发送到第9位了
			
					bBus0SendBit = !(bit)(byBus0RecSendCount & 0x0f);
					Bus0SendPin = bBus0SendBit; 					
				}
				else if(0x0a == (byBus0SendCount & 0x0f))
				{	//发送到结束位了
					bBus0SendBit = 1;
					Bus0SendPin = 1;	
				}
				else if(0x0b == (byBus0SendCount & 0x0f))
				{	//已经发送完结束位了
					bBus0OnSendBit = 0; 					
					byBus0RecSendCount += 0x01; 								
					if((byBus0RecSendCount & 0x0f) >= BUS0_FREQ_SIZE)
					{	//发送完一帧数据												
						byBus0RecSendCount &= 0xf0; 		//重新进入数据帧的发送阶段
						byBus0State0 &= (~BUS0_SEND_CON);
						byBus0SendStopCount = 240;
						byBus0State1 |= BUS0_SEND_FINISH;								
						byBus0DisableCount = 10;						
					}
					else
					{						
						byBus0SendStopCount = 10;
						bBus0ReqSend = 1;
					}
					EnableBus0RecInt(); 					//再次使能接收中断
				}
				else
				{
					if(byBus0SendBuf & 0x01)
					{	//发送高电平
						bBus0SendBit = 1;
						Bus0SendPin = 1;					
					}
					else
					{	//发送低电平
						bBus0SendBit = 0;
						Bus0SendPin = 0;
					}
					byBus0SendBuf >>= 1;					//发送数据位移位操作
				}
			}
		}
/*		else
		{	//不相同,发送失败			
			byBus0State0 &= ~BUS0_SEND_CON;
			byBus0RecSendCount &= 0xf0;
			bBus0SendError = 1; 				
			Bus0SendPin = 1; 
			byBus0SendStopCount = 240;
		}*/
	}	
	/*******************************总线1发送处理***********************************/
	if((byBus1State0 & BUS1_CAN_SEND) == BUS1_CAN_SEND)
	{	//总线0上有数据发送请求,且总线允许发送		
		if(bBus1RecBit)
		{	//总线正常,可以发送
			Bus1SendPin = 0;
			bBus1SendBit = 0; 			
			byBus1SendCount = 0;
			byBus1State0 &= (~BUS1_CAN_SEND);
			byBus1SendBuf = byBus1SendData[byBus1RecSendCount & 0x0f];
			bBus1OnSendBit = 1;			
		}
		else
		{	//总线不正常,停止发送
			byBus1State0 &= (~BUS1_SEND_CON);
			bBus1SendError = 1;			
			byBus1RecSendCount &= 0xf0;
			Bus1SendPin = 1;
			byBus1SendStopCount = 240;
		}
	}
	else if(bBus1OnSendBit)
	{ 	//有数据位正在发送,首先发送的是起始位			
		if(bBus1SendBit == bBus1RecBit)   		
		{	//发送的数据和接收的数据相同
			byBus1SendCount += 0x10;
			if(0xa0 == (byBus1SendCount & 0xf0))
			{	//一位数据发送完毕,首先发送的是起始位
				byBus1SendCount &= 0x0f;
				byBus1SendCount += 0x01;				
				if(0x09 == (byBus1SendCount & 0x0f))
				{	//发送到第9位了
					bBus1SendBit = !(bit)(byBus1RecSendCount & 0x0f);
					Bus1SendPin = bBus1SendBit;						
				}
				else if(0x0a == (byBus1SendCount & 0x0f))
				{	//发送到结束位了
					bBus1SendBit = 1;
					Bus1SendPin = 1;	
				}
				else if(0x0b == (byBus1SendCount & 0x0f))
				{	//已经发送完结束位了
					bBus1OnSendBit = 0;						
					byBus1RecSendCount += 0x01;									
					if((byBus1RecSendCount & 0x0f) >= BUS1_FREQ_SIZE)
					{	//发送完一帧数据												
						byBus1RecSendCount &= 0xf0;			//重新进入数据帧的发送阶段
						byBus1State0 &= (~BUS1_SEND_CON);
						byBus1SendStopCount = 240;
						byBus1State1 |= BUS1_SEND_FINISH;									
						byBus1DisableCount = 10;								
					}
					else
					{  						
						byBus1SendStopCount = 10;
						bBus1ReqSend = 1;
					}
					EnableBus1RecInt();						//再次使能接收中断
				}
				else
				{
					if(byBus1SendBuf & 0x01)
					{	//发送高电平
						bBus1SendBit = 1;
						Bus1SendPin = 1;					
					}
					else
					{	//发送低电平
						bBus1SendBit = 0;
						Bus1SendPin = 0;
					}
					byBus1SendBuf >>= 1;					//发送数据位移位操作
				}
			}
		}
		else
		{	//不相同,发送失败								
			byBus1State0 &= ~BUS1_SEND_CON;
			byBus1RecSendCount &= 0xf0;
			bBus1SendError = 1;					
			Bus1SendPin = 1; 
			byBus1SendStopCount = 240;
		}
	} 	
	
	/*******************************总线0控制处理***********************************/
	if(0 == (byBus0State0 & BUS0_ON_REC))
	{	
		if(byBus0SendStopCount != 0)
		{
			if((--byBus0SendStopCount) == 0)
			{				
				bBus0Enable = 1;								
			}
		}		
		if(bBus0Error)
		{								
			bBus0Enable = 0;			
			if(bBus0RecBit)
			{				
				bBus0Error = 0;
				EnableBus0RecInt();
				byBus0SendStopCount = 240;
			}
		}
	}	
	/*******************************总线1控制处理***********************************/
	if(0 == (byBus1State0 & BUS1_ON_REC))
	{
		if(byBus1SendStopCount != 0)
		{
			if((--byBus1SendStopCount) == 0)
			{				
				bBus1Enable = 1;								
			}
		}		
		if(bBus1Error)
		{								
			bBus1Enable = 0;			
			if(bBus1RecBit)
			{				
				bBus1Error = 0;
				EnableBus1RecInt();
				byBus1SendStopCount = 240;
			}
		}
	}  	
	
	/*******************************总线0超时处理***********************************/
	if(byBus0RecTimeOut != 0)
	{
		if(--byBus0RecTimeOut == 0) 						
		{	//接收超时到
			byBus0RecSendCount &= 0x0f; 		
		}
	}
	if(byBus0DisableCount != 0)
	{
		if(--byBus0DisableCount == 0)						
		{	//禁止超时到		
			bBus0Disable = 0;
		}
	}
	
	/*******************************总线1超时处理***********************************/
	if(byBus1RecTimeOut != 0)
	{
		if(--byBus1RecTimeOut == 0)							
		{	//接收超时到
			byBus1RecSendCount &= 0x0f;
		}
	}
	if(byBus1DisableCount != 0)
	{
		if(--byBus1DisableCount == 0)						
		{	//禁止超时到		
			bBus1Disable = 0;
		}
	} 
	
	/***********总线0自动发送管理**********/	 
	if((byBus0State0 & BUS0_ON_WORK) == 0x00)				
	{	//总线0没有工作 			
		if(bBus0SendError)						
		{	//产生了发送错误,自动重发								
			bBus0SendError = 0; 			
			byBus0State0 |= BUS0_REQ_SEND;		
		}
		else									
		{	//总线0无发送错误		
			if(!(bBus0SendFinish|bBus0Disable))
			{	//总线0没有禁止使用,且发送结束处理已经完成
				if(Bus0TxBuffLen() >= BUS0_FREQ_SIZE)
				{	//有一帧完整的数据在发送队列中																		
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
					byBus0State0 |= BUS0_REQ_SEND;	

				}
				else
				{	//没有一帧完整的数据在发送队列中了
					byBus0TxHead = byBus0TxTail = 0;
				}								
			}
		}
	}	
	/***********总线1自动发送管理**********/	 
	if((byBus1State0 & BUS1_ON_WORK) == 0x00)	
	{	//总线1没有工作				
		if(bBus1SendError)						
		{	//产生了发送错误,自动重发								
			bBus1SendError = 0;				
			byBus1State0 |= BUS1_REQ_SEND;		
		}
		else									
		{	//总线1无发送错误		
			if(!(bBus1SendFinish|bBus1Disable))
			{	//总线0没有禁止使用,且发送结束处理已经完成
				if(Bus1TxBuffLen() >= BUS1_FREQ_SIZE)
				{	//有一帧完整的数据在发送队列中  																		
					byBus1SendData[0] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[1] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[2] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[3] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[4] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[5] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[6] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);	
					byBus1State0 |= BUS1_REQ_SEND;						
				}
				else
				{	//没有一帧完整的数据在发送队列中了
					byBus1TxHead = byBus1TxTail = 0;
				}								
			}
		}
	}
	EA=1;
}


/**********************************************************
*函数名称			:SaveCallAddr	
*函数描述        	:保存主动通话方地址
*输入参数   		:pstBus0SendFreq:待保存的数据帧指针
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SaveCallAddr(pSTBusFreq pstBusFreq)
{ 	
	stLocalControl.stCallAddr.bySndSecAddr = pstBusFreq->bySndSecAddr;
	stLocalControl.stCallAddr.bySndRoomAddr = pstBusFreq->bySndRoomAddr;
	stLocalControl.stCallAddr.bySndBedAddr = pstBusFreq->bySndBedAddr;

	stLocalControl.stCallAddr.byRecSecAddr = pstBusFreq->byRecSecAddr;
	stLocalControl.stCallAddr.byRecRoomAddr = pstBusFreq->byRecRoomAddr;
	stLocalControl.stCallAddr.byRecBedAddr = pstBusFreq->byRecBedAddr;	
}

/**********************************************************
*函数名称			:AddrCompare	
*函数描述        	:比较地址(含广播地址)
*输入参数   		:pstBusFreq:待比较的数据帧指针
*返回值				:1:发送到本机的数据,0:不是发送到本机的数据	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
bit AddrCompare(uint8 *pAddrA,uint8 *pAddrB)
{

	if((*pAddrA != 0xff) && (*pAddrA != *pAddrB) && (*pAddrB != 0xff))
	{	
		return(0);
	} 
	
	if((*(pAddrA+1) != 0xff) && (*(pAddrA+1) != *(pAddrB+1)) && (*(pAddrB+1) != 0xff))
	{	
		return(0);
	}
	
	if((*(pAddrA+2) != 0xff) && (*(pAddrA+2) != *(pAddrB+2)) && (*(pAddrB+2) != 0xff))
	{	
		return(0);
	}
	
	return(1); 

}

/**********************************************************
*函数名称			:DirAddrCompare	
*函数描述        	:绝对比较地址
*输入参数   		:pstBusFreq:待比较的数据帧指针
*返回值				:1:发送到本机的数据,0:不是发送到本机的数据	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
bit DirAddrCompare(uint8 *pAddrA,uint8 *pAddrB)
{
	if(*pAddrA != *pAddrB)
	{	
		return(0);
	} 
	if(*(pAddrA+1) != *(pAddrB+1))
	{	
		return(0);
	}
	if(*(pAddrA+2) != *(pAddrB+2))
	{	
		return(0);
	}
	return(1); 
}
/**********************************************************
*函数名称			:Bus0OutputData	
*函数描述        	:单总线0将待发送数据放入缓冲区
*输入参数   		:pbyData:待发送的数据指针
*返回值				:true:发送成功,false:发送失败
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 Bus0OutputData(uint8* pbyData)
{
	uint8 byTemp = BUS0_FREQ_SIZE;

	EA=0;
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//已经没有存储空间了
		EA=1;
		return(false);
	}	
	while(byTemp--)
	{
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	EA=1;	
	return(true);	
}

/**********************************************************
*函数名称			:Bus1OutputData	
*函数描述        	:单总线1将待发送数据放入缓冲区
*输入参数   		:pbyData:待发送的数据指针
*返回值				:TRUE:发送成功,FALSE:队列满,发送失败 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 Bus1OutputData(uint8* pbyData)
{
	uint8 byTemp = BUS1_FREQ_SIZE;

	EA=0;
	if(Bus1TxBuffLen() >= (BUS1_TX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		EA=1;
		return(FALSE);
	}  
	while(byTemp--)
	{	//数据入发送队列
		byBus1TxQ[byBus1TxTail] = *pbyData++;
		IncBus1TxPtr(byBus1TxTail);
	}
	EA=1;	
	return(TRUE);	
}

/**********************************************************
*函数名称			:MakeCH0TimerOut	
*函数描述        	:设置通道0超时参数
*输入参数   		:byTimerOut:超时时间计数,byTimerOutCount:超时次数
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH0TimerOut.byTimerOutSet = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOut = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOutCount = byTimerOutCount;	
}
/**********************************************************
*函数名称			:MakeCH1TimerOut	
*函数描述        	:设置通道1超时参数
*输入参数   		:byTimerOut:超时时间计数,byTimerOutCount:超时次数
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH1TimerOut.byTimerOutSet = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOut = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
}
 
/**********************************************************
*函数名称			:AddCallNod	
*函数描述        	:增加呼叫节点
*输入参数   		:pstBusFreq:待入链表的总线数据指针
*返回值				:SAVE_FAIL:失败,SAVE_NORMAL:成功,
					 SAVE_PRIO_HI:成功,且有高优先级				     
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 AddCallNod(pSTBusFreq pstBusFreq)
{
    pSTNod xdata pstNodTemp;
    pSTNod xdata pstCallNod;
    
    if(NULL == (uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
    {   //链表为空
		
    	//pstNodTemp = OSMemNew(sizeof(STNod));
        pstNodTemp = malloc(sizeof(STNod));
        if(NULL ==(uint8 xdata *)pstNodTemp)
        {        	
            return(SAVE_FAIL);
        }
        pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= (pstBusFreq->byCmd & 0x1f);   
        pstNodTemp->pstPrev = pstNodTemp->pstNext = NULL;
        stLocalControl.stCallPointer.pstTail = 
    		stLocalControl.stCallPointer.pstHead = pstNodTemp;    	
        return(SAVE_ONLY); 
    }
    //链表不为空,先查找是否有该节点了
    pstCallNod = stLocalControl.stCallPointer.pstHead;
    while(NULL != (uint8 xdata *)pstCallNod)
    {
        if((pstCallNod->byCallSecAddr == pstBusFreq->bySndSecAddr) &&
        	(pstCallNod->byCallRoomAddr == pstBusFreq->bySndRoomAddr) &&
        	(pstCallNod->byCallBedAddr == pstBusFreq->bySndBedAddr) &&
        	((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
        {	//已经有了该呼叫节点的信息
            return(SAVE_EXIST);
        }
        pstCallNod = pstCallNod->pstNext;
    }
    //没有相同的呼叫,重新排列呼叫优先级
    pstCallNod = stLocalControl.stCallPointer.pstTail;
	//查看是否为最后一个节点
	if(((pstCallNod->byCallPriority) & 0xe0) >= (pstBusFreq->byRecSecAddr & 0xe0))
	{	//为最后一个节点
		//pstNodTemp = OSMemNew(sizeof(STNod));
		pstNodTemp = malloc(sizeof(STNod));
        if(NULL ==(uint8 xdata *)pstNodTemp)
        {        	
            return(SAVE_FAIL);
        }
        pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= (pstBusFreq->byCmd & 0x1f); 
		pstCallNod->pstNext = pstNodTemp;
		pstNodTemp->pstPrev = pstCallNod;	
		pstNodTemp->pstNext	= NULL;
		stLocalControl.stCallPointer.pstTail = pstNodTemp;	
		if(NULL == (stLocalControl.stIndicationData.pstNext))
		{	//正在指示的节点为最后一个节点，将刚才添加的节点添加到后面
			stLocalControl.stIndicationData.pstNext = pstNodTemp;
		}	
		return(SAVE_NORMAL);		
	}
	//不是最后一个节点,向前查找
	pstCallNod = pstCallNod->pstPrev;
	while(NULL !=(uint8 xdata *)pstCallNod)
	{
		if(((pstCallNod->byCallPriority) & 0xe0) < (pstBusFreq->byRecSecAddr & 0xe0))
		{	//继续向前查找
			pstCallNod = pstCallNod->pstPrev;
		}
		else
		{	//找到合适的位置,不为第一个节点，插入节点
//		    pstNodTemp = OSMemNew(sizeof(STNod));  
			    pstNodTemp = malloc(sizeof(STNod));
	        if(NULL == (uint8 xdata *)pstNodTemp)
	        {        	
	            return(SAVE_FAIL);
	        }
			pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        	pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        	pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        	pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        	pstNodTemp->byCallPriority |= pstBusFreq->byCmd;
        	(pstCallNod->pstNext)->pstPrev = pstNodTemp;
        	pstNodTemp->pstPrev = pstCallNod;
        	pstNodTemp->pstNext = pstCallNod->pstNext;
        	pstCallNod->pstNext = pstNodTemp;   
        	if(pstNodTemp->pstNext == stLocalControl.stIndicationData.pstNext)
        	{	//该节点添加到了正在指示的节点的后面，更新正在指示的节点指针信息
        		stLocalControl.stIndicationData.pstNext = pstNodTemp;
        	}      	
        	return(SAVE_NORMAL);													
		}
	}
	//该插入的节点优先级为最高优先级，插入到链表的头部
	if(NULL == (uint8 xdata *)pstCallNod)
	{
		//pstNodTemp = OSMemNew(sizeof(STNod));
		pstNodTemp = malloc(sizeof(STNod));
	    if(NULL ==(uint8 xdata *)pstNodTemp)
	 	{        	
	 		return(SAVE_FAIL);
		}
		pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= pstBusFreq->byCmd;
        (stLocalControl.stCallPointer.pstHead)->pstPrev = pstNodTemp;
        pstNodTemp->pstNext = stLocalControl.stCallPointer.pstHead;
        pstNodTemp->pstPrev = NULL;
        stLocalControl.stCallPointer.pstHead = pstNodTemp;  
        stLocalControl.stIndicationData.byState = STATE_HI;              
        return(SAVE_PRIO_HI);
	}  
	return(SAVE_FAIL);
}
/**********************************************************
*函数名称			:RemoveCallNod	
*函数描述        	:删除呼叫节点
*输入参数   		:pstBusFreq:待删除的节点数据指针
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void RemoveCallNod(pSTBusFreq pstBusFreq)
{
	pSTNod xdata pstCallNod;
	
	//查找是否有该呼叫纪录
	pstCallNod = stLocalControl.stCallPointer.pstHead;
	if((uint8 xdata *)pstCallNod == NULL)
	{	//没有呼叫纪录,直接退出		
		return;
	}
	//如果删除的该呼叫为正在指示的节点，置正在指示的节点的不存在标志
	if((stLocalControl.stIndicationData.byIndicationSecAddr == pstBusFreq->byRecSecAddr) &&
		(stLocalControl.stIndicationData.byIndicationRoomAddr == pstBusFreq->byRecRoomAddr) &&
		(stLocalControl.stIndicationData.byIndicationBedAddr == pstBusFreq->byRecBedAddr) && 
		((stLocalControl.stIndicationData.byCallCmd & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
	{
		stLocalControl.stIndicationData.byState = STATE_DEL;
	}
	else
	{	//删除的节点不是正在指示的节点，看是否为后一个节点
		if(((uint8 xdata *)stLocalControl.stIndicationData.pstNext != NULL) && 
			(stLocalControl.stIndicationData.pstNext->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
			((stLocalControl.stIndicationData.pstNext)->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
			(stLocalControl.stIndicationData.pstNext->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
			((stLocalControl.stIndicationData.pstNext->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))	
		{	//是正在指示的呼叫的后一个节点，调整指针
			stLocalControl.stIndicationData.pstNext = stLocalControl.stIndicationData.pstNext->pstNext;
		}	
	}		
	if((pstCallNod->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
      (pstCallNod->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
      (pstCallNod->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
      ((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
 	{	//找到该呼叫信息，并且是最高优先级的
 		if(NULL ==(uint8 xdata *)pstCallNod->pstNext)
 		{	//该呼叫纪录为唯一的一条纪录
 			stLocalControl.stCallPointer.pstHead = 
 				stLocalControl.stCallPointer.pstTail = NULL; 
			//OSMemFree(pstCallNod);
 			free(pstCallNod); 			
 			return;			
 		}
 		else
 		{	//不是唯一的一条记录
 			stLocalControl.stCallPointer.pstHead = pstCallNod->pstNext;
 			(pstCallNod->pstNext)->pstPrev = NULL;
			//OSMemFree(pstCallNod);
 			free(pstCallNod);
 			return;
 		}
	}
	//不是第一条记录,继续查找
	pstCallNod = pstCallNod->pstNext;
	while(NULL != (uint8 xdata *)pstCallNod)
	{
		if((pstCallNod->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
	      (pstCallNod->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
	      (pstCallNod->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
	      ((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
	 	{	//找到该呼叫信息
	 		if(NULL == (uint8 xdata *)pstCallNod->pstNext)
	 		{	//该呼叫信息为最后一条记录,直接删除最后一个节点
	 			(pstCallNod->pstPrev)->pstNext = NULL;
	 			stLocalControl.stCallPointer.pstTail = pstCallNod->pstPrev;	 			
	 		}
	 		else
	 		{	//不是最后一条记录,从链表中删除该节点 
	 			(pstCallNod->pstPrev)->pstNext = pstCallNod->pstNext;
	 			(pstCallNod->pstNext)->pstPrev = pstCallNod->pstPrev;
	 		}
			//OSMemFree(pstCallNod);
	 		free(pstCallNod);
	 		return;
	 	}
	 	pstCallNod = pstCallNod->pstNext;
	}	
}
/**********************************************************
*函数名称			:CheckCallNod	
*函数描述        	:检查该节点是否还存在
*输入参数   		:pstBus0SendFreq:数据指针
*返回值				:true:查找到该节点,false:没有查找到该节点
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
/*uint8 CheckCallNod(pSTBusFreq pstBusFreq)
{
	pSTNod xdata pstCallNod;
	
	pstCallNod = stLocalControl.stCallPointer.pstHead;
    while(NULL != (uint8 xdata *)pstCallNod)
    {
        if((pstCallNod->byCallSecAddr == pstBusFreq->bySndSecAddr) &&
        	(pstCallNod->byCallRoomAddr == pstBusFreq->bySndRoomAddr) &&
        	(pstCallNod->byCallBedAddr == pstBusFreq->bySndBedAddr) &&
        	((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
        {	//该呼叫信息还存在
            return(true);
        }
        pstCallNod = pstCallNod->pstNext;
    }
   return(false);
}*/
/**********************************************************
*函数名称			:SaveIndicationData	
*函数描述        	:保存信息指示数据内容
*输入参数   		:pstBus0SendFreq:总线数据指针，byFistNod:是否为保存第一个节点信息
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SaveIndicationData(pSTBusFreq pstBusFreq, uint8 byFirstNod)
{
	//保存信息指示方的地址
	stLocalControl.stIndicationData.byIndicationSecAddr = pstBusFreq->bySndSecAddr;
	stLocalControl.stIndicationData.byIndicationRoomAddr = pstBusFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.byIndicationBedAddr = pstBusFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCallCmd = pstBusFreq->byRecSecAddr;
	stLocalControl.stIndicationData.byState = STATE_EXIST;
	if(byFirstNod)
	{	//保存第一个节点信息		
		stLocalControl.stIndicationData.pstNext = stLocalControl.stCallPointer.pstHead->pstNext;
	}
	else
	{	//保存当前记录的下一条记录			
		stLocalControl.stIndicationData.pstNext = stLocalControl.stIndicationData.pstNext->pstNext;
	}		
}


/**********************************************************
*函数名称			:StopVoicePlay	
*函数描述        	:停止振铃
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void StopVoicePlay(void)
{	
//	stLocalControl.stVoicePlay.byISD1700State &= ~(ISD_REPLAY|ISD_PLAYING);
}
/**********************************************************
*函数名称			:StartVoicePlay	
*函数描述        	:开始语音报号
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void StartVoicePlay(void)
{
//	stLocalControl.stVoicePlay.byISD1700State &= ~ISD_TIME_OVER;
//	stLocalControl.stVoicePlay.byISD1700State |= ISD_REPLAY;
}
/**********************************************************
*函数名称			:VoiceChannelCtx
*函数描述        	:语音通道切换处理函数
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-7
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void VoiceChannelCtx(void)
{
/*	if(stLocalControl.uiDevState & (FLAG_NOTE|FLAG_MUSIC))
	{	//背景音乐播放或者提示音状态		
		KBD = 1;
	}
	else 
	{		
		KBD = 0;
	}
	if(stLocalControl.uiDevState & (FLAG_WAIT_LISTEN|FLAG_INDICATION))
	{	//等待接听或者报号状态		
		KBC = 0;
		GB_FZ=0;
		KGB=1;		//通道0打开
		return;
	}
	if(stLocalControl.uiDevState & FLAG_BROADCAST)
	{	//广播状态				
//		 KGB = 0;
//		 KBC = 1;

//为了使广播音量加大 ,增加SD通道负载
//		KGB = 1;
//		KBC = 0;

		GB_FZ=1;
		return;
	} 	
	//其它状态，全部关闭			
	 KGB = 0;
	 KBC = 0;
	 GB_FZ=0;
	 */

	if(stLocalControl.uiDevState & (FLAG_NOTE|FLAG_MUSIC))
	{	//背景音乐播放或者提示音状态		
		KBD = 1;
		KBC = 0;
		KGB = 0;
		GB_FZ=0;		
	}
	else
	{
		if(stLocalControl.uiDevState & (/*FLAG_WAIT_LISTEN|*/FLAG_INDICATION))
		{//等待接听或者报号状态			
			KBD = 0;
			KBC = 0;
			KGB=1;		//通道0打开			
			GB_FZ=0;	
		}

		else 
		{
			if(stLocalControl.uiDevState & FLAG_BROADCAST)
			{	//广播状态				
				KBD = 0;
				KBC = 0;
				KGB=0;		
				GB_FZ=1;
			} 
			else
			{
			//其它状态，全部关闭
			 KBD = 0;	
			 KGB = 0;
			 KBC = 0;
			 GB_FZ=0;
			}
		}
	} 
}
/**********************************************************
*函数名称			:RestartIndication	
*函数描述        	:重新开始信息指示
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void RestartIndication(void)
{	
	pSTNod	xdata pstNod;
	
	if(stLocalControl.uiDevState & (FLAG_BUSY|FLAG_INDICATION_EXIST))
	{	//如果有呼叫指示的命令正在发送或者忙，直接退出
		return;
	}
	//检查上次报号记录
	if(STATE_HI == (stLocalControl.stIndicationData.byState))
	{	//有更高优先级的呼叫存在，直接从第一条呼叫开始			
FeltchFirst:
		//取出第一条记录
		pstNod = stLocalControl.stCallPointer.pstHead;
		if(NULL ==(uint8 xdata *)pstNod)
		{	//没有呼叫记录了，直接退出
		
			if(!(stLocalControl.uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
			{//没有通话存在
			    stLocalControl.byChar =0x00;	//系统不忙
	    		Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//报告系统不忙状态
			}		
			return;
		}										
		stLocalControl.stBusFreq.bySndSecAddr = pstNod->byCallSecAddr;
		stLocalControl.stBusFreq.bySndRoomAddr = pstNod->byCallRoomAddr;
		stLocalControl.stBusFreq.bySndBedAddr = pstNod->byCallBedAddr;
		stLocalControl.stBusFreq.byCmd = CMD_INFO_INDICATION;
		stLocalControl.stBusFreq.byRecSecAddr = pstNod->byCallPriority;
		stLocalControl.stBusFreq.byRecRoomAddr = 0x00;
		stLocalControl.stBusFreq.byRecBedAddr = 0x00;					
		if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
		{
			stLocalControl.stBusFreq.byCmd = pstNod->byCallPriority & 0x1f;
			SaveIndicationData(&(stLocalControl.stBusFreq), true);
			stLocalControl.uiDevState |= FLAG_INDICATION_EXIST;
		}
		return;	
	}
	else
	{	//没有更高优先级呼叫存在
		//看是否还有呼叫记录
		pstNod = stLocalControl.stCallPointer.pstHead;
		if(NULL ==(uint8 xdata *)pstNod)
		{	//没有呼叫记录了，直接退出
			if(!(stLocalControl.uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
			{//没有通话存在
		    stLocalControl.byChar =0x00;	//系统不忙
    		Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//报告系统不忙状态		
			}
			return;
		}	
		else
		{	//还有呼叫记录
			pstNod = stLocalControl.stIndicationData.pstNext;
			if(NULL == (uint8 xdata *)pstNod)
			{	//后面没有呼叫了，直接取第一条呼叫
				goto FeltchFirst;
			}
			else
			{	//后面还有呼叫
				if((stLocalControl.stIndicationData.byCallCmd & 0xe0)
					== (pstNod->byCallPriority & 0xe0))
				{	//是同一个优先级的呼叫，取出来																			
					stLocalControl.stBusFreq.bySndSecAddr = pstNod->byCallSecAddr;
					stLocalControl.stBusFreq.bySndRoomAddr = pstNod->byCallRoomAddr;
					stLocalControl.stBusFreq.bySndBedAddr = pstNod->byCallBedAddr;
					stLocalControl.stBusFreq.byCmd = CMD_INFO_INDICATION;
					stLocalControl.stBusFreq.byRecSecAddr = pstNod->byCallPriority;
					stLocalControl.stBusFreq.byRecRoomAddr = 0x00;
					stLocalControl.stBusFreq.byRecBedAddr = 0x00;										
					if(true == Bus0OutputData(&stLocalControl.stBusFreq.bySndSecAddr))
					{
						stLocalControl.stBusFreq.byCmd = pstNod->byCallPriority & 0x1f;
						SaveIndicationData(&(stLocalControl.stBusFreq), false);
						stLocalControl.uiDevState |= FLAG_INDICATION_EXIST;
					}
					return;								
				}
				else
				{	//后面的优先级低，直接取第一条呼叫
					goto FeltchFirst;
				}
			}
		}
	}	
}
/**********************************************************
*函数名称			:Bus0RecDeal
*函数描述        	:单总线0数据接收处理
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-7
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus0RecDeal(void)
{		
	static uint8 byTemp;
	static uint8 byCmd;
	static uint8 byPriority;	
	pSTUartFreq pstDataFreq;


	//复位看门狗                                                                                                                                                                                                                                                                                     
	WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M

	LedStateFlash();
	
	EA=0;			
	memcpy(&(stLocalControl.stBusFreq.bySndSecAddr), byBus0RecData, sizeof(STBusFreq));	//获取单总线数据
	bBus0RecFinish = 0;
	EA=1;	


	//调试用
	#if DEBUG==1
//	Usart1SndData(&(stLocalControl.stBusFreq));
	#endif
   	/////////////////////////////////////////////////////////////////////////////

	//数据处理	
	switch(stLocalControl.stBusFreq.byCmd)
	{
		case CMD_LANDING:									//登记命令,返回确认命令
			stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stBusFreq.bySndSecAddr;
			stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stBusFreq.bySndRoomAddr;
			stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stBusFreq.bySndBedAddr;
			stLocalControl.stBusFreq.byCmd = CMD_ENTER;
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{
				stLocalControl.stBusFreq.bySndSecAddr = 0x80;
			}
			else
			{
				stLocalControl.stBusFreq.bySndSecAddr = 0x00;
			}

			if(bPCState)
			{
				stLocalControl.stBusFreq.bySndSecAddr |= 0x40;
			}
			
			stLocalControl.stBusFreq.bySndRoomAddr = 0x00;
			stLocalControl.stBusFreq.bySndBedAddr = 0x00;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;
			
		case CMD_INSPECTOR_CALL:							//查房呼叫命令,返回确认命令
			errtime = 0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;		
			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;

			pstDataFreq->byData[0] = stLocalControl.stBusFreq.bySndBedAddr;
			memcpy(&(pstDataFreq->byData[1]),&(stLocalControl.stBusFreq.byRecSecAddr),3);
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
			pstDataFreq->uiLen = 0x04;
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;

			stLocalControl.stBusFreq.byCmd = CMD_INSPERCTOR_ENTER;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));			
			break;
		case CMD_INFUSION_CALL:								//输液呼叫命令
		case CMD_SERVICE_CALL:								//服务呼叫命令
		case CMD_EMERGENCY_CALL:							//紧急呼叫命令
		case CMD_HELP_CALL:									//求援呼叫命令
			if((stLocalControl.stBusFreq.bySndRoomAddr == ADD_BED_FJ)&&
			   (stLocalControl.stBusFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//是加床分机发过来的呼叫且区号不对 ,直接返回
				return;
			}
			//保存命令和优先级
			byCmd = stLocalControl.stBusFreq.byCmd;
			byPriority = stLocalControl.stBusFreq.byRecSecAddr & 0xe0;
			byTemp = AddCallNod(&(stLocalControl.stBusFreq));
			if(SAVE_FAIL == byTemp)
			{	//添加呼叫队列失败，直接返回
				return;
			}	

			//其他类型，都是添加成功，返回确认命令
			stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stBusFreq.bySndSecAddr;
			stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stBusFreq.bySndRoomAddr;
			stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stBusFreq.bySndBedAddr;
			
			stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusFreq.byCmd = byCmd + 0x1b;

			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));	

			
			if(flag_origin_music)
			{//之前在播放背景音乐,停止
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stBusFreq.byRecSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stBusFreq.byRecRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stBusFreq.byRecBedAddr;			
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stBusFreq.bySndSecAddr;
				stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stBusFreq.bySndRoomAddr;
				stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stBusFreq.bySndBedAddr;				
			}
			
			if(stLocalControl.uiDevState & (FLAG_INDICATION_EXIST|FLAG_BUSY|FLAG_TALK1))
			{	//有呼叫指示命令正在发送队列中,但没有发送完或者处于忙状态,退出
				return;
			}
			//没有指示正在发送,也不忙
			if(stLocalControl.uiDevState & FLAG_INDICATION)
			{	//有呼叫正在指示中
				if(SAVE_PRIO_HI == byTemp)
				{	//有高优先级呼叫存在,发送终止指示命令
					if(stLocalControl.uiDevState & FLAG_STOP_EXIST)
					{	//有正在发送停止命令
						return;
					}
					stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stIndicationData.byIndicationSecAddr;
					stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stIndicationData.byIndicationRoomAddr;
					stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stIndicationData.byIndicationBedAddr;
					stLocalControl.stBusFreq.byCmd = CMD_STOP_INDICATION;	
					stLocalControl.stBusFreq.byRecSecAddr =0;
					stLocalControl.stBusFreq.byRecRoomAddr=0;
					stLocalControl.stBusFreq.byRecBedAddr=0;					
					if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
					{
						stLocalControl.uiDevState |= FLAG_STOP_EXIST;
					}								
				}
			}
			else
			{	//没有呼叫在指示中,直接发送指示命令,保存呼叫指示的指针位置
				if(SAVE_ONLY == byTemp)
				{	//该呼叫是唯一一条呼叫记录
					if(stLocalControl.uiDevState & FLAG_STOP_EXIST)
					{	//有正在发送停止命令
						return;
					}
					stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stBusFreq.byRecSecAddr;
					stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stBusFreq.byRecRoomAddr;
					stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stBusFreq.byRecBedAddr;
					stLocalControl.stBusFreq.byCmd = CMD_INFO_INDICATION;
					stLocalControl.stBusFreq.byRecSecAddr = byPriority|byCmd;													
					if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
					{	//置指示正在发送队列中标志
						//保存指示方地址、命令、优先级和正在指示的呼叫记录指针
						stLocalControl.stBusFreq.byCmd = byCmd;
						SaveIndicationData(&(stLocalControl.stBusFreq), true);	
						stLocalControl.uiDevState |= FLAG_INDICATION_EXIST;
					}
					return;
				}
				else
				{	//有多条呼叫记录存在,但是没有FLAG_INDICATION_EXIST,
					//FLAG_BUSY,FLAG_INDICATION标志,表明指示发送完成后呼叫信息不存在了
					//停止信息指示正在发送,发送完成后再处理		
					if(stLocalControl.uiDevState & FLAG_STOP_EXIST)
					{	//有正在发送停止命令
						return;
					}
					byCmd = stLocalControl.stCallPointer.pstHead->byCallPriority & 0x1f;															
					stLocalControl.stBusFreq.bySndSecAddr = 
							stLocalControl.stCallPointer.pstHead->byCallSecAddr;
					stLocalControl.stBusFreq.bySndRoomAddr = 
							stLocalControl.stCallPointer.pstHead->byCallRoomAddr;
					stLocalControl.stBusFreq.bySndBedAddr = 
							stLocalControl.stCallPointer.pstHead->byCallBedAddr;
					stLocalControl.stBusFreq.byCmd = CMD_INFO_INDICATION;
					stLocalControl.stBusFreq.byRecSecAddr = 
							stLocalControl.stCallPointer.pstHead->byCallPriority;
					if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
					{	//保存指示方地址和命令
						stLocalControl.stBusFreq.byCmd = byCmd;
						SaveIndicationData(&(stLocalControl.stBusFreq), true);
						stLocalControl.uiDevState |= FLAG_INDICATION_EXIST;
					}
					return;	
				}								
			}			
			break;		
		case CMD_INFUSION_CLEAR:							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR:								//清除服务呼叫命令
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫命令
		case CMD_HELP_CLEAR:								//清除求援呼叫命令
			if((stLocalControl.stBusFreq.bySndRoomAddr == MOVE_FJ)&&
			   (stLocalControl.stBusFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//是移动分机发过来的清除呼叫且区号不对 ,直接返回
				return;
			}		
			if( (stLocalControl.stBusFreq.byRecSecAddr == stLocalControl.stBusFreq.bySndSecAddr) &&
				(stLocalControl.stBusFreq.byRecRoomAddr == stLocalControl.stBusFreq.bySndRoomAddr) &&
				(stLocalControl.stBusFreq.byRecBedAddr == stLocalControl.stBusFreq.bySndBedAddr) )
			{//是呼叫分机自己挂断
				errtime =0xff;
				while(stUsartCfg[1].uiSndLen!=0)	
				{//有数据正在发送中
					Delayms(1);
					WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
					if(--errtime ==0) break;

				}			
				pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
				pstDataFreq->byStartByte1 = START_BYTE1;
				
				memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.bySndSecAddr),3); //主呼分机地址
				memcpy(&(pstDataFreq->byData[3]),&(stLocalControl.stBusFreq.bySndSecAddr),3); //主呼分机地址			
				pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
				pstDataFreq->uiLen			=0X06;
				pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
				pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;				
				stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
				stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
				S2CON &= ~S2TI;
				S2BUF=pstDataFreq->byStartByte1;					
			}
			else
			{//是其它处理分机挂断分机
				errtime=0xff;
				while(stUsartCfg[1].uiSndLen!=0)	
				{//有数据正在发送中
					Delayms(1);
					WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
					if(--errtime ==0) break;
				}			
				pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
				pstDataFreq->byStartByte1 = START_BYTE1;
				
				memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.bySndSecAddr),3); //主呼分机地址
				memcpy(&(pstDataFreq->byData[3]),&(stLocalControl.stBusFreq.byRecSecAddr),3); //主呼分机地址			
				pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
				pstDataFreq->uiLen			=0X06;
				pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
				pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;				
				stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
				stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
				S2CON &= ~S2TI;
				S2BUF=pstDataFreq->byStartByte1;			
			}
			//清呼叫记录
			stLocalControl.stBusFreq.byCmd -= 0x0a;			//将清除命令还原成对应的呼叫命令		
			RemoveCallNod(&(stLocalControl.stBusFreq));			
			if((stLocalControl.uiDevState & FLAG_INDICATION) && 
				(stLocalControl.stBusFreq.byRecSecAddr == stLocalControl.stIndicationData.byIndicationSecAddr) &&
				(stLocalControl.stBusFreq.byRecRoomAddr == stLocalControl.stIndicationData.byIndicationRoomAddr) &&
				(stLocalControl.stBusFreq.byRecBedAddr == stLocalControl.stIndicationData.byIndicationBedAddr) &&
				((stLocalControl.stBusFreq.byCmd & 0x1f) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//正在呼叫指示，且清除的呼叫为正在指示的呼叫,发送终止呼叫指示命令
				if(stLocalControl.uiDevState & FLAG_STOP_EXIST)
				{	//有正在发送停止命令
					return;
				}
				//没有停止指示正在发送中				
				stLocalControl.stBusFreq.byCmd = CMD_STOP_INDICATION;				
				if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
				{
					stLocalControl.uiDevState |= FLAG_STOP_EXIST;
				}							
			}

			if(NULL ==(uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
			{//没有分机存在
				if(flag_origin_music)
				{//之前有播放背景音乐
				   stLocalControl.stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stLocalControl.stBusFreq.byRecSecAddr =0xff;
				   stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				   stLocalControl.stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));					   
				}
			}
			break;	
		case CMD_INFUSION_ANSWER:							//处理输液呼叫
		case CMD_SERVICE_ANSWER:							//处理服务呼叫
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫
		case CMD_HELP_ANSWER:								//处理求援呼叫	
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，退出
				break;
			}
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stLocalControl.stBusFreq));			
			stLocalControl.stCallAddr.byCmd = stLocalControl.stBusFreq.byCmd;


			//清呼叫记录
			stLocalControl.stBusFreq.byCmd -= 0x06;			//将处理命令还原成对应的呼叫命令
			RemoveCallNod(&(stLocalControl.stBusFreq));			
			//置等待应答和忙标志
			stLocalControl.uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);	
			if(stLocalControl.uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				stLocalControl.uiDevState &= ~FLAG_INDICATION;
				
//				//停止报号			
//				StopVoicePlay();

				//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				//语音通道切换
				VoiceChannelCtx();								
			}			

			//设置等待应答超时1S
			MakeCH0TimerOut(50, 0);			
			break;

			
		case CMD_COMM_CALL:									//普通呼叫命令
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，直接退出
				break;
			}
			//置忙和等待应答标志 
			stLocalControl.uiDevState |= (FLAG_BUSY|FLAG_WAIT_ACK);			
			if(stLocalControl.uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				stLocalControl.uiDevState &= ~FLAG_INDICATION;				
//				//停止报号			
//				StopVoicePlay();

				//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				//语音通道切换
				VoiceChannelCtx();								
			}
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stLocalControl.stBusFreq));			
			
			//设置等待应答超时5S
			MakeCH0TimerOut(50, 5);
			break;
		case CMD_COMM_ANSWER:								//应答命令
			if(0x0000 == (stLocalControl.uiDevState & FLAG_BUSY))
			{	//不忙，发送系统复位命令
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				break;
			}

			if(AddrCompare(&(stLocalControl.stBusFreq.bySndSecAddr),&(stLocalControl.stCallAddr.byRecSecAddr)))
			{//是对应分机的应答信号
				//忙,清等待应答标志 
				stLocalControl.uiDevState &= ~FLAG_WAIT_ACK;			
				//置等待接听标志			
				stLocalControl.uiDevState |= FLAG_WAIT_LISTEN;	

				
/*				//启动振铃
				MakeRingVoiceData(stLocalControl.stBusFreq.byRecBedAddr);	
				//构建语音信息
				//MakeRingVoiceData(&(stLocalControl.stIndicationData));	
				//播放语音
				StartVoicePlay();
				//语音通道切换
				VoiceChannelCtx();

*/
				//设置等待接听超时15S
				MakeCH0TimerOut(250, 3);	

			}
			else MakeCH0TimerOut(1, 0);	//使通道0超时时间到
			break;

		case CMD_CALL_LISTEN:							//接听命令
			if(0x0000 == (stLocalControl.uiDevState & FLAG_BUSY))
			{	//不忙，发送系统复位命令
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&stLocalControl.stBusFreq.bySndSecAddr);
				break;
			}

			if(AddrCompare(&(stLocalControl.stBusFreq.bySndSecAddr),&(stLocalControl.stCallAddr.byRecSecAddr)))
			{//是对应分机的接听信号			
				//忙,清等待接听标志，置通话标志 
				stLocalControl.uiDevState &= ~FLAG_WAIT_LISTEN;			
				stLocalControl.uiDevState |= FLAG_TALK;
				
//				//停止振铃
//				StopVoicePlay();	
				//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));


				//语音通道切换
				VoiceChannelCtx();
				//设置等待通话超时60S
				MakeCH0TimerOut(250, 12);			
				if(0x0000 == (stLocalControl.uiDevState & (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC)))
				{	//没有通话1，语音提示，背景音乐播放存在，发送切换语音通道命令								
					stLocalControl.stBusFreq.byCmd = CMD_CHANNEL_CHANGE;								
					Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
					//置通道切换命令在总线队列中标志
					stLocalControl.uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				}

				if((stLocalControl.stBusFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)&&
					(stLocalControl.stBusFreq.byRecRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
					(stLocalControl.stBusFreq.byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				{	//是管理主机处理床头分机的呼叫
					Usart1SndResult(byUsart1RecBuf[CMD_POSITION],TRUE);
					byUsart1RecBuf[CMD_POSITION]=0x00;	//清空原命令数据
				}
				else
				{	//是其他分机处理床头分机的呼叫
					errtime =0xff;
					while(stUsartCfg[1].uiSndLen!=0)	
					{//有数据正在发送中
						Delayms(1);
						WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
						if(--errtime ==0) break;
					}			
					pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
					pstDataFreq->byStartByte1 = START_BYTE1;
					memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.byRecSecAddr),3); //处理分机地址
					memcpy(&(pstDataFreq->byData[3]),&(stLocalControl.stBusFreq.bySndSecAddr),3); //主呼分机地址			
					pstDataFreq->byCmd = stLocalControl.stCallAddr.byCmd;	//原命令;
					pstDataFreq->uiLen			=0X06;
					pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
					pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;				
					stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
					S2CON &= ~S2TI;
					S2BUF=pstDataFreq->byStartByte1;
				}
			}
			else MakeCH0TimerOut(1, 0); //使通道0超时时间到
			break;		
		case CMD_BROADCAST1:								
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:								//广播命令
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，发送系统复位命令
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				break;
			}
			//不忙，置忙标志和广播标志
			stLocalControl.uiDevState |= (FLAG_BUSY|FLAG_BROADCAST);
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stLocalControl.stBusFreq));

			//清呼叫指示标志
			stLocalControl.uiDevState &= ~FLAG_INDICATION;
			
//			//停止报号			
//			StopVoicePlay();

			//向语音模块发送停止报号命令
			stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
			Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

			//语音通道切换
			VoiceChannelCtx();
			//设置等待广播超时180S
			MakeCH0TimerOut(250, 36);
			break;  
		case CMD_SYSTERM_RESET:								//系统复位命令
			//清标志 //这些标志之外的标志被清0
			stLocalControl.uiDevState &= (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_INDICATION_EXIST|FLAG_STOP_EXIST);
		
//			//停止报号
//			StopVoicePlay();

			//向语音模块发送停止报号命令
			stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
			Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

			//语音通道切换
			VoiceChannelCtx();
			//清超时
			MakeCH0TimerOut(0, 0);

			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;

			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			
			memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.bySndSecAddr),3); 
			memcpy(&(pstDataFreq->byData[3]),&(stLocalControl.stBusFreq.byRecSecAddr),3);
			pstDataFreq->byCmd = CMD_TALK_CLEAR;
			pstDataFreq->uiLen			=0X06;
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;				
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;

					
			//重新开始报号	
			if(0x00 == (stLocalControl.uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
			{
				RestartIndication();
			}	

			if(NULL ==(uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
			{//没有分机存在
				if(flag_origin_music)
				{//之前有播放背景音乐
				   stLocalControl.stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stLocalControl.stBusFreq.byRecSecAddr =0xff;
				   stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				   stLocalControl.stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));			   
				}
			}	
				
			break;
		case CMD_CHANNEL_CLOSE:								//关闭语音通道命令
			stLocalControl.uiDevState &= ~FLAG_TALK1;
			
			if(NULL ==(uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
			{	//没有呼叫记录了
			
				if(!(stLocalControl.uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//没有通话存在
					stLocalControl.byChar =0x00;	//系统不忙
					Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//报告系统不忙状态
				}	
			}	
	
			//语音通道切换
			VoiceChannelCtx();
			//清通道1通话超时
			MakeCH1TimerOut(0, 0);

			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;

			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			
			memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.bySndSecAddr),3); 
			memcpy(&(pstDataFreq->byData[3]),&(stLocalControl.stBusFreq.byRecSecAddr),3);
			pstDataFreq->byCmd = CMD_TALK_CLEAR;
			pstDataFreq->uiLen			=0X06;
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;				
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			
			if(stLocalControl.uiDevState & FLAG_TALK)
			{	//通道0通话存在，发送切换语音通道命令				
				stLocalControl.stBusFreq.byCmd = CMD_CHANNEL_CHANGE;
				//置通道切换命令在总线队列中标志
				stLocalControl.uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));	
			}


			if(NULL ==(uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
			{//没有分机存在
				if(flag_origin_music)
				{//之前有播放背景音乐
				   stLocalControl.stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stLocalControl.stBusFreq.byRecSecAddr =0xff;
				   stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				   stLocalControl.stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));			   
				}
			}	

			else
			{
				RestartIndication();
			}
			break;	

			
		case CMD_DATE_SEND:
			stTime.bySecond=0x00;
			stTime.byMinute = stLocalControl.stBusFreq.bySndRoomAddr;
			stTime.byHour   = stLocalControl.stBusFreq.bySndBedAddr;
			memcpy(&(stTime.byDay),&(stLocalControl.stBusFreq.byRecSecAddr),3);
			P8563_settime();			
			break;	
			
	   case CMD_BUS_ANSWER:	//总线通用应答命令
		   switch(stLocalControl.stBusFreq.byRecSecAddr)
			{
				case CMD_COLOR_CLEAR:
				case CMD_BL_TIMER_SET:
				case CMD_VOICE_TIMER_SET:
					stLocalControl.uiDevState &= ~(FLAG_WAIT_COMM_ACK);
					//清等待总线应答超时
					stLocalControl.byWaitCommAckDTime = 0;
 				
					Usart1SndResult(stLocalControl.stBusFreq.byRecSecAddr,TRUE);
					return;
					
				case CMD_DISPLAY_DATA:
				case CMD_HANDLE_DISPLAY_DATA:	
					if(/*(stLocalControl.stBusFreq.bySndRoomAddr == YHFJ_ADDR2) ||(stLocalControl.stBusFreq.bySndRoomAddr == ZLDIS_ADDR2)||*/
					   (stLocalControl.stBusFreq.bySndBedAddr == stLocalControl.stRS485Addr.byBedAddr))
					{//床号相等
						stLocalControl.byWaitRS485AckDTime = 0;
						errtime =0xff;
						while(stUsartCfg[1].uiSndLen!=0)	
						{//有数据正在发送中
							Delayms(1);
							WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
							if(--errtime ==0) break;

						}
						pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
						
						pstDataFreq->byStartByte1	=START_BYTE1;
						pstDataFreq->bySndSecAddr	=stLocalControl.stBusFreq.bySndSecAddr;
						pstDataFreq->bySndRoomAddr	=stLocalControl.stBusFreq.bySndRoomAddr;
						pstDataFreq->bySndBedAddr	=stLocalControl.stBusFreq.bySndBedAddr;
//						if((stLocalControl.stBusFreq.bySndRoomAddr == YHFJ_ADDR2) )
//							pstDataFreq->bySndBedAddr	=0xff;				//医护分机第3个地址发广播地址
						
						pstDataFreq->byRecSecAddr	=stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstDataFreq->byRecRoomAddr	=PC_ADDR2;
						pstDataFreq->byRecBedAddr	=PC_ADDR3;
						pstDataFreq->byCmd			=CMD_RESULT_PC; 	//确认命令
						pstDataFreq->uiLen			=0X02;
						pstDataFreq->byData[0]		=stLocalControl.stBusFreq.byRecSecAddr;
						pstDataFreq->byData[1]		=TRUE;		//成功或失败
						pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
						pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;	
						stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
						stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
						
						S2CON &= ~S2TI;
						S2BUF=pstDataFreq->byStartByte1;
						
						stLocalControl.stRS485Addr.byBedAddr=0; 	//结果发完，将485床号清零

					}
					return;

				default:
					break;
		   	}	
		   
	   		//if(AddrCompare(&(stLocalControl.stBusFreq.bySndSecAddr),&(stLocalControl.stCommAddr.bySecAddr)))
	   		if((stLocalControl.stBusFreq.bySndBedAddr == stLocalControl.stCommAddr.byBedAddr)||
				(stLocalControl.stBusFreq.bySndBedAddr == 0xff) ||
				(stLocalControl.stCommAddr.byBedAddr == 0xff) )
	   		{
				stLocalControl.uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				stLocalControl.byWaitCommAckDTime = 0;
				switch(stLocalControl.stBusFreq.byRecSecAddr)
				{
				case CMD_POWER_ON:
				case CMD_ONLINE_WRITE:
				case CMD_SCHEDULE_SEND:
				case CMD_OPEN_LCD:
				case CMD_CLOSE_LCD:
				case CMD_CLEAR_LCD:
				case CMD_CLEAR_HANDLE_LCD:
				case CMD_CLEAR_MK_LCD:						
					Usart1SndResult(stLocalControl.stBusFreq.byRecSecAddr,TRUE);
					break;
				default:
					break;
				}
	   		
	   		}
			else stLocalControl.byWaitCommAckDTime = 1;	//地址不区配,提前退出
			break;
		case CMD_DATA_ERROR:		//一帧数据出错
			//清等待应答超时
			if(stLocalControl.stBusFreq.bySndBedAddr == stLocalControl.stRS485Addr.byBedAddr)
			{//床号相等			
	             stLocalControl.byWaitRS485AckDTime = 0;						
				 Usart1SndRS485Result(byUsart0SndBuf[CMD_POSITION],FALSE);
			}
			break;
		case CMD_ONLINE_OUTPUT:
	   		if((stLocalControl.stBusFreq.bySndSecAddr==stLocalControl.stCommAddr.bySecAddr)&&
				(stLocalControl.stBusFreq.bySndRoomAddr==stLocalControl.stCommAddr.byRoomAddr)&&
				(stLocalControl.stBusFreq.bySndBedAddr==stLocalControl.stCommAddr.byBedAddr))
	   		{
				stLocalControl.uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				stLocalControl.byWaitCommAckDTime = 0;
				Usart1SndResult1(CMD_ONLINE_READ,TRUE,stLocalControl.stBusFreq.byRecSecAddr);		
	   		}
			else stLocalControl.byWaitCommAckDTime = 1;	//地址不区配,提前退出
			break;

		case CMD_QUEST_ANSWER:
	   		if((stLocalControl.stBusFreq.bySndSecAddr==stLocalControl.stCommAddr.bySecAddr)&&
				(stLocalControl.stBusFreq.bySndRoomAddr==stLocalControl.stCommAddr.byRoomAddr)&&
				(stLocalControl.stBusFreq.bySndBedAddr==stLocalControl.stCommAddr.byBedAddr))
	   		{
				//stLocalControl.uiDevState &= ~(FLAG_WAIT_ACK|FLAG_BUSY);
				stLocalControl.uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				stLocalControl.byWaitCommAckDTime = 0;
				Usart1SndResult3(byUsart1RecBuf[CMD_POSITION],TRUE,&(stLocalControl.stBusFreq.byRecSecAddr),3);	//发送附带参数的结果码			
	   		}
			else stLocalControl.byWaitCommAckDTime = 1;	//地址不区配,提前退出
			break;

		case CMD_NURSE_IN:
		case CMD_NURSE_OUT:
			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;
			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			pstDataFreq->bySndSecAddr	=stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->bySndRoomAddr	=stLocalControl.stEepromCfgData.bySelfRoomAddr;
			pstDataFreq->bySndBedAddr	=stLocalControl.stEepromCfgData.bySelfBedAddr;
			pstDataFreq->byRecSecAddr	=stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->byRecRoomAddr	=PC_ADDR2;
			pstDataFreq->byRecBedAddr	=PC_ADDR3;
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
			pstDataFreq->uiLen = 04;
			pstDataFreq->byData[0]= stLocalControl.stBusFreq.bySndBedAddr;
			memcpy(&(pstDataFreq->byData[1]),&(stLocalControl.stBusFreq.byRecSecAddr),3);
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			break;

		case CMD_SUPPLY_OX_START:			
		case CMD_SUPPLY_OX_END:
			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;
			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			pstDataFreq->bySndSecAddr	=stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->bySndRoomAddr	=stLocalControl.stEepromCfgData.bySelfRoomAddr;
			pstDataFreq->bySndBedAddr	=stLocalControl.stEepromCfgData.bySelfBedAddr;
			pstDataFreq->byRecSecAddr	=stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->byRecRoomAddr	=PC_ADDR2;
			pstDataFreq->byRecBedAddr	=PC_ADDR3;
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
			pstDataFreq->uiLen = 0x05;
			memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.bySndSecAddr),3);
			pstDataFreq->byData[3]=stLocalControl.stBusFreq.byRecRoomAddr;
			pstDataFreq->byData[4]=stLocalControl.stBusFreq.byRecBedAddr;
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			break;

		case CMD_VOICE_TIMER_SET:
			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;

			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			memcpy(&(pstDataFreq->bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
			memset(&(pstDataFreq->byRecSecAddr),0x00,3);
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
			pstDataFreq->uiLen = 0x05;
			pstDataFreq->byData[0]=stLocalControl.stBusFreq.bySndSecAddr;
			pstDataFreq->byData[1]=stLocalControl.stBusFreq.bySndRoomAddr;
			pstDataFreq->byData[2]=stLocalControl.stBusFreq.byRecSecAddr;
			pstDataFreq->byData[3]=stLocalControl.stBusFreq.byRecRoomAddr;
			pstDataFreq->byData[4]=stLocalControl.stBusFreq.byRecBedAddr;
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			break;

		case CMD_BL_TIMER_SET:
			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;
			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			memcpy(&(pstDataFreq->bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
			memset(&(pstDataFreq->byRecSecAddr),0x00,3);
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
			pstDataFreq->uiLen = 0x04;
			pstDataFreq->byData[0]=stLocalControl.stBusFreq.bySndSecAddr;
			pstDataFreq->byData[1]=stLocalControl.stBusFreq.bySndRoomAddr;
			pstDataFreq->byData[2]=stLocalControl.stBusFreq.byRecSecAddr;
			pstDataFreq->byData[3]=stLocalControl.stBusFreq.byRecRoomAddr;

			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			break;
		case CMD_COLOR_CLEAR:
			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;
			}
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			memcpy(&(pstDataFreq->bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
			memset(&(pstDataFreq->byRecSecAddr),0x00,3);
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd;
			pstDataFreq->uiLen = 0x03;
			pstDataFreq->byData[0]=stLocalControl.stBusFreq.bySndSecAddr;
			pstDataFreq->byData[1]=stLocalControl.stBusFreq.bySndRoomAddr;
			pstDataFreq->byData[2]=stLocalControl.stBusFreq.bySndBedAddr;
 

			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			break;
			

		default:
			break;
	}
}


/**********************************************************
*函数名称			:Bus0SendFinishDeal
*函数描述        	:单总线0数据发送完成处理
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-7
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus0SendFinishDeal(void)
{	 
	pSTUartFreq pstDataFreq;

	//复位看门狗                                                                                                                                                                                                                                                                                     
	WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M	
	EA=0;	
	bBus0SendFinish=0;		
	memcpy(&(stLocalControl.stBusFreq.bySndSecAddr), byBus0SendData, sizeof(STBusFreq));	//获取单总线数据
	EA=1;

//调试用
	#if DEBUG==1
//	Usart1SndData(&(stLocalControl.stBusFreq));
	///////////////////////////////////////////////////////////////////////////// 
	#endif
	
	//数据处理	
	switch(stLocalControl.stBusFreq.byCmd)
	{
		case CMD_POWER_ON:									//上电复位命令发送完成
			if(stLocalControl.uiDevState & FLAG_LANDING)
			{	//确实处在上电状态
				stLocalControl.uiDevState &= ~FLAG_LANDING;
				stLocalControl.uiDetectPcTime = 250;
			}
			else
			{
				//置等待应答和忙标志
				stLocalControl.uiDevState |= (FLAG_WAIT_COMM_ACK);
				//保存分机地址
				stLocalControl.stCommAddr.bySecAddr = stLocalControl.stBusFreq.byRecSecAddr;
				stLocalControl.stCommAddr.byRoomAddr = stLocalControl.stBusFreq.byRecRoomAddr;
				stLocalControl.stCommAddr.byBedAddr = stLocalControl.stBusFreq.byRecBedAddr;
				//设置等待应答超时1S
				stLocalControl.byWaitCommAckDTime = 50;				
			}
			break;
		case CMD_INFO_INDICATION:							//呼叫指示命令			
			stLocalControl.uiDevState &= ~FLAG_INDICATION_EXIST;
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，退出				
				break;
			}			
			//检查该记录			
			if((stLocalControl.stIndicationData.byState == STATE_HI) ||
				(stLocalControl.stIndicationData.byState == STATE_DEL))
			{	//有高优先级呼叫存在或者该呼叫已经不存在了，发送终止呼叫命令
				if(stLocalControl.uiDevState & FLAG_STOP_EXIST)
				{	//有正在发送停止命令
					return;
				}
				stLocalControl.stBusFreq.byCmd = CMD_STOP_INDICATION;						
				if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
				{
					stLocalControl.uiDevState |= FLAG_STOP_EXIST;
				}				
			}
			else
			{	//该呼叫还存在于记录中,且没有高优先级的呼叫存在,置指示标志
				stLocalControl.uiDevState |= FLAG_INDICATION;
				
/*				//构建语音信息
				MakeCallVoiceData(&(stLocalControl.stIndicationData));	
				//播放语音
				StartVoicePlay();
*/

				//因中医研究院分机多，线路长，SD与SA之间存在很大的线间电容，造成在对讲时，
				//如果语音报号，医护分机通话时有很大的语音报号声，造成通话听不到。
//				if((stLocalControl.uiDevState &(FLAG_TALK |FLAG_TALK1)) ==0)
				{

					//将单总线0上的数据原样发给单总线1
					Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
					//语音通道切换
					VoiceChannelCtx();
					//设置报号超时15S
					MakeCH0TimerOut(250, 3);
				}
			}							
			break;
		case CMD_STOP_INDICATION:							//终止呼叫指示命令			
			//清指示标志
			stLocalControl.uiDevState &= ~(FLAG_INDICATION|FLAG_STOP_EXIST);
			//清报号超时
			MakeCH0TimerOut(0, 0);
			
//			//停止报号
//			StopVoicePlay();

			//向语音模块发送停止报号命令
			stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
			Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

			//语音通道切换
			VoiceChannelCtx();			
			//不忙，且没有呼叫指示正在发送队列中,重新开始报号			
			RestartIndication();			
			break;	
		case CMD_SYSTERM_RESET:								//系统复位命令
			stLocalControl.uiDevState &= (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_INDICATION_EXIST|FLAG_STOP_EXIST);	// 将此几位保持原状态,其它标志清0		

//			//停止报号
//			StopVoicePlay();

			//向语音模块发送停止报号命令
			stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
			Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));		
			
			//语音通道切换
			VoiceChannelCtx();
			//清超时
			MakeCH0TimerOut(0, 0);				
			//重新开始报号
			if(0x00 == (stLocalControl.uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
			{
				RestartIndication();
			}									
			break;	
		case CMD_START_VOICE:								//语音提示命令
			//清通道1通话标志和超时等待
			stLocalControl.uiDevState &= ~FLAG_TALK1;			
			MakeCH1TimerOut(0, 0);
			//置语音提醒标志
			stLocalControl.uiDevState |= FLAG_NOTE;
			//语音通道切换
			VoiceChannelCtx();	
			Usart1SndResult(CMD_START_VOICE,TRUE);
			break;
		case CMD_MUSIC_PLAY:								//背景音乐播放标志
			//清通道1通话标志和超时等待
			stLocalControl.uiDevState &= ~FLAG_TALK1;				
			MakeCH1TimerOut(0, 0);
			//置背景音乐标志
			stLocalControl.uiDevState |= FLAG_MUSIC;
			//语音通道切换
			VoiceChannelCtx();
			Usart1SndResult(CMD_MUSIC_PLAY,TRUE);
			break;
		case CMD_STOP_VOICE:
			//清播音和背景音乐标志
			stLocalControl.uiDevState &= ~(FLAG_NOTE|FLAG_MUSIC);
			//语音通道切换
			VoiceChannelCtx();
			Usart1SndResult(CMD_STOP_VOICE,TRUE);
			
			if(NULL ==(uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
			{	//没有呼叫记录了，直接退出
			
				if(!(stLocalControl.uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//没有通话存在
					stLocalControl.byChar =0x00;	//系统不忙
					Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//报告系统不忙状态
				}	
			}			
			break;	
		case CMD_INFUSION_CLEAR:							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR:								//清除服务呼叫
		case CMD_HELP_CLEAR:								//清除求援呼叫
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫
			//暂存原命令数据
			stLocalControl.byChar = stLocalControl.stBusFreq.byCmd;	
			
			//清呼叫记录
			stLocalControl.stBusFreq.byCmd -= 0x0a;			//将清除命令还原成对应的呼叫命令		
			RemoveCallNod(&(stLocalControl.stBusFreq));			
			if((stLocalControl.uiDevState & FLAG_INDICATION) && 
				(stLocalControl.stBusFreq.byRecSecAddr == stLocalControl.stIndicationData.byIndicationSecAddr) &&
				(stLocalControl.stBusFreq.byRecRoomAddr == stLocalControl.stIndicationData.byIndicationRoomAddr) &&
				(stLocalControl.stBusFreq.byRecBedAddr == stLocalControl.stIndicationData.byIndicationBedAddr) &&
				(stLocalControl.stBusFreq.byCmd == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//正在呼叫指示,且清除的呼叫为正在指示的呼叫,清报号标志
				stLocalControl.uiDevState &= ~FLAG_INDICATION;
				
//				//停止报号
//				StopVoicePlay();

				//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				//语音通道切换
				VoiceChannelCtx();
				//清超时
				MakeCH0TimerOut(0, 0);				
				//重新开始报号
				if(0x00 == (stLocalControl.uiDevState&FLAG_STOP_EXIST))
				{
					RestartIndication();
				}
			}
			stLocalControl.stBusFreq.byCmd = stLocalControl.byChar;										
			Usart1SndResult(stLocalControl.stBusFreq.byCmd,TRUE);			
			break;	
		case CMD_CHANNEL_CHANGE:							//通道切换命令
			if(stLocalControl.uiDevState & FLAG_TALK)
			{	//通话状态
				stLocalControl.uiDevState &= ~(FLAG_BUSY|FLAG_TALK);

				//将通道0的主动呼叫方地址复制到通道1的主动呼叫方地址中
				memcpy(&(stLocalControl.stCH1CallAddr),&(stLocalControl.stCallAddr),sizeof(STAddr));
				//将通道0的主动呼叫方地址清0
				stLocalControl.stCallAddr.bySndSecAddr = 0x00;
				stLocalControl.stCallAddr.bySndRoomAddr = 0x00;
				stLocalControl.stCallAddr.bySndBedAddr = 0x00;
				stLocalControl.stCallAddr.byRecSecAddr = 0x00;
				stLocalControl.stCallAddr.byRecRoomAddr = 0x00;
				stLocalControl.stCallAddr.byRecBedAddr = 0x00;				
				//清通道切换在总线队列中标志
				stLocalControl.uiDevState &= ~(FLAG_CHANNEL_CHANGE_EXIST);


				//清超时
				MakeCH0TimerOut(0, 0);	
				//置通话1标志
				stLocalControl.uiDevState |= FLAG_TALK1;					
				//语音通道切换
				VoiceChannelCtx();
				//通道1通话定时60S
				MakeCH1TimerOut(250, 12);


				//因中医研究院分机多，线路长，SD与SA之间存在很大的线间电容，造成在对讲时，
				//如果语音报号，医护分机通话时有很大的语音报号声，造成通话听不到?

				
/*				//重新开始报号
				if(0x00 == (stLocalControl.uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
				{

					RestartIndication();
				}*/
			}
			else
			{	//不是通话状态，发送关闭通道命令
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stCallAddr.bySndBedAddr;			
				stLocalControl.stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
				stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
				stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
				stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}					
			break;
			
		case CMD_CHANNEL_CLOSE:			
			stLocalControl.uiDevState &= ~FLAG_TALK1;
			
			if(NULL ==(uint8 xdata *)(stLocalControl.stCallPointer.pstHead))
			{	//没有呼叫记录了，直接退出
			
				if(!(stLocalControl.uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//没有通话存在
					stLocalControl.byChar =0x00;	//系统不忙
					Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//报告系统不忙状态
				}	
			}	
			
			//语音通道切换
			VoiceChannelCtx();
			//清通道1通话超时
			MakeCH1TimerOut(0, 0);
			if(stLocalControl.uiDevState & FLAG_TALK)
			{	//通道0通话存在，发送切换语音通道命令				
				stLocalControl.stBusFreq.byCmd = CMD_CHANNEL_CHANGE;
				//置通道切换命令在总线队列中标志
				stLocalControl.uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));	
			}
			break;

		case CMD_INFUSION_ENTER:		//发送完呼叫确认命令后才将此信息上传至PC机
		case CMD_SERVICE_ENTER:
		case CMD_EMERGENCY_ENTER:
		case CMD_HELP_ENTER:
			errtime =0xff;
			while(stUsartCfg[1].uiSndLen!=0)	
			{//有数据正在发送中
				Delayms(1);
				WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M  
				if(--errtime ==0) break;
			}			
			pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
			pstDataFreq->byStartByte1 = START_BYTE1;
			pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
			pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
			pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->byRecRoomAddr  =PC_ADDR2;
			pstDataFreq->byRecBedAddr   =PC_ADDR3;			
			memcpy(&(pstDataFreq->byData[0]),&(stLocalControl.stBusFreq.byRecSecAddr),3);
			pstDataFreq->byCmd = stLocalControl.stBusFreq.byCmd-0x1b;
			pstDataFreq->uiLen = 0x03;
			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
						
			stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
			S2CON &= ~S2TI;
			S2BUF=pstDataFreq->byStartByte1;
			break;
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，退出
				Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);	//发送操作失败命令
				break;
			}	
			//清呼叫记录
			stLocalControl.stBusFreq.byCmd -= 0x06;			//将处理命令还原成对应的呼叫命令
			RemoveCallNod(&(stLocalControl.stBusFreq));			
			//置等待应答和忙标志
			stLocalControl.uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);	
			if(stLocalControl.uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				stLocalControl.uiDevState &= ~FLAG_INDICATION;
				
//				//停止报号			
//				StopVoicePlay();

				//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				//语音通道切换
				VoiceChannelCtx();								
			}	
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stLocalControl.stBusFreq));

			//设置等待应答超时1S
			MakeCH0TimerOut(50, 0);			
			break;
		case CMD_COMM_CALL:									//普通呼叫命令
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，直接退出
				break;
			}
			//置忙和等待应答标志 
			stLocalControl.uiDevState |= (FLAG_BUSY|FLAG_WAIT_ACK);			
			if(stLocalControl.uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				stLocalControl.uiDevState &= ~FLAG_INDICATION;
				
//				//停止报号			
//				StopVoicePlay();

				//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				//语音通道切换
				VoiceChannelCtx();								
			}	
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stLocalControl.stBusFreq));

			//设置等待应答超时1S
			MakeCH0TimerOut(50, 0);
			break;		
		case CMD_ONLINE_WRITE:
		case CMD_ONLINE_READ:
		case CMD_SCHEDULE_SEND:
		case CMD_QUEST:
			//置等待应答和忙标志
			//stLocalControl.uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);
			stLocalControl.uiDevState |= (FLAG_WAIT_COMM_ACK);
			//保存分机地址
			stLocalControl.stCommAddr.bySecAddr = stLocalControl.stBusFreq.byRecSecAddr;
			stLocalControl.stCommAddr.byRoomAddr = stLocalControl.stBusFreq.byRecRoomAddr;
			stLocalControl.stCommAddr.byBedAddr = stLocalControl.stBusFreq.byRecBedAddr;
			//设置等待应答超时1S
			stLocalControl.byWaitCommAckDTime = 50;
			break;
		case CMD_OPEN_LCD:
		case CMD_CLOSE_LCD:
		case CMD_CLEAR_LCD:
		case CMD_CLEAR_HANDLE_LCD:
		case CMD_CLEAR_MK_LCD:
			if((stLocalControl.stBusFreq.byRecSecAddr == 0xff)||(stLocalControl.stBusFreq.byRecRoomAddr == 0xff)||(stLocalControl.stBusFreq.byRecBedAddr == 0xff))
			{//是广播地址
				Usart1SndResult(byUsart1RecBuf[CMD_POSITION],TRUE);				//发送操作成功命令
			}			
			else
			{//单个
				//置等待应答和忙标志
				//stLocalControl.uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);
				stLocalControl.uiDevState |= (FLAG_WAIT_COMM_ACK);
				//保存分机地址
				stLocalControl.stCommAddr.bySecAddr = stLocalControl.stBusFreq.byRecSecAddr;
				stLocalControl.stCommAddr.byRoomAddr = stLocalControl.stBusFreq.byRecRoomAddr;
				stLocalControl.stCommAddr.byBedAddr = stLocalControl.stBusFreq.byRecBedAddr;
				//设置等待应答超时1S
				stLocalControl.byWaitCommAckDTime = 50;			
			}
			break;
		case CMD_BROADCAST1:								
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:								//广播命令
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，发送系统复位命令
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				Usart1SndResult(stLocalControl.stBusFreq.byCmd,FALSE);
				break;
			}
			//不忙，置忙标志和广播标志
			stLocalControl.uiDevState |= (FLAG_BUSY|FLAG_BROADCAST);
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stLocalControl.stBusFreq));

			//清呼叫指示标志
			stLocalControl.uiDevState &= ~FLAG_INDICATION;
			
//			//停止报号			
//			StopVoicePlay();

			//暂存原命令
			stLocalControl.byChar = stLocalControl.stBusFreq.byCmd;

			//向语音模块发送停止报号命令
			stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
			Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

			//语音通道切换
			VoiceChannelCtx();
			//设置等待广播超时180S
			MakeCH0TimerOut(250, 36);

			stLocalControl.stBusFreq.byCmd= stLocalControl.byChar;
			Usart1SndResult(stLocalControl.stBusFreq.byCmd,TRUE);
			break;
		case CMD_VOICE_TIMER_SET:
		case CMD_BL_TIMER_SET:
			//置等待应答标志
			stLocalControl.uiDevState |= (FLAG_WAIT_COMM_ACK);	
			break;
			
		case CMD_RS485_BRT_SET:
			stLocalControl.byReSndBrtTimes--;
			if(stLocalControl.byReSndBrtTimes!=0) 
			{				
				Delayms(500);	//延时1S,再发送一次
				if(stLocalControl.byReSndBrtTimes == RE_SND_TIMES-1)
				{
					Usart1SndResult(stLocalControl.stBusFreq.byCmd,TRUE);	//发送完第一次发结果命令
				}
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));			
			}
			break;


		case CMD_RS485_INFO_SEC_ROOM:
			stLocalControl.byReSndRs485RoomTimes--;
			if(stLocalControl.byReSndRs485RoomTimes!=0)
			{
				Delayms(500);	//延时1S,再发送一次
				if(stLocalControl.byReSndRs485RoomTimes == RE_SND_TIMES-1)
				{
					Usart1SndResult(stLocalControl.stBusFreq.byCmd,TRUE);				
				}
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));	
			}
			break;
			
		case CMD_BED_VOICE_PLAY:
			stLocalControl.byReSndBedVoiPlaTimes--;
			if(stLocalControl.byReSndBedVoiPlaTimes!=0)
			{
				Delayms(500);	//延时1S,再发送一次	
				if(stLocalControl.byReSndBedVoiPlaTimes == RE_SND_TIMES-1)
				{
					Usart1SndResult(stLocalControl.stBusFreq.byCmd,TRUE);				
				}
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));	
			}
			break;			

		case CMD_SYSTEM_SEC_SET:
			Delayms(500);
			Usart1SndResult(CMD_NUMBER_SET,TRUE);				//发送操作成功命令
			break;
			
		case CMD_DISP_MODE:
			Delayms(500);
			Usart1SndResult(stLocalControl.stBusFreq.byCmd,TRUE);
			break;

		case CMD_COLOR_CLEAR:
			stLocalControl.uiDevState |= (FLAG_WAIT_COMM_ACK); 
			//设置等待应答超时1S
			stLocalControl.byWaitCommAckDTime = 50; 
			break;
			
		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 0;								//制造总线故障
			SaveParameter(IAP0_ADDR);
			Bus0SendPin = 1;								//释放总线				
			break;			
		default:
			break;

	}
}

/**********************************************************
*函数名称			:Bus1RecDeal	
*函数描述        	:单总线1收到一帧数据处理函数,该函数首先
					 取出收到的数据,针对每条命令执行对应的控
					 制动作
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus1RecDeal(void)
{
	//取出单总线1收到的数据帧	
	EA=0;
	memcpy(&(stLocalControl.stBusFreq), byBus1RecData, sizeof(STBusFreq));	
	bBus1RecFinish = 0;
	EA=1;

//调试用
	#if DEBUG==1
//	Usart1SndData(&(stLocalControl.stBusFreq));
	///////////////////////////////////////////////////////////////////////////// 
	#endif	

	//数据处理	
	switch(stLocalControl.stBusFreq.byCmd)
	{
		case CMD_STOP_VOICE_PLAY:
			if(stLocalControl.uiDevState & FLAG_INDICATION)
			{
				if(0x00 == (stLocalControl.uiDevState & FLAG_STOP_EXIST))
				{	//没有正在发送停止命令

					stLocalControl.stBusFreq.byCmd = CMD_STOP_INDICATION;						
					if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
					{
						stLocalControl.uiDevState |= FLAG_STOP_EXIST;
					}						
				}
			}
			break;
			
		default:
			break;
	}
}
/**********************************************************
*函数名称			:Bus1SendDeal	
*函数描述        	:单总线1发送完一帧数据处理函数,该函数首先
					 取出收到的数据,针对每条命令执行对应的控
					 制动作
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus1SendFinishDeal(void)
{ 
	//取出数据帧
	EA=0;	
	bBus1SendFinish = 0;
	memcpy(&(stLocalControl.stBusFreq), byBus1SendData, sizeof(STBusFreq));	
	EA=1;	
	
//调试用
	#if DEBUG==1
//	Usart1SndData(&(stLocalControl.stBusFreq));
	#endif	
	///////////////////////////////////////////////////////////////////////////// 
	switch(stLocalControl.stBusFreq.byCmd)
	{
		case CMD_VOICE_PLAY_ROOM:
			Usart1SndResult(CMD_VOICE_PLAY_ROOM,TRUE);				//发送操作成功命令
			break;

		case CMD_VOICE_TIMES:
			Usart1SndResult(CMD_VOICE_TIMES,TRUE);				//发送操作成功命令
			break;

			
		default:
			break;
	}

}

/**********************************************************
*函数名称			:TimerOutDeal
*函数描述        	:超时处理程序
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-7
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void TimerOutDeal(void)
{
//	wdt_reset();											//复位看门狗	
	//通话通道0超时处理
	if(stLocalControl.stCH0TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stLocalControl.stCH0TimerOut.byTimerOut--;
		if(stLocalControl.stCH0TimerOut.byTimerOut == 0)
		{	//超时一次到了			
			if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
			{	//所有超时次数已到	
CH0TimerOutDeal:

				DTimeState |= bCH0TimerOutDeal;

			}
			else
			{	//超时次数没有到				
				stLocalControl.stCH0TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00) //所有超时次数已到 
					goto CH0TimerOutDeal;
				else
				{	//所有超时次数没有到,重新加载超时单位时间
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}
			}
		}			
	}
	//通话通道1超时处理
	if(stLocalControl.stCH1TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stLocalControl.stCH1TimerOut.byTimerOut--;
		if(stLocalControl.stCH1TimerOut.byTimerOut == 0)
		{	//超时一次到了
			if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
			{	//所有超时次数已到
CH1TimerOutDeal:
				DTimeState |= bCH1TimerOutDeal;

			}
			else
			{	//超时次数没有到
				stLocalControl.stCH1TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00) goto CH1TimerOutDeal;
				else
				{	//所有超时次数没有到,重新加载超时单位时间
					stLocalControl.stCH1TimerOut.byTimerOut = stLocalControl.stCH1TimerOut.byTimerOutSet;
				}
			}
		}			
	}



	if(stLocalControl.byWaitCommAckDTime)
	{
		if(--stLocalControl.byWaitCommAckDTime ==0)
		{
			if(stLocalControl.uiDevState & FLAG_WAIT_COMM_ACK)
			{//处于等待状态
				DTimeState |= bWaitCommAckDTime;

			}		
		}
	}
	
	if(stLocalControl.byWaitRS485AckDTime)
	{
		if(--stLocalControl.byWaitRS485AckDTime ==0)
		{
			DTimeState |= bWaitRS485AckDTime;
			
			
		}
	}



	if(stLocalControl.byWaitAckTimeLanding)
	{
		if(--stLocalControl.byWaitAckTimeLanding== 0x00)
		{	//上电登记等待应答超时
			DTimeState |= bWaitAckTimeLand;

		}
	}

	
	if(stLocalControl.uiDetectPcTime)
	{	//每10分钟检测PC机一次
		if((--stLocalControl.uiDetectPcTime) == 0x00)
		{
			DTimeState |= bDetectPcTime;

		}
	}


	if(stLocalControl.byUsart1SdResultDTime)
	{
		if(--stLocalControl.byUsart1SdResultDTime==0)
		{
			DTimeState |= bUsart1SdResultDTime;
		}
	}
	return;
}
