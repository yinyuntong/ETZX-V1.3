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
sbit  Bus0SendPin	= P3^7;									//������0�������Ŷ���
//---------------------------------------------------------


uint8 xdata BusLowDTime=100;
uint8 xdata bySec=2;
uint8 xdata byRoom=8;

bit	bLanding=0; 
bit	bCalling=0; 

bit bBus0SndBitOk=0;
bit bNumSeting =0;
//---------------------------------------------------------	 
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
	//����0���ͽ�����Ϊ������������ս�����Ϊ��������:M0=0,M1=0:׼˫��M0=0,M1=1:���죬M0=1,M1=0:���裬M0=1,M1=1:��©
	P3M0 |= 0x04;
	P3M1 &= 0xFB;
	P3M0 &= 0x7F;
	P3M1 |= 0x80;	
	//���ߵ�ƽ����
	Bus0SendPin = 0;
	Bus0RecPin = 1;	 
	//---------------------------------------------
	//--------------------------------------------- 
	//��ʱ��0��ʼ��:�Զ����ط�ʽ,��ʱ93.75us     
    //TMOD |= 0x02;
	//TL0 = TIMER0_COUNT;
	//TH0 = TIMER0_COUNT;
	//---------------------------------------------
	//---------------------------------------------
	TMOD &= 0xf0;		//��ʱ��1����
	TMOD |= 0x01;		//��ʱ��0ģʽ1:16Ϊ����
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	//---------------------------------------------
	//---------------------------------------------
	TF0 = 0;												//����жϱ�־										
	AUXR &= ~T0x12;											//0:��ʱ��0��ͳ12��Ƶ�ٶ�  
   	ET0 = 1;												//����ʱ��0�ж� 
	TR0 = 1;												//������ʱ��
	//������������
	byBus0SendStopCount = 240;								//�ϵ�����0��ֹ����ʱ������	  	
	IE0 = 0;												//����жϱ�־
	IT0 = 1;												//�ⲿ�ж�0Ϊ�½��ش���ģʽ	   				
	if(Bus0RecPin)
	{	//�����������,���ж�	
		EX0 = 1;
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
void Bus0RecInt(void) interrupt 0
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
						Bus0SendPin = 0;					//�ͷ�����
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
		{
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


//����stLocalControl.stEepromCfgData�е�����У���
uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);
//	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}
//	DisableIAP();
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
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
//	addr  = IAP0_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(addr); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}

/*	
	//��дÿ���ݿ���
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
	addr  = IAP1_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(addr); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}

	
	//��дÿ���ݿ���
	uint8 *pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
	addr  = IAP2_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(addr); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}*/
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
CheckIapSec0:	
	if(ParaCrcCheck(IAP0_ADDR) ==1)
	{//��һ��У�����ȷ	
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���
			ReadParameter(IAP0_ADDR);
			//SaveParameter(IAP1_ADDR);
			return;
		}
		else	goto CheckIapSec1;	
	}


CheckIapSec1:	
    if(ParaCrcCheck(IAP1_ADDR) ==1)
	{//�ڶ���У�����ȷ 
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP1_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���
			ReadParameter(IAP1_ADDR);
			SaveParameter(IAP0_ADDR);
			return;
		}
		else	goto CheckIapSec2;	
	}


CheckIapSec2:	
	if(ParaCrcCheck(IAP2_ADDR) ==1)
	{//������У�����ȷ 
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP2_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���
			ReadParameter(IAP2_ADDR);
			SaveParameter(IAP0_ADDR);
			SaveParameter(IAP1_ADDR);
			return;
		}
		else	goto InitParaVal;	
	}



	//�����������ݶ�����ȷ������Ĭ��ֵ
