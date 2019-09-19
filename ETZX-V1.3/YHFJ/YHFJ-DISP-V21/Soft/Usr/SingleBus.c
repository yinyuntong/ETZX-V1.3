/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SingleBus.c
*�ļ�����    		:�����߳���
*������     		:����ͬ
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
uint8 xdata byBus0RecCount = 0;								//��4λ������0���ն�ʱ�жϼ�������4λ�ǽ��յ���λ����
uint8 xdata byBus0RecBuf;									//����0���ջ��嵥Ԫ
uint8 xdata byBus0SendBuf;									//����0���ͻ��嵥Ԫ	
uint8 data byBus0RecData[BUS0_FREQ_SIZE];					//����0���ջ�����
uint8 data byBus0SendData[BUS0_FREQ_SIZE];					//����0���ͻ�����
uint8 xdata byBus0SendCount = 0;								//��4λ������0���Ͷ�ʱ�жϼ�������4λ�Ƿ��͵�λ����
uint8 xdata byBus0RecSendCount = 0;							//��4λ������0���յ����ֽڼ�������4λ������0��������ֽڼ���
uint8 xdata byBus0SendStopCount;								//����0ֹͣʱ�����
uint8 xdata byBus0RecTimeOut = 0;							//����0���ճ�ʱ����
uint8 xdata byBus0DisableCount = 0;							//����0��ֹ����	 
uint8 xdata byBus0TxQ[BUS0_TX_Q_ZISE];						//����0���Ͷ���							
uint8 xdata byBus0TxHead = 0;								//������0���Ͷ���ͷָ��
uint8 xdata byBus0TxTail = 0; 								//������0���Ͷ���βָ��
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 


sbit  Bus0RecPin	= P3^3;									//������0�������Ŷ���
sbit  Bus0SendPin	= P3^4;									//������0�������Ŷ���

uint8 xdata VolumnValue=0;

bit bSecondDisState;										//������˸��־
bit bSignflag=0;											//�������ű�־

//STBusFreq xdata stBus0RxQ[MAX_FREQ_NUMBER];						////����0���ն���
//uint8 xdata byBus0FreqNum=0;							//֡��
uint8 xdata byBus0RxQ[BUS0_RX_Q_ZISE];						//����0���ն���
uint8 xdata byBus0RxHead = 0;								//������0���ն���ͷָ��
uint8 xdata byBus0RxTail = 0; 								//������0���ն���βָ��

uint8 code byNumPW[7]={8,8,6,6,2,2,4};						//�������


uint8 Bus0RecData(uint8* pbyData);

/**********************************************************
*��������			:SingleBusInit	
*��������        	:�����߳�ʼ��
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SingleBusInit(void)
{
	//STC12C54/56ϵ�УM0=0,M1=0:׼˫��M0=0,M1=1:���죬M0=1,M1=0:���裬M0=1,M1=1:��©
	//����0���ͽ�����Ϊ������������ս�����Ϊ��������:


	//STC12C52/5Aϵ��:M0=0,M1=0:׼˫��M0=0,M1=1:���裬M0=1,M1=0:���죬M0=1,M1=1:��©
	//����0���ս�����Ϊ��������:���ͽ�����Ϊ���������
	//P3^3,��������
	P3M0 &= (~Bin(0,0,0,0,1,0,0,0));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);

	//P3^4�������
//	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
//	P3M1 &= (~Bin(0,0,0,1,0,0,0,0));

	//P3^4��©���
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);


	//���ߵ�ƽ����
	Bus0SendPin = 1;
	Bus0RecPin = 1;	 
	
	

 	TMOD &= 0xf0;   //  ;t1�������ʷ�����(����)��
	TMOD |= 0X01;	//t0����ʽ1��16λ)��ʱ��

	TL0 = TIMER0_L;
	TH0 = TIMER0_H;

/*	TMOD |= 0X02;	//T0����ʽ2��8λ)��ʱ��
	TL0 = TIMER0_COUNT;
	TH0 = TIMER0_COUNT;*/
	
	TF0 = 0;												//����жϱ�־
	AUXR &= (~T0x12);										//��ͳ12��Ƶ�ٶ�  
   	ET0 = 1;												//����ʱ��0�ж� 
	TR0 = 1;												//������ʱ��
	//������������
	byBus0SendStopCount = 240;								//�ϵ�����0��ֹ����ʱ������		
	IE1 = 0;												//����жϱ�־
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
*��������        	:�ⲿ�ж�0����,������0�����ж�
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
#pragma disable
void Bus0RecInt(void) interrupt X1_INTNO
{ 	
	DisableBus0RecInt();									//��ֹ�ٴ��½����ж�
	bBus0StartRec = 1;										//������ʼλ�ؼ��
	bBus0Enable = 0;										//��ֹ���߷���							
	byBus0RecCount = 0;										//����ռĴ��� 		
}
/**********************************************************
*��������			:Timer0Int	
*��������        	:��ʱ��0����ж�,��ʱ��ÿ500us�ж�һ��
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
void Timer0Int(void) interrupt T0_INTNO
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
								if(TRUE==Bus0RecData(byBus0RecData))
								{//���浽������	
									//bBus0RecFinish = 1;
								}
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
*������	      		:����ͬ
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
*��������			:Bus0RecData	
*��������        	:������0���������Ͻ��յ����ݷ��뻺����
*�������   		:pbyData:�����յ�����ָ��
*����ֵ				:TRUE:���ճɹ�,FALSE:������,����ʧ�� 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
uint8 Bus0RecData(uint8* pbyData)
{
	uint8 byTemp = BUS0_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus0RxBuffLen() >= (BUS0_RX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}	
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus0RxQ[byBus0RxTail] = *pbyData++;
		IncBus0RxPtr(byBus0RxTail);
	}
	OS_EXIT_CRITICAL(); 
	return(TRUE);

}

/**********************************************************
*��������			:BcdToHex	
*��������        	:BCDת����ʮ������
*�������   		:byData:��ת����BCD������
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
uint8 BcdToHex(uint8 byData)
{
	uint8 xdata byRet;

	byRet = byData >> 4;
	byRet *= 10;
	byData &= 0x0f;
	byRet += byData;
	return(byRet);
}

/**********************************************************
*��������			:AddrCompare	
*��������        	:�Ƚϵ�ַ(���㲥��ַ)
*�������   		:pstBusFreq:���Ƚϵ�����ָ֡��
*����ֵ				:1:���͵�����������,0:���Ƿ��͵�����������	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
bit AddrCompare(pSTBusFreq pstBusFreq)
{
	if((pstBusFreq->byRecSecAddr != 0xff) && (pstBusFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
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

/*	if((pstBusFreq->byRecSecAddr  == 0xff)&&
	   (pstBusFreq->byRecRoomAddr == 0xff)&&
	   (pstBusFreq->byRecBedAddr  == 0xff))
		return(1);
	else if((pstBusFreq->byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)&&
		   (pstBusFreq->byRecRoomAddr== stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
		   (pstBusFreq->byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
		   return(1);
	else return(0);
*/
}
/**********************************************************
*��������			:DirAddrCompare	
*��������        	:���ԱȽϵ�ַ
*�������   		:pstBusFreq:���Ƚϵ�����ָ֡��
*����ֵ				:1:���͵�����������,0:���Ƿ��͵�����������	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
bit DirAddrCompare(pSTBusFreq pstBusFreq)
{
	if(pstBusFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr)
	{	
		return(0);
	} 
	if(pstBusFreq->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr)
	{	
		return(0);
	}
	if(pstBusFreq->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr)
	{	
		return(0);
	}
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
*������	      		:����ͬ
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
*��������			:AddCallNod	
*��������        	:���Ӻ��нڵ�
*�������   		:pstBusFreq:�����������������ָ��
*����ֵ				:SAVE_FAIL:ʧ��,SAVE_NORMAL:�ɹ�,
					 SAVE_PRIO_HI:�ɹ�,���и����ȼ�				     
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
uint8 AddCallNod(pSTBusFreq pstBusFreq)
{
	uint8 xdata i;
	 WDT_CONTR = 0x3d; 
	if(stLocalControl.byCallCount ==0)
	{//���ж��п�
		memcpy(stLocalControl.byCallArray[0],pstBusFreq,4);
		stLocalControl.byCallCount++;
		return true;
	}
	else 
	{//���ж��зǿ�,�Ȳ����Ƿ��иýڵ���
		for(i=0;i<stLocalControl.byCallCount;i++)
		{
			if((stLocalControl.byCallArray[i][0]==pstBusFreq->bySndSecAddr)&&
			   (stLocalControl.byCallArray[i][1]==pstBusFreq->bySndRoomAddr)&&
			   (stLocalControl.byCallArray[i][2]==pstBusFreq->bySndBedAddr)&&
			   (stLocalControl.byCallArray[i][3]==pstBusFreq->byCmd))
			   break;	//���иýڵ�,�˳�
		}
		if(i>=stLocalControl.byCallCount)
		{//û�иýڵ�,
			if(stLocalControl.byCallCount<MAX_CALL_COUNT)
			{
				//���ýڵ���뵽���ж�����
				memcpy(stLocalControl.byCallArray[stLocalControl.byCallCount],pstBusFreq,4);
				stLocalControl.byCallCount++;
				return true;
			}
			else return false;
			
		}
		else return false;
	}
	
}
/**********************************************************
*��������			:RemoveCallNod	
*��������        	:ɾ�����нڵ�
*�������   		:pstBusFreq:��ɾ���Ľڵ�����ָ��
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2009-4-6
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void RemoveCallNod(pSTBusFreq pstBusFreq)
{

	uint8 xdata i;
	 WDT_CONTR = 0x3d; 
	if(stLocalControl.byCallCount ==0)
	{//���ж��п�
		return;
	}
	else 
	{//���ж��зǿ�,�Ȳ����Ƿ��иýڵ�
		for(i=0;i<stLocalControl.byCallCount;i++)
		{
			if((stLocalControl.byCallArray[i][0]==pstBusFreq->byRecSecAddr)&&
			   (stLocalControl.byCallArray[i][1]==pstBusFreq->byRecRoomAddr)&&
			   (stLocalControl.byCallArray[i][2]==pstBusFreq->byRecBedAddr)&&
			   (stLocalControl.byCallArray[i][3]==pstBusFreq->byCmd))
			   break;	//���иýڵ�,�˳�ѭ��
		}
		if(i>=stLocalControl.byCallCount)
		{//û�иýڵ�,
			return;
		}
		else 
		{
			stLocalControl.byUpdataRecNum=i;
			stLocalControl.byCallCount--;
			if(stLocalControl.byUpdataRecNum != stLocalControl.byCallCount)
			{//���µļ�¼�������һ����¼
				memcpy(stLocalControl.byCallArray[i],stLocalControl.byCallArray[stLocalControl.byCallCount],4);
			}
			memset(stLocalControl.byCallArray[stLocalControl.byCallCount],0x00,4);
		}
	}	
}



/**********************************************************
*��������			:SysReset	
*��������        	:ϵͳ��λ,�ú���������ͨ��0��λ,��ͨ��0
					 �ָ�������״̬
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SysReset(void)
{	

}

uint8 ParaCrcCheck(uint16 addr)
{
	uint8 crcResult=0;
	uint8 crcData =0;
	uint8 i;
	uint8 paraData;
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		paraData = IapReadByte(addr);
		crcResult +=paraData; /*IapReadByte(addr)*/;
		addr++;
	}
	crcData = IapReadByte(addr);
	DisableIAP();
	
	if(crcData == crcResult) return 1;
	else return 0;
}



uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}
	DisableIAP();
	return crcResult;
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
void SaveParameter(uint16 addr)
{  
	uint8 i;
	uint16 addr_temp=addr;
	uint8 *pbyData;
	uint8 readtimes;
	uint8 writetimes;

	stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
	WDT_CONTR = 0x3d;	//ι��
	writetimes =3;
	do
	{
		EnableIAP(IAP_ERASE);
		IapErase(addr); 

		
		EnableIAP(IAP_WRITE); 
		addr = addr_temp;
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}

		//��ַ����ǰһ������
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}
		

		EnableIAP(IAP_READ);
		readtimes=3;
		do
		{
			WDT_CONTR = 0x3d;	//ι��
			pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<(sizeof(STEepromCfgData))*2;)
			{
				if(*pbyData != IapReadByte(addr)) break;
				
				pbyData++;
				addr++;
				i++;
				if(i== sizeof(STEepromCfgData)) pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			}	

			if(i>= (sizeof(STEepromCfgData)*2))
			{//д��ȥ���������������������ͬ
				break;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			break;	//��ͬ
		}
	}while(--writetimes);

	
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
void ReadParameter(uint16 addr)
{
	uint8 i=0;
	uint8 *pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	
	EnableIAP(IAP_READ); 
	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		*pbyData = IapReadByte(addr);
		pbyData++;
		addr++;
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
	uint8 flagData;
	uint8 readtimes=3;
CheckIapSec0:	

	do
	{

		if(ParaCrcCheck(IAP0_ADDR) ==1)  break;
			

	}while(--readtimes);


	if(readtimes)
	{//��һ��У�����ȷ

		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == INIT_FLAG)
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���
			ReadParameter(IAP0_ADDR);
			return;
		}
		else	goto CheckIapSec1;	
	}

	else
	{
CheckIapSec1:	
		readtimes=3;	
		do
		{
			if(ParaCrcCheck(IAP0_ADDR+sizeof(STEepromCfgData)) ==1)  break;
				
		}while(--readtimes);
	
	
		if(readtimes)
		{//�ڶ���У�����ȷ
			EnableIAP(IAP_READ);
			flagData = IapReadByte(IAP0_ADDR+sizeof(STEepromCfgData));
			DisableIAP();
			
			if(flagData == INIT_FLAG)
			{//�����Ѿ���ʼ�������,��ȡ���ݲ���
				ReadParameter(IAP0_ADDR+sizeof(STEepromCfgData));
				SaveParameter(IAP0_ADDR);
				return;
			}
			else	goto UseInitValue;	
		}
		else
		{//��һ��͵ڶ���洢��3�ζ�ȡ��ʧ�ܻ���δ��ʼ����Ӧ�ó�ʼֵ���������浽�ڲ�EEPROM��ȥ
UseInitValue:
			stLocalControl.stEepromCfgData.byInitFlag = INIT_FLAG;
			stLocalControl.stEepromCfgData.bySelfSecAddr = 0x01;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = YHFJ_ROOM_ADDR;
			stLocalControl.stEepromCfgData.bySelfBedAddr = 0x01;
			stLocalControl.stEepromCfgData.bySerialNum1= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum2= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum3= 0xff;
			stLocalControl.stEepromCfgData.byVersionHi = 0x03;
			stLocalControl.stEepromCfgData.byVersionLo= 0x00;
			stLocalControl.stEepromCfgData.byXCoorPara = 0x64;
			stLocalControl.stEepromCfgData.byYCoorPara = 0x53;
			
			stLocalControl.stEepromCfgData.byXTopLeftADValue = 0x0c;
			stLocalControl.stEepromCfgData.byYTopLeftADValue = 0x16;
			
			stLocalControl.stEepromCfgData.uiRs485Brt = 57600;	//���ϲ�����57600
			stLocalControl.stEepromCfgData.byMaxPlayVolVal=8;

			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
		}	
	}
	DisableIAP();	
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
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus0RecDeal(void)
{

	uint16 xdata x,y;
	
/*	//ȡ���յ�������֡
	uint8 xdata i;
	OS_ENTER_CRITICAL();
//	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
//	bBus0RecFinish = 0;
	memcpy(&(stLocalControl.stBusDealFreq), stBus0RxQ, sizeof(STBusFreq));
	byBus0FreqNum--;
	for(i=0;i<byBus0FreqNum;i++)
	{
		stBus0RxQ[i] = stBus0RxQ[i+1];
	}

	OS_EXIT_CRITICAL();	
*/

//���½���������
	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);

