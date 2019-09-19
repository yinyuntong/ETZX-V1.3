/*
************************Copyright(c)************************
*	  			   湖南一特股份有限公司
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

bit  fled0					= 0;
bit  bKeyEnterDown			= 0;
bit  bKeyBroadDown			= 0;
bit  bVl0State				= 0;
bit  bNumSeting				= 0;
bit  bMainMenuSet			= 0;
bit  bSendCmdCloseLcd		= 0;
bit  bSendCmdOpenLcd		= 0;

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
sbit  bBus0RecBit			= byBus0State1^5; 				//单总线0正在接收的数据位

uint8 data byBus0RecCount = 0;								//高4位是总线0接收定时中断计数，低4位是接收到的位计数
uint8 data byBus0RecBuf;									//总线0接收缓冲单元
uint8 data byBus0SendBuf;									//总线0发送缓冲单元	
uint8 data byBus0RecData[BUS0_FREQ_SIZE];					//总线0接收缓冲区
uint8 data byBus0SendData[BUS0_FREQ_SIZE];					//总线0发送缓冲区
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
sbit  Bus0SendPin	= P3^6;									//单总线0发送引脚定义
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
uint8 data byBus1RecCount = 0;								//高4位是总线1接收定时中断计数，低4位是接收到的位计数
uint8 data byBus1RecBuf;									//总线1接收缓冲单元
uint8 data byBus1SendBuf;									//总线1发送缓冲单元	
uint8 data byBus1RecData[BUS1_FREQ_SIZE];					//总线1接收缓冲区
uint8 data byBus1SendData[BUS1_FREQ_SIZE];					//总线1发送缓冲区
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
//变量定义  
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[]; 
extern STLocalControl 	xdata stLocalControl; 

extern uint8	xdata byUsart0SndDealBuf[];
extern unsigned char code MyParameter[];


uint8 xdata byBus0RxQ[BUS0_RX_Q_ZISE];						//总线0接收队列
uint8 xdata byBus0RxHead = 0;								//单总线0接收队列头指针
uint8 xdata byBus0RxTail = 0; 								//单总线0接收队列尾指针
uint8 Bus0RecData(uint8* pbyData);



uint8 xdata byBus1RxQ[BUS1_RX_Q_ZISE];						//总线1接收队列
uint8 xdata byBus1RxHead = 0;								//单总线1接收队列头指针
uint8 xdata byBus1RxTail = 0; 								//单总线1接收队列尾指针


bit bBus0SndBitOk=0;


code uint8 VolValIndex[9]={0,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff };




uint8 Bus1RecData(uint8* pbyData);



uint8 LevelToValue(uint8 VoiceLevel);




void led0init(void)
{
	P5M0 |=	Bin(0,0,0,0,1,0,0,0);
	P5M1 &=	(~Bin(0,0,0,0,1,0,0,0));
	fled0=led0=0;
}

void led0flash(void)
{
	if(fled0==0)
	{
		led0=1;
		fled0=1;
	}
	else
	{
 		led0=0;
		fled0=0;
	}
}

	 
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
//     单总线1
	//P3^3,高阻输入
	P3M0 &= (~(Bin(0,0,0,0,1,0,0,0)));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);


	//P3^4,开漏输出	
	P3M0 |=  (Bin(0,0,0,1,0,0,0,0));
	P3M1 |=  (Bin(0,0,0,1,0,0,0,0));



//	   单总线0发送管脚P3.6:强推挽输出
	P3M0 |=    Bin(0,1,0,0,0,0,0,0);
	P3M1 &= (~(Bin(0,1,0,0,0,0,0,0)));


	//总线电平设置
	Bus0SendPin = 0;
	Bus0RecPin = 1;	
	Bus1SendPin = 1;
	Bus1RecPin = 1;	

    TMOD&= 0xf0;	 //  ;t1作波特率发生器(不变)，
    TMOD|=0X01;	//t0作方式1（16位)定时器
	
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	TF0 = 0;												//清除中断标志
	AUXR &= 0x7f;											//传统12分频速度  
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
void Bus0RecInt(void) interrupt 0
{ 	
	DisableBus0RecInt();									//禁止再次下降沿中断
	bBus0StartRec = 1;										//启动起始位沿检测
	bBus0Enable = 0;										//禁止总线发送							
	byBus0RecCount = 0;										//清接收寄存器 		
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
#pragma disable
void Bus1RecInt(void) interrupt 2
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
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#pragma disable
void Timer0Int(void) interrupt 1
{ 

	TR0=0;
	TH0=TIMER0_H;
	TL0=TIMER0_L;
	TR0=1;

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
				PW = bPWState; 								//恢复语音功放的控制引脚
				PW2 = bPW2State;							//恢复语音功放的控制引脚
				VL0 =bVl0State;
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
								if(TRUE==Bus0RecData(byBus0RecData))
								{//保存到队列中	
									//bBus0RecFinish = 1;
								}																							  																						
							} 																		
							byBus0RecTimeOut = 0;
							byBus0DisableCount = 10;
							bBus0Disable = 1;				//禁止总线使用
							PW = bPWState;					//恢复语音功放的控制引脚
							PW2 = bPW2State;				//恢复语音功放的控制引脚
							VL0 =bVl0State;
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
					PW = bPWState;							//恢复语音功放的控制引脚
					PW2 = bPW2State;						//恢复语音功放的控制引脚
					VL0 =bVl0State;
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
								if(TRUE==Bus1RecData(byBus1RecData))
								{//保存到队列中	
									//bBus1RecFinish = 1;
								}																							  																						
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
			if(bBus0SendBit == bBus0RecBit)  bBus0SndBitOk=1;
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
			//byBus0SendCount += 0x10;
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
			PW = bPWState;									//恢复语音功放的控制引脚
			PW2 = bPW2State;								//恢复语音功放的控制引脚
			VL0 = bVl0State;
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
*函数名称			:Bus0RecData	
*函数描述        	:单总线0将单总线上接收的数据放入缓冲区
*输入参数   		:pbyData:待接收的数据指针
*返回值				:TRUE:接收成功,FALSE:队列满,接收失败 	
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
uint8 Bus0RecData(uint8* pbyData)
{
	uint8 byTemp = BUS0_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus0RxBuffLen() >= (BUS0_RX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入发送队列
		byBus0RxQ[byBus0RxTail] = *pbyData++;
		IncBus0RxPtr(byBus0RxTail);
	}
	OS_EXIT_CRITICAL(); 
	return(TRUE);

}


/**********************************************************
*函数名称			:Bus1RecData	
*函数描述        	:单总线1将单总线上接收的数据放入缓冲区
*输入参数   		:pbyData:待接收的数据指针
*返回值				:TRUE:接收成功,FALSE:队列满,接收失败 	
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
uint8 Bus1RecData(uint8* pbyData)
{	
	uint8 byTemp = BUS1_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus1RxBuffLen() >= (BUS1_RX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入发送队列
		byBus1RxQ[byBus1RxTail] = *pbyData++;
		IncBus1RxPtr(byBus1RxTail);
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
	else return(0);
*/
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
	if(bChannel1Talk)
	{	//通道1通话状态			
		if(bHandleDown)
		{	//免提通话
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;
			CloseHandleMIC();
			OpenPanelMIC();		
		}
		else
		{//摘机通话
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HUpTalkVol; 
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;		//只用此音量作为通道1通话音量		
		bPW2State = PW2 = 0;		//打开LM4871	
		CloseCGB();		
		XTD = 0;
		OpenCBD();
		XTA = 1;
		bPWState = PW = 0;				//打开34018
		bVl0State=1;
	}
	else if(bChannel1Talked)
	{	//通道1通话状态		
		if(bHandleDown)
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkedVol;
			CloseHandleMIC();
			OpenPanelMIC();			
		}
		else
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HUpTalkedVol;	
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;		//只用此音量作为通道1通话音量		
		bPW2State = PW2 = 0;		//打开LM4871			
		CloseCGB();
		XTD = 0;
		OpenCBD();	//接收
		XTA = 1;	//发送
		bPWState = PW = 0;				//打开34018
		bVl0State=1;
	}
	else if((bSickRoomBroad|bOfficeBroad|bAllBroad))
	{	//广播状态		

		CloseHandleMIC();
		ClosePanelMIC();
		
		CloseCBD();
		XTA = XTD = 0;
		OpenCGB();
		bPW2State = PW2 = 0;		//打开LM4871	
		bPWState = PW = 0;				//打开34018	
		bVl0State=1;
	}
	else if(bChannel0Talk)
	{	//通道0主动通话状态			
		if(bHandleDown)
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;
			CloseHandleMIC();
			OpenPanelMIC();			
		}
		else
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH0HUpTalkVol;
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;		//只用此音量作为通道0通话音量		
		bPW2State = PW2 = 0;		//打开LM4871		
		CloseCBD();
		XTA = 0;
		OpenCGB();
		XTD = 1;
		bPWState = PW = 0;				//打开34018
		bVl0State=1;
	} 
	else if(bChannel0Talked)
	{	//通道0被叫通话状态			
		if(bHandleDown)
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkedVol;
			CloseHandleMIC();
			OpenPanelMIC();		
		}
		else
		{
//			bPW2State = PW2 = 1;				
//			P2 = stLocalControl.stEepromCfgData.byCH0HUpTalkedVol; 	
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;		//只用此音量作为通道0通话音量		
		bPW2State = PW2 = 0;		//打开LM4871		
		CloseCBD();
		XTA = 0;
		OpenCGB();
		XTD = 1;
		bPWState = PW = 0;				//打开34018
		bVl0State=1;
	}
	else if(bSelfBroad)
	{	//主动广播状态
		if(bHandleDown)
		{
			CloseHandleMIC();
			OpenPanelMIC();
			bPWState = PW = 0;				//打开34018			
		}
		else
		{
			ClosePanelMIC();
			OpenHandleMIC();
			bPWState = PW = 1;				//关闭34018
		}
		CloseCGB();
		CloseCBD();
		bPW2State = PW2 = 1;		//关闭lm7841
		XTA = 0;
		XTD = 1;
		bVl0State=0;
	} 
	else if(bCalledRing)
	{	//被呼叫振铃	//信息指示

		bPW2State = PW2 = 1;		//打开LM4871	
		bPWState = PW = 0;				//打开34018
		P2=stLocalControl.stEepromCfgData.byHDwnSelfRingVol;
		CloseHandleMIC();
		ClosePanelMIC();

		CloseCBD();		
		XTA = XTD = 0; 	
		OpenCGB();
		bVl0State=1;
	}	
	else if(bWaitListen)
	{	//等待接听	
		bPW2State = PW2 = 0;		//打开LM4871
		bPWState = PW = 0;				//打开34018
		P2=stLocalControl.stEepromCfgData.byHDwnSelfRingVol;		
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCBD();			
		XTA = XTD = 0; 
		OpenCGB();	
		bVl0State=1;
	}				
	else if(bIndicatingOther)
	{	//正在指示其它分机  		
		bPW2State = PW2 = 0;		//打开LM4871
		bPWState = PW = 0;				//打开34018

		if( stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin)
		{
			P2=stLocalControl.stEepromCfgData.byHDwnRingVol;
		}
		else
		{
			if( (stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour) ||
				(stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin))
			{//属于Start<End这种情况
				if(stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour)
				{//start.hour<end.hour
					if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour && stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour) ||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin)||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin))
					{//现在时间在此区间内,使用小音量值
					
						P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
					}

					else 
					{//现在时间不在此区间内,使用原始值
						P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;

					}				
				}
				else
				{//start.hour==end.hour
					if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour)
					{//现在时间不在此区间内,使用原始值
						P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
					
					}

					else
					{//stLocalControl.stTime.byHour == start.hour
						if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin)
						{//现在时间在此区间内,使用小音量值
						
							P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
						}

						else 
						{
							//现在时间不在此区间内,使用原始值
							P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
						}	
			
					}
				}
						
			}
			else
			{//属于Start>End这种情况   时间段跨零点 
			
				if(stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour)
				{//start.hour>end.hour
					if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour || stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour) ||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin)||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin))
					{//现在时间在此区间内,使用小音量值
					
						P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
					}

					else 
					{
						//现在时间不在此区间内,使用原始值
						P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
					}				
				}
				else
				{//start.hour==end.hour  start.min>end.min
					if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour)
					{//现在时间在此区间内,使用小音量值
					
						P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
					}
			
					else
					{//stLocalControl.stTime.byHour == start.hour
						if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin)
						{
							//现在时间不在此区间内,使用原始值
							P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
						}					
			
						else 
						{//现在时间在此区间内,使用小音量值
						
							P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
						}	
			
					}
				}
						
			}
		}

					
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCBD();
		XTA = XTD = 0;
		OpenCGB();
		bVl0State=1;
	} 
	else if(bVoiceNoting)
	{	//语音提示			
		bPW2State = PW2 = 0;		//打开LM4871	
		bPWState = PW = 0;				//打开34018		
		P2 = stLocalControl.stEepromCfgData.byCH0HUpTalkedVol;		
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCGB();
		XTA = XTD = 0;
		OpenCBD();
		bVl0State=1;
	}	 	 
	else if(bMusicPlaying)
	{	//背景音乐播放状态 				
		bPW2State = PW2 = 0;		//打开LM4871
		bPWState = PW = 0;				//打开34018		
		P2 = stLocalControl.stEepromCfgData.byHDwnMusicVol;			
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCGB();
		XTA = XTD = 0;
		OpenCBD();	
		bVl0State=1;
	}		
	else
	{
		bPW2State = PW2 = 1;		//关闭LM4871
		bPWState = PW = 1;				//关闭34018
		CloseCBD();
		CloseCGB();
		CloseHandleMIC();
		ClosePanelMIC();
		XTA = XTD=0;
		bVl0State=0;
	} 	
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
	{
		bKeyAnswer = bHandAnswer = 0;
	} 	
	bBusy = bWaitAck = bWaitListen = bCalledRing = bChannel0Talked = bChannel0Talk = 
		bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;		
	MakeCH0TimerOut(0, 0);	
	VoiceChannelCtx();
	if(!(bChannel1Talk|bChannel1Talked))
	{
		if(bIndicatingOther)
		{ 
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
			stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
		else
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}	
	} 	
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
		crcResult +=paraData; /*IapReadByte(addr)*/;
		addr++;
	}
	crcData = IapReadByte(addr);
	DisableIAP();
	
	if(crcData == crcResult) return 1;
	else return 0;
}



uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}
	DisableIAP();
	return crcResult;
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

	stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
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

		//地址紧跟前一数据区
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
			WDT_CONTR = 0x3d;	//喂狗
			pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<(sizeof(STEepromCfgData))*2;)
			{
				if(*pbyData != IapReadByte(addr)) break;
				
				pbyData++;
				addr++;
				i++;
				if(i== sizeof(STEepromCfgData)) pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			}	

			if(i>= (sizeof(STEepromCfgData)*2))
			{//写进去的数据与读出来的数据相同
				break;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			break;	//相同
		}
	}while(--writetimes);

	
	DisableIAP();

	//加载使能数据
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

	//加载使能数据
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;	
}



void BackLightTimerTreat(pSTBusFreq pstBusFreq)
{
	
	pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstBusFreq->byRecSecAddr = 0xff;
	pstBusFreq->byRecRoomAddr= 0xff;
	pstBusFreq->byRecBedAddr = 0xff;
	
	
	if( (stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour<stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour) ||
		(stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin))
	{//属于Start<End这种情况
		if(stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour)
		{//start.hour<end.hour
			if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour && stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour) ||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin)||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin))
			{//现在时间在此区间内,关闭显示屏
				if(!bSendCmdCloseLcd)
				{
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//延时1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					bSendCmdCloseLcd =1;
					bSendCmdOpenLcd  =0;
				}
			}
			else 
			{//现在时间不在此区间内,打开显示屏
				if(!bSendCmdOpenLcd)
				{
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//延时1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =0;
					bSendCmdOpenLcd  =1;							
				}
			}				
		}
		else
		{//start.hour==end.hour
			if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour)
			{//现在时间不在此区间内,打开显示屏
				if(!bSendCmdOpenLcd)
				{
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//延时1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =0;
					bSendCmdOpenLcd  =1;							
				}				
			}
			else
			{//stLocalControl.stTime.byHour == start.hour
				if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
				{//现在时间在此区间内,关闭显示屏
					if(!bSendCmdCloseLcd)
					{
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//延时1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =1;
						bSendCmdOpenLcd  =0;
					}
				}
				else 
				{//现在时间不在此区间内,打开显示屏
					if(!bSendCmdOpenLcd)
					{
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//延时1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =0;
						bSendCmdOpenLcd  =1;							
					}
				}	

			}
		}
				
	}
	else
	{//属于Start>End这种情况   时间段跨零点 

		if(stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour)
		{//start.hour>end.hour
			if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour || stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour) ||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin)||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin))
			{//现在时间在此区间内,关闭显示屏
				if(!bSendCmdCloseLcd)
				{
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//延时1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =1;
					bSendCmdOpenLcd  =0;
				}
			}
			else 
			{//现在时间不在此区间内,打开显示屏
				if(!bSendCmdOpenLcd)
				{
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//延时1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =0;
					bSendCmdOpenLcd  =1;							
				}
			}				
		}
		else
		{//start.hour==end.hour  start.min>end.min
			if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour)
			{//现在时间在此区间内,关闭显示屏
				if(!bSendCmdCloseLcd)
				{
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//延时1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =1;
					bSendCmdOpenLcd  =0;
				}
			}

			else
			{//stLocalControl.stTime.byHour == start.hour
				if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin)
				{//现在时间不在此区间内,打开显示屏
					if(!bSendCmdOpenLcd)
					{
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//延时1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =0;
						bSendCmdOpenLcd  =1;							
					}
				}					

				else 
				{//现在时间在此区间内,关闭显示屏
					if(!bSendCmdCloseLcd)
					{
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//延时1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =1;
						bSendCmdOpenLcd  =0;
					}
				}	

			}
		}
				
	}

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

		if(ParaCrcCheck(IAP0_ADDR) ==1)  break;
			

	}while(--readtimes);


	if(readtimes)
	{//第一块校验和正确

		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == INIT_FLAG)
		{//数据已经初始化完毕了,读取数据参数
			ReadParameter(IAP0_ADDR);
			return;
		}
		else	goto CheckIapSec1;	
	}

	else
	{
CheckIapSec1:	
		readtimes=3;	
		do
		{
			if(ParaCrcCheck(IAP0_ADDR+sizeof(STEepromCfgData)) ==1)  break;
				
		}while(--readtimes);
	
	
		if(readtimes)
		{//第二块校验和正确
			EnableIAP(IAP_READ);
			flagData = IapReadByte(IAP0_ADDR+sizeof(STEepromCfgData));
			DisableIAP();
			
			if(flagData == INIT_FLAG)
			{//数据已经初始化完毕了,读取数据参数
				ReadParameter(IAP0_ADDR+sizeof(STEepromCfgData));
				SaveParameter(IAP0_ADDR);
				return;
			}
			else	goto UseInitValue;	
		}
		else
		{//第一块和第二块存储区3次读取都失败或者未初始化，应用初始值，但不保存到内部EEPROM中去
UseInitValue:
			stLocalControl.stEepromCfgData.byInitFlag = MyParameter[0];
			stLocalControl.stEepromCfgData.bySelfSecAddr = MyParameter[1];
			stLocalControl.stEepromCfgData.bySelfRoomAddr = MyParameter[2];
			stLocalControl.stEepromCfgData.bySelfBedAddr = MyParameter[3];
			stLocalControl.stEepromCfgData.byEnable1 = MyParameter[4];
			stLocalControl.stEepromCfgData.byEnable2 = MyParameter[5];
			stLocalControl.stEepromCfgData.byPrio = MyParameter[6];
			stLocalControl.stEepromCfgData.byRingTime =MyParameter[7];
			stLocalControl.stEepromCfgData.byTalkTime = MyParameter[8];
			stLocalControl.stEepromCfgData.byBroadTime = MyParameter[9];
			stLocalControl.stEepromCfgData.byListenDelay = MyParameter[10];
			stLocalControl.stEepromCfgData.byVolumStep = MyParameter[11];
			stLocalControl.stEepromCfgData.byHDwnRingVol= MyParameter[12];
			stLocalControl.stEepromCfgData.byHUpRingVol = MyParameter[13];
			stLocalControl.stEepromCfgData.byCH0HDwnTalkedVol = MyParameter[14];
			stLocalControl.stEepromCfgData.byCH1HDwnTalkedVol = MyParameter[15];
			stLocalControl.stEepromCfgData.byCH0HUpTalkedVol = MyParameter[16];
			stLocalControl.stEepromCfgData.byCH1HUpTalkedVol = MyParameter[17];
			stLocalControl.stEepromCfgData.byHDwnBroadVol = MyParameter[18];
			stLocalControl.stEepromCfgData.byHUpBroadVol = MyParameter[19];
			stLocalControl.stEepromCfgData.byHDwnNoteVol = MyParameter[20];
			stLocalControl.stEepromCfgData.byHUpNoteVol = MyParameter[21];
			stLocalControl.stEepromCfgData.byHDwnMusicVol =MyParameter[22];
			stLocalControl.stEepromCfgData.byHUpMusicVol =MyParameter[23];
			stLocalControl.stEepromCfgData.byCH0HDwnTalkVol =MyParameter[24];
			stLocalControl.stEepromCfgData.byCH1HDwnTalkVol =MyParameter[25];
			stLocalControl.stEepromCfgData.byCH0HUpTalkVol =MyParameter[26];
			stLocalControl.stEepromCfgData.byCH1HUpTalkVol =MyParameter[27];
			stLocalControl.stEepromCfgData.byHDwnSelfRingVol =MyParameter[28];
			stLocalControl.stEepromCfgData.byHUpSelfRingVol = MyParameter[29];
			stLocalControl.stEepromCfgData.byMaxVol =MyParameter[30];
			stLocalControl.stEepromCfgData.byMinVol = MyParameter[31];
			stLocalControl.stEepromCfgData.byReserve1 =MyParameter[32];
			stLocalControl.stEepromCfgData.byReserve2 =MyParameter[33];
			stLocalControl.stEepromCfgData.byReserve3 =MyParameter[34];
			
			stLocalControl.stEepromCfgData.bySerialNum1= MyParameter[35];
			stLocalControl.stEepromCfgData.bySerialNum2= MyParameter[36];
			stLocalControl.stEepromCfgData.bySerialNum3= MyParameter[37];
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour = MyParameter[38];
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin = MyParameter[39];
			stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour = MyParameter[40];
			stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin = MyParameter[41]; 
			
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour = MyParameter[42];
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin = MyParameter[43];
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour = MyParameter[44];
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin = MyParameter[45];
			
			stLocalControl.stEepromCfgData.byMicroVoiceVal= MyParameter[46];
			stLocalControl.stEepromCfgData.byVersionHi = MyParameter[47];
			stLocalControl.stEepromCfgData.byVersionLo= MyParameter[48];

			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

						//加载使能数据
			byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
			byEnable2 = stLocalControl.stEepromCfgData.byEnable2;	
		}	
	}
	DisableIAP();			
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
/*
	//取出收到的数据帧 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;	
	OS_EXIT_CRITICAL();
*/
	//以下仅供测试用
