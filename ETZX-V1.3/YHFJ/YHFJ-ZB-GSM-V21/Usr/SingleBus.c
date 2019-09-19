/*
************************Copyright(c)************************
*	  			   ����һ�عɷ����޹�˾
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

bit  fled0					= 0;
bit  bKeyEnterDown			= 0;
bit  bKeyBroadDown			= 0;
bit  bVl0State				= 0;
bit  bNumSeting				= 0;
bit  bMainMenuSet			= 0;
bit  bSendCmdCloseLcd		= 0;
bit  bSendCmdOpenLcd		= 0;

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
uint8 data byBus0RecData[BUS0_FREQ_SIZE];					//����0���ջ�����
uint8 data byBus0SendData[BUS0_FREQ_SIZE];					//����0���ͻ�����
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
sbit  Bus0SendPin	= P3^6;									//������0�������Ŷ���
//����1��������
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
uint8 data byBus1RecData[BUS1_FREQ_SIZE];					//����1���ջ�����
uint8 data byBus1SendData[BUS1_FREQ_SIZE];					//����1���ͻ�����
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
//��������  
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[]; 
extern STLocalControl 	xdata stLocalControl; 

extern uint8	xdata byUsart0SndDealBuf[];
extern unsigned char code MyParameter[];


uint8 xdata byBus0RxQ[BUS0_RX_Q_ZISE];						//����0���ն���
uint8 xdata byBus0RxHead = 0;								//������0���ն���ͷָ��
uint8 xdata byBus0RxTail = 0; 								//������0���ն���βָ��
uint8 Bus0RecData(uint8* pbyData);



uint8 xdata byBus1RxQ[BUS1_RX_Q_ZISE];						//����1���ն���
uint8 xdata byBus1RxHead = 0;								//������1���ն���ͷָ��
uint8 xdata byBus1RxTail = 0; 								//������1���ն���βָ��


bit bBus0SndBitOk=0;


code uint8 VolValIndex[9]={0,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff };




uint8 Bus1RecData(uint8* pbyData);



uint8 LevelToValue(uint8 VoiceLevel);




void led0init(void)
{
	P5M0 |=	Bin(0,0,0,0,1,0,0,0);
	P5M1 &=	(~Bin(0,0,0,0,1,0,0,0));
	fled0=led0=0;
}

void led0flash(void)
{
	if(fled0==0)
	{
		led0=1;
		fled0=1;
	}
	else
	{
 		led0=0;
		fled0=0;
	}
}

	 
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
//     ������1
	//P3^3,��������
	P3M0 &= (~(Bin(0,0,0,0,1,0,0,0)));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);


	//P3^4,��©���	
	P3M0 |=  (Bin(0,0,0,1,0,0,0,0));
	P3M1 |=  (Bin(0,0,0,1,0,0,0,0));



//	   ������0���͹ܽ�P3.6:ǿ�������
	P3M0 |=    Bin(0,1,0,0,0,0,0,0);
	P3M1 &= (~(Bin(0,1,0,0,0,0,0,0)));


	//���ߵ�ƽ����
	Bus0SendPin = 0;
	Bus0RecPin = 1;	
	Bus1SendPin = 1;
	Bus1RecPin = 1;	

    TMOD&= 0xf0;	 //  ;t1�������ʷ�����(����)��
    TMOD|=0X01;	//t0����ʽ1��16λ)��ʱ��
	
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	TF0 = 0;												//����жϱ�־
	AUXR &= 0x7f;											//��ͳ12��Ƶ�ٶ�  
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
void Bus0RecInt(void) interrupt 0
{ 	
	DisableBus0RecInt();									//��ֹ�ٴ��½����ж�
	bBus0StartRec = 1;										//������ʼλ�ؼ��
	bBus0Enable = 0;										//��ֹ���߷���							
	byBus0RecCount = 0;										//����ռĴ��� 		
}
/**********************************************************
*��������			:Bus1RecInt	
*��������        	:�ⲿ�ж�1����,������1�����ж�
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
void Bus1RecInt(void) interrupt 2
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
*������	      		:����ͬ
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
				PW = bPWState; 								//�ָ��������ŵĿ�������
				PW2 = bPW2State;							//�ָ��������ŵĿ�������
				VL0 =bVl0State;
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
							PW = bPWState;					//�ָ��������ŵĿ�������
							PW2 = bPW2State;				//�ָ��������ŵĿ�������
							VL0 =bVl0State;
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
					PW = bPWState;							//�ָ��������ŵĿ�������
					PW2 = bPW2State;						//�ָ��������ŵĿ�������
					VL0 =bVl0State;
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
								if(TRUE==Bus1RecData(byBus1RecData))
								{//���浽������	
									//bBus1RecFinish = 1;
								}																							  																						
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
			if(bBus0SendBit == bBus0RecBit)  bBus0SndBitOk=1;
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
			//byBus0SendCount += 0x10;
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
			PW = bPWState;									//�ָ��������ŵĿ�������
			PW2 = bPW2State;								//�ָ��������ŵĿ�������
			VL0 = bVl0State;
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
*������	      		:����ͬ
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
*��������			:Bus1RecData	
*��������        	:������1���������Ͻ��յ����ݷ��뻺����
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
uint8 Bus1RecData(uint8* pbyData)
{	
	uint8 byTemp = BUS1_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(Bus1RxBuffLen() >= (BUS1_RX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		OS_EXIT_CRITICAL();
		return(FALSE);
	}	
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus1RxQ[byBus1RxTail] = *pbyData++;
		IncBus1RxPtr(byBus1RxTail);
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
	if(bChannel1Talk)
	{	//ͨ��1ͨ��״̬			
		if(bHandleDown)
		{	//����ͨ��
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;
			CloseHandleMIC();
			OpenPanelMIC();		
		}
		else
		{//ժ��ͨ��
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HUpTalkVol; 
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;		//ֻ�ô�������Ϊͨ��1ͨ������		
		bPW2State = PW2 = 0;		//��LM4871	
		CloseCGB();		
		XTD = 0;
		OpenCBD();
		XTA = 1;
		bPWState = PW = 0;				//��34018
		bVl0State=1;
	}
	else if(bChannel1Talked)
	{	//ͨ��1ͨ��״̬		
		if(bHandleDown)
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkedVol;
			CloseHandleMIC();
			OpenPanelMIC();			
		}
		else
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH1HUpTalkedVol;	
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;		//ֻ�ô�������Ϊͨ��1ͨ������		
		bPW2State = PW2 = 0;		//��LM4871			
		CloseCGB();
		XTD = 0;
		OpenCBD();	//����
		XTA = 1;	//����
		bPWState = PW = 0;				//��34018
		bVl0State=1;
	}
	else if((bSickRoomBroad|bOfficeBroad|bAllBroad))
	{	//�㲥״̬		

		CloseHandleMIC();
		ClosePanelMIC();
		
		CloseCBD();
		XTA = XTD = 0;
		OpenCGB();
		bPW2State = PW2 = 0;		//��LM4871	
		bPWState = PW = 0;				//��34018	
		bVl0State=1;
	}
	else if(bChannel0Talk)
	{	//ͨ��0����ͨ��״̬			
		if(bHandleDown)
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;
			CloseHandleMIC();
			OpenPanelMIC();			
		}
		else
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH0HUpTalkVol;
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;		//ֻ�ô�������Ϊͨ��0ͨ������		
		bPW2State = PW2 = 0;		//��LM4871		
		CloseCBD();
		XTA = 0;
		OpenCGB();
		XTD = 1;
		bPWState = PW = 0;				//��34018
		bVl0State=1;
	} 
	else if(bChannel0Talked)
	{	//ͨ��0����ͨ��״̬			
		if(bHandleDown)
		{
//			bPW2State = PW2 = 1;		
//			P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkedVol;
			CloseHandleMIC();
			OpenPanelMIC();		
		}
		else
		{
//			bPW2State = PW2 = 1;				
//			P2 = stLocalControl.stEepromCfgData.byCH0HUpTalkedVol; 	
			ClosePanelMIC();
			OpenHandleMIC();		
		}
		P2 = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;		//ֻ�ô�������Ϊͨ��0ͨ������		
		bPW2State = PW2 = 0;		//��LM4871		
		CloseCBD();
		XTA = 0;
		OpenCGB();
		XTD = 1;
		bPWState = PW = 0;				//��34018
		bVl0State=1;
	}
	else if(bSelfBroad)
	{	//�����㲥״̬
		if(bHandleDown)
		{
			CloseHandleMIC();
			OpenPanelMIC();
			bPWState = PW = 0;				//��34018			
		}
		else
		{
			ClosePanelMIC();
			OpenHandleMIC();
			bPWState = PW = 1;				//�ر�34018
		}
		CloseCGB();
		CloseCBD();
		bPW2State = PW2 = 1;		//�ر�lm7841
		XTA = 0;
		XTD = 1;
		bVl0State=0;
	} 
	else if(bCalledRing)
	{	//����������	//��Ϣָʾ

		bPW2State = PW2 = 1;		//��LM4871	
		bPWState = PW = 0;				//��34018
		P2=stLocalControl.stEepromCfgData.byHDwnSelfRingVol;
		CloseHandleMIC();
		ClosePanelMIC();

		CloseCBD();		
		XTA = XTD = 0; 	
		OpenCGB();
		bVl0State=1;
	}	
	else if(bWaitListen)
	{	//�ȴ�����	
		bPW2State = PW2 = 0;		//��LM4871
		bPWState = PW = 0;				//��34018
		P2=stLocalControl.stEepromCfgData.byHDwnSelfRingVol;		
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCBD();			
		XTA = XTD = 0; 
		OpenCGB();	
		bVl0State=1;
	}				
	else if(bIndicatingOther)
	{	//����ָʾ�����ֻ�  		
		bPW2State = PW2 = 0;		//��LM4871
		bPWState = PW = 0;				//��34018

		if( stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin)
		{
			P2=stLocalControl.stEepromCfgData.byHDwnRingVol;
		}
		else
		{
			if( (stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour) ||
				(stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin))
			{//����Start<End�������
				if(stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour)
				{//start.hour<end.hour
					if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour && stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour) ||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin)||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin))
					{//����ʱ���ڴ�������,ʹ��С����ֵ
					
						P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
					}

					else 
					{//����ʱ�䲻�ڴ�������,ʹ��ԭʼֵ
						P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;

					}				
				}
				else
				{//start.hour==end.hour
					if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour)
					{//����ʱ�䲻�ڴ�������,ʹ��ԭʼֵ
						P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
					
					}

					else
					{//stLocalControl.stTime.byHour == start.hour
						if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin)
						{//����ʱ���ڴ�������,ʹ��С����ֵ
						
							P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
						}

						else 
						{
							//����ʱ�䲻�ڴ�������,ʹ��ԭʼֵ
							P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
						}	
			
					}
				}
						
			}
			else
			{//����Start>End�������   ʱ��ο���� 
			
				if(stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour)
				{//start.hour>end.hour
					if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour || stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour) ||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin)||
						(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin))
					{//����ʱ���ڴ�������,ʹ��С����ֵ
					
						P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
					}

					else 
					{
						//����ʱ�䲻�ڴ�������,ʹ��ԭʼֵ
						P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
					}				
				}
				else
				{//start.hour==end.hour  start.min>end.min
					if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour)
					{//����ʱ���ڴ�������,ʹ��С����ֵ
					
						P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
					}
			
					else
					{//stLocalControl.stTime.byHour == start.hour
						if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin)
						{
							//����ʱ�䲻�ڴ�������,ʹ��ԭʼֵ
							P2 = stLocalControl.stEepromCfgData.byHDwnRingVol;
						}					
			
						else 
						{//����ʱ���ڴ�������,ʹ��С����ֵ
						
							P2 = LevelToValue(stLocalControl.stEepromCfgData.byMicroVoiceVal);
						}	
			
					}
				}
						
			}
		}

					
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCBD();
		XTA = XTD = 0;
		OpenCGB();
		bVl0State=1;
	} 
	else if(bVoiceNoting)
	{	//������ʾ			
		bPW2State = PW2 = 0;		//��LM4871	
		bPWState = PW = 0;				//��34018		
		P2 = stLocalControl.stEepromCfgData.byCH0HUpTalkedVol;		
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCGB();
		XTA = XTD = 0;
		OpenCBD();
		bVl0State=1;
	}	 	 
	else if(bMusicPlaying)
	{	//�������ֲ���״̬ 				
		bPW2State = PW2 = 0;		//��LM4871
		bPWState = PW = 0;				//��34018		
		P2 = stLocalControl.stEepromCfgData.byHDwnMusicVol;			
		CloseHandleMIC();
		ClosePanelMIC();		
		CloseCGB();
		XTA = XTD = 0;
		OpenCBD();	
		bVl0State=1;
	}		
	else
	{
		bPW2State = PW2 = 1;		//�ر�LM4871
		bPWState = PW = 1;				//�ر�34018
		CloseCBD();
		CloseCGB();
		CloseHandleMIC();
		ClosePanelMIC();
		XTA = XTD=0;
		bVl0State=0;
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
	//�����Ӧ��־
	if(!(bChannel1Talk|bChannel1Talked))
	{
		bKeyAnswer = bHandAnswer = 0;
	} 	
	bBusy = bWaitAck = bWaitListen = bCalledRing = bChannel0Talked = bChannel0Talk = 
		bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;		
	MakeCH0TimerOut(0, 0);	
	VoiceChannelCtx();
	if(!(bChannel1Talk|bChannel1Talked))
	{
		if(bIndicatingOther)
		{ 
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
			stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}
		else
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		}	
	} 	
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

	//����ʹ������
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

	//����ʹ������
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;	
}



void BackLightTimerTreat(pSTBusFreq pstBusFreq)
{
	
	pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	pstBusFreq->byRecSecAddr = 0xff;
	pstBusFreq->byRecRoomAddr= 0xff;
	pstBusFreq->byRecBedAddr = 0xff;
	
	
	if( (stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour<stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour) ||
		(stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin))
	{//����Start<End�������
		if(stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour)
		{//start.hour<end.hour
			if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour && stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour) ||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin)||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin))
			{//����ʱ���ڴ�������,�ر���ʾ��
				if(!bSendCmdCloseLcd)
				{
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//��ʱ1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					bSendCmdCloseLcd =1;
					bSendCmdOpenLcd  =0;
				}
			}
			else 
			{//����ʱ�䲻�ڴ�������,����ʾ��
				if(!bSendCmdOpenLcd)
				{
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//��ʱ1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =0;
					bSendCmdOpenLcd  =1;							
				}
			}				
		}
		else
		{//start.hour==end.hour
			if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour)
			{//����ʱ�䲻�ڴ�������,����ʾ��
				if(!bSendCmdOpenLcd)
				{
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//��ʱ1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =0;
					bSendCmdOpenLcd  =1;							
				}				
			}
			else
			{//stLocalControl.stTime.byHour == start.hour
				if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
				{//����ʱ���ڴ�������,�ر���ʾ��
					if(!bSendCmdCloseLcd)
					{
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//��ʱ1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =1;
						bSendCmdOpenLcd  =0;
					}
				}
				else 
				{//����ʱ�䲻�ڴ�������,����ʾ��
					if(!bSendCmdOpenLcd)
					{
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//��ʱ1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =0;
						bSendCmdOpenLcd  =1;							
					}
				}	

			}
		}
				
	}
	else
	{//����Start>End�������   ʱ��ο���� 

		if(stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour!=stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour)
		{//start.hour>end.hour
			if( (stLocalControl.stTime.byHour>stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour || stLocalControl.stTime.byHour<stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour) ||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour && stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin)||
				(stLocalControl.stTime.byHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin))
			{//����ʱ���ڴ�������,�ر���ʾ��
				if(!bSendCmdCloseLcd)
				{
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//��ʱ1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =1;
					bSendCmdOpenLcd  =0;
				}
			}
			else 
			{//����ʱ�䲻�ڴ�������,����ʾ��
				if(!bSendCmdOpenLcd)
				{
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//��ʱ1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_OPEN_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =0;
					bSendCmdOpenLcd  =1;							
				}
			}				
		}
		else
		{//start.hour==end.hour  start.min>end.min
			if(stLocalControl.stTime.byHour != stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour)
			{//����ʱ���ڴ�������,�ر���ʾ��
				if(!bSendCmdCloseLcd)
				{
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));
					OSWait(K_TMO,200);		//��ʱ1S
					
					pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					pstBusFreq->byCmd = CMD_CLOSE_LCD;
					pstBusFreq->byRecSecAddr = 0xff;
					pstBusFreq->byRecRoomAddr= 0xff;
					pstBusFreq->byRecBedAddr = 0xff;
					Bus0OutputData(&(pstBusFreq->bySndSecAddr));					
					bSendCmdCloseLcd =1;
					bSendCmdOpenLcd  =0;
				}
			}

			else
			{//stLocalControl.stTime.byHour == start.hour
				if(stLocalControl.stTime.byMin>=stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin && stLocalControl.stTime.byMin<stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin)
				{//����ʱ�䲻�ڴ�������,����ʾ��
					if(!bSendCmdOpenLcd)
					{
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//��ʱ1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_OPEN_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =0;
						bSendCmdOpenLcd  =1;							
					}
				}					

				else 
				{//����ʱ���ڴ�������,�ر���ʾ��
					if(!bSendCmdCloseLcd)
					{
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));
						OSWait(K_TMO,200);		//��ʱ1S
						
						pstBusFreq->bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						pstBusFreq->bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						pstBusFreq->bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						pstBusFreq->byCmd = CMD_CLOSE_LCD;
						pstBusFreq->byRecSecAddr = 0xff;
						pstBusFreq->byRecRoomAddr= 0xff;
						pstBusFreq->byRecBedAddr = 0xff;
						Bus0OutputData(&(pstBusFreq->bySndSecAddr));						
						bSendCmdCloseLcd =1;
						bSendCmdOpenLcd  =0;
					}
				}	

			}
		}
				
	}

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
			stLocalControl.stEepromCfgData.byInitFlag = MyParameter[0];
			stLocalControl.stEepromCfgData.bySelfSecAddr = MyParameter[1];
			stLocalControl.stEepromCfgData.bySelfRoomAddr = MyParameter[2];
			stLocalControl.stEepromCfgData.bySelfBedAddr = MyParameter[3];
			stLocalControl.stEepromCfgData.byEnable1 = MyParameter[4];
			stLocalControl.stEepromCfgData.byEnable2 = MyParameter[5];
			stLocalControl.stEepromCfgData.byPrio = MyParameter[6];
			stLocalControl.stEepromCfgData.byRingTime =MyParameter[7];
			stLocalControl.stEepromCfgData.byTalkTime = MyParameter[8];
			stLocalControl.stEepromCfgData.byBroadTime = MyParameter[9];
			stLocalControl.stEepromCfgData.byListenDelay = MyParameter[10];
			stLocalControl.stEepromCfgData.byVolumStep = MyParameter[11];
			stLocalControl.stEepromCfgData.byHDwnRingVol= MyParameter[12];
			stLocalControl.stEepromCfgData.byHUpRingVol = MyParameter[13];
			stLocalControl.stEepromCfgData.byCH0HDwnTalkedVol = MyParameter[14];
			stLocalControl.stEepromCfgData.byCH1HDwnTalkedVol = MyParameter[15];
			stLocalControl.stEepromCfgData.byCH0HUpTalkedVol = MyParameter[16];
			stLocalControl.stEepromCfgData.byCH1HUpTalkedVol = MyParameter[17];
			stLocalControl.stEepromCfgData.byHDwnBroadVol = MyParameter[18];
			stLocalControl.stEepromCfgData.byHUpBroadVol = MyParameter[19];
			stLocalControl.stEepromCfgData.byHDwnNoteVol = MyParameter[20];
			stLocalControl.stEepromCfgData.byHUpNoteVol = MyParameter[21];
			stLocalControl.stEepromCfgData.byHDwnMusicVol =MyParameter[22];
			stLocalControl.stEepromCfgData.byHUpMusicVol =MyParameter[23];
			stLocalControl.stEepromCfgData.byCH0HDwnTalkVol =MyParameter[24];
			stLocalControl.stEepromCfgData.byCH1HDwnTalkVol =MyParameter[25];
			stLocalControl.stEepromCfgData.byCH0HUpTalkVol =MyParameter[26];
			stLocalControl.stEepromCfgData.byCH1HUpTalkVol =MyParameter[27];
			stLocalControl.stEepromCfgData.byHDwnSelfRingVol =MyParameter[28];
			stLocalControl.stEepromCfgData.byHUpSelfRingVol = MyParameter[29];
			stLocalControl.stEepromCfgData.byMaxVol =MyParameter[30];
			stLocalControl.stEepromCfgData.byMinVol = MyParameter[31];
			stLocalControl.stEepromCfgData.byReserve1 =MyParameter[32];
			stLocalControl.stEepromCfgData.byReserve2 =MyParameter[33];
			stLocalControl.stEepromCfgData.byReserve3 =MyParameter[34];
			
			stLocalControl.stEepromCfgData.bySerialNum1= MyParameter[35];
			stLocalControl.stEepromCfgData.bySerialNum2= MyParameter[36];
			stLocalControl.stEepromCfgData.bySerialNum3= MyParameter[37];
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour = MyParameter[38];
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin = MyParameter[39];
			stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour = MyParameter[40];
			stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin = MyParameter[41]; 
			
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour = MyParameter[42];
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin = MyParameter[43];
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour = MyParameter[44];
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin = MyParameter[45];
			
			stLocalControl.stEepromCfgData.byMicroVoiceVal= MyParameter[46];
			stLocalControl.stEepromCfgData.byVersionHi = MyParameter[47];
			stLocalControl.stEepromCfgData.byVersionLo= MyParameter[48];

			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

						//����ʹ������
			byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
			byEnable2 = stLocalControl.stEepromCfgData.byEnable2;	
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
/*
	//ȡ���յ�������֡ 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;	
	OS_EXIT_CRITICAL();
*/
	//���½���������
//	uart_send((uint8 *)&(stLocalControl.stBusDealFreq),7);
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
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//���͵�����,ȡ��ϵͳ״̬	
					bLanding = 0; 
					stLocalControl.stBusDealFreq.bySndSecAddr &= 0x80;				
					byDevState1 |= stLocalControl.stBusDealFreq.bySndSecAddr; 
					MakeCH0TimerOut(0, 0); 															
				}
			}			
			break;
		case CMD_QUEST:										//��ѯ����
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd =  CMD_QUEST_ANSWER;  			
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				//stLocalControl.stBusDealFreq.byCmd = CMD_QUEST;			
				//Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
			}
			break;			
		case CMD_COMM_CALL:	 								//��ͨ��������
			if(!bBusy)
			{ 	//ͨ��0����			
				bBusy = 1;	
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ
					bIndicatingOther = 0;
					VoiceChannelCtx();
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
				}			
//				//�����������з���ַ
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				//���ó�ʱ5s	
				MakeCH0TimerOut(250, 0);
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{	//���б����豸 
					//�����������з���ַ
					SaveCallAddr(&(stLocalControl.stBusDealFreq));				
					bMainMenuSet =0;					
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
			{ 	//Ӧ�𱾻�,��ȴ�Ӧ��,����ȴ�����״̬	
				bWaitAck = 0;					
				bWaitListen = 1;  										
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))
				{	//���ͨ��1æ,���߱�������ͨ��������������,���̳�ʱʱ��	
					//(ͨ������:����ͨ����Ҫ�������һֱ���ڰ���״̬,��Ͳͨ��
					//��Ҫ����Ͳһֱ����ժ��״̬)			
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();	 							
			}
			break;
		case CMD_CALL_LISTEN:  								//��������			
			bBusy = 1;
			//����ͨ����ʱ
			MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);			
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{ 	//��������,��ȴ�����,����ͨ��0����ͨ��״̬				
				bWaitAck = bWaitListen = 0; 				
				bChannel0Talk = 1;	 											
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))
				{	//���ͨ��1æ,���߱�������ͨ��������������,���̳�ʱʱ��	
					//(ͨ������:����ͨ����Ҫ�������һֱ���ڰ���״̬,��Ͳͨ��
					//��Ҫ����Ͳһֱ����ժ��״̬)			
					MakeCH0TimerOut(5, 0);					
					break;
				}							
				VoiceChannelCtx();
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	 					  			
			}
			break;			
		case CMD_BROADCAST1:								//�յ������㲥����
		case CMD_BROADCAST2:								//�յ��칫���㲥����
		case CMD_BROADCAST3: 								//�յ�ȫ���㲥����
			if(!bBusy)
			{ 	//ͨ��0����				
				bBusy = 1;	
				//�ݴ�����				
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;				
				//���ù㲥��ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);				
				//�����������з���ַ	
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ
					bIndicatingOther = 0;  								
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					} 
				}							
				if((bChannel1Talk|bChannel1Talked))
				{	//ͨ��1ͨ����,�϶�û����ʾ������Ϣ��,�����κδ���	
					break;
				}							
				switch(stLocalControl.stBusDealFreq.byRecSecAddr)
				{	//��Ծ�������,�������Ƿ�������Ӧ�Ĺ㲥
					case CMD_BROADCAST1:
						bSickRoomBroad = bEnSickRoomBroad;						
						break;
					case CMD_BROADCAST2:
						bOfficeBroad = bEnOfficeBroad;						
						break;
					case CMD_BROADCAST3:
						bAllBroad = bEnAllBroad; 						
						break;
				}
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stBusDealFreq.byRecSecAddr;	
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				VoiceChannelCtx();				
			}
			break;			
		case CMD_INFO_INDICATION:							//�յ�����ָʾ���� 									
			if(!bBusy)		
			{	//ͨ��0��æ		
				switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
				{
					case CMD_INFUSION_CALL:	   					//��Һ����
						if(!bEnInfusionDeal)
						{	//����������Һ����
							return;													
						} 						
						break;
					case CMD_SERVICE_CALL:						//�������
						if(!bEnServiceDeal)
						{	//��������������
							return;							
						}						
						break;
					case CMD_HELP_CALL:							//��Ԯ����
						if(!bEnHelpDeal)
						{	//����������Ԯ����
							return;							
						} 						
						break;
					case CMD_EMERGENCY_CALL: 					//��������
						if(!bEnEmergencyDeal)
						{	//���������������
							return;						
						} 						
						break;
					default:
						return;
				}
				bMainMenuSet = 0;
				//����ָʾ����ַ 				
				SaveIndicationData(&(stLocalControl.stBusDealFreq));
				//��ָʾ��־												
				bIndicatingOther = 1;
				if((bChannel1Talked|bChannel1Talk))
				{	//���ͨ��1����ͨ��״̬,��������
					return;
				}	
				VoiceChannelCtx(); 										
				stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
//				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return; 				
			}
			break;
		case CMD_INFUSION_ANSWER:							//������Һ��������
		case CMD_SERVICE_ANSWER:							//��������������
		case CMD_EMERGENCY_ANSWER:							//���������������
		case CMD_HELP_ANSWER:								//������Ԯ��������
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
			if(!bBusy)
			{	//ͨ��0��æ			
				bBusy = 1;	
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ 	
					bIndicatingOther = 0; 					
					VoiceChannelCtx();										
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}							
				} 
				
