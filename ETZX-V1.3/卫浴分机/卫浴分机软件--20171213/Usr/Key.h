/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.h
*文件描述    		:按键驱动程序头文件
*创建人     		:尹运同
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
