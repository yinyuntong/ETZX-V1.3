/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Uart.c
*文件描述    		:串口驱动程序
*创建人     		:尹运同
*创建日期   		:2009-4-6
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_USART_
#include "config.h"

//串口变量定义
STUsartCfg  xdata stUsartCfg[2];

//串口0配置   与下位机通迅
uint8	xdata byUsart0SndBuf[UART0_TX_BUF_SIZE];

//串口1配置	   与上位机通迅
uint8	xdata byUsart1RecBuf[UART1_RX_BUF_SIZE];
uint8	xdata byUsart1SndBuf[UART1_TX_BUF_SIZE];
//uint8   xdata byUsart1RecDealBuf[UART1_RX_DEAL_BUF_SIZE];

uint8 	xdata errtime =0;


bit bUsart1RecFinish	= 0;
bit bRS485DataTreat =0;

/**********************************************************
*函数名称			:UsartInit	
*函数描述        	:串口初始化函数
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
void Usart0Init(uint16 uiRs485Brt)
{
	TR1=0;
	//串口0初始化
	stUsartCfg[0].pbyRecBuf = NULL;
	stUsartCfg[0].pbySndBuf = byUsart0SndBuf;	
	stUsartCfg[0].uiRecPosition=0;
	stUsartCfg[0].uiRecLen=0;
	stUsartCfg[0].byRecCount=0;	
	stUsartCfg[0].byRecTimeout =0;
	stUsartCfg[0].uiSndLen  = stUsartCfg[0].uiSndPos  = 0;
	
	SCON=0X50;		//工作方式1

	TMOD&=0X0F;		//TO方式不变
	TMOD|=0X20;		//T1 方式2 :8位自动重载


//	TL1	=	0XF6;		//波特率4800  在18.432M晶振的情况下
	if(uiRs485Brt>=2400)
	{
		AUXR |= T1x12;		//1T模式,最后波特率为:4800*12=57.6K
		TL1= 256-(CPU_CLK/32/uiRs485Brt );	
	}
	
	else
	{
		AUXR	&=	~T1x12;	//T1x12=0; 12T模式
		TL1= 256-(CPU_CLK/32/12/uiRs485Brt);		
	}


	TH1	=	TL1;
    PCON&=	~SMOD;   //  ;波特率倍增选择		smod=0

    TF1=0;
    TR1=1;

	
	RI=0;
	TI=0;
	REN=1;
	ES=1;	
}


void Usart1Init(void)
{
	//串口1初始化
//	AUXR1 |= S2_P4; 	//串口2切换到P4口
	stUsartCfg[1].pbyRecBuf = byUsart1RecBuf;
	stUsartCfg[1].pbySndBuf = byUsart1SndBuf;
	stUsartCfg[1].byRecState=0;
	stUsartCfg[1].uiRecPosition=0;
	stUsartCfg[1].uiRecLen=0;
	stUsartCfg[1].byRecCount=0; 
	stUsartCfg[1].byRecTimeout=0;
	stUsartCfg[1].uiSndLen	= stUsartCfg[1].uiSndPos  = 0;

	S2CON	=	0X50;		//0101 0000 8位可变波特率,无奇偶校验位
	AUXR	&=	~S2SMOD;	//S2MOD=0;
	AUXR	|=	BRTx12; 	//BRTX12=1; 1T模式,最后波特率为:4800*12=57.6K
	BRT 	=	0XF6;
	AUXR	|=	BRTR;		//启动独立波特率发生器
	
	IE2 	|= ES2;
}
/**********************************************************
*函数名称			:UART0(void)	
*函数描述        	:串口0中断函数
*输入参数   		:
*返回值				:
*全局变量			:stUsartCfg
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#pragma disable
void UART0(void) interrupt UART_INTNO
{
	if(RI==1)
	{  
		RI=0;
	}
	
	else if(TI==1)
	{
		TI=0;
		stUsartCfg[0].uiSndLen--;
		stUsartCfg[0].uiSndPos++;
		if(stUsartCfg[0].uiSndLen!=0)
		{
			SBUF = stUsartCfg[0].pbySndBuf[stUsartCfg[0].uiSndPos];				
			
		}
		else 
		{//一帧数据发送完成

			if((stUsartCfg[0].pbySndBuf[REC_ROOM_POSITION]== 0xf0) ||(stUsartCfg[0].pbySndBuf[REC_ROOM_POSITION]== YHFJ_ADDR2))	//走廊显示屏的数据或医护分机的数据
			{
				stLocalControl.byWaitRS485AckDTime = 0;		//广播数据不再等侍
				stLocalControl.byUsart1SdResultDTime = 50;	//50*20 = 1000ms
			}
			else
			{
				if(stUsartCfg[0].pbySndBuf[DATA4_POSITION]==0x00)	//不是结束帧
				{

					stLocalControl.byWaitRS485AckDTime = 0;
					if(stUsartCfg[0].pbySndBuf[DATA2_POSITION]==1)
					{
						//第一帧数据要清存储器 ，擦除32K典型18ms
						stLocalControl.byUsart1SdResultDTime = 20;	//20*20 = 400ms
					}
							
					else stLocalControl.byUsart1SdResultDTime = 10;	//10*20 = 200ms
				}

				else 
				{//是结束帧
	//				DE0=0;		//	整个数据发送完成后才禁止R485发送	
					if(stUsartCfg[0].pbySndBuf[CMD_POSITION]==CMD_DISPLAY_FORM)
					{//是发送床头分机格式
						stLocalControl.byUsart1SdResultDTime = 10;	//10*20 = 200ms   为分机保存数据而保留时间以免串口接收缓冲区的数据被盖
					}
				}
			}
		}
	}
} 


/**********************************************************
*函数名称			:UART1(void)	
*函数描述        	:串口2中断函数
*输入参数   		:
*返回值				:
*全局变量			:stUsartCfg
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#pragma disable
void UART1(void) interrupt UART2_INTNO
{
	uint8 xdata byTemp;

	if(S2CON&S2RI)
	{  
		S2CON &= ~S2RI;
		byTemp = S2BUF;

		
/*		if(stUsartCfg[1].uiRecPosition!=0)	//不是起始字节
		{
			if(stUsartCfg[1].byRecTimeout==0)
			{//超时到了
				stUsartCfg[1].byRecState=0;
				stUsartCfg[1].uiRecPosition=0;
				return;
			}
		}
*/		
		stUsartCfg[1].byRecTimeout=200; //又开始定时5ms*200=1000ms
		byUsart1RecBuf[stUsartCfg[1].uiRecPosition++] = byTemp; 	
		switch(stUsartCfg[1].byRecState)
		{
			case 0: 							
				if(byTemp == START_BYTE1)
				{
					stUsartCfg[1].byRecState = 1;
//					byCount = 7;
					stUsartCfg[1].byRecCount = 6;
				}
				else
				{
					stUsartCfg[1].uiRecPosition = 0;
				}
				break;
			case 1: 							
				if(--stUsartCfg[1].byRecCount == 0)
				{	//收完地址数据																		
					stUsartCfg[1].byRecState = 2;
					stUsartCfg[1].byRecCount = 3;	
				}
				break;				
			case 2: 			
				stUsartCfg[1].byRecCount--;
				if(stUsartCfg[1].byRecCount == 1)
				{	
					stUsartCfg[1].uiRecLen = byTemp<<8; 
				}
				else if(stUsartCfg[1].byRecCount == 0)
				{	//接收完数据长度
					//stUsartCfg[0].uiRecLen += (byTemp << 8);			//获取数据域长度值
					stUsartCfg[1].uiRecLen += byTemp;
					if(stUsartCfg[1].uiRecLen <= MAX_UART1_DATA_LEN)					
					{	//数据长度符合要求
						stUsartCfg[1].byRecState = 3;
						stUsartCfg[1].uiRecLen += 2; 
					}
					else
					{
						stUsartCfg[1].byRecState = 0;
						stUsartCfg[1].uiRecPosition = 0;
					}										
				}
				break;			
			case 3: 			
				if(--stUsartCfg[1].uiRecLen == 0)
				{	//接收完所有数据
					if(END_BYTE == byTemp)
					{	//结束位正确
						bUsart1RecFinish=1; 
					}
					stUsartCfg[1].byRecState = 0;
					stUsartCfg[1].uiRecPosition = 0;			
				}
				break;			
			default:
				stUsartCfg[1].byRecState = 0;
				stUsartCfg[1].uiRecPosition = 0;
				break;		
		}
	}


	if(S2CON&S2TI)
	{	
		S2CON &= ~S2TI;
		stUsartCfg[1].uiSndLen--;
		stUsartCfg[1].uiSndPos++;		
		if(stUsartCfg[1].uiSndLen!=0)
		{    

			S2BUF = stUsartCfg[1].pbySndBuf[stUsartCfg[1].uiSndPos];			
		}		
	}
}
/**********************************************************
*函数名称			:GetPskCrc	
*函数描述        	:计算数据桢累加校验和
*输入参数   		:pstDataFreq:待计算的数据桢指针
*返回值				:累加校验和
*全局变量			:stUsartCfg
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 GetPskCrc(pSTUartFreq pstDataFreq)
{
	uint8  *pbyData;
	uint16 uiLen;
	uint8  byCrc = 0;
	
	pbyData = &(pstDataFreq->bySndSecAddr);
	uiLen = pstDataFreq->uiLen + HEAD_SIZE;
	while(uiLen--)
	{
		byCrc += *pbyData++;
	}	
	return(byCrc);
}
/**********************************************************
*函数名称			:CheckPsk	
*函数描述        	:检验数据包的正确型
*输入参数   		:pstDataFreq:待校验的数据桢指针
*返回值				:true:数据包正确,false:数据包有误
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
uint8 CheckPsk(pSTUartFreq pstDataFreq)
{
	uint8* pbyData;
	uint16 uiLen;
	uint8  byCrc = 0;
	
	pbyData = &(pstDataFreq->bySndSecAddr);
	uiLen = pstDataFreq->uiLen + HEAD_SIZE;
	while(uiLen--)
	{
		byCrc += *pbyData++;
	}
	if(byCrc != *pbyData)
	{
		return(false);
	}
	return(true);	
}

/**********************************************************
*函数名称			:UsartSndCmd	
*函数描述        	:串口发送命令给PC机
*输入参数   		
                                    byCmd:命令
                                    byLen:数据长度
                                    pData:数据指针
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
void Usart1SndCmd(uint8 Cmd,uint8 byLen,uint8 *pData)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
	
	pstDataFreq->byStartByte1   =START_BYTE1;
	pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;	
	pstDataFreq->byCmd			=Cmd;		//命令
	pstDataFreq->uiLen			=byLen;
	if(byLen)
	{
		memcpy(pstDataFreq->byData,pData,byLen);	//复制数据
	}
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;		
}


/**********************************************************
*函数名称			:UsartSndEnter	
*函数描述        	:串口发送确认命令
*输入参数   		:byPort :串口号
                                    byOriginalCmd:原命令
                                    byData:数据
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
void Usart1SndEnter(uint8 byOriginalCmd,uint8 byData)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;

	}
	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
	
	pstDataFreq->byStartByte1   =START_BYTE1;
	pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;	
	pstDataFreq->byCmd			=CMD_ENTER_PC;		//确认命令
	pstDataFreq->uiLen			=0X02;
	pstDataFreq->byData[0]		=byOriginalCmd;	//原命令	
	pstDataFreq->byData[1]		=byData;		//成功或失败
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;		
}
/**********************************************************
*函数名称			:Usart1SndResult	
*函数描述        	:串口发送结果命令
*输入参数   		:byPort :串口号
                                    byOriginalCmd:原命令
                                    byData:数据
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
void Usart1SndResult(uint8 byOriginalCmd,uint8 byData)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;

	pstDataFreq->byStartByte1   =START_BYTE1;
/*	pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
	if( (byUsart1RecBuf[CMD_POSITION] == CMD_OPEN_LCD) || (byUsart1RecBuf[CMD_POSITION] == CMD_CLOSE_LCD) || 
		(byUsart1RecBuf[CMD_POSITION] == CMD_CLEAR_LCD)|| (byUsart1RecBuf[CMD_POSITION] == CMD_CLEAR_HANDLE_LCD)||
		(byUsart1RecBuf[CMD_POSITION] == CMD_CLEAR_MK_LCD) ||
		(byUsart1RecBuf[CMD_POSITION] == CMD_START_VOICE)||  (byUsart1RecBuf[CMD_POSITION] == CMD_STOP_VOICE)||
		(byUsart1RecBuf[CMD_POSITION] == CMD_MUSIC_PLAY) ||  (byUsart1RecBuf[CMD_POSITION] == CMD_BROADCAST1) ||
		(byUsart1RecBuf[CMD_POSITION] == CMD_BROADCAST2) ||  (byUsart1RecBuf[CMD_POSITION] == CMD_BROADCAST3) 
		)*/
	{//为避免管理主机区号与上位机区号不一致，将回结果命时的源地址直接为收到串口数据的目的地标
		pstDataFreq->bySndSecAddr   =byUsart1RecBuf[REC_SEC_POSITION];
		pstDataFreq->bySndRoomAddr  =byUsart1RecBuf[REC_ROOM_POSITION];
		pstDataFreq->bySndBedAddr   =byUsart1RecBuf[REC_BED_POSITION];	
	}	
	pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;
	pstDataFreq->byCmd			=CMD_RESULT_PC;		//确认命令
	pstDataFreq->uiLen			=0X02;
	pstDataFreq->byData[0]		=byOriginalCmd;	//原命令	
	pstDataFreq->byData[1]		=byData;		//成功或失败
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;		
}


