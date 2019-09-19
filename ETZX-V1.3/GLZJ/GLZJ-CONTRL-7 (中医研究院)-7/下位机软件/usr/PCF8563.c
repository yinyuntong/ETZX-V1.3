/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:PCF8563.c
*文件描述    		:时钟驱动程序
*创建人     		:尹运同
*创建日期   		:2009-4-6
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_PCF8563_
#include "config.h"

STTime xdata stTime;
uchar xdata byMinute;

void P8563_Read();
void P8563_gettime();
void P8563_settime();
void P8563_init();
void P8563Manager(void);

extern STLocalControl  xdata 			stLocalControl;

/********************************************
内部函数,读入时间到内部缓冲区
********************************************/
void P8563_Read()
{   
    ReadDataStr(PCF8563_SEC_ADDR,0x04,(uchar *)&stTime);	//读取秒,分,小时,天
    ReadDataStr(PCF8563_MONTH_ADDR,0x02,&(stTime.byMonth));	//读取月年
    
    stTime.bySecond =stTime.bySecond&0x7f; /*秒*///BCD
    stTime.byMinute =stTime.byMinute&0x7f; /*分*///BCD
    stTime.byHour   =stTime.byHour&0x3f; /*小时*///BCD
    stTime.byDay	=stTime.byDay&0x3f; /*天*///BCD
    stTime.byMonth	=stTime.byMonth&0x1f; /*世纪/月*///BCD   bit7=0:表示20
	//stTime.byYear  占用8位
  
}
/********************************************
读入时间到内部缓冲区----外部调用 
********************************************/
void P8563_gettime()
{
    P8563_Read();
//    if(stTime.bySecond==0)
//      P8563_Read(); /*如果为秒=0，为防止时间变化，再读一次*/
}
/********************************************
写时间修改值
********************************************/
void P8563_settime()
{
	 writeDataStr(PCF8563_SEC_ADDR,0x04,(uchar *)&stTime);//写秒,分,小时,天
     writeDataStr(PCF8563_MONTH_ADDR,0x02,&(stTime.byMonth));	//写月年
}
/********************************************
P8563的初始化-----外部调用
********************************************/
void P8563_init()
{
    writeData(0x0,0x00);
    writeData(0xa,0x8); /*8:00报警*/
    writeData(0x1,0x12); /*报警有效*/
    writeData(0xd,0xf0);
}
