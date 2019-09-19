/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SingleBus.c
*文件描述    		:单总线程序
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
#define _IN_SINGLE_BUS_
#include "config.h"




//总线0变量定义  总线0用于与外部设备通信
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
sbit  bBus0RecBit			= byBus0State1^5; 				//单总线0正在接收的数据位
uint8 data byBus0RecCount = 0;								//高4位是总线0接收定时中断计数，低4位是接收到的位计数
uint8 data byBus0RecBuf;									//总线0接收缓冲单元
uint8 data byBus0SendBuf;									//总线0发送缓冲单元	
uint8 xdata byBus0RecData[BUS0_FREQ_SIZE];					//总线0接收缓冲区
uint8 xdata byBus0SendData[BUS0_FREQ_SIZE];					//总线0发送缓冲区
uint8 data byBus0SendCount = 0;								//高4位是总线0发送定时中断计数，低4位是发送的位计数
uint8 data byBus0RecSendCount = 0;							//高4位是总线0接收到的字节计数，低4位是总线0发送完的字节计数
uint8 data byBus0SendStopCount;								//总线0停止时间计数
uint8 data byBus0RecTimeOut = 0;							//总线0接收超时计数
uint8 data byBus0DisableCount = 0;							//总线0禁止计数	 
uint8 xdata byBus0TxQ[BUS0_TX_Q_ZISE];						//总线0发送队列							
uint8 data byBus0TxHead = 0;								//单总线0发送队列头指针
uint8 data byBus0TxTail = 0; 								//单总线0发送队列尾指针
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE)  
sbit  Bus0RecPin	= P3^2;									//单总线0接收引脚定义
sbit  Bus0SendPin	= P1^7;									//单总线0发送引脚定义



//总线1变量定义    总线1用于与内部模块通信
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
uint8 data byBus1RecCount = 0;								//高4位是总线1接收定时中断计数，低4位是接收到的位计数
uint8 data byBus1RecBuf;									//总线1接收缓冲单元
uint8 data byBus1SendBuf;									//总线1发送缓冲单元	
uint8 xdata byBus1RecData[BUS1_FREQ_SIZE];					//总线1接收缓冲区
uint8 xdata byBus1SendData[BUS1_FREQ_SIZE];					//总线1发送缓冲区
uint8 data byBus1SendCount = 0;								//高4位是总线1发送定时中断计数，低4位是发送的位计数
uint8 data byBus1RecSendCount = 0;							//高4位是总线1接收到的字节计数，低4位是总线1发送完的字节计数
uint8 data byBus1SendStopCount;								//总线1停止时间计数
uint8 data byBus1RecTimeOut = 0;							//总线1接收超时计数
uint8 data byBus1DisableCount = 0;							//总线1禁止计数	 
uint8 xdata byBus1TxQ[BUS1_TX_Q_ZISE];						//总线1发送队列							
uint8 data byBus1TxHead = 0;								//单总线1发送队列头指针
uint8 data byBus1TxTail = 0; 								//单总线1发送队列尾指针
#define IncBus1TxPtr(addr)    {addr=(addr+1)%BUS1_TX_Q_ZISE;}
#define Bus1TxBuffLen()	      ((byBus1TxTail+BUS1_TX_Q_ZISE-byBus1TxHead)%BUS1_TX_Q_ZISE)  
sbit  Bus1RecPin	= P3^3;									//单总线1接收引脚定义
sbit  Bus1SendPin	= P3^4;	   								//单总线1发送引脚定义


uint8 xdata byIrDARecBuff[8], byIrDADealBuff[6];		//红外接收区、处理缓冲区

uint8 xdata MoveFjNum;	//移动分机号
//变量定义   
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[]; 
extern STLocalControl 	xdata stLocalControl; 
extern uint8	xdata byUsart0SndDealBuf[];	 

extern unsigned char code ParameterData[];


bit bBus0SndBitOk=0;
//bit bBus1SndBitOk=0;