/////////////////////////////////////////////////////////////////////////////////////



	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//����ǵǼ�״̬,�յ�������ǵǼ�ȷ������,��������
		return;
	}	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//ȷ�ϵǼ�����				
			if(bLanding)
			{					
				bLanding =0;
				MakeCH0TimerOut(0, 0);
				stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
				SaveParameter(IAP0_ADDR);								
			}			
			break;
		case CMD_QUEST:										//��ѯ����
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_QUEST_ANSWER; 
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;

		case CMD_DATE_SEND:									//�յ�Уʱ����
			stLocalControl.stTime.bySec = stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f;
			stLocalControl.stTime.byMin = stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f;
			stLocalControl.stTime.byHour = stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f;
			stLocalControl.stTime.byDay = stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f;
			stLocalControl.stTime.byMonth = stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f;
			stLocalControl.stTime.byYear = stLocalControl.stBusDealFreq.byRecBedAddr;


			DisplayDigit(128,0,stLocalControl.stTime.byYear>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(144,0,stLocalControl.stTime.byYear&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);
			
			DisplayDigit(176,0,stLocalControl.stTime.byMonth>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(192,0,stLocalControl.stTime.byMonth&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);	
						
			DisplayDigit(224,0,stLocalControl.stTime.byDay>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(240,0,stLocalControl.stTime.byDay&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);

			DisplayDigit(304,0,stLocalControl.stTime.byHour>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(320,0,stLocalControl.stTime.byHour&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0);
			
			DisplayDigit(352,0,stLocalControl.stTime.byMin>>4,WHITE,BLUE,3,DIS_FRAM_MEM0);
			DisplayDigit(368,0,stLocalControl.stTime.byMin&0x0f,WHITE,BLUE,3,DIS_FRAM_MEM0); 

			
			if(stLocalControl.byDisplayFace==SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);				
			break;
			
		case CMD_CALL_LISTEN:								//��������
			ShowTalkFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr,DIS_FRAM_MEM1);
			stLocalControl.byKeyInTimes =0;
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{	//���������Ǳ������͵�
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 1,DIS_FRAM_MEM1);
			}
			else
			{	//���������������ֻ����͵�
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 0,DIS_FRAM_MEM1);
			}
			break;
			
/*		case CMD_INFUSION_CALL:								//��Һ��������
		case CMD_SERVICE_CALL:								//�����������
		case CMD_EMERGENCY_CALL:							//������������
		case CMD_HELP_CALL:									//��Ԯ��������
			if(AddCallNod(&(stLocalControl.stBusDealFreq)))
			{//�ɹ�
				ShowCallFace(0x00);
			}
			break;	
*/

		case CMD_INFUSION_ENTER: 							//��Һ��������
		case CMD_SERVICE_ENTER:								//�����������
		case CMD_EMERGENCY_ENTER:							//������������
		case CMD_HELP_ENTER: 								//��Ԯ��������
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stBusDealFreq.byRecRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stBusDealFreq.byRecBedAddr;
			stLocalControl.stBusDealFreq.byCmd -= 0x1b; 		//��������ԭ�ɶ�Ӧ�ĺ�������
			AddCallNod(&(stLocalControl.stBusDealFreq));
			ShowCallFace1(DIS_FRAM_MEM2);
			break;	
			
		case CMD_INFO_INDICATION:							//����ʾ������Ϣ
			if(stLocalControl.byDisplayFace != INFO_INDICATION_FACE)
			{//֮ǰ��ʾ���治����Ϣָʾ����
				if(stLocalControl.byCallCount)
				{
					ShowCallFace0(DIS_FRAM_MEM2);
				}
			}
			break;
			
		case CMD_STOP_INDICATION:	 						//���رպ�����Ϣ��ʾ 				
			//ShowSecInfo();
			DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
			if(stLocalControl.byCallCount==0)
			{
				Set_VisualPage(DIS_FRAM_MEM0);
				stLocalControl.byDisplayFace = SEC_INFO_FACE;
			}
			break;
		case CMD_INFUSION_CLEAR:							//�����Һ��������
		case CMD_SERVICE_CLEAR: 							//��������������
		case CMD_HELP_CLEAR:								//�����Ԯ��������
		case CMD_EMERGENCY_CLEAR:							//���������������
			stLocalControl.stBusDealFreq.byCmd -= 0x0a; 		//��������ԭ�ɶ�Ӧ�ĺ�������		
			RemoveCallNod(&(stLocalControl.stBusDealFreq));
//			if(stLocalControl.stCallPointer.pstHead)
			if(stLocalControl.byCallCount)
			{//���в�Ϊ��
				ShowCallFace2(DIS_FRAM_MEM2);
			}
			else
			{
				Set_VisualPage(DIS_FRAM_MEM0);
				stLocalControl.byDisplayFace=SEC_INFO_FACE;			
			}
			break;

		case CMD_INFUSION_ANSWER:							//������Һ��������
		case CMD_SERVICE_ANSWER:							//��������������
		case CMD_EMERGENCY_ANSWER:							//���������������
		case CMD_HELP_ANSWER:								//������Ԯ��������
			stLocalControl.stBusDealFreq.byCmd -= 0x06; 		//��������ԭ�ɶ�Ӧ�ĺ�������		
			RemoveCallNod(&(stLocalControl.stBusDealFreq));
			if((stLocalControl.stBusDealFreq.bySndSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr)||
			   (stLocalControl.stBusDealFreq.bySndRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr)||
			   (stLocalControl.stBusDealFreq.bySndBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
			{//���Ǳ�ҽ���ֻ������
				if(stLocalControl.byDisplayFace == INFO_INDICATION_FACE)
				{
					if(stLocalControl.byCallCount)
					{//���в�Ϊ��
						ShowCallFace2(DIS_FRAM_MEM2);
					}
					else
					{
						Set_VisualPage(DIS_FRAM_MEM0);
						stLocalControl.byDisplayFace=SEC_INFO_FACE;			
					}
					break;				
				}
			}
//			ShowTalkFace(stLocalControl.stBusDealFreq.byRecRoomAddr,stLocalControl.stBusDealFreq.byRecBedAddr,DIS_FRAM_MEM1);			 
			break;			

		

		case CMD_CLEAR_LCD:									//���Һ������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
			Lcd_Clear(BLUE,DIS_FRAM_MEM1);
			}
			break;

		case CMD_POWER_ON:	  								//��������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
				OS_ENTER_CRITICAL();						
				//�ر������ж�
				CCAPM0 = 0;
				ET0 = 0;
				TR0 = 0;
				EX1 = 0;
				ISP_CONTR = 0x20; 			
			}
			break;			

			  
		case CMD_CHANNEL_CLOSE:
			  //ShowSecInfo();
			  DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
			  DisplayBlock(16,240,479,271,BLUE,DIS_FRAM_MEM1);	//��ͨ�������������
			  Set_VisualPage(DIS_FRAM_MEM0);
			  stLocalControl.byDisplayFace = SEC_INFO_FACE;
			  stLocalControl.byKeyInTimes =0;
			  break;	

		case CMD_KEY_DOWN:		
			switch(stLocalControl.byDisplayFace)
			{
				case MAIN_MENU_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
				 	{
						case KEY_1:	//ʱ������
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_YEAR;
							stLocalControl.uiCursorX = 244;		//(480-24*11)/2+5.5*24+4=244
							stLocalControl.uiCursorY = 51;		//(272-32*5-10)/2=51
							ShowTimeSet(DIS_FRAM_MEM1);	//��1ҳ��ʾ
							break;							
				 		case KEY_2:	//�ֻ��������
				 			ShowNumberPWInputFace(DIS_FRAM_MEM1);
							//ShowNumberSetFace(DIS_FRAM_MEM1);
							break;							
						case KEY_3:	
							stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_TIMER_GET;
							Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							break;
				 		case KEY_4:
							stLocalControl.stBusDealFreq.byCmd = CMD_BL_TIMER_GET;
							Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));							
							break;
						case KEY_5:
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_WARD_SEC;
							stLocalControl.uiCursorX = 72;		
							stLocalControl.uiCursorY = 72;	
							stLocalControl.byKeyInTimes = 0;
							ShowSecInfoSet(DIS_FRAM_MEM1);	//������Ϣ����
							break;

						case KEY_6:
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_FIRST_OPER;
							stLocalControl.uiCursorX = 364;
							stLocalControl.uiCursorY = 104;
							stLocalControl.byKeyInTimes = 0;
							stLocalControl.byOperType = 0;
							bSignflag = 0;
							ShowCalculateFace(DIS_FRAM_MEM1);	//��ʾ�������
							break;	
							

				 		case KEY_7:	
							//stLocalControl.stBusDealFreq.byCmd = CMD_MAX_VOL_GET;
							//Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
							
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_PLAY_VOL_MAX; 		
							stLocalControl.uiCursorX = 272;
							stLocalControl.uiCursorY = 60;
							ShowMaxVolSetFace(DIS_FRAM_MEM1);	//��ʾ����������ý���
							break;
							
						case KEY_9:	
						case KEY_0:							
							break;
						case KEY_CLEAR:
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
							memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
							stLocalControl.stBusDealFreq.byCmd = CMD_MAIN_MENU_ESC;
							Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							stLocalControl.byKeyInTimes =0 ;
							break;
						default:
							break;
						
					 }
					 break;
				case NUMBER_PW_INPUT_FACE://��������������
					if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
					{
						uint8 i=0;
						y=30;x=250;
						for(;i<6;i++)
						{
							(unOperData.ASCII_BCD)[i]=(unOperData.ASCII_BCD)[i+1];
							
						}
						(unOperData.ASCII_BCD)[6] = stLocalControl.stBusDealFreq.bySndSecAddr;
						DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),7,BLUE,WHITE,3,DIS_FRAM_MEM1);
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_ENTER)
					{
						uint8 i=0;
						for(;i<7;i++)
						{
							if(byNumPW[i]!=(unOperData.ASCII_BCD)[i]) break;
						}
						if(i>=7)	//������ȷ
						{
							ShowNumberSetFace(DIS_FRAM_MEM1);
						}
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
						stLocalControl.byKeyInTimes =0 ;				
					}
					break;
					
				case NUMBER_SET_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
					 {
						case KEY_1://��ҽ���ֻ����
							memset((unOperData.ASCII_BCD),0x00,10);
							stLocalControl.byInputBox = INPUT_SECTION;
							//stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowSelfNumberSet(DIS_FRAM_MEM1);	//��1ҳ��ʾ 
							break;

						case KEY_2://��ԡ�ſڷֻ����
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowWcDoorFjNumberSet(DIS_FRAM_MEM1);	//��1ҳ��ʾ 
							break;

						case KEY_3:// �ƶ��ֻ����߱��
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		
							stLocalControl.uiCursorY = 102;		
							ShowMoveFjNumberSet(DIS_FRAM_MEM1);	//��1ҳ��ʾ 
							break;

						case KEY_4:// �Ӵ��ֻ����߱��
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_BED;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowAddFjNumberSet(DIS_FRAM_MEM1);	//��1ҳ��ʾ 
							break;
							
				 		case KEY_5:	//��ͷ�ֻ���������
							memset((unOperData.ASCII_BCD),0x00,10);
							//stLocalControl.byInputBox = INPUT_SECTION;
							stLocalControl.byInputBox = INPUT_ROOM;
							stLocalControl.uiCursorX = 204;		//(480-24*8)/2+5*12=204
							stLocalControl.uiCursorY = 118;		//(272-24*4-8)/2+34=118
							ShowBedFjNumberSet(DIS_FRAM_MEM1);	//��1ҳ��ʾ*/
							break;
							
						case KEY_CLEAR:
							ShowMainMenuFace(DIS_FRAM_MEM1);
							stLocalControl.byKeyInTimes =0 ;
							break;
					 }
					 break;

				case TIME_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_YEAR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;		
								x -=4;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
								DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
								stLocalControl.byInputBox = INPUT_MONTH;
								stLocalControl.uiCursorX = 244;		
								stLocalControl.uiCursorY = 85;		//51+34=85	
								x= stLocalControl.uiCursorX+60;		//28+32
								y= stLocalControl.uiCursorY;
								DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);
							}
							break;
						case INPUT_MONTH:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[2]=(unOperData.ASCII_BCD)[3];
								(unOperData.ASCII_BCD)[3]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((((unOperData.ASCII_BCD)[2]*16+(unOperData.ASCII_BCD)[3])<=0x12) && (((unOperData.ASCII_BCD)[2]*16+(unOperData.ASCII_BCD)[3])!=0x00))
								{//�����·�������Ч
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;	
									x -= 4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_DAY;
									stLocalControl.uiCursorX = 244; 	
									stLocalControl.uiCursorY = 119;		//85+34=119	
									x= stLocalControl.uiCursorX+60; 	//28+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);

								}
							}
							break;							
						case INPUT_DAY:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[4]=(unOperData.ASCII_BCD)[5];
								(unOperData.ASCII_BCD)[5]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[4]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((((unOperData.ASCII_BCD)[4]*16+(unOperData.ASCII_BCD)[5])<=0x31) && (((unOperData.ASCII_BCD)[4]*16+(unOperData.ASCII_BCD)[5])!=0x00))
								{//��������������Ч							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;	
									x -=4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[4]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_HOUR;
									stLocalControl.uiCursorX = 244; 	
									stLocalControl.uiCursorY = 153;		//119+34=153	
									x= stLocalControl.uiCursorX+60; 	//28+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);							
								}
							}
							break;	
						case INPUT_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if(((unOperData.ASCII_BCD)[6]*16+(unOperData.ASCII_BCD)[7])<0x24)
								{//����Сʱ������Ч								
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									x -=4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+159,y+23,BLUE,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_MIN;
									stLocalControl.uiCursorX = 244; 	
									stLocalControl.uiCursorY = 187;		//153+34=187	
									x= stLocalControl.uiCursorX+60; 	//28+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);							
								}
							}
							break;	
						case INPUT_MIN:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[8]=(unOperData.ASCII_BCD)[9];
								(unOperData.ASCII_BCD)[9]=stLocalControl.stBusDealFreq.bySndSecAddr;

								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[8]),2,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if(((unOperData.ASCII_BCD)[8]*16+(unOperData.ASCII_BCD)[9])<0x60)
								{//�������������Ч							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;	
									x -=4;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[8]),2,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����

									stLocalControl.stTime.byYear  = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];	//��
									stLocalControl.stTime.byMonth =  (unOperData.ASCII_BCD)[2]*16+(unOperData.ASCII_BCD)[3];	//��
									stLocalControl.stTime.byDay = (unOperData.ASCII_BCD)[4]*16+(unOperData.ASCII_BCD)[5];	//��
									stLocalControl.stTime.byHour= (unOperData.ASCII_BCD)[6]*16+(unOperData.ASCII_BCD)[7];	//ʱ
									stLocalControl.stTime.byMin= (unOperData.ASCII_BCD)[8]*16+(unOperData.ASCII_BCD)[9];	//��
									stLocalControl.stTime.bySec = 0x00;

									//���͵�����˳���룬�֣�ʱ������죬�£���
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stTime.bySec;
									stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stTime.byMin;
									stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stTime.byHour;
									stLocalControl.stBusDealFreq.byCmd = CMD_DATE_SEND;
									stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stTime.byDay;
									stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stTime.byMonth;
									stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stTime.byYear;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

									ShowMainMenuFace(DIS_FRAM_MEM1);
								}
														
							}
							break;	
					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;
					
				case BED_FJ_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