//	uart_send((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////
	
	
	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//如果是登记状态,收到的命令不是登记确认命令,不作处理
		return;
	}	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//确认登记命令
			if(bLanding)
			{
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//发送到本机,取出系统状态	
					bLanding = 0; 
					stLocalControl.stBusDealFreq.bySndSecAddr &= 0x80;				
					byDevState1 |= stLocalControl.stBusDealFreq.bySndSecAddr; 
					MakeCH0TimerOut(0, 0); 															
				}
			}			
			break;
		case CMD_QUEST:										//查询命令
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd =  CMD_QUEST_ANSWER;  			
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				//stLocalControl.stBusDealFreq.byCmd = CMD_QUEST;			
				//Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
			}
			break;			
		case CMD_COMM_CALL:	 								//普通呼叫命令
			if(!bBusy)
			{ 	//通道0空闲			
				bBusy = 1;	
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示
					bIndicatingOther = 0;
					VoiceChannelCtx();
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
				}			
//				//保存主动呼叫方地址
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				//设置超时5s	
				MakeCH0TimerOut(250, 0);
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{	//呼叫本机设备 
					//保存主动呼叫方地址
					SaveCallAddr(&(stLocalControl.stBusDealFreq));				
					bMainMenuSet =0;					
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
			{ 	//应答本机,清等待应答,进入等待接听状态	
				bWaitAck = 0;					
				bWaitListen = 1;  										
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))
				{	//如果通道1忙,或者本机主动通话条件不存在了,缩短超时时间	
					//(通话条件:免提通话则要求免提键一直处于按下状态,听筒通话
					//则要求听筒一直处于摘机状态)			
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();	 							
			}
			break;
		case CMD_CALL_LISTEN:  								//接听命令			
			bBusy = 1;
			//设置通话超时
			MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);			
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{ 	//接听本机,清等待接听,进入通道0主动通话状态				
				bWaitAck = bWaitListen = 0; 				
				bChannel0Talk = 1;	 											
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))
				{	//如果通道1忙,或者本机主动通话条件不存在了,缩短超时时间	
					//(通话条件:免提通话则要求免提键一直处于按下状态,听筒通话
					//则要求听筒一直处于摘机状态)			
					MakeCH0TimerOut(5, 0);					
					break;
				}							
				VoiceChannelCtx();
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	 					  			
			}
			break;			
		case CMD_BROADCAST1:								//收到病区广播命令
		case CMD_BROADCAST2:								//收到办公区广播命令
		case CMD_BROADCAST3: 								//收到全区广播命令
			if(!bBusy)
			{ 	//通道0空闲				
				bBusy = 1;	
				//暂存命令				
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;				
				//设置广播超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);				
				//保存主动呼叫方地址	
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示
					bIndicatingOther = 0;  								
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					} 
				}							
				if((bChannel1Talk|bChannel1Talked))
				{	//通道1通话中,肯定没有显示呼叫信息了,不做任何处理	
					break;
				}							
				switch(stLocalControl.stBusDealFreq.byRecSecAddr)
				{	//针对具体命令,看本机是否允许相应的广播
					case CMD_BROADCAST1:
						bSickRoomBroad = bEnSickRoomBroad;						
						break;
					case CMD_BROADCAST2:
						bOfficeBroad = bEnOfficeBroad;						
						break;
					case CMD_BROADCAST3:
						bAllBroad = bEnAllBroad; 						
						break;
				}
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stBusDealFreq.byRecSecAddr;	
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				VoiceChannelCtx();				
			}
			break;			
		case CMD_INFO_INDICATION:							//收到呼叫指示命令 									
			if(!bBusy)		
			{	//通道0不忙		
				switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
				{
					case CMD_INFUSION_CALL:	   					//输液呼叫
						if(!bEnInfusionDeal)
						{	//不允许处理输液呼叫
							return;													
						} 						
						break;
					case CMD_SERVICE_CALL:						//服务呼叫
						if(!bEnServiceDeal)
						{	//不允许处理服务呼叫
							return;							
						}						
						break;
					case CMD_HELP_CALL:							//求援呼叫
						if(!bEnHelpDeal)
						{	//不允许处理求援呼叫
							return;							
						} 						
						break;
					case CMD_EMERGENCY_CALL: 					//紧急呼叫
						if(!bEnEmergencyDeal)
						{	//不允许处理紧急呼叫
							return;						
						} 						
						break;
					default:
						return;
				}
				bMainMenuSet = 0;
				//保存指示方地址 				
				SaveIndicationData(&(stLocalControl.stBusDealFreq));
				//置指示标志												
				bIndicatingOther = 1;
				if((bChannel1Talked|bChannel1Talk))
				{	//如果通道1处于通话状态,不作处理
					return;
				}	
				VoiceChannelCtx(); 										
				stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
//				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return; 				
			}
			break;
		case CMD_INFUSION_ANSWER:							//处理输液呼叫命令
		case CMD_SERVICE_ANSWER:							//处理服务呼叫命令
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令
		case CMD_HELP_ANSWER:								//处理求援呼叫命令
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
			if(!bBusy)
			{	//通道0不忙			
				bBusy = 1;	
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示 	
					bIndicatingOther = 0; 					
					VoiceChannelCtx();										
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}							
				} 
				
