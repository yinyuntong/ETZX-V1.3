/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SingleBus.c
*文件描述    		:单总线程序
*创建人     		:陈卫国
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
extern uint8    xdata  GT23_Rec_Data[128];

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
uint8 idata byBus0TxQ[BUS0_TX_Q_ZISE];						//总线0发送队列							
uint8 data byBus0TxHead = 0;								//单总线0发送队列头指针
uint8 data byBus0TxTail = 0; 								//单总线0发送队列尾指针
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE)
//发送和接收引脚定义
sbit  Bus0RecPin	= P3^2;									//单总线1接收引脚定义
sbit  Bus0SendPin	= P4^3;									//单总线1发送引脚定义

extern STUartControl    idata stUartControl;
extern STLocalControl 	xdata stLocalControl;
uint16  xdata PageNumber=0;
uint16  xdata SerchRoomCount=0;					           //查房的时间计数器
uint16  xdata SET_BY_SERIAL_COUNT=0;					   //编号的时间计数器	 
uint8   xdata SerchRoom=0;
uint8   xdata Real_TimeBuf[5]={0x00,0x00,0x00,0x00,0x00};
uint32  xdata INSPERCTOR_Count=0;
uint16  xdata BusLow_Count=0;
uint16  xdata Self_BusLow_Count=0;
uint16	xdata Self_BusHigh_Count=0;
bit   BusLow_Flag;
bit   Self_BusLow_Flag;
bit   SerchRoomFlag;
bit   SerchRoomKeyFlag;
bit   SET_BY_SERIAL_FLAG;                                 //单总线对设备编号标志位
bit   HaveSerchRoomFlag;
bit   Page_Flag;
bit   INSPERCTOR_Flag;
bit   INDICTION_Flag;
bit   NUMBER_SET_STOP_FLAG;
bit   AllBusLow_Flag;
bit   BusLowRenewOK_Flag;
extern bit   Display_BusLow_Flag;
extern uint8 xdata  Modify_Room_Number;
extern uint8 xdata  Modify_SEC_Name;
extern bit Broadcasting_Flag;
extern bit CheckBusLow_Flag;
uint16 xdata  INDICTION_Count=0; 
//---------------------------------
//---------------------------------
//2012/5/26添加多个护士到位
uint16 xdata NurseComeCount=0;
uint8  xdata NurseComeRoom[3]={0,0,0};
//---------------------------------
//---------------------------------
extern uint8 xdata  Page_Color[5]; 
extern bit Key_SerchRoom_Flag2;
extern bit Key_SerchRoom_Flag3;
//uint16 xdata AllBusLow_Count=0;
extern uint8  xdata Fbaud_Type;
extern uint32 xdata Fbaud;   //波特率的数值


extern bit bLcdOn;
uint8 xdata BusLowDTime=100;

bit bBus0SndBitOk =0;

extern void Delayms(uint16 ms);


