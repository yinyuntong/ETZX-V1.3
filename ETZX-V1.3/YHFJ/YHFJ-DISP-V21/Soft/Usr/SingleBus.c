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
uint8 xdata byBus0RecCount = 0;								//高4位是总线0接收定时中断计数，低4位是接收到的位计数
uint8 xdata byBus0RecBuf;									//总线0接收缓冲单元
uint8 xdata byBus0SendBuf;									//总线0发送缓冲单元	
uint8 data byBus0RecData[BUS0_FREQ_SIZE];					//总线0接收缓冲区
uint8 data byBus0SendData[BUS0_FREQ_SIZE];					//总线0发送缓冲区
uint8 xdata byBus0SendCount = 0;								//高4位是总线0发送定时中断计数，低4位是发送的位计数
uint8 xdata byBus0RecSendCount = 0;							//高4位是总线0接收到的字节计数，低4位是总线0发送完的字节计数
uint8 xdata byBus0SendStopCount;								//总线0停止时间计数
uint8 xdata byBus0RecTimeOut = 0;							//总线0接收超时计数
uint8 xdata byBus0DisableCount = 0;							//总线0禁止计数	 
uint8 xdata byBus0TxQ[BUS0_TX_Q_ZISE];						//总线0发送队列							
uint8 xdata byBus0TxHead = 0;								//单总线0发送队列头指针
uint8 xdata byBus0TxTail = 0; 								//单总线0发送队列尾指针
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 


sbit  Bus0RecPin	= P3^3;									//单总线0接收引脚定义
sbit  Bus0SendPin	= P3^4;									//单总线0发送引脚定义

uint8 xdata VolumnValue=0;

bit bSecondDisState;										//秒钟闪烁标志
bit bSignflag=0;											//正负符号标志

//STBusFreq xdata stBus0RxQ[MAX_FREQ_NUMBER];						////总线0接收队列
//uint8 xdata byBus0FreqNum=0;							//帧数
uint8 xdata byBus0RxQ[BUS0_RX_Q_ZISE];						//总线0接收队列
uint8 xdata byBus0RxHead = 0;								//单总线0接收队列头指针
uint8 xdata byBus0RxTail = 0; 								//单总线0接收队列尾指针

uint8 code byNumPW[7]={8,8,6,6,2,2,4};						//编号密码


uint8 Bus0RecData(uint8* pbyData);

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

	//P3^4推挽输出
//	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
//	P3M1 &= (~Bin(0,0,0,1,0,0,0,0));

	//P3^4开漏输出
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);


	//总线电平设置
	Bus0SendPin = 1;
	Bus0RecPin = 1;	 
	
	

 	TMOD &= 0xf0;   //  ;t1作波特率发生器(不变)，
	TMOD |= 0X01;	//t0作方式1（16位)定时器

	TL0 = TIMER0_L;
	TH0 = TIMER0_H;

