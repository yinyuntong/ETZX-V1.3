#define _IN_UART_

#include"custom.h"

#define FOSC                        12000000                            /*  ����Ƶ��                  */
#define FCCLK                      (FOSC  * 8)                          /*  ��ʱ��Ƶ��<=100Mhz          */
#define FPCLK                      (FCCLK / 4)                          /*  ����ʱ��Ƶ��,FCCLK��1/2��1/4*/


//���ڱ�������
STUsartCfg  stUsartCfg[4];

//����0����   ����λ��ͨѸ
uint8_t	byUsart0SndBuf[UART0_TX_BUF_SIZE];		//����RS485����


//����1����   ��wifi ģ��ͨѸ
uint8_t	byUsart1SndBuf[UART1_TX_BUF_SIZE];		//����wifi����



//����2����	   ����λ��ͨѸ
uint8_t	byUsart2SndBuf[UART2_TX_BUF_SIZE];


//����3����	   ��������ʾоƬͨѸ
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
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void init_uart0 (uint32_t uart_bps)
{

	uint16_t ulFdiv;

	LPC_PINCON->PINSEL0 |= (0x01 << 4)|(0x01 << 6); 
	LPC_SC->PCONP |= 0x08;												/* �򿪴���0����				*/	  
	LPC_UART0->LCR	= 0x83; 											/*	�������ò�����				*/
	ulFdiv = (SystemFrequency / 4 / 16) / uart_bps; 					/*	���ò�����					*/
	LPC_UART0->DLM	= ulFdiv / 256;
	LPC_UART0->DLL	= ulFdiv % 256; 
	LPC_UART0->LCR	= 0x03; 											/*	����������					*/
	LPC_UART0->FCR	= 0x87; 											/*	ʹ��FIFO������8���ֽڴ����� */
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn, 8);
	LPC_UART0->IER	= 0x01; 
