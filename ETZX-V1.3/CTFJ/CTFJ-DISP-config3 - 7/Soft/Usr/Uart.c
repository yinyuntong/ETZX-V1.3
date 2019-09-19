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
uint8   xdata byUsart0SndBuf[UART0_TX_BUF_SIZE];


bit bUsart0RecFinish= 0;
bit bRS485DataError = 0;

bit bUsartRecedDietData = 0;
bit bUsartRecedPrescriptionData = 0;
bit bUsartRecedPromptData =0;




extern STLocalControl xdata stLocalControl;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];


uint8 xdata Frame_No=0;		//֡��
uint8 xdata Frame_Type=0xff;	//֡���ݵ�����

uint16 xdata uiBedFaceFormLen=0;
uint16 xdata uiBedFaceDataLen=0;

bit bDispBedFace=0;
bit bDispBedForm=0;
bit bBus0AckOk=0;
bit bBus0AckFail=0;

uint8 CheckSum=0;	//������ͷ����Ϣ��У���
uint8 checkFormData(int16 datalen)
{
	uint32 dataaddr = BED_FACE_FORM_START_ADDR_2;
	STItemForm stItemForm;
	if(datalen%sizeof(STItemForm) ==0)
	{//��������Ҫ��
		if(datalen/sizeof(STItemForm)<MAX_ITEM_CODE)
		{
			for(;datalen>0;)
			{
				Read_Cont((uint8 *)&stItemForm,dataaddr,sizeof(STItemForm));
				if( stItemForm.ItemCode>=MAX_ITEM_CODE || stItemForm.StartPoint.x>=stItemForm.EndPoint.x ||stItemForm.StartPoint.y >=stItemForm.EndPoint.y || \
					stItemForm.EndPoint.x>=LCD_PIX_X || stItemForm.EndPoint.y >= LCD_PIX_Y || stItemForm.bcolor > COLOR_MAX_VAL || stItemForm.fcolor >COLOR_MAX_VAL)
						break;
				else
				{
					dataaddr+= sizeof(STItemForm);
					datalen -= sizeof(STItemForm);
				}
			}
			if(datalen ==0)
			{//����У��ȫ����ȷ
				return 1;
			}
			else return 0;
		}
		else return 0;
	}

	else return 0;

}


uint8 checkFaceData(int16 datalen)
{
	uint32 dataaddr = BED_FACE_DATA_START_ADDR_2;
	uint8 ItemData[2];   //ItemData[0]:��Ŀ����   //ItemData[1]:��Ŀ���ݳ���
	if(datalen<2) return 0;
	
	for(;datalen>0;)
	{
		Read_Cont(ItemData,dataaddr,2);
		if(ItemData[0]>=MAX_ITEM_CODE ) return 0;			//��Ŀ���볬�������

		dataaddr += (2+ ItemData[1]);
		
		if(datalen < (2+ItemData[1]))   return 0;			//��Ŀ���ȳ����ܳ���

		datalen -= (2+ ItemData[1]);

		if(datalen ==0) return 1;			//�ܳ����Ǹ����֮��

		else if(datalen<2 ) return 0;
		
	}
}



uint8 crcCheckData(uint32 addr,uint16 length,uint8 crc)
{
	uint8 i;
	uint8 crcSum=0;

	OS_ENTER_CRITICAL();
	do
	{
		if(length<=128)
		{
			Read_Cont(GT23_Rec_Data,addr,length);
			for(i=0;i<length;i++)
			{
				crcSum += GT23_Rec_Data[i];
			}
			if(crc == crcSum) i=1;
			else 			  i=0;
			break;
		}
		else
		{
			
			Read_Cont(GT23_Rec_Data,addr,128);
			for(i=0;i<128;i++)
			{
				crcSum += GT23_Rec_Data[i];
			}
			addr+=128;
			length -= 128;
			
			//��λ���Ź�
			WDT_CONTR = 0x3e; 			
		}		
	}while(length>0);
	WDT_CONTR = 0x3e;	//ι��
    OS_EXIT_CRITICAL();
	return i;
}