/*	TMOD |= 0X02;	//T0作方式2（8位)定时器
	TL0 = TIMER0_COUNT;
	TH0 = TIMER0_COUNT;*/
	
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
								if(TRUE==Bus0RecData(byBus0RecData))
								{//保存到队列中	
									//bBus0RecFinish = 1;
								}
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
	uint8 xdata i;
	 WDT_CONTR = 0x3d; 
	if(stLocalControl.byCallCount ==0)
	{//呼叫队列空
		memcpy(stLocalControl.byCallArray[0],pstBusFreq,4);
		stLocalControl.byCallCount++;
		return true;
	}
	else 
	{//呼叫队列非空,先查找是否有该节点了
		for(i=0;i<stLocalControl.byCallCount;i++)
		{
			if((stLocalControl.byCallArray[i][0]==pstBusFreq->bySndSecAddr)&&
			   (stLocalControl.byCallArray[i][1]==pstBusFreq->bySndRoomAddr)&&
			   (stLocalControl.byCallArray[i][2]==pstBusFreq->bySndBedAddr)&&
			   (stLocalControl.byCallArray[i][3]==pstBusFreq->byCmd))
			   break;	//已有该节点,退出
		}
		if(i>=stLocalControl.byCallCount)
		{//没有该节点,
			if(stLocalControl.byCallCount<MAX_CALL_COUNT)
			{
				//将该节点加入到呼叫队列中
				memcpy(stLocalControl.byCallArray[stLocalControl.byCallCount],pstBusFreq,4);
				stLocalControl.byCallCount++;
				return true;
			}
			else return false;
			
		}
		else return false;
	}
	
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

	uint8 xdata i;
	 WDT_CONTR = 0x3d; 
	if(stLocalControl.byCallCount ==0)
	{//呼叫队列空
		return;
	}
	else 
	{//呼叫队列非空,先查找是否有该节点
		for(i=0;i<stLocalControl.byCallCount;i++)
		{
			if((stLocalControl.byCallArray[i][0]==pstBusFreq->byRecSecAddr)&&
			   (stLocalControl.byCallArray[i][1]==pstBusFreq->byRecRoomAddr)&&
			   (stLocalControl.byCallArray[i][2]==pstBusFreq->byRecBedAddr)&&
			   (stLocalControl.byCallArray[i][3]==pstBusFreq->byCmd))
			   break;	//已有该节点,退出循环
		}
		if(i>=stLocalControl.byCallCount)
		{//没有该节点,
			return;
		}
		else 
		{
			stLocalControl.byUpdataRecNum=i;
			stLocalControl.byCallCount--;
			if(stLocalControl.byUpdataRecNum != stLocalControl.byCallCount)
			{//更新的记录不是最后一条记录
				memcpy(stLocalControl.byCallArray[i],stLocalControl.byCallArray[stLocalControl.byCallCount],4);
			}
			memset(stLocalControl.byCallArray[stLocalControl.byCallCount],0x00,4);
		}
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
			stLocalControl.stEepromCfgData.byInitFlag = INIT_FLAG;
			stLocalControl.stEepromCfgData.bySelfSecAddr = 0x01;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = YHFJ_ROOM_ADDR;
			stLocalControl.stEepromCfgData.bySelfBedAddr = 0x01;
			stLocalControl.stEepromCfgData.bySerialNum1= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum2= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum3= 0xff;
			stLocalControl.stEepromCfgData.byVersionHi = 0x03;
			stLocalControl.stEepromCfgData.byVersionLo= 0x00;
			stLocalControl.stEepromCfgData.byXCoorPara = 0x64;
			stLocalControl.stEepromCfgData.byYCoorPara = 0x53;
			
			stLocalControl.stEepromCfgData.byXTopLeftADValue = 0x0c;
			stLocalControl.stEepromCfgData.byYTopLeftADValue = 0x16;
			
			stLocalControl.stEepromCfgData.uiRs485Brt = 57600;	//黪认波特率57600
			stLocalControl.stEepromCfgData.byMaxPlayVolVal=8;

			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
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

	uint16 xdata x,y;
	
/*	//取出收到的数据帧
	uint8 xdata i;
	OS_ENTER_CRITICAL();
//	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
//	bBus0RecFinish = 0;
	memcpy(&(stLocalControl.stBusDealFreq), stBus0RxQ, sizeof(STBusFreq));
	byBus0FreqNum--;
	for(i=0;i<byBus0FreqNum;i++)
	{
		stBus0RxQ[i] = stBus0RxQ[i+1];
	}

	OS_EXIT_CRITICAL();	
*/

//以下仅供测试用
	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);

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
				bLanding =0;
				MakeCH0TimerOut(0, 0);
				stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
				SaveParameter(IAP0_ADDR);								
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
			stLocalControl.stTime.bySec = stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f;
			stLocalControl.stTime.byMin = stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f;
			stLocalControl.stTime.byHour = stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f;
			stLocalControl.stTime.byDay = stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f;
			stLocalControl.stTime.byMonth = stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f;
			stLocalControl.stTime.byYear = stLocalControl.stBusDealFreq.byRecBedAddr;


			DisplayDigit(128,0,stLocalControl.stTime.byYear>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(144,0,stLocalControl.stTime.byYear&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);
			
			DisplayDigit(176,0,stLocalControl.stTime.byMonth>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(192,0,stLocalControl.stTime.byMonth&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);	
						
			DisplayDigit(224,0,stLocalControl.stTime.byDay>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(240,0,stLocalControl.stTime.byDay&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);

			DisplayDigit(304,0,stLocalControl.stTime.byHour>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(320,0,stLocalControl.stTime.byHour&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);
			
			DisplayDigit(352,0,stLocalControl.stTime.byMin>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(368,0,stLocalControl.stTime.byMin&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0); 

			
			if(stLocalControl.byDisplayFace==SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);				
			break;
			
		case CMD_CALL_LISTEN:								//接听命令
			ShowTalkFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr,DIS_FRAM_MEM1);
			stLocalControl.byKeyInTimes =0;
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{	//这条命令是本机发送的
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 1,DIS_FRAM_MEM1);
			}
			else
			{	//这条命令是其他分机发送的
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 0,DIS_FRAM_MEM1);
			}
			break;
			
/*		case CMD_INFUSION_CALL:								//输液呼叫命令
		case CMD_SERVICE_CALL:								//服务呼叫命令
		case CMD_EMERGENCY_CALL:							//紧急呼叫命令
		case CMD_HELP_CALL:									//求援呼叫命令
			if(AddCallNod(&(stLocalControl.stBusDealFreq)))
			{//成功
				ShowCallFace(0x00);
			}
			break;	
*/

		case CMD_INFUSION_ENTER: 							//输液呼叫命令
		case CMD_SERVICE_ENTER:								//服务呼叫命令
		case CMD_EMERGENCY_ENTER:							//紧急呼叫命令
		case CMD_HELP_ENTER: 								//求援呼叫命令
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stBusDealFreq.byRecRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stBusDealFreq.byRecBedAddr;
			stLocalControl.stBusDealFreq.byCmd -= 0x1b; 		//将清除命令还原成对应的呼叫命令
			AddCallNod(&(stLocalControl.stBusDealFreq));
			ShowCallFace1(DIS_FRAM_MEM2);
			break;	
			
		case CMD_INFO_INDICATION:							//屏显示呼叫信息
			if(stLocalControl.byDisplayFace != INFO_INDICATION_FACE)
			{//之前显示界面不是信息指示界面
				if(stLocalControl.byCallCount)
				{
					ShowCallFace0(DIS_FRAM_MEM2);
				}
			}
			break;
			
		case CMD_STOP_INDICATION:	 						//屏关闭呼叫信息显示 				
			//ShowSecInfo();
			DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
			if(stLocalControl.byCallCount==0)
			{
				Set_VisualPage(DIS_FRAM_MEM0);
				stLocalControl.byDisplayFace = SEC_INFO_FACE;
			}
			break;
		case CMD_INFUSION_CLEAR:							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR: 							//清除服务呼叫命令
		case CMD_HELP_CLEAR:								//清除求援呼叫命令
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫命令
			stLocalControl.stBusDealFreq.byCmd -= 0x0a; 		//将清除命令还原成对应的呼叫命令		
			RemoveCallNod(&(stLocalControl.stBusDealFreq));
//			if(stLocalControl.stCallPointer.pstHead)
			if(stLocalControl.byCallCount)
			{//队列不为空
				ShowCallFace2(DIS_FRAM_MEM2);
			}
			else
			{
				Set_VisualPage(DIS_FRAM_MEM0);
				stLocalControl.byDisplayFace=SEC_INFO_FACE;			
			}
			break;

		case CMD_INFUSION_ANSWER:							//处理输液呼叫命令
		case CMD_SERVICE_ANSWER:							//处理服务呼叫命令
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令
		case CMD_HELP_ANSWER:								//处理求援呼叫命令
			stLocalControl.stBusDealFreq.byCmd -= 0x06; 		//将清除命令还原成对应的呼叫命令		
			RemoveCallNod(&(stLocalControl.stBusDealFreq));
			if((stLocalControl.stBusDealFreq.bySndSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr)||
			   (stLocalControl.stBusDealFreq.bySndRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr)||
			   (stLocalControl.stBusDealFreq.bySndBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
			{//不是本医护分机处理的
				if(stLocalControl.byDisplayFace == INFO_INDICATION_FACE)
				{
					if(stLocalControl.byCallCount)
					{//队列不为空
						ShowCallFace2(DIS_FRAM_MEM2);
					}
					else
					{
						Set_VisualPage(DIS_FRAM_MEM0);
						stLocalControl.byDisplayFace=SEC_INFO_FACE;			
					}
					break;				
				}
			}
//			ShowTalkFace(stLocalControl.stBusDealFreq.byRecRoomAddr,stLocalControl.stBusDealFreq.byRecBedAddr,DIS_FRAM_MEM1);			 
			break;			

		

		case CMD_CLEAR_LCD:									//清除液晶命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
			Lcd_Clear(BLUE,DIS_FRAM_MEM1);
			}
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

			  
		case CMD_CHANNEL_CLOSE:
			  //ShowSecInfo();
			  DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
			  DisplayBlock(16,240,479,271,BLUE,DIS_FRAM_MEM1);	//将通话音量区域清空
			  Set_VisualPage(DIS_FRAM_MEM0);
			  stLocalControl.byDisplayFace = SEC_INFO_FACE;
			  stLocalControl.byKeyInTimes =0;
			  break;	

		case CMD_KEY_DOWN:		
			switch(stLocalControl.byDisplayFace)
			{
				case MAIN_MENU_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
				 	{
						case KEY_1:	//时间设置
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_YEAR;
							stLocalControl.uiCursorX = 244;		//(480-24*11)/2+5.5*24+4=244
							stLocalControl.uiCursorY = 51;		//(272-32*5-10)/2=51
							ShowTimeSet(DIS_FRAM_MEM1);	//在1页显示
							break;							
				 		case KEY_2:	//分机编号设置
				 			ShowNumberPWInputFace(DIS_FRAM_MEM1);
							//ShowNumberSetFace(DIS_FRAM_MEM1);
							break;							
						case KEY_3:	
							stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_TIMER_GET;
							Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							break;
				 		case KEY_4:
							stLocalControl.stBusDealFreq.byCmd = CMD_BL_TIMER_GET;
							Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));							
							break;
						case KEY_5:
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_WARD_SEC;
							stLocalControl.uiCursorX = 72;		
							stLocalControl.uiCursorY = 72;	
							stLocalControl.byKeyInTimes = 0;
							ShowSecInfoSet(DIS_FRAM_MEM1);	//病区信息设置
							break;

						case KEY_6:
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_FIRST_OPER;
							stLocalControl.uiCursorX = 364;
							stLocalControl.uiCursorY = 104;
							stLocalControl.byKeyInTimes = 0;
							stLocalControl.byOperType = 0;
							bSignflag = 0;
							ShowCalculateFace(DIS_FRAM_MEM1);	//显示计算界面
							break;	
							

				 		case KEY_7:	
							//stLocalControl.stBusDealFreq.byCmd = CMD_MAX_VOL_GET;
							//Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
							
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_PLAY_VOL_MAX; 		
							stLocalControl.uiCursorX = 272;
							stLocalControl.uiCursorY = 60;
							ShowMaxVolSetFace(DIS_FRAM_MEM1);	//显示最大音量设置界面
							break;
							
						case KEY_9:	
						case KEY_0:							
							break;
						case KEY_CLEAR:
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
							memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
							stLocalControl.stBusDealFreq.byCmd = CMD_MAIN_MENU_ESC;
							Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							stLocalControl.byKeyInTimes =0 ;
							break;
						default:
							break;
						
					 }
					 break;
				case NUMBER_PW_INPUT_FACE://编号密码输入界面
					if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
					{
						uint8 i=0;
						y=30;x=250;
						for(;i<6;i++)
						{
							(unOperData.ASCII_BCD)[i]=(unOperData.ASCII_BCD)[i+1];
							
						}
						(unOperData.ASCII_BCD)[6] = stLocalControl.stBusDealFreq.bySndSecAddr;
						DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),7,BLUE,WHITE,3,DIS_FRAM_MEM1);
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_ENTER)
					{
						uint8 i=0;
						for(;i<7;i++)
						{
							if(byNumPW[i]!=(unOperData.ASCII_BCD)[i]) break;
						}
						if(i>=7)	//密码正确
						{
							ShowNumberSetFace(DIS_FRAM_MEM1);
						}
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
						stLocalControl.byKeyInTimes =0 ;				
					}
					break;
					
				case NUMBER_SET_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
					 {
						case KEY_1://本医护分机编号
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_SECTION;
							//stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowSelfNumberSet(DIS_FRAM_MEM1);	//在1页显示 
							break;

						case KEY_2://卫浴门口分机编号
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowWcDoorFjNumberSet(DIS_FRAM_MEM1);	//在1页显示 
							break;

						case KEY_3:// 移动分机在线编号
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		
							stLocalControl.uiCursorY = 102;		
							ShowMoveFjNumberSet(DIS_FRAM_MEM1);	//在1页显示 
							break;

						case KEY_4:// 加床分机在线编号
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowAddFjNumberSet(DIS_FRAM_MEM1);	//在1页显示 
							break;
							
				 		case KEY_5:	//床头分机号码设置
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_ROOM;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowBedFjNumberSet(DIS_FRAM_MEM1);	//在1页显示*/
							break;
							
						case KEY_CLEAR:
							ShowMainMenuFace(DIS_FRAM_MEM1);
							stLocalControl.byKeyInTimes =0 ;
							break;
					 }
					 break;

				case TIME_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_YEAR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;		
								x -=4;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
								DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
								stLocalControl.byInputBox = INPUT_MONTH;
								stLocalControl.uiCursorX = 244;		
								stLocalControl.uiCursorY = 85;		//51+34=85	
								x= stLocalControl.uiCursorX+60;		//28+32
								y= stLocalControl.uiCursorY;
								DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);
							}
							break;
						case INPUT_MONTH:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[3];
								(unOperData.ASCII_BCD)[3]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((((unOperData.ASCII_BCD)[2]*16+(unOperData.ASCII_BCD)[3])<=0x12) && (((unOperData.ASCII_BCD)[2]*16+(unOperData.ASCII_BCD)[3])!=0x00))
								{//输入月份数据有效
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;	
									x -= 4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_DAY;
									stLocalControl.uiCursorX = 244; 	
									stLocalControl.uiCursorY = 119;		//85+34=119	
									x= stLocalControl.uiCursorX+60; 	//28+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);

								}
							}
							break;							
						case INPUT_DAY:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[4]=(unOperData.ASCII_BCD)[5];
								(unOperData.ASCII_BCD)[5]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[4]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((((unOperData.ASCII_BCD)[4]*16+(unOperData.ASCII_BCD)[5])<=0x31) && (((unOperData.ASCII_BCD)[4]*16+(unOperData.ASCII_BCD)[5])!=0x00))
								{//输入日期数据有效							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;	
									x -=4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[4]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_HOUR;
									stLocalControl.uiCursorX = 244; 	
									stLocalControl.uiCursorY = 153;		//119+34=153	
									x= stLocalControl.uiCursorX+60; 	//28+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);							
								}
							}
							break;	
						case INPUT_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if(((unOperData.ASCII_BCD)[6]*16+(unOperData.ASCII_BCD)[7])<0x24)
								{//输入小时数据有效								
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									x -=4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_MIN;
									stLocalControl.uiCursorX = 244; 	
									stLocalControl.uiCursorY = 187;		//153+34=187	
									x= stLocalControl.uiCursorX+60; 	//28+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);							
								}
							}
							break;	
						case INPUT_MIN:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[8]=(unOperData.ASCII_BCD)[9];
								(unOperData.ASCII_BCD)[9]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[8]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if(((unOperData.ASCII_BCD)[8]*16+(unOperData.ASCII_BCD)[9])<0x60)
								{//输入分钟数据有效							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;	
									x -=4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[8]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常

									stLocalControl.stTime.byYear  = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];	//年
									stLocalControl.stTime.byMonth =  (unOperData.ASCII_BCD)[2]*16+(unOperData.ASCII_BCD)[3];	//月
									stLocalControl.stTime.byDay = (unOperData.ASCII_BCD)[4]*16+(unOperData.ASCII_BCD)[5];	//日
									stLocalControl.stTime.byHour= (unOperData.ASCII_BCD)[6]*16+(unOperData.ASCII_BCD)[7];	//时
									stLocalControl.stTime.byMin= (unOperData.ASCII_BCD)[8]*16+(unOperData.ASCII_BCD)[9];	//分
									stLocalControl.stTime.bySec = 0x00;

									//发送到总线顺序：秒，分，时，命令，天，月，年
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stTime.bySec;
									stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stTime.byMin;
									stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stTime.byHour;
									stLocalControl.stBusDealFreq.byCmd = CMD_DATE_SEND;
									stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stTime.byDay;
									stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stTime.byMonth;
									stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stTime.byYear;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

									ShowMainMenuFace(DIS_FRAM_MEM1);
								}
														
							}
							break;	
					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;
					
				case BED_FJ_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