/*						case INPUT_SECTION:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[2];
								(unOperData.ASCII_BCD)[2]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2] <255)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									stLocalControl.byInputBox = INPUT_ROOM;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 152;		//118+34=152
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//��ʾ��һ�������	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);						
									
								}
							}
							break;*/
						case INPUT_ROOM:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[3]=(unOperData.ASCII_BCD)[4];
								(unOperData.ASCII_BCD)[4]=(unOperData.ASCII_BCD)[5];
								(unOperData.ASCII_BCD)[5]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5] <255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									//stLocalControl.uiCursorY = 186;		//152+34=186
									stLocalControl.uiCursorY = 152;		//118+34=152

									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 						
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//��ʾ��һ�������									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);									
								}
							}
							break;
						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120	
									
									//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = (unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5];
									stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
									stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
									stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//��ʼ���;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

									
									(unOperData.ASCII_BCD)[8] += 1;
									if((unOperData.ASCII_BCD)[8]==10)
									{
										(unOperData.ASCII_BCD)[8]=0;
										(unOperData.ASCII_BCD)[7]+= 1;
										if((unOperData.ASCII_BCD)[7]==10)
										{
											(unOperData.ASCII_BCD)[7]=0;
											(unOperData.ASCII_BCD)[6]+= 1;
										}
									}
									
									//stLocalControl.byInputBox = INPUT_SECTION;
									stLocalControl.byInputBox = INPUT_ROOM;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 118;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);
										
								}
							}
							break;							

					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);
					}
					break;
										
				case VOICE_TIMER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_VOICE_START_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[5] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[5] <0x24)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1); 	//32+120
									stLocalControl.byInputBox = INPUT_VOICE_START_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 85;	//51+34=85 
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[6]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[6]&0x0f;

								}
							}
							break;
						case INPUT_VOICE_START_MIN:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[6] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[6]<0x60)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_VOICE_END_HOUR;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 153; 	//51+34+34*2=153
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[7]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[7]&0x0f;									
								}
							}
							break;
						case INPUT_VOICE_END_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[7] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[7] <0x24)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);
									stLocalControl.byInputBox = INPUT_VOICE_END_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 187; 	//153+34=187
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[8]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[8]&0x0f;									
								}
							}
							break;	
							
					case INPUT_VOICE_END_MIN:
						if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
						{
							(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
							(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
						
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
						}
						else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
						{
							(unOperData.ASCII_BCD)[8] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
							if((unOperData.ASCII_BCD)[8] <0x60)
							{							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY; 
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
								DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);
								
								stLocalControl.byInputBox = INPUT_VOICE_VALUE;
								stLocalControl.uiCursorX = 196;
								stLocalControl.uiCursorY = 221; 	//187+34=221
								x= stLocalControl.uiCursorX+56; 	//24+32
								y= stLocalControl.uiCursorY;
								DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
								unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[9]>>4;
								unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[9]&0x0f;		
							}
						}
						break;

					case INPUT_VOICE_VALUE:
						if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
						{
							(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
							(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;

							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,POWDER_BLUE,CYAN,2,DIS_FRAM_MEM1);
						}
						else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
						{
							(unOperData.ASCII_BCD)[9] = (unOperData.ASCII_BCD)[0]*10+(unOperData.ASCII_BCD)[1];
							if((unOperData.ASCII_BCD)[9] <9)
							{							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY; 
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,YELLOW,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
								DisplayBlock(x,y,x+151,y+23,YELLOW,DIS_FRAM_MEM1);

								stLocalControl.stBusDealFreq.bySndSecAddr = 	(unOperData.ASCII_BCD)[5];		//��ʼСʱ
								stLocalControl.stBusDealFreq.bySndRoomAddr = 	(unOperData.ASCII_BCD)[6];		//��ʼ����
								stLocalControl.stBusDealFreq.bySndBedAddr =     0x00;
								stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_TIMER_SET;
								stLocalControl.stBusDealFreq.byRecSecAddr = 	(unOperData.ASCII_BCD)[7];		//��ʼСʱ
								stLocalControl.stBusDealFreq.byRecRoomAddr = 	(unOperData.ASCII_BCD)[8];		//��ʼ����
								stLocalControl.stBusDealFreq.byRecBedAddr =     (unOperData.ASCII_BCD)[9];

								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
								ShowMainMenuFace(DIS_FRAM_MEM1);		
							}
						}
						break;							
					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;

				case MAX_VOL_SET_FACE:
					if(stLocalControl.stBusDealFreq.bySndSecAddr<9)		//��Ч������ֵ
					{
						(unOperData.ASCII_BCD)[0]= stLocalControl.stBusDealFreq.bySndSecAddr;
						x=stLocalControl.uiCursorX;
						y=stLocalControl.uiCursorY;
						DisplayDigitString(&x,&y,unOperData.ASCII_BCD,1,RED,CYAN,2,DIS_FRAM_MEM1);
					}
					
					else  if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
					{
						stLocalControl.stEepromCfgData.byMaxPlayVolVal= (unOperData.ASCII_BCD)[0];
							
						stLocalControl.stBusDealFreq.bySndSecAddr = 	stLocalControl.stEepromCfgData.byMaxPlayVolVal;
						stLocalControl.stBusDealFreq.byCmd = CMD_MAX_VOL_SET;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						ShowMainMenuFace(DIS_FRAM_MEM1);	
						SaveParameter(IAP0_ADDR);
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;

				case BACK_LIGHT_TIMER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_BL_START_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLACK,RED,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[5] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[5] <0x24)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),2,BLACK,CYAN,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//���"��ȷ�ϼ�"
									stLocalControl.byInputBox = INPUT_BL_START_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 85;	//51+34=85  
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[6]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[6]&0x0f;
								}
							}
							break;
						case INPUT_BL_START_MIN:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,RED,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[6] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[6] <0x60)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,CYAN,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//���"��ȷ�ϼ�"
									stLocalControl.byInputBox = INPUT_BL_END_HOUR;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 187; 	//51+34+34*3=187
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[7]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[7]&0x0f;									
								}
							}
							break;
						case INPUT_BL_END_HOUR:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,RED,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								(unOperData.ASCII_BCD)[7] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
								if((unOperData.ASCII_BCD)[7] <0x24)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,CYAN,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//���"��ȷ�ϼ�"
									stLocalControl.byInputBox = INPUT_BL_END_MIN;
									stLocalControl.uiCursorX = 196;
									stLocalControl.uiCursorY = 221; 	//51+34+34*3+34=221
									x= stLocalControl.uiCursorX+56; 	//24+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,CYAN,2,DIS_FRAM_MEM1);
									unOperData.ASCII_BCD[0] =unOperData.ASCII_BCD[8]>>4;
									unOperData.ASCII_BCD[1] =unOperData.ASCII_BCD[8]&0x0f;										
								}
							}
							break;	
							
					case INPUT_BL_END_MIN:
						if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
						{
							(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
							(unOperData.ASCII_BCD)[1]=stLocalControl.stBusDealFreq.bySndSecAddr;
						
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,RED,2,DIS_FRAM_MEM1);
						}
						else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
						{
							(unOperData.ASCII_BCD)[8] = (unOperData.ASCII_BCD)[0]*16+(unOperData.ASCII_BCD)[1];
							if((unOperData.ASCII_BCD)[8] <0x60)
							{							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY; 
								DisplayDigitString(&x,&y,unOperData.ASCII_BCD,2,BLACK,CYAN,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
								DisplayBlock(x,y,x+151,y+23,CYAN,DIS_FRAM_MEM1);	//32+120	//���"��ȷ�ϼ�"						
								stLocalControl.stBusDealFreq.bySndSecAddr = 	(unOperData.ASCII_BCD)[5];		//��ʼСʱ
								stLocalControl.stBusDealFreq.bySndRoomAddr = 	(unOperData.ASCII_BCD)[6];		//��ʼ����
								stLocalControl.stBusDealFreq.bySndBedAddr =     0x00;
								stLocalControl.stBusDealFreq.byCmd = CMD_BL_TIMER_SET;
								stLocalControl.stBusDealFreq.byRecSecAddr = 	(unOperData.ASCII_BCD)[7];		//��ʼСʱ
								stLocalControl.stBusDealFreq.byRecRoomAddr = 	(unOperData.ASCII_BCD)[8];		//��ʼ����
								stLocalControl.stBusDealFreq.byRecBedAddr =     0x00;

								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
								ShowMainMenuFace(DIS_FRAM_MEM1);		
							}
						}
						break;					
					}
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}
					break;

				case SEC_INFO_SET_FACE:
					stLocalControl.bySecondFlashTime =0;
					if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
					{
						if(stLocalControl.byKeyInTimes<3)
						{
							(unOperData.ASCII_BCD)[stLocalControl.byKeyInTimes]= stLocalControl.stBusDealFreq.bySndSecAddr;
							stLocalControl.byKeyInTimes ++;
						}
						else
						{
							(unOperData.ASCII_BCD)[0]=stLocalControl.stBusDealFreq.bySndSecAddr;
							stLocalControl.byKeyInTimes =1;
						}
						
						x=stLocalControl.uiCursorX;
						y=stLocalControl.uiCursorY;
						DisplayBlock(x,y,x+39,y+23,GREEN,DIS_FRAM_MEM1);
						DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,GREEN,2,DIS_FRAM_MEM1);
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_MR)	//�˸�
					{
						if(stLocalControl.byKeyInTimes>0)
						{
							stLocalControl.byKeyInTimes--;
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY;
							DisplayBlock(x,y,x+39,y+23,GREEN,DIS_FRAM_MEM1);
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,GREEN,2,DIS_FRAM_MEM1);									
						}
					}
					else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
					{
						stLocalControl.bySecondFlashTime =25;
						if(stLocalControl.byKeyInTimes==0)
						{
							(unOperData.uiOperData)[4] = WardDataBuff[stLocalControl.byInputBox+6];
							(unOperData.ASCII_BCD)[0] = (unOperData.uiOperData)[4]/100;
							(unOperData.ASCII_BCD)[1] = ((unOperData.uiOperData)[4]%100)/10;
							(unOperData.ASCII_BCD)[2] = ((unOperData.uiOperData)[4]%100)%10;
							if((unOperData.ASCII_BCD)[0])
							{
								stLocalControl.byKeyInTimes =3;		//����һ��3
							}
							else if((unOperData.ASCII_BCD)[1])
							{
								(unOperData.ASCII_BCD)[0] =(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1] =(unOperData.ASCII_BCD)[2];
								stLocalControl.byKeyInTimes =2;		//����һ��2
							}
							else 
							{							
								(unOperData.ASCII_BCD)[0] =(unOperData.ASCII_BCD)[2];
								stLocalControl.byKeyInTimes =1;		//����һ��1
							}							
						}

						else 
						{
							if(stLocalControl.byKeyInTimes==3)
							{
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
								
							}
							else if(stLocalControl.byKeyInTimes==2)
							{
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*10+(unOperData.ASCII_BCD)[1];
								
							}
							else if(stLocalControl.byKeyInTimes==1)
							{
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0];
								
							}							
						}
						if((unOperData.uiOperData)[4]<=255)
						{
							x=stLocalControl.uiCursorX;
							y=stLocalControl.uiCursorY; 
							DisplayBlock(x,y,x+39,y+23,WHITE,DIS_FRAM_MEM1);
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,WHITE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
															
							switch(stLocalControl.byInputBox)
							{
								case INPUT_WARD_SEC:							
									WardDataBuff[6]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_PATIENT_SUM;
									stLocalControl.uiCursorX = 360;
									stLocalControl.uiCursorY = 72;
									break;							
								case INPUT_PATIENT_SUM:							
									WardDataBuff[7]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_TERMINAL_PATIENT_SUM;
									stLocalControl.uiCursorX = 72;
									stLocalControl.uiCursorY = 107;
									break;
								case INPUT_TERMINAL_PATIENT_SUM:
									WardDataBuff[8]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_BED_SUM;
									stLocalControl.uiCursorX = 232;
									stLocalControl.uiCursorY = 107;
									break;	
								case INPUT_BED_SUM:
									WardDataBuff[9]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_EMPTY_BED_SUM;
									stLocalControl.uiCursorX = 392;
									stLocalControl.uiCursorY = 107;
									break;
								case INPUT_EMPTY_BED_SUM:
									WardDataBuff[10]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_ADMISSION_SUM;
									stLocalControl.uiCursorX = 72;
									stLocalControl.uiCursorY = 142;
									break;
								case INPUT_ADMISSION_SUM:
									WardDataBuff[11]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_DISCHARGE_SUM;
									stLocalControl.uiCursorX = 312;
									stLocalControl.uiCursorY = 142;
									break;	
								case INPUT_DISCHARGE_SUM:	//��Ժ
									WardDataBuff[12]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_SWITCH_IN_SUM;
									stLocalControl.uiCursorX = 72;
									stLocalControl.uiCursorY = 175;
									break;
								case INPUT_SWITCH_IN_SUM:
									WardDataBuff[13]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_SWITCH_OUT_SUM;
									stLocalControl.uiCursorX = 312;
									stLocalControl.uiCursorY = 175;
									break;	
								case INPUT_SWITCH_OUT_SUM:
									WardDataBuff[14]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_PRIMARY_CARE_SUM;
									stLocalControl.uiCursorX = 120;
									stLocalControl.uiCursorY = 209;
									break;
								case INPUT_PRIMARY_CARE_SUM:
									WardDataBuff[15]= (uint8)(unOperData.uiOperData)[4];
									stLocalControl.byInputBox = INPUT_OXYGEN_SUPPLY_SUM;
									stLocalControl.uiCursorX = 312;
									stLocalControl.uiCursorY = 209;
									break;	
								case INPUT_OXYGEN_SUPPLY_SUM:
									WardDataBuff[16]= (uint8)(unOperData.uiOperData)[4];

									AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);

									ShowMainMenuFace(DIS_FRAM_MEM1);
									StoreSecInfo(DIS_FRAM_MEM0);

									memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(WardDataBuff[6]),0x03);
									stLocalControl.stBusDealFreq.byCmd = CMD_WARD_SEC_INFO_SET;
									stLocalControl.stBusDealFreq.byRecSecAddr =  WardDataBuff[9];
									stLocalControl.stBusDealFreq.byRecRoomAddr = WardDataBuff[10];
									stLocalControl.stBusDealFreq.byRecBedAddr = 0x01;	//��һ֡����
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									//OSWait(K_TMO,100);	//��ʱ500ms
									Delayms(1000);

									memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(WardDataBuff[11]),0x03);
									stLocalControl.stBusDealFreq.byCmd = CMD_WARD_SEC_INFO_SET;
									stLocalControl.stBusDealFreq.byRecSecAddr =  WardDataBuff[14];
									stLocalControl.stBusDealFreq.byRecRoomAddr = WardDataBuff[15];
									stLocalControl.stBusDealFreq.byRecBedAddr = 0x02;	//�ڶ�֡����
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									//OSWait(K_TMO,100);	//��ʱ500ms
									Delayms(1000);

									stLocalControl.stBusDealFreq.bySndSecAddr = WardDataBuff[16];
									stLocalControl.stBusDealFreq.byCmd = CMD_WARD_SEC_INFO_SET;
									stLocalControl.stBusDealFreq.byRecBedAddr = 0x03;	//����֡����
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));									
									break;										
							}
							stLocalControl.byKeyInTimes =0;
						}
					} 							
						
					else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						stLocalControl.bySecondFlashTime =25;
						ShowMainMenuFace(DIS_FRAM_MEM1);
					}					
					break;


				case MOVE_FJ_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{

						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[2];
								(unOperData.ASCII_BCD)[2]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2] <255)
								{							
							    	x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									
									stLocalControl.byInputBox = INPUT_BED_START;
									stLocalControl.uiCursorX = 228;		
									stLocalControl.uiCursorY = 136;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//��ʾ��һ�������	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);	
								}
							}
							break;


						case INPUT_BED_START:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[3]=(unOperData.ASCII_BCD)[4];
								(unOperData.ASCII_BCD)[4]=(unOperData.ASCII_BCD)[5];
								(unOperData.ASCII_BCD)[5]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5] <=255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[3]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									
									stLocalControl.byInputBox = INPUT_BED_END;
									stLocalControl.uiCursorX = 228; 	
									stLocalControl.uiCursorY = 170; 	
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//��ʾ��һ�������	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);	
								}
							}
							break;

									
									
						case INPUT_BED_END:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <=255)
								{							
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120


									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
									stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
									stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
									stLocalControl.stBusDealFreq.byRecSecAddr =  (unOperData.ASCII_BCD)[3]*100+(unOperData.ASCII_BCD)[4]*10+(unOperData.ASCII_BCD)[5];
									stLocalControl.stBusDealFreq.byRecRoomAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
									stLocalControl.stBusDealFreq.byRecBedAddr  =  0x00;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											

									(unOperData.ASCII_BCD)[2] += 1;
									if((unOperData.ASCII_BCD)[2]==10)
									{
										(unOperData.ASCII_BCD)[2]=0;
										(unOperData.ASCII_BCD)[1]+= 1;
										if((unOperData.ASCII_BCD)[1]==10)
										{
											(unOperData.ASCII_BCD)[1]=0;
											(unOperData.ASCII_BCD)[0]+= 1;
										}
									}
									
									//stLocalControl.byInputBox = INPUT_SECTION;
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 102;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD[0]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);											

								}
							}
							break;							
					}
					
						
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);						
						
					}					
					break;



				case WC_FJ_NUMBER_SET_FACE:
				//case MOVE_FJ_NUMBER_SET_FACE:
				case ADD_FJ_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <255)
								{							
							/*		x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 								
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120	*/

									switch(stLocalControl.byDisplayFace) 
									{
										
										case WC_FJ_NUMBER_SET_FACE:
											//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
											stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
											stLocalControl.stBusDealFreq.bySndRoomAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
											stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;
											stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
											stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//��ʼ���;
											Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											
											break;
										
										//case MOVE_FJ_NUMBER_SET_FACE:
											//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
											stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
											stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
											stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
											stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
											stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//��ʼ���;
											Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											
											break;

										case ADD_FJ_NUMBER_SET_FACE:
											//stLocalControl.stBusDealFreq.bySndSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
											stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
											stLocalControl.stBusDealFreq.bySndRoomAddr = 250;
											stLocalControl.stBusDealFreq.bySndBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
											stLocalControl.stBusDealFreq.byCmd = CMD_SET_NUMBER;
											stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;	//��ʼ���;
											Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));											
											break;											


										default:
											break;
									}

									(unOperData.ASCII_BCD)[8] += 1;
									if((unOperData.ASCII_BCD)[8]==10)
									{
										(unOperData.ASCII_BCD)[8]=0;
										(unOperData.ASCII_BCD)[7]+= 1;
										if((unOperData.ASCII_BCD)[7]==10)
										{
											(unOperData.ASCII_BCD)[7]=0;
											(unOperData.ASCII_BCD)[6]+= 1;
										}
									}
									
									//stLocalControl.byInputBox = INPUT_SECTION;
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 118;		
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
									
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);											

								}
							}
							break;							
					}
					
						
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);						
						
					}					
					break;


				case SELF_NUMBER_SET_FACE:
					switch(stLocalControl.byInputBox)
					{
						case INPUT_SECTION:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[1];
								(unOperData.ASCII_BCD)[1]=(unOperData.ASCII_BCD)[2];
								(unOperData.ASCII_BCD)[2]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2] <255)
								{
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY; 
									DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),3,WHITE,BLUE,2,DIS_FRAM_MEM1);//������������ɫ�ָ�����
									DisplayBlock(x,y,x+151,y+23,BLUE,DIS_FRAM_MEM1);	//32+120
									stLocalControl.byInputBox = INPUT_BED;
									stLocalControl.uiCursorX = 204;		
									stLocalControl.uiCursorY = 152;		//118+34=152
									x=stLocalControl.uiCursorX;
									y=stLocalControl.uiCursorY;
									DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);	//��ʾ��һ�������	
									x= stLocalControl.uiCursorX+68; 	//36+32
									y= stLocalControl.uiCursorY;
									DisplayString(&x,&y,"��[ȷ��]��",10,YELLOW,BLUE,2,DIS_FRAM_MEM1);						
									
								}
							}
							break;
							

						case INPUT_BED:
							if(stLocalControl.stBusDealFreq.bySndSecAddr<10)
							{
								(unOperData.ASCII_BCD)[6]=(unOperData.ASCII_BCD)[7];
								(unOperData.ASCII_BCD)[7]=(unOperData.ASCII_BCD)[8];
								(unOperData.ASCII_BCD)[8]=stLocalControl.stBusDealFreq.bySndSecAddr;
							
								x=stLocalControl.uiCursorX;
								y=stLocalControl.uiCursorY;
								DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[6]),3,BLUE,WHITE,2,DIS_FRAM_MEM1);
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ENTER)
							{
								if((unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8] <255)
								{							
	
									stLocalControl.stEepromCfgData.bySelfSecAddr = (unOperData.ASCII_BCD)[0]*100+(unOperData.ASCII_BCD)[1]*10+(unOperData.ASCII_BCD)[2];
									stLocalControl.stEepromCfgData.bySelfRoomAddr = 253;
									stLocalControl.stEepromCfgData.bySelfBedAddr = (unOperData.ASCII_BCD)[6]*100+(unOperData.ASCII_BCD)[7]*10+(unOperData.ASCII_BCD)[8];
									SaveParameter(IAP0_ADDR);
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
									stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
									stLocalControl.stBusDealFreq.byCmd = CMD_SELF_SET_NUMBER;
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
										
									ShowNumberSetFace(DIS_FRAM_MEM1);
								}
							}
							break;							
					}
					
						
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowNumberSetFace(DIS_FRAM_MEM1);						
						
					}					
					break;
					
				case CALCULATE_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
				 	{
				 		case KEY_0:
						case KEY_1:
				 		case KEY_2:
						case KEY_3:	
				 		case KEY_4:
						case KEY_5:
				 		case KEY_6:
						case KEY_7:
				 		case KEY_8:
						case KEY_9:	
							if((stLocalControl.byKeyInTimes==0)&& (stLocalControl.stBusDealFreq.bySndSecAddr==KEY_0))
								break;
							if(stLocalControl.byKeyInTimes==0) 
							{
								DisplayBlock(100,104,380,127,WHITE,DIS_FRAM_MEM1);
								bSignflag = 0;	
							}
							(unOperData.ASCII_BCD)[stLocalControl.byKeyInTimes]= stLocalControl.stBusDealFreq.bySndSecAddr;
							stLocalControl.byKeyInTimes++;
							if(stLocalControl.byKeyInTimes==6)
							{//���ֻ������5λ��
								DisplayBlock(304,104,380,127,WHITE,DIS_FRAM_MEM1);
								stLocalControl.byKeyInTimes=0;
								if(stLocalControl.stBusDealFreq.bySndSecAddr==KEY_0)	break;
								(unOperData.ASCII_BCD)[0] = stLocalControl.stBusDealFreq.bySndSecAddr;
								stLocalControl.byKeyInTimes=1;
							}
							x=stLocalControl.uiCursorX-(stLocalControl.byKeyInTimes-1)*12;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,WHITE,2,DIS_FRAM_MEM1);
							break;

						case KEY_ADDBED:	// +
						case KEY_SEC:		// - 
						case KEY_BROAD:		// X
						case KEY_MUSIC:		//  /
							switch(stLocalControl.byKeyInTimes)
							{
								case 5:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*10000+
																 (unOperData.ASCII_BCD)[1]*1000+ 
																 (unOperData.ASCII_BCD)[2]*100+
																 (unOperData.ASCII_BCD)[3]*10+ 
																 (unOperData.ASCII_BCD)[4];
									break;
								case 4:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*1000+
																 (unOperData.ASCII_BCD)[1]*100+ 
																 (unOperData.ASCII_BCD)[2]*10+
																 (unOperData.ASCII_BCD)[3];
									break;
								case 3:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*100+
																 (unOperData.ASCII_BCD)[1]*10+ 
																 (unOperData.ASCII_BCD)[2];
									break;
								case 2:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]*10+
																 (unOperData.ASCII_BCD)[1]; 
									break;	
								case 1:
									(unOperData.uiOperData)[3] = (unOperData.ASCII_BCD)[0]; 
									break;	
							
								default:
									break;
							}

							if(bSignflag)
							{//��һ������
								(unOperData.uiOperData)[3] = (unOperData.uiOperData)[3] ^ 0xffff;
								(unOperData.uiOperData)[3] = (unOperData.uiOperData)[3]+1;
								(unOperData.uiOperData)[3] = (unOperData.uiOperData)[3] | 0x8000;
							}
