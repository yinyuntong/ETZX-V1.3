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

extern unsigned char code ParameterData[];

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



void UsartSendSBNextFrame(void);	
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

	//,P3^4��©���
	P3M0 |=   Bin(0,0,0,1,0,0,0,0);
	P3M1 |=   Bin(0,0,0,1,0,0,0,0);

	//���ߵ�ƽ����
	Bus0SendPin = 1;
	Bus0RecPin = 1;	 
	
	

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
	addr  = IAP_ADDR;
	
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
	}
	//��дÿ���ݿ���
	pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);

	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		IapWriteByte(addr,*pbyData);
		pbyData++;
		addr++;
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
	if(ParaCrcCheck(IAP_ADDR) ==1)
	{//��һ��У�����ȷ	
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP_ADDR);
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//�����Ѿ���ʼ�������,��ȡ���ݲ���
			ReadParameter(IAP_ADDR);
			SaveParameter(IAP_ADDR);
		}
		else	goto InitParaVal;	
	}
	else if(ParaCrcCheck(IAP_ADDR+sizeof(STEepromCfgData)) == 1)
	{
		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP_ADDR+sizeof(STEepromCfgData));
		DisableIAP();
		
		if(flagData == ParameterData[0])
		{//�����Ѿ���ʼ�������,�ӵڶ����ȡ���ݲ���
			ReadParameter(IAP_ADDR+sizeof(STEepromCfgData));
			SaveParameter(IAP_ADDR);
		}
		else	goto InitParaVal;			
	}
	else
	{//���鶼����ȷ������Ĭ��ֵ
InitParaVal:
		stLocalControl.stEepromCfgData.byInitFlag = ParameterData[0];
		stLocalControl.stEepromCfgData.bySelfSecAddr = ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr = ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr = ParameterData[3];
		stLocalControl.stEepromCfgData.bySerialNum1= ParameterData[4];
		stLocalControl.stEepromCfgData.bySerialNum2= ParameterData[5];
		stLocalControl.stEepromCfgData.bySerialNum3= ParameterData[6];
		stLocalControl.stEepromCfgData.byVersionHi = ParameterData[7];
		stLocalControl.stEepromCfgData.byVersionLo= ParameterData[8];
		stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour=0x00;
		stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin=0x00;
		stLocalControl.stEepromCfgData.byRFSerialNum1=ParameterData[12];
		stLocalControl.stEepromCfgData.byRFSerialNum2=ParameterData[13];	
		stLocalControl.stEepromCfgData.byRFSerialNum3=ParameterData[14];
		stLocalControl.stEepromCfgData.byRFSerialNum4=ParameterData[15];
		stLocalControl.stEepromCfgData.byRFSerialNum5=ParameterData[16];
		stLocalControl.stEepromCfgData.byRFSerialNum6=ParameterData[17];
		stLocalControl.stEepromCfgData.uiRs485Brt	 = (ParameterData[18]<<8)+ParameterData[19];
		stLocalControl.stEepromCfgData.byRs485SecRoom= ParameterData[20];
		stLocalControl.stEepromCfgData.byCrc =         ParameterData[21];
		SaveParameter(IAP_ADDR);
	}
}


