#define _IN_IR_
#include "config.h"
#include "string.h"


uint8 xdata MoveFjNum=0;		//移动分机号码


uint8 xdata UartRecData;
uint8 xdata IrDataPosit=0;

uint16  uiNurseInTime;						//检测护士到位定时时间
uint8 	byIrDataIntervalTimer;				//红外数据接收过程中字节之间间隔时间
uint8 	byNumSetTime;						//编号时间

uint8 xdata byIrDARecBuff[8], byIrDADealBuff[6];		//红外接收区、处理缓冲区

bit bFourByteRec=0;					//串口接收到4字节命令
bit bSixByteRec=0;					//串口接收到6字节命�


bit	bSetKeyDown =0;				//设置键按下状态
bit	bNurseIn	=0;	 			//护士到位标志

/**********************************************************
*函数名称			:IRInit	
*函数描述        	:红外接收初始化
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-16
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void IRInit(void)
{
	RXD=1;
	TXD=1;

	SCON = 0x50;	//串口模式1
	PCON &= 0x7F;	//串口波特率速位0:保持原样
	TMOD &= 0X0F;		//TO方式不变
	TMOD |= 0X20;		//T1 8位自动重载	
	TH1 = 0xD8;
	TL1 = 0xD8;		//波特率设置为1200	

	PCON &= (~SMOD);   // 波特率倍增选择		smod=0
	AUXR &= (~T1x12);		//定时器1时钟控制,0:Fosc/12(传统的12分频) 	
	
	TF1=0;
	TR1 = 1;

	
	RI=0;
	TI=0;
	REN=1;
	ES = 1;
	
}
/**********************************************************
*函数名称			:comm	
*函数描述        	:串口中断，用来红外解码
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-16
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#pragma disable
void comm(void) interrupt 4
{	 
	if(TI==1)
		TI=0;
	if(RI==1)
	{
		RI=0;
		UartRecData = ~SBUF;	//按位取反
		byIrDataIntervalTimer=5;		//每字节间隔不超过100ms
		if(IrDataPosit==0)
		{	
			if(UartRecData==0x05)	//起始码
				byIrDARecBuff[IrDataPosit++]=	UartRecData;
		}
		
		else 
		{
			byIrDARecBuff[IrDataPosit++]=	UartRecData;
			if(IrDataPosit >=7)   IrDataPosit =0;

		}	
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
			uiNurseInTime=NURSE_IN_TIME;				//如果5秒内未收到护士到位命令，即认为护士离开
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
				MoveFjNum= byIrDADealBuff[1];
			}
			bNurseIn=1;	
			break;


			
		case IrDA_CMD_INSPECTION:			//护士查房命令	
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

	if(byIrDADealBuff[5] == byIrDADealBuff[0]+byIrDADealBuff[1]+byIrDADealBuff[2]+byIrDADealBuff[3]+byIrDADealBuff[4])
	{//校验和正确
		if(byIrDADealBuff[3] ==0)
		{//是卫生间分机编号
			switch(byIrDADealBuff[4])
			{
				case 0x5a:
					bNumSeting=1;
					byNumSetTime = 250;
					stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;
					SetLedRedState(LED_FLASH);
					bySec =  byIrDADealBuff[1];	//区号
					byRoom = byIrDADealBuff[2];//房号
					break;

					
				default:
					break;
			}
		}
	}
}