/*						case INPUT_SECTION:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[2];
								(unOperData.ASCII_BCD)[2]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2] <255)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									stLocalControl.byInputBox = INPUT_ROOM;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 152;		//118+34=152
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//显示下一个输入框	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);						
									
								}
							}
							break;*/
						case INPUT_ROOM:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[3]=(unOperData.ASCII_BCD)[4];
								(unOperData.ASCII_BCD)[4]=(unOperData.ASCII_BCD)[5];
								(unOperData.ASCII_BCD)[5]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5] <255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									//stLocalControl.uiCursorY = 186;		//152+34=186
									stLocalControl.uiCursorY = 152;		//118+34=152

									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 						
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//显示下一个输入框									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);									
								}
							}
							break;
						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120	
									
									//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = (unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5];
									stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
									stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
									stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//开始编号;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

									
									(unOperData.ASCII_BCD)[8] += 1;
									if((unOperData.ASCII_BCD)[8]==10)
									{
										(unOperData.ASCII_BCD)[8]=0;
										(unOperData.ASCII_BCD)[7]+= 1;
										if((unOperData.ASCII_BCD)[7]==10)
										{
											(unOperData.ASCII_BCD)[7]=0;
											(unOperData.ASCII_BCD)[6]+= 1;
										}
									}
									
									//stLocalControl.byInputBox = INPUT_SECTION;
									stLocalControl.byInputBox = INPUT_ROOM;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 118;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);
										
								}
							}
							break;							

					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);
					}
					break;
										
				case VOICE_TIMER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_VOICE_START_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[5] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[5] <0x24)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1); 	//32+120
									stLocalControl.byInputBox = INPUT_VOICE_START_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 85;	//51+34=85 
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[6]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[6]&0x0f;

								}
							}
							break;
						case INPUT_VOICE_START_MIN:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[6] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[6]<0x60)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_VOICE_END_HOUR;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 153; 	//51+34+34*2=153
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[7]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[7]&0x0f;									
								}
							}
							break;
						case INPUT_VOICE_END_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[7] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[7] <0x24)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_VOICE_END_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 187; 	//153+34=187
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[8]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[8]&0x0f;									
								}
							}
							break;	
							
					case INPUT_VOICE_END_MIN:
						if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
						{
							(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
							(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
						
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
						}
						else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
						{
							(unOperData.ASCII_BCD)[8] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
							if((unOperData.ASCII_BCD)[8] <0x60)
							{							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY; 
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
								DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);
								
								stLocalControl.byInputBox = INPUT_VOICE_VALUE;
								stLocalControl.uiCursorX = 196;
								stLocalControl.uiCursorY = 221; 	//187+34=221
								x= stLocalControl.uiCursorX+56; 	//24+32
								y= stLocalControl.uiCursorY;
								DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
								unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[9]>>4;
								unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[9]&0x0f;		
							}
						}
						break;

					case INPUT_VOICE_VALUE:
						if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
						{
							(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
							(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;

							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
						}
						else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
						{
							(unOperData.ASCII_BCD)[9] = (unOperData.ASCII_BCD)[0]*10+(unOperData.ASCII_BCD)[1];
							if((unOperData.ASCII_BCD)[9] <9)
							{							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY; 
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
								DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);

								stLocalControl.stBusDealFreq.bySndSecAddr = 	(unOperData.ASCII_BCD)[5];		//开始小时
								stLocalControl.stBusDealFreq.bySndRoomAddr = 	(unOperData.ASCII_BCD)[6];		//开始分钟
								stLocalControl.stBusDealFreq.bySndBedAddr =     0x00;
								stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_TIMER_SET;
								stLocalControl.stBusDealFreq.byRecSecAddr = 	(unOperData.ASCII_BCD)[7];		//开始小时
								stLocalControl.stBusDealFreq.byRecRoomAddr = 	(unOperData.ASCII_BCD)[8];		//开始分钟
								stLocalControl.stBusDealFreq.byRecBedAddr =     (unOperData.ASCII_BCD)[9];

								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
								ShowMainMenuFace(DIS_FRAM_MEM1);		
							}
						}
						break;							
					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;

				case MAX_VOL_SET_FACE:
					if(stLocalControl.stBusDealFreq.bySndSecAddr<9)		//有效的音量值
					{
						(unOperData.ASCII_BCD)[0]= stLocalControl.stBusDealFreq.bySndSecAddr;
						x=stLocalControl.uiCursorX;
						y=stLocalControl.uiCursorY;
						DisplayDigitString(&x,&y,unOperData.ASCII_BCD,1,RED,CYAN,2,DIS_FRAM_MEM1);
					}
					
					else  if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
					{
						stLocalControl.stEepromCfgData.byMaxPlayVolVal= (unOperData.ASCII_BCD)[0];
							
						stLocalControl.stBusDealFreq.bySndSecAddr = 	stLocalControl.stEepromCfgData.byMaxPlayVolVal;
						stLocalControl.stBusDealFreq.byCmd = CMD_MAX_VOL_SET;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						ShowMainMenuFace(DIS_FRAM_MEM1);	
						SaveParameter(IAP0_ADDR);
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;

				case BACK_LIGHT_TIMER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_BL_START_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLACK,RED,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[5] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[5] <0x24)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLACK,CYAN,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//清除"按确认键"
									stLocalControl.byInputBox = INPUT_BL_START_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 85;	//51+34=85  
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[6]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[6]&0x0f;
								}
							}
							break;
						case INPUT_BL_START_MIN:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,RED,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[6] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[6] <0x60)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,CYAN,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//清除"按确认键"
									stLocalControl.byInputBox = INPUT_BL_END_HOUR;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 187; 	//51+34+34*3=187
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[7]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[7]&0x0f;									
								}
							}
							break;
						case INPUT_BL_END_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,RED,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[7] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[7] <0x24)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,CYAN,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//清除"按确认键"
									stLocalControl.byInputBox = INPUT_BL_END_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 221; 	//51+34+34*3+34=221
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[8]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[8]&0x0f;										
								}
							}
							break;	
							
					case INPUT_BL_END_MIN:
						if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
						{
							(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
							(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
						
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,RED,2,DIS_FRAM_MEM1);
						}
						else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
						{
							(unOperData.ASCII_BCD)[8] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
							if((unOperData.ASCII_BCD)[8] <0x60)
							{							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY; 
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,CYAN,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
								DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//清除"按确认键"						
								stLocalControl.stBusDealFreq.bySndSecAddr = 	(unOperData.ASCII_BCD)[5];		//开始小时
								stLocalControl.stBusDealFreq.bySndRoomAddr = 	(unOperData.ASCII_BCD)[6];		//开始分钟
								stLocalControl.stBusDealFreq.bySndBedAddr =     0x00;
								stLocalControl.stBusDealFreq.byCmd = CMD_BL_TIMER_SET;
								stLocalControl.stBusDealFreq.byRecSecAddr = 	(unOperData.ASCII_BCD)[7];		//开始小时
								stLocalControl.stBusDealFreq.byRecRoomAddr = 	(unOperData.ASCII_BCD)[8];		//开始分钟
								stLocalControl.stBusDealFreq.byRecBedAddr =     0x00;

								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
								ShowMainMenuFace(DIS_FRAM_MEM1);		
							}
						}
						break;					
					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;

				case SEC_INFO_SET_FACE:
					stLocalControl.bySecondFlashTime =0;
					if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
					{
						if(stLocalControl.byKeyInTimes<3)
						{
							(unOperData.ASCII_BCD)[stLocalControl.byKeyInTimes]= stLocalControl.stBusDealFreq.bySndSecAddr;
							stLocalControl.byKeyInTimes ++;
						}
						else
						{
							(unOperData.ASCII_BCD)[0]=stLocalControl.stBusDealFreq.bySndSecAddr;
							stLocalControl.byKeyInTimes =1;
						}
						
						x=stLocalControl.uiCursorX;
						y=stLocalControl.uiCursorY;
						DisplayBlock(x,y,x+39,y+23,GREEN,DIS_FRAM_MEM1);
						DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,GREEN,2,DIS_FRAM_MEM1);
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_MR)	//退格
					{
						if(stLocalControl.byKeyInTimes>0)
						{
							stLocalControl.byKeyInTimes--;
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayBlock(x,y,x+39,y+23,GREEN,DIS_FRAM_MEM1);
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,GREEN,2,DIS_FRAM_MEM1);									
						}
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
					{
						stLocalControl.bySecondFlashTime =25;
						if(stLocalControl.byKeyInTimes==0)
						{
							(unOperData.uiOperData)[4] = WardDataBuff[stLocalControl.byInputBox+6];
							(unOperData.ASCII_BCD)[0] = (unOperData.uiOperData)[4]/100;
							(unOperData.ASCII_BCD)[1] = ((unOperData.uiOperData)[4]%100)/10;
							(unOperData.ASCII_BCD)[2] = ((unOperData.uiOperData)[4]%100)%10;
							if((unOperData.ASCII_BCD)[0])
							{
								stLocalControl.byKeyInTimes =3;		//虚拟一个3
							}
							else if((unOperData.ASCII_BCD)[1])
							{
								(unOperData.ASCII_BCD)[0] =(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1] =(unOperData.ASCII_BCD)[2];
								stLocalControl.byKeyInTimes =2;		//虚拟一个2
							}
							else 
							{							
								(unOperData.ASCII_BCD)[0] =(unOperData.ASCII_BCD)[2];
								stLocalControl.byKeyInTimes =1;		//虚拟一个1
							}							
						}

						else 
						{
							if(stLocalControl.byKeyInTimes==3)
							{
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
								
							}
							else if(stLocalControl.byKeyInTimes==2)
							{
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*10+(unOperData.ASCII_BCD)[1];
								
							}
							else if(stLocalControl.byKeyInTimes==1)
							{
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0];
								
							}							
						}
						if((unOperData.uiOperData)[4]<=255)
						{
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY; 
							DisplayBlock(x,y,x+39,y+23,WHITE,DIS_FRAM_MEM1);
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,WHITE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
															
							switch(stLocalControl.byInputBox)
							{
								case INPUT_WARD_SEC:							
									WardDataBuff[6]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_PATIENT_SUM;
									stLocalControl.uiCursorX = 360;
									stLocalControl.uiCursorY = 72;
									break;							
								case INPUT_PATIENT_SUM:							
									WardDataBuff[7]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_TERMINAL_PATIENT_SUM;
									stLocalControl.uiCursorX = 72;
									stLocalControl.uiCursorY = 107;
									break;
								case INPUT_TERMINAL_PATIENT_SUM:
									WardDataBuff[8]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_BED_SUM;
									stLocalControl.uiCursorX = 232;
									stLocalControl.uiCursorY = 107;
									break;	
								case INPUT_BED_SUM:
									WardDataBuff[9]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_EMPTY_BED_SUM;
									stLocalControl.uiCursorX = 392;
									stLocalControl.uiCursorY = 107;
									break;
								case INPUT_EMPTY_BED_SUM:
									WardDataBuff[10]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_ADMISSION_SUM;
									stLocalControl.uiCursorX = 72;
									stLocalControl.uiCursorY = 142;
									break;
								case INPUT_ADMISSION_SUM:
									WardDataBuff[11]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_DISCHARGE_SUM;
									stLocalControl.uiCursorX = 312;
									stLocalControl.uiCursorY = 142;
									break;	
								case INPUT_DISCHARGE_SUM:	//出院
									WardDataBuff[12]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_SWITCH_IN_SUM;
									stLocalControl.uiCursorX = 72;
									stLocalControl.uiCursorY = 175;
									break;
								case INPUT_SWITCH_IN_SUM:
									WardDataBuff[13]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_SWITCH_OUT_SUM;
									stLocalControl.uiCursorX = 312;
									stLocalControl.uiCursorY = 175;
									break;	
								case INPUT_SWITCH_OUT_SUM:
									WardDataBuff[14]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_PRIMARY_CARE_SUM;
									stLocalControl.uiCursorX = 120;
									stLocalControl.uiCursorY = 209;
									break;
								case INPUT_PRIMARY_CARE_SUM:
									WardDataBuff[15]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_OXYGEN_SUPPLY_SUM;
									stLocalControl.uiCursorX = 312;
									stLocalControl.uiCursorY = 209;
									break;	
								case INPUT_OXYGEN_SUPPLY_SUM:
									WardDataBuff[16]= (uint8)(unOperData.uiOperData)[4];

									AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);

									ShowMainMenuFace(DIS_FRAM_MEM1);
									StoreSecInfo(DIS_FRAM_MEM0);

									memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(WardDataBuff[6]),0x03);
									stLocalControl.stBusDealFreq.byCmd = CMD_WARD_SEC_INFO_SET;
									stLocalControl.stBusDealFreq.byRecSecAddr =  WardDataBuff[9];
									stLocalControl.stBusDealFreq.byRecRoomAddr = WardDataBuff[10];
									stLocalControl.stBusDealFreq.byRecBedAddr = 0x01;	//第一帧数据
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									//OSWait(K_TMO,100);	//延时500ms
									Delayms(1000);

									memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(WardDataBuff[11]),0x03);
									stLocalControl.stBusDealFreq.byCmd = CMD_WARD_SEC_INFO_SET;
									stLocalControl.stBusDealFreq.byRecSecAddr =  WardDataBuff[14];
									stLocalControl.stBusDealFreq.byRecRoomAddr = WardDataBuff[15];
									stLocalControl.stBusDealFreq.byRecBedAddr = 0x02;	//第二帧数据
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									//OSWait(K_TMO,100);	//延时500ms
									Delayms(1000);

									stLocalControl.stBusDealFreq.bySndSecAddr = WardDataBuff[16];
									stLocalControl.stBusDealFreq.byCmd = CMD_WARD_SEC_INFO_SET;
									stLocalControl.stBusDealFreq.byRecBedAddr = 0x03;	//第三帧数据
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));									
									break;										
							}
							stLocalControl.byKeyInTimes =0;
						}
					} 							
						
					else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						stLocalControl.bySecondFlashTime =25;
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}					
					break;


				case MOVE_FJ_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{

						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[2];
								(unOperData.ASCII_BCD)[2]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2] <255)
								{							
							    	x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									
									stLocalControl.byInputBox = INPUT_BED_START;
									stLocalControl.uiCursorX = 228;		
									stLocalControl.uiCursorY = 136;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//显示下一个输入框	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);	
								}
							}
							break;


						case INPUT_BED_START:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[3]=(unOperData.ASCII_BCD)[4];
								(unOperData.ASCII_BCD)[4]=(unOperData.ASCII_BCD)[5];
								(unOperData.ASCII_BCD)[5]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5] <=255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									
									stLocalControl.byInputBox = INPUT_BED_END;
									stLocalControl.uiCursorX = 228; 	
									stLocalControl.uiCursorY = 170; 	
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//显示下一个输入框	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);	
								}
							}
							break;

									
									
						case INPUT_BED_END:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <=255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120


									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
									stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
									stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
									stLocalControl.stBusDealFreq.byRecSecAddr =  (unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5];
									stLocalControl.stBusDealFreq.byRecRoomAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
									stLocalControl.stBusDealFreq.byRecBedAddr  =  0x00;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											

									(unOperData.ASCII_BCD)[2] += 1;
									if((unOperData.ASCII_BCD)[2]==10)
									{
										(unOperData.ASCII_BCD)[2]=0;
										(unOperData.ASCII_BCD)[1]+= 1;
										if((unOperData.ASCII_BCD)[1]==10)
										{
											(unOperData.ASCII_BCD)[1]=0;
											(unOperData.ASCII_BCD)[0]+= 1;
										}
									}
									
									//stLocalControl.byInputBox = INPUT_SECTION;
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 102;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD[0]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);											

								}
							}
							break;							
					}
					
						
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);						
						
					}					
					break;



				case WC_FJ_NUMBER_SET_FACE:
				//case MOVE_FJ_NUMBER_SET_FACE:
				case ADD_FJ_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <255)
								{							
							/*		x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120	*/

									switch(stLocalControl.byDisplayFace) 
									{
										
										case WC_FJ_NUMBER_SET_FACE:
											//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
											stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
											stLocalControl.stBusDealFreq.bySndRoomAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
											stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;
											stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
											stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//开始编号;
											Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											
											break;
										
										//case MOVE_FJ_NUMBER_SET_FACE:
											//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
											stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
											stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
											stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
											stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
											stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//开始编号;
											Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											
											break;

										case ADD_FJ_NUMBER_SET_FACE:
											//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
											stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
											stLocalControl.stBusDealFreq.bySndRoomAddr = 250;
											stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
											stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
											stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//开始编号;
											Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											
											break;											


										default:
											break;
									}

									(unOperData.ASCII_BCD)[8] += 1;
									if((unOperData.ASCII_BCD)[8]==10)
									{
										(unOperData.ASCII_BCD)[8]=0;
										(unOperData.ASCII_BCD)[7]+= 1;
										if((unOperData.ASCII_BCD)[7]==10)
										{
											(unOperData.ASCII_BCD)[7]=0;
											(unOperData.ASCII_BCD)[6]+= 1;
										}
									}
									
									//stLocalControl.byInputBox = INPUT_SECTION;
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 118;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);											

								}
							}
							break;							
					}
					
						
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);						
						
					}					
					break;


				case SELF_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_SECTION:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[2];
								(unOperData.ASCII_BCD)[2]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2] <255)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//已完成输入框颜色恢复正常
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 152;		//118+34=152
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//显示下一个输入框	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"按[确认]键",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);						
									
								}
							}
							break;
							

						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <255)
								{							
	
									stLocalControl.stEepromCfgData.bySelfSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
									stLocalControl.stEepromCfgData.bySelfRoomAddr = 253;
									stLocalControl.stEepromCfgData.bySelfBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
									SaveParameter(IAP0_ADDR);
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
									stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
									stLocalControl.stBusDealFreq.byCmd = CMD_SELF_SET_NUMBER;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
										
									ShowNumberSetFace(DIS_FRAM_MEM1);
								}
							}
							break;							
					}
					
						
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);						
						
					}					
					break;
					
				case CALCULATE_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
				 	{
				 		case KEY_0:
						case KEY_1:
				 		case KEY_2:
						case KEY_3:	
				 		case KEY_4:
						case KEY_5:
				 		case KEY_6:
						case KEY_7:
				 		case KEY_8:
						case KEY_9:	
							if((stLocalControl.byKeyInTimes==0)&& (stLocalControl.stBusDealFreq.bySndSecAddr==KEY_0))
								break;
							if(stLocalControl.byKeyInTimes==0) 
							{
								DisplayBlock(100,104,380,127,WHITE,DIS_FRAM_MEM1);
								bSignflag = 0;	
							}
							(unOperData.ASCII_BCD)[stLocalControl.byKeyInTimes]= stLocalControl.stBusDealFreq.bySndSecAddr;
							stLocalControl.byKeyInTimes++;
							if(stLocalControl.byKeyInTimes==6)
							{//最多只能输入5位数
								DisplayBlock(304,104,380,127,WHITE,DIS_FRAM_MEM1);
								stLocalControl.byKeyInTimes=0;
								if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_0)	break;
								(unOperData.ASCII_BCD)[0] = stLocalControl.stBusDealFreq.bySndSecAddr;
								stLocalControl.byKeyInTimes=1;
							}
							x=stLocalControl.uiCursorX-(stLocalControl.byKeyInTimes-1)*12;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,WHITE,2,DIS_FRAM_MEM1);
							break;

						case KEY_ADDBED:	// +
						case KEY_SEC:		// - 
						case KEY_BROAD:		// X
						case KEY_MUSIC:		//  /
							switch(stLocalControl.byKeyInTimes)
							{
								case 5:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*10000+
																 (unOperData.ASCII_BCD)[1]*1000+ 
																 (unOperData.ASCII_BCD)[2]*100+
																 (unOperData.ASCII_BCD)[3]*10+ 
																 (unOperData.ASCII_BCD)[4];
									break;
								case 4:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*1000+
																 (unOperData.ASCII_BCD)[1]*100+ 
																 (unOperData.ASCII_BCD)[2]*10+
																 (unOperData.ASCII_BCD)[3];
									break;
								case 3:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*100+
																 (unOperData.ASCII_BCD)[1]*10+ 
																 (unOperData.ASCII_BCD)[2];
									break;
								case 2:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*10+
																 (unOperData.ASCII_BCD)[1]; 
									break;	
								case 1:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]; 
									break;	
							
								default:
									break;
							}

							if(bSignflag)
							{//是一个负数
								(unOperData.uiOperData)[3] = (unOperData.uiOperData)[3] ^ 0xffff;
								(unOperData.uiOperData)[3] = (unOperData.uiOperData)[3]+1;
								(unOperData.uiOperData)[3] = (unOperData.uiOperData)[3] | 0x8000;
							}
