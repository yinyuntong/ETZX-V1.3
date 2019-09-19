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
STUsartCfg  xdata stUsartCfg;
uint8	xdata byUsart0RecBuf[UART0_RX_BUF_SIZE];
bit bUsart0RecFinish	= 0;
bit bRS485DataError		= 0;
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
void UsartInit(void)
{
	TR1=0;
	
	//串口0初始化
	stUsartCfg.pbyRecBuf = byUsart0RecBuf;
	stUsartCfg.byRecState=0;
	stUsartCfg.uiRecPosition=0;
	stUsartCfg.uiRecLen=0;
	stUsartCfg.byRecCount=0;	
	
	SCON=0X50;		//工作方式1
	TMOD&=0X0F;		//TO方式不变
	TMOD|=0X20;		//;t1 方式2作波特率发生器

//	TL1 =	0XF6;		//波特率4800  在18.432M晶振的情况下
	if(stLocalControl.stEepromCfgData.uiRs485Brt>=2400)
	{
		AUXR |= T1x12;		//1T模式,最后波特率为:4800*12=57.6K
		TL1= 256-(Fosc/32/stLocalControl.stEepromCfgData.uiRs485Brt );	
	}
	
	else
	{
		AUXR	&=	~T1x12; //T1x12=0; 12T模式
		TL1= 256-(Fosc/32/12/stLocalControl.stEepromCfgData.uiRs485Brt	);		
	}


	TH1	=	TL1;
    PCON	&=	~SMOD;   //  ;波特率倍增选择		smod=0
    TF1=0;
    TR1=1;
	
	RI=0;
	TI=0;
	REN=1;
	ES=1;	
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
	uint8 xdata byTemp;
	if(RI==1)
	{  
		RI=0;
		byTemp=SBUF;
		if(stUsartCfg.uiRecPosition!=0)	//不是起始字节
		{
			if(stUsartCfg.byRecTimeout==0)
			{//超时到了
			 	stUsartCfg.byRecState=0;
				stUsartCfg.uiRecPosition=0;
				return;
			}
		}
		stUsartCfg.byRecTimeout=5;	//又开始定时
		byUsart0RecBuf[stUsartCfg.uiRecPosition++] = byTemp;		
		switch(stUsartCfg.byRecState)
		{
			case 0: 							
				if(byTemp == START_BYTE1)
				{
					stUsartCfg.byRecState = 1;
//					byCount = 7;
					stUsartCfg.byRecCount = 6;
				}
				else
				{
					stUsartCfg.uiRecPosition = 0;
				}
				break;
			case 1: 							
				if(--stUsartCfg.byRecCount == 0)
				{	//收完地址数据																		
					stUsartCfg.byRecState = 2;
					stUsartCfg.byRecCount = 3;						
				}
				break;				
			case 2: 			
				stUsartCfg.byRecCount--;
				if(stUsartCfg.byRecCount == 1)
				{	
					stUsartCfg.uiRecLen = byTemp<<8; 
				}
				else if(stUsartCfg.byRecCount == 0)
				{	//接收完数据长度
					//stUsartCfg[0].uiRecLen += (byTemp << 8); 			//获取数据域长度值
					stUsartCfg.uiRecLen += byTemp;
					if(stUsartCfg.uiRecLen <= MAX_UART0_DATA_LEN)					
					{	//数据长度符合要求
						stUsartCfg.byRecState = 3;
						stUsartCfg.uiRecLen += 2; 														
					}
					else
					{
						stUsartCfg.byRecState = 0;
						stUsartCfg.uiRecPosition = 0;
					}										
				}
				break;			
			case 3: 			
				if(--stUsartCfg.uiRecLen == 0)
				{	//接收完所有数据
					if(END_BYTE == byTemp)
					{	//结束位正确
						bUsart0RecFinish=1;
					}
					stUsartCfg.byRecState = 0;
					stUsartCfg.uiRecPosition = 0; 			
				}
				break;			
			default:
				stUsartCfg.byRecState = 0;
				stUsartCfg.uiRecPosition = 0;
				break;		
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
uint8 GetPskCrc(STUartFreq *pstDataFreq)
{
	uint8   *xdata pbyData;
	uint16 xdata uiLen;
	uint8  xdata byCrc = 0;
	
	WDT_CONTR = 0x3d; 
//	pbyData = &(pstDataFreq->byRecSecAddr);
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
	uint8 *xdata pbyData;
	uint16 xdata uiLen;
	uint8  xdata byCrc = 0;

	WDT_CONTR = 0x3d; 
//	pbyData = &(pstDataFreq->byRecSecAddr);
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
*函数名称			:Usart0Manager	
*函数描述        	:串口0管理函数
*输入参数   		:
*返回值				:
*全局变量			:stUsartCfg,stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Usart0RecDeal(void)
{		
	STUartFreq *xdata pstDataFreq;
	uint8 xdata i;
	uint16 xdata uiDataLen;

	uiDataLen = byUsart0RecBuf[LEN_HIGH_POSITION]*256+byUsart0RecBuf[LEN_LOW_POSITION];
	
	bUsart0RecFinish=0;
	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	
	pstDataFreq = (pSTUartFreq)byUsart0RecBuf;
	stLocalControl.stBusDealFreq.byRecSecAddr = pstDataFreq->byRecSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr = pstDataFreq->byRecRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = pstDataFreq->byRecBedAddr;

	if(uiDataLen>5)
	{//数据域中的数据长度符合要求	
		if( AddrCompare(&(stLocalControl.stBusDealFreq))&& ( pstDataFreq->byCmd == CMD_DISPLAY_DATA ))
		{//是本机液晶显示数据
			if(!CheckPsk(pstDataFreq))	
			{//数据帧出错	
				if(byUsart0RecBuf[DATA_POSITION4]==0)
				{//不是结束帧数据
					bRS485DataError	=1;
				}
				else
				{//是最后一帧
					stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					bRS485DataError	=0;		//标志恢复
				}
			}

			
			else
			{//校验和正确

				if(byUsart0RecBuf[DATA_POSITION2] == 0x01)
				{//第一帧数据
					bRS485DataError = 0;
				}
				
				if(byUsart0RecBuf[DATA_POSITION4]==1)
				{//最后一帧数据
					if(bRS485DataError ==0)
					{//一串信息之前未曾出过错,发送确认码
						stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
						stLocalControl.stBusDealFreq.byRecSecAddr = pstDataFreq->byCmd;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
				
					else
					{
						stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
					}
				}

				if(bRS485DataError==0)
				{//未曾出错
				
					if(byUsart0RecBuf[DATA_POSITION1]==WARD_SEC_TYPE)	//病区信息类型
					{
					
						Read_Cont(WARD_FACE_DATA_START_ADDR+stLocalControl.uiWardFaceDataLen,uiDataLen-5,WardDataBuff);
						for(i=0;i<uiDataLen-5;i++)
						{
							if(byUsart0RecBuf[DATA_POSITION5+i]==NO_CHANGE_DATA)
							{//不需改变数据
								byUsart0RecBuf[DATA_POSITION5+i]=WardDataBuff[i];
							}
						}

						
						if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
						{//第一帧数据
							WREN();
							Block_Erase_32K(WARD_FACE_DATA_START_ADDR);
							Wait_Busy();
							
							AAI_Write(WARD_FACE_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
					
							stLocalControl.uiWardFaceDataLen = uiDataLen-5;
							

						}
						
						else 
						{	//不进行擦除写					
							AAI_Write1(WARD_FACE_DATA_START_ADDR+stLocalControl.uiWardFaceDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiWardFaceDataLen  += (uiDataLen-5);		//加数据长度					
						
						}

						if(byUsart0RecBuf[DATA_POSITION4])	
						{//一屏数据结束,立即显示

							Read_Cont(WARD_FACE_DATA_START_ADDR,WARD_FACE_DATA_LEN,WardDataBuff);						
							StoreSecInfo(DIS_FRAM_MEM0);
							stLocalControl.uiWardFaceDataLen =0;
						}
					}

				}
				else
				{//出过错
					if(byUsart0RecBuf[DATA_POSITION4]==1)   bRS485DataError=0;
				}				
			}
		}
	}
}


/**********************************************************
*函数名称			:UsartManager
*函数描述        	:单总线0管理线程
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
void UsartManager(void)	 
{	
	while(true)
	{
		WDT_CONTR = 0x3d; 
		if(bUsart0RecFinish)									//串口0收到一帧数据
		{ 	
			OSQPost(byMainCmdQ,UART0_CMD);	
		}
 
		OSWait(K_TMO, 1);
					
	}

}
