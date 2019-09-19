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
STUsartCfg  xdata stUsartCfg[1];
#define IncRxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiRecBufSize)
#define IncTxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiSndBufSize)



uint8	xdata byUsart0RecBuf[UART0_RX_BUF_SIZE];



bit bUsart0RecFinish	= 0;
bit bRS485DataError		=0;


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
	AUXR	&=	~BRTR;		//停止独立波特率发生器 
	
	//rxd配置为高阻输入态
	P3M0 &= 0xfe;
	P3M1 |= 0x01;
	
	//串口0初始化
	stUsartCfg[0].pbyRecBuf = byUsart0RecBuf;
	stUsartCfg[0].pbySndBuf = NULL;
	stUsartCfg[0].byRecState=0;
	stUsartCfg[0].uiRecPosition=0;
	stUsartCfg[0].uiRecLen=0;
	stUsartCfg[0].byRecCount=0;	
	stUsartCfg[0].uiSndLen  = stUsartCfg[0].uiSndPos  = 0;
	
	SCON=0X50;		//工作方式1
	
/*	TMOD&=0X0F;		//TO方式不变
	TMOD|=0X20;		//T1 方式2 :8位自动重载
	TL1	=	0XF6;		//波特率4800  在18.432M晶振的情况下
	TH1	=	TL1;
    PCON&=	~SMOD;   //  ;波特率倍增选择		smod=0
  	AUXR |= T1x12;		//1T模式,最后波特率为:4800*12=57.6K
    TF1=0;
    TR1=1;*/
    PCON&=	~SMOD;   //  ;波特率倍增选择		smod=0
    AUXR	|=	S1BRS;	//特用波特率发生器
    
 	if(stLocalControl.stEepromCfgData.uiRs485Brt>=2400)
	{
		AUXR	|=	BRTx12; 	//BRTX12=1; 1T模式,最后波特率为:4800*12=57.6K
//		BRT 	=	0XF6;
		BRT= 256-(Fosc/32/stLocalControl.stEepromCfgData.uiRs485Brt	);
		
	
}
	else
	{
		AUXR	&=	~BRTx12; 	//BRTX12=0; 12T模式
//		BRT 	=	0XF6;
		BRT= 256-(Fosc/32/12/stLocalControl.stEepromCfgData.uiRs485Brt	);		
	}   

	AUXR	|=	BRTR;		//启动独立波特率发生器
	
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
	uint8 byTemp;
	if(RI==1)
	{  
		RI=0;
		byTemp=SBUF;
		if(stUsartCfg[0].uiRecPosition!=0)	//不是起始字节
		{
			if(stUsartCfg[0].byRecTimeout==0)
			{//超时到了
			 	stUsartCfg[0].byRecState=0;
				stUsartCfg[0].uiRecPosition=0;
				return;
			}
		}
		stUsartCfg[0].byRecTimeout=5;	//又开始定时
		byUsart0RecBuf[stUsartCfg[0].uiRecPosition++] = byTemp;		
		switch(stUsartCfg[0].byRecState)
		{
			case 0: 							
				if(byTemp == START_BYTE1)
				{
					stUsartCfg[0].byRecState = 1;
//					byCount = 7;
					stUsartCfg[0].byRecCount = 6;
				}
				else
				{
					stUsartCfg[0].uiRecPosition = 0;
				}
				break;
			case 1: 							
				if(--stUsartCfg[0].byRecCount == 0)
				{	//收完地址数据																		
					stUsartCfg[0].byRecState = 2;
					stUsartCfg[0].byRecCount = 3;						
				}
				break;				
			case 2: 			
				stUsartCfg[0].byRecCount--;
				if(stUsartCfg[0].byRecCount == 1)
				{	
					stUsartCfg[0].uiRecLen = byTemp<<8; 
				}
				else if(stUsartCfg[0].byRecCount == 0)
				{	//接收完数据长度
					//stUsartCfg[0].uiRecLen += (byTemp << 8); 			//获取数据域长度值
					stUsartCfg[0].uiRecLen += byTemp;
					if(stUsartCfg[0].uiRecLen <= MAX_UART0_DATA_LEN)					
					{	//数据长度符合要求
						stUsartCfg[0].byRecState = 3;
						stUsartCfg[0].uiRecLen += 2; 														
					}
					else
					{
						stUsartCfg[0].byRecState = 0;
						stUsartCfg[0].uiRecPosition = 0;
					}										
				}
				break;			
			case 3: 			
				if(--stUsartCfg[0].uiRecLen == 0)
				{	//接收完所有数据
					if(END_BYTE == byTemp)
					{	//结束位正确
						bUsart0RecFinish=1;
					}
					stUsartCfg[0].byRecState = 0;
					stUsartCfg[0].uiRecPosition = 0; 			
				}
				break;			
			default:
				stUsartCfg[0].byRecState = 0;
				stUsartCfg[0].uiRecPosition = 0;
				break;		
		}
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
	if(S2CON&S2RI)
	{
		S2CON &= ~S2RI;
	}
	else if(S2CON&S2TI)
	{
		S2CON &= ~S2TI;
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
	pSTUartFreq pstDataFreq;
	uint16 xdata uiDataLen;
	uint8 xdata i;

	uiDataLen = byUsart0RecBuf[LEN_HIGH_POSITION]*256+byUsart0RecBuf[LEN_LOW_POSITION];

	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstDataFreq = (pSTUartFreq)byUsart0RecBuf;



	if((pstDataFreq->byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)&&
		(pstDataFreq->byRecRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
		(pstDataFreq->byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr)&&
		(pstDataFreq->byCmd == CMD_SB_LCD_DISPLAY_DATA))
	{//是本机地址
	
		if(uiDataLen>=5)
		{	
			if(!CheckPsk(pstDataFreq))	
			{//数据帧出错
				stLocalControl.stBusDealFreq.byCmd = CMD_SB_DATA_ANSWER; 	
				stLocalControl.stBusDealFreq.byRecSecAddr = 0X00;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			}
			else
			{	
				if(byUsart0RecBuf[DATA_POSITION4] == 0x01)	//一串信息结束		
				{//恢复MIC原来的状态
					MIC_CTRL=MicCtrlState;
				}
				
				switch(byUsart0RecBuf[DATA_POSITION1])	//信息类型
			 	{		 	
				 	case DIET_TYPE:
						if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
						{//第一帧数据
							SpiNormalMode();
							WREN();
							Block_Erase_32K(DIET_DATA_START_ADDR);
							Wait_Busy();
							AAI_Write1(DIET_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiDietDataLen = (uiDataLen-5);
							
							//AAI_Write(DIET_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiDietDataLen)),2);	//保存真正的数据长度 ,原有此信息长度覆盖
							SpiLcdMode();
						}
						else 
						{	//不进行擦除写
							SpiNormalMode();
							AAI_Write1(DIET_DATA_START_ADDR+stLocalControl.uiDietDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiDietDataLen  += (uiDataLen-5);		//加数据长度
							SpiLcdMode();
						}

						if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
						{//结束标志
							SpiNormalMode();
							AAI_Write(DIET_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiDietDataLen)),2);	//保存数据长度
							SpiLcdMode();
							if(stLocalControl.byDisplayFace != TALK_FACE)
							{
								DisplayedLen=0;
								ShowFace(DIET_TYPE);								
								stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
							}
						}
						break;

					case PRESCRIPTION_TYPE:
						if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
						{//第一帧数据
							WREN();
							Block_Erase_32K(PRESCRIPTION_DATA_START_ADDR);
							Wait_Busy();							
							AAI_Write1(PRESCRIPTION_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);					
							stLocalControl.uiPrescriptionDataLen = uiDataLen-5;
							
							//AAI_Write(PRESCRIPTION_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),2);	//保存真正的数据长度 ,原有此信息长度覆盖
						}
						else 
						{	//不进行擦除写
							AAI_Write1(PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiPrescriptionDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiPrescriptionDataLen  += (uiDataLen-5);		//加数据长度						
						}

						if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
						{//结束标志
							AAI_Write(PRESCRIPTION_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),2);	//保存数据长度
							if(stLocalControl.byDisplayFace !=TALK_FACE)
							{
								DisplayedLen=0;
								ShowFace(PRESCRIPTION_TYPE);								
								stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
							}
						}
						break;
					case PROMPT_INFO_TYPE:
						if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
						{//第一帧数据
							WREN();
							Block_Erase_32K(PROMPT_DATA_START_ADDR);
							Wait_Busy();							
							AAI_Write1(PROMPT_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);					
							stLocalControl.uiPromptDataLen = uiDataLen-5;
							
							//AAI_Write(PROMPT_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPromptDataLen)),2);	//保存真正的数据长度 ,原有此信息长度覆盖
						}
						else 
						{	//不进行擦除写
							AAI_Write1(PROMPT_DATA_START_ADDR+stLocalControl.uiPromptDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiPromptDataLen  += (uiDataLen-5);		//加数据长度
						}
						
						if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
						{//结束标志
							AAI_Write(PROMPT_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPromptDataLen)),2);	//保存数据长度
							if(stLocalControl.byDisplayFace !=TALK_FACE)
							{
								DisplayedLen=0;
								ShowFace(PROMPT_INFO_TYPE);
								stLocalControl.uiLcdDisplayInfoTime = LCD_DISPLAY_INFO_TIME;
							}
						}
						break;
						
			 	}
				memset(byUsart0RecBuf,0x00,UART0_RX_BUF_SIZE);
				
				stLocalControl.stBusDealFreq.byCmd = CMD_SB_DATA_ANSWER; 
				stLocalControl.stBusDealFreq.byRecSecAddr = 0X01;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		
			}		
		}	
		else
		{//数据长度不相符
			stLocalControl.stBusDealFreq.byCmd = CMD_SB_DATA_ANSWER; 	
			stLocalControl.stBusDealFreq.byRecSecAddr = 0X00;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
		}		
	}		
}

void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
#if DEBUG==1
    uint8 xdata i;
	WDT_CONTR = 0x3d;	//喂狗
    ES=0;
	for(i=0;i<l;i++)
     {
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;
	 }
	ES=1;
#endif

/*
#if DEBUG==1
	stUsartCfg[0].uiSndLen=l;
	stUsartCfg[0].uiSndPos=0;
	stUsartCfg[0].pbySndBuf=Databuf;
	SBUF=stUsartCfg[0].pbySndBuf[0];
#endif
*/
}


void Send_Data_Byte(uint8 SendData)
{ 
#if DEBUG==1
    ES=0;

    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
#endif	

/*	
#if DEBUG==1
	stUsartCfg[0].uiSndLen=1;
	stUsartCfg[0].uiSndPos=0; 
	SBUF=SendData;
#endif	
*/
}