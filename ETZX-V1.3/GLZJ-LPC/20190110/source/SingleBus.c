/**********************************************************************************************************************/
//һ��֮�ǵ������շ�����
/**********************************************************************************************************************/
#define _IN_SINGLE_BUS_
#include"custom.h"

//����0��������
uint16_t byBus0State 	= 0;							//������0״̬��
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
#define SetBus0State(x) (byBus0State|=(x))		//��λ����
#define ClrBus0State(x) (byBus0State&=~(x))		//�������
#define GetBus0State(x) (byBus0State&(x))			//��ȡλ״̬
uint8_t byBus0RecCount = 0;						//��4λ������0���ն�ʱ�жϼ�������4λ�ǽ��յ���λ����
uint8_t byBus0RecBuf;								//����0���ջ��嵥Ԫ
uint8_t byBus0SendBuf;							//����0���ͻ��嵥Ԫ	
uint8_t byBus0RecData[BUS0_FREQ_SIZE];			//����0���ջ�����
uint8_t byBus0SendData[BUS0_FREQ_SIZE];			//����0���ͻ�����
uint8_t byBus0SendCount = 0;						//��4λ������0���Ͷ�ʱ�жϼ�������4λ�Ƿ��͵�λ����
uint8_t byBus0RecSendCount = 0;					//��4λ������0���յ����ֽڼ�������4λ������0��������ֽڼ���
uint8_t byBus0SendStopCount;						//����0ֹͣʱ�����
uint8_t byBus0RecTimeOut = 0;						//����0���ճ�ʱ����
uint8_t byBus0DisableCount = 0;					//����0��ֹ����	 

uint8_t byBus0TxQ[BUS0_TX_Q_ZISE];				//����0���Ͷ���							
uint8_t byBus0TxHead = 0;							//������0���Ͷ���ͷָ��
uint8_t byBus0TxTail = 0; 						//������0���Ͷ���βָ��
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE) 

uint8_t byBus0RxQ[BUS0_RX_Q_ZISE];				//����0���ն���							
uint8_t byBus0RxHead = 0;							//������0���ն���ͷָ��
uint8_t byBus0RxTail = 0; 						//������0���ն���βָ��
#define IncBus0RxPtr(addr)    {addr=(addr+1)%BUS0_RX_Q_ZISE;}
#define Bus0RxBuffLen()	      ((byBus0RxTail+BUS0_RX_Q_ZISE-byBus0RxHead)%BUS0_RX_Q_ZISE) 

//��������
uint8_t 			byChar;
uint8_t				byWaitAckTimeLanding=0;				//�ϵ�Ǽ�ʱ�ȴ�PC����Ӧ��ʱ��
uint16_t			uiDevState=0; 						//�豸��ǰ������״̬
uint16_t			uiDetectPcTime=0;					//����PC�����ʱ��	
uint8_t				byVoicePlayTime=0;					//���Ŷ�ʱ��
STBusFreq			stBusFreq;							//���������ݴ���ṹ��	
STTimerout			stCH0TimerOut;						//��ʱ����ṹ��
STTimerout			stCH1TimerOut;						//ͨ��1ͨ����ʱ����ṹ��	
uint8_t				byWaitCommAckDTime=0;				//ͨ�õȴ���ʱʱ��
uint8_t 			byUsart2HandingDTime=0;				//����2���ڴ���ʱʱ��
uint8_t 			byUsart0SdResultDTime=0;			//����2���ͽ������ʱʱ��
STPointer			stCallPointer;						//���ж���˫������
STIndicationData	stIndicationData;					//��Ϣָʾ���ݽṹ��
//STVoicePlay 		stVoicePlay;						//�������ſ���
STAddr				stCallAddr; 						//ͨ��0�������з���ַ
STAddr				stCH1CallAddr; 						//ͨ��1�������з���ַ
STAddr_S			stRS485Addr;						//RS485���ݽ��շ���ַ
uint8_t				byWaitRS485AckDTime=0;				//ͨ�õȴ���ʱʱ��
STEepromCfgData 	stEepromCfgData;					//���������ֽṹ��
uint8_t				byReSndBrtTimes=0;					//�ظ��������ò����ʵĴ���
uint8_t				byReSndRs485RoomTimes=0;			//�ظ����ʹ�ͷ�ֻ�����RS485��Ϣ�Ƿ���Ҫ�жϷ��ŵĴ���
uint8_t 			byReSndBedVoiPlaTimes=0;			//�ظ�������������ʱ��ͷ�ֻ��Ƿ��������ŵĴ���
STAddr_S			stCommAddr;							//ͨ�õ�ַ

//---------------------------------


uint8_t TimerOutCount=0;			//20ms��Ԫ��ʱ
uint8_t bBus0SndBitOk=0;



uint16_t State	=0;								//R485��ر�־
/*#define  bRS485DataTreat 	(1<<0)				//RS485���ݴ����־
#define  bOriginMusic		(1<<1)				//�������ֳ�ʼ״̬
#define  bPCOpen			(1<<2)				//��λ����״̬
#define  bTimerOutDeal		(1<<3)				//TimerOutDeal()���������־
#define  bLedState			(1<<4)				//LED��״̬
#define  bPowerEnState		(1<<5)				//�ź��ߵ�Դ״̬


#define SetState(x) (State|=(x))			//��λ����
#define ClrState(x) (State&=~(x))		//�������
#define GetState(x) (State&(x))			//��ȡλ״̬

*/

uint16_t BusErrorState=0;


STTime	stTime;
uint8_t byYearH=0x20;		//����ֽ�


void SingleBusInit(void);
void TIMER1_IRQHandler (void);			 //100us��ʱ
uint8_t Bus0OutputData(uint8_t *pbyData);
void  eint3Isr (void);
void save_parameter(void);
void Bus0Manager(void);
void SaveCallAddr(pSTBusFreq pstBusFreq);
uint8_t AddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
uint8_t DirAddrCompare(uint8_t *pAddrA,uint8_t *pAddrB);
void MakeCH0TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
void MakeCH1TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount);
uint8_t AddCallNod(pSTBusFreq pstBusFreq);
void RemoveCallNod(pSTBusFreq pstBusFreq);
void SaveIndicationData(pSTBusFreq pstBusFreq, uint8_t byFirstNod);
void VoiceChannelCtx(void);
void RestartIndication(void);
void LedStateFlash(void);
void Bus0RecDeal(void);
void Bus0SendFinishDeal(void);
void TimerOutDeal(void);
//----------------------------------


uint8_t check_ee_485brt(void)
{
	if(stEepromCfgData.uiRs485Brt==57600) return 1;
	if(stEepromCfgData.uiRs485Brt==38400) return 1;
	if(stEepromCfgData.uiRs485Brt==28800) return 1;
	if(stEepromCfgData.uiRs485Brt==19200) return 1;
	if(stEepromCfgData.uiRs485Brt==14400) return 1;
	if(stEepromCfgData.uiRs485Brt==9600) return 1;
	if(stEepromCfgData.uiRs485Brt==4800) return 1;
	if(stEepromCfgData.uiRs485Brt==2400) return 1;
	if(stEepromCfgData.uiRs485Brt==1200) return 1;
	return 0;
}