//							Send_Data(&((unOperData.uiOperData)[3]),2);

							if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_ADDBED)
							{
								DisplayASCII(120,104,'+',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = ADD_OPER;
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_SEC)
							{
								DisplayASCII(120,104,'-',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = SUB_OPER;
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_BROAD)
							{
								DisplayASCII(120,104,'*',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = MUL_OPER;
							}
							else if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_MUSIC)
							{
								DisplayASCII(120,104,'/',BLACK,WHITE,2,DIS_FRAM_MEM1);
								stLocalControl.byOperType = DIV_OPER;
							}							
							stLocalControl.byKeyInTimes = 0;
							bSignflag = 0;
							return;

						
						default:
							break;
					 }
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
						return;
					}

					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_MR)		//Ԥ�����˸����
					{
						DisplayBlock(100,104,380,127,WHITE,DIS_FRAM_MEM1);	//�����
						if(stLocalControl.byKeyInTimes)						
						{
							stLocalControl.byKeyInTimes--;						
							x=stLocalControl.uiCursorX-(stLocalControl.byKeyInTimes-1)*12;
							y=stLocalControl.uiCursorY;
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),stLocalControl.byKeyInTimes,BLACK,WHITE,2,DIS_FRAM_MEM1);							
						}
						else bSignflag = 0;
						return;
					}


					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_SEARCH)		//��������+/-��
					{
						if(stLocalControl.byKeyInTimes)
						{
							if(bSignflag)
							{//1--ԭΪ����,�ٰ�һ��,��Ϊ����
								DisplayBlock(292,104,303,127,WHITE,DIS_FRAM_MEM1);	//���
								bSignflag = 0;
							}
							else
							{
								DisplayASCII(292,104,'-',BLACK,WHITE,2,DIS_FRAM_MEM1);
								bSignflag = 1;
							}
						}
						return;
					}					
					
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CALCULATE)
					{
						switch(stLocalControl.byKeyInTimes)
						{
							case 5:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*10000+
									                    	 (unOperData.ASCII_BCD)[1]*1000+ 
									                    	 (unOperData.ASCII_BCD)[2]*100+
									                    	 (unOperData.ASCII_BCD)[3]*10+ 
									                    	 (unOperData.ASCII_BCD)[4];
								break;
							case 4:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*1000+
									                    	 (unOperData.ASCII_BCD)[1]*100+ 
									                    	 (unOperData.ASCII_BCD)[2]*10+
									                    	 (unOperData.ASCII_BCD)[3];
								break;
							case 3:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*100+
									                    	 (unOperData.ASCII_BCD)[1]*10+ 
									                    	 (unOperData.ASCII_BCD)[2];
								break;
							case 2:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0]*10+
									                    	 (unOperData.ASCII_BCD)[1];	
								break;	
							case 1:
								(unOperData.uiOperData)[4] = (unOperData.ASCII_BCD)[0];	
								break;	

							default:
								break;
						}
						
						if(bSignflag)
						{//��һ������
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4] ^ 0xffff;	//����
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]+1;			//����
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4] | 0x8000;
							bSignflag = 0;
						}

