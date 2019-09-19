/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Data.asm
*文件描述    		:预先存储在Flash中的配置字
*创建人     		:尹运同
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:该文件中的内容与STEepromCfgData中参数
					 一一对应;所有卫生间分机的初始化地址为1区1房90床								
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
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