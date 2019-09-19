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
sbit  Bus0SendPin	= P3^7;									//单总线0发送引脚定义
//---------------------------------------------------------


uint8 xdata BusLowDTime=100;
uint8 xdata bySec=2;
uint8 xdata byRoom=8;

bit	bLanding=0; 
bit	bCalling=0; 

bit bBus0SndBitOk=0;
bit bNumSeting =0;
//---------------------------------------------------------	 
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
	//总线0发送脚设置为推挽输出，接收脚设置为高阻输入:M0=0,M1=0:准双向，M0=0,M1=1:推挽，M0=1,M1=0:高阻，M0=1,M1=1:开漏
	P3M0 |= 0x04;
	P3M1 &= 0xFB;
	P3M0 &= 0x7F;
	P3M1 |= 0x80;	
	//总线电平设置
	Bus0SendPin = 0;
	Bus0RecPin = 1;	 
	//---------------------------------------------
	//--------------------------------------------- 
	//定时器0初始化:自动重载方式,定时93.75us     
    //TMOD |= 0x02;
	//TL0 = TIMER0_COUNT;
	//TH0 = TIMER0_COUNT;
	//---------------------------------------------
	//---------------------------------------------
	TMOD &= 0xf0;		//定时器1不变
	TMOD |= 0x01;		//定时器0模式1:16为计数
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	//---------------------------------------------
	//---------------------------------------------
	TF0 = 0;												//清除中断标志										
	AUXR &= ~T0x12;											//0:定时器0传统12分频速度  
   	ET0 = 1;												//允许定时器0中断 
	TR0 = 1;												//启动定时器
	//其它控制设置
	byBus0SendStopCount = 240;								//上电总线0禁止发送时间设置	  	
	IE0 = 0;												//清除中断标志
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
void Bus0RecInt(void) interrupt 0
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
		byBus0SendCount += 0x10;
		if(0x50 == (byBus0SendCount & 0xf0))
		{
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


//计算stLocalControl.stEepromCfgData中的数据校验和
uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);
//	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}
//	DisableIAP();
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
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
//	addr  = IAP0_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(addr); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}

/*	
	//再写每二份拷贝
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
	addr  = IAP1_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(addr); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}

	
	//再写每三份拷贝
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
	addr  = IAP2_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(addr); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}*/
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
CheckIapSec0:	
	if(ParaCrcCheck(IAP0_ADDR) ==1)
	{//第一块校验和正确	
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//数据已经初始化完毕了,读取数据参数
			ReadParameter(IAP0_ADDR);
			//SaveParameter(IAP1_ADDR);
			return;
		}
		else	goto CheckIapSec1;	
	}


CheckIapSec1:	
    if(ParaCrcCheck(IAP1_ADDR) ==1)
	{//第二块校验和正确 
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP1_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//数据已经初始化完毕了,读取数据参数
			ReadParameter(IAP1_ADDR);
			SaveParameter(IAP0_ADDR);
			return;
		}
		else	goto CheckIapSec2;	
	}


CheckIapSec2:	
	if(ParaCrcCheck(IAP2_ADDR) ==1)
	{//第三块校验和正确 
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP2_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//数据已经初始化完毕了,读取数据参数
			ReadParameter(IAP2_ADDR);
			SaveParameter(IAP0_ADDR);
			SaveParameter(IAP1_ADDR);
			return;
		}
		else	goto InitParaVal;	
	}



	//三个扇区数据都不正确，调用默认值