//						Send_Data(&((unOperData.uiOperData)[4]),2);
						DisplayASCII(120,104,'=',BLACK,WHITE,2,DIS_FRAM_MEM1);

						switch(stLocalControl.byOperType)
						{
							case ADD_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]+(unOperData.uiOperData)[4];
								break;
							case SUB_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]-(unOperData.uiOperData)[4];
								break;
							case MUL_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]*(unOperData.uiOperData)[4];	
								break;	
							case DIV_OPER:
								(unOperData.uiOperData)[0] =(unOperData.uiOperData)[3]/(unOperData.uiOperData)[4];	
								break;								
						}
						(unOperData.uiOperData)[3] =(unOperData.uiOperData)[0];	//������(unOperData.uiOperData)[3]��,��Ϊ�´������Դ������
						(unOperData.uiOperData)[4] =(unOperData.uiOperData)[0];
						if( (unOperData.uiOperData)[4] & 0x8000 )
						{
							DisplayASCII(292,104,'-',BLUE,WHITE,2,DIS_FRAM_MEM1);
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]&0x7fff ;
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]-1;
							(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]^0x7fff;
							bSignflag = 0;	
						}
						//else bSignflag =0;	//���������

//						Send_Data(&((unOperData.uiOperData)[4]),2);	
						
						(unOperData.ASCII_BCD)[0]  = (unOperData.uiOperData)[4]/10000;
						(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]%10000;
						(unOperData.ASCII_BCD)[1]  = (unOperData.uiOperData)[4]/1000;
						(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]%1000;
						(unOperData.ASCII_BCD)[2]  = (unOperData.uiOperData)[4]/100;
						(unOperData.uiOperData)[4] = (unOperData.uiOperData)[4]%100;
						(unOperData.ASCII_BCD)[3]  = (unOperData.uiOperData)[4]/10;
						(unOperData.ASCII_BCD)[4] = (unOperData.uiOperData)[4]%10;
						
						DisplayBlock(304,104,380,127,WHITE,DIS_FRAM_MEM1);	//�����
						stLocalControl.byKeyInTimes = 0;
						stLocalControl.byOperType = 0;


						if((unOperData.ASCII_BCD)[0])
						{
							x=316;y=104;
							DisplayDigitString(&x,&y,(unOperData.ASCII_BCD),5,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else if((unOperData.ASCII_BCD)[1])
						{
							x=328;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[1],4,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else if((unOperData.ASCII_BCD)[2])
						{
							x=340;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[2],3,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else if((unOperData.ASCII_BCD)[3])
						{
							x=352;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[3],2,BLUE,WHITE,2,DIS_FRAM_MEM1);
						}
						else 
						{
							x=364;y=104;
							DisplayDigitString(&x,&y,&(unOperData.ASCII_BCD)[4],1,BLUE,WHITE,2,DIS_FRAM_MEM1);							
						}
						return;
					}					
					 break;
					 
/*				case TOUCH_ADJUST_FACE:
					if(stLocalControl.stBusDealFreq.bySndSecAddr == KEY_CLEAR)
					{
						ShowMainMenuFace(DIS_FRAM_MEM1);
						return;
					}
					break;*/
				
				case SEC_INFO_FACE:
					 switch(stLocalControl.stBusDealFreq.bySndSecAddr)
				 	{
				 		case KEY_0:
						case KEY_1:
				 		case KEY_2:
						case KEY_3:	
				 		case KEY_4:
						case KEY_5:
				 		case KEY_6:
						case KEY_7:
				 		case KEY_8:
						case KEY_9:	
							if(stLocalControl.byKeyInTimes==0)
							{
								DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
							}
							DisplayDigit(stLocalControl.byKeyInTimes*16,240,stLocalControl.stBusDealFreq.bySndSecAddr,WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
						case KEY_MODE:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'M',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;

						case KEY_ROOM:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'R',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;

						case KEY_SEC:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'S',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
						case KEY_ADDBED:
							DisplayASCII(stLocalControl.byKeyInTimes*16,240,'B',WHITE,BLUE,3,DIS_FRAM_MEM0);
							if(++stLocalControl.byKeyInTimes==30) stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
							
						case KEY_CLEAR:
							DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
							stLocalControl.byKeyInTimes=0;
							Set_VisualPage(DIS_FRAM_MEM0);
							stLocalControl.byDisplayFace = SEC_INFO_FACE;
							break;
							
						case KEY_ENTER:
							if(stLocalControl.byKeyInTimes == 0)
							{
								ShowMainMenuFace(DIS_FRAM_MEM1);
								memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
								memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
								stLocalControl.stBusDealFreq.byCmd = CMD_MAIN_MENU_SET;
								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							}
							break;
													
						default:
							stLocalControl.byKeyInTimes=0;
							DisplayBlock(0,240,479,271,BLUE,DIS_FRAM_MEM0);
							if(stLocalControl.byDisplayFace == SEC_INFO_FACE)
								Set_VisualPage(DIS_FRAM_MEM0);
							break;
					 	}						
					 break;
				
				}
			 break;
		case CMD_INCREACE_VOL:
		case CMD_DECREACE_VOL:			
			x=16;y=240;
			if(INFO_INDICATION_FACE == stLocalControl.byDisplayFace)
			{//��Ϣָʾ����
				DisplayChineseString(&x,&y,"����",2,WHITE,BLUE,3,DIS_FRAM_MEM2);
				DisplayASCIIString(&x,&y,":",1,WHITE,BLUE,3,DIS_FRAM_MEM2);
				DisplayDigitString(&x,&y,&(stLocalControl.stBusDealFreq.byRecSecAddr),1,WHITE,BLUE,3,DIS_FRAM_MEM2);
				Set_VisualPage(DIS_FRAM_MEM2);					
			}
			else
			{
				DisplayChineseString(&x,&y,"����",2,WHITE,BLUE,3,DIS_FRAM_MEM1);
				DisplayASCIIString(&x,&y,":",1,WHITE,BLUE,3,DIS_FRAM_MEM1);
				DisplayDigitString(&x,&y,&(stLocalControl.stBusDealFreq.byRecSecAddr),1,WHITE,BLUE,3,DIS_FRAM_MEM1);
				Set_VisualPage(DIS_FRAM_MEM1);
			}
			break;
			
		case CMD_SET_NUMBER:
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x01)
			{		
				ShowStartNumSet(DIS_FRAM_MEM1);
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x02)
			{
				ShowNumSetOk(DIS_FRAM_MEM1);	
				stLocalControl.byDispNumSetOkTime=50;		//��ʾһ��
				//�����ַ
				stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
				SaveParameter(IAP0_ADDR);			
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x00)	//��ų�ʱ
			{
				Set_VisualPage(DIS_FRAM_MEM0);
				stLocalControl.byDisplayFace=SEC_INFO_FACE;
				break;
			}
			break;

		case CMD_SUPPLY_OX_START:
			WardDataBuff[16]=Read(WARD_FACE_DATA_START_ADDR+16);
			WardDataBuff[16]++;
			AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);

			x=312;y=175 /*209*/;
			DisplayBlock(312,175/*209*/,312+35,175/*209*/+23,BLUE,DIS_FRAM_MEM0);
			(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
			
			(unOperData.ASCII_BCD)[1]=(WardDataBuff[16]%100)/10;	
			(unOperData.ASCII_BCD)[2]=(WardDataBuff[16]%100)%10;
			
			if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
			{
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}
			else
			{
				if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,DIS_FRAM_MEM0);
				else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}	
			break;

		case CMD_SUPPLY_OX_END:
			WardDataBuff[16]=Read(WARD_FACE_DATA_START_ADDR+16);
			WardDataBuff[16]--;
			AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);

			x=312;y=175/*209*/;
			DisplayBlock(312,175/*209*/,312+35,175/*209*/+23,BLUE,DIS_FRAM_MEM0);
			(unOperData.ASCII_BCD)[0]=WardDataBuff[16]/100;
			
			(unOperData.ASCII_BCD)[1]=(WardDataBuff[16]%100)/10;	
			(unOperData.ASCII_BCD)[2]=(WardDataBuff[16]%100)%10;
			
			if((unOperData.ASCII_BCD)[0])	//��λ��Ϊ0
			{
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}
			else
			{
				if((unOperData.ASCII_BCD)[1])	//ʮλ��Ϊ0
					DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[1]),2,WHITE,BLUE,2,DIS_FRAM_MEM0);
				else  DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[2]),1,WHITE,BLUE,2,DIS_FRAM_MEM0);
			}	
			break;	

		case CMD_VOICE_TIMER_GET:
			memset((unOperData.ASCII_BCD),0x00,10);
			unOperData.ASCII_BCD[5] =stLocalControl.stBusDealFreq.bySndSecAddr;
			unOperData.ASCII_BCD[6] =stLocalControl.stBusDealFreq.bySndRoomAddr;
			unOperData.ASCII_BCD[7] =stLocalControl.stBusDealFreq.byRecSecAddr;
			unOperData.ASCII_BCD[8] =stLocalControl.stBusDealFreq.byRecRoomAddr;
			unOperData.ASCII_BCD[9] =stLocalControl.stBusDealFreq.byRecBedAddr;
			stLocalControl.byInputBox = INPUT_VOICE_START_HOUR;
			stLocalControl.uiCursorX = 196; 	//64+12*11=196
			stLocalControl.uiCursorY = 51;		//(272-32*7-14)/2+34=51
			ShowVoiceTimerSet(DIS_FRAM_MEM1);	//����ʱ�������  ��1ҳ��ʾ 
			break;
		case CMD_BL_TIMER_GET:
			memset((unOperData.ASCII_BCD),0x00,10);
			unOperData.ASCII_BCD[5] =stLocalControl.stBusDealFreq.bySndSecAddr;
			unOperData.ASCII_BCD[6] =stLocalControl.stBusDealFreq.bySndRoomAddr;
			unOperData.ASCII_BCD[7] =stLocalControl.stBusDealFreq.byRecSecAddr;
			unOperData.ASCII_BCD[8] =stLocalControl.stBusDealFreq.byRecRoomAddr;		
			stLocalControl.byInputBox = INPUT_BL_START_HOUR;
			stLocalControl.uiCursorX = 196;		//64+12*11=196
			stLocalControl.uiCursorY = 51;		//(272-32*7-14)/2+34=51			
			ShowBackLightTimerSet(DIS_FRAM_MEM1);	//����ʱ�������  ��1ҳ��ʾ			
			break;