//	else
	{	//û�г�ʼ��,��ʼ������
InitParaVal:	
		stLocalControl.stEepromCfgData.byInitFlag		=ParameterData[0];
	    stLocalControl.stEepromCfgData.bySelfSecAddr	=ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr	=ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr	=ParameterData[3];
		stLocalControl.stEepromCfgData.byFlag			=ParameterData[4];
		stLocalControl.stEepromCfgData.byPrio			=ParameterData[5];
		stLocalControl.stEepromCfgData.byReserve1		=ParameterData[6];
		stLocalControl.stEepromCfgData.byReserve2		=ParameterData[7];
		stLocalControl.stEepromCfgData.byReserve3		=ParameterData[8];
		stLocalControl.stEepromCfgData.bySerialNum1		=ParameterData[9];
		stLocalControl.stEepromCfgData.bySerialNum2		=ParameterData[10];
		stLocalControl.stEepromCfgData.bySerialNum3		=ParameterData[11];
		stLocalControl.stEepromCfgData.byVersionHi		=ParameterData[12];
		stLocalControl.stEepromCfgData.byVersionLo		=ParameterData[13];
		stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

		SaveParameter(IAP0_ADDR);
		SaveParameter(IAP1_ADDR);
		SaveParameter(IAP2_ADDR);
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
				{ 	
					bLanding = 0;
					stLocalControl.byLedTime = 50;
					if(stLocalControl.stEepromCfgData.byFlag&0x01)
					{	//�趨Ϊ�Ƴ���
						SetLedRedState(LED_ON);	
					}
					else
					{	//�趨Ϊ����
						SetLedRedState(LED_OFF);	
					}				
					MakeCH0TimerOut(0, 0); 										
				}
			}			
			break;	
		case CMD_QUEST:										//��ѯ����
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_ANSWER; 			
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
		case CMD_EMERGENCY_ENTER:							//ȷ�Ͻ�����������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//��ȷ�ϱ����ĺ���,���õ���˸,�����ں��б�־	
				SetLedRedState(LED_FLASH);	
				bCalling = 1; 	
			} 	
			break; 
		case CMD_EMERGENCY_CLEAR: 							//��������������� 
		case CMD_EMERGENCY_ANSWER:							//���������������			
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//���������������ĺ���,ֹͣ����˸,�����ں��б�־	
				if(stLocalControl.stEepromCfgData.byFlag&0x01)
				{	//�趨Ϊ�Ƴ���
					SetLedRedState(LED_ON);	
				}
				else
				{	//�趨Ϊ����
					SetLedRedState(LED_OFF);	
				}
				bCalling = 0; 	
			} 			
			break; 			
		case CMD_SYSTERM_RESET:								//ϵͳ��λ����
			if(stLocalControl.stEepromCfgData.byFlag&0x01)
			{	//�趨Ϊ�Ƴ���
				SetLedRedState(LED_ON);	
			}
			else
			{	//�趨Ϊ����
				SetLedRedState(LED_OFF);	
			}	
			bCalling = 0; 	
			break; 	  		
			
		case CMD_ONLINE_WRITE:								//����д������
			break;
		case CMD_ONLIEN_READ: 								//���߶�������
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{
				if(stLocalControl.stBusDealFreq.bySndSecAddr >= sizeof(STEepromCfgData))
				{	//��������ȷ
					break;
				}			
				stLocalControl.uiShortType = IAP0_ADDR + stLocalControl.stBusDealFreq.bySndSecAddr;
				EnableIAP(IAP_READ);	
				stLocalControl.stBusDealFreq.bySndRoomAddr = IapReadByte(stLocalControl.uiShortType);		
				DisableIAP();
				stLocalControl.stBusDealFreq.byCmd = CMD_ONLINE_OUTPUT;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			}		
			break;
		case CMD_POWER_ON:	  								//��������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{			
				OS_ENTER_CRITICAL();						//�ر��ж�
			 	CCAPM0 = 0;
				ET0 = 0;
				TR0 = 0;
				EX0 = 0;
				ISP_CONTR = 0x20; 	
	
			} 
			break; 	
		case CMD_WRITE_SERIAL:								//д�����к�����
			break; 
			
		case CMD_READ_SERIAL:								//�����к�����
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySerialNum1;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySerialNum2;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySerialNum3;
			stLocalControl.stBusDealFreq.byCmd = CMD_OUT_SERIAL;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySerialNum1;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySerialNum2;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySerialNum3;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_NUMBER_SET:								//�޸ĵ�ַ����									
			// ��ʼ���豸���б��
			if(stLocalControl.stBusDealFreq.bySndBedAddr==0x00) //����Ϊ0��ʾΪ�����ֻ����
			 {
			   bNumSeting =1;
			   byNumSetTime = 250;	//5��
			   stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;
		       SetLedRedState(LED_FLASH);
			   bySec =  stLocalControl.stBusDealFreq.bySndSecAddr;
			   byRoom = stLocalControl.stBusDealFreq.bySndRoomAddr;
			 }
			break;

			
		case CMD_SEC_SET:
			stLocalControl.stEepromCfgData.bySelfSecAddr=stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			SaveParameter(IAP1_ADDR);
			SaveParameter(IAP2_ADDR);
			break;	
			
	    case CMD_BUS_TEST:                 //��������߲�������
		    stLocalControl.stBusDealFreq.bySndSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
		    stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	        stLocalControl.stBusDealFreq.bySndBedAddr  = stLocalControl.stEepromCfgData.bySelfBedAddr;
	        stLocalControl.stBusDealFreq.byRecSecAddr  = CMD_BUS_TEST;
	        stLocalControl.stBusDealFreq.byRecRoomAddr = 0x00;
	        stLocalControl.stBusDealFreq.byRecBedAddr  = 0x00; 
			stLocalControl.stBusDealFreq.byCmd = 0x02;   				
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
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
//	Send_Data(&(stLocalControl.stBusDealFreq),7);
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ
				MakeCH0TimerOut(150, 0);	
			}			
			break;					
		case CMD_EMERGENCY_CLEAR:							//�����������			
			if(stLocalControl.stEepromCfgData.byFlag&0x01)
			{	//�趨Ϊ�Ƴ���
				SetLedRedState(LED_ON);	
			}
			else
			{	//�趨Ϊ����
				SetLedRedState(LED_OFF);	
			}		
			bCalling = 0; 				 			
			break; 	
		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 1;								//�������߹���
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			SaveParameter(IAP0_ADDR);
			SaveParameter(IAP1_ADDR);
			SaveParameter(IAP2_ADDR);
			Bus0SendPin = 0;								//�ͷ�����					
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
		if(bBus0RecFinish)									//����0�յ�����
		{ 			
			OSQPost(byMainCmdQ, BUS0_REC);				
		}
		if(bBus0SendFinish)									//����0����������֡
		{
			OSQPost(byMainCmdQ, BUS0_SND);			
		} 
		OSWait(K_TMO, 1);
		WDT_CONTR = 0x3d;
	}
}

