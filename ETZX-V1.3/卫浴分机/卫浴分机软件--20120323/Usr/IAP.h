/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:IAP.h
*�ļ�����    		:дFlash����ͷ�ļ�
*������     		:������
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:����STCϵ��CPU,������CPU���ڲ�Flash��
				     EEPromʹ��,������������Ϊ��λ,�ڽ���
				     IAP����������,CPU�����趨��ʱ�����,
				     ��ʱCPU���ܹ������κι���
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define		IAP_READ			0x01						//IAP������
#define		IAP_WRITE			0x02						//IAPд����
#define		IAP_ERASE			0x03						//IAP��������
#define		IAP_WAIT_TIME		0x02						//IAP�ȴ�ʱ��

#ifndef _IN_IAP_
#define _IN_IAP_
void EnableIAP(uint8 byCmd);
void DisableIAP(void);
uint8 IapReadByte(uint16 uiAddr);
void IapWriteByte(uint16 uiAddr, uint8 byData);
void IapErase(uint16 uiAddr);
#endif