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
STUsartCfg  xdata stUsartCfg;
uint8	xdata byUsart0RecBuf[UART0_RX_BUF_SIZE];
bit bUsart0RecFinish	= 0;
bit bRS485DataError		= 0;
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
	TR1=0;
	
	//����0��ʼ��
	stUsartCfg.pbyRecBuf = byUsart0RecBuf;
	stUsartCfg.byRecState=0;
	stUsartCfg.uiRecPosition=0;
	stUsartCfg.uiRecLen=0;
	stUsartCfg.byRecCount=0;	
	
	SCON=0X50;		//������ʽ1
	TMOD&=0X0F;		//TO��ʽ����
	TMOD|=0X20;		//;t1 ��ʽ2�������ʷ�����

//	TL1 =	0XF6;		//������4800  ��18.432M����������
	if(stLocalControl.stEepromCfgData.uiRs485Brt>=2400)
	{
		AUXR |= T1x12;		//1Tģʽ,�������Ϊ:4800*12=57.6K
		TL1= 256-(Fosc/32/stLocalControl.stEepromCfgData.uiRs485Brt );	
	}
	
	else
	{
		AUXR	&=	~T1x12; //T1x12=0; 12Tģʽ
		TL1= 256-(Fosc/32/12/stLocalControl.stEepromCfgData.uiRs485Brt	);		
	}


	TH1	=	TL1;
    PCON	&=	~SMOD;   //  ;�����ʱ���ѡ��		smod=0
    TF1=0;
    TR1=1;
	
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
	uint8 xdata byTemp;
	if(RI==1)
	{  
		RI=0;
		byTemp=SBUF;
		if(stUsartCfg.uiRecPosition!=0)	//������ʼ�ֽ�
		{
			if(stUsartCfg.byRecTimeout==0)
			{//��ʱ����
			 	stUsartCfg.byRecState=0;
				stUsartCfg.uiRecPosition=0;
				return;
			}
		}
		stUsartCfg.byRecTimeout=5;	//�ֿ�ʼ��ʱ
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
				{	//�����ַ����																		
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
				{	//���������ݳ���
					//stUsartCfg[0].uiRecLen += (byTemp << 8); 			//��ȡ�����򳤶�ֵ
					stUsartCfg.uiRecLen += byTemp;
					if(stUsartCfg.uiRecLen <= MAX_UART0_DATA_LEN)					
					{	//���ݳ��ȷ���Ҫ��
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
				{	//��������������
					if(END_BYTE == byTemp)
					{	//����λ��ȷ
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
	{//�������е����ݳ��ȷ���Ҫ��	
		if( AddrCompare(&(stLocalControl.stBusDealFreq))&& ( pstDataFreq->byCmd == CMD_DISPLAY_DATA ))
		{//�Ǳ���Һ����ʾ����
			if(!CheckPsk(pstDataFreq))	
			{//����֡����	
				if(byUsart0RecBuf[DATA_POSITION4]==0)
				{//���ǽ���֡����
					bRS485DataError	=1;
				}
				else
				{//�����һ֡
					stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					bRS485DataError	=0;		//��־�ָ�
				}
			}

			
			else
			{//У�����ȷ

				if(byUsart0RecBuf[DATA_POSITION2] == 0x01)
				{//��һ֡����
					bRS485DataError = 0;
				}
				
				if(byUsart0RecBuf[DATA_POSITION4]==1)
				{//���һ֡����
					if(bRS485DataError ==0)
					{//һ����Ϣ֮ǰδ��������,����ȷ����
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
				{//δ������
				
					if(byUsart0RecBuf[DATA_POSITION1]==WARD_SEC_TYPE)	//������Ϣ����
					{
					
						Read_Cont(WARD_FACE_DATA_START_ADDR+stLocalControl.uiWardFaceDataLen,uiDataLen-5,WardDataBuff);
						for(i=0;i<uiDataLen-5;i++)
						{
							if(byUsart0RecBuf[DATA_POSITION5+i]==NO_CHANGE_DATA)
							{//����ı�����
								byUsart0RecBuf[DATA_POSITION5+i]=WardDataBuff[i];
							}
						}

						
						if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
						{//��һ֡����
							WREN();
							Block_Erase_32K(WARD_FACE_DATA_START_ADDR);
							Wait_Busy();
							
							AAI_Write(WARD_FACE_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
					
							stLocalControl.uiWardFaceDataLen = uiDataLen-5;
							

						}
						
						else 
						{	//�����в���д					
							AAI_Write1(WARD_FACE_DATA_START_ADDR+stLocalControl.uiWardFaceDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
							stLocalControl.uiWardFaceDataLen  += (uiDataLen-5);		//�����ݳ���					
						
						}

						if(byUsart0RecBuf[DATA_POSITION4])	
						{//һ�����ݽ���,������ʾ

							Read_Cont(WARD_FACE_DATA_START_ADDR,WARD_FACE_DATA_LEN,WardDataBuff);						
							StoreSecInfo(DIS_FRAM_MEM0);
							stLocalControl.uiWardFaceDataLen =0;
						}
					}

				}
				else
				{//������
					if(byUsart0RecBuf[DATA_POSITION4]==1)   bRS485DataError=0;
				}				
			}
		}
	}
}


/**********************************************************
*��������			:UsartManager
*��������        	:������0�����߳�
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-7
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void UsartManager(void)	 
{	
	while(true)
	{
		WDT_CONTR = 0x3d; 
		if(bUsart0RecFinish)									//����0�յ�һ֡����
		{ 	
			OSQPost(byMainCmdQ,UART0_CMD);	
		}
 
		OSWait(K_TMO, 1);
					
	}

}
