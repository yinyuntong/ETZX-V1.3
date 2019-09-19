/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.c
*�ļ�����    		:������������
*������     		:������
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
sbit	DealKey = P3^3;									//���ü�	
bit		bLedRedState;									//�ŵƺ�ɫ��״̬
bit		bLedGreenState;									//�ŵ���ɫ��״̬
bit		bLedBlueState;									//�ŵ���ɫ��״̬		
bit		bLedDealState;									//�����״̬ 
  
extern STLocalControl 	xdata stLocalControl;
extern bit   SET_BY_SERIAL_FLAG;
extern bit   SerchRoomFlag;
extern bit   SerchRoomKeyFlag;
extern bit   Broadcasting_Flag;
extern uint16 xdata  SerchRoomCount;
extern uint8  xdata  Modify_Room_Number;
extern uint8  xdata  Modify_SEC_Name;
extern void SaveParameter(void);


extern uint8 xdata byDealKeyErrorDownTime;
extern uint8 xdata byDealKeyShortDownTime;
extern uint8 xdata byDealKeyAlwaysDownTime;

bit   SET_BY_SERIAL_OK_FLAG;
bit   Key_SerchRoom_Flag2;
bit   Key_SerchRoom_Flag3;
/**********************************************************
***********************************************************
*��������			:SetMDLedState	
*��������        	:�����ŵ���ɫ��״̬
*�������   		:byState:bit0-1:���״̬,bit2-3:�̵�״̬
					 bit4-5:����״̬
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SetMDLedState(uint8 byState)
{
	stLocalControl.byMDLedState = byState;					//���������ŵƵ�״̬
	#ifdef DL_SWAP
	bLedRedState   = (bit)(byState & 0x03);
	MDR = !bLedRedState;
	bLedGreenState = (bit)(byState & 0x0C);
	MDG = !bLedGreenState;	
	bLedBlueState  = (bit)(byState & 0x30);			//���������ŵƵ�״̬
	MDB = !bLedBlueState;
	#else
	MDR = bLedRedState   = (bit)(byState & 0x03);
	MDG = bLedGreenState = (bit)(byState & 0x0C);	
	MDB = bLedBlueState  = (bit)(byState & 0x30);			//���������ŵƵ�״̬
	#endif
}
/**********************************************************
*��������			:SetLedDealState	
*��������        	:���ú�ɫLed��״̬
*�������   		:byState:��״̬
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
void SetLedDealState(uint8 byState)
{
	LED_DEAL = (bit)byState;
	bLedDealState = (bit)byState;
	stLocalControl.byLedState &= 0xF0;
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
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
uint8 KeyScan(void)
{
	EA=0;  	
	P3M1 &= 0xF7; 			                                //���ó�������ģʽ
	P3M0 &= 0xF7;
    DealKey = 1;									        //�øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();  
	_nop_();						
	if(0 == DealKey)
	{ 												
		P3M0 |= 0x08;                                      //��ԭ��ǿ�������ģʽ	
		LED_DEAL = bLedDealState; 						   //�ָ�ָʾ�Ƶ�״̬ 

		if((byDealKeyErrorDownTime==0)&&(byDealKeyShortDownTime==0)&&(byDealKeyAlwaysDownTime==0))						//û�н����������̰�������״̬
			byDealKeyErrorDownTime=4;							//20ms����
		EA=1;
		return(DEAL_KEY);
	}  	
	P3M0 |= 0x08;											//��ԭ��ǿ�������ģʽ	
	LED_DEAL = bLedDealState; 							   //�ָ�ָʾ�Ƶ�״̬
	EA=1;
	return(NO_KEY);	
} 

/**********************************************************
*��������			:KeyDownDeal	
*��������        	:�������´�����
*�������   		:byKey:���¼��ļ�ֵ
*����ֵ				: 	
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:Bus0OutputData
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyDownDeal(uint8 byKey)
{ 
    if(Broadcasting_Flag)
	{
	  return;   //�㲥״̬��������
	}
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	}
    if(SET_BY_SERIAL_FLAG)
     {
		SET_BY_SERIAL_FLAG=0;
		stLocalControl.stEepromCfgData.bySelfSecAddr  = stLocalControl.stBusDealFreq.bySndSecAddr;
	    stLocalControl.stEepromCfgData.bySelfRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	    stLocalControl.stEepromCfgData.bySelfBedAddr  = stLocalControl.stBusDealFreq.bySndBedAddr;
		if(FmReadByte(Close_EnSoundNote_ADDR)==0x01)
		stLocalControl.stEepromCfgData.byEnable1=0xF7;
		else
		stLocalControl.stEepromCfgData.byEnable1=CBYTE[0x2803];
		stLocalControl.stEepromCfgData.byEnable2=CBYTE[0x2804];
		stLocalControl.stEepromCfgData.byPrio=CBYTE[0x2805];
		stLocalControl.stEepromCfgData.byRingTime=CBYTE[0x2806];
		stLocalControl.stEepromCfgData.byTalkTime=CBYTE[0x2807];
		stLocalControl.stEepromCfgData.byBroadTime=CBYTE[0x2808];
		stLocalControl.stEepromCfgData.byListenDelay=CBYTE[0x2809];
		stLocalControl.stEepromCfgData.byVolumStep=CBYTE[0x280A];
		stLocalControl.stEepromCfgData.byRingVol=CBYTE[0x280B];
		stLocalControl.stEepromCfgData.byCH0TalkedVol=CBYTE[0x280C];
		stLocalControl.stEepromCfgData.byCH1TalkedVol=CBYTE[0x280D];
		stLocalControl.stEepromCfgData.byBroadVol=CBYTE[0x280E];
		stLocalControl.stEepromCfgData.byNoteVol=CBYTE[0x280F];
		stLocalControl.stEepromCfgData.byMusicVol=CBYTE[0x2810];
		stLocalControl.stEepromCfgData.byCH0TalkVol=CBYTE[0x2811];
		stLocalControl.stEepromCfgData.byCH1TalkVol=CBYTE[0x2812];
		stLocalControl.stEepromCfgData.bySelfRingVol=CBYTE[0x2813];
		stLocalControl.stEepromCfgData.byMaxVol=CBYTE[0x2814];
		stLocalControl.stEepromCfgData.byMinVol=CBYTE[0x2815];
		stLocalControl.stEepromCfgData.byReserve1=CBYTE[0x2816];
		stLocalControl.stEepromCfgData.byReserve2=CBYTE[0x2817];
		stLocalControl.stEepromCfgData.byReserve3=CBYTE[0x2818];
		stLocalControl.stEepromCfgData.bySerialNum1=CBYTE[0x2819];
		stLocalControl.stEepromCfgData.bySerialNum2=CBYTE[0x281A];
		stLocalControl.stEepromCfgData.bySerialNum3=CBYTE[0x281A];
		stLocalControl.stEepromCfgData.byVersionHi=CBYTE[0x281C];
		stLocalControl.stEepromCfgData.byVersionLo=CBYTE[0x281D];
		byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	    byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	    SaveParameter();
	    SetLedDealState(LED_OFF);
		Modify_Room_Number = 0x01;
		FmWriteByte(Modify_Room_Number_ADDR,0x01);
		ShowNumberPageOK();	 		//��ųɹ�����ʾ
		SET_BY_SERIAL_OK_FLAG=1;
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
			bDealKeyDown = 1;								//���洦�ü���״̬			
			if((bConfusionNoting|bServiceNoting|bHelpNoting|bEmergencyNoting))
			{	//�б������д���,�����������
				stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
				stLocalControl.stBusDealFreq.byCmd += 0x0A; 
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.byRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.byBedAddr;						
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Key_SerchRoom_Flag2=0;
		        Key_SerchRoom_Flag3=0;				
			} 			
			else if(bIndicatingOther)
			{	//�������ָʾ�����ֻ��ĺ���,���������ֻ�����
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.byRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.byBedAddr;
				if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
				{	//���������,����Ҫ����,ֱ�����
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
				}
				else
				{	//�������ͺ���,����
					stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
					stLocalControl.stBusDealFreq.byCmd += 0x0A;
				} 				
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				Key_SerchRoom_Flag2=0;
		        Key_SerchRoom_Flag3=0;				
			} 
		   else if(Key_SerchRoom_Flag3)	//�������鷿��¼��ʾ,��ʾʱ��Ϊ5S
		   {
		      Key_SerchRoom_Flag2=0;
		      Key_SerchRoom_Flag3=0;
			  SerchRoomKeyFlag=1;
			  SerchRoomCount=0;
			  SerchRoomFlag=0;
			  ShowSerchRommPage3();
		   }
		   else if(Key_SerchRoom_Flag2)  //�ڶ����鷿��¼��ʾ����ʾʱ��Ϊ5S
		   {
		      Key_SerchRoom_Flag3=1;
			  Key_SerchRoom_Flag2=0;	  
			  SerchRoomKeyFlag=1;
			  SerchRoomCount=0;
			  SerchRoomFlag=0;
		      ShowSerchRommPage2();
		   }
		   else                        //��һ���鷿��¼��ʾ����ʾʱ��Ϊ5S
		   {
		      Key_SerchRoom_Flag2=1;	  
			  Key_SerchRoom_Flag3=0;
			  SerchRoomKeyFlag=1;
			  SerchRoomCount=0;
			  SerchRoomFlag=0;
		      ShowSerchRommPage1();
		   } 
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
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyUpDeal(uint8 byKey)
{
    if(Broadcasting_Flag)
	{
	  return;   //�㲥״̬��������
	}
	if(bLanding)
	{	//�Ǽ�״̬,������	
		return;
	} 
	switch(byKey)
	{
		case DEAL_KEY:			
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
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void KeyAlwaysDeal(uint8 byKey)
{
    if(Broadcasting_Flag)
	{
	  return;   //�㲥״̬��������
	}
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
		if(bNurseIn) 		//��������ʾȫ���㲥
		{
		  stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		  stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		  stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		  stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
		  stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
		  stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
		  stLocalControl.stBusDealFreq.byCmd = CMD_BROADCAST3;  				
		  Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)); 
		  ShowCalling();	
		  SerchRoomKeyFlag=0;
		}		
	}		
}