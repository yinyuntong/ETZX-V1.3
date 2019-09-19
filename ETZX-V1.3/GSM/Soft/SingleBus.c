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
sbit  Bus0SendPin	= P3^7;									//单总线0发送引脚定义




extern unsigned char code ParameterData[];

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
	//P3^2,准双向,P3^7,推挽
	P3M0 &= (~Bin(0,0,0,0,0,1,0,0));
	P3M1 &= (~Bin(0,0,0,0,0,1,0,0));
	P3M0 &= (~Bin(1,0,0,0,0,0,0,0));
	P3M1 |=   Bin(1,0,0,0,0,0,0,0);

	
	//总线电平设置
	Bus0SendPin = 0;
	Bus0RecPin = 1;	
	 
	
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
	byBus0State0 =0;
	byBus0State1 =0;

	IE0 = 0;												//清除外部中断0标志
	IT0 = 1;												//外部中断0为下降沿触发模式	 
 			
	if(Bus0RecPin)
	{	//如果总线正常,开中断		
		EX0 = 1;
		
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

	bBus0RecBit = Bus0RecPin; 								//保存总线0接收引脚状态	          
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
						Bus0SendPin = 0;					//释放总线
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
		else
		{	//不相同,发送失败								
			byBus0State0 &= ~BUS0_SEND_CON;
			byBus0RecSendCount &= 0xf0;
			bBus0SendError = 1;					
			Bus0SendPin = 0; 
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

	EA=0;
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		EA=1;
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入发送队列
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	EA=1;	
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
*函数名称			:SndAddrCompare	
*函数描述        	:比较源地址
*输入参数   		:pstBusFreq:待比较的数据帧指针
*返回值				:1:发送地址与本机相符,0:发送地址与本机不相符	
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
bit SndAddrCompare(pSTBusFreq pstBusFreq)
{
	if( (pstBusFreq->bySndSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
	{	
		return(0);
	} 
	if((pstBusFreq->bySndRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr))
	{	
		return(0);
	}
	if( (pstBusFreq->bySndBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
	{	
		return(0);
	}
	return(1); 

}

/**********************************************************
*函数名称			:SndRecAddrCompare	
*函数描述        	:比较源地址或者目的地址
*输入参数   		:pstBusFreq:待比较的数据帧指针
*返回值				:1:发送地址或接收地址与参数指向的地址数据相符,0:发送地址或接收地址与参数指向的地址数据不相符	
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
bit SndRecAddrCompare(uint8* pAddr)
{

	if(( 	(stLocalControl.stBusDealFreq.bySndSecAddr	== *pAddr)&&
			(stLocalControl.stBusDealFreq.bySndRoomAddr	== *(pAddr+1))&&
			(stLocalControl.stBusDealFreq.bySndBedAddr 	== *(pAddr+2)))||
	   	(	(stLocalControl.stBusDealFreq.byRecSecAddr 	== *pAddr)&&
			(stLocalControl.stBusDealFreq.byRecRoomAddr == *(pAddr+1))&&
			(stLocalControl.stBusDealFreq.byRecBedAddr 	== *(pAddr+2))))		
		return(1);
	else return(0);

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
	stLocalControl.stIndicationData.bySndSecAddr 	= pstBus0SendFreq->bySndSecAddr;
	stLocalControl.stIndicationData.bySndRoomAddr 	= pstBus0SendFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.bySndBedAddr 	= pstBus0SendFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCmd 			= pstBus0SendFreq->byRecSecAddr; 
//	stLocalControl.stIndicationData.byCallCmd = pstBus0SendFreq->byCmd;

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
	{ 	//通道1主动通话
		XTD = 0;
		XTA = 1;	
	} 
	else if(bChannel0Talk)
	{	//通道0主动通话状态
		XTA = 0;
		XTD = 1;		
	}	

	else if(bWaitListen)
	{	//等待接听状态
		
	}
	else if(bIndicatingOther)
	{	//正在指示其它分机
		
	}
	else
	{	//没有任何语音状态存在		
		XTA = XTD =0;	
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
	bBusy = bWaitAck = bWaitListen = 
    bChannel0Talk = 
	bSickRoomBroad = bOfficeBroad = bAllBroad = 
	bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
	bIndicatingOther=0;			
	VoiceChannelCtx();
	if(AddrCompare(&(stLocalControl.stBusDealFreq)))
	{
		//向移动电话主动发送挂断命令;
		GSMHandUpTreat();
	}
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
void SaveParameter(void)
{  
	stLocalControl.uiShortType = IAP_ADDR;	
	stLocalControl.pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 
	while(stLocalControl.byChar1--)
	{
		IapWriteByte(stLocalControl.uiShortType++, *stLocalControl.pbyData++);
	}
	DisableIAP();
}


//保存主动接听处理的电话号码
void SaveTelNum(void)
{  
	stLocalControl.uiShortType = TEL_NUM_ADDR;	  //EEPROM中起始地址	
	stLocalControl.pbyData = TelNum;				//被保存的数据
	stLocalControl.byChar1 = TEL_NUM_LEN;						//11个字节
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 
	while(stLocalControl.byChar1--)
	{
		IapWriteByte(stLocalControl.uiShortType++, *stLocalControl.pbyData++);
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
void ReadParameter(void)
{

	stLocalControl.uiShortType = IAP_ADDR;	
	stLocalControl.pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	
	EnableIAP(IAP_READ); 
	while(stLocalControl.byChar1--)
	{		
		*stLocalControl.pbyData++ = IapReadByte(stLocalControl.uiShortType++);		
	}
	DisableIAP();
	
}


void ReadTelNum(void)
{

	stLocalControl.uiShortType = TEL_NUM_ADDR;	//EEPROM中起始地址	
	stLocalControl.pbyData = TelNum;				//被保存的数据
	stLocalControl.byChar1 = TEL_NUM_LEN;						//11个字节

	
	EnableIAP(IAP_READ); 
	while(stLocalControl.byChar1--)
	{		
		*stLocalControl.pbyData++ = IapReadByte(stLocalControl.uiShortType++);		
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
	EnableIAP(IAP_READ);
	if(IapReadByte(IAP_ADDR) == ParameterData[0])
	{//数据已经初始化完毕了,读取数据参数
		ReadParameter();

	}
	else
	{	//没有初始化,初始化参数
		stLocalControl.stEepromCfgData.byInitFlag = ParameterData[0];
		stLocalControl.stEepromCfgData.bySelfSecAddr = ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr = ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr = ParameterData[3];
		
		stLocalControl.stEepromCfgData.bySerialNum1= ParameterData[4];
		stLocalControl.stEepromCfgData.bySerialNum2= ParameterData[5];
		stLocalControl.stEepromCfgData.bySerialNum3= ParameterData[6];
		stLocalControl.stEepromCfgData.byVersionHi = ParameterData[7];
		stLocalControl.stEepromCfgData.byVersionLo= ParameterData[8];
		
		stLocalControl.stEepromCfgData.byTelNum[0]	= ParameterData[9];
		stLocalControl.stEepromCfgData.byTelNum[1]	= ParameterData[10];	
		stLocalControl.stEepromCfgData.byTelNum[2]	= ParameterData[11];
		stLocalControl.stEepromCfgData.byTelNum[3]	= ParameterData[12];
		stLocalControl.stEepromCfgData.byTelNum[4]	= ParameterData[13];
		stLocalControl.stEepromCfgData.byTelNum[5]	= ParameterData[14];
		stLocalControl.stEepromCfgData.byTelNum[6]	= ParameterData[15];
		stLocalControl.stEepromCfgData.byTelNum[7]	= ParameterData[16];	
		stLocalControl.stEepromCfgData.byTelNum[8]	= ParameterData[17];
		stLocalControl.stEepromCfgData.byTelNum[9]	= ParameterData[18];
		stLocalControl.stEepromCfgData.byTelNum[10]	= ParameterData[19];
		stLocalControl.stEepromCfgData.byTelNum[11]	= ParameterData[20];
		stLocalControl.stEepromCfgData.byTelNumValidLen	= ParameterData[21];
		
		stLocalControl.stEepromCfgData.byTelFuncState	= ParameterData[22];
		stLocalControl.stEepromCfgData.byRingTime		= ParameterData[23];
		stLocalControl.stEepromCfgData.byTalkTime		= ParameterData[24];
		stLocalControl.stEepromCfgData.byBroadTime		= ParameterData[25];
		SaveParameter();
	}
}


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
	stLocalControl.stCH1TimerOut.byTimerOutSet = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOut = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
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
	uint8 i;
	//取出收到的数据帧 	 		
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	EA=1;	

//以下仅供测试用
#if DEBUG==1
    Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
#endif
/////////////////////////////////////////////////////////////////////////////////////




	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//确认登记命令				
			if(bLanding)
			{	
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//发送到本机
					SYS_STATE_LED = LED_ON;
					bLanding = 0;
				}
			}		
			break;
				
		case CMD_COMM_CALL:	 								//普通呼叫命令  -----点对点呼叫
			if(!bBusy)
			{	//通道0空闲				
				bBusy = 1;
				//设置超时5s				
				MakeCH0TimerOut(250, 0);
				
/*				if(bIndicatingOther) 
				{//如果正在指示其他分机,停止指示
					bIndicatingOther=0;
					//发送主动挂断电话
					GSMHandUpTreat();
					
					VoiceChannelCtx();
					LedControl();
				}
				//保存主动呼叫方地址
				SaveCallAddr(&(stLocalControl.stBusDealFreq));*/
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
				if((bChannel1Talk)||(!bDealKeyDown))
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
				if((bChannel1Talk)||(!bDealKeyDown))
				{	//如果通道1忙,或者本机主动通话条件不存在了,缩短超时时间					
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();
				LedControl();
			}
			break;	
			
		case CMD_BROADCAST1:								//收到病区广播命令
		case CMD_BROADCAST2:								//收到办公区广播命令
		case CMD_BROADCAST3: 								//收到全区广播命令
			if(!bBusy)
			{	//通道0空闲				
				bBusy = 1;
				//设置广播超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime); 
				
/*				//暂存命令				
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;				

				//保存主动呼叫方地址				
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示	
					bIndicatingOther = 0;	
					//发送主动挂断电话命令
					GSMHandUpTreat();
					
					LedControl();  
					VoiceChannelCtx();					
				}				
			
				if((bChannel1Talk))
				{	//通道1忙,不作处理	
					break;
				}			
				switch(stLocalControl.stBusDealFreq.byRecSecAddr)
				{	//针对具体命令,看本机是否允许相应的广播
					case CMD_BROADCAST1:
						bSickRoomBroad = 1; 
						break;
					case CMD_BROADCAST2:
						bOfficeBroad = 1; 
						break;
					case CMD_BROADCAST3:
						bAllBroad = 1;  
						break;
				}
				VoiceChannelCtx();
				LedControl();*/
			}
			break;	
		
		case CMD_INFUSION_ENTER:								//输液呼叫确认命令
		case CMD_SERVICE_ENTER:									//服务呼叫确认命令
		case CMD_HELP_ENTER:									//求援呼叫确认命令
		case CMD_EMERGENCY_ENTER:								//紧急呼叫确认命令
			//
			break; 

			
		case CMD_INFO_INDICATION:								//收到呼叫指示命令  
			if(!(bBusy ))
			{	//本机通道0处于空闲状态,可以指示其他分机的呼叫命令了
				if((bChannel1Talk))
				{	//如果通道1处于通话状态,不作处理
					return;
				}	
				
				switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
				{
					case CMD_INFUSION_CALL:	   				//输液呼叫 
					case CMD_SERVICE_CALL:					//服务呼叫 
					case CMD_HELP_CALL:						//求援呼叫
 						//呼叫移动电话
						GSMCallTreat();
						
						break;
					case CMD_EMERGENCY_CALL: 				//紧急呼叫 
					default:
						return;
				}

				SaveIndicationData(&(stLocalControl.stBusDealFreq));												
				bIndicatingOther = 1;					
				VoiceChannelCtx();
				LedControl();	
				UartSendByte(0xaa);
			}  
			break;

			
		case CMD_INFUSION_ANSWER:							//处理输液呼叫命令
		case CMD_SERVICE_ANSWER:							//处理服务呼叫命令
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令
		case CMD_HELP_ANSWER:								//处理求援呼叫命令
			if(!bBusy)
			{	//通道0不忙				
				bBusy = 1;	
				//设置通道0超时					
				MakeCH0TimerOut(250, 0);

				//保存主动呼叫方地址	
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));					
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//处理本机的呼叫命令,清相应的标志 
					//GSM模块不存在主动呼叫，然后被其它处理机处理
 			
				}
				if(bIndicatingOther)
				{	//正在指示其他分机,清除标志	 					
					bIndicatingOther = 0;	
					//发送主动挂断电话命令
					GSMHandUpTreat();
					
					VoiceChannelCtx();						
					LedControl(); 			
				} 
				
			}
			break;
		case CMD_STOP_INDICATION:							//停止指示命令
			if(bIndicatingOther)
			{
				bIndicatingOther = 0;
				//发送主动挂断电话命令
				GSMHandUpTreat();
				
				VoiceChannelCtx();
				LedControl(); 									
			}
			break;
		case CMD_INFUSION_CLEAR: 							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR: 							//清除服务呼叫命令
		case CMD_HELP_CLEAR: 								//清除求援呼叫命令
		case CMD_EMERGENCY_CLEAR: 							//清除紧急呼叫命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//清除床头分机的呼叫,清除相应的标志			
				if(bIndicatingOther &&
					(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.bySndSecAddr) &&
					(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.bySndRoomAddr) &&
					(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.bySndBedAddr) &&
					((stLocalControl.stBusDealFreq.byCmd-0x0a) == (stLocalControl.stIndicationData.byCmd & 0x1f)))
				{ 	//清除本机正在指示的呼叫信息 					
					bIndicatingOther = 0;
					//向移动电话发送主动挂断命令
					GSMHandUpTreat();
					
					VoiceChannelCtx();
					LedControl(); 													
				}
			}
			break; 			
		case CMD_SYSTERM_RESET:								//系统复位命令
			SysReset();
			break; 

		case CMD_SYSTERM_RESTART:
			bBusy = bWaitAck = bWaitListen =  
			bChannel0Talk = 
			bChannel1Talk = 
			bSickRoomBroad = bOfficeBroad = bAllBroad = 
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
			bIndicatingOther=0;		
			MakeCH0TimerOut(0, 0);
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			break;
	
		case CMD_START_VOICE:								//启动播音命令
			break;
		case CMD_MUSIC_PLAY:								//背景音乐播放
			break;	
			
		case CMD_STOP_VOICE:								//停止所有音乐播放
			break; 
		case CMD_CHANNEL_CHANGE: 							//通道切换命令
			//将通道0的状态切换到通道1上,同时清除通道0的通话状态
			bBusy = bWaitAck = bWaitListen =  
				bSickRoomBroad = bOfficeBroad = bAllBroad =  0;			
			MakeCH0TimerOut(0, 0);					 				
			if((bChannel0Talk))
			{ 				
				bChannel1Talk = bChannel0Talk;
				bChannel0Talk = 0; 				
				//设置通道1通话超时						
				MakeCH1TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
				if(bChannel1Talk&&(!bDealKeyDown))
				{	//本机主动通话条件不存在了,缩短超时时间 	 									
					MakeCH1TimerOut(5, 0);
					break; 			
				}
				VoiceChannelCtx();
				LedControl(); 
			}							
			break;
			
		case CMD_CHANNEL_CLOSE: 							//关闭通道切换命令
			//清除通道1所有状态			
			bChannel1Talk  =0;
			MakeCH1TimerOut(0, 0);	
			VoiceChannelCtx();
			LedControl();
			if(SndRecAddrCompare(&(stLocalControl.stEepromCfgData.bySelfSecAddr))||
			   SndRecAddrCompare(&(stLocalControl.stCallAddr.byRecSecAddr)))	//两路通话时，后一路通话分机主动挂通
			{
				//向移动电话主动发送挂断命令;
				GSMHandUpTreat();
			}
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
					EA=0;
					//关闭所有打开的中断
					CCAPM0 = 0x00;
					ET0 = 0;
					TR0 = 0;
					EX0 = 0;
					ISP_CONTR = 0x20; 				
				}
				
			}				
			break;

		case CMD_DATE_SEND:
			stLocalControl.stTime.bySec = BcdToHex(stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f);
			stLocalControl.stTime.byMin = BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f);
			stLocalControl.stTime.byHour = BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f);
			stLocalControl.stTime.byDay = BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f);
			stLocalControl.stTime.byMonth = BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f);
			stLocalControl.stTime.byYear = BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);
			break;			
			

	
		case CMD_SD_TAL_VOL_CHECK:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=1;
			break;
			
		case CMD_SD_TAL_VOL_CHECK_END:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			XTD=0;
			break;

		case CMD_SA_TAL_VOL_CHECK:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 1;
			break;

		case CMD_SA_TAL_VOL_CHECK_END:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			break;
			
		case CMD_GSM_FUNC_SEARCH:
			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			//命令还是原来命令
			stLocalControl.stBusDealFreq.byRecSecAddr	= stLocalControl.stEepromCfgData.byTelFuncState;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_GSM_FUNC_SET:
			stLocalControl.stEepromCfgData.byTelFuncState = stLocalControl.stBusDealFreq.byRecSecAddr;
			//SaveParameter();

			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd          = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr	= CMD_GSM_FUNC_SET;
			stLocalControl.stBusDealFreq.byRecRoomAddr	= stLocalControl.stEepromCfgData.byTelFuncState;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_GSM_NUM_DEL:
/*			TelNum[0]	= stLocalControl.stBusDealFreq.bySndSecAddr>>4;
			TelNum[1]	= stLocalControl.stBusDealFreq.bySndSecAddr&0x0f;
			TelNum[2]	= stLocalControl.stBusDealFreq.bySndRoomAddr>>4;
			TelNum[3]	= stLocalControl.stBusDealFreq.bySndRoomAddr&0x0f;	
			TelNum[4]	= stLocalControl.stBusDealFreq.bySndBedAddr>>4;
			TelNum[5]	= stLocalControl.stBusDealFreq.bySndBedAddr&0x0f;	
			TelNum[6]	= stLocalControl.stBusDealFreq.byRecSecAddr>>4;
			TelNum[7]	= stLocalControl.stBusDealFreq.byRecSecAddr&0x0f;
			TelNum[8]	= stLocalControl.stBusDealFreq.byRecRoomAddr>>4;
			TelNum[9]	= stLocalControl.stBusDealFreq.byRecRoomAddr&0x0f;	
			TelNum[10]	= stLocalControl.stBusDealFreq.byRecBedAddr>>4;
			TelNum[11]	= stLocalControl.stBusDealFreq.byRecBedAddr&0x0f;*/

			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd          = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr	= CMD_GSM_NUM_DEL;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			

