/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:uart.c
*�ļ�����    		:������������
*������     		:������
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
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

STUartControl stUartControl;								//���ڿ��ƽṹ��
INT8U byUartBuff[UART0_BUF_SIZE];        				//����������Ϣ���з���Ķ��пռ�
const INT8U NumberTable[10][16] =      //�������ֵ�������
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

extern STBusFreq stBusDealFreq;									//����������ʱ����


INT8U PreDispColor=0;


void uartSendByte (INT8U ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  д������                    */
    while ((LPC_UART->LSR & 0x40) == 0);                                /*  �ȴ����ݷ������            */
}



/**********************************************************
*��������			:CheckPsk	
*��������        	:�������ݰ�����ȷ��
*�������   		:pstDataFreq:��У���������ָ��
*����ֵ				:true:���ݰ���ȷ,false:���ݰ�����
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
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
*��������			:UartInit	
*��������        	:���ڳ�ʼ��
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void UartInit(INT8U baud)
{
	INT32U usFdiv;
	//�����ֳ�ʼ��
	stUartControl.byState = 0x00; 
	stUartControl.byTimerout = 0x00;
	stUartControl.byRxHead = stUartControl.byRxTail = 0x00;	

	//P3.2���տ���
	LPC_IOCON->PIO3_2 &=~0x07;
	LPC_GPIO3->DATA &= ~(1<<2); //�������
	LPC_GPIO3->DIR |= (1<<2);
	LPC_GPIO3->DATA &= ~(1<<2); //�������

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                             /* ʹ��IOCONʱ��               */
    LPC_IOCON->PIO1_6 |= 0x01;                                          /* ��P1.6 1.7����ΪRXD��TXD    */
    LPC_IOCON->PIO1_7 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);                               /* ��UART���ܲ���ʱ��         */
    LPC_SYSCON->UARTCLKDIV       = 0x01;                                /* UARTʱ�ӷ�Ƶ                 */

    LPC_UART->LCR  = 0x83;                                              /* �������ò�����               */
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
    usFdiv = (SystemFrequency/LPC_SYSCON->UARTCLKDIV/16)/usFdiv;      /* ���ò�����                   */
    LPC_UART->DLM  = usFdiv / 256;
    LPC_UART->DLL  = usFdiv % 256;
    LPC_UART->LCR  = 0x03;                                              /* ����������                   */
    LPC_UART->FCR  = 0x87;                                              /* ʹ��FIFO������8���ֽڴ�����  */
    
    NVIC_EnableIRQ(UART_IRQn);                                          /* ʹ��UART�жϣ����������ȼ�   */
    NVIC_SetPriority(UART_IRQn, 5);

    LPC_UART->IER  = 0x01;                                              /* ʹ�ܽ����ж�                 */
}

void UartReInit(INT8U baud)
{
	LPC_UART->IER = 0;
	NVIC_DisableIRQ(UART_IRQn); //���ж�
	UartInit(baud);
}