/**********************************************************
*函数名称			:SingleBusInit	
*函数描述        	:单总线初始化
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
void SingleBusInit(void)
{

	//STC12C56系列::M0=0,M1=0:准双向，M0=0,M1=1:推挽，M0=1,M1=0:高阻，M0=1,M1=1:开漏
	//总线0发送脚设置为推挽输出，接收脚设置为准双向
	//P3^2,准双向,P1^7,推挽
	P3M0 &= (~Bin(0,0,0,0,0,1,0,0));
	P3M1 &= (~Bin(0,0,0,0,0,1,0,0));
	P1M0 &= (~Bin(1,0,0,0,0,0,0,0));
	P1M1 |=   Bin(1,0,0,0,0,0,0,0);

	
	//总线1发送脚设置为开漏输出，接收脚设置为高阻输入
	//P3^3,高阻输入
	P3M0 |= ( Bin(0,0,0,0,1,0,0,0));
	P3M1 &= (~Bin(0,0,0,0,1,0,0,0));


	//P3^4,开漏输出
	P3M0 |=  (Bin(0,0,0,1,0,0,0,0));
	P3M1 |=  (Bin(0,0,0,1,0,0,0,0));


	//总线电平设置
	Bus0SendPin = 0;
	Bus0RecPin = 1;	
	Bus1SendPin = 1;
	Bus1RecPin = 1;		 
	
	
	//定时器0初始化:自动重载方式,定时93.75us   
 	TMOD &= 0xf0;   //  ;t1作波特率发生器(不变)，
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
	IE0 = 0;												//清除外部中断0标志
	IT0 = 1;												//外部中断0为下降沿触发模式	 
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
#pragma disable
void Bus0RecInt(void) interrupt X0_INTNO
{ 	
	DisableBus0RecInt();									//禁止再次下降沿中断
	bBus0StartRec = 1;										//启动起始位沿检测
	bBus0Enable = 0;										//禁止总线发送							
	byBus0RecCount = 0;										//清接收寄存器 	

	if(bBus0ReqSend ==1)
	{//请求发送状态
		byBus0SendStopCount = 240;
	}

}
/**********************************************************
*函数名称			:Bus1RecInt	
*函数描述        	:外部中断1函数,单总线1接收中断
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
#pragma disable
void Bus1RecInt(void) interrupt X1_INTNO
{ 	
	DisableBus1RecInt();									//禁止再次下降沿中断
	bBus1StartRec = 1;										//启动起始位沿检测
	bBus1Enable = 0;										//禁止总线发送							
	byBus1RecCount = 0;										//清接收寄存器	
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
#pragma disable
void Timer0Int(void) interrupt T0_INTNO
{ 

	TR0=0;
	TH0=TIMER0_H;
	TL0=TIMER0_L;
	TR0=1;


//	TXD =!TXD;

	bBus0RecBit = Bus0RecPin; 
	bBus1RecBit = Bus1RecPin; 								//保存总线0、1接收引脚状态	          
	/*******************************总线0接收处理***********************************/
	if(bBus0StartRec)										//判断总线所处的状态，接收到起始位
	{			
		byBus0RecCount += 0x10;								//增加定时中断计数次数
		if(0x50 == (byBus0RecCount & 0xf0))					//到总线起始位检测时间
		{			
			bBus0StartRec = 0;
			byBus0RecCount = 0x00;							//重新开始计数			
			if(bBus0RecBit)			
			{ 	//无效起始位																		
				if((!bBus0SendError)&&bBus0OnSendFreq)
				{	//没有发生过总线发送错误,且有一帧数据正在发送,停止帧数据发送,置发送错误标志						
					byBus0State0 &= (~BUS0_SEND_CON);
					bBus0SendError = 1;
					Bus0SendPin = 0;						//释放总线	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;					//接收出错,重置接收发送计数值
				EnableBus0RecInt();
				MUT = bMUTState; 								//恢复语音功放的控制引脚
			}
			else								
			{	//有效起始位
				bBus0OnRec = 1;								//开始接收数据位 	
			}
		}
	}
	else if(bBus0OnRec)
	{		
		byBus0RecCount += 0x10;								//增加定时中断计数次数	 		
		if(0xa0 == (byBus0RecCount & 0xf0))
		{
			byBus0RecCount &= 0x0f;							//清除定时中断计数次数
			byBus0RecCount += 0x01;
			if(0x0a == (byBus0RecCount & 0x0f))	
			{	//收到第10位,结束位	  				
				bBus0OnRec = 0;								//停止数据接收					
				if(bBus0RecBit)
				{	//有效的结束位							
					if(((bit)(byBus0RecSendCount & 0xf0) == bBus0RecBit9)) 
					{	//数据桢错误
						byBus0RecTimeOut = 0;
						byBus0RecSendCount &= 0x0f;											
					}
					else 
					{	//数据桢正确					
						byBus0RecTimeOut = 230;				//设置下一个字节数据接收超时时间
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
							MUT = bMUTState;					//恢复语音功放的控制引脚
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
						Bus0SendPin = 0;					//释放总线
					}
					byBus0RecSendCount = 0x00;				//接收出错,重置接收发送计数值
					MUT = bMUTState;							//恢复语音功放的控制引脚
				}
			}
			else if(0x09 == (byBus0RecCount & 0x0f))
			{	//第9位数据
				bBus0RecBit9 = bBus0RecBit;					
			}
			else 											
			{ 	//有效数据位
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
			Bus0SendPin = 1;
			bBus0SendBit = 0;								//发送起始位数据 			
			byBus0SendCount = 0;
			byBus0State0 &= (~BUS0_CAN_SEND);
			byBus0SendBuf = byBus0SendData[byBus0RecSendCount & 0x0f];
			bBus0OnSendBit = 1;								//取出待发送的数据并置正在发送标志		
		}
		else
		{	//总线不正常,停止发送
			byBus0State0 &= (~BUS0_SEND_CON);
			bBus0SendError = 1;			
			byBus0RecSendCount &= 0xf0;
			Bus0SendPin = 0;
			byBus0SendStopCount = 240;			
		}
	}
	else if(bBus0OnSendBit)
	{ 	//有数据位正在发送,首先发送的是起始位	
		byBus0SendCount += 0x10;
		if(0x50 == (byBus0SendCount & 0xf0))
		{//处于一个数据位的中间
			if(bBus0SendBit == bBus0RecBit)  bBus0SndBitOk =1;
			else
			{	//不相同,发送失败								
				byBus0State0 &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				bBus0SendError = 1;					
				Bus0SendPin = 0; 
				byBus0SendStopCount = 240;
				bBus0SndBitOk =0;
			}			
		}
		
		//if(bBus0SendBit == bBus0RecBit)   
		else if(bBus0SndBitOk==1)
		{	//发送的数据和接收的数据相同
		//	byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//一位数据发送完毕,首先发送的是起始位
				bBus0SndBitOk=0;
			
				byBus0SendCount &= 0x0f;
				byBus0SendCount += 0x01;				
				if(0x09 == (byBus0SendCount & 0x0f))
				{	//发送到第9位了
					bBus0SendBit = !(bit)(byBus0RecSendCount & 0x0f);
					Bus0SendPin = !bBus0SendBit;						
				}
				else if(0x0a == (byBus0SendCount & 0x0f))
				{	//发送到结束位了
					bBus0SendBit = 1;
					Bus0SendPin = 0;	
				}
				else if(0x0b == (byBus0SendCount & 0x0f))
				{	//已经发送完结束位了
					bBus0OnSendBit = 0;						
					byBus0RecSendCount += 0x01;									
					if((byBus0RecSendCount & 0x0f) >= BUS0_FREQ_SIZE)
					{	//发送完一帧数据												
						byBus0RecSendCount &= 0xf0;			//重新进入数据帧的发送阶段
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
					EnableBus0RecInt();						//再次使能接收中断
				}
				else
				{
					if(byBus0SendBuf & 0x01)
					{	//发送高电平
						bBus0SendBit = 1;
						Bus0SendPin = 0;					
					}
					else
					{	//发送低电平
						bBus0SendBit = 0;
						Bus0SendPin = 1;
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
			Bus0SendPin = 0; 
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
			MUT = bMUTState;									//恢复语音功放的控制引脚
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
				{ 	//有一帧完整的数据在发送队列中 																		
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
} 
/**********************************************************
*函数名称			:BcdToHex	
*函数描述        	:BCD转换成十六进制
*输入参数   		:byData:待转换的BCD码数据
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
uint8 BcdToHex(uint8 byData)
{
	uint8 xdata byRet;

	byRet = byData >> 4;
	byRet *= 10;
	byData &= 0x0f;
	byRet += byData;
	return(byRet);
}

/**********************************************************
*函数名称			:Bus0OutputData	
*函数描述        	:单总线0将待发送数据放入缓冲区
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
uint8 Bus0OutputData(uint8* pbyData)
{
	uint8 byTemp = BUS0_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入发送队列
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	OS_EXIT_CRITICAL();	
	return(TRUE);	
}
/**********************************************************
*函数名称			:Bus1OutputData	
*函数描述        	:单总线1将待发送数据放入缓冲区
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
uint8 Bus1OutputData(uint8* pbyData)
{
	uint8 byTemp = BUS1_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus1TxBuffLen() >= (BUS1_TX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}  
	while(byTemp--)
	{	//数据入发送队列
		byBus1TxQ[byBus1TxTail] = *pbyData++;
		IncBus1TxPtr(byBus1TxTail);
	}
	OS_EXIT_CRITICAL();	
	return(TRUE);	
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
bit AddrCompare(pSTBusFreq pstBusFreq)
{
	if((pstBusFreq->byRecSecAddr != 0xff) && (pstBusFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
	{	
		return(0);
	} 
	if((pstBusFreq->byRecRoomAddr != 0xff) && (pstBusFreq->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr))
	{	
		return(0);
	}
	if((pstBusFreq->byRecBedAddr != 0xff) && (pstBusFreq->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
	{	
		return(0);
	}
	return(1); 

/*	if((pstBusFreq->byRecSecAddr  == 0xff)&&
	   (pstBusFreq->byRecRoomAddr == 0xff)&&
	   (pstBusFreq->byRecBedAddr  == 0xff))
		return(1);
	else if((pstBusFreq->byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)&&
		   (pstBusFreq->byRecRoomAddr== stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
		   (pstBusFreq->byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
		   return(1);
	else return(0);*/	
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
bit DirAddrCompare(pSTBusFreq pstBusFreq)
{
	if(pstBusFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr)
	{	
		return(0);
	} 
	if(pstBusFreq->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr)
	{	
		return(0);
	}
	if(pstBusFreq->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr)
	{	
		return(0);
	}
	return(1); 
}



uint8 DirRFSerialCompare()	//序列号最高位忽略
{
	if(stLocalControl.stEepromCfgData.byRFSerialNum1!= stLocalControl.stBusDealFreq.bySndSecAddr)
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum2!= stLocalControl.stBusDealFreq.bySndRoomAddr)
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum3!= stLocalControl.stBusDealFreq.bySndBedAddr)
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum4!= stLocalControl.stBusDealFreq.byRecSecAddr)
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum5!= stLocalControl.stBusDealFreq.byRecRoomAddr)
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum6!= stLocalControl.stBusDealFreq.byRecBedAddr)
	{	
		return(0);
	}
	
	return(1); 	
}

/**********************************************************
*函数名称			:MakeCH0TimerOut	
*函数描述        	:设置通道0超时参数
*输入参数   		:byTimerOut:超时时间,byTimerOutCount:超时次数
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
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH0TimerOut.byTimerOutSet = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOut = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOutCount = byTimerOutCount;	
}
/**********************************************************
*函数名称			:MakeCH1TimerOut	
*函数描述        	:设置通道1超时参数
*输入参数   		:byTimerOut:超时时间,byTimerOutCount:超时次数
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
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH1Timerout.byTimerOutSet = byTimerOut;
	stLocalControl.stCH1Timerout.byTimerOut = byTimerOut;
	stLocalControl.stCH1Timerout.byTimerOutCount = byTimerOutCount;	
} 


uint8 ParaCrcCheck(uint16 addr)
{
	uint8 crcResult=0;
	uint8 crcData =0;
	uint8 i;
	uint8 paraData;
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		paraData = IapReadByte(addr);
		crcResult +=paraData; 
		addr++;
	}
	crcData = IapReadByte(addr);
	DisableIAP();
	
	if(crcData == crcResult) return 1;
	else return 0;
}


//计算stLocalControl.stEepromCfgData中的数据校验和
uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);

	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}

	return crcResult;
}

/**********************************************************
*函数名称			:SaveIndicationData	
*函数描述        	:保存信息指示数据内容
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
void SaveIndicationData(pSTBusFreq pstBus0SendFreq)
{ 	
	stLocalControl.stIndicationData.stAddr.bySndSecAddr = pstBus0SendFreq->bySndSecAddr;
	stLocalControl.stIndicationData.stAddr.bySndRoomAddr = pstBus0SendFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.stAddr.bySndBedAddr = pstBus0SendFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCallCmd = pstBus0SendFreq->byRecSecAddr; 
//	stLocalControl.stIndicationData.byCallCmd = pstBus0SendFreq->byCmd;

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
*函数名称			:VoiceChannelCtx	
*函数描述        	:语音通道切换处理函数
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
void VoiceChannelCtx(void)	
{ 

	if(bChannel1Talked)
	{	//通道1被动通话
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkedVol;
		CloseCGB();
		OpenCBD();
		XTD = 0;
		XTA = 1;
		bMUTState = MUT = 0;			
	}
	else if(bChannel1Talk)
	{ 	//通道1主动通话
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkVol;
		CloseCGB();
		OpenCBD();
		XTD = 0;
		XTA = 1;
		bMUTState = MUT = 0;	
	} 
	else if((bSickRoomBroad|bOfficeBroad|bAllBroad))
	{	//广播状态
		CCAP1H = stLocalControl.stEepromCfgData.byBroadVol;
		CloseCBD();
		OpenCGB();
		XTA = XTD = 0;
		bMUTState = MUT = 0;		
	}
	else if(bChannel0Talked)
	{	//通道0被叫通话状态
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkedVol;
		CloseCBD();
		OpenCGB();
		XTA = 0;
		XTD = 1;
		bMUTState = MUT = 0;		
	}
	else if(bChannel0Talk)
	{	//通道0主动通话状态
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkVol;
		CloseCBD();
		OpenCGB();
		XTA = 0;
		XTD = 1;
		bMUTState = MUT = 0;			
	}	
	else if(bSelfBroad)
	{	//主动广播状态
		CloseCGB();
		CloseCBD();
		XTA = 0;
		XTD = 1;
		bMUTState = MUT = 1;
	}
	else if((bCalledRing|bWaitListen))
	{	//被呼叫振铃或者等待接听
		CCAP1H = stLocalControl.stEepromCfgData.bySelfRingVol;
		CloseCBD();
		OpenCGB();
		XTA = XTD = 0;
		bMUTState = MUT = 0;		
	}
	else if((bEnSoundNote&bNurseIn&bIndicatingOther))
	{	//正在指示其它分机且护士到位
		CCAP1H = stLocalControl.stEepromCfgData.byRingVol;
		CloseCBD();
		OpenCGB();
		XTA = XTD = 0;
		bMUTState = MUT = 0;		
	}
	else if(bMusicPlaying)
	{	//背景音乐播放状态
		CCAP1H = stLocalControl.stEepromCfgData.byMusicVol;
		CloseCGB();
		OpenCBD();
		XTA = XTD = 0;
		bMUTState = MUT = 0;				
	}
	else if(bVoiceNoting)
	{	//语音提示
		CCAP1H = stLocalControl.stEepromCfgData.byNoteVol;
		CloseCGB();
		OpenCBD();
		XTA = XTD = 0;
		bMUTState = MUT = 0;
	}
	else
	{	//没有任何语音状态存在		
		CCAP1H = 10;
		CloseCGB();
		CloseCBD();
		XTA = XTD =0;
		bMUTState =MUT = 1;		
	} 
}
/**********************************************************
*函数名称			:LedControl	
*函数描述        	:指示灯显示状态控制
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:SetLedDealState,SetLedSetState
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void LedControl(void)
{

}
/**********************************************************
*函数名称			:SysReset	
*函数描述        	:系统复位,该函数仅仅将通道0复位,将通道0
					 恢复到空闲状态
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
void SysReset(void)
{
	//清除对应标志
	if(!(bChannel1Talk|bChannel1Talked))
	{//通道1空闲
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
	} 	
	bBusy = bWaitAck = bWaitListen = bCalledRing = 
	bChannel0Talked = bChannel0Talk = 
	bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 
	bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
	bIndicatingOther=0;		
	MakeCH0TimerOut(0, 0);	
	VoiceChannelCtx();
	
/*	if(!(bChannel1Talk|bChannel1Talked))
	{
		if(bIndicatingOther)
		{ 
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.byRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.byBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
			stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
		else
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}	
	} */	
}

/**********************************************************
*函数名称			:SaveParameter	
*函数描述        	:存储配置参数,存储参数前先擦除整个扇区,
					 然后将所有参数重新写入扇区
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:EnableIAP,IapErase,IapWriteByte,DisableIAP
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SaveParameter(uint16 addr)
{  
	uint8 i;
	uint16 addr_temp=addr;
	uint8 *pbyData;
	uint8 readtimes;
	uint8 writetimes;
	uint8 crc_temp;

	WDT_CONTR = 0x3d;	//喂狗
	writetimes =3;
	do
	{

		EnableIAP(IAP_ERASE);
		IapErase(addr); 

		
		EnableIAP(IAP_WRITE); 
		addr = addr_temp;
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}

		EnableIAP(IAP_READ);
		readtimes=3;
		do
		{
			pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<sizeof(STEepromCfgData);i++)
			{
				if(*pbyData != IapReadByte(addr)) break;
				
				pbyData++;
				addr++;
			}	

			if(i>= sizeof(STEepromCfgData))
			{//写进去的数据与读出来的数据相同

				break;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			break;	//相同
		}
	}while(--writetimes);
	
savaparameterRet:
	DisableIAP();
	//如果是在写参数命令中调用该函数,重新加载使能数据
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
}


/**********************************************************
*函数名称			:ReadParameter	
*函数描述        	:将Flash中的配置数据读入到内存中
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:EnableIAP,IapReadByte,DisableIAP
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void ReadParameter(uint16 addr)
{

	uint8 i=0;
	uint8 *pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	
	EnableIAP(IAP_READ); 
	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		*pbyData = IapReadByte(addr);
		pbyData++;
		addr++;
	}
	DisableIAP();

	//如果是在读参数命令中调用该函数,重新加载使能数据
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	
}


/**********************************************************
*函数名称			:InitParameter	
*函数描述        	:初始化Flash中的配置数据
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:EnableIAP,IapReadByte,DisableIAP
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void InitParameter(void)
{
	uint8 flagData;
	uint8 readtimes=3;
CheckIapSec0:	

	do
	{

		if(ParaCrcCheck(IAP0_ADDR) ==1)	 break;
			

	}while(--readtimes);


	if(readtimes)
	{//第一块校验和正确

		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//数据已经初始化完毕了,读取数据参数

			ReadParameter(IAP0_ADDR);
			return;
		}
		else	goto UseInitValue;	
	}

	else
	{//3次读取都失败或者未初始化，应用初始值，但不保存到内部EEPROM中去
UseInitValue:
		stLocalControl.stEepromCfgData.byInitFlag = ParameterData[0];
		stLocalControl.stEepromCfgData.bySelfSecAddr = ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr = ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr = ParameterData[3];
		stLocalControl.stEepromCfgData.byEnable1 = ParameterData[4];
		stLocalControl.stEepromCfgData.byEnable2 = ParameterData[5];
		stLocalControl.stEepromCfgData.byPrio = ParameterData[6];
		stLocalControl.stEepromCfgData.byRingTime = ParameterData[7];
		stLocalControl.stEepromCfgData.byTalkTime = ParameterData[8];
		stLocalControl.stEepromCfgData.byBroadTime = ParameterData[9];	
		stLocalControl.stEepromCfgData.byListenDelay = ParameterData[10];
		stLocalControl.stEepromCfgData.byVolumStep = ParameterData[11];
		stLocalControl.stEepromCfgData.byRingVol = ParameterData[12];
		stLocalControl.stEepromCfgData.byCH0TalkedVol = ParameterData[13];
		stLocalControl.stEepromCfgData.byCH1TalkedVol = ParameterData[14];
		stLocalControl.stEepromCfgData.byBroadVol = ParameterData[15];
		stLocalControl.stEepromCfgData.byNoteVol = ParameterData[16];
		stLocalControl.stEepromCfgData.byMusicVol = ParameterData[17];
		stLocalControl.stEepromCfgData.byCH0TalkVol = ParameterData[18];
		stLocalControl.stEepromCfgData.byCH1TalkVol = ParameterData[19];
		stLocalControl.stEepromCfgData.bySelfRingVol = ParameterData[20];
		stLocalControl.stEepromCfgData.byMaxVol = ParameterData[21];
		stLocalControl.stEepromCfgData.byMinVol = ParameterData[22];
		stLocalControl.stEepromCfgData.byReserve1 = ParameterData[23];
		stLocalControl.stEepromCfgData.byReserve2 = ParameterData[24];
		stLocalControl.stEepromCfgData.byReserve3 = ParameterData[25];
		stLocalControl.stEepromCfgData.bySerialNum1= ParameterData[26];
		stLocalControl.stEepromCfgData.bySerialNum2= ParameterData[27];
		stLocalControl.stEepromCfgData.bySerialNum3= ParameterData[28];
		stLocalControl.stEepromCfgData.byVersionHi = ParameterData[29];
		stLocalControl.stEepromCfgData.byVersionLo= ParameterData[30];
		stLocalControl.stEepromCfgData.byRFSerialNum1= ParameterData[31];
		stLocalControl.stEepromCfgData.byRFSerialNum2= ParameterData[32];	
		stLocalControl.stEepromCfgData.byRFSerialNum3= ParameterData[33];
		stLocalControl.stEepromCfgData.byRFSerialNum4= ParameterData[34];
		stLocalControl.stEepromCfgData.byRFSerialNum5= ParameterData[35];
		stLocalControl.stEepromCfgData.byRFSerialNum6= ParameterData[36];
		stLocalControl.stEepromCfgData.byBedFjFlag = ParameterData[37];

		byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
		byEnable2 = stLocalControl.stEepromCfgData.byEnable2;

	}

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
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus0RecDeal(void)
{
	//取出收到的数据帧 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	OS_EXIT_CRITICAL();	

//以下仅供测试用
#if DEBUG==1
//   Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
#endif
/////////////////////////////////////////////////////////////////////////////////////



/*	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//如果是登记状态,收到的命令不是登记确认命令,不作处理
		return;
	}
*/
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//确认登记命令				
			if(bLanding)
			{	
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//发送到本机,关闭指示灯显示,取出系统状态	
					bLanding=0;
					byDevState1 = stLocalControl.stBusDealFreq.bySndSecAddr & 0x80;				
					MakeCH0TimerOut(0, 0); 
					if(stLocalControl.stBusDealFreq.bySndSecAddr & 0x40)	//PC机打开
					{
						stLocalControl.stBusDealFreq.byCmd = CMD_PC_OPEN;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}					
				}
			}			
			break;
		case CMD_QUEST:										//查询命令
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_QUEST_ANSWER; 
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;
				stLocalControl.stBusDealFreq.byRecRoomAddr = byDevState1;	//设备状态信息1
				stLocalControl.stBusDealFreq.byRecBedAddr = byDevState2;	//设备状态信息2
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
/*		case CMD_NURSE_IN:							//护士到位命令	 		
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
			{	//如确认的设备是本房的,表明护士到达本房,置到位标志 	
				bNurseIn = 1; 
				VoiceChannelCtx();  							
			}
			break;*/
