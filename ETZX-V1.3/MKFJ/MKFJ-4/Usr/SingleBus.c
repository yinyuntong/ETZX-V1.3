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
extern uint8    xdata  GT23_Rec_Data[128];

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
uint8 idata byBus0TxQ[BUS0_TX_Q_ZISE];						//����0���Ͷ���							
uint8 data byBus0TxHead = 0;								//������0���Ͷ���ͷָ��
uint8 data byBus0TxTail = 0; 								//������0���Ͷ���βָ��
#define IncBus0TxPtr(addr)    {addr=(addr+1)%BUS0_TX_Q_ZISE;}
#define Bus0TxBuffLen()	      ((byBus0TxTail+BUS0_TX_Q_ZISE-byBus0TxHead)%BUS0_TX_Q_ZISE)
//���ͺͽ������Ŷ���
sbit  Bus0RecPin	= P3^2;									//������1�������Ŷ���
sbit  Bus0SendPin	= P4^3;									//������1�������Ŷ���

extern STUartControl    idata stUartControl;
extern STLocalControl 	xdata stLocalControl;
uint16  xdata PageNumber=0;
uint16  xdata SerchRoomCount=0;					           //�鷿��ʱ�������
uint16  xdata SET_BY_SERIAL_COUNT=0;					   //��ŵ�ʱ�������	 
uint8   xdata SerchRoom=0;
uint8   xdata Real_TimeBuf[5]={0x00,0x00,0x00,0x00,0x00};
uint32  xdata INSPERCTOR_Count=0;
uint16  xdata BusLow_Count=0;
uint16  xdata Self_BusLow_Count=0;
uint16	xdata Self_BusHigh_Count=0;
bit   BusLow_Flag;
bit   Self_BusLow_Flag;
bit   SerchRoomFlag;
bit   SerchRoomKeyFlag;
bit   SET_BY_SERIAL_FLAG;                                 //�����߶��豸��ű�־λ
bit   HaveSerchRoomFlag;
bit   Page_Flag;
bit   INSPERCTOR_Flag;
bit   INDICTION_Flag;
bit   NUMBER_SET_STOP_FLAG;
bit   AllBusLow_Flag;
bit   BusLowRenewOK_Flag;
extern bit   Display_BusLow_Flag;
extern uint8 xdata  Modify_Room_Number;
extern uint8 xdata  Modify_SEC_Name;
extern bit Broadcasting_Flag;
extern bit CheckBusLow_Flag;
uint16 xdata  INDICTION_Count=0; 
//---------------------------------
//---------------------------------
//2012/5/26��Ӷ����ʿ��λ
uint16 xdata NurseComeCount=0;
uint8  xdata NurseComeRoom[3]={0,0,0};
//---------------------------------
//---------------------------------
extern uint8 xdata  Page_Color[5]; 
extern bit Key_SerchRoom_Flag2;
extern bit Key_SerchRoom_Flag3;
//uint16 xdata AllBusLow_Count=0;
extern uint8  xdata Fbaud_Type;
extern uint32 xdata Fbaud;   //�����ʵ���ֵ


extern bit bLcdOn;
uint8 xdata BusLowDTime=100;

bit bBus0SndBitOk =0;

extern void Delayms(uint16 ms);