//							Send_Data(&((unOperData.uiOperData)[3]),2);

							if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ADDBED)
							{
								DisplayASCII(120,104,'+',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = ADD_OPER;
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_SEC)
							{
								DisplayASCII(120,104,'-',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = SUB_OPER;
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_BROAD)
							{
								DisplayASCII(120,104,'*',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = MUL_OPER;
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_MUSIC)
							{
								DisplayASCII(120,104,'/',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = DIV_OPER;
							}							
							stLocalControl.byKeyInTimes = 0;
							bSignflag = 0;
							return;

						
						default:
							break;
					 }
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
						return;
					}

					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_MR)		//预留作退格键用
					{
						DisplayBlock(100,104,380,127,WHITE,DIS_FRAM_MEM1);	//先清空
						if(stLocalControl.byKeyInTimes)						
						{
							stLocalControl.byKeyInTimes--;						
							x=stLocalControl.uiCursorX-(stLocalControl.byKeyInTimes-1)*12;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,WHITE,2,DIS_FRAM_MEM1);							
						}
						else bSignflag = 0;
						return;
					}


					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_SEARCH)		//计算器用+/-键
					{
						if(stLocalControl.byKeyInTimes)
						{
							if(bSignflag)
							{//1--原为负号,再按一次,则为正号
								DisplayBlock(292,104,303,127,WHITE,DIS_FRAM_MEM1);	//清空
								bSignflag = 0;
							}
							else
							{
								DisplayASCII(292,104,'-',BLACK,WHITE,2,DIS_FRAM_MEM1);
								bSignflag = 1;
							}
						}
						return;
					}					
					
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CALCULATE)
					{
						switch(stLocalControl.byKeyInTimes)
						{
							case 5:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*10000+
									                    	 (unOperData.ASCII_BCD)[1]*1000+ 
									                    	 (unOperData.ASCII_BCD)[2]*100+
									                    	 (unOperData.ASCII_BCD)[3]*10+ 
									                    	 (unOperData.ASCII_BCD)[4];
								break;
							case 4:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*1000+
									                    	 (unOperData.ASCII_BCD)[1]*100+ 
									                    	 (unOperData.ASCII_BCD)[2]*10+
									                    	 (unOperData.ASCII_BCD)[3];
								break;
							case 3:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*100+
									                    	 (unOperData.ASCII_BCD)[1]*10+ 
									                    	 (unOperData.ASCII_BCD)[2];
								break;
							case 2:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*10+
									                    	 (unOperData.ASCII_BCD)[1];	
								break;	
							case 1:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0];	
								break;	

							default:
								break;
						}
						
						if(bSignflag)
						{//是一个负数
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4] ^ 0xffff;	//反码
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]+1;			//补码
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4] | 0x8000;
							bSignflag = 0;
						}