//	else
	{	//没有初始化,初始化参数
InitParaVal:	
		stLocalControl.stEepromCfgData.byInitFlag		=ParameterData[0];
	    stLocalControl.stEepromCfgData.bySelfSecAddr	=ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr	=ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr	=ParameterData[3];
		stLocalControl.stEepromCfgData.byFlag			=ParameterData[4];
		stLocalControl.stEepromCfgData.byPrio			=ParameterData[5];
		stLocalControl.stEepromCfgData.byReserve1		=ParameterData[6];
		stLocalControl.stEepromCfgData.byReserve2		=ParameterData[7];
		stLocalControl.stEepromCfgData.byReserve3		=ParameterData[8];
		stLocalControl.stEepromCfgData.bySerialNum1		=ParameterData[9];
		stLocalControl.stEepromCfgData.bySerialNum2		=ParameterData[10];
		stLocalControl.stEepromCfgData.bySerialNum3		=ParameterData[11];
		stLocalControl.stEepromCfgData.byVersionHi		=ParameterData[12];
		stLocalControl.stEepromCfgData.byVersionLo		=ParameterData[13];
		stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

		SaveParameter(IAP0_ADDR);
		SaveParameter(IAP1_ADDR);
		SaveParameter(IAP2_ADDR);
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
				{ 	
					bLanding = 0;
					stLocalControl.byLedTime = 50;
					if(stLocalControl.stEepromCfgData.byFlag&0x01)
					{	//设定为灯长亮
						SetLedRedState(LED_ON);	
					}
					else
					{	//设定为灯灭
						SetLedRedState(LED_OFF);	
					}				
					MakeCH0TimerOut(0, 0); 										
				}
			}			
			break;	
		case CMD_QUEST:										//查询命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_ANSWER; 			
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_EMERGENCY_ENTER:							//确认紧急呼叫命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//是确认本机的呼叫,设置灯闪烁,置正在呼叫标志	
				SetLedRedState(LED_FLASH);	
				bCalling = 1; 	
			} 	
			break; 
		case CMD_EMERGENCY_CLEAR: 							//清除紧急呼叫命令 
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令			
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//处理或者清除本机的呼叫,停止灯闪烁,清正在呼叫标志	
				if(stLocalControl.stEepromCfgData.byFlag&0x01)
				{	//设定为灯长亮
					SetLedRedState(LED_ON);	
				}
				else
				{	//设定为灯灭
					SetLedRedState(LED_OFF);	
				}
				bCalling = 0; 	
			} 			
			break; 			
		case CMD_SYSTERM_RESET:								//系统复位命令
			if(stLocalControl.stEepromCfgData.byFlag&0x01)
			{	//设定为灯长亮
				SetLedRedState(LED_ON);	
			}
			else
			{	//设定为灯灭
				SetLedRedState(LED_OFF);	
			}	
			bCalling = 0; 	
			break; 	  		
			
		case CMD_ONLINE_WRITE:								//在线写入命令
			break;
		case CMD_ONLIEN_READ: 								//在线读出命令
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(stLocalControl.stBusDealFreq.bySndSecAddr >= sizeof(STEepromCfgData))
				{	//参数不正确
					break;
				}			
				stLocalControl.uiShortType = IAP0_ADDR + stLocalControl.stBusDealFreq.bySndSecAddr;
				EnableIAP(IAP_READ);	
				stLocalControl.stBusDealFreq.bySndRoomAddr = IapReadByte(stLocalControl.uiShortType);		
				DisableIAP();
				stLocalControl.stBusDealFreq.byCmd = CMD_ONLINE_OUTPUT;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			}		
			break;
		case CMD_POWER_ON:	  								//本机重新热启动
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{			
				OS_ENTER_CRITICAL();						//关闭中断
			 	CCAPM0 = 0;
				ET0 = 0;
				TR0 = 0;
				EX0 = 0;
				ISP_CONTR = 0x20; 	
	
			} 
			break; 	
		case CMD_WRITE_SERIAL:								//写入序列号命令
			break; 
			
		case CMD_READ_SERIAL:								//读序列号命令
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySerialNum1;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySerialNum2;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySerialNum3;
			stLocalControl.stBusDealFreq.byCmd = CMD_OUT_SERIAL;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySerialNum1;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySerialNum2;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySerialNum3;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_NUMBER_SET:								//修改地址命令									
			// 开始对设备进行编号
			if(stLocalControl.stBusDealFreq.bySndBedAddr==0x00) //床号为0表示为厕所分机编号
			 {
			   bNumSeting =1;
			   byNumSetTime = 250;	//5秒
			   stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;
		       SetLedRedState(LED_FLASH);
			   bySec =  stLocalControl.stBusDealFreq.bySndSecAddr;
			   byRoom = stLocalControl.stBusDealFreq.bySndRoomAddr;
			 }
			break;

			
		case CMD_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr=stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			SaveParameter(IAP1_ADDR);
			SaveParameter(IAP2_ADDR);
			break;	
			
	    case CMD_BUS_TEST:                 //如果是总线测试命令
		    stLocalControl.stBusDealFreq.bySndSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
		    stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	        stLocalControl.stBusDealFreq.bySndBedAddr  = stLocalControl.stEepromCfgData.bySelfBedAddr;
	        stLocalControl.stBusDealFreq.byRecSecAddr  = CMD_BUS_TEST;
	        stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
	        stLocalControl.stBusDealFreq.byRecBedAddr  = 0x00; 
			stLocalControl.stBusDealFreq.byCmd = 0x02;   				
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
//	Send_Data(&(stLocalControl.stBusDealFreq),7);
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时
				MakeCH0TimerOut(150, 0);	
			}			
			break;					
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫			
			if(stLocalControl.stEepromCfgData.byFlag&0x01)
			{	//设定为灯长亮
				SetLedRedState(LED_ON);	
			}
			else
			{	//设定为灯灭
				SetLedRedState(LED_OFF);	
			}		
			bCalling = 0; 				 			
			break; 	
		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 1;								//制造总线故障
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			SaveParameter(IAP1_ADDR);
			SaveParameter(IAP2_ADDR);
			Bus0SendPin = 0;								//释放总线					
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
		WDT_CONTR = 0x3d;
	}
}

