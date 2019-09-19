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




//����0��������  ����0�������ⲿ�豸ͨ��
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
sbit  Bus0RecPin	= P3^2;									//������0�������Ŷ���
sbit  Bus0SendPin	= P1^7;									//������0�������Ŷ���



//����1��������    ����1�������ڲ�ģ��ͨ��
uint8 bdata byBus1State0 	= 0;							//������1״̬��1
sbit  bBus1StartRec			= byBus1State0^0;				//������1��ʼ������ʼλ��־
sbit  bBus1OnRec			= byBus1State0^1;				//������1��ʼ��������λ��־
sbit  bBus1Enable			= byBus1State0^2;				//������1�����ͱ�־
sbit  bBus1ReqSend			= byBus1State0^3;				//������1�����ͱ�־
sbit  bBus1OnSendBit		= byBus1State0^4;				//������1���ڷ���һ������λ��־
sbit  bBus1OnSendFreq		= byBus1State0^5;				//������1���ڷ���һ������֡��־
sbit  bBus1SendError		= byBus1State0^6;				//������1���ʹ����־
sbit  bBus1Error			= byBus1State0^7;				//������1���ϱ�־
uint8 bdata byBus1State1 	= 0;  							//������1״̬��2
sbit  bBus1RecFinish		= byBus1State1^0;				//������1������ɱ�־
sbit  bBus1RecBit9			= byBus1State1^1;				//������1�����ֽ����ݵĵ�9λ
sbit  bBus1SendFinish		= byBus1State1^2;				//������1������ɱ�־
sbit  bBus1Disable			= byBus1State1^3;				//������1��ֹ���ͱ�־
sbit  bBus1SendBit			= byBus1State1^4;				//������1���ڷ��͵�����λ
sbit  bBus1RecBit			= byBus1State1^5; 				//������1���ڽ��յ�����λ
uint8 data byBus1RecCount = 0;								//��4λ������1���ն�ʱ�жϼ�������4λ�ǽ��յ���λ����
uint8 data byBus1RecBuf;									//����1���ջ��嵥Ԫ
uint8 data byBus1SendBuf;									//����1���ͻ��嵥Ԫ	
uint8 xdata byBus1RecData[BUS1_FREQ_SIZE];					//����1���ջ�����
uint8 xdata byBus1SendData[BUS1_FREQ_SIZE];					//����1���ͻ�����
uint8 data byBus1SendCount = 0;								//��4λ������1���Ͷ�ʱ�жϼ�������4λ�Ƿ��͵�λ����
uint8 data byBus1RecSendCount = 0;							//��4λ������1���յ����ֽڼ�������4λ������1��������ֽڼ���
uint8 data byBus1SendStopCount;								//����1ֹͣʱ�����
uint8 data byBus1RecTimeOut = 0;							//����1���ճ�ʱ����
uint8 data byBus1DisableCount = 0;							//����1��ֹ����	 
uint8 xdata byBus1TxQ[BUS1_TX_Q_ZISE];						//����1���Ͷ���							
uint8 data byBus1TxHead = 0;								//������1���Ͷ���ͷָ��
uint8 data byBus1TxTail = 0; 								//������1���Ͷ���βָ��
#define IncBus1TxPtr(addr)    {addr=(addr+1)%BUS1_TX_Q_ZISE;}
#define Bus1TxBuffLen()	      ((byBus1TxTail+BUS1_TX_Q_ZISE-byBus1TxHead)%BUS1_TX_Q_ZISE)  
sbit  Bus1RecPin	= P3^3;									//������1�������Ŷ���
sbit  Bus1SendPin	= P3^4;	   								//������1�������Ŷ���


uint8 xdata byIrDARecBuff[8], byIrDADealBuff[6];		//�������������������

uint8 xdata MoveFjNum;	//�ƶ��ֻ���
//��������   
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[]; 
extern STLocalControl 	xdata stLocalControl; 
extern uint8	xdata byUsart0SndDealBuf[];	 

extern unsigned char code ParameterData[];


bit bBus0SndBitOk=0;
//bit bBus1SndBitOk=0;

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

	//STC12C56ϵ��::M0=0,M1=0:׼˫��M0=0,M1=1:���죬M0=1,M1=0:���裬M0=1,M1=1:��©
	//����0���ͽ�����Ϊ������������ս�����Ϊ׼˫��
	//P3^2,׼˫��,P1^7,����
	P3M0 &= (~Bin(0,0,0,0,0,1,0,0));
	P3M1 &= (~Bin(0,0,0,0,0,1,0,0));
	P1M0 &= (~Bin(1,0,0,0,0,0,0,0));
	P1M1 |=   Bin(1,0,0,0,0,0,0,0);

	
	//����1���ͽ�����Ϊ��©��������ս�����Ϊ��������
	//P3^3,��������
	P3M0 |= ( Bin(0,0,0,0,1,0,0,0));
	P3M1 &= (~Bin(0,0,0,0,1,0,0,0));


	//P3^4,��©���
	P3M0 |=  (Bin(0,0,0,1,0,0,0,0));
	P3M1 |=  (Bin(0,0,0,1,0,0,0,0));


	//���ߵ�ƽ����
	Bus0SendPin = 0;
	Bus0RecPin = 1;	
	Bus1SendPin = 1;
	Bus1RecPin = 1;		 
	
	
	//��ʱ��0��ʼ��:�Զ����ط�ʽ,��ʱ93.75us   
 	TMOD &= 0xf0;   //  ;t1�������ʷ�����(����)��
	TMOD |= 0X01;	//t0����ʽ1��16λ)��ʱ��

	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	TF0 = 0;												//����жϱ�־
	AUXR &= (~T0x12);										//��ͳ12��Ƶ�ٶ�  
   	ET0 = 1;												//����ʱ��0�ж� 
	TR0 = 1;												//������ʱ��
	//������������
	byBus0SendStopCount = 240;								//�ϵ�����0��ֹ����ʱ������
	byBus1SendStopCount = 240;								//�ϵ�����1��ֹ����ʱ������
	IE0 = 0;												//����ⲿ�ж�0��־
	IT0 = 1;												//�ⲿ�ж�0Ϊ�½��ش���ģʽ	 
	IE1 = 0;												//����ⲿ�ж�1��־
	IT1 = 1;												//�ⲿ�ж�1Ϊ�½��ش���ģʽ 			
	if(Bus0RecPin)
	{	//�����������,���ж�		
		EX0 = 1;
		
	}
	else
	{	//������߲�����,�����߹��ϱ�־				
		bBus0Error = 1;
	} 
	if(Bus1RecPin)
	{	//�����������,���ж�		
		EX1 = 1;
	}
	else
	{	//������߲�����,�����߹��ϱ�־				
		bBus1Error = 1;
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
void Bus0RecInt(void) interrupt X0_INTNO
{ 	
	DisableBus0RecInt();									//��ֹ�ٴ��½����ж�
	bBus0StartRec = 1;										//������ʼλ�ؼ��
	bBus0Enable = 0;										//��ֹ���߷���							
	byBus0RecCount = 0;										//����ռĴ��� 	

	if(bBus0ReqSend ==1)
	{//������״̬
		byBus0SendStopCount = 240;
	}

}
/**********************************************************
*��������			:Bus1RecInt	
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
void Bus1RecInt(void) interrupt X1_INTNO
{ 	
	DisableBus1RecInt();									//��ֹ�ٴ��½����ж�
	bBus1StartRec = 1;										//������ʼλ�ؼ��
	bBus1Enable = 0;										//��ֹ���߷���							
	byBus1RecCount = 0;										//����ռĴ���	
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
void Timer0Int(void) interrupt T0_INTNO
{ 

	TR0=0;
	TH0=TIMER0_H;
	TL0=TIMER0_L;
	TR0=1;


//	TXD =!TXD;

	bBus0RecBit = Bus0RecPin; 
	bBus1RecBit = Bus1RecPin; 								//��������0��1��������״̬	          
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
					Bus0SendPin = 0;						//�ͷ�����	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;					//���ճ���,���ý��շ��ͼ���ֵ
				EnableBus0RecInt();
				MUT = bMUTState; 								//�ָ��������ŵĿ�������
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
							MUT = bMUTState;					//�ָ��������ŵĿ�������
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
						Bus0SendPin = 0;					//�ͷ�����
					}
					byBus0RecSendCount = 0x00;				//���ճ���,���ý��շ��ͼ���ֵ
					MUT = bMUTState;							//�ָ��������ŵĿ�������
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
	/*******************************����1���մ���***********************************/
	if(bBus1StartRec)										//�ж�����������״̬,���յ���ʼλ
	{			
		byBus1RecCount += 0x10;								//���Ӷ�ʱ�жϼ�������
		if(0x50 == (byBus1RecCount & 0xf0))					//��������ʼλ���ʱ��
		{			
			bBus1StartRec = 0;
			byBus1RecCount = 0x00;							//���¿�ʼ����			
			if(bBus1RecBit)			
			{ 	//��Ч��ʼλ																		
				if((!bBus1SendError)&&bBus1OnSendFreq)
				{	//û�з��������߷��ʹ���,����һ֡�������ڷ���,ֹͣ֡���ݷ���,�÷��ʹ����־					
					byBus1State0 &= (~BUS1_SEND_CON);
					bBus1SendError = 1;
					Bus1SendPin = 1;						//�ͷ�����	 				
				}				
				byBus1SendStopCount = 240;					
				byBus1RecSendCount = 0x00;					//���ճ������ý��շ��ͼ���ֵ
				EnableBus1RecInt();
			}
			else											//��Ч��ʼλ
			{
				bBus1OnRec = 1;								//��ʼ��������λ 											
			}
		}
	}
	else if(bBus1OnRec)
	{		
		byBus1RecCount += 0x10;								//���Ӷ�ʱ�жϼ�������	 		
		if(0xa0 == (byBus1RecCount & 0xf0))
		{
			byBus1RecCount &= 0x0f;							//�����ʱ�жϼ�������
			byBus1RecCount += 0x01;
			if(0x0a == (byBus1RecCount & 0x0f))	
			{	//�յ���10λ,����λ	  				
				bBus1OnRec = 0;								//ֹͣ���ݽ���					
				if(bBus1RecBit)
				{	//��Ч�Ľ���λ							
					if(((bit)(byBus1RecSendCount & 0xf0) == bBus1RecBit9)) 
					{	//���������
						byBus1RecTimeOut = 0;
						byBus1RecSendCount &= 0x0f;											
					}
					else 
					{	//��������ȷ					
						byBus1RecTimeOut = 230;
						byBus1RecData[byBus1RecSendCount>>4] = byBus1RecBuf;
						byBus1RecSendCount += 0x10;																					
						if((byBus1RecSendCount & 0xf0) >= BUS1_FREQ_SIZE_HI)
						{																						
							byBus1RecSendCount &= 0x0f;	
							if(!((bBus1OnSendFreq == 1)&&(bBus1ReqSend==0)))
							{	//������յ�����֡�������Լ����͵Ĳ��ñ�־						
								bBus1RecFinish = 1;																  																						
							} 																		
							byBus1RecTimeOut = 0;
							byBus1DisableCount = 10;
							bBus1Disable = 1;				//��ֹ����ʹ��							
						}	
					}				
					byBus1SendStopCount = 240;
					EnableBus1RecInt();						
				}
				else							
				{	//��Ч����λ
					bBus1Error = 1;										
					if((!bBus1SendError) && bBus1OnSendFreq)
					{	//û�з��������߷��ʹ���,����һ֡�������ڷ���,ֹͣ֡���ݷ���,�÷��ʹ����־
						byBus1State0 &= (~BUS1_SEND_CON);
						bBus1SendError = 1;							
						Bus1SendPin = 1;					//�ͷ�����
					}
					byBus1RecSendCount = 0x00;				//���ճ���,���ý��շ��ͼ���ֵ
				}
			}
			else if(0x09 == (byBus1RecCount & 0x0f))
			{	//��9λ����
				bBus1RecBit9 = bBus1RecBit;					
			}
			else 											//��Ч����λ
			{ 
				byBus1RecBuf >>= 1;
				if(bBus1RecBit)
				{	//Ϊ�ߵ�ƽ
					byBus1RecBuf |= 0x80;
				}	
			}
		}
	}	
	/*******************************����0���ʹ���***********************************/
	if((byBus0State0 & BUS0_CAN_SEND) == BUS0_CAN_SEND)
	{	//����0�������ݷ�������,������������		
		if(bBus0RecBit)
		{	//��������,���Է���
			Bus0SendPin = 1;
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
			Bus0SendPin = 0;
			byBus0SendStopCount = 240;			
		}
	}
	else if(bBus0OnSendBit)
	{ 	//������λ���ڷ���,���ȷ��͵�����ʼλ	
		byBus0SendCount += 0x10;
		if(0x50 == (byBus0SendCount & 0xf0))
		{//����һ������λ���м�
			if(bBus0SendBit == bBus0RecBit)  bBus0SndBitOk =1;
			else
			{	//����ͬ,����ʧ��								
				byBus0State0 &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				bBus0SendError = 1;					
				Bus0SendPin = 0; 
				byBus0SendStopCount = 240;
				bBus0SndBitOk =0;
			}			
		}
		
		//if(bBus0SendBit == bBus0RecBit)   
		else if(bBus0SndBitOk==1)
		{	//���͵����ݺͽ��յ�������ͬ
		//	byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//һλ���ݷ������,���ȷ��͵�����ʼλ
				bBus0SndBitOk=0;
			
				byBus0SendCount &= 0x0f;
				byBus0SendCount += 0x01;				
				if(0x09 == (byBus0SendCount & 0x0f))
				{	//���͵���9λ��
					bBus0SendBit = !(bit)(byBus0RecSendCount & 0x0f);
					Bus0SendPin = !bBus0SendBit;						
				}
				else if(0x0a == (byBus0SendCount & 0x0f))
				{	//���͵�����λ��
					bBus0SendBit = 1;
					Bus0SendPin = 0;	
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
						Bus0SendPin = 0;					
					}
					else
					{	//���͵͵�ƽ
						bBus0SendBit = 0;
						Bus0SendPin = 1;
					}
					byBus0SendBuf >>= 1;					//��������λ��λ����
				}
			}
		}
