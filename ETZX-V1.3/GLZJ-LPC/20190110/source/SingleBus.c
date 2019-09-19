/**********************************************************************************************************************/
//一特之星单总线收发程序
/**********************************************************************************************************************/
#define _IN_SINGLE_BUS_
#include"custom.h"

//总线0变量定义
uint16_t byBus0State 	= 0;							//单总线0状态字
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
#define SetBus0State(x) (byBus0State|=(x))		//置位操作
#define ClrBus0State(x) (byBus0State&=~(x))		//清零操作
#define GetBus0State(x) (byBus0State&(x))			//获取位状态
uint8_t byBus0RecCount = 0;						//高4位是总线0接收定时中断计数，低4位是接收到的位计数
uint8_t byBus0RecBuf;								//总线0接收缓冲单元
uint8_t byBus0SendBuf;							//总线0发送缓冲单元	
uint8_t byBus0RecData[BUS0_FREQ_SIZE];			//总线0接收缓冲区
uint8_t byBus0SendData[BUS0_FREQ_SIZE];			//总线0发送缓冲区
uint8_t byBus0SendCount = 0;						//高4位是总线0发送定时中断计数，低4位是发送的位计数
uint8_t byBus0RecSendCount = 0;					//高4位是总线0接收到的字节计数，低4位是总线0发送完的字节计数
uint8_t byBus0SendStopCount;						//总线0停止时间计数
uint8_t byBus0RecTimeOut = 0;						//总线0接收超时计数
uint8_t byBus0DisableCount = 0;					//总线0禁止计数	 

uint8_t byBus0TxQ[BUS0_TX_Q_ZISE];				//总线0发送队列							
uint8_t byBus0TxHead = 0;							//单总线0发送队列头指针
uint8_t byBus0TxTail = 0; 						//单总线0发送队列尾指针
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 

uint8_t byBus0RxQ[BUS0_RX_Q_ZISE];				//总线0接收队列							
uint8_t byBus0RxHead = 0;							//单总线0接收队列头指针
uint8_t byBus0RxTail = 0; 						//单总线0接收队列尾指针
#define IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE) 

//变量定义
uint8_t 			byChar;
uint8_t				byWaitAckTimeLanding=0;				//上电登记时等待PC机的应答时间
uint16_t			uiDevState=0; 						//设备当前所处的状态
uint16_t			uiDetectPcTime=0;					//侦听PC机间隔时间	
uint8_t				byVoicePlayTime=0;					//报号定时器
STBusFreq			stBusFreq;							//单总线数据处理结构体	
STTimerout			stCH0TimerOut;						//超时管理结构体
STTimerout			stCH1TimerOut;						//通道1通话超时管理结构体	
uint8_t				byWaitCommAckDTime=0;				//通用等待定时时间
uint8_t 			byUsart2HandingDTime=0;				//串口2正在处理定时时间
uint8_t 			byUsart0SdResultDTime=0;			//串口2发送结果码延时时间
STPointer			stCallPointer;						//呼叫队列双向链表
STIndicationData	stIndicationData;					//信息指示数据结构体
//STVoicePlay 		stVoicePlay;						//语音播放控制
STAddr				stCallAddr; 						//通道0主动呼叫方地址
STAddr				stCH1CallAddr; 						//通道1主动呼叫方地址
STAddr_S			stRS485Addr;						//RS485数据接收方地址
uint8_t				byWaitRS485AckDTime=0;				//通用等待定时时间
STEepromCfgData 	stEepromCfgData;					//本机配置字结构体
uint8_t				byReSndBrtTimes=0;					//重复发送设置波特率的次数
uint8_t				byReSndRs485RoomTimes=0;			//重复发送床头分机接收RS485信息是否需要判断房号的次数
uint8_t 			byReSndBedVoiPlaTimes=0;			//重复发送语音报号时床头分机是否语音报号的次数
STAddr_S			stCommAddr;							//通用地址

//---------------------------------


uint8_t TimerOutCount=0;			//20ms单元定时
uint8_t bBus0SndBitOk=0;



uint16_t State	=0;								//R485相关标志
/*#define  bRS485DataTreat 	(1<<0)				//RS485数据处理标志
#define  bOriginMusic		(1<<1)				//背景音乐初始状态
#define  bPCOpen			(1<<2)				//上位机打开状态
#define  bTimerOutDeal		(1<<3)				//TimerOutDeal()函数处理标志
#define  bLedState			(1<<4)				//LED灯状态
#define  bPowerEnState		(1<<5)				//信号线电源状态


#define SetState(x) (State|=(x))			//置位操作
#define ClrState(x) (State&=~(x))		//清零操作
#define GetState(x) (State&(x))			//获取位状态

*/

uint16_t BusErrorState=0;


STTime	stTime;
uint8_t byYearH=0x20;		//年高字节


void SingleBusInit(void);
void TIMER1_IRQHandler (void);			 //100us定时
uint8_t Bus0OutputData(uint8_t *pbyData);
void  eint3Isr (void);
void save_parameter(void);
void Bus0Manager(void);
void SaveCallAddr(pSTBusFreq pstBusFreq);
uint8_t AddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
uint8_t DirAddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
void MakeCH0TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
void MakeCH1TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
uint8_t AddCallNod(pSTBusFreq pstBusFreq);
void RemoveCallNod(pSTBusFreq pstBusFreq);
void SaveIndicationData(pSTBusFreq pstBusFreq, uint8_t byFirstNod);
void VoiceChannelCtx(void);
void RestartIndication(void);
void LedStateFlash(void);
void Bus0RecDeal(void);
void Bus0SendFinishDeal(void);
void TimerOutDeal(void);
//----------------------------------


uint8_t check_ee_485brt(void)
{
	if(stEepromCfgData.uiRs485Brt==57600) return 1;
	if(stEepromCfgData.uiRs485Brt==38400) return 1;
	if(stEepromCfgData.uiRs485Brt==28800) return 1;
	if(stEepromCfgData.uiRs485Brt==19200) return 1;
	if(stEepromCfgData.uiRs485Brt==14400) return 1;
	if(stEepromCfgData.uiRs485Brt==9600) return 1;
	if(stEepromCfgData.uiRs485Brt==4800) return 1;
	if(stEepromCfgData.uiRs485Brt==2400) return 1;
	if(stEepromCfgData.uiRs485Brt==1200) return 1;
	return 0;
}