/*		case CMD_NURSE_OUT:							//护士离开命令
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
			{	//表明护士离开本房,清到位标志
				bNurseIn = 0;					
				LedControl();
				VoiceChannelCtx(); 				
			}
			break;*/					
		case CMD_COMM_CALL:	 								//普通呼叫命令
			if(!bBusy)
			{	//通道0空闲				
				bBusy = 1;
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示
					bIndicatingOther = 0; 					
					LedControl();
					if(bNurseIn)
					{
						VoiceChannelCtx(); 						
					}
				}
				//保存主动呼叫方地址
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				//设置超时5s				
				MakeCH0TimerOut(250, 0);
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{	//呼叫本机设备
					if(!(bChannel1Talk|bChannel1Talked))
					{	//通道1空闲,则本机为空闲状态,返回应答命令	
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_COMM_ANSWER;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					}				
				}
			}
			break;
		case CMD_COMM_ANSWER: 								//普通应答命令			
			bBusy = 1; 
			//设置振铃超时			
			MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byRingTime);
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//应答本机,清等待应答,进入等待接听状态
				bWaitAck = 0;				
				bWaitListen = 1;  										
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//如果通道1忙,或者本机主动通话条件不存在了,缩短超时时间				
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();
				LedControl(); 				
			}
			break;
		case CMD_CALL_LISTEN:  								//接听命令				
			bBusy = 1;
			//设置通话超时
			MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//接听本机,清等待接听,进入通道0主动通话状态 			
				bWaitListen = 0; 			
				bChannel0Talk = 1;								
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//如果通道1忙,或者本机主动通话条件不存在了,缩短超时时间					
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();
				LedControl();
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;			
		case CMD_BROADCAST1:								//收到病区广播命令
		case CMD_BROADCAST2:								//收到办公区广播命令
		case CMD_BROADCAST3: 								//收到全区广播命令
			if(!bBusy)
			{	//通道0空闲				
				bBusy = 1;
				//暂存命令				
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;				
				//设置广播超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime); 
				//保存主动呼叫方地址				
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示	
					bIndicatingOther = 0;				
					LedControl(); 
					if(bNurseIn)
					{ 
						VoiceChannelCtx();					
					}
				}			
				if((bChannel1Talk|bChannel1Talked))
				{	//通道1忙,不作处理	
					break;
				}			
				switch(stLocalControl.stBusDealFreq.byRecSecAddr)
				{	//针对具体命令,看本机是否允许相应的广播
					case CMD_BROADCAST1:
						bSickRoomBroad = bEnSickRoomBroad;
						if(bSickRoomBroad) Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						break;
					case CMD_BROADCAST2:
						bOfficeBroad = bEnOfficeBroad;
						if(bOfficeBroad) Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						break;
					case CMD_BROADCAST3:
						bAllBroad = bEnAllBroad; 
						if(bAllBroad) Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						break;
				}
				VoiceChannelCtx();
				LedControl();
			}
			break;			
		case CMD_INFUSION_ENTER:								//输液呼叫确认命令
		case CMD_SERVICE_ENTER:									//服务呼叫确认命令
		case CMD_HELP_ENTER:									//求援呼叫确认命令
		case CMD_EMERGENCY_ENTER:								//紧急呼叫确认命令			
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//确认本机的呼叫命令
				switch(stLocalControl.stBusDealFreq.byCmd)
				{
					case CMD_INFUSION_ENTER:
						bConfusionNoting = 1;  						
						break;
					case CMD_SERVICE_ENTER:
						bServiceNoting = 1;								
						break;
					case CMD_HELP_ENTER:
						bHelpNoting = 1;  						
						break;
					case CMD_EMERGENCY_ENTER:
						bEmergencyNoting = 1; 						
						break;
				} 			
				if(bIndicatingOther)
				{	//如果正在指示其他分机,清指示其他分机 	 					
					bIndicatingOther = 0;
					if(bNurseIn)
					{						
						VoiceChannelCtx();							
					}			
				}  				
				LedControl();
				//命令还是原来的命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break; 			
		case CMD_INFO_INDICATION:								//收到呼叫指示命令 
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.bySndRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
			{	//如果收到的是本房的呼叫指示,不需要作处理,清除上次的呼叫指示	
				if(bIndicatingOther)
				{	
					bIndicatingOther = 0;
					if(bNurseIn)
					{						 	
						VoiceChannelCtx();
						LedControl();
					}
				}
				if(stLocalControl.stBusDealFreq.bySndBedAddr==stLocalControl.stEepromCfgData.bySelfBedAddr)
				{//是本床头分机,在屏上显示
					//命令还是原来的命令
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					bIndicatingOther =1;

					if(stLocalControl.stEepromCfgData.byBedFjFlag & BedVoicePlay)
					{
						CloseCBD();
						OpenCGB();		//打开SD 通道上的本机语言报号音
						XTA = XTD = 0;
						bMUTState = MUT = 0;
					}

				}
			}
			else
			{	//收到的不是本房的呼叫指示,判断是否本机能够处理
				if(!(bBusy|bConfusionNoting|bServiceNoting|bHelpNoting|bEmergencyNoting))
				{	//本机通道0处于空闲状态,可以指示其他分机的呼叫命令了
					switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
					{
						case CMD_INFUSION_CALL:	   				//输液呼叫
							if(!bEnInfusionDeal)
							{
								return;													
							} 						
							break;
						case CMD_SERVICE_CALL:					//服务呼叫
							if(!bEnServiceDeal)
							{
								return;							
							}						
							break;
						case CMD_HELP_CALL:						//求援呼叫
							if(!bEnHelpDeal)
							{
								return;							
							} 						
							break;
						case CMD_EMERGENCY_CALL: 				//紧急呼叫
							if(!bEnEmergencyDeal)
							{
								return;						
							} 						
							break;
						default:
							return;
					}
//					SaveIndicationData(&(stLocalControl.stBusDealFreq));												
//					bIndicatingOther = 1;
					if((bChannel1Talked|bChannel1Talk))
					{	//如果通道1处于通话状态,不作处理
						return;
					}						
					if(bNurseIn)
					{	//护士在位,指示其他分机
						SaveIndicationData(&(stLocalControl.stBusDealFreq));												
						bIndicatingOther = 1;					
						VoiceChannelCtx();
						LedControl();	
						//命令还是原来的命令
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));						
					}	
				}
			} 