/*		else
		{	//����ͬ,����ʧ��								
			byBus0State0 &= ~BUS0_SEND_CON;
			byBus0RecSendCount &= 0xf0;
			bBus0SendError = 1;					
			Bus0SendPin = 0; 
			byBus0SendStopCount = 240;
		}*/
	} 	
	/*******************************����1���ʹ���***********************************/
	if((byBus1State0 & BUS1_CAN_SEND) == BUS1_CAN_SEND)
	{	//����0�������ݷ�������,������������		
		if(bBus1RecBit)
		{	//��������,���Է���
			Bus1SendPin = 0;
			bBus1SendBit = 0; 			
			byBus1SendCount = 0;
			byBus1State0 &= (~BUS1_CAN_SEND);
			byBus1SendBuf = byBus1SendData[byBus1RecSendCount & 0x0f];
			bBus1OnSendBit = 1;			
		}
		else
		{	//���߲�����,ֹͣ����
			byBus1State0 &= (~BUS1_SEND_CON);
			bBus1SendError = 1;			
			byBus1RecSendCount &= 0xf0;
			Bus1SendPin = 1;
			byBus1SendStopCount = 240;
		}
	}
	else if(bBus1OnSendBit)
	{ 	//������λ���ڷ���,���ȷ��͵�����ʼλ			
		if(bBus1SendBit == bBus1RecBit)   		
		{	//���͵����ݺͽ��յ�������ͬ
			byBus1SendCount += 0x10;
			if(0xa0 == (byBus1SendCount & 0xf0))
			{	//һλ���ݷ������,���ȷ��͵�����ʼλ
				byBus1SendCount &= 0x0f;
				byBus1SendCount += 0x01;				
				if(0x09 == (byBus1SendCount & 0x0f))
				{	//���͵���9λ��
					bBus1SendBit = !(bit)(byBus1RecSendCount & 0x0f);
					Bus1SendPin = bBus1SendBit;						
				}
				else if(0x0a == (byBus1SendCount & 0x0f))
				{	//���͵�����λ��
					bBus1SendBit = 1;
					Bus1SendPin = 1;	
				}
				else if(0x0b == (byBus1SendCount & 0x0f))
				{	//�Ѿ����������λ��
					bBus1OnSendBit = 0;						
					byBus1RecSendCount += 0x01;									
					if((byBus1RecSendCount & 0x0f) >= BUS1_FREQ_SIZE)
					{	//������һ֡����												
						byBus1RecSendCount &= 0xf0;			//���½�������֡�ķ��ͽ׶�
						byBus1State0 &= (~BUS1_SEND_CON);
						byBus1SendStopCount = 240;
						byBus1State1 |= BUS1_SEND_FINISH;								
						byBus1DisableCount = 10;								
					}
					else
					{  						
						byBus1SendStopCount = 10;
						bBus1ReqSend = 1;
					}
					EnableBus1RecInt();						//�ٴ�ʹ�ܽ����ж�
				}
				else
				{
					if(byBus1SendBuf & 0x01)
					{	//���͸ߵ�ƽ
						bBus1SendBit = 1;
						Bus1SendPin = 1;					
					}
					else
					{	//���͵͵�ƽ
						bBus1SendBit = 0;
						Bus1SendPin = 0;
					}
					byBus1SendBuf >>= 1;					//��������λ��λ����
				}
			}
		}
		else
		{	//����ͬ,����ʧ��								
			byBus1State0 &= ~BUS1_SEND_CON;
			byBus1RecSendCount &= 0xf0;
			bBus1SendError = 1;					
			Bus1SendPin = 1; 
			byBus1SendStopCount = 240;
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
	/*******************************����1���ƴ���***********************************/
	if(0 == (byBus1State0 & BUS1_ON_REC))
	{
		if(byBus1SendStopCount != 0)
		{
			if((--byBus1SendStopCount) == 0)
			{				
				bBus1Enable = 1;								
			}
		}		
		if(bBus1Error)
		{								
			bBus1Enable = 0;			
			if(bBus1RecBit)
			{				
				bBus1Error = 0;
				EnableBus1RecInt();
				byBus1SendStopCount = 240;
			}
		}
	}  	
	/*******************************����0��ʱ����***********************************/
	if(byBus0RecTimeOut != 0)
	{
		if(--byBus0RecTimeOut == 0)							
		{	//���ճ�ʱ��
			byBus0RecSendCount &= 0x0f;
			MUT = bMUTState;									//�ָ��������ŵĿ�������
		}
	}
	if(byBus0DisableCount != 0)
	{
		if(--byBus0DisableCount == 0)						
		{	//��ֹ��ʱ��		
			bBus0Disable = 0;
		}
	}	
	/*******************************����1��ʱ����***********************************/
	if(byBus1RecTimeOut != 0)
	{
		if(--byBus1RecTimeOut == 0)							
		{	//���ճ�ʱ��
			byBus1RecSendCount &= 0x0f;
		}
	}
	if(byBus1DisableCount != 0)
	{
		if(--byBus1DisableCount == 0)						
		{	//��ֹ��ʱ��		
			bBus1Disable = 0;
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
	/***********����1�Զ����͹���**********/	 
	if((byBus1State0 & BUS1_ON_WORK) == 0x00)	
	{	//����1û�й���				
		if(bBus1SendError)						
		{	//�����˷��ʹ���,�Զ��ط�								
			bBus1SendError = 0;				
			byBus1State0 |= BUS1_REQ_SEND;		
		}
		else									
		{	//����1�޷��ʹ���		
			if(!(bBus1SendFinish|bBus1Disable))
			{	//����0û�н�ֹʹ��,�ҷ��ͽ��������Ѿ����
				if(Bus1TxBuffLen() >= BUS1_FREQ_SIZE)
				{	//��һ֡�����������ڷ��Ͷ�����  																		
					byBus1SendData[0] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[1] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[2] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[3] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[4] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[5] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);
					byBus1SendData[6] = byBus1TxQ[byBus1TxHead];
					IncBus1TxPtr(byBus1TxHead);	
					byBus1State0 |= BUS1_REQ_SEND;						
				}
				else
				{	//û��һ֡�����������ڷ��Ͷ�������
					byBus1TxHead = byBus1TxTail = 0;
				}								
			}
		}
	}  	
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
*��������			:Bus1OutputData	
*��������        	:������1�����������ݷ��뻺����
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
uint8 Bus1OutputData(uint8* pbyData)
{
	uint8 byTemp = BUS1_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus1TxBuffLen() >= (BUS1_TX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}  
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus1TxQ[byBus1TxTail] = *pbyData++;
		IncBus1TxPtr(byBus1TxTail);
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
	else return(0);*/	
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



uint8 DirRFSerialCompare()	//���к����λ����
{
	if(stLocalControl.stEepromCfgData.byRFSerialNum1!= stLocalControl.stBusDealFreq.bySndSecAddr)
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum2!= stLocalControl.stBusDealFreq.bySndRoomAddr)
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum3!= stLocalControl.stBusDealFreq.bySndBedAddr)
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum4!= stLocalControl.stBusDealFreq.byRecSecAddr)
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum5!= stLocalControl.stBusDealFreq.byRecRoomAddr)
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum6!= stLocalControl.stBusDealFreq.byRecBedAddr)
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
*��������			:MakeCH1TimerOut	
*��������        	:����ͨ��1��ʱ����
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
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH1Timerout.byTimerOutSet = byTimerOut;
	stLocalControl.stCH1Timerout.byTimerOut = byTimerOut;
	stLocalControl.stCH1Timerout.byTimerOutCount = byTimerOutCount;	
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
		crcResult +=paraData; 
		addr++;
	}
	crcData = IapReadByte(addr);
	DisableIAP();
	
	if(crcData == crcResult) return 1;
	else return 0;
}


