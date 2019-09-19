/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.h
*文件描述    		:按键驱动程序头文件
*创建人     		:陈卫国
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
#define		KEY_DELAY			100
#ifndef _IN_KEY_
#define _IN_KEY_
extern  bit	bLedDealState;						   //处理灯状态
extern  bit	bLedRedState;						  //门灯红色灯状态
extern  bit	bLedGreenState;						 //门灯绿色灯状态
extern  bit	bLedBlueState;						 //门灯蓝色灯状态
void SetLedDealState(uint8 byState);
void KeyDownDeal(uint8 byKey);
void KeyUpDeal(uint8 byKey);
void KeyAlwaysDeal(uint8 byKey);
uint8 KeyScan(void);

void SetMDLedState(uint8 byState);
#endif
