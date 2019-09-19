/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:IAP.h
*�ļ�����    		:дFlash����ͷ�ļ�
*������     		:����ͬ
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
#define		IAP_WAIT_TIME		0x02						//IAP�ȴ�ʱ�� 20M����
//#define		IAP_WAIT_TIME		0x03						//IAP�ȴ�ʱ��   12M����

#define 	ISP_DATA	IAP_DATA	
#define 	ISP_CONTR	IAP_CONTR	
#define 	ISP_CMD		IAP_CMD		
#define 	ISP_ADDRH	IAP_ADDRH	
#define 	ISP_ADDRL	IAP_ADDRL	
#define 	ISP_TRIG	IAP_TRIG	


#ifndef _IN_IAP_
#define _IN_IAP_
void EnableIAP(uint8 byCmd);
void DisableIAP(void);
uint8 IapReadByte(uint16 uiAddr);
void IapWriteByte(uint16 uiAddr, uint8 byData);
void IapErase(uint16 uiAddr);
#endif