//����stLocalControl.stEepromCfgData�е�����У���
uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);

	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}

	return crcResult;
}

/**********************************************************
*��������			:SaveIndicationData	
*��������        	:������Ϣָʾ��������
*�������   		:pstBus0SendFreq:�����������ָ֡��
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
void SaveIndicationData(pSTBusFreq pstBus0SendFreq)
{ 	
	stLocalControl.stIndicationData.stAddr.bySndSecAddr = pstBus0SendFreq->bySndSecAddr;
	stLocalControl.stIndicationData.stAddr.bySndRoomAddr = pstBus0SendFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.stAddr.bySndBedAddr = pstBus0SendFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCallCmd = pstBus0SendFreq->byRecSecAddr; 
//	stLocalControl.stIndicationData.byCallCmd = pstBus0SendFreq->byCmd;

}
/**********************************************************
*��������			:SaveCallAddr	
*��������        	:��������ͨ������ַ
*�������   		:pstBus0SendFreq:�����������ָ֡��
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
void SaveCallAddr(pSTBusFreq pstBusFreq)
{ 	
	stLocalControl.stCallAddr.bySndSecAddr = pstBusFreq->bySndSecAddr;
	stLocalControl.stCallAddr.bySndRoomAddr = pstBusFreq->bySndRoomAddr;
	stLocalControl.stCallAddr.bySndBedAddr = pstBusFreq->bySndBedAddr;

	stLocalControl.stCallAddr.byRecSecAddr = pstBusFreq->byRecSecAddr;
	stLocalControl.stCallAddr.byRecRoomAddr = pstBusFreq->byRecRoomAddr;
	stLocalControl.stCallAddr.byRecBedAddr = pstBusFreq->byRecBedAddr;	
}
/**********************************************************
*��������			:VoiceChannelCtx	
*��������        	:����ͨ���л�������
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
void VoiceChannelCtx(void)	
{ 

	if(bChannel1Talked)
	{	//ͨ��1����ͨ��
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkedVol;
		CloseCGB();
		OpenCBD();
		XTD = 0;
		XTA = 1;
		bMUTState = MUT = 0;			
	}
	else if(bChannel1Talk)
	{ 	//ͨ��1����ͨ��
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkVol;
		CloseCGB();
		OpenCBD();
		XTD = 0;
		XTA = 1;
		bMUTState = MUT = 0;	
	} 
	else if((bSickRoomBroad|bOfficeBroad|bAllBroad))
	{	//�㲥״̬
		CCAP1H = stLocalControl.stEepromCfgData.byBroadVol;
		CloseCBD();
		OpenCGB();
		XTA = XTD = 0;
		bMUTState = MUT = 0;		
	}
	else if(bChannel0Talked)
	{	//ͨ��0����ͨ��״̬
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkedVol;
		CloseCBD();
		OpenCGB();
		XTA = 0;
		XTD = 1;
		bMUTState = MUT = 0;		
	}
	else if(bChannel0Talk)
	{	//ͨ��0����ͨ��״̬
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkVol;
		CloseCBD();
		OpenCGB();
		XTA = 0;
		XTD = 1;
		bMUTState = MUT = 0;			
	}	
	else if(bSelfBroad)
	{	//�����㲥״̬
		CloseCGB();
		CloseCBD();
		XTA = 0;
		XTD = 1;
		bMUTState = MUT = 1;
	}
	else if((bCalledRing|bWaitListen))
	{	//������������ߵȴ�����
		CCAP1H = stLocalControl.stEepromCfgData.bySelfRingVol;
		CloseCBD();
		OpenCGB();
		XTA = XTD = 0;
		bMUTState = MUT = 0;		
	}
	else if((bEnSoundNote&bNurseIn&bIndicatingOther))
	{	//����ָʾ�����ֻ��һ�ʿ��λ
		CCAP1H = stLocalControl.stEepromCfgData.byRingVol;
		CloseCBD();
		OpenCGB();
		XTA = XTD = 0;
		bMUTState = MUT = 0;		
	}
	else if(bMusicPlaying)
	{	//�������ֲ���״̬
		CCAP1H = stLocalControl.stEepromCfgData.byMusicVol;
		CloseCGB();
		OpenCBD();
		XTA = XTD = 0;
		bMUTState = MUT = 0;				
	}
	else if(bVoiceNoting)
	{	//������ʾ
		CCAP1H = stLocalControl.stEepromCfgData.byNoteVol;
		CloseCGB();
		OpenCBD();
		XTA = XTD = 0;
		bMUTState = MUT = 0;
	}
	else
	{	//û���κ�����״̬����		
		CCAP1H = 10;
		CloseCGB();
		CloseCBD();
		XTA = XTD =0;
		bMUTState =MUT = 1;		
	} 
}
/**********************************************************
*��������			:LedControl	
*��������        	:ָʾ����ʾ״̬����
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:SetLedDealState,SetLedSetState
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/ 
void LedControl(void)
{

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
	//�����Ӧ��־
	if(!(bChannel1Talk|bChannel1Talked))
	{//ͨ��1����
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
	} 	
	bBusy = bWaitAck = bWaitListen = bCalledRing = 
	bChannel0Talked = bChannel0Talk = 
	bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 
	bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
	bIndicatingOther=0;		
	MakeCH0TimerOut(0, 0);	
	VoiceChannelCtx();
	
/*	if(!(bChannel1Talk|bChannel1Talked))
	{
		if(bIndicatingOther)
		{ 
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.byRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.byBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
			stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
		else
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}	
	} */	
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
	uint8 crc_temp;

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

		EnableIAP(IAP_READ);
		readtimes=3;
		do
		{
			pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<sizeof(STEepromCfgData);i++)
			{
				if(*pbyData != IapReadByte(addr)) break;
				
				pbyData++;
				addr++;
			}	

			if(i>= sizeof(STEepromCfgData))
			{//д��ȥ���������������������ͬ

				break;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			break;	//��ͬ
		}
	}while(--writetimes);
	
savaparameterRet:
	DisableIAP();
	//�������д���������е��øú���,���¼���ʹ������
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
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

	//������ڶ����������е��øú���,���¼���ʹ������
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	
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

		if(ParaCrcCheck(IAP0_ADDR) ==1)	 break;
			

	}while(--readtimes);


	if(readtimes)
	{//��һ��У�����ȷ

		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���

			ReadParameter(IAP0_ADDR);
			return;
		}
		else	goto UseInitValue;	
	}

	else
	{//3�ζ�ȡ��ʧ�ܻ���δ��ʼ����Ӧ�ó�ʼֵ���������浽�ڲ�EEPROM��ȥ
UseInitValue:
		stLocalControl.stEepromCfgData.byInitFlag = ParameterData[0];
		stLocalControl.stEepromCfgData.bySelfSecAddr = ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr = ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr = ParameterData[3];
		stLocalControl.stEepromCfgData.byEnable1 = ParameterData[4];
		stLocalControl.stEepromCfgData.byEnable2 = ParameterData[5];
		stLocalControl.stEepromCfgData.byPrio = ParameterData[6];
		stLocalControl.stEepromCfgData.byRingTime = ParameterData[7];
		stLocalControl.stEepromCfgData.byTalkTime = ParameterData[8];
		stLocalControl.stEepromCfgData.byBroadTime = ParameterData[9];	
		stLocalControl.stEepromCfgData.byListenDelay = ParameterData[10];
		stLocalControl.stEepromCfgData.byVolumStep = ParameterData[11];
		stLocalControl.stEepromCfgData.byRingVol = ParameterData[12];
		stLocalControl.stEepromCfgData.byCH0TalkedVol = ParameterData[13];
		stLocalControl.stEepromCfgData.byCH1TalkedVol = ParameterData[14];
		stLocalControl.stEepromCfgData.byBroadVol = ParameterData[15];
		stLocalControl.stEepromCfgData.byNoteVol = ParameterData[16];
		stLocalControl.stEepromCfgData.byMusicVol = ParameterData[17];
		stLocalControl.stEepromCfgData.byCH0TalkVol = ParameterData[18];
		stLocalControl.stEepromCfgData.byCH1TalkVol = ParameterData[19];
		stLocalControl.stEepromCfgData.bySelfRingVol = ParameterData[20];
		stLocalControl.stEepromCfgData.byMaxVol = ParameterData[21];
		stLocalControl.stEepromCfgData.byMinVol = ParameterData[22];
		stLocalControl.stEepromCfgData.byReserve1 = ParameterData[23];
		stLocalControl.stEepromCfgData.byReserve2 = ParameterData[24];
		stLocalControl.stEepromCfgData.byReserve3 = ParameterData[25];
		stLocalControl.stEepromCfgData.bySerialNum1= ParameterData[26];
		stLocalControl.stEepromCfgData.bySerialNum2= ParameterData[27];
		stLocalControl.stEepromCfgData.bySerialNum3= ParameterData[28];
		stLocalControl.stEepromCfgData.byVersionHi = ParameterData[29];
		stLocalControl.stEepromCfgData.byVersionLo= ParameterData[30];
		stLocalControl.stEepromCfgData.byRFSerialNum1= ParameterData[31];
		stLocalControl.stEepromCfgData.byRFSerialNum2= ParameterData[32];	
		stLocalControl.stEepromCfgData.byRFSerialNum3= ParameterData[33];
		stLocalControl.stEepromCfgData.byRFSerialNum4= ParameterData[34];
		stLocalControl.stEepromCfgData.byRFSerialNum5= ParameterData[35];
		stLocalControl.stEepromCfgData.byRFSerialNum6= ParameterData[36];
		stLocalControl.stEepromCfgData.byBedFjFlag = ParameterData[37];

		byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
		byEnable2 = stLocalControl.stEepromCfgData.byEnable2;

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
*������	      		:����ͬ
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

//���½���������
#if DEBUG==1
//   Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
#endif
/////////////////////////////////////////////////////////////////////////////////////



/*	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//����ǵǼ�״̬,�յ�������ǵǼ�ȷ������,��������
		return;
	}
*/
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//ȷ�ϵǼ�����				
			if(bLanding)
			{	
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//���͵�����,�ر�ָʾ����ʾ,ȡ��ϵͳ״̬	
					bLanding=0;
					byDevState1 = stLocalControl.stBusDealFreq.bySndSecAddr & 0x80;				
					MakeCH0TimerOut(0, 0); 
					if(stLocalControl.stBusDealFreq.bySndSecAddr & 0x40)	//PC����
					{
						stLocalControl.stBusDealFreq.byCmd = CMD_PC_OPEN;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}					
				}
			}			
			break;
		case CMD_QUEST:										//��ѯ����
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_QUEST_ANSWER; 
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;
				stLocalControl.stBusDealFreq.byRecRoomAddr = byDevState1;	//�豸״̬��Ϣ1
				stLocalControl.stBusDealFreq.byRecBedAddr = byDevState2;	//�豸״̬��Ϣ2
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
/*		case CMD_NURSE_IN:							//��ʿ��λ����	 		
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
			{	//��ȷ�ϵ��豸�Ǳ�����,������ʿ���ﱾ��,�õ�λ��־ 	
				bNurseIn = 1; 
				VoiceChannelCtx();  							
			}
			break;*/