/*---------------------------------------------------------------------------
函数原型: void Delayms(uint ms)
参数说明: ms--需要延时的值
返 回 值: 无
函数功能：延时程序(对于18.432M晶振单指令周期延时1mS)
----------------------------------------------------------------------------*/
/*void Delayms(uint16 ms)	  
{
   uint16 xdata i;
   for(;ms!=0;ms--)
		for(i=900;i!=0;i--);
}*/



/**********************************************************
*函数名称			:TimerOutDeal	
*函数描述        	:超时处理函数
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:Bus0OutputData					 
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
					} 									
				}
				else
				{ 	//超时次数没有完成，重新加载单位超时时间 				
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}				
			}			
		}
	} 
	if(--stLocalControl.byLedDelay == 0x00)
	{
		stLocalControl.byLedDelay = stLocalControl.byLedTime;
		if(stLocalControl.byLedState == LED_FLASH)
		{	//如果灯为闪烁状态,交替灯的状态
			bLedRedState = !bLedRedState;
			LED_RED = LED_GREEN = bLedRedState;
		}
	}

	if(uiNurseInTime)
	{
		if(--uiNurseInTime == 0x00)	
		{	//检测护士到位的定时时间到	定为5S
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
			stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
			stLocalControl.stBusDealFreq.bySndBedAddr= MoveFjNum;//移动分机号
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
			stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_OUT;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			bNurseIn=0;
		}
	}


	if(byIrDataIntervalTimer)
	{
		if(--byIrDataIntervalTimer==0x00)	//间隔时间到
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

	if(byNumSetTime)
	{
		if(--byNumSetTime==0)				//编号时间到
		{
			bNumSeting =0;
//			stLocalControl.byLedTime = stLocalControl.byLedDelay = 0;
			SetLedRedState(LED_ON);
			
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
				RST_BUS =1;	//断开SD到总线
				Bus0SendPin =0;	//使SD为高电平
				OSWait(K_TMO,100);	//延时500ms    一个单位5ms
				
				if(Bus0RecPin ==0)
				{//本机有故障 //使灯快闪
					do
					{
						RST_BUS =1;	//断开SD到总线
						Bus0SendPin =0;	//使SD为高电平
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						SetLedRedState(LED_OFF);
						OSWait(K_TMO,40);		//延时200ms
					
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						SetLedRedState(LED_ON);
						OSWait(K_TMO,40);
						
					}while(Bus0RecPin==0);	//一直等待为高

					
					RST_BUS = 0;	//使SD线路连通					
					BusLowDTime = 100;
					//使灯恢复常亮
					SetLedRedState(LED_ON);
				}

				
				else 
				{//是外部总线引起
					RST_BUS = 0;	//使SD线路连通
					Bus0SendPin =0;	//使SD为高电平

					do
					{
							
						RST_BUS = 0;	//使SD线路连通
						Bus0SendPin =0;	//使SD为高电平
						
						SetLedRedState(LED_OFF);	//灯慢闪
						OSWait(K_TMO,120);		//延时600ms 
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						SetLedRedState(LED_ON);
						OSWait(K_TMO,120);
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						
					}while(Bus0RecPin==0);	//一直等待为高
					

					//使灯恢复常亮
Bus0BugRet:
					SetLedRedState(LED_ON);
					Bus0SendPin =0;	//使SD为高电平
					RST_BUS = 0;	//使SD线路连通					
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
	while(TRUE)
	{ 	
		OSWait(K_TMO, 4);									//延时20ms 			 		
		if(stLocalControl.stCH0TimerOut.byTimerOut)
		{	
			OSQPost(byMainCmdQ, TIMER_OUT);				
		}
		else
		{		
			OSQPost(byMainCmdQ, TIMER_OUT);	
		}
		WDT_CONTR = 0x3d;
	}
} 