/**********************************************************
*函数名称			:Usart1SndRS485Result	
*函数描述        	:串口发送结果命令
*输入参数   		:byPort :串口号
                                    byOriginalCmd:原命令
                                    byData:数据
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
void Usart1SndRS485Result(uint8 byOriginalCmd,uint8 byData)
{
	pSTUartFreq pstDataFreq;

	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;

	pstDataFreq->byStartByte1	=START_BYTE1;
	pstDataFreq->bySndSecAddr	=stLocalControl.stRS485Addr.bySecAddr;
	pstDataFreq->bySndRoomAddr	=stLocalControl.stRS485Addr.byRoomAddr;
	pstDataFreq->bySndBedAddr	=stLocalControl.stRS485Addr.byBedAddr;
	pstDataFreq->byRecSecAddr	=stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr	=PC_ADDR2;
	pstDataFreq->byRecBedAddr	=PC_ADDR3;
	pstDataFreq->byCmd			=CMD_RESULT_PC; 	//确认命令
	pstDataFreq->uiLen			=0X02;
	pstDataFreq->byData[0]		=byOriginalCmd;
	pstDataFreq->byData[1]		=byData;		//成功或失败
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;

	stLocalControl.stRS485Addr.byBedAddr=0;		//结果发完，将485床号清零
}


/**********************************************************
*函数名称			:UsartSndResult1	
*函数描述        	:串口发送结果命令
*输入参数   		:byPort :串口号
                                    byOriginalCmd:原命令
                                    byData0:数据0
                                    byData1:数据1
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
void Usart1SndResult1(uint8 byOriginalCmd,uint8 byData0,uint8 byData1)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;

	pstDataFreq->byStartByte1   =START_BYTE1;
	pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;
	pstDataFreq->byCmd			=CMD_RESULT_PC;		//确认命令
	pstDataFreq->uiLen			=0X03;
	pstDataFreq->byData[0]		=byOriginalCmd;	//原命令	
	pstDataFreq->byData[1]		=byData0;		//成功或失败
	pstDataFreq->byData[2]		=byData1;		//数据	
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;		

}
/**********************************************************
*函数名称			:UsartSndResult3	
*函数描述        	:串口发送结果命令
*输入参数   		:byPort :串口号
                                    byOriginalCmd:原命令
                                    byData0:数据0
                                    *byData:结果命令中附带的要返回数据的数据指针
                                    byLen:附带要返回数据的数据长度
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
void Usart1SndResult3(uint8 byOriginalCmd,uint8 byData0,uint8 *byData,uint8 byLen)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;

	}

	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;

	pstDataFreq->byStartByte1   =START_BYTE1;
	pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;
	pstDataFreq->byCmd			=CMD_RESULT_PC;		//确认命令
	pstDataFreq->uiLen			=0X02+byLen;
	pstDataFreq->byData[0]		=byOriginalCmd;	//原命令	
	pstDataFreq->byData[1]		=byData0;		//成功或失败
	memcpy(&(pstDataFreq->byData[2]),byData,byLen);	//复制附带的数据
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;		

}

/**********************************************************
*函数名称			:Usart1SndResult	
*函数描述        	:串口发送错误命令
*输入参数   		:byPort :串口号
                                    byData:数据
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
void Usart1SndError(uint8 byData)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;

	}

	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;

	pstDataFreq->byStartByte1   =START_BYTE1;
	pstDataFreq->bySndSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->bySndRoomAddr  =stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->bySndBedAddr   =stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstDataFreq->byRecSecAddr   =stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;
	pstDataFreq->byCmd			=CMD_ERROR_PC;		//错误命令
	pstDataFreq->uiLen			=0X01;
	pstDataFreq->byData[0]		=byData;	  //错误代码	

	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF	=pstDataFreq->byStartByte1;		
}


/**********************************************************/

