/*
************************Copyright(c)************************
*	  			   ����һ�عɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.c
*�ļ�����    		:��Ͳ��س���
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
#define _IN_KEY_
#include "config.h"

sbit 	HandlerKey = P4^3;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];  
extern STLocalControl 	xdata stLocalControl;
 
/**********************************************************
*��������			:KeyManager	
*��������        	:��Ͳ����߳�
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
void KeyManager(void)
{
	HandlerKey = 1;											//��Ͳ���ó�������
	while(TRUE)
	{
		WDT_CONTR = 0x3d;	//ι��
		OSTimeDly(OS_TICKS_PER_SEC/40);
		if(HandlerKey == 0)									//��Ͳ����Ϊ�͵�ƽ
		{
			continue;
		}
		OSTimeDly(OS_TICKS_PER_SEC/40);
		if(HandlerKey == 0)
		{
			continue;
		}
		OSQPost(byMainCmdQ, KEY_UP);						//��Ͳժ����
		fled0=led0=1;
		while(HandlerKey == 1)
		{
			WDT_CONTR = 0x3d;	//ι��
			OSTimeDly(OS_TICKS_PER_SEC/50);
		}
		OSQPost(byMainCmdQ, KEY_DOWN);						//��Ͳ�һ���
		fled0=led0=0;
	}
}
/**********************************************************
*��������			:KeyDown	
*��������        	:��Ͳ�һ�������
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
void KeyDown(void)
{
	bHandleDown = 1; 										//������Ͳ��ǰ״̬	
	VoiceChannelCtx();										//����ͨ���л���������
	if((bChannel1Talk&bHandAnswer))
	{	//ȷʵ������Ͳ�������е�,���̳�ʱʱ��,��ʱ���Զ�����ͨ���л�����		
		MakeCH1TimerOut(5, 0);
		
	}	
	else if((bWaitAck|bWaitListen|bChannel0Talk|bSelfBroad)&&bHandAnswer)
	{	//ȷʵ������Ͳ�������е�,���̳�ʱʱ��,��ʱ���Զ����͸�λ����		
		MakeCH0TimerOut(5, 0);
		
	}
	else if((!bHandAnswer)&&(!bKeyAnswer)&&(!bIndicatingOther))
	{				
		InitKeyCallAddr();
	}	
}
/**********************************************************
*��������			:KeyUp	
*��������        	:��Ͳժ��������
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
void KeyUp(void)
{
	bHandleDown = 0;											//������Ͳ��ǰ״̬
	VoiceChannelCtx(); 											//�����л�����Ͳ��
	if(bIndicatingOther&&(!bHandAnswer)&&(!bKeyAnswer))
	{	//������ڱ���,���ʹ����������(�������з����������)		
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stIndicationData.stAddr.bySndSecAddr;
		stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stIndicationData.stAddr.bySndRoomAddr;
		stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stIndicationData.stAddr.bySndBedAddr;				
		if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCallCmd & 0x1f))
		{
			stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
		}
		else
		{	//���ǽ�������,����Ͳ������־,���ʹ�������									
			stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCallCmd & 0x1f;
			stLocalControl.stBusDealFreq.byCmd += 0x06; 
		}		
		if(TRUE == Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr)))
		{
			bCallDealSending = 1;
		}		
	}
	else
	{
		stLocalControl.stBusDealFreq.bySndSecAddr = KEY_CLEAR;
		stLocalControl.stBusDealFreq.byCmd = CMD_KEY_DOWN;
		Bus1OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));		
		stLocalControl.byKeyValue[MAX_KEY_SIZE-1] = 0xff;
		bRoomAddr = 0;
		bSetPrio = 0;
	}
}