/*//测试时用
			CloseCBD();
			OpenCGB();
			XTA = XTD = 0;
			bMUTState = MUT = 0;	
*/
			break;
		case CMD_INFUSION_ANSWER:							//处理输液呼叫命令
		case CMD_SERVICE_ANSWER:							//处理服务呼叫命令
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令
		case CMD_HELP_ANSWER:								//处理求援呼叫命令
			if(!bBusy)
			{	//通道0不忙				
				bBusy = 1;	
				//保存主动呼叫方地址	
				SaveCallAddr(&(stLocalControl.stBusDealFreq));	
				//设置通道0超时					
				MakeCH0TimerOut(250, 0);
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//处理本机的呼叫命令,清相应的标志 	
					if(stLocalControl.stBusDealFreq.byCmd == CMD_INFUSION_ANSWER)
					{  					
						bConfusionNoting = 0;
					}
					else if(stLocalControl.stBusDealFreq.byCmd == CMD_SERVICE_ANSWER)
					{  						 
						bServiceNoting = 0;
					}
					else if(stLocalControl.stBusDealFreq.byCmd == CMD_EMERGENCY_ANSWER)
					{  					
						bEmergencyNoting = 0;
					}
					else if(stLocalControl.stBusDealFreq.byCmd == CMD_HELP_ANSWER)
					{ 						  
						bHelpNoting = 0;
					} 					
					VoiceChannelCtx(); 				
					LedControl();
					bCalledRing = 1;
					if(!(bChannel1Talk|bChannel1Talked))
					{	//如果通道1不处于通话状态,返回应答命令
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_COMM_ANSWER;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					}
					break;				
				}
				if(bIndicatingOther)
				{	//正在指示其他分机,清除标志	 					
					bIndicatingOther = 0;
					if(bNurseIn)
					{						
						VoiceChannelCtx();						
						LedControl(); 
						//命令还是原来的命令
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

					}			
				} 
			}
			break;
		case CMD_STOP_INDICATION:							//停止指示命令
			if(bIndicatingOther)
			{
				bIndicatingOther = 0;
				if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
					((stLocalControl.stBusDealFreq.bySndRoomAddr) == (stLocalControl.stEepromCfgData.bySelfRoomAddr))&&
					(stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				{//是本机
					CloseCGB();
					CloseCBD();
					XTA = XTD =0;
					bMUTState =MUT = 1;						
				}
				else
				{
					if(bNurseIn)
					{
						VoiceChannelCtx();
						LedControl(); 					
					}
				}
				//命令还是原来的命令
				//Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			//不再向单总线1发送	
			
			}
			break;
		case CMD_INFUSION_CLEAR: 							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR: 							//清除服务呼叫命令
		case CMD_HELP_CLEAR: 								//清除求援呼叫命令
		case CMD_EMERGENCY_CLEAR: 							//清除紧急呼叫命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//清除本机的呼叫,清除相应的标志
				switch(stLocalControl.stBusDealFreq.byCmd-0x0a)	//查询对应的呼叫命令
				{
					case CMD_INFUSION_CALL:
						bConfusionNoting = 0;
						break;
					case CMD_SERVICE_CALL:
						bServiceNoting = 0;
						break;
					case CMD_HELP_CALL:
						bHelpNoting = 0;
						break;
					case CMD_EMERGENCY_CALL:
						bEmergencyNoting = 0;
						break;
				}
				SysReset();
				//VoiceChannelCtx();
				//LedControl();								
			}
			else
			{	//将命令恢复成相应的呼叫命令	
				//stLocalControl.stBusDealFreq.byCmd -= 0x0a;	
				if(bIndicatingOther &&
					(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
					(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
					(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
					((stLocalControl.stBusDealFreq.byCmd-0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
				{ 	//清除本机正在指示的呼叫信息 					
					bIndicatingOther = 0;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));


				
					if(bNurseIn)
					{
						VoiceChannelCtx();
						LedControl(); 				
					}									
				}
			}
			break; 			
		case CMD_SYSTERM_RESET:								//系统复位命令
			SysReset();
			break; 

		case CMD_SYSTERM_RESTART:
			bBusy = bWaitAck = bWaitListen = bCalledRing = 
			bChannel0Talked = bChannel0Talk = 
			bChannel1Talked = bChannel1Talk = 
			bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
			bIndicatingOther=0;		
			MakeCH0TimerOut(0, 0);
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			break;
	
		case CMD_START_VOICE:								//启动播音命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					stLocalControl.stBusDealFreq.byRecSecAddr = CMD_START_VOICE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				bChannel1Talked = bChannel1Talk = 0; 				
				MakeCH1TimerOut(0, 0);				
				bVoiceNoting = bEnVoiceNote;				
				VoiceChannelCtx();
				LedControl();
			}
			break;
		case CMD_MUSIC_PLAY:								//背景音乐播放
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					stLocalControl.stBusDealFreq.byRecSecAddr = CMD_MUSIC_PLAY;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}			
				bChannel1Talked = bChannel1Talk = 0;
				MakeCH1TimerOut(0, 0);  			
				bMusicPlaying = bEnMusicplay;	 				
				VoiceChannelCtx();
				LedControl();					
			}
			break;	
		case CMD_STOP_VOICE:								//停止所有音乐播放
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					stLocalControl.stBusDealFreq.byRecSecAddr = CMD_STOP_VOICE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}		
				bVoiceNoting = bMusicPlaying = 0;
				VoiceChannelCtx();					
			}
			break; 
		case CMD_CHANNEL_CHANGE: 							//通道切换命令
			//将通道0的状态切换到通道1上,同时清除通道0的通话状态
			bBusy = bWaitAck = bWaitListen = bCalledRing =  
				bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;			
			MakeCH0TimerOut(0, 0);					 				
			if((bChannel0Talked|bChannel0Talk))
			{ 				
				bChannel1Talked = bChannel0Talked;
				bChannel1Talk = bChannel0Talk;
				bChannel0Talked = bChannel0Talk = 0; 				
				//设置通道1通话超时						
				MakeCH1TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
				if(bChannel1Talk&&(!bDealKeyDown))
				{	//本机主动通话条件不存在了,缩短超时时间 	 									
					MakeCH1TimerOut(5, 0);
					break; 			
				}
				VoiceChannelCtx();
				LedControl(); 
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}							
			break;
		case CMD_CHANNEL_CLOSE: 							//关闭通道切换命令
			//清除通道1所有状态			
			//bChannel1Talked = bChannel1Talk = bConfusionNoting =0; 
			bChannel1Talked = bChannel1Talk  =0;
			MakeCH1TimerOut(0, 0);	
			VoiceChannelCtx();
			LedControl();
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		case CMD_NUMBER_SET:								//在线编房号，床号命令
/*			if(stLocalControl.stBusDealFreq.byRecSecAddr != stLocalControl.stEepromCfgData.bySerialNum1)
			{
				return;
			}
			if(stLocalControl.stBusDealFreq.byRecRoomAddr != stLocalControl.stEepromCfgData.bySerialNum2)
			{
				return;
			}
			if(stLocalControl.stBusDealFreq.byRecBedAddr != stLocalControl.stEepromCfgData.bySerialNum3)
			{
				return;
			}
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			
			//序列号正确,保存新的地址数据						
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;					

			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stBusDealFreq.byRecRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stBusDealFreq.byRecBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SET_BY_SERIAL;			
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));*/

			if((stLocalControl.stBusDealFreq.bySndRoomAddr >=250)||
			   (stLocalControl.stBusDealFreq.bySndBedAddr ==0x00) )
			{//不是床头分机的地址范围
				break;
			}


			//暂存主呼方地址数据中
			stLocalControl.stCallAddr.bySndSecAddr=stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stCallAddr.bySndRoomAddr=stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stCallAddr.bySndBedAddr =stLocalControl.stBusDealFreq.bySndBedAddr;
			bNumSeting=1;
			stLocalControl.byNumSetTime=250;			//编号5秒内有效
			stLocalControl.stBusDealFreq.byRecSecAddr=0x01;	//开始编号
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//显示开始编号
			break;
			
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			
			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr 	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr 	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//先送显示，手柄等子模块
			
