/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:uart.c
*文件描述    		:串口驱动程序
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

#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "singlebus.h"
#include "uart.h"
#include "E2PROM.h"
#include "string.h"
#include "cc1101.h"
#include "led.h"
#include "main.h"
#include <stdio.h>

STUartControl stUartControl;								//串口控制结构体
INT8U byUartBuff[UART0_BUF_SIZE];        				//给读串口消息队列分配的队列空间
const INT8U NumberTable[10][16] =      //常用数字点阵数据
{	
	0xFF,0xFF,0xC3,0x81,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x81,0xC3,0xFF,0xFF,0xFF, 	//0
	0xFF,0xFF,0xE7,0x87,0x87,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xFF,0xFF,0xFF, 	//1
	0xFF,0xFF,0xC3,0x81,0x99,0x99,0xF1,0xF3,0xE7,0xC7,0x8F,0x81,0x81,0xFF,0xFF,0xFF,	//2
	0xFF,0xFF,0xC3,0x81,0x99,0xF9,0xE3,0xE3,0xF9,0x99,0x99,0x81,0xC3,0xFF,0xFF,0xFF,	//3
	0xFF,0xFF,0xF3,0xE3,0xC3,0xD3,0x93,0x33,0x01,0x01,0xF3,0xF3,0xF3,0xFF,0xFF,0xFF, 	//4
	0xFF,0xFF,0x81,0x81,0x9F,0x9F,0x83,0x81,0x99,0xF9,0x99,0x81,0xC3,0xFF,0xFF,0xFF,	//5
	0xFF,0xFF,0xC3,0x81,0x99,0x9F,0x83,0x81,0x99,0x99,0x99,0x81,0xC3,0xFF,0xFF,0xFF, 	//6
	0xFF,0xFF,0x81,0x81,0xF9,0xF3,0xF3,0xE3,0xE7,0xE7,0xE7,0xC7,0xC7,0xFF,0xFF,0xFF,  	//7
	0xFF,0xFF,0xC3,0x81,0x99,0x99,0xC3,0xC3,0x99,0x99,0x99,0x81,0xC3,0xFF,0xFF,0xFF,	//8
	0xFF,0xFF,0xC3,0x81,0x99,0x99,0x99,0x81,0xC1,0xF9,0x99,0x81,0xC3,0xFF,0xFF,0xFF, 	//9
};

extern STBusFreq stBusDealFreq;									//总线数据临时变量


INT8U PreDispColor=0;