/**********************************************************
*��������			:init_parameter	
*��������        	:������ʼ��
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
void init_parameter(void)
{	
	sst_read(&(stEepromCfgData.byInitFlag),PARA_ADDR,sizeof(STEepromCfgData));
//	uart2_send_str2(&(stEepromCfgData.byInitFlag),sizeof(STEepromCfgData));

	
	if(stEepromCfgData.byInitFlag != INIT_FLAG)
	{
para_init:
		stEepromCfgData.byInitFlag = INIT_FLAG;
		stEepromCfgData.bySelfSecAddr = 0x01;
		stEepromCfgData.bySelfRoomAddr = GLZJ_ADDR2;
		stEepromCfgData.bySelfBedAddr = GLZJ_ADDR3;
		stEepromCfgData.uiRs485Brt	= 57600;
		stEepromCfgData.byBLOffStartHour = 0x20;
		stEepromCfgData.byBLOffStartMin = 0x0;
		stEepromCfgData.byBLOffEndHour = 0x6;
		stEepromCfgData.byBLOffEndMin = 0x0;
		stEepromCfgData.byLVoiceStartHour = 0x20;
		stEepromCfgData.byLVoiceStartMin =0x0;
		stEepromCfgData.byLVoiceEndHour = 0x8;
		stEepromCfgData.byLVoiceEndMin =0x0;
		stEepromCfgData.byLVoiceVal =5;
		stEepromCfgData.bPlayRoom =0;
		
		stEepromCfgData.byHostPlayVol = 8;
		stEepromCfgData.bySlaPlayVol  =8;

		stEepromCfgData.byHostPlayLittleVol = 5;
		stEepromCfgData.bySlaPlayLittleVol =5;
		
		stEepromCfgData.byHostTalkVol = 28;
		stEepromCfgData.bySlaTalkVol = 20;

		
		stEepromCfgData.byVersionH = 0x01;
		stEepromCfgData.byVersionL =0x00;

		stEepromCfgData.bySysMode = 0xf0;		//��ʼʱ��ʹ��WIFIͨѸ

		save_parameter();

	}
	else if( stEepromCfgData.bySelfRoomAddr!=GLZJ_ADDR2 || stEepromCfgData.bySelfBedAddr!= GLZJ_ADDR3)
	{
		goto para_init;
	}
	else if(check_ee_485brt()==0)
	{
		goto para_init;
	}
	
}


void reset_parameter(void)
{
	stEepromCfgData.byInitFlag = INIT_FLAG;
	stEepromCfgData.bySelfSecAddr = 0x01;
	stEepromCfgData.bySelfRoomAddr = GLZJ_ADDR2;
	stEepromCfgData.bySelfBedAddr = GLZJ_ADDR3;
	stEepromCfgData.uiRs485Brt	= 57600;
	stEepromCfgData.byBLOffStartHour = 0x20;
	stEepromCfgData.byBLOffStartMin = 0x0;
	stEepromCfgData.byBLOffEndHour = 0x6;
	stEepromCfgData.byBLOffEndMin = 0x0;
	stEepromCfgData.byLVoiceStartHour = 0x20;
	stEepromCfgData.byLVoiceStartMin =0x0;
	stEepromCfgData.byLVoiceEndHour = 0x8;
	stEepromCfgData.byLVoiceEndMin =0x0;
	stEepromCfgData.byLVoiceVal =5;
	stEepromCfgData.bPlayRoom =0;
	
	stEepromCfgData.byHostPlayVol = 8;
	stEepromCfgData.bySlaPlayVol  =8;

	stEepromCfgData.byHostPlayLittleVol = 5;
	stEepromCfgData.bySlaPlayLittleVol =5;

	
	stEepromCfgData.byHostTalkVol = 28;
	stEepromCfgData.bySlaTalkVol = 20;
	
	
	stEepromCfgData.byVersionH = 0x01;
	stEepromCfgData.byVersionL =0x00;
	
	save_parameter();

}



void save_parameter(void)
{
//	uint8_t eeprom_buf[20];
	
//	uart2_send_str2(&(stEepromCfgData.byInitFlag),sizeof(STEepromCfgData));
	sst_write(&(stEepromCfgData.byInitFlag),PARA_ADDR,sizeof(STEepromCfgData));	

//	sst_read(eeprom_buf,PARA_ADDR,sizeof(STEepromCfgData));
//	uart2_send_str2(eeprom_buf,sizeof(STEepromCfgData));
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

	//��ʼ�����ͽ���IO	
	LPC_PINCON->PINSEL4 &= (~(0x03 << 26)); 			// P2.13  TX ��̬Ϊ1
	LPC_GPIO2->FIODIR    |= sd_tx_pin;
	sd_tx_ctrl(1);
	
		
	LPC_PINCON->PINSEL0 &= (~(0x03 << 2)); 			    // P0.1  rx
	LPC_GPIO2->FIODIR	&= ~sd_rx_pin;

	
	//��ʼ����ʱ����Ϊ����ʹ��
	init_timer1();
	byBus0SendStopCount = 240;

	
	//ʹ�ܽ����ж�(�½�����Ч)
	if (GetBus0RecPin()) 
	{
		IO0IntClr =  Bus0RecPin;		//����жϱ�־
		IO0IntEnF |= Bus0RecPin;		//�½����ж�		
	}
	else
	{	//������߲�����,�����߹��ϱ�־ 				
		SetBus0State(bBus0Error);
	}		
	NVIC_EnableIRQ(EINT3_IRQn);
    NVIC_SetPriority(EINT3_IRQn, 1);   	// �����ⲿ�жϲ�ʹ�� 
}


/**********************************************************
*��������			:Timer1Int	
*��������        	:��ʱ��0����ж�,��ʱ��ÿ100us�ж�һ��
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
void TIMER1_IRQHandler (void)			 //100us��ʱ
{  //ƥ��ʱ�����жϺ͸�λ
//	uint32_t i;
	uint8_t byTemp;
	LPC_TIM1->IR		 = 0x01;	   //����ж�

	
	TimerOutCount++;
	if(TimerOutCount>=200)
	{
		TimerOutCount=0;
		SetState(bTimerOutDeal);
	}

	if (GetBus0RecPin()) 
	{
		SetBus0State(bBus0RecBit);
	}
	else 
	{
		ClrBus0State(bBus0RecBit);
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
				if((!GetBus0State(bBus0SendError))&&(GetBus0State(bBus0OnSendFreq)))
				{	//û�з��������߷��ʹ�������һ֡�������ڷ��ͣ�ֹͣ֡���ݷ��ͣ��÷��ʹ����־						
					byBus0State &= (~BUS0_SEND_CON);
					SetBus0State(bBus0SendError);
					SetBus0SendPin();			//�ͷ�����	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;		//���ճ������ý��շ��ͼ���ֵ
				EnableBus0RecInt();

				SetBusErrorState(bStartBitError);
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

						SetBusErrorState(bNightBitError);
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
						SetBus0SendPin();		//�ͷ�����
					}
					byBus0RecSendCount = 0x00;	//���ճ������ý��շ��ͼ���ֵ

					SetBusErrorState(bEndBitError);
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
			ClrBus0SendPin();
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
			SetBus0SendPin();
			byBus0SendStopCount = 240;
		}
	}
	else if(GetBus0State(bBus0OnSendBit))
	{ 	//������λ���ڷ���,���ȷ��͵�����ʼλ
		byBus0SendCount += 0x10;
		if(0x50 == (byBus0SendCount & 0xf0))
		{//����һ������λ���м�
			if((GetBus0State(bBus0SendBit)==0&&GetBus0State(bBus0RecBit)==0)||(GetBus0State(bBus0SendBit)!=0&&GetBus0State(bBus0RecBit)!=0))   bBus0SndBitOk=1;
			else
			{	//����ͬ,����ʧ��								
				byBus0State &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				SetBus0State(bBus0SendError);					
				SetBus0SendPin(); 
				byBus0SendStopCount = 240;

				bBus0SndBitOk=0;
			}				
					
		}
	
		//if((GetBus0State(bBus0SendBit)==0&&GetBus0State(bBus0RecBit)==0)||(GetBus0State(bBus0SendBit)!=0&&GetBus0State(bBus0RecBit)!=0))   		
		else if(bBus0SndBitOk ==1)
		{	//���͵����ݺͽ��յ�������ͬ
			//byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//һλ���ݷ������,���ȷ��͵�����ʼλ
				bBus0SndBitOk=0;
				
				byBus0SendCount &= 0x0f;
				byBus0SendCount += 0x01;				
				if(0x09 == (byBus0SendCount & 0x0f))
				{	//���͵���9λ��
					if ((byBus0RecSendCount&0x0f)==0) 
					{
						SetBus0State(bBus0SendBit);
						SetBus0SendPin();
					}
					else
					{
						ClrBus0State(bBus0SendBit);
						ClrBus0SendPin();
					}					
				}
				else if(0x0a == (byBus0SendCount & 0x0f))
				{	//���͵�����λ��
					SetBus0State(bBus0SendBit);
					SetBus0SendPin();	
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
						SetBus0SendPin();					
					}
					else
					{	//���͵͵�ƽ
						ClrBus0State(bBus0SendBit);
						ClrBus0SendPin();
					}
					byBus0SendBuf >>= 1;
				}
			}
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
			if(GetBus0State(bBus0SendFinish|bBus0Disable)==0)
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
					
					byBus0State |= BUS0_REQ_SEND;	

				}
				else
				{
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
uint8_t Bus0OutputData(uint8_t *pbyData)
{
	uint8_t byTemp = BUS0_FREQ_SIZE;
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		return(FALSE);
	}	
	__disable_irq();
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	__enable_irq();
	return(TRUE);	
}



//���������ݽ������ﴦ��
void Bus0Manager(void)
{
	uint8_t i;
	uint8_t rx_buf[7];
	//����ɽ����ź�
	if (GetBus0State(bBus0RecFinish))
	{
		memcpy((void*)rx_buf,(void*)byBus0RecData,sizeof(STBusFreq));
		ClrBus0State(bBus0RecFinish);
		memcpy((void*)&stBusFreq, rx_buf, sizeof(STBusFreq));
//		for(i=0;i<7;i++)	uart2_send_byte(rx_buf[i]);
		Bus0RecDeal();

	}
	if( GetBus0State(bBus0SendFinish))
	{
		ClrBus0State(bBus0SendFinish);
//		for(i=0;i<7;i++)   uart2_send_byte(byBus0SendData[i]);
		Bus0SendFinishDeal();
	}


	
	//���ն�����������δ����
	if(Bus0RxBuffLen() >= BUS0_FREQ_SIZE)
	{  																		
		rx_buf[0] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[1] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[2] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[3] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[4] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[5] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		rx_buf[6] = byBus0RxQ[byBus0RxHead];
		IncBus0RxPtr(byBus0RxHead);
		if(Bus0RxBuffLen() < BUS0_FREQ_SIZE) byBus0RxHead = byBus0RxTail = 0;
		memcpy((void*)&stBusFreq, rx_buf, sizeof(STBusFreq));
//		for(i=0;i<7;i++)	uart2_send_byte(rx_buf[i]);
		Bus0RecDeal();
	}
}



/*********************************************************************************************************
** Function name:	    eint3Isr
** Descriptions:	    �ⲿ�ж�3������     
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
**                      Note��EINT3��GPIO����ͬһ���жϺ�
*********************************************************************************************************/
void  eint3Isr (void)
{
	uint32_t		i;
	uint32_t		WholeIntState;

	WholeIntState = IOIntStatus;   //�����ж�״̬
	if(WholeIntState & P0Int)		//IO0������һ��������ж�
	{//p0��
	
		/////////////////////////////////
		i = IO0IntStatR;		 //��ȡIO0�������жϼĴ���
		if(i)
		{



		 	IO0IntClr = i;		//�������ж�״̬�Ĵ�����0
		}	
		//////////////////////////////////
		i = IO0IntStatF;		 //��ȡIO0�½����жϼĴ���
		if(i)
		{
			if(i & Bus0RecPin )
			{	
				reset_start_timer1();
				DisableBus0RecInt();					//��ֹ�ٴ��½����ж�
				SetBus0State(bBus0StartRec);			//������ʼλ�ؼ��
				ClrBus0State(bBus0Enable);				//��ֹ���߷���
				byBus0RecCount=0;						//����ռĴ��� 			
			}

			IO0IntClr = i;     //�½����ж�״̬�Ĵ�����0
		}
	  ////////////////////////////////////////
	}

	
	if(WholeIntState & P2Int)		//IO2������һ��������ж�
	{//p2��
		i = IO2IntStatR;	  //��ȡIO2�������жϼĴ���     	//������
		if(i)
		{

			 IO2IntClr = i;				//�������ж�״̬�Ĵ�����0
		}
		//////////////////////////////////////
		i = IO2IntStatF;	   //��ȡIO2�½����жϼĴ���			//�½���
		if(i)
		{//���½����ж�



			IO2IntClr = i;           //  �½����ж�״̬�Ĵ�����0

		}
	}
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
	stCallAddr.bySndSecAddr = pstBusFreq->bySndSecAddr;
	stCallAddr.bySndRoomAddr = pstBusFreq->bySndRoomAddr;
	stCallAddr.bySndBedAddr = pstBusFreq->bySndBedAddr;

	stCallAddr.byRecSecAddr = pstBusFreq->byRecSecAddr;
	stCallAddr.byRecRoomAddr = pstBusFreq->byRecRoomAddr;
	stCallAddr.byRecBedAddr = pstBusFreq->byRecBedAddr;	
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
uint8_t AddrCompare(uint8_t *pAddrA,uint8_t *pAddrB)
{

	if((*pAddrA != 0xff) && (*pAddrA != *pAddrB) && (*pAddrB != 0xff))
	{	
		return(0);
	} 
	
	if((*(pAddrA+1) != 0xff) && (*(pAddrA+1) != *(pAddrB+1)) && (*(pAddrB+1) != 0xff))
	{	
		return(0);
	}
	
	if((*(pAddrA+2) != 0xff) && (*(pAddrA+2) != *(pAddrB+2)) && (*(pAddrB+2) != 0xff))
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
uint8_t DirAddrCompare(uint8_t *pAddrA,uint8_t *pAddrB)
{
	if(*pAddrA != *pAddrB)
	{	
		return(0);
	} 
	if(*(pAddrA+1) != *(pAddrB+1))
	{	
		return(0);
	}
	if(*(pAddrA+2) != *(pAddrB+2))
	{	
		return(0);
	}
	return(1); 
}


/**********************************************************
*��������			:MakeCH0TimerOut	
*��������        	:����ͨ��0��ʱ����
*�������   		:byTimerOut:��ʱʱ�����,byTimerOutCount:��ʱ����
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
void MakeCH0TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount)
{
	stCH0TimerOut.byTimerOutSet = byTimerOut;
	stCH0TimerOut.byTimerOut = byTimerOut;
	stCH0TimerOut.byTimerOutCount = byTimerOutCount;	
}



/**********************************************************
*��������			:MakeCH1TimerOut	
*��������        	:����ͨ��1��ʱ����
*�������   		:byTimerOut:��ʱʱ�����,byTimerOutCount:��ʱ����
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
void MakeCH1TimerOut(uint8_t byTimerOut, uint8_t byTimerOutCount)
{
	stCH1TimerOut.byTimerOutSet = byTimerOut;
	stCH1TimerOut.byTimerOut = byTimerOut;
	stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
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
uint8_t AddCallNod(pSTBusFreq pstBusFreq)
{
    pSTNod pstNodTemp;
    pSTNod pstCallNod;


    if(NULL == (uint8_t *)(stCallPointer.pstHead))
    {   //����Ϊ��
		
        pstNodTemp = malloc(sizeof(STNod));
        if(NULL ==(uint8_t *)pstNodTemp)
        {        	
            return(SAVE_FAIL);
        }
        pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= (pstBusFreq->byCmd & 0x1f);   
        pstNodTemp->pstPrev = pstNodTemp->pstNext = NULL;
        stCallPointer.pstTail = stCallPointer.pstHead = pstNodTemp;    	
        return(SAVE_ONLY); 
    }
    //����Ϊ��,�Ȳ����Ƿ��иýڵ���
    pstCallNod = stCallPointer.pstHead;
    while(NULL != (uint8_t *)pstCallNod)
    {
        if((pstCallNod->byCallSecAddr == pstBusFreq->bySndSecAddr) &&
        	(pstCallNod->byCallRoomAddr == pstBusFreq->bySndRoomAddr) &&
        	(pstCallNod->byCallBedAddr == pstBusFreq->bySndBedAddr) &&
        	((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
        {	//�Ѿ����˸ú��нڵ����Ϣ
            return(SAVE_EXIST);
        }
        pstCallNod = pstCallNod->pstNext;
    }
    //û����ͬ�ĺ���,�������к������ȼ�
    pstCallNod = stCallPointer.pstTail;
	//�鿴�Ƿ�Ϊ���һ���ڵ�
	if(((pstCallNod->byCallPriority) & 0xe0) >= (pstBusFreq->byRecSecAddr & 0xe0))
	{	//Ϊ���һ���ڵ�
		pstNodTemp = malloc(sizeof(STNod));
        if(NULL ==(uint8_t *)pstNodTemp)
        {        	
            return(SAVE_FAIL);
        }
        pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= (pstBusFreq->byCmd & 0x1f); 
		pstCallNod->pstNext = pstNodTemp;
		pstNodTemp->pstPrev = pstCallNod;	
		pstNodTemp->pstNext	= NULL;
		stCallPointer.pstTail = pstNodTemp;	
		if(NULL == (stIndicationData.pstNext))
		{	//����ָʾ�Ľڵ�Ϊ���һ���ڵ㣬���ղ���ӵĽڵ���ӵ�����
			stIndicationData.pstNext = pstNodTemp;
		}	
		return(SAVE_NORMAL);		
	}
	//�������һ���ڵ�,��ǰ����
	pstCallNod = pstCallNod->pstPrev;
	while(NULL !=(uint8_t *)pstCallNod)
	{
		if(((pstCallNod->byCallPriority) & 0xe0) < (pstBusFreq->byRecSecAddr & 0xe0))
		{	//������ǰ����
			pstCallNod = pstCallNod->pstPrev;
		}
		else
		{	//�ҵ����ʵ�λ��,��Ϊ��һ���ڵ㣬����ڵ�
			pstNodTemp = malloc(sizeof(STNod));
	        if(NULL == (uint8_t *)pstNodTemp)
	        {        	
	            return(SAVE_FAIL);
	        }
			pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        	pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        	pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        	pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        	pstNodTemp->byCallPriority |= pstBusFreq->byCmd;
        	(pstCallNod->pstNext)->pstPrev = pstNodTemp;
        	pstNodTemp->pstPrev = pstCallNod;
        	pstNodTemp->pstNext = pstCallNod->pstNext;
        	pstCallNod->pstNext = pstNodTemp;   
        	if(pstNodTemp->pstNext == stIndicationData.pstNext)
        	{	//�ýڵ���ӵ�������ָʾ�Ľڵ�ĺ��棬��������ָʾ�Ľڵ�ָ����Ϣ
        		stIndicationData.pstNext = pstNodTemp;
        	}      	
        	return(SAVE_NORMAL);													
		}
	}
	//�ò���Ľڵ����ȼ�Ϊ������ȼ������뵽�����ͷ��
	if(NULL == (uint8_t *)pstCallNod)
	{
		pstNodTemp = malloc(sizeof(STNod));
	    if(NULL ==(uint8_t *)pstNodTemp)
	 	{        	
	 		return(SAVE_FAIL);
		}
		pstNodTemp->byCallSecAddr = pstBusFreq->bySndSecAddr;
        pstNodTemp->byCallRoomAddr = pstBusFreq->bySndRoomAddr;
        pstNodTemp->byCallBedAddr = pstBusFreq->bySndBedAddr;  
        pstNodTemp->byCallPriority = pstBusFreq->byRecSecAddr & 0xe0;
        pstNodTemp->byCallPriority |= pstBusFreq->byCmd;
        (stCallPointer.pstHead)->pstPrev = pstNodTemp;
        pstNodTemp->pstNext = stCallPointer.pstHead;
        pstNodTemp->pstPrev = NULL;
        stCallPointer.pstHead = pstNodTemp;  
        stIndicationData.byState = STATE_HI;              
        return(SAVE_PRIO_HI);
	}  
	return(SAVE_FAIL);
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
	pSTNod pstCallNod;
	
	//�����Ƿ��иú��м�¼
	pstCallNod = stCallPointer.pstHead;
	if((uint8_t *)pstCallNod == NULL)
	{	//û�к��м�¼,ֱ���˳�		
		return;
	}
	//���ɾ���ĸú���Ϊ����ָʾ�Ľڵ㣬������ָʾ�Ľڵ�Ĳ����ڱ�־
	if((stIndicationData.byIndicationSecAddr == pstBusFreq->byRecSecAddr) &&
		(stIndicationData.byIndicationRoomAddr == pstBusFreq->byRecRoomAddr) &&
		(stIndicationData.byIndicationBedAddr == pstBusFreq->byRecBedAddr) && 
		((stIndicationData.byCallCmd & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
	{
		stIndicationData.byState = STATE_DEL;
	}
	else
	{	//ɾ���Ľڵ㲻������ָʾ�Ľڵ㣬���Ƿ�Ϊ��һ���ڵ�
		if(((uint8_t *)stIndicationData.pstNext != NULL) && 
			(stIndicationData.pstNext->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
			((stIndicationData.pstNext)->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
			(stIndicationData.pstNext->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
			((stIndicationData.pstNext->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))	
		{	//������ָʾ�ĺ��еĺ�һ���ڵ㣬����ָ��
			stIndicationData.pstNext = stIndicationData.pstNext->pstNext;
		}	
	}		
	if((pstCallNod->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
      (pstCallNod->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
      (pstCallNod->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
      ((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
 	{	//�ҵ��ú�����Ϣ��������������ȼ���
 		if(NULL ==(uint8_t *)pstCallNod->pstNext)
 		{	//�ú��м�¼ΪΨһ��һ����¼
 			stCallPointer.pstHead = stCallPointer.pstTail = NULL; 
 			free(pstCallNod); 			
 			return;			
 		}
 		else
 		{	//����Ψһ��һ����¼
 			stCallPointer.pstHead = pstCallNod->pstNext;
 			(pstCallNod->pstNext)->pstPrev = NULL;
 			free(pstCallNod);
 			return;
 		}
	}
	//���ǵ�һ����¼,��������
	pstCallNod = pstCallNod->pstNext;
	while(NULL != (uint8_t *)pstCallNod)
	{
		if((pstCallNod->byCallSecAddr == pstBusFreq->byRecSecAddr) &&
	      (pstCallNod->byCallRoomAddr == pstBusFreq->byRecRoomAddr) &&
	      (pstCallNod->byCallBedAddr == pstBusFreq->byRecBedAddr) &&
	      ((pstCallNod->byCallPriority & 0x1f) == (pstBusFreq->byCmd & 0x1f)))
	 	{	//�ҵ��ú�����Ϣ
	 		if(NULL == (uint8_t *)pstCallNod->pstNext)
	 		{	//�ú�����ϢΪ���һ����¼,ֱ��ɾ�����һ���ڵ�
	 			(pstCallNod->pstPrev)->pstNext = NULL;
	 			stCallPointer.pstTail = pstCallNod->pstPrev;	 			
	 		}
	 		else
	 		{	//�������һ����¼,��������ɾ���ýڵ� 
	 			(pstCallNod->pstPrev)->pstNext = pstCallNod->pstNext;
	 			(pstCallNod->pstNext)->pstPrev = pstCallNod->pstPrev;
	 		}
	 		free(pstCallNod);
	 		return;
	 	}
	 	pstCallNod = pstCallNod->pstNext;
	}	
}	



/**********************************************************
*��������			:SaveIndicationData	
*��������        	:������Ϣָʾ��������
*�������   		:pstBus0SendFreq:��������ָ�룬byFistNod:�Ƿ�Ϊ�����һ���ڵ���Ϣ
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
void SaveIndicationData(pSTBusFreq pstBusFreq, uint8_t byFirstNod)
{
	//������Ϣָʾ���ĵ�ַ
	stIndicationData.byIndicationSecAddr = pstBusFreq->bySndSecAddr;
	stIndicationData.byIndicationRoomAddr = pstBusFreq->bySndRoomAddr;
	stIndicationData.byIndicationBedAddr = pstBusFreq->bySndBedAddr;
	stIndicationData.byCallCmd = pstBusFreq->byRecSecAddr;
	stIndicationData.byState = STATE_EXIST;
	if(byFirstNod)
	{	//�����һ���ڵ���Ϣ		
		stIndicationData.pstNext = stCallPointer.pstHead->pstNext;
	}
	else
	{	//���浱ǰ��¼����һ����¼			
		stIndicationData.pstNext = stIndicationData.pstNext->pstNext;
	}		
}
		

/**********************************************************
*��������			:VoiceChannelCtx
*��������        	:����ͨ���л�������
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
void VoiceChannelCtx(void)
{

	if(GetState(bHostTalk) || GetState(bHostTalk1) || GetDevState(FLAG_NOTE|FLAG_MUSIC) || GetDevState(FLAG_INDICATION)
	    || GetDevState(FLAG_BROADCAST) )
	{
		if(GetState(bHostTalk))
		{//ʹ��SDͨ���Խ�
			phone_tlq_ctrl(0);
			cgb_ctrl(1);
			xtd_ctrl(1);

			cbd_ctrl(0);
			xta_ctrl(0);

			//Ӱ��SDͨ�����������عر�
			kgb_ctrl(0);
			gb_fz_ctrl(0);
		}
		else if(GetState(bHostTalk1))
		{//ʹ��SAͨ���Խ�
			phone_tlq_ctrl(0);
			cbd_ctrl(1);
			xta_ctrl(1);

			cgb_ctrl(0);
			xtd_ctrl(0);

			//Ӱ��Saͨ�����������عر�
			kbc_ctrl(0);
			kbd_ctrl(0);			
		}
	
		if(uiDevState & (FLAG_NOTE|FLAG_MUSIC))
		{	//�������ֲ��Ż�����ʾ��״̬ʹ��SAͨ��
			kbd_ctrl(1);

			//Ӱ��Saͨ�����������عر�
			xta_ctrl(0);
			kbc_ctrl(0);
	
		}

		if(uiDevState & (FLAG_INDICATION))
		{//�ȴ��������߱���״̬ʹ��SDͨ��
			kgb_ctrl(1);		//ͨ��0��	

			kbc_ctrl(0);

			//Ӱ��SDͨ�����������عر�
			xtd_ctrl(0);
			gb_fz_ctrl(0);
			
		}

		if(uiDevState & FLAG_BROADCAST)
		{	//�㲥״̬ʹ��SDͨ��
			phone_tlq_ctrl(1);
			xtd_ctrl(1);
			gb_fz_ctrl(1);

			xta_ctrl(0);
			//Ӱ��SDͨ�����������عر�
			kgb_ctrl(0);

		} 

	} 
	else
	{
	//����״̬��ȫ���ر�
		phone_tlq_ctrl(1);
		xta_ctrl(0);
		xtd_ctrl(0);
		
		kbd_ctrl(0);
		kbc_ctrl(0);
		kgb_ctrl(0);		
		gb_fz_ctrl(0);	

		cgb_ctrl(0);
		cbd_ctrl(0);

	}	
}

/**********************************************************
*��������			:RestartIndication	
*��������        	:���¿�ʼ��Ϣָʾ
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
void RestartIndication(void)
{	
	pSTNod pstNod;
	STAddr_S source_addr;
	uint8_t data_buff[2];

	if(uiDevState & (FLAG_BUSY|FLAG_INDICATION_EXIST))
	{	//����к���ָʾ���������ڷ��ͻ���æ��ֱ���˳�
		return;
	}
	//����ϴα��ż�¼
	if(STATE_HI == (stIndicationData.byState))
	{	//�и������ȼ��ĺ��д��ڣ�ֱ�Ӵӵ�һ�����п�ʼ			
FeltchFirst:
		//ȡ����һ����¼
		pstNod = stCallPointer.pstHead;
		if(NULL ==(uint8_t *)pstNod)
		{	//û�к��м�¼�ˣ�ֱ���˳�
		
			if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
			{//û��ͨ������
			   	memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				data_buff[0] = 0x00;
	    		Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬
	    		wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);

				AM79R70_standby();
			}		
			return;
		}										
		stBusFreq.bySndSecAddr = pstNod->byCallSecAddr;
		stBusFreq.bySndRoomAddr = pstNod->byCallRoomAddr;
		stBusFreq.bySndBedAddr = pstNod->byCallBedAddr;
		stBusFreq.byCmd = CMD_INFO_INDICATION;
		stBusFreq.byRecSecAddr = pstNod->byCallPriority;
		stBusFreq.byRecRoomAddr = 0x00;
		stBusFreq.byRecBedAddr = 0x00;					
		if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
		{
			stBusFreq.byCmd = pstNod->byCallPriority & 0x1f;
			SaveIndicationData(&(stBusFreq), true);
			uiDevState |= FLAG_INDICATION_EXIST;
		}
		return;	
	}
	else
	{	//û�и������ȼ����д���
		//���Ƿ��к��м�¼
		pstNod = stCallPointer.pstHead;
		if(NULL ==(uint8_t *)pstNod)
		{	//û�к��м�¼�ˣ�ֱ���˳�
			if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
			{//û��ͨ������
			   	memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				data_buff[0] = 0x00;
	    		Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬	
	    		wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬

				set_play_voice_volue(PLAY_HOST_CH,0);
				set_play_voice_volue(PLAY_SLAVE_CH,0);

				AM79R70_standby();
			}
			return;
		}	
		else
		{	//���к��м�¼
			pstNod = stIndicationData.pstNext;
			if(NULL == (uint8_t *)pstNod)
			{	//����û�к����ˣ�ֱ��ȡ��һ������
				goto FeltchFirst;
			}
			else
			{	//���滹�к���
				if((stIndicationData.byCallCmd & 0xe0)== (pstNod->byCallPriority & 0xe0))
				{	//��ͬһ�����ȼ��ĺ��У�ȡ����																			
					stBusFreq.bySndSecAddr = pstNod->byCallSecAddr;
					stBusFreq.bySndRoomAddr = pstNod->byCallRoomAddr;
					stBusFreq.bySndBedAddr = pstNod->byCallBedAddr;
					stBusFreq.byCmd = CMD_INFO_INDICATION;
					stBusFreq.byRecSecAddr = pstNod->byCallPriority;
					stBusFreq.byRecRoomAddr = 0x00;
					stBusFreq.byRecBedAddr = 0x00;										
					if(true == Bus0OutputData(&stBusFreq.bySndSecAddr))
					{
						stBusFreq.byCmd = pstNod->byCallPriority & 0x1f;
						SaveIndicationData(&(stBusFreq), false);
						uiDevState |= FLAG_INDICATION_EXIST;
					}
					return;								
				}
				else
				{	//��������ȼ��ͣ�ֱ��ȡ��һ������
					goto FeltchFirst;
				}
			}
		}
	}	
}


void LedStateFlash(void)
{


}


/**********************************************************
*��������			:Bus0RecDeal
*��������        	:������0���ݽ��մ���
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
void Bus0RecDeal(void)
{		
	static uint8_t byTemp;
	static uint8_t byCmd;
	static uint8_t byPriority;	
	pSTUartFreq pstDataFreq;
	STAddr_S  source_addr;

	uint8_t data_buff[10];
	//��λ���Ź�                                                                                                                                                                                                                                                                                     
#ifdef	enable_wdt
	feed_wdt();
#endif


	//���ݴ���	
	switch(stBusFreq.byCmd)
	{
		case CMD_LANDING:									//�Ǽ�����,����ȷ������
			stBusFreq.byRecSecAddr = stBusFreq.bySndSecAddr;
			stBusFreq.byRecRoomAddr = stBusFreq.bySndRoomAddr;
			stBusFreq.byRecBedAddr = stBusFreq.bySndBedAddr;
			stBusFreq.byCmd = CMD_ENTER;
			if(uiDevState & FLAG_BUSY)
			{
				stBusFreq.bySndSecAddr = 0x80;
			}
			else
			{
				stBusFreq.bySndSecAddr = 0x00;
			}
			if(GetState(bPCOpen))
			{
				stBusFreq.bySndSecAddr |= 0x40;
			}
			
			stBusFreq.bySndRoomAddr = 0x00;
			stBusFreq.bySndBedAddr = 0x00;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
			
		case CMD_INSPECTOR_CALL:							//�鷿��������,����ȷ���������λ��
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]= stBusFreq.bySndBedAddr;
			memcpy(&(data_buff[1]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,CMD_INSPECTOR_CALL,4,data_buff);
			wifi_send_cmd(source_addr,CMD_INSPECTOR_CALL,4,data_buff);
			
			stBusFreq.byCmd = CMD_INSPERCTOR_ENTER;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));			
			break;
		case CMD_INFUSION_CALL:								//��Һ��������
		case CMD_SERVICE_CALL:								//�����������
		case CMD_EMERGENCY_CALL:							//������������
		case CMD_HELP_CALL:									//��Ԯ��������
			if((stBusFreq.bySndRoomAddr == ADD_BED_FJ)&&(stBusFreq.bySndSecAddr!=stEepromCfgData.bySelfSecAddr))
			{//�ǼӴ��ֻ��������ĺ��������Ų��� ,ֱ�ӷ���
				return;
			}
			//������������ȼ�
			byCmd = stBusFreq.byCmd;
			byPriority = stBusFreq.byRecSecAddr & 0xe0;
			byTemp = AddCallNod(&(stBusFreq));
			if(SAVE_FAIL == byTemp)
			{	//��Ӻ��ж���ʧ�ܣ�ֱ�ӷ���
				return;
			}	
			//�������ͣ�������ӳɹ�������ȷ������
			stBusFreq.byRecSecAddr = stBusFreq.bySndSecAddr;
			stBusFreq.byRecRoomAddr = stBusFreq.bySndRoomAddr;
			stBusFreq.byRecBedAddr = stBusFreq.bySndBedAddr;
			
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = byCmd + 0x1b;
		
			Bus0OutputData(&(stBusFreq.bySndSecAddr));	
		
			if(GetState(bOriginMusic))
			{//֮ǰ�ڲ��ű�������,ֹͣ
				stBusFreq.bySndSecAddr = stBusFreq.byRecSecAddr;
				stBusFreq.bySndRoomAddr = stBusFreq.byRecRoomAddr;
				stBusFreq.bySndBedAddr = stBusFreq.byRecBedAddr;			
				stBusFreq.byCmd = CMD_STOP_VOICE;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
		
				stBusFreq.byRecSecAddr = stBusFreq.bySndSecAddr;
				stBusFreq.byRecRoomAddr = stBusFreq.bySndRoomAddr;
				stBusFreq.byRecBedAddr = stBusFreq.bySndBedAddr;				
			}
			
			if(uiDevState & (FLAG_INDICATION_EXIST|FLAG_BUSY|FLAG_TALK1))
			{	//�к���ָʾ�������ڷ��Ͷ�����,��û�з�������ߴ���æ״̬,�˳�
				return;
			}
			//û��ָʾ���ڷ���,Ҳ��æ
			if(uiDevState & FLAG_INDICATION)
			{	//�к�������ָʾ��
				if(SAVE_PRIO_HI == byTemp)
				{	//�и����ȼ����д���,������ָֹʾ����
					if(uiDevState & FLAG_STOP_EXIST)
					{	//�����ڷ���ֹͣ����
						return;
					}
					stBusFreq.byCmd = CMD_STOP_INDICATION;						
					if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
					{
						uiDevState |= FLAG_STOP_EXIST;
					}								
				}
			}
			else
			{	//û�к�����ָʾ��,ֱ�ӷ���ָʾ����,�������ָʾ��ָ��λ��
				if(SAVE_ONLY == byTemp)
				{	//�ú�����Ψһһ�����м�¼
					if(uiDevState & FLAG_STOP_EXIST)
					{	//�����ڷ���ֹͣ����
						return;
					}
					stBusFreq.bySndSecAddr = stBusFreq.byRecSecAddr;
					stBusFreq.bySndRoomAddr = stBusFreq.byRecRoomAddr;
					stBusFreq.bySndBedAddr = stBusFreq.byRecBedAddr;
					stBusFreq.byCmd = CMD_INFO_INDICATION;
					stBusFreq.byRecSecAddr = byPriority|byCmd;													
					if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
					{	//��ָʾ���ڷ��Ͷ����б�־
						//����ָʾ����ַ��������ȼ�������ָʾ�ĺ��м�¼ָ��
						stBusFreq.byCmd = byCmd;
						SaveIndicationData(&(stBusFreq), true);	
						uiDevState |= FLAG_INDICATION_EXIST;
					}
					return;
				}
				else
				{	//�ж������м�¼����,����û��FLAG_INDICATION_EXIST,
					//FLAG_BUSY,FLAG_INDICATION��־,����ָʾ������ɺ������Ϣ��������
					//ֹͣ��Ϣָʾ���ڷ���,������ɺ��ٴ��� 	
					if(uiDevState & FLAG_STOP_EXIST)
					{	//�����ڷ���ֹͣ����
						return;
					}
					byCmd =stCallPointer.pstHead->byCallPriority & 0x1f;															
					stBusFreq.bySndSecAddr = stCallPointer.pstHead->byCallSecAddr;
					stBusFreq.bySndRoomAddr = stCallPointer.pstHead->byCallRoomAddr;
					stBusFreq.bySndBedAddr = stCallPointer.pstHead->byCallBedAddr;
					stBusFreq.byCmd = CMD_INFO_INDICATION;
					stBusFreq.byRecSecAddr = 
					stCallPointer.pstHead->byCallPriority;
					if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
					{	//����ָʾ����ַ������
						stBusFreq.byCmd = byCmd;
						SaveIndicationData(&(stBusFreq), true);
						uiDevState |= FLAG_INDICATION_EXIST;
					}
					return; 
				}								
			}			
			break;		
		case CMD_INFUSION_CLEAR:							//�����Һ��������
		case CMD_SERVICE_CLEAR: 							//��������������
		case CMD_EMERGENCY_CLEAR:							//���������������
		case CMD_HELP_CLEAR:								//�����Ԯ��������
			if((stBusFreq.bySndRoomAddr == MOVE_FJ)&&
			   (stBusFreq.bySndSecAddr!=stEepromCfgData.bySelfSecAddr))
			{//���ƶ��ֻ���������������������Ų��� ,ֱ�ӷ���
				return;
			}		
			if( (stBusFreq.byRecSecAddr == stBusFreq.bySndSecAddr) &&
				(stBusFreq.byRecRoomAddr == stBusFreq.bySndRoomAddr) &&
				(stBusFreq.byRecBedAddr == stBusFreq.bySndBedAddr) )
			{//�Ǻ��зֻ��Լ��Ҷ�	
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); //�����ֻ���ַ
				memcpy(&(data_buff[3]),&(stBusFreq.bySndSecAddr),3); //�����ֻ���ַ
				Usart2SndCmd(source_addr,stBusFreq.byCmd,6,data_buff);
				wifi_send_cmd(source_addr,stBusFreq.byCmd,6,data_buff);
			}
			else
			{//����������ֻ��ҶϷֻ�
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); //�����ֻ���ַ
				memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3); //�����ֻ���ַ
				Usart2SndCmd(source_addr,stBusFreq.byCmd,6,data_buff);
				wifi_send_cmd(source_addr,stBusFreq.byCmd,6,data_buff);
					
			}
			//����м�¼
			stBusFreq.byCmd -= 0x0a; 		//��������ԭ�ɶ�Ӧ�ĺ�������		
			RemoveCallNod(&(stBusFreq)); 		
			if((uiDevState & FLAG_INDICATION) && 
				(stBusFreq.byRecSecAddr == stIndicationData.byIndicationSecAddr) &&
				(stBusFreq.byRecRoomAddr == stIndicationData.byIndicationRoomAddr) &&
				(stBusFreq.byRecBedAddr == stIndicationData.byIndicationBedAddr) &&
				((stBusFreq.byCmd & 0x1f) == (stIndicationData.byCallCmd & 0x1f)))
			{	//���ں���ָʾ��������ĺ���Ϊ����ָʾ�ĺ���,������ֹ����ָʾ����
				if(uiDevState & FLAG_STOP_EXIST)
				{	//�����ڷ���ֹͣ����
					return;
				}
				//û��ָֹͣʾ���ڷ�����				
				stBusFreq.byCmd = CMD_STOP_INDICATION;				
				if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
				{
					uiDevState |= FLAG_STOP_EXIST;
				}							
			}
	
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{//û�зֻ�����
				if(GetState(bOriginMusic))
				{//֮ǰ�в��ű�������
				   stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stBusFreq.byRecSecAddr =0xff;
				   stBusFreq.byRecRoomAddr = 0xff;
				   stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stBusFreq.bySndSecAddr));					   
				}
			}
			break;	
		case CMD_INFUSION_ANSWER:							//������Һ����
		case CMD_SERVICE_ANSWER:							//����������
		case CMD_EMERGENCY_ANSWER:							//�����������
		case CMD_HELP_ANSWER:								//������Ԯ����	
			if(uiDevState & FLAG_BUSY)
			{	//æ���˳�
				break;
			}
			
			//�����������з����������з���ַ
			SaveCallAddr(&(stBusFreq));			
			stCallAddr.byCmd = stBusFreq.byCmd;


			//����м�¼
			stBusFreq.byCmd -= 0x06; 		//���������ԭ�ɶ�Ӧ�ĺ�������
			RemoveCallNod(&(stBusFreq)); 		
			//�õȴ�Ӧ���æ��־
			uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY); 
			if(uiDevState & FLAG_INDICATION)
			{	//���ں���ָʾ,��ָʾ��־				
				uiDevState &= ~FLAG_INDICATION;
				
				//ֹͣ����			
				StopVoicePlay();

				//����ͨ���л�
				VoiceChannelCtx();	
			}			

			//���õȴ�Ӧ��ʱ1S
			MakeCH0TimerOut(50, 0); 		
			break;
	
				
		case CMD_COMM_CALL: 								//��ͨ��������
			if(uiDevState & FLAG_BUSY)
			{	//æ��ֱ���˳�
				break;
			}
			//��æ�͵ȴ�Ӧ���־ 
			uiDevState |= (FLAG_BUSY|FLAG_WAIT_ACK); 		
			if(uiDevState & FLAG_INDICATION)
			{	//���ں���ָʾ,��ָʾ��־				
				uiDevState &= ~FLAG_INDICATION;				
				//ֹͣ����			
				StopVoicePlay();

				//����ͨ���л�
				VoiceChannelCtx();								
			}
			
			//�����������з����������з���ַ
			SaveCallAddr(&(stBusFreq));			
			
			//���õȴ�Ӧ��ʱ5S
			MakeCH0TimerOut(50, 5);
			break;
		case CMD_COMM_ANSWER:								//Ӧ������
			if(0x0000 == (uiDevState & FLAG_BUSY))
			{	//��æ������ϵͳ��λ����
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				break;
			}

			if(AddrCompare(&(stBusFreq.bySndSecAddr),&(stCallAddr.byRecSecAddr)))
			{//�Ƕ�Ӧ�ֻ���Ӧ���ź�
				//æ,��ȴ�Ӧ���־ 				
				uiDevState &= ~FLAG_WAIT_ACK;			
				//�õȴ�������־			
				uiDevState |= FLAG_WAIT_LISTEN;	

				//���õȴ�������ʱ15S
				MakeCH0TimerOut(250, 3);	

			}
			else MakeCH0TimerOut(1, 0); //ʹͨ��0��ʱʱ�䵽
			break;
	
		case CMD_CALL_LISTEN:							//��������
			if(0x0000 == (uiDevState & FLAG_BUSY))
			{	//��æ������ϵͳ��λ����
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&stBusFreq.bySndSecAddr);
				break;
			}
	
			if(AddrCompare(&(stBusFreq.bySndSecAddr),&(stCallAddr.byRecSecAddr)))
			{//�Ƕ�Ӧ�ֻ��Ľ����ź� 		
				//æ,��ȴ�������־����ͨ����־ 				
				uiDevState &= ~FLAG_WAIT_LISTEN; 		
				uiDevState |= FLAG_TALK;

				if(DirAddrCompare(&(stEepromCfgData.bySelfSecAddr),&(stBusFreq.byRecSecAddr)))
				{
					SetState(bHostTalk);
				}

				//����ͨ���л�
				VoiceChannelCtx();
				//���õȴ�ͨ����ʱ60S
				MakeCH0TimerOut(250, 12);	

				
				if(0x0000 == (uiDevState & (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC)))
				{	//û��ͨ��1��������ʾ���������ֲ��Ŵ��ڣ������л�����ͨ������								
					stBusFreq.byCmd = CMD_CHANNEL_CHANGE;								
					Bus0OutputData(&(stBusFreq.bySndSecAddr));
					//��ͨ���л����������߶����б�־
					uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;

				}

				if((stBusFreq.byRecSecAddr == stEepromCfgData.bySelfSecAddr)&&
					(stBusFreq.byRecRoomAddr == stEepromCfgData.bySelfRoomAddr)&&
					(stBusFreq.byRecBedAddr == stEepromCfgData.bySelfBedAddr))
				{	//�ǹ�����������ͷ�ֻ��ĺ���

					AM79R70_active();
						
					set_talk_voice_volue(TALK_HOST_CH,stEepromCfgData.byHostTalkVol);
					set_talk_voice_volue(TALK_SLAVE_CH,stEepromCfgData.bySlaTalkVol);

					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0] = stEepromCfgData.bySelfSecAddr;
					data_buff[1] = PC_ADDR2;
					data_buff[2] = PC_ADDR3;
					data_buff[3] = stBusFreq.bySndSecAddr;
					data_buff[4] = stBusFreq.bySndRoomAddr;
					data_buff[5] = stBusFreq.bySndBedAddr;
					wifi_send_cmd(source_addr,CMD_INFUSION_ANSWER,6,data_buff);
					
					data_buff[0] = CMD_INFUSION_ANSWER;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
					scon2_Rcvbuf[CMD_POSITION]=0x00;	//���ԭ��������
				}
				else
				{	//�������ֻ�����ͷ�ֻ��ĺ���
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);				
					memcpy(&(data_buff[0]),&(stBusFreq.byRecSecAddr),3); //����ֻ���ַ
					memcpy(&(data_buff[3]),&(stBusFreq.bySndSecAddr),3); //�����ֻ���ַ	
					Usart2SndCmd(source_addr,stCallAddr.byCmd,6,data_buff);
					wifi_send_cmd(source_addr,stCallAddr.byCmd,6,data_buff);
				}
			}
			else MakeCH0TimerOut(1, 0); //ʹͨ��0��ʱʱ�䵽
			break;		
		case CMD_BROADCAST1:								
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:								//�㲥����
			if(uiDevState & FLAG_BUSY)
			{	//æ������ϵͳ��λ����
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				break;
			}
			//��æ����æ��־�͹㲥��־
			uiDevState |= (FLAG_BUSY|FLAG_BROADCAST);
			
			//�����������з����������з���ַ
			SaveCallAddr(&(stBusFreq));

			if(uiDevState & FLAG_INDICATION)
			{
				//�����ָʾ��־
				uiDevState &= ~FLAG_INDICATION;
				
				//ֹͣ����			
				StopVoicePlay();
			}

			//����ͨ���л�
			VoiceChannelCtx();
			//���õȴ��㲥��ʱ180S
			MakeCH0TimerOut(250, 36);
			break;	
		case CMD_SYSTERM_RESET: 							//ϵͳ��λ����

			if(uiDevState & FLAG_INDICATION)
			{
				uiDevState &= ~FLAG_INDICATION;
	//			//ֹͣ����
				StopVoicePlay();				
			}
			//���־ //��Щ��־֮��ı�־����0,��Щ��־λ����ԭ̬����
			uiDevState &= (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_INDICATION_EXIST|FLAG_STOP_EXIST);
			if(GetState(bHostTalk))
			{
				ClrState(bHostTalk);
				set_talk_voice_volue(TALK_HOST_CH,0);
				set_talk_voice_volue(TALK_SLAVE_CH,0);				
			}
			
			//����ͨ���л�
			VoiceChannelCtx();
			//�峬ʱ
			MakeCH0TimerOut(0, 0);

			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); 
			memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			wifi_send_cmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			
			//���¿�ʼ����	
			if(0x00 == (uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
			{
				RestartIndication();
			}	

			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{//û�зֻ�����
				if(GetState(bOriginMusic))
				{//֮ǰ�в��ű�������
				   stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stBusFreq.byRecSecAddr =0xff;
				   stBusFreq.byRecRoomAddr = 0xff;
				   stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stBusFreq.bySndSecAddr));			   
				}
			}	
				
			break;
		case CMD_CHANNEL_CLOSE: 							//�ر�����ͨ������
			uiDevState &= ~FLAG_TALK1;
			if(GetState(bHostTalk1))
			{
				ClrState(bHostTalk1);
				set_talk_voice_volue(TALK_HOST_CH,0);
				set_talk_voice_volue(TALK_SLAVE_CH,0);	
			}
			
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{	//û�к��м�¼��
			
				if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//û��ͨ������
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0] = 0x00;	//ϵͳ��æ
					Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬
					wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬
				}	
			}	
	
			//����ͨ���л�
			VoiceChannelCtx();
			//��ͨ��1ͨ����ʱ
			MakeCH1TimerOut(0, 0);

			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3); 
			memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			wifi_send_cmd(source_addr,CMD_TALK_CLEAR,6,data_buff);
			
			if(uiDevState & FLAG_TALK)
			{	//ͨ��0ͨ�����ڣ������л�����ͨ������				
				stBusFreq.byCmd = CMD_CHANNEL_CHANGE;
				//��ͨ���л����������߶����б�־
				uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}


			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{//û�зֻ�����
				if(GetState(bOriginMusic))
				{//֮ǰ�в��ű�������
				   stBusFreq.byCmd = CMD_MUSIC_PLAY;
				   stBusFreq.byRecSecAddr =0xff;
				   stBusFreq.byRecRoomAddr = 0xff;
				   stBusFreq.byRecBedAddr  =0xff;
				   Bus0OutputData(&(stBusFreq.bySndSecAddr));			   
				}
			}	

			else
			{
				RestartIndication();
			}
			break;	
	
				
		case CMD_DATE_SEND:
			stTime.bySecond=0x00;
			stTime.byMinute = stBusFreq.bySndRoomAddr;
			stTime.byHour	= stBusFreq.bySndBedAddr;
			memcpy(&(stTime.byDay),&(stBusFreq.byRecSecAddr),3);
			WriteDS3231_time();
			break;	
				
	   case CMD_BUS_ANSWER: //����ͨ��Ӧ������
		   switch(stBusFreq.byRecSecAddr)
			{
			case CMD_COLOR_CLEAR:
//			case CMD_BL_TIMER_SET:
//			case CMD_VOICE_TIMER_SET:
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//��ȴ�����Ӧ��ʱ
				byWaitCommAckDTime = 0;
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0]		=stBusFreq.byRecSecAddr;
				data_buff[1]		=TRUE;		//�ɹ���ʧ��
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
				return;
				
			case CMD_DISPLAY_DATA:
			case CMD_HANDLE_DISPLAY_DATA:	
				if((stBusFreq.bySndBedAddr == stRS485Addr.byBedAddr))
				{//�������
					byWaitRS485AckDTime = 0;
					memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
					data_buff[0]		=stBusFreq.byRecSecAddr;
					data_buff[1]		=TRUE;		//�ɹ���ʧ��
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
					stRS485Addr.byBedAddr=0; 	//������꣬��485��������

				}
				return;

			default:
				break;
		   	}	
		   
	   		if((stBusFreq.bySndBedAddr == stCommAddr.byBedAddr)||(stBusFreq.bySndBedAddr == 0xff) ||(stCommAddr.byBedAddr == 0xff) )
	   		{
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//��ȴ�����Ӧ��ʱ
				byWaitCommAckDTime = 0;
				switch(stBusFreq.byRecSecAddr)
				{
				case CMD_POWER_ON:
				case CMD_ONLINE_WRITE:
				case CMD_SCHEDULE_SEND:
				case CMD_OPEN_LCD:
				case CMD_CLOSE_LCD:
				case CMD_CLEAR_LCD:
				case CMD_CLEAR_HANDLE_LCD:
				case CMD_CLEAR_MK_LCD:	
					memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
					data_buff[0]		=stBusFreq.byRecSecAddr;
					data_buff[1]		=TRUE;		//�ɹ���ʧ��
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
					break;
				default:
					break;
				}
	   		
	   		}
			else byWaitCommAckDTime = 1;	//��ַ������,��ǰ�˳�
			break;
		case CMD_DATA_ERROR:		//һ֡���ݳ���
			//��ȴ�Ӧ��ʱ
			if(stBusFreq.bySndBedAddr == stRS485Addr.byBedAddr)
			{//�������			
	             byWaitRS485AckDTime = 0;	
				 memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
				 data_buff[0] = byUsart0SndBuf[CMD_POSITION];
				 data_buff[1] = FALSE;
				 Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
				 wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
				 stRS485Addr.byBedAddr = 0;
			}
			break;
		case CMD_ONLINE_OUTPUT:
	   		if((stBusFreq.bySndSecAddr	==stCommAddr.bySecAddr)&&
				(stBusFreq.bySndRoomAddr==stCommAddr.byRoomAddr)&&
				(stBusFreq.bySndBedAddr	==stCommAddr.byBedAddr))
	   		{
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//��ȴ�����Ӧ��ʱ
				byWaitCommAckDTime = 0;
				memcpy(&(source_addr.bySecAddr),&(stBusFreq.bySndSecAddr),3);
				data_buff[0]		=CMD_ONLINE_READ;
				data_buff[1]		=TRUE;		//�ɹ���ʧ��
				data_buff[2]		=stBusFreq.byRecSecAddr;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,3,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,3,data_buff);	
	   		}
			else byWaitCommAckDTime = 1;	//��ַ������,��ǰ�˳�
			break;

		case CMD_QUEST_ANSWER:
	   		if((stBusFreq.bySndSecAddr	==stCommAddr.bySecAddr)&&
				(stBusFreq.bySndRoomAddr==stCommAddr.byRoomAddr)&&
				(stBusFreq.bySndBedAddr	==stCommAddr.byBedAddr))
	   		{
				//uiDevState &= ~(FLAG_WAIT_ACK|FLAG_BUSY);
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				//��ȴ�����Ӧ��ʱ
				byWaitCommAckDTime = 0;

				memcpy(&(source_addr.bySecAddr),&(stBusFreq.bySndSecAddr),3);
				data_buff[0]		=scon2_Rcvbuf[CMD_POSITION];
				data_buff[1]		=TRUE;		//�ɹ���ʧ��
				memcpy(&(data_buff[2]),&(stBusFreq.byRecSecAddr),3);	//����3������
				Usart2SndCmd(source_addr,CMD_RESULT_PC,5,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,5,data_buff);
	   		}
			else byWaitCommAckDTime = 1;	//��ַ������,��ǰ�˳�
			break;

		case CMD_NURSE_IN:
		case CMD_NURSE_OUT:

			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]= stBusFreq.bySndBedAddr;						//�ƶ��ֻ���
			memcpy(&(data_buff[1]),&(stBusFreq.byRecSecAddr),3);		//��ͷ�ֻ���
			Usart2SndCmd(source_addr,stBusFreq.byCmd,4,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,4,data_buff);
			break;

		case CMD_SUPPLY_OX_START:			
		case CMD_SUPPLY_OX_END:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.bySndSecAddr),3);		//��ͷ�ֻ���
			data_buff[3]=stBusFreq.byRecRoomAddr;						//Сʱ
			data_buff[4]=stBusFreq.byRecBedAddr;						//����
			Usart2SndCmd(source_addr,stBusFreq.byCmd,5,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,5,data_buff);
			break;

		case CMD_VOICE_TIMER_SET:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]=stBusFreq.bySndSecAddr;		//����Сʱ
			data_buff[1]=stBusFreq.bySndRoomAddr;		
			data_buff[2]=stBusFreq.byRecSecAddr;		//����Сʱ
			data_buff[3]=stBusFreq.byRecRoomAddr;
			data_buff[4]=stBusFreq.byRecBedAddr;		//С����ֵ
			Usart2SndCmd(source_addr,stBusFreq.byCmd,5,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,5,data_buff);
			break;

		case CMD_BL_TIMER_SET:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]=stBusFreq.bySndSecAddr;		//����Сʱ
			data_buff[1]=stBusFreq.bySndRoomAddr;		
			data_buff[2]=stBusFreq.byRecSecAddr;		//����Сʱ
			data_buff[3]=stBusFreq.byRecRoomAddr;
			Usart2SndCmd(source_addr,stBusFreq.byCmd,4,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,4,data_buff);
			break;

			
		case CMD_COLOR_CLEAR:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]=stBusFreq.bySndSecAddr;		//����Сʱ
			data_buff[1]=stBusFreq.bySndRoomAddr;		
			data_buff[2]=stBusFreq.bySndBedAddr;		
			Usart2SndCmd(source_addr,stBusFreq.byCmd,3,data_buff);
			wifi_send_cmd(source_addr,stBusFreq.byCmd,3,data_buff);
			break;
			
		default:
			break;
	}
}


/**********************************************************
*��������			:Bus0SendFinishDeal
*��������        	:������0���ݷ�����ɴ���
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
void Bus0SendFinishDeal(void)
{	 
	pSTUartFreq pstDataFreq;
	STAddr_S  source_addr;
	uint8_t   data_buff[10];

#ifdef	enable_wdt
    feed_wdt();
#endif
	
	__disable_irq();
	memcpy(&(stBusFreq.bySndSecAddr), byBus0SendData, sizeof(STBusFreq));	//��ȡ����������
	__enable_irq();

//	uart2_send_str2(&(stBusFreq.bySndSecAddr),7);


	//���ݴ���	
	switch(stBusFreq.byCmd)
	{
		case CMD_POWER_ON:									//�ϵ縴λ��������
			if(uiDevState & FLAG_LANDING)
			{	//ȷʵ�����ϵ�״̬
				uiDevState &= ~FLAG_LANDING;
				uiDetectPcTime = 250;
			}
			else
			{
				//�õȴ�Ӧ���æ��־
				uiDevState |= (FLAG_WAIT_COMM_ACK);
				//����ֻ���ַ
				stCommAddr.bySecAddr = 	stBusFreq.byRecSecAddr;
				stCommAddr.byRoomAddr = stBusFreq.byRecRoomAddr;
				stCommAddr.byBedAddr = 	stBusFreq.byRecBedAddr;
				//���õȴ�Ӧ��ʱ1S
				byWaitCommAckDTime = 50;				
			}
			break;
		case CMD_INFO_INDICATION:							//����ָʾ����			
			uiDevState &= ~FLAG_INDICATION_EXIST;
			if(uiDevState & FLAG_BUSY)
			{	//æ���˳�				
				break;
			}			
			//���ü�¼			
			if((stIndicationData.byState == STATE_HI) ||
				(stIndicationData.byState == STATE_DEL))
			{	//�и����ȼ����д��ڻ��߸ú����Ѿ��������ˣ�������ֹ��������
				if(uiDevState & FLAG_STOP_EXIST)
				{	//�����ڷ���ֹͣ����
					return;
				}
				stBusFreq.byCmd = CMD_STOP_INDICATION;						
				if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
				{
					uiDevState |= FLAG_STOP_EXIST;
				}				
			}
			else
			{	//�ú��л������ڼ�¼��,��û�и����ȼ��ĺ��д���,��ָʾ��־
				uiDevState |= FLAG_INDICATION;
				
				//����������Ϣ
				if(MakeCallVoiceData(&(stIndicationData)))	
				{
					//��������
					check_little_voice_status();
					if(little_voice_effective==1)
					{//С����ʱ��
/*						set_play_voice_volue(PLAY_HOST_CH,(stEepromCfgData.byHostPlayLittleVol+1)*3);
						set_play_voice_volue(PLAY_SLAVE_CH,(stEepromCfgData.bySlaPlayLittleVol+1)*3);  */

						set_play_voice_volue(PLAY_HOST_CH, (stEepromCfgData.byLVoiceVal+1)*3);
						set_play_voice_volue(PLAY_SLAVE_CH,(stEepromCfgData.byLVoiceVal+1)*3);

						
					}
					else
					{
						set_play_voice_volue(PLAY_HOST_CH,(stEepromCfgData.byHostPlayVol+1)*3);
						set_play_voice_volue(PLAY_SLAVE_CH,(stEepromCfgData.bySlaPlayVol+1)*3);					
					}
					played_times = 0;
					played_turns =0;
					StartVoicePlay();
					start_tel_ring();
				}

				//����ͨ���л�
				VoiceChannelCtx();
				//���ñ��ų�ʱ15S
				MakeCH0TimerOut(250, 3);
				
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				data_buff[0]	= stBusFreq.bySndSecAddr;
				data_buff[1]	= stBusFreq.bySndRoomAddr;
				data_buff[2] 	= stBusFreq.bySndBedAddr;
				data_buff[3] 	= (stBusFreq.byRecSecAddr&0x1f);	//��������
				data_buff[4]	= (stBusFreq.byRecSecAddr&0xe0)>>5;	//���ȼ�
				wifi_send_cmd(source_addr,CMD_INFO_INDICATION,5,data_buff);
				
			}							
			break;

			
		case CMD_STOP_INDICATION:							//��ֹ����ָʾ����			
			//��ָʾ��־
			uiDevState &= ~(FLAG_INDICATION|FLAG_STOP_EXIST);
			//�屨�ų�ʱ
			MakeCH0TimerOut(0, 0);
			
			//ֹͣ����
			StopVoicePlay();
			stop_tel_ring();

			//����ͨ���л�
			VoiceChannelCtx();			
			//��æ����û�к���ָʾ���ڷ��Ͷ�����,���¿�ʼ����			
			RestartIndication();			
			break;	
		case CMD_SYSTERM_RESET:								//ϵͳ��λ����
			if(uiDevState & FLAG_INDICATION)
			{
				uiDevState &= ~FLAG_INDICATION;
	//			//ֹͣ����
				StopVoicePlay();				
			}
			
			uiDevState &= (FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_INDICATION_EXIST|FLAG_STOP_EXIST);	// ���˼�λ����ԭ״̬,������־��0		
			
			//����ͨ���л�
			VoiceChannelCtx();
			//�峬ʱ
			MakeCH0TimerOut(0, 0);
			
			//���¿�ʼ����
			if(0x00 == (uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
			{
				RestartIndication();
			}									
			break;	
		case CMD_START_VOICE:								//������ʾ����
			//��ͨ��1ͨ����־�ͳ�ʱ�ȴ�
			uiDevState &= ~FLAG_TALK1;			
			MakeCH1TimerOut(0, 0);
			//���������ѱ�־
			uiDevState |= FLAG_NOTE;
			//����ͨ���л�
			VoiceChannelCtx();
			
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = 0xff;
			source_addr.byBedAddr = 0xff;

			data_buff[0]		=CMD_START_VOICE;
			data_buff[1]		=TRUE;		//�ɹ���ʧ��
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			break;

			
		case CMD_MUSIC_PLAY:								//�������ֲ��ű�־
			//��ͨ��1ͨ����־�ͳ�ʱ�ȴ�
			uiDevState &= ~FLAG_TALK1;				
			MakeCH1TimerOut(0, 0);
			//�ñ������ֱ�־
			uiDevState |= FLAG_MUSIC;
			//����ͨ���л�
			VoiceChannelCtx();

			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = 0xff;
			source_addr.byBedAddr = 0xff;
			data_buff[0]		=CMD_MUSIC_PLAY;
			data_buff[1]		=TRUE;		//�ɹ���ʧ��
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			break;
			
		case CMD_STOP_VOICE:
			//�岥���ͱ������ֱ�־
			uiDevState &= ~(FLAG_NOTE|FLAG_MUSIC);
			//����ͨ���л�
			VoiceChannelCtx();

			memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
			data_buff[0]		=CMD_STOP_VOICE;
			data_buff[1]		=TRUE;		//�ɹ���ʧ��
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{	//û�к��м�¼�ˣ�ֱ���˳�
			
				if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//û��ͨ������
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0] =0x00;	//ϵͳ��æ
					Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬
					wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);	//����ϵͳ��æ״̬
				}	
			}			
			break;	
		case CMD_INFUSION_CLEAR:							//�����Һ��������
		case CMD_SERVICE_CLEAR:								//����������
		case CMD_HELP_CLEAR:								//�����Ԯ����
		case CMD_EMERGENCY_CLEAR:							//�����������
			//�ݴ�ԭ��������
			byChar = stBusFreq.byCmd;	
			
			//����м�¼
			stBusFreq.byCmd -= 0x0a;			//��������ԭ�ɶ�Ӧ�ĺ�������		
			RemoveCallNod(&(stBusFreq));	

			memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
			data_buff[0]		=byChar;	//ԭ����
			data_buff[1]		=TRUE;		//�ɹ���ʧ��
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);	
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);


			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0] = stEepromCfgData.bySelfSecAddr;
			data_buff[1] = PC_ADDR2;
			data_buff[2] = PC_ADDR3;
			memcpy(&(data_buff[3]),&(stBusFreq.byRecSecAddr),3);
			wifi_send_cmd(source_addr,byChar,6,data_buff);
			
			if((uiDevState & FLAG_INDICATION) && 
				(stBusFreq.byRecSecAddr == stIndicationData.byIndicationSecAddr) &&
				(stBusFreq.byRecRoomAddr == stIndicationData.byIndicationRoomAddr) &&
				(stBusFreq.byRecBedAddr == stIndicationData.byIndicationBedAddr) &&
				(stBusFreq.byCmd == (stIndicationData.byCallCmd & 0x1f)))
			{	//���ں���ָʾ,������ĺ���Ϊ����ָʾ�ĺ���,�屨�ű�־
				uiDevState &= ~FLAG_INDICATION;
				
				//ֹͣ����
				StopVoicePlay();

				//����ͨ���л�
				VoiceChannelCtx();
				//�峬ʱ
				MakeCH0TimerOut(0, 0);				
				//���¿�ʼ����
				if(0x00 == (uiDevState&FLAG_STOP_EXIST))
				{
					RestartIndication();
				}
			}			
			break;	

			
		case CMD_CHANNEL_CHANGE:							//ͨ���л�����
			if(uiDevState & FLAG_TALK)
			{	//ͨ��״̬
				uiDevState &= ~(FLAG_BUSY|FLAG_TALK);

				//��ͨ��0���������з���ַ���Ƶ�ͨ��1���������з���ַ��
				memcpy(&(stCH1CallAddr),&(stCallAddr),sizeof(STAddr));
				//��ͨ��0���������з���ַ��0
				stCallAddr.bySndSecAddr = 0x00;
				stCallAddr.bySndRoomAddr = 0x00;
				stCallAddr.bySndBedAddr = 0x00;
				stCallAddr.byRecSecAddr = 0x00;
				stCallAddr.byRecRoomAddr = 0x00;
				stCallAddr.byRecBedAddr = 0x00;				
				//��ͨ���л������߶����б�־
				uiDevState &= ~(FLAG_CHANNEL_CHANGE_EXIST);


				//�峬ʱ
				MakeCH0TimerOut(0, 0);	
				//��ͨ��1��־
				uiDevState |= FLAG_TALK1;	

				if(DirAddrCompare(&(stEepromCfgData.bySelfSecAddr),&(stCH1CallAddr.bySndSecAddr)))
				{
					ClrState(bHostTalk);
					SetState(bHostTalk1);
				}				
				//����ͨ���л�
				VoiceChannelCtx();
				//ͨ��1ͨ����ʱ60S
				MakeCH1TimerOut(250, 12);
//				MakeCH1TimerOut(0, 0);		//����ʱ����ʱ


				//����ҽ�о�Ժ�ֻ��࣬��·����SD��SA֮����ںܴ���߼���ݣ�����ڶԽ�ʱ��
				//����������ţ�ҽ���ֻ�ͨ��ʱ�кܴ�����������������ͨ��������?

				
/*				//���¿�ʼ����
				if(0x00 == (stLocalControl.uiDevState&(FLAG_INDICATION_EXIST|FLAG_STOP_EXIST)))
				{

					RestartIndication();
				}*/
			}
			else
			{	//����ͨ��״̬�����͹ر�ͨ������
				stBusFreq.bySndSecAddr = stCallAddr.bySndSecAddr;
				stBusFreq.bySndRoomAddr = stCallAddr.bySndRoomAddr;
				stBusFreq.bySndBedAddr = stCallAddr.bySndBedAddr;			
				stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
				stBusFreq.byRecSecAddr = stCallAddr.byRecSecAddr;
				stBusFreq.byRecRoomAddr = stCallAddr.byRecRoomAddr;
				stBusFreq.byRecBedAddr = stCallAddr.byRecBedAddr;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}					
			break;
			
		case CMD_CHANNEL_CLOSE:			
			uiDevState &= ~FLAG_TALK1;
			
			if(NULL ==(uint8_t *)(stCallPointer.pstHead))
			{	//û�к��м�¼�ˣ�ֱ���˳�
			
				if(!(uiDevState & (FLAG_TALK |FLAG_TALK1|FLAG_NOTE|FLAG_MUSIC|FLAG_BROADCAST|FLAG_INDICATION)))
				{//û��ͨ������
					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					data_buff[0]		=0x00;		//ϵͳ��æ
					Usart2SndCmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);		//����ϵͳ��æ״̬
					wifi_send_cmd(source_addr,CMD_SYSTEM_STATE,1,data_buff);		//����ϵͳ��æ״̬
				}	
			}	
			
			//����ͨ���л�
			VoiceChannelCtx();
			//��ͨ��1ͨ����ʱ
			MakeCH1TimerOut(0, 0);
			if(uiDevState & FLAG_TALK)
			{	//ͨ��0ͨ�����ڣ������л�����ͨ������				
				stBusFreq.byCmd = CMD_CHANNEL_CHANGE;
				//��ͨ���л����������߶����б�־
				uiDevState |= FLAG_CHANNEL_CHANGE_EXIST;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}

			else
			{	
				RestartIndication();
			}
			break;

		case CMD_INFUSION_ENTER:		//���������ȷ�������Ž�����Ϣ�ϴ���PC��
		case CMD_SERVICE_ENTER:
		case CMD_EMERGENCY_ENTER:
		case CMD_HELP_ENTER:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			memcpy(&(data_buff[0]),&(stBusFreq.byRecSecAddr),3);
			Usart2SndCmd(source_addr,stBusFreq.byCmd-0x1b,3,data_buff);	
			wifi_send_cmd(source_addr,stBusFreq.byCmd-0x1b,3,data_buff);	
			break;
			
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(uiDevState & FLAG_BUSY)
			{	//æ���˳�
				memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
 				data_buff[0] = stBusFreq.byCmd;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ���ʧ������
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ���ʧ������
				break;
			}	
			//����м�¼
			stBusFreq.byCmd -= 0x06;			//���������ԭ�ɶ�Ӧ�ĺ�������
			RemoveCallNod(&(stBusFreq));			
			//�õȴ�Ӧ���æ��־
			uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);	
			if(uiDevState & FLAG_INDICATION)
			{	//���ں���ָʾ,��ָʾ��־				
				uiDevState &= ~FLAG_INDICATION;
				
				//ֹͣ����			
				StopVoicePlay();

				//����ͨ���л�
				VoiceChannelCtx();	

			}	
			
			//�����������з����������з���ַ
			SaveCallAddr(&(stBusFreq));

			//���õȴ�Ӧ��ʱ1S
			MakeCH0TimerOut(50, 0);			
			break;
		case CMD_COMM_CALL:									//��ͨ��������
			if(uiDevState & FLAG_BUSY)
			{	//æ��ֱ���˳�
				break;
			}
		
			//��æ�͵ȴ�Ӧ���־ 
			uiDevState |= (FLAG_BUSY|FLAG_WAIT_ACK);			
			if(uiDevState & FLAG_INDICATION)
			{	//���ں���ָʾ,��ָʾ��־				
				uiDevState &= ~FLAG_INDICATION;
				
				//ֹͣ����			
				StopVoicePlay();

				//����ͨ���л�
				VoiceChannelCtx();								
			}	
			
			//�����������з����������з���ַ
			SaveCallAddr(&(stBusFreq));

			//���õȴ�Ӧ��ʱ1S
			MakeCH0TimerOut(50, 0);
			break;		
		case CMD_ONLINE_WRITE:
		case CMD_ONLINE_READ:
		case CMD_SCHEDULE_SEND:
		case CMD_QUEST:
			//�õȴ�Ӧ���æ��־
			//uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);
			uiDevState |= (FLAG_WAIT_COMM_ACK);
			//����ֻ���ַ
			stCommAddr.bySecAddr = stBusFreq.byRecSecAddr;
			stCommAddr.byRoomAddr = stBusFreq.byRecRoomAddr;
			stCommAddr.byBedAddr = stBusFreq.byRecBedAddr;
			//���õȴ�Ӧ��ʱ1S
			byWaitCommAckDTime = 50;
			break;
		case CMD_OPEN_LCD:
		case CMD_CLOSE_LCD:
		case CMD_CLEAR_LCD:
		case CMD_CLEAR_HANDLE_LCD:
		case CMD_CLEAR_MK_LCD:
			if((stBusFreq.byRecSecAddr == 0xff)||(stBusFreq.byRecRoomAddr == 0xff)||(stBusFreq.byRecBedAddr == 0xff))
			{//�ǹ㲥��ַ

				memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
				data_buff[0] = stBusFreq.byCmd;
				data_buff[1] = TRUE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			}			
			else
			{//����
				//�õȴ�Ӧ���æ��־
				//uiDevState |= (FLAG_WAIT_ACK|FLAG_BUSY);
				uiDevState |= (FLAG_WAIT_COMM_ACK);
				//����ֻ���ַ
				stCommAddr.bySecAddr = stBusFreq.byRecSecAddr;
				stCommAddr.byRoomAddr = stBusFreq.byRecRoomAddr;
				stCommAddr.byBedAddr = stBusFreq.byRecBedAddr;
				//���õȴ�Ӧ��ʱ1S
				byWaitCommAckDTime = 50;			
			}

			if(stBusFreq.byCmd == CMD_CLOSE_LCD)	
			{
				blight_off_state =1;	//�����״̬
			}
			else if(stBusFreq.byCmd == CMD_OPEN_LCD)	
			{
				blight_off_state =0;	//���⿪״̬
			}
			break;
		case CMD_BROADCAST1:								
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:								//�㲥����
			if(uiDevState & FLAG_BUSY)
			{	//æ������ϵͳ��λ����
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;				
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
				data_buff[0] = stBusFreq.byCmd;
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ���ʧ������
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ���ʧ������
				break;
			}
			//��æ����æ��־�͹㲥��־
			uiDevState |= (FLAG_BUSY|FLAG_BROADCAST);
			
			//�����������з����������з���ַ
			SaveCallAddr(&(stBusFreq));

			if(uiDevState & FLAG_INDICATION)
			{
				//�����ָʾ��־
				uiDevState &= ~FLAG_INDICATION;
				//ֹͣ����
				StopVoicePlay();				
			}

			AM79R70_active();		//ʹ�绰оƬ����ͨ��״̬
			set_talk_voice_volue(TALK_SLAVE_CH,stEepromCfgData.bySlaTalkVol);
			
			//����ͨ���л�
			VoiceChannelCtx();

			//���õȴ��㲥��ʱ180S
			MakeCH0TimerOut(250, 36);

			memcpy(&(source_addr.bySecAddr),&(stBusFreq.byRecSecAddr),3);
			data_buff[0] = stBusFreq.byCmd;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			break;

			
		case CMD_VOICE_TIMER_SET:
		case CMD_BL_TIMER_SET:
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			data_buff[0]		=stBusFreq.byCmd;
			data_buff[1]		=TRUE;		//�ɹ���ʧ��
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			break;
			
		case CMD_RS485_BRT_SET:
			byReSndBrtTimes--;
			if(byReSndBrtTimes!=0) 
			{	
				soft_delay_half_ms(1000); //��ʱ1S,�ٷ���һ��
				if(byReSndBrtTimes == RE_SND_TIMES-1)
				{
					source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
					source_addr.byRoomAddr = stEepromCfgData.bySelfRoomAddr;
					source_addr.byBedAddr = stEepromCfgData.bySelfBedAddr;

					data_buff[0] = stBusFreq.byCmd;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//�������һ�η��������
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//�������һ�η��������
				}
				Bus0OutputData(&(stBusFreq.bySndSecAddr));			
			}
			break;


		case CMD_RS485_INFO_SEC_ROOM:
			byReSndRs485RoomTimes--;
			if(byReSndRs485RoomTimes!=0)
			{
				soft_delay_half_ms(1000);	//��ʱ1S,�ٷ���һ��
				if(byReSndRs485RoomTimes == RE_SND_TIMES-1)
				{
					source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
					source_addr.byRoomAddr = stEepromCfgData.bySelfRoomAddr;
					source_addr.byBedAddr = stEepromCfgData.bySelfBedAddr;

					data_buff[0] = stBusFreq.byCmd;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//�������һ�η��������
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//�������һ�η��������
				}
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}
			break;
			
		case CMD_BED_VOICE_PLAY:
			byReSndBedVoiPlaTimes--;
			if(byReSndBedVoiPlaTimes!=0)
			{
				soft_delay_half_ms(1000);	//��ʱ1S,�ٷ���һ��	
				if(byReSndBedVoiPlaTimes == RE_SND_TIMES-1)
				{
					source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
					source_addr.byRoomAddr = GLZJ_ADDR2;
					source_addr.byBedAddr = GLZJ_ADDR3;
					data_buff[0] = stBusFreq.byCmd;
					data_buff[1] = TRUE;
					Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//�������һ�η��������
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//�������һ�η��������
				}
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	
			}
			break;			

		case CMD_SYSTEM_SEC_SET:
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = GLZJ_ADDR2;
			source_addr.byBedAddr = GLZJ_ADDR3;
			data_buff[0] = 0x5a;
			data_buff[1] = TRUE;

			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����

#ifdef  enable_wdt
			feed_wdt();
#endif
			//while(1);
			break;


		case CMD_NUMBER_SET:
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = GLZJ_ADDR2;
			source_addr.byBedAddr = GLZJ_ADDR3;
			data_buff[0] = 0x5b;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			break;

			
		case CMD_DISP_MODE:
			source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
			source_addr.byRoomAddr = GLZJ_ADDR2;
			source_addr.byBedAddr = GLZJ_ADDR3;

			data_buff[0] =stBusFreq.byCmd;
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);		//���Ͳ����ɹ�����
			break;

		case CMD_COLOR_CLEAR:
			uiDevState |= (FLAG_WAIT_COMM_ACK); 
			//���õȴ�Ӧ��ʱ1S
			byWaitCommAckDTime = 50; 
			break;

		case CMD_DATE_SEND:
			if(stEepromCfgData.bySysMode & USE_TIME_DEBUG)
			{
				source_addr.bySecAddr = stEepromCfgData.bySelfSecAddr;
				source_addr.byRoomAddr = GLZJ_ADDR2;
				source_addr.byBedAddr = GLZJ_ADDR3;

				data_buff[0] = stBusFreq.byRecBedAddr;			//��
				data_buff[1] = stBusFreq.byRecRoomAddr;			//��
				data_buff[2] = stBusFreq.byRecSecAddr;			//��
				data_buff[3] = stBusFreq.bySndBedAddr;			//ʱ
				data_buff[4] = stBusFreq.bySndRoomAddr;			//��
				data_buff[5] = stBusFreq.bySndSecAddr;			//��
				Usart2SndCmd(source_addr,stBusFreq.byCmd,6,data_buff);
			}
			break;
			
		default:
			break;

	}
}



