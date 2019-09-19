/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:PCF8563.h
*文件描述    		:时钟驱动程序头文件
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
#ifndef _PCF8563_H_
#define _PCF8563_H_

#define PCF8563Addr     	0xa2         		//PCF8563的地址
#define PCF8563_SEC_ADDR    0x02
#define PCF8563_MIN_ADDR    0x03
#define PCF8563_HOUR_ADDR   0x04        		//B7=0/1--24/12h B6=0/1--AM/PM B5B4(ten hours 0 to 2) B3~B0(Unit hours) 
#define PCF8563_DAY_ADDR    0x05        		//B7B6(year 0 to 3) B5B4(ten days 0 to 3) B3~B0(Unit days)
#define PCF8563_WEEK_ADDR   0x06      			//B7_B5(Weekdays 0 to 6) B4(ten months) B3~B0(Unit months)
#define PCF8563_MONTH_ADDR  0x07
#define PCF8563_YEAR_ADDR   0x08


//时间结构体
typedef struct
{
uint8 bySecond;
uint8 byMinute;
uint8 byHour;
uint8 byDay;
uint8 byMonth;
uint8 byYear;
}STTime;


#ifndef _IN_PCF8563_
#define _IN_PCF8563_
extern STTime xdata stTime;
extern uchar xdata byMinute;

extern void P8563_Read();
extern void P8563_gettime();
extern void P8563_settime();
extern void P8563_init();
#endif


#endif