//						Send_Data(&((unOperData.uiOperData)[4]),2);
						DisplayASCII(120,104,'=',BLACK,WHITE,2,DIS_FRAM_MEM1);

						switch(stLocalControl.byOperType)
						{
							case ADD_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]+(unOperData.uiOperData)[4];
								break;
							case SUB_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]-(unOperData.uiOperData)[4];
								break;
							case MUL_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]*(unOperData.uiOperData)[4];	
								break;	
							case DIV_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]/(unOperData.uiOperData)[4];	
								break;								
						}
						(unOperData.uiOperData)[3] =(unOperData.uiOperData)[0];	//保留在(unOperData.uiOperData)[3]中,作为下次运算的源操作数
						(unOperData.uiOperData)[4] =(unOperData.uiOperData)[0];
						if( (unOperData.uiOperData)[4] & 0x8000 )
						{
							DisplayASCII(292,104,'-',BLUE,WHITE,2,DIS_FRAM_MEM1);
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]&0x7fff ;
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]-1;
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]^0x7fff;
							bSignflag = 0;	
						}
						//else bSignflag =0;	//结果是正数

//						Send_Data(&((unOperData.uiOperData)[4]),2);	
						
						(unOperData.ASCII_BCD)[0]  = (unOperData.uiOperData)[4]/10000;
						(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]%10000;
						(unOperData.ASCII_BCD)[1]  = (unOperData.uiOperData)[4]/1000;
						(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]%1000;
						(unOperData.ASCII_BCD)[2]  = (unOperData.uiOperData)[4]/100;
						(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]%100;
						(unOperData.ASCII_BCD)[3]  = (unOperData.uiOperData)[4]/10;
						(unOperData.ASCII_BCD)[4] = (unOperData.uiOperData)[4]%10;
						
						DisplayBlock(304,104,380,127,WHITE,DIS_FRAM_MEM1);	//先清空
						stLocalControl.byKeyInTimes = 0;
						stLocalControl.byOperType = 0;


						if((unOperData.ASCII_BCD)[0])
						{
							x=316;y=104;
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),5,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else if((unOperData.ASCII_BCD)[1])
						{
							x=328;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[1],4,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else if((unOperData.ASCII_BCD)[2])
						{
							x=340;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[2],3,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else if((unOperData.ASCII_BCD)[3])
						{
							x=352;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[3],2,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else 
						{
							x=364;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[4],1,BLUE,WHITE,2,DIS_FRAM_MEM1);							
						}
						return;
					}					
					 break;
					 
/*				case TOUCH_ADJUST_FACE:
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
						return;
					}
					break;*/
				
				case SEC_INFO_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
				 	{
				 		case KEY_0:
						case KEY_1:
				 		case KEY_2:
						case KEY_3:	
				 		case KEY_4:
						case KEY_5:
				 		case KEY_6:
						case KEY_7:
				 		case KEY_8:
						case KEY_9:	
							if(stLocalControl.byKeyInTimes==0)
							{
								DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
							}
							DisplayDigit(stLocalControl.byKeyInTimes*16,240,stLocalControl.stBusDealFreq.bySndSecAddr,WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
						case KEY_MODE:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'M',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;

						case KEY_ROOM:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'R',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;

						case KEY_SEC:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'S',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
						case KEY_ADDBED:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'B',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
							
						case KEY_CLEAR:
							DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
							stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
							
						case KEY_ENTER:
							if(stLocalControl.byKeyInTimes == 0)
							{
								ShowMainMenuFace(DIS_FRAM_MEM1);
								memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
								memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
								stLocalControl.stBusDealFreq.byCmd = CMD_MAIN_MENU_SET;
								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							}
							break;
													
						default:
							stLocalControl.byKeyInTimes=0;
							DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
							if(stLocalControl.byDisplayFace == SEC_INFO_FACE)
								Set_VisualPage(DIS_FRAM_MEM0);
							break;
					 	}						
					 break;
				
				}
			 break;
		case CMD_INCREACE_VOL:
		case CMD_DECREACE_VOL:			
			x=16;y=240;
			if(INFO_INDICATION_FACE == stLocalControl.byDisplayFace)
			{//信息指示界面
				DisplayChineseString(&x,&y,"音量",2,WHITE,BLUE,3,DIS_FRAM_MEM2);
				DisplayASCIIString(&x,&y,":",1,WHITE,BLUE,3,DIS_FRAM_MEM2);
				DisplayDigitString(&x,&y,&(stLocalControl.stBusDealFreq.byRecSecAddr),1,WHITE,BLUE,3,DIS_FRAM_MEM2);
				Set_VisualPage(DIS_FRAM_MEM2);					
			}
			else
			{
				DisplayChineseString(&x,&y,"音量",2,WHITE,BLUE,3,DIS_FRAM_MEM1);
				DisplayASCIIString(&x,&y,":",1,WHITE,BLUE,3,DIS_FRAM_MEM1);
				DisplayDigitString(&x,&y,&(stLocalControl.stBusDealFreq.byRecSecAddr),1,WHITE,BLUE,3,DIS_FRAM_MEM1);
				Set_VisualPage(DIS_FRAM_MEM1);
			}
			break;
			
		case CMD_SET_NUMBER:
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x01)
			{		
				ShowStartNumSet(DIS_FRAM_MEM1);
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x02)
			{
				ShowNumSetOk(DIS_FRAM_MEM1);	
				stLocalControl.byDispNumSetOkTime=50;		//显示一秒
				//保存地址
				stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
				SaveParameter(IAP0_ADDR);			
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x00)	//编号超时
			{
				Set_VisualPage(DIS_FRAM_MEM0);
				stLocalControl.byDisplayFace=SEC_INFO_FACE;
				break;
			}
			break;

		case CMD_SUPPLY_OX_START:
			WardDataBuff[16]=Read(WARD_FACE_DATA_START_ADDR+16);
			WardDataBuff[16]++;
			AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);

			x=312;y=175 /*209*/;
			DisplayBlock(312,175/*209*/,312+35,175/*209*/+23,BLUE,DIS_FRAM_MEM0);
			(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
			
			(unOperData.ASCII_BCD)[1]=(WardDataBuff[16]%100)/10;	
			(unOperData.ASCII_BCD)[2]=(WardDataBuff[16]%100)%10;
			
			if((unOperData.ASCII_BCD)[0])	//百位不为0
			{
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}
			else
			{
				if((unOperData.ASCII_BCD)[1])	//十位不为0
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,DIS_FRAM_MEM0);
				else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}	
			break;

		case CMD_SUPPLY_OX_END:
			WardDataBuff[16]=Read(WARD_FACE_DATA_START_ADDR+16);
			WardDataBuff[16]--;
			AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);

			x=312;y=175/*209*/;
			DisplayBlock(312,175/*209*/,312+35,175/*209*/+23,BLUE,DIS_FRAM_MEM0);
			(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
			
			(unOperData.ASCII_BCD)[1]=(WardDataBuff[16]%100)/10;	
			(unOperData.ASCII_BCD)[2]=(WardDataBuff[16]%100)%10;
			
			if((unOperData.ASCII_BCD)[0])	//百位不为0
			{
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}
			else
			{
				if((unOperData.ASCII_BCD)[1])	//十位不为0
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,DIS_FRAM_MEM0);
				else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}	
			break;	

		case CMD_VOICE_TIMER_GET:
			memset((unOperData.ASCII_BCD),0x00,10);
			unOperData.ASCII_BCD[5] =stLocalControl.stBusDealFreq.bySndSecAddr;
			unOperData.ASCII_BCD[6] =stLocalControl.stBusDealFreq.bySndRoomAddr;
			unOperData.ASCII_BCD[7] =stLocalControl.stBusDealFreq.byRecSecAddr;
			unOperData.ASCII_BCD[8] =stLocalControl.stBusDealFreq.byRecRoomAddr;
			unOperData.ASCII_BCD[9] =stLocalControl.stBusDealFreq.byRecBedAddr;
			stLocalControl.byInputBox = INPUT_VOICE_START_HOUR;
			stLocalControl.uiCursorX = 196; 	//64+12*11=196
			stLocalControl.uiCursorY = 51;		//(272-32*7-14)/2+34=51
			ShowVoiceTimerSet(DIS_FRAM_MEM1);	//音量时间段设置  在1页显示 
			break;
		case CMD_BL_TIMER_GET:
			memset((unOperData.ASCII_BCD),0x00,10);
			unOperData.ASCII_BCD[5] =stLocalControl.stBusDealFreq.bySndSecAddr;
			unOperData.ASCII_BCD[6] =stLocalControl.stBusDealFreq.bySndRoomAddr;
			unOperData.ASCII_BCD[7] =stLocalControl.stBusDealFreq.byRecSecAddr;
			unOperData.ASCII_BCD[8] =stLocalControl.stBusDealFreq.byRecRoomAddr;		
			stLocalControl.byInputBox = INPUT_BL_START_HOUR;
			stLocalControl.uiCursorX = 196;		//64+12*11=196
			stLocalControl.uiCursorY = 51;		//(272-32*7-14)/2+34=51			
			ShowBackLightTimerSet(DIS_FRAM_MEM1);	//背光时间段设置  在1页显示			
			break;


