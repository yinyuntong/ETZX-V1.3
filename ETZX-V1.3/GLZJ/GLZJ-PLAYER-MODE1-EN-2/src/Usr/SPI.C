/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SPI.c
*文件描述    		:SPI驱动程序
*创建人     		:尹运同
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     							
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_SPI_
#include "config.h"

			
 
/**********************************************************
*函数名称			:SpiInit
*函数描述        	:SPI接口初始化
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SpiInit(void)
{ 	
	//工作模式设置,主动模式,低位在前,Fosc/4,起始下降沿采样	
	//SPCTL	=	(SSIG|SPEN|DORD|MSTR|CPOL|CPHA|SPSPEEDHH);
	//工作模式设置,主动模式,高位在前,Fosc/4,起始下降沿采样
	//SPCTL	=	(SSIG|SPEN|MSTR|CPOL|CPHA|SPSPEEDHH);

	//工作模式设置,主动模式,高位在前,Fosc/4,起始下降沿采样
	SPCTL	=	(SSIG|SPEN|MSTR/*|CPOL|CPHA*/|SPSPEEDHH);
	
	SPSTAT |= (SPIF|WCOL);
	EADC_SPI =0;
	AUXR &= ~ESPI;	//禁止SPI中断
	MISO =1;//输入
}




void Vs1003SpiInit(void)
{ 	
	//工作模式设置,主动模式,低位在前,Fosc/4,起始下降沿采样	
	//SPCTL	=	(SSIG|SPEN|DORD|MSTR|CPOL|CPHA|SPSPEEDHH);
	//工作模式设置,主动模式,高位在前,Fosc/4,起始下降沿采样
	//SPCTL	=	(SSIG|SPEN|MSTR|CPOL|CPHA|SPSPEEDHH);

	//工作模式设置,主动模式,高位在前,Fosc/4,起始下降沿采样
	SPCTL	=	(SSIG|SPEN|MSTR/*|CPOL|CPHA*/|SPSPEEDH);
	
	SPSTAT |= (SPIF|WCOL);
	EADC_SPI =0;
	AUXR &= ~ESPI;	//禁止SPI中断
	MISO =1;//输入
}

/**********************************************************
*函数名称			:SpiInt
*函数描述        	:SPI中断服务程序
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/

void SpiInt(void) interrupt ADC_SPI_INTNO		//5号中断
{
	SPSTAT |= SPIF;		//清标志
	AUXR &= ~ESPI;		//禁止中断
}


/**********************************************************
*函数名称			:SPI_WriteByte	
*函数描述        	:写一个字节
*输入参数   		:dat:要写入的数据
*返回值				:   	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 SPI_WriteByte(unsigned char dat)
{

	SPDAT=dat;					 	
   	while((SPSTAT & SPIF) == 0x00);						//等待发送完成 
	SPSTAT	|=	SPIF;//SPI传输完成标志,当SPI中断使能时会触发SPI中断请求,需要用软件向此位写"1"来清0
	return(SPDAT);	
}



/** Read and return an octet from SPI port*/
unsigned char SPIGetChar()
{
	SPI_WriteByte(0xff); /* send 0xff */
	return SPI_RESULT_BYTE; /* Return the received byte */
}
  

/** Send nClocks x 8 clock transitions with MOSI=1 (0xff) */
void SPI8Clocks(unsigned char nClocks)
{
  	while (nClocks--)
  	{
		SPI_WriteByte(0xff);
  	}
}