//				//�����������з���ַ				
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				
				//����ͨ��0��ʱ				
				MakeCH0TimerOut(250, 0);
				if((bChannel1Talk|bChannel1Talked))
				{	//���ͨ��1����ͨ��״̬,���̳�ʱʱ��,��ʱ��λ			
					MakeCH0TimerOut(5, 0);
					break;
				}				
				VoiceChannelCtx();   				
			}
			break;
		case CMD_STOP_INDICATION:							//ָֹͣʾ����						
			if(bIndicatingOther)
			{
				bIndicatingOther = 0;
				VoiceChannelCtx(); 
				if(!(bChannel1Talk|bChannel1Talked))
				{				
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}					
			}
			break;
		case CMD_INFUSION_CLEAR: 							//�����Һ��������
		case CMD_SERVICE_CLEAR: 							//��������������
		case CMD_HELP_CLEAR: 								//�����Ԯ��������
		case CMD_EMERGENCY_CLEAR: 							//���������������
			if((stLocalControl.stBusDealFreq.bySndRoomAddr == MOVE_FJ)&&
			   (stLocalControl.stBusDealFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//���ƶ��ֻ���������������������Ų��� ,ֱ�ӷ���
				return;
			}	
			if((stLocalControl.stBusDealFreq.bySndRoomAddr == ADD_BED_FJ)&&
			   (stLocalControl.stBusDealFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//�ǼӴ��ֻ���������������������Ų��� ,ֱ�ӷ���
				return;
			}			
			while(FALSE==Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
			{
				WDT_CONTR = 0x3d;	//ι��
				OSWait(K_TMO,100);	//��ʱ500ms
				
			}

			if(bIndicatingOther && 
				(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//�����������ָʾ�ĺ�����Ϣ 				
				bIndicatingOther = 0;  						
				VoiceChannelCtx();				
				if(!(bChannel1Talk|bChannel1Talked))
				{				
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}							 												
			}
			break; 			
		case CMD_SYSTERM_RESET:								//ϵͳ��λ����
			if(bBusy|bChannel1Talked|bChannel1Talk)
			{//ϵͳ(ͨ��0)æ����ͨ��1æ
				SysReset();
			}
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

				if((bChannel1Talk|bChannel1Talked))
				{	//ͨ����,��ͨ����Ϣ
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}		
				bChannel1Talked = bChannel1Talk = 0; 				
				MakeCH1TimerOut(0, 0);			
				bVoiceNoting = bEnVoiceNote; 				
				VoiceChannelCtx(); 													
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
				if((bChannel1Talk|bChannel1Talked))
				{	//ͨ����,��ͨ����Ϣ
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}	
				bChannel1Talked = bChannel1Talk = 0;
				MakeCH1TimerOut(0, 0);  				
				bMusicPlaying = bEnMusicplay;					
				VoiceChannelCtx();													
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
			//��ͨ��0��״̬�л���ͨ��1��,ͬʱ���ͨ��0��״̬
			bBusy = bWaitAck = bWaitListen = bCalledRing =  
				bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;	
			MakeCH0TimerOut(0, 0);				 				
			if((bChannel0Talked||bChannel0Talk))
			{ 				
				bChannel1Talked = bChannel0Talked;
				bChannel1Talk = bChannel0Talk;
				bChannel0Talked = bChannel0Talk = 0; 
				//����ͨ��1ͨ����ʱ 							
				MakeCH1TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
				if(bChannel1Talk)
				{	//�л�ǰͨ��0��������ͨ��״̬
					if((bKeyAnswer&&(!bKeyEnterDown))||
						(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))					
					{	//��������ͨ��������������,���̳�ʱʱ��
						//(ͨ������:����ͨ����Ҫ�������һֱ���ڰ���״̬,��Ͳͨ��
						//��Ҫ����Ͳһֱ����ժ��״̬)	
						MakeCH1TimerOut(5, 0); 
						break;						
					}			  															
				} 	
				VoiceChannelCtx();			
			}					
			break;
		case CMD_CHANNEL_CLOSE: 							//�ر�ͨ���л�����
			if(bChannel1Talked||bChannel1Talk)
			{	//���ͨ��1����ͨ��״̬,������ͨ����־
				bKeyAnswer = bHandAnswer = 0;
			}
			if((bChannel1Talked|bChannel1Talk))
			{	//���ͨ��1����ͨ��״̬,��������ָʾ�µ���Ϣ,��û��,����״̬
				if(bIndicatingOther)
				{ 	//�����ָʾ,������ʾָʾ��Ϣ	
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
					stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}	
			}
			bChannel1Talked = bChannel1Talk = 0;  
			MakeCH1TimerOut(0, 0); 							
			VoiceChannelCtx(); 			
			break;

/*		case CMD_CLOSE_485BUS:  							//�ر�485���
			KDR = 1;   			
			break;
*/			
			
/*		case CMD_OPEN_485BUS_IN:
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{   KDR = 0;
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;	
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_485BUS_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
*/			

		case CMD_OPEN_LCD:									//��LCD������ʾ
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				bSendCmdCloseLcd=0;
				bSendCmdOpenLcd =0;
			}
			break;
		case CMD_CLOSE_LCD:									//�ر�LCD������ʾ
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				bSendCmdCloseLcd=0;
				bSendCmdOpenLcd =0;				
			}
			break;
		case CMD_POWER_ON:	  								//��������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{					
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
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
			break;
		case CMD_CLEAR_LCD:									//���Һ����ʾ
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;//ԭ����
				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;	
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_DATA_SEND:									//Уʱ����,У��Һ����ʾʱ��			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			stLocalControl.stTime.bySec = stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f;
			stLocalControl.stTime.byMin = stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f;
			stLocalControl.stTime.byHour = stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f;
			stLocalControl.stTime.byDay = stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f;
			stLocalControl.stTime.byMonth = stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f;
			stLocalControl.stTime.byYear = stLocalControl.stBusDealFreq.byRecBedAddr;

			
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;			

			if( stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin==stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
			{
				bSendCmdCloseLcd = 0;
				bSendCmdOpenLcd  = 0;
			}
			
			else 
			{
				BackLightTimerTreat(&(stLocalControl.stBusDealFreq));
			}
			break;
			
		case CMD_INFUSION_CALL:								//��Һ��������
		case CMD_SERVICE_CALL:								//�����������
		case CMD_EMERGENCY_CALL:							//������������
		case CMD_HELP_CALL:									//��Ԯ��������
			if((stLocalControl.stBusDealFreq.bySndRoomAddr == ADD_BED_FJ)&&
			   (stLocalControl.stBusDealFreq.bySndSecAddr!=stLocalControl.stEepromCfgData.bySelfSecAddr))
			{//�ǼӴ��ֻ��������ĺ��������Ų��� ,ֱ�ӷ���
				return;
			}

			break;

		case CMD_INFUSION_ENTER:		
		case CMD_SERVICE_ENTER:
		case CMD_EMERGENCY_ENTER:
		case CMD_HELP_ENTER:			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

	case CMD_BL_TIMER_SET:
		stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
		stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

		stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
		stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;
		SaveParameter(IAP0_ADDR);
		
		stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
		stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BL_TIMER_SET;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		//������������Ӧ��
		
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
		stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
		stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
		
		
		if( stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin==stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			OSWait(K_TMO,200);		//��ʱ1S
			
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			bSendCmdCloseLcd = 0;
			bSendCmdOpenLcd  = 0;
		}

		else 
		{
			BackLightTimerTreat(&(stLocalControl.stBusDealFreq));
		}
		break;

	case CMD_VOICE_TIMER_SET:					
		stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
		stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

		stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
		stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;

		stLocalControl.stEepromCfgData.byMicroVoiceVal = stLocalControl.stBusDealFreq.byRecBedAddr;
		SaveParameter(IAP0_ADDR);


		stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
		stLocalControl.stBusDealFreq.byRecSecAddr = CMD_VOICE_TIMER_SET;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		//������������Ӧ��
		
		break;	

	case CMD_SUPPLY_OX_START:
	case CMD_SUPPLY_OX_END:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;		
		
	case CMD_WARD_SEC_INFO_SET:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;	

	case CMD_GSM_NUM_SEARCH:
//	case CMD_GSM_NUM_SET:
//	case CMD_GSM_NUM_DEL:
	case CMD_GSM_FUNC_SEARCH:
//	case CMD_GSM_FUNC_SET:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;	
		
	case CMD_BUS_ANSWER:
		switch(stLocalControl.stBusDealFreq.byRecSecAddr)
		{
			case CMD_GSM_NUM_SET:
			case CMD_GSM_NUM_DEL:
			case CMD_GSM_FUNC_SET:
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				break;				
		}
		break;
		
	case CMD_SYSTEM_SEC_SET:
		stLocalControl.stEepromCfgData.bySelfSecAddr=	stLocalControl.stBusDealFreq.byRecSecAddr;
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

		stLocalControl.stBusDealFreq.byCmd =	CMD_GET_BUS;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;
		
	case CMD_RS485_BRT_SET:
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		break;			
	}	
}



uint8 LevelToValue(uint8 VoiceLevel)
{
	uint8 xdata VoiceValue;
	switch(VoiceLevel)
	{
		case 0:
			VoiceValue = 0x00;
			break;
		case 1:
			VoiceValue = 0x01;
			break;			
		case 2:
			VoiceValue = 0x03;
			break;
		case 3:
			VoiceValue = 0x07;
			break;
		case 4:
			VoiceValue = 0x0f;
			break;
		case 5:
			VoiceValue = 0x1f;
			break;			
		case 6:
			VoiceValue = 0x3f;
			break;
		case 7:
			VoiceValue = 0x7f;
			break;
		case 8:
			VoiceValue = 0xff;
			break;			
	}
	return(VoiceValue);
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
//	uart_send((uint8 *)&(stLocalControl.stBusDealFreq),7);

	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ	
				MakeCH0TimerOut(150, 0); 								
			}			
			break;			
		case CMD_COMM_CALL:	 								//��ͨ�������� 				
			if(!bBusy)
			{	//��æ,����ȴ�Ӧ��״̬,�������з���ַ,���ó�ʱ	
				if(bIndicatingOther)
				{	//ֹͣ����ָʾ�ĺ���
					bIndicatingOther = 0;
					VoiceChannelCtx();
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}					  			
				}	
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq)); 				
				MakeCH0TimerOut(50, 0);
				if(bHandleDown)
				{	//�����Ͳ�ǹһ�״̬,�鿴�Ƿ������������
					if(bKeyEnterDown)
					{
						bKeyAnswer = 1;
						bHandAnswer = 0;
					}
					else
					{						
						MakeCH0TimerOut(5, 0);
						break;
					}	
				}
				else
				{	//�����Ͳ��ժ��״̬,��ô������Ͳ������,����Ͳ����״̬	
					bHandAnswer = 1;
					bKeyAnswer = 0;
				}	
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))				
				{	//���ͨ��1�Ѿ�����ͨ��״̬���߱�������ͨ��������������,
					//���̳�ʱʱ��
					//(ͨ������:����ͨ����Ҫ�������һֱ���ڰ���״̬,��Ͳͨ��
					//��Ҫ����Ͳһֱ����ժ��״̬)			
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx(); 						
			}			
			break;								
		case CMD_INFUSION_ANSWER:							//������Һ����
		case CMD_SERVICE_ANSWER:							//����������
		case CMD_EMERGENCY_ANSWER:							//�����������
		case CMD_HELP_ANSWER:								//������Ԯ����	
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			bCallDealSending = 0;
			if(!bBusy)
			{	//��æ,����ȴ�Ӧ��״̬,�������з���ַ,���ó�ʱ	
				if(bIndicatingOther)
				{	//ֹͣ����ָʾ�ĺ���
					bIndicatingOther = 0;
					VoiceChannelCtx(); 							
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}	 			
				}
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				MakeCH0TimerOut(250, 0); 
				if(bHandleDown)
				{	//�����Ͳ�ǹһ�״̬,�鿴�Ƿ������������
					if(bKeyEnterDown)
					{
						bKeyAnswer = 1;
						bHandAnswer = 0;
					}
					else
					{						
						MakeCH0TimerOut(5, 0);
						break;
					}	
				}
				else
				{	//�����Ͳ��ժ��״̬,��ô������Ͳ������,����Ͳ����״̬	
					bHandAnswer = 1;
					bKeyAnswer = 0;
				}				
				if(bChannel1Talk||bChannel1Talked||(bKeyAnswer&&(!bKeyEnterDown))||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer)))
				{	//���ͨ��1�Ѿ�����ͨ��״̬���߱�������ͨ��������������,
					//���̳�ʱʱ��
					//(ͨ������:����ͨ����Ҫ�������һֱ���ڰ���״̬,��Ͳͨ��
					//��Ҫ����Ͳһֱ����ժ��״̬)				
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();						
			}			
			break;	
		case CMD_COMM_ANSWER:  								//��ͨӦ������
			if(bBusy)
			{ 			
				bCalledRing = 1; 				 				
				MakeCH0TimerOut(2, stLocalControl.stEepromCfgData.byListenDelay); 			
				if((bChannel1Talk|bChannel1Talked))
				{	//�����Ѿ�����ͨ��״̬��,���̳�ʱ,�˳�					
					MakeCH0TimerOut(5, 0);
					break;
				} 				
				VoiceChannelCtx();								
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
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);				
				if((bChannel1Talk||bChannel1Talked))
				{	//�����Ѿ�����ͨ��״̬��,���̳�ʱ,�˳� 					
					MakeCH0TimerOut(5, 0);
					break;
				}													
				VoiceChannelCtx();
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);	//������ʾģ����ʾ��Ҫ��
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 			
			}
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   								//�㲥����
			//�ݴ�����
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;
			if(!bBusy)
			{ 	
				bBusy = 1;	
				if(bIndicatingOther)
				{	//ֹͣ����ָʾ�ĺ���
					bIndicatingOther = 0;
					VoiceChannelCtx();	 			
					if(!(bChannel1Talk|bChannel1Talked))
					{				
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}	
				}
				if(bHandleDown)
				{	//�����Ͳ�ǹһ�״̬,�鿴�Ƿ������������
					if(bKeyBroadDown)
					{
						bKeyAnswer = 1;
						bHandAnswer = 0;
					}
					else
					{						
						MakeCH0TimerOut(5, 0);
						break;
					}	
				}
				else
				{	//�����Ͳ��ժ��״̬,��ô������Ͳ������,����Ͳ����״̬	
					bHandAnswer = 1;
					bKeyAnswer = 0;
				}				
			  	MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);
			  	//�����������з���ַ(������ַ)
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				bSelfBroad = 1;
				if((bChannel1Talk|bChannel1Talked))
				{	//�����㲥������������,���̳�ʱ,�˳�							
					MakeCH0TimerOut(5, 0);
					break;
				}
				if((CMD_BROADCAST1==stLocalControl.stBusDealFreq.byRecSecAddr) &&
					(((!bKeyBroadDown)&&bKeyAnswer)||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer))))
				{	//�����㲥,��Ͳ���µ�,�����㲥��������
					MakeCH0TimerOut(5, 0);
					break;
				}
				if((CMD_BROADCAST2 == stLocalControl.stBusDealFreq.byRecSecAddr) &&
					(((!bKeyBroadDown)&&bKeyAnswer)||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer))))
				{	//�칫���㲥,��Ͳ���µ�,�칫���㲥��������
					MakeCH0TimerOut(5, 0);
					break;
				}
				if((CMD_BROADCAST3 == stLocalControl.stBusDealFreq.byRecSecAddr) &&
					(((!bKeyBroadDown)&&bKeyAnswer)||
					(bHandAnswer&&bHandleDown)||(!(bKeyAnswer||bHandAnswer))))
				{	//ȫ���㲥,��Ͳ���µ�,ȫ���㲥��������
					MakeCH0TimerOut(5, 0);
					break;
				}
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stBusDealFreq.byRecSecAddr;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				VoiceChannelCtx();					
			}
			break;
		case CMD_INFUSION_CLEAR:							//�����Һ����
		case CMD_SERVICE_CLEAR:								//����������
		case CMD_HELP_CLEAR:								//�����Ԯ����
		case CMD_EMERGENCY_CLEAR:							//�����������
			bCallDealSending = 0; 
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
/*			if(stLocalControl.stBusDealFreq.byCmd== CMD_EMERGENCY_CLEAR)
			{
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				break;
			}
*/
			if(bIndicatingOther && 
				(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//�����������ָʾ�ĺ��� 			
				bIndicatingOther = 0;					
				VoiceChannelCtx();			
				stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
//				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break; 			
		case CMD_CHANNEL_CLOSE:								//�ر�����ͨ������
			if(bChannel1Talked||bChannel1Talk)
			{
				bKeyAnswer = bHandAnswer = 0;
			}
			if((bChannel1Talked|bChannel1Talk))
			{	//���ͨ��1����ͨ��״̬,��������ָʾ�µ���Ϣ,��û��,����״̬
				if(bIndicatingOther)
				{ 	//�����ָʾ,������ʾָʾ��Ϣ	
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
					stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}	
			}
			bChannel1Talk = bChannel1Talked = 0; 			 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			break;
		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 1;								//�������߹���
			SaveParameter(IAP0_ADDR);
			Bus0SendPin = 0;								//�ͷ�����
			PW = bPWState;
			PW2 = bPW2State;
			VL0 = bVl0State;			
			break;	
		case CMD_SYSTERM_RESET:	 							//ϵͳ��λ����
			SysReset();
			break; 
		case CMD_DATA_SEND:									//Уʱ����,У��Һ����ʾʱ��			
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
			
				case CMD_POWER_ON:
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
					break;
			}

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
		WDT_CONTR = 0x3d;	//ι��
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
*��������			:InitKeyCallAddr	
*��������        	:��ʼ�����е�ַ,����ַ��ʼ��Ϊ����ĵ�ַ
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
void InitKeyCallAddr(void)
{
	stLocalControl.stKeyCallAddr.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stKeyCallAddr.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stKeyCallAddr.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
	bRoomAddr = 0;
	bSetPrio = 0;
//	stLocalControl.stKeyCallAddr.byCmd = CMD_STOP_INDICATION;
//	Bus1OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr));
}
/**********************************************************
*��������			:AddKeyValue	
*��������        	:��������ֵ���뻺����
*�������   		:byKey:���̰�����ֵ
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
void AddKeyValue(uint8 byKey)
{
	uint8 byTemp;

	for(byTemp = 0; byTemp < (MAX_KEY_SIZE-1); byTemp++)
	{
		stLocalControl.byKeyValue[byTemp] = stLocalControl.byKeyValue[byTemp+1]; 
	}
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = byKey;
}
/**********************************************************
*��������			:GetKeyCallAddr	
*��������        	:��ȡ�������µĸ�����е�ַ
*�������   		:byFlag:��ȡ��ַ������
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
uint8 GetKeyCallAddr(uint8 byFlag)
{
	uint8 byTemp;

	switch(byFlag)
	{	//��ֹʹ����û�а�����Ч�ĵ�ַ��Ϣ,Ԥ�ȳ�ʼ��������ַ
		case GET_SEC_ADDR:									//��ȡ����ַ
			byTemp = stLocalControl.stEepromCfgData.bySelfSecAddr;
			break;
		case GET_ROOM_ADDR:									//��ȡ����ַ
			byTemp = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			break;
		case GET_BED_ADDR:									//��ȡ����ַ
			byTemp = stLocalControl.stEepromCfgData.bySelfBedAddr;
			break;
		default:
			return(0);
			break;
	}
	if(stLocalControl.byKeyValue[MAX_KEY_SIZE-1] == 0xff)
	{	//û����Ч����		
		return(byTemp);
	}
	byTemp = stLocalControl.byKeyValue[MAX_KEY_SIZE-1];
	stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
	if(stLocalControl.byKeyValue[MAX_KEY_SIZE-2] == 0xff)
	{		
		return(byTemp);
	}
	byTemp = byTemp + stLocalControl.byKeyValue[MAX_KEY_SIZE-2]*10;
	if(stLocalControl.byKeyValue[MAX_KEY_SIZE-3] == 0xff)
	{		
		return(byTemp);
	}
	byTemp = byTemp + stLocalControl.byKeyValue[MAX_KEY_SIZE-3] * 100;	
	return(byTemp);
}

/**********************************************************/
//��ȡ��Ӧ��ֵ�ļ���
uint8 GetVolLevel(uint8 VloValue)
{
	if((VloValue&0x01)==0) return(0);
	else if(VloValue==0x01) return(1);
	else if(VloValue==0x03) return(2);	
	else if(VloValue==0x07) return(3);
	else if(VloValue==0x0f) return(4);
	else if(VloValue==0x1f) return(5);
	else if(VloValue==0x3f) return(6);	
	else if(VloValue==0x7f) return(7);
	else if(VloValue==0xff) return(8);
	else return(8);	
}


/**********************************************************
*��������			:AddVol
*��������        	:��������
*�������   		:pbyData:�����ӵ������洢����ַ
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
void AddVol(uint8 xdata *pbyData)
{  	
	if(*pbyData < MAX_VOLUMN_VALUE)
	{
		*pbyData = ((*pbyData<<1)|0x01);
		VoiceChannelCtx();
		//�洢�ı��˵�����ֵ
//		stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
	}
}


/**********************************************************
*��������			:AddVol
*��������        	:��������
*�������   		:pbyData:�����ӵ������洢����ַ
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
void AddPlayVoiceVol(uint8 xdata *pbyData)
{  	
	if(*pbyData < stLocalControl.stEepromCfgData.byMaxVol)
	{
		*pbyData = ((*pbyData<<1)|0x01);
		VoiceChannelCtx();
		//�洢�ı��˵�����ֵ
//		stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
	}
}


/**********************************************************
*��������			:DecVol
*��������        	:������С
*�������   		:pbyData:����С�������洢����ַ
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
void DecVol(uint8 xdata *pbyData)
{  	
	if(*pbyData > MIN_VOLUMN_VALUE)
	{
		*pbyData = (*pbyData>>1);
		VoiceChannelCtx();
		//�洢�ı��˵�����ֵ
//		stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS; 			
//		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));								
	}
}
/**********************************************************
*��������			:KeyDownDeal
*��������        	:���̰������´�����
*�������   		:byKey:���̰��µļ�ֵ
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
void KeyDownDeal(uint8 byKey)
{

	byKey &= 0x1f;	
	if(byKey==KEY_ENTER)  bKeyEnterDown=1;
	else if(byKey==KEY_BROAD) bKeyBroadDown=1;

	if(bNumSeting==1)
	{
		if(byKey==KEY_ENTER)		
		{//���״̬ ȷ�ϼ�����
			bNumSeting=0;
			stLocalControl.stKeyCallAddr.byCmd = CMD_NUMBER_SET;
			stLocalControl.stKeyCallAddr.byRecSecAddr = 0x02;	//�ɹ����;
			//Bus0OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr)); 							
			Bus1OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr));
			//�����µĵ�ַ����						
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stKeyCallAddr.bySndSecAddr;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stKeyCallAddr.bySndRoomAddr;
			stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stKeyCallAddr.bySndBedAddr;					
			SaveParameter(IAP0_ADDR);
			stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
			return;
		}	
	}
	if(bIndicatingOther&&(!bHandAnswer)&&(!bKeyAnswer))
	{	//����ָʾ�����ֻ��ĺ���
		switch(byKey)
		{
			case KEY_VOICEINC:									//�������������Ͳ��������
/*				if(bHandleDown)
				{  				
					AddVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));					
				}
				else
				{
					AddVol(&(stLocalControl.stEepromCfgData.byHUpRingVol));	 					
				}*/
				AddPlayVoiceVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byHDwnRingVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_INCREACE_VOL; 	//������������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				return;
			case KEY_VOICEDEC:									//�������������Ͳ��������
/*				if(bHandleDown)
				{
					DecVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));					
				}
				else
				{
					DecVol(&(stLocalControl.stEepromCfgData.byHUpRingVol));					
				}*/
				DecVol(&(stLocalControl.stEepromCfgData.byHDwnRingVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byHDwnRingVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_DECREACE_VOL; 	//������������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;
			case KEY_ENTER:									//��������ָʾ�ĺ�����Ϣ	
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;				
				if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				}
				else
				{											
					stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
					stLocalControl.stBusDealFreq.byCmd += 0x06; 
				} 							 				
				if(TRUE == Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
				{
					bCallDealSending = 1;
				}
				InitKeyCallAddr(); 				
				return;			
			default: 										//������������,�һ�
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
				stLocalControl.stBusDealFreq.byCmd += 0x0a; 												 				
				if(TRUE == Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
				{
					bCallDealSending = 1;
				} 

			
				InitKeyCallAddr(); 
				return;				
		}
	}
	if(KEY_CLEAR == byKey)
	{		
		if((bChannel1Talk&bHandAnswer))
		{	//ȷʵ������Ͳ�������е�,���̳�ʱʱ��,��ʱ���Զ�����ͨ���л�����		
			MakeCH1TimerOut(5, 0);
		
		}	
		else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad)&&bHandAnswer)
		{	//ȷʵ������Ͳ�������е�,���̳�ʱʱ��,��ʱ���Զ����͸�λ����		
			MakeCH0TimerOut(5, 0);		
		}
		InitKeyCallAddr(); 
		return;	
	}
	if(!(bBusy|bChannel1Talked|bChannel1Talk))
	{
		switch(byKey)
		{
			case KEY_0:
				AddKeyValue(0);				
				break;
			case KEY_1:
				AddKeyValue(1);				
				break;
			case KEY_2:
				AddKeyValue(2);			
				break;
			case KEY_3:
				AddKeyValue(3);			
				break;
			case KEY_4:
				AddKeyValue(4);
				break;
			case KEY_5:
				AddKeyValue(5);
				break;
			case KEY_6:
				AddKeyValue(6);
				break;
			case KEY_7:
				AddKeyValue(7);
				break;
			case KEY_8:
				AddKeyValue(8);
				break;
			case KEY_9:
				AddKeyValue(9);
				break;
			case KEY_ROOM:
				stLocalControl.stKeyCallAddr.byRecRoomAddr = GetKeyCallAddr(GET_ROOM_ADDR);				
				bRoomAddr = 1;
				break;
			case KEY_SEC:
				stLocalControl.stKeyCallAddr.byRecSecAddr = GetKeyCallAddr(GET_SEC_ADDR); 								
				break;
			case KEY_MODE:									//��������
				AddKeyValue(21);
				//stLocalControl.stKeyCallAddr.byRecBedAddr = GetKeyCallAddr(GET_BED_ADDR);
				//bSetPrio = 1;									//�����趨�������ȼ�
				break;	
/*			case K_SICK_BROAD:									//�����㲥��
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST1;  				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				break;
			case K_OFFICE_BROAD:								//�칫���㲥��
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST2;  				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				break;*/
			case KEY_BROAD:									//ȫ���㲥��
				if(bMainMenuSet==0)
				{
				    switch(stLocalControl.byKeyValue[MAX_KEY_SIZE-1])
			    	{
			    		case 1:
							stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST1;     //  1�����͹㲥:�����㲥
			    			break;
						case 2:
							stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST2;     //  2�����͹㲥:�칫���㲥
			    			break;
						
						case 3:
						default:
							stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST3;     //  3�����͹㲥:ȫ���㲥
			    			break;						
			    	}
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0xff; 				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					stLocalControl.byKeyValue[MAX_KEY_SIZE-1]=0xff;
				}
				break;
			case KEY_ENTER:
				if(bMainMenuSet==0)
				{
					if(stLocalControl.byKeyValue[MAX_KEY_SIZE-1] !=0XFF)
					{
					//���й���
						stLocalControl.stKeyCallAddr.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;					
						stLocalControl.stKeyCallAddr.byRecBedAddr = GetKeyCallAddr(GET_BED_ADDR);
						if(stLocalControl.stKeyCallAddr.byRecBedAddr <=200)
						{
							if(!bRoomAddr)
							{	
								//stLocalControl.stKeyCallAddr.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
								stLocalControl.stKeyCallAddr.byRecRoomAddr = 0xff;							
							}	
						}
						else
						{//����200Ϊҽ���ֻ���ַ
							if(!bRoomAddr)
							{//û�����뷿��
								stLocalControl.stKeyCallAddr.byRecRoomAddr = YHFJ_ROOM_ADDR;
								stLocalControl.stKeyCallAddr.byRecBedAddr  -= 200;
							}
						}
						if(!DirAddrCompare(&(stLocalControl.stKeyCallAddr)))
						{//���Ǳ���
							stLocalControl.stKeyCallAddr.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
							stLocalControl.stKeyCallAddr.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
							stLocalControl.stKeyCallAddr.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
							stLocalControl.stKeyCallAddr.byCmd = CMD_COMM_CALL;
							Bus0OutputData(&(stLocalControl.stKeyCallAddr.bySndSecAddr));
						}	 			
					}
				}
				InitKeyCallAddr(); 			
				break; 			
			default: 			
				break;				
		}
	}
	if(bChannel0Talked||bChannel0Talk)
	{
		switch(byKey)
		{
			case KEY_VOICEINC:									//����ͨ��0�Խ�����
				AddVol(&(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_INCREACE_VOL; 	//������������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;

			case KEY_VOICEDEC:									//����ͨ��0�Խ�����
				DecVol(&(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH0HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_DECREACE_VOL; 	//������������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;			
		}
	}
	else if(bChannel1Talked||bChannel1Talk)
	{
		switch(byKey)
		{
			case KEY_VOICEINC:									//����ͨ��1�Խ�����
				AddVol(&(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_INCREACE_VOL; 	//������������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;

			case KEY_VOICEDEC:									//����ͨ��1�Խ�����
				DecVol(&(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol));
				SaveParameter(IAP0_ADDR);
				stLocalControl.stBusDealFreq.byRecSecAddr=GetVolLevel(stLocalControl.stEepromCfgData.byCH1HDwnTalkVol);
				stLocalControl.stBusDealFreq.byCmd = CMD_DECREACE_VOL; 	//������������
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				return;	

		}
	}
}
/**********************************************************
*��������			:KeyUpDeal
*��������        	:���̰�����������
*�������   		:byKey:���̰��µļ�ֵ
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
void KeyUpDeal(uint8 byKey)
{
	byKey &= 0x1f;
	if(byKey==KEY_ENTER)  bKeyEnterDown=0;		//������
	else if(byKey==KEY_BROAD) bKeyBroadDown=0;
	switch(byKey)
	{
		case KEY_ENTER: 			
			if((bChannel1Talk&bKeyAnswer))
			{	//���̳�ʱʱ�䣬��ʱ���Զ����͸�λ����
				MakeCH1TimerOut(5, 0);

			}			 	  			
			else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad)&&bKeyAnswer)
			{ 				
				MakeCH0TimerOut(5, 0);
			} 			
			break; 
		case KEY_BROAD:									//ȫ���㲥��������
			if(bSelfBroad&&bKeyAnswer)
			{	//ȷʵ���ڹ㲥״̬,����Ͳ�Ƿ��µ�
				MakeCH0TimerOut(5, 0);
			}
			break;	
	}	 	
}
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
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus1RecDeal(void)
{
/*
	//ȡ��������1�յ�������֡	
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus1RecData, sizeof(STBusFreq));	
	bBus1RecFinish = 0;
	OS_EXIT_CRITICAL();
*/
	//���½���������
//	uart_send((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////
	
	if(bLanding)
	{	//�Ǽ�״̬��������
		return;
	}

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING:  									//�Ǽ�����
			//�������ֵ
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.byHDwnRingVol;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.byCH0HDwnTalkVol;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.byCH1HDwnTalkVol;
			stLocalControl.stBusDealFreq.byCmd = CMD_ENTER;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							
			break; 		
		case CMD_RECEIVE_OK:
		case CMD_RECEIVE_ERROR:								//ת������������							
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_KEY_DOWN: 									//�յ����̰�����������	
			KeyDownDeal(stLocalControl.stBusDealFreq.bySndSecAddr);
			break;
		case CMD_KEY_UP:									//�յ����̰�����������
			KeyUpDeal(stLocalControl.stBusDealFreq.bySndSecAddr);
			break;
		case CMD_DATA_ERROR:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_BUS_ANSWER:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_MAIN_MENU_SET:
			bMainMenuSet=1;
			break;
		case CMD_MAIN_MENU_ESC:
			bMainMenuSet = 0;
			break;
		case CMD_DATA_SEND:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_NUMBER_SET:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;	

		case CMD_VOICE_TIMER_SET:					
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;

			stLocalControl.stEepromCfgData.byMicroVoiceVal = stLocalControl.stBusDealFreq.byRecBedAddr;
			SaveParameter(IAP0_ADDR);
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			
			break;

		case CMD_BL_TIMER_SET:
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin  = stLocalControl.stBusDealFreq.bySndRoomAddr;

			stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin  = stLocalControl.stBusDealFreq.byRecRoomAddr;
			
			SaveParameter(IAP0_ADDR);
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
			
			
			if( stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour==stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour && stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin==stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				OSWait(K_TMO,200);		//��ʱ1S
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_LCD;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr= 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				bSendCmdCloseLcd = 0;
				bSendCmdOpenLcd  = 0;
			}

			else
			{
				BackLightTimerTreat(&(stLocalControl.stBusDealFreq));
			}
			break;


		case CMD_VOL_VAL_SET:
			stLocalControl.stEepromCfgData.byMaxVol=VolValIndex[stLocalControl.stBusDealFreq.bySndSecAddr];
			SaveParameter(IAP0_ADDR);
			break;

		case CMD_SELF_SET_NUMBER:
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			SaveParameter(IAP0_ADDR);
			break;

		case CMD_VOICE_TIMER_GET:
			if((stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin>=0x60)||
			   (stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin>=0x60))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = 0;
				stLocalControl.stBusDealFreq.bySndRoomAddr = 0;				
				stLocalControl.stBusDealFreq.byRecSecAddr = 0;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0;

			}
			else 
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartHour;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byStartMin;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndHour;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.stVoiceTimeZone.byEndMin;	
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.byMicroVoiceVal;
			}
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_BL_TIMER_GET:
			if((stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin>=0x60)||
			   (stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour>=0x24)||
			   (stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin>=0x60))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = 0;
				stLocalControl.stBusDealFreq.bySndRoomAddr = 0;				
				stLocalControl.stBusDealFreq.byRecSecAddr = 0;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0;

			}	
			else
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byStartHour;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byStartMin;
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byEndHour;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.stBLTimeZone.byEndMin;	
			}
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;



		case CMD_VOL_VAL_GET:
			
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.byMaxVol;
			stLocalControl.stBusDealFreq.bySndRoomAddr = 0;
			stLocalControl.stBusDealFreq.byRecSecAddr = 0;
			stLocalControl.stBusDealFreq.byRecRoomAddr = 0;
			Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;

/*		case CMD_485BUS_OK:
			KDR = 1;	//�ϵ�485���߳ɹ�֮��,��ֹ485����
			break;*/

		case CMD_WARD_SEC_INFO_SET:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_GSM_FUNC_SEARCH:
		case CMD_GSM_NUM_SEARCH:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;

			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = GSM_MODULE_ADDR;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;

		case CMD_GSM_NUM_SET:
		case CMD_GSM_NUM_DEL:
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
		case CMD_GSM_FUNC_SET:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
*������	      		:����ͬ
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
	bBus1SendFinish = 0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus1SendData, sizeof(STBusFreq));	
	OS_EXIT_CRITICAL();	
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING:  									//�Ǽ�����
			break;

		case CMD_POWER_ON:
			OS_ENTER_CRITICAL();
			//�ر����д򿪵��ж�
			CCAPM0 = 0x00;
			ET0 = 0;
			TR0 = 0;
			EX0 = 0;
			ISP_CONTR = 0x20; 
			break;
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
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus1Manage(void)
{ 	
	while(TRUE)
	{	
		WDT_CONTR = 0x3d;	//ι��	
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
	//ͨ��0��ʱ���� 	
	if(stLocalControl.stCH0TimerOut.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stLocalControl.stCH0TimerOut.byTimerOut--;
		if(stLocalControl.stCH0TimerOut.byTimerOut == 0)
		{	//��ʱһ�ε��� 
			if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
			{ 	//���г�ʱ���
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
/*					stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
					stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
					Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));*/
				}
				else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
				{				
				 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bCalledRing)
				{	//��������״̬
					bCalledRing = 0;   									
					if(bEnAutoListen)
					{	//�Զ�����
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
				else
				{
					bBusy = bCalledRing = bChannel0Talked = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
					VoiceChannelCtx(); 										
				}
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH0TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH0TimerOut.byTimerOutCount == 0x00)
				{ 	//���г�ʱ���
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
/*						stLocalControl.stBusDealFreq.byCmd = CMD_POWER_ON;
						stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
						stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
*/
					}
					else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
					{				
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					else if(bCalledRing)
					{	//��������״̬
						bCalledRing = 0;   										
						if(bEnAutoListen)
						{	//�Զ�����
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
					else
					{
						bBusy = bCalledRing = bChannel0Talked = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
						VoiceChannelCtx();						
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
				if(bChannel1Talk)
				{	//����ͨ��״̬�������л�����
				
/*					stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
					stLocalControl.stBusDealFreq.bySndRoomAddr = 0x00;
					stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;	
*/	
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*					stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
					stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
					stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;	
*/
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;	
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
				}
				else if(bChannel1Talked)
				{ 	//����ͨ��״̬���Լ���λ
					bChannel1Talked = 0; 
					if(bIndicatingOther)
					{ 	//�����ָʾ,������ʾָʾ��Ϣ	
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
						stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					else
					{
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//						Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}				
					VoiceChannelCtx();					
				}	
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH1Timerout.byTimerOutCount--;
				if(stLocalControl.stCH1Timerout.byTimerOutCount == 0x00)
				{ 	//���г�ʱ���
					if(bChannel1Talk)
					{	//����ͨ��״̬�������л�����	
					
/*						stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
						stLocalControl.stBusDealFreq.bySndRoomAddr = 0x00;
						stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;
*/
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;	
/*
						stLocalControl.stBusDealFreq.byRecSecAddr = 0x00;
						stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
						stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;	
*/
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					}
					else if(bChannel1Talked)
					{	//����ͨ��״̬���Լ���λ
						bChannel1Talked = 0; 
						if(bIndicatingOther)
						{ 	//�����ָʾ,������ʾָʾ��Ϣ	
							stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
							stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
							stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
							stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.byCallCmd;
							stLocalControl.stBusDealFreq.byCmd = CMD_INFO_INDICATION;
							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						}
						else
						{
							stLocalControl.stBusDealFreq.byCmd = CMD_STOP_INDICATION;
							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
//							Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						}					
						VoiceChannelCtx(); 					
					}
				}
				else
				{ 	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 				
					stLocalControl.stCH1Timerout.byTimerOut = stLocalControl.stCH1Timerout.byTimerOutSet;
				}
			}			
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
		WDT_CONTR = 0x3d;	//ι��	
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

