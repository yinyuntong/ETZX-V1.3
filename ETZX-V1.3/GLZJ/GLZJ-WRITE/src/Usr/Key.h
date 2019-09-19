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
#ifndef KEY_H
#define KEY_H

#define		KEY_DELAY			100							//设置长按键时间
//按键定义
#define COPY_KEY_VAL			0x01							//数据拷贝键
#define NO_KEY_VAL			0xff								//无键按下

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