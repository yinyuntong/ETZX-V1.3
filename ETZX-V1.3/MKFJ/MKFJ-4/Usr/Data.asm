/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Data.asm
*文件描述    		:预先存储在Flash中的配置字
*创建人     		:陈卫国
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:					
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
	db		0x01				//0x2800	:-0
SelfRoomAddr:
	db		0x01				//0x2801	:-1
SelfBedAddr:
	db		0x00				//0x2802	:-2
cEnable1:
	db		0xFF      			//0x2803	:-3	//默认是许可报号	
cEnable2:
	db		0x3F				//0x2804	:-4	
cPrio:
	db		0x60				//0x2805	:-5
cRingTime:
	db		0x0E				//0x2806	:-6	
cTalkTime:
	db		0x40				//0x2807	:-7	
cBroadTime:
	db		0x80				//0x2808	:-8	
cListenDelay:
	db		0x03				//0x2809	:-9	
cVolumStep:
	db		0x04				//0x280a	:-a	
cRingVol:
	db		0xD0				//0x280b	:-b	
cCH0TalkedVol:
	db		0xD0				//0x280c	:-c	
cCH1TalkedVol:
	db		0xD0				//0x280d	:-d
cBroadVol:
	db		0xD0				//0x280e	:-e
cNoteVol:
	db		0xD0				//0x280f	:-f	
cMusicVol:
	db		0xD0				//0x2810	:-10
cCH0TalkVol:
	db		0xD0				//0x2811	:-11
cCH1TalkVol:
	db		0xD0				//0x2812	:-12
cSelfRingVol:
	db		0xD0				//0x2813	:-13
cMaxVol:
	db		0xF9				//0x2814	:-14
cMinVol:
	db		0x0A				//0x2815	:-15
cReserve1:
	db		0xFF				//0x2816	:-16
cReserve2:
	db		0xFF				//0x2817	:-17
cReserve3:
	db		0xFF				//0x2818	:-18
cSerialNum1:
	db		0xFF				//0x2819	:-19
cSerialNum2:
	db		0xFF				//0x281a	:-1a
cSerialNum3:
	db		0xFF				//0x281b	:-1b
cVersionHi:
	db		0x10				//0x281c	:-1c
cVersionLo:
	db		0x0B				//0x281d	:-1d
	end	