//			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			break;	
			
		case CMD_POWER_ON:	  									//本机重新热启动  	
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//比较地址(含广播地址)
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{//绝对地址
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//原命令
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else
				{//广播地址
					if((bOxSupplyState==1)/*&&(bEnableOxTimer==1)*/)
					{//如果正处于供氧计时状态
					
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					OSWait(K_TMO,200);
					stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
				
			}				
			break;

			
/*		case CMD_OPEN_485BUS_IN:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;	
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_485BUS_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				KDR=0;
			}
			break;*/
			
		/*case CMD_CLOSE_485BUS:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(stLocalControl.stBusDealFreq.bySndSecAddr==CMD_SB_DISPLAY_DATA)
				{//是发送到床头分机手柄上的数据
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
			}			
			//关闭所有
			KDR=1;
			break;*/
		case CMD_OPEN_LCD:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//比较地址(含广播地址)

				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{//绝对地址
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//原命令
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}				
			}
			break;
			
		case CMD_CLOSE_LCD:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//比较地址(含广播地址)

				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{//绝对地址
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//原命令
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}				
			}
			break;
			
		case CMD_CLEAR_LCD:						
		case CMD_CLEAR_HANDLE_LCD:   
			if(stLocalControl.stEepromCfgData.byBedFjFlag & bSecRoom)	//需判断区号、房号
			{
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{//比较地址(含广播地址)			
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					
					if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
					{//绝对地址
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//原命令
						stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					
				}
			}
			else if(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr)	//床号相等
			{
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;			
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//原命令
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
			}
			break;
		case CMD_DATE_SEND:
			bEnableOxTimer=1;	//开始允许供氧统计
			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.stTime.bySec = BcdToHex(stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f);
			stLocalControl.stTime.byMin = BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f);
			stLocalControl.stTime.byHour = BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f);
			stLocalControl.stTime.byDay = BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f);
			stLocalControl.stTime.byMonth = BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f);
			stLocalControl.stTime.byYear = BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);

			if(bWillSndOxSupplyStart==1)
			{//上电(或者没收到时间数据之前)就处于供氧状态
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
				stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_START;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				bWillSndOxSupplyStart=0;
				bOxSupplyState=1;
			}
			break;
			
		case CMD_RF_NUMSET_OK:		//此输液报警器的序列号已被别人占用
			if(DirRFSerialCompare())
			{

				memset(&(stLocalControl.stEepromCfgData.byRFSerialNum1),0x00,6);
				stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
				
				WDT_CONTR = 0x3d; 	//喂狗
				EA=0;
				SaveParameter(IAP0_ADDR);
				EA=1;
				stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
				stLocalControl.stBusDealFreq.bySndRoomAddr=0x00;
				stLocalControl.stBusDealFreq.bySndBedAddr =0x00;
				stLocalControl.stBusDealFreq.byCmd= CMD_TELL_RF_NUM;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
				stLocalControl.stBusDealFreq.byRecRoomAddr=0x00;
				stLocalControl.stBusDealFreq.byRecBedAddr =0x00;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		case CMD_BUS_ANSWER:
	   		if(( stLocalControl.stBusDealFreq.bySndSecAddr ==stLocalControl.stEepromCfgData.bySelfSecAddr)&&
				(stLocalControl.stBusDealFreq.bySndRoomAddr==stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
				(stLocalControl.stBusDealFreq.bySndBedAddr ==stLocalControl.stEepromCfgData.bySelfBedAddr))
	   		{//是本机应答信号
	   			if(stLocalControl.uiWaitAckTimeSupplyOxEnd)
   				{
   					if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_SUPPLY_OX_END)
					{
						stLocalControl.uiWaitAckTimeSupplyOxEnd =0;	//停止计时
						//bWillSndTotalOx =0;
					}
   				}
	   			if(stLocalControl.uiWaitAckTimeSupplyOxTotal)
   				{
   					if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_SUPPLY_OX_TOTAL)
					{
						stLocalControl.uiWaitAckTimeSupplyOxTotal =0;	//停止计时
						//bWillSndTotalOx =0;
					}
   				}
				
	   		}
			break;
		case CMD_PC_OPEN:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(bWillSndTotalOx)
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				}
			}
			break;
		case CMD_PC_CLOSE:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;			
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_BUS0_CHECK:									//外部总线测试命令
			MUS=0;	//SD、SA通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS0_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	
			
/*		case CMD_SD_MS_TEST_START:	//SD通道主到从测试开始
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_MS_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=0;
			OpenCGB();
			MUT =0;	//打开功放34119
			break;

		case CMD_SD_SM_TEST_START:	//SD通道从到主测试开始
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_SM_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=1;
			CloseCGB();
			MUT =1;	//关闭功放34119
			break;
			
		case CMD_SD_TEST_END:	//SD通道测试结束
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TEST_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=0;
			CloseCGB();
			MUT =1;	//关闭功放34119
			break;

		case CMD_SA_MS_TEST_START:	//SA通道主到从测试开始
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_MS_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			OpenCBD();
			MUT =0;	//打开功放34119
			break;

		case CMD_SA_SM_TEST_START:	//SA通道从到主测试开始
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_SM_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 1;
			CloseCBD();
			MUT =1;	//关闭功放34119
			break;
			
		case CMD_SA_TEST_END:	//SA通道测试结束
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TEST_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			CloseCBD();
			MUT =1;	//关闭功放34119
			break;
*/	
		case CMD_SD_TAL_VOL_CHECK:
			MUS=0;	//SD通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=1;
			OpenCGB();
			MUT =0;	//打开功放34119
			break;
		case CMD_SD_TAL_VOL_CHECK_END:
			MUS=0;	//SD通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;

		case CMD_SA_TAL_VOL_CHECK:
			MUS=0;	//SD通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 1;
			OpenCBD();
			MUT =0;	//打开功放34119
			break;

		case CMD_SA_TAL_VOL_CHECK_END:
			MUS=0;	//SD通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			CloseCBD();
			MUT =1;	//关闭功放34119
			break;			
		case CMD_RS485_BRT_SET:
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;	

		case CMD_RS485_INFO_SEC_ROOM:
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;	
			
		case CMD_BED_VOICE_PLAY:
			if(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				stLocalControl.stEepromCfgData.byBedFjFlag |= BedVoicePlay;
			}
			else 
			{
				stLocalControl.stEepromCfgData.byBedFjFlag &= ~BedVoicePlay;
			}
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			break;

		case CMD_DISP_MODE:
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			 			
			break;

		case CMD_COLOR_CLEAR:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_COLOR_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		default:
			break;

	}	
}

