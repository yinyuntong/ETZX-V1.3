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
#ifndef KEY_H
#define KEY_H

#define		KEY_DELAY			100							//���ó�����ʱ��
//��������
#define COPY_KEY_VAL			0x01							//���ݿ�����
#define NO_KEY_VAL			0xff								//�޼�����

#ifndef _IN_KEY_
#define _IN_KEY_
extern bit bPrgState;
					
extern void SetLedState(uint8 byState);
extern void KeyDownDeal(uint8 byKey);
extern void KeyUpDeal(uint8 byKey);
extern void KeyAlwaysDeal(uint8 byKey);
extern void KeyManager(void);

#endif

#endif