void ReadPageColour(void)
{
    uint16 xdata fcolor,bcolor,ccolor;
	uint8  xdata byData1,byData2,byData3;
    if(Modify_SEC_Name==0x01)
    {
      byData1=Page_Color[0];//字体颜色 
	  byData2=Page_Color[1];//字体背景色,也是清屏色
	  byData3=Page_Color[2];//中间填充色
	  if(byData1==0x00) 
	  fcolor= BLACK;
	  else if(byData1==0x01) 
	  fcolor= RED;
	  else if(byData1==0x02) 
	  fcolor= GREEN;
	  else if(byData1==0x03) 
	  fcolor= BLUE1;
	  else if(byData1==0x04) 
	  fcolor= BLUE;
	  else if(byData1==0x05) 
	  fcolor= YELLOW;
	  else if(byData1==0x06) 
	  fcolor= PURPLE;
	  else if(byData1==0x07) 
	  fcolor= CYAN;
	  else if(byData1==0x08) 
	  fcolor= WHITE;
	  else
	  fcolor=WHITE;//字体颜色
      if(byData2==0x00) 
	  bcolor= BLACK;
	  else if(byData2==0x01) 
	  bcolor= RED;
	  else if(byData2==0x02) 
	  bcolor= GREEN;
	  else if(byData2==0x03) 
	  bcolor= BLUE1;
	  else if(byData2==0x04) 
	  bcolor= BLUE;
	  else if(byData2==0x05) 
	  bcolor= YELLOW;
	  else if(byData2==0x06) 
	  bcolor= PURPLE;
	  else if(byData2==0x07) 
	  bcolor= CYAN;
	  else if(byData2==0x08) 
	  bcolor= WHITE;
	  else
	  bcolor=BLUE1; 
	  if(byData3==0x00) 
	  ccolor= BLACK;
	  else if(byData3==0x01) 
	  ccolor= RED;
	  else if(byData3==0x02) 
	  ccolor= GREEN;
	  else if(byData3==0x03) 
	  ccolor= BLUE1;
	  else if(byData3==0x04) 
	  ccolor= BLUE;
	  else if(byData3==0x05) 
	  ccolor= YELLOW;
	  else if(byData3==0x06) 
	  ccolor= PURPLE;
	  else if(byData3==0x07) 
	  ccolor= CYAN;
	  else if(byData3==0x08) 
	  ccolor= WHITE;
	  else
	  ccolor=YELLOW; 
    }
	else
	{
	  fcolor=WHITE;//字体颜色
	  bcolor=BLUE1; //字体背景色,也是清屏色
	  ccolor=YELLOW;//房号与几床到几床的中间填充颜色
	}
    Lcd_Clear(bcolor,0);
	Lcd_Fill(0,168,480,10,ccolor);
    if(Modify_SEC_Name==0x01)
    {
       byData1=Page_Color[3];//字体颜色 
	   byData2=Page_Color[4];//字体背景色,也是清屏色
	   if(byData1==0x00) 
	   fcolor= BLACK;
	   else if(byData1==0x01) 
	   fcolor= RED;
	   else if(byData1==0x02) 
	   fcolor= GREEN;
	   else if(byData1==0x03) 
	   fcolor= BLUE1;
	   else if(byData1==0x04) 
	   fcolor= BLUE;
	   else if(byData1==0x05) 
	   fcolor= YELLOW;
	   else if(byData1==0x06) 
	   fcolor= PURPLE;
	   else if(byData1==0x07) 
	   fcolor= CYAN;
	   else if(byData1==0x08) 
	   fcolor= WHITE;
	   else
	   fcolor=WHITE;//字体颜色
       if(byData2==0x00) 
	   bcolor= BLACK;
	   else if(byData2==0x01) 
	   bcolor= RED;
	   else if(byData2==0x02) 
	   bcolor= GREEN;
	   else if(byData2==0x03) 
	   bcolor= BLUE1;
	   else if(byData2==0x04) 
	   bcolor= BLUE;
	   else if(byData2==0x05) 
	   bcolor= YELLOW;
	   else if(byData2==0x06) 
	   bcolor= PURPLE;
	   else if(byData2==0x07) 
	   bcolor= CYAN;
	   else if(byData2==0x08) 
	   bcolor= WHITE;
	   else
	   bcolor=WHITE; 
    }
	else
	{
	   fcolor=BLUE1;//字体颜色
	   bcolor=WHITE;//字体背景色,也是清屏色
	}
    Lcd_Fill(0,178,480,94,bcolor);	
} 
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
	//总线0发送脚设置为推挽输出，接收脚设置为高阻输入
	P4M1 &= 0xF7;
    P4M0 |= 0x08;
	P3M1 |= 0x04;
    P3M0 &= 0xFB;
	//总线电平设置
	Bus0SendPin = 0;
	Bus0RecPin  = 1;	
	TMOD |= 0x01;
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	TF0 = 0;												//清除中断标志
	AUXR &= 0x7F;											//传统12分频速度  
   	ET0 = 1;												//允许定时器0中断 
	TR0 = 1;												//启动定时器
	//其它控制设置
	byBus0SendStopCount = 240;								//上电总线0禁止发送时间设置
	IE0 = 0;
	IT0 = 1;												//外部中断0为下降沿触发模式   			
	if(Bus0RecPin)
	{	//如果总线正常,开中断			
		EX0 = 1;
	}
	else
	{	//如果总线不正常,置总线故障标志			
		bBus0Error = 1;
	} 
	PageNumber=0;	
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
*创建人	      		:陈卫国
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
				PW = bPWState; 								//恢复语音功放的控制引脚
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
							PW = bPWState;					//恢复语音功放的控制引脚
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
		{//处于一个数据位的中间
			if(bBus0SendBit == bBus0RecBit) bBus0SndBitOk =1;
			else
			{	//不相同,发送失败								
				byBus0State0 &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				bBus0SendError = 1;					
				Bus0SendPin = 0; 
				byBus0SendStopCount = 240;

				bBus0SndBitOk = 0;
			}
		}
		
		//if(bBus0SendBit == bBus0RecBit)   	
		else if(bBus0SndBitOk ==1)
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
			PW = bPWState;
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
*创建人	      		:陈卫国
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
		EA=1;;
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入发送队列
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	EA=1;;	
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
uint8 BcdToHex(uint8 byData)
{
	uint8 byRet;
	byRet = byData >> 4;
	byRet *= 10;
	byData &= 0x0f;
	byRet += byData;
	return(byRet);
}
/**********************************************************
/**********************************************************
*函数名称			:AddrCompareCheck	
*函数描述        	:比较检测时的地址
*输入参数   		:pstBusFreq:待比较的数据帧指针
*返回值				:1:发送的是检测时数据,0:不是发送检测到的数据	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2012-3-14
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
bit AddrCompareCheck(pSTBusFreq pstBusFreq)
{
	if((pstBusFreq->byRecSecAddr==0x01)&&(pstBusFreq->byRecRoomAddr==0x02)&&(pstBusFreq->byRecBedAddr==0x03))
	{	
		return(1);
	} 
	else
    return(0);
}
/**********************************************************
/**********************************************************
*函数名称			:AddrCompare	
*函数描述        	:比较地址(含广播地址)
*输入参数   		:pstBusFreq:待比较的数据帧指针
*返回值				:1:发送到本机的数据,0:不是发送到本机的数据	
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
*函数名称			:MakeCH0TimerOut	
*函数描述        	:设置通道0超时参数
*输入参数   		:byTimerOut:超时时间,byTimerOutCount:超时次数
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
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH0TimerOut.byTimerOutSet   = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOut      = byTimerOut;
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/   
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH1TimerOut.byTimerOutSet   = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOut      = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
} 
/**********************************************************
*函数名称			:SaveIndicationData	
*函数描述        	:保存信息指示数据内容
*输入参数   		:pstBus0SendFreq:待保存的数据帧指针
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
void SaveIndicationData(pSTBusFreq pstBus0SendFreq)
{ 	
	stLocalControl.stIndicationData.stAddr.bySecAddr  = pstBus0SendFreq->bySndSecAddr;
	stLocalControl.stIndicationData.stAddr.byRoomAddr = pstBus0SendFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.stAddr.byBedAddr  = pstBus0SendFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCallCmd         = pstBus0SendFreq->byRecSecAddr; 
}
/**********************************************************
*函数名称			:SaveCallAddr	
*函数描述        	:保存主动通话方地址
*输入参数   		:pstBus0SendFreq:待保存的数据帧指针
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
void SaveCallAddr(pSTBusFreq pstBusFreq)
{ 	
	stLocalControl.stCallAddr.bySecAddr  = pstBusFreq->bySndSecAddr;
	stLocalControl.stCallAddr.byRoomAddr = pstBusFreq->bySndRoomAddr;
	stLocalControl.stCallAddr.byBedAddr  = pstBusFreq->bySndBedAddr;
}
/**********************************************************
*函数名称			:LedControl	
*函数描述        	:指示灯显示状态控制
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:SetLedSetState,SetLedDealState
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/ 
void LedControl(void)
{
	if(bNurseIn)
	{
		SetMDLedState(LED_ON<<4);         //护士到位门灯指示为绿色
	}
	else if((bConfusionNoting|bServiceNoting|bHelpNoting))
	{
		SetMDLedState(LED_ON<<2);//红色	  //有普通呼叫门灯显示为红色
	}
	else if(bEmergencyNoting)
	{ 		
		SetMDLedState(LED_FLASH<<2); //有紧急呼叫门灯显示为红色闪烁
	}
	else
	{
		SetMDLedState(0);
	}
	if(bIndicatingOther)
	{
	    SetLedDealState(LED_FLASH);	//如果有其他呼叫，门口处理灯闪烁
	}
	else if((bChannel1Talk|bChannel1Talked|bChannel0Talk|bChannel0Talked|bSelfBroad|bSickRoomBroad|bOfficeBroad|bAllBroad))
	{	
		SetLedDealState(LED_ON);	//设备处于通话状态设定处理灯长亮
	} 
	else
	{
		SetLedDealState(LED_OFF);
	} 	
}
/**********************************************************
*函数名称			:VoiceChannelCtx	
*函数描述        	:语音通道切换处理函数
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
void VoiceChannelCtx(void)
{ 	
	if(bChannel1Talked)	  //通道1为SA
	{	//通道1被动通话
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkedVol;
		CloseCGB();
		OpenCBD();
		CTD = 0;
		CTA = 1;
		bPWState =  PW = 0;			
	}
	else if(bChannel1Talk)
	{ 	//通道1主动通话
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkVol;
		CloseCGB();
		OpenCBD();
		CTD = 0;
		CTA = 1;
		bPWState =  PW = 0;		
	} 
	else if((bSickRoomBroad|bOfficeBroad|bAllBroad))	//广播状态为SD通道
	{	//广播状态
		CCAP1H = stLocalControl.stEepromCfgData.byBroadVol;
		CloseCBD();
		OpenCGB();
		CTA = CTD = 0;
		bPWState = PW = 0;		
	}
	else if(bChannel0Talked)  //通道2为SD
	{	//通道0被叫通话状态
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkedVol;
		CloseCBD();
		OpenCGB();
		CTA = 0;
		CTD = 1;
		bPWState = PW = 0;		
	}
	else if(bChannel0Talk)
	{	//通道0主动通话状态
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkVol;
		CloseCBD();
		OpenCGB();
		CTA = 0;
		CTD = 1;
		bPWState = PW = 0;		
	}	
	else if(bSelfBroad)	   	//广播状态为SD通道
	{	//主动广播状态
		CCAP1H = stLocalControl.stEepromCfgData.byBroadVol;
		CloseCBD();
		OpenCGB();
		CTA = 0;
		CTD = 1;
		bPWState = PW = 0;			
	}
	else if((bCalledRing|bWaitListen))	//报号关闭SA、SD通道
	{	//被呼叫振铃或者等待接听
		CCAP1H = stLocalControl.stEepromCfgData.bySelfRingVol;
		CloseCBD();
		OpenCGB();
		CTA = CTD = 0;
		bPWState = PW = 0;		
	}
	else if(bIndicatingOther&&bEnSoundNote)	 //报号为SD通道
	{	//正在指示其它分机且许可报号
		CCAP1H = stLocalControl.stEepromCfgData.byRingVol;
		CloseCBD();
		OpenCGB();
		CTA = CTD = 0;
		bPWState = PW = 0;		
	}
	else if(bMusicPlaying)
	{	//背景音乐播放状态
		CCAP1H = stLocalControl.stEepromCfgData.byMusicVol;
		CloseCGB();
		OpenCBD();
		CTA = CTD = 0;
		bPWState = PW = 0;				
	}
	else if(bVoiceNoting)
	{	//语音提示
		CCAP1H = stLocalControl.stEepromCfgData.byNoteVol;
		CloseCGB();
		OpenCBD();
		CTA = CTD = 0;
		bPWState = PW = 0;
	}
	else
	{	//没有任何语音状态存在		
		CCAP1H = 10;
		CloseCGB();
		CloseCBD();
		CTA = CTD = 0;
		bPWState = PW = 1;
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SysReset(void)
{	
    Broadcasting_Flag=0;
	bBusy = bWaitAck = bWaitListen = bCalledRing = bChannel0Talked = bChannel0Talk = bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;			
	MakeCH0TimerOut(0, 0);	
	VoiceChannelCtx();
	LedControl();
	bIndication = 0; 
	SetLcdState(1);		
	ShowPage();	
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
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Bus0RecDeal(void)
{
	uint8 idata timebuf[5];
	//取出收到的数据帧 	 		
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;	
	EA=1;;	
	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//如果是登记状态,收到的命令不是登记确认命令,不作处理
		return;
	}
	//-----------------------------------------
	//-----------------------------------------
	//调试使用
	 //SBUF=stLocalControl.stBusDealFreq.byCmd;
	//while(!TI);
	//TI=0;		
	//-----------------------------------------
	//-----------------------------------------
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
	  /*case CMD_ENTER:										//确认登记命令
			if(bLanding)
			{	
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 					 					
					SetLedDealState(LED_OFF);
					SetMDLedState(LED_OFF);
					SetLcdState(1);
					ReadPageColour();	
					ShowPage();								
					byDevState1 = stLocalControl.stBusDealFreq.byRecSecAddr & 0x80;										
					MakeCH0TimerOut(0, 0); 										
				}
			}			
			break;*/
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		//增加自动检测命令--2012.3.13
		case CMD_BUS0_CHECK:	 //单总线检测
			RST_BUS=0;	
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS0_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
			break;	

		case CMD_SD_TAL_VOL_CHECK:
			RST_BUS=0;	 //SD、SA通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			CTD=1;
			OpenCGB();
			CloseCBD();
			PW =0;	//打开功放34119
			break;

		case CMD_SD_TAL_VOL_CHECK_END:
			RST_BUS=0;	//SD、SA通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;
			

		case CMD_SA_TAL_VOL_CHECK:
			RST_BUS=0;	//SD、SA通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			CTA = 1;
			CloseCGB();
			OpenCBD();
			PW=0;	//打开功放34119
			break;


		case CMD_SA_TAL_VOL_CHECK_END:
			RST_BUS=0;	//SD、SA通道正常
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			CTA = 0;
			CloseCBD();
			PW=1;	//关闭功放34119
			break;
		//---------------------------------------------------------------------
		case CMD_QUEST:										//查询命令
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_ANSWER; 			
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;			
		case CMD_NURSE_COME:							//护士到位命令并且是护士查房命令	 		
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))  //...比较区号和房号
			{		
				bNurseIn = 1;                          //如确认的设备是本房的,表明护士到达本房,置到位标志   				
				LedControl();
				VoiceChannelCtx();  
				INSPERCTOR_Count=0;
				//----------------------------------------------------------------
				//----------------------------------------------------------------
				//2012/5/26添加多个护士到位
				NurseComeCount++;
				if(NurseComeCount==1)
				{
				  NurseComeRoom[0]=stLocalControl.stBusDealFreq.bySndBedAddr;
				  INSPERCTOR_Flag=0;
				}
				else if(NurseComeCount>1)
				{
                  if(NurseComeRoom[0]==stLocalControl.stBusDealFreq.bySndBedAddr)
				  goto  NurseIN;
				  if(NurseComeRoom[1]==0x00)
				  {
				     if(stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[0]&&stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[2])
					 {
				       NurseComeRoom[1]=stLocalControl.stBusDealFreq.bySndBedAddr;
				       INSPERCTOR_Flag=0;
				       goto  NurseIN;  
					 }
				  }
				  else if(NurseComeRoom[1]==stLocalControl.stBusDealFreq.bySndBedAddr)
				  goto  NurseIN;				  	
				  if(NurseComeRoom[2]==0x00)
				  {
				     if(stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[0]&&stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[1])
					 {
				       NurseComeRoom[2]=stLocalControl.stBusDealFreq.bySndBedAddr;
				       INSPERCTOR_Flag=0;
					   goto  NurseIN;
					 }
				  }
				  else if(NurseComeRoom[2]==stLocalControl.stBusDealFreq.bySndBedAddr)
				  goto  NurseIN;	
				}
				//----------------------------------------------------------------
				//----------------------------------------------------------------
				//2012/4/24将此命令同时认为是查房命令
NurseIN:		if(!INSPERCTOR_Flag)
		         {
			         INSPERCTOR_Flag=1;
			         SerchRoom++;
				     if(SerchRoom>15)	//查房大于15次的话，重新回到第1页进行显示
				     {
				       PageNumber++;
				       SerchRoom=1;
				     }			   
		       		 if(PageNumber==1)   //第一屏已经写满
		       		 {
		          	   Page_Flag=1;
			           FmWriteByte(Serch_Room_Page_ADDR,INIT_FLAG);
		             }							   
		   		     FmWriteByte(Serch_Room_ADDR, SerchRoom);//存储第几次查房数据

					 
		   			 if(SerchRoom==1)	  //如果第一次查房
		        	 {
			      	   FmWriteByte(Serch_Room_ADDR+0x1000, stLocalControl.stBusDealFreq.bySndBedAddr);
				  	   HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x1000,5,Real_TimeBuf);
			         }
		   		     else if(SerchRoom==2) //如果第二次查房
		             { 
			           FmWriteByte(Serch_Room_ADDR+0x2000, stLocalControl.stBusDealFreq.bySndBedAddr);
			           HaveSerchRoomFlag=1; 
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x2000,5,Real_TimeBuf);
			         }
		   		     else if(SerchRoom==3)//如果第三次查房
		             {
			           FmWriteByte(Serch_Room_ADDR+0x3000, stLocalControl.stBusDealFreq.bySndBedAddr);
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x3000,5,Real_TimeBuf);
			         }
		   		     else if(SerchRoom==4)//如果第四次查房
		             {
		               FmWriteByte(Serch_Room_ADDR+0x4000, stLocalControl.stBusDealFreq.bySndBedAddr);
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x4000,5,Real_TimeBuf);
		             }
		   		     else if(SerchRoom==5)//如果第五次查房
		             {
			           FmWriteByte(Serch_Room_ADDR+0x5000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x5000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==6)//如果第六次查房
		             {
			           FmWriteByte(Serch_Room_ADDR+0x6000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x6000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==7)//如果第七次查房
		             {
			           FmWriteByte(Serch_Room_ADDR+0x7000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x7000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==8)//如果第八次查房
		             {
			            FmWriteByte(Serch_Room_ADDR+0x8000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			            HaveSerchRoomFlag=1;
				        FmWriteBytes(Serch_Room_TIME_ADDR+0x8000,5,Real_TimeBuf);
		        	 }
				     else if(SerchRoom==9)//如果第九次查房
		             {
			            FmWriteByte(Serch_Room_ADDR+0x9000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			            HaveSerchRoomFlag=1;
				        FmWriteBytes(Serch_Room_TIME_ADDR+0x9000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==10)//如果第十次查房
		             {
			            FmWriteByte(Serch_Room_ADDR+0xA000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			            HaveSerchRoomFlag=1;
				        FmWriteBytes(Serch_Room_TIME_ADDR+0xA000,5,Real_TimeBuf);
		             }
				    else if(SerchRoom==11)//如果第十一次查房
		            {
			           FmWriteByte(Serch_Room_ADDR+0xB000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0xB000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==12)//如果第十二次查房
		            {
			           FmWriteByte(Serch_Room_ADDR+0xC000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0xC000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==13)//如果第十三次查房
		            {	 
			           FmWriteByte(Serch_Room_ADDR+0x2D000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0xD000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==14)//如果第十四次查房
		            {	
			          FmWriteByte(Serch_Room_ADDR+0x2E000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			          HaveSerchRoomFlag=1;
				      FmWriteBytes(Serch_Room_TIME_ADDR+0xE000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==15)//如果第十五次查房
		            {
			          FmWriteByte(Serch_Room_ADDR+0x2F000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			          HaveSerchRoomFlag=1;
				      FmWriteBytes(Serch_Room_TIME_ADDR+0xF000,5,Real_TimeBuf);
		            } 
		          }	
			    //--------------------------------------------------------------------------------------
				//--------------------------------------------------------------------------------------					
			 }
			 //-----------------------------------------------------------------------------------------
			 //-----------------------------------------------------------------------------------------
			 //取消掉当有护士到位前提下，如果护士到位别的房间，门灯护士到位指示灯熄灭的情况，而是只有当护士离开后才熄灭
			/*else if(bNurseIn)
			{					
				bNurseIn = 0;	                     //不是到位本房间但本机的护士到位标志置位了,清除之前的护士到位标志 				
				LedControl();
				VoiceChannelCtx(); 			
			} */
			//------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------
			break;
		case CMD_NURSE_BACK:							//护士离开命令
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f))) //...比较区号和房号
			{	//清除的设备是本房的,表明护士离开本房,清到位标志
				bNurseIn = 0;					
				LedControl();
				VoiceChannelCtx();				
			}  			
			break;		
		case CMD_COMM_CALL:	 								//普通呼叫命令
			if(!bBusy)
			{ 	//通道0空闲			
				bBusy = 1;
				if(bIndicatingOther)
				{	//如果正在指示其他分机,停止指示	
				    //----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------									 					
					bIndication = 0; 
			        SetLcdState(1);			
			        ShowPage();	
				}				
				//清除所有呼叫指示标志
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();							//语音切换	 				
				//保存主动呼叫方地址
				SaveCallAddr(&(stLocalControl.stBusDealFreq));			
				MakeCH0TimerOut(250, 0);					//设置超时5s
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//呼叫本机设备	
					if(!(bChannel1Talk|bChannel1Talked))
					{	//通道1空闲,则本机为空闲状态,返回应答命令
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_COMM_ANSWER;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}				
				}
				LedControl();								//指示灯切换
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
				{	
				   	//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------										
					bIndication = 0; 
			        SetLcdState(1);			
			        ShowPage();	 				
				}
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();
				LedControl();
				if((bChannel1Talk|bChannel1Talked))
				{
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
						bAllBroad = bEnAllBroad; 	//全区广播状态					
						break;
				}
				VoiceChannelCtx();
				LedControl();
				ShowBROADCASTing();
			}
			break;			
		case CMD_INFO_INDICATION:							//收到呼叫指示命令 														
			if(!bBusy)		
			{			
				SaveIndicationData(&(stLocalControl.stBusDealFreq));
				if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
					((stLocalControl.stBusDealFreq.bySndRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
				{	//收到的是本房的呼叫指示,不需要作处理,清除上次的呼叫指示
					if(bIndicatingOther)
				    {	
					   //----------------------------------------------------------------------------------------
					   //----------------------------------------------------------------------------------------
					   bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				       VoiceChannelCtx();
			           LedControl();
					   //----------------------------------------------------------------------------------------
					   //----------------------------------------------------------------------------------------										
					   bIndication = 0; 
			           SetLcdState(1);			
			           ShowPage();	 				
				    }
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
					switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
					{
						case CMD_INFUSION_CALL:	   			//输液呼叫
							bConfusionNoting = bEnInfusionDeal;												
							break;
						case CMD_SERVICE_CALL:				//服务呼叫
							bServiceNoting = bEnServiceDeal;											
							break;
						case CMD_HELP_CALL:					//求援呼叫
							bHelpNoting = bEnHelpDeal;											
							break;
						case CMD_EMERGENCY_CALL: 			//紧急呼叫
							bEmergencyNoting = bEnEmergencyDeal;											
							break;
						default:
							return;
					}
					VoiceChannelCtx();
					LedControl(); 					
				}
				else
				{	//收到的不是本房的呼叫指示,判断是否本机能够处理
				/*	bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
					VoiceChannelCtx();
				    LedControl();
					switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
					{
						case CMD_INFUSION_CALL:	   			//输液呼叫
							if(!bEnInfusionDeal)
							{	
								return;													
							} 												
							break;
						case CMD_SERVICE_CALL:				//服务呼叫
							if(!bEnServiceDeal)
							{	
								return;							
							} 											
							break;
						case CMD_HELP_CALL:					//求援呼叫
							if(!bEnHelpDeal)
							{	
								return;							
							} 											
							break;
						case CMD_EMERGENCY_CALL: 			//紧急呼叫
							if(!bEnEmergencyDeal)
							{
								return;						
							}  											
							break;
						default:
							return;
					} */
					bIndicatingOther = 1;
					if((bChannel1Talked|bChannel1Talk))
					{	//如果通道1处于通话状态,不作处理
						return;
					}	
					VoiceChannelCtx();
					LedControl();
					bIndication = 1;
			        ShowCallFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, stLocalControl.stIndicationData.byCallCmd&0x1f);			
				} 		
			} 			
			break;
		case CMD_INFUSION_ANSWER:							//处理输液呼叫命令
		case CMD_SERVICE_ANSWER:							//处理服务呼叫命令
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫命令
		case CMD_HELP_ANSWER:								//处理求援呼叫命令
			if(!bBusy)
			{	//通道0不忙				
				bBusy = 1;
			    if(bIndicatingOther)
				{
				   	//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------					
					bIndication = 0; 
			        SetLcdState(1);			
			        ShowPage();										
				} 				
				//保存主动呼叫方地址				
				SaveCallAddr(&(stLocalControl.stBusDealFreq));				
				MakeCH0TimerOut(250, 0);
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();				
				LedControl();  
			}
			break;
		case CMD_STOP_INDICATION:							//停止指示命令
			 if(bIndicatingOther)
			 { 	
				//----------------------------------------------------------------------------------------
			    //----------------------------------------------------------------------------------------
			    bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				VoiceChannelCtx();
			    LedControl();
			    //----------------------------------------------------------------------------------------
			    //----------------------------------------------------------------------------------------		
				bIndication = 0; 
			    SetLcdState(1);			
			    ShowPage();	
			 } 
			 bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
			 VoiceChannelCtx();
			 LedControl();
			 break;
		case CMD_INFUSION_CLEAR: 							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR: 							//清除服务呼叫命令
		case CMD_HELP_CLEAR: 								//清除求援呼叫命令
		case CMD_EMERGENCY_CLEAR: 							//清除紧急呼叫命令			 
			if(	(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.byRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.byBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd-0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//清除本机正在指示的呼叫信息 
			   if(bIndicatingOther)
				{ 	
				    //----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bIndication = 0; 
					SetLcdState(1);			
					ShowPage();						
				} 				
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();
				LedControl();	
			}	
			break; 			
		case CMD_SYSTERM_RESET:								//系统复位命令
			SysReset();
			break; 
		case CMD_START_VOICE:								//启动播音命令
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
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
				bVoiceNoting = bMusicPlaying = 0;
				VoiceChannelCtx();					
			}
			break; 
		case CMD_CHANNEL_CHANGE: 							//通道切换命令	
			//将通道0的状态切换到通道1上,同时清除通道0的状态				 			
			bBusy = bWaitAck = bWaitListen = bCalledRing =  
				bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;			
			MakeCH0TimerOut(0, 0);	
			//-----------------------------------------------------------------------------------------------
			//-----------------------------------------------------------------------------------------------
			//2012/5/18消除非本房分机时的呼叫指示	
			if(bIndicatingOther)
		    {
			    bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				VoiceChannelCtx();
			    LedControl();				
			    bIndication = 0; 
			    SetLcdState(1);			
			    ShowPage();											
			} 
			//-----------------------------------------------------------------------------------------------
			//-----------------------------------------------------------------------------------------------	
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
			}	
			break;
		case CMD_CHANNEL_CLOSE: 							//关闭通道切换命令			
			if((bChannel1Talked|bChannel1Talk))
			{		//如果通道1处于通话状态,结束后需指示新的信息,如没有,保持状态
				if(bIndicatingOther)
				{	//如果有指示,立即显示指示信息 
				  bIndication = 0; 
				  SetLcdState(1);			
				  ShowPage();
				}
			}
			bChannel1Talked = bChannel1Talk = 0; 				
			MakeCH1TimerOut(0, 0);	
			VoiceChannelCtx();
			LedControl();			
			break;
         /*	case CMD_OPEN_485BUS_IN:							//打开485输出命令	
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{ 
				KDR = 0;
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_485BUS_IN;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = 0x02;							
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			}
			break;
		case CMD_CLOSE_485BUS:  							//关闭485输出
			KDR = 1;   			
			break;*/
		//------------------------------------------------------
		//------------------------------------------------------
		//2012/4/9设置系统波特率
		case CMD_SET_BAUD:
		     if(stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)
			 {
		       Fbaud_Type=stLocalControl.stBusDealFreq.byRecSecAddr;
			   FmWriteByte(Set_Baud_ADDR,INIT_FLAG);
			   FmWriteByte(System_Baud_ADDR,Fbaud_Type);
			   switch(Fbaud_Type)
	           {
	             case 1:
		         Fbaud=1200;
		         break;
		         case 2:
		         Fbaud=2400;
		 	     break;
		         case 3:
		         Fbaud=4800;
		         break;
		         case 4:
		         Fbaud=9600;
		         break;
		         case 5:
		         Fbaud=14400;
		         break;
		         case 6:
		         Fbaud=19200;
		         break;
		         case 7:
		         Fbaud=28800;
		         break;
		         case 8:
		         Fbaud=38400;
		         break;
		         case 9:
		         Fbaud=57600;
		         break;
		         case 10:
		         Fbaud=115200;
		         break;
	            }
	           if(Fbaud>2000)
	           {
			     AUXR |= 0x40;	           //1T模式 
	             TL1=TH1=256-(Fosc/32/Fbaud);
	           }
	           else
	           {
	  			 AUXR &= 0xBF;
	  			 TL1=TH1=256-(Fosc/32/Fbaud/12);
	           }
			 }  
		    break;
		//------------------------------------------------------
		//------------------------------------------------------
		//2012/5/8设置系统区号命令
		case CMD_SET_SEC_SERIAL:
			 stLocalControl.stEepromCfgData.bySelfSecAddr=stLocalControl.stBusDealFreq.byRecSecAddr;
			 SaveParameter();
			 ReadPageColour();
			 SetLcdState(1);	
			 ShowPage();
		break;
		//------------------------------------------------------
		//------------------------------------------------------
		case CMD_OPEN_LCD:									//打开LCD背光显示
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_LCD;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   SetLcdState(1);
			 } 
		     break;
		case CMD_CLOSE_LCD:									//关闭LCD背光显示
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_CLOSE_LCD;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   SetLcdState(0);
			 }
		     break;
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		 case CMD_ENABLE_SOUND:									//许可语音报号命令
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_ENABLE_SOUND;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   FmWriteByte(Close_EnSoundNote_ADDR,0x00);
		       byEnable1=0xFF;
			 } 
		     break;
		case CMD_DISABLE_SOUND:									//关闭语音报号命令 
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_DISABLE_SOUND;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   FmWriteByte(Close_EnSoundNote_ADDR,0x01);
		       byEnable1=0xF7;
			 }
		     break;
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		case CMD_SEC_CLR:									//清除科室内容
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SEC_CLR;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   Modify_SEC_Name	= 0x00;
			   FmWriteByte(LCD_SEC_MSK_ADDR, 0);
			   SetLcdState(1);
			   ReadPageColour();	 				 				
			   ShowPage(); 
			}
			break;			
		case CMD_POWER_ON:	  								//本机重新热启动
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{					
				EA=0;
				//关闭所有打开的中断
				CCAPM0 = 0x00;
				ET0 = 0;
				TR0 = 0;
				EX0 = EX1 = 0;
				IAP_CONTR = 0x20; 		
				break;			 				
			}
			break;	
		case CMD_SET_BY_SERIAL:								 //收到编号的命令	
		     if(stLocalControl.stBusDealFreq.bySndBedAddr==0x00) //床号为0表示为门口分机编号
			 {
			   SET_BY_SERIAL_FLAG=1;
			   SetLedDealState(LED_FLASH);
			   ShowNumberPage();
			 }
			 break;
		 /*case  CMD_INSPERCTOR_ENTER:  //收到查房命令，发送给TFT显示屏
		 {
		   if(!INSPERCTOR_Flag)
		   {
		      if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7F) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7F)))  //...比较区号和房号
			  {
			    INSPERCTOR_Flag=1;
			    SerchRoom++;
				if(SerchRoom>15)	//查房大于15次的话，重新回到第1页进行显示
				{
				   PageNumber++;
				   SerchRoom=1;
				}			   
		       if(PageNumber==1)   //第一屏已经写满
		       {
		          Page_Flag=1;
			      FmWriteByte(Serch_Room_Page_ADDR,INIT_FLAG);
		       }							   
		   		FmWriteByte(Serch_Room_ADDR, SerchRoom);//存储第几次查房数据
		   		if(SerchRoom==1)	  //如果第一次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x1000, stLocalControl.stBusDealFreq.bySndBedAddr);
				  HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x1000,5,Real_TimeBuf);
			    }
		   		else if(SerchRoom==2) //如果第二次查房
		        { 
			      FmWriteByte(Serch_Room_ADDR+0x2000, stLocalControl.stBusDealFreq.bySndBedAddr);
			      HaveSerchRoomFlag=1; 
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x2000,5,Real_TimeBuf);
			    }
		   		else if(SerchRoom==3)//如果第三次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x3000, stLocalControl.stBusDealFreq.bySndBedAddr);
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x3000,5,Real_TimeBuf);
			    }
		   		else if(SerchRoom==4)//如果第四次查房
		        {
		          FmWriteByte(Serch_Room_ADDR+0x4000, stLocalControl.stBusDealFreq.bySndBedAddr);
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x4000,5,Real_TimeBuf);
		         }
		   		else if(SerchRoom==5)//如果第五次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x5000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x5000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==6)//如果第六次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x6000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x6000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==7)//如果第七次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x7000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x7000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==8)//如果第八次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x8000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x8000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==9)//如果第九次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x9000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x9000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==10)//如果第十次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0xA000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xA000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==11)//如果第十一次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0xB000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xB000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==12)//如果第十二次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0xC000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xC000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==13)//如果第十三次查房
		        {	 
			      FmWriteByte(Serch_Room_ADDR+0x2D000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xD000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==14)//如果第十四次查房
		        {	
			      FmWriteByte(Serch_Room_ADDR+0x2E000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xE000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==15)//如果第十五次查房
		        {
			      FmWriteByte(Serch_Room_ADDR+0x2F000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xF000,5,Real_TimeBuf);
		        } 
		      }
		   }
	 	 }
		 break;	*/
	   case   CMD_DATA_SEND:   
	   {
		  Real_TimeBuf[4]=BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr); // 获取实时时间
		  Real_TimeBuf[3]=BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1F);
		  Real_TimeBuf[2]=BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3F);
	      Real_TimeBuf[1]=BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3F); 
		  Real_TimeBuf[0]=BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7F); 
		  if(HaveSerchRoomFlag)
		  {
		    HaveSerchRoomFlag=0;
			timebuf[0]=BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7F);	//分
		    timebuf[1]=BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3F);	//时
			timebuf[2]=BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3F);	//日
			timebuf[3]=BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1F); //月
			timebuf[4]=BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);		   //年
			if(SerchRoom==1)
			 { 
				FmWriteBytes(Serch_Room_TIME_ADDR+0x1000,5,timebuf);
			 }
			else if(SerchRoom==2)
			 {
				FmWriteBytes(Serch_Room_TIME_ADDR+0x2000,5,timebuf);
			 }
			else if(SerchRoom==3)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x3000,5,timebuf);
			}
			else if(SerchRoom==4)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x4000,5,timebuf);
			}
			else if(SerchRoom==5)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x5000,5,timebuf);
			}
			else if(SerchRoom==6)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x6000,5,timebuf);
			}
			else if(SerchRoom==7)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x7000,5,timebuf);
			}
			else if(SerchRoom==8)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x8000,5,timebuf);
			}
			else if(SerchRoom==9)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x9000,5,timebuf);
			}
			else if(SerchRoom==10)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xA000,5,timebuf);
			}
			else if(SerchRoom==11)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xB000,5,timebuf);
			}
			else if(SerchRoom==12)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xC000,5,timebuf);
			}
			else if(SerchRoom==13)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xD000,5,timebuf);
			}
			else if(SerchRoom==14)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xE000,5,timebuf);
			}
			else if(SerchRoom==15)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xF000,5,timebuf);
			} 
		 }
	   }
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
*创建人	      		:陈卫国
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
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
	   /*case CMD_LANDING: 									//登记命令
			if(bLanding)
			{	//本机确实处在登记状态,设置等待确认超时
				MakeCH0TimerOut(150, 0);								
			}			
			break;*/
		case CMD_COMM_CALL:	 								//普通呼叫命令 							
		case CMD_INFUSION_ANSWER:							//处理输液呼叫
		case CMD_SERVICE_ANSWER:							//处理服务呼叫
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫
		case CMD_HELP_ANSWER:								//处理求援呼叫							
			if(!bBusy)
			{
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq)); 
				MakeCH0TimerOut(250, 0); 						
			}		
		case CMD_INFUSION_CLEAR:							//清除输液呼叫
		case CMD_SERVICE_CLEAR:								//清除服务呼叫
		case CMD_HELP_CLEAR:								//清除求援呼叫
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫
			//停止正在指示的呼叫	
			if(bIndicatingOther)
			{
			  //----------------------------------------------------------------------------------------
			  //----------------------------------------------------------------------------------------
			  bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
			  VoiceChannelCtx();
			  LedControl();
			  //----------------------------------------------------------------------------------------
			  //----------------------------------------------------------------------------------------				
			  bIndication = 0; 
			  SetLcdState(1);			
			  ShowPage();			
			}
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
			VoiceChannelCtx();
			LedControl();			
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
				VoiceChannelCtx();
				LedControl(); 				
				if(bEnAutoListen)
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_CALL_LISTEN;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
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
				{	//本机已经处于通话状态了,缩短超时,退出 					
					MakeCH0TimerOut(5, 0);
					break;
				}
				//设置通话超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);				
				VoiceChannelCtx();
				LedControl();
			}
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   								//广播命令
			//停止正在指示的呼叫
			if(bIndicatingOther)
			{				
			  bIndication = 0; 
			  SetLcdState(1);			
			  ShowPage();
			}
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
			VoiceChannelCtx(); 			
			if(!bBusy)
			{ 	
				bBusy = 1;			  	
			  	//保存主动呼叫方地址(本机地址)
				SaveCallAddr(&(stLocalControl.stBusDealFreq));				
				bSelfBroad = 1;
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//本机广播条件不存在了,缩短超时,退出			
					MakeCH0TimerOut(5, 0);
					break;
				}
				//设置广播超时
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);
				VoiceChannelCtx();				
			}
			LedControl();			
			break; 			
		case CMD_CHANNEL_CLOSE:								//关闭语音通道命令
			bChannel1Talk = bChannel1Talked = 0; 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			LedControl();
			if(bIndicatingOther)
			{ 
			  bIndication = 0; 
			  SetLcdState(1);			
			  ShowPage();
			}	
			break;	
		case CMD_GET_BUS:									//占用总线
			Bus0SendPin = 1;								//制造总线故障
			SaveParameter();
			Bus0SendPin = 0;								//释放总线
			PW = bPWState; 									//恢复语音功放的控制引脚			
			break;	
		case CMD_SYSTERM_RESET:	 							//系统复位命令
			SysReset();
			break;
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case CMD_SD_TAL_VOL_CHECK_END:
					CTD=0;
					CloseCGB();
					PW=1; //关闭功放34119
					break;
			}
			break;
	}	
} 	