void SectorDataCopy(unsigned long dst,unsigned long src,uint16 length)
{
	OS_ENTER_CRITICAL();
	do
	{
		if(length<=128)
		{
			Read_Cont(GT23_Rec_Data,src,length);
			Write_Cont(dst,GT23_Rec_Data,length);
			break;
		}

		else
		{
			
			Read_Cont(GT23_Rec_Data,src,128);
			Write_Cont(dst,GT23_Rec_Data,128);
			src+=128;
			dst+=128;
			length -= 128;
			WDT_CONTR = 0x3e;	//ι��
			
		}
	}while(length>0);
	WDT_CONTR = 0x3e;	//ι��
    OS_EXIT_CRITICAL();

}


uint8 StoreDataCheck(unsigned long dst,uint16 length)
{
	uint8 i;
	uint8 crc=0;
	OS_ENTER_CRITICAL();
	while(1)
	{
		if(length<=128)
		{
			Read_Cont(GT23_Rec_Data,dst,length);
			for(i=0;i<length;i++) crc += GT23_Rec_Data[i];
		
			dst += length;
			if(Read(dst)== crc) i=1;	//���У������
			else i=0;
			OS_EXIT_CRITICAL();
			return i;
		}
		else
		{
			Read_Cont(GT23_Rec_Data,dst,128);
			for(i=0;i<128;i++) crc += GT23_Rec_Data[i];
			dst+=128;
			length -= 128;
			WDT_CONTR = 0x3e;	//ι��
		}
	}
}

uint8 IsSingleItem(uint8 *startaddr,uint8 length)
{
	if( *(startaddr+2)==1 && *(startaddr+4)==1 )
	{//һ������ֻ��һ֡����
		startaddr +=5;
		length -=5;

		if( (length-2)== *(startaddr+1) ) return 1;		// ֻ��һ��
		else return 0;
	}
	else return 0;
}


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

	//P3.1 TXD ��©����������Ŵ�ͷ�ֻ���MK������10k������5V
	P3M0 |= 0x02;
	P3M1 |= 0x02;
	TXD =1;

	//����0��ʼ��
	stUsartCfg.pbyRecBuf = byUsart0RecBuf;
	stUsartCfg.pbySndBuf = byUsart0SndBuf;	
	stUsartCfg.byRecState=0;
	stUsartCfg.uiRecPosition=0;
	stUsartCfg.uiRecLen=0;
	stUsartCfg.byRecCount=0;
	stUsartCfg.uiSndLen  = stUsartCfg.uiSndPos  = 0;
	
	SCON=0X50;		//������ʽ1
    PCON&=	~SMOD;   //  ;�����ʱ���ѡ��		smod=0
    AUXR	|=	S1BRS;	//���ò����ʷ�����


	if(stLocalControl.stEepromCfgData.uiRs485Brt>=2400)
	{
		AUXR	|=	BRTx12; 	//BRTX12=1; 1Tģʽ,�������Ϊ:4800*12=57.6K
		BRT= 256-(Fosc/32/stLocalControl.stEepromCfgData.uiRs485Brt	);
		
	}
	
	else
	{
		AUXR	&=	~BRTx12; 	//BRTX12=0; 12Tģʽ
		BRT= 256-(Fosc/32/12/stLocalControl.stEepromCfgData.uiRs485Brt	);		
	}


	
	AUXR	|=	BRTR;		//�������������ʷ�����   
	
	RI=0;
	TI=0;
	REN=1;
	ES=1;

}