/*---------------------------------------------------------------------------
����ԭ��: void Delayms(uint ms)
����˵��: ms--��Ҫ��ʱ��ֵ
�� �� ֵ: ��
�������ܣ���ʱ����(����18.432M����ָ��������ʱ1mS)
----------------------------------------------------------------------------*/
/*void Delayms(uint16 ms)	  
{
   uint16 xdata i;
   for(;ms!=0;ms--)
		for(i=900;i!=0;i--);
}*/



/**********************************************************
*��������			:TimerOutDeal	
*��������        	:��ʱ������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:Bus0OutputData					 
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
					} 									
				}
				else
				{ 	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 				
					stLocalControl.stCH0TimerOut.byTimerOut = stLocalControl.stCH0TimerOut.byTimerOutSet;
				}				
			}			
		}
	} 
	if(--stLocalControl.byLedDelay == 0x00)
	{
		stLocalControl.byLedDelay = stLocalControl.byLedTime;
		if(stLocalControl.byLedState == LED_FLASH)
		{	//�����Ϊ��˸״̬,����Ƶ�״̬
			bLedRedState = !bLedRedState;
			LED_RED = LED_GREEN = bLedRedState;
		}
	}

	if(uiNurseInTime)
	{
		if(--uiNurseInTime == 0x00)	
		{	//��⻤ʿ��λ�Ķ�ʱʱ�䵽	��Ϊ5S
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;;
			stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
			stLocalControl.stBusDealFreq.bySndBedAddr= MoveFjNum;//�ƶ��ֻ���
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
			stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_OUT;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			bNurseIn=0;
		}
	}


	if(byIrDataIntervalTimer)
	{
		if(--byIrDataIntervalTimer==0x00)	//���ʱ�䵽
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

	if(byNumSetTime)
	{
		if(--byNumSetTime==0)				//���ʱ�䵽
		{
			bNumSeting =0;
//			stLocalControl.byLedTime = stLocalControl.byLedDelay = 0;
			SetLedRedState(LED_ON);
			
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
				RST_BUS =1;	//�Ͽ�SD������
				Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
				OSWait(K_TMO,100);	//��ʱ500ms    һ����λ5ms
				
				if(Bus0RecPin ==0)
				{//�����й��� //ʹ�ƿ���
					do
					{
						RST_BUS =1;	//�Ͽ�SD������
						Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						SetLedRedState(LED_OFF);
						OSWait(K_TMO,40);		//��ʱ200ms
					
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						SetLedRedState(LED_ON);
						OSWait(K_TMO,40);
						
					}while(Bus0RecPin==0);	//һֱ�ȴ�Ϊ��

					
					RST_BUS = 0;	//ʹSD��·��ͨ					
					BusLowDTime = 100;
					//ʹ�ƻָ�����
					SetLedRedState(LED_ON);
				}

				
				else 
				{//���ⲿ��������
					RST_BUS = 0;	//ʹSD��·��ͨ
					Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ

					do
					{
							
						RST_BUS = 0;	//ʹSD��·��ͨ
						Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
						
						SetLedRedState(LED_OFF);	//������
						OSWait(K_TMO,120);		//��ʱ600ms 
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						SetLedRedState(LED_ON);
						OSWait(K_TMO,120);
						WDT_CONTR = 0x3d;		//@20MHz,1.25s
						
					}while(Bus0RecPin==0);	//һֱ�ȴ�Ϊ��
					

					//ʹ�ƻָ�����
Bus0BugRet:
					SetLedRedState(LED_ON);
					Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
					RST_BUS = 0;	//ʹSD��·��ͨ					
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
	while(TRUE)
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
		WDT_CONTR = 0x3d;
	}
} 