/**********************************************************
*函数名称			:Bus0SendDeal	
*函数描述        	:单总线0发送完一帧数据处理函数,该函数首先
					 取出收到的数据,针对每条命令执行对应的控
					 制动作
*输入参数   		:
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
void Bus0SendDeal(void)
{ 
	//取出发送完成的数据帧
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;
	OS_EXIT_CRITICAL();	

	//以下仅供测试用
	//调试用
#if DEBUG==1	
//    Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
#endif
	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时
				MakeCH0TimerOut(150, 0);				
			}			
			break;
		case CMD_INFUSION_CALL:
		case CMD_SERVICE_CALL:
		case CMD_EMERGENCY_CALL:
		case CMD_HELP_CALL:
			//命令还是原命令不变
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_COMM_CALL:	 								//普通呼叫命令 							
		case CMD_INFUSION_ANSWER:							//处理输液呼叫
		case CMD_SERVICE_ANSWER:							//处理服务呼叫
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫
		case CMD_HELP_ANSWER:								//处理求援呼叫
			//停止正在指示的呼叫
			if(bIndicatingOther)
			{	
				bIndicatingOther = 0; 
				if(bNurseIn)
				{ 				
					VoiceChannelCtx();
					LedControl();				
				}			
			}
			if(!bBusy)
			{	//不忙,进入等待应答状态,保存主叫方地址,设置超时
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq)); 				
				MakeCH0TimerOut(250, 0); 						
			}				
			break;	
		case CMD_COMM_ANSWER:  								//普通应答命令
			if(bBusy)
			{

				bCalledRing = 1;  
				if((bChannel1Talk|bChannel1Talked))
				{	//本机已经处于通话状态了,缩短超时,退出						
					MakeCH0TimerOut(5, 0);
					break;
				} 
				
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byRingTime); 				
				//VoiceChannelCtx();
				LedControl();
				if(bEnAutoListen)
				{	//自动接听
					OSWait(K_TMO,50);	//此处延时1000ms,以避免主机会莫名收不到CMD_CALL_LISTEN命令
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_CALL_LISTEN;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					
				}		 					
			}
			break;
		case CMD_CALL_LISTEN:								//接听命令
			if(bBusy)
			{
				bCalledRing = 0;
				bChannel0Talked = 1;							
				if((bChannel1Talk|bChannel1Talked))
				{ 	//本机已经处于通话状态了,缩短超时,退出 						
					MakeCH0TimerOut(5, 0);
					break;
				}
				//设置通话超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);					 						
				VoiceChannelCtx();
				LedControl();
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   										//广播命令
			//停止正在指示的呼叫
			if(bIndicatingOther)
			{	
				bIndicatingOther = 0;
				if(bNurseIn)
				{
					VoiceChannelCtx();
					LedControl(); 					
				}
			}
			if(!bBusy)
			{ 
				bBusy = 1;	
				//保存主动呼叫方地址(本机地址)		  	
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				bSelfBroad = 1;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//本机广播条件不存在了,缩短超时,退出				
					MakeCH0TimerOut(5, 0);
					break;
				}
				//设置广播超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);
				VoiceChannelCtx();
				LedControl();
			}
			break;
		case CMD_INFUSION_CLEAR:							//清除输液呼叫
		case CMD_SERVICE_CLEAR:								//清除服务呼叫
		case CMD_HELP_CLEAR:								//清除求援呼叫
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//清除自己的呼叫,清除相应标志

				if(stLocalControl.stBusDealFreq.byCmd == CMD_INFUSION_CLEAR)
				{				
					bConfusionNoting = 0;
				}
				else if(stLocalControl.stBusDealFreq.byCmd == CMD_SERVICE_CLEAR)
				{
					bServiceNoting = 0;
				}
				else if(stLocalControl.stBusDealFreq.byCmd == CMD_HELP_CLEAR)
				{
					bHelpNoting = 0;
				}
				else
				{
					bEmergencyNoting = 0;
				} 
				SysReset();
				//VoiceChannelCtx();
				//LedControl();					
			}
			else if(bIndicatingOther &&
				(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))//对应的呼叫命令
			{	//清除本机正在指示的呼叫  					
				bIndicatingOther = 0;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				if(bNurseIn)
				{
					VoiceChannelCtx();
					LedControl();					
				}									
			} 			
			break; 	

		case CMD_CHANNEL_CLOSE:								//关闭语音通道命令
			bChannel1Talk = bChannel1Talked = 0; 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			LedControl();
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	
		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 1;								//制造总线故障
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			Bus0SendPin = 0;								//释放总线
			MUT = bMUTState; 				
			break;
		case CMD_SYSTERM_RESET:	 							//系统复位命令
			SysReset();
			break; 
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case CMD_NUMBER_SET:
				//发送占用总线命令
					stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;			
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					break;
				case CMD_POWER_ON:
					if((bOxSupplyState==1)/*&&(bEnableOxTimer==1)*/)
					{//如果正处于供氧计时状态
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					OSWait(K_TMO,200);
					stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					break; 
					
/*				case CMD_OPEN_485BUS_IN:
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					break;					
*/	

				case CMD_SD_TAL_VOL_CHECK_END:
					XTD=0;
					CloseCGB();
					MUT =1; //关闭功放34119
					break;

				case CMD_SD_TAL_VOL_CHECK:
//					LED_DEAL = bLedDealState=1;	//灯亮
					break;

				case CMD_SA_TAL_VOL_CHECK:
					LED_DEAL = bLedDealState=1;	//灯亮
					break;
			}
			break;
		case CMD_SUPPLY_OX_TOTAL:
			bWillSndTotalOx=0;
			break;
			
		default:
			break;			

	}	
}


