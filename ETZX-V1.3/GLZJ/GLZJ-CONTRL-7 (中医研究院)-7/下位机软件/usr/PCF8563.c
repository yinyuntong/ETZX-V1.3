/*
************************Copyright(c)************************
*	  			   ����һ�ص��ӹɷ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:PCF8563.c
*�ļ�����    		:ʱ����������
*������     		:����ͬ
*��������   		:2009-4-6
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
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
�ڲ�����,����ʱ�䵽�ڲ�������
********************************************/
void P8563_Read()
{   
    ReadDataStr(PCF8563_SEC_ADDR,0x04,(uchar *)&stTime);	//��ȡ��,��,Сʱ,��
    ReadDataStr(PCF8563_MONTH_ADDR,0x02,&(stTime.byMonth));	//��ȡ����
    
    stTime.bySecond =stTime.bySecond&0x7f; /*��*///BCD
    stTime.byMinute =stTime.byMinute&0x7f; /*��*///BCD
    stTime.byHour   =stTime.byHour&0x3f; /*Сʱ*///BCD
    stTime.byDay	=stTime.byDay&0x3f; /*��*///BCD
    stTime.byMonth	=stTime.byMonth&0x1f; /*����/��*///BCD   bit7=0:��ʾ20
	//stTime.byYear  ռ��8λ
  
}
/********************************************
����ʱ�䵽�ڲ�������----�ⲿ���� 
********************************************/
void P8563_gettime()
{
    P8563_Read();
//    if(stTime.bySecond==0)
//      P8563_Read(); /*���Ϊ��=0��Ϊ��ֹʱ��仯���ٶ�һ��*/
}
/********************************************
дʱ���޸�ֵ
********************************************/
void P8563_settime()
{
	 writeDataStr(PCF8563_SEC_ADDR,0x04,(uchar *)&stTime);//д��,��,Сʱ,��
     writeDataStr(PCF8563_MONTH_ADDR,0x02,&(stTime.byMonth));	//д����
}
/********************************************
P8563�ĳ�ʼ��-----�ⲿ����
********************************************/
void P8563_init()
{
    writeData(0x0,0x00);
    writeData(0xa,0x8); /*8:00����*/
    writeData(0x1,0x12); /*������Ч*/
    writeData(0xd,0xf0);
}
