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
/*#define  C12BaseAdd  0x00000;	//11*12汉字
#define  C16BaseAdd  0x2c9d0;	//15*16汉字
#define  C24BaseAdd  0x68190;  //24*24 汉字
#define  C32BaseAdd  0xEDF00;  //32*32 汉字

#define  A6x12BaseAdd 0x1DBE00	//6*12 ASCII	
#define  A8x16BaseAdd   0x1DD780;  //8*16 ASCII
#define  A12x24BaseAdd  0x1DFF00;  //12*24 ASCII
#define  A16x32BaseAdd  0x1E5A50;  //16*32 ASCII
#define  A24ArialBaseAdd 0x1E22D0;	//Arical--24点阵不等宽
#define  A32ArialBaseAdd 0x1E99D0;	//Arical--32点阵不等宽


#define  A32RomanBaseAdd 0x1ECA90;	//Roman--32点阵不等宽*/
//--------------------------------------

//字库各字体偏移地址宏定义
	

#define  A8BaseAdd    0x00000000  //16*8ASCI
#define  A12BaseAdd   0x000005f0  //16*8ASCI
#define  A16BaseAdd   0x000017c0  //32*16ASCI

#define  D48BaseAdd   0x00002f80   //96*48数字

#define  C16BaseAdd  0x00004600;  //16*16 汉字
#define  C32BaseAdd  0x000c0640;  //32*32 汉字
//--------------------------------------

#define A6x12Font   0
#define A8x16Font	1
#define A12x24Font	2
#define A16x32Font	3

#define A128x80Aria	4		//微软黑雅字体



#ifndef _IN_GT23_
extern uint8  xdata  GT23_Rec_Data[];
extern uint8  xdata  GT23_Rec_Length;
extern uint16   xdata  GT23_16LAddress;
extern uint16   xdata  GT23_16HAddress;
extern uint32  xdata  GT23_32Address; 


extern void Get_GT23_Data(uint8 *word,uint8 word_size);
extern uint8 Rec_GT23_Data(void);

extern uint8  code  Digit128x64[];

#endif


#endif