//			memset(stLocalControl.stEepromCfgData.byTelNum,0x00,TEL_NUM_LEN+1);	//清零
//			stLocalControl.stEepromCfgData.byTelNumValidLen =0;
//			SaveParameter();
			break;
			
		case CMD_GSM_NUM_SEARCH:	//查询手机号码		
			for(i=0;i<TEL_NUM_LEN+1;i++)
			{
				if(stLocalControl.stEepromCfgData.byTelNum[i]== 0x0f) break;
			}

			if(i<TEL_NUM_LEN+1)
			{//找到电话号码
				switch(i)
				{
					case 0:	//没有1个数字
						return;
					case 1:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+0x0f;
						break;
					case 2:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr  =0xf0;
						break;
					case 3:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+0x0f;						
						break;
					case 4:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr   =0xf0;
						break;
					case 5:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];						
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+0x0f;
						break;
					case 6:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr   =0xf0;
						break;
					case 7:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+0x0f;				
						break;
					case 8:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];						
						stLocalControl.stBusDealFreq.byRecRoomAddr 	=0xf0;
						break;
					case 9:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];						
						stLocalControl.stBusDealFreq.byRecRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[8]<<4)+0x0f;						
						break;
					case 10:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];
						stLocalControl.stBusDealFreq.byRecRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[8]<<4)+stLocalControl.stEepromCfgData.byTelNum[9];						
						stLocalControl.stBusDealFreq.byRecBedAddr	=0xf0;
						break;
					case 11:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];
						stLocalControl.stBusDealFreq.byRecRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[8]<<4)+stLocalControl.stEepromCfgData.byTelNum[9];						
						stLocalControl.stBusDealFreq.byRecBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[10]<<4)+0x0f;						
						break;
				}
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		case CMD_GSM_NUM_SET:
			TelNum[0]	= stLocalControl.stBusDealFreq.bySndSecAddr>>4;
			TelNum[1]	= stLocalControl.stBusDealFreq.bySndSecAddr&0x0f;
			TelNum[2]	= stLocalControl.stBusDealFreq.bySndRoomAddr>>4;
			TelNum[3]	= stLocalControl.stBusDealFreq.bySndRoomAddr&0x0f;	
			TelNum[4]	= stLocalControl.stBusDealFreq.bySndBedAddr>>4;
			TelNum[5]	= stLocalControl.stBusDealFreq.bySndBedAddr&0x0f;	
			TelNum[6]	= stLocalControl.stBusDealFreq.byRecSecAddr>>4;
			TelNum[7]	= stLocalControl.stBusDealFreq.byRecSecAddr&0x0f;
			TelNum[8]	= stLocalControl.stBusDealFreq.byRecRoomAddr>>4;
			TelNum[9]	= stLocalControl.stBusDealFreq.byRecRoomAddr&0x0f;	
			TelNum[10]	= stLocalControl.stBusDealFreq.byRecBedAddr>>4;
			TelNum[11]	= stLocalControl.stBusDealFreq.byRecBedAddr&0x0f;
			for(i=0;i<TEL_NUM_LEN+1;i++)
			{
				if(TelNum[i]==0x0f) break;
			}
			if(i>=TEL_NUM_LEN+1) break;		//无效电话号码
			stLocalControl.stEepromCfgData.byTelNumValidLen	=	i;			
			memcpy(stLocalControl.stEepromCfgData.byTelNum,TelNum,stLocalControl.stEepromCfgData.byTelNumValidLen+1);

			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd          = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr	= CMD_GSM_NUM_SET;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			