/*		case CMD_NURSE_OUT:							//��ʿ�뿪����
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
			{	//������ʿ�뿪����,�嵽λ��־
				bNurseIn = 0;					
				LedControl();
				VoiceChannelCtx(); 				
			}
			break;*/					
		case CMD_COMM_CALL:	 								//��ͨ��������
			if(!bBusy)
			{	//ͨ��0����				
				bBusy = 1;
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ
					bIndicatingOther = 0; 					
					LedControl();
					if(bNurseIn)
					{
						VoiceChannelCtx(); 						
					}
				}
				//�����������з���ַ
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				//���ó�ʱ5s				
				MakeCH0TimerOut(250, 0);
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{	//���б����豸
					if(!(bChannel1Talk|bChannel1Talked))
					{	//ͨ��1����,�򱾻�Ϊ����״̬,����Ӧ������	
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_COMM_ANSWER;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					}				
				}
			}
			break;
		case CMD_COMM_ANSWER: 								//��ͨӦ������			
			bBusy = 1; 
			//�������峬ʱ			
			MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byRingTime);
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//Ӧ�𱾻�,��ȴ�Ӧ��,����ȴ�����״̬
				bWaitAck = 0;				
				bWaitListen = 1;  										
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//���ͨ��1æ,���߱�������ͨ��������������,���̳�ʱʱ��				
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();
				LedControl(); 				
			}
			break;
		case CMD_CALL_LISTEN:  								//��������				
			bBusy = 1;
			//����ͨ����ʱ
			MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//��������,��ȴ�����,����ͨ��0����ͨ��״̬ 			
				bWaitListen = 0; 			
				bChannel0Talk = 1;								
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//���ͨ��1æ,���߱�������ͨ��������������,���̳�ʱʱ��					
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();
				LedControl();
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;			
		case CMD_BROADCAST1:								//�յ������㲥����
		case CMD_BROADCAST2:								//�յ��칫���㲥����
		case CMD_BROADCAST3: 								//�յ�ȫ���㲥����
			if(!bBusy)
			{	//ͨ��0����				
				bBusy = 1;
				//�ݴ�����				
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;				
				//���ù㲥��ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime); 
				//�����������з���ַ				
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ	
					bIndicatingOther = 0;				
					LedControl(); 
					if(bNurseIn)
					{ 
						VoiceChannelCtx();					
					}
				}			
				if((bChannel1Talk|bChannel1Talked))
				{	//ͨ��1æ,��������	
					break;
				}			
				switch(stLocalControl.stBusDealFreq.byRecSecAddr)
				{	//��Ծ�������,�������Ƿ�������Ӧ�Ĺ㲥
					case CMD_BROADCAST1:
						bSickRoomBroad = bEnSickRoomBroad;
						if(bSickRoomBroad) Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						break;
					case CMD_BROADCAST2:
						bOfficeBroad = bEnOfficeBroad;
						if(bOfficeBroad) Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						break;
					case CMD_BROADCAST3:
						bAllBroad = bEnAllBroad; 
						if(bAllBroad) Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						break;
				}
				VoiceChannelCtx();
				LedControl();
			}
			break;			
		case CMD_INFUSION_ENTER:								//��Һ����ȷ������
		case CMD_SERVICE_ENTER:									//�������ȷ������
		case CMD_HELP_ENTER:									//��Ԯ����ȷ������
		case CMD_EMERGENCY_ENTER:								//��������ȷ������			
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//ȷ�ϱ����ĺ�������
				switch(stLocalControl.stBusDealFreq.byCmd)
				{
					case CMD_INFUSION_ENTER:
						bConfusionNoting = 1;  						
						break;
					case CMD_SERVICE_ENTER:
						bServiceNoting = 1;								
						break;
					case CMD_HELP_ENTER:
						bHelpNoting = 1;  						
						break;
					case CMD_EMERGENCY_ENTER:
						bEmergencyNoting = 1; 						
						break;
				} 			
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,��ָʾ�����ֻ� 	 					
					bIndicatingOther = 0;
					if(bNurseIn)
					{						
						VoiceChannelCtx();							
					}			
				}  				
				LedControl();
				//�����ԭ��������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break; 			
		case CMD_INFO_INDICATION:								//�յ�����ָʾ���� 
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.bySndRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
			{	//����յ����Ǳ����ĺ���ָʾ,����Ҫ������,����ϴεĺ���ָʾ	
				if(bIndicatingOther)
				{	
					bIndicatingOther = 0;
					if(bNurseIn)
					{						 	
						VoiceChannelCtx();
						LedControl();
					}
				}
				if(stLocalControl.stBusDealFreq.bySndBedAddr==stLocalControl.stEepromCfgData.bySelfBedAddr)
				{//�Ǳ���ͷ�ֻ�,��������ʾ
					//�����ԭ��������
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					bIndicatingOther =1;

					if(stLocalControl.stEepromCfgData.byBedFjFlag & BedVoicePlay)
					{
						CloseCBD();
						OpenCGB();		//��SD ͨ���ϵı������Ա�����
						XTA = XTD = 0;
						bMUTState = MUT = 0;
					}

				}
			}
			else
			{	//�յ��Ĳ��Ǳ����ĺ���ָʾ,�ж��Ƿ񱾻��ܹ�����
				if(!(bBusy|bConfusionNoting|bServiceNoting|bHelpNoting|bEmergencyNoting))
				{	//����ͨ��0���ڿ���״̬,����ָʾ�����ֻ��ĺ���������
					switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
					{
						case CMD_INFUSION_CALL:	   				//��Һ����
							if(!bEnInfusionDeal)
							{
								return;													
							} 						
							break;
						case CMD_SERVICE_CALL:					//�������
							if(!bEnServiceDeal)
							{
								return;							
							}						
							break;
						case CMD_HELP_CALL:						//��Ԯ����
							if(!bEnHelpDeal)
							{
								return;							
							} 						
							break;
						case CMD_EMERGENCY_CALL: 				//��������
							if(!bEnEmergencyDeal)
							{
								return;						
							} 						
							break;
						default:
							return;
					}
//					SaveIndicationData(&(stLocalControl.stBusDealFreq));												
//					bIndicatingOther = 1;
					if((bChannel1Talked|bChannel1Talk))
					{	//���ͨ��1����ͨ��״̬,��������
						return;
					}						
					if(bNurseIn)
					{	//��ʿ��λ,ָʾ�����ֻ�
						SaveIndicationData(&(stLocalControl.stBusDealFreq));												
						bIndicatingOther = 1;					
						VoiceChannelCtx();
						LedControl();	
						//�����ԭ��������
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));						
					}	
				}
			} 

