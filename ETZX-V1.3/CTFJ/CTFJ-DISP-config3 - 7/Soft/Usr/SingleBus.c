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

extern unsigned char code ParameterData[];

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
uint8 data byBus0TxQ[BUS0_TX_Q_ZISE];						//总线0发送队列							
uint8 data byBus0TxHead = 0;								//单总线0发送队列头指针
uint8 data byBus0TxTail = 0; 								//单总线0发送队列尾指针
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 


sbit  Bus0RecPin	= P3^3;									//单总线0接收引脚定义
sbit  Bus0SendPin	= P3^4;									//单总线0发送引脚定义



void UsartSendSBNextFrame(void);	
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
	//STC12C54/56系列0=0,M1=0:准双向，M0=0,M1=1:推挽，M0=1,M1=0:高阻，M0=1,M1=1:开漏
	//总线0发送脚设置为推挽输出，接收脚设置为高阻输入:


	//STC12C52/5A系列:M0=0,M1=0:准双向，M0=0,M1=1:高阻，M0=1,M1=0:推挽，M0=1,M1=1:开漏
	//总线0接收脚设置为高阻输入:发送脚设置为推挽输出，

	
	//P3^3,高阻输入
	P3M0 &= (~Bin(0,0,0,0,1,0,0,0));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);

	//,P3^4开漏输出
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);

	//总线电平设置
	Bus0SendPin = 1;
	Bus0RecPin = 1;	 
	
	

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
	IE1 = 0;												//清除中断标志
	IT1 = 1;												//外部中断1为下降沿触发模式	 				
	if(Bus0RecPin)
	{	//如果总线正常,开中断	
		EX1 = 1;
	}
	else
	{	//如果总线不正常,置总线故障标志					
		bBus0Error = 1;
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
void Bus0RecInt(void) interrupt X1_INTNO
{ 	
	DisableBus0RecInt();									//禁止再次下降沿中断
	bBus0StartRec = 1;										//启动起始位沿检测
	bBus0Enable = 0;										//禁止总线发送							
	byBus0RecCount = 0;										//清接收寄存器 		
}
/**********************************************************
*函数名称			:Timer0Int	
*函数描述        	:定时器0溢出中断,定时器每500us中断一次
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
	bBus0RecBit = Bus0RecPin;	
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
					Bus0SendPin = 1;						//释放总线	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;					//接收出错,重置接收发送计数值
				EnableBus0RecInt();				
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
			{ 	//有效数据位
				byBus0RecBuf >>= 1;
				if(bBus0RecBit)
				{	//为高电平
					byBus0RecBuf |= 0x80;
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
			bBus0OnSendBit = 1;								//取出待发送的数据并置正在发送标志		
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
	{ 	//有数据位正在发送,首先发送的是起始位			
		if(bBus0SendBit == bBus0RecBit)   		
		{	//发送的数据和接收的数据相同
			byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//一位数据发送完毕,首先发送的是起始位
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
		else
		{	//不相同,发送失败								
			byBus0State0 &= ~BUS0_SEND_CON;
			byBus0RecSendCount &= 0xf0;
			bBus0SendError = 1;					
			Bus0SendPin = 1; 
			byBus0SendStopCount = 240;
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
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
	addr  = IAP_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}
	//再写每二份拷贝
	pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}
	DisableIAP();
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
	if(ParaCrcCheck(IAP_ADDR) ==1)
	{//第一块校验和正确	
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//数据已经初始化完毕了,读取数据参数
			ReadParameter(IAP_ADDR);
			SaveParameter(IAP_ADDR);
		}
		else	goto InitParaVal;	
	}
	else if(ParaCrcCheck(IAP_ADDR+sizeof(STEepromCfgData)) == 1)
	{
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP_ADDR+sizeof(STEepromCfgData));
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//数据已经初始化完毕了,从第二块读取数据参数
			ReadParameter(IAP_ADDR+sizeof(STEepromCfgData));
			SaveParameter(IAP_ADDR);
		}
		else	goto InitParaVal;			
	}
	else
	{//两块都不正确，调用默认值
InitParaVal:
		stLocalControl.stEepromCfgData.byInitFlag = ParameterData[0];
		stLocalControl.stEepromCfgData.bySelfSecAddr = ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr = ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr = ParameterData[3];
		stLocalControl.stEepromCfgData.bySerialNum1= ParameterData[4];
		stLocalControl.stEepromCfgData.bySerialNum2= ParameterData[5];
		stLocalControl.stEepromCfgData.bySerialNum3= ParameterData[6];
		stLocalControl.stEepromCfgData.byVersionHi = ParameterData[7];
		stLocalControl.stEepromCfgData.byVersionLo= ParameterData[8];
		stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour=0x00;
		stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin=0x00;
		stLocalControl.stEepromCfgData.byRFSerialNum1=ParameterData[12];
		stLocalControl.stEepromCfgData.byRFSerialNum2=ParameterData[13];	
		stLocalControl.stEepromCfgData.byRFSerialNum3=ParameterData[14];
		stLocalControl.stEepromCfgData.byRFSerialNum4=ParameterData[15];
		stLocalControl.stEepromCfgData.byRFSerialNum5=ParameterData[16];
		stLocalControl.stEepromCfgData.byRFSerialNum6=ParameterData[17];
		stLocalControl.stEepromCfgData.uiRs485Brt	 = (ParameterData[18]<<8)+ParameterData[19];
		stLocalControl.stEepromCfgData.byRs485SecRoom= ParameterData[20];
		stLocalControl.stEepromCfgData.byCrc =         ParameterData[21];
		SaveParameter(IAP_ADDR);
	}
}


void UsartSendSBNextFrame(void)
{
	uint8 xdata i=1000;
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;	

	if(stLocalControl.uiSendSBTotalDataLen==0)
	{
		memset(byUsart0SndBuf,0x00,UART0_TX_BUF_SIZE);
		stLocalControl.bySendSBTimes =0;
		stLocalControl.byUsart1SdDelayTime =0;
		return;
	}

	
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	WDT_CONTR = 0x3e;	//喂狗
	switch(pstDataFreq->byData[1])	//信息类型
	{//之前信息类型
		case DIET_TYPE:
			if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
			{//一帧可以发送完成
				Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR+stLocalControl.uiSendedDataLen,stLocalControl.uiSendSBTotalDataLen);
				pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
				pstDataFreq->byData[4] =1;	//结束标志
				stLocalControl.uiSendSBTotalDataLen =0;
			}
			else 
			{
				Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR+stLocalControl.uiSendedDataLen,UART0_TX_BUF_SIZE-EXT_SIZE-5);
				pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
				stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
				pstDataFreq->byData[4] =0;	//非结束
			}
			//pstDataFreq->byData[1] = DIET_TYPE;	
			break;
		case PRESCRIPTION_TYPE:
	
			if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
			{
				Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiSendedDataLen,stLocalControl.uiSendSBTotalDataLen);
				pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
				pstDataFreq->byData[4] =1;	//结束标志
				stLocalControl.uiSendSBTotalDataLen =0;
			}
			else 
			{
				Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiSendedDataLen,UART0_TX_BUF_SIZE-EXT_SIZE-5);
				pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
				stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
				pstDataFreq->byData[4] =0;	//非结束
			}
			//pstDataFreq->byData[1] = PRESCRIPTION_TYPE;	
			break;
		case PROMPT_INFO_TYPE:						
			if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
			{
				Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR+stLocalControl.uiSendedDataLen,stLocalControl.uiSendSBTotalDataLen);
				pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
				pstDataFreq->byData[4] =1;	//结束标志
				stLocalControl.uiSendSBTotalDataLen =0;
			}
			else 
			{
				Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR+stLocalControl.uiSendedDataLen,UART0_TX_BUF_SIZE-EXT_SIZE-5);
				pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
				stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
				pstDataFreq->byData[4] =0;	//非结束
			}
			//pstDataFreq->byData[1] = PROMPT_INFO_TYPE;
			break;
		default:
			return;
	}
	
	stLocalControl.bySendSBTimes =3;
	stLocalControl.byUsart1SdDelayTime =150;
	stLocalControl.uiSendedDataLen += (pstDataFreq->uiLen-5);
	
	pstDataFreq->byStartByte1 = START_BYTE1;
	//源地址忽略
	pstDataFreq->bySndSecAddr = 0x00;
	pstDataFreq->bySndRoomAddr= 0x00;
	pstDataFreq->bySndBedAddr =0x00;
	
	pstDataFreq->byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr= stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	
	pstDataFreq->byCmd= CMD_SB_LCD_DISPLAY_DATA;
	pstDataFreq->byData[0] = 0x00;
	
	pstDataFreq->byData[2] ++;	//帧号加1
	//pstDataFreq->byData[3] = 0x01;	//屏号，未用
	
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	
	stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg.uiSndPos = 0;	//从数据帧头部开始发送数据
	stUsartCfg.pbySndBuf = byUsart0SndBuf;
	
	TI=0;
	SBUF= byUsart0SndBuf[0];	

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
	uint8 xdata i;
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;	
	//取出收到的数据帧 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	OS_EXIT_CRITICAL();	

//以下仅供测试用
//	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);

/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//确认登记命令				
			if(bLanding)
			{					
				bLanding =0;
				MakeCH0TimerOut(0, 0);
				if( (stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.bySndSecAddr)||
					(stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.bySndRoomAddr)||
					(stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.bySndBedAddr))
				{
					stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
					stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
					stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
					
					stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
					SaveParameter(IAP_ADDR);

					page=1;
					ShowItem(BED_NUMBER);
					if(stLocalControl.byDisplayFace == BED_INFO_FACE) Set_VisualPage(1);
				}

				if(stLocalControl.bySlaveState != stLocalControl.stBusDealFreq.byRecSecAddr)
				{
					stLocalControl.bySlaveState = stLocalControl.stBusDealFreq.byRecSecAddr;
					
					if( stLocalControl.bySlaveState	&PATIENT_DISCHARGE)
					{//bit0==1:表示病人出院 
						BL_OFF();//关背光 
					}

					if(stLocalControl.bySlaveState &LCD_ON)
					{//bit1==1:表示显示屏背光打开
						if(!(stLocalControl.bySlaveState&PATIENT_DISCHARGE))
						{//有病人
							BL_ON();
						}
						else   BL_OFF();
	
					}
					else 
					{
						BL_OFF();
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
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;

		case CMD_DATE_SEND:									//收到校时命令
			//stLocalControl.stTime.bySec = BcdToHex(stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f);
			stLocalControl.stTime.byMin = BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f);
			stLocalControl.stTime.byHour = BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f);
			stLocalControl.stTime.byDay = BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f);
			stLocalControl.stTime.byMonth = BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f);
			stLocalControl.stTime.byYear = BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);		
			stLocalControl.byMSecond = 0x00;
			stLocalControl.stTime.bySec=0x00;
			break;
			
		case CMD_CALL_LISTEN:								//接听命令
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{	//这条命令是本机发送的
				page=0;
				ShowTalkFace(stLocalControl.stBusDealFreq.byRecRoomAddr, stLocalControl.stBusDealFreq.byRecBedAddr);
			}
//			else
//			{	//这条命令是其他分机发送的
//				ShowTalkFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr,0);
//			}
			break;

		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{	//这条命令是本机发送的
				page=0;
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 1);
			}
			else
			{	//这条命令是其他分机发送的
				page=0;
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 0);
			}
			break;
		case CMD_INFO_INDICATION:							//屏显示呼叫信息
		
			if(stLocalControl.stBusDealFreq.bySndRoomAddr == 250 )
			{
				page=0;
				ShowAddBedCallFace(stLocalControl.stBusDealFreq.bySndBedAddr);
			}
			else 
			{
				page=0;
				ShowCallFace(stLocalControl.stBusDealFreq.bySndRoomAddr&0x7f, stLocalControl.stBusDealFreq.bySndBedAddr, 
						 stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f);	
			}
			break;
		case CMD_STOP_INDICATION:	 						//屏关闭呼叫信息显示 
		case CMD_SYSTERM_RESET:
			Set_VisualPage(1);
			stLocalControl.byDisplayFace=BED_INFO_FACE;			
			break;

		case CMD_CLEAR_LCD:									//清除液晶命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
				stLocalControl.uiBedFaceDataLen=0;
				Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&(stLocalControl.uiBedFaceDataLen),2);
				Write_Cont(BED_FACE_DATA_BAK,&(stLocalControl.uiBedFaceDataLen),2);
			
				memset(&(stLocalControl.stSupplyOxStartTime.byYear),0x00,6);
				memset(&(stLocalControl.stSupplyOxEndTime.byYear),0x00,6);
				memset(&(stLocalControl.stSupplyOxSubtotalTime.uiHour),0x00,4);
				memset(&(stLocalControl.stSupplyOxTotalTime.uiHour),0x00,4);
				
				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour=0x00;
				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin=0x00;	
				page=1;
			
				ShowBedFace();

				if(stLocalControl.byDisplayFace==BED_INFO_FACE)
						Set_VisualPage(1);
				
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				//SaveParameter();

				stLocalControl.bySlaveState |= PATIENT_DISCHARGE; //病人出院
				BL_OFF();

			}
			break;
			
		case CMD_NUMBER_SET:								//修改本机房号，床号
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x01)
			{	
				page=0;
				ShowStartNumSet();
				stLocalControl.uiLcdDisplayTime = 500;	//如果黑时亮10S
				BL_ON();
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x02)
			{	
				stLocalControl.byDispNumSetOkTime=50;		//显示一秒

				if((stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.bySndSecAddr)||
				   (stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.bySndRoomAddr)||
				   (stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.bySndBedAddr))
				{
				//保存地址
					stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
					stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
					stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
					
					stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				page=0;
				ShowNumSetOk();
				//SaveParameter();
				
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x00)	//编号超时
			{
				Set_VisualPage(1);
				stLocalControl.byDisplayFace=BED_INFO_FACE;
				break;
			}
			break;
		case CMD_SYSTEM_SEC_SET:							//系统区号设置
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;
			
		case CMD_POWER_ON:	  								//本机重新热启动
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
				OS_ENTER_CRITICAL();						
				//关闭所有中断
				CCAPM0 = 0;
				ET0 = 0;
				TR0 = 0;
				EX1 = 0;
				ISP_CONTR = 0x20; 			
			}
			break;			
		case CMD_INFUSION_CALL:							//输液呼叫		 
		case CMD_SERVICE_CALL:							//服务呼叫			 
		case CMD_EMERGENCY_CALL:						//紧急呼叫
		case CMD_HELP_CALL:								//求援呼叫
			 break;
		case CMD_INFUSION_ENTER:								//输液呼叫确认命令
		case CMD_SERVICE_ENTER:									//服务呼叫确认命令
		case CMD_HELP_ENTER:									//求援呼叫确认命令
		case CMD_EMERGENCY_ENTER:								//紧急呼叫确认命令
			page=0;
			 ShowCallSuccess();
			 stLocalControl.uiLcdDisplayTime = 250;	//如果黑时亮5S
			 BL_ON();
			 break;
			 
		 case CMD_SERVICE_CLEAR:								 //服务呼叫清除命令 
		 case CMD_CHANNEL_CLOSE:		 						 //通道关闭
		 	 #if CONFIG_CC1101==1
		 	  memcpy(&(byWireLessSndBuff[1]),&(stLocalControl.stEepromCfgData.byRFSerialNum1),6);
			  byWireLessSndBuff[7]= CMD_INFUSION_CLEAR;

			  halRfSendPacket(byWireLessSndBuff,8);	//发送数据
			  Delayms(1000);
			  halRfSendPacket(byWireLessSndBuff,8);	//发送数据
			  Delayms(1000);		 			  
  			  halSpiStrobe(CCxxx0_SRX);	//允许接收
  			  CCAPM1 |= PCA1_ECCF;	//开启中断	
			 #endif
			 
			  Set_VisualPage(1);
			  stLocalControl.byDisplayFace=BED_INFO_FACE;			
			  break;  			 
		 case CMD_INFUSION_CLEAR:								 //输液呼叫清除命令
		 case CMD_HELP_CLEAR:									 //求援呼叫清除命令
		 case CMD_EMERGENCY_CLEAR:								 //紧急呼叫清除命令 	
			   Set_VisualPage(1);
			   stLocalControl.byDisplayFace=BED_INFO_FACE;			
			  break;
		case CMD_INFUSION_ANSWER:							//处理输液呼叫命令
		case CMD_SERVICE_ANSWER:							//处理服务呼叫命令
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令
		case CMD_HELP_ANSWER :
			 Set_VisualPage(1);
			stLocalControl.byDisplayFace=BED_INFO_FACE;			 
			  break;	

		case CMD_CHANNEL_CHANGE:
			page=0;
			ShowTalkFace(stLocalControl.stBusDealFreq.byRecRoomAddr, stLocalControl.stBusDealFreq.byRecBedAddr);
			break;	
		case CMD_BUS_ANSWER:
			 break;
		case CMD_SUPPLY_OX_START:
			bEnableOxTimer=1;
			 memcpy(&(stLocalControl.stSupplyOxStartTime.byYear),&(stLocalControl.stTime.byYear),6);

			 ShowItem(OX_START);

			 memset(&(stLocalControl.stSupplyOxEndTime.byYear),0x00,sizeof(STTime));
			 ShowItem(OX_END);
				

			 memset(&(stLocalControl.stSupplyOxSubtotalTime.uiHour),0x00,sizeof(STOXTime)); //本次时间区域清0
			 ShowItem(OX_SUBTOTAL);
			 
			
			if(stLocalControl.byDisplayFace==BED_INFO_FACE) Set_VisualPage(1);	

			stLocalControl.uiLcdDisplayTime = 250; //如果黑时亮5S
			//BL_SW=1;
			BL_ON();
			break;
		case CMD_SUPPLY_OX_END:
			if(bEnableOxTimer==1)
			{
				bEnableOxTimer=0;
				 memcpy(&(stLocalControl.stSupplyOxEndTime.byYear),&(stLocalControl.stTime.byYear),6);

				//计算单次时间
				stLocalControl.stSupplyOxSubtotalTime=timeInterval(stLocalControl.stSupplyOxEndTime,stLocalControl.stSupplyOxStartTime);
				//如果时间大于3秒,将算1分钟
				if(stLocalControl.stSupplyOxSubtotalTime.bySec>3)
				{
					stLocalControl.stSupplyOxSubtotalTime.byMin++;
					if(stLocalControl.stSupplyOxSubtotalTime.byMin==60)
					{
						if(stLocalControl.stSupplyOxSubtotalTime.uiHour<=9998) stLocalControl.stSupplyOxSubtotalTime.uiHour++;
					}
				}
				//发送单次时间数据给主控模块
				stLocalControl.stBusDealFreq.bySndSecAddr =  stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_SUPPLY_OX_END;
				stLocalControl.stBusDealFreq.byRecSecAddr =  (stLocalControl.stSupplyOxSubtotalTime.uiHour>>8);
				stLocalControl.stBusDealFreq.byRecRoomAddr = (uint8)stLocalControl.stSupplyOxSubtotalTime.uiHour;
				stLocalControl.stBusDealFreq.byRecBedAddr =  stLocalControl.stSupplyOxSubtotalTime.byMin;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				//计算累次时间
				stLocalControl.stSupplyOxTotalTime.byMin += stLocalControl.stSupplyOxSubtotalTime.byMin;
				stLocalControl.stSupplyOxTotalTime.uiHour+= stLocalControl.stSupplyOxSubtotalTime.uiHour;
				if(stLocalControl.stSupplyOxTotalTime.byMin>=60)
				{
					stLocalControl.stSupplyOxTotalTime.byMin = stLocalControl.stSupplyOxTotalTime.byMin-60;
					stLocalControl.stSupplyOxTotalTime.uiHour++;
				}	

				memset(&(stLocalControl.stSupplyOxStartTime.byYear),0x00,sizeof(STTime));	//开始时间清0
				ShowItem(OX_START);
				ShowItem(OX_END);	//显示供氧结束时间
				ShowItem(OX_SUBTOTAL);	//显示本次供氧时间
				ShowItem(OX_TOTAL);    //显示累次时间

			   if(stLocalControl.byDisplayFace==BED_INFO_FACE) Set_VisualPage(1);

				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour=stLocalControl.stSupplyOxTotalTime.uiHour;
				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin=stLocalControl.stSupplyOxTotalTime.byMin;

				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				//SaveParameter();

				stLocalControl.uiLcdDisplayTime = 250;	//如果黑时亮5S
				 //BL_SW=1;
				 BL_ON();
			}
			break;	

		case CMD_RF_NUMSET_START:
			page=0;
			ShowStartRfNumSet();
			 stLocalControl.uiLcdDisplayTime = 250;	//如果黑时亮5S
			 //BL_SW=1;	
			 BL_ON();
			break;
		case CMD_RF_NUMSET_OK:
			 stLocalControl.uiLcdDisplayTime = 250;	//如果黑时亮5S
			 //BL_SW=1;	
			 BL_ON();
			 page=0;
			ShowRfNumSetOk();	
			stLocalControl.byDispNumSetOkTime=50;		//显示一秒
			//保存输液报警器序列号

			if( (stLocalControl.stEepromCfgData.byRFSerialNum1!= stLocalControl.stBusDealFreq.bySndSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum2!= stLocalControl.stBusDealFreq.bySndRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum3!= stLocalControl.stBusDealFreq.bySndBedAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum4!= stLocalControl.stBusDealFreq.byRecSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum5!= stLocalControl.stBusDealFreq.byRecRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum6!= stLocalControl.stBusDealFreq.byRecBedAddr))
			{
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum1),&(stLocalControl.stBusDealFreq.bySndSecAddr),3);
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum4),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);

				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			//SaveParameter();
			}
			break;
		case CMD_RF_NUMSET_END:
			Set_VisualPage(1);			//恢复到常态界面
			stLocalControl.byDisplayFace=BED_INFO_FACE;			
			break;	
	    case CMD_TELL_RF_NUM:
			if( (stLocalControl.stEepromCfgData.byRFSerialNum1!= stLocalControl.stBusDealFreq.bySndSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum2!= stLocalControl.stBusDealFreq.bySndRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum3!= stLocalControl.stBusDealFreq.bySndBedAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum4!= stLocalControl.stBusDealFreq.byRecSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum5!= stLocalControl.stBusDealFreq.byRecRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum6!= stLocalControl.stBusDealFreq.byRecBedAddr))
			{
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum1),&(stLocalControl.stBusDealFreq.bySndSecAddr),3);
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum4),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);

				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
				//SaveParameter();	
			}
			break;
			
		case CMD_OPEN_LCD:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//绝对地址
				stLocalControl.bySlaveState |= LCD_ON;
				BL_ON();
			}
			else if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//广播打开
				stLocalControl.bySlaveState |= LCD_ON;
				if(!(stLocalControl.bySlaveState &PATIENT_DISCHARGE))
				{//此床位有病人
					BL_ON();
				}
			}
			break;
			
		case CMD_CLOSE_LCD:
			stLocalControl.bySlaveState &= ~LCD_ON;
			BL_OFF();
			break;	
			
		case CMD_NURSE_IN:
			if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState&PATIENT_DISCHARGE)) //黑屏的情况下	
			{
				BL_ON();
			}
			break;
			
		case CMD_NURSE_OUT:
			if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState&PATIENT_DISCHARGE)) //黑屏的情况下	
			{
				BL_OFF();
			}
			break;
			
		case CMD_PC_OPEN:
			stLocalControl.stBusDealFreq.bySndSecAddr =  stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;			
			stLocalControl.stBusDealFreq.byCmd = CMD_SUPPLY_OX_TOTAL;			
			stLocalControl.stBusDealFreq.byRecSecAddr = (stLocalControl.stSupplyOxTotalTime.uiHour>>8);
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stSupplyOxTotalTime.uiHour;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stSupplyOxTotalTime.byMin;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_OPEN_485BUS_IN:	
			stLocalControl.bySlaveState &= ~PATIENT_DISCHARGE;  //病人入院
			BL_ON();
			stLocalControl.uiLcdDisplayTime= 500;	//亮10秒钟
			break;


		case CMD_BUS1_TEST:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS1_TEST_ANSWER;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			if( (stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.byRecSecAddr)||
				(stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.byRecRoomAddr)||
				(stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.byRecBedAddr))
			{
				stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.byRecSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.byRecRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.byRecBedAddr;
				
				stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
				SaveParameter(IAP_ADDR);

				page=1;
				ShowItem(BED_NUMBER);
				if(stLocalControl.byDisplayFace == BED_INFO_FACE) Set_VisualPage(1);				
			}			
			break;

		case CMD_SEND_PIN_BUG:
			if(stLocalControl.stBusDealFreq.byRecSecAddr ==0x00)
			{
				page=1;
				DisSendPinBugFace();
			}
			else if(stLocalControl.stBusDealFreq.byRecSecAddr ==0x01)
			{
				page=1;			
				ShowBedFace();
				Set_VisualPage(1);
			}
			break;
		case CMD_SB_DATA_ANSWER:

			if(stLocalControl.stBusDealFreq.byRecSecAddr == 0x01)
			{//收到回应答,发送余下信息
				UsartSendSBNextFrame();
			}
			else
			{//重发
				if(--stLocalControl.bySendSBTimes !=0x00)
				{
					i=1000;
					while(stUsartCfg.uiSndLen)
					{
						delay_nms(1);
						if(--i ==0) break;
					}
					WDT_CONTR = 0x3e;	//喂狗
					
					stLocalControl.byUsart1SdDelayTime =150;
					stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg.uiSndPos = 0; 	//从数据帧头部开始发送数据
					stUsartCfg.pbySndBuf = byUsart0SndBuf;
					
					TI=0;
					SBUF= byUsart0SndBuf[0];
				}
				else UsartSendSBNextFrame();
			}
			break;
		case CMD_RS485_BRT_SET:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case 1:
					stLocalControl.stEepromCfgData.uiRs485Brt	=1200;
					break;
				case 2:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 2400;
					break;
				case 3:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 4800;
					break;
				case 4:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 9600;
					break;
				case 5:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 14400;
					break;
				case 6:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 19200;
					break;
				case 7:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 28800;
					break;
				case 8:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 38400;
					break;
				case 9:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 57600;
					break;
				default:
					return;		//其它无效，退出
					
			}
			UsartInit();
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_RS485_INFO_SEC_ROOM:
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0 ||stLocalControl.stBusDealFreq.byRecSecAddr==1)
			{
				stLocalControl.stEepromCfgData.byRs485SecRoom =  stLocalControl.stBusDealFreq.byRecSecAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
			}
			break;
		case CMD_DISP_MODE:
			break;

		case CMD_COLOR_CLEAR:
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
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;	
	uint8 xdata i;
	//取出发送完成的数据帧
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;
	OS_EXIT_CRITICAL();	

	//以下仅供测试用
//	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时
				MakeCH0TimerOut(50, 30);				
			}	
			break;

		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 0;								//制造总线故障

			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			SaveParameter(IAP_ADDR);
			
			Bus0SendPin = 1;								//释放总线				
			break;

		case CMD_OPEN_485BUS_IN:
			i=1000;
			while(stUsartCfg.uiSndLen)
			{
				delay_nms(1);
				if(--i ==0) break;
			}
			WDT_CONTR = 0x3e;	//喂狗
			//一类信息的第一帧
			
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case DIET_TYPE:
					 Read_Cont((uint8 *)(&(stLocalControl.uiSendSBTotalDataLen)),DIET_LENGTH_ADDR,2);
					if(stLocalControl.uiSendSBTotalDataLen <=UART0_TX_BUF_SIZE-EXT_SIZE-5)
					{//一帧可以发送完成
						Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR,stLocalControl.uiSendSBTotalDataLen);
						pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
						pstDataFreq->byData[4] =1;	//结束标志
						stLocalControl.uiSendSBTotalDataLen =0;
					}
					else 
					{
						Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR,UART0_TX_BUF_SIZE-EXT_SIZE-5);
						pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
						stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
						pstDataFreq->byData[4] =0;	//非结束
					}
					pstDataFreq->byData[1] = DIET_TYPE;
					break;
				case PRESCRIPTION_TYPE:
					Read_Cont((uint8 *)(&(stLocalControl.uiSendSBTotalDataLen)),PRESCRIPTION_LENGTH_ADDR,2);
					if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
					{
						Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR,stLocalControl.uiSendSBTotalDataLen);
						pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
						pstDataFreq->byData[4] =1;	//结束标志
						stLocalControl.uiSendSBTotalDataLen =0;
					}
					else 
					{
						Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR,UART0_TX_BUF_SIZE-EXT_SIZE-5);
						pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
						stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
						pstDataFreq->byData[4] =0;	//非结束
					}
					pstDataFreq->byData[1] = PRESCRIPTION_TYPE;
					break;
				case PROMPT_INFO_TYPE:
					Read_Cont((uint8 *)(&(stLocalControl.uiSendSBTotalDataLen)),PROMPT_LENGTH_ADDR,2);
					if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
					{
						Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR,stLocalControl.uiSendSBTotalDataLen);
						pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
						pstDataFreq->byData[4] =1;	//结束标志
						stLocalControl.uiSendSBTotalDataLen =0;
					}
					else 
					{
						Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR,UART0_TX_BUF_SIZE-EXT_SIZE-5);
						pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
						stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
						pstDataFreq->byData[4] =0;	//非结束
					}
					pstDataFreq->byData[1] = PROMPT_INFO_TYPE;
					break;
				default:
					return;	//返回
			}


			stLocalControl.bySendSBTimes=3;	//重发次数
			stLocalControl.byUsart1SdDelayTime =150;
			stLocalControl.uiSendedDataLen = pstDataFreq->uiLen-5;
			
			pstDataFreq->byStartByte1 = START_BYTE1;
			//源地址忽略
			pstDataFreq->bySndSecAddr = 0x00;
			pstDataFreq->bySndRoomAddr= 0x00;
			pstDataFreq->bySndBedAddr =0x00;
			
			pstDataFreq->byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->byRecRoomAddr= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			pstDataFreq->byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;

			pstDataFreq->byCmd= CMD_SB_LCD_DISPLAY_DATA;
			pstDataFreq->byData[0] = 0x00;	//数据关型，都不为点阵码数据
			pstDataFreq->byData[2] = 0x01;	//起始帧
			pstDataFreq->byData[3] = 0x01;	//屏号，未用

			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
			
			
			stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg.uiSndPos = 0; 	//从数据帧头部开始发送数据
			stUsartCfg.pbySndBuf = byUsart0SndBuf;
			
			TI=0;
			SBUF= byUsart0SndBuf[0];			
			break;
			
		default:
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
		if(bBus0RecFinish)									//总线0收到数据
		{ 	
			OSQPost(byMainCmdQ, BUS0_REC);				
		}
		if(bBus0SendFinish)									//总线0发送完数据帧
		{
			OSQPost(byMainCmdQ, BUS0_SND);			
		} 
		OSWait(K_TMO, 1);	
		
		//复位看门狗
		WDT_CONTR = 0x3e; 		
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
	uint8 xdata i;
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;
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
				}
				else
				{ 	//超时次数没有完成，重新加载单位超时时间 				
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}
			}			
		}
	}
	if(stLocalControl.byDispNumSetOkTime)
	{
		if(--stLocalControl.byDispNumSetOkTime==0)
		{
			Set_VisualPage(0x01);
			stLocalControl.byDisplayFace = BED_INFO_FACE;
		}
	}

	if(++stLocalControl.byMSecond==50)
	{
		stLocalControl.byMSecond=0;
		stLocalControl.stTime.bySec++;
		if(stLocalControl.stTime.bySec==60)
		{
			stLocalControl.stTime.bySec=0;
		}
	}
	
	if(stLocalControl.uiLcdDisplayTime)
	{
		if(--stLocalControl.uiLcdDisplayTime==0)	//LCD在显示常关的情况下显示时间已到
		{
			if( !(stLocalControl.bySlaveState&LCD_ON) || (stLocalControl.bySlaveState &PATIENT_DISCHARGE))
			{//此床没病人
				BL_OFF();
			}
		}
	}

#if CONFIG_CC1101==1
	if(stLocalControl.uiCC1101ReReadTime)	//CC1101每隔5分钟读取1次接收缓冲区
	{
		stLocalControl.uiCC1101ReReadTime--;
		if(stLocalControl.uiCC1101ReReadTime==0)
		{
			if(bWireLessRec==0)
			{
				byWireLessRecLen=8; //要读取数据的最大长度
				halRfReceivePacket(byWireLessRecBuff,&byWireLessRecLen);	//清洗掉接收缓冲区
				halSpiStrobe(CCxxx0_SRX);	//允许接收
			}
			stLocalControl.uiCC1101ReReadTime = CC1101_REREAD_TIME;		//重新赋时
		}
	}
#endif	

	if(stLocalControl.byUsart1SdDelayTime)
	{
		if(--stLocalControl.byUsart1SdDelayTime ==0)
		{//时间到
			if(stLocalControl.bySendSBTimes)
			{//有次数定时
				if(--stLocalControl.bySendSBTimes !=0)
				{
					//重发数据
					i=1000;
					while(stUsartCfg.uiSndLen)
					{
						delay_nms(1);
						if(--i ==0) break;
					}
					WDT_CONTR = 0x3e;	//喂狗
					
					stLocalControl.byUsart1SdDelayTime =150;
					stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg.uiSndPos = 0;	//从数据帧头部开始发送数据
					stUsartCfg.pbySndBuf = byUsart0SndBuf;
					
					TI=0;
					SBUF= byUsart0SndBuf[0];
				}
				else UsartSendSBNextFrame();
			}
		}
	}
	else
	{
		if(stLocalControl.byDisplayFace !=TALK_FACE)
		{//非通话状态
			if(bUsartRecedDietData == 1)
			{
				if(stUsartCfg.uiSndLen==0)
				{//没有串口信息正在发送
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					stLocalControl.stBusDealFreq.byRecSecAddr = DIET_TYPE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					bUsartRecedDietData=0;
					stLocalControl.byUsart1SdDelayTime =150;
				}
			}
			
			else if(bUsartRecedPrescriptionData == 1)
			{
				if(stUsartCfg.uiSndLen == 0)
				{//没有其它信息正在发送
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					stLocalControl.stBusDealFreq.byRecSecAddr = PRESCRIPTION_TYPE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					bUsartRecedPrescriptionData=0;	
					stLocalControl.byUsart1SdDelayTime =150;
				}
					
			}

			else if(bUsartRecedPromptData ==1)
			{
				if(stUsartCfg.uiSndLen == 0)
				{//没有其它信息正在发送
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					stLocalControl.stBusDealFreq.byRecSecAddr = PROMPT_INFO_TYPE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					bUsartRecedPromptData=0;
					stLocalControl.byUsart1SdDelayTime =150;
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
		if(stLocalControl.stCH0TimerOut.byTimerOut)
		{	
			OSQPost(byMainCmdQ, TIMER_OUT);				
		}
		else
		{		
			OSQPost(byMainCmdQ, TIMER_OUT);	
		}
		//复位看门狗
		WDT_CONTR = 0x3e; 
	}
} 
  	  