void UsartSendSBNextFrame(void)
{
	uint8 xdata i=1000;
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;	

	if(stLocalControl.uiSendSBTotalDataLen==0)
	{
		memset(byUsart0SndBuf,0x00,UART0_TX_BUF_SIZE);
		stLocalControl.bySendSBTimes =0;
		stLocalControl.byUsart1SdDelayTime =0;
		return;
	}

	
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	WDT_CONTR = 0x3e;	//ι��
	switch(pstDataFreq->byData[1])	//��Ϣ����
	{//֮ǰ��Ϣ����
		case DIET_TYPE:
			if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
			{//һ֡���Է������
				Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR+stLocalControl.uiSendedDataLen,stLocalControl.uiSendSBTotalDataLen);
				pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
				pstDataFreq->byData[4] =1;	//������־
				stLocalControl.uiSendSBTotalDataLen =0;
			}
			else 
			{
				Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR+stLocalControl.uiSendedDataLen,UART0_TX_BUF_SIZE-EXT_SIZE-5);
				pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
				stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
				pstDataFreq->byData[4] =0;	//�ǽ���
			}
			//pstDataFreq->byData[1] = DIET_TYPE;	
			break;
		case PRESCRIPTION_TYPE:
	
			if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
			{
				Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiSendedDataLen,stLocalControl.uiSendSBTotalDataLen);
				pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
				pstDataFreq->byData[4] =1;	//������־
				stLocalControl.uiSendSBTotalDataLen =0;
			}
			else 
			{
				Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiSendedDataLen,UART0_TX_BUF_SIZE-EXT_SIZE-5);
				pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
				stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
				pstDataFreq->byData[4] =0;	//�ǽ���
			}
			//pstDataFreq->byData[1] = PRESCRIPTION_TYPE;	
			break;
		case PROMPT_INFO_TYPE:						
			if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
			{
				Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR+stLocalControl.uiSendedDataLen,stLocalControl.uiSendSBTotalDataLen);
				pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
				pstDataFreq->byData[4] =1;	//������־
				stLocalControl.uiSendSBTotalDataLen =0;
			}
			else 
			{
				Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR+stLocalControl.uiSendedDataLen,UART0_TX_BUF_SIZE-EXT_SIZE-5);
				pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
				stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
				pstDataFreq->byData[4] =0;	//�ǽ���
			}
			//pstDataFreq->byData[1] = PROMPT_INFO_TYPE;
			break;
		default:
			return;
	}
	
	stLocalControl.bySendSBTimes =3;
	stLocalControl.byUsart1SdDelayTime =150;
	stLocalControl.uiSendedDataLen += (pstDataFreq->uiLen-5);
	
	pstDataFreq->byStartByte1 = START_BYTE1;
	//Դ��ַ����
	pstDataFreq->bySndSecAddr = 0x00;
	pstDataFreq->bySndRoomAddr= 0x00;
	pstDataFreq->bySndBedAddr =0x00;
	
	pstDataFreq->byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	pstDataFreq->byRecRoomAddr= stLocalControl.stEepromCfgData.bySelfRoomAddr;
	pstDataFreq->byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	
	pstDataFreq->byCmd= CMD_SB_LCD_DISPLAY_DATA;
	pstDataFreq->byData[0] = 0x00;
	
	pstDataFreq->byData[2] ++;	//֡�ż�1
	//pstDataFreq->byData[3] = 0x01;	//���ţ�δ��
	
	pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
	pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
	
	
	stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
	stUsartCfg.uiSndPos = 0;	//������֡ͷ����ʼ��������
	stUsartCfg.pbySndBuf = byUsart0SndBuf;
	
	TI=0;
	SBUF= byUsart0SndBuf[0];	

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
	uint8 xdata i;
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;	
	//ȡ���յ�������֡ 	 		
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	OS_EXIT_CRITICAL();	

//���½���������
//	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);

/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//ȷ�ϵǼ�����				
			if(bLanding)
			{					
				bLanding =0;
				MakeCH0TimerOut(0, 0);
				if( (stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.bySndSecAddr)||
					(stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.bySndRoomAddr)||
					(stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.bySndBedAddr))
				{
					stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.bySndSecAddr;
					stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.bySndRoomAddr;
					stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.bySndBedAddr;
					
					stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
					SaveParameter(IAP_ADDR);

					page=1;
					ShowItem(BED_NUMBER);
					if(stLocalControl.byDisplayFace == BED_INFO_FACE) Set_VisualPage(1);
				}

				if(stLocalControl.bySlaveState != stLocalControl.stBusDealFreq.byRecSecAddr)
				{
					stLocalControl.bySlaveState = stLocalControl.stBusDealFreq.byRecSecAddr;
					
					if( stLocalControl.bySlaveState	&PATIENT_DISCHARGE)
					{//bit0==1:��ʾ���˳�Ժ 
						BL_OFF();//�ر��� 
					}

					if(stLocalControl.bySlaveState &LCD_ON)
					{//bit1==1:��ʾ��ʾ�������
						if(!(stLocalControl.bySlaveState&PATIENT_DISCHARGE))
						{//�в���
							BL_ON();
						}
						else   BL_OFF();
	
					}
					else 
					{
						BL_OFF();
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
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;

		case CMD_DATE_SEND:									//�յ�Уʱ����
			//stLocalControl.stTime.bySec = BcdToHex(stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f);
			stLocalControl.stTime.byMin = BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f);
			stLocalControl.stTime.byHour = BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f);
			stLocalControl.stTime.byDay = BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f);
			stLocalControl.stTime.byMonth = BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f);
			stLocalControl.stTime.byYear = BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);		
			stLocalControl.byMSecond = 0x00;
			stLocalControl.stTime.bySec=0x00;
			break;
			
		case CMD_CALL_LISTEN:								//��������
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{	//���������Ǳ������͵�
				page=0;
				ShowTalkFace(stLocalControl.stBusDealFreq.byRecRoomAddr, stLocalControl.stBusDealFreq.byRecBedAddr);
			}
