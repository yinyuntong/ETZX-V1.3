#define _IN_IR_
#include "config.h"
#include "string.h"


uint8 xdata MoveFjNum=0;		//ÒÆ¶¯·Ö»úºÅÂë


uint8 xdata UartRecData;
uint8 xdata IrDataPosit=0;

uint16  uiNurseInTime;						//¼ì²â»¤Ê¿µ½Î»¶¨Ê±Ê±¼ä
uint8 	byIrDataIntervalTimer;				//ºìÍâÊý¾Ý½ÓÊÕ¹ý³ÌÖÐ×Ö½ÚÖ®¼ä¼ä¸ôÊ±¼ä
uint8 	byNumSetTime;						//±àºÅÊ±¼ä

uint8 xdata byIrDARecBuff[8], byIrDADealBuff[6];		//ºìÍâ½ÓÊÕÇø¡¢´¦Àí»º³åÇø

bit bFourByteRec=0;					//´®¿Ú½ÓÊÕµ½4×Ö½ÚÃüÁî
bit bSixByteRec=0;					//´®¿Ú½ÓÊÕµ½6×Ö½ÚÃüÁ


bit	bSetKeyDown =0;				//ÉèÖÃ¼ü°´ÏÂ×´Ì¬
bit	bNurseIn	=0;	 			//»¤Ê¿µ½Î»±êÖ¾

/**********************************************************
*º¯ÊýÃû³Æ			:IRInit	
*º¯ÊýÃèÊö        	:ºìÍâ½ÓÊÕ³õÊ¼»¯
*ÊäÈë²ÎÊý   		:
*·µ»ØÖµ				: 	
*È«¾Ö±äÁ¿			:
*µ÷ÓÃÄ£¿é  			:
***********************************************************
*´´½¨ÈË	      		:ÐÜ¼áÇ¿
*´´½¨ÈÕÆÚ	 		:2011-2-16
***********************************************************
*ÐÞ¸ÄÈË	  			:
*ÐÞ¸ÄÈÕÆÚ    		:
*×¢ÊÍ		      	:
**********************************************************/
void IRInit(void)
{
	RXD=1;
	TXD=1;

	SCON = 0x50;	//´®¿ÚÄ£Ê½1
	PCON &= 0x7F;	//´®¿Ú²¨ÌØÂÊËÙÎ»0:±£³ÖÔ­Ñù
	TMOD &= 0X0F;		//TO·½Ê½²»±ä
	TMOD |= 0X20;		//T1 8Î»×Ô¶¯ÖØÔØ	
	TH1 = 0xD8;
	TL1 = 0xD8;		//²¨ÌØÂÊÉèÖÃÎª1200	

	PCON &= (~SMOD);   // ²¨ÌØÂÊ±¶ÔöÑ¡Ôñ		smod=0
	AUXR &= (~T1x12);		//¶¨Ê±Æ÷1Ê±ÖÓ¿ØÖÆ,0:Fosc/12(´«Í³µÄ12·ÖÆµ) 	
	
	TF1=0;
	TR1 = 1;

	
	RI=0;
	TI=0;
	REN=1;
	ES = 1;
	
}
/**********************************************************
*º¯ÊýÃû³Æ			:comm	
*º¯ÊýÃèÊö        	:´®¿ÚÖÐ¶Ï£¬ÓÃÀ´ºìÍâ½âÂë
*ÊäÈë²ÎÊý   		:
*·µ»ØÖµ				: 	
*È«¾Ö±äÁ¿			:
*µ÷ÓÃÄ£¿é  			:
***********************************************************
*´´½¨ÈË	      		:ÐÜ¼áÇ¿
*´´½¨ÈÕÆÚ	 		:2011-2-16
***********************************************************
*ÐÞ¸ÄÈË	  			:
*ÐÞ¸ÄÈÕÆÚ    		:
*×¢ÊÍ		      	:
**********************************************************/
#pragma disable
void comm(void) interrupt 4
{	 
	if(TI==1)
		TI=0;
	if(RI==1)
	{
		RI=0;
		UartRecData = ~SBUF;	//°´Î»È¡·´
		byIrDataIntervalTimer=5;		//Ã¿×Ö½Ú¼ä¸ô²»³¬¹ý100ms
		if(IrDataPosit==0)
		{	
			if(UartRecData==0x05)	//ÆðÊ¼Âë
				byIrDARecBuff[IrDataPosit++]=	UartRecData;
		}
		
		else 
		{
			byIrDARecBuff[IrDataPosit++]=	UartRecData;
			if(IrDataPosit >=7)   IrDataPosit =0;

		}	
	}
}



void IrDATreat(void)	//Êý¾Ý´¦Àíº¯Êý
{

	OS_ENTER_CRITICAL();
	memcpy(byIrDADealBuff,byIrDARecBuff,4);				//°ÑÊý¾Ý´Ó½ÓÊÕÇø¸´ÖÆµ½»º³åÇø
	memset(byIrDARecBuff,0x00,4);						//½«½ÓÊÕ»º³åÇøÇå0
	OS_EXIT_CRITICAL();
	if(byIrDADealBuff[3]==(uint8)(byIrDADealBuff[0]+byIrDADealBuff[1]+byIrDADealBuff[2]))
	{//Ð£ÑéºÍÕýÈ·
		switch(byIrDADealBuff[2])
		{
		case IrDA_CMD_NURSE_IN:
			uiNurseInTime=NURSE_IN_TIME;				//Èç¹û5ÃëÄÚÎ´ÊÕµ½»¤Ê¿µ½Î»ÃüÁî£¬¼´ÈÏÎª»¤Ê¿Àë¿ª
			if(bNurseIn==0)
			{
				MoveFjNum = byIrDADealBuff[1];//ÒÆ¶¯·Ö»úºÅ
				
				stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.bySndRoomAddr= MOVE_FJ;
				stLocalControl.stBusDealFreq.bySndBedAddr= byIrDADealBuff[1];//ÒÆ¶¯·Ö»úºÅ
				stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
				stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
				stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;		
				stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_IN;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				MoveFjNum= byIrDADealBuff[1];
			}
			bNurseIn=1;	
			break;


			
		case IrDA_CMD_INSPECTION:			//»¤Ê¿²é·¿ÃüÁî	
			break;
			
		}
	}
}



void IrDANumberSet(void)	//Êý¾Ý´¦Àíº¯Êý
{

	OS_ENTER_CRITICAL();
	memcpy(byIrDADealBuff,byIrDARecBuff,6);				//°ÑÊý¾Ý´Ó½ÓÊÕÇø¸´ÖÆµ½»º³åÇø
	memset(byIrDARecBuff,0x00,6);						//½«½ÓÊÕ»º³åÇøÇå0
	OS_EXIT_CRITICAL();

	if(byIrDADealBuff[5] == byIrDADealBuff[0]+byIrDADealBuff[1]+byIrDADealBuff[2]+byIrDADealBuff[3]+byIrDADealBuff[4])
	{//Ð£ÑéºÍÕýÈ·
		if(byIrDADealBuff[3] ==0)
		{//ÊÇÎÀÉú¼ä·Ö»ú±àºÅ
			switch(byIrDADealBuff[4])
			{
				case 0x5a:
					bNumSeting=1;
					byNumSetTime = 250;
					stLocalControl.byLedTime = stLocalControl.byLedDelay = 50;
					SetLedRedState(LED_FLASH);
					bySec =  byIrDADealBuff[1];	//ÇøºÅ
					byRoom = byIrDADealBuff[2];//·¿ºÅ
					break;

					
				default:
					break;
			}
		}
	}
}
