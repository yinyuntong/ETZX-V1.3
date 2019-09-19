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


bit FLED;

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
uint8 data byBus0TxQ[BUS0_TX_Q_ZISE];						//����0���Ͷ���							
uint8 data byBus0TxHead = 0;								//������0���Ͷ���ͷָ��
uint8 data byBus0TxTail = 0; 								//������0���Ͷ���βָ��
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 


sbit  Bus0RecPin	= P3^3;									//������0�������Ŷ���
sbit  Bus0SendPin	= P3^4;									//������0�������Ŷ���

uint16	xdata uitTimer0TimerCount=TIMER0_COUNT; 

void led0flash(void)
{
	if(FLED==FLAG_LED_ON)
	{
		LED=LED_OFF;
		FLED=FLAG_LED_OFF;
	}
	else
	{
 		LED=LED_ON;
		FLED=FLAG_LED_ON;
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
	//STC12C54/56ϵ�УM0=0,M1=0:׼˫��M0=0,M1=1:���죬M0=1,M1=0:���裬M0=1,M1=1:��©
	//����0���ͽ�����Ϊ������������ս�����Ϊ��������:


	//STC12C52/5Aϵ��:M0=0,M1=0:׼˫��M0=0,M1=1:���裬M0=1,M1=0:���죬M0=1,M1=1:��©
	//����0���ս�����Ϊ��������:���ͽ�����Ϊ���������

	
	//P3^3,��������
	P3M0 &= (~Bin(0,0,0,0,1,0,0,0));
	P3M1 |=   Bin(0,0,0,0,1,0,0,0);

	//,P3^4,��©
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);

	//���ߵ�ƽ����
	Bus0SendPin = 1;
	Bus0RecPin = 1;	 
	
	

 	TMOD &= 0xf0;   //  ;t1�������ʷ�����(����)��
	TMOD |= 0X01;	//t0����ʽ1��16λ)��ʱ��

	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
//	TL0 = (uint8)uitTimer0TimerCount;
//	TH0 = (uint8)(uitTimer0TimerCount>>8);
	
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
			stLocalControl.stEepromCfgData.bySelfRoomAddr = 0x02;
			stLocalControl.stEepromCfgData.bySelfBedAddr = 0x03;
			
			stLocalControl.stEepromCfgData.bySerialNum1= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum2= 0xff;
			stLocalControl.stEepromCfgData.bySerialNum3= 0xff;
			stLocalControl.stEepromCfgData.uiRs485Brt  = 57600; 	//Ĭ�ϲ�����57600
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
	uint8 x,y,temp1,temp2;
	//ȡ���յ�������֡ 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	OS_EXIT_CRITICAL();	


	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//����ǵǼ�״̬,�յ�������ǵǼ�ȷ������,��������
		return;
	}
	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//ȷ�ϵǼ�����				
			if(bLanding)
			{	
				bLanding=0;
				MakeCH0TimerOut(0, 0); 
				if( (stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.bySndSecAddr)||
					(stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.bySndRoomAddr)||
					(stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.bySndBedAddr))
				{//�Ѹ����ֱ�,������ֱ���Ϣ
					Byte_Write(DIET_LENGTH_ADDR,0x00);
					Byte_Write1(DIET_LENGTH_ADDR+1,0x00);
					stLocalControl.uiDietDataLen =0x00;
					
					Byte_Write(PRESCRIPTION_LENGTH_ADDR,0x00);
					Byte_Write1(PRESCRIPTION_LENGTH_ADDR+1,0x00);
					stLocalControl.uiPrescriptionDataLen =0x00;
					
					Byte_Write(PROMPT_LENGTH_ADDR,0x00);
					Byte_Write1(PROMPT_LENGTH_ADDR+1,0x00);
					stLocalControl.uiPromptDataLen = 0x00;

					stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
					stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
					stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
					SaveParameter(IAP0_ADDR);					
				}
				
				if( (stLocalControl.stBusDealFreq.byRecSecAddr&0x01) == 0x01)
				{//bit0==1:��ʾ���˳�Ժ
					stLocalControl.byPatientDischarge =1;
					BL_SW=0;	//�ر��� 
				}
				else
				{//�˴�λ�в���
					stLocalControl.byPatientDischarge =0;
				}
				

				if( (stLocalControl.stBusDealFreq.byRecSecAddr&0x02) == 0x02)
				{//bit1==1:��ʾ��ʾ�����ش�
					bBL_SW_State=1;
					if(stLocalControl.byPatientDischarge ==0)
					{
						BL_SW=1;	//�����
					}
					else BL_SW=0;
				}
				else 
				{
					bBL_SW_State=BL_SW=0;	//����ر�
				}
				ShowLandingFace();
			}			
			break;
		case CMD_INFUSION_ENTER:								//��Һ����ȷ������
		case CMD_SERVICE_ENTER: 								//�������ȷ������
		case CMD_HELP_ENTER:									//��Ԯ����ȷ������
		case CMD_EMERGENCY_ENTER:								//��������ȷ������
			 SetLedState(LED_ON);
			 ShowCallSuccess(); 
			 stLocalControl.uiLcdDisplayOnTime = 250;	//�����ʱ��5S
			 BL_SW=1;			 
			 break;
		case CMD_INFUSION_CLEAR:								 //��Һ�����������
		case CMD_SERVICE_CLEAR:								 //��������������
		case CMD_HELP_CLEAR:									 //��Ԯ�����������
		case CMD_EMERGENCY_CLEAR:								 //���������������
			  SetLedState(LED_OFF);
			  ShowLandingFace();			 
			  break;
		case CMD_TALK_CLEAR:
			  MicCtrlState=MIC_CTRL=0;
			  SetLedState(LED_OFF);
			  ShowLandingFace();			 
			  break;
		case CMD_CALL_LISTEN:
		case CMD_CHANNEL_CHANGE:
			  SetLedState(LED_ON);
			  MicCtrlState=MIC_CTRL=1;
			  ShowTalkFace();			 
			  break;
		case CMD_CHANNEL_CLOSE:
		case CMD_STOP_INDICATION:
		case CMD_SYSTERM_RESET:
			  MicCtrlState=MIC_CTRL=0;
			  SetLedState(LED_OFF);
			  ShowLandingFace();			 
			  break;
		case CMD_DATE_SEND:
			  stLocalControl.stTime.bySecond=stLocalControl.stBusDealFreq.bySndSecAddr;
			  stLocalControl.stTime.byMinute=stLocalControl.stBusDealFreq.bySndRoomAddr;
			  stLocalControl.stTime.byHour	=stLocalControl.stBusDealFreq.bySndBedAddr;
			  stLocalControl.stTime.byDay=stLocalControl.stBusDealFreq.byRecSecAddr;
			  stLocalControl.stTime.byMonth=stLocalControl.stBusDealFreq.byRecRoomAddr;
			  stLocalControl.stTime.byYear=stLocalControl.stBusDealFreq.byRecBedAddr;
			  if(stLocalControl.byDisplayFace==LAND_FACE)
			  {
				x=0;y=32;
				temp1=stLocalControl.stTime.byYear;	//BCD��
				temp2=temp1%16; //��λ
				temp1=temp1/16; //ʮλ
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayASCIIString(&x,&y,"-",1,BLACK,CYAN,3);	

				x+=16;
				temp1=stLocalControl.stTime.byMonth;	//BCD��
				temp2=temp1%16; //��λ
				temp1=temp1/16; //ʮλ
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayASCIIString(&x,&y,"-",1,BLACK,CYAN,3);

				x+=16;
				temp1=stLocalControl.stTime.byDay;	//BCD��
				temp2=temp1%16; //��λ
				temp1=temp1/16; //ʮλ
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);

				//DisplayBlock(0,64,23,95,CYAN_CODE);
				x=24;y=64;
				temp1=stLocalControl.stTime.byHour; //BCD��
				temp2=temp1%16; //��λ
				temp1=temp1/16; //ʮλ
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayASCIIString(&x,&y,":",1,BLACK,CYAN,3);	

				x+=16;
				temp1=stLocalControl.stTime.byMinute; //BCD��
				temp2=temp1%16; //��λ
				temp1=temp1/16; //ʮλ
				DisplayDigitString(&x,&y,&temp1,1,BLACK,CYAN,3);
				DisplayDigitString(&x,&y,&temp2,1,BLACK,CYAN,3);
				//DisplayBlock(x,y,127,95,CYAN_CODE);			  	
			  }
			  break;
			 
		case CMD_OPEN_485BUS_IN:
//			if(stLocalControl.stBusDealFreq.byRecSecAddr==CMD_SB_LCD_DISPLAY_DATA)
			if((stLocalControl.stBusDealFreq.byRecSecAddr==DIET_TYPE) ||
				(stLocalControl.stBusDealFreq.byRecSecAddr==PRESCRIPTION_TYPE) ||
				(stLocalControl.stBusDealFreq.byRecSecAddr==PROMPT_INFO_TYPE) )
			{//�Ƿ�����ʾ���ݵ��ֱ� ,�ر�MIC
				MIC_CTRL=0;				
			}
			//������ʾ������
			stLocalControl.byPatientDischarge = 0;
			BL_SW=1;	//����ʾ������
			stLocalControl.uiLcdDisplayOnTime= 500;	//��10����
			break;

		case CMD_BUS_ANSWER:
			if(stLocalControl.stBusDealFreq.byRecSecAddr == CMD_DISPLAY_DATA)
			{//��ʾ���Ļ�Ӧ���ź�
				stLocalControl.byPatientDischarge = 0;
				BL_SW=1;	//����ʾ������
				stLocalControl.uiLcdDisplayOnTime= 500;	//��10����			
			}
			break;
			
		case CMD_CLOSE_485BUS:
			if(stLocalControl.stBusDealFreq.bySndSecAddr==CMD_SB_LCD_DISPLAY_DATA)
			{//�ָ�MICԭ����״̬
				MIC_CTRL=MicCtrlState;
			}
			break;			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{//���Լ��㲥
			  MicCtrlState=MIC_CTRL=1;
			  ShowBroadFace();			 
			}
			  break;
		case CMD_OPEN_LCD:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				bBL_SW_State=BL_SW=1;
			}
			else if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//�㲥��
				bBL_SW_State =1;
				if(stLocalControl.byPatientDischarge ==0 )
				{//���˻�û��Ժ
					BL_SW = 1;
				}
			}
			break;
			
		case CMD_CLOSE_LCD:
			bBL_SW_State = BL_SW =0;
			break;
			
		case CMD_CLEAR_LCD:
			stLocalControl.byPatientDischarge = 1;	//���˳�Ժ
			BL_SW=0;	//�ر���ʾ������
			//�崲ͷ�ֻ���ʾ��ʱ,Ҳ�����ֱ�����Ϣ
			Byte_Write(DIET_LENGTH_ADDR,0x00);
			Byte_Write1(DIET_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PRESCRIPTION_LENGTH_ADDR,0x00);
			Byte_Write1(PRESCRIPTION_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PROMPT_LENGTH_ADDR,0x00);
			Byte_Write1(PROMPT_LENGTH_ADDR+1,0x00);
			
			stLocalControl.uiDietDataLen =0x00;
			stLocalControl.uiPrescriptionDataLen =0x00;
			stLocalControl.uiPromptDataLen =0x00;			
			break;

		case CMD_CLEAR_HANDLE_LCD:	//����ֱ�������
			Byte_Write(DIET_LENGTH_ADDR,0x00);
			Byte_Write1(DIET_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PRESCRIPTION_LENGTH_ADDR,0x00);
			Byte_Write1(PRESCRIPTION_LENGTH_ADDR+1,0x00);
			
			Byte_Write(PROMPT_LENGTH_ADDR,0x00);
			Byte_Write1(PROMPT_LENGTH_ADDR+1,0x00);

			stLocalControl.uiDietDataLen =0x00;
			stLocalControl.uiPrescriptionDataLen =0x00;
			stLocalControl.uiPromptDataLen =0x00;
			break;
			
		case CMD_NURSE_IN:
			if((bBL_SW_State==0)||(stLocalControl.byPatientDischarge == 1))	//�����������
			{
				BL_SW=1;	//����ʾ��
			}
			break;
		case CMD_NURSE_OUT:
			if((bBL_SW_State==0)||(stLocalControl.byPatientDischarge == 1))	//�����������
			{
				BL_SW=0;	//�ر���ʾ��
			}
			break;	
		case CMD_NUMBER_SET:
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x02)		//�ɹ�	
			{
				stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
				SaveParameter(IAP0_ADDR);
				ShowLandingFace();
			}
			break;
		case CMD_SYSTEM_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			SaveParameter(IAP0_ADDR);
			break;
			
		case CMD_BUS1_TEST:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS1_TEST_ANSWER;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
			
		case CMD_BUS1_CHECK:
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS1_CHECK;
			stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
			stLocalControl.stBusDealFreq.byRecBedAddr = 0x00;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
			SaveParameter(IAP0_ADDR);
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
//	led0flash();

	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ
				MakeCH0TimerOut(150, 0);				
			}			
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
uint8 xdata x,y;

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
	//��״̬����
	if(--stLocalControl.byLedFlashTime == 0x00)
	{
		stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;
		if((stLocalControl.byLedState & 0x0f) == LED_FLASH)
		{ 	//����˸״̬
			FLED = !FLED;
			LED = FLED;
		}	
	}

	if(--stLocalControl.bySecondFlashTime==0)
	{
		stLocalControl.bySecondFlashTime=25;
		if(stLocalControl.byDisplayFace==LAND_FACE)
		{
			x=56;y=64;
			bSecondDisState=!bSecondDisState;
			if(bSecondDisState)
			{
				Display_ASCII(56,64,ColonCode16x32,BLACK,CYAN,3);	//":"				
			}
			else
			{
				DisplayBlock(56,64,56+15,64+31,CYAN_CODE);
			}
			
		}
	}
	if(stLocalControl.uiLcdDisplayOnTime)
	{
		if(--stLocalControl.uiLcdDisplayOnTime==0)	//LCD����ʾ���ص��������ʾʱ���ѵ�
		{
			if((bBL_SW_State==0)||(stLocalControl.byPatientDischarge==1))
			{//�˴�û���˻����ڹر���ʱ��
				BL_SW =0;	//����ʾ��
			}
		}
	}

	if(stLocalControl.uiLcdDisplayInfoTime)
	{
		if(--stLocalControl.uiLcdDisplayInfoTime==0)
		{
			CurrentPage =0;
			stLocalControl.uiLcdDisplayInfoTime =0;		
			ShowLandingFace();
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
	  	  