/*//����ʱ��
			CloseCBD();
			OpenCGB();
			XTA = XTD = 0;
			bMUTState = MUT = 0;	
*/
			break;
		case CMD_INFUSION_ANSWER:							//������Һ��������
		case CMD_SERVICE_ANSWER:							//��������������
		case CMD_EMERGENCY_ANSWER:							//���������������
		case CMD_HELP_ANSWER:								//������Ԯ��������
			if(!bBusy)
			{	//ͨ��0��æ				
				bBusy = 1;	
				//�����������з���ַ	
				SaveCallAddr(&(stLocalControl.stBusDealFreq));	
				//����ͨ��0��ʱ					
				MakeCH0TimerOut(250, 0);
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//�������ĺ�������,����Ӧ�ı�־ 	
					if(stLocalControl.stBusDealFreq.byCmd == CMD_INFUSION_ANSWER)
					{  					
						bConfusionNoting = 0;
					}
					else if(stLocalControl.stBusDealFreq.byCmd == CMD_SERVICE_ANSWER)
					{  						 
						bServiceNoting = 0;
					}
					else if(stLocalControl.stBusDealFreq.byCmd == CMD_EMERGENCY_ANSWER)
					{  					
						bEmergencyNoting = 0;
					}
					else if(stLocalControl.stBusDealFreq.byCmd == CMD_HELP_ANSWER)
					{ 						  
						bHelpNoting = 0;
					} 					
					VoiceChannelCtx(); 				
					LedControl();
					bCalledRing = 1;
					if(!(bChannel1Talk|bChannel1Talked))
					{	//���ͨ��1������ͨ��״̬,����Ӧ������
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_COMM_ANSWER;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					}
					break;				
				}
				if(bIndicatingOther)
				{	//����ָʾ�����ֻ�,�����־	 					
					bIndicatingOther = 0;
					if(bNurseIn)
					{						
						VoiceChannelCtx();						
						LedControl(); 
						//�����ԭ��������
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

					}			
				} 
			}
			break;
		case CMD_STOP_INDICATION:							//ָֹͣʾ����
			if(bIndicatingOther)
			{
				bIndicatingOther = 0;
				if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
					((stLocalControl.stBusDealFreq.bySndRoomAddr) == (stLocalControl.stEepromCfgData.bySelfRoomAddr))&&
					(stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				{//�Ǳ���
					CloseCGB();
					CloseCBD();
					XTA = XTD =0;
					bMUTState =MUT = 1;						
				}
				else
				{
					if(bNurseIn)
					{
						VoiceChannelCtx();
						LedControl(); 					
					}
				}
				//�����ԭ��������
				//Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			//����������1����	
			
			}
			break;
		case CMD_INFUSION_CLEAR: 							//�����Һ��������
		case CMD_SERVICE_CLEAR: 							//��������������
		case CMD_HELP_CLEAR: 								//�����Ԯ��������
		case CMD_EMERGENCY_CLEAR: 							//���������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//��������ĺ���,�����Ӧ�ı�־
				switch(stLocalControl.stBusDealFreq.byCmd-0x0a)	//��ѯ��Ӧ�ĺ�������
				{
					case CMD_INFUSION_CALL:
						bConfusionNoting = 0;
						break;
					case CMD_SERVICE_CALL:
						bServiceNoting = 0;
						break;
					case CMD_HELP_CALL:
						bHelpNoting = 0;
						break;
					case CMD_EMERGENCY_CALL:
						bEmergencyNoting = 0;
						break;
				}
				SysReset();
				//VoiceChannelCtx();
				//LedControl();								
			}
			else
			{	//������ָ�����Ӧ�ĺ�������	
				//stLocalControl.stBusDealFreq.byCmd -= 0x0a;	
				if(bIndicatingOther &&
					(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
					(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
					(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
					((stLocalControl.stBusDealFreq.byCmd-0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
				{ 	//�����������ָʾ�ĺ�����Ϣ 					
					bIndicatingOther = 0;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));


				
					if(bNurseIn)
					{
						VoiceChannelCtx();
						LedControl(); 				
					}									
				}
			}
			break; 			
		case CMD_SYSTERM_RESET:								//ϵͳ��λ����
			SysReset();
			break; 

		case CMD_SYSTERM_RESTART:
			bBusy = bWaitAck = bWaitListen = bCalledRing = 
			bChannel0Talked = bChannel0Talk = 
			bChannel1Talked = bChannel1Talk = 
			bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
			bIndicatingOther=0;		
			MakeCH0TimerOut(0, 0);
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			break;
	
		case CMD_START_VOICE:								//������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					stLocalControl.stBusDealFreq.byRecSecAddr = CMD_START_VOICE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				bChannel1Talked = bChannel1Talk = 0; 				
				MakeCH1TimerOut(0, 0);				
				bVoiceNoting = bEnVoiceNote;				
				VoiceChannelCtx();
				LedControl();
			}
			break;
		case CMD_MUSIC_PLAY:								//�������ֲ���
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					stLocalControl.stBusDealFreq.byRecSecAddr = CMD_MUSIC_PLAY;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}			
				bChannel1Talked = bChannel1Talk = 0;
				MakeCH1TimerOut(0, 0);  			
				bMusicPlaying = bEnMusicplay;	 				
				VoiceChannelCtx();
				LedControl();					
			}
			break;	
		case CMD_STOP_VOICE:								//ֹͣ�������ֲ���
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					stLocalControl.stBusDealFreq.byRecSecAddr = CMD_STOP_VOICE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}		
				bVoiceNoting = bMusicPlaying = 0;
				VoiceChannelCtx();					
			}
			break; 
		case CMD_CHANNEL_CHANGE: 							//ͨ���л�����
			//��ͨ��0��״̬�л���ͨ��1��,ͬʱ���ͨ��0��ͨ��״̬
			bBusy = bWaitAck = bWaitListen = bCalledRing =  
				bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;			
			MakeCH0TimerOut(0, 0);					 				
			if((bChannel0Talked|bChannel0Talk))
			{ 				
				bChannel1Talked = bChannel0Talked;
				bChannel1Talk = bChannel0Talk;
				bChannel0Talked = bChannel0Talk = 0; 				
				//����ͨ��1ͨ����ʱ						
				MakeCH1TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
				if(bChannel1Talk&&(!bDealKeyDown))
				{	//��������ͨ��������������,���̳�ʱʱ�� 	 									
					MakeCH1TimerOut(5, 0);
					break; 			
				}
				VoiceChannelCtx();
				LedControl(); 
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}							
			break;
		case CMD_CHANNEL_CLOSE: 							//�ر�ͨ���л�����
			//���ͨ��1����״̬			
			//bChannel1Talked = bChannel1Talk = bConfusionNoting =0; 
			bChannel1Talked = bChannel1Talk  =0;
			MakeCH1TimerOut(0, 0);	
			VoiceChannelCtx();
			LedControl();
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		case CMD_NUMBER_SET:								//���߱෿�ţ���������
/*			if(stLocalControl.stBusDealFreq.byRecSecAddr != stLocalControl.stEepromCfgData.bySerialNum1)
			{
				return;
			}
			if(stLocalControl.stBusDealFreq.byRecRoomAddr != stLocalControl.stEepromCfgData.bySerialNum2)
			{
				return;
			}
			if(stLocalControl.stBusDealFreq.byRecBedAddr != stLocalControl.stEepromCfgData.bySerialNum3)
			{
				return;
			}
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			
			//���к���ȷ,�����µĵ�ַ����						
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;					

			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stBusDealFreq.byRecRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stBusDealFreq.byRecBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SET_BY_SERIAL;			
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));*/

			if((stLocalControl.stBusDealFreq.bySndRoomAddr >=250)||
			   (stLocalControl.stBusDealFreq.bySndBedAddr ==0x00) )
			{//���Ǵ�ͷ�ֻ��ĵ�ַ��Χ
				break;
			}


			//�ݴ���������ַ������
			stLocalControl.stCallAddr.bySndSecAddr=stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stCallAddr.bySndRoomAddr=stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stCallAddr.bySndBedAddr =stLocalControl.stBusDealFreq.bySndBedAddr;
			bNumSeting=1;
			stLocalControl.byNumSetTime=250;			//���5������Ч
			stLocalControl.stBusDealFreq.byRecSecAddr=0x01;	//��ʼ���
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//��ʾ��ʼ���
			break;
			
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			
			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr 	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr 	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//������ʾ���ֱ�����ģ��
			
//			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			break;	
			
		case CMD_POWER_ON:	  									//��������������  	
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//�Ƚϵ�ַ(���㲥��ַ)
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{//���Ե�ַ
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//ԭ����
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else
				{//�㲥��ַ
					if((bOxSupplyState==1)/*&&(bEnableOxTimer==1)*/)
					{//��������ڹ�����ʱ״̬
					
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					OSWait(K_TMO,200);
					stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
				
			}				
			break;

			
/*		case CMD_OPEN_485BUS_IN:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;	
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_485BUS_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				KDR=0;
			}
			break;*/
			
		/*case CMD_CLOSE_485BUS:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(stLocalControl.stBusDealFreq.bySndSecAddr==CMD_SB_DISPLAY_DATA)
				{//�Ƿ��͵���ͷ�ֻ��ֱ��ϵ�����
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
			}			
			//�ر�����
			KDR=1;
			break;*/
		case CMD_OPEN_LCD:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//�Ƚϵ�ַ(���㲥��ַ)

				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{//���Ե�ַ
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//ԭ����
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}				
			}
			break;
			
		case CMD_CLOSE_LCD:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//�Ƚϵ�ַ(���㲥��ַ)

				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{//���Ե�ַ
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//ԭ����
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}				
			}
			break;
			
		case CMD_CLEAR_LCD:						
		case CMD_CLEAR_HANDLE_LCD:   
			if(stLocalControl.stEepromCfgData.byBedFjFlag & bSecRoom)	//���ж����š�����
			{
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{//�Ƚϵ�ַ(���㲥��ַ)			
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					
					if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
					{//���Ե�ַ
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//ԭ����
						stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					
				}
			}
			else if(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr)	//�������
			{
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;			
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//ԭ����
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
			}
			break;
		case CMD_DATE_SEND:
			bEnableOxTimer=1;	//��ʼ������ͳ��
			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.stTime.bySec = BcdToHex(stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f);
			stLocalControl.stTime.byMin = BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f);
			stLocalControl.stTime.byHour = BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f);
			stLocalControl.stTime.byDay = BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f);
			stLocalControl.stTime.byMonth = BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f);
			stLocalControl.stTime.byYear = BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);

			if(bWillSndOxSupplyStart==1)
			{//�ϵ�(����û�յ�ʱ������֮ǰ)�ʹ��ڹ���״̬
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
				stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_START;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				bWillSndOxSupplyStart=0;
				bOxSupplyState=1;
			}
			break;
			
		case CMD_RF_NUMSET_OK:		//����Һ�����������к��ѱ�����ռ��
			if(DirRFSerialCompare())
			{

				memset(&(stLocalControl.stEepromCfgData.byRFSerialNum1),0x00,6);
				stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
				
				WDT_CONTR = 0x3d; 	//ι��
				EA=0;
				SaveParameter(IAP0_ADDR);
				EA=1;
				stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
				stLocalControl.stBusDealFreq.bySndRoomAddr=0x00;
				stLocalControl.stBusDealFreq.bySndBedAddr =0x00;
				stLocalControl.stBusDealFreq.byCmd= CMD_TELL_RF_NUM;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
				stLocalControl.stBusDealFreq.byRecRoomAddr=0x00;
				stLocalControl.stBusDealFreq.byRecBedAddr =0x00;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		case CMD_BUS_ANSWER:
	   		if(( stLocalControl.stBusDealFreq.bySndSecAddr ==stLocalControl.stEepromCfgData.bySelfSecAddr)&&
				(stLocalControl.stBusDealFreq.bySndRoomAddr==stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
				(stLocalControl.stBusDealFreq.bySndBedAddr ==stLocalControl.stEepromCfgData.bySelfBedAddr))
	   		{//�Ǳ���Ӧ���ź�
	   			if(stLocalControl.uiWaitAckTimeSupplyOxEnd)
   				{
   					if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_SUPPLY_OX_END)
					{
						stLocalControl.uiWaitAckTimeSupplyOxEnd =0;	//ֹͣ��ʱ
						//bWillSndTotalOx =0;
					}
   				}
	   			if(stLocalControl.uiWaitAckTimeSupplyOxTotal)
   				{
   					if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_SUPPLY_OX_TOTAL)
					{
						stLocalControl.uiWaitAckTimeSupplyOxTotal =0;	//ֹͣ��ʱ
						//bWillSndTotalOx =0;
					}
   				}
				
	   		}
			break;
		case CMD_PC_OPEN:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(bWillSndTotalOx)
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				}
			}
			break;
		case CMD_PC_CLOSE:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;			
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_BUS0_CHECK:									//�ⲿ���߲�������
			MUS=0;	//SD��SAͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS0_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	
			