/*		case CMD_MAX_VOL_GET:
			memset((unOperData.ASCII_BCD),0x00,10);
			unOperData.ASCII_BCD[5] =stLocalControl.stBusDealFreq.bySndSecAddr;		//语音报号最大音量
			unOperData.ASCII_BCD[6] =stLocalControl.stBusDealFreq.bySndRoomAddr;	//预留 : 语音报号最小音量
			unOperData.ASCII_BCD[7] =stLocalControl.stBusDealFreq.byRecSecAddr;		//预留 : 听筒通话时最大音量
			unOperData.ASCII_BCD[8] =stLocalControl.stBusDealFreq.byRecRoomAddr;	//预留 : 听筒通话时最小音量	
			stLocalControl.byInputBox = INPUT_PLAY_VOL_MAX;			
			stLocalControl.uiCursorX = 272;
			stLocalControl.uiCursorY = 60;
			ShowMaxVolSetFace(DIS_FRAM_MEM1);	//显示最大音量设置界面
			break;
*/							

		case CMD_WARD_SEC_INFO_SET:
			switch(stLocalControl.stBusDealFreq.byRecBedAddr)
			{//帧号标志
				case 1:
					memcpy(&(WardDataBuff[6]),&(stLocalControl.stBusDealFreq.bySndSecAddr),0x03);
					WardDataBuff[9]	=	stLocalControl.stBusDealFreq.byRecSecAddr;
					WardDataBuff[10]=	stLocalControl.stBusDealFreq.byRecRoomAddr;
//					AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);
//					StoreSecInfo(DIS_FRAM_MEM0);
//					if(stLocalControl.byDisplayFace == SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);
					break;
				case 2:
					memcpy(&(WardDataBuff[11]),&(stLocalControl.stBusDealFreq.bySndSecAddr),0x03);
					WardDataBuff[14]	=	stLocalControl.stBusDealFreq.byRecSecAddr;
					WardDataBuff[15]=	stLocalControl.stBusDealFreq.byRecRoomAddr;
//					AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);
//					StoreSecInfo(DIS_FRAM_MEM0);
//					if(stLocalControl.byDisplayFace == SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);					
					break;
					
				case 3:
					WardDataBuff[16]	=	stLocalControl.stBusDealFreq.bySndSecAddr;
					AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);
					StoreSecInfo(DIS_FRAM_MEM0);
					if(stLocalControl.byDisplayFace == SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);					
					break;
					
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
			
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr	= stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
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
	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时
