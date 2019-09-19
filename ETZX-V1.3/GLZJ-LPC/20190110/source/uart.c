#define _IN_UART_

#include"custom.h"

#define FOSC                        12000000                            /*  振荡器频率                  */
#define FCCLK                      (FOSC  * 8)                          /*  主时钟频率<=100Mhz          */
#define FPCLK                      (FCCLK / 4)                          /*  外设时钟频率,FCCLK的1/2、1/4*/


//串口变量定义
STUsartCfg  stUsartCfg[4];

//串口0配置   与下位机通迅
uint8_t	byUsart0SndBuf[UART0_TX_BUF_SIZE];		//发送RS485数据


//串口1配置   与wifi 模块通迅
uint8_t	byUsart1SndBuf[UART1_TX_BUF_SIZE];		//发送wifi数据



//串口2配置	   与上位机通迅
uint8_t	byUsart2SndBuf[UART2_TX_BUF_SIZE];


//串口3配置	   与来电显示芯片通迅
uint8_t	byUsart3SndBuf[UART3_TX_BUF_SIZE];



volatile uint8_t		scon0_Rcvbuf[UART0_RX_BUF_SIZE];
volatile uint8_t		scon1_Rcvbuf[UART1_RX_BUF_SIZE];
volatile uint8_t		scon2_Rcvbuf[UART2_RX_BUF_SIZE];
volatile uint8_t		scon3_Rcvbuf[UART3_RX_BUF_SIZE];


volatile uint8_t		scon0_time=0;
volatile uint8_t		scon0_tov;
volatile uint16_t		scon0_received=0;
volatile uint16_t		scon0_len=0;
volatile uint8_t		*scon0_ptr;
volatile uint8_t		scon0_GucRcvNew = 0;

volatile uint8_t		scon1_time=0;
volatile uint8_t		scon1_tov;
volatile uint16_t		scon1_received=0;
volatile uint16_t		scon1_len=0;
volatile uint8_t		*scon1_ptr;
volatile uint8_t		scon1_GucRcvNew = 0;


volatile uint8_t		scon2_time=0;
volatile uint8_t		scon2_tov;
volatile uint16_t		scon2_received=0;
volatile uint16_t		scon2_len=0;
volatile uint8_t		*scon2_ptr;
volatile uint8_t		scon2_GucRcvNew = 0;

volatile uint8_t		scon3_time=0;
volatile uint8_t		scon3_tov;
volatile uint16_t		scon3_received=0;
volatile uint16_t		scon3_len=0;
volatile uint8_t		*scon3_ptr;
//volatile uint8_t			uart3_auto_send;
volatile uint8_t		uart3_auto_time;



void init_uart0(uint32_t);
void init_uart1(uint32_t);
void init_uart2(uint32_t);
void init_uart3(uint32_t);
void uart0_send_byte (uint8_t);
void uart1_send_byte (uint8_t);
void uart2_send_byte (uint8_t);
void uart3_send_byte (uint8_t);
void uart0_send_str (uint8_t *);
void uart1_send_str (uint8_t *);
void uart2_send_str (uint8_t *);
void uart3_send_str (uint8_t *);


/*********************************************************************************************************
** Function name:     	uartInit
** Descriptions:	    串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为9600
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void init_uart0 (uint32_t uart_bps)
{

	uint16_t ulFdiv;

	LPC_PINCON->PINSEL0 |= (0x01 << 4)|(0x01 << 6); 
	LPC_SC->PCONP |= 0x08;												/* 打开串口0功能				*/	  
	LPC_UART0->LCR	= 0x83; 											/*	允许设置波特率				*/
	ulFdiv = (SystemFrequency / 4 / 16) / uart_bps; 					/*	设置波特率					*/
	LPC_UART0->DLM	= ulFdiv / 256;
	LPC_UART0->DLL	= ulFdiv % 256; 
	LPC_UART0->LCR	= 0x03; 											/*	锁定波特率					*/
	LPC_UART0->FCR	= 0x87; 											/*	使能FIFO，设置8个字节触发点 */
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn, 8);
	LPC_UART0->IER	= 0x01; 
//	LPC_UART0->IER  |= 0x02;		//使能发送保持寄存器空中断THRE

	scon0_ptr=scon0_Rcvbuf;
	scon0_len=0;
}
/*********************************************************************************************************
** Function name:     	uartInit
** Descriptions:	    串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为9600
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void init_uart1 (uint32_t uart_bps)
{
    uint16_t ulFdiv;

    LPC_PINCON->PINSEL4 |= (0x02 |(0x02 << 2)); 						//p2.0 p2.1
    LPC_SC->PCONP |= (1<<4);                                               /* 开启串口1功能模块            */    
    LPC_UART1->LCR  = 0x83;                                             /*  允许设置波特率              */
    ulFdiv = (SystemFrequency / 4 / 16) / uart_bps;                     /*  设置波特率                  */
    LPC_UART1->DLM  = ulFdiv / 256;
    LPC_UART1->DLL  = ulFdiv % 256; 
    LPC_UART1->LCR  = 0x03;                                             /*  锁定波特率                  */
    LPC_UART1->FCR  = 0x87;                                             /*  使能FIFO，设置8个字节触发点 */
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 9);
    LPC_UART1->IER  = 0x01;                                             /*  使能接收中断                */

	scon1_ptr=scon1_Rcvbuf;
	scon1_len=0;

}
/*********************************************************************************************************
** Function name:     	uartInit
** Descriptions:	    串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为9600
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void init_uart2 (uint32_t uart_bps)
{
    uint16_t ulFdiv;
	
    LPC_PINCON->PINSEL0 |= ((0x01<<20) |(0x01 << 22)); 
    LPC_SC->PCONP |= (1<<24);                                               /* 开启串口2功能模块            */    
    LPC_UART2->LCR  = 0x83;                                             /*  允许设置波特率              */
    ulFdiv = (SystemFrequency / 4 / 16) / uart_bps;                     /*  设置波特率                  */
    LPC_UART2->DLM  = ulFdiv / 256;
    LPC_UART2->DLL  = ulFdiv % 256; 
    LPC_UART2->LCR  = 0x03;                                             /*  锁定波特率                  */
    LPC_UART2->FCR  = 0x87;                                                                                   /*  使能FIFO，设置8个字节触发点 */
    NVIC_EnableIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn, 10);
    LPC_UART2->IER  =  0x01;                                    //  使能接收中断  
	
	scon2_ptr=scon2_Rcvbuf;
	scon2_len=0; 
}
/*********************************************************************************************************
** Function name:     	init_uart3
** Descriptions:	    串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为9600
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void init_uart3 (uint32_t uart_bps)
{
    uint16_t ulFdiv;

    LPC_PINCON->PINSEL9 |= ((0x03<<24) |(0x03 << 26)); 					//p4.28 --TXD3     P4.29--RXD3
    LPC_SC->PCONP |= (1<<25);                                               /* 开启UART3的功率寄存器控制位  */ 
    LPC_UART3->LCR  = 0x83;                                             /*  允许设置波特率              */
    ulFdiv = (SystemFrequency / 4 / 16) / uart_bps;                     /*  设置波特率                  */
    LPC_UART3->DLM  = ulFdiv / 256;
    LPC_UART3->DLL  = ulFdiv % 256; 
    LPC_UART3->LCR  = 0x03;                                             /*  锁定波特率                  */
    LPC_UART3->FCR  = 0x87;                                             /*  使能FIFO，设置8个字节触发点 */
    NVIC_EnableIRQ(UART3_IRQn);
    NVIC_SetPriority(UART3_IRQn, 11);
    LPC_UART3->IER  = 0x01;                                             /*  使能接收中断                */

	scon3_ptr=scon3_Rcvbuf;
	scon3_len=0; 
}

void init_scon0_Rcvbuf(void)
{
	uint16_t i;
	for(i=0;i<UART0_RX_BUF_SIZE;i++) scon0_Rcvbuf[i]=0;
}



void init_scon1_Rcvbuf(void)
{
	uint16_t i;
	for(i=0;i<UART1_RX_BUF_SIZE;i++) scon1_Rcvbuf[i]=0;
}


/*********************************************************************************************************
** Function name:	    uart0_send_byte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    uiDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart0_send_byte (uint8_t ch)
{
	LPC_UART0->IER &= ~0X02;
    LPC_UART0->THR = ch;                                                /*  写入数据                    */
    while ((LPC_UART0->LSR & 0x20) == 0);                               /*  等待数据发送完毕            */

}
/*********************************************************************************************************
** Function name:	    uart1_send_byte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    uiDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart1_send_byte (uint8_t ch)
{
    LPC_UART1->THR = ch;                                                /*  写入数据                    */
    while ((LPC_UART1->LSR & 0x20) == 0);                               /*  等待数据发送完毕            */

}
/*********************************************************************************************************
** Function name:	    uart2_send_byte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    uiDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2_send_byte (uint8_t ch)
{
    LPC_UART2->THR = ch;                                                /*  写入数据                    */
    while ((LPC_UART2->LSR & 0x20) == 0);                               /*  等待数据发送完毕            */
	
}
/*********************************************************************************************************
** Function name:	    uart3_send_byte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    uiDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart3_send_byte (uint8_t ch)
{
   LPC_UART3->THR = ch;                                                /*  写入数据                    */
   while ((LPC_UART3->LSR & 0x20) == 0);                               /*  等待数据发送完毕            */
	
}
/*********************************************************************************************************
** Function name:	    uart0_send_str
** Descriptions:	    向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart0_send_str (uint8_t *pucStr)
{

	uint8_t i;
	for(;;)
	{
	  i = *pucStr++;
	  if(i == NULL)	break;
	  else	uart0_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}


void uart0_send_str2 (uint8_t *pucStr,uint16_t len)
{

	uint8_t i;
	for(;len>0;len--)
	{
	  i = *pucStr++;
	  uart0_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}



/*********************************************************************************************************
** Function name:	    uart0_send_str
** Descriptions:	    向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart1_send_str (uint8_t *pucStr)
{

	uint8_t i;
	for(;;)
	{
	  i = *pucStr++;
	  if(i == NULL)	break;
	  else	uart1_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}

/*********************************************************************************************************
** Function name:	    uart0_send_str
** Descriptions:	    向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart1_send_str_len (uint8_t *pucStr,uint16_t len)
{

	uint8_t i;
	for(;len>0;len--)
	{
	  i = *pucStr++;
	  uart1_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}


/*********************************************************************************************************
** Function name:	    uart2_send_str
** Descriptions:	    向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2_send_str (uint8_t *pucStr)
{

	uint8_t i;
	for(;;)
	{
	  i = *pucStr++;
	  if(i == NULL)	break;
	  else	uart2_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}

/*********************************************************************************************************
** Function name:	    uart2_send_str2
** Descriptions:	    向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart2_send_str2 (uint8_t *pucStr,uint16_t num)
{
	uint8_t i;
	for(;num>0;num--)
	{
	  i = *pucStr++;
	  uart2_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}

/*********************************************************************************************************
** Function name:	    uart3_send_str
** Descriptions:	    向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart3_send_str (uint8_t *pucStr)
{

	uint8_t i;
	for(;;)
	{
	  i = *pucStr++;
	  if(i == NULL)	break;
	  else	uart3_send_byte(i);
#ifdef	enable_wdt
	  feed_wdt();
#endif

	}
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/



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
uint8_t GetPskCrc(pSTUartFreq pstDataFreq)
{
	uint8_t  *pbyData;
	uint16_t uiLen;
	uint8_t  byCrc = 0;
	uint16_t uiDataLen = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
	
	pbyData = &(pstDataFreq->bySndSecAddr);
	uiLen = uiDataLen + HEAD_SIZE;
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
uint8_t CheckPsk(pSTUartFreq pstDataFreq)
{
	uint8_t *pbyData;
	uint16_t uiLen;
	uint8_t  byCrc = 0;
	uint16_t uiDataLen = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;

	
	pbyData = &(pstDataFreq->bySndSecAddr);
	uiLen = uiDataLen + HEAD_SIZE;
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
void Usart2SndCmd(STAddr_S sur_addr,uint8_t Cmd,uint8_t byLen,uint8_t *pData)
{
	pSTUartFreq pstDataFreq;
	uint8_t errtime;

	
	if((stEepromCfgData.bySysMode & USE_USART2_MODE) ==0)	return;	//不使用串口模式，不发送数据，直接退出

	errtime =0xff;
	while(stUsartCfg[2].uiSndLen!=0)	
	{//有数据正在发送中
		soft_delay_half_ms(2);
		if(--errtime ==0) break;
	}
	pstDataFreq = (pSTUartFreq)byUsart2SndBuf;
	
	pstDataFreq->byStartByte1   =START_BYTE1;
	pstDataFreq->bySndSecAddr   =sur_addr.bySecAddr;
	pstDataFreq->bySndRoomAddr  =sur_addr.byRoomAddr;
	pstDataFreq->bySndBedAddr   =sur_addr.byBedAddr;
	pstDataFreq->byRecSecAddr   =stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr  =PC_ADDR2;
	pstDataFreq->byRecBedAddr   =PC_ADDR3;	
	pstDataFreq->byCmd			=Cmd;		//命令
	pstDataFreq->byLenH			=0x00;
	pstDataFreq->byLenL			=byLen;
	if(byLen)
	{
		memcpy(pstDataFreq->byData,pData,byLen);	//复制数据
	}

	
	pstDataFreq->byData[byLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[byLen+1] = END_BYTE;
	
	stUsartCfg[2].uiSndLen	= byLen+EXT_SIZE;
	stUsartCfg[2].uiSndPos = 0; 	//从数据帧头部开始发送数据


	LPC_UART2->THR = pstDataFreq->byStartByte1;	
	LPC_UART2->IER  |= 0x02;		//允许发送中断
}





void Usart2RecDeal(void)
{	
	uint16_t data_len;
	STAddr_S source_addr;
	uint8_t  data_buff[10];
	
	pSTUartFreq pstDataFreq=scon2_Rcvbuf;

	//起始位
	if(pstDataFreq->byStartByte1!= START_BYTE1) return;

	//数据长度
	data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
	if( data_len> MAX_UART2_DATA_LEN) return;

	//结束位
	if(pstDataFreq->byData[data_len+1] != END_BYTE) return;

	//校验和
	if(!(CheckPsk(pstDataFreq))) 	return;


	if(scon2_Rcvbuf[CMD_POSITION]  == CMD_ENTER_PC)
	{		
		return;	//收到PC机的确认命令
	}

	memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
	data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
	data_buff[1] = TRUE;
	Usart2SndCmd(source_addr,CMD_ENTER_PC,2,data_buff); //发送确认命令	

	pstDataFreq = (pSTUartFreq)scon2_Rcvbuf;

	if((pstDataFreq->byCmd!= CMD_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_HANDLE_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_DISPLAY_FORM))
	{
		ClrState(bRS485DataTreat);		//因为数据可能已覆盖，将485数据处理标志清0
	}
	
	switch(pstDataFreq->byCmd)
	{
		case CMD_VOICE_PLAY_ROOM:	//报号是否播报房号

			stEepromCfgData.bPlayRoom= pstDataFreq->byData[0];
			save_parameter();
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_VOICE_PLAY_ROOM;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作成功命令
			break;
		
		case CMD_DATE_SEND:		//校时数据
			memcpy(&(stTime.bySecond),&(scon2_Rcvbuf[DATA0_POSITION]),6);
			
			//构建总线上的校时数据包   
			if((stTime.bySecond>=0x60)||(stTime.byMinute>=0x60)
			 ||(stTime.byHour>=0x24)||(stTime.byDay>=0x32)
			 ||(stTime.byMonth>=0x13)||(stTime.byYearL>0x99))
			{
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_DATE_SEND;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作失败命令			
				break;
			}
			WriteDS3231_time();
			
			memcpy(&(stBusFreq.bySndSecAddr),&(stTime.bySecond),3);
		    memcpy(&(stBusFreq.byRecSecAddr),&(stTime.byDay),3);
		    stBusFreq.byCmd = CMD_DATE_SEND;   
		   	Bus0OutputData(&(stBusFreq.bySndSecAddr));
			
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_DATE_SEND;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作成功命令				
			break;
		case CMD_POWER_ON:
  			if((pstDataFreq->byData[0] == 0xff) ||(pstDataFreq->byData[1] == 0xff) ||(pstDataFreq->byData[2] == 0xff)||
				(pstDataFreq->byRecRoomAddr ==GLZJ_ADDR2)   )
  			{//全体设备重启
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_POWER_ON;
				data_buff[1] = TRUE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作成功命令 

				while(stUsartCfg[2].uiSndLen!=0)	
				{//有数据正在发送中,等待结果码发送完成

				}

	    		while(1);
  			}
			else
			{//某个单一设备重启
				stBusFreq.byCmd = CMD_POWER_ON;
				stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
				stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
				stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
    		break;
		case CMD_DISPLAY_DATA:	 //一帧液晶屏数据
		case CMD_HANDLE_DISPLAY_DATA:	//发送一帧手柄 液晶屏数据
			if(stEepromCfgData.bySysMode & USE_USART2_MODE)
			{
				if(stUsartCfg[0].uiSndLen )
				{//RS485有数据正在发送
					SetState(bRS485DataTreat);
					return;
				}


				pstDataFreq = (pSTUartFreq)scon2_Rcvbuf;
				data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
				__disable_irq();
				memcpy(byUsart0SndBuf,scon2_Rcvbuf,data_len+EXT_SIZE);
				__enable_irq();
							
				stUsartCfg[0].uiSndLen = data_len+EXT_SIZE;
				stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	
				
				LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
				LPC_UART0->IER  |= 0x02;		//允许串口0发送中断
				
				//保存分机地址					
				stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
				stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
				stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

				//设置等待应答超时2S  在波特率为57.6K的情况下,100字节数据只要17.3ms
				byWaitRS485AckDTime = 95;		//等待1.9S	
			}
			break;
			
		case CMD_DISPLAY_FORM:	//发送一帧床头分机格式
			if(stUsartCfg[0].uiSndLen )
			{//RS485有数据正在发送
			
				SetState(bRS485DataTreat);
				return;
			}

			pstDataFreq = (pSTUartFreq)scon2_Rcvbuf;
			data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
			__disable_irq();
			memcpy(byUsart0SndBuf,scon2_Rcvbuf,data_len+EXT_SIZE);
			__enable_irq();

						
			stUsartCfg[0].uiSndLen = data_len+EXT_SIZE;
			stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	

			LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
			LPC_UART0->IER  |= 0x02;		//允许串口0发送中断

			//保存分机地址					
			stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
			stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
			stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;			
			break;
	
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(uiDevState & FLAG_BUSY)
			{	//忙，退出
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}
			memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			stBusFreq.byCmd = pstDataFreq->byCmd;	//原命令
			memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
		case CMD_INFUSION_CLEAR:
		case CMD_SERVICE_CLEAR:
		case CMD_EMERGENCY_CLEAR:
		case CMD_HELP_CLEAR:
			memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			stBusFreq.byCmd = pstDataFreq->byCmd;	//原命令
			memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
		case CMD_COMM_CALL:
			if(uiDevState & FLAG_BUSY)
			{	//系统忙
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 

			}
			else
			{
				memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				stBusFreq.byCmd = CMD_COMM_CALL;	//原命令
				memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
			break;	
		case CMD_TALK_CLEAR:
			if(0x0000 == (uiDevState & (FLAG_TALK1|FLAG_TALK)))
			{//通道0与通道1都未处于对讲状态
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 

				break;
			}
			
			if(uiDevState & FLAG_CHANNEL_CHANGE_EXIST)	
			{//有通道切换在总线发送队列中
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}
			
			if((uiDevState & FLAG_TALK))
			{//通道0处于对讲状态
				if(((scon2_Rcvbuf[DATA0_POSITION] == stCallAddr.bySndSecAddr)&&
					(scon2_Rcvbuf[DATA1_POSITION] == stCallAddr.bySndRoomAddr)&&
					(scon2_Rcvbuf[DATA2_POSITION] == stCallAddr.bySndBedAddr))||
				   ((scon2_Rcvbuf[DATA0_POSITION] == stCallAddr.byRecSecAddr)&&
					(scon2_Rcvbuf[DATA1_POSITION] == stCallAddr.byRecRoomAddr)&&
					(scon2_Rcvbuf[DATA2_POSITION] == stCallAddr.byRecBedAddr))||				   
				   ((scon2_Rcvbuf[DATA3_POSITION] == stCallAddr.bySndSecAddr)&&
				    (scon2_Rcvbuf[DATA4_POSITION] == stCallAddr.bySndRoomAddr)&&
				    (scon2_Rcvbuf[DATA5_POSITION] == stCallAddr.bySndBedAddr))||
				   ((scon2_Rcvbuf[DATA3_POSITION] == stCallAddr.byRecSecAddr)&&
				    (scon2_Rcvbuf[DATA4_POSITION] == stCallAddr.byRecRoomAddr)&&
				    (scon2_Rcvbuf[DATA5_POSITION] == stCallAddr.byRecBedAddr)))
				{//是通道0中的分机,发送系统复位命令
					MakeCH0TimerOut(5,0);//	缩短超时时间,准备退出
					memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
					data_buff[0] = CMD_TALK_CLEAR;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
					break;
				}
			}
			
			if((uiDevState & FLAG_TALK1))
			{//通道1处于对讲状态
				if(((scon2_Rcvbuf[DATA0_POSITION] == stCH1CallAddr.bySndSecAddr)&&
					(scon2_Rcvbuf[DATA1_POSITION] == stCH1CallAddr.bySndRoomAddr)&&
					(scon2_Rcvbuf[DATA2_POSITION] == stCH1CallAddr.bySndBedAddr))||
				   ((scon2_Rcvbuf[DATA0_POSITION] == stCH1CallAddr.byRecSecAddr)&&
					(scon2_Rcvbuf[DATA1_POSITION] == stCH1CallAddr.byRecRoomAddr)&&
					(scon2_Rcvbuf[DATA2_POSITION] == stCH1CallAddr.byRecBedAddr))||					
				   ((scon2_Rcvbuf[DATA3_POSITION] == stCH1CallAddr.bySndSecAddr)&&
					(scon2_Rcvbuf[DATA4_POSITION] == stCH1CallAddr.bySndRoomAddr)&&
					(scon2_Rcvbuf[DATA5_POSITION] == stCH1CallAddr.bySndBedAddr))||
				   ((scon2_Rcvbuf[DATA3_POSITION] == stCH1CallAddr.byRecSecAddr)&&
					(scon2_Rcvbuf[DATA4_POSITION] == stCH1CallAddr.byRecRoomAddr)&&
					(scon2_Rcvbuf[DATA5_POSITION] == stCH1CallAddr.byRecBedAddr)))


				{//是通道1中的分机,发送系统复位命令
					MakeCH1TimerOut(5,0);//	缩短超时时间,准备退出
					memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
					data_buff[0] = CMD_TALK_CLEAR;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
					break;

				}
			}

			//地址不对
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_TALK_CLEAR;
			data_buff[1] = FALSE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			break;
			
		case CMD_NUMBER_SET:		//根据地址修改管理主机地址
			if(stEepromCfgData.bySysMode & USE_USART2_MODE)
			{		
				if(scon2_Rcvbuf[REC_ROOM_POSITION]==GLZJ_ADDR2)
				{//给管理主机编区号,同时将此区号发送给其它下位机设备
					stEepromCfgData.bySelfSecAddr = scon2_Rcvbuf[DATA0_POSITION];
					save_parameter();
					
					stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
					stBusFreq.bySndRoomAddr=	stEepromCfgData.bySelfRoomAddr;
					stBusFreq.bySndBedAddr =	stEepromCfgData.bySelfBedAddr;
					stBusFreq.byCmd		  =	CMD_SYSTEM_SEC_SET;
					stBusFreq.byRecSecAddr =	stEepromCfgData.bySelfSecAddr;
					stBusFreq.byRecRoomAddr = 0xff;
					stBusFreq.byRecBedAddr  = 0xff;
					Bus0OutputData(&(stBusFreq.bySndSecAddr));
				}
			}
			break;	

		case CMD_SYS_MOD_SET:
			if(scon2_Rcvbuf[REC_ROOM_POSITION]==GLZJ_ADDR2)
			{//给管理主机编区号,同时将此区号发送给其它下位机设备
				if(stEepromCfgData.bySysMode != scon2_Rcvbuf[DATA0_POSITION])
				{
					stEepromCfgData.bySysMode= scon2_Rcvbuf[DATA0_POSITION];
					save_parameter();
					while(1);	//系统重启
				}
			}			
			break;

		case CMD_QUEST:
			stBusFreq.byCmd = scon2_Rcvbuf[CMD_POSITION];
			stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;	
			
		case CMD_SCHEDULE_SEND:	//护理级别修改命令		
			stBusFreq.bySndSecAddr = scon2_Rcvbuf[DATA3_POSITION]; //ADDR或data
			stBusFreq.byCmd = scon2_Rcvbuf[CMD_POSITION];
			stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;	
		case CMD_OPEN_LCD:
		case CMD_CLOSE_LCD:
		case CMD_CLEAR_LCD:
		case CMD_CLEAR_HANDLE_LCD:
		case CMD_CLEAR_MK_LCD:
			stBusFreq.byCmd = scon2_Rcvbuf[CMD_POSITION];
			stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case CMD_STOP_VOICE:			
			stBusFreq.byCmd = scon2_Rcvbuf[CMD_POSITION];
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			ClrState(bOriginMusic);
			break;

		case CMD_START_VOICE:			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((uiDevState & FLAG_BUSY)||(uiDevState & FLAG_TALK1))
			{	//系统忙
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}

			stBusFreq.byCmd = scon2_Rcvbuf[CMD_POSITION];
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case CMD_MUSIC_PLAY:			
			if((uiDevState & FLAG_BUSY)||(uiDevState & FLAG_TALK1))
			{	//系统忙
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_MUSIC_PLAY;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}

			stBusFreq.byCmd = scon2_Rcvbuf[CMD_POSITION];
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			SetState(bOriginMusic);
			break;

			
		case CMD_VOICE_TIMER_SET:
			if((scon2_Rcvbuf[DATA0_POSITION]>0x23) || (scon2_Rcvbuf[DATA1_POSITION]>0x59) ||
			   (scon2_Rcvbuf[DATA2_POSITION]>0x23) || (scon2_Rcvbuf[DATA3_POSITION]>0x59) ||
			   (scon2_Rcvbuf[DATA4_POSITION]>8))
			   return;
			
			stEepromCfgData.byLVoiceStartHour =  scon2_Rcvbuf[DATA0_POSITION]; //起始小时
			stEepromCfgData.byLVoiceStartMin =  scon2_Rcvbuf[DATA1_POSITION];	//起始分钟
			stEepromCfgData.byLVoiceEndHour  = scon2_Rcvbuf[DATA2_POSITION]; //结束小时
			stEepromCfgData.byLVoiceEndMin = scon2_Rcvbuf[DATA3_POSITION];	//结束分钟
			stEepromCfgData.byLVoiceVal =  scon2_Rcvbuf[DATA4_POSITION]; //音量值
			save_parameter();
			
			stBusFreq.bySndSecAddr = stEepromCfgData.byLVoiceStartHour; //起始小时
			stBusFreq.bySndRoomAddr = stEepromCfgData.byLVoiceStartMin;	//起始分钟
			stBusFreq.bySndBedAddr = 0x00;
			stBusFreq.byCmd = CMD_VOICE_TIMER_SET;
			stBusFreq.byRecSecAddr = stEepromCfgData.byLVoiceEndHour; //结束小时
			stBusFreq.byRecRoomAddr = stEepromCfgData.byLVoiceEndMin;	//结束分钟
			stBusFreq.byRecBedAddr = stEepromCfgData.byLVoiceVal; //音量值
			Bus0OutputData(&(stBusFreq.bySndSecAddr));

			check_little_voice_status();				//检查是否设置了小音量时段,如果设置了则置标志位,并对起始和结束时间赋值
			break;
			
		case CMD_BL_TIMER_SET:
			if((scon2_Rcvbuf[DATA0_POSITION]>0x23) || (scon2_Rcvbuf[DATA1_POSITION]>0x59) ||
			   (scon2_Rcvbuf[DATA2_POSITION]>0x23) || (scon2_Rcvbuf[DATA3_POSITION]>0x59) )	//时间信息不对
			   return;
			stEepromCfgData.byBLOffStartHour = 	scon2_Rcvbuf[DATA0_POSITION];	//起始小时
			stEepromCfgData.byBLOffStartMin = 	scon2_Rcvbuf[DATA1_POSITION];	//起始分钟
			stEepromCfgData.byBLOffEndHour = 	scon2_Rcvbuf[DATA2_POSITION]; //结束小时
			stEepromCfgData.byBLOffEndMin =  	scon2_Rcvbuf[DATA3_POSITION];	//结束分钟
			save_parameter();
			
			stBusFreq.bySndSecAddr = stEepromCfgData.byBLOffStartHour; //起始小时
			stBusFreq.bySndRoomAddr = stEepromCfgData.byBLOffStartMin;	//起始分钟
			stBusFreq.bySndBedAddr = 0x00;
			stBusFreq.byCmd = CMD_BL_TIMER_SET;
			stBusFreq.byRecSecAddr = stEepromCfgData.byBLOffEndHour; //结束小时
			stBusFreq.byRecRoomAddr = stEepromCfgData.byBLOffEndMin;	//结束分钟
			stBusFreq.byRecBedAddr = 0x00; //音量值
			Bus0OutputData(&(stBusFreq.bySndSecAddr));

			init_black_light_status();
			break;	

			
		case CMD_ENTER:	//来自PC机上的登记确认命令
			if(byWaitAckTimeLanding)
			{//正在等待PC机上的登记确认命令
				byWaitAckTimeLanding = 0;
				uiDetectPcTime = 30000;	//侦听PC机间隔10分钟
				
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;	
				stBusFreq.byCmd = CMD_PC_OPEN;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr =0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				SetState(bPCOpen);   //集成主机开机状态
			}
			break;
			
		case CMD_RS485_BRT_SET:
			if(scon2_Rcvbuf[DATA0_POSITION] >0 && scon2_Rcvbuf[DATA0_POSITION] <10)
			{
				switch(scon2_Rcvbuf[DATA0_POSITION])
				{
					case 1:
						stEepromCfgData.uiRs485Brt	=1200;
						break;
					case 2:
						stEepromCfgData.uiRs485Brt	= 2400;
						break;
					case 3:
						stEepromCfgData.uiRs485Brt	= 4800;
						break;
					case 4:
						stEepromCfgData.uiRs485Brt	= 9600;
						break;
					case 5:
						stEepromCfgData.uiRs485Brt	= 14400;
						break;
					case 6:
						stEepromCfgData.uiRs485Brt	= 19200;
						break;
					case 7:
						stEepromCfgData.uiRs485Brt	= 28800;
						break;
					case 8:
						stEepromCfgData.uiRs485Brt	= 38400;
						break;
					case 9:
						stEepromCfgData.uiRs485Brt	= 57600;
						break;
					default:
						stEepromCfgData.uiRs485Brt	=1200;
						break;				
				}					
				//同时将RS485总线的波特（串口0）进行设置
				init_uart0(stEepromCfgData.uiRs485Brt);
				save_parameter();				
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_RS485_BRT_SET;
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];	//波特率代码
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndBrtTimes = RE_SND_TIMES;
			}
			else 
			{
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_RS485_BRT_SET;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			}
			break;

		case CMD_RS485_INFO_SEC_ROOM:
			if(scon2_Rcvbuf[DATA0_POSITION] ==0 || scon2_Rcvbuf[DATA0_POSITION] ==1)
			{
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_RS485_INFO_SEC_ROOM;
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];	//床头分机接怍RS485信息是否需要判断房号
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndRs485RoomTimes = RE_SND_TIMES;			//重复发送三次
			}
			else
			{
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_RS485_INFO_SEC_ROOM;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			}
			break;

		case CMD_BED_VOICE_PLAY:
			if(scon2_Rcvbuf[DATA0_POSITION] ==0 || scon2_Rcvbuf[DATA0_POSITION] ==1)
			{
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_BED_VOICE_PLAY;
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];	//床头分机是否需要语音报号
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndBedVoiPlaTimes = RE_SND_TIMES;			//重复发送三次	
			}
			else 
			{
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_BED_VOICE_PLAY;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			}
			break;
			
		case CMD_DISP_MODE:
			if(scon2_Rcvbuf[DATA0_POSITION] ==0 || scon2_Rcvbuf[DATA0_POSITION] ==1)
			{			
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_DISP_MODE;	
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];
				stBusFreq.byRecRoomAddr	=	scon2_Rcvbuf[DATA1_POSITION];
				stBusFreq.byRecBedAddr	=	scon2_Rcvbuf[DATA2_POSITION];
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
			else 
			{
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_DISP_MODE;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				
			}
			break;

		case CMD_COLOR_CLEAR:
			stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd			=	CMD_COLOR_CLEAR;	
			stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];
			stBusFreq.byRecRoomAddr	=	scon2_Rcvbuf[DATA1_POSITION];
			stBusFreq.byRecBedAddr	=	scon2_Rcvbuf[DATA2_POSITION];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case CMD_VOLUME_SET:
			switch(scon2_Rcvbuf[DATA0_POSITION])
			{
				case 1://语音报号主机音量
					stEepromCfgData.byHostPlayVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;
				case 2://语音报号分机音量
					stEepromCfgData.bySlaPlayVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;				
				case 3://对讲通话音量
					stEepromCfgData.byHostTalkVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;
				case 4://对讲分机音量
					stEepromCfgData.bySlaTalkVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;						
			}
			//继default发送结果命令				
		default:
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 		//其它命令都 发结果成功，以免引起重发
			break;
	}
}