//	LPC_UART0->IER  |= 0x02;		//ʹ�ܷ��ͱ��ּĴ������ж�THRE

	scon0_ptr=scon0_Rcvbuf;
	scon0_len=0;
}
/*********************************************************************************************************
** Function name:     	uartInit
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void init_uart1 (uint32_t uart_bps)
{
    uint16_t ulFdiv;

    LPC_PINCON->PINSEL4 |= (0x02 |(0x02 << 2)); 						//p2.0 p2.1
    LPC_SC->PCONP |= (1<<4);                                               /* ��������1����ģ��            */    
    LPC_UART1->LCR  = 0x83;                                             /*  �������ò�����              */
    ulFdiv = (SystemFrequency / 4 / 16) / uart_bps;                     /*  ���ò�����                  */
    LPC_UART1->DLM  = ulFdiv / 256;
    LPC_UART1->DLL  = ulFdiv % 256; 
    LPC_UART1->LCR  = 0x03;                                             /*  ����������                  */
    LPC_UART1->FCR  = 0x87;                                             /*  ʹ��FIFO������8���ֽڴ����� */
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 9);
    LPC_UART1->IER  = 0x01;                                             /*  ʹ�ܽ����ж�                */

	scon1_ptr=scon1_Rcvbuf;
	scon1_len=0;

}
/*********************************************************************************************************
** Function name:     	uartInit
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void init_uart2 (uint32_t uart_bps)
{
    uint16_t ulFdiv;
	
    LPC_PINCON->PINSEL0 |= ((0x01<<20) |(0x01 << 22)); 
    LPC_SC->PCONP |= (1<<24);                                               /* ��������2����ģ��            */    
    LPC_UART2->LCR  = 0x83;                                             /*  �������ò�����              */
    ulFdiv = (SystemFrequency / 4 / 16) / uart_bps;                     /*  ���ò�����                  */
    LPC_UART2->DLM  = ulFdiv / 256;
    LPC_UART2->DLL  = ulFdiv % 256; 
    LPC_UART2->LCR  = 0x03;                                             /*  ����������                  */
    LPC_UART2->FCR  = 0x87;                                                                                   /*  ʹ��FIFO������8���ֽڴ����� */
    NVIC_EnableIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn, 10);
    LPC_UART2->IER  =  0x01;                                    //  ʹ�ܽ����ж�  
	
	scon2_ptr=scon2_Rcvbuf;
	scon2_len=0; 
}
/*********************************************************************************************************
** Function name:     	init_uart3
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void init_uart3 (uint32_t uart_bps)
{
    uint16_t ulFdiv;

    LPC_PINCON->PINSEL9 |= ((0x03<<24) |(0x03 << 26)); 					//p4.28 --TXD3     P4.29--RXD3
    LPC_SC->PCONP |= (1<<25);                                               /* ����UART3�Ĺ��ʼĴ�������λ  */ 
    LPC_UART3->LCR  = 0x83;                                             /*  �������ò�����              */
    ulFdiv = (SystemFrequency / 4 / 16) / uart_bps;                     /*  ���ò�����                  */
    LPC_UART3->DLM  = ulFdiv / 256;
    LPC_UART3->DLL  = ulFdiv % 256; 
    LPC_UART3->LCR  = 0x03;                                             /*  ����������                  */
    LPC_UART3->FCR  = 0x87;                                             /*  ʹ��FIFO������8���ֽڴ����� */
    NVIC_EnableIRQ(UART3_IRQn);
    NVIC_SetPriority(UART3_IRQn, 11);
    LPC_UART3->IER  = 0x01;                                             /*  ʹ�ܽ����ж�                */

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
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    uiDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart0_send_byte (uint8_t ch)
{
	LPC_UART0->IER &= ~0X02;
    LPC_UART0->THR = ch;                                                /*  д������                    */
    while ((LPC_UART0->LSR & 0x20) == 0);                               /*  �ȴ����ݷ������            */

}
/*********************************************************************************************************
** Function name:	    uart1_send_byte
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    uiDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart1_send_byte (uint8_t ch)
{
    LPC_UART1->THR = ch;                                                /*  д������                    */
    while ((LPC_UART1->LSR & 0x20) == 0);                               /*  �ȴ����ݷ������            */

}
/*********************************************************************************************************
** Function name:	    uart2_send_byte
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    uiDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart2_send_byte (uint8_t ch)
{
    LPC_UART2->THR = ch;                                                /*  д������                    */
    while ((LPC_UART2->LSR & 0x20) == 0);                               /*  �ȴ����ݷ������            */
	
}
/*********************************************************************************************************
** Function name:	    uart3_send_byte
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    uiDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart3_send_byte (uint8_t ch)
{
   LPC_UART3->THR = ch;                                                /*  д������                    */
   while ((LPC_UART3->LSR & 0x20) == 0);                               /*  �ȴ����ݷ������            */
	
}
/*********************************************************************************************************
** Function name:	    uart0_send_str
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
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
*��������			:GetPskCrc	
*��������        	:�����������ۼ�У���
*�������   		:pstDataFreq:�������������ָ��
*����ֵ				:�ۼ�У���
*ȫ�ֱ���			:stUsartCfg
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
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
*��������			:UsartSndCmd	
*��������        	:���ڷ��������PC��
*�������   		
                                    byCmd:����
                                    byLen:���ݳ���
                                    pData:����ָ��
*����ֵ				:
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
void Usart2SndCmd(STAddr_S sur_addr,uint8_t Cmd,uint8_t byLen,uint8_t *pData)
{
	pSTUartFreq pstDataFreq;
	uint8_t errtime;

	
	if((stEepromCfgData.bySysMode & USE_USART2_MODE) ==0)	return;	//��ʹ�ô���ģʽ�����������ݣ�ֱ���˳�

	errtime =0xff;
	while(stUsartCfg[2].uiSndLen!=0)	
	{//���������ڷ�����
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
	pstDataFreq->byCmd			=Cmd;		//����
	pstDataFreq->byLenH			=0x00;
	pstDataFreq->byLenL			=byLen;
	if(byLen)
	{
		memcpy(pstDataFreq->byData,pData,byLen);	//��������
	}

	
	pstDataFreq->byData[byLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[byLen+1] = END_BYTE;
	
	stUsartCfg[2].uiSndLen	= byLen+EXT_SIZE;
	stUsartCfg[2].uiSndPos = 0; 	//������֡ͷ����ʼ��������


	LPC_UART2->THR = pstDataFreq->byStartByte1;	
	LPC_UART2->IER  |= 0x02;		//�������ж�
}





void Usart2RecDeal(void)
{	
	uint16_t data_len;
	STAddr_S source_addr;
	uint8_t  data_buff[10];
	
	pSTUartFreq pstDataFreq=scon2_Rcvbuf;

	//��ʼλ
	if(pstDataFreq->byStartByte1!= START_BYTE1) return;

	//���ݳ���
	data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
	if( data_len> MAX_UART2_DATA_LEN) return;

	//����λ
	if(pstDataFreq->byData[data_len+1] != END_BYTE) return;

	//У���
	if(!(CheckPsk(pstDataFreq))) 	return;


	if(scon2_Rcvbuf[CMD_POSITION]  == CMD_ENTER_PC)
	{		
		return;	//�յ�PC����ȷ������
	}

	memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
	data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
	data_buff[1] = TRUE;
	Usart2SndCmd(source_addr,CMD_ENTER_PC,2,data_buff); //����ȷ������	

	pstDataFreq = (pSTUartFreq)scon2_Rcvbuf;

	if((pstDataFreq->byCmd!= CMD_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_HANDLE_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_DISPLAY_FORM))
	{
		ClrState(bRS485DataTreat);		//��Ϊ���ݿ����Ѹ��ǣ���485���ݴ����־��0
	}
	
	switch(pstDataFreq->byCmd)
	{
		case CMD_VOICE_PLAY_ROOM:	//�����Ƿ񲥱�����

			stEepromCfgData.bPlayRoom= pstDataFreq->byData[0];
			save_parameter();
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_VOICE_PLAY_ROOM;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //���Ͳ����ɹ�����
			break;
		
		case CMD_DATE_SEND:		//Уʱ����
			memcpy(&(stTime.bySecond),&(scon2_Rcvbuf[DATA0_POSITION]),6);
			
			//���������ϵ�Уʱ���ݰ�   
			if((stTime.bySecond>=0x60)||(stTime.byMinute>=0x60)
			 ||(stTime.byHour>=0x24)||(stTime.byDay>=0x32)
			 ||(stTime.byMonth>=0x13)||(stTime.byYearL>0x99))
			{
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_DATE_SEND;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //���Ͳ���ʧ������			
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
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //���Ͳ����ɹ�����				
			break;
		case CMD_POWER_ON:
  			if((pstDataFreq->byData[0] == 0xff) ||(pstDataFreq->byData[1] == 0xff) ||(pstDataFreq->byData[2] == 0xff)||
				(pstDataFreq->byRecRoomAddr ==GLZJ_ADDR2)   )
  			{//ȫ���豸����
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_POWER_ON;
				data_buff[1] = TRUE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); //���Ͳ����ɹ����� 

				while(stUsartCfg[2].uiSndLen!=0)	
				{//���������ڷ�����,�ȴ�����뷢�����

				}

	    		while(1);
  			}
			else
			{//ĳ����һ�豸����
				stBusFreq.byCmd = CMD_POWER_ON;
				stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
				stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
				stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
    		break;
		case CMD_DISPLAY_DATA:	 //һ֡Һ��������
		case CMD_HANDLE_DISPLAY_DATA:	//����һ֡�ֱ� Һ��������
			if(stEepromCfgData.bySysMode & USE_USART2_MODE)
			{
				if(stUsartCfg[0].uiSndLen )
				{//RS485���������ڷ���
					SetState(bRS485DataTreat);
					return;
				}


				pstDataFreq = (pSTUartFreq)scon2_Rcvbuf;
				data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
				__disable_irq();
				memcpy(byUsart0SndBuf,scon2_Rcvbuf,data_len+EXT_SIZE);
				__enable_irq();
							
				stUsartCfg[0].uiSndLen = data_len+EXT_SIZE;
				stUsartCfg[0].uiSndPos = 0; 	//������֡ͷ����ʼ��������	
				
				LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
				LPC_UART0->IER  |= 0x02;		//������0�����ж�
				
				//����ֻ���ַ					
				stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
				stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
				stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

				//���õȴ�Ӧ��ʱ2S  �ڲ�����Ϊ57.6K�������,100�ֽ�����ֻҪ17.3ms
				byWaitRS485AckDTime = 95;		//�ȴ�1.9S	
			}
			break;
			
		case CMD_DISPLAY_FORM:	//����һ֡��ͷ�ֻ���ʽ
			if(stUsartCfg[0].uiSndLen )
			{//RS485���������ڷ���
			
				SetState(bRS485DataTreat);
				return;
			}

			pstDataFreq = (pSTUartFreq)scon2_Rcvbuf;
			data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
			__disable_irq();
			memcpy(byUsart0SndBuf,scon2_Rcvbuf,data_len+EXT_SIZE);
			__enable_irq();

						
			stUsartCfg[0].uiSndLen = data_len+EXT_SIZE;
			stUsartCfg[0].uiSndPos = 0; 	//������֡ͷ����ʼ��������	

			LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
			LPC_UART0->IER  |= 0x02;		//������0�����ж�

			//����ֻ���ַ					
			stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
			stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
			stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;			
			break;
	
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(uiDevState & FLAG_BUSY)
			{	//æ���˳�
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}
			memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			stBusFreq.byCmd = pstDataFreq->byCmd;	//ԭ����
			memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
		case CMD_INFUSION_CLEAR:
		case CMD_SERVICE_CLEAR:
		case CMD_EMERGENCY_CLEAR:
		case CMD_HELP_CLEAR:
			memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			stBusFreq.byCmd = pstDataFreq->byCmd;	//ԭ����
			memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
		case CMD_COMM_CALL:
			if(uiDevState & FLAG_BUSY)
			{	//ϵͳæ
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 

			}
			else
			{
				memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				stBusFreq.byCmd = CMD_COMM_CALL;	//ԭ����
				memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
			break;	
		case CMD_TALK_CLEAR:
			if(0x0000 == (uiDevState & (FLAG_TALK1|FLAG_TALK)))
			{//ͨ��0��ͨ��1��δ���ڶԽ�״̬
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 

				break;
			}
			
			if(uiDevState & FLAG_CHANNEL_CHANGE_EXIST)	
			{//��ͨ���л������߷��Ͷ�����
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}
			
			if((uiDevState & FLAG_TALK))
			{//ͨ��0���ڶԽ�״̬
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
				{//��ͨ��0�еķֻ�,����ϵͳ��λ����
					MakeCH0TimerOut(5,0);//	���̳�ʱʱ��,׼���˳�
					memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
					data_buff[0] = CMD_TALK_CLEAR;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
					break;
				}
			}
			
			if((uiDevState & FLAG_TALK1))
			{//ͨ��1���ڶԽ�״̬
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


				{//��ͨ��1�еķֻ�,����ϵͳ��λ����
					MakeCH1TimerOut(5,0);//	���̳�ʱʱ��,׼���˳�
					memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
					data_buff[0] = CMD_TALK_CLEAR;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
					break;

				}
			}

			//��ַ����
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_TALK_CLEAR;
			data_buff[1] = FALSE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			break;
			
		case CMD_NUMBER_SET:		//���ݵ�ַ�޸Ĺ���������ַ
			if(stEepromCfgData.bySysMode & USE_USART2_MODE)
			{		
				if(scon2_Rcvbuf[REC_ROOM_POSITION]==GLZJ_ADDR2)
				{//����������������,ͬʱ�������ŷ��͸�������λ���豸
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
			{//����������������,ͬʱ�������ŷ��͸�������λ���豸
				if(stEepromCfgData.bySysMode != scon2_Rcvbuf[DATA0_POSITION])
				{
					stEepromCfgData.bySysMode= scon2_Rcvbuf[DATA0_POSITION];
					save_parameter();
					while(1);	//ϵͳ����
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
			
		case CMD_SCHEDULE_SEND:	//�������޸�����		
			stBusFreq.bySndSecAddr = scon2_Rcvbuf[DATA3_POSITION]; //ADDR��data
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
			{	//ϵͳæ
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
			{	//ϵͳæ
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
			
			stEepromCfgData.byLVoiceStartHour =  scon2_Rcvbuf[DATA0_POSITION]; //��ʼСʱ
			stEepromCfgData.byLVoiceStartMin =  scon2_Rcvbuf[DATA1_POSITION];	//��ʼ����
			stEepromCfgData.byLVoiceEndHour  = scon2_Rcvbuf[DATA2_POSITION]; //����Сʱ
			stEepromCfgData.byLVoiceEndMin = scon2_Rcvbuf[DATA3_POSITION];	//��������
			stEepromCfgData.byLVoiceVal =  scon2_Rcvbuf[DATA4_POSITION]; //����ֵ
			save_parameter();
			
			stBusFreq.bySndSecAddr = stEepromCfgData.byLVoiceStartHour; //��ʼСʱ
			stBusFreq.bySndRoomAddr = stEepromCfgData.byLVoiceStartMin;	//��ʼ����
			stBusFreq.bySndBedAddr = 0x00;
			stBusFreq.byCmd = CMD_VOICE_TIMER_SET;
			stBusFreq.byRecSecAddr = stEepromCfgData.byLVoiceEndHour; //����Сʱ
			stBusFreq.byRecRoomAddr = stEepromCfgData.byLVoiceEndMin;	//��������
			stBusFreq.byRecBedAddr = stEepromCfgData.byLVoiceVal; //����ֵ
			Bus0OutputData(&(stBusFreq.bySndSecAddr));

			check_little_voice_status();				//����Ƿ�������С����ʱ��,������������ñ�־λ,������ʼ�ͽ���ʱ�丳ֵ
			break;
			
		case CMD_BL_TIMER_SET:
			if((scon2_Rcvbuf[DATA0_POSITION]>0x23) || (scon2_Rcvbuf[DATA1_POSITION]>0x59) ||
			   (scon2_Rcvbuf[DATA2_POSITION]>0x23) || (scon2_Rcvbuf[DATA3_POSITION]>0x59) )	//ʱ����Ϣ����
			   return;
			stEepromCfgData.byBLOffStartHour = 	scon2_Rcvbuf[DATA0_POSITION];	//��ʼСʱ
			stEepromCfgData.byBLOffStartMin = 	scon2_Rcvbuf[DATA1_POSITION];	//��ʼ����
			stEepromCfgData.byBLOffEndHour = 	scon2_Rcvbuf[DATA2_POSITION]; //����Сʱ
			stEepromCfgData.byBLOffEndMin =  	scon2_Rcvbuf[DATA3_POSITION];	//��������
			save_parameter();
			
			stBusFreq.bySndSecAddr = stEepromCfgData.byBLOffStartHour; //��ʼСʱ
			stBusFreq.bySndRoomAddr = stEepromCfgData.byBLOffStartMin;	//��ʼ����
			stBusFreq.bySndBedAddr = 0x00;
			stBusFreq.byCmd = CMD_BL_TIMER_SET;
			stBusFreq.byRecSecAddr = stEepromCfgData.byBLOffEndHour; //����Сʱ
			stBusFreq.byRecRoomAddr = stEepromCfgData.byBLOffEndMin;	//��������
			stBusFreq.byRecBedAddr = 0x00; //����ֵ
			Bus0OutputData(&(stBusFreq.bySndSecAddr));

			init_black_light_status();
			break;	

			
		case CMD_ENTER:	//����PC���ϵĵǼ�ȷ������
			if(byWaitAckTimeLanding)
			{//���ڵȴ�PC���ϵĵǼ�ȷ������
				byWaitAckTimeLanding = 0;
				uiDetectPcTime = 30000;	//����PC�����10����
				
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;	
				stBusFreq.byCmd = CMD_PC_OPEN;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr =0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				SetState(bPCOpen);   //������������״̬
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
				//ͬʱ��RS485���ߵĲ��أ�����0����������
				init_uart0(stEepromCfgData.uiRs485Brt);
				save_parameter();				
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_RS485_BRT_SET;
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];	//�����ʴ���
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
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];	//��ͷ�ֻ�����RS485��Ϣ�Ƿ���Ҫ�жϷ���
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndRs485RoomTimes = RE_SND_TIMES;			//�ظ���������
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
				stBusFreq.byRecSecAddr	=	scon2_Rcvbuf[DATA0_POSITION];	//��ͷ�ֻ��Ƿ���Ҫ��������
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndBedVoiPlaTimes = RE_SND_TIMES;			//�ظ���������	
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
				case 1://����������������
					stEepromCfgData.byHostPlayVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;
				case 2://�������ŷֻ�����
					stEepromCfgData.bySlaPlayVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;				
				case 3://�Խ�ͨ������
					stEepromCfgData.byHostTalkVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;
				case 4://�Խ��ֻ�����
					stEepromCfgData.bySlaTalkVol = scon2_Rcvbuf[DATA1_POSITION];
					save_parameter();
					break;						
			}
			//��default���ͽ������				
		default:
			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff); 		//������� ������ɹ������������ط�
			break;
	}
}



