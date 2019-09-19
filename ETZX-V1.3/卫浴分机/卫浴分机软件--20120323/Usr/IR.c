#define _IN_IR_
#include "config.h"
#include "string.h"

bit  IRByteEnd;
bit  FiveSec_Flag;
bit  IRHaveRec_Flag;
bit  NurseComeHave_Flag;
bit  IR_SET_BY_SERIAL_FLAG;
//uint8 IRDATA[4];
//uint8 TEMPIRDATA[4];
uint8 xdata IRDATA[6]={0,0,0,0,0,0};
uint8 xdata TEMPIRDATA[6]={0,0,0,0,0,0};
uint8 xdata IRDATASum=0;
uint8 xdata IRByteCount=0;

uint8 xdata MoveFjNum=0;		//�ƶ��ֻ�����

extern uint16 xdata TimerCount;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;
extern bit SET_BY_SERIAL_FLAG;
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
	SCON = 0x50;
	PCON &= 0x7F;
	TH1 = 0xD8;
	TL1 = 0xD8;		//����������Ϊ1200		
	TR1 = 1;
	RI=0;
	TI=0;
	ES = 1;
	IRByteCount=0;
	IRDATASum=0;
	memset(IRDATA,0,6);
	memset(TEMPIRDATA,0,6);
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
	if(RI == 1)
	{ 
		RI = 0;	
		IRDATA[IRByteCount]=SBUF;
		IRDATA[IRByteCount]=~IRDATA[IRByteCount];
		IRByteCount++;
        if(IRByteCount==4) //�˴����ж��Ǳ�����ݻ��ǻ�ʿ��λ��Ϣ
		{
		   IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2];
		   if(IRDATASum==IRDATA[3]&&IRDATA[2]==0x01) //��Ϊ�ǻ�ʿ��λ�źţ���ʱֻ���ĸ��ֽ�
		   {
			  IRHaveRec_Flag=1;	//�Ѿ��л�ʿ��λ��־
			  IRByteEnd=1;
			  strcpy(TEMPIRDATA,IRDATA);
			  memset(IRDATA,0,6);
		   }
		  else if(IRDATASum==IRDATA[3]&&IRDATA[2]==0x02) //����ǲ鷿����
		  {
		     IRByteEnd=1;
			 strcpy(TEMPIRDATA,IRDATA);
			 memset(IRDATA,0,6); 
		  } 	
		  IRDATASum=0;
		}
		if(IRDATA[4]==0x5A&&IRByteCount==6)      //����Ǳ������������ŵ�����
		{
		   IRByteCount=0;
		   if(IRDATA[3]==0x00)                   //����ǲ����ֻ��ı������
		   {
		     IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2]+IRDATA[3]+IRDATA[4];
		     if(IRDATASum==IRDATA[5])	//����У���
		      {
			    IRDATASum=0;
			    IR_SET_BY_SERIAL_FLAG=1; //�Ѿ��յ���ȷ�����Ϣ��־
				IRByteEnd=1;
				strcpy(TEMPIRDATA,IRDATA);
				memset(IRDATA,0,6);
			  }
		     else
			 {
				IRByteEnd=0;
				IRDATASum=0;
				memset(IRDATA,0,6);
				memset(TEMPIRDATA,0,6);
			 }
		  }
		  else
		  {
		    IRDATASum=0;
			if(!NurseComeHave_Flag)
			 {
			   IRHaveRec_Flag=0;	
			   IRByteEnd=0;
			   memset(TEMPIRDATA,0,6);
			 }
			else if(NurseComeHave_Flag&&IRByteEnd==1) 
			{
			  NurseComeHave_Flag=0; 
			  strcpy(TEMPIRDATA,IRDATA);
			}
			memset(IRDATA,0,6);
		  }
		}
	  }
		/*if(IRByteCount>3)
		{
		   if(IRDATA[3]==0x00)	     //����Ǳ������������ݣ���ô���ű�����0x00��������Ϊ�ǻ�ʿ��λ��Ϣ
		   {
		      if(IRByteCount>5)		 //�����������������6���ֽ�
			   {
		          IRByteCount=0;
		          IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2]+IRDATA[3]+IRDATA[4];
		          if(IRDATASum==IRDATA[5])	//����У���
		          {
			        IRDATASum=0;
			        if(IRDATA[4]==0x5A) //���Ϊ0x5A,����Ϊ�Ǳ�������б��
			        {
			          IR_SET_BY_SERIAL_FLAG=1; //�Ѿ��յ���ȷ�����Ϣ��־
				      IRByteEnd=1;
				      strcpy(TEMPIRDATA,IRDATA);
				      memset(IRDATA,0,6);
			        }
			        else
			        {
				      IRByteEnd=0;
				      memset(IRDATA,0,6);
			        }
			     }
			     else
		         {
		           IRDATASum=0;  
			       IRByteEnd=0;
			       memset(IRDATA,0,6);
		         }  
			   }   
		  }
		  else	                    //�������Ϊ��ʿ��λ����Ϣ
		  {
		     IRByteCount=0;
		     IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2];
		     if(IRDATASum==IRDATA[3])	//����У���
		     {
			   IRDATASum=0;
			   if(IRDATA[2]==0x01) //���Ϊ0x01��˵����ʿ�Ѿ���λ
			   {
			      IRHaveRec_Flag=1;	//�Ѿ��л�ʿ��λ��־
				  IRByteEnd=1;
				  strcpy(TEMPIRDATA,IRDATA);
				  memset(IRDATA,0,6);
			   }
			   else
			   {
				  IRByteEnd=0;
				  memset(IRDATA,0,6);
			   }
			 }
		     else
		     {
		        IRDATASum=0;  
			    IRByteEnd=0;
			    memset(IRDATA,0,6);
		     }
		  }
	   }
	} */
	if(TI == 1)
	{
	    IRByteCount=0;
		TI = 0;		
	}
}
void IRRecDeal(void)
{ 	
    IRByteEnd=0;
	TimerCount=0;
	IRByteCount=0;
	if(bLanding)
	{		
		return;	//�Ǽ�״̬,������
	} 
	if(FiveSec_Flag)
	{
	   FiveSec_Flag=0;
	   TEMPIRDATA[2]=0x55;
	}
    if(IR_SET_BY_SERIAL_FLAG) //����Ǳ����������
	{
	   SET_BY_SERIAL_FLAG=1;   //��ʼ���б����ʾ���
	   stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;
	   SetLedRedState(LED_FLASH);
	   return;
	}
	switch(TEMPIRDATA[2])
	{
		case 0x01:	
		    if(!NurseComeHave_Flag)
			 {
			   NurseComeHave_Flag=1;
			   stLocalControl.stBusDealFreq.bySndSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
			   stLocalControl.stBusDealFreq.bySndRoomAddr = 255;
	           stLocalControl.stBusDealFreq.bySndBedAddr  = TEMPIRDATA[1];
			   stLocalControl.stBusDealFreq.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
			   stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			   stLocalControl.stBusDealFreq.byRecBedAddr  = 0x00; 							
			   stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_COME; //��ʿ��λ����  				
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			   MoveFjNum = TEMPIRDATA[1];
			 }
			memset(TEMPIRDATA,0,6);
			memset(IRDATA,0,6);
			break;
		case 0x02:	  //����ǲ鷿�������˳�
			 memset(TEMPIRDATA,0,6);
			 memset(IRDATA,0,6);
		     break;
		case 0x55:	
			stLocalControl.stBusDealFreq.bySndSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = 255;
	        stLocalControl.stBusDealFreq.bySndBedAddr  = MoveFjNum;
			stLocalControl.stBusDealFreq.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr  = 0x00; 
		    NurseComeHave_Flag=0;							
			stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_BACK;//��ʿ�뿪����   				
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			memset(TEMPIRDATA,0,6);
			memset(IRDATA,0,6);
		    break;
	}	 
}
/**********************************************************
*��������			:IRRecManage	
*��������        	:����������ݹ����߳�
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:byMainCmdQ
*����ģ��  			:OSQPost
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2011-2-16
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void IRRecManage(void)
{ 	
	while(TRUE)
	{		
		if(IRByteEnd)									//������յ�����
		{ 		
			OSQPost(byMainCmdQ, IR_REC);				
		}
		OSWait(K_TMO, 1);
		WDT_CONTR = 0x3d;
	}
}

	  