//				//保存主动呼叫方地址				
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				
				//设置通道0超时				
				MakeCH0TimerOut(250, 0);
				if((bChannel1Talk|bChannel1Talked))
				{	//如果通道1处于通话状态,缩短超时时间,超时后复位			
					MakeCH0TimerOut(5, 0);
					break;
				}				
				VoiceChannelCtx();   				
			}
			break;
		case CMD_STOP_INDICATION:							//停止指示命令						
			if(bIndicatingOther)
			{
				bIndicatingOther = 0;
				VoiceChannelCtx(); 
				if(!(bChannel1Talk|bChannel1Talked))
				{				
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}					
			}
			break;
		case CMD_INFUSION_CLEAR: 							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR: 							//清除服务呼叫命令
		case CMD_HELP_CLEAR: 								//清除求援呼叫命令
		case CMD_EMERGENCY_CLEAR: 							//清除紧急呼叫命令
			if((stLocalControl.stBusDealFreq.bySndRoomAddr == MOVE_FJ)&&
			   (stLocalControl.stBusDealFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//是移动分机发过来的清除呼叫且区号不对 ,直接返回
				return;
			}	
			if((stLocalControl.stBusDealFreq.bySndRoomAddr == ADD_BED_FJ)&&
			   (stLocalControl.stBusDealFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//是加床分机发过来的清除呼叫且区号不对 ,直接返回
				return;
			}			
			while(FALSE==Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
			{
				WDT_CONTR = 0x3d;	//喂狗
				OSWait(K_TMO,100);	//延时500ms
				
			}

			if(bIndicatingOther && 
				(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//清除本机正在指示的呼叫信息 				
				bIndicatingOther = 0;  						
				VoiceChannelCtx();				
				if(!(bChannel1Talk|bChannel1Talked))
				{				
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}							 												
			}
			break; 			
		case CMD_SYSTERM_RESET:								//系统复位命令
			if(bBusy|bChannel1Talked|bChannel1Talk)
			{//系统(通道0)忙或者通道1忙
				SysReset();
			}
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

				if((bChannel1Talk|bChannel1Talked))
				{	//通话中,清通话信息
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}		
				bChannel1Talked = bChannel1Talk = 0; 				
				MakeCH1TimerOut(0, 0);			
				bVoiceNoting = bEnVoiceNote; 				
				VoiceChannelCtx(); 													
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
				if((bChannel1Talk|bChannel1Talked))
				{	//通话中,清通话信息
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}	
				bChannel1Talked = bChannel1Talk = 0;
				MakeCH1TimerOut(0, 0);  				
				bMusicPlaying = bEnMusicplay;					
				VoiceChannelCtx();													
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
			//将通道0的状态切换到通道1上,同时清除通道0的状态
			bBusy = bWaitAck = bWaitListen = bCalledRing =  
				bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;	
			MakeCH0TimerOut(0, 0);				 				
			if((bChannel0Talked||bChannel0Talk))
			{ 				
				bChannel1Talked = bChannel0Talked;
				bChannel1Talk = bChannel0Talk;
				bChannel0Talked = bChannel0Talk = 0; 
				//设置通道1通话超时 							
				MakeCH1TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
				if(bChannel1Talk)
				{	//切换前通道0处于主动通话状态
					if((bKeyAnswer&&(!bKeyEnterDown))||
						(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))					
					{	//本机主动通话条件不存在了,缩短超时时间
						//(通话条件:免提通话则要求免提键一直处于按下状态,听筒通话
						//则要求听筒一直处于摘机状态)	
						MakeCH1TimerOut(5, 0); 
						break;						
					}			  															
				} 	
				VoiceChannelCtx();			
			}					
			break;
		case CMD_CHANNEL_CLOSE: 							//关闭通道切换命令
			if(bChannel1Talked||bChannel1Talk)
			{	//如果通道1处于通话状态,清所有通话标志
				bKeyAnswer = bHandAnswer = 0;
			}
			if((bChannel1Talked|bChannel1Talk))
			{	//如果通道1处于通话状态,结束后需指示新的信息,如没有,保持状态
				if(bIndicatingOther)
				{ 	//如果有指示,立即显示指示信息	
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
					stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}	
			}
			bChannel1Talked = bChannel1Talk = 0;  
			MakeCH1TimerOut(0, 0); 							
			VoiceChannelCtx(); 			
			break;

/*		case CMD_CLOSE_485BUS:  							//关闭485输出
			KDR = 1;   			
			break;
*/			
			
/*		case CMD_OPEN_485BUS_IN:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{   KDR = 0;
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;	
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_485BUS_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
*/			

		case CMD_OPEN_LCD:									//打开LCD背光显示
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				bSendCmdCloseLcd=0;
				bSendCmdOpenLcd =0;
			}
			break;
		case CMD_CLOSE_LCD:									//关闭LCD背光显示
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				bSendCmdCloseLcd=0;
				bSendCmdOpenLcd =0;				
			}
			break;
		case CMD_POWER_ON:	  								//本机重新热启动
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{					
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
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
			break;
		case CMD_CLEAR_LCD:									//清除液晶显示
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//原命令
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;	
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_DATA_SEND:									//校时命令,校正液晶显示时间			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			stLocalControl.stTime.bySec = stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f;
			stLocalControl.stTime.byMin = stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f;
			stLocalControl.stTime.byHour = stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f;
			stLocalControl.stTime.byDay = stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f;
			stLocalControl.stTime.byMonth = stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f;
			stLocalControl.stTime.byYear = stLocalControl.stBusDealFreq.byRecBedAddr;

			
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;			

			if( stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin==stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
			{
				bSendCmdCloseLcd = 0;
				bSendCmdOpenLcd  = 0;
			}
			
			else 
			{
				BackLightTimerTreat(&(stLocalControl.stBusDealFreq));
			}
			break;
			
		case CMD_INFUSION_CALL:								//输液呼叫命令
		case CMD_SERVICE_CALL:								//服务呼叫命令
		case CMD_EMERGENCY_CALL:							//紧急呼叫命令
		case CMD_HELP_CALL:									//求援呼叫命令
			if((stLocalControl.stBusDealFreq.bySndRoomAddr == ADD_BED_FJ)&&
			   (stLocalControl.stBusDealFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//是加床分机发过来的呼叫且区号不对 ,直接返回
				return;
			}

			break;

		case CMD_INFUSION_ENTER:		
		case CMD_SERVICE_ENTER:
		case CMD_EMERGENCY_ENTER:
		case CMD_HELP_ENTER:			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

	case CMD_BL_TIMER_SET:
		stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
		stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

		stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
		stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;
		SaveParameter(IAP0_ADDR);
		
		stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
		stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BL_TIMER_SET;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		//给管理主机回应答
		
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
		stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
		stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
		
		
		if( stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin==stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			OSWait(K_TMO,200);		//延时1S
			
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			bSendCmdCloseLcd = 0;
			bSendCmdOpenLcd  = 0;
		}

		else 
		{
			BackLightTimerTreat(&(stLocalControl.stBusDealFreq));
		}
		break;

	case CMD_VOICE_TIMER_SET:					
		stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
		stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

		stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
		stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;

		stLocalControl.stEepromCfgData.byMicroVoiceVal = stLocalControl.stBusDealFreq.byRecBedAddr;
		SaveParameter(IAP0_ADDR);


		stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
		stLocalControl.stBusDealFreq.byRecSecAddr = CMD_VOICE_TIMER_SET;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		//给管理主机回应答
		
		break;	

	case CMD_SUPPLY_OX_START:
	case CMD_SUPPLY_OX_END:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;		
		
	case CMD_WARD_SEC_INFO_SET:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;	

	case CMD_GSM_NUM_SEARCH:
//	case CMD_GSM_NUM_SET:
//	case CMD_GSM_NUM_DEL:
	case CMD_GSM_FUNC_SEARCH:
//	case CMD_GSM_FUNC_SET:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;	
		
	case CMD_BUS_ANSWER:
		switch(stLocalControl.stBusDealFreq.byRecSecAddr)
		{
			case CMD_GSM_NUM_SET:
			case CMD_GSM_NUM_DEL:
			case CMD_GSM_FUNC_SET:
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				break;				
		}
		break;
		
	case CMD_SYSTEM_SEC_SET:
		stLocalControl.stEepromCfgData.bySelfSecAddr=	stLocalControl.stBusDealFreq.byRecSecAddr;
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

		stLocalControl.stBusDealFreq.byCmd =	CMD_GET_BUS;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;
		
	case CMD_RS485_BRT_SET:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;			
	}	
}



uint8 LevelToValue(uint8 VoiceLevel)
{
	uint8 xdata VoiceValue;
	switch(VoiceLevel)
	{
		case 0:
			VoiceValue = 0x00;
			break;
		case 1:
			VoiceValue = 0x01;
			break;			
		case 2:
			VoiceValue = 0x03;
			break;
		case 3:
			VoiceValue = 0x07;
			break;
		case 4:
			VoiceValue = 0x0f;
			break;
		case 5:
			VoiceValue = 0x1f;
			break;			
		case 6:
			VoiceValue = 0x3f;
			break;
		case 7:
			VoiceValue = 0x7f;
			break;
		case 8:
			VoiceValue = 0xff;
			break;			
	}
	return(VoiceValue);
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
//	uart_send((uint8 *)&(stLocalControl.stBusDealFreq),7);

	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时	
				MakeCH0TimerOut(150, 0); 								
			}			
			break;			
		case CMD_COMM_CALL:	 								//普通呼叫命令 				
			if(!bBusy)
			{	//不忙,进入等待应答状态,保存主叫方地址,设置超时	
				if(bIndicatingOther)
				{	//停止正在指示的呼叫
					bIndicatingOther = 0;
					VoiceChannelCtx();
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}					  			
				}	
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq)); 				
				MakeCH0TimerOut(50, 0);
				if(bHandleDown)
				{	//如果听筒是挂机状态,查看是否由免提键接听
					if(bKeyEnterDown)
					{
						bKeyAnswer = 1;
						bHandAnswer = 0;
					}
					else
					{						
						MakeCH0TimerOut(5, 0);
						break;
					}	
				}
				else
				{	//如果听筒是摘机状态,那么是由听筒接听的,置听筒接听状态	
					bHandAnswer = 1;
					bKeyAnswer = 0;
				}	
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))				
				{	//如果通道1已经处于通话状态或者本机主动通话条件不存在了,
					//缩短超时时间
					//(通话条件:免提通话则要求免提键一直处于按下状态,听筒通话
					//则要求听筒一直处于摘机状态)			
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx(); 						
			}			
			break;								
		case CMD_INFUSION_ANSWER:							//处理输液呼叫
		case CMD_SERVICE_ANSWER:							//处理服务呼叫
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫
		case CMD_HELP_ANSWER:								//处理求援呼叫	
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			bCallDealSending = 0;
			if(!bBusy)
			{	//不忙,进入等待应答状态,保存主叫方地址,设置超时	
				if(bIndicatingOther)
				{	//停止正在指示的呼叫
					bIndicatingOther = 0;
					VoiceChannelCtx(); 							
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}	 			
				}
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				MakeCH0TimerOut(250, 0); 
				if(bHandleDown)
				{	//如果听筒是挂机状态,查看是否由免提键接听
					if(bKeyEnterDown)
					{
						bKeyAnswer = 1;
						bHandAnswer = 0;
					}
					else
					{						
						MakeCH0TimerOut(5, 0);
						break;
					}	
				}
				else
				{	//如果听筒是摘机状态,那么是由听筒接听的,置听筒接听状态	
					bHandAnswer = 1;
					bKeyAnswer = 0;
				}				
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))
				{	//如果通道1已经处于通话状态或者本机主动通话条件不存在了,
					//缩短超时时间
					//(通话条件:免提通话则要求免提键一直处于按下状态,听筒通话
					//则要求听筒一直处于摘机状态)				
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();						
			}			
			break;	
		case CMD_COMM_ANSWER:  								//普通应答命令
			if(bBusy)
			{ 			
				bCalledRing = 1; 				 				
				MakeCH0TimerOut(2, stLocalControl.stEepromCfgData.byListenDelay); 			
				if((bChannel1Talk|bChannel1Talked))
				{	//本机已经处于通话状态了,缩短超时,退出					
					MakeCH0TimerOut(5, 0);
					break;
				} 				
				VoiceChannelCtx();								
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
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);				
				if((bChannel1Talk||bChannel1Talked))
				{	//本机已经处于通话状态了,缩短超时,退出 					
					MakeCH0TimerOut(5, 0);
					break;
				}													
				VoiceChannelCtx();
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);	//满足显示模块显示的要求
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 			
			}
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   								//广播命令
			//暂存命令
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;
			if(!bBusy)
			{ 	
				bBusy = 1;	
				if(bIndicatingOther)
				{	//停止正在指示的呼叫
					bIndicatingOther = 0;
					VoiceChannelCtx();	 			
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}	
				}
				if(bHandleDown)
				{	//如果听筒是挂机状态,查看是否由免提键接听
					if(bKeyBroadDown)
					{
						bKeyAnswer = 1;
						bHandAnswer = 0;
					}
					else
					{						
						MakeCH0TimerOut(5, 0);
						break;
					}	
				}
				else
				{	//如果听筒是摘机状态,那么是由听筒接听的,置听筒接听状态	
					bHandAnswer = 1;
					bKeyAnswer = 0;
				}				
			  	MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);
			  	//保存主动呼叫方地址(本机地址)
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				bSelfBroad = 1;
				if((bChannel1Talk|bChannel1Talked))
				{	//本机广播条件不存在了,缩短超时,退出							
					MakeCH0TimerOut(5, 0);
					break;
				}
				if((CMD_BROADCAST1==stLocalControl.stBusDealFreq.byRecSecAddr) &&
					(((!bKeyBroadDown)&&bKeyAnswer)||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer))))
				{	//病区广播,听筒放下的,病区广播键弹起了
					MakeCH0TimerOut(5, 0);
					break;
				}
				if((CMD_BROADCAST2 == stLocalControl.stBusDealFreq.byRecSecAddr) &&
					(((!bKeyBroadDown)&&bKeyAnswer)||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer))))
				{	//办公区广播,听筒放下的,办公区广播键弹起了
					MakeCH0TimerOut(5, 0);
					break;
				}
				if((CMD_BROADCAST3 == stLocalControl.stBusDealFreq.byRecSecAddr) &&
					(((!bKeyBroadDown)&&bKeyAnswer)||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer))))
				{	//全区广播,听筒放下的,全区广播键弹起了
					MakeCH0TimerOut(5, 0);
					break;
				}
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stBusDealFreq.byRecSecAddr;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				VoiceChannelCtx();					
			}
			break;
		case CMD_INFUSION_CLEAR:							//清除输液呼叫
		case CMD_SERVICE_CLEAR:								//清除服务呼叫
		case CMD_HELP_CLEAR:								//清除求援呼叫
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫
			bCallDealSending = 0; 
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
/*			if(stLocalControl.stBusDealFreq.byCmd== CMD_EMERGENCY_CLEAR)
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				break;
			}
*/
			if(bIndicatingOther && 
				(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//清除本机正在指示的呼叫 			
				bIndicatingOther = 0;					
				VoiceChannelCtx();			
				stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
//				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break; 			
		case CMD_CHANNEL_CLOSE:								//关闭语音通道命令
			if(bChannel1Talked||bChannel1Talk)
			{
				bKeyAnswer = bHandAnswer = 0;
			}
			if((bChannel1Talked|bChannel1Talk))
			{	//如果通道1处于通话状态,结束后需指示新的信息,如没有,保持状态
				if(bIndicatingOther)
				{ 	//如果有指示,立即显示指示信息	
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
					stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}	
			}
			bChannel1Talk = bChannel1Talked = 0; 			 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			break;
		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 1;								//制造总线故障
			SaveParameter(IAP0_ADDR);
			Bus0SendPin = 0;								//释放总线
			PW = bPWState;
			PW2 = bPW2State;
			VL0 = bVl0State;			
			break;	
		case CMD_SYSTERM_RESET:	 							//系统复位命令
			SysReset();
			break; 
		case CMD_DATA_SEND:									//校时命令,校正液晶显示时间			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
			
				case CMD_POWER_ON:
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					break;
			}

			break;	
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
void Bus0Manage(void)
{ 	
	while(TRUE)
	{	
		WDT_CONTR = 0x3d;	//喂狗
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
/**********************************************************
*函数名称			:InitKeyCallAddr	
*函数描述        	:初始化呼叫地址,将地址初始化为自身的地址
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
void InitKeyCallAddr(void)
{
	stLocalControl.stKeyCallAddr.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stKeyCallAddr.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stKeyCallAddr.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
	bRoomAddr = 0;
	bSetPrio = 0;
//	stLocalControl.stKeyCallAddr.byCmd = CMD_STOP_INDICATION;
//	Bus1OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr));
}
/**********************************************************
*函数名称			:AddKeyValue	
*函数描述        	:将按键键值送入缓冲区
*输入参数   		:byKey:键盘按键键值
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
void AddKeyValue(uint8 byKey)
{
	uint8 byTemp;

	for(byTemp = 0; byTemp < (MAX_KEY_SIZE-1); byTemp++)
	{
		stLocalControl.byKeyValue[byTemp] = stLocalControl.byKeyValue[byTemp+1]; 
	}
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = byKey;
}
/**********************************************************
*函数名称			:GetKeyCallAddr	
*函数描述        	:获取按键按下的各类呼叫地址
*输入参数   		:byFlag:获取地址的类型
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
uint8 GetKeyCallAddr(uint8 byFlag)
{
	uint8 byTemp;

	switch(byFlag)
	{	//防止使用者没有按下有效的地址信息,预先初始化本机地址
		case GET_SEC_ADDR:									//获取区地址
			byTemp = stLocalControl.stEepromCfgData.bySelfSecAddr;
			break;
		case GET_ROOM_ADDR:									//获取房地址
			byTemp = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			break;
		case GET_BED_ADDR:									//获取床地址
			byTemp = stLocalControl.stEepromCfgData.bySelfBedAddr;
			break;
		default:
			return(0);
			break;
	}
	if(stLocalControl.byKeyValue[MAX_KEY_SIZE-1] == 0xff)
	{	//没有有效数据		
		return(byTemp);
	}
	byTemp = stLocalControl.byKeyValue[MAX_KEY_SIZE-1];
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
	if(stLocalControl.byKeyValue[MAX_KEY_SIZE-2] == 0xff)
	{		
		return(byTemp);
	}
	byTemp = byTemp + stLocalControl.byKeyValue[MAX_KEY_SIZE-2]*10;
	if(stLocalControl.byKeyValue[MAX_KEY_SIZE-3] == 0xff)
	{		
		return(byTemp);
	}
	byTemp = byTemp + stLocalControl.byKeyValue[MAX_KEY_SIZE-3] * 100;	
	return(byTemp);
}

/**********************************************************/
//获取对应音值的级别
uint8 GetVolLevel(uint8 VloValue)
{
	if((VloValue&0x01)==0) return(0);
	else if(VloValue==0x01) return(1);
	else if(VloValue==0x03) return(2);	
	else if(VloValue==0x07) return(3);
	else if(VloValue==0x0f) return(4);
	else if(VloValue==0x1f) return(5);
	else if(VloValue==0x3f) return(6);	
	else if(VloValue==0x7f) return(7);
	else if(VloValue==0xff) return(8);
	else return(8);	
}


/**********************************************************
*函数名称			:AddVol
*函数描述        	:音量增加
*输入参数   		:pbyData:待增加的音量存储器地址
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
void AddVol(uint8 xdata *pbyData)
{  	
	if(*pbyData < MAX_VOLUMN_VALUE)
	{
		*pbyData = ((*pbyData<<1)|0x01);
		VoiceChannelCtx();
		//存储改变了的音量值
//		stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
	}
}


/**********************************************************
*函数名称			:AddVol
*函数描述        	:音量增加
*输入参数   		:pbyData:待增加的音量存储器地址
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
void AddPlayVoiceVol(uint8 xdata *pbyData)
{  	
	if(*pbyData < stLocalControl.stEepromCfgData.byMaxVol)
	{
		*pbyData = ((*pbyData<<1)|0x01);
		VoiceChannelCtx();
		//存储改变了的音量值
//		stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
	}
}


/**********************************************************
*函数名称			:DecVol
*函数描述        	:音量减小
*输入参数   		:pbyData:待减小的音量存储器地址
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
void DecVol(uint8 xdata *pbyData)
{  	
	if(*pbyData > MIN_VOLUMN_VALUE)
	{
		*pbyData = (*pbyData>>1);
		VoiceChannelCtx();
		//存储改变了的音量值
//		stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));								
	}
}
/**********************************************************
*函数名称			:KeyDownDeal
*函数描述        	:键盘按键按下处理函数
*输入参数   		:byKey:键盘按下的键值
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
void KeyDownDeal(uint8 byKey)
{

	byKey &= 0x1f;	
	if(byKey==KEY_ENTER)  bKeyEnterDown=1;
	else if(byKey==KEY_BROAD) bKeyBroadDown=1;

	if(bNumSeting==1)
	{
		if(byKey==KEY_ENTER)		
		{//编号状态 确认键按下
			bNumSeting=0;
			stLocalControl.stKeyCallAddr.byCmd = CMD_NUMBER_SET;
			stLocalControl.stKeyCallAddr.byRecSecAddr = 0x02;	//成功编号;
			//Bus0OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr)); 							
			Bus1OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr));
			//保存新的地址数据						
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stKeyCallAddr.bySndSecAddr;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stKeyCallAddr.bySndRoomAddr;
			stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stKeyCallAddr.bySndBedAddr;					
			SaveParameter(IAP0_ADDR);
			stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
			return;
		}	
	}
	if(bIndicatingOther&&(!bHandAnswer)&&(!bKeyAnswer))
	{	//正在指示其他分机的呼叫
		switch(byKey)
		{
			case KEY_VOICEINC:									//调整免提或者听筒报号音量
/*				if(bHandleDown)
				{  				
					AddVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));					
				}
				else
				{
					AddVol(&(stLocalControl.stEepromCfgData.byHUpRingVol));	 					
				}*/
				AddPlayVoiceVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byHDwnRingVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_INCREACE_VOL; 	//音量增加命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				return;
			case KEY_VOICEDEC:									//调整免提或者听筒报号音量
/*				if(bHandleDown)
				{
					DecVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));					
				}
				else
				{
					DecVol(&(stLocalControl.stEepromCfgData.byHUpRingVol));					
				}*/
				DecVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byHDwnRingVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_DECREACE_VOL; 	//音量减少命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;
			case KEY_ENTER:									//处理正在指示的呼叫信息	
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;				
				if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				}
				else
				{											
					stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
					stLocalControl.stBusDealFreq.byCmd += 0x06; 
				} 							 				
				if(TRUE == Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
				{
					bCallDealSending = 1;
				}
				InitKeyCallAddr(); 				
				return;			
			default: 										//其他按键按下,挂机
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
				stLocalControl.stBusDealFreq.byCmd += 0x0a; 												 				
				if(TRUE == Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
				{
					bCallDealSending = 1;
				} 

			
				InitKeyCallAddr(); 
				return;				
		}
	}
	if(KEY_CLEAR == byKey)
	{		
		if((bChannel1Talk&bHandAnswer))
		{	//确实是用听筒主动呼叫的,缩短超时时间,超时后自动发送通道切换命令		
			MakeCH1TimerOut(5, 0);
		
		}	
		else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad)&&bHandAnswer)
		{	//确实是用听筒主动呼叫的,缩短超时时间,超时后自动发送复位命令		
			MakeCH0TimerOut(5, 0);		
		}
		InitKeyCallAddr(); 
		return;	
	}
	if(!(bBusy|bChannel1Talked|bChannel1Talk))
	{
		switch(byKey)
		{
			case KEY_0:
				AddKeyValue(0);				
				break;
			case KEY_1:
				AddKeyValue(1);				
				break;
			case KEY_2:
				AddKeyValue(2);			
				break;
			case KEY_3:
				AddKeyValue(3);			
				break;
			case KEY_4:
				AddKeyValue(4);
				break;
			case KEY_5:
				AddKeyValue(5);
				break;
			case KEY_6:
				AddKeyValue(6);
				break;
			case KEY_7:
				AddKeyValue(7);
				break;
			case KEY_8:
				AddKeyValue(8);
				break;
			case KEY_9:
				AddKeyValue(9);
				break;
			case KEY_ROOM:
				stLocalControl.stKeyCallAddr.byRecRoomAddr = GetKeyCallAddr(GET_ROOM_ADDR);				
				bRoomAddr = 1;
				break;
			case KEY_SEC:
				stLocalControl.stKeyCallAddr.byRecSecAddr = GetKeyCallAddr(GET_SEC_ADDR); 								
				break;
			case KEY_MODE:									//功能设置
				AddKeyValue(21);
				//stLocalControl.stKeyCallAddr.byRecBedAddr = GetKeyCallAddr(GET_BED_ADDR);
				//bSetPrio = 1;									//表明设定护理优先级
				break;	
/*			case K_SICK_BROAD:									//病区广播键
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST1;  				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				break;
			case K_OFFICE_BROAD:								//办公区广播键
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST2;  				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				break;*/
			case KEY_BROAD:									//全区广播键
				if(bMainMenuSet==0)
				{
				    switch(stLocalControl.byKeyValue[MAX_KEY_SIZE-1])
			    	{
			    		case 1:
							stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST1;     //  1号类型广播:病区广播
			    			break;
						case 2:
							stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST2;     //  2号类型广播:办公区广播
			    			break;
						
						case 3:
						default:
							stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST3;     //  3号类型广播:全区广播
			    			break;						
			    	}
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0xff; 				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					stLocalControl.byKeyValue[MAX_KEY_SIZE-1]=0xff;
				}
				break;
			case KEY_ENTER:
				if(bMainMenuSet==0)
				{
					if(stLocalControl.byKeyValue[MAX_KEY_SIZE-1] !=0XFF)
					{
					//呼叫功能
						stLocalControl.stKeyCallAddr.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;					
						stLocalControl.stKeyCallAddr.byRecBedAddr = GetKeyCallAddr(GET_BED_ADDR);
						if(stLocalControl.stKeyCallAddr.byRecBedAddr <=200)
						{
							if(!bRoomAddr)
							{	
								//stLocalControl.stKeyCallAddr.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
								stLocalControl.stKeyCallAddr.byRecRoomAddr = 0xff;							
							}	
						}
						else
						{//大于200为医护分机地址
							if(!bRoomAddr)
							{//没有输入房号
								stLocalControl.stKeyCallAddr.byRecRoomAddr = YHFJ_ROOM_ADDR;
								stLocalControl.stKeyCallAddr.byRecBedAddr  -= 200;
							}
						}
						if(!DirAddrCompare(&(stLocalControl.stKeyCallAddr)))
						{//不是本机
							stLocalControl.stKeyCallAddr.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
							stLocalControl.stKeyCallAddr.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
							stLocalControl.stKeyCallAddr.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
							stLocalControl.stKeyCallAddr.byCmd = CMD_COMM_CALL;
							Bus0OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr));
						}	 			
					}
				}
				InitKeyCallAddr(); 			
				break; 			
			default: 			
				break;				
		}
	}
	if(bChannel0Talked||bChannel0Talk)
	{
		switch(byKey)
		{
			case KEY_VOICEINC:									//调整通道0对讲音量
				AddVol(&(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_INCREACE_VOL; 	//音量增加命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;

			case KEY_VOICEDEC:									//调整通道0对讲音量
				DecVol(&(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_DECREACE_VOL; 	//音量减少命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;			
		}
	}
	else if(bChannel1Talked||bChannel1Talk)
	{
		switch(byKey)
		{
			case KEY_VOICEINC:									//调整通道1对讲音量
				AddVol(&(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_INCREACE_VOL; 	//音量增加命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;

			case KEY_VOICEDEC:									//调整通道1对讲音量
				DecVol(&(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_DECREACE_VOL; 	//音量减少命令
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;	

		}
	}
}
/**********************************************************
*函数名称			:KeyUpDeal
*函数描述        	:键盘按键弹起处理函数
*输入参数   		:byKey:键盘按下的键值
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
void KeyUpDeal(uint8 byKey)
{
	byKey &= 0x1f;
	if(byKey==KEY_ENTER)  bKeyEnterDown=0;		//键弹起
	else if(byKey==KEY_BROAD) bKeyBroadDown=0;
	switch(byKey)
	{
		case KEY_ENTER: 			
			if((bChannel1Talk&bKeyAnswer))
			{	//缩短超时时间，超时后自动发送复位命令
				MakeCH1TimerOut(5, 0);

			}			 	  			
			else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad)&&bKeyAnswer)
			{ 				
				MakeCH0TimerOut(5, 0);
			} 			
			break; 
		case KEY_BROAD:									//全区广播键弹起了
			if(bSelfBroad&&bKeyAnswer)
			{	//确实处于广播状态,且听筒是放下的
				MakeCH0TimerOut(5, 0);
			}
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
/*
	//取出单总线1收到的数据帧	
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
	bBus1RecFinish = 0;
	OS_EXIT_CRITICAL();
*/
	//以下仅供测试用
//	uart_send((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////
	
	if(bLanding)
	{	//登记状态不作处理
		return;
	}

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING:  									//登记命令
			//相关音量值
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byHDwnRingVol;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;
			stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							
			break; 		
		case CMD_RECEIVE_OK:
		case CMD_RECEIVE_ERROR:								//转发单总线数据							
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_KEY_DOWN: 									//收到键盘按键按下命令	
			KeyDownDeal(stLocalControl.stBusDealFreq.bySndSecAddr);
			break;
		case CMD_KEY_UP:									//收到键盘按键弹起命令
			KeyUpDeal(stLocalControl.stBusDealFreq.bySndSecAddr);
			break;
		case CMD_DATA_ERROR:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_BUS_ANSWER:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_MAIN_MENU_SET:
			bMainMenuSet=1;
			break;
		case CMD_MAIN_MENU_ESC:
			bMainMenuSet = 0;
			break;
		case CMD_DATA_SEND:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_NUMBER_SET:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	

		case CMD_VOICE_TIMER_SET:					
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;

			stLocalControl.stEepromCfgData.byMicroVoiceVal = stLocalControl.stBusDealFreq.byRecBedAddr;
			SaveParameter(IAP0_ADDR);
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			
			break;

		case CMD_BL_TIMER_SET:
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

			stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;
			
			SaveParameter(IAP0_ADDR);
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
			
			
			if( stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin==stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				OSWait(K_TMO,200);		//延时1S
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				bSendCmdCloseLcd = 0;
				bSendCmdOpenLcd  = 0;
			}

			else
			{
				BackLightTimerTreat(&(stLocalControl.stBusDealFreq));
			}
			break;


		case CMD_VOL_VAL_SET:
			stLocalControl.stEepromCfgData.byMaxVol=VolValIndex[stLocalControl.stBusDealFreq.bySndSecAddr];
			SaveParameter(IAP0_ADDR);
			break;

		case CMD_SELF_SET_NUMBER:
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			SaveParameter(IAP0_ADDR);
			break;

		case CMD_VOICE_TIMER_GET:
			if((stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin>=0x60)||
			   (stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin>=0x60))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = 0;
				stLocalControl.stBusDealFreq.bySndRoomAddr = 0;				
				stLocalControl.stBusDealFreq.byRecSecAddr = 0;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0;

			}
			else 
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin;	
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.byMicroVoiceVal;
			}
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_BL_TIMER_GET:
			if((stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin>=0x60)||
			   (stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin>=0x60))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = 0;
				stLocalControl.stBusDealFreq.bySndRoomAddr = 0;				
				stLocalControl.stBusDealFreq.byRecSecAddr = 0;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0;

			}	
			else
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin;	
			}
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;



		case CMD_VOL_VAL_GET:
			
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.byMaxVol;
			stLocalControl.stBusDealFreq.bySndRoomAddr = 0;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0;
			stLocalControl.stBusDealFreq.byRecRoomAddr = 0;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;

/*		case CMD_485BUS_OK:
			KDR = 1;	//上电485总线成功之后,禁止485接收
			break;*/

		case CMD_WARD_SEC_INFO_SET:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_GSM_FUNC_SEARCH:
		case CMD_GSM_NUM_SEARCH:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;

			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = GSM_MODULE_ADDR;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_GSM_NUM_SET:
		case CMD_GSM_NUM_DEL:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_GSM_FUNC_SET:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
void Bus1SendDeal(void)
{ 
	//取出数据帧
	OS_ENTER_CRITICAL();	
	bBus1SendFinish = 0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus1SendData, sizeof(STBusFreq));	
	OS_EXIT_CRITICAL();	
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING:  									//登记命令
			break;

		case CMD_POWER_ON:
			OS_ENTER_CRITICAL();
			//关闭所有打开的中断
			CCAPM0 = 0x00;
			ET0 = 0;
			TR0 = 0;
			EX0 = 0;
			ISP_CONTR = 0x20; 
			break;
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
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus1Manage(void)
{ 	
	while(TRUE)
	{	
		WDT_CONTR = 0x3d;	//喂狗	
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
}
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
	//通道0超时处理 	
	if(stLocalControl.stCH0TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stLocalControl.stCH0TimerOut.byTimerOut--;
		if(stLocalControl.stCH0TimerOut.byTimerOut == 0)
		{	//超时一次到了 
			if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
			{ 	//所有超时完成
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
/*					stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));*/
				}
				else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
				{				
				 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bCalledRing)
				{	//被呼振铃状态
					bCalledRing = 0;   									
					if(bEnAutoListen)
					{	//自动接听
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
				else
				{
					bBusy = bCalledRing = bChannel0Talked = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
					VoiceChannelCtx(); 										
				}
			}
			else
			{	//超时次数没有完
				stLocalControl.stCH0TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
				{ 	//所有超时完成
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
/*						stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
						stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
*/
					}
					else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
					{				
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					else if(bCalledRing)
					{	//被呼振铃状态
						bCalledRing = 0;   										
						if(bEnAutoListen)
						{	//自动接听
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
					else
					{
						bBusy = bCalledRing = bChannel0Talked = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
						VoiceChannelCtx();						
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
				if(bChannel1Talk)
				{	//主动通话状态，发送切换命令
				
/*					stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
					stLocalControl.stBusDealFreq.bySndRoomAddr = 0x00;
					stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;	
*/	
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*					stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;	
*/
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;	
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
				}
				else if(bChannel1Talked)
				{ 	//被动通话状态，自己复位
					bChannel1Talked = 0; 
					if(bIndicatingOther)
					{ 	//如果有指示,立即显示指示信息	
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
						stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					else
					{
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}				
					VoiceChannelCtx();					
				}	
			}
			else
			{	//超时次数没有完
				stLocalControl.stCH1Timerout.byTimerOutCount--;
				if(stLocalControl.stCH1Timerout.byTimerOutCount == 0x00)
				{ 	//所有超时完成
					if(bChannel1Talk)
					{	//主动通话状态，发送切换命令	
					
/*						stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
						stLocalControl.stBusDealFreq.bySndRoomAddr = 0x00;
						stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;
*/
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*
						stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
						stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
						stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;	
*/
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					}
					else if(bChannel1Talked)
					{	//被动通话状态，自己复位
						bChannel1Talked = 0; 
						if(bIndicatingOther)
						{ 	//如果有指示,立即显示指示信息	
							stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
							stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
							stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
							stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
							stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						}
						else
						{
							stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						}					
						VoiceChannelCtx(); 					
					}
				}
				else
				{ 	//超时次数没有完成，重新加载单位超时时间 				
					stLocalControl.stCH1Timerout.byTimerOut = stLocalControl.stCH1Timerout.byTimerOutSet;
				}
			}			
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
		WDT_CONTR = 0x3d;	//喂狗	
		OSWait(K_TMO, 4);									//延时20ms 	 			
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