/*		case CMD_MAX_VOL_GET:
			memset((unOperData.ASCII_BCD),0x00,10);
			unOperData.ASCII_BCD[5] =stLocalControl.stBusDealFreq.bySndSecAddr;		//���������������
			unOperData.ASCII_BCD[6] =stLocalControl.stBusDealFreq.bySndRoomAddr;	//Ԥ�� : ����������С����
			unOperData.ASCII_BCD[7] =stLocalControl.stBusDealFreq.byRecSecAddr;		//Ԥ�� : ��Ͳͨ��ʱ�������
			unOperData.ASCII_BCD[8] =stLocalControl.stBusDealFreq.byRecRoomAddr;	//Ԥ�� : ��Ͳͨ��ʱ��С����	
			stLocalControl.byInputBox = INPUT_PLAY_VOL_MAX;			
			stLocalControl.uiCursorX = 272;
			stLocalControl.uiCursorY = 60;
			ShowMaxVolSetFace(DIS_FRAM_MEM1);	//��ʾ����������ý���
			break;
*/							

		case CMD_WARD_SEC_INFO_SET:
			switch(stLocalControl.stBusDealFreq.byRecBedAddr)
			{//֡�ű�־
				case 1:
					memcpy(&(WardDataBuff[6]),&(stLocalControl.stBusDealFreq.bySndSecAddr),0x03);
					WardDataBuff[9]	=	stLocalControl.stBusDealFreq.byRecSecAddr;
					WardDataBuff[10]=	stLocalControl.stBusDealFreq.byRecRoomAddr;
//					AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);
//					StoreSecInfo(DIS_FRAM_MEM0);
//					if(stLocalControl.byDisplayFace == SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);
					break;
				case 2:
					memcpy(&(WardDataBuff[11]),&(stLocalControl.stBusDealFreq.bySndSecAddr),0x03);
					WardDataBuff[14]	=	stLocalControl.stBusDealFreq.byRecSecAddr;
					WardDataBuff[15]=	stLocalControl.stBusDealFreq.byRecRoomAddr;
//					AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);
//					StoreSecInfo(DIS_FRAM_MEM0);
//					if(stLocalControl.byDisplayFace == SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);					
					break;
					
				case 3:
					WardDataBuff[16]	=	stLocalControl.stBusDealFreq.bySndSecAddr;
					AAI_Write(WARD_FACE_DATA_START_ADDR,WardDataBuff,WARD_FACE_DATA_LEN);
					StoreSecInfo(DIS_FRAM_MEM0);
					if(stLocalControl.byDisplayFace == SEC_INFO_FACE) Set_VisualPage(DIS_FRAM_MEM0);					
					break;
					
			}
			break;
			
		case CMD_RS485_BRT_SET:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case 1:
					stLocalControl.stEepromCfgData.uiRs485Brt	=1200;
					break;
				case 2:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 2400;
					break;
				case 3:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 4800;
					break;
				case 4:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 9600;
					break;
				case 5:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 14400;
					break;
				case 6:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 19200;
					break;
				case 7:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 28800;
					break;
				case 8:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 38400;
					break;
				case 9:
					stLocalControl.stEepromCfgData.uiRs485Brt	= 57600;
					break;
				default:
					return;		//������Ч���˳�
					
			}
			UsartInit();
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;
			
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr	= stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;
		default:
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
*������	      		:����ͬ
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

	//���½���������
	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ
/*				CCAPM0=0X48;	//��ֹ����
				CCF0=0;
				MCUMasterSPI();	//�ڴ˷�����������ģʽ�л�����,�ɹ�
				FRAM_TEST();	
				EPH1660MasterSPI();
				CCAPM0=0X49;	//��������*/
				MakeCH0TimerOut(150, 0);				
			}			
			break;
			
		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 0;								//�������߹���
			SaveParameter(IAP0_ADDR);
			Bus0SendPin = 1;								//�ͷ�����				
			break;	
			
		default:
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
*������	      		:����ͬ
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
		WDT_CONTR = 0x3d; 
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
*����ģ��  			:Bus0OutputData,Bus1OutputData,VoiceChannelCtx,
					 LedControl,SetHandLedState
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void TimerOutDeal(void)
{ 

	uint16 xdata x;
	uint16 xdata y;
	if(stLocalControl.stCH0TimerOut.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stLocalControl.stCH0TimerOut.byTimerOut--;
		if(stLocalControl.stCH0TimerOut.byTimerOut == 0)
		{	//��ʱһ�ε��� 
			if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
			{ 	//���г�ʱ���
				goto TIMER0OUTDEAL;
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH0TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
				{ 	//���г�ʱ���
TIMER0OUTDEAL:
					if(bLanding)
					{	//�ϵ�״̬
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_LANDING;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySerialNum1;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySerialNum2;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySerialNum3;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));						
					}
				}
				else
				{ 	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 				
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}
			}			
		}
	}
	if(--stLocalControl.bySecondFlashTime==0)
	{
		stLocalControl.bySecondFlashTime=25;
		
		bSecondDisState=!bSecondDisState;
		if(stLocalControl.byDisplayFace==SEC_INFO_FACE)
		{
			if(bSecondDisState)
			{
				Display_ASCII(336,0,ColonCode16x32,WHITE,BLUE,3,DIS_FRAM_MEM0);	//":"				
			}
			else
			{
				DisplayBlock(336,0,336+15,31,BLUE,DIS_FRAM_MEM0);
			}
			Set_VisualPage(DIS_FRAM_MEM0);
		}
		else if(stLocalControl.byDisplayFace == SEC_INFO_SET_FACE)
		{
			(unOperData.ASCII_BCD)[9]= WardDataBuff[stLocalControl.byInputBox+6];	//��ȡҪ��˸������
			x = stLocalControl.uiCursorX;
			y = stLocalControl.uiCursorY;
			if(bSecondDisState)
			{	
				(unOperData.ASCII_BCD)[0]=(unOperData.ASCII_BCD)[9]/100;
				(unOperData.ASCII_BCD)[1]=((unOperData.ASCII_BCD)[9]%100)/10;	
				(unOperData.ASCII_BCD)[2]=((unOperData.ASCII_BCD)[9]%100)%10;
				
				DisplayDigitString(&x,&y,&((unOperData.ASCII_BCD)[0]),3,WHITE,BLUE,2,DIS_FRAM_MEM1);
				
			}
			else
			{
				DisplayBlock(x,y,x+35,y+23,BLUE,DIS_FRAM_MEM1);
			}

		}
	}
	
	if(stLocalControl.byDispNumSetOkTime)
	{
		if(--stLocalControl.byDispNumSetOkTime==0)
		{
			Set_VisualPage(DIS_FRAM_MEM0);
			stLocalControl.byDisplayFace = SEC_INFO_FACE;
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
*������	      		:����ͬ
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
		WDT_CONTR = 0x3d; 
		OSWait(K_TMO, 4);	 								//����20ms��ʱ
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
  	  