void ReadPageColour(void)
{
    uint16 xdata fcolor,bcolor,ccolor;
	uint8  xdata byData1,byData2,byData3;
    if(Modify_SEC_Name==0x01)
    {
      byData1=Page_Color[0];//������ɫ 
	  byData2=Page_Color[1];//���屳��ɫ,Ҳ������ɫ
	  byData3=Page_Color[2];//�м����ɫ
	  if(byData1==0x00) 
	  fcolor= BLACK;
	  else if(byData1==0x01) 
	  fcolor= RED;
	  else if(byData1==0x02) 
	  fcolor= GREEN;
	  else if(byData1==0x03) 
	  fcolor= BLUE1;
	  else if(byData1==0x04) 
	  fcolor= BLUE;
	  else if(byData1==0x05) 
	  fcolor= YELLOW;
	  else if(byData1==0x06) 
	  fcolor= PURPLE;
	  else if(byData1==0x07) 
	  fcolor= CYAN;
	  else if(byData1==0x08) 
	  fcolor= WHITE;
	  else
	  fcolor=WHITE;//������ɫ
      if(byData2==0x00) 
	  bcolor= BLACK;
	  else if(byData2==0x01) 
	  bcolor= RED;
	  else if(byData2==0x02) 
	  bcolor= GREEN;
	  else if(byData2==0x03) 
	  bcolor= BLUE1;
	  else if(byData2==0x04) 
	  bcolor= BLUE;
	  else if(byData2==0x05) 
	  bcolor= YELLOW;
	  else if(byData2==0x06) 
	  bcolor= PURPLE;
	  else if(byData2==0x07) 
	  bcolor= CYAN;
	  else if(byData2==0x08) 
	  bcolor= WHITE;
	  else
	  bcolor=BLUE1; 
	  if(byData3==0x00) 
	  ccolor= BLACK;
	  else if(byData3==0x01) 
	  ccolor= RED;
	  else if(byData3==0x02) 
	  ccolor= GREEN;
	  else if(byData3==0x03) 
	  ccolor= BLUE1;
	  else if(byData3==0x04) 
	  ccolor= BLUE;
	  else if(byData3==0x05) 
	  ccolor= YELLOW;
	  else if(byData3==0x06) 
	  ccolor= PURPLE;
	  else if(byData3==0x07) 
	  ccolor= CYAN;
	  else if(byData3==0x08) 
	  ccolor= WHITE;
	  else
	  ccolor=YELLOW; 
    }
	else
	{
	  fcolor=WHITE;//������ɫ
	  bcolor=BLUE1; //���屳��ɫ,Ҳ������ɫ
	  ccolor=YELLOW;//�����뼸�����������м������ɫ
	}
    Lcd_Clear(bcolor,0);
	Lcd_Fill(0,168,480,10,ccolor);
    if(Modify_SEC_Name==0x01)
    {
       byData1=Page_Color[3];//������ɫ 
	   byData2=Page_Color[4];//���屳��ɫ,Ҳ������ɫ
	   if(byData1==0x00) 
	   fcolor= BLACK;
	   else if(byData1==0x01) 
	   fcolor= RED;
	   else if(byData1==0x02) 
	   fcolor= GREEN;
	   else if(byData1==0x03) 
	   fcolor= BLUE1;
	   else if(byData1==0x04) 
	   fcolor= BLUE;
	   else if(byData1==0x05) 
	   fcolor= YELLOW;
	   else if(byData1==0x06) 
	   fcolor= PURPLE;
	   else if(byData1==0x07) 
	   fcolor= CYAN;
	   else if(byData1==0x08) 
	   fcolor= WHITE;
	   else
	   fcolor=WHITE;//������ɫ
       if(byData2==0x00) 
	   bcolor= BLACK;
	   else if(byData2==0x01) 
	   bcolor= RED;
	   else if(byData2==0x02) 
	   bcolor= GREEN;
	   else if(byData2==0x03) 
	   bcolor= BLUE1;
	   else if(byData2==0x04) 
	   bcolor= BLUE;
	   else if(byData2==0x05) 
	   bcolor= YELLOW;
	   else if(byData2==0x06) 
	   bcolor= PURPLE;
	   else if(byData2==0x07) 
	   bcolor= CYAN;
	   else if(byData2==0x08) 
	   bcolor= WHITE;
	   else
	   bcolor=WHITE; 
    }
	else
	{
	   fcolor=BLUE1;//������ɫ
	   bcolor=WHITE;//���屳��ɫ,Ҳ������ɫ
	}
    Lcd_Fill(0,178,480,94,bcolor);	
} 
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
	//����0���ͽ�����Ϊ������������ս�����Ϊ��������
	P4M1 &= 0xF7;
    P4M0 |= 0x08;
	P3M1 |= 0x04;
    P3M0 &= 0xFB;
	//���ߵ�ƽ����
	Bus0SendPin = 0;
	Bus0RecPin  = 1;	
	TMOD |= 0x01;
	TL0 = TIMER0_L;
	TH0 = TIMER0_H;
	TF0 = 0;												//����жϱ�־
	AUXR &= 0x7F;											//��ͳ12��Ƶ�ٶ�  
   	ET0 = 1;												//����ʱ��0�ж� 
	TR0 = 1;												//������ʱ��
	//������������
	byBus0SendStopCount = 240;								//�ϵ�����0��ֹ����ʱ������
	IE0 = 0;
	IT0 = 1;												//�ⲿ�ж�0Ϊ�½��ش���ģʽ   			
	if(Bus0RecPin)
	{	//�����������,���ж�			
		EX0 = 1;
	}
	else
	{	//������߲�����,�����߹��ϱ�־			
		bBus0Error = 1;
	} 
	PageNumber=0;	
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
					Bus0SendPin = 0;						//�ͷ�����	 				
				}				
				byBus0SendStopCount = 240;					
				byBus0RecSendCount = 0x00;					//���ճ���,���ý��շ��ͼ���ֵ
				EnableBus0RecInt();
				PW = bPWState; 								//�ָ��������ŵĿ�������
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
							PW = bPWState;					//�ָ��������ŵĿ�������
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
		{//����һ������λ���м�
			if(bBus0SendBit == bBus0RecBit) bBus0SndBitOk =1;
			else
			{	//����ͬ,����ʧ��								
				byBus0State0 &= ~BUS0_SEND_CON;
				byBus0RecSendCount &= 0xf0;
				bBus0SendError = 1;					
				Bus0SendPin = 0; 
				byBus0SendStopCount = 240;

				bBus0SndBitOk = 0;
			}
		}
		
		//if(bBus0SendBit == bBus0RecBit)   	
		else if(bBus0SndBitOk ==1)
		{	//���͵����ݺͽ��յ�������ͬ
			//byBus0SendCount += 0x10;
			if(0xa0 == (byBus0SendCount & 0xf0))
			{	//һλ���ݷ������,���ȷ��͵�����ʼλ
				bBus0SndBitOk =0;
				
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
			PW = bPWState;
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

	EA=0;
	if(Bus0TxBuffLen() >= (BUS0_TX_Q_ZISE - 1))
	{	//û�пռ�洢��,ʧ��	
		EA=1;;
		return(FALSE);
	}	
	while(byTemp--)
	{	//�����뷢�Ͷ���
		byBus0TxQ[byBus0TxTail] = *pbyData++;
		IncBus0TxPtr(byBus0TxTail);
	}
	EA=1;;	
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
uint8 BcdToHex(uint8 byData)
{
	uint8 byRet;
	byRet = byData >> 4;
	byRet *= 10;
	byData &= 0x0f;
	byRet += byData;
	return(byRet);
}
/**********************************************************
/**********************************************************
*��������			:AddrCompareCheck	
*��������        	:�Ƚϼ��ʱ�ĵ�ַ
*�������   		:pstBusFreq:���Ƚϵ�����ָ֡��
*����ֵ				:1:���͵��Ǽ��ʱ����,0:���Ƿ��ͼ�⵽������	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2012-3-14
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
bit AddrCompareCheck(pSTBusFreq pstBusFreq)
{
	if((pstBusFreq->byRecSecAddr==0x01)&&(pstBusFreq->byRecRoomAddr==0x02)&&(pstBusFreq->byRecBedAddr==0x03))
	{	
		return(1);
	} 
	else
    return(0);
}
/**********************************************************
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
	stLocalControl.stCH0TimerOut.byTimerOutSet   = byTimerOut;
	stLocalControl.stCH0TimerOut.byTimerOut      = byTimerOut;
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
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/   
void MakeCH1TimerOut(uint8 byTimerOut, uint8 byTimerOutCount)
{
	stLocalControl.stCH1TimerOut.byTimerOutSet   = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOut      = byTimerOut;
	stLocalControl.stCH1TimerOut.byTimerOutCount = byTimerOutCount;	
} 
/**********************************************************
*��������			:SaveIndicationData	
*��������        	:������Ϣָʾ��������
*�������   		:pstBus0SendFreq:�����������ָ֡��
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
void SaveIndicationData(pSTBusFreq pstBus0SendFreq)
{ 	
	stLocalControl.stIndicationData.stAddr.bySecAddr  = pstBus0SendFreq->bySndSecAddr;
	stLocalControl.stIndicationData.stAddr.byRoomAddr = pstBus0SendFreq->bySndRoomAddr;
	stLocalControl.stIndicationData.stAddr.byBedAddr  = pstBus0SendFreq->bySndBedAddr;
	stLocalControl.stIndicationData.byCallCmd         = pstBus0SendFreq->byRecSecAddr; 
}
/**********************************************************
*��������			:SaveCallAddr	
*��������        	:��������ͨ������ַ
*�������   		:pstBus0SendFreq:�����������ָ֡��
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
void SaveCallAddr(pSTBusFreq pstBusFreq)
{ 	
	stLocalControl.stCallAddr.bySecAddr  = pstBusFreq->bySndSecAddr;
	stLocalControl.stCallAddr.byRoomAddr = pstBusFreq->bySndRoomAddr;
	stLocalControl.stCallAddr.byBedAddr  = pstBusFreq->bySndBedAddr;
}
/**********************************************************
*��������			:LedControl	
*��������        	:ָʾ����ʾ״̬����
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:SetLedSetState,SetLedDealState
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/ 
void LedControl(void)
{
	if(bNurseIn)
	{
		SetMDLedState(LED_ON<<4);         //��ʿ��λ�ŵ�ָʾΪ��ɫ
	}
	else if((bConfusionNoting|bServiceNoting|bHelpNoting))
	{
		SetMDLedState(LED_ON<<2);//��ɫ	  //����ͨ�����ŵ���ʾΪ��ɫ
	}
	else if(bEmergencyNoting)
	{ 		
		SetMDLedState(LED_FLASH<<2); //�н��������ŵ���ʾΪ��ɫ��˸
	}
	else
	{
		SetMDLedState(0);
	}
	if(bIndicatingOther)
	{
	    SetLedDealState(LED_FLASH);	//������������У��ſڴ������˸
	}
	else if((bChannel1Talk|bChannel1Talked|bChannel0Talk|bChannel0Talked|bSelfBroad|bSickRoomBroad|bOfficeBroad|bAllBroad))
	{	
		SetLedDealState(LED_ON);	//�豸����ͨ��״̬�趨����Ƴ���
	} 
	else
	{
		SetLedDealState(LED_OFF);
	} 	
}
/**********************************************************
*��������			:VoiceChannelCtx	
*��������        	:����ͨ���л�������
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
void VoiceChannelCtx(void)
{ 	
	if(bChannel1Talked)	  //ͨ��1ΪSA
	{	//ͨ��1����ͨ��
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkedVol;
		CloseCGB();
		OpenCBD();
		CTD = 0;
		CTA = 1;
		bPWState =  PW = 0;			
	}
	else if(bChannel1Talk)
	{ 	//ͨ��1����ͨ��
		CCAP1H = stLocalControl.stEepromCfgData.byCH1TalkVol;
		CloseCGB();
		OpenCBD();
		CTD = 0;
		CTA = 1;
		bPWState =  PW = 0;		
	} 
	else if((bSickRoomBroad|bOfficeBroad|bAllBroad))	//�㲥״̬ΪSDͨ��
	{	//�㲥״̬
		CCAP1H = stLocalControl.stEepromCfgData.byBroadVol;
		CloseCBD();
		OpenCGB();
		CTA = CTD = 0;
		bPWState = PW = 0;		
	}
	else if(bChannel0Talked)  //ͨ��2ΪSD
	{	//ͨ��0����ͨ��״̬
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkedVol;
		CloseCBD();
		OpenCGB();
		CTA = 0;
		CTD = 1;
		bPWState = PW = 0;		
	}
	else if(bChannel0Talk)
	{	//ͨ��0����ͨ��״̬
		CCAP1H = stLocalControl.stEepromCfgData.byCH0TalkVol;
		CloseCBD();
		OpenCGB();
		CTA = 0;
		CTD = 1;
		bPWState = PW = 0;		
	}	
	else if(bSelfBroad)	   	//�㲥״̬ΪSDͨ��
	{	//�����㲥״̬
		CCAP1H = stLocalControl.stEepromCfgData.byBroadVol;
		CloseCBD();
		OpenCGB();
		CTA = 0;
		CTD = 1;
		bPWState = PW = 0;			
	}
	else if((bCalledRing|bWaitListen))	//���Źر�SA��SDͨ��
	{	//������������ߵȴ�����
		CCAP1H = stLocalControl.stEepromCfgData.bySelfRingVol;
		CloseCBD();
		OpenCGB();
		CTA = CTD = 0;
		bPWState = PW = 0;		
	}
	else if(bIndicatingOther&&bEnSoundNote)	 //����ΪSDͨ��
	{	//����ָʾ�����ֻ�����ɱ���
		CCAP1H = stLocalControl.stEepromCfgData.byRingVol;
		CloseCBD();
		OpenCGB();
		CTA = CTD = 0;
		bPWState = PW = 0;		
	}
	else if(bMusicPlaying)
	{	//�������ֲ���״̬
		CCAP1H = stLocalControl.stEepromCfgData.byMusicVol;
		CloseCGB();
		OpenCBD();
		CTA = CTD = 0;
		bPWState = PW = 0;				
	}
	else if(bVoiceNoting)
	{	//������ʾ
		CCAP1H = stLocalControl.stEepromCfgData.byNoteVol;
		CloseCGB();
		OpenCBD();
		CTA = CTD = 0;
		bPWState = PW = 0;
	}
	else
	{	//û���κ�����״̬����		
		CCAP1H = 10;
		CloseCGB();
		CloseCBD();
		CTA = CTD = 0;
		bPWState = PW = 1;
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
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SysReset(void)
{	
    Broadcasting_Flag=0;
	bBusy = bWaitAck = bWaitListen = bCalledRing = bChannel0Talked = bChannel0Talk = bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;			
	MakeCH0TimerOut(0, 0);	
	VoiceChannelCtx();
	LedControl();
	bIndication = 0; 
	SetLcdState(1);		
	ShowPage();	
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
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Bus0RecDeal(void)
{
	uint8 idata timebuf[5];
	//ȡ���յ�������֡ 	 		
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0RecData, sizeof(STBusFreq));
	bBus0RecFinish = 0;	
	EA=1;;	
	if(bLanding && (stLocalControl.stBusDealFreq.byCmd != CMD_ENTER))
	{	//����ǵǼ�״̬,�յ�������ǵǼ�ȷ������,��������
		return;
	}
	//-----------------------------------------
	//-----------------------------------------
	//����ʹ��
	 //SBUF=stLocalControl.stBusDealFreq.byCmd;
	//while(!TI);
	//TI=0;		
	//-----------------------------------------
	//-----------------------------------------
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
	  /*case CMD_ENTER:										//ȷ�ϵǼ�����
			if(bLanding)
			{	
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 					 					
					SetLedDealState(LED_OFF);
					SetMDLedState(LED_OFF);
					SetLcdState(1);
					ReadPageColour();	
					ShowPage();								
					byDevState1 = stLocalControl.stBusDealFreq.byRecSecAddr & 0x80;										
					MakeCH0TimerOut(0, 0); 										
				}
			}			
			break;*/
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		//�����Զ��������--2012.3.13
		case CMD_BUS0_CHECK:	 //�����߼��
			RST_BUS=0;	
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_BUS0_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
			break;	

		case CMD_SD_TAL_VOL_CHECK:
			RST_BUS=0;	 //SD��SAͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			CTD=1;
			OpenCGB();
			CloseCBD();
			PW =0;	//�򿪹���34119
			break;

		case CMD_SD_TAL_VOL_CHECK_END:
			RST_BUS=0;	//SD��SAͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SD_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;
			

		case CMD_SA_TAL_VOL_CHECK:
			RST_BUS=0;	//SD��SAͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			CTA = 1;
			CloseCGB();
			OpenCBD();
			PW=0;	//�򿪹���34119
			break;


		case CMD_SA_TAL_VOL_CHECK_END:
			RST_BUS=0;	//SD��SAͨ������
			
			stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
			stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SA_TAL_VOL_CHECK_END;
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			CTA = 0;
			CloseCBD();
			PW=1;	//�رչ���34119
			break;
		//---------------------------------------------------------------------
		case CMD_QUEST:										//��ѯ����
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byCmd = CMD_ANSWER; 			
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));			
			break;			
		case CMD_NURSE_COME:							//��ʿ��λ������ǻ�ʿ�鷿����	 		
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))  //...�Ƚ����źͷ���
			{		
				bNurseIn = 1;                          //��ȷ�ϵ��豸�Ǳ�����,������ʿ���ﱾ��,�õ�λ��־   				
				LedControl();
				VoiceChannelCtx();  
				INSPERCTOR_Count=0;
				//----------------------------------------------------------------
				//----------------------------------------------------------------
				//2012/5/26��Ӷ����ʿ��λ
				NurseComeCount++;
				if(NurseComeCount==1)
				{
				  NurseComeRoom[0]=stLocalControl.stBusDealFreq.bySndBedAddr;
				  INSPERCTOR_Flag=0;
				}
				else if(NurseComeCount>1)
				{
                  if(NurseComeRoom[0]==stLocalControl.stBusDealFreq.bySndBedAddr)
				  goto  NurseIN;
				  if(NurseComeRoom[1]==0x00)
				  {
				     if(stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[0]&&stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[2])
					 {
				       NurseComeRoom[1]=stLocalControl.stBusDealFreq.bySndBedAddr;
				       INSPERCTOR_Flag=0;
				       goto  NurseIN;  
					 }
				  }
				  else if(NurseComeRoom[1]==stLocalControl.stBusDealFreq.bySndBedAddr)
				  goto  NurseIN;				  	
				  if(NurseComeRoom[2]==0x00)
				  {
				     if(stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[0]&&stLocalControl.stBusDealFreq.bySndBedAddr!=NurseComeRoom[1])
					 {
				       NurseComeRoom[2]=stLocalControl.stBusDealFreq.bySndBedAddr;
				       INSPERCTOR_Flag=0;
					   goto  NurseIN;
					 }
				  }
				  else if(NurseComeRoom[2]==stLocalControl.stBusDealFreq.bySndBedAddr)
				  goto  NurseIN;	
				}
				//----------------------------------------------------------------
				//----------------------------------------------------------------
				//2012/4/24��������ͬʱ��Ϊ�ǲ鷿����
NurseIN:		if(!INSPERCTOR_Flag)
		         {
			         INSPERCTOR_Flag=1;
			         SerchRoom++;
				     if(SerchRoom>15)	//�鷿����15�εĻ������»ص���1ҳ������ʾ
				     {
				       PageNumber++;
				       SerchRoom=1;
				     }			   
		       		 if(PageNumber==1)   //��һ���Ѿ�д��
		       		 {
		          	   Page_Flag=1;
			           FmWriteByte(Serch_Room_Page_ADDR,INIT_FLAG);
		             }							   
		   		     FmWriteByte(Serch_Room_ADDR, SerchRoom);//�洢�ڼ��β鷿����

					 
		   			 if(SerchRoom==1)	  //�����һ�β鷿
		        	 {
			      	   FmWriteByte(Serch_Room_ADDR+0x1000, stLocalControl.stBusDealFreq.bySndBedAddr);
				  	   HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x1000,5,Real_TimeBuf);
			         }
		   		     else if(SerchRoom==2) //����ڶ��β鷿
		             { 
			           FmWriteByte(Serch_Room_ADDR+0x2000, stLocalControl.stBusDealFreq.bySndBedAddr);
			           HaveSerchRoomFlag=1; 
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x2000,5,Real_TimeBuf);
			         }
		   		     else if(SerchRoom==3)//��������β鷿
		             {
			           FmWriteByte(Serch_Room_ADDR+0x3000, stLocalControl.stBusDealFreq.bySndBedAddr);
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x3000,5,Real_TimeBuf);
			         }
		   		     else if(SerchRoom==4)//������Ĵβ鷿
		             {
		               FmWriteByte(Serch_Room_ADDR+0x4000, stLocalControl.stBusDealFreq.bySndBedAddr);
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x4000,5,Real_TimeBuf);
		             }
		   		     else if(SerchRoom==5)//�������β鷿
		             {
			           FmWriteByte(Serch_Room_ADDR+0x5000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x5000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==6)//��������β鷿
		             {
			           FmWriteByte(Serch_Room_ADDR+0x6000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x6000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==7)//������ߴβ鷿
		             {
			           FmWriteByte(Serch_Room_ADDR+0x7000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0x7000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==8)//����ڰ˴β鷿
		             {
			            FmWriteByte(Serch_Room_ADDR+0x8000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			            HaveSerchRoomFlag=1;
				        FmWriteBytes(Serch_Room_TIME_ADDR+0x8000,5,Real_TimeBuf);
		        	 }
				     else if(SerchRoom==9)//����ھŴβ鷿
		             {
			            FmWriteByte(Serch_Room_ADDR+0x9000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			            HaveSerchRoomFlag=1;
				        FmWriteBytes(Serch_Room_TIME_ADDR+0x9000,5,Real_TimeBuf);
		             }
				     else if(SerchRoom==10)//�����ʮ�β鷿
		             {
			            FmWriteByte(Serch_Room_ADDR+0xA000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			            HaveSerchRoomFlag=1;
				        FmWriteBytes(Serch_Room_TIME_ADDR+0xA000,5,Real_TimeBuf);
		             }
				    else if(SerchRoom==11)//�����ʮһ�β鷿
		            {
			           FmWriteByte(Serch_Room_ADDR+0xB000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0xB000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==12)//�����ʮ���β鷿
		            {
			           FmWriteByte(Serch_Room_ADDR+0xC000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0xC000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==13)//�����ʮ���β鷿
		            {	 
			           FmWriteByte(Serch_Room_ADDR+0x2D000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			           HaveSerchRoomFlag=1;
				       FmWriteBytes(Serch_Room_TIME_ADDR+0xD000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==14)//�����ʮ�Ĵβ鷿
		            {	
			          FmWriteByte(Serch_Room_ADDR+0x2E000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			          HaveSerchRoomFlag=1;
				      FmWriteBytes(Serch_Room_TIME_ADDR+0xE000,5,Real_TimeBuf);
		            }
				    else if(SerchRoom==15)//�����ʮ��β鷿
		            {
			          FmWriteByte(Serch_Room_ADDR+0x2F000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			          HaveSerchRoomFlag=1;
				      FmWriteBytes(Serch_Room_TIME_ADDR+0xF000,5,Real_TimeBuf);
		            } 
		          }	
			    //--------------------------------------------------------------------------------------
				//--------------------------------------------------------------------------------------					
			 }
			 //-----------------------------------------------------------------------------------------
			 //-----------------------------------------------------------------------------------------
			 //ȡ�������л�ʿ��λǰ���£������ʿ��λ��ķ��䣬�ŵƻ�ʿ��λָʾ��Ϩ������������ֻ�е���ʿ�뿪���Ϩ��
			/*else if(bNurseIn)
			{					
				bNurseIn = 0;	                     //���ǵ�λ�����䵫�����Ļ�ʿ��λ��־��λ��,���֮ǰ�Ļ�ʿ��λ��־ 				
				LedControl();
				VoiceChannelCtx(); 			
			} */
			//------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------
			break;
		case CMD_NURSE_BACK:							//��ʿ�뿪����
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f))) //...�Ƚ����źͷ���
			{	//������豸�Ǳ�����,������ʿ�뿪����,�嵽λ��־
				bNurseIn = 0;					
				LedControl();
				VoiceChannelCtx();				
			}  			
			break;		
		case CMD_COMM_CALL:	 								//��ͨ��������
			if(!bBusy)
			{ 	//ͨ��0����			
				bBusy = 1;
				if(bIndicatingOther)
				{	//�������ָʾ�����ֻ�,ָֹͣʾ	
				    //----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------									 					
					bIndication = 0; 
			        SetLcdState(1);			
			        ShowPage();	
				}				
				//������к���ָʾ��־
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();							//�����л�	 				
				//�����������з���ַ
				SaveCallAddr(&(stLocalControl.stBusDealFreq));			
				MakeCH0TimerOut(250, 0);					//���ó�ʱ5s
				if(AddrCompare(&(stLocalControl.stBusDealFreq)))
				{ 	//���б����豸	
					if(!(bChannel1Talk|bChannel1Talked))
					{	//ͨ��1����,�򱾻�Ϊ����״̬,����Ӧ������
						stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_COMM_ANSWER;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}				
				}
				LedControl();								//ָʾ���л�
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
				{	
				   	//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------										
					bIndication = 0; 
			        SetLcdState(1);			
			        ShowPage();	 				
				}
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();
				LedControl();
				if((bChannel1Talk|bChannel1Talked))
				{
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
						bAllBroad = bEnAllBroad; 	//ȫ���㲥״̬					
						break;
				}
				VoiceChannelCtx();
				LedControl();
				ShowBROADCASTing();
			}
			break;			
		case CMD_INFO_INDICATION:							//�յ�����ָʾ���� 														
			if(!bBusy)		
			{			
				SaveIndicationData(&(stLocalControl.stBusDealFreq));
				if((stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
					((stLocalControl.stBusDealFreq.bySndRoomAddr&0x7f) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7f)))
				{	//�յ����Ǳ����ĺ���ָʾ,����Ҫ������,����ϴεĺ���ָʾ
					if(bIndicatingOther)
				    {	
					   //----------------------------------------------------------------------------------------
					   //----------------------------------------------------------------------------------------
					   bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				       VoiceChannelCtx();
			           LedControl();
					   //----------------------------------------------------------------------------------------
					   //----------------------------------------------------------------------------------------										
					   bIndication = 0; 
			           SetLcdState(1);			
			           ShowPage();	 				
				    }
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
					switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
					{
						case CMD_INFUSION_CALL:	   			//��Һ����
							bConfusionNoting = bEnInfusionDeal;												
							break;
						case CMD_SERVICE_CALL:				//�������
							bServiceNoting = bEnServiceDeal;											
							break;
						case CMD_HELP_CALL:					//��Ԯ����
							bHelpNoting = bEnHelpDeal;											
							break;
						case CMD_EMERGENCY_CALL: 			//��������
							bEmergencyNoting = bEnEmergencyDeal;											
							break;
						default:
							return;
					}
					VoiceChannelCtx();
					LedControl(); 					
				}
				else
				{	//�յ��Ĳ��Ǳ����ĺ���ָʾ,�ж��Ƿ񱾻��ܹ�����
				/*	bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
					VoiceChannelCtx();
				    LedControl();
					switch(stLocalControl.stBusDealFreq.byRecSecAddr & 0x1f)
					{
						case CMD_INFUSION_CALL:	   			//��Һ����
							if(!bEnInfusionDeal)
							{	
								return;													
							} 												
							break;
						case CMD_SERVICE_CALL:				//�������
							if(!bEnServiceDeal)
							{	
								return;							
							} 											
							break;
						case CMD_HELP_CALL:					//��Ԯ����
							if(!bEnHelpDeal)
							{	
								return;							
							} 											
							break;
						case CMD_EMERGENCY_CALL: 			//��������
							if(!bEnEmergencyDeal)
							{
								return;						
							}  											
							break;
						default:
							return;
					} */
					bIndicatingOther = 1;
					if((bChannel1Talked|bChannel1Talk))
					{	//���ͨ��1����ͨ��״̬,��������
						return;
					}	
					VoiceChannelCtx();
					LedControl();
					bIndication = 1;
			        ShowCallFace(stLocalControl.stBusDealFreq.bySndRoomAddr, stLocalControl.stBusDealFreq.bySndBedAddr, stLocalControl.stIndicationData.byCallCmd&0x1f);			
				} 		
			} 			
			break;
		case CMD_INFUSION_ANSWER:							//������Һ��������
		case CMD_SERVICE_ANSWER:							//��������������
		case CMD_EMERGENCY_ANSWER:							//���������������
		case CMD_HELP_ANSWER:								//������Ԯ��������
			if(!bBusy)
			{	//ͨ��0��æ				
				bBusy = 1;
			    if(bIndicatingOther)
				{
				   	//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------					
					bIndication = 0; 
			        SetLcdState(1);			
			        ShowPage();										
				} 				
				//�����������з���ַ				
				SaveCallAddr(&(stLocalControl.stBusDealFreq));				
				MakeCH0TimerOut(250, 0);
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();				
				LedControl();  
			}
			break;
		case CMD_STOP_INDICATION:							//ָֹͣʾ����
			 if(bIndicatingOther)
			 { 	
				//----------------------------------------------------------------------------------------
			    //----------------------------------------------------------------------------------------
			    bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				VoiceChannelCtx();
			    LedControl();
			    //----------------------------------------------------------------------------------------
			    //----------------------------------------------------------------------------------------		
				bIndication = 0; 
			    SetLcdState(1);			
			    ShowPage();	
			 } 
			 bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
			 VoiceChannelCtx();
			 LedControl();
			 break;
		case CMD_INFUSION_CLEAR: 							//�����Һ��������
		case CMD_SERVICE_CLEAR: 							//��������������
		case CMD_HELP_CLEAR: 								//�����Ԯ��������
		case CMD_EMERGENCY_CLEAR: 							//���������������			 
			if(	(stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stIndicationData.stAddr.bySecAddr) &&
				(stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stIndicationData.stAddr.byRoomAddr) &&
				(stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stIndicationData.stAddr.byBedAddr) &&
				((stLocalControl.stBusDealFreq.byCmd-0x0a) == (stLocalControl.stIndicationData.byCallCmd & 0x1f)))
			{	//�����������ָʾ�ĺ�����Ϣ 
			   if(bIndicatingOther)
				{ 	
				    //----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				    VoiceChannelCtx();
			        LedControl();
					//----------------------------------------------------------------------------------------
					//----------------------------------------------------------------------------------------
					bIndication = 0; 
					SetLcdState(1);			
					ShowPage();						
				} 				
				bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
				VoiceChannelCtx();
				LedControl();	
			}	
			break; 			
		case CMD_SYSTERM_RESET:								//ϵͳ��λ����
			SysReset();
			break; 
		case CMD_START_VOICE:								//������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{	
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
				bVoiceNoting = bMusicPlaying = 0;
				VoiceChannelCtx();					
			}
			break; 
		case CMD_CHANNEL_CHANGE: 							//ͨ���л�����	
			//��ͨ��0��״̬�л���ͨ��1��,ͬʱ���ͨ��0��״̬				 			
			bBusy = bWaitAck = bWaitListen = bCalledRing =  
				bSickRoomBroad = bOfficeBroad = bAllBroad = bSelfBroad = 0;			
			MakeCH0TimerOut(0, 0);	
			//-----------------------------------------------------------------------------------------------
			//-----------------------------------------------------------------------------------------------
			//2012/5/18�����Ǳ����ֻ�ʱ�ĺ���ָʾ	
			if(bIndicatingOther)
		    {
			    bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
				VoiceChannelCtx();
			    LedControl();				
			    bIndication = 0; 
			    SetLcdState(1);			
			    ShowPage();											
			} 
			//-----------------------------------------------------------------------------------------------
			//-----------------------------------------------------------------------------------------------	
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
			}	
			break;
		case CMD_CHANNEL_CLOSE: 							//�ر�ͨ���л�����			
			if((bChannel1Talked|bChannel1Talk))
			{		//���ͨ��1����ͨ��״̬,��������ָʾ�µ���Ϣ,��û��,����״̬
				if(bIndicatingOther)
				{	//�����ָʾ,������ʾָʾ��Ϣ 
				  bIndication = 0; 
				  SetLcdState(1);			
				  ShowPage();
				}
			}
			bChannel1Talked = bChannel1Talk = 0; 				
			MakeCH1TimerOut(0, 0);	
			VoiceChannelCtx();
			LedControl();			
			break;
         /*	case CMD_OPEN_485BUS_IN:							//��485�������	
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{ 
				KDR = 0;
				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_485BUS_IN;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byCmd = 0x02;							
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
			}
			break;
		case CMD_CLOSE_485BUS:  							//�ر�485���
			KDR = 1;   			
			break;*/
		//------------------------------------------------------
		//------------------------------------------------------
		//2012/4/9����ϵͳ������
		case CMD_SET_BAUD:
		     if(stLocalControl.stBusDealFreq.bySndSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)
			 {
		       Fbaud_Type=stLocalControl.stBusDealFreq.byRecSecAddr;
			   FmWriteByte(Set_Baud_ADDR,INIT_FLAG);
			   FmWriteByte(System_Baud_ADDR,Fbaud_Type);
			   switch(Fbaud_Type)
	           {
	             case 1:
		         Fbaud=1200;
		         break;
		         case 2:
		         Fbaud=2400;
		 	     break;
		         case 3:
		         Fbaud=4800;
		         break;
		         case 4:
		         Fbaud=9600;
		         break;
		         case 5:
		         Fbaud=14400;
		         break;
		         case 6:
		         Fbaud=19200;
		         break;
		         case 7:
		         Fbaud=28800;
		         break;
		         case 8:
		         Fbaud=38400;
		         break;
		         case 9:
		         Fbaud=57600;
		         break;
		         case 10:
		         Fbaud=115200;
		         break;
	            }
	           if(Fbaud>2000)
	           {
			     AUXR |= 0x40;	           //1Tģʽ 
	             TL1=TH1=256-(Fosc/32/Fbaud);
	           }
	           else
	           {
	  			 AUXR &= 0xBF;
	  			 TL1=TH1=256-(Fosc/32/Fbaud/12);
	           }
			 }  
		    break;
		//------------------------------------------------------
		//------------------------------------------------------
		//2012/5/8����ϵͳ��������
		case CMD_SET_SEC_SERIAL:
			 stLocalControl.stEepromCfgData.bySelfSecAddr=stLocalControl.stBusDealFreq.byRecSecAddr;
			 SaveParameter();
			 ReadPageColour();
			 SetLcdState(1);	
			 ShowPage();
		break;
		//------------------------------------------------------
		//------------------------------------------------------
		case CMD_OPEN_LCD:									//��LCD������ʾ
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_OPEN_LCD;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   SetLcdState(1);
			 } 
		     break;
		case CMD_CLOSE_LCD:									//�ر�LCD������ʾ
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_CLOSE_LCD;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   SetLcdState(0);
			 }
		     break;
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		 case CMD_ENABLE_SOUND:									//���������������
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_ENABLE_SOUND;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   FmWriteByte(Close_EnSoundNote_ADDR,0x00);
		       byEnable1=0xFF;
			 } 
		     break;
		case CMD_DISABLE_SOUND:									//�ر������������� 
		     if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			 {
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_DISABLE_SOUND;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   FmWriteByte(Close_EnSoundNote_ADDR,0x01);
		       byEnable1=0xF7;
			 }
		     break;
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		case CMD_SEC_CLR:									//�����������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{
			   stLocalControl.stBusDealFreq.byRecSecAddr = CMD_SEC_CLR;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
			   stLocalControl.stBusDealFreq.byCmd = 0x02;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
			   stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			   Modify_SEC_Name	= 0x00;
			   FmWriteByte(LCD_SEC_MSK_ADDR, 0);
			   SetLcdState(1);
			   ReadPageColour();	 				 				
			   ShowPage(); 
			}
			break;			
		case CMD_POWER_ON:	  								//��������������
			if(AddrCompare(&(stLocalControl.stBusDealFreq)))
			{					
				EA=0;
				//�ر����д򿪵��ж�
				CCAPM0 = 0x00;
				ET0 = 0;
				TR0 = 0;
				EX0 = EX1 = 0;
				IAP_CONTR = 0x20; 		
				break;			 				
			}
			break;	
		case CMD_SET_BY_SERIAL:								 //�յ���ŵ�����	
		     if(stLocalControl.stBusDealFreq.bySndBedAddr==0x00) //����Ϊ0��ʾΪ�ſڷֻ����
			 {
			   SET_BY_SERIAL_FLAG=1;
			   SetLedDealState(LED_FLASH);
			   ShowNumberPage();
			 }
			 break;
		 /*case  CMD_INSPERCTOR_ENTER:  //�յ��鷿������͸�TFT��ʾ��
		 {
		   if(!INSPERCTOR_Flag)
		   {
		      if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr) &&
				((stLocalControl.stBusDealFreq.byRecRoomAddr&0x7F) == (stLocalControl.stEepromCfgData.bySelfRoomAddr&0x7F)))  //...�Ƚ����źͷ���
			  {
			    INSPERCTOR_Flag=1;
			    SerchRoom++;
				if(SerchRoom>15)	//�鷿����15�εĻ������»ص���1ҳ������ʾ
				{
				   PageNumber++;
				   SerchRoom=1;
				}			   
		       if(PageNumber==1)   //��һ���Ѿ�д��
		       {
		          Page_Flag=1;
			      FmWriteByte(Serch_Room_Page_ADDR,INIT_FLAG);
		       }							   
		   		FmWriteByte(Serch_Room_ADDR, SerchRoom);//�洢�ڼ��β鷿����
		   		if(SerchRoom==1)	  //�����һ�β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x1000, stLocalControl.stBusDealFreq.bySndBedAddr);
				  HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x1000,5,Real_TimeBuf);
			    }
		   		else if(SerchRoom==2) //����ڶ��β鷿
		        { 
			      FmWriteByte(Serch_Room_ADDR+0x2000, stLocalControl.stBusDealFreq.bySndBedAddr);
			      HaveSerchRoomFlag=1; 
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x2000,5,Real_TimeBuf);
			    }
		   		else if(SerchRoom==3)//��������β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x3000, stLocalControl.stBusDealFreq.bySndBedAddr);
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x3000,5,Real_TimeBuf);
			    }
		   		else if(SerchRoom==4)//������Ĵβ鷿
		        {
		          FmWriteByte(Serch_Room_ADDR+0x4000, stLocalControl.stBusDealFreq.bySndBedAddr);
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x4000,5,Real_TimeBuf);
		         }
		   		else if(SerchRoom==5)//�������β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x5000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x5000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==6)//��������β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x6000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x6000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==7)//������ߴβ鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x7000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x7000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==8)//����ڰ˴β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x8000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x8000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==9)//����ھŴβ鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x9000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0x9000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==10)//�����ʮ�β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0xA000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xA000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==11)//�����ʮһ�β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0xB000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xB000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==12)//�����ʮ���β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0xC000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xC000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==13)//�����ʮ���β鷿
		        {	 
			      FmWriteByte(Serch_Room_ADDR+0x2D000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xD000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==14)//�����ʮ�Ĵβ鷿
		        {	
			      FmWriteByte(Serch_Room_ADDR+0x2E000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xE000,5,Real_TimeBuf);
		        }
				else if(SerchRoom==15)//�����ʮ��β鷿
		        {
			      FmWriteByte(Serch_Room_ADDR+0x2F000, stLocalControl.stBusDealFreq.bySndBedAddr); 
			      HaveSerchRoomFlag=1;
				  FmWriteBytes(Serch_Room_TIME_ADDR+0xF000,5,Real_TimeBuf);
		        } 
		      }
		   }
	 	 }
		 break;	*/
	   case   CMD_DATA_SEND:   
	   {
		  Real_TimeBuf[4]=BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr); // ��ȡʵʱʱ��
		  Real_TimeBuf[3]=BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1F);
		  Real_TimeBuf[2]=BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3F);
	      Real_TimeBuf[1]=BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3F); 
		  Real_TimeBuf[0]=BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7F); 
		  if(HaveSerchRoomFlag)
		  {
		    HaveSerchRoomFlag=0;
			timebuf[0]=BcdToHex(stLocalControl.stBusDealFreq.bySndRoomAddr & 0x7F);	//��
		    timebuf[1]=BcdToHex(stLocalControl.stBusDealFreq.bySndBedAddr & 0x3F);	//ʱ
			timebuf[2]=BcdToHex(stLocalControl.stBusDealFreq.byRecSecAddr & 0x3F);	//��
			timebuf[3]=BcdToHex(stLocalControl.stBusDealFreq.byRecRoomAddr & 0x1F); //��
			timebuf[4]=BcdToHex(stLocalControl.stBusDealFreq.byRecBedAddr);		   //��
			if(SerchRoom==1)
			 { 
				FmWriteBytes(Serch_Room_TIME_ADDR+0x1000,5,timebuf);
			 }
			else if(SerchRoom==2)
			 {
				FmWriteBytes(Serch_Room_TIME_ADDR+0x2000,5,timebuf);
			 }
			else if(SerchRoom==3)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x3000,5,timebuf);
			}
			else if(SerchRoom==4)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x4000,5,timebuf);
			}
			else if(SerchRoom==5)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x5000,5,timebuf);
			}
			else if(SerchRoom==6)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x6000,5,timebuf);
			}
			else if(SerchRoom==7)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x7000,5,timebuf);
			}
			else if(SerchRoom==8)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x8000,5,timebuf);
			}
			else if(SerchRoom==9)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0x9000,5,timebuf);
			}
			else if(SerchRoom==10)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xA000,5,timebuf);
			}
			else if(SerchRoom==11)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xB000,5,timebuf);
			}
			else if(SerchRoom==12)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xC000,5,timebuf);
			}
			else if(SerchRoom==13)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xD000,5,timebuf);
			}
			else if(SerchRoom==14)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xE000,5,timebuf);
			}
			else if(SerchRoom==15)
			{
			   FmWriteBytes(Serch_Room_TIME_ADDR+0xF000,5,timebuf);
			} 
		 }
	   }
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
	EA=0;
	memcpy(&(stLocalControl.stBusDealFreq), byBus0SendData, sizeof(STBusFreq));		
	bBus0SendFinish = 0;										
	EA=1;	
	switch(stLocalControl.stBusDealFreq.byCmd)
	{
	   /*case CMD_LANDING: 									//�Ǽ�����
			if(bLanding)
			{	//����ȷʵ���ڵǼ�״̬,���õȴ�ȷ�ϳ�ʱ
				MakeCH0TimerOut(150, 0);								
			}			
			break;*/
		case CMD_COMM_CALL:	 								//��ͨ�������� 							
		case CMD_INFUSION_ANSWER:							//������Һ����
		case CMD_SERVICE_ANSWER:							//����������
		case CMD_EMERGENCY_ANSWER:							//�����������
		case CMD_HELP_ANSWER:								//������Ԯ����							
			if(!bBusy)
			{
				bBusy = bWaitAck = 1;
				SaveCallAddr(&(stLocalControl.stBusDealFreq)); 
				MakeCH0TimerOut(250, 0); 						
			}		
		case CMD_INFUSION_CLEAR:							//�����Һ����
		case CMD_SERVICE_CLEAR:								//����������
		case CMD_HELP_CLEAR:								//�����Ԯ����
		case CMD_EMERGENCY_CLEAR:							//�����������
			//ֹͣ����ָʾ�ĺ���	
			if(bIndicatingOther)
			{
			  //----------------------------------------------------------------------------------------
			  //----------------------------------------------------------------------------------------
			  bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;	
			  VoiceChannelCtx();
			  LedControl();
			  //----------------------------------------------------------------------------------------
			  //----------------------------------------------------------------------------------------				
			  bIndication = 0; 
			  SetLcdState(1);			
			  ShowPage();			
			}
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
			VoiceChannelCtx();
			LedControl();			
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
				VoiceChannelCtx();
				LedControl(); 				
				if(bEnAutoListen)
				{
					stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_CALL_LISTEN;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
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
				{	//�����Ѿ�����ͨ��״̬��,���̳�ʱ,�˳� 					
					MakeCH0TimerOut(5, 0);
					break;
				}
				//����ͨ����ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byTalkTime);				
				VoiceChannelCtx();
				LedControl();
			}
			break;
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:   								//�㲥����
			//ֹͣ����ָʾ�ĺ���
			if(bIndicatingOther)
			{				
			  bIndication = 0; 
			  SetLcdState(1);			
			  ShowPage();
			}
			bConfusionNoting = bServiceNoting = bHelpNoting = bEmergencyNoting = bIndicatingOther = 0;
			VoiceChannelCtx(); 			
			if(!bBusy)
			{ 	
				bBusy = 1;			  	
			  	//�����������з���ַ(������ַ)
				SaveCallAddr(&(stLocalControl.stBusDealFreq));				
				bSelfBroad = 1;
				if((bChannel1Talk|bChannel1Talked)||(!bDealKeyDown))
				{	//�����㲥������������,���̳�ʱ,�˳�			
					MakeCH0TimerOut(5, 0);
					break;
				}
				//���ù㲥��ʱ
				MakeCH0TimerOut(50, stLocalControl.stEepromCfgData.byBroadTime);
				VoiceChannelCtx();				
			}
			LedControl();			
			break; 			
		case CMD_CHANNEL_CLOSE:								//�ر�����ͨ������
			bChannel1Talk = bChannel1Talked = 0; 			
			MakeCH1TimerOut(0, 0);
			VoiceChannelCtx();
			LedControl();
			if(bIndicatingOther)
			{ 
			  bIndication = 0; 
			  SetLcdState(1);			
			  ShowPage();
			}	
			break;	
		case CMD_GET_BUS:									//ռ������
			Bus0SendPin = 1;								//�������߹���
			SaveParameter();
			Bus0SendPin = 0;								//�ͷ�����
			PW = bPWState; 									//�ָ��������ŵĿ�������			
			break;	
		case CMD_SYSTERM_RESET:	 							//ϵͳ��λ����
			SysReset();
			break;
		case CMD_BUS_ANSWER:
			switch(stLocalControl.stBusDealFreq.byRecSecAddr)
			{
				case CMD_SD_TAL_VOL_CHECK_END:
					CTD=0;
					CloseCGB();
					PW=1; //�رչ���34119
					break;
			}
			break;
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
*������	      		:�ܼ�ǿ
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
				else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
				{	//�ȴ�Ӧ�𣬵ȴ�����������ͨ���������㲥״̬			
				 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
					stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
					stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
					stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
					stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
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
					else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
					{	//�ȴ�Ӧ�𣬵ȴ�����������ͨ���������㲥״̬		
					 	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
						stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
						stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
						stLocalControl.stBusDealFreq.byCmd = CMD_SYSTERM_RESET;
						stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stCallAddr.bySecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stCallAddr.byBedAddr;
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
				{	//��ʱ����û����ɣ����¼��ص�λ��ʱʱ�� 			
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
				if(bChannel1Talk)
				{	//����ͨ��״̬�������л�����			
					stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;						
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bChannel1Talked)
				{	//����ͨ��״̬���Լ���λ
					bChannel1Talked = 0;
					if(bIndicatingOther)
					{ 
					   bIndication = 0; 
			           SetLcdState(1);			
			           ShowPage();	
					}
					VoiceChannelCtx();
					LedControl();
				}	
			}
			else
			{	//��ʱ����û����
				stLocalControl.stCH1TimerOut.byTimerOutCount--;
				if(stLocalControl.stCH1TimerOut.byTimerOutCount == 0x00)
				{ 	//���г�ʱ���
					if(bChannel1Talk)
					{	//����ͨ��״̬�������л�����						
						stLocalControl.stBusDealFreq.byCmd = CMD_CHANNEL_CLOSE;						
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
					}
					else if(bChannel1Talked)
					{	//����ͨ��״̬���Լ���λ
						bChannel1Talked = 0;
						if(bIndicatingOther)
						{ 
						  bIndication = 0; 
			              SetLcdState(1);			
			              ShowPage();	
						}
						VoiceChannelCtx();
						LedControl();
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
	if(--stLocalControl.byLedFlashTime == 0x00)
	{
		stLocalControl.byLedFlashTime = 50;
		if((stLocalControl.byLedState & 0x0f) == LED_FLASH)
		{ 	//�����˸״̬	
			bLedDealState = !bLedDealState;
			LED_DEAL = bLedDealState;
		}
	}
	if(--stLocalControl.byLedFlashDelay == 0x00)
	{
		stLocalControl.byLedFlashDelay = 25;
		if((stLocalControl.byMDLedState&LED_FLASH) == LED_FLASH)
		{	//�����˸״̬
			bLedRedState = !bLedRedState;
			#ifdef DL_SWAP
			MDR = !bLedRedState;
			#else
			MDR = bLedRedState;
			#endif
		}
		if((stLocalControl.byMDLedState&(LED_FLASH<<2)) == (LED_FLASH<<2))
		{	//�̵���˸״̬
			bLedGreenState = !bLedGreenState;
			#ifdef DL_SWAP
			MDG = !bLedGreenState;
			#else
			MDG = bLedGreenState;
			#endif
		}  
		if((stLocalControl.byMDLedState&(LED_FLASH<<4)) == (LED_FLASH<<4))
		{	//������˸״̬
			bLedBlueState = !bLedBlueState;
			#ifdef DL_SWAP
			MDB = !bLedBlueState;
			#else
			MDB = bLedBlueState;
			#endif
		}
	}

	//������Ļ��
	if (bLcdOn)
	{
		LCD_LED=0;
		LCD_LED=1;
	}
	else LCD_LED=0;

	
	if(SET_BY_SERIAL_FLAG)//����յ��豸��������ʼ5�붨ʱ
	{
		SET_BY_SERIAL_COUNT++;
	}
	 else	SET_BY_SERIAL_COUNT=0;

	 
	 if(SET_BY_SERIAL_COUNT>250)		//5��ʱ�䵽
	 {
	   	SET_BY_SERIAL_FLAG=0;
		SET_BY_SERIAL_COUNT=0;
		SetLedDealState(LED_OFF);
		NUMBER_SET_STOP_FLAG=1;
	 }
	 
	if(SerchRoomKeyFlag)  SerchRoomCount++;
	else SerchRoomCount=0;
	
	if(SerchRoomCount>300)			//6��ʱ�䵽
	{ 
	  SerchRoomCount=0;
	  SerchRoomFlag=1;
	  SerchRoomKeyFlag=0;
	  Key_SerchRoom_Flag2=0;
	  Key_SerchRoom_Flag3=0;
	}

	
	if(INSPERCTOR_Flag)  INSPERCTOR_Count++;
	else INSPERCTOR_Count=0;
	if(INSPERCTOR_Count>3000) //���λ�ʿ�鷿��ʱ����ʱ��������1���Ӳ���Ч��¼ 
	{
	  INSPERCTOR_Count=0;
	  INSPERCTOR_Flag=0;
      //---------------------------------
      //---------------------------------
      //2012/5/26��Ӷ����ʿ��λ
	  memset(NurseComeRoom,0,3);
	  NurseComeCount=0;
      //---------------------------------
      //---------------------------------
	}

	
	//-----------------------------------
	if(INDICTION_Flag)  INDICTION_Count++;
	else  INDICTION_Count=0;
	if(INDICTION_Count>50)	// 1�뵽
	{
	  INDICTION_Count=0;
	  INDICTION_Flag=0;
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
				Delayms(400);

				
				if(Bus0RecPin ==0)
				{//�����й��� //ʹ�ƿ���
					ShowSelfError();     //Һ������ʾ��������
					do
					{
						RST_BUS =1;	//�Ͽ�SD������
						Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
						
						SetLedDealState(1);
						Delayms(100);		//��ʱ200ms

						SetLedDealState(0);
						Delayms(100);
					}while(Bus0RecPin==0);	//һֱ�ȴ�Ϊ��

					
					RST_BUS = 0;	//ʹSD��·��ͨ					
					BusLowDTime = 100;
					//ʹ�ƻָ�����
					SetLedDealState(0);
					ShowNormal();
				}

				
				else 
				{//���ⲿ��������
					ShowBusError();
					RST_BUS = 0;	//ʹSD��·��ͨ
					Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ

					do
					{
							
						RST_BUS = 0;	//ʹSD��·��ͨ
						Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
						
						SetLedDealState(1);	//������
						Delayms(400);		//��ʱ600ms 
						SetLedDealState(0);
						Delayms(400);
						
					}while(Bus0RecPin==0);	//һֱ�ȴ�Ϊ��
					

					//ʹ�ƻָ�����
Bus0BugRet:
					SetLedDealState(0);
					ShowNormal();
					Bus0SendPin =0;	//ʹSDΪ�ߵ�ƽ
					RST_BUS = 0;	//ʹSD��·��ͨ					
					BusLowDTime = 100;

				}				
			}
		}
	}	
}
	  