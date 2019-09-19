/**********************************************************************************************************************/
//һ��֮�ǵ������շ�����
/**********************************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "singlebus.h"
#include "hzxsp.h"
#include "led.h"

//����0��������
INT16U byBus0State 	= 0;							//������0״̬��
#define bBus0StartRec		(1<<0)				//������0��ʼ������ʼλ��־
#define bBus0OnRec			(1<<1)				//������0��ʼ��������λ��־
#define  bBus0Enable		(1<<2)				//������0�����ͱ�־
#define  bBus0ReqSend		(1<<3)				//������0�����ͱ�־
#define  bBus0OnSendBit		(1<<4)				//������0���ڷ���һ������λ��־
#define  bBus0OnSendFreq	(1<<5)				//������0���ڷ���һ������֡��־
#define  bBus0SendError		(1<<6)				//������0���ʹ����־
#define  bBus0Error			(1<<7)				//������0���ϱ�־
#define  bBus0RecFinish		(1<<8)				//������0������ɱ�־
#define  bBus0RecBit9		(1<<9)				//������0�����ֽ����ݵĵ�9λ
#define  bBus0SendFinish	(1<<10)				//������0������ɱ�־
#define  bBus0Disable		(1<<11)				//������0��ֹ���ͱ�־
#define  bBus0SendBit		(1<<12)				//������0���ڷ��͵�����λ
#define  bBus0RecBit		(1<<13) 			//������0���ڽ��յ�����λ
#define nbBus0SendBit 12
#define SetBus0State(x) (byBus0State|=x)		//��λ����
#define ClrBus0State(x) (byBus0State&=~x)		//�������
#define GetBus0State(x) (byBus0State&x)			//��ȡλ״̬
INT8U byBus0RecCount = 0;						//��4λ������0���ն�ʱ�жϼ�������4λ�ǽ��յ���λ����
INT8U byBus0RecBuf;								//����0���ջ��嵥Ԫ
INT8U byBus0SendBuf;							//����0���ͻ��嵥Ԫ	
INT8U byBus0RecData[BUS0_FREQ_SIZE];			//����0���ջ�����
INT8U byBus0SendData[BUS0_FREQ_SIZE];			//����0���ͻ�����
INT8U byBus0SendCount = 0;						//��4λ������0���Ͷ�ʱ�жϼ�������4λ�Ƿ��͵�λ����
INT8U byBus0RecSendCount = 0;					//��4λ������0���յ����ֽڼ�������4λ������0��������ֽڼ���
INT8U byBus0SendStopCount;						//����0ֹͣʱ�����
INT8U byBus0RecTimeOut = 0;						//����0���ճ�ʱ����
INT8U byBus0DisableCount = 0;					//����0��ֹ����	 

INT8U byBus0TxQ[BUS0_TX_Q_ZISE];				//����0���Ͷ���							
INT8U byBus0TxHead = 0;							//������0���Ͷ���ͷָ��
INT8U byBus0TxTail = 0; 						//������0���Ͷ���βָ��
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 

INT8U byBus0RxQ[BUS0_RX_Q_ZISE];				//����0���ն���							
INT8U byBus0RxHead = 0;							//������0���ն���ͷָ��
INT8U byBus0RxTail = 0; 						//������0���ն���βָ��
#define IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE) 

//��������
STBusFreq stBusDealFreq;									//����������ʱ����

//---------------------------------
//----------------------------------
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
	//��ʼ�����ͽ���IO
    LPC_IOCON->PIO1_4&=~0X07;
#ifdef RELEASE
	LPC_IOCON->PIO1_11&=~0X07;
	LPC_GPIO1->DIR  &= ~Bus0RecPin;
#else
	LPC_IOCON->PIO2_6&=~0X07;
	LPC_GPIO2->DIR  &= ~Bus0RecPin;
#endif
	LPC_GPIO1->DATA &= ~Bus0SendPin;
	LPC_GPIO1->DIR  |= Bus0SendPin;
	//��ʼ���������IO
	LPC_IOCON->PIO2_8&=~0x07;
	ClrProtectPin();
	LPC_GPIO2->DIR |= ProtectPin;
	ClrProtectPin();
	//��ʼ����ʱ����Ϊ����ʹ��
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);                              /* �򿪶�ʱ��ģ��               */

    LPC_TMR32B0->IR      = 1;
    LPC_TMR32B0->PR      = 1000;                                        /* ���÷�Ƶϵ��                 */
    LPC_TMR32B0->MCR     = 3;                                           /* ����MR0ƥ���λTC�������ж�*/
    LPC_TMR32B0->MR0     = SystemFrequency/10000000;                       /* ����100uS�ж�ʱ��             */
    LPC_TMR32B0->TCR     = 0x01;                                        /* ������ʱ��                   */
    NVIC_EnableIRQ(TIMER_32_0_IRQn);                                    /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER_32_0_IRQn, 1);
	byBus0SendStopCount = 230;
	//ʹ�ܽ����ж�(��������Ч)