//			SaveParameter();
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
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;
	EA=1;	

	//以下仅供测试用
	//调试用
#if DEBUG==1	
    Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
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

		case CMD_COMM_CALL:	 								//普通呼叫命令 							
		case CMD_INFUSION_ANSWER:							//处理输液呼叫
		case CMD_SERVICE_ANSWER:							//处理服务呼叫
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫
		case CMD_HELP_ANSWER:								//处理求援呼叫
			//停止正在指示的呼叫
			if(bIndicatingOther)
			{	
				bIndicatingOther = 0; 				
				VoiceChannelCtx();
				LedControl();							
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
			{//GSM模块不存在被别的设备主动点对点呼叫，然后GSM模块发送应答

	 					
			}
			break;
		case CMD_CALL_LISTEN:								//接听命令
			if(bBusy)
			{//GSM模块不存在主动发送此命令

			}
			break;
			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   										//广播命令
			//GSM模块不存在主动广播命令
			break;
			
		case CMD_INFUSION_CLEAR:							//清除输液呼叫
		case CMD_SERVICE_CLEAR:								//清除服务呼叫
		case CMD_HELP_CLEAR:								//清除求援呼叫
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫
			 if(bIndicatingOther &&
				(stLocalControl.stBusDealFreq.byRecSecAddr 	== stLocalControl.stIndicationData.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr 	== stLocalControl.stIndicationData.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCmd & 0x1f)))//对应的呼叫命令
			{	//清除本机正在指示的呼叫  					
				bIndicatingOther = 0;
				VoiceChannelCtx();
				LedControl();													
			} 			
			break; 	

		case CMD_CHANNEL_CLOSE:								//关闭语音通道命令
			bChannel1Talk  = 0; 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			LedControl();
			break;
			
		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 1;								//制造总线故障
			SaveParameter();
			Bus0SendPin = 0;								//释放总线				
			break;
			
		case CMD_SYSTERM_RESET:	 							//系统复位命令
			SysReset();
			break; 
			
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case CMD_POWER_ON:
					EA=0;
					//关闭所有打开的中断
					CCAPM0 = 0x00;
					ET0 = 0;
					TR0 = 0;
					EX0 = 0;
					ISP_CONTR = 0x20; 	

					
				case CMD_SD_TAL_VOL_CHECK_END:
					XTD=0;
					break;

				case CMD_GSM_FUNC_SET:
					SaveParameter();
					break;
					
				case CMD_GSM_NUM_DEL:
					memset(stLocalControl.stEepromCfgData.byTelNum,0x00,TEL_NUM_LEN+1); //清零
					stLocalControl.stEepromCfgData.byTelNumValidLen =0;
					SaveParameter();
					break;

				case CMD_GSM_NUM_SET:
					SaveParameter();
					break;

			}
			break;
			
		default:
			break;			

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
					else 
					if((bWaitAck|bWaitListen|bChannel0Talk))
					{	//等待应答，等待接听，主动通话，主动广播状态				
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;											
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

						GSMHandUpTreat();
					}								
					else
					{
						bBusy = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
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
	if(stLocalControl.stCH1TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stLocalControl.stCH1TimerOut.byTimerOut--;
		if(stLocalControl.stCH1TimerOut.byTimerOut == 0)
		{	//超时一次到了 
			if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
			{ 	//所有超时完成
				goto TIMER1OUTDEAL;
			}
			else
			{	//超时次数没有完
				stLocalControl.stCH1TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
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
						
						GSMHandUpTreat();
					}

				}
				else
				{ 	//超时次数没有完成，重新加载单位超时时间 				
					stLocalControl.stCH1TimerOut.byTimerOut = stLocalControl.stCH1TimerOut.byTimerOutSet;
				}
			}			
		}
	}	

	//灯状态控制
	if(!bLanding)
	{
		if(--stLocalControl.byLedFlashTime == 0x00)
		{
			stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;
			SYS_STATE_LED = !SYS_STATE_LED;
		}
	}
}
 	  
