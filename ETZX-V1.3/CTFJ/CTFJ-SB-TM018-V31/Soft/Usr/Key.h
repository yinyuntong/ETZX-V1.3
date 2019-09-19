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
*注释	     		:采用按键与灯显示控制共用一个I/O口的方式					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define		KEY_DELAY			100							//设置长按键时间

#ifndef _IN_KEY_
#define _IN_KEY_
					
void SetLedState(uint8 byState);
void KeyDownDeal(uint8 byKey);
void KeyUpDeal(uint8 byKey);
void KeyAlwaysDeal(uint8 byKey);
void KeyManager(void);

extern uint8 xdata MainMenuSelect;
#endif