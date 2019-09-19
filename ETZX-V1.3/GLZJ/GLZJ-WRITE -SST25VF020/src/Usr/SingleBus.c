/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SingleBus.c
*�ļ�����    		:�����߳���
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
#define _IN_SINGLE_BUS_
#include "config.h"


//����0��������
uint8 bdata byBus0State0 	= 0;							//������0״̬��1
sbit  bBus0StartRec			= byBus0State0^0;				//������0��ʼ������ʼλ��־
sbit  bBus0OnRec			= byBus0State0^1;				//������0��ʼ��������λ��־
sbit  bBus0Enable			= byBus0State0^2;				//������0�����ͱ�־
sbit  bBus0ReqSend			= byBus0State0^3;				//������0�����ͱ�־
sbit  bBus0OnSendBit		= byBus0State0^4;				//������0���ڷ���һ������λ��־
sbit  bBus0OnSendFreq		= byBus0State0^5;				//������0���ڷ���һ������֡��־
sbit  bBus0SendError		= byBus0State0^6;				//������0���ʹ����־
sbit  bBus0Error			= byBus0State0^7;				//������0���ϱ�־
uint8 bdata byBus0State1 	= 0;  							//������0״̬��2
sbit  bBus0RecFinish		= byBus0State1^0;				//������0������ɱ�־
sbit  bBus0RecBit9			= byBus0State1^1;				//������0�����ֽ����ݵĵ�9λ
sbit  bBus0SendFinish		= byBus0State1^2;				//������0������ɱ�־
sbit  bBus0Disable			= byBus0State1^3;				//������0��ֹ���ͱ�־
sbit  bBus0SendBit			= byBus0State1^4;				//������0���ڷ��͵�����λ
sbit  bBus0RecBit			= byBus0State1^5; 				//������0���ڽ��յ�����λ
uint8 data byBus0RecCount = 0;								//��4λ������0���ն�ʱ�жϼ�������4λ�ǽ��յ���λ����
uint8 data byBus0RecBuf;									//����0���ջ��嵥Ԫ
uint8 data byBus0SendBuf;									//����0���ͻ��嵥Ԫ	
uint8 xdata byBus0RecData[BUS0_FREQ_SIZE];					//����0���ջ�����
uint8 xdata byBus0SendData[BUS0_FREQ_SIZE];					//����0���ͻ�����
uint8 data byBus0SendCount = 0;								//��4λ������0���Ͷ�ʱ�жϼ�������4λ�Ƿ��͵�λ����
uint8 data byBus0RecSendCount = 0;							//��4λ������0���յ����ֽڼ�������4λ������0��������ֽڼ���
uint8 data byBus0SendStopCount;								//����0ֹͣʱ�����
uint8 data byBus0RecTimeOut = 0;							//����0���ճ�ʱ����
uint8 data byBus0DisableCount = 0;							//����0��ֹ����	 
uint8 xdata byBus0TxQ[BUS0_TX_Q_ZISE];						//����0���Ͷ���							
uint8 data byBus0TxHead = 0;								//������0���Ͷ���ͷָ��
uint8 data byBus0TxTail = 0; 								//������0���Ͷ���βָ��
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE)
//���ͺͽ������Ŷ���
sbit  Bus0RecPin	= P3^3;									//������0�������Ŷ���
sbit  Bus0SendPin	= P3^4;									//������0�������Ŷ���

/**********************************************************
*��������			:SingleBusInit	
*��������        	:�����߳�ʼ��
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
void SingleBusInit(void)
{
	//tx:ǿ�������
//	P3M0 &= (~(Bin(0,0,0,1,0,0,0,0)));
//	P3M1 |= Bin(0,0,0,1,0,0,0,0);

	//P3^3,��������
	P3M0 |=   Bin(0,0,0,0,1,0,0,0);
	P3M1 &= (~Bin(0,0,0,0,1,0,0,0));

	//P3^4��©���
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);

	
	//���ߵ�ƽ����
	Bus0SendPin = 1;
	Bus0RecPin = 1;	
	//��ʱ��0��ʼ��:�Զ����ط�ʽ,��ʱ93.75us
   // TMOD |= 0x02;
//	TL0 = 0;
//	TH0 = TIMER0_COUNT;

    TMOD&= 0xf0;	 //  ;t1�������ʷ�����(����)��
    TMOD|=0X01;	//t0����ʽ1��16λ)��ʱ��
	
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	
	TF0 = 0;												//����жϱ�־
	AUXR &= 0x7f;											//��ͳ12��Ƶ�ٶ�  
   	ET0 = 1;												//����ʱ��0�ж� 
	TR0 = 1;												//������ʱ��
	//������������
	byBus0SendStopCount = 230;								//�ϵ�����0��ֹ����ʱ������
	IT1 = 1;												//�ⲿ�ж�1Ϊ�½��ش���ģʽ   			
	if(Bus0RecPin)
	{	//�����������,���ж�			
		EX1 = 1;
	}
	else
	{	//������߲�����,�����߹��ϱ�־			
		bBus0Error = 1;
	} 	
}

/**********************************************************
*��������			:Bus0RecInt	
*��������        	:�ⲿ�ж�1����,������1�����ж�
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
#pragma disable
void Bus0RecInt(void) interrupt 2
{ 
	DisableBus0RecInt();									//��ֹ�ٴ��½����ж�
	bBus0StartRec = 1;										//������ʼλ�ؼ��
	bBus0Enable = 0;										//��ֹ���߷���							
	byBus0RecCount = 0;										//����ռĴ���	
}
/**********************************************************
*��������			:Timer0Int	
*��������        	:��ʱ��0����ж�,��ʱ��ÿ93.75us�ж�һ��
					 �������������0������1�Ľ��պͷ���
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
#pragma disable
void Timer0Int(void) interrupt 1
{ 

	TR0=0;
	TH0=TIMER0_H;
	TL0=TIMER0_L;
	TR0=1;

	bBus0RecBit = Bus0RecPin;	
	/*******************************����0���մ���***********************************/
	if(bBus0StartRec)										//�ж�����������״̬�����յ���ʼλ
	{			
		byBus0RecCount += 0x10;								//���Ӷ�ʱ�жϼ�������
		if(0x50 == (byBus0RecCount & 0xf0))					//��������ʼλ���ʱ��
		{			
			bBus0StartRec = 0;
			byBus0RecCount = 0x00;							//���¿�ʼ����			
			if(bBus0RecBit)			
			{ 	//��Ч��ʼλ																		
				if((!bBus0SendError)&&bBus0OnSendFreq)
				{	//û�з��������߷��ʹ���,����һ֡�������ڷ���,ֹͣ֡���ݷ���,�÷��ʹ����־						
					byBus0State0 &= (~BUS0_SEND_CON);
					bBus0SendError = 1;
					Bus0SendPin = 1;						//�ͷ�����	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;					//���ճ���,���ý��շ��ͼ���ֵ
				EnableBus0RecInt();				
			}
			else								
			{	//��Ч��ʼλ
				bBus0OnRec = 1;								//��ʼ��������λ 											
			}
		}
	}
	else if(bBus0OnRec)
	{		
		byBus0RecCount += 0x10;								//���Ӷ�ʱ�жϼ�������	 		
		if(0xa0 == (byBus0RecCount & 0xf0))
		{
			byBus0RecCount &= 0x0f;							//�����ʱ�жϼ�������
			byBus0RecCount += 0x01;
			if(0x0a == (byBus0RecCount & 0x0f))	
			{	//�յ���10λ,����λ	  				
				bBus0OnRec = 0;								//ֹͣ���ݽ���					
				if(bBus0RecBit)
				{	//��Ч�Ľ���λ							
					if(((bit)(byBus0RecSendCount & 0xf0) == bBus0RecBit9)) 
					{	//���������
						byBus0RecTimeOut = 0;
						byBus0RecSendCount &= 0x0f;											
					}
					else 
					{	//��������ȷ					
						byBus0RecTimeOut = 230;				//������һ���ֽ����ݽ��ճ�ʱʱ��
						byBus0RecData[byBus0RecSendCount>>4] = byBus0RecBuf;
						byBus0RecSendCount += 0x10;																					
						if((byBus0RecSendCount & 0xf0) >= BUS0_FREQ_SIZE_HI)
						{																						
							byBus0RecSendCount &= 0x0f;							
							if(!((bBus0OnSendFreq == 1)&&(bBus0ReqSend == 0)))
							{	//������յ�����֡�������Լ����͵Ĳ��ñ�־							
								bBus0RecFinish = 1;																  																						
							} 																		
							byBus0RecTimeOut = 0;
							byBus0DisableCount = 10;
							bBus0Disable = 1;				//��ֹ����ʹ��							
						}	
					}				
					byBus0SendStopCount = 240;
					EnableBus0RecInt();						
				}
				else							
				{	//��Ч����λ
					bBus0Error = 1;										
					if((!bBus0SendError) && bBus0OnSendFreq)
					{	//û�з��������߷��ʹ���,����һ֡�������ڷ���,ֹͣ֡���ݷ���,�÷��ʹ����־
						byBus0State0 &= (~BUS0_SEND_CON);
						bBus0SendError = 1;							
						Bus0SendPin = 1;					//�ͷ�����
					}
					byBus0RecSendCount = 0x00;				//���ճ���,���ý��շ��ͼ���ֵ					
				}
			}
			else if(0x09 == (byBus0RecCount & 0x0f))
			{	//��9λ����
				bBus0RecBit9 = bBus0RecBit;					
			}
			else 											
			{ 	//��Ч����λ
				byBus0RecBuf >>= 1;
				if(bBus0RecBit)
				{	//Ϊ�ߵ�ƽ
					byBus0RecBuf |= 0x80;
				}	
			}
		}
	}
	/*******************************����0���ʹ���***********************************/
	if((byBus0State0 & BUS0_CAN_SEND) == BUS0_CAN_SEND)
	{	//����0�������ݷ�������,������������		
		if(bBus0RecBit)
		{	//��������,���Է���
			Bus0SendPin = 0;
			bBus0SendBit = 0;								//������ʼλ���� 			
			byBus0SendCount = 0;
			byBus0State0 &= (~BUS0_CAN_SEND);
			byBus0SendBuf = byBus0SendData[byBus0RecSendCount & 0x0f];
			bBus0OnSendBit = 1;								//ȡ�������͵����ݲ������ڷ��ͱ�־		
		}
		else
		{	//���߲�����,ֹͣ����
			byBus0State0 &= (~BUS0_SEND_CON);
			bBus0SendError = 1;			
			byBus0RecSendCount &= 0xf0;
			Bus0SendPin = 1;
			byBus0SendStopCount = 240;
		}
	}
	else if(bBus0OnSendBit)
	{ 	//������λ���ڷ���,���ȷ��͵�����ʼλ			
		if(bBus0SendBit == bBus0RecBit)   		
		{	//���͵����ݺͽ��յ�������ͬ
			byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//һλ���ݷ������,���ȷ��͵�����ʼλ
				byBus0SendCount &= 0x0f;
				byBus0SendCount += 0x01;				
				if(0x09 == (byBus0SendCount & 0x0f))
				{	//���͵���9λ��
					bBus0SendBit = !(bit)(byBus0RecSendCount & 0x0f);
					Bus0SendPin = bBus0SendBit;						
				}
				else if(0x0a == (byBus0SendCount & 0x0f))
				{	//���͵�����λ��
					bBus0SendBit = 1;
					Bus0SendPin = 1;	
				}
				else if(0x0b == (byBus0SendCount & 0x0f))
				{	//�Ѿ����������λ��
					bBus0OnSendBit = 0;						
					byBus0RecSendCount += 0x01;									
					if((byBus0RecSendCount & 0x0f) >= BUS0_FREQ_SIZE)
					{	//������һ֡����												
						byBus0RecSendCount &= 0xf0;			//���½�������֡�ķ��ͽ׶�
						byBus0State0 &= (~BUS0_SEND_CON);
						byBus0SendStopCount = 240;
						byBus0State1 |= BUS0_SEND_FINISH;								
						byBus0DisableCount = 10; 						
					}
					else
					{  						
						byBus0SendStopCount = 10;
						bBus0ReqSend = 1;
					}
					EnableBus0RecInt();						//�ٴ�ʹ�ܽ����ж�
				}
				else
				{
					if(byBus0SendBuf & 0x01)
					{	//���͸ߵ�ƽ
						bBus0SendBit = 1;
						Bus0SendPin = 1;					
					}
					else
					{	//���͵͵�ƽ
						bBus0SendBit = 0;
						Bus0SendPin = 0;
					}
					byBus0SendBuf >>= 1;					//��������λ��λ����
				}
			}
		}
		else
		{	//����ͬ,����ʧ��								
			byBus0State0 &= ~BUS0_SEND_CON;
			byBus0RecSendCount &= 0xf0;
			bBus0SendError = 1;					
			Bus0SendPin = 1; 
			byBus0SendStopCount = 240;
		}
	} 	
	/*******************************����0���ƴ���***********************************/
	if(0 == (byBus0State0 & BUS0_ON_REC))
	{	
		if(byBus0SendStopCount != 0)
		{
			if((--byBus0SendStopCount) == 0)
			{				
				bBus0Enable = 1;								
			}
		}		
		if(bBus0Error)
		{								
			bBus0Enable = 0;			
			if(bBus0RecBit)
			{				
				bBus0Error = 0;
				EnableBus0RecInt();
				byBus0SendStopCount = 240;
			}
		}
	} 	
	/*******************************����0��ʱ����***********************************/
	if(byBus0RecTimeOut != 0)
	{
		if(--byBus0RecTimeOut == 0)							
		{	//���ճ�ʱ��
			byBus0RecSendCount &= 0x0f;			
		}
	}
	if(byBus0DisableCount != 0)
	{
		if(--byBus0DisableCount == 0)						
		{	//��ֹ��ʱ��		
			bBus0Disable = 0;
		}
	}	
	/***********����0�Զ����͹���**********/	 
	if((byBus0State0 & BUS0_ON_WORK) == 0x00)				
	{	//����0û�й���				
		if(bBus0SendError)						
		{	//�����˷��ʹ���,�Զ��ط�								
			bBus0SendError = 0;				
			byBus0State0 |= BUS0_REQ_SEND;		
		}
		else									
		{	//����0�޷��ʹ���		
			if(!(bBus0SendFinish|bBus0Disable))
			{	//����0û�н�ֹʹ��,�ҷ��ͽ��������Ѿ����
				if(Bus0TxBuffLen() >= BUS0_FREQ_SIZE)
				{ 	//��һ֡�����������ڷ��Ͷ����� 																		
					byBus0SendData[0] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);
					byBus0SendData[1] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);
					byBus0SendData[2] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);
					byBus0SendData[3] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);
					byBus0SendData[4] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);
					byBus0SendData[5] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);
					byBus0SendData[6] = byBus0TxQ[byBus0TxHead];
					IncBus0TxPtr(byBus0TxHead);	
					byBus0State0 |= BUS0_REQ_SEND;						
				}
				else
				{	//û��һ֡�����������ڷ��Ͷ�������
					byBus0TxHead = byBus0TxTail = 0;
				}								
			}
		}
	}	
} 
/**********************************************************
*��������			:Bus0OutputData	
*��������        	:������0�����������ݷ��뻺����
*�������   		:pbyData:�����͵�����ָ��
*����ֵ				:TRUE:���ͳɹ�,FALSE:������,����ʧ�� 	
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
uint8 Bus0OutputData(uint8* pbyData)
{
	uint8 byTemp = BUS0_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}	
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	OS_EXIT_CRITICAL();	
	return(TRUE);	
}
/**********************************************************
*��������			:AddrCompare	
*��������        	:�Ƚϵ�ַ(���㲥��ַ)
*�������   		:pstBusFreq:���Ƚϵ�����ָ֡��
*����ֵ				:1:���͵�����������,0:���Ƿ��͵�����������	
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
bit AddrCompare(pSTBusFreq pstBusFreq)
{
/*	if((pstBusFreq->byRecSecAddr != 0xff) && (pstBusFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
	{	
		return(0);
	} 
	if((pstBusFreq->byRecRoomAddr != 0xff) && (pstBusFreq->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr))
	{	
		return(0);
	}
	if((pstBusFreq->byRecBedAddr != 0xff) && (pstBusFreq->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
	{	
		return(0);
	}
	return(1); 
*/
	pstBusFreq=pstBusFreq;
	return(1);
}
/**********************************************************
*��������			:MakeCH0TimerOut	
*��������        	:����ͨ��0��ʱ����
*�������   		:byTimerOut:��ʱʱ��,byTimerOutCount:��ʱ����
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/  
void MakeCH0TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH0TimerOut.byTimerOutSet = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOut = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOutCount = byTimerOutCount;	
}

/**********************************************************
*��������			:SaveParameter	
*��������        	:�洢���ò���,�洢����ǰ�Ȳ�����������,
					 Ȼ�����в�������д������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:EnableIAP,IapErase,IapWriteByte,DisableIAP
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SaveParameter(void)
{  
	stLocalControl.uiShortType = IAP_ADDR;	
	stLocalControl.pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 
	while(stLocalControl.byChar1--)
	{
		IapWriteByte(stLocalControl.uiShortType++, *stLocalControl.pbyData++);
	}
	DisableIAP();
}

/**********************************************************
*��������			:ReadParameter	
*��������        	:��Flash�е��������ݶ��뵽�ڴ���
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:EnableIAP,IapReadByte,DisableIAP
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void ReadParameter(void)
{

	stLocalControl.uiShortType = IAP_ADDR;	
	stLocalControl.pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	
	EnableIAP(IAP_READ); 
	while(stLocalControl.byChar1--)
	{		
		*stLocalControl.pbyData++ = IapReadByte(stLocalControl.uiShortType++);		
	}
	DisableIAP();
	
}
/**********************************************************
*��������			:InitParameter	
*��������        	:��ʼ��Flash�е���������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:EnableIAP,IapReadByte,DisableIAP
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void InitParameter(void)
{
	EnableIAP(IAP_READ);
	if(IapReadByte(IAP_ADDR) == INIT_FLAG)
	{//�����Ѿ���ʼ�������,��ȡ���ݲ���
		ReadParameter();

	}
	else
	{	//û�г�ʼ��,��ʼ������
		stLocalControl.stEepromCfgData.byInitFlag = INIT_FLAG;		
		SaveParameter();
	}
}



/**********************************************************
*��������			:Bus0RecDeal	
*��������        	:������0�յ�һ֡���ݴ�����,�ú�������
					 ȡ���յ�������,���ÿ������ִ�ж�Ӧ�Ŀ�
					 �ƶ���
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus0RecDeal(void)
{			
	//ȡ���յ�������֡ 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;	
	OS_EXIT_CRITICAL();	
	
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//ȷ�ϵǼ�����
			break;	
	}		
}
/**********************************************************
*��������			:Bus0SendDeal	
*��������        	:������0������һ֡���ݴ�����,�ú�������
					 ȡ���յ�������,���ÿ������ִ�ж�Ӧ�Ŀ�
					 �ƶ���
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus0SendDeal(void)
{
	//ȡ��������ɵ�����֡
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;										
	OS_EXIT_CRITICAL();	

//	Send_Data(byBus0SendData,7); 	//�����յ������ݷ���PC��
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			
			break;
	}	 	
}
/**********************************************************
*��������			:Bus0Manage	
*��������        	:������0�����߳�
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:byMainCmdQ
*����ģ��  			:OSQPost
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus0Manage(void)
{ 	
	while(TRUE)
	{		
		if(bBus0RecFinish)									//����0�յ�����
		{ 			
			OSQPost(byMainCmdQ, BUS0_REC);				
		}
		if(bBus0SendFinish)									//����0����������֡
		{
			OSQPost(byMainCmdQ, BUS0_SND);			
		} 
		OSWait(K_TMO, 1);			 		
	}
}
/**********************************************************
*��������			:TimerOutDeal	
*��������        	:��ʱ������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:Bus0OutputData					 
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void TimerOutDeal(void)
{  	
	if(stLocalControl.stCH0TimerOut.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stLocalControl.stCH0TimerOut.byTimerOut--;
		if(stLocalControl.stCH0TimerOut.byTimerOut == 0)
		{	//��ʱһ�ε���
			if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
			{ 	//���г�ʱ���
					
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH0TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
				{ 	//���г�ʱ���

				} 			
			}
		}
	}
}
/**********************************************************
*��������			:TimerOutManager	
*��������        	:��ʱ�����߳�
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl,byMainCmdQ
*����ģ��  			:OSQPost				
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/		
void TimerOutManager(void) 	 
{  	
	while(1)
	{ 	
		OSWait(K_TMO, 4);									//��ʱ20ms 								
		if(stLocalControl.stCH0TimerOut.byTimerOut)
		{	
			OSQPost(byMainCmdQ, TIMER_OUT);				
		}
		else
		{		
			OSQPost(byMainCmdQ, TIMER_OUT);	
		}			
	}
} 
	  
