/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.h
*�ļ�����    		:������������ͷ�ļ�
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
#define		KEY_DELAY			100							//���ó�����ʱ��

#ifndef _IN_KEY_
#define _IN_KEY_
extern  bit	bLedDealState;					
extern  bit	bLedSetState;					
void SetLedDealState(uint8 byState);
void KeyDownDeal(uint8 byKey);
void KeyUpDeal(uint8 byKey);
void KeyAlwaysDeal(uint8 byKey);
void KeyManager(void);
void OXManager(void);
#endif