/**********************************************************
*��������			:UartDeal	
*��������        	:���ڽ��յ�һ֡���ݴ�����
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void UartDeal(void)
{ 
	
//   INT8U *pbyData;
//	INT32U tempaddr;
	uint16_t len;

    STUartFreq *pstUartFreq= (STUartFreq*)&(byUartBuff[stUartControl.byDealHead]);	

	len= pstUartFreq->byLenHigh*256+ pstUartFreq->byLenLow;

	if((pstUartFreq->byCmd) != CMD_DISPLAY_DATA)
	{	//�����ȷ
UartRecError:
		stBusDealFreq.bySndSecAddr = SystemInfo.LOCAL_ADDR;
		stBusDealFreq.bySndRoomAddr = 240;
		stBusDealFreq.bySndBedAddr = 0;
		stBusDealFreq.byCmd = CMD_DATA_ERROR; 			
		Bus0OutputData(&stBusDealFreq.bySndSecAddr); 
		return;
	}


	if(pstUartFreq->stLcdCtlData.byDataType != 0x00)  goto UartRecError; 	////�����벻������

	if(pstUartFreq->stLcdCtlData.byInfoType != 0x05)  goto UartRecError;   //����������ʾ��������


    if((len-7)> sizeof(SystemInfo.DisplayInfo.MessageBuf))  goto UartRecError;   //���ݳ��ȳ���

	if(pstUartFreq->stLcdCtlData.byEndFlag ==0x00) 
	{
		if(pstUartFreq->byData[0] !=0x20)
		{//��ɫ����
			
			
			memset((void *)(SystemInfo.DisplayInfo.MessageBuf),0x00,MESG_BUF_LEN);
			memcpy((void *)(SystemInfo.DisplayInfo.MessageBuf),(void *)(pstUartFreq->byData),len-7);	

			SaveParameter(SYS_INFO_ADD);
			SaveParameter(SYS_INFO_ADD_BAK);

			if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
			else
			{
				PreDispColor = RED;		//��Ԥ��ʾ��ɫ������������ʾ

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
		{//��ɫ����
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

	else if(0x02 == pstUartFreq->stLcdCtlData.byEndFlag)		                  //....�������������
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
*��������			:UartManager	
*��������        	:���ڹ����߳�
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void UartManager(void)
{
	INT8U byTemp;
	if (stUartControl.byRxHead != stUartControl.byRxTail)
	{
		//������,���ý��ճ�ʱ		
		stUartControl.byTimerout = 15;
		byTemp = byUartBuff[stUartControl.byRxHead];
		stUartControl.byRxHead = (stUartControl.byRxHead + 1) % UART0_BUF_SIZE;					
		switch(stUartControl.byState)
		{
			case 0:											//��ʼ�׶�					
				if(byTemp == START_BYTE1)
				{	//��ʼ�ַ���ȷ					
					stUartControl.byCrc = 0x00;					
					stUartControl.byState = 1; 	
					stUartControl.uiLen = 3;  

					stUartControl.byDealHead = (stUartControl.byRxHead +UART0_BUF_SIZE -1)%UART0_BUF_SIZE;
				} 			
				break;
			case 1:
				stUartControl.byCrc += byTemp;	          //��ʼ���շ��͵�ַ			
				if(--stUartControl.uiLen == 0x00)
				{	
				    stUartControl.byState = 2; 							
				}
				break;
			case 2:	 				
				//if(SystemInfo.LOCAL_ADDR == byTemp)
				{	//��������ַ
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
				{	//���շ���ַ
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
				{	//���մ���ַ
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
				stUartControl.byCrc += byTemp;				//��������					
				stUartControl.byState = 6; 
				break;
			case 6:										   //�������ݳ��ȸ��ֽ�
				stUartControl.byCrc += byTemp;	
				stUartControl.uiLen = byTemp;
				stUartControl.byState = 7;
				break;
			case 7:
				stUartControl.byCrc += byTemp;			   //�������ݳ��ȵ��ֽ�	
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
				{	//�������ݽ������
					stUartControl.byState = 9; 	
				}
				break;
		 	case 9:
				if(stUartControl.byCrc == byTemp)
				{	//У�����ȷ  				
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
				{	//����֡��ȷ				
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
		//û������
		if(stUartControl.byTimerout==1)
		{
			stUartControl.byTimerout=0;	
			//��ʱ��
			stUartControl.byState = 0x00;
			stUartControl.byRxHead = 0;
			stUartControl.byRxTail = 0;
		}
	}		 
}
/**********************************************************
*��������			:UartInt	
*��������        	:�����жϺ���
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
/*********************************************************************************************************
* Function Name:        UART_IRQHandler
* Description:          UART�жϷ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void UART_IRQHandler (void)
{
    INT8U GulNum = 0;

    while ((LPC_UART->IIR & 0x01) == 0)
	{                                /*  �ж��Ƿ����жϹ���          */
        switch (LPC_UART->IIR & 0x0E)
		{                                  /*  �ж��жϱ�־                */
        
            case 0x04:                                                  /*  ���������ж�                */
                //GucRcvNew = 1;                                          /*  �ý��������ݱ�־            */
                for (GulNum = 0; GulNum < 8; GulNum++)
				{                 /*  ��������8���ֽ�             */

					byUartBuff[stUartControl.byRxTail] = LPC_UART->RBR;
					stUartControl.byRxTail = (stUartControl.byRxTail + 1) % UART0_BUF_SIZE;

                }
                break;
            
            case 0x0C:                                                  /*  �ַ���ʱ�ж�                */
                //GucRcvNew = 1;
                while ((LPC_UART->LSR & 0x01) == 0x01)
				{                 /*  �ж������Ƿ�������        */
                    byUartBuff[stUartControl.byRxTail] = LPC_UART->RBR; 
					stUartControl.byRxTail = (stUartControl.byRxTail + 1) % UART0_BUF_SIZE;
                }
                break;
                
            default:
                break;
        }
    }
}
