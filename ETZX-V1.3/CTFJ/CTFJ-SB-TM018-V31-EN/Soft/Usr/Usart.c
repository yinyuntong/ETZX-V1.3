/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Uart.c
*�ļ�����    		:������������
*������     		:����ͬ
*��������   		:2009-4-6
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_USART_
#include "config.h"

//���ڱ�������
STUsartCfg  xdata stUsartCfg[1];
#define IncRxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiRecBufSize)
#define IncTxPtr(byPort,addr) 		(addr=(addr+1)%stUsartCfg[byPort].uiSndBufSize)



uint8	xdata byUsart0RecBuf[UART0_RX_BUF_SIZE];



bit bUsart0RecFinish	= 0;
bit bRS485DataError		=0;


/**********************************************************
*��������			:UsartInit	
*��������        	:���ڳ�ʼ������
*�������   		:
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
void UsartInit(void)
{
	AUXR	&=	~BRTR;		//ֹͣ���������ʷ����� 
	
	//rxd����Ϊ��������̬
	P3M0 &= 0xfe;
	P3M1 |= 0x01;
	
	//����0��ʼ��
	stUsartCfg[0].pbyRecBuf = byUsart0RecBuf;
	stUsartCfg[0].pbySndBuf = NULL;
	stUsartCfg[0].byRecState=0;
	stUsartCfg[0].uiRecPosition=0;
	stUsartCfg[0].uiRecLen=0;
	stUsartCfg[0].byRecCount=0;	
	stUsartCfg[0].uiSndLen  = stUsartCfg[0].uiSndPos  = 0;
	
	SCON=0X50;		//������ʽ1
	
/*	TMOD&=0X0F;		//TO��ʽ����
	TMOD|=0X20;		//T1 ��ʽ2 :8λ�Զ�����
	TL1	=	0XF6;		//������4800  ��18.432M����������
	TH1	=	TL1;
    PCON&=	~SMOD;   //  ;�����ʱ���ѡ��		smod=0
  	AUXR |= T1x12;		//1Tģʽ,�������Ϊ:4800*12=57.6K
    TF1=0;
    TR1=1;*/
    PCON&=	~SMOD;   //  ;�����ʱ���ѡ��		smod=0
    AUXR	|=	S1BRS;	//���ò����ʷ�����
    
 	if(stLocalControl.stEepromCfgData.uiRs485Brt>=2400)
	{
		AUXR	|=	BRTx12; 	//BRTX12=1; 1Tģʽ,�������Ϊ:4800*12=57.6K
//		BRT 	=	0XF6;
		BRT= 256-(Fosc/32/stLocalControl.stEepromCfgData.uiRs485Brt	);
		
	
}
	else
	{
		AUXR	&=	~BRTx12; 	//BRTX12=0; 12Tģʽ
//		BRT 	=	0XF6;
		BRT= 256-(Fosc/32/12/stLocalControl.stEepromCfgData.uiRs485Brt	);		
	}   

	AUXR	|=	BRTR;		//�������������ʷ�����
	
	RI=0;
	TI=0;
	REN=1;
	ES=1;
}
/**********************************************************
*��������			:UART0(void)	
*��������        	:����0�жϺ���
*�������   		:
*����ֵ				:
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
#pragma disable
void UART0(void) interrupt UART_INTNO
{
	uint8 byTemp;
	if(RI==1)
	{  
		RI=0;
		byTemp=SBUF;
		if(stUsartCfg[0].uiRecPosition!=0)	//������ʼ�ֽ�
		{
			if(stUsartCfg[0].byRecTimeout==0)
			{//��ʱ����
			 	stUsartCfg[0].byRecState=0;
				stUsartCfg[0].uiRecPosition=0;
				return;
			}
		}
		stUsartCfg[0].byRecTimeout=5;	//�ֿ�ʼ��ʱ
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
				{	//�����ַ����																		
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
				{	//���������ݳ���
					//stUsartCfg[0].uiRecLen += (byTemp << 8); 			//��ȡ�����򳤶�ֵ
					stUsartCfg[0].uiRecLen += byTemp;
					if(stUsartCfg[0].uiRecLen <= MAX_UART0_DATA_LEN)					
					{	//���ݳ��ȷ���Ҫ��
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
				{	//��������������
					if(END_BYTE == byTemp)
					{	//����λ��ȷ
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
*��������			:UART1(void)	
*��������        	:����2�жϺ���
*�������   		:
*����ֵ				:
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
*��������			:Usart0Manager	
*��������        	:����0������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stUsartCfg,stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
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
	{//�Ǳ�����ַ
	
		if(uiDataLen>=5)
		{	
			if(!CheckPsk(pstDataFreq))	
			{//����֡����
				stLocalControl.stBusDealFreq.byCmd = CMD_SB_DATA_ANSWER; 	
				stLocalControl.stBusDealFreq.byRecSecAddr = 0X00;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			}
			else
			{	
				if(byUsart0RecBuf[DATA_POSITION4] == 0x01)	//һ����Ϣ����		
				{//�ָ�MICԭ����״̬
					MIC_CTRL=MicCtrlState;
				}
				
				switch(byUsart0RecBuf[DATA_POSITION1])	//��Ϣ����
			 	{		 	
				 	case DIET_TYPE:
						if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
						{//��һ֡����
							SpiNormalMode();
							WREN();
							Block_Erase_32K(DIET_DATA_START_ADDR);
							Wait_Busy();
							AAI_Write1(DIET_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiDietDataLen = (uiDataLen-5);
							
							//AAI_Write(DIET_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiDietDataLen)),2);	//�������������ݳ��� ,ԭ�д���Ϣ���ȸ���
							SpiLcdMode();
						}
						else 
						{	//�����в���д
							SpiNormalMode();
							AAI_Write1(DIET_DATA_START_ADDR+stLocalControl.uiDietDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiDietDataLen  += (uiDataLen-5);		//�����ݳ���
							SpiLcdMode();
						}

						if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
						{//������־
							SpiNormalMode();
							AAI_Write(DIET_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiDietDataLen)),2);	//�������ݳ���
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
						{//��һ֡����
							WREN();
							Block_Erase_32K(PRESCRIPTION_DATA_START_ADDR);
							Wait_Busy();							
							AAI_Write1(PRESCRIPTION_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);					
							stLocalControl.uiPrescriptionDataLen = uiDataLen-5;
							
							//AAI_Write(PRESCRIPTION_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),2);	//�������������ݳ��� ,ԭ�д���Ϣ���ȸ���
						}
						else 
						{	//�����в���д
							AAI_Write1(PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiPrescriptionDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiPrescriptionDataLen  += (uiDataLen-5);		//�����ݳ���						
						}

						if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
						{//������־
							AAI_Write(PRESCRIPTION_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),2);	//�������ݳ���
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
						{//��һ֡����
							WREN();
							Block_Erase_32K(PROMPT_DATA_START_ADDR);
							Wait_Busy();							
							AAI_Write1(PROMPT_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);					
							stLocalControl.uiPromptDataLen = uiDataLen-5;
							
							//AAI_Write(PROMPT_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPromptDataLen)),2);	//�������������ݳ��� ,ԭ�д���Ϣ���ȸ���
						}
						else 
						{	//�����в���д
							AAI_Write1(PROMPT_DATA_START_ADDR+stLocalControl.uiPromptDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiPromptDataLen  += (uiDataLen-5);		//�����ݳ���
						}
						
						if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
						{//������־
							AAI_Write(PROMPT_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPromptDataLen)),2);	//�������ݳ���
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
		{//���ݳ��Ȳ����
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
	WDT_CONTR = 0x3d;	//ι��
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