/*		case CMD_SD_MS_TEST_START:	//SDͨ�������Ӳ��Կ�ʼ
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_MS_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=0;
			OpenCGB();
			MUT =0;	//�򿪹���34119
			break;

		case CMD_SD_SM_TEST_START:	//SDͨ���ӵ������Կ�ʼ
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_SM_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=1;
			CloseCGB();
			MUT =1;	//�رչ���34119
			break;
			
		case CMD_SD_TEST_END:	//SDͨ�����Խ���
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TEST_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=0;
			CloseCGB();
			MUT =1;	//�رչ���34119
			break;

		case CMD_SA_MS_TEST_START:	//SAͨ�������Ӳ��Կ�ʼ
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_MS_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			OpenCBD();
			MUT =0;	//�򿪹���34119
			break;

		case CMD_SA_SM_TEST_START:	//SAͨ���ӵ������Կ�ʼ
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_SM_TEST_START;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 1;
			CloseCBD();
			MUT =1;	//�رչ���34119
			break;
			
		case CMD_SA_TEST_END:	//SAͨ�����Խ���
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TEST_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			CloseCBD();
			MUT =1;	//�رչ���34119
			break;
*/	
		case CMD_SD_TAL_VOL_CHECK:
			MUS=0;	//SDͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=1;
			OpenCGB();
			MUT =0;	//�򿪹���34119
			break;
		case CMD_SD_TAL_VOL_CHECK_END:
			MUS=0;	//SDͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;

		case CMD_SA_TAL_VOL_CHECK:
			MUS=0;	//SDͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 1;
			OpenCBD();
			MUT =0;	//�򿪹���34119
			break;

		case CMD_SA_TAL_VOL_CHECK_END:
			MUS=0;	//SDͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			CloseCBD();
			MUT =1;	//�رչ���34119
			break;			
		case CMD_RS485_BRT_SET:
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;	

		case CMD_RS485_INFO_SEC_ROOM:
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;	
			
		case CMD_BED_VOICE_PLAY:
			if(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				stLocalControl.stEepromCfgData.byBedFjFlag |= BedVoicePlay;
			}
			else 
			{
				stLocalControl.stEepromCfgData.byBedFjFlag &= ~BedVoicePlay;
			}
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			break;

		case CMD_DISP_MODE:
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			 			
			break;

		case CMD_COLOR_CLEAR:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_COLOR_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
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
	//������
#if DEBUG==1	
//    Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
#endif
	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ
				MakeCH0TimerOut(150, 0);				
			}			
			break;
		case CMD_INFUSION_CALL:
		case CMD_SERVICE_CALL:
		case CMD_EMERGENCY_CALL:
		case CMD_HELP_CALL:
			//�����ԭ�����
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_COMM_CALL:	 								//��ͨ�������� 							
		case CMD_INFUSION_ANSWER:							//������Һ����
		case CMD_SERVICE_ANSWER:							//����������
		case CMD_EMERGENCY_ANSWER:							//�����������
		case CMD_HELP_ANSWER:								//������Ԯ����
			//ֹͣ����ָʾ�ĺ���
			if(bIndicatingOther)
			{	
				bIndicatingOther = 0; 
				if(bNurseIn)
				{ 				
					VoiceChannelCtx();
					LedControl();				
				}			
			}
			if(!bBusy)
			{	//��æ,����ȴ�Ӧ��״̬,�������з���ַ,���ó�ʱ
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq)); 				
				MakeCH0TimerOut(250, 0); 						
			}				
			break;	
		case CMD_COMM_ANSWER:  								//��ͨӦ������
			if(bBusy)
			{

				bCalledRing = 1;  
				if((bChannel1Talk|bChannel1Talked))
				{	//�����Ѿ�����ͨ��״̬��,���̳�ʱ,�˳�						
					MakeCH0TimerOut(5, 0);
					break;
				} 
				
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byRingTime); 				
				//VoiceChannelCtx();
				LedControl();
				if(bEnAutoListen)
				{	//�Զ�����
					OSWait(K_TMO,50);	//�˴���ʱ1000ms,�Ա���������Ī���ղ���CMD_CALL_LISTEN����
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_CALL_LISTEN;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					
				}		 					
			}
			break;
		case CMD_CALL_LISTEN:								//��������
			if(bBusy)
			{
				bCalledRing = 0;
				bChannel0Talked = 1;							
				if((bChannel1Talk|bChannel1Talked))
				{ 	//�����Ѿ�����ͨ��״̬��,���̳�ʱ,�˳� 						
					MakeCH0TimerOut(5, 0);
					break;
				}
				//����ͨ����ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);					 						
				VoiceChannelCtx();
				LedControl();
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   										//�㲥����
			//ֹͣ����ָʾ�ĺ���
			if(bIndicatingOther)
			{	
				bIndicatingOther = 0;
				if(bNurseIn)
				{
					VoiceChannelCtx();
					LedControl(); 					
				}
			}
			if(!bBusy)
			{ 
				bBusy = 1;	
				//�����������з���ַ(������ַ)		  	
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				bSelfBroad = 1;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//�����㲥������������,���̳�ʱ,�˳�				
					MakeCH0TimerOut(5, 0);
					break;
				}
				//���ù㲥��ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);
				VoiceChannelCtx();
				LedControl();
			}
			break;
		case CMD_INFUSION_CLEAR:							//�����Һ����
		case CMD_SERVICE_CLEAR:								//����������
		case CMD_HELP_CLEAR:								//�����Ԯ����
		case CMD_EMERGENCY_CLEAR:							//�����������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//����Լ��ĺ���,�����Ӧ��־

				if(stLocalControl.stBusDealFreq.byCmd == CMD_INFUSION_CLEAR)
				{				
					bConfusionNoting = 0;
				}
				else if(stLocalControl.stBusDealFreq.byCmd == CMD_SERVICE_CLEAR)
				{
					bServiceNoting = 0;
				}
				else if(stLocalControl.stBusDealFreq.byCmd == CMD_HELP_CLEAR)
				{
					bHelpNoting = 0;
				}
				else
				{
					bEmergencyNoting = 0;
				} 
				SysReset();
				//VoiceChannelCtx();
				//LedControl();					
			}
			else if(bIndicatingOther &&
				(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))//��Ӧ�ĺ�������
			{	//�����������ָʾ�ĺ���  					
				bIndicatingOther = 0;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				if(bNurseIn)
				{
					VoiceChannelCtx();
					LedControl();					
				}									
			} 			
			break; 	

		case CMD_CHANNEL_CLOSE:								//�ر�����ͨ������
			bChannel1Talk = bChannel1Talked = 0; 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			LedControl();
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	
		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 1;								//�������߹���
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			Bus0SendPin = 0;								//�ͷ�����
			MUT = bMUTState; 				
			break;
		case CMD_SYSTERM_RESET:	 							//ϵͳ��λ����
			SysReset();
			break; 
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case CMD_NUMBER_SET:
				//����ռ����������
					stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;			
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					break;
				case CMD_POWER_ON:
					if((bOxSupplyState==1)/*&&(bEnableOxTimer==1)*/)
					{//��������ڹ�����ʱ״̬
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					OSWait(K_TMO,200);
					stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					break; 
					
/*				case CMD_OPEN_485BUS_IN:
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					break;					
*/	

				case CMD_SD_TAL_VOL_CHECK_END:
					XTD=0;
					CloseCGB();
					MUT =1; //�رչ���34119
					break;

				case CMD_SD_TAL_VOL_CHECK:
//					LED_DEAL = bLedDealState=1;	//����
					break;

				case CMD_SA_TAL_VOL_CHECK:
					LED_DEAL = bLedDealState=1;	//����
					break;
			}
			break;
		case CMD_SUPPLY_OX_TOTAL:
			bWillSndTotalOx=0;
			break;
			
		default:
			break;			

	}	
}


/**********************************************************
*��������			:BusManage	
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
void BusManage(void)
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
		if(bBus1RecFinish)									//����1�յ�����
		{ 			
			OSQPost(byMainCmdQ, BUS1_REC);				
		}
		if(bBus1SendFinish)									//����1����������֡
		{
			OSQPost(byMainCmdQ, BUS1_SND);			
		}		
		OSWait(K_TMO, 1);	
		WDT_CONTR = 0x3d;		//@20MHz,1.25s
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
/*void Bus0Manage(void)
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
 */
