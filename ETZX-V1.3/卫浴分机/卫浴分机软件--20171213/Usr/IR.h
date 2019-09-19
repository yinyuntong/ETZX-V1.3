/*
*-----------------------ÎÄ¼şĞÅÏ¢---------------------------
*ÎÄ¼şÃû³Æ     		:IR.h
*ÎÄ¼şÃèÊö    		:ºìÍâ½ÓÊÕ³ÌĞòÍ·ÎÄ¼ş
*´´½¨ÈË     		:ĞÜ¼áÇ¿
*´´½¨ÈÕÆÚ   		:2011-2-16
*°æ±¾ºÅ       		:
*×¢ÊÍ	     		:
*----------------------------------------------------------
*ĞŞ¸ÄÈË  			:
*ĞŞ¸ÄÈÕÆÚ  			:
*°æ±¾ºÅ        		:
*×¢ÊÍ	     		:
***********************************************************
*/
#ifndef _IR_H_
#define _IR_H_

#define IrDA_CMD_NURSE_IN   0x01							//ºìÍâ½Ó¿Ú»¤Ê¿µ½Î»ÃüÁî
#define IrDA_CMD_INSPECTION	0x02							//ºìÍâ½Ó¿Ú²é·¿ÃüÁî


#define NURSE_IN_TIME	350								//»¤Ê¿µ½Î»ĞÅºÅÓĞĞ§Ê±¼ä
#define MOVE_FJ				251								//ÒÆ¶¯·Ö»ú



#ifndef _IN_IR_

extern uint8 xdata MoveFjNum;		//ÒÆ¶¯·Ö»úºÅÂë

extern uint8 xdata UartRecData;
extern uint8 xdata IrDataPosit;

extern uint16  uiNurseInTime;						//¼ì²â»¤Ê¿µ½Î»¶¨Ê±Ê±¼ä
extern uint8 	byIrDataIntervalTimer;				//ºìÍâÊı¾İ½ÓÊÕ¹ı³ÌÖĞ×Ö½ÚÖ®¼ä¼ä¸ôÊ±¼ä
extern uint8 	byNumSetTime;						//±àºÅÊ±¼ä

extern uint8 xdata byIrDARecBuff[], byIrDADealBuff[];		//ºìÍâ½ÓÊÕÇø¡¢´¦Àí»º³åÇø

extern bit bFourByteRec;					//´®¿Ú½ÓÊÕµ½4×Ö½ÚÃüÁî
extern bit bSixByteRec;					//´®¿Ú½ÓÊÕµ½6×Ö½ÚÃüÁ


extern bit	bSetKeyDown ;				//ÉèÖÃ¼ü°´ÏÂ×´Ì¬
extern bit	bNurseIn;	 			//»¤Ê¿µ½Î»±êÖ¾


extern void IRInit(void);
extern void IrDATreat(void);	//Êı¾İ´¦Àíº¯Êı
extern void IrDANumberSet(void);	//Êı¾İ´¦Àíº¯Êı

#endif



#endif