/*				CCAPM0=0X48;	//禁止调度
				CCF0=0;
				MCUMasterSPI();	//在此反复进行主从模式切换测试,成功
				FRAM_TEST();	
				EPH1660MasterSPI();
				CCAPM0=0X49;	//开启调度*/
				MakeCH0TimerOut(150, 0);				
			}			
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
		WDT_CONTR = 0x3d; 
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

	uint16 xdata x;
	uint16 xdata y;
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
	if(--stLocalControl.bySecondFlashTime==0)
	{
		stLocalControl.bySecondFlashTime=25;
		
		bSecondDisState=!bSecondDisState;
		if(stLocalControl.byDisplayFace==SEC_INFO_FACE)
		{
			if(bSecondDisState)
			{
				Display_ASCII(336,0,ColonCode16x32,WHITE,BLUE,3,DIS_FRAM_MEM0);	//":"				
			}
			else
			{
				DisplayBlock(336,0,336+15,31,BLUE,DIS_FRAM_MEM0);
			}
			Set_VisualPage(DIS_FRAM_MEM0);
		}
		else if(stLocalControl.byDisplayFace == SEC_INFO_SET_FACE)
		{
			(unOperData.ASCII_BCD)[9]= WardDataBuff[stLocalControl.byInputBox+6];	//获取要闪烁的数据
			x = stLocalControl.uiCursorX;
			y = stLocalControl.uiCursorY;
			if(bSecondDisState)
			{	
				(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[9]/100;
				(unOperData.ASCII_BCD)[1]=((unOperData.ASCII_BCD)[9]%100)/10;	
				(unOperData.ASCII_BCD)[2]=((unOperData.ASCII_BCD)[9]%100)%10;
				
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);
				
			}
			else
			{
				DisplayBlock(x,y,x+35,y+23,BLUE,DIS_FRAM_MEM1);
			}

		}
	}
	
	if(stLocalControl.byDispNumSetOkTime)
	{
		if(--stLocalControl.byDispNumSetOkTime==0)
		{
			Set_VisualPage(DIS_FRAM_MEM0);
			stLocalControl.byDisplayFace = SEC_INFO_FACE;
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
		WDT_CONTR = 0x3d; 
		OSWait(K_TMO, 4);	 								//设置20ms超时
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
  	  