/**********************************************************
*函数名称			:init_parameter	
*函数描述        	:参数初始化
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
void init_parameter(void)
{	
	sst_read(&(stEepromCfgData.byInitFlag),PARA_ADDR,sizeof(STEepromCfgData));
//	uart2_send_str2(&(stEepromCfgData.byInitFlag),sizeof(STEepromCfgData));

	
	if(stEepromCfgData.byInitFlag != INIT_FLAG)
	{
para_init:
		stEepromCfgData.byInitFlag = INIT_FLAG;
		stEepromCfgData.bySelfSecAddr = 0x01;
		stEepromCfgData.bySelfRoomAddr = GLZJ_ADDR2;
		stEepromCfgData.bySelfBedAddr = GLZJ_ADDR3;
		stEepromCfgData.uiRs485Brt	= 57600;
		stEepromCfgData.byBLOffStartHour = 0x20;
		stEepromCfgData.byBLOffStartMin = 0x0;
		stEepromCfgData.byBLOffEndHour = 0x6;
		stEepromCfgData.byBLOffEndMin = 0x0;
		stEepromCfgData.byLVoiceStartHour = 0x20;
		stEepromCfgData.byLVoiceStartMin =0x0;
		stEepromCfgData.byLVoiceEndHour = 0x8;
		stEepromCfgData.byLVoiceEndMin =0x0;
		stEepromCfgData.byLVoiceVal =5;
		stEepromCfgData.bPlayRoom =0;
		
		stEepromCfgData.byHostPlayVol = 8;
		stEepromCfgData.bySlaPlayVol  =8;

		stEepromCfgData.byHostPlayLittleVol = 5;
		stEepromCfgData.bySlaPlayLittleVol =5;
		
		stEepromCfgData.byHostTalkVol = 28;
		stEepromCfgData.bySlaTalkVol = 20;

		
		stEepromCfgData.byVersionH = 0x01;
		stEepromCfgData.byVersionL =0x00;

		stEepromCfgData.bySysMode = 0xf0;		//初始时仅使用WIFI通迅

		save_parameter();

	}
	else if( stEepromCfgData.bySelfRoomAddr!=GLZJ_ADDR2 || stEepromCfgData.bySelfBedAddr!= GLZJ_ADDR3)
	{
		goto para_init;
	}
	else if(check_ee_485brt()==0)
	{
		goto para_init;
	}
	
}


void reset_parameter(void)
{
	stEepromCfgData.byInitFlag = INIT_FLAG;
	stEepromCfgData.bySelfSecAddr = 0x01;
	stEepromCfgData.bySelfRoomAddr = GLZJ_ADDR2;
	stEepromCfgData.bySelfBedAddr = GLZJ_ADDR3;
	stEepromCfgData.uiRs485Brt	= 57600;
	stEepromCfgData.byBLOffStartHour = 0x20;
	stEepromCfgData.byBLOffStartMin = 0x0;
	stEepromCfgData.byBLOffEndHour = 0x6;
	stEepromCfgData.byBLOffEndMin = 0x0;
	stEepromCfgData.byLVoiceStartHour = 0x20;
	stEepromCfgData.byLVoiceStartMin =0x0;
	stEepromCfgData.byLVoiceEndHour = 0x8;
	stEepromCfgData.byLVoiceEndMin =0x0;
	stEepromCfgData.byLVoiceVal =5;
	stEepromCfgData.bPlayRoom =0;
	
	stEepromCfgData.byHostPlayVol = 8;
	stEepromCfgData.bySlaPlayVol  =8;

	stEepromCfgData.byHostPlayLittleVol = 5;
	stEepromCfgData.bySlaPlayLittleVol =5;

	
	stEepromCfgData.byHostTalkVol = 28;
	stEepromCfgData.bySlaTalkVol = 20;
	
	
	stEepromCfgData.byVersionH = 0x01;
	stEepromCfgData.byVersionL =0x00;
	
	save_parameter();

}



void save_parameter(void)
{
//	uint8_t eeprom_buf[20];
	
//	uart2_send_str2(&(stEepromCfgData.byInitFlag),sizeof(STEepromCfgData));
	sst_write(&(stEepromCfgData.byInitFlag),PARA_ADDR,sizeof(STEepromCfgData));	

//	sst_read(eeprom_buf,PARA_ADDR,sizeof(STEepromCfgData));
//	uart2_send_str2(eeprom_buf,sizeof(STEepromCfgData));
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

	//初始化发送接收IO	
	LPC_PINCON->PINSEL4 &= (~(0x03 << 26)); 			// P2.13  TX 常态为1
	LPC_GPIO2->FIODIR    |= sd_tx_pin;
	sd_tx_ctrl(1);
	
		
	LPC_PINCON->PINSEL0 &= (~(0x03 << 2)); 			    // P0.1  rx
	LPC_GPIO2->FIODIR	&= ~sd_rx_pin;

	
	//初始化定时器作为接收使用
	init_timer1();
	byBus0SendStopCount = 240;

	
	//使能接收中断(下降沿有效)
	if (GetBus0RecPin()) 
	{
		IO0IntClr =  Bus0RecPin;		//清除中断标志
		IO0IntEnF |= Bus0RecPin;		//下降沿中断		
	}
	else
	{	//如果总线不正常,置总线故障标志 				
		SetBus0State(bBus0Error);
	}		
	NVIC_EnableIRQ(EINT3_IRQn);
    NVIC_SetPriority(EINT3_IRQn, 1);   	// 设置外部中断并使能 
}


/**********************************************************
*函数名称			:Timer1Int	
*函数描述        	:定时器0溢出中断,定时器每100us中断一次
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
void TIMER1_IRQHandler (void)			 //100us定时
{  //匹配时产生中断和复位
//	uint32_t i;
	uint8_t byTemp;
	LPC_TIM1->IR		 = 0x01;	   //清除中断

	
	TimerOutCount++;
	if(TimerOutCount>=200)
	{
		TimerOutCount=0;
		SetState(bTimerOutDeal);
	}

	if (GetBus0RecPin()) 
	{
		SetBus0State(bBus0RecBit);
	}
	else 
	{
		ClrBus0State(bBus0RecBit);
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
				if((!GetBus0State(bBus0SendError))&&(GetBus0State(bBus0OnSendFreq)))
				{	//没有发生过总线发送错误，且有一帧数据正在发送，停止帧数据发送，置发送错误标志						
					byBus0State &= (~BUS0_SEND_CON);
					SetBus0State(bBus0SendError);
					SetBus0SendPin();			//释放总线	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;		//接收出错，重置接收发送计数值
				EnableBus0RecInt();

				SetBusErrorState(bStartBitError);
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

						SetBusErrorState(bNightBitError);
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
						SetBus0SendPin();		//释放总线
					}
					byBus0RecSendCount = 0x00;	//接收出错，重置接收发送计数值

					SetBusErrorState(bEndBitError);
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
			ClrBus0SendPin();
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
			SetBus0SendPin();
			byBus0SendStopCount = 240;
		}
	}
	else if(GetBus0State(bBus0OnSendBit))
	{ 	//有数据位正在发送,首先发送的是起始位
		byBus0SendCount += 0x10;
		if(0x50 == (byBus0SendCount & 0xf0))
		{//处于一个数据位的中间
			if((GetBus0State(bBus0SendBit)==0&&GetBus0State(bBus0RecBit)==0)||(GetBus0State(bBus0SendBit)!=0&&GetBus0State(bBus0RecBit)!=0))   bBus0SndBitOk=1;
			else
			{	//不相同,发送失败								
				byBus0State &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				SetBus0State(bBus0SendError);					
				SetBus0SendPin(); 
				byBus0SendStopCount = 240;

				bBus0SndBitOk=0;
			}				
					
		}
	
		//if((GetBus0State(bBus0SendBit)==0&&GetBus0State(bBus0RecBit)==0)||(GetBus0State(bBus0SendBit)!=0&&GetBus0State(bBus0RecBit)!=0))   		
		else if(bBus0SndBitOk ==1)
		{	//发送的数据和接收的数据相同
			//byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//一位数据发送完毕,首先发送的是起始位
				bBus0SndBitOk=0;
				
				byBus0SendCount &= 0x0f;
				byBus0SendCount += 0x01;				
				if(0x09 == (byBus0SendCount & 0x0f))
				{	//发送到第9位了
					if ((byBus0RecSendCount&0x0f)==0) 
					{
						SetBus0State(bBus0SendBit);
						SetBus0SendPin();
					}
					else
					{
						ClrBus0State(bBus0SendBit);
						ClrBus0SendPin();
					}					
				}
				else if(0x0a == (byBus0SendCount & 0x0f))
				{	//发送到结束位了
					SetBus0State(bBus0SendBit);
					SetBus0SendPin();	
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
						SetBus0SendPin();					
					}
					else
					{	//发送低电平
						ClrBus0State(bBus0SendBit);
						ClrBus0SendPin();
					}
					byBus0SendBuf >>= 1;
				}
			}
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
			if(GetBus0State(bBus0SendFinish|bBus0Disable)==0)
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
					
					byBus0State |= BUS0_REQ_SEND;	

				}
				else
				{
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
uint8_t Bus0OutputData(uint8_t *pbyData)
{
	uint8_t byTemp = BUS0_FREQ_SIZE;
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//没有空间存储了,失败	
		return(FALSE);
	}	
	__disable_irq();
	while(byTemp--)
	{	//数据入发送队列
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	__enable_irq();
	return(TRUE);	
}



//单总线数据接收事物处理
void Bus0Manager(void)
{
	uint8_t i;
	uint8_t rx_buf[7];
	//有完成接收信号
	if (GetBus0State(bBus0RecFinish))
	{
		memcpy((void*)rx_buf,(void*)byBus0RecData,sizeof(STBusFreq));
		ClrBus0State(bBus0RecFinish);
		memcpy((void*)&stBusFreq, rx_buf, sizeof(STBusFreq));
//		for(i=0;i<7;i++)	uart2_send_byte(rx_buf[i]);
		Bus0RecDeal();

	}
	if( GetBus0State(bBus0SendFinish))
	{
		ClrBus0State(bBus0SendFinish);
//		for(i=0;i<7;i++)   uart2_send_byte(byBus0SendData[i]);
		Bus0SendFinishDeal();
	}


	
	//接收队列中有数据未处理
	if(Bus0RxBuffLen() >= BUS0_FREQ_SIZE)
	{  																		
		rx_buf[0] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[1] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[2] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[3] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[4] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[5] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[6] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		if(Bus0RxBuffLen() < BUS0_FREQ_SIZE) byBus0RxHead = byBus0RxTail = 0;
		memcpy((void*)&stBusFreq, rx_buf, sizeof(STBusFreq));
//		for(i=0;i<7;i++)	uart2_send_byte(rx_buf[i]);
		Bus0RecDeal();
	}
}



/*********************************************************************************************************
** Function name:	    eint3Isr
** Descriptions:	    外部中断3服务函数     
** input parameters:    无
** output parameters:   无
** Returned value:      无
**                      Note：EINT3与GPIO共用同一个中断号
*********************************************************************************************************/
void  eint3Isr (void)
{
	uint32_t		i;
	uint32_t		WholeIntState;

	WholeIntState = IOIntStatus;   //整体中断状态
	if(WholeIntState & P0Int)		//IO0至少有一个挂起的中断
	{//p0口
	
		/////////////////////////////////
		i = IO0IntStatR;		 //读取IO0上升沿中断寄存器
		if(i)
		{



		 	IO0IntClr = i;		//上升沿中断状态寄存器清0
		}	
		//////////////////////////////////
		i = IO0IntStatF;		 //读取IO0下降沿中断寄存器
		if(i)
		{
			if(i & Bus0RecPin )
			{	
				reset_start_timer1();
				DisableBus0RecInt();					//禁止再次下降沿中断
				SetBus0State(bBus0StartRec);			//启动起始位沿检测
				ClrBus0State(bBus0Enable);				//禁止总线发送
				byBus0RecCount=0;						//清接收寄存器 			
			}

			IO0IntClr = i;     //下降沿中断状态寄存器清0
		}
	  ////////////////////////////////////////
	}

	
	if(WholeIntState & P2Int)		//IO2至少有一个挂起的中断
	{//p2口
		i = IO2IntStatR;	  //读取IO2上升沿中断寄存器     	//上升沿
		if(i)
		{

			 IO2IntClr = i;				//上升沿中断状态寄存器清0
		}
		//////////////////////////////////////
		i = IO2IntStatF;	   //读取IO2下降沿中断寄存器			//下降沿
		if(i)
		{//有下降沿中断



			IO2IntClr = i;           //  下降沿中断状态寄存器清0

		}
	}
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
	stCallAddr.bySndSecAddr = pstBusFreq->bySndSecAddr;
	stCallAddr.bySndRoomAddr = pstBusFreq->bySndRoomAddr;
	stCallAddr.bySndBedAddr = pstBusFreq->bySndBedAddr;

	stCallAddr.byRecSecAddr = pstBusFreq->byRecSecAddr;
	stCallAddr.byRecRoomAddr = pstBusFreq->byRecRoomAddr;
	stCallAddr.byRecBedAddr = pstBusFreq->byRecBedAddr;	
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
uint8_t AddrCompare(uint8_t *pAddrA,uint8_t *pAddrB)
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
uint8_t DirAddrCompare(uint8_t *pAddrA,uint8_t *pAddrB)
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
void MakeCH0TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount)
{
	stCH0TimerOut.byTimerOutSet = byTimerOut;
	stCH0TimerOut.byTimerOut = byTimerOut;
	stCH0TimerOut.byTimerOutCount = byTimerOutCount;	
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
void MakeCH1TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount)
{
	stCH1TimerOut.byTimerOutSet = byTimerOut;
	stCH1TimerOut.byTimerOut = byTimerOut;
	stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
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
uint8_t AddCallNod(pSTBusFreq pstBusFreq)
{
    pSTNod pstNodTemp;
    pSTNod pstCallNod;


    if(NULL == (uint8_t *)(stCallPointer.pstHead))
    {   //链表为空
		
        pstNodTemp = malloc(sizeof(STNod));
        if(NULL ==(uint8_t *)pstNodTemp)
        {        	
            return(SAVE_FAIL);
        }
        pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= (pstBusFreq->byCmd & 0x1f);   
        pstNodTemp->pstPrev = pstNodTemp->pstNext = NULL;
        stCallPointer.pstTail = stCallPointer.pstHead = pstNodTemp;    	
        return(SAVE_ONLY); 
    }
    //链表不为空,先查找是否有该节点了
    pstCallNod = stCallPointer.pstHead;
    while(NULL != (uint8_t *)pstCallNod)
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
    pstCallNod = stCallPointer.pstTail;
	//查看是否为最后一个节点
	if(((pstCallNod->byCallPriority) & 0xe0) >= (pstBusFreq->byRecSecAddr & 0xe0))
	{	//为最后一个节点
		pstNodTemp = malloc(sizeof(STNod));
        if(NULL ==(uint8_t *)pstNodTemp)
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
		stCallPointer.pstTail = pstNodTemp;	
		if(NULL == (stIndicationData.pstNext))
		{	//正在指示的节点为最后一个节点，将刚才添加的节点添加到后面
			stIndicationData.pstNext = pstNodTemp;
		}	
		return(SAVE_NORMAL);		
	}
	//不是最后一个节点,向前查找
	pstCallNod = pstCallNod->pstPrev;
	while(NULL !=(uint8_t *)pstCallNod)
	{
		if(((pstCallNod->byCallPriority) & 0xe0) < (pstBusFreq->byRecSecAddr & 0xe0))
		{	//继续向前查找
			pstCallNod = pstCallNod->pstPrev;
		}
		else
		{	//找到合适的位置,不为第一个节点，插入节点
			pstNodTemp = malloc(sizeof(STNod));
	        if(NULL == (uint8_t *)pstNodTemp)
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
        	if(pstNodTemp->pstNext == stIndicationData.pstNext)
        	{	//该节点添加到了正在指示的节点的后面，更新正在指示的节点指针信息
        		stIndicationData.pstNext = pstNodTemp;
        	}      	
        	return(SAVE_NORMAL);													
		}
	}
	//该插入的节点优先级为最高优先级，插入到链表的头部
	if(NULL == (uint8_t *)pstCallNod)
	{
		pstNodTemp = malloc(sizeof(STNod));
	    if(NULL ==(uint8_t *)pstNodTemp)
	 	{        	
	 		return(SAVE_FAIL);
		}
		pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= pstBusFreq->byCmd;
        (stCallPointer.pstHead)->pstPrev = pstNodTemp;
        pstNodTemp->pstNext = stCallPointer.pstHead;
        pstNodTemp->pstPrev = NULL;
        stCallPointer.pstHead = pstNodTemp;  
        stIndicationData.byState = STATE_HI;              
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
	pSTNod pstCallNod;
	
	//查找是否有该呼叫纪录
	pstCallNod = stCallPointer.pstHead;
	if((uint8_t *)pstCallNod == NULL)
	{	//没有呼叫纪录,直接退出		
		return;
	}
	//如果删除的该呼叫为正在指示的节点，置正在指示的节点的不存在标志
	if((stIndicationData.byIndicationSecAddr == pstBusFreq->byRecSecAddr) &&
		(stIndicationData.byIndicationRoomAddr == pstBusFreq->byRecRoomAddr) &&
		(stIndicationData.byIndicationBedAddr == pstBusFreq->byRecBedAddr) && 
		((stIndicationData.byCallCmd & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
	{
		stIndicationData.byState = STATE_DEL;
	}
	else
	{	//删除的节点不是正在指示的节点，看是否为后一个节点
		if(((uint8_t *)stIndicationData.pstNext != NULL) && 
			(stIndicationData.pstNext->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
			((stIndicationData.pstNext)->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
			(stIndicationData.pstNext->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
			((stIndicationData.pstNext->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))	
		{	//是正在指示的呼叫的后一个节点，调整指针
			stIndicationData.pstNext = stIndicationData.pstNext->pstNext;
		}	
	}		
	if((pstCallNod->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
      (pstCallNod->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
      (pstCallNod->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
      ((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
 	{	//找到该呼叫信息，并且是最高优先级的
 		if(NULL ==(uint8_t *)pstCallNod->pstNext)
 		{	//该呼叫纪录为唯一的一条纪录
 			stCallPointer.pstHead = stCallPointer.pstTail = NULL; 
 			free(pstCallNod); 			
 			return;			
 		}
 		else
 		{	//不是唯一的一条记录
 			stCallPointer.pstHead = pstCallNod->pstNext;
 			(pstCallNod->pstNext)->pstPrev = NULL;
 			free(pstCallNod);
 			return;
 		}
	}
	//不是第一条记录,继续查找
	pstCallNod = pstCallNod->pstNext;
	while(NULL != (uint8_t *)pstCallNod)
	{
		if((pstCallNod->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
	      (pstCallNod->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
	      (pstCallNod->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
	      ((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
	 	{	//找到该呼叫信息
	 		if(NULL == (uint8_t *)pstCallNod->pstNext)
	 		{	//该呼叫信息为最后一条记录,直接删除最后一个节点
	 			(pstCallNod->pstPrev)->pstNext = NULL;
	 			stCallPointer.pstTail = pstCallNod->pstPrev;	 			
	 		}
	 		else
	 		{	//不是最后一条记录,从链表中删除该节点 
	 			(pstCallNod->pstPrev)->pstNext = pstCallNod->pstNext;
	 			(pstCallNod->pstNext)->pstPrev = pstCallNod->pstPrev;
	 		}
	 		free(pstCallNod);
	 		return;
	 	}
	 	pstCallNod = pstCallNod->pstNext;
	}	
}	



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
void SaveIndicationData(pSTBusFreq pstBusFreq, uint8_t byFirstNod)
{
	//保存信息指示方的地址
	stIndicationData.byIndicationSecAddr = pstBusFreq->bySndSecAddr;
	stIndicationData.byIndicationRoomAddr = pstBusFreq->bySndRoomAddr;
	stIndicationData.byIndicationBedAddr = pstBusFreq->bySndBedAddr;
	stIndicationData.byCallCmd = pstBusFreq->byRecSecAddr;
	stIndicationData.byState = STATE_EXIST;
	if(byFirstNod)
	{	//保存第一个节点信息		
		stIndicationData.pstNext = stCallPointer.pstHead->pstNext;
	}
	else
	{	//保存当前记录的下一条记录			
		stIndicationData.pstNext = stIndicationData.pstNext->pstNext;
	}		
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

	if(GetState(bHostTalk) || GetState(bHostTalk1) || GetDevState(FLAG_NOTE|FLAG_MUSIC) || GetDevState(FLAG_INDICATION)
	    || GetDevState(FLAG_BROADCAST) )
	{
		if(GetState(bHostTalk))
		{//使用SD通道对讲
			phone_tlq_ctrl(0);
			cgb_ctrl(1);
			xtd_ctrl(1);

			cbd_ctrl(0);
			xta_ctrl(0);

			//影响SD通道的其它开关关闭
			kgb_ctrl(0);
			gb_fz_ctrl(0);
		}
		else if(GetState(bHostTalk1))
		{//使用SA通道对讲
			phone_tlq_ctrl(0);
			cbd_ctrl(1);
			xta_ctrl(1);

			cgb_ctrl(0);
			xtd_ctrl(0);

			//影响Sa通道的其它开关关闭
			kbc_ctrl(0);
			kbd_ctrl(0);			
		}
	
		if(uiDevState & (FLAG_NOTE|FLAG_MUSIC))
		{	//背景音乐播放或者提示音状态使用SA通道
			kbd_ctrl(1);

			//影响Sa通道的其它开关关闭
			xta_ctrl(0);
			kbc_ctrl(0);
	
		}

		if(uiDevState & (FLAG_INDICATION))
		{//等待接听或者报号状态使用SD通道
			kgb_ctrl(1);		//通道0打开	

			kbc_ctrl(0);

			//影响SD通道的其它开关关闭
			xtd_ctrl(0);
			gb_fz_ctrl(0);
			
		}

		if(uiDevState & FLAG_BROADCAST)
		{	//广播状态使用SD通道
			phone_tlq_ctrl(1);
			xtd_ctrl(1);
			gb_fz_ctrl(1);

			xta_ctrl(0);
			//影响SD通道的其它开关关闭
			kgb_ctrl(0);

		} 

	} 
	else
	{
	//其它状态，全部关闭
		phone_tlq_ctrl(1);
		xta_ctrl(0);
		xtd_ctrl(0);
		
		kbd_ctrl(0);
		kbc_ctrl(0);
		kgb_ctrl(0);		
		gb_fz_ctrl(0);	

		cgb_ctrl(0);
		cbd_ctrl(0);

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
	pSTNod pstNod;
	STAddr_S source_addr;
	uint8_t data_buff[2];

	if(uiDevState & (FLAG_BUSY|FLAG_INDICATION_EXIST))
	{	//如果有呼叫指示的命令正在发送或者忙，直接退出
		return;
	}
	//检查上次报号记录
	if(STATE_HI == (stIndicationData.byState))
	{	//有更高优先级的呼叫存在，直接从第一条呼叫开始			
FeltchFirst:
		//取出第一条记录
		pstNod = stCallPointer.pstHead;
		if(NULL ==(uint8_t *)pstNod)
		{	//没有呼叫记录了，直接退出
		
			if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
			{//没有通话存在
			   	memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				data_buff[0] = 0x00;
	    		Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态
	    		wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);

				AM79R70_standby();
			}		
			return;
		}										
		stBusFreq.bySndSecAddr = pstNod->byCallSecAddr;
		stBusFreq.bySndRoomAddr = pstNod->byCallRoomAddr;
		stBusFreq.bySndBedAddr = pstNod->byCallBedAddr;
		stBusFreq.byCmd = CMD_INFO_INDICATION;
		stBusFreq.byRecSecAddr = pstNod->byCallPriority;
		stBusFreq.byRecRoomAddr = 0x00;
		stBusFreq.byRecBedAddr = 0x00;					
		if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
		{
			stBusFreq.byCmd = pstNod->byCallPriority & 0x1f;
			SaveIndicationData(&(stBusFreq), true);
			uiDevState |= FLAG_INDICATION_EXIST;
		}
		return;	
	}
	else
	{	//没有更高优先级呼叫存在
		//看是否还有呼叫记录
		pstNod = stCallPointer.pstHead;
		if(NULL ==(uint8_t *)pstNod)
		{	//没有呼叫记录了，直接退出
			if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
			{//没有通话存在
			   	memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				data_buff[0] = 0x00;
	    		Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态	
	    		wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态

				set_play_voice_volue(PLAY_HOST_CH,0);
				set_play_voice_volue(PLAY_SLAVE_CH,0);

				AM79R70_standby();
			}
			return;
		}	
		else
		{	//还有呼叫记录
			pstNod = stIndicationData.pstNext;
			if(NULL == (uint8_t *)pstNod)
			{	//后面没有呼叫了，直接取第一条呼叫
				goto FeltchFirst;
			}
			else
			{	//后面还有呼叫
				if((stIndicationData.byCallCmd & 0xe0)== (pstNod->byCallPriority & 0xe0))
				{	//是同一个优先级的呼叫，取出来																			
					stBusFreq.bySndSecAddr = pstNod->byCallSecAddr;
					stBusFreq.bySndRoomAddr = pstNod->byCallRoomAddr;
					stBusFreq.bySndBedAddr = pstNod->byCallBedAddr;
					stBusFreq.byCmd = CMD_INFO_INDICATION;
					stBusFreq.byRecSecAddr = pstNod->byCallPriority;
					stBusFreq.byRecRoomAddr = 0x00;
					stBusFreq.byRecBedAddr = 0x00;										
					if(true == Bus0OutputData(&stBusFreq.bySndSecAddr))
					{
						stBusFreq.byCmd = pstNod->byCallPriority & 0x1f;
						SaveIndicationData(&(stBusFreq), false);
						uiDevState |= FLAG_INDICATION_EXIST;
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


void LedStateFlash(void)
{


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
	static uint8_t byTemp;
	static uint8_t byCmd;
	static uint8_t byPriority;	
	pSTUartFreq pstDataFreq;
	STAddr_S  source_addr;

	uint8_t data_buff[10];
	//复位看门狗                                                                                                                                                                                                                                                                                     
#ifdef	enable_wdt
	feed_wdt();
#endif


	//数据处理	
	switch(stBusFreq.byCmd)
	{
		case CMD_LANDING:									//登记命令,返回确认命令
			stBusFreq.byRecSecAddr = stBusFreq.bySndSecAddr;
			stBusFreq.byRecRoomAddr = stBusFreq.bySndRoomAddr;
			stBusFreq.byRecBedAddr = stBusFreq.bySndBedAddr;
			stBusFreq.byCmd = CMD_ENTER;
			if(uiDevState & FLAG_BUSY)
			{
				stBusFreq.bySndSecAddr = 0x80;
			}
			else
			{
				stBusFreq.bySndSecAddr = 0x00;
			}
			if(GetState(bPCOpen))
			{
				stBusFreq.bySndSecAddr |= 0x40;
			}
			
			stBusFreq.bySndRoomAddr = 0x00;
			stBusFreq.bySndBedAddr = 0x00;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
			
		case CMD_INSPECTOR_CALL:							//查房呼叫命令,返回确认命令给上位机
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]= stBusFreq.bySndBedAddr;
			memcpy(&(data_buff[1]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,CMD_INSPECTOR_CALL,4,data_buff);
			wifi_send_cmd(source_addr,CMD_INSPECTOR_CALL,4,data_buff);
			
			stBusFreq.byCmd = CMD_INSPERCTOR_ENTER;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));			
			break;
		case CMD_INFUSION_CALL:								//输液呼叫命令
		case CMD_SERVICE_CALL:								//服务呼叫命令
		case CMD_EMERGENCY_CALL:							//紧急呼叫命令
		case CMD_HELP_CALL:									//求援呼叫命令
			if((stBusFreq.bySndRoomAddr == ADD_BED_FJ)&&(stBusFreq.bySndSecAddr!=stEepromCfgData.bySelfSecAddr))
			{//是加床分机发过来的呼叫且区号不对 ,直接返回
				return;
			}
			//保存命令和优先级
			byCmd = stBusFreq.byCmd;
			byPriority = stBusFreq.byRecSecAddr & 0xe0;
			byTemp = AddCallNod(&(stBusFreq));
			if(SAVE_FAIL == byTemp)
			{	//添加呼叫队列失败，直接返回
				return;
			}	
			//其他类型，都是添加成功，返回确认命令
			stBusFreq.byRecSecAddr = stBusFreq.bySndSecAddr;
			stBusFreq.byRecRoomAddr = stBusFreq.bySndRoomAddr;
			stBusFreq.byRecBedAddr = stBusFreq.bySndBedAddr;
			
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = byCmd + 0x1b;
		
			Bus0OutputData(&(stBusFreq.bySndSecAddr));	
		
			if(GetState(bOriginMusic))
			{//之前在播放背景音乐,停止
				stBusFreq.bySndSecAddr = stBusFreq.byRecSecAddr;
				stBusFreq.bySndRoomAddr = stBusFreq.byRecRoomAddr;
				stBusFreq.bySndBedAddr = stBusFreq.byRecBedAddr;			
				stBusFreq.byCmd = CMD_STOP_VOICE;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
		
				stBusFreq.byRecSecAddr = stBusFreq.bySndSecAddr;
				stBusFreq.byRecRoomAddr = stBusFreq.bySndRoomAddr;
				stBusFreq.byRecBedAddr = stBusFreq.bySndBedAddr;				
			}
			
			if(uiDevState & (FLAG_INDICATION_EXIST|FLAG_BUSY|FLAG_TALK1))
			{	//有呼叫指示命令正在发送队列中,但没有发送完或者处于忙状态,退出
				return;
			}
			//没有指示正在发送,也不忙
			if(uiDevState & FLAG_INDICATION)
			{	//有呼叫正在指示中
				if(SAVE_PRIO_HI == byTemp)
				{	//有高优先级呼叫存在,发送终止指示命令
					if(uiDevState & FLAG_STOP_EXIST)
					{	//有正在发送停止命令
						return;
					}
					stBusFreq.byCmd = CMD_STOP_INDICATION;						
					if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
					{
						uiDevState |= FLAG_STOP_EXIST;
					}								
				}
			}
			else
			{	//没有呼叫在指示中,直接发送指示命令,保存呼叫指示的指针位置
				if(SAVE_ONLY == byTemp)
				{	//该呼叫是唯一一条呼叫记录
					if(uiDevState & FLAG_STOP_EXIST)
					{	//有正在发送停止命令
						return;
					}
					stBusFreq.bySndSecAddr = stBusFreq.byRecSecAddr;
					stBusFreq.bySndRoomAddr = stBusFreq.byRecRoomAddr;
					stBusFreq.bySndBedAddr = stBusFreq.byRecBedAddr;
					stBusFreq.byCmd = CMD_INFO_INDICATION;
					stBusFreq.byRecSecAddr = byPriority|byCmd;													
					if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
					{	//置指示正在发送队列中标志
						//保存指示方地址、命令、优先级和正在指示的呼叫记录指针
						stBusFreq.byCmd = byCmd;
						SaveIndicationData(&(stBusFreq), true);	
						uiDevState |= FLAG_INDICATION_EXIST;
					}
					return;
				}
				else
				{	//有多条呼叫记录存在,但是没有FLAG_INDICATION_EXIST,
					//FLAG_BUSY,FLAG_INDICATION标志,表明指示发送完成后呼叫信息不存在了
					//停止信息指示正在发送,发送完成后再处理 	
					if(uiDevState & FLAG_STOP_EXIST)
					{	//有正在发送停止命令
						return;
					}
					byCmd =stCallPointer.pstHead->byCallPriority & 0x1f;															
					stBusFreq.bySndSecAddr = stCallPointer.pstHead->byCallSecAddr;
					stBusFreq.bySndRoomAddr = stCallPointer.pstHead->byCallRoomAddr;
					stBusFreq.bySndBedAddr = stCallPointer.pstHead->byCallBedAddr;
					stBusFreq.byCmd = CMD_INFO_INDICATION;
					stBusFreq.byRecSecAddr = 
					stCallPointer.pstHead->byCallPriority;
					if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
					{	//保存指示方地址和命令
						stBusFreq.byCmd = byCmd;
						SaveIndicationData(&(stBusFreq), true);
						uiDevState |= FLAG_INDICATION_EXIST;
					}
					return; 
				}								
			}			
			break;		
		case CMD_INFUSION_CLEAR:							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR: 							//清除服务呼叫命令
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫命令
		case CMD_HELP_CLEAR:								//清除求援呼叫命令
			if((stBusFreq.bySndRoomAddr == MOVE_FJ)&&
			   (stBusFreq.bySndSecAddr!=stEepromCfgData.bySelfSecAddr))
			{//是移动分机发过来的清除呼叫且区号不对 ,直接返回
				return;
			}		
			if( (stBusFreq.byRecSecAddr == stBusFreq.bySndSecAddr) &&
				(stBusFreq.byRecRoomAddr == stBusFreq.bySndRoomAddr) &&
				(stBusFreq.byRecBedAddr == stBusFreq.bySndBedAddr) )
			{//是呼叫分机自己挂断	
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); //主呼分机地址
				memcpy(&(data_buff[3]),&(stBusFreq.bySndSecAddr),3); //主呼分机地址
				Usart2SndCmd(source_addr,stBusFreq.byCmd,6,data_buff);
				wifi_send_cmd(source_addr,stBusFreq.byCmd,6,data_buff);
			}
			else
			{//是其它处理分机挂断分机
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); //主呼分机地址
				memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3); //主呼分机地址
				Usart2SndCmd(source_addr,stBusFreq.byCmd,6,data_buff);
				wifi_send_cmd(source_addr,stBusFreq.byCmd,6,data_buff);
					
			}
			//清呼叫记录
			stBusFreq.byCmd -= 0x0a; 		//将清除命令还原成对应的呼叫命令		
			RemoveCallNod(&(stBusFreq)); 		
			if((uiDevState & FLAG_INDICATION) && 
				(stBusFreq.byRecSecAddr == stIndicationData.byIndicationSecAddr) &&
				(stBusFreq.byRecRoomAddr == stIndicationData.byIndicationRoomAddr) &&
				(stBusFreq.byRecBedAddr == stIndicationData.byIndicationBedAddr) &&
				((stBusFreq.byCmd & 0x1f) == (stIndicationData.byCallCmd & 0x1f)))
			{	//正在呼叫指示，且清除的呼叫为正在指示的呼叫,发送终止呼叫指示命令
				if(uiDevState & FLAG_STOP_EXIST)
				{	//有正在发送停止命令
					return;
				}
				//没有停止指示正在发送中				
				stBusFreq.byCmd = CMD_STOP_INDICATION;				
				if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
				{
					uiDevState |= FLAG_STOP_EXIST;
				}							
			}
	
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{//没有分机存在
				if(GetState(bOriginMusic))
				{//之前有播放背景音乐
				   stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stBusFreq.byRecSecAddr =0xff;
				   stBusFreq.byRecRoomAddr = 0xff;
				   stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stBusFreq.bySndSecAddr));					   
				}
			}
			break;	
		case CMD_INFUSION_ANSWER:							//处理输液呼叫
		case CMD_SERVICE_ANSWER:							//处理服务呼叫
		case CMD_EMERGENCY_ANSWER:							//处理紧急呼叫
		case CMD_HELP_ANSWER:								//处理求援呼叫	
			if(uiDevState & FLAG_BUSY)
			{	//忙，退出
				break;
			}
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stBusFreq));			
			stCallAddr.byCmd = stBusFreq.byCmd;


			//清呼叫记录
			stBusFreq.byCmd -= 0x06; 		//将处理命令还原成对应的呼叫命令
			RemoveCallNod(&(stBusFreq)); 		
			//置等待应答和忙标志
			uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY); 
			if(uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				uiDevState &= ~FLAG_INDICATION;
				
				//停止报号			
				StopVoicePlay();

				//语音通道切换
				VoiceChannelCtx();	
			}			

			//设置等待应答超时1S
			MakeCH0TimerOut(50, 0); 		
			break;
	
				
		case CMD_COMM_CALL: 								//普通呼叫命令
			if(uiDevState & FLAG_BUSY)
			{	//忙，直接退出
				break;
			}
			//置忙和等待应答标志 
			uiDevState |= (FLAG_BUSY|FLAG_WAIT_ACK); 		
			if(uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				uiDevState &= ~FLAG_INDICATION;				
				//停止报号			
				StopVoicePlay();

				//语音通道切换
				VoiceChannelCtx();								
			}
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stBusFreq));			
			
			//设置等待应答超时5S
			MakeCH0TimerOut(50, 5);
			break;
		case CMD_COMM_ANSWER:								//应答命令
			if(0x0000 == (uiDevState & FLAG_BUSY))
			{	//不忙，发送系统复位命令
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				break;
			}

			if(AddrCompare(&(stBusFreq.bySndSecAddr),&(stCallAddr.byRecSecAddr)))
			{//是对应分机的应答信号
				//忙,清等待应答标志 				
				uiDevState &= ~FLAG_WAIT_ACK;			
				//置等待接听标志			
				uiDevState |= FLAG_WAIT_LISTEN;	

				//设置等待接听超时15S
				MakeCH0TimerOut(250, 3);	

			}
			else MakeCH0TimerOut(1, 0); //使通道0超时时间到
			break;
	
		case CMD_CALL_LISTEN:							//接听命令
			if(0x0000 == (uiDevState & FLAG_BUSY))
			{	//不忙，发送系统复位命令
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&stBusFreq.bySndSecAddr);
				break;
			}
	
			if(AddrCompare(&(stBusFreq.bySndSecAddr),&(stCallAddr.byRecSecAddr)))
			{//是对应分机的接听信号 		
				//忙,清等待接听标志，置通话标志 				
				uiDevState &= ~FLAG_WAIT_LISTEN; 		
				uiDevState |= FLAG_TALK;

				if(DirAddrCompare(&(stEepromCfgData.bySelfSecAddr),&(stBusFreq.byRecSecAddr)))
				{
					SetState(bHostTalk);
				}

				//语音通道切换
				VoiceChannelCtx();
				//设置等待通话超时60S
				MakeCH0TimerOut(250, 12);	

				
				if(0x0000 == (uiDevState & (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC)))
				{	//没有通话1，语音提示，背景音乐播放存在，发送切换语音通道命令								
					stBusFreq.byCmd = CMD_CHANNEL_CHANGE;								
					Bus0OutputData(&(stBusFreq.bySndSecAddr));
					//置通道切换命令在总线队列中标志
					uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;

				}

				if((stBusFreq.byRecSecAddr == stEepromCfgData.bySelfSecAddr)&&
					(stBusFreq.byRecRoomAddr == stEepromCfgData.bySelfRoomAddr)&&
					(stBusFreq.byRecBedAddr == stEepromCfgData.bySelfBedAddr))
				{	//是管理主机处理床头分机的呼叫

					AM79R70_active();
						
					set_talk_voice_volue(TALK_HOST_CH,stEepromCfgData.byHostTalkVol);
					set_talk_voice_volue(TALK_SLAVE_CH,stEepromCfgData.bySlaTalkVol);

					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0] = stEepromCfgData.bySelfSecAddr;
					data_buff[1] = PC_ADDR2;
					data_buff[2] = PC_ADDR3;
					data_buff[3] = stBusFreq.bySndSecAddr;
					data_buff[4] = stBusFreq.bySndRoomAddr;
					data_buff[5] = stBusFreq.bySndBedAddr;
					wifi_send_cmd(source_addr,CMD_INFUSION_ANSWER,6,data_buff);
					
					data_buff[0] = CMD_INFUSION_ANSWER;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
					scon2_Rcvbuf[CMD_POSITION]=0x00;	//清空原命令数据
				}
				else
				{	//是其他分机处理床头分机的呼叫
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);				
					memcpy(&(data_buff[0]),&(stBusFreq.byRecSecAddr),3); //处理分机地址
					memcpy(&(data_buff[3]),&(stBusFreq.bySndSecAddr),3); //主呼分机地址	
					Usart2SndCmd(source_addr,stCallAddr.byCmd,6,data_buff);
					wifi_send_cmd(source_addr,stCallAddr.byCmd,6,data_buff);
				}
			}
			else MakeCH0TimerOut(1, 0); //使通道0超时时间到
			break;		
		case CMD_BROADCAST1:								
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:								//广播命令
			if(uiDevState & FLAG_BUSY)
			{	//忙，发送系统复位命令
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				break;
			}
			//不忙，置忙标志和广播标志
			uiDevState |= (FLAG_BUSY|FLAG_BROADCAST);
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stBusFreq));

			if(uiDevState & FLAG_INDICATION)
			{
				//清呼叫指示标志
				uiDevState &= ~FLAG_INDICATION;
				
				//停止报号			
				StopVoicePlay();
			}

			//语音通道切换
			VoiceChannelCtx();
			//设置等待广播超时180S
			MakeCH0TimerOut(250, 36);
			break;	
		case CMD_SYSTERM_RESET: 							//系统复位命令

			if(uiDevState & FLAG_INDICATION)
			{
				uiDevState &= ~FLAG_INDICATION;
	//			//停止报号
				StopVoicePlay();				
			}
			//清标志 //这些标志之外的标志被清0,这些标志位保持原态不变
			uiDevState &= (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_INDICATION_EXIST|FLAG_STOP_EXIST);
			if(GetState(bHostTalk))
			{
				ClrState(bHostTalk);
				set_talk_voice_volue(TALK_HOST_CH,0);
				set_talk_voice_volue(TALK_SLAVE_CH,0);				
			}
			
			//语音通道切换
			VoiceChannelCtx();
			//清超时
			MakeCH0TimerOut(0, 0);

			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); 
			memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			wifi_send_cmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			
			//重新开始报号	
			if(0x00 == (uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
			{
				RestartIndication();
			}	

			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{//没有分机存在
				if(GetState(bOriginMusic))
				{//之前有播放背景音乐
				   stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stBusFreq.byRecSecAddr =0xff;
				   stBusFreq.byRecRoomAddr = 0xff;
				   stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stBusFreq.bySndSecAddr));			   
				}
			}	
				
			break;
		case CMD_CHANNEL_CLOSE: 							//关闭语音通道命令
			uiDevState &= ~FLAG_TALK1;
			if(GetState(bHostTalk1))
			{
				ClrState(bHostTalk1);
				set_talk_voice_volue(TALK_HOST_CH,0);
				set_talk_voice_volue(TALK_SLAVE_CH,0);	
			}
			
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{	//没有呼叫记录了
			
				if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//没有通话存在
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0] = 0x00;	//系统不忙
					Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态
					wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态
				}	
			}	
	
			//语音通道切换
			VoiceChannelCtx();
			//清通道1通话超时
			MakeCH1TimerOut(0, 0);

			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); 
			memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			wifi_send_cmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			
			if(uiDevState & FLAG_TALK)
			{	//通道0通话存在，发送切换语音通道命令				
				stBusFreq.byCmd = CMD_CHANNEL_CHANGE;
				//置通道切换命令在总线队列中标志
				uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}


			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{//没有分机存在
				if(GetState(bOriginMusic))
				{//之前有播放背景音乐
				   stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stBusFreq.byRecSecAddr =0xff;
				   stBusFreq.byRecRoomAddr = 0xff;
				   stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stBusFreq.bySndSecAddr));			   
				}
			}	

			else
			{
				RestartIndication();
			}
			break;	
	
				
		case CMD_DATE_SEND:
			stTime.bySecond=0x00;
			stTime.byMinute = stBusFreq.bySndRoomAddr;
			stTime.byHour	= stBusFreq.bySndBedAddr;
			memcpy(&(stTime.byDay),&(stBusFreq.byRecSecAddr),3);
			WriteDS3231_time();
			break;	
				
	   case CMD_BUS_ANSWER: //总线通用应答命令
		   switch(stBusFreq.byRecSecAddr)
			{
			case CMD_COLOR_CLEAR:
//			case CMD_BL_TIMER_SET:
//			case CMD_VOICE_TIMER_SET:
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				byWaitCommAckDTime = 0;
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0]		=stBusFreq.byRecSecAddr;
				data_buff[1]		=TRUE;		//成功或失败
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
				return;
				
			case CMD_DISPLAY_DATA:
			case CMD_HANDLE_DISPLAY_DATA:	
				if((stBusFreq.bySndBedAddr == stRS485Addr.byBedAddr))
				{//床号相等
					byWaitRS485AckDTime = 0;
					memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
					data_buff[0]		=stBusFreq.byRecSecAddr;
					data_buff[1]		=TRUE;		//成功或失败
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
					stRS485Addr.byBedAddr=0; 	//结果发完，将485床号清零

				}
				return;

			default:
				break;
		   	}	
		   
	   		if((stBusFreq.bySndBedAddr == stCommAddr.byBedAddr)||(stBusFreq.bySndBedAddr == 0xff) ||(stCommAddr.byBedAddr == 0xff) )
	   		{
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				byWaitCommAckDTime = 0;
				switch(stBusFreq.byRecSecAddr)
				{
				case CMD_POWER_ON:
				case CMD_ONLINE_WRITE:
				case CMD_SCHEDULE_SEND:
				case CMD_OPEN_LCD:
				case CMD_CLOSE_LCD:
				case CMD_CLEAR_LCD:
				case CMD_CLEAR_HANDLE_LCD:
				case CMD_CLEAR_MK_LCD:	
					memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
					data_buff[0]		=stBusFreq.byRecSecAddr;
					data_buff[1]		=TRUE;		//成功或失败
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
					break;
				default:
					break;
				}
	   		
	   		}
			else byWaitCommAckDTime = 1;	//地址不区配,提前退出
			break;
		case CMD_DATA_ERROR:		//一帧数据出错
			//清等待应答超时
			if(stBusFreq.bySndBedAddr == stRS485Addr.byBedAddr)
			{//床号相等			
	             byWaitRS485AckDTime = 0;	
				 memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
				 data_buff[0] = byUsart0SndBuf[CMD_POSITION];
				 data_buff[1] = FALSE;
				 Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
				 wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
				 stRS485Addr.byBedAddr = 0;
			}
			break;
		case CMD_ONLINE_OUTPUT:
	   		if((stBusFreq.bySndSecAddr	==stCommAddr.bySecAddr)&&
				(stBusFreq.bySndRoomAddr==stCommAddr.byRoomAddr)&&
				(stBusFreq.bySndBedAddr	==stCommAddr.byBedAddr))
	   		{
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				byWaitCommAckDTime = 0;
				memcpy(&(source_addr.bySecAddr),&(stBusFreq.bySndSecAddr),3);
				data_buff[0]		=CMD_ONLINE_READ;
				data_buff[1]		=TRUE;		//成功或失败
				data_buff[2]		=stBusFreq.byRecSecAddr;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,3,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,3,data_buff);	
	   		}
			else byWaitCommAckDTime = 1;	//地址不区配,提前退出
			break;

		case CMD_QUEST_ANSWER:
	   		if((stBusFreq.bySndSecAddr	==stCommAddr.bySecAddr)&&
				(stBusFreq.bySndRoomAddr==stCommAddr.byRoomAddr)&&
				(stBusFreq.bySndBedAddr	==stCommAddr.byBedAddr))
	   		{
				//uiDevState &= ~(FLAG_WAIT_ACK|FLAG_BUSY);
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//清等待总线应答超时
				byWaitCommAckDTime = 0;

				memcpy(&(source_addr.bySecAddr),&(stBusFreq.bySndSecAddr),3);
				data_buff[0]		=scon2_Rcvbuf[CMD_POSITION];
				data_buff[1]		=TRUE;		//成功或失败
				memcpy(&(data_buff[2]),&(stBusFreq.byRecSecAddr),3);	//附带3个参数
				Usart2SndCmd(source_addr,CMD_RESULT_PC,5,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,5,data_buff);
	   		}
			else byWaitCommAckDTime = 1;	//地址不区配,提前退出
			break;

		case CMD_NURSE_IN:
		case CMD_NURSE_OUT:

			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]= stBusFreq.bySndBedAddr;						//移动分机号
			memcpy(&(data_buff[1]),&(stBusFreq.byRecSecAddr),3);		//床头分机号
			Usart2SndCmd(source_addr,stBusFreq.byCmd,4,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,4,data_buff);
			break;

		case CMD_SUPPLY_OX_START:			
		case CMD_SUPPLY_OX_END:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3);		//床头分机号
			data_buff[3]=stBusFreq.byRecRoomAddr;						//小时
			data_buff[4]=stBusFreq.byRecBedAddr;						//分钟
			Usart2SndCmd(source_addr,stBusFreq.byCmd,5,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,5,data_buff);
			break;

		case CMD_VOICE_TIMER_SET:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]=stBusFreq.bySndSecAddr;		//开如小时
			data_buff[1]=stBusFreq.bySndRoomAddr;		
			data_buff[2]=stBusFreq.byRecSecAddr;		//结束小时
			data_buff[3]=stBusFreq.byRecRoomAddr;
			data_buff[4]=stBusFreq.byRecBedAddr;		//小音量值
			Usart2SndCmd(source_addr,stBusFreq.byCmd,5,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,5,data_buff);
			break;

		case CMD_BL_TIMER_SET:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]=stBusFreq.bySndSecAddr;		//开如小时
			data_buff[1]=stBusFreq.bySndRoomAddr;		
			data_buff[2]=stBusFreq.byRecSecAddr;		//结束小时
			data_buff[3]=stBusFreq.byRecRoomAddr;
			Usart2SndCmd(source_addr,stBusFreq.byCmd,4,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,4,data_buff);
			break;

			
		case CMD_COLOR_CLEAR:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]=stBusFreq.bySndSecAddr;		//开如小时
			data_buff[1]=stBusFreq.bySndRoomAddr;		
			data_buff[2]=stBusFreq.bySndBedAddr;		
			Usart2SndCmd(source_addr,stBusFreq.byCmd,3,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,3,data_buff);
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
	STAddr_S  source_addr;
	uint8_t   data_buff[10];

