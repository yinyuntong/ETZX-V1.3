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

void MCUMasterSPI(void);
uint8 SPI_WriteByte(uint8 dat);
void EPH1660MasterSPI(void);


 
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
void MCUMasterSPI(void)
{ 	

	//全部禁止
	SST25VF_CS = 1;
	GT23L_CS = 1;
	ADS7843_CS = 1;
	EPH1660_CS=0;	//禁止EPH1660

	SPCTL  &= ~SPEN;	//关SPI功能
	//SCLK   P1.7改为准双向口
	P1M0 &= 0x7f;
	P1M1 &= 0x7f;
	SCLK=1;
	while(!SCLK);	//等待SCLK恢复为高
	
	SPCTL	=	(SSIG|SPEN|MSTR|CPOL|CPHA|SPSPEEDLL);
	SPSTAT |= (SPIF|WCOL);
	IE2 &= ~ESPI;	//关闭SPI中断
		
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

/*void SpiInt(void) interrupt SPI_INTNO		//9号中断
{
	SPSTAT |= SPIF;		//清标志
	IE2 &= ~ESPI;
}*/

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
uint8 SPI_WriteByte(uint8 dat)
{
	SPDAT=dat;					 	
   	while((SPSTAT & SPIF) == 0x00);						//等待发送完成 
	SPSTAT	|=	SPIF;//SPI传输完成标志,当SPI中断使能时会触发SPI中断请求,需要用软件向此位写"1"来清0
	return(SPDAT);
}



void EPH1660MasterSPI(void)	//EPH1660作主机
{
	//全部禁止
	EPH1660_CS = 0;	
	SST25VF_CS = 1;
	GT23L_CS = 1;
	ADS7843_CS = 1;	

	//WKICOM=1; //唤醒EPH1660   //推挽输出
	WKICOM=1;
		
	//SPI_CMD高阻输入(悬浮)
	SPI_CMD=1;		//悬浮状态也可用高电平输入


	SPCTL &= ~SPEN;
	
	//以下单片机为从机模式,EPH1660为主机模式
	SPCTL = (SSIG|SPEN/*|MSTR|CPOL*/|CPHA|SPSPEEDHH);//;
	MOSI=1;	   //作输入
	SCLK =1;	//作输入
	SPSTAT |= (SPIF|WCOL);	//清中断		
	IE2 |= ESPI;			//打开SPI中断

	IP2	|= PSPI;
	IP2H|= PSPIH;	//SPI优先级最高
	EPH1660_CS = 1;
}




void SPIInt(void) interrupt SPI_INTNO		//9号中断
{

	EA=0;
	SPSTAT |= (SPIF|WCOL);	//清中断

/*

	if(MCUSndFlag==0)	//接收过程中
	{
		Rec_Data[InNum++]=SPDAT;
		if(InNum==MAXLEN) InNum=0;

	}
	else
	{
		if(MCUSndDataLength==0) //有错误产生
		{
			goto MCUSndCmdFinish;		
		}
		else
		{
			MCUSndDataLength--;
			MCUSndDataPosition++;
			if(MCUSndDataLength)	//发送还未完成
			{
				SPDAT=MCUSndDataBuff[MCUSndDataPosition];
			}
			else		//整帧命令发送完成
			{
MCUSndCmdFinish:
				MCUSndCmdFinishFlag=1;
				MCUSndFlag=0;
				WKICOM=1;
			}
		}
	}
*/
	EA=1;

}