void UsartInitConfig(uint16 boudRate)
{
	AUXR	&=	~BRTR;		//ֹͣ���������ʷ����� 

	//P3.1 TXD ��©����������Ŵ�ͷ�ֻ���MK������10k������5V
	P3M0 |= 0x02;
	P3M1 |= 0x02;
	TXD =1;

	//����0��ʼ��
	stUsartCfg.pbyRecBuf = byUsart0RecBuf;
	stUsartCfg.pbySndBuf = byUsart0SndBuf;	
	stUsartCfg.byRecState=0;
	stUsartCfg.uiRecPosition=0;
	stUsartCfg.uiRecLen=0;
	stUsartCfg.byRecCount=0;
	stUsartCfg.uiSndLen  = stUsartCfg.uiSndPos  = 0;
	
	SCON=0X50;		//������ʽ1
    PCON&=	~SMOD;   //  ;�����ʱ���ѡ��		smod=0
    AUXR	|=	S1BRS;	//���ò����ʷ�����

	if(boudRate>=2400)
	{
		AUXR	|=	BRTx12; 	//BRTX12=1; 1Tģʽ,�������Ϊ:4800*12=57.6K
		BRT= 256-(Fosc/32/boudRate);
		
	
	}
	else
	{
		AUXR	&=	~BRTx12; 	//BRTX12=0; 12Tģʽ
		BRT= 256-(Fosc/32/12/boudRate);		
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
	uint8 xdata byTemp;
	STUartFreq *xdata pstDataFreq;
	
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
					
					pstDataFreq =(STUartFreq *)byUsart0RecBuf;
					if((pstDataFreq->byRecSecAddr != 0xff) && (pstDataFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
					{//���Ų����	
						stUsartCfg.byRecState=0;
						stUsartCfg.uiRecPosition=0;
						return;

					} 

					if(stLocalControl.stEepromCfgData.byRs485SecRoom)//��Ҫ�жϷ���
					{
						if((pstDataFreq->byRecRoomAddr != 0xff) && (pstDataFreq->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr))
						{//���Ų����	
							stUsartCfg.byRecState=0;
							stUsartCfg.uiRecPosition=0;
							return;
						}
					}//����Ҫ�жϷ��žͲ���
					
					if((pstDataFreq->byRecBedAddr != 0xff) && (pstDataFreq->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
					{//���Ų����	
						stUsartCfg.byRecState=0;
						stUsartCfg.uiRecPosition=0;
						return;
					}				

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
	if(TI==1)
	{
		TI=0;

		pstDataFreq = (STUartFreq *)byUsart0SndBuf;
		stUsartCfg.uiSndLen--;
		stUsartCfg.uiSndPos++;		
		if(stUsartCfg.uiSndLen!=0)
		{    

			SBUF = stUsartCfg.pbySndBuf[stUsartCfg.uiSndPos];			
		}
		else
		{//һ֡���ݷ������			
			switch(pstDataFreq->byData[1])	//��Ϣ����
			{
			case DIET_TYPE:
				stLocalControl.byUsart1SdDelayTime =150;
				break;
				
			case PRESCRIPTION_TYPE:	
				stLocalControl.byUsart1SdDelayTime =150;
				break;
			case PROMPT_INFO_TYPE:
				stLocalControl.byUsart1SdDelayTime =150;
				break;
			}		
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
	uint8 xdata temp;    //bit4=1:����㲥
	uint16 xdata uiDataLen;


	SCON  &= 0XEF;      //�ر�REN
	uiDataLen = byUsart0RecBuf[LEN_HIGH_POSITION]*256+byUsart0RecBuf[LEN_LOW_POSITION];
	bUsart0RecFinish=0;

	pstDataFreq = (pSTUartFreq)byUsart0RecBuf;
	

	
	if(stLocalControl.stEepromCfgData.byRs485SecRoom==0x00)//����Ҫ�жϷ���
	{
		if(pstDataFreq->byRecBedAddr  == stLocalControl.stEepromCfgData.bySelfBedAddr)	//��ַ���
			temp =0x01;	//ֱ�ӵ�ַ
		else
		{
			if(pstDataFreq->byRecBedAddr==0xff)
			{//�����ǹ㲥��ַ
				if(pstDataFreq->byRecRoomAddr==0xff)
				{//�����ǹ㲥��ַ	
					temp =0x11;		//�㲥��ַ
				}
				else if(pstDataFreq->byRecRoomAddr<240)
				{//������ʱ�涨ֻ��240
					temp =0x11;    //�㲥��ַ
				}
				else   temp=0x00;
			}
			else temp =0x00;
		}
	}
	else	//��Ҫ�ж�
	{

		stLocalControl.stBusDealFreq.byRecSecAddr = pstDataFreq->byRecSecAddr;
		stLocalControl.stBusDealFreq.byRecRoomAddr = pstDataFreq->byRecRoomAddr;
		stLocalControl.stBusDealFreq.byRecBedAddr = pstDataFreq->byRecBedAddr;	
		if(AddrCompare(&(stLocalControl.stBusDealFreq)))
		{
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)&&
			   (stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
			   (stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				  temp =0x01;   //ֱ�ӵ�ַ
			else temp =0x11;    //�㲥��ַ
		}
		else temp=0;
	}


	if(temp&0x01)
	{//�Ǳ�������
	
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;	

		
		if(uiDataLen>5)
		{//�������е����ݳ��ȷ���Ҫ��		
			if(!CheckPsk(pstDataFreq))	
			{//����֡����	
				if(byUsart0RecBuf[DATA_POSITION4]==0)
				{//���ǽ���֡����
					bRS485DataError	=1;
					//Send_Data_Byte(0x01);
				}
				else
				{//�����һ֡
					//Send_Data_Byte(0x02);
					if( temp!=0x11)
					{//���ǹ㲥��ַ
						stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						
					}
					bRS485DataError	=0;		//��־�ָ�
					Frame_No =0;
					Frame_Type = 0xff;					
				}
			}

			
			else
			{//У�����ȷ
				//BL_SW=1;	//����ʾ������
				BL_ON();
				stLocalControl.uiLcdDisplayTime= 500;	//��10����


				if(byUsart0RecBuf[DATA_POSITION2] == 0x01)
				{//��һ֡����
					bRS485DataError = 0;
					Frame_No =0;
					Frame_Type =byUsart0RecBuf[DATA_POSITION1];
				}
								
				if(bRS485DataError==0)   
				{//δ������

					if( pstDataFreq->byCmd == CMD_DISPLAY_DATA ) 
					{	//������ͷҺ����ʾ����			

						if(byUsart0RecBuf[DATA_POSITION1]==BED_CARD_TYPE)	//BED_CARD_TYPE ������������ֵĴ�ͷ����Ϣ
						{
						
							if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
							{//��һ֡����

								//��������������Ч���ݵ�У���
								CheckSum = *(pstDataFreq->byData + pstDataFreq->uiLen);
								for(temp =0;temp<14;temp++) 	//��ַ6�ֽ�+����1�ֽ�+����2�ֽ�+��������5�ֽ�(��14�ֽ�)
								{
									CheckSum -= byUsart0RecBuf[temp+1];		//��sndsec��ʼ
								}	
								
								Frame_No =1;
								Frame_Type = byUsart0RecBuf[DATA_POSITION1];								
								Write_Cont(BED_FACE_DATA_START_ADDR_2,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
								uiBedFaceDataLen = uiDataLen-5;

							}
							
							else 
							{								
							/*	if(Frame_No >= byUsart0RecBuf[DATA_POSITION2]) 
								{//����֡�ű�ǰ֡��С,��Ӱ��ԭ���ݵĽ���
									goto Usart0RecDealRet;
								}*/
										
								if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
								{//û�ж�֡���ǰ�˳������
									if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
									{//��Ϣ����һ��
										Frame_No ++;
									    //�����в���д
										if(uiBedFaceDataLen < LENGTH_4K)
										{
											//��������������Ч���ݵ�У���
											CheckSum += *(pstDataFreq->byData + pstDataFreq->uiLen);
											for(temp =0;temp<14;temp++) 	//��ַ6�ֽ�+����1�ֽ�+����2�ֽ�+��������5�ֽ�(��14�ֽ�)
											{
												CheckSum -= byUsart0RecBuf[temp+1];		//��sndsec��ʼ
											}	
											
											Write_Cont(BED_FACE_DATA_START_ADDR_2+uiBedFaceDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
											uiBedFaceDataLen += (uiDataLen-5);
										}
										else  bRS485DataError=1;   //��ͷ���ܳ��ȳ���	

									}
									else  bRS485DataError=1;    //��Ϣ���Ͳ�һ��											
								}
								
								else   bRS485DataError=1;    //�м��ж�֡

							}
							
							if(byUsart0RecBuf[DATA_POSITION4])	
							{//һ�����ݽ���,������ʾ
								if(bRS485DataError ==0)
								{//δ������
									if(checkFaceData(uiBedFaceDataLen))
									{
										if(crcCheckData(BED_FACE_DATA_START_ADDR_2,uiBedFaceDataLen,CheckSum)==1)
										{//Ԥ����������У�����ȷ

											Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&uiBedFaceDataLen,2);		//д����
											SectorDataCopy(BED_FACE_DATA_START_ADDR,BED_FACE_DATA_START_ADDR_2,uiBedFaceDataLen); 	//д����
											CheckSum += uiBedFaceDataLen>>8;
											CheckSum += (uiBedFaceDataLen&0x00ff);
											Write_Cont(BED_FACE_DATA_START_ADDR+uiBedFaceDataLen,&CheckSum,1);	//дУ���
											//�ٽ���У��
											if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,uiBedFaceDataLen+2))	//�����ݳ���Ҳ������ȥ	
											{
												stLocalControl.uiBedFaceDataLen = uiBedFaceDataLen;										
	usart0RecDisp:	
												bDispBedFace=1;
												bBus0AckOk =1;
												stLocalControl.bySlaveState &= ~PATIENT_DISCHARGE; //������Ժ
											}
											else bBus0AckFail=1;
										}
										else bBus0AckFail =1;
									}
									else bBus0AckFail =1;
								}
								else 
								{
									bBus0AckFail=1;	
									goto usart0RecFailRet;	//����������ǿ���˳�
								}
							}	
						}
					}

					else if(pstDataFreq->byCmd ==  CMD_SB_LCD_DISPLAY_DATA)
					{//���ֱ�����			
						switch(byUsart0RecBuf[DATA_POSITION1])	//��Ϣ����
					 	{
						 	case DIET_TYPE:
								if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
								{//��һ֡����
									Frame_No =1;
									Frame_Type = byUsart0RecBuf[DATA_POSITION1];
								/*	WREN();
									Block_Erase_32K(DIET_DATA_START_ADDR);
									Wait_Busy();*/
									Write_Cont(DIET_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
															
									stLocalControl.uiDietDataLen = (uiDataLen-5);

								}
								else 
								{										
									if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
									{//û�ж�֡���ǰ�˳������
										if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
										{//��Ϣ����һ��
											Frame_No ++;
											if(stLocalControl.uiDietDataLen <LENGTH_32K)
											{
												Write_Cont(DIET_DATA_START_ADDR+stLocalControl.uiDietDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
												stLocalControl.uiDietDataLen  += (uiDataLen-5);		//�����ݳ���
											}
											else  bRS485DataError=1; //�ܳ��ȳ���
										}
										else  bRS485DataError=1; //��Ϣ���Ͳ�һ��											
									}
									else  bRS485DataError=1; //�м��ж�֡														
								}

								if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
								{//������־
									if(bRS485DataError ==0)
									{//δ������								
										Write_Cont(DIET_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiDietDataLen)),2);	//�������ݳ���								    	
										bUsartRecedDietData =1;
										bBus0AckOk =1;
									}
									else 
									{
										bBus0AckFail=1;	
										goto usart0RecFailRet;	//����������ǿ���˳�
									}									
								}
								break;

							case PRESCRIPTION_TYPE:
								if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
								{//��һ֡����
									Frame_No =1;
									Frame_Type = byUsart0RecBuf[DATA_POSITION1];
									/*WREN();
									Block_Erase_32K(PRESCRIPTION_DATA_START_ADDR);
									Wait_Busy();*/
									Write_Cont(PRESCRIPTION_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);	//ÿ�������ݿɴ�32K
									stLocalControl.uiPrescriptionDataLen = uiDataLen-5;
								}
								else 
								{	
									if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
									{
										if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
										{//��Ϣ����һ��	
											Frame_No++;
											//�����в���д
											if(stLocalControl.uiPrescriptionDataLen< LENGTH_32K)
											{
												Write_Cont(PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiPrescriptionDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
												stLocalControl.uiPrescriptionDataLen  += (uiDataLen-5);		//�����ݳ���
											}
											else bRS485DataError=1; //�ܳ��ȳ���
										}
										else   bRS485DataError=1; //��Ϣ���Ͳ�һ��									
									}
									else bRS485DataError=1; //�м��ж�֡
								}

								if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
								{//������־
									if(bRS485DataError ==0)
									{//δ������								
										Write_Cont(PRESCRIPTION_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),2);	//�������ݳ���									
										bUsartRecedPrescriptionData =1;	
										bBus0AckOk =1;
									}
									else 
									{
										bBus0AckFail=1;	
										goto usart0RecFailRet;	//����������ǿ���˳�
									}									
								}
								break;
							case PROMPT_INFO_TYPE:
								if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
								{//��һ֡����
									Frame_No =1;
									Frame_Type = byUsart0RecBuf[DATA_POSITION1];
								/*	WREN();
									Block_Erase_32K(PROMPT_DATA_START_ADDR);
									Wait_Busy();*/
									Write_Cont(PROMPT_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);      	//ÿ�������ݿɴ�32K																						
									stLocalControl.uiPromptDataLen = uiDataLen-5;
								}
								else 
								{	
									if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
									{	
										if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
										{//��Ϣ����һ��	
											Frame_No++;
											//�����в���д
											if(stLocalControl.uiPromptDataLen < LENGTH_32K)
											{
												Write_Cont(PROMPT_DATA_START_ADDR+stLocalControl.uiPromptDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
												stLocalControl.uiPromptDataLen  += (uiDataLen-5);		//�����ݳ���
											}
											else bRS485DataError=1; //�ܳ��ȳ���
										}
										else  bRS485DataError=1; //��Ϣ���Ͳ�һ��										
									}
									else    bRS485DataError=1; //�м��ж�֡
								}
								
								if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
								{//������־
									if(bRS485DataError ==0)
									{//δ������								
										Write_Cont(PROMPT_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPromptDataLen)),2);	//�������ݳ���     
										bUsartRecedPromptData =1;
										bBus0AckOk =1;
									}
									else 
									{
										bBus0AckFail=1;	
										goto usart0RecFailRet;	//����������ǿ���˳�
									}									
								}
								break;
								
					 	}	
					}
					else if(pstDataFreq->byCmd == CMD_DISPLAY_FORM)
					{
						if(byUsart0RecBuf[DATA_POSITION1]==BED_CARD_TYPE)	//BED_CARD_TYPE ������������ֵĴ�ͷ����Ϣ
						{
							if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
							{//��һ֡����
								//��������������Ч���ݵ�У���
								CheckSum = *(pstDataFreq->byData + pstDataFreq->uiLen);
								for(temp =0;temp<14;temp++) 	//��ַ6�ֽ�+����1�ֽ�+����2�ֽ�+��������5�ֽ�(��14�ֽ�)
								{
									CheckSum -= byUsart0RecBuf[temp+1];		//��sndsec��ʼ
								}
								
								Frame_No =1;
								Frame_Type = byUsart0RecBuf[DATA_POSITION1];							
								Write_Cont(BED_FACE_FORM_START_ADDR_2,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
								uiBedFaceFormLen = uiDataLen-5;
							}
							
							else 
							{									
								if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
								{	
									if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
									{//��Ϣ����һ��	
										Frame_No++;
										//�����в���д	
										if(uiBedFaceFormLen <LENGTH_4K)
										{
											//��������������Ч���ݵ�У���
											CheckSum += *(pstDataFreq->byData + pstDataFreq->uiLen);
											for(temp =0;temp<14;temp++) 	//��ַ6�ֽ�+����1�ֽ�+����2�ֽ�+��������5�ֽ�(��14�ֽ�)
											{
												CheckSum -= byUsart0RecBuf[temp+1];		//��sndsec��ʼ
											}
											
											Write_Cont(BED_FACE_FORM_START_ADDR_2+uiBedFaceFormLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
											uiBedFaceFormLen += (uiDataLen-5);
										}
										else bRS485DataError=1; //�ܳ��ȳ���
									}
									else  bRS485DataError=1; //��Ϣ���Ͳ�һ��									
								}
								else  bRS485DataError=1; //�м��ж�֡
							}
							
							if(byUsart0RecBuf[DATA_POSITION4])	
							{//һ����ʽ���ݽ���,�������ݳ���
								if(bRS485DataError ==0)
								{//δ������
									if(checkFormData(uiBedFaceFormLen))
									{
										if(crcCheckData(BED_FACE_FORM_START_ADDR_2,uiBedFaceFormLen,CheckSum)==1)
										{//Ԥ����������У�����ȷ
	
											Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&uiBedFaceFormLen,2);		//д����
											SectorDataCopy(BED_FACE_FORM_START_ADDR,BED_FACE_FORM_START_ADDR_2,uiBedFaceFormLen);	//д����
											CheckSum += uiBedFaceFormLen>>8;
											CheckSum += (uiBedFaceFormLen&0x00ff);
											Write_Cont(BED_FACE_FORM_START_ADDR+uiBedFaceFormLen,&CheckSum,1);	//дУ���
											//�ٽ���У��
											if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,uiBedFaceFormLen+2))	//�����ݳ���Ҳ������ȥ	
											{
												stLocalControl.uiBedFaceFormLen = uiBedFaceFormLen; 									
												bDispBedForm=1;
											}
										}
									}
								
								}
							}
						}
					}
				}
				else
				{//����������
					if(byUsart0RecBuf[DATA_POSITION4]==1)
					{//���һ֡����
						if(temp != 0x11)
						{//���ǹ㲥��ַ
							bBus0AckFail=1;
							goto usart0RecFailRet;
						}
					}
				}
usart0RecFailRet:
				if(byUsart0RecBuf[DATA_POSITION4]==1)
				{//���һ֡����
					bRS485DataError=0;	//���־
					Frame_No =0;
					Frame_Type= 0xff;
				}

				
				if(bBus0AckOk==1)
				{
					bBus0AckOk=0;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
					stLocalControl.stBusDealFreq.byRecSecAddr = pstDataFreq->byCmd;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				}

				if(bBus0AckFail==1)
				{
					bBus0AckFail=0;
					stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
		}
		else
		{//���ݳ��Ȳ���
			if(temp	!=0x11)
			{//���ǹ㲥��ַ
				stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				//bRS485DataError	=0;		//��־�ָ�
			}
		}		
	}
Usart0RecDealRet:	
	SCON  |= 0X10;    //��REN
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
		if(bUsart0RecFinish)									//����0�յ�һ֡����
		{ 	

//			OSQPost(byMainCmdQ,UART0_CMD);	
		}
 
		OSWait(K_TMO, 1);
		WDT_CONTR = 0x3e;	//ι��			
	}

}
