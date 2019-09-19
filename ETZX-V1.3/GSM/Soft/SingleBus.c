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
sbit  Bus0SendPin	= P3^7;									//������0�������Ŷ���




extern unsigned char code ParameterData[];

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
	//P3^2,׼˫��,P3^7,����
	P3M0 &= (~Bin(0,0,0,0,0,1,0,0));
	P3M1 &= (~Bin(0,0,0,0,0,1,0,0));
	P3M0 &= (~Bin(1,0,0,0,0,0,0,0));
	P3M1 |=   Bin(1,0,0,0,0,0,0,0);

	
	//���ߵ�ƽ����
	Bus0SendPin = 0;
	Bus0RecPin = 1;	
	 
	
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
	byBus0State0 =0;
	byBus0State1 =0;

	IE0 = 0;												//����ⲿ�ж�0��־
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

	bBus0RecBit = Bus0RecPin; 								//��������0��������״̬	          
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
		else
		{	//����ͬ,����ʧ��								
			byBus0State0 &= ~BUS0_SEND_CON;
			byBus0RecSendCount &= 0xf0;
			bBus0SendError = 1;					
			Bus0SendPin = 0; 
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

	EA=0;
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		EA=1;
		return(FALSE);
	}	
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	EA=1;	
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
*��������			:SndAddrCompare	
*��������        	:�Ƚ�Դ��ַ
*�������   		:pstBusFreq:���Ƚϵ�����ָ֡��
*����ֵ				:1:���͵�ַ�뱾�����,0:���͵�ַ�뱾�������	
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
bit SndAddrCompare(pSTBusFreq pstBusFreq)
{
	if( (pstBusFreq->bySndSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
	{	
		return(0);
	} 
	if((pstBusFreq->bySndRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr))
	{	
		return(0);
	}
	if( (pstBusFreq->bySndBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
	{	
		return(0);
	}
	return(1); 

}

/**********************************************************
*��������			:SndRecAddrCompare	
*��������        	:�Ƚ�Դ��ַ����Ŀ�ĵ�ַ
*�������   		:pstBusFreq:���Ƚϵ�����ָ֡��
*����ֵ				:1:���͵�ַ����յ�ַ�����ָ��ĵ�ַ�������,0:���͵�ַ����յ�ַ�����ָ��ĵ�ַ���ݲ����	
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
bit SndRecAddrCompare(uint8* pAddr)
{

	if(( 	(stLocalControl.stBusDealFreq.bySndSecAddr	== *pAddr)&&
			(stLocalControl.stBusDealFreq.bySndRoomAddr	== *(pAddr+1))&&
			(stLocalControl.stBusDealFreq.bySndBedAddr 	== *(pAddr+2)))||
	   	(	(stLocalControl.stBusDealFreq.byRecSecAddr 	== *pAddr)&&
			(stLocalControl.stBusDealFreq.byRecRoomAddr == *(pAddr+1))&&
			(stLocalControl.stBusDealFreq.byRecBedAddr 	== *(pAddr+2))))		
		return(1);
	else return(0);

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
	stLocalControl.stIndicationData.bySndSecAddr 	= pstBus0SendFreq->bySndSecAddr;
	stLocalControl.stIndicationData.bySndRoomAddr 	= pstBus0SendFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.bySndBedAddr 	= pstBus0SendFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCmd 			= pstBus0SendFreq->byRecSecAddr; 
//	stLocalControl.stIndicationData.byCallCmd = pstBus0SendFreq->byCmd;

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
	{ 	//ͨ��1����ͨ��
		XTD = 0;
		XTA = 1;	
	} 
	else if(bChannel0Talk)
	{	//ͨ��0����ͨ��״̬
		XTA = 0;
		XTD = 1;		
	}	

	else if(bWaitListen)
	{	//�ȴ�����״̬
		
	}
	else if(bIndicatingOther)
	{	//����ָʾ�����ֻ�
		
	}
	else
	{	//û���κ�����״̬����		
		XTA = XTD =0;	
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
	bBusy = bWaitAck = bWaitListen = 
    bChannel0Talk = 
	bSickRoomBroad = bOfficeBroad = bAllBroad = 
	bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
	bIndicatingOther=0;			
	VoiceChannelCtx();
	if(AddrCompare(&(stLocalControl.stBusDealFreq)))
	{
		//���ƶ��绰�������͹Ҷ�����;
		GSMHandUpTreat();
	}
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


//����������������ĵ绰����
void SaveTelNum(void)
{  
	stLocalControl.uiShortType = TEL_NUM_ADDR;	  //EEPROM����ʼ��ַ	
	stLocalControl.pbyData = TelNum;				//�����������
	stLocalControl.byChar1 = TEL_NUM_LEN;						//11���ֽ�
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


void ReadTelNum(void)
{

	stLocalControl.uiShortType = TEL_NUM_ADDR;	//EEPROM����ʼ��ַ	
	stLocalControl.pbyData = TelNum;				//�����������
	stLocalControl.byChar1 = TEL_NUM_LEN;						//11���ֽ�

	
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
	if(IapReadByte(IAP_ADDR) == ParameterData[0])
	{//�����Ѿ���ʼ�������,��ȡ���ݲ���
		ReadParameter();

	}
	else
	{	//û�г�ʼ��,��ʼ������
		stLocalControl.stEepromCfgData.byInitFlag = ParameterData[0];
		stLocalControl.stEepromCfgData.bySelfSecAddr = ParameterData[1];
		stLocalControl.stEepromCfgData.bySelfRoomAddr = ParameterData[2];
		stLocalControl.stEepromCfgData.bySelfBedAddr = ParameterData[3];
		
		stLocalControl.stEepromCfgData.bySerialNum1= ParameterData[4];
		stLocalControl.stEepromCfgData.bySerialNum2= ParameterData[5];
		stLocalControl.stEepromCfgData.bySerialNum3= ParameterData[6];
		stLocalControl.stEepromCfgData.byVersionHi = ParameterData[7];
		stLocalControl.stEepromCfgData.byVersionLo= ParameterData[8];
		
		stLocalControl.stEepromCfgData.byTelNum[0]	= ParameterData[9];
		stLocalControl.stEepromCfgData.byTelNum[1]	= ParameterData[10];	
		stLocalControl.stEepromCfgData.byTelNum[2]	= ParameterData[11];
		stLocalControl.stEepromCfgData.byTelNum[3]	= ParameterData[12];
		stLocalControl.stEepromCfgData.byTelNum[4]	= ParameterData[13];
		stLocalControl.stEepromCfgData.byTelNum[5]	= ParameterData[14];
		stLocalControl.stEepromCfgData.byTelNum[6]	= ParameterData[15];
		stLocalControl.stEepromCfgData.byTelNum[7]	= ParameterData[16];	
		stLocalControl.stEepromCfgData.byTelNum[8]	= ParameterData[17];
		stLocalControl.stEepromCfgData.byTelNum[9]	= ParameterData[18];
		stLocalControl.stEepromCfgData.byTelNum[10]	= ParameterData[19];
		stLocalControl.stEepromCfgData.byTelNum[11]	= ParameterData[20];
		stLocalControl.stEepromCfgData.byTelNumValidLen	= ParameterData[21];
		
		stLocalControl.stEepromCfgData.byTelFuncState	= ParameterData[22];
		stLocalControl.stEepromCfgData.byRingTime		= ParameterData[23];
		stLocalControl.stEepromCfgData.byTalkTime		= ParameterData[24];
		stLocalControl.stEepromCfgData.byBroadTime		= ParameterData[25];
		SaveParameter();
	}
}


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
	stLocalControl.stCH1TimerOut.byTimerOutSet = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOut = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
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
	uint8 i;
	//ȡ���յ�������֡ 	 		
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;
	EA=1;	

//���½���������
#if DEBUG==1
    Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
#endif
/////////////////////////////////////////////////////////////////////////////////////




	switch(stLocalControl.stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//ȷ�ϵǼ�����				
			if(bLanding)
			{	
				if(DirAddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//���͵�����
					SYS_STATE_LED = LED_ON;
					bLanding = 0;
				}
			}		
			break;
				
		case CMD_COMM_CALL:	 								//��ͨ��������  -----��Ե����
			if(!bBusy)
			{	//ͨ��0����				
				bBusy = 1;
				//���ó�ʱ5s				
				MakeCH0TimerOut(250, 0);
				
/*				if(bIndicatingOther) 
				{//�������ָʾ�����ֻ�,ָֹͣʾ
					bIndicatingOther=0;
					//���������Ҷϵ绰
					GSMHandUpTreat();
					
					VoiceChannelCtx();
					LedControl();
				}
				//�����������з���ַ
				SaveCallAddr(&(stLocalControl.stBusDealFreq));*/
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
				if((bChannel1Talk)||(!bDealKeyDown))
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
				if((bChannel1Talk)||(!bDealKeyDown))
				{	//���ͨ��1æ,���߱�������ͨ��������������,���̳�ʱʱ��					
					MakeCH0TimerOut(5, 0);
					break;
				}
				VoiceChannelCtx();
				LedControl();
			}
			break;	
			
		case CMD_BROADCAST1:								//�յ������㲥����
		case CMD_BROADCAST2:								//�յ��칫���㲥����
		case CMD_BROADCAST3: 								//�յ�ȫ���㲥����
			if(!bBusy)
			{	//ͨ��0����				
				bBusy = 1;
				//���ù㲥��ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime); 
				
/*				//�ݴ�����				
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.byCmd;				

				//�����������з���ַ				
				SaveCallAddr(&(stLocalControl.stBusDealFreq));
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ	
					bIndicatingOther = 0;	
					//���������Ҷϵ绰����
					GSMHandUpTreat();
					
					LedControl();  
					VoiceChannelCtx();					
				}				
			
				if((bChannel1Talk))
				{	//ͨ��1æ,��������	
					break;
				}			
				switch(stLocalControl.stBusDealFreq.byRecSecAddr)
				{	//��Ծ�������,�������Ƿ�������Ӧ�Ĺ㲥
					case CMD_BROADCAST1:
						bSickRoomBroad = 1; 
						break;
					case CMD_BROADCAST2:
						bOfficeBroad = 1; 
						break;
					case CMD_BROADCAST3:
						bAllBroad = 1;  
						break;
				}
				VoiceChannelCtx();
				LedControl();*/
			}
			break;	
		
		case CMD_INFUSION_ENTER:								//��Һ����ȷ������
		case CMD_SERVICE_ENTER:									//�������ȷ������
		case CMD_HELP_ENTER:									//��Ԯ����ȷ������
		case CMD_EMERGENCY_ENTER:								//��������ȷ������
			//
			break; 

			
		case CMD_INFO_INDICATION:								//�յ�����ָʾ����  
			if(!(bBusy ))
			{	//����ͨ��0���ڿ���״̬,����ָʾ�����ֻ��ĺ���������
				if((bChannel1Talk))
				{	//���ͨ��1����ͨ��״̬,��������
					return;
				}	
				
				switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
				{
					case CMD_INFUSION_CALL:	   				//��Һ���� 
					case CMD_SERVICE_CALL:					//������� 
					case CMD_HELP_CALL:						//��Ԯ����
 						//�����ƶ��绰
						GSMCallTreat();
						
						break;
					case CMD_EMERGENCY_CALL: 				//�������� 
					default:
						return;
				}

				SaveIndicationData(&(stLocalControl.stBusDealFreq));												
				bIndicatingOther = 1;					
				VoiceChannelCtx();
				LedControl();	
				UartSendByte(0xaa);
			}  
			break;

			
		case CMD_INFUSION_ANSWER:							//������Һ��������
		case CMD_SERVICE_ANSWER:							//��������������
		case CMD_EMERGENCY_ANSWER:							//���������������
		case CMD_HELP_ANSWER:								//������Ԯ��������
			if(!bBusy)
			{	//ͨ��0��æ				
				bBusy = 1;	
				//����ͨ��0��ʱ					
				MakeCH0TimerOut(250, 0);

				//�����������з���ַ	
//				SaveCallAddr(&(stLocalControl.stBusDealFreq));					
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//�������ĺ�������,����Ӧ�ı�־ 
					//GSMģ�鲻�����������У�Ȼ���������������
 			
				}
				if(bIndicatingOther)
				{	//����ָʾ�����ֻ�,�����־	 					
					bIndicatingOther = 0;	
					//���������Ҷϵ绰����
					GSMHandUpTreat();
					
					VoiceChannelCtx();						
					LedControl(); 			
				} 
				
			}
			break;
		case CMD_STOP_INDICATION:							//ָֹͣʾ����
			if(bIndicatingOther)
			{
				bIndicatingOther = 0;
				//���������Ҷϵ绰����
				GSMHandUpTreat();
				
				VoiceChannelCtx();
				LedControl(); 									
			}
			break;
		case CMD_INFUSION_CLEAR: 							//�����Һ��������
		case CMD_SERVICE_CLEAR: 							//��������������
		case CMD_HELP_CLEAR: 								//�����Ԯ��������
		case CMD_EMERGENCY_CLEAR: 							//���������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	//�����ͷ�ֻ��ĺ���,�����Ӧ�ı�־			
				if(bIndicatingOther &&
					(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.bySndSecAddr) &&
					(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.bySndRoomAddr) &&
					(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.bySndBedAddr) &&
					((stLocalControl.stBusDealFreq.byCmd-0x0a) == (stLocalControl.stIndicationData.byCmd & 0x1f)))
				{ 	//�����������ָʾ�ĺ�����Ϣ 					
					bIndicatingOther = 0;
					//���ƶ��绰���������Ҷ�����
					GSMHandUpTreat();
					
					VoiceChannelCtx();
					LedControl(); 													
				}
			}
			break; 			
		case CMD_SYSTERM_RESET:								//ϵͳ��λ����
			SysReset();
			break; 

		case CMD_SYSTERM_RESTART:
			bBusy = bWaitAck = bWaitListen =  
			bChannel0Talk = 
			bChannel1Talk = 
			bSickRoomBroad = bOfficeBroad = bAllBroad = 
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting=
			bIndicatingOther=0;		
			MakeCH0TimerOut(0, 0);
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			break;
	
		case CMD_START_VOICE:								//������������
			break;
		case CMD_MUSIC_PLAY:								//�������ֲ���
			break;	
			
		case CMD_STOP_VOICE:								//ֹͣ�������ֲ���
			break; 
		case CMD_CHANNEL_CHANGE: 							//ͨ���л�����
			//��ͨ��0��״̬�л���ͨ��1��,ͬʱ���ͨ��0��ͨ��״̬
			bBusy = bWaitAck = bWaitListen =  
				bSickRoomBroad = bOfficeBroad = bAllBroad =  0;			
			MakeCH0TimerOut(0, 0);					 				
			if((bChannel0Talk))
			{ 				
				bChannel1Talk = bChannel0Talk;
				bChannel0Talk = 0; 				
				//����ͨ��1ͨ����ʱ						
				MakeCH1TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);
				if(bChannel1Talk&&(!bDealKeyDown))
				{	//��������ͨ��������������,���̳�ʱʱ�� 	 									
					MakeCH1TimerOut(5, 0);
					break; 			
				}
				VoiceChannelCtx();
				LedControl(); 
			}							
			break;
			
		case CMD_CHANNEL_CLOSE: 							//�ر�ͨ���л�����
			//���ͨ��1����״̬			
			bChannel1Talk  =0;
			MakeCH1TimerOut(0, 0);	
			VoiceChannelCtx();
			LedControl();
			if(SndRecAddrCompare(&(stLocalControl.stEepromCfgData.bySelfSecAddr))||
			   SndRecAddrCompare(&(stLocalControl.stCallAddr.byRecSecAddr)))	//��·ͨ��ʱ����һ·ͨ���ֻ�������ͨ
			{
				//���ƶ��绰�������͹Ҷ�����;
				GSMHandUpTreat();
			}
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
					EA=0;
					//�ر����д򿪵��ж�
					CCAPM0 = 0x00;
					ET0 = 0;
					TR0 = 0;
					EX0 = 0;
					ISP_CONTR = 0x20; 				
				}
				
			}				
			break;

		case CMD_DATE_SEND:
			stLocalControl.stTime.bySec = BcdToHex(stLocalControl.stBusDealFreq.bySndSecAddr & 0x7f);
			stLocalControl.stTime.byMin = BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7f);
			stLocalControl.stTime.byHour = BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3f);
			stLocalControl.stTime.byDay = BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3f);
			stLocalControl.stTime.byMonth = BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1f);
			stLocalControl.stTime.byYear = BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);
			break;			
			

	
		case CMD_SD_TAL_VOL_CHECK:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTD=1;
			break;
			
		case CMD_SD_TAL_VOL_CHECK_END:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			XTD=0;
			break;

		case CMD_SA_TAL_VOL_CHECK:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 1;
			break;

		case CMD_SA_TAL_VOL_CHECK_END:
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			XTA = 0;
			break;
			
		case CMD_GSM_FUNC_SEARCH:
			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			//�����ԭ������
			stLocalControl.stBusDealFreq.byRecSecAddr	= stLocalControl.stEepromCfgData.byTelFuncState;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_GSM_FUNC_SET:
			stLocalControl.stEepromCfgData.byTelFuncState = stLocalControl.stBusDealFreq.byRecSecAddr;
			//SaveParameter();

			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd          = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr	= CMD_GSM_FUNC_SET;
			stLocalControl.stBusDealFreq.byRecRoomAddr	= stLocalControl.stEepromCfgData.byTelFuncState;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			break;
			
		case CMD_GSM_NUM_DEL:
/*			TelNum[0]	= stLocalControl.stBusDealFreq.bySndSecAddr>>4;
			TelNum[1]	= stLocalControl.stBusDealFreq.bySndSecAddr&0x0f;
			TelNum[2]	= stLocalControl.stBusDealFreq.bySndRoomAddr>>4;
			TelNum[3]	= stLocalControl.stBusDealFreq.bySndRoomAddr&0x0f;	
			TelNum[4]	= stLocalControl.stBusDealFreq.bySndBedAddr>>4;
			TelNum[5]	= stLocalControl.stBusDealFreq.bySndBedAddr&0x0f;	
			TelNum[6]	= stLocalControl.stBusDealFreq.byRecSecAddr>>4;
			TelNum[7]	= stLocalControl.stBusDealFreq.byRecSecAddr&0x0f;
			TelNum[8]	= stLocalControl.stBusDealFreq.byRecRoomAddr>>4;
			TelNum[9]	= stLocalControl.stBusDealFreq.byRecRoomAddr&0x0f;	
			TelNum[10]	= stLocalControl.stBusDealFreq.byRecBedAddr>>4;
			TelNum[11]	= stLocalControl.stBusDealFreq.byRecBedAddr&0x0f;*/

			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd          = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr	= CMD_GSM_NUM_DEL;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			

//			memset(stLocalControl.stEepromCfgData.byTelNum,0x00,TEL_NUM_LEN+1);	//����
//			stLocalControl.stEepromCfgData.byTelNumValidLen =0;
//			SaveParameter();
			break;
			
		case CMD_GSM_NUM_SEARCH:	//��ѯ�ֻ�����		
			for(i=0;i<TEL_NUM_LEN+1;i++)
			{
				if(stLocalControl.stEepromCfgData.byTelNum[i]== 0x0f) break;
			}

			if(i<TEL_NUM_LEN+1)
			{//�ҵ��绰����
				switch(i)
				{
					case 0:	//û��1������
						return;
					case 1:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+0x0f;
						break;
					case 2:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr  =0xf0;
						break;
					case 3:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+0x0f;						
						break;
					case 4:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr   =0xf0;
						break;
					case 5:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];						
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+0x0f;
						break;
					case 6:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr   =0xf0;
						break;
					case 7:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+0x0f;				
						break;
					case 8:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];						
						stLocalControl.stBusDealFreq.byRecRoomAddr 	=0xf0;
						break;
					case 9:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];						
						stLocalControl.stBusDealFreq.byRecRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[8]<<4)+0x0f;						
						break;
					case 10:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];
						stLocalControl.stBusDealFreq.byRecRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[8]<<4)+stLocalControl.stEepromCfgData.byTelNum[9];						
						stLocalControl.stBusDealFreq.byRecBedAddr	=0xf0;
						break;
					case 11:
						stLocalControl.stBusDealFreq.bySndSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[0]<<4)+stLocalControl.stEepromCfgData.byTelNum[1];
						stLocalControl.stBusDealFreq.bySndRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[2]<<4)+stLocalControl.stEepromCfgData.byTelNum[3];
						stLocalControl.stBusDealFreq.bySndBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[4]<<4)+stLocalControl.stEepromCfgData.byTelNum[5];
						stLocalControl.stBusDealFreq.byRecSecAddr	=(stLocalControl.stEepromCfgData.byTelNum[6]<<4)+stLocalControl.stEepromCfgData.byTelNum[7];
						stLocalControl.stBusDealFreq.byRecRoomAddr	=(stLocalControl.stEepromCfgData.byTelNum[8]<<4)+stLocalControl.stEepromCfgData.byTelNum[9];						
						stLocalControl.stBusDealFreq.byRecBedAddr	=(stLocalControl.stEepromCfgData.byTelNum[10]<<4)+0x0f;						
						break;
				}
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			}
			break;
			
		case CMD_GSM_NUM_SET:
			TelNum[0]	= stLocalControl.stBusDealFreq.bySndSecAddr>>4;
			TelNum[1]	= stLocalControl.stBusDealFreq.bySndSecAddr&0x0f;
			TelNum[2]	= stLocalControl.stBusDealFreq.bySndRoomAddr>>4;
			TelNum[3]	= stLocalControl.stBusDealFreq.bySndRoomAddr&0x0f;	
			TelNum[4]	= stLocalControl.stBusDealFreq.bySndBedAddr>>4;
			TelNum[5]	= stLocalControl.stBusDealFreq.bySndBedAddr&0x0f;	
			TelNum[6]	= stLocalControl.stBusDealFreq.byRecSecAddr>>4;
			TelNum[7]	= stLocalControl.stBusDealFreq.byRecSecAddr&0x0f;
			TelNum[8]	= stLocalControl.stBusDealFreq.byRecRoomAddr>>4;
			TelNum[9]	= stLocalControl.stBusDealFreq.byRecRoomAddr&0x0f;	
			TelNum[10]	= stLocalControl.stBusDealFreq.byRecBedAddr>>4;
			TelNum[11]	= stLocalControl.stBusDealFreq.byRecBedAddr&0x0f;
			for(i=0;i<TEL_NUM_LEN+1;i++)
			{
				if(TelNum[i]==0x0f) break;
			}
			if(i>=TEL_NUM_LEN+1) break;		//��Ч�绰����
			stLocalControl.stEepromCfgData.byTelNumValidLen	=	i;			
			memcpy(stLocalControl.stEepromCfgData.byTelNum,TelNum,stLocalControl.stEepromCfgData.byTelNumValidLen+1);

			stLocalControl.stBusDealFreq.bySndSecAddr 	= stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr	= stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr	= stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd          = CMD_BUS_ANSWER;
			stLocalControl.stBusDealFreq.byRecSecAddr	= CMD_GSM_NUM_SET;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			
//			SaveParameter();
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
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;
	EA=1;	

	//���½���������
	//������
#if DEBUG==1	
    Send_Data(&(stLocalControl.stBusDealFreq.bySndSecAddr),7);
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

		case CMD_COMM_CALL:	 								//��ͨ�������� 							
		case CMD_INFUSION_ANSWER:							//������Һ����
		case CMD_SERVICE_ANSWER:							//����������
		case CMD_EMERGENCY_ANSWER:							//�����������
		case CMD_HELP_ANSWER:								//������Ԯ����
			//ֹͣ����ָʾ�ĺ���
			if(bIndicatingOther)
			{	
				bIndicatingOther = 0; 				
				VoiceChannelCtx();
				LedControl();							
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
			{//GSMģ�鲻���ڱ�����豸������Ե���У�Ȼ��GSMģ�鷢��Ӧ��

	 					
			}
			break;
		case CMD_CALL_LISTEN:								//��������
			if(bBusy)
			{//GSMģ�鲻�����������ʹ�����

			}
			break;
			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   										//�㲥����
			//GSMģ�鲻���������㲥����
			break;
			
		case CMD_INFUSION_CLEAR:							//�����Һ����
		case CMD_SERVICE_CLEAR:								//����������
		case CMD_HELP_CLEAR:								//�����Ԯ����
		case CMD_EMERGENCY_CLEAR:							//�����������
			 if(bIndicatingOther &&
				(stLocalControl.stBusDealFreq.byRecSecAddr 	== stLocalControl.stIndicationData.bySndSecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.bySndRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr 	== stLocalControl.stIndicationData.bySndBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd - 0x0a) == (stLocalControl.stIndicationData.byCmd & 0x1f)))//��Ӧ�ĺ�������
			{	//�����������ָʾ�ĺ���  					
				bIndicatingOther = 0;
				VoiceChannelCtx();
				LedControl();													
			} 			
			break; 	

		case CMD_CHANNEL_CLOSE:								//�ر�����ͨ������
			bChannel1Talk  = 0; 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			LedControl();
			break;
			
		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 1;								//�������߹���
			SaveParameter();
			Bus0SendPin = 0;								//�ͷ�����				
			break;
			
		case CMD_SYSTERM_RESET:	 							//ϵͳ��λ����
			SysReset();
			break; 
			
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case CMD_POWER_ON:
					EA=0;
					//�ر����д򿪵��ж�
					CCAPM0 = 0x00;
					ET0 = 0;
					TR0 = 0;
					EX0 = 0;
					ISP_CONTR = 0x20; 	

					
				case CMD_SD_TAL_VOL_CHECK_END:
					XTD=0;
					break;

				case CMD_GSM_FUNC_SET:
					SaveParameter();
					break;
					
				case CMD_GSM_NUM_DEL:
					memset(stLocalControl.stEepromCfgData.byTelNum,0x00,TEL_NUM_LEN+1); //����
					stLocalControl.stEepromCfgData.byTelNumValidLen =0;
					SaveParameter();
					break;

				case CMD_GSM_NUM_SET:
					SaveParameter();
					break;

			}
			break;
			
		default:
			break;			

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
					else 
					if((bWaitAck|bWaitListen|bChannel0Talk))
					{	//�ȴ�Ӧ�𣬵ȴ�����������ͨ���������㲥״̬				
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;											
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

						GSMHandUpTreat();
					}								
					else
					{
						bBusy = bSickRoomBroad = bOfficeBroad = bAllBroad = 0;
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
	if(stLocalControl.stCH1TimerOut.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stLocalControl.stCH1TimerOut.byTimerOut--;
		if(stLocalControl.stCH1TimerOut.byTimerOut == 0)
		{	//��ʱһ�ε��� 
			if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
			{ 	//���г�ʱ���
				goto TIMER1OUTDEAL;
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH1TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
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
						
						GSMHandUpTreat();
					}

				}
				else
				{ 	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 				
					stLocalControl.stCH1TimerOut.byTimerOut = stLocalControl.stCH1TimerOut.byTimerOutSet;
				}
			}			
		}
	}	

	//��״̬����
	if(!bLanding)
	{
		if(--stLocalControl.byLedFlashTime == 0x00)
		{
			stLocalControl.byLedFlashTime = stLocalControl.byLedTimeSet;
			SYS_STATE_LED = !SYS_STATE_LED;
		}
	}
}
 	  
