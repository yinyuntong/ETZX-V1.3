/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.c
*�ļ�����    		:������������
*������     		:����ͬ
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:���ð��������ʾ���ƹ���һ��I/O�ڵķ�ʽ					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_KEY_
#include "config.h"


bit		bLedDealState;										//�����״̬				
					
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;

/**********************************************************
*��������			:SetLedDealState	
*��������        	:���ô����״̬
*�������   		:byState:��״̬
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
void SetLedDealState(uint8 byState)
{
	LED_DEAL = (bit)byState;
	bLedDealState = (bit)byState;
	stLocalControl.byLedState &= 0xf0;
	stLocalControl.byLedState |= byState;
}

/**********************************************************
*��������			:KeyScan	
*��������        	:����ɨ�躯��
*�������   		:
*����ֵ				:NO_KEY:�ް�������,SET_KEY:���ü�����
					 DEAL_KEY:���ü�����   	
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
uint8 KeyScan(void)
{
	OS_ENTER_CRITICAL();  	
	P1M1 &= 0xfb;											//���ó�������ģʽ
	DealKey = 1;									//�øߵ�ƽ
	_nop_();
	_nop_();
	_nop_(); 
	_nop_(); 						
	
	if(0 == DealKey)
	{ 		
		P1M1 |= 0x04;										//��ԭ��ǿ�������ģʽ
		LED_DEAL = bLedDealState; 	 							//�ָ�ָʾ�Ƶ�״̬  
		OS_EXIT_CRITICAL();
		return(DEAL_KEY);
	} 
	
	P1M1 |= 0x04;											//��ԭ��ǿ�������ģʽ
	LED_DEAL = bLedDealState; 						//�ָ�ָʾ�Ƶ�״̬
	OS_EXIT_CRITICAL();

	return(NO_KEY);	
} 
/**********************************************************
*��������			:KeyManager	
*��������        	:���������߳�
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:byMainCmdQ
*����ģ��  			:KeyScan,OSQPost
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyManager(void)
{ 	
	static uint8 byKeyValue;
	static uint8 byKeyDelay;
	
    while(TRUE)
    {

//		OxTimerTreat();

		
        OSWait(K_TMO, OS_TICKS_PER_SEC/50); 				//20ms��ʱ          					   
        byKeyValue = KeyScan();              			
        if(NO_KEY == byKeyValue)
        {			       	
        	continue;
        }        
        OSWait(K_TMO, OS_TICKS_PER_SEC/50);    				//20ms��ʱ����       					
        if(byKeyValue != KeyScan())
        { 			
            continue;
        }
        OSQPost(byMainCmdQ, KEY_DOWN|byKeyValue);
//		Send_Data_Byte(KEY_DOWN|byKeyValue);
		byKeyDelay = KEY_DELAY;
		while(byKeyDelay--)
		{
			if(byKeyValue == KeyScan())
			{	
				OSWait(K_TMO, OS_TICKS_PER_SEC/50); 
			}
			else
			{ 	
				goto KeyUp;
			}
		}		
		OSQPost(byMainCmdQ, KEY_ALWAYS|byKeyValue);				                                              
        while(byKeyValue == KeyScan())
        {	
			OSWait(K_TMO, OS_TICKS_PER_SEC/20);  			       
        }
KeyUp:
		OSQPost(byMainCmdQ, KEY_UP|byKeyValue);	
//		Send_Data_Byte(KEY_UP|byKeyValue);
		WDT_CONTR = 0x3d;		//@20MHz,1.25s

    }
}
/**********************************************************
*��������			:KeyDownDeal	
*��������        	:�������´�����
*�������   		:byKey:���¼��ļ�ֵ
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
void KeyDownDeal(uint8 byKey)
{ 
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	}
	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr; 
	switch(byKey)
	{

		case DEAL_KEY:
			bLedDealState = LED_DEAL =1;
			bDealKeyDown = 1;								//���洦�ü���״̬
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;					
			if(bNumSeting)	//���״̬
			{
				bNumSeting=0;
				stLocalControl.byNumSetTime=0;
				stLocalControl.stBusDealFreq.bySndSecAddr=stLocalControl.stCallAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr=stLocalControl.stCallAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr=stLocalControl.stCallAddr.bySndBedAddr;
				stLocalControl.stBusDealFreq.byCmd=	CMD_NUMBER_SET;
				stLocalControl.stBusDealFreq.byRecSecAddr=0x02;		//�ɹ�
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			
				//�����µĵ�ַ����						
				stLocalControl.stEepromCfgData.bySelfSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
				stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
				stLocalControl.stEepromCfgData.bySelfBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;					

//				stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER;
//				stLocalControl.stBusDealFreq.byRecSecAddr = CMD_NUMBER_SET;			
				stLocalControl.stBusDealFreq.byCmd = CMD_GET_BUS;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}

			if(bRFNumSeting)	//��Һ����������״̬
			{
				bRFNumSeting=0;
				stLocalControl.uiRFNumSetTime=0;
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),RFSerial,3);
				memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(RFSerial[3]),3);
				stLocalControl.stBusDealFreq.byCmd= CMD_RF_NUMSET_OK;	//�ɹ� 	
				Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

				memcpy(&(stLocalControl.stEepromCfgData.byRFSerialNum1),RFSerial,6);
				//������Һ���������к�
				WDT_CONTR = 0x3d; 	//ι��
				EA=0;
				SaveParameter(IAP0_ADDR);
				EA=1;
				memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),RFSerial,3);
				stLocalControl.stBusDealFreq.byCmd = CMD_RF_NUMSET_OK;	//�ɹ� 
				memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(RFSerial[3]),3);			
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
				return;
			}


			//������ڱ����ĺ���,���´��ü�,��������ĺ���	
			if(bConfusionNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			if(bServiceNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			if(bHelpNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_HELP_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			if(bEmergencyNoting)
			{
				stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}
			//�������ָʾ�����ֻ��ĺ���,���������ֻ�����
			if((bNurseIn&bIndicatingOther))
			{
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;
				if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
				{	//���������,����Ҫ����,ֱ�����
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				}
				else
				{	//�������ͺ���,����
					if(/*stLocalControl.stIndicationData.stAddr.bySecAddr==WIRELESS_SEC&&*/
					   stLocalControl.stIndicationData.stAddr.bySndRoomAddr==ADD_BED_FJ)
					{//�Ӵ��ֻ�����,����Ҫ����,ֱ�����
						stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
					}
					else
					{
						stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
						stLocalControl.stBusDealFreq.byCmd += 0x06;
					}
				} 				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				return;
			}

			if(bVoiceNoting || bMusicPlaying)
			{//������ʾ״̬���߱������ֲ���״̬
				if(bCloseCGB)
				{//�㲥��״̬,������Ӧ�رչ㲥
					bCloseCGB=0;
					CloseCGB();
					bMUTState = MUT =OFF;
				}
				else
				{
					bCloseCGB=1;
					OpenCGB();
					bMUTState = MUT =ON;					
				}
			}
			break;

		default:
			break;
	}		
}
/**********************************************************
*��������			:KeyUpDeal	
*��������        	:������������
*�������   		:byKey:������ļ�ֵ
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:MakeCH0TimerOut,MakeCH1TimerOut
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
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	} 
	switch(byKey)
	{
		case DEAL_KEY:
			bLedDealState = LED_DEAL =0;
			bDealKeyDown = 0; 								//���洦�ü���״̬	  			
			if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad))
			{	//ͨ��0�������л���ͨ��״̬,���ٸ�ͨ����ʱʱ��				
				MakeCH0TimerOut(5, 0);
			}
			else if(bChannel1Talk)
			{	//ͨ��1����ͨ��״̬,���ٸ�ͨ����ʱʱ��
				MakeCH1TimerOut(5, 0);
			}			
			break;
		default:
			break;
	}		
}
/**********************************************************
*��������			:KeyAlwaysDeal	
*��������        	:���������´�����
*�������   		:byKey:�������ļ�ֵ
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
void KeyAlwaysDeal(uint8 byKey)
{
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	} 
	switch(byKey)
	{		
		case DEAL_KEY:
			if((bBusy|bChannel1Talk|bChannel1Talked))
			{	//�������Ǵ��ڿ���״̬,�����κδ���
				break;
			} 			
			if(bNurseIn)
			{	//��ʿ��λ,���Ͱ칫���㲥����
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
				stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
				stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST2;  				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 			
			}			
			break; 	
		default:
			break;
	}		
}
