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
*ע��	     		:
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#ifndef _KEY_H_
#define _KEY_H_

#ifndef _IN_KEY_
#define _IN_KEY_

extern bit		bLedRedState;
extern void SetLedRedState(uint8 byState);
extern void KeyDownDeal(uint8 byKey); 
extern void KeyManager(void);
#endif

#endif