/**********************************************************
*函数名称			:TimerOutDeal	
*函数描述        	:超时处理函数
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:Bus0OutputData					 
***********************************************************
*创建人	      		:熊坚强
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
				else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
				{	//等待应答，等待接听，主动通话，主动广播状态			
				 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
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
					else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
					{	//等待应答，等待接听，主动通话，主动广播状态		
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
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
				{	//超时次数没有完成，重新加载单位超时时间 			
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
				if(bChannel1Talk)
				{	//主动通话状态，发送切换命令			
					stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;						
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bChannel1Talked)
				{	//被动通话状态，自己复位
					bChannel1Talked = 0;
					if(bIndicatingOther)
					{ 
					   bIndication = 0; 
			           SetLcdState(1);			
			           ShowPage();	
					}
					VoiceChannelCtx();
					LedControl();
				}	
			}
			else
			{	//超时次数没有完
				stLocalControl.stCH1TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
				{ 	//所有超时完成
					if(bChannel1Talk)
					{	//主动通话状态，发送切换命令						
						stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;						
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					}
					else if(bChannel1Talked)
					{	//被动通话状态，自己复位
						bChannel1Talked = 0;
						if(bIndicatingOther)
						{ 
						  bIndication = 0; 
			              SetLcdState(1);			
			              ShowPage();	
						}
						VoiceChannelCtx();
						LedControl();
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
	if(--stLocalControl.byLedFlashTime == 0x00)
	{
		stLocalControl.byLedFlashTime = 50;
		if((stLocalControl.byLedState & 0x0f) == LED_FLASH)
		{ 	//红灯闪烁状态	
			bLedDealState = !bLedDealState;
			LED_DEAL = bLedDealState;
		}
	}
	if(--stLocalControl.byLedFlashDelay == 0x00)
	{
		stLocalControl.byLedFlashDelay = 25;
		if((stLocalControl.byMDLedState&LED_FLASH) == LED_FLASH)
		{	//红灯闪烁状态
			bLedRedState = !bLedRedState;
			#ifdef DL_SWAP
			MDR = !bLedRedState;
			#else
			MDR = bLedRedState;
			#endif
		}
		if((stLocalControl.byMDLedState&(LED_FLASH<<2)) == (LED_FLASH<<2))
		{	//绿灯闪烁状态
			bLedGreenState = !bLedGreenState;
			#ifdef DL_SWAP
			MDG = !bLedGreenState;
			#else
			MDG = bLedGreenState;
			#endif
		}  
		if((stLocalControl.byMDLedState&(LED_FLASH<<4)) == (LED_FLASH<<4))
		{	//蓝灯闪烁状态
			bLedBlueState = !bLedBlueState;
			#ifdef DL_SWAP
			MDB = !bLedBlueState;
			#else
			MDB = bLedBlueState;
			#endif
		}
	}

	//控制屏幕打开
	if (bLcdOn)
	{
		LCD_LED=0;
		LCD_LED=1;
	}
	else LCD_LED=0;

	
	if(SET_BY_SERIAL_FLAG)//如果收到设备编号命令，则开始5秒定时
	{
		SET_BY_SERIAL_COUNT++;
	}
	 else	SET_BY_SERIAL_COUNT=0;

	 
	 if(SET_BY_SERIAL_COUNT>250)		//5秒时间到
	 {
	   	SET_BY_SERIAL_FLAG=0;
		SET_BY_SERIAL_COUNT=0;
		SetLedDealState(LED_OFF);
		NUMBER_SET_STOP_FLAG=1;
	 }
	 
	if(SerchRoomKeyFlag)  SerchRoomCount++;
	else SerchRoomCount=0;
	
	if(SerchRoomCount>300)			//6秒时间到
	{ 
	  SerchRoomCount=0;
	  SerchRoomFlag=1;
	  SerchRoomKeyFlag=0;
	  Key_SerchRoom_Flag2=0;
	  Key_SerchRoom_Flag3=0;
	}

	
	if(INSPERCTOR_Flag)  INSPERCTOR_Count++;
	else INSPERCTOR_Count=0;
	if(INSPERCTOR_Count>3000) //两次护士查房的时间间隔时间必须大于1分钟才有效记录 
	{
	  INSPERCTOR_Count=0;
	  INSPERCTOR_Flag=0;
      //---------------------------------
      //---------------------------------
      //2012/5/26添加多个护士到位
	  memset(NurseComeRoom,0,3);
	  NurseComeCount=0;
      //---------------------------------
      //---------------------------------
	}

	
	//-----------------------------------
	if(INDICTION_Flag)  INDICTION_Count++;
	else  INDICTION_Count=0;
	if(INDICTION_Count>50)	// 1秒到
	{
	  INDICTION_Count=0;
	  INDICTION_Flag=0;
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
				Delayms(400);

				
				if(Bus0RecPin ==0)
				{//本机有故障 //使灯快闪
					ShowSelfError();     //液晶屏提示本机故障
					do
					{
						RST_BUS =1;	//断开SD到总线
						Bus0SendPin =0;	//使SD为高电平
						
						SetLedDealState(1);
						Delayms(100);		//延时200ms

						SetLedDealState(0);
						Delayms(100);
					}while(Bus0RecPin==0);	//一直等待为高

					
					RST_BUS = 0;	//使SD线路连通					
					BusLowDTime = 100;
					//使灯恢复常灭
					SetLedDealState(0);
					ShowNormal();
				}

				
				else 
				{//是外部总线引起
					ShowBusError();
					RST_BUS = 0;	//使SD线路连通
					Bus0SendPin =0;	//使SD为高电平

					do
					{
							
						RST_BUS = 0;	//使SD线路连通
						Bus0SendPin =0;	//使SD为高电平
						
						SetLedDealState(1);	//灯慢闪
						Delayms(400);		//延时600ms 
						SetLedDealState(0);
						Delayms(400);
						
					}while(Bus0RecPin==0);	//一直等待为高
					

					//使灯恢复常灭
Bus0BugRet:
					SetLedDealState(0);
					ShowNormal();
					Bus0SendPin =0;	//使SD为高电平
					RST_BUS = 0;	//使SD线路连通					
					BusLowDTime = 100;

				}				
			}
		}
	}	
}
	  