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


bit FLED;

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

uint16	xdata uitTimer0TimerCount=TIMER0_COUNT; 

void led0flash(void)
{
	if(FLED==FLAG_LED_ON)
	{
		LED=LED_OFF;
		FLED=FLAG_LED_OFF;
	}
	else
	{
 		LED=LED_ON;
		FLED=FLAG_LED_ON;
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
	//STC12C54/56系列0=0,M1=0:准双向，M0=0,M1=1:推挽，M0=1,M1=0:高阻，M0=1,M1=1:开漏
	//总线0发送脚设置为推挽输出，接收脚设置为高阻输入:


	//STC12C52/5A系列:M0=0,M1=0:准双向，M0=0,M1=1:高阻，M0=1,M1=0:推挽，M0=1,M1=1:开漏
	//总线0接收脚设置为高阻输入:发送脚设置为推挽输出，

	
	//P3^3,高阻输入
	P3M0 &= (~Bin(0,0,0,0,1,0,0,0));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);

	//,P3^4,开漏
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);

	//总线电平设置
	Bus0SendPin = 1;
	Bus0RecPin = 1;	 
	
	

 	TMOD &= 0xf0;   //  ;t1作波特率发生器(不变)，
	TMOD |= 0X01;	//t0作方式1（16位)定时器

	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
//	TL0 = (uint8)uitTimer0TimerCount;
//	TH0 = (uint8)(uitTimer0TimerCount>>8);
	
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
			stLocalControl.stEepromCfgData.bySelfRoomAddr = 0x02;
			stLocalControl.stEepromCfgData.bySelfBedAddr = 0x03;
			
			stLocalControl.stEepromCfgData.bySerialNum1= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum2= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum3= 0xff;
			stLocalControl.stEepromCfgData.uiRs485Brt  = 57600; 	//默认波特率57600
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
	uint8 x,y,temp1,temp2;
	//取出收到的数据帧 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	OS_EXIT_CRITICAL();	


	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//如果是登记状态,收到的命令不是登记确认命令,不作处理
		return;
	}
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//确认登记命令				
			if(bLanding)
			{	
				bLanding=0;
				MakeCH0TimerOut(0, 0); 
				if( (stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.bySndSecAddr)||
					(stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.bySndRoomAddr)||
					(stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.bySndBedAddr))
				{//已更换手柄,需清空手柄信息
					Byte_Write(DIET_LENGTH_ADDR,0x00);
					Byte_Write1(DIET_LENGTH_ADDR+1,0x00);
					stLocalControl.uiDietDataLen =0x00;
					
					Byte_Write(PRESCRIPTION_LENGTH_ADDR,0x00);
					Byte_Write1(PRESCRIPTION_LENGTH_ADDR+1,0x00);
					stLocalControl.uiPrescriptionDataLen =0x00;
					
					Byte_Write(PROMPT_LENGTH_ADDR,0x00);
					Byte_Write1(PROMPT_LENGTH_ADDR+1,0x00);
					stLocalControl.uiPromptDataLen = 0x00;

					stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
					stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
					stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
					SaveParameter(IAP0_ADDR);					
				}
				
				if( (stLocalControl.stBusDealFreq.byRecSecAddr&0x01) == 0x01)
				{//bit0==1:表示病人出院
					stLocalControl.byPatientDischarge =1;
					BL_SW=0;	//关背光 
				}
				else
				{//此床位有病人
					stLocalControl.byPatientDischarge =0;
				}
				

				if( (stLocalControl.stBusDealFreq.byRecSecAddr&0x02) == 0x02)
				{//bit1==1:表示显示屏背关打开
					bBL_SW_State=1;
					if(stLocalControl.byPatientDischarge ==0)
					{
						BL_SW=1;	//背光打开
					}
					else BL_SW=0;
				}
				else 
				{
					bBL_SW_State=BL_SW=0;	//背光关闭
				}
				ShowLandingFace();
			}			
			break;
		case CMD_INFUSION_ENTER:								//输液呼叫确认命令
		case CMD_SERVICE_ENTER: 								//服务呼叫确认命令
		case CMD_HELP_ENTER:									//求援呼叫确认命令
		case CMD_EMERGENCY_ENTER:								//紧急呼叫确认命令
			 SetLedState(LED_ON);
			 ShowCallSuccess(); 
			 stLocalControl.uiLcdDisplayOnTime = 250;	//如果黑时亮5S
			 BL_SW=1;			 
			 break;
		case CMD_INFUSION_CLEAR:								 //输液呼叫清除命令
		case CMD_SERVICE_CLEAR:								 //服务呼叫清除命令
		case CMD_HELP_CLEAR:									 //求援呼叫清除命令
		case CMD_EMERGENCY_CLEAR:								 //紧急呼叫清除命令
			  SetLedState(LED_OFF);
			  ShowLandingFace();			 
			  break;
		case CMD_TALK_CLEAR:
			  MicCtrlState=MIC_CTRL=0;
			  SetLedState(LED_OFF);
			  ShowLandingFace();			 
			  break;
		case CMD_CALL_LISTEN:
		case CMD_CHANNEL_CHANGE:
			  SetLedState(LED_ON);
			  MicCtrlState=MIC_CTRL=1;
			  ShowTalkFace();			 
			  break;
		case CMD_CHANNEL_CLOSE:
		case CMD_STOP_INDICATION:
		case CMD_SYSTERM_RESET:
			  MicCtrlState=MIC_CTRL=0;
			  SetLedState(LED_OFF);
			  ShowLandingFace();			 
			  break;
		case CMD_DATE_SEND:
			  stLocalControl.stTime.bySecond=stLocalControl.stBusDealFreq.bySndSecAddr;
			  stLocalControl.stTime.byMinute=stLocalControl.stBusDealFreq.bySndRoomAddr;
			  stLocalControl.stTime.byHour	=stLocalControl.stBusDealFreq.bySndBedAddr;
			  stLocalControl.stTime.byDay=stLocalControl.stBusDealFreq.byRecSecAddr;
			  stLocalControl.stTime.byMonth=stLocalControl.stBusDealFreq.byRecRoomAddr;
			  stLocalControl.stTime.byYear=stLocalControl.stBusDealFreq.byRecBedAddr;
			  if(stLocalControl.byDisplayFace==LAND_FACE)
			  {
				x=0;y=32;
				temp1=stLocalControl.stTime.byYear;	//BCD码
				temp2=temp1%16; //个位
				temp1=temp1/16; //十位
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayASCIIString(&x,&y,"-",1,BLACK,CYAN,3);	

				x+=16;
				temp1=stLocalControl.stTime.byMonth;	//BCD码
				temp2=temp1%16; //个位
				temp1=temp1/16; //十位
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayASCIIString(&x,&y,"-",1,BLACK,CYAN,3);

				x+=16;
				temp1=stLocalControl.stTime.byDay;	//BCD码
				temp2=temp1%16; //个位
				temp1=temp1/16; //十位
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);

				//DisplayBlock(0,64,23,95,CYAN_CODE);
				x=24;y=64;
				temp1=stLocalControl.stTime.byHour; //BCD码
				temp2=temp1%16; //个位
				temp1=temp1/16; //十位
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayASCIIString(&x,&y,":",1,BLACK,CYAN,3);	

				x+=16;
				temp1=stLocalControl.stTime.byMinute; //BCD码
				temp2=temp1%16; //个位
				temp1=temp1/16; //十位
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayBlock(x,y,127,95,CYAN_CODE);			  	
			  }
			  break;
			 
		case CMD_OPEN_485BUS_IN:
//			if(stLocalControl.stBusDealFreq.byRecSecAddr==CMD_SB_LCD_DISPLAY_DATA)
			if((stLocalControl.stBusDealFreq.byRecSecAddr==DIET_TYPE) ||
				(stLocalControl.stBusDealFreq.byRecSecAddr==PRESCRIPTION_TYPE) ||
				(stLocalControl.stBusDealFreq.byRecSecAddr==PROMPT_INFO_TYPE) )
			{//是发送显示数据到手柄 ,关闭MIC
				MIC_CTRL=0;				
			}
			//包含显示屏数据
			stLocalControl.byPatientDischarge = 0;
			BL_SW=1;	//打开显示屏背光
			stLocalControl.uiLcdDisplayOnTime= 500;	//亮10秒钟
			break;

		case CMD_BUS_ANSWER:
			if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_DISPLAY_DATA)
			{//显示屏的回应答信号
				stLocalControl.byPatientDischarge = 0;
				BL_SW=1;	//打开显示屏背光
				stLocalControl.uiLcdDisplayOnTime= 500;	//亮10秒钟			
			}
			break;
			
		case CMD_CLOSE_485BUS:
			if(stLocalControl.stBusDealFreq.bySndSecAddr==CMD_SB_LCD_DISPLAY_DATA)
			{//恢复MIC原来的状态
				MIC_CTRL=MicCtrlState;
			}
			break;			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{//是自己广播
			  MicCtrlState=MIC_CTRL=1;
			  ShowBroadFace();			 
			}
			  break;
		case CMD_OPEN_LCD:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				bBL_SW_State=BL_SW=1;
			}
			else if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//广播打开
				bBL_SW_State =1;
				if(stLocalControl.byPatientDischarge ==0 )
				{//病人还没出院
					BL_SW = 1;
				}
			}
			break;
			
		case CMD_CLOSE_LCD:
			bBL_SW_State = BL_SW =0;
			break;
			
		case CMD_CLEAR_LCD:
			stLocalControl.byPatientDischarge = 1;	//病人出院
			BL_SW=0;	//关闭显示屏背光
			//清床头分机显示屏时,也将清手柄内消息
			Byte_Write(DIET_LENGTH_ADDR,0x00);
			Byte_Write1(DIET_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PRESCRIPTION_LENGTH_ADDR,0x00);
			Byte_Write1(PRESCRIPTION_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PROMPT_LENGTH_ADDR,0x00);
			Byte_Write1(PROMPT_LENGTH_ADDR+1,0x00);
			
			stLocalControl.uiDietDataLen =0x00;
			stLocalControl.uiPrescriptionDataLen =0x00;
			stLocalControl.uiPromptDataLen =0x00;			
			break;

		case CMD_CLEAR_HANDLE_LCD:	//清空手柄各数据
			Byte_Write(DIET_LENGTH_ADDR,0x00);
			Byte_Write1(DIET_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PRESCRIPTION_LENGTH_ADDR,0x00);
			Byte_Write1(PRESCRIPTION_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PROMPT_LENGTH_ADDR,0x00);
			Byte_Write1(PROMPT_LENGTH_ADDR+1,0x00);

			stLocalControl.uiDietDataLen =0x00;
			stLocalControl.uiPrescriptionDataLen =0x00;
			stLocalControl.uiPromptDataLen =0x00;
			break;
			
		case CMD_NURSE_IN:
			if((bBL_SW_State==0)||(stLocalControl.byPatientDischarge == 1))	//黑屏的情况下
			{
				BL_SW=1;	//打开显示屏
			}
			break;
		case CMD_NURSE_OUT:
			if((bBL_SW_State==0)||(stLocalControl.byPatientDischarge == 1))	//黑屏的情况下
			{
				BL_SW=0;	//关闭显示屏
			}
			break;	
		case CMD_NUMBER_SET:
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x02)		//成功	
			{
				stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
				SaveParameter(IAP0_ADDR);
				ShowLandingFace();
			}
			break;
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			SaveParameter(IAP0_ADDR);
			break;
			
		case CMD_BUS1_TEST:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS1_TEST_ANSWER;
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
			
		case CMD_BUS1_CHECK:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS1_CHECK;
			stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
			SaveParameter(IAP0_ADDR);
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
//	led0flash();

	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时
				MakeCH0TimerOut(150, 0);				
			}			
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
uint8 xdata x,y;

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
	//灯状态控制
	if(--stLocalControl.byLedFlashTime == 0x00)
	{
		stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;
		if((stLocalControl.byLedState & 0x0f) == LED_FLASH)
		{ 	//灯闪烁状态
			FLED = !FLED;
			LED = FLED;
		}	
	}

	if(--stLocalControl.bySecondFlashTime==0)
	{
		stLocalControl.bySecondFlashTime=25;
		if(stLocalControl.byDisplayFace==LAND_FACE)
		{
			x=56;y=64;
			bSecondDisState=!bSecondDisState;
			if(bSecondDisState)
			{
				Display_ASCII(56,64,ColonCode16x32,BLACK,CYAN,3);	//":"				
			}
			else
			{
				DisplayBlock(56,64,56+15,64+31,CYAN_CODE);
			}
			
		}
	}
	if(stLocalControl.uiLcdDisplayOnTime)
	{
		if(--stLocalControl.uiLcdDisplayOnTime==0)	//LCD在显示常关的情况下显示时间已到
		{
			if((bBL_SW_State==0)||(stLocalControl.byPatientDischarge==1))
			{//此床没病人或者在关背光时段
				BL_SW =0;	//关显示屏
			}
		}
	}

	if(stLocalControl.uiLcdDisplayInfoTime)
	{
		if(--stLocalControl.uiLcdDisplayInfoTime==0)
		{
			CurrentPage =0;
			stLocalControl.uiLcdDisplayInfoTime =0;		
			ShowLandingFace();
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
	  	  