//			else
//			{	//���������������ֻ����͵�
//				ShowTalkFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr,0);
//			}
			break;

		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
			   (stLocalControl.stBusDealFreq.bySndRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr) &&
			   (stLocalControl.stBusDealFreq.bySndBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
			{	//���������Ǳ������͵�
				page=0;
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 1);
			}
			else
			{	//���������������ֻ����͵�
				page=0;
				ShowBroadFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, 0);
			}
			break;
		case CMD_INFO_INDICATION:							//����ʾ������Ϣ
		
			if(stLocalControl.stBusDealFreq.bySndRoomAddr == 250 )
			{
				page=0;
				ShowAddBedCallFace(stLocalControl.stBusDealFreq.bySndBedAddr);
			}
			else 
			{
				page=0;
				ShowCallFace(stLocalControl.stBusDealFreq.bySndRoomAddr&0x7f, stLocalControl.stBusDealFreq.bySndBedAddr, 
						 stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f);	
			}
			break;
		case CMD_STOP_INDICATION:	 						//���رպ�����Ϣ��ʾ 
		case CMD_SYSTERM_RESET:
			Set_VisualPage(1);
			stLocalControl.byDisplayFace=BED_INFO_FACE;			
			break;

		case CMD_CLEAR_LCD:									//���Һ������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
				stLocalControl.uiBedFaceDataLen=0;
				Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&(stLocalControl.uiBedFaceDataLen),2);
				Write_Cont(BED_FACE_DATA_BAK,&(stLocalControl.uiBedFaceDataLen),2);
			
				memset(&(stLocalControl.stSupplyOxStartTime.byYear),0x00,6);
				memset(&(stLocalControl.stSupplyOxEndTime.byYear),0x00,6);
				memset(&(stLocalControl.stSupplyOxSubtotalTime.uiHour),0x00,4);
				memset(&(stLocalControl.stSupplyOxTotalTime.uiHour),0x00,4);
				
				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour=0x00;
				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin=0x00;	
				page=1;
			
				ShowBedFace();

				if(stLocalControl.byDisplayFace==BED_INFO_FACE)
						Set_VisualPage(1);
				
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				//SaveParameter();

				stLocalControl.bySlaveState |= PATIENT_DISCHARGE; //���˳�Ժ
				BL_OFF();

			}
			break;
			
		case CMD_NUMBER_SET:								//�޸ı������ţ�����
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x01)
			{	
				page=0;
				ShowStartNumSet();
				stLocalControl.uiLcdDisplayTime = 500;	//�����ʱ��10S
				BL_ON();
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x02)
			{	
				stLocalControl.byDispNumSetOkTime=50;		//��ʾһ��

				if((stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.bySndSecAddr)||
				   (stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.bySndRoomAddr)||
				   (stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.bySndBedAddr))
				{
				//�����ַ
					stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
					stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
					stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
					
					stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				page=0;
				ShowNumSetOk();
				//SaveParameter();
				
				break;
			}
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0x00)	//��ų�ʱ
			{
				Set_VisualPage(1);
				stLocalControl.byDisplayFace=BED_INFO_FACE;
				break;
			}
			break;
		case CMD_SYSTEM_SEC_SET:							//ϵͳ��������
			stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.byRecSecAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
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
		case CMD_INFUSION_CALL:							//��Һ����		 
		case CMD_SERVICE_CALL:							//�������			 
		case CMD_EMERGENCY_CALL:						//��������
		case CMD_HELP_CALL:								//��Ԯ����
			 break;
		case CMD_INFUSION_ENTER:								//��Һ����ȷ������
		case CMD_SERVICE_ENTER:									//�������ȷ������
		case CMD_HELP_ENTER:									//��Ԯ����ȷ������
		case CMD_EMERGENCY_ENTER:								//��������ȷ������
			page=0;
			 ShowCallSuccess();
			 stLocalControl.uiLcdDisplayTime = 250;	//�����ʱ��5S
			 BL_ON();
			 break;
			 
		 case CMD_SERVICE_CLEAR:								 //�������������� 
		 case CMD_CHANNEL_CLOSE:		 						 //ͨ���ر�
		 	 #if CONFIG_CC1101==1
		 	  memcpy(&(byWireLessSndBuff[1]),&(stLocalControl.stEepromCfgData.byRFSerialNum1),6);
			  byWireLessSndBuff[7]= CMD_INFUSION_CLEAR;

			  halRfSendPacket(byWireLessSndBuff,8);	//��������
			  Delayms(1000);
			  halRfSendPacket(byWireLessSndBuff,8);	//��������
			  Delayms(1000);		 			  
  			  halSpiStrobe(CCxxx0_SRX);	//�������
  			  CCAPM1 |= PCA1_ECCF;	//�����ж�	
			 #endif
			 
			  Set_VisualPage(1);
			  stLocalControl.byDisplayFace=BED_INFO_FACE;			
			  break;  			 
		 case CMD_INFUSION_CLEAR:								 //��Һ�����������
		 case CMD_HELP_CLEAR:									 //��Ԯ�����������
		 case CMD_EMERGENCY_CLEAR:								 //��������������� 	
			   Set_VisualPage(1);
			   stLocalControl.byDisplayFace=BED_INFO_FACE;			
			  break;
		case CMD_INFUSION_ANSWER:							//������Һ��������
		case CMD_SERVICE_ANSWER:							//��������������
		case CMD_EMERGENCY_ANSWER:							//���������������
		case CMD_HELP_ANSWER :
			 Set_VisualPage(1);
			stLocalControl.byDisplayFace=BED_INFO_FACE;			 
			  break;	

		case CMD_CHANNEL_CHANGE:
			page=0;
			ShowTalkFace(stLocalControl.stBusDealFreq.byRecRoomAddr, stLocalControl.stBusDealFreq.byRecBedAddr);
			break;	
		case CMD_BUS_ANSWER:
			 break;
		case CMD_SUPPLY_OX_START:
			bEnableOxTimer=1;
			 memcpy(&(stLocalControl.stSupplyOxStartTime.byYear),&(stLocalControl.stTime.byYear),6);

			 ShowItem(OX_START);

			 memset(&(stLocalControl.stSupplyOxEndTime.byYear),0x00,sizeof(STTime));
			 ShowItem(OX_END);
				

			 memset(&(stLocalControl.stSupplyOxSubtotalTime.uiHour),0x00,sizeof(STOXTime)); //����ʱ��������0
			 ShowItem(OX_SUBTOTAL);
			 
			
			if(stLocalControl.byDisplayFace==BED_INFO_FACE) Set_VisualPage(1);	

			stLocalControl.uiLcdDisplayTime = 250; //�����ʱ��5S
			//BL_SW=1;
			BL_ON();
			break;
		case CMD_SUPPLY_OX_END:
			if(bEnableOxTimer==1)
			{
				bEnableOxTimer=0;
				 memcpy(&(stLocalControl.stSupplyOxEndTime.byYear),&(stLocalControl.stTime.byYear),6);

				//���㵥��ʱ��
				stLocalControl.stSupplyOxSubtotalTime=timeInterval(stLocalControl.stSupplyOxEndTime,stLocalControl.stSupplyOxStartTime);
				//���ʱ�����3��,����1����
				if(stLocalControl.stSupplyOxSubtotalTime.bySec>3)
				{
					stLocalControl.stSupplyOxSubtotalTime.byMin++;
					if(stLocalControl.stSupplyOxSubtotalTime.byMin==60)
					{
						if(stLocalControl.stSupplyOxSubtotalTime.uiHour<=9998) stLocalControl.stSupplyOxSubtotalTime.uiHour++;
					}
				}
				//���͵���ʱ�����ݸ�����ģ��
				stLocalControl.stBusDealFreq.bySndSecAddr =  stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_SUPPLY_OX_END;
				stLocalControl.stBusDealFreq.byRecSecAddr =  (stLocalControl.stSupplyOxSubtotalTime.uiHour>>8);
				stLocalControl.stBusDealFreq.byRecRoomAddr = (uint8)stLocalControl.stSupplyOxSubtotalTime.uiHour;
				stLocalControl.stBusDealFreq.byRecBedAddr =  stLocalControl.stSupplyOxSubtotalTime.byMin;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				
				//�����۴�ʱ��
				stLocalControl.stSupplyOxTotalTime.byMin += stLocalControl.stSupplyOxSubtotalTime.byMin;
				stLocalControl.stSupplyOxTotalTime.uiHour+= stLocalControl.stSupplyOxSubtotalTime.uiHour;
				if(stLocalControl.stSupplyOxTotalTime.byMin>=60)
				{
					stLocalControl.stSupplyOxTotalTime.byMin = stLocalControl.stSupplyOxTotalTime.byMin-60;
					stLocalControl.stSupplyOxTotalTime.uiHour++;
				}	

				memset(&(stLocalControl.stSupplyOxStartTime.byYear),0x00,sizeof(STTime));	//��ʼʱ����0
				ShowItem(OX_START);
				ShowItem(OX_END);	//��ʾ��������ʱ��
				ShowItem(OX_SUBTOTAL);	//��ʾ���ι���ʱ��
				ShowItem(OX_TOTAL);    //��ʾ�۴�ʱ��

			   if(stLocalControl.byDisplayFace==BED_INFO_FACE) Set_VisualPage(1);

				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour=stLocalControl.stSupplyOxTotalTime.uiHour;
				stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin=stLocalControl.stSupplyOxTotalTime.byMin;

				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				//SaveParameter();

				stLocalControl.uiLcdDisplayTime = 250;	//�����ʱ��5S
				 //BL_SW=1;
				 BL_ON();
			}
			break;	

		case CMD_RF_NUMSET_START:
			page=0;
			ShowStartRfNumSet();
			 stLocalControl.uiLcdDisplayTime = 250;	//�����ʱ��5S
			 //BL_SW=1;	
			 BL_ON();
			break;
		case CMD_RF_NUMSET_OK:
			 stLocalControl.uiLcdDisplayTime = 250;	//�����ʱ��5S
			 //BL_SW=1;	
			 BL_ON();
			 page=0;
			ShowRfNumSetOk();	
			stLocalControl.byDispNumSetOkTime=50;		//��ʾһ��
			//������Һ���������к�

			if( (stLocalControl.stEepromCfgData.byRFSerialNum1!= stLocalControl.stBusDealFreq.bySndSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum2!= stLocalControl.stBusDealFreq.bySndRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum3!= stLocalControl.stBusDealFreq.bySndBedAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum4!= stLocalControl.stBusDealFreq.byRecSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum5!= stLocalControl.stBusDealFreq.byRecRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum6!= stLocalControl.stBusDealFreq.byRecBedAddr))
			{
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum1),&(stLocalControl.stBusDealFreq.bySndSecAddr),3);
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum4),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);

				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			//SaveParameter();
			}
			break;
		case CMD_RF_NUMSET_END:
			Set_VisualPage(1);			//�ָ�����̬����
			stLocalControl.byDisplayFace=BED_INFO_FACE;			
			break;	
	    case CMD_TELL_RF_NUM:
			if( (stLocalControl.stEepromCfgData.byRFSerialNum1!= stLocalControl.stBusDealFreq.bySndSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum2!= stLocalControl.stBusDealFreq.bySndRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum3!= stLocalControl.stBusDealFreq.bySndBedAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum4!= stLocalControl.stBusDealFreq.byRecSecAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum5!= stLocalControl.stBusDealFreq.byRecRoomAddr)||
				(stLocalControl.stEepromCfgData.byRFSerialNum6!= stLocalControl.stBusDealFreq.byRecBedAddr))
			{
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum1),&(stLocalControl.stBusDealFreq.bySndSecAddr),3);
				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum4),&(stLocalControl.stBusDealFreq.byRecSecAddr),3);

				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
				//SaveParameter();	
			}
			break;
			
		case CMD_OPEN_LCD:
			if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
			{//���Ե�ַ
				stLocalControl.bySlaveState |= LCD_ON;
				BL_ON();
			}
			else if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{//�㲥��
				stLocalControl.bySlaveState |= LCD_ON;
				if(!(stLocalControl.bySlaveState &PATIENT_DISCHARGE))
				{//�˴�λ�в���
					BL_ON();
				}
			}
			break;
			
		case CMD_CLOSE_LCD:
			stLocalControl.bySlaveState &= ~LCD_ON;
			BL_OFF();
			break;	
			
		case CMD_NURSE_IN:
			if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState&PATIENT_DISCHARGE)) //�����������	
			{
				BL_ON();
			}
			break;
			
		case CMD_NURSE_OUT:
			if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState&PATIENT_DISCHARGE)) //�����������	
			{
				BL_OFF();
			}
			break;
			
		case CMD_PC_OPEN:
			stLocalControl.stBusDealFreq.bySndSecAddr =  stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr =  stLocalControl.stEepromCfgData.bySelfBedAddr;			
			stLocalControl.stBusDealFreq.byCmd = CMD_SUPPLY_OX_TOTAL;			
			stLocalControl.stBusDealFreq.byRecSecAddr = (stLocalControl.stSupplyOxTotalTime.uiHour>>8);
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stSupplyOxTotalTime.uiHour;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stSupplyOxTotalTime.byMin;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_OPEN_485BUS_IN:	
			stLocalControl.bySlaveState &= ~PATIENT_DISCHARGE;  //������Ժ
			BL_ON();
			stLocalControl.uiLcdDisplayTime= 500;	//��10����
			break;


		case CMD_BUS1_TEST:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS1_TEST_ANSWER;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			if( (stLocalControl.stEepromCfgData.bySelfSecAddr != stLocalControl.stBusDealFreq.byRecSecAddr)||
				(stLocalControl.stEepromCfgData.bySelfRoomAddr != stLocalControl.stBusDealFreq.byRecRoomAddr)||
				(stLocalControl.stEepromCfgData.bySelfBedAddr != stLocalControl.stBusDealFreq.byRecBedAddr))
			{
				stLocalControl.stEepromCfgData.bySelfSecAddr= stLocalControl.stBusDealFreq.byRecSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr= stLocalControl.stBusDealFreq.byRecRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr= stLocalControl.stBusDealFreq.byRecBedAddr;
				
				stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
				SaveParameter(IAP_ADDR);

				page=1;
				ShowItem(BED_NUMBER);
				if(stLocalControl.byDisplayFace == BED_INFO_FACE) Set_VisualPage(1);				
			}			
			break;

		case CMD_SEND_PIN_BUG:
			if(stLocalControl.stBusDealFreq.byRecSecAddr ==0x00)
			{
				page=1;
				DisSendPinBugFace();
			}
			else if(stLocalControl.stBusDealFreq.byRecSecAddr ==0x01)
			{
				page=1;			
				ShowBedFace();
				Set_VisualPage(1);
			}
			break;
		case CMD_SB_DATA_ANSWER:

			if(stLocalControl.stBusDealFreq.byRecSecAddr == 0x01)
			{//�յ���Ӧ��,����������Ϣ
				UsartSendSBNextFrame();
			}
			else
			{//�ط�
				if(--stLocalControl.bySendSBTimes !=0x00)
				{
					i=1000;
					while(stUsartCfg.uiSndLen)
					{
						delay_nms(1);
						if(--i ==0) break;
					}
					WDT_CONTR = 0x3e;	//ι��
					
					stLocalControl.byUsart1SdDelayTime =150;
					stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg.uiSndPos = 0; 	//������֡ͷ����ʼ��������
					stUsartCfg.pbySndBuf = byUsart0SndBuf;
					
					TI=0;
					SBUF= byUsart0SndBuf[0];
				}
				else UsartSendSBNextFrame();
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

		case CMD_RS485_INFO_SEC_ROOM:
			if(stLocalControl.stBusDealFreq.byRecSecAddr==0 ||stLocalControl.stBusDealFreq.byRecSecAddr==1)
			{
				stLocalControl.stEepromCfgData.byRs485SecRoom =  stLocalControl.stBusDealFreq.byRecSecAddr;
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
			}
			break;
		case CMD_DISP_MODE:
			break;

		case CMD_COLOR_CLEAR:
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
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;	
	uint8 xdata i;
	//ȡ��������ɵ�����֡
	OS_ENTER_CRITICAL();
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;
	OS_EXIT_CRITICAL();	

	//���½���������
//	Send_Data((uint8 *)&(stLocalControl.stBusDealFreq),7);
	/////////////////////////////////////////////////////////////////////////////////////

	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ
				MakeCH0TimerOut(50, 30);				
			}	
			break;

		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 0;								//�������߹���

			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
			
			SaveParameter(IAP_ADDR);
			
			Bus0SendPin = 1;								//�ͷ�����				
			break;

		case CMD_OPEN_485BUS_IN:
			i=1000;
			while(stUsartCfg.uiSndLen)
			{
				delay_nms(1);
				if(--i ==0) break;
			}
			WDT_CONTR = 0x3e;	//ι��
			//һ����Ϣ�ĵ�һ֡
			
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case DIET_TYPE:
					 Read_Cont((uint8 *)(&(stLocalControl.uiSendSBTotalDataLen)),DIET_LENGTH_ADDR,2);
					if(stLocalControl.uiSendSBTotalDataLen <=UART0_TX_BUF_SIZE-EXT_SIZE-5)
					{//һ֡���Է������
						Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR,stLocalControl.uiSendSBTotalDataLen);
						pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
						pstDataFreq->byData[4] =1;	//������־
						stLocalControl.uiSendSBTotalDataLen =0;
					}
					else 
					{
						Read_Cont(&(byUsart0SndBuf[15]),DIET_DATA_START_ADDR,UART0_TX_BUF_SIZE-EXT_SIZE-5);
						pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
						stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
						pstDataFreq->byData[4] =0;	//�ǽ���
					}
					pstDataFreq->byData[1] = DIET_TYPE;
					break;
				case PRESCRIPTION_TYPE:
					Read_Cont((uint8 *)(&(stLocalControl.uiSendSBTotalDataLen)),PRESCRIPTION_LENGTH_ADDR,2);
					if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
					{
						Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR,stLocalControl.uiSendSBTotalDataLen);
						pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
						pstDataFreq->byData[4] =1;	//������־
						stLocalControl.uiSendSBTotalDataLen =0;
					}
					else 
					{
						Read_Cont(&(byUsart0SndBuf[15]),PRESCRIPTION_DATA_START_ADDR,UART0_TX_BUF_SIZE-EXT_SIZE-5);
						pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
						stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
						pstDataFreq->byData[4] =0;	//�ǽ���
					}
					pstDataFreq->byData[1] = PRESCRIPTION_TYPE;
					break;
				case PROMPT_INFO_TYPE:
					Read_Cont((uint8 *)(&(stLocalControl.uiSendSBTotalDataLen)),PROMPT_LENGTH_ADDR,2);
					if(stLocalControl.uiSendSBTotalDataLen<=UART0_TX_BUF_SIZE-EXT_SIZE-5)
					{
						Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR,stLocalControl.uiSendSBTotalDataLen);
						pstDataFreq->uiLen = stLocalControl.uiSendSBTotalDataLen+5;
						pstDataFreq->byData[4] =1;	//������־
						stLocalControl.uiSendSBTotalDataLen =0;
					}
					else 
					{
						Read_Cont(&(byUsart0SndBuf[15]),PROMPT_DATA_START_ADDR,UART0_TX_BUF_SIZE-EXT_SIZE-5);
						pstDataFreq->uiLen = UART0_TX_BUF_SIZE-EXT_SIZE;
						stLocalControl.uiSendSBTotalDataLen -= (UART0_TX_BUF_SIZE-EXT_SIZE-5);	
						pstDataFreq->byData[4] =0;	//�ǽ���
					}
					pstDataFreq->byData[1] = PROMPT_INFO_TYPE;
					break;
				default:
					return;	//����
			}


			stLocalControl.bySendSBTimes=3;	//�ط�����
			stLocalControl.byUsart1SdDelayTime =150;
			stLocalControl.uiSendedDataLen = pstDataFreq->uiLen-5;
			
			pstDataFreq->byStartByte1 = START_BYTE1;
			//Դ��ַ����
			pstDataFreq->bySndSecAddr = 0x00;
			pstDataFreq->bySndRoomAddr= 0x00;
			pstDataFreq->bySndBedAddr =0x00;
			
			pstDataFreq->byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			pstDataFreq->byRecRoomAddr= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			pstDataFreq->byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;

			pstDataFreq->byCmd= CMD_SB_LCD_DISPLAY_DATA;
			pstDataFreq->byData[0] = 0x00;	//���ݹ��ͣ�����Ϊ����������
			pstDataFreq->byData[2] = 0x01;	//��ʼ֡
			pstDataFreq->byData[3] = 0x01;	//���ţ�δ��

			pstDataFreq->byData[pstDataFreq->uiLen] = GetPskCrc(pstDataFreq);
			pstDataFreq->byData[pstDataFreq->uiLen+1] = END_BYTE;
			
			
			stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
			stUsartCfg.uiSndPos = 0; 	//������֡ͷ����ʼ��������
			stUsartCfg.pbySndBuf = byUsart0SndBuf;
			
			TI=0;
			SBUF= byUsart0SndBuf[0];			
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
		if(bBus0RecFinish)									//����0�յ�����
		{ 	
			OSQPost(byMainCmdQ, BUS0_REC);				
		}
		if(bBus0SendFinish)									//����0����������֡
		{
			OSQPost(byMainCmdQ, BUS0_SND);			
		} 
		OSWait(K_TMO, 1);	
		
		//��λ���Ź�
		WDT_CONTR = 0x3e; 		
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
	uint8 xdata i;
	STUartFreq *xdata pstDataFreq = byUsart0SndBuf;
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
	if(stLocalControl.byDispNumSetOkTime)
	{
		if(--stLocalControl.byDispNumSetOkTime==0)
		{
			Set_VisualPage(0x01);
			stLocalControl.byDisplayFace = BED_INFO_FACE;
		}
	}

	if(++stLocalControl.byMSecond==50)
	{
		stLocalControl.byMSecond=0;
		stLocalControl.stTime.bySec++;
		if(stLocalControl.stTime.bySec==60)
		{
			stLocalControl.stTime.bySec=0;
		}
	}
	
	if(stLocalControl.uiLcdDisplayTime)
	{
		if(--stLocalControl.uiLcdDisplayTime==0)	//LCD����ʾ���ص��������ʾʱ���ѵ�
		{
			if( !(stLocalControl.bySlaveState&LCD_ON) || (stLocalControl.bySlaveState &PATIENT_DISCHARGE))
			{//�˴�û����
				BL_OFF();
			}
		}
	}

