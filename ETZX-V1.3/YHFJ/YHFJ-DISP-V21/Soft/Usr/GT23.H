/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:HZ.c
*文件描述    		:汉字识别、汉字字库头文件
*创建人     		:尹运同
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		     :
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#ifndef _GT23_H_
#define _GT23_H_

//字库各字体偏移地址宏定义
#define  C16BaseAdd  0x2c9d0;	//15*16汉字
#define  C24BaseAdd  0x68190;  //24*24 汉字
#define  C32BaseAdd  0xEDF00;  //32*32 汉字
	
#define  A8BaseAdd   0x1DD780;  //8*16 ASCII
#define  A12BaseAdd  0x1DFF00;  //12*24 ASCII
#define  A16BaseAdd  0x1E5A50;  //16*32 ASCII
	//--------------------------------------



#ifndef _IN_GT23_
extern uint8  xdata  GT23_Rec_Data[];
extern uint8  xdata  GT23_Rec_Length;
extern uint16   xdata  GT23_16LAddress;
extern uint16   xdata  GT23_16HAddress;
extern uint32  xdata  GT23_32Address; 

extern void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size);
extern void Get_GT23_Data1(uint8 *word,uint8  word_size);
extern uint8 Rec_GT23_Data(void);

#endif


#endif

