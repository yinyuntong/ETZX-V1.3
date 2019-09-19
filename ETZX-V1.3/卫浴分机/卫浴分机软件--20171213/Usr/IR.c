#define _IN_IR_
#include "config.h"
#include "string.h"


uint8 xdata MoveFjNum=0;		//�ƶ��ֻ�����


uint8 xdata UartRecData;
uint8 xdata IrDataPosit=0;

uint16  uiNurseInTime;						//��⻤ʿ��λ��ʱʱ��
uint8 	byIrDataIntervalTimer;				//�������ݽ��չ������ֽ�֮����ʱ��
uint8 	byNumSetTime;						//���ʱ��

uint8 xdata byIrDARecBuff[8], byIrDADealBuff[6];		//�������������������

bit bFourByteRec=0;					//���ڽ��յ�4�ֽ�����
bit bSixByteRec=0;					//���ڽ��յ�6�ֽ����


bit	bSetKeyDown =0;				//���ü�����״̬
bit	bNurseIn	=0;	 			//��ʿ��λ��־

/**********************************************************
*��������			:IRInit	
*��������        	:������ճ�ʼ��
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2011-2-16
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void IRInit(void)
{
	RXD=1;
	TXD=1;

	SCON = 0x50;	//����ģʽ1
	PCON &= 0x7F;	//���ڲ�������λ0:����ԭ��
	TMOD &= 0X0F;		//TO��ʽ����
	TMOD |= 0X20;		//T1 8λ�Զ�����	
	TH1 = 0xD8;
	TL1 = 0xD8;		//����������Ϊ1200	

	PCON &= (~SMOD);   // �����ʱ���ѡ��		smod=0
	AUXR &= (~T1x12);		//��ʱ��1ʱ�ӿ���,0:Fosc/12(��ͳ��12��Ƶ) 	
	
	TF1=0;
	TR1 = 1;

	
	RI=0;
	TI=0;
	REN=1;
	ES = 1;
	
}
/**********************************************************
*��������			:comm	
*��������        	:�����жϣ������������
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2011-2-16
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
#pragma disable
void comm(void) interrupt 4
{	 
	if(TI==1)
		TI=0;
	if(RI==1)
	{
		RI=0;
		UartRecData = ~SBUF;	//��λȡ��
		byIrDataIntervalTimer=5;		//ÿ�ֽڼ��������100ms
		if(IrDataPosit==0)
		{	
			if(UartRecData==0x05)	//��ʼ��
				byIrDARecBuff[IrDataPosit++]=	UartRecData;
		}
		
		else 
		{
			byIrDARecBuff[IrDataPosit++]=	UartRecData;
			if(IrDataPosit >=7)   IrDataPosit =0;

		}	
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
			uiNurseInTime=NURSE_IN_TIME;				//���5����δ�յ���ʿ��λ�������Ϊ��ʿ�뿪
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
				MoveFjNum= byIrDADealBuff[1];
			}
			bNurseIn=1;	
			break;


			
		case IrDA_CMD_INSPECTION:			//��ʿ�鷿����	
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

	if(byIrDADealBuff[5] == byIrDADealBuff[0]+byIrDADealBuff[1]+byIrDADealBuff[2]+byIrDADealBuff[3]+byIrDADealBuff[4])
	{//У�����ȷ
		if(byIrDADealBuff[3] ==0)
		{//��������ֻ����
			switch(byIrDADealBuff[4])
			{
				case 0x5a:
					bNumSeting=1;
					byNumSetTime = 250;
					stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;
					SetLedRedState(LED_FLASH);
					bySec =  byIrDADealBuff[1];	//����
					byRoom = byIrDADealBuff[2];//����
					break;

					
				default:
					break;
			}
		}
	}
}