#ifdef	enable_wdt
    feed_wdt();
#endif
	
	__disable_irq();
	memcpy(&(stBusFreq.bySndSecAddr), byBus0SendData, sizeof(STBusFreq));	//获取单总线数据
	__enable_irq();

//	uart2_send_str2(&(stBusFreq.bySndSecAddr),7);


	//数据处理	
	switch(stBusFreq.byCmd)
	{
		case CMD_POWER_ON:									//上电复位命令发送完成
			if(uiDevState & FLAG_LANDING)
			{	//确实处在上电状态
				uiDevState &= ~FLAG_LANDING;
				uiDetectPcTime = 250;
			}
			else
			{
				//置等待应答和忙标志
				uiDevState |= (FLAG_WAIT_COMM_ACK);
				//保存分机地址
				stCommAddr.bySecAddr = 	stBusFreq.byRecSecAddr;
				stCommAddr.byRoomAddr = stBusFreq.byRecRoomAddr;
				stCommAddr.byBedAddr = 	stBusFreq.byRecBedAddr;
				//设置等待应答超时1S
				byWaitCommAckDTime = 50;				
			}
			break;
		case CMD_INFO_INDICATION:							//呼叫指示命令			
			uiDevState &= ~FLAG_INDICATION_EXIST;
			if(uiDevState & FLAG_BUSY)
			{	//忙，退出				
				break;
			}			
			//检查该记录			
			if((stIndicationData.byState == STATE_HI) ||
				(stIndicationData.byState == STATE_DEL))
			{	//有高优先级呼叫存在或者该呼叫已经不存在了，发送终止呼叫命令
				if(uiDevState & FLAG_STOP_EXIST)
				{	//有正在发送停止命令
					return;
				}
				stBusFreq.byCmd = CMD_STOP_INDICATION;						
				if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
				{
					uiDevState |= FLAG_STOP_EXIST;
				}				
			}
			else
			{	//该呼叫还存在于记录中,且没有高优先级的呼叫存在,置指示标志
				uiDevState |= FLAG_INDICATION;
				
				//构建语音信息
				if(MakeCallVoiceData(&(stIndicationData)))	
				{
					//播放语音
					check_little_voice_status();
					if(little_voice_effective==1)
					{//小音量时段
/*						set_play_voice_volue(PLAY_HOST_CH,(stEepromCfgData.byHostPlayLittleVol+1)*3);
						set_play_voice_volue(PLAY_SLAVE_CH,(stEepromCfgData.bySlaPlayLittleVol+1)*3);  */

						set_play_voice_volue(PLAY_HOST_CH, (stEepromCfgData.byLVoiceVal+1)*3);
						set_play_voice_volue(PLAY_SLAVE_CH,(stEepromCfgData.byLVoiceVal+1)*3);

						
					}
					else
					{
						set_play_voice_volue(PLAY_HOST_CH,(stEepromCfgData.byHostPlayVol+1)*3);
						set_play_voice_volue(PLAY_SLAVE_CH,(stEepromCfgData.bySlaPlayVol+1)*3);					
					}
					played_times = 0;
					played_turns =0;
					StartVoicePlay();
					start_tel_ring();
				}

				//语音通道切换
				VoiceChannelCtx();
				//设置报号超时15S
				MakeCH0TimerOut(250, 3);
				
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				data_buff[0]	= stBusFreq.bySndSecAddr;
				data_buff[1]	= stBusFreq.bySndRoomAddr;
				data_buff[2] 	= stBusFreq.bySndBedAddr;
				data_buff[3] 	= (stBusFreq.byRecSecAddr&0x1f);	//呼叫类型
				data_buff[4]	= (stBusFreq.byRecSecAddr&0xe0)>>5;	//优先级
				wifi_send_cmd(source_addr,CMD_INFO_INDICATION,5,data_buff);
				
			}							
			break;

			
		case CMD_STOP_INDICATION:							//终止呼叫指示命令			
			//清指示标志
			uiDevState &= ~(FLAG_INDICATION|FLAG_STOP_EXIST);
			//清报号超时
			MakeCH0TimerOut(0, 0);
			
			//停止报号
			StopVoicePlay();
			stop_tel_ring();

			//语音通道切换
			VoiceChannelCtx();			
			//不忙，且没有呼叫指示正在发送队列中,重新开始报号			
			RestartIndication();			
			break;	
		case CMD_SYSTERM_RESET:								//系统复位命令
			if(uiDevState & FLAG_INDICATION)
			{
				uiDevState &= ~FLAG_INDICATION;
	//			//停止报号
				StopVoicePlay();				
			}
			
			uiDevState &= (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_INDICATION_EXIST|FLAG_STOP_EXIST);	// 将此几位保持原状态,其它标志清0		
			
			//语音通道切换
			VoiceChannelCtx();
			//清超时
			MakeCH0TimerOut(0, 0);
			
			//重新开始报号
			if(0x00 == (uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
			{
				RestartIndication();
			}									
			break;	
		case CMD_START_VOICE:								//语音提示命令
			//清通道1通话标志和超时等待
			uiDevState &= ~FLAG_TALK1;			
			MakeCH1TimerOut(0, 0);
			//置语音提醒标志
			uiDevState |= FLAG_NOTE;
			//语音通道切换
			VoiceChannelCtx();
			
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = 0xff;
			source_addr.byBedAddr = 0xff;

			data_buff[0]		=CMD_START_VOICE;
			data_buff[1]		=TRUE;		//成功或失败
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			break;

			
		case CMD_MUSIC_PLAY:								//背景音乐播放标志
			//清通道1通话标志和超时等待
			uiDevState &= ~FLAG_TALK1;				
			MakeCH1TimerOut(0, 0);
			//置背景音乐标志
			uiDevState |= FLAG_MUSIC;
			//语音通道切换
			VoiceChannelCtx();

			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = 0xff;
			source_addr.byBedAddr = 0xff;
			data_buff[0]		=CMD_MUSIC_PLAY;
			data_buff[1]		=TRUE;		//成功或失败
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			break;
			
		case CMD_STOP_VOICE:
			//清播音和背景音乐标志
			uiDevState &= ~(FLAG_NOTE|FLAG_MUSIC);
			//语音通道切换
			VoiceChannelCtx();

			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0]		=CMD_STOP_VOICE;
			data_buff[1]		=TRUE;		//成功或失败
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{	//没有呼叫记录了，直接退出
			
				if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//没有通话存在
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0] =0x00;	//系统不忙
					Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态
					wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//报告系统不忙状态
				}	
			}			
			break;	
		case CMD_INFUSION_CLEAR:							//清除输液呼叫命令
		case CMD_SERVICE_CLEAR:								//清除服务呼叫
		case CMD_HELP_CLEAR:								//清除求援呼叫
		case CMD_EMERGENCY_CLEAR:							//清除紧急呼叫
			//暂存原命令数据
			byChar = stBusFreq.byCmd;	
			
			//清呼叫记录
			stBusFreq.byCmd -= 0x0a;			//将清除命令还原成对应的呼叫命令		
			RemoveCallNod(&(stBusFreq));	

			memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
			data_buff[0]		=byChar;	//原命令
			data_buff[1]		=TRUE;		//成功或失败
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);	
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);


			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0] = stEepromCfgData.bySelfSecAddr;
			data_buff[1] = PC_ADDR2;
			data_buff[2] = PC_ADDR3;
			memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3);
			wifi_send_cmd(source_addr,byChar,6,data_buff);
			
			if((uiDevState & FLAG_INDICATION) && 
				(stBusFreq.byRecSecAddr == stIndicationData.byIndicationSecAddr) &&
				(stBusFreq.byRecRoomAddr == stIndicationData.byIndicationRoomAddr) &&
				(stBusFreq.byRecBedAddr == stIndicationData.byIndicationBedAddr) &&
				(stBusFreq.byCmd == (stIndicationData.byCallCmd & 0x1f)))
			{	//正在呼叫指示,且清除的呼叫为正在指示的呼叫,清报号标志
				uiDevState &= ~FLAG_INDICATION;
				
				//停止报号
				StopVoicePlay();

				//语音通道切换
				VoiceChannelCtx();
				//清超时
				MakeCH0TimerOut(0, 0);				
				//重新开始报号
				if(0x00 == (uiDevState&FLAG_STOP_EXIST))
				{
					RestartIndication();
				}
			}			
			break;	

			
		case CMD_CHANNEL_CHANGE:							//通道切换命令
			if(uiDevState & FLAG_TALK)
			{	//通话状态
				uiDevState &= ~(FLAG_BUSY|FLAG_TALK);

				//将通道0的主动呼叫方地址复制到通道1的主动呼叫方地址中
				memcpy(&(stCH1CallAddr),&(stCallAddr),sizeof(STAddr));
				//将通道0的主动呼叫方地址清0
				stCallAddr.bySndSecAddr = 0x00;
				stCallAddr.bySndRoomAddr = 0x00;
				stCallAddr.bySndBedAddr = 0x00;
				stCallAddr.byRecSecAddr = 0x00;
				stCallAddr.byRecRoomAddr = 0x00;
				stCallAddr.byRecBedAddr = 0x00;				
				//清通道切换在总线队列中标志
				uiDevState &= ~(FLAG_CHANNEL_CHANGE_EXIST);


				//清超时
				MakeCH0TimerOut(0, 0);	
				//置通话1标志
				uiDevState |= FLAG_TALK1;	

				if(DirAddrCompare(&(stEepromCfgData.bySelfSecAddr),&(stCH1CallAddr.bySndSecAddr)))
				{
					ClrState(bHostTalk);
					SetState(bHostTalk1);
				}				
				//语音通道切换
				VoiceChannelCtx();
				//通道1通话定时60S
				MakeCH1TimerOut(250, 12);
//				MakeCH1TimerOut(0, 0);		//调试时不限时


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
				stBusFreq.bySndSecAddr = stCallAddr.bySndSecAddr;
				stBusFreq.bySndRoomAddr = stCallAddr.bySndRoomAddr;
				stBusFreq.bySndBedAddr = stCallAddr.bySndBedAddr;			
				stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
				stBusFreq.byRecSecAddr = stCallAddr.byRecSecAddr;
				stBusFreq.byRecRoomAddr = stCallAddr.byRecRoomAddr;
				stBusFreq.byRecBedAddr = stCallAddr.byRecBedAddr;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}					
			break;
			
		case CMD_CHANNEL_CLOSE:			
			uiDevState &= ~FLAG_TALK1;
			
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{	//没有呼叫记录了，直接退出
			
				if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//没有通话存在
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0]		=0x00;		//系统不忙
					Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);		//报告系统不忙状态
					wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);		//报告系统不忙状态
				}	
			}	
			
			//语音通道切换
			VoiceChannelCtx();
			//清通道1通话超时
			MakeCH1TimerOut(0, 0);
			if(uiDevState & FLAG_TALK)
			{	//通道0通话存在，发送切换语音通道命令				
				stBusFreq.byCmd = CMD_CHANNEL_CHANGE;
				//置通道切换命令在总线队列中标志
				uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}

			else
			{	
				RestartIndication();
			}
			break;

		case CMD_INFUSION_ENTER:		//发送完呼叫确认命令后才将此信息上传至PC机
		case CMD_SERVICE_ENTER:
		case CMD_EMERGENCY_ENTER:
		case CMD_HELP_ENTER:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,stBusFreq.byCmd-0x1b,3,data_buff);	
			wifi_send_cmd(source_addr,stBusFreq.byCmd-0x1b,3,data_buff);	
			break;
			
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(uiDevState & FLAG_BUSY)
			{	//忙，退出
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
 				data_buff[0] = stBusFreq.byCmd;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作失败命令
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作失败命令
				break;
			}	
			//清呼叫记录
			stBusFreq.byCmd -= 0x06;			//将处理命令还原成对应的呼叫命令
			RemoveCallNod(&(stBusFreq));			
			//置等待应答和忙标志
			uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);	
			if(uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				uiDevState &= ~FLAG_INDICATION;
				
				//停止报号			
				StopVoicePlay();

				//语音通道切换
				VoiceChannelCtx();	

			}	
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stBusFreq));

			//设置等待应答超时1S
			MakeCH0TimerOut(50, 0);			
			break;
		case CMD_COMM_CALL:									//普通呼叫命令
			if(uiDevState & FLAG_BUSY)
			{	//忙，直接退出
				break;
			}
		
			//置忙和等待应答标志 
			uiDevState |= (FLAG_BUSY|FLAG_WAIT_ACK);			
			if(uiDevState & FLAG_INDICATION)
			{	//正在呼叫指示,清指示标志				
				uiDevState &= ~FLAG_INDICATION;
				
				//停止报号			
				StopVoicePlay();

				//语音通道切换
				VoiceChannelCtx();								
			}	
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stBusFreq));

			//设置等待应答超时1S
			MakeCH0TimerOut(50, 0);
			break;		
		case CMD_ONLINE_WRITE:
		case CMD_ONLINE_READ:
		case CMD_SCHEDULE_SEND:
		case CMD_QUEST:
			//置等待应答和忙标志
			//uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);
			uiDevState |= (FLAG_WAIT_COMM_ACK);
			//保存分机地址
			stCommAddr.bySecAddr = stBusFreq.byRecSecAddr;
			stCommAddr.byRoomAddr = stBusFreq.byRecRoomAddr;
			stCommAddr.byBedAddr = stBusFreq.byRecBedAddr;
			//设置等待应答超时1S
			byWaitCommAckDTime = 50;
			break;
		case CMD_OPEN_LCD:
		case CMD_CLOSE_LCD:
		case CMD_CLEAR_LCD:
		case CMD_CLEAR_HANDLE_LCD:
		case CMD_CLEAR_MK_LCD:
			if((stBusFreq.byRecSecAddr == 0xff)||(stBusFreq.byRecRoomAddr == 0xff)||(stBusFreq.byRecBedAddr == 0xff))
			{//是广播地址

				memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
				data_buff[0] = stBusFreq.byCmd;
				data_buff[1] = TRUE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			}			
			else
			{//单个
				//置等待应答和忙标志
				//uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);
				uiDevState |= (FLAG_WAIT_COMM_ACK);
				//保存分机地址
				stCommAddr.bySecAddr = stBusFreq.byRecSecAddr;
				stCommAddr.byRoomAddr = stBusFreq.byRecRoomAddr;
				stCommAddr.byBedAddr = stBusFreq.byRecBedAddr;
				//设置等待应答超时1S
				byWaitCommAckDTime = 50;			
			}

			if(stBusFreq.byCmd == CMD_CLOSE_LCD)	
			{
				blight_off_state =1;	//背光关状态
			}
			else if(stBusFreq.byCmd == CMD_OPEN_LCD)	
			{
				blight_off_state =0;	//背光开状态
			}
			break;
		case CMD_BROADCAST1:								
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:								//广播命令
			if(uiDevState & FLAG_BUSY)
			{	//忙，发送系统复位命令
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
				data_buff[0] = stBusFreq.byCmd;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作失败命令
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作失败命令
				break;
			}
			//不忙，置忙标志和广播标志
			uiDevState |= (FLAG_BUSY|FLAG_BROADCAST);
			
			//保存主动呼叫方、被动呼叫方地址
			SaveCallAddr(&(stBusFreq));

			if(uiDevState & FLAG_INDICATION)
			{
				//清呼叫指示标志
				uiDevState &= ~FLAG_INDICATION;
				//停止报号
				StopVoicePlay();				
			}

			AM79R70_active();		//使电话芯片进入通话状态
			set_talk_voice_volue(TALK_SLAVE_CH,stEepromCfgData.bySlaTalkVol);
			
			//语音通道切换
			VoiceChannelCtx();

			//设置等待广播超时180S
			MakeCH0TimerOut(250, 36);

			memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
			data_buff[0] = stBusFreq.byCmd;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			break;

			
		case CMD_VOICE_TIMER_SET:
		case CMD_BL_TIMER_SET:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]		=stBusFreq.byCmd;
			data_buff[1]		=TRUE;		//成功或失败
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			break;
			
		case CMD_RS485_BRT_SET:
			byReSndBrtTimes--;
			if(byReSndBrtTimes!=0) 
			{	
				soft_delay_half_ms(1000); //延时1S,再发送一次
				if(byReSndBrtTimes == RE_SND_TIMES-1)
				{
					source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
					source_addr.byRoomAddr = stEepromCfgData.bySelfRoomAddr;
					source_addr.byBedAddr = stEepromCfgData.bySelfBedAddr;

					data_buff[0] = stBusFreq.byCmd;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送完第一次发结果命令
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送完第一次发结果命令
				}
				Bus0OutputData(&(stBusFreq.bySndSecAddr));			
			}
			break;


		case CMD_RS485_INFO_SEC_ROOM:
			byReSndRs485RoomTimes--;
			if(byReSndRs485RoomTimes!=0)
			{
				soft_delay_half_ms(1000);	//延时1S,再发送一次
				if(byReSndRs485RoomTimes == RE_SND_TIMES-1)
				{
					source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
					source_addr.byRoomAddr = stEepromCfgData.bySelfRoomAddr;
					source_addr.byBedAddr = stEepromCfgData.bySelfBedAddr;

					data_buff[0] = stBusFreq.byCmd;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送完第一次发结果命令
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送完第一次发结果命令
				}
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}
			break;
			
		case CMD_BED_VOICE_PLAY:
			byReSndBedVoiPlaTimes--;
			if(byReSndBedVoiPlaTimes!=0)
			{
				soft_delay_half_ms(1000);	//延时1S,再发送一次	
				if(byReSndBedVoiPlaTimes == RE_SND_TIMES-1)
				{
					source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
					source_addr.byRoomAddr = GLZJ_ADDR2;
					source_addr.byBedAddr = GLZJ_ADDR3;
					data_buff[0] = stBusFreq.byCmd;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送完第一次发结果命令
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送完第一次发结果命令
				}
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}
			break;			

		case CMD_SYSTEM_SEC_SET:
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = GLZJ_ADDR2;
			source_addr.byBedAddr = GLZJ_ADDR3;
			data_buff[0] = 0x5a;
			data_buff[1] = TRUE;

			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令