/**********************************************************
*函数名称			:BusManage	
*函数描述        	:单总线0管理线程
*输入参数   		:
*返回值				:
*全局变量			:byMainCmdQ
*调用模块  			:OSQPost
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void BusManage(void)
{ 	
	while(TRUE)
	{		
		if(bBus0RecFinish)									//总线0收到数据
		{ 	
			OSQPost(byMainCmdQ, BUS0_REC);				
		}
		if(bBus0SendFinish)									//总线0发送完数据帧
		{
			OSQPost(byMainCmdQ, BUS0_SND);			
		}
		if(bBus1RecFinish)									//总线1收到数据
		{ 			
			OSQPost(byMainCmdQ, BUS1_REC);				
		}
		if(bBus1SendFinish)									//总线1发送完数据帧
		{
			OSQPost(byMainCmdQ, BUS1_SND);			
		}		
		OSWait(K_TMO, 1);	
		WDT_CONTR = 0x3d;		//@20MHz,1.25s
	}
}



/**********************************************************
*函数名称			:Bus0Manage	
*函数描述        	:单总线0管理线程
*输入参数   		:
*返回值				:
*全局变量			:byMainCmdQ
*调用模块  			:OSQPost
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
/*void Bus0Manage(void)
{ 	
	while(TRUE)
	{		
		if(bBus0RecFinish)									//总线0收到数据
		{ 	
			OSQPost(byMainCmdQ, BUS0_REC);				
		}
		if(bBus0SendFinish)									//总线0发送完数据帧
		{
			OSQPost(byMainCmdQ, BUS0_SND);			
		} 
		OSWait(K_TMO, 1);			 		
	}
}
 */
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus1RecDeal(void)
{
	//取出单总线1收到的数据帧	
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
	bBus1RecFinish = 0;
	OS_EXIT_CRITICAL();
#if DEBUG==1	
//	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);
#endif

 
//	if(bLanding) return;	//上电状态
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING:  									//登记命令	
			if(bLanding == 0)
			{//已经上电登记成功		
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byBedFjFlag;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break; 	


		case CALL_KEY_DOWN:
			if(bLanding == 0)
			{//已经上电登记成功
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;				
				if(!(bChannel1Talk|bChannel1Talked|bConfusionNoting|bServiceNoting|
					bChannel0Talk|bChannel0Talked|bCalledRing))		
				{	//设备空闲状态
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;
					stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CALL;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
				else if((bConfusionNoting&&bEnHandDown))
				{	//设备正在呼叫中,且允许手柄挂断呼叫		
					stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
				
				else if((bServiceNoting&&bEnHandDown))
				{	//设备正在服务呼叫中,且允许手柄挂断呼叫		
					stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}				
				else if(bChannel1Talked&&bEnHandDown)
				{	//设备正中通道1中被动对讲,且允许手柄挂断						
					stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*					stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
*/
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
				}
				else if(bChannel0Talked&&bEnHandDown)
				{   //设备正中通道0中被动对讲,且允许手柄挂断
					stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
			break;

			
		case CALL_KEY_UP:
		case CALL_KEY_ALWAYS:
			break;
			
		case CMD_DATA_ERROR:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_BUS_ANSWER:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_DISPLAY_DATA /*||stLocalControl.stBusDealFreq.byRecSecAddr == CMD_SB_DISPLAY_DATA*/)
			{//只有床头信息才保存数据
				//bit0=0:该床位有病人
				stLocalControl.stEepromCfgData.byBedFjFlag &= 0xfe;			
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}			
			break;	


		case CMD_INFUSION_CALL:	  	//从输液报警器上无线传过来的信号
			if(bLanding == 0)
			{//已经上电登记成功
				if(!(bChannel1Talk|bChannel1Talked|bConfusionNoting|
					bChannel0Talk|bChannel0Talked|bCalledRing))		
				{	//设备空闲状态
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	//				stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CALL;
					stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CALL;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;
					stLocalControl.stBusDealFreq.byRecRoomAddr =0x00;
					stLocalControl.stBusDealFreq.byRecBedAddr =0x00;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
			break;
	  	case CMD_RF_NUMSET_START:
			if(!(bChannel1Talk|bChannel1Talked|bConfusionNoting|
				bChannel0Talk|bChannel0Talked|bCalledRing|bRFNumSeting))	
			{//设备空闲状态	且没在输液报警器对码状态
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//返回显示模块使其显示
				memcpy(RFSerial,&(stLocalControl.stBusDealFreq.bySndSecAddr),3);
				memcpy(&(RFSerial[3]),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);
				bRFNumSeting=1;
				stLocalControl.uiRFNumSetTime=250;			//编号10秒内有效				
			}
			break;
		case CMD_SUPPLY_OX_END:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.uiWaitAckTimeSupplyOxEnd=250;		//5S内收到供氧结束信号的应答命令
			break;
			
		case CMD_SUPPLY_OX_TOTAL:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.uiWaitAckTimeSupplyOxTotal=250;		//5S内收到供氧总计信号的应答命令
			break;	

		case CMD_BUS1_TEST_ANSWER:	//来自显示模块的确认命令
			bBus1Answer =1;
			break;

		case CMD_BUS1_CHECK:  	   //单总线1检测命令
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS1_CHECK;
			stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break; 

		case CMD_NURSE_IN:
			stLocalControl.uiNurseInTime=NURSE_IN_TIME;				//如果5秒内未收到护士到位命令，即认为护士离开
			if(bNurseIn==0)
			{		
				MoveFjNum = stLocalControl.stBusDealFreq.bySndBedAddr;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			}
			bNurseIn=1;	
			VoiceChannelCtx();
			break;

		case CMD_INSPECTOR_CALL:			//护士查房命令	
			stLocalControl.uiNurseInTime=NURSE_IN_TIME;
			MoveFjNum = stLocalControl.stBusDealFreq.bySndBedAddr;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		default:
			break;			
	} 	

}



void IrDATreat(void)	//数据处理函数
{

	OS_ENTER_CRITICAL();
	memcpy(byIrDADealBuff,byIrDARecBuff,4);				//把数据从接收区复制到缓冲区
	memset(byIrDARecBuff,0x00,4);						//将接收缓冲区清0
	OS_EXIT_CRITICAL();
	if(byIrDADealBuff[3]==(uint8)(byIrDADealBuff[0]+byIrDADealBuff[1]+byIrDADealBuff[2]))
	{//校验和正确
		switch(byIrDADealBuff[2])
		{
		case IrDA_CMD_NURSE_IN:
			bSetKeyDown = 1;								//保存设置键的状态
			stLocalControl.uiNurseInTime=NURSE_IN_TIME;				//如果5秒内未收到护士到位命令，即认为护士离开
			if(bNurseIn==0)
			{
				MoveFjNum = byIrDADealBuff[1];//移动分机号
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
				stLocalControl.stBusDealFreq.bySndBedAddr= byIrDADealBuff[1];//移动分机号
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
				stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			bNurseIn=1;	
			VoiceChannelCtx();
			break;
		case IrDA_CMD_INSPECTION:			//护士查房命令	
			if((stLocalControl.stEepromCfgData.byBedFjFlag & bDispMode)==0)
			{//供氧信息显示模式
              //将查房操作视供氧计时开始与结束

				if(stLocalControl.stEepromCfgData.byBedFjFlag&OX_SUPPLY_STATE) //处于供氧状态
				{//停止供氧计时
					stLocalControl.stEepromCfgData.byBedFjFlag &= (~OX_SUPPLY_STATE);   //标志清0
					stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
					
					SaveParameter(IAP0_ADDR);
					bWillSndOxSupplyStart=0;
						
					
					if(bEnableOxTimer==1)	//已接收到时间数据
					{					
						
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
						bOxSupplyState=0;
//						bWillSndOxSupplyEnd =0;
					}
//					else bWillSndOxSupplyEnd =1;
				}
				
				else//处于未供氧状态
				{//开始计时	
					stLocalControl.stEepromCfgData.byBedFjFlag |= (OX_SUPPLY_STATE);   //标志置1
					stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
					
					WDT_CONTR = 0x3d; 	//喂狗
					SaveParameter(IAP0_ADDR);
//					bWillSndOxSupplyEnd =0;
					
					if(bEnableOxTimer==1)	//已接收到时间数据
					{
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_START;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						bWillSndOxSupplyStart=0;
						bOxSupplyState=1;
			
					}
					else bWillSndOxSupplyStart=1;
				}



//不再执行查房操作
/*				stLocalControl.uiNurseInTime=NURSE_IN_TIME;
				MoveFjNum = byIrDADealBuff[1];//移动分机号
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
				stLocalControl.stBusDealFreq.bySndBedAddr= byIrDADealBuff[1];//移动分机号	
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
				stLocalControl.stBusDealFreq.byCmd = CMD_INSPECTOR_CALL;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));*/
			}
			else 
			{//色块模式

				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_COLOR_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		}
	}
}



void IrDANumberSet(void)	//数据处理函数
{

	OS_ENTER_CRITICAL();
	memcpy(byIrDADealBuff,byIrDARecBuff,6);				//把数据从接收区复制到缓冲区
	memset(byIrDARecBuff,0x00,6);						//将接收缓冲区清0
	OS_EXIT_CRITICAL();

	switch(byIrDADealBuff[4])
	{
		case 0x5a:
			//暂存主呼方地址数据中
			stLocalControl.stBusDealFreq.bySndSecAddr = byIrDADealBuff[1];
			stLocalControl.stBusDealFreq.bySndRoomAddr = byIrDADealBuff[2];
			stLocalControl.stBusDealFreq.bySndBedAddr = byIrDADealBuff[3];
			stLocalControl.stBusDealFreq.byCmd    =   0x5a;
				
			stLocalControl.stCallAddr.bySndSecAddr=stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stCallAddr.bySndRoomAddr=stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stCallAddr.bySndBedAddr =stLocalControl.stBusDealFreq.bySndBedAddr;
			bNumSeting=1;
			stLocalControl.byNumSetTime=250;			//编号5秒内有效
			stLocalControl.stBusDealFreq.byRecSecAddr=0x01;	//开始编号
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//显示开始编号
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus1SendDeal(void)
{ 
	//取出数据帧
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus1SendData, sizeof(STBusFreq)); 
	bBus1SendFinish = 0;
	OS_EXIT_CRITICAL();
	
#if DEBUG==1
//	Send_Data(byBus1SendData,7);
#endif

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
/*		case CMD_ENTER: 
			OS_ENTER_CRITICAL();
			memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.byRFSerialNum1),3);
			stLocalControl.stBusDealFreq.byCmd = CMD_TELL_RF_NUM;
			memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.byRFSerialNum4),3);

			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			OS_EXIT_CRITICAL(); 		
			break;
*/
		case CMD_POWER_ON:
			OS_ENTER_CRITICAL();
			//关闭所有打开的中断
			CCAPM0 = 0x00;
			ET0 = 0;
			TR0 = 0;
			EX0 = 0;
			ISP_CONTR = 0x20; 
			break;
		case CMD_OPEN_LCD:
			//bit1=1:显示屏背光打开
			stLocalControl.stEepromCfgData.byBedFjFlag |= 0x02;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//绝对地址
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			else 
			{//广播地址
				Bus0SendPin = 1;								//制造总线故障
				WDT_CONTR = 0x3d; 	//喂狗
				SaveParameter(IAP0_ADDR);
				Bus0SendPin = 0;								//释放总线						
			}
			break;
			
		case CMD_CLOSE_LCD:
			//bit1=0:显示屏背光关闭
			stLocalControl.stEepromCfgData.byBedFjFlag &= 0xfd;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//绝对地址
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			else 
			{//广播地址
				Bus0SendPin = 1;								//制造总线故障
				WDT_CONTR = 0x3d; 	//喂狗
				SaveParameter(IAP0_ADDR);
				Bus0SendPin = 0;								//释放总线						
			}
			break;

		case CMD_OPEN_485BUS_IN:
			//bit0=0:该床位有病人
//			stLocalControl.stEepromCfgData.byBedFjFlag &= 0xfe;
//			SaveParameter();
			break;
			
		case CMD_CLEAR_LCD:
			//bit0=1:该床位病人出院
			stLocalControl.stEepromCfgData.byBedFjFlag |= 0x01;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

			
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//绝对地址
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			else 
			{//广播地址
				Bus0SendPin = 1;								//制造总线故障
				WDT_CONTR = 0x3d; 	//喂狗
				SaveParameter(IAP0_ADDR);
				Bus0SendPin = 0;								//释放总线						
			}
			break;
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 		//为保存数据作准备		
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			break;
			
		case CMD_DISP_MODE:
			if(stLocalControl.stBusDealFreq.byRecSecAddr)  stLocalControl.stEepromCfgData.byBedFjFlag |= bDispMode;
			else stLocalControl.stEepromCfgData.byBedFjFlag &= ~bDispMode;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			//保存显示模式
			WDT_CONTR = 0x3d; 	//喂狗
			EA=0;
			SaveParameter(IAP0_ADDR);
			EA=1;
			break;	

		case CMD_RS485_INFO_SEC_ROOM:
			if(stLocalControl.stBusDealFreq.byRecSecAddr)  stLocalControl.stEepromCfgData.byBedFjFlag |= bSecRoom;
			else stLocalControl.stEepromCfgData.byBedFjFlag &= ~bSecRoom;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			//保存是否判断区号房号
			WDT_CONTR = 0x3d; 	//喂狗
			EA=0;
			SaveParameter(IAP0_ADDR);
			EA=1;
			break;			

//		case CMD_COLOR_CLEAR:
//			bColorSet=0;
//			break;
			
		default:
			break;


	}
}
/**********************************************************
*函数名称			:Bus1Manage	
*函数描述        	:单总线1管理线程
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
/*void Bus1Manage(void)
{ 	
	while(TRUE)
	{		
		if(bBus1RecFinish)									//总线1收到数据
		{ 			
			OSQPost(byMainCmdQ, BUS1_REC);				
		}
		if(bBus1SendFinish)									//总线1发送完数据帧
		{
			OSQPost(byMainCmdQ, BUS1_SND);			
		} 
		OSWait(K_TMO, 1);			 		
	}
} */



/**********************************************************




/**********************************************************
*函数名称			:TimerOutDeal	
*函数描述        	:超时处理函数
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:Bus0OutputData,Bus1OutputData,VoiceChannelCtx,
					 LedControl,SetHandLedState
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void TimerOutDeal(void)
{ 
	if(stLocalControl.stCH0TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stLocalControl.stCH0TimerOut.byTimerOut--;
		if(stLocalControl.stCH0TimerOut.byTimerOut == 0)
		{	//超时一次到了 
			if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
			{ 	//所有超时完成
				goto TIMER0OUTDEAL;
			}
			else
			{	//超时次数没有完
				stLocalControl.stCH0TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
				{ 	//所有超时完成
TIMER0OUTDEAL:
					if(bLanding)
					{	//上电状态
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_LANDING;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySerialNum1;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySerialNum2;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySerialNum3;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));						
					}
					else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
					{	//等待应答，等待接听，主动通话，主动广播状态				
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}								
					else
					{
						bBusy = bCalledRing = bChannel0Talked = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
						VoiceChannelCtx();
						LedControl();						
					}
				}
				else
				{ 	//超时次数没有完成，重新加载单位超时时间 				
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}
			}			
		}
	}
	//通道1超时处理
	if(stLocalControl.stCH1Timerout.byTimerOut != 0)
	{	//有超时设置存在
		stLocalControl.stCH1Timerout.byTimerOut--;
		if(stLocalControl.stCH1Timerout.byTimerOut == 0)
		{	//超时一次到了 
			if(stLocalControl.stCH1Timerout.byTimerOutCount == 0x00)
			{ 	//所有超时完成
				goto TIMER1OUTDEAL;
			}
			else
			{	//超时次数没有完
				stLocalControl.stCH1Timerout.byTimerOutCount--;
				if(stLocalControl.stCH1Timerout.byTimerOutCount == 0x00)
				{ 	//所有超时完成
TIMER1OUTDEAL:
					if(bChannel1Talk)
					{	//主动通话状态，发送切换命令						
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;						
						stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*						
						stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;	
*/
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					}
					else if(bChannel1Talked)
					{	//被动通话状态，自己复位
						bChannel1Talked = 0;  						
						VoiceChannelCtx();
						LedControl();
					}
				}
				else
				{ 	//超时次数没有完成，重新加载单位超时时间 				
					stLocalControl.stCH1Timerout.byTimerOut = stLocalControl.stCH1Timerout.byTimerOutSet;
				}
			}			
		}
	}	
	if(stLocalControl.uiNurseInTime)
	{
		if(--stLocalControl.uiNurseInTime == 0x00)	
		{	//检测护士到位的定时时间到	定为5S
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
			stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
			stLocalControl.stBusDealFreq.bySndBedAddr= MoveFjNum;//移动分机号
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
			stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_OUT;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			bNurseIn=0;


			if(bIndicatingOther)
			{

				if((stLocalControl.stIndicationData.stAddr.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
					((stLocalControl.stIndicationData.stAddr.bySndRoomAddr) == (stLocalControl.stEepromCfgData.bySelfRoomAddr))&&
					(stLocalControl.stIndicationData.stAddr.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				{//是本机
					
				}
				else
				{//信息指示分机不是本机
					if(bServiceNoting || bConfusionNoting || bHelpNoting ||bEmergencyNoting||bChannel0Talk||bChannel1Talk||bChannel0Talked||bChannel1Talked)
					{//分机呼叫状态
					}

					else
					{
						bIndicatingOther = 0;				

						//VoiceChannelCtx();
						LedControl(); 

						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;	
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;

						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					
				}
	
			}

			VoiceChannelCtx();
		}
	}
	//灯状态控制
	if(--stLocalControl.byLedFlashTime == 0x00)
	{
		stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;
		if((stLocalControl.byLedState & 0x0f) == LED_FLASH)
		{ 	//红灯闪烁状态
			bLedDealState = !bLedDealState;
			LED_DEAL = bLedDealState;
		}	
	}
	if(stLocalControl.byIrDataIntervalTimer)
	{
		if(--stLocalControl.byIrDataIntervalTimer==0x00)	//间隔时间到
		{
			if(IrDataPosit==4)  
			{
				if(byIrDARecBuff[3]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]))
				{
					bFourByteRec=1;

				}
			}

			else if(IrDataPosit==6)
			{
				if(byIrDARecBuff[5]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]+byIrDARecBuff[3]+byIrDARecBuff[4]))
				{

					bSixByteRec =1;
				}

			}
			
			IrDataPosit=0x00;
			
		}
	}


	if(stLocalControl.byNumSetTime)
	{
		if(--stLocalControl.byNumSetTime==0)				//编号时间到
		{
			bNumSeting=0;
			stLocalControl.stBusDealFreq.byCmd =CMD_NUMBER_SET;
			stLocalControl.stBusDealFreq.byRecSecAddr =0x00;	//退出编号
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
	}
	if(stLocalControl.uiRFNumSetTime)
	{
		if(--stLocalControl.uiRFNumSetTime==0)				//编号时间到
		{
			bRFNumSeting=0;
			stLocalControl.stBusDealFreq.byCmd =CMD_RF_NUMSET_END;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
	}
	
/*	if(stLocalControl.uiWaitAckTimeSupplyOxEnd)
	{
		if(--stLocalControl.uiWaitAckTimeSupplyOxEnd==0)
		{//供氧计时结束的等待应答时间到
			bWillSndTotalOx =1;
		}
	}

	if(stLocalControl.uiWaitAckTimeSupplyOxTotal)
	{
		if(--stLocalControl.uiWaitAckTimeSupplyOxTotal==0)
		{//供氧计时总计的等待应答时间到
			bWillSndTotalOx =1;
		}
	}
*/

	stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	if(stLocalControl.uiBus1TestTime)
	{
		if(--stLocalControl.uiBus1TestTime==0)
		{//总线1测试间隔时间到
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS1_TEST;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.uiBus1TestTime = BUS1_TEST_TIME;
			stLocalControl.byBus1AnswerTime = 250;
		}
	}

	if(stLocalControl.byBus1AnswerTime)
	{
		if(--stLocalControl.byBus1AnswerTime==0)
		{//总线1测试应答时间到
			if(bBus1Answer)
			{
				bBus1Answer = 0;
			}
			else
			{
				if((bOxSupplyState==1)/*&&(bEnableOxTimer==1)*/)
				{//如果正处于供氧计时状态
					memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
					stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
					memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				OSWait(K_TMO,200);			
				stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
		}
	}


	
	if(Bus0RecPin ==1)	//总线正常
	{
		BusLowDTime = 100;	//20ms*100=2s		
	}
	else
	{//Bus0RecPin =0
		if(BusLowDTime)
		{
			if(--BusLowDTime ==0)
			{
				MUS =1;	//断开SD到总线
				Bus0SendPin =0;	//使SD为高电平
				OSWait(K_TMO,100);
				WDT_CONTR = 0x3d;
				if(Bus0RecPin ==0)
				{//本机有故障
					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					while(Bus0RecPin==0)	//一直等待为高
					{
						if(bBus1RecFinish)
						{
							memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
							bBus1RecFinish = 0;	
							if(stLocalControl.stBusDealFreq.byCmd==CMD_LANDING)
							{
								stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
								stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
								stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
								stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
								stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byBedFjFlag;
								stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
								stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
								Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));							
							}
							
						}
						Bus0SendPin =0;	//使SD为高电平
						MUS =1;	//断开SD到总线

						OSWait(K_TMO,50);	//延时200ms－－－－快闪
						bLedDealState = !bLedDealState;
						LED_DEAL = bLedDealState;
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
					}					
					MUS = 0;	//使SD线路连通
					Bus0SendPin =0;	//使SD为高电平
					bLedDealState = 0;
					LED_DEAL = 0;

					BusLowDTime = 100;
					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				}

				
				else 
				{//是外部总线引起
					//总线故障
					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x02;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));


				
					MUS = 0;	//使SD线路连通
					Bus0SendPin=0;
					OSWait(K_TMO,100);
					WDT_CONTR = 0x3d;
					
					do
					{
						MUS = 0;	//使SD线路连通
						Bus0SendPin=0;


						OSWait(K_TMO,150);	//延时600ms－－－－慢闪
						bLedDealState = !bLedDealState;
						LED_DEAL = bLedDealState;
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
				
						
						if(bBus1RecFinish)
						{
							memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
							bBus1RecFinish = 0;	
							if(stLocalControl.stBusDealFreq.byCmd==CMD_LANDING)
							{
								stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
								stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
								stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
								stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
								stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byBedFjFlag;
								stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
								stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
								Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));							
							}
							
						}						
					}while(Bus0RecPin==0);	//等待SD线路一直为高
					
					bLedDealState = 0;
					LED_DEAL = 0;

					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x03;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
Bus0BugRet:					
					MUS = 0;	//使SD线路连通					
					BusLowDTime = 100;
				}				
			}
		}
	}	
}
/**********************************************************
*函数名称			:TimerOutManager	
*函数描述        	:超时管理线程
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl,byMainCmdQ
*调用模块  			:OSQPost				
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/	
void TimerOutManager(void) 	 
{  	
	while(1)
	{ 	
		OSWait(K_TMO, 4);	 								//设置20ms超时
		WDT_CONTR = 0x3d;		//@20MHz,1.25s
		if(stLocalControl.stCH0TimerOut.byTimerOut)
		{	
			OSQPost(byMainCmdQ, TIMER_OUT);				
		}
		else
		{		
			OSQPost(byMainCmdQ, TIMER_OUT);	
		}			
	}
} 	  
