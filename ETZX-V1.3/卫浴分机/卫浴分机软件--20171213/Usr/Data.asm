/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:Data.asm
*�ļ�����    		:Ԥ�ȴ洢��Flash�е�������
*������     		:����ͬ
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:���ļ��е�������STEepromCfgData�в���
					 һһ��Ӧ;����������ֻ��ĳ�ʼ����ַΪ1��1��90��								
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/

	name Data_Asm

	cseg at 0x2800
SelfSecAddr:
	db		0x01				//0x2600	:-0
SelfRoomAddr:
	db		0x01				//0x2601	:-1
SelfBedAddr:
	db		0xF0				//0x2602	:-2
cFlag:
	db		0x01				//0x2803	:-3
cPrio:
	db		0xC0				//0x2804	:-4	
cReserve1:
	db		0xFF				//0x2805	:-5
cReserve2:
	db		0xFF				//0x2806	:-6
cReserve3:
	db		0xFF				//0x2807	:-7
cSerialNum1:
	db		0xFF				//0x2808	:-8
cSerialNum2:
	db		0xFF				//0x2809	:-9
cSerialNum3:
	db		0xFF				//0x280a	:-a
cVersionHi:
	db		0x10				//0x280b	:-b
cVersionLo:
	db		0x0B				//0x280c	:-c   			

	end