void uartSendByte (INT8U ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  写入数据                    */
    while ((LPC_UART->LSR & 0x40) == 0);                                /*  等待数据发送完毕            */
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
INT8U CheckPsk(pSTUartFreq pstDataFreq)
{
	INT8U  *pbyData;
	INT16U  uiLen;
	INT8U  byCrc = 0;

	
	pbyData = &(pstDataFreq->bySndSecAddr);
	
	uiLen = pstDataFreq->byLenHigh*256+ pstDataFreq->byLenLow + FREQ_HEAD_SIZE-1;
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
*函数名称			:UartInit	
*函数描述        	:串口初始化
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
void UartInit(INT8U baud)
{
	INT32U usFdiv;
	//控制字初始化
	stUartControl.byState = 0x00; 
	stUartControl.byTimerout = 0x00;
	stUartControl.byRxHead = stUartControl.byRxTail = 0x00;	

	//P3.2接收控制
	LPC_IOCON->PIO3_2 &=~0x07;
	LPC_GPIO3->DATA &= ~(1<<2); //允许接收
	LPC_GPIO3->DIR |= (1<<2);
	LPC_GPIO3->DATA &= ~(1<<2); //允许接收

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                             /* 使能IOCON时钟               */
    LPC_IOCON->PIO1_6 |= 0x01;                                          /* 将P1.6 1.7配置为RXD和TXD    */
    LPC_IOCON->PIO1_7 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);                               /* 打开UART功能部件时钟         */
    LPC_SYSCON->UARTCLKDIV       = 0x01;                                /* UART时钟分频                 */

    LPC_UART->LCR  = 0x83;                                              /* 允许设置波特率               */
	switch (baud)
	{
	case 1:
		usFdiv=1200;
	 	break;
	case 2:
	 	usFdiv=2400;
	 	break;
	case 3:
	 usFdiv=4800;
	 break;
	case 4:
	 usFdiv=9600;
	 break;
	case 5:
	 usFdiv=14400;
	 break;
	case 6:
	 usFdiv=19200;
	 break;
	case 7:
	 usFdiv=28800;
	 break;
	case 8:
	 usFdiv=38400;
	 break;
	case 9:
	 usFdiv=57600;
	 break;
	case 10:
	 usFdiv=115200;
	default:
	 usFdiv = 57600;
	 break;
	}
    usFdiv = (SystemFrequency/LPC_SYSCON->UARTCLKDIV/16)/usFdiv;      /* 设置波特率                   */
    LPC_UART->DLM  = usFdiv / 256;
    LPC_UART->DLL  = usFdiv % 256;
    LPC_UART->LCR  = 0x03;                                              /* 锁定波特率                   */
    LPC_UART->FCR  = 0x87;                                              /* 使能FIFO，设置8个字节触发点  */
    
    NVIC_EnableIRQ(UART_IRQn);                                          /* 使能UART中断，并配置优先级   */
    NVIC_SetPriority(UART_IRQn, 5);

    LPC_UART->IER  = 0x01;                                              /* 使能接收中断                 */
}

void UartReInit(INT8U baud)
{
	LPC_UART->IER = 0;
	NVIC_DisableIRQ(UART_IRQn); //关中断
	UartInit(baud);
}

/**********************************************************
*函数名称			:UartDeal	
*函数描述        	:串口接收到一帧数据处理函数
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
void UartDeal(void)
{ 
	
//   INT8U *pbyData;
//	INT32U tempaddr;
	uint16_t len;

    STUartFreq *pstUartFreq= (STUartFreq*)&(byUartBuff[stUartControl.byDealHead]);	

	len= pstUartFreq->byLenHigh*256+ pstUartFreq->byLenLow;

	if((pstUartFreq->byCmd) != CMD_DISPLAY_DATA)
	{	//命令不正确
UartRecError:
		stBusDealFreq.bySndSecAddr = SystemInfo.LOCAL_ADDR;
		stBusDealFreq.bySndRoomAddr = 240;
		stBusDealFreq.bySndBedAddr = 0;
		stBusDealFreq.byCmd = CMD_DATA_ERROR; 			
		Bus0OutputData(&stBusDealFreq.bySndSecAddr); 
		return;
	}


	if(pstUartFreq->stLcdCtlData.byDataType != 0x00)  goto UartRecError; 	////数据码不是内码

	if(pstUartFreq->stLcdCtlData.byInfoType != 0x05)  goto UartRecError;   //不是走廊显示屏的数据


    if((len-7)> sizeof(SystemInfo.DisplayInfo.MessageBuf))  goto UartRecError;   //数据长度超限

	if(pstUartFreq->stLcdCtlData.byEndFlag ==0x00) 
	{
		if(pstUartFreq->byData[0] !=0x20)
		{//红色数据
			
			
			memset((void *)(SystemInfo.DisplayInfo.MessageBuf),0x00,MESG_BUF_LEN);
			memcpy((void *)(SystemInfo.DisplayInfo.MessageBuf),(void *)(pstUartFreq->byData),len-7);	

			SaveParameter(SYS_INFO_ADD);
			SaveParameter(SYS_INFO_ADD_BAK);

			if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
			else
			{
				PreDispColor = RED;		//置预显示颜色，但不真正显示

			}
UartRecOk:	
			stBusDealFreq.bySndSecAddr = SystemInfo.LOCAL_ADDR;
			stBusDealFreq.bySndRoomAddr = 240;
			stBusDealFreq.bySndBedAddr = 0;
			stBusDealFreq.byCmd = 0x02; 
			stBusDealFreq.byRecSecAddr = CMD_DISPLAY_DATA;
			Bus0OutputData(&stBusDealFreq.bySndSecAddr);  
			return;		
		}
		else 
		{
			PreDispColor =0;
			goto UartRecOk;
		}
	}

    else if(pstUartFreq->stLcdCtlData.byEndFlag ==0x01) 
	{
		if(pstUartFreq->byData[0] !=0x20)
		{//绿色数据
			memset((INT8U *)(SystemInfo.DisplayInfo.MessageBuf),0x00,MESG_BUF_LEN);
			memcpy((INT8U *)(SystemInfo.DisplayInfo.MessageBuf),(INT8U *)(pstUartFreq->byData),len-7);	

				
			if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
			else
			{
				if(PreDispColor == RED)	
				{	
					SystemInfo.DisplayInfo.color = YELLOW; 
				}
				else 		
				{
					SystemInfo.DisplayInfo.color = GREEN; 
				}
				PreDispColor =0;			
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGEING;
				SystemInfo.DisplayInfo.ChangeDelay=MESSAGE_CHANGED_DELAY+1 /*31*/;
				
			}

			
			SaveParameter(SYS_INFO_ADD);
			SaveParameter(SYS_INFO_ADD_BAK);

			goto UartRecOk;
	
		}
		else
		{
			if(PreDispColor ==RED)
			{
				SystemInfo.DisplayInfo.color = RED;
				PreDispColor =0;			
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGEING;
				SystemInfo.DisplayInfo.ChangeDelay=MESSAGE_CHANGED_DELAY+1 /*31*/;


			}
			else
			{
				memset((void*)SystemInfo.DisplayInfo.MessageBuf,0,MESG_BUF_LEN);
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
				SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;				
			}
			SaveParameter(SYS_INFO_ADD);
			SaveParameter(SYS_INFO_ADD_BAK);
				
			goto UartRecOk;
		}
	}

	else if(0x02 == pstUartFreq->stLcdCtlData.byEndFlag)		                  //....如果是清屏命令
	{
	   	memset((void*)SystemInfo.DisplayInfo.MessageBuf,0,MESG_BUF_LEN);
		SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;

		SaveParameter(SYS_INFO_ADD);
		SaveParameter(SYS_INFO_ADD_BAK);		
	    goto UartRecOk;	
	}	
}
/**********************************************************
*函数名称			:UartManager	
*函数描述        	:串口管理线程
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
void UartManager(void)
{
	INT8U byTemp;
	if (stUartControl.byRxHead != stUartControl.byRxTail)
	{
		//有数据,设置接收超时		
		stUartControl.byTimerout = 15;
		byTemp = byUartBuff[stUartControl.byRxHead];
		stUartControl.byRxHead = (stUartControl.byRxHead + 1) % UART0_BUF_SIZE;					
		switch(stUartControl.byState)
		{
			case 0:											//起始阶段					
				if(byTemp == START_BYTE1)
				{	//起始字符正确					
					stUartControl.byCrc = 0x00;					
					stUartControl.byState = 1; 	
					stUartControl.uiLen = 3;  

					stUartControl.byDealHead = (stUartControl.byRxHead +UART0_BUF_SIZE -1)%UART0_BUF_SIZE;
				} 			
				break;
			case 1:
				stUartControl.byCrc += byTemp;	          //开始接收发送地址			
				if(--stUartControl.uiLen == 0x00)
				{	
				    stUartControl.byState = 2; 							
				}
				break;
			case 2:	 				
				//if(SystemInfo.LOCAL_ADDR == byTemp)
				{	//接收区地址
					stUartControl.byCrc += byTemp;					
					stUartControl.byState = 3; 							
				}
				/*else
				{
					stUartControl.byState = 0;
					stUartControl.byRxHead = 0;
					stUartControl.byRxTail = 0;
				}*/
				break;
			case 3:
				if(240 == byTemp)
				{	//接收房地址
					stUartControl.byCrc += byTemp;					
					stUartControl.byState = 4; 
				}
				else
				{
					stUartControl.byState = 0;
					stUartControl.byRxHead = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 4:
				if(0 == byTemp)
				{	//接收床地址
					stUartControl.byCrc += byTemp;						
					stUartControl.byState = 5; 	 					
				}
				else
				{
					stUartControl.byState = 0;
					stUartControl.byRxHead = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 5:
				stUartControl.byCrc += byTemp;				//接收命令					
				stUartControl.byState = 6; 
				break;
			case 6:										   //接收数据长度高字节
				stUartControl.byCrc += byTemp;	
				stUartControl.uiLen = byTemp;
				stUartControl.byState = 7;
				break;
			case 7:
				stUartControl.byCrc += byTemp;			   //接收数据长度低字节	
				//stUartControl.uiLen *= 256;
				//stUartControl.uiLen += byTemp;
				stUartControl.uiLen = byTemp;	
				if(stUartControl.uiLen >7 )
				{ 					
					stUartControl.byState = 8;	
				}
				else
				{  					
					stUartControl.byState = 0;
					stUartControl.byRxHead = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 8:
				stUartControl.byCrc += byTemp;					
				if(--stUartControl.uiLen == 0x00)
				{	//所有数据接收完成
					stUartControl.byState = 9; 	
				}
				break;
		 	case 9:
				if(stUartControl.byCrc == byTemp)
				{	//校验和正确  				
					stUartControl.byState = 10;
				}
				else
				{
					stUartControl.byState = 0;
					stUartControl.byRxHead = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 10:
				if(byTemp == END_BYTE)
				{	//数据帧正确				
					UartDeal();					
				}
				stUartControl.byState = 0;
				stUartControl.byRxHead = 0;
				stUartControl.byRxTail = 0;
				break;
		   	default:
				stUartControl.byState = 0;
				stUartControl.byRxHead = 0;
				stUartControl.byRxTail = 0;
				break;					
		}
	}
	else
	{
		//没有数据
		if(stUartControl.byTimerout==1)
		{
			stUartControl.byTimerout=0;	
			//超时到
			stUartControl.byState = 0x00;
			stUartControl.byRxHead = 0;
			stUartControl.byRxTail = 0;
		}
	}		 
}
/**********************************************************
*函数名称			:UartInt	
*函数描述        	:串口中断函数
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
/*********************************************************************************************************
* Function Name:        UART_IRQHandler
* Description:          UART中断服务函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void UART_IRQHandler (void)
{
    INT8U GulNum = 0;

    while ((LPC_UART->IIR & 0x01) == 0)
	{                                /*  判断是否有中断挂起          */
        switch (LPC_UART->IIR & 0x0E)
		{                                  /*  判断中断标志                */
        
            case 0x04:                                                  /*  接收数据中断                */
                //GucRcvNew = 1;                                          /*  置接收新数据标志            */
                for (GulNum = 0; GulNum < 8; GulNum++)
				{                 /*  连续接收8个字节             */

					byUartBuff[stUartControl.byRxTail] = LPC_UART->RBR;
					stUartControl.byRxTail = (stUartControl.byRxTail + 1) % UART0_BUF_SIZE;

                }
                break;
            
            case 0x0C:                                                  /*  字符超时中断                */
                //GucRcvNew = 1;
                while ((LPC_UART->LSR & 0x01) == 0x01)
				{                 /*  判断数据是否接收完毕        */
                    byUartBuff[stUartControl.byRxTail] = LPC_UART->RBR; 
					stUartControl.byRxTail = (stUartControl.byRxTail + 1) % UART0_BUF_SIZE;
                }
                break;
                
            default:
                break;
        }
    }
}