void Usart1Sendbyte(uint8 dat)
{
	stUsartCfg[1].uiSndLen	= 1;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据

	S2CON &= ~S2TI;
	S2BUF	=dat;	
}

/**********************************************************
*函数名称			:Usart1RecDeal	
*函数描述        	:串口1协议处理
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
void Usart1RecDeal(void)
{		
	pSTUartFreq pstDataFreq=byUsart1RecBuf;
	bUsart1RecFinish=0;

	if(CheckPsk(pstDataFreq))
	{//校验和正确

	if(byUsart1RecBuf[CMD_POSITION]  == CMD_ENTER_PC)
	{		
		return;	//收到PC机的确认命令
	}


	Usart1SndEnter(byUsart1RecBuf[CMD_POSITION],TRUE);	//发送确认命令

	pstDataFreq = (pSTUartFreq)byUsart1RecBuf;

	if((pstDataFreq->byCmd!= CMD_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_HANDLE_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_DISPLAY_FORM))
		bRS485DataTreat=0;		//因为数据可能已覆盖，将485数据处理标志清0

	
	switch(pstDataFreq->byCmd)
	{
		case CMD_VOICE_PLAY_ROOM:	//报号是否播报房号
			stLocalControl.stBusFreq.byCmd = CMD_VOICE_PLAY_ROOM;
			 if(pstDataFreq->byData[0])
		 	{//报房号
		 		stLocalControl.stBusFreq.byRecSecAddr = 0x01;
		 	}
			else stLocalControl.stBusFreq.byRecSecAddr = 0x00;
			Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;
		
		case CMD_DATE_SEND:		//校时数据
			memcpy(&(stTime.bySecond),&(byUsart1RecBuf[DATA0_POSITION]),6);
			
			//构建总线上的校时数据包   
			if((stTime.bySecond>=0x60)||(stTime.byMinute>=0x60)
			 ||(stTime.byHour>=0x24)||(stTime.byDay>=0x32)
			 ||(stTime.byMonth>=0x13)||(stTime.byYear>0x99))
			{
				Usart1SndResult(CMD_DATE_SEND,FALSE);				//发送操作失败命令
				break;
			}
			P8563_settime();
			memcpy(&(stLocalControl.stBusFreq.bySndSecAddr),&(stTime.bySecond),3);
		    memcpy(&(stLocalControl.stBusFreq.byRecSecAddr),&(stTime.byDay),3);
		   	stLocalControl.stBusFreq.byCmd = CMD_DATE_SEND;   
		   	Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			Usart1SndResult(CMD_DATE_SEND,TRUE);				//发送操作成功命令			
			break;
		case CMD_POWER_ON:
  			if((pstDataFreq->byData[0] == 0xff) ||(pstDataFreq->byData[1] == 0xff) ||(pstDataFreq->byData[2] == 0xff)||
				(pstDataFreq->byRecRoomAddr ==GLZJ_ADDR2)   )
  			{//全体设备重启
  				Usart1SndResult(CMD_POWER_ON,TRUE);				//发送操作成功命令
				while(stUsartCfg[1].uiSndLen!=0)	
				{//有数据正在发送中,等待结果码发送完成
					_nop_();		//延时
				}
				 //关闭所有打开的中断
				CCAPM0 = 0x00;
				ET0 = 0;
				TR0 = 0;
				EX0 = 0;
				IE=0X00;
				IE2=0X00;
				IAP_CONTR = 0x20; 	//软复位
	    		while(1);
  			}
			else
			{//某个单一设备重启
				stLocalControl.stBusFreq.byCmd = CMD_POWER_ON;
				stLocalControl.stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
				stLocalControl.stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
				stLocalControl.stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}
    		break;
		case CMD_DISPLAY_DATA:	 //一帧液晶屏数据
		case CMD_HANDLE_DISPLAY_DATA:	//发送一帧手柄 液晶屏数据
			if(stUsartCfg[0].uiSndLen )
			{//RS485有数据正在发送
				bRS485DataTreat=1;
				return;
			}
	
			DE0=1;
//			Delayms(1);
			EA=0;
			memcpy(byUsart0SndBuf,byUsart1RecBuf,pstDataFreq->uiLen+EXT_SIZE);
			EA=1;
			
			stUsartCfg[0].pbySndBuf = byUsart0SndBuf;
			stUsartCfg[0].uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	
			TI=0;
			SBUF= stUsartCfg[0].pbySndBuf[stUsartCfg[0].uiSndPos];
			
			//保存分机地址					
			stLocalControl.stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
			stLocalControl.stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
			stLocalControl.stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

			//设置等待应答超时2S  在波特率为57.6K的情况下,100字节数据只要17.3ms
			stLocalControl.byWaitRS485AckDTime = 95;		//等待1.9S	
			break;
			
		case CMD_DISPLAY_FORM:	//发送一帧床头分机格式
			if(stUsartCfg[0].uiSndLen )
			{//RS485有数据正在发送
				bRS485DataTreat=1;
				return;
			}
	
			DE0=1;
//			Delayms(1);
			EA=0;
			memcpy(byUsart0SndBuf,byUsart1RecBuf,pstDataFreq->uiLen+EXT_SIZE);
			EA=1;

			
			stUsartCfg[0].pbySndBuf = byUsart0SndBuf;
			stUsartCfg[0].uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	
			TI=0;
			SBUF= stUsartCfg[0].pbySndBuf[stUsartCfg[0].uiSndPos];

			//保存分机地址					
			stLocalControl.stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
			stLocalControl.stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
			stLocalControl.stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;			
			break;
	
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//忙，退出
				Usart1SndResult(pstDataFreq->byCmd,FALSE);
				break;
			}
			memcpy(&(stLocalControl.stBusFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
			stLocalControl.stBusFreq.byCmd = pstDataFreq->byCmd;	//原命令
			memcpy(&(stLocalControl.stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;
		case CMD_INFUSION_CLEAR:
		case CMD_SERVICE_CLEAR:
		case CMD_EMERGENCY_CLEAR:
		case CMD_HELP_CLEAR:
			memcpy(&(stLocalControl.stBusFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
			stLocalControl.stBusFreq.byCmd = pstDataFreq->byCmd;	//原命令
			memcpy(&(stLocalControl.stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;
		case CMD_COMM_CALL:
			if(stLocalControl.uiDevState & FLAG_BUSY)
			{	//系统忙
				Usart1SndResult(CMD_COMM_CALL,FALSE);				//发送操作失败命令
			}
			else
			{
				memcpy(&(stLocalControl.stBusFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
				stLocalControl.stBusFreq.byCmd = CMD_COMM_CALL;	//原命令
				memcpy(&(stLocalControl.stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}
			break;	
		case CMD_TALK_CLEAR:
			if(0x0000 == (stLocalControl.uiDevState & (FLAG_TALK1|FLAG_TALK)))
			{//通道0与通道1都未处于对讲状态
				Usart1SndResult(CMD_TALK_CLEAR,FALSE);				//发送操作失败命令
				break;
			}
			
			if(stLocalControl.uiDevState & FLAG_CHANNEL_CHANGE_EXIST)	
			{//有通道切换在总线发送队列中
				Usart1SndResult(CMD_TALK_CLEAR,FALSE);
				break;
			}
			
			if((stLocalControl.uiDevState & FLAG_TALK))
			{//通道0处于对讲状态
				if(((byUsart1RecBuf[DATA0_POSITION] == stLocalControl.stCallAddr.bySndSecAddr)&&
					(byUsart1RecBuf[DATA1_POSITION] == stLocalControl.stCallAddr.bySndRoomAddr)&&
					(byUsart1RecBuf[DATA2_POSITION] == stLocalControl.stCallAddr.bySndBedAddr))||
				   ((byUsart1RecBuf[DATA0_POSITION] == stLocalControl.stCallAddr.byRecSecAddr)&&
					(byUsart1RecBuf[DATA1_POSITION] == stLocalControl.stCallAddr.byRecRoomAddr)&&
					(byUsart1RecBuf[DATA2_POSITION] == stLocalControl.stCallAddr.byRecBedAddr))||				   
				   ((byUsart1RecBuf[DATA3_POSITION] == stLocalControl.stCallAddr.bySndSecAddr)&&
				    (byUsart1RecBuf[DATA4_POSITION] == stLocalControl.stCallAddr.bySndRoomAddr)&&
				    (byUsart1RecBuf[DATA5_POSITION] == stLocalControl.stCallAddr.bySndBedAddr))||
				   ((byUsart1RecBuf[DATA3_POSITION] == stLocalControl.stCallAddr.byRecSecAddr)&&
				    (byUsart1RecBuf[DATA4_POSITION] == stLocalControl.stCallAddr.byRecRoomAddr)&&
				    (byUsart1RecBuf[DATA5_POSITION] == stLocalControl.stCallAddr.byRecBedAddr)))
				{//是通道0中的分机,发送系统复位命令
					MakeCH0TimerOut(5,0);//	缩短超时时间,准备退出
					Usart1SndResult(CMD_TALK_CLEAR,TRUE);				//发送操作成功命令
					break;
				}
			}
			
			if((stLocalControl.uiDevState & FLAG_TALK1))
			{//通道1处于对讲状态
				if(((byUsart1RecBuf[DATA0_POSITION] == stLocalControl.stCH1CallAddr.bySndSecAddr)&&
					(byUsart1RecBuf[DATA1_POSITION] == stLocalControl.stCH1CallAddr.bySndRoomAddr)&&
					(byUsart1RecBuf[DATA2_POSITION] == stLocalControl.stCH1CallAddr.bySndBedAddr))||
				   ((byUsart1RecBuf[DATA0_POSITION] == stLocalControl.stCH1CallAddr.byRecSecAddr)&&
					(byUsart1RecBuf[DATA1_POSITION] == stLocalControl.stCH1CallAddr.byRecRoomAddr)&&
					(byUsart1RecBuf[DATA2_POSITION] == stLocalControl.stCH1CallAddr.byRecBedAddr))||					
				   ((byUsart1RecBuf[DATA3_POSITION] == stLocalControl.stCH1CallAddr.bySndSecAddr)&&
					(byUsart1RecBuf[DATA4_POSITION] == stLocalControl.stCH1CallAddr.bySndRoomAddr)&&
					(byUsart1RecBuf[DATA5_POSITION] == stLocalControl.stCH1CallAddr.bySndBedAddr))||
				   ((byUsart1RecBuf[DATA3_POSITION] == stLocalControl.stCH1CallAddr.byRecSecAddr)&&
					(byUsart1RecBuf[DATA4_POSITION] == stLocalControl.stCH1CallAddr.byRecRoomAddr)&&
					(byUsart1RecBuf[DATA5_POSITION] == stLocalControl.stCH1CallAddr.byRecBedAddr)))


				{//是通道1中的分机,发送系统复位命令
					MakeCH1TimerOut(5,0);//	缩短超时时间,准备退出
					Usart1SndResult(CMD_TALK_CLEAR,TRUE);				//发送操作成功命令
					break;

				}
			}

			//地址不对
			Usart1SndResult(CMD_TALK_CLEAR,FALSE);				//发送操作失败命令
			break;
			
		case CMD_NUMBER_SET:		//根据地址修改管理主机地址
			if(byUsart1RecBuf[REC_ROOM_POSITION]==GLZJ_ADDR2)
			{//给管理主机编区号,同时将此区号发送给其它下位机设备
				stLocalControl.stEepromCfgData.bySelfSecAddr = byUsart1RecBuf[DATA0_POSITION];
				EA=0;
				SaveParameter(IAP0_ADDR);
				EA=1;
				
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr =	stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd		  =	CMD_SYSTEM_SEC_SET;
				stLocalControl.stBusFreq.byRecSecAddr =	stLocalControl.stEepromCfgData.bySelfSecAddr;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}
			break;	

		case CMD_QUEST:
			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stLocalControl.stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stLocalControl.stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;	
			
		case CMD_SCHEDULE_SEND:	//护理级别修改命令		
			stLocalControl.stBusFreq.bySndSecAddr = byUsart1RecBuf[DATA3_POSITION]; //ADDR或data
			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stLocalControl.stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stLocalControl.stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;	
		case CMD_OPEN_LCD:
		case CMD_CLOSE_LCD:
		case CMD_CLEAR_LCD:
		case CMD_CLEAR_HANDLE_LCD:
		case CMD_CLEAR_MK_LCD:
			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stLocalControl.stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stLocalControl.stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;

		case CMD_STOP_VOICE:			
			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
			stLocalControl.stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			flag_origin_music =0;
			break;

		case CMD_START_VOICE:			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.uiDevState & FLAG_BUSY)||(stLocalControl.uiDevState & FLAG_TALK1))
			{	//系统忙
				Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);				//发送操作失败命令
				break;
			}

			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
			stLocalControl.stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;

		case CMD_MUSIC_PLAY:			
			if((stLocalControl.uiDevState & FLAG_BUSY)||(stLocalControl.uiDevState & FLAG_TALK1))
			{	//系统忙
				Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);				//发送操作失败命令
				break;
			}

			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
			stLocalControl.stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			flag_origin_music =1;
			break;

			
		case CMD_VOICE_TIMER_SET:
		case CMD_BL_TIMER_SET:
			stLocalControl.stBusFreq.bySndSecAddr = byUsart1RecBuf[DATA0_POSITION]; //起始小时
			stLocalControl.stBusFreq.bySndRoomAddr = byUsart1RecBuf[DATA1_POSITION];	//起始分钟
			stLocalControl.stBusFreq.bySndBedAddr = 0x00;
			stLocalControl.stBusFreq.byCmd = byUsart1RecBuf[CMD_POSITION];
			stLocalControl.stBusFreq.byRecSecAddr = byUsart1RecBuf[DATA2_POSITION]; //结束小时
			stLocalControl.stBusFreq.byRecRoomAddr = byUsart1RecBuf[DATA3_POSITION];	//结束分钟
			stLocalControl.stBusFreq.byRecBedAddr = byUsart1RecBuf[DATA4_POSITION]; //音量值
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;	

			
		case CMD_ENTER:	//来自PC机上的登记确认命令
			if(stLocalControl.byWaitAckTimeLanding)
			{//正在等待PC机上的登记确认命令
				stLocalControl.byWaitAckTimeLanding = 0;
				stLocalControl.uiDetectPcTime = 30000;	//侦听PC机间隔10分钟
				
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;	
				stLocalControl.stBusFreq.byCmd = CMD_PC_OPEN;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr =0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				bPCState =1;	//集成主机开机状态
			}
			break;
			
		case CMD_RS485_BRT_SET:
			if(byUsart1RecBuf[DATA0_POSITION] >0 && byUsart1RecBuf[DATA0_POSITION] <10)
			{
				switch(byUsart1RecBuf[DATA0_POSITION])
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
						stLocalControl.stEepromCfgData.uiRs485Brt	=1200;
						break;				
				}					
				//同时将RS485总线的波特（串口0）进行设置
				Usart0Init(stLocalControl.stEepromCfgData.uiRs485Brt);
				SaveParameter(IAP0_ADDR);				
				stLocalControl.stBusFreq.bySndSecAddr 	= 	stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr	=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr	=	stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd			=	CMD_RS485_BRT_SET;
				stLocalControl.stBusFreq.byRecSecAddr	=	byUsart1RecBuf[DATA0_POSITION];	//波特率代码
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				stLocalControl.byReSndBrtTimes = RE_SND_TIMES;
			}
			else Usart1SndResult(CMD_RS485_BRT_SET,FALSE);
			break;

		case CMD_RS485_INFO_SEC_ROOM:
			if(byUsart1RecBuf[DATA0_POSITION] ==0 || byUsart1RecBuf[DATA0_POSITION] ==1)
			{
				stLocalControl.stBusFreq.bySndSecAddr 	= 	stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr	=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr	=	stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd			=	CMD_RS485_INFO_SEC_ROOM;
				stLocalControl.stBusFreq.byRecSecAddr	=	byUsart1RecBuf[DATA0_POSITION];	//床头分机接怍RS485信息是否需要判断房号
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				stLocalControl.byReSndRs485RoomTimes = RE_SND_TIMES;			//重复发送三次
			}
			else Usart1SndResult(CMD_RS485_INFO_SEC_ROOM,FALSE);
			break;

		case CMD_BED_VOICE_PLAY:
			if(byUsart1RecBuf[DATA0_POSITION] ==0 || byUsart1RecBuf[DATA0_POSITION] ==1)
			{
				stLocalControl.stBusFreq.bySndSecAddr 	= 	stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr	=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr	=	stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd			=	CMD_BED_VOICE_PLAY;
				stLocalControl.stBusFreq.byRecSecAddr	=	byUsart1RecBuf[DATA0_POSITION];	//床头分机是否需要语音报号
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				stLocalControl.byReSndBedVoiPlaTimes = RE_SND_TIMES;			//重复发送三次	
			}
			else Usart1SndResult(CMD_BED_VOICE_PLAY,FALSE);
			break;
			
		case CMD_VOICE_TIMES:
			if(byUsart1RecBuf[DATA0_POSITION] !=0 && byUsart1RecBuf[DATA0_POSITION] <=10)
			{//语音报号遍数设置10遍以及下
				stLocalControl.stBusFreq.bySndSecAddr	=	stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr	=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr	=	stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd			=	CMD_VOICE_TIMES;
				stLocalControl.stBusFreq.byRecSecAddr	=	byUsart1RecBuf[DATA0_POSITION]; //报号遍数
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
					
			}
			else Usart1SndResult(CMD_VOICE_TIMES,FALSE);
			break;

			
		case CMD_DISP_MODE:
			if(byUsart1RecBuf[DATA0_POSITION] ==0 || byUsart1RecBuf[DATA0_POSITION] ==1)
			{			
				stLocalControl.stBusFreq.bySndSecAddr 	= 	stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr	=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr	=	stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusFreq.byCmd			=	CMD_DISP_MODE;	
				stLocalControl.stBusFreq.byRecSecAddr	=	byUsart1RecBuf[DATA0_POSITION];
				stLocalControl.stBusFreq.byRecRoomAddr	=	byUsart1RecBuf[DATA1_POSITION];
				stLocalControl.stBusFreq.byRecBedAddr	=	byUsart1RecBuf[DATA2_POSITION];
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}
			else Usart1SndResult(CMD_DISP_MODE,FALSE);
			break;

		case CMD_COLOR_CLEAR:
			stLocalControl.stBusFreq.bySndSecAddr 	= 	stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusFreq.bySndRoomAddr	=	stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusFreq.bySndBedAddr	=	stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusFreq.byCmd			=	CMD_COLOR_CLEAR;	
			stLocalControl.stBusFreq.byRecSecAddr	=	byUsart1RecBuf[DATA0_POSITION];
			stLocalControl.stBusFreq.byRecRoomAddr	=	byUsart1RecBuf[DATA1_POSITION];
			stLocalControl.stBusFreq.byRecBedAddr	=	byUsart1RecBuf[DATA2_POSITION];
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			break;
				
		default:
			Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);				//发送操作失败命令
			break;
	}
	}
}



/**********************************************************
*函数名称			:Usart1SndData
*函数描述        	:串口发送数据
*输入参数   		 
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
void Usart1SndData(pSTBusFreq pstBusFreq)
{
	pSTUartFreq pstDataFreq;
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	pstDataFreq = (pSTUartFreq)byUsart1SndBuf;

	pstDataFreq->byStartByte1   =START_BYTE1;
    memcpy(&(pstDataFreq->bySndSecAddr),&(pstBusFreq->bySndSecAddr),3);
	memcpy(&(pstDataFreq->byRecSecAddr),&(pstBusFreq->byRecSecAddr),3);
	pstDataFreq->byCmd			=pstBusFreq->byCmd;		
	pstDataFreq->uiLen			=0X00;
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	stUsartCfg[1].uiSndLen	= pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
	S2CON &= ~S2TI;
	S2BUF=pstDataFreq->byStartByte1;

}



void uart1_send_byte(unsigned char SDATA)
{
#if DEBUG==1
	errtime =0xff;
	while(stUsartCfg[1].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;

	}
	stUsartCfg[1].uiSndLen	= 1;
	stUsartCfg[1].uiSndPos = 0; 	//从数据帧头部开始发送数据
	S2CON &= ~S2TI;
	S2BUF=SDATA;
#endif
}


void uart0_send_byte(unsigned char SDATA)
{
/*#if DEBUG==1
	stUsartCfg[0].pbySndBuf[DATA4_POSITION]  = 0X01;
	errtime =0xff;
	while(stUsartCfg[0].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	stUsartCfg[0].uiSndLen	= 1;
	stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据
	TI=0;
	SBUF=SDATA;
#endif*/

	uint8 xdata i;
	SDATA=SDATA;
	i=i;
#if DEBUG ==1
	errtime =0xff;
	while(stUsartCfg[0].uiSndLen!=0)	
	{//有数据正在发送中
		Delayms(1);
		WDT_CONTR = 0x3e;		//看门狗定时2.5s@20M  
		if(--errtime ==0) break;
	}
	ES=0;
    SBUF=SDATA;
    while(!TI);
    TI=0;
	ES=1;
#endif

}