/**********************************************************
*��������			:Bus1RecDeal	
*��������        	:������1�յ�һ֡���ݴ�����,�ú�������
					 ȡ���յ�������,���ÿ������ִ�ж�Ӧ�Ŀ�
					 �ƶ���
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
void Bus1RecDeal(void)
{
	//ȡ��������1�յ�������֡	
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
	bBus1RecFinish = 0;
	OS_EXIT_CRITICAL();
#if DEBUG==1	
//	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);
#endif

 
//	if(bLanding) return;	//�ϵ�״̬
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING:  									//�Ǽ�����	
			if(bLanding == 0)
			{//�Ѿ��ϵ�Ǽǳɹ�		
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byBedFjFlag;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break; 	


		case CALL_KEY_DOWN:
			if(bLanding == 0)
			{//�Ѿ��ϵ�Ǽǳɹ�
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;				
				if(!(bChannel1Talk|bChannel1Talked|bConfusionNoting|bServiceNoting|
					bChannel0Talk|bChannel0Talked|bCalledRing))		
				{	//�豸����״̬
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;
					stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CALL;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
				else if((bConfusionNoting&&bEnHandDown))
				{	//�豸���ں�����,�������ֱ��ҶϺ���		
					stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
				
				else if((bServiceNoting&&bEnHandDown))
				{	//�豸���ڷ��������,�������ֱ��ҶϺ���		
					stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}				
				else if(bChannel1Talked&&bEnHandDown)
				{	//�豸����ͨ��1�б����Խ�,�������ֱ��Ҷ�						
					stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*					stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
*/
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
				}
				else if(bChannel0Talked&&bEnHandDown)
				{   //�豸����ͨ��0�б����Խ�,�������ֱ��Ҷ�
					stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
			break;

			
		case CALL_KEY_UP:
		case CALL_KEY_ALWAYS:
			break;
			
		case CMD_DATA_ERROR:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_BUS_ANSWER:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_DISPLAY_DATA /*||stLocalControl.stBusDealFreq.byRecSecAddr == CMD_SB_DISPLAY_DATA*/)
			{//ֻ�д�ͷ��Ϣ�ű�������
				//bit0=0:�ô�λ�в���
				stLocalControl.stEepromCfgData.byBedFjFlag &= 0xfe;			
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}			
			break;	


		case CMD_INFUSION_CALL:	  	//����Һ�����������ߴ��������ź�
			if(bLanding == 0)
			{//�Ѿ��ϵ�Ǽǳɹ�
				if(!(bChannel1Talk|bChannel1Talked|bConfusionNoting|
					bChannel0Talk|bChannel0Talked|bCalledRing))		
				{	//�豸����״̬
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	//				stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CALL;
					stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CALL;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byPrio;
					stLocalControl.stBusDealFreq.byRecRoomAddr =0x00;
					stLocalControl.stBusDealFreq.byRecBedAddr =0x00;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
			break;
	  	case CMD_RF_NUMSET_START:
			if(!(bChannel1Talk|bChannel1Talked|bConfusionNoting|
				bChannel0Talk|bChannel0Talked|bCalledRing|bRFNumSeting))	
			{//�豸����״̬	��û����Һ����������״̬
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//������ʾģ��ʹ����ʾ
				memcpy(RFSerial,&(stLocalControl.stBusDealFreq.bySndSecAddr),3);
				memcpy(&(RFSerial[3]),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);
				bRFNumSeting=1;
				stLocalControl.uiRFNumSetTime=250;			//���10������Ч				
			}
			break;
		case CMD_SUPPLY_OX_END:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.uiWaitAckTimeSupplyOxEnd=250;		//5S���յ����������źŵ�Ӧ������
			break;
			
		case CMD_SUPPLY_OX_TOTAL:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.uiWaitAckTimeSupplyOxTotal=250;		//5S���յ������ܼ��źŵ�Ӧ������
			break;	

		case CMD_BUS1_TEST_ANSWER:	//������ʾģ���ȷ������
			bBus1Answer =1;
			break;

		case CMD_BUS1_CHECK:  	   //������1�������
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS1_CHECK;
			stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break; 

		case CMD_NURSE_IN:
			stLocalControl.uiNurseInTime=NURSE_IN_TIME;				//���5����δ�յ���ʿ��λ�������Ϊ��ʿ�뿪
			if(bNurseIn==0)
			{		
				MoveFjNum = stLocalControl.stBusDealFreq.bySndBedAddr;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			}
			bNurseIn=1;	
			VoiceChannelCtx();
			break;

		case CMD_INSPECTOR_CALL:			//��ʿ�鷿����	
			stLocalControl.uiNurseInTime=NURSE_IN_TIME;
			MoveFjNum = stLocalControl.stBusDealFreq.bySndBedAddr;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		default:
			break;			
	} 	

}



void IrDATreat(void)	//���ݴ�����
{

	OS_ENTER_CRITICAL();
	memcpy(byIrDADealBuff,byIrDARecBuff,4);				//�����ݴӽ��������Ƶ�������
	memset(byIrDARecBuff,0x00,4);						//�����ջ�������0
	OS_EXIT_CRITICAL();
	if(byIrDADealBuff[3]==(uint8)(byIrDADealBuff[0]+byIrDADealBuff[1]+byIrDADealBuff[2]))
	{//У�����ȷ
		switch(byIrDADealBuff[2])
		{
		case IrDA_CMD_NURSE_IN:
			bSetKeyDown = 1;								//�������ü���״̬
			stLocalControl.uiNurseInTime=NURSE_IN_TIME;				//���5����δ�յ���ʿ��λ�������Ϊ��ʿ�뿪
			if(bNurseIn==0)
			{
				MoveFjNum = byIrDADealBuff[1];//�ƶ��ֻ���
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
				stLocalControl.stBusDealFreq.bySndBedAddr= byIrDADealBuff[1];//�ƶ��ֻ���
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
				stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			bNurseIn=1;	
			VoiceChannelCtx();
			break;
		case IrDA_CMD_INSPECTION:			//��ʿ�鷿����	
			if((stLocalControl.stEepromCfgData.byBedFjFlag & bDispMode)==0)
			{//������Ϣ��ʾģʽ
              //���鷿�����ӹ�����ʱ��ʼ�����

				if(stLocalControl.stEepromCfgData.byBedFjFlag&OX_SUPPLY_STATE) //���ڹ���״̬
				{//ֹͣ������ʱ
					stLocalControl.stEepromCfgData.byBedFjFlag &= (~OX_SUPPLY_STATE);   //��־��0
					stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
					
					SaveParameter(IAP0_ADDR);
					bWillSndOxSupplyStart=0;
						
					
					if(bEnableOxTimer==1)	//�ѽ��յ�ʱ������
					{					
						
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
						bOxSupplyState=0;
//						bWillSndOxSupplyEnd =0;
					}
//					else bWillSndOxSupplyEnd =1;
				}
				
				else//����δ����״̬
				{//��ʼ��ʱ	
					stLocalControl.stEepromCfgData.byBedFjFlag |= (OX_SUPPLY_STATE);   //��־��1
					stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
					
					WDT_CONTR = 0x3d; 	//ι��
					SaveParameter(IAP0_ADDR);
//					bWillSndOxSupplyEnd =0;
					
					if(bEnableOxTimer==1)	//�ѽ��յ�ʱ������
					{
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
						stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_START;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						bWillSndOxSupplyStart=0;
						bOxSupplyState=1;
			
					}
					else bWillSndOxSupplyStart=1;
				}



//����ִ�в鷿����
/*				stLocalControl.uiNurseInTime=NURSE_IN_TIME;
				MoveFjNum = byIrDADealBuff[1];//�ƶ��ֻ���
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
				stLocalControl.stBusDealFreq.bySndBedAddr= byIrDADealBuff[1];//�ƶ��ֻ���	
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
				stLocalControl.stBusDealFreq.byCmd = CMD_INSPECTOR_CALL;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));*/
			}
			else 
			{//ɫ��ģʽ

				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_COLOR_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		}
	}
}



void IrDANumberSet(void)	//���ݴ�����
{

	OS_ENTER_CRITICAL();
	memcpy(byIrDADealBuff,byIrDARecBuff,6);				//�����ݴӽ��������Ƶ�������
	memset(byIrDARecBuff,0x00,6);						//�����ջ�������0
	OS_EXIT_CRITICAL();

	switch(byIrDADealBuff[4])
	{
		case 0x5a:
			//�ݴ���������ַ������
			stLocalControl.stBusDealFreq.bySndSecAddr = byIrDADealBuff[1];
			stLocalControl.stBusDealFreq.bySndRoomAddr = byIrDADealBuff[2];
			stLocalControl.stBusDealFreq.bySndBedAddr = byIrDADealBuff[3];
			stLocalControl.stBusDealFreq.byCmd    =   0x5a;
				
			stLocalControl.stCallAddr.bySndSecAddr=stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stCallAddr.bySndRoomAddr=stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stCallAddr.bySndBedAddr =stLocalControl.stBusDealFreq.bySndBedAddr;
			bNumSeting=1;
			stLocalControl.byNumSetTime=250;			//���5������Ч
			stLocalControl.stBusDealFreq.byRecSecAddr=0x01;	//��ʼ���
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	//��ʾ��ʼ���
			break;
		default:
			break;
	}
}

/**********************************************************
*��������			:Bus1SendDeal	
*��������        	:������1������һ֡���ݴ�����,�ú�������
					 ȡ���յ�������,���ÿ������ִ�ж�Ӧ�Ŀ�
					 �ƶ���
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
void Bus1SendDeal(void)
{ 
	//ȡ������֡
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus1SendData, sizeof(STBusFreq)); 
	bBus1SendFinish = 0;
	OS_EXIT_CRITICAL();
	
#if DEBUG==1
//	Send_Data(byBus1SendData,7);
#endif

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
/*		case CMD_ENTER: 
			OS_ENTER_CRITICAL();
			memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.byRFSerialNum1),3);
			stLocalControl.stBusDealFreq.byCmd = CMD_TELL_RF_NUM;
			memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.byRFSerialNum4),3);

			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			OS_EXIT_CRITICAL(); 		
			break;
*/
		case CMD_POWER_ON:
			OS_ENTER_CRITICAL();
			//�ر����д򿪵��ж�
			CCAPM0 = 0x00;
			ET0 = 0;
			TR0 = 0;
			EX0 = 0;
			ISP_CONTR = 0x20; 
			break;
		case CMD_OPEN_LCD:
			//bit1=1:��ʾ�������
			stLocalControl.stEepromCfgData.byBedFjFlag |= 0x02;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//���Ե�ַ
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			else 
			{//�㲥��ַ
				Bus0SendPin = 1;								//�������߹���
				WDT_CONTR = 0x3d; 	//ι��
				SaveParameter(IAP0_ADDR);
				Bus0SendPin = 0;								//�ͷ�����						
			}
			break;
			
		case CMD_CLOSE_LCD:
			//bit1=0:��ʾ������ر�
			stLocalControl.stEepromCfgData.byBedFjFlag &= 0xfd;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//���Ե�ַ
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			else 
			{//�㲥��ַ
				Bus0SendPin = 1;								//�������߹���
				WDT_CONTR = 0x3d; 	//ι��
				SaveParameter(IAP0_ADDR);
				Bus0SendPin = 0;								//�ͷ�����						
			}
			break;

		case CMD_OPEN_485BUS_IN:
			//bit0=0:�ô�λ�в���
//			stLocalControl.stEepromCfgData.byBedFjFlag &= 0xfe;
//			SaveParameter();
			break;
			
		case CMD_CLEAR_LCD:
			//bit0=1:�ô�λ���˳�Ժ
			stLocalControl.stEepromCfgData.byBedFjFlag |= 0x01;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

			
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//���Ե�ַ
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			else 
			{//�㲥��ַ
				Bus0SendPin = 1;								//�������߹���
				WDT_CONTR = 0x3d; 	//ι��
				SaveParameter(IAP0_ADDR);
				Bus0SendPin = 0;								//�ͷ�����						
			}
			break;
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 		//Ϊ����������׼��		
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			break;
			
		case CMD_DISP_MODE:
			if(stLocalControl.stBusDealFreq.byRecSecAddr)  stLocalControl.stEepromCfgData.byBedFjFlag |= bDispMode;
			else stLocalControl.stEepromCfgData.byBedFjFlag &= ~bDispMode;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			//������ʾģʽ
			WDT_CONTR = 0x3d; 	//ι��
			EA=0;
			SaveParameter(IAP0_ADDR);
			EA=1;
			break;	

		case CMD_RS485_INFO_SEC_ROOM:
			if(stLocalControl.stBusDealFreq.byRecSecAddr)  stLocalControl.stEepromCfgData.byBedFjFlag |= bSecRoom;
			else stLocalControl.stEepromCfgData.byBedFjFlag &= ~bSecRoom;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			//�����Ƿ��ж����ŷ���
			WDT_CONTR = 0x3d; 	//ι��
			EA=0;
			SaveParameter(IAP0_ADDR);
			EA=1;
			break;			

//		case CMD_COLOR_CLEAR:
//			bColorSet=0;
//			break;
			
		default:
			break;


	}
}
/**********************************************************
*��������			:Bus1Manage	
*��������        	:������1�����߳�
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
/*void Bus1Manage(void)
{ 	
	while(TRUE)
	{		
		if(bBus1RecFinish)									//����1�յ�����
		{ 			
			OSQPost(byMainCmdQ, BUS1_REC);				
		}
		if(bBus1SendFinish)									//����1����������֡
		{
			OSQPost(byMainCmdQ, BUS1_SND);			
		} 
		OSWait(K_TMO, 1);			 		
	}
} */