#if CONFIG_CC1101==1
	if(stLocalControl.uiCC1101ReReadTime)	//CC1101ÿ��5���Ӷ�ȡ1�ν��ջ�����
	{
		stLocalControl.uiCC1101ReReadTime--;
		if(stLocalControl.uiCC1101ReReadTime==0)
		{
			if(bWireLessRec==0)
			{
				byWireLessRecLen=8; //Ҫ��ȡ���ݵ���󳤶�
				halRfReceivePacket(byWireLessRecBuff,&byWireLessRecLen);	//��ϴ�����ջ�����
				halSpiStrobe(CCxxx0_SRX);	//�������
			}
			stLocalControl.uiCC1101ReReadTime = CC1101_REREAD_TIME;		//���¸�ʱ
		}
	}
#endif	

	if(stLocalControl.byUsart1SdDelayTime)
	{
		if(--stLocalControl.byUsart1SdDelayTime ==0)
		{//ʱ�䵽
			if(stLocalControl.bySendSBTimes)
			{//�д�����ʱ
				if(--stLocalControl.bySendSBTimes !=0)
				{
					//�ط�����
					i=1000;
					while(stUsartCfg.uiSndLen)
					{
						delay_nms(1);
						if(--i ==0) break;
					}
					WDT_CONTR = 0x3e;	//ι��
					
					stLocalControl.byUsart1SdDelayTime =150;
					stUsartCfg.uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg.uiSndPos = 0;	//������֡ͷ����ʼ��������
					stUsartCfg.pbySndBuf = byUsart0SndBuf;
					
					TI=0;
					SBUF= byUsart0SndBuf[0];
				}
				else UsartSendSBNextFrame();
			}
		}
	}
	else
	{
		if(stLocalControl.byDisplayFace !=TALK_FACE)
		{//��ͨ��״̬
			if(bUsartRecedDietData == 1)
			{
				if(stUsartCfg.uiSndLen==0)
				{//û�д�����Ϣ���ڷ���
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					stLocalControl.stBusDealFreq.byRecSecAddr = DIET_TYPE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					bUsartRecedDietData=0;
					stLocalControl.byUsart1SdDelayTime =150;
				}
			}
			
			else if(bUsartRecedPrescriptionData == 1)
			{
				if(stUsartCfg.uiSndLen == 0)
				{//û��������Ϣ���ڷ���
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					stLocalControl.stBusDealFreq.byRecSecAddr = PRESCRIPTION_TYPE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					bUsartRecedPrescriptionData=0;	
					stLocalControl.byUsart1SdDelayTime =150;
				}
					
			}

			else if(bUsartRecedPromptData ==1)
			{
				if(stUsartCfg.uiSndLen == 0)
				{//û��������Ϣ���ڷ���
					stLocalControl.stBusDealFreq.byCmd = CMD_OPEN_485BUS_IN;
					stLocalControl.stBusDealFreq.byRecSecAddr = PROMPT_INFO_TYPE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					bUsartRecedPromptData=0;
					stLocalControl.byUsart1SdDelayTime =150;
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
		if(stLocalControl.stCH0TimerOut.byTimerOut)
		{	
			OSQPost(byMainCmdQ, TIMER_OUT);				
		}
		else
		{		
			OSQPost(byMainCmdQ, TIMER_OUT);	
		}
		//��λ���Ź�
		WDT_CONTR = 0x3e; 
	}
} 
  	  