#ifdef  enable_wdt
			feed_wdt();
#endif
			//while(1);
			break;


		case CMD_NUMBER_SET:
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = GLZJ_ADDR2;
			source_addr.byBedAddr = GLZJ_ADDR3;
			data_buff[0] = 0x5b;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			break;

			
		case CMD_DISP_MODE:
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = GLZJ_ADDR2;
			source_addr.byBedAddr = GLZJ_ADDR3;

			data_buff[0] =stBusFreq.byCmd;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//发送操作成功命令
			break;

		case CMD_COLOR_CLEAR:
			uiDevState |= (FLAG_WAIT_COMM_ACK); 
			//设置等待应答超时1S
			byWaitCommAckDTime = 50; 
			break;

		case CMD_DATE_SEND:
			if(stEepromCfgData.bySysMode & USE_TIME_DEBUG)
			{
				source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
				source_addr.byRoomAddr = GLZJ_ADDR2;
				source_addr.byBedAddr = GLZJ_ADDR3;

				data_buff[0] = stBusFreq.byRecBedAddr;			//年
				data_buff[1] = stBusFreq.byRecRoomAddr;			//月
				data_buff[2] = stBusFreq.byRecSecAddr;			//日
				data_buff[3] = stBusFreq.bySndBedAddr;			//时
				data_buff[4] = stBusFreq.bySndRoomAddr;			//分
				data_buff[5] = stBusFreq.bySndSecAddr;			//秒
				Usart2SndCmd(source_addr,stBusFreq.byCmd,6,data_buff);
			}
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
	STAddr_S source_addr;
	uint8_t data_buff[10];
	
	//通话通道0超时处理
	if(stCH0TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stCH0TimerOut.byTimerOut--;
		if(stCH0TimerOut.byTimerOut == 0)
		{	//超时一次到了			
			if(stCH0TimerOut.byTimerOutCount == 0x00)
			{	//所有超时次数已到	
CH0TimerOutDeal:
				if(uiDevState & FLAG_LANDING)
				{	//上电登记状态，复位所有分机						
					stBusFreq.bySndSecAddr = 0x00;
					stBusFreq.bySndRoomAddr = 0x00;
					stBusFreq.bySndBedAddr = 0x00;
					stBusFreq.byCmd = CMD_POWER_ON;
					stBusFreq.byRecSecAddr = 0xff;
					stBusFreq.byRecRoomAddr = 0xff;
					stBusFreq.byRecBedAddr = 0xff;
					Bus0OutputData(&(stBusFreq.bySndSecAddr));

					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					Usart2SndCmd(source_addr,CMD_LANDING,0,data_buff);		//发送登陆命令
					wifi_send_cmd(source_addr,CMD_LANDING,0,data_buff);		//发送登陆命令
					byWaitAckTimeLanding = 250;	//等待5S
					
				}
				else if(uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN|FLAG_TALK|FLAG_BROADCAST))
				{	//等待应答超时到
					if((uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN)) && ((scon2_Rcvbuf[CMD_POSITION] ==CMD_INFUSION_ANSWER)||(scon2_Rcvbuf[CMD_POSITION] ==CMD_COMM_CALL)) )
					{//如果在等待应答或者等待接听状态下,且串口中的命令为处理输液呼叫或普通呼叫,将发操作不成功码
						memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
						data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
						data_buff[1] = FALSE;
						Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
						wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
						scon2_Rcvbuf[CMD_POSITION] =0x00; 	//清空此数据
						
					}
					if(GetState(bHostTalk))
					{
						ClrState(bHostTalk);
						set_talk_voice_volue(TALK_HOST_CH,0);
						set_talk_voice_volue(TALK_SLAVE_CH,0);	
						tel_pickup_time = TEL_PICKUP_TIME;
					}

					if(uiDevState & FLAG_BROADCAST)
					{
						set_talk_voice_volue(TALK_SLAVE_CH,0);
						AM79R70_standby();
					}
					VoiceChannelCtx();
					stBusFreq.bySndSecAddr = stCallAddr.bySndSecAddr;
					stBusFreq.bySndRoomAddr = stCallAddr.bySndRoomAddr;
					stBusFreq.bySndBedAddr = stCallAddr.bySndBedAddr;
					stBusFreq.byCmd = CMD_SYSTERM_RESET;
					stBusFreq.byRecSecAddr = stCallAddr.byRecSecAddr;
					stBusFreq.byRecRoomAddr = stCallAddr.byRecRoomAddr;
					stBusFreq.byRecBedAddr = stCallAddr.byRecBedAddr;;					
					Bus0OutputData(&(stBusFreq.bySndSecAddr));
				}	

			}
			else
			{	//超时次数没有到				
				stCH0TimerOut.byTimerOutCount--;
				if(stCH0TimerOut.byTimerOutCount == 0x00) //所有超时次数已到 
					goto CH0TimerOutDeal;
				else
				{	//所有超时次数没有到,重新加载超时单位时间
					stCH0TimerOut.byTimerOut = stCH0TimerOut.byTimerOutSet;
				}
			}
		}			
	}
	//通话通道1超时处理
	if(stCH1TimerOut.byTimerOut != 0)
	{	//有超时设置存在
		stCH1TimerOut.byTimerOut--;
		if(stCH1TimerOut.byTimerOut == 0)
		{	//超时一次到了
			if(stCH1TimerOut.byTimerOutCount == 0x00)
			{	//所有超时次数已到
CH1TimerOutDeal:
				stBusFreq.bySndSecAddr =	stCH1CallAddr.bySndSecAddr;
				stBusFreq.bySndRoomAddr = 	stCH1CallAddr.bySndRoomAddr;
				stBusFreq.bySndBedAddr = 	stCH1CallAddr.bySndBedAddr;
				stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
				stBusFreq.byRecSecAddr = stCH1CallAddr.byRecSecAddr;
				stBusFreq.byRecRoomAddr = stCH1CallAddr.byRecRoomAddr;
				stBusFreq.byRecBedAddr = stCH1CallAddr.byRecBedAddr;			
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	

				if(GetState(bHostTalk1)) 
				{
					ClrState(bHostTalk1);
					set_talk_voice_volue(TALK_HOST_CH,0);
					set_talk_voice_volue(TALK_SLAVE_CH,0);	

					tel_pickup_time = TEL_PICKUP_TIME;
				}
				VoiceChannelCtx();
			}
			else
			{	//超时次数没有到
				stCH1TimerOut.byTimerOutCount--;
				if(stCH1TimerOut.byTimerOutCount == 0x00) goto CH1TimerOutDeal;
				else
				{	//所有超时次数没有到,重新加载超时单位时间
					stCH1TimerOut.byTimerOut = stCH1TimerOut.byTimerOutSet;
				}
			}
		}			
	}



	if(byWaitCommAckDTime)
	{
		if(--byWaitCommAckDTime ==0)
		{
			if(uiDevState & FLAG_WAIT_COMM_ACK)
			{//处于等待状态
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			}		
		}
	}
	
	if(byWaitRS485AckDTime)
	{
		if(--byWaitRS485AckDTime ==0)
		{
			memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
			data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
			data_buff[1] = FALSE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);	
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			stRS485Addr.byBedAddr=0;		//结果发完，将485床号清零
			
		}
	}



	if(byWaitAckTimeLanding)
	{
		if(--byWaitAckTimeLanding== 0x00)
		{	//上电登记等待应答超时
			uiDetectPcTime = 30000;	//侦听PC机间隔10分钟
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = CMD_PC_CLOSE;
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr =0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr)); 

		}
	}

	
	if(uiDetectPcTime)
	{	//每10分钟检测PC机一次
		if((--uiDetectPcTime) == 0x00)
		{
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			Usart2SndCmd(source_addr,CMD_LANDING,0,data_buff);	
			wifi_send_cmd(source_addr,CMD_LANDING,0,data_buff);
			byWaitAckTimeLanding = 250;	//等待5S
		}
	}


	if(byUsart0SdResultDTime)
	{
		if(--byUsart0SdResultDTime==0)
		{

			memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
			data_buff[0] = byUsart0SndBuf[CMD_POSITION];
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			stRS485Addr.byBedAddr=0;		//结果发完，将485床号清零

		}
	}
	return;
}
		



