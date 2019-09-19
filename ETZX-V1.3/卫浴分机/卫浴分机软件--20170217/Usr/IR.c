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

uint8 xdata MoveFjNum=0;		//移动分机号码

extern uint16 xdata TimerCount;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;
extern bit SET_BY_SERIAL_FLAG;
/**********************************************************
*函数名称			:IRInit	
*函数描述        	:红外接收初始化
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-16
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void IRInit(void)
{
	SCON = 0x50;
	PCON &= 0x7F;
	TH1 = 0xD8;
	TL1 = 0xD8;		//波特率设置为1200		
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
*函数名称			:comm	
*函数描述        	:串口中断，用来红外解码
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-16
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
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
        if(IRByteCount==4) //此处先判断是编号数据还是护士到位信息
		{
		   IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2];
		   if(IRDATASum==IRDATA[3]&&IRDATA[2]==0x01) //认为是护士到位信号，此时只有四个字节
		   {
			  IRHaveRec_Flag=1;	//已经有护士到位标志
			  IRByteEnd=1;
			  strcpy(TEMPIRDATA,IRDATA);
			  memset(IRDATA,0,6);
		   }
		  else if(IRDATASum==IRDATA[3]&&IRDATA[2]==0x02) //如果是查房命令
		  {
		     IRByteEnd=1;
			 strcpy(TEMPIRDATA,IRDATA);
			 memset(IRDATA,0,6); 
		  } 	
		  IRDATASum=0;
		}
		if(IRDATA[4]==0x5A&&IRByteCount==6)      //如果是编号器发过来编号的数据
		{
		   IRByteCount=0;
		   if(IRDATA[3]==0x00)                   //如果是厕所分机的编号数据
		   {
		     IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2]+IRDATA[3]+IRDATA[4];
		     if(IRDATASum==IRDATA[5])	//计算校验和
		      {
			    IRDATASum=0;
			    IR_SET_BY_SERIAL_FLAG=1; //已经收到正确编号信息标志
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
		   if(IRDATA[3]==0x00)	     //如果是编号器输入的数据，那么床号必须是0x00，否则认为是护士到位信息
		   {
		      if(IRByteCount>5)		 //编号器过来的数据是6个字节
			   {
		          IRByteCount=0;
		          IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2]+IRDATA[3]+IRDATA[4];
		          if(IRDATASum==IRDATA[5])	//计算校验和
		          {
			        IRDATASum=0;
			        if(IRDATA[4]==0x5A) //如果为0x5A,则认为是编号器进行编号
			        {
			          IR_SET_BY_SERIAL_FLAG=1; //已经收到正确编号信息标志
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
		  else	                    //其余的则为护士到位的信息
		  {
		     IRByteCount=0;
		     IRDATASum=IRDATA[0]+IRDATA[1]+IRDATA[2];
		     if(IRDATASum==IRDATA[3])	//计算校验和
		     {
			   IRDATASum=0;
			   if(IRDATA[2]==0x01) //如果为0x01，说明护士已经到位
			   {
			      IRHaveRec_Flag=1;	//已经有护士到位标志
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
		return;	//登记状态,不处理
	} 
	if(FiveSec_Flag)
	{
	   FiveSec_Flag=0;
	   TEMPIRDATA[2]=0x55;
	}
    if(IR_SET_BY_SERIAL_FLAG) //如果是编号器的数据
	{
	   SET_BY_SERIAL_FLAG=1;   //开始进行编号提示编号
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
			   stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_COME; //护士到位命令  				
			   Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

			   MoveFjNum = TEMPIRDATA[1];
			 }
			memset(TEMPIRDATA,0,6);
			memset(IRDATA,0,6);
			break;
		case 0x02:	  //如果是查房命令则退出
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
			stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_BACK;//护士离开命令   				
			Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
			memset(TEMPIRDATA,0,6);
			memset(IRDATA,0,6);
		    break;
	}	 
}
/**********************************************************
*函数名称			:IRRecManage	
*函数描述        	:红外接收数据管理线程
*输入参数   		:
*返回值				:
*全局变量			:byMainCmdQ
*调用模块  			:OSQPost
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-2-16
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void IRRecManage(void)
{ 	
	while(TRUE)
	{		
		if(IRByteEnd)									//红外接收到数据
		{ 		
			OSQPost(byMainCmdQ, IR_REC);				
		}
		OSWait(K_TMO, 1);
		WDT_CONTR = 0x3d;
	}
}

	  