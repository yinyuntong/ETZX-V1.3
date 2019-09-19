/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.h
*文件描述    		:医护分机键盘处理程序头文件
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

#ifndef _KEY_H_
#define _KEY_H_

#define KEY_0   0
#define KEY_1   1
#define KEY_2   2
#define KEY_3   3
#define KEY_4   4
#define KEY_5   5
#define KEY_6   6
#define KEY_7   7
#define KEY_8   8
#define KEY_9   9
#define KEY_CLEAR   10
#define KEY_ENTER   11
#define KEY_VOICEINC   12
#define KEY_VOICEDEC   13
#define KEY_ADDBED     14
#define KEY_SEC   	  15
#define KEY_ROOM		  16
#define KEY_MS		  17
#define KEY_MR    	18
#define KEY_SEARCH   19
#define KEY_CALCULATE  20
#define KEY_MODE		  21
#define KEY_BROAD	22
#define KEY_MUSIC	23
#define KEY_RESERVE 24

#ifndef _IN_KEY_
#define _IN_KEY_
void KeyDownDeal(uint8 byKey);
void KeyUpDeal(uint8 byKey);
void KeyManager(void);
#endif



#endif