#ifdef RELEASE
		LPC_GPIO1->IS &= ~Bus0RecPin; //�����ж�
		LPC_GPIO1->IEV |= Bus0RecPin; //�������ж�
		LPC_GPIO1->IE |= Bus0RecPin; //�����ж�
		NVIC_EnableIRQ(EINT1_IRQn);                                    /* �����жϲ�ʹ��               */
    	NVIC_SetPriority(EINT1_IRQn, 2);
#else
		LPC_GPIO2->IS &= ~Bus0RecPin; //�����ж�
		LPC_GPIO2->IEV |= Bus0RecPin; //�������ж�
		LPC_GPIO2->IE |= Bus0RecPin; //�����ж�
		NVIC_EnableIRQ(EINT2_IRQn);                                    /* �����жϲ�ʹ��               */
    	NVIC_SetPriority(EINT2_IRQn, 2);
#endif	
}
/**********************************************************
*��������			:Bus0RecInt	
*��������        	:�ⲿ�ж�0����,������0�����ж�
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
#ifdef RELEASE
void PIOINT1_IRQHandler(void)
{
	LPC_GPIO1->IC |= Bus0RecPin;
#else
void PIOINT2_IRQHandler(void)
{
	LPC_GPIO2->IC |= Bus0RecPin;
#endif								          
	DisableBus0RecInt();					//��ֹ�ٴ��½����ж�
	SetBus0State(bBus0StartRec);			//������ʼλ�ؼ��
	ClrBus0State(bBus0Enable);				//��ֹ���߷���							
	ClrBus0State(byBus0RecCount);						//����ռĴ��� 
	uartSendByte(GetBus0RecPin());
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
void TIMER32_0_IRQHandler (void)
{ 
	static INT16U display_tick=0;
	INT8U  byTemp;
    LPC_TMR32B0->IR = 0x01;                                             /* ����жϱ�־                 */
	//��ʾˢ����ʱ����
	display_tick++;
	if (display_tick>=9) //�����1ms �Ķ�ʱ
	{
		display_tick=0;
	   //������ʾ����
	   ShowNextLine();
	   PutDataToSSP();
	}
	else
	{
		if (screen.DISP_first==1&&(LPC_SSP0->SR&0x10)==0)
		{
			PutDataToSSP();
		}
	}
	if ((SystemInfo.state&(SYSTEM_STATE_TESTMODE|SYSTEM_STATE_LOCAL_ERROR))==0) //�ǲ���ģʽ����������
	{
		if (GetBus0RecPin()) 
		{
			ClrBus0State(bBus0RecBit);
		}
		else 
		{
			SetBus0State(bBus0RecBit);
			SystemInfo.error=0;
		}
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------	           
		/*******************************����0���մ���***********************************/
		if(GetBus0State(bBus0StartRec))							//�ж�����������״̬�����յ���ʼλ
		{	
					
			byBus0RecCount += 0x10;					//���Ӷ�ʱ�жϼ�������
			if(0x50 == (byBus0RecCount & 0xf0))		//��������ʼλ���ʱ��
			{			
				ClrBus0State(bBus0StartRec);
				byBus0RecCount = 0x00;				//���¿�ʼ����			
				if(GetBus0State(bBus0RecBit))			
				{ 	//��Ч��ʼλ																		
					if((!GetBus0State(bBus0SendError))&&GetBus0State(bBus0OnSendFreq))
					{	//û�з��������߷��ʹ�������һ֡�������ڷ��ͣ�ֹͣ֡���ݷ��ͣ��÷��ʹ����־						
						byBus0State &= (~BUS0_SEND_CON);
						SetBus0State(bBus0SendError);
						ClrBus0SendPin();			//�ͷ�����	 				
					}				
					byBus0SendStopCount = 240;					
					byBus0RecSendCount = 0x00;		//���ճ������ý��շ��ͼ���ֵ
					EnableBus0RecInt();
				}
				else								
				{	//��Ч��ʼλ
					SetBus0State(bBus0OnRec);					//��ʼ��������λ 											
				}
			}
		}
		else if(GetBus0State(bBus0OnRec))
		{	
			byBus0RecCount += 0x10;					//���Ӷ�ʱ�жϼ�������	 		
			if(0xa0 == (byBus0RecCount & 0xf0))
			{
				byBus0RecCount &= 0x0f;				//�����ʱ�жϼ�������
				byBus0RecCount ++;
				if(0x0a == (byBus0RecCount & 0x0f))	
				{	//�յ���10λ,����λ	  				
					ClrBus0State(bBus0OnRec);					//ֹͣ���ݽ���					
					if(GetBus0State(bBus0RecBit))
					{	//��Ч�Ľ���λ						
						if(((byBus0RecSendCount & 0xf0)==0&&GetBus0State(bBus0RecBit9)==0)||((byBus0RecSendCount & 0xf0)!=0&&GetBus0State(bBus0RecBit9)!=0)) 
						{	//���������
							byBus0RecTimeOut = 0;
							byBus0RecSendCount &= 0x0f;											
						}
						else 
						{	//��������ȷ
											
							byBus0RecTimeOut = 230;	//������һ���ֽ����ݽ��ճ�ʱʱ��
							byBus0RecData[byBus0RecSendCount>>4] = byBus0RecBuf;
							byBus0RecSendCount += 0x10;
															
							if((byBus0RecSendCount & 0xf0) >= BUS0_FREQ_SIZE_HI)
							{																						
								byBus0RecSendCount &= 0x0f;															
								//������յ�����֡�������Լ����͵Ĳ��ñ�־
								if(!((GetBus0State(bBus0OnSendFreq) != 0)&&(GetBus0State(bBus0ReqSend) == 0)))
								{
																						  																						
									if(Bus0RxBuffLen() >= (BUS0_RX_Q_ZISE - 1))
									{	//û�пռ�洢��,ʧ��	
										SetBus0State(bBus0RecFinish);
									}	
									else
									{
										for (byTemp=0; byTemp<BUS0_FREQ_SIZE; byTemp++)
										{	//��������ն���
											byBus0RxQ[byBus0RxTail] = byBus0RecData[byTemp];
											IncBus0RxPtr(byBus0RxTail);
										}
									}
								} 																		
								byBus0RecTimeOut = 0;
								byBus0DisableCount = 10;
								SetBus0State(bBus0Disable);	//��ֹ����ʹ��						
							}	
						}				
						byBus0SendStopCount = 240;
						EnableBus0RecInt();						
					}
					else							//��Ч����λ
					{
						SetBus0State(bBus0Error);										
						if((!GetBus0State(bBus0SendError)) && GetBus0State(bBus0OnSendFreq))
						{	//û�з��������߷��ʹ�������һ֡�������ڷ��ͣ�ֹͣ֡���ݷ��ͣ��÷��ʹ����־
							byBus0State &= (~BUS0_SEND_CON);
							SetBus0State(bBus0SendError);							
							ClrBus0SendPin();		//�ͷ�����
						}
						byBus0RecSendCount = 0x00;	//���ճ������ý��շ��ͼ���ֵ
					}
				}
				else if(0x09 == (byBus0RecCount & 0x0f))
				{	//��9λ����
					if (GetBus0State(bBus0RecBit)) SetBus0State(bBus0RecBit9); else ClrBus0State(bBus0RecBit9);					
				}
				else 								//��Ч����λ
				{ 
					byBus0RecBuf >>= 1;
					if(GetBus0State(bBus0RecBit))
					{	//Ϊ�ߵ�ƽ
						byBus0RecBuf |= 0x80;
					}	
				}
			}
		}
		/*******************************����0���ʹ���***********************************/
		if((byBus0State & BUS0_CAN_SEND) == BUS0_CAN_SEND)
		{	//����0�������ݷ�������������������		
			if(GetBus0State(bBus0RecBit))
			{	//��������,���Է���
				SetBus0SendPin();
				ClrBus0State(bBus0SendBit);					//������ʼλ���� 			
				byBus0SendCount = 0;
				byBus0State &= (~BUS0_CAN_SEND);
				byBus0SendBuf = byBus0SendData[byBus0RecSendCount & 0x0f];
				SetBus0State(bBus0OnSendBit);					//ȡ�������͵����ݲ������ڷ��ͱ�־		
			}
			else
			{	//���߲�����,ֹͣ����
				byBus0State &= (~BUS0_SEND_CON);
				SetBus0State(bBus0SendError);			
				byBus0RecSendCount &= 0xf0;
				ClrBus0SendPin();
				byBus0SendStopCount = 240;
			}
		}
		else if(GetBus0State(bBus0OnSendBit))
		{ 	//������λ���ڷ���,���ȷ��͵�����ʼλ			
			if((GetBus0State(bBus0SendBit)==0&&GetBus0State(bBus0RecBit)==0)||(GetBus0State(bBus0SendBit)!=0&&GetBus0State(bBus0RecBit)!=0))   		
			{	//���͵����ݺͽ��յ�������ͬ
				byBus0SendCount += 0x10;
				if(0xa0 == (byBus0SendCount & 0xf0))
				{	//һλ���ݷ������,���ȷ��͵�����ʼλ
					byBus0SendCount &= 0x0f;
					byBus0SendCount += 0x01;				
					if(0x09 == (byBus0SendCount & 0x0f))
					{	//���͵���9λ��
						if ((byBus0RecSendCount&0x0f)==0) 
						{
							SetBus0State(bBus0SendBit);
							ClrBus0SendPin();
						}
						else
						{
							ClrBus0State(bBus0SendBit);
							SetBus0SendPin();
						}					
					}
					else if(0x0a == (byBus0SendCount & 0x0f))
					{	//���͵�����λ��
						SetBus0State(bBus0SendBit);
						ClrBus0SendPin();	
					}
					else if(0x0b == (byBus0SendCount & 0x0f))
					{	//�Ѿ����������λ��
						ClrBus0State(bBus0OnSendBit);						
						byBus0RecSendCount += 0x01;									
						if((byBus0RecSendCount & 0x0f) >= BUS0_FREQ_SIZE)
						{	//������һ֡����												
							byBus0RecSendCount &= 0xf0;		//���½�������֡�ķ��ͽ׶�
							byBus0State &= (~BUS0_SEND_CON);
							byBus0SendStopCount = 240;
							byBus0State |= BUS0_SEND_FINISH;								
							byBus0DisableCount = 10;								
						}
						else
						{  						
							byBus0SendStopCount = 10;
							SetBus0State(bBus0ReqSend);
						}
						EnableBus0RecInt();			//�ٴ�ʹ�ܽ����ж�
					}
					else
					{
						if(byBus0SendBuf & 0x01)
						{	//���͸ߵ�ƽ
							SetBus0State(bBus0SendBit);
							ClrBus0SendPin();					
						}
						else
						{	//���͵͵�ƽ
							ClrBus0State(bBus0SendBit);
							SetBus0SendPin();
						}
						byBus0SendBuf >>= 1;
					}
				}
			}
			else
			{	//����ͬ,����ʧ��								
				byBus0State &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				SetBus0State(bBus0SendError);					
				ClrBus0SendPin(); 
				byBus0SendStopCount = 240;
			}
		} 	
		/*******************************����0���ƴ���***********************************/
		if(0 == GetBus0State(BUS0_ON_REC))
		{
			if(byBus0SendStopCount != 0)
			{
				if((--byBus0SendStopCount) == 0)
				{				
					SetBus0State(bBus0Enable);								
				}
			}		
			if(GetBus0State(bBus0Error))
			{								
				ClrBus0State(bBus0Enable);			
				if(GetBus0State(bBus0RecBit))
				{				
					ClrBus0State(bBus0Error);
					EnableBus0RecInt();
					byBus0SendStopCount = 240;
				}
			}
		} 	
		/*******************************����0��ʱ����***********************************/
		if(byBus0RecTimeOut != 0)
		{
			if(--byBus0RecTimeOut == 0)				//���ճ�ʱ��
			{
				byBus0RecSendCount &= 0x0f;
			}
		}
		if(byBus0DisableCount != 0)
		{
			if(--byBus0DisableCount == 0)			//��ֹ��ʱ��
			{			
				ClrBus0State(bBus0Disable);
			}
		} 
		/***********����0�Զ����͹���**********/	 
		if((byBus0State & BUS0_ON_WORK) == 0x00)	//����0û�й���
		{				
			if(GetBus0State(bBus0SendError))		//����0�з��ʹ���
			{	//�����˷��ʹ���,�Զ��ط�								
				ClrBus0State(bBus0SendError);				
				byBus0State |= BUS0_REQ_SEND;		
			}
			else									//����0�޷��ʹ���
			{		
				if(GetBus0State(bBus0Disable)==0)
				{	//����0û�н�ֹʹ��,�ҷ��ͽ��������Ѿ����
					if(Bus0TxBuffLen() >= BUS0_FREQ_SIZE)
					{  																		
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
						
						SetBus0State(BUS0_REQ_SEND);						
					}
					else
					{
						byBus0TxHead = byBus0TxTail = 0;
					}								
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
INT8U Bus0OutputData(INT8U * pbyData)
{
	INT8U byTemp = BUS0_FREQ_SIZE;

	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		return(FALSE);
	}	
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	return(TRUE);	
}


/**********************************************************
*��������			:BcdToHex	
*��������        	:BCDת����ʮ������
*�������   		:byData:��ת����BCD��
*����ֵ				:byRet:ת�����16��������
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
INT8U BcdToHex(INT8U byData)
{
	INT8U byRet;
	byRet = byData >> 4;
	byRet *= 10;
	byData &= 0x0f;
	byRet += byData;
	return(byRet);
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
	switch(stBusDealFreq.byCmd)
	{
		case CMD_ENTER:										//�յ�ȷ������
			SystemInfo.state |= SYSTEM_STATE_ONLINE; //��ϵͳ����״̬			
			break; 
		case CMD_QUEST:										//��ѯ����
			stBusDealFreq.bySndSecAddr = SystemInfo.LOCAL_ADDR;
			stBusDealFreq.bySndRoomAddr = 0;
			stBusDealFreq.bySndBedAddr = 0;
			stBusDealFreq.byCmd = CMD_ANSWER; 			
			Bus0OutputData(&(stBusDealFreq.bySndSecAddr));
			break;
		//2012/4/9����ϵͳ������
		case CMD_SET_BAUD:
			/*uartSendByte(stBusDealFreq.byRecSecAddr);
			uartSendByte(stBusDealFreq.byRecRoomAddr);
			uartSendByte(stBusDealFreq.byRecBedAddr); */
		    SystemInfo.UartBaud=stBusDealFreq.byRecSecAddr;
			eepromWrite((INT8U*)&SystemInfo); //�������
	        UartReInit(SystemInfo.UartBaud); //�����µĲ�����
		    break;
		//------------------------------------------------------
		//------------------------------------------------------
		case CMD_DATA_SEND:									//�յ�Уʱ����
			SystemInfo.sTime.second = BcdToHex(stBusDealFreq.bySndSecAddr & 0x7f);
			SystemInfo.sTime.minute = BcdToHex(stBusDealFreq.bySndRoomAddr & 0x7f);
			SystemInfo.sTime.hour   = BcdToHex(stBusDealFreq.bySndBedAddr & 0x3f);
			SystemInfo.sTime.day    = BcdToHex(stBusDealFreq.byRecSecAddr & 0x3f);
			SystemInfo.sTime.month  = BcdToHex(stBusDealFreq.byRecRoomAddr & 0x1f);
			SystemInfo.sTime.year   = BcdToHex(stBusDealFreq.byRecBedAddr);			
			break;
		case CMD_INFO_INDICATION:  							//��Ϣָʾ����		
			SystemInfo.DisplayInfo.CallBuf[0] =  stBusDealFreq.byRecSecAddr&0x1f;  //��������
			SystemInfo.DisplayInfo.CallBuf[1] =  stBusDealFreq.bySndSecAddr;		//����
			SystemInfo.LOCAL_ADDR = stBusDealFreq.bySndSecAddr; //�洢Ϊ��������
			SystemInfo.DisplayInfo.CallBuf[2] =  stBusDealFreq.bySndRoomAddr;		//����
			SystemInfo.DisplayInfo.CallBuf[3] =  stBusDealFreq.bySndBedAddr;		//����
			SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CALL;
			SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
			break;
		case CMD_NURSE_COME:		//��ʿ��λ����
			InsertNurse(stBusDealFreq.byRecRoomAddr);
			break;	
		case CMD_NURSE_BACK:	   //��ʿ�뿪����
			DeleteNurse(stBusDealFreq.byRecRoomAddr);
			break;
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
		case CMD_INFUSION_CLEAR:
		case CMD_SERVICE_CLEAR:
		case CMD_EMERGENCY_CLEAR:
		case CMD_HELP_CLEAR:								//������л����������ָ��
			//�Ƚ�����ָʾ�ĵ�ַ�봦��ĵ�ַ
			if((stBusDealFreq.byRecSecAddr != 0xff) && (stBusDealFreq.byRecSecAddr != SystemInfo.DisplayInfo.CallBuf[1]))
			{	//����ַ����ȷ
				break;
			}
			if((stBusDealFreq.byRecRoomAddr != 0xff) && (stBusDealFreq.byRecRoomAddr != SystemInfo.DisplayInfo.CallBuf[2]))
			{	//����ַ����ȷ
				break;
			}
			if((stBusDealFreq.byRecBedAddr != 0xff) && (stBusDealFreq.byRecBedAddr != SystemInfo.DisplayInfo.CallBuf[3]))
			{	//����ַ����ȷ
				break;
			} 
		case CMD_STOP_INDICATION:
			SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CALL;
			SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
			break;
		case CMD_POWER_ON:	  								//��������������
			//if(AddrCompare(&stBusDealFreq))
			//{	//���͵�������������� 				
				while(1);			
			//}
			break;			
	}
	SystemInfo.state|=SYSTEM_STATE_ONLINE; //��ʶ����״̬		
}


//���������ݽ������ﴦ��
void Bus0Manager(void)
{
	INT8U tx_buf[7];
	//����ɽ����ź�
	if (GetBus0State(bBus0RecFinish))
	{
		memcpy((void*)tx_buf,(void*)byBus0RecData,sizeof(STBusFreq));
		ClrBus0State(bBus0RecFinish);
		memcpy((void*)&stBusDealFreq, tx_buf, sizeof(STBusFreq));
		Bus0RecDeal();

	}
	//���ն�����������δ����
	if(Bus0RxBuffLen() >= BUS0_FREQ_SIZE)
	{  																		
		tx_buf[0] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[1] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[2] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[3] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[4] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[5] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		tx_buf[6] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		if(Bus0RxBuffLen() < BUS0_FREQ_SIZE) byBus0TxHead = byBus0TxTail = 0;
		memcpy((void*)&stBusDealFreq, tx_buf, sizeof(STBusFreq));
		Bus0RecDeal();
	}
}