/**********************************************************
*��������			:TimerOutDeal
*��������        	:��ʱ�������
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
void TimerOutDeal(void)
{
	STAddr_S source_addr;
	uint8_t data_buff[10];
	
	//ͨ��ͨ��0��ʱ����
	if(stCH0TimerOut.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stCH0TimerOut.byTimerOut--;
		if(stCH0TimerOut.byTimerOut == 0)
		{	//��ʱһ�ε���			
			if(stCH0TimerOut.byTimerOutCount == 0x00)
			{	//���г�ʱ�����ѵ�	
CH0TimerOutDeal:
				if(uiDevState & FLAG_LANDING)
				{	//�ϵ�Ǽ�״̬����λ���зֻ�						
					stBusFreq.bySndSecAddr = 0x00;
					stBusFreq.bySndRoomAddr = 0x00;
					stBusFreq.bySndBedAddr = 0x00;
					stBusFreq.byCmd = CMD_POWER_ON;
					stBusFreq.byRecSecAddr = 0xff;
					stBusFreq.byRecRoomAddr = 0xff;
					stBusFreq.byRecBedAddr = 0xff;
					Bus0OutputData(&(stBusFreq.bySndSecAddr));

					memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
					Usart2SndCmd(source_addr,CMD_LANDING,0,data_buff);		//���͵�½����
					wifi_send_cmd(source_addr,CMD_LANDING,0,data_buff);		//���͵�½����
					byWaitAckTimeLanding = 250;	//�ȴ�5S
					
				}
				else if(uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN|FLAG_TALK|FLAG_BROADCAST))
				{	//�ȴ�Ӧ��ʱ��
					if((uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN)) && ((scon2_Rcvbuf[CMD_POSITION] ==CMD_INFUSION_ANSWER)||(scon2_Rcvbuf[CMD_POSITION] ==CMD_COMM_CALL)) )
					{//����ڵȴ�Ӧ����ߵȴ�����״̬��,�Ҵ����е�����Ϊ������Һ���л���ͨ����,�����������ɹ���
						memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
						data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
						data_buff[1] = FALSE;
						Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
						wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
						scon2_Rcvbuf[CMD_POSITION] =0x00; 	//��մ�����
						
					}
					if(GetState(bHostTalk))
					{
						ClrState(bHostTalk);
						set_talk_voice_volue(TALK_HOST_CH,0);
						set_talk_voice_volue(TALK_SLAVE_CH,0);	
						tel_pickup_time = TEL_PICKUP_TIME;
					}

					if(uiDevState & FLAG_BROADCAST)
					{
						set_talk_voice_volue(TALK_SLAVE_CH,0);
						AM79R70_standby();
					}
					VoiceChannelCtx();
					stBusFreq.bySndSecAddr = stCallAddr.bySndSecAddr;
					stBusFreq.bySndRoomAddr = stCallAddr.bySndRoomAddr;
					stBusFreq.bySndBedAddr = stCallAddr.bySndBedAddr;
					stBusFreq.byCmd = CMD_SYSTERM_RESET;
					stBusFreq.byRecSecAddr = stCallAddr.byRecSecAddr;
					stBusFreq.byRecRoomAddr = stCallAddr.byRecRoomAddr;
					stBusFreq.byRecBedAddr = stCallAddr.byRecBedAddr;;					
					Bus0OutputData(&(stBusFreq.bySndSecAddr));
				}	

			}
			else
			{	//��ʱ����û�е�				
				stCH0TimerOut.byTimerOutCount--;
				if(stCH0TimerOut.byTimerOutCount == 0x00) //���г�ʱ�����ѵ� 
					goto CH0TimerOutDeal;
				else
				{	//���г�ʱ����û�е�,���¼��س�ʱ��λʱ��
					stCH0TimerOut.byTimerOut = stCH0TimerOut.byTimerOutSet;
				}
			}
		}			
	}
	//ͨ��ͨ��1��ʱ����
	if(stCH1TimerOut.byTimerOut != 0)
	{	//�г�ʱ���ô���
		stCH1TimerOut.byTimerOut--;
		if(stCH1TimerOut.byTimerOut == 0)
		{	//��ʱһ�ε���
			if(stCH1TimerOut.byTimerOutCount == 0x00)
			{	//���г�ʱ�����ѵ�
CH1TimerOutDeal:
				stBusFreq.bySndSecAddr =	stCH1CallAddr.bySndSecAddr;
				stBusFreq.bySndRoomAddr = 	stCH1CallAddr.bySndRoomAddr;
				stBusFreq.bySndBedAddr = 	stCH1CallAddr.bySndBedAddr;
				stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
				stBusFreq.byRecSecAddr = stCH1CallAddr.byRecSecAddr;
				stBusFreq.byRecRoomAddr = stCH1CallAddr.byRecRoomAddr;
				stBusFreq.byRecBedAddr = stCH1CallAddr.byRecBedAddr;			
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	

				if(GetState(bHostTalk1)) 
				{
					ClrState(bHostTalk1);
					set_talk_voice_volue(TALK_HOST_CH,0);
					set_talk_voice_volue(TALK_SLAVE_CH,0);	

					tel_pickup_time = TEL_PICKUP_TIME;
				}
				VoiceChannelCtx();
			}
			else
			{	//��ʱ����û�е�
				stCH1TimerOut.byTimerOutCount--;
				if(stCH1TimerOut.byTimerOutCount == 0x00) goto CH1TimerOutDeal;
				else
				{	//���г�ʱ����û�е�,���¼��س�ʱ��λʱ��
					stCH1TimerOut.byTimerOut = stCH1TimerOut.byTimerOutSet;
				}
			}
		}			
	}



	if(byWaitCommAckDTime)
	{
		if(--byWaitCommAckDTime ==0)
		{
			if(uiDevState & FLAG_WAIT_COMM_ACK)
			{//���ڵȴ�״̬
				uiDevState &= ~(FLAG_WAIT_COMM_ACK);
				memcpy(&(source_addr.bySecAddr),&(scon2_Rcvbuf[REC_SEC_POSITION]),3);
				data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
				data_buff[1] = FALSE;
				Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			}		
		}
	}
	
	if(byWaitRS485AckDTime)
	{
		if(--byWaitRS485AckDTime ==0)
		{
			memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
			data_buff[0] = scon2_Rcvbuf[CMD_POSITION];
			data_buff[1] = FALSE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);	
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			stRS485Addr.byBedAddr=0;		//������꣬��485��������
			
		}
	}



	if(byWaitAckTimeLanding)
	{
		if(--byWaitAckTimeLanding== 0x00)
		{	//�ϵ�Ǽǵȴ�Ӧ��ʱ
			uiDetectPcTime = 30000;	//����PC�����10����
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = CMD_PC_CLOSE;
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr =0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr)); 

		}
	}

	
	if(uiDetectPcTime)
	{	//ÿ10���Ӽ��PC��һ��
		if((--uiDetectPcTime) == 0x00)
		{
			memcpy(&(source_addr.bySecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			Usart2SndCmd(source_addr,CMD_LANDING,0,data_buff);	
			wifi_send_cmd(source_addr,CMD_LANDING,0,data_buff);
			byWaitAckTimeLanding = 250;	//�ȴ�5S
		}
	}


	if(byUsart0SdResultDTime)
	{
		if(--byUsart0SdResultDTime==0)
		{

			memcpy(&(source_addr.bySecAddr),&(stRS485Addr.bySecAddr),3);
			data_buff[0] = byUsart0SndBuf[CMD_POSITION];
			data_buff[1] = TRUE;
			Usart2SndCmd(source_addr,CMD_RESULT_PC,2,data_buff);
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff);
			stRS485Addr.byBedAddr=0;		//������꣬��485��������

		}
	}
	return;
}
		