/**********************************************************




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
					else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
					{	//�ȴ�Ӧ�𣬵ȴ�����������ͨ���������㲥״̬				
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}								
					else
					{
						bBusy = bCalledRing = bChannel0Talked = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
						VoiceChannelCtx();
						LedControl();						
					}
				}
				else
				{ 	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 				
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}
			}			
		}
	}
	//ͨ��1��ʱ����
	if(stLocalControl.stCH1Timerout.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stLocalControl.stCH1Timerout.byTimerOut--;
		if(stLocalControl.stCH1Timerout.byTimerOut == 0)
		{	//��ʱһ�ε��� 
			if(stLocalControl.stCH1Timerout.byTimerOutCount == 0x00)
			{ 	//���г�ʱ���
				goto TIMER1OUTDEAL;
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH1Timerout.byTimerOutCount--;
				if(stLocalControl.stCH1Timerout.byTimerOutCount == 0x00)
				{ 	//���г�ʱ���
TIMER1OUTDEAL:
					if(bChannel1Talk)
					{	//����ͨ��״̬�������л�����						
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;						
						stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*						
						stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;	
*/
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					}
					else if(bChannel1Talked)
					{	//����ͨ��״̬���Լ���λ
						bChannel1Talked = 0;  						
						VoiceChannelCtx();
						LedControl();
					}
				}
				else
				{ 	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 				
					stLocalControl.stCH1Timerout.byTimerOut = stLocalControl.stCH1Timerout.byTimerOutSet;
				}
			}			
		}
	}	
	if(stLocalControl.uiNurseInTime)
	{
		if(--stLocalControl.uiNurseInTime == 0x00)	
		{	//��⻤ʿ��λ�Ķ�ʱʱ�䵽	��Ϊ5S
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
			stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
			stLocalControl.stBusDealFreq.bySndBedAddr= MoveFjNum;//�ƶ��ֻ���
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
			stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_OUT;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			bNurseIn=0;


			if(bIndicatingOther)
			{

				if((stLocalControl.stIndicationData.stAddr.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
					((stLocalControl.stIndicationData.stAddr.bySndRoomAddr) == (stLocalControl.stEepromCfgData.bySelfRoomAddr))&&
					(stLocalControl.stIndicationData.stAddr.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				{//�Ǳ���
					
				}
				else
				{//��Ϣָʾ�ֻ����Ǳ���
					if(bServiceNoting || bConfusionNoting || bHelpNoting ||bEmergencyNoting||bChannel0Talk||bChannel1Talk||bChannel0Talked||bChannel1Talked)
					{//�ֻ�����״̬
					}

					else
					{
						bIndicatingOther = 0;				

						//VoiceChannelCtx();
						LedControl(); 

						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;	
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;

						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					
				}
	
			}

			VoiceChannelCtx();
		}
	}
	//��״̬����
	if(--stLocalControl.byLedFlashTime == 0x00)
	{
		stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;
		if((stLocalControl.byLedState & 0x0f) == LED_FLASH)
		{ 	//�����˸״̬
			bLedDealState = !bLedDealState;
			LED_DEAL = bLedDealState;
		}	
	}
	if(stLocalControl.byIrDataIntervalTimer)
	{
		if(--stLocalControl.byIrDataIntervalTimer==0x00)	//���ʱ�䵽
		{
			if(IrDataPosit==4)  
			{
				if(byIrDARecBuff[3]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]))
				{
					bFourByteRec=1;

				}
			}

			else if(IrDataPosit==6)
			{
				if(byIrDARecBuff[5]==(uint8)(byIrDARecBuff[0]+byIrDARecBuff[1]+byIrDARecBuff[2]+byIrDARecBuff[3]+byIrDARecBuff[4]))
				{

					bSixByteRec =1;
				}

			}
			
			IrDataPosit=0x00;
			
		}
	}


	if(stLocalControl.byNumSetTime)
	{
		if(--stLocalControl.byNumSetTime==0)				//���ʱ�䵽
		{
			bNumSeting=0;
			stLocalControl.stBusDealFreq.byCmd =CMD_NUMBER_SET;
			stLocalControl.stBusDealFreq.byRecSecAddr =0x00;	//�˳����
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
	}
	if(stLocalControl.uiRFNumSetTime)
	{
		if(--stLocalControl.uiRFNumSetTime==0)				//���ʱ�䵽
		{
			bRFNumSeting=0;
			stLocalControl.stBusDealFreq.byCmd =CMD_RF_NUMSET_END;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
	}
	
/*	if(stLocalControl.uiWaitAckTimeSupplyOxEnd)
	{
		if(--stLocalControl.uiWaitAckTimeSupplyOxEnd==0)
		{//������ʱ�����ĵȴ�Ӧ��ʱ�䵽
			bWillSndTotalOx =1;
		}
	}

	if(stLocalControl.uiWaitAckTimeSupplyOxTotal)
	{
		if(--stLocalControl.uiWaitAckTimeSupplyOxTotal==0)
		{//������ʱ�ܼƵĵȴ�Ӧ��ʱ�䵽
			bWillSndTotalOx =1;
		}
	}
*/

	stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	if(stLocalControl.uiBus1TestTime)
	{
		if(--stLocalControl.uiBus1TestTime==0)
		{//����1���Լ��ʱ�䵽
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS1_TEST;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			stLocalControl.uiBus1TestTime = BUS1_TEST_TIME;
			stLocalControl.byBus1AnswerTime = 250;
		}
	}

	if(stLocalControl.byBus1AnswerTime)
	{
		if(--stLocalControl.byBus1AnswerTime==0)
		{//����1����Ӧ��ʱ�䵽
			if(bBus1Answer)
			{
				bBus1Answer = 0;
			}
			else
			{
				if((bOxSupplyState==1)/*&&(bEnableOxTimer==1)*/)
				{//��������ڹ�����ʱ״̬
					memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
					stLocalControl.stBusDealFreq.byCmd= CMD_SUPPLY_OX_END;				
					memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				OSWait(K_TMO,200);			
				stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
		}
	}


	
	if(Bus0RecPin ==1)	//��������
	{
		BusLowDTime = 100;	//20ms*100=2s		
	}
	else
	{//Bus0RecPin =0
		if(BusLowDTime)
		{
			if(--BusLowDTime ==0)
			{
				MUS =1;	//�Ͽ�SD������
				Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
				OSWait(K_TMO,100);
				WDT_CONTR = 0x3d;
				if(Bus0RecPin ==0)
				{//�����й���
					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					while(Bus0RecPin==0)	//һֱ�ȴ�Ϊ��
					{
						if(bBus1RecFinish)
						{
							memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
							bBus1RecFinish = 0;	
							if(stLocalControl.stBusDealFreq.byCmd==CMD_LANDING)
							{
								stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
								stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
								stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
								stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
								stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byBedFjFlag;
								stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
								stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
								Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));							
							}
							
						}
						Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
						MUS =1;	//�Ͽ�SD������

						OSWait(K_TMO,50);	//��ʱ200ms������������
						bLedDealState = !bLedDealState;
						LED_DEAL = bLedDealState;
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
					}					
					MUS = 0;	//ʹSD��·��ͨ
					Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
					bLedDealState = 0;
					LED_DEAL = 0;

					BusLowDTime = 100;
					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x01;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				}

				
				else 
				{//���ⲿ��������
					//���߹���
					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x02;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));


				
					MUS = 0;	//ʹSD��·��ͨ
					Bus0SendPin=0;
					OSWait(K_TMO,100);
					WDT_CONTR = 0x3d;
					
					do
					{
						MUS = 0;	//ʹSD��·��ͨ
						Bus0SendPin=0;


						OSWait(K_TMO,150);	//��ʱ600ms������������
						bLedDealState = !bLedDealState;
						LED_DEAL = bLedDealState;
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
				
						
						if(bBus1RecFinish)
						{
							memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
							bBus1RecFinish = 0;	
							if(stLocalControl.stBusDealFreq.byCmd==CMD_LANDING)
							{
								stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
								stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
								stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
								stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
								stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byBedFjFlag;
								stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
								stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;			
								Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));							
							}
							
						}						
					}while(Bus0RecPin==0);	//�ȴ�SD��·һֱΪ��
					
					bLedDealState = 0;
					LED_DEAL = 0;

					stLocalControl.stBusDealFreq.byCmd = CMD_SEND_PIN_BUG;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0x03;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
Bus0BugRet:					
					MUS = 0;	//ʹSD��·��ͨ					
					BusLowDTime = 100;
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
		OSWait(K_TMO, 4);	 								//����20ms��ʱ
		WDT_CONTR = 0x3d;		//@20MHz,1.25s
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
