/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Key.h
*�ļ�����    		:������������ͷ�ļ�
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
#define		KEY_DELAY			100
#ifndef _IN_KEY_
#define _IN_KEY_
extern  bit	bLedDealState;						   //�����״̬
extern  bit	bLedRedState;						  //�ŵƺ�ɫ��״̬
extern  bit	bLedGreenState;						 //�ŵ���ɫ��״̬
extern  bit	bLedBlueState;						 //�ŵ���ɫ��״̬
void SetLedDealState(uint8 byState);
void KeyDownDeal(uint8 byKey);
void KeyUpDeal(uint8 byKey);
void KeyAlwaysDeal(uint8 byKey);
uint8 KeyScan(void);

void SetMDLedState(uint8 byState);
#endif
