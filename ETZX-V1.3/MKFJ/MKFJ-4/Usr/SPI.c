/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:SPI.c
*文件描述    		:SPI总线驱动程序
*创建人     		:熊坚强
*创建日期   		:2008-9-22
*版本号       		:V1.1
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_SPI_
#include "config.h"

extern STLocalControl 	xdata stLocalControl;

uint8    xdata  GT23_Rec_Data[128];
uint8    xdata  GT23_Rec_Length;
uint16   xdata  GT23_16LAddress;
uint16   xdata  GT23_16HAddress;
uint32   xdata  GT23_32Address; 
extern uint8   xdata SerchRoom;
extern bit     Page_Flag;
uint8  xdata   Modify_Room_Number=0;
uint8  xdata   Modify_SEC_Name=0;
extern uint8 xdata  Page_Color[5];
uint8  xdata  ID_Number=0;
uint8  xdata  ID_Number_Flag=0;

uint8 xdata 
FRAM_BUFF[12];

void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes);
extern void Delayms(uint16 ms);

/**********************************************************
*函数名称			:Delay_nus	
*函数描述        	:延时程序
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Delay_nus(uint8 xdata i)
{
 	uint8 xdata j;
 	for(;i>0;i--)
 	{
 	  for(j=2;j>0;j--);
 	}
}
/**********************************************************
*函数名称			:SpiTransferByte	
*函数描述        	:SPI总线传输一个字节数据,并返回读到的一个字节数据
*输入参数   		:byData:待传输的数据
*返回值				:SPDAT:读取到的数据 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 SpiTransferByte(uint8 byData)
{
	SPDAT = byData;
	while((SPSTAT&0x80) == 0x00);				//等待传输完成
	SPSTAT = 0xC0;
	return(SPDAT);
}
uint8 READ_ID(void)
{
   	uint8 byRet;
	SST_CS = 0;	 	
	SpiTransferByte(SST_READ_ID); 		    //传输读写ID号命令
	SpiTransferByte(0x00);	
	SpiTransferByte(0x00);
	SpiTransferByte(0x01);	
	byRet = SpiTransferByte(0xFF); 	       //接收ID号数据 	
	SST_CS = 1;		
	return(byRet);
}
/**********************************************************
*函数名称			:SpiInit	
*函数描述        	:SPI接口初始化
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SpiInit(void)
{
	P4M0  &= 0xDD;
	P4M1  &= 0xDD;
	SPCTL  = 0xDC;											//忽略SS引脚,使能SPI接口,高位在前,Fosc/4	
	SPSTAT = 0xC0;											//清除标志 
	SST_CS=0;
	SpiTransferByte(SST_EWSR);
	SST_CS=1;
	SST_CS=0;
	SpiTransferByte(SST_WRSR);		
	SpiTransferByte(0x00);		
	SST_CS=1;
	ID_Number=READ_ID();
	if(ID_Number==0x8C)      //PCT
	ID_Number_Flag=1;
	else if(ID_Number==0x43) //SST
	ID_Number_Flag=2;
}
/**********************************************************
*函数名称			:Read_Status_Register	
*函数描述        	:读取SST存储器状态寄存器中的数据
*输入参数   		:
*返回值				:byRet:读取到的数据 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 Read_Status_Register(void)
{
	uint8 byRet;
	SST_CS=0;				
	SpiTransferByte(0x05);		
	byRet = SpiTransferByte(0xFF); 	
	SST_CS=1;				
	return byRet;
}
/**********************************************************
*函数名称			:Wait_Busy	
*函数描述        	:等待写数据结束
*输入参数   		:
*返回值				: 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Wait_Busy(void)
{
   while((Read_Status_Register()&0x01) == 0x01)
   {
     Delay_nus(10);
   }
}
/**********************************************************
*函数名称			:FmReadByte	
*函数描述        	:读取SST存储器指定地址的一个字节数据
*输入参数   		:uiAddr:地址
*返回值				:byRet:读取到的数据 	
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 FmReadByte(uint32 uiAddr)
{
	uint8 byRet;
	SST_CS = 0;	 	
	SpiTransferByte(SST_READ); 		        //传输读命令
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);	
	byRet = SpiTransferByte(0xFF); 	       //接收数据 	
	SST_CS = 1;		
	return(byRet);
}

/************************************************************************/
/* PROCEDURE:	Read_Cont												*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*          SST25VF020:Dst:	Destination Address 000000H - 03FFFFH              	*/
/*      no_bytes	Number of bytes to read	(max = 128)					*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes)
{
	unsigned long i = 0;
	SST_CS = 0;				/* enable device */
	SpiTransferByte(0x03); 			/* read command */
	SpiTransferByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SpiTransferByte(((Dst & 0xFFFF) >> 8));
	SpiTransferByte(Dst & 0xFF);
	for (i = 0; i < no_bytes; i++)		/* read until no_bytes is reached */
	{
		pBuffer[i] = SpiTransferByte(0xFF);	/* receive byte and store at address 80H - FFH */
	}
	SST_CS = 1;					/* disable device */

}

/**********************************************************
*函数名称			:FmWriteByte	
*函数描述        	:在SST存储器指定地址写入一个字节数据
*输入参数   		:uiAddr:地址,byData:待写入的数据
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void FmWriteByte(uint32 uiAddr, uint8 byData)
{ 
    SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //传输写使能命令
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_SECTOR_EARSE); //传输擦除命令	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);			
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //传输写使能命令
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;	 	
	SpiTransferByte(SST_WRITE);       //传输写命令	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);		
	SpiTransferByte(byData); 	      //写入数据  	
	SST_CS = 1;
	Wait_Busy();
}


void FmWriteByte1(uint32 uiAddr, uint8 byData)
{ 

	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //传输写使能命令
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;	 	
	SpiTransferByte(SST_WRITE);       //传输写命令	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);		
	SpiTransferByte(byData); 	      //写入数据  	
	SST_CS = 1;
	Wait_Busy();
}


/**********************************************************
*函数名称			:FmWriteBytes	
*函数描述        	:在SST存储器指定地址存多个字节数据
*输入参数   		:uiAddr:地址,uiLen:待写入的数据长度,pbyData:待写入数据首地址
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#if 1
void FmWriteBytes(uint32 uiAddr, uint16 uiLen, uint8* pbyData)
{
	if(0 == uiLen)
	{
		return;
	} 
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //传输写使能命令
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_SECTOR_EARSE); //传输擦除命令	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));                            /* 发送3个字节的地址信息        */
    SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
    SpiTransferByte(uiAddr & 0xFF);		
	SST_CS = 1;
	Wait_Busy();
	while (uiLen--)
	{
		SST_CS = 0;
        SpiTransferByte(SST_WREN);                                                    /* 发送写使能命令               */
        SST_CS = 1;
    	Wait_Busy();
        SST_CS = 0;
        SpiTransferByte(0x02);                                                    /* 发送字节数据烧写命令         */
        SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));                            /* 发送3个字节的地址信息        */
        SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
        SpiTransferByte(uiAddr & 0xFF);
        SpiTransferByte(*pbyData++);                                             /* 发送被烧写的数据             */
		uiAddr++;
        SST_CS = 1;
    	Wait_Busy();
    }
	SST_CS=0;
	SpiTransferByte(SST_WRDI);		
	SST_CS = 1;  
	Wait_Busy();
}
#else
void FmWriteBytes(uint32 uiAddr, uint16 uiLen, uint8* pbyData)
{
	uint16  xdata highaddr;
	uint16  xdata lowaddr;	
	highaddr = uiAddr/65536;
    lowaddr  = uiAddr%65536;  
	if(0 == uiLen)
	{
		return;
	} 
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //传输写使能命令
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_SECTOR_EARSE); //传输擦除命令	
	SpiTransferByte(highaddr%256);
	SpiTransferByte(lowaddr/256);
	SpiTransferByte(lowaddr%256);		
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //传输写使能命令
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	if(ID_Number_Flag==0x01)
	SpiTransferByte(SST_AAI_P);         //传输地址自动增加命令 
	else if(ID_Number_Flag==0x02)
	SpiTransferByte(SST_AAI_S);	
	SpiTransferByte(highaddr%256);
	SpiTransferByte(lowaddr/256);
	SpiTransferByte(lowaddr%256);		
	SpiTransferByte(*pbyData);
	if(ID_Number_Flag==0x01)
	{
	  pbyData++;
	  SpiTransferByte(*pbyData);
	}
	SST_CS=1;
	Wait_Busy();
	uiLen--;
    if(ID_Number_Flag==0x01)
	uiLen--;
    pbyData++;
	for(;uiLen>0;)				      //写入数据 
	{	
		SST_CS=0;
	    if(ID_Number_Flag==0x01)
		SpiTransferByte(SST_AAI_P);     //传输地址自动增加命令 
		else if(ID_Number_Flag==0x02)
		SpiTransferByte(SST_AAI_S);
		SpiTransferByte(*pbyData);
	    if(ID_Number_Flag==0x01)
		{
		  pbyData++;
		  SpiTransferByte(*pbyData);
		}
		SST_CS=1;
		Wait_Busy();
		uiLen--;
	    if(ID_Number_Flag==0x01)
		uiLen--;
		pbyData++;
	}	
	SST_CS=0;
	SpiTransferByte(SST_WRDI);		
	SST_CS = 1;  
	Wait_Busy();
}
#endif


void FmWriteBytes1(uint32 uiAddr, uint16 uiLen, uint8* pbyData)
{
	if(0 == uiLen)
	{
		return;
	} 
 
	while (uiLen--)
	{
		SST_CS = 0;
        SpiTransferByte(SST_WREN);                                                    /* 发送写使能命令               */
        SST_CS = 1;
    	Wait_Busy();
        SST_CS = 0;
        SpiTransferByte(0x02);                                                    /* 发送字节数据烧写命令         */
        SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));                            /* 发送3个字节的地址信息        */
        SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
        SpiTransferByte(uiAddr & 0xFF);
        SpiTransferByte(*pbyData++);                                             /* 发送被烧写的数据             */
		uiAddr++;
        SST_CS = 1;
    	Wait_Busy();
    }
	SST_CS=0;
	SpiTransferByte(SST_WRDI);		
	SST_CS = 1;  
	Wait_Busy();
}

/**********************************************************
*函数名称			:ReadParameter	
*函数描述        	:读取配置参数
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void ReadParameter(void)
{
	stLocalControl.uiShortType = IAP_ADDR;
	stLocalControl.pbyData= & (stLocalControl.stEepromCfgData.bySelfSecAddr);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	EnableIAP(IAP_READ); 		
	while(stLocalControl.byChar1--)
	{ 		
		*stLocalControl.pbyData++ = IapReadByte(stLocalControl.uiShortType++);		
	}
	DisableIAP(); 
	//加载使能数据
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
}
/**********************************************************
*函数名称			:SaveParameter	
*函数描述        	:存储配置参数,存储参数前先擦除整个扇区,
					 然后将所有参数重新写入扇区
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:EnableIAP,IapErase,IapWriteByte,DisableIAP
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void SaveParameter(void)
{  
	stLocalControl.uiShortType = IAP_ADDR;	
	stLocalControl.pbyData = &(stLocalControl.stEepromCfgData.bySelfSecAddr);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 
	while(stLocalControl.byChar1--)
	{
		IapWriteByte(stLocalControl.uiShortType++, *stLocalControl.pbyData++);
	}
	//如果是在写参数命令中调用该函数,重新加载使能数据
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	DisableIAP();	
	EnableIAP(IAP_ERASE);
	IapErase(0x0000); 	
	EnableIAP(IAP_WRITE);
	IapWriteByte(0x0000,INIT_FLAG);
	DisableIAP();
}
/**********************************************************
*函数名称			:ParameterInit	
*函数描述        	:参数初始化
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void ParameterInit(void)
{ 
	uint8 byTemp;
	//开始读取系统参数
    EnableIAP(IAP_READ);
	if(IapReadByte(0x0000)==INIT_FLAG)
	{
		ReadParameter();
	}
	else
	{
		stLocalControl.stEepromCfgData.bySelfSecAddr=CBYTE[0x2800];
		stLocalControl.stEepromCfgData.bySelfRoomAddr=CBYTE[0x2801];
		stLocalControl.stEepromCfgData.bySelfBedAddr=CBYTE[0x2802];
		if(FmReadByte(Close_EnSoundNote_ADDR)==0x01)
			stLocalControl.stEepromCfgData.byEnable1=0xF7;
		else
			stLocalControl.stEepromCfgData.byEnable1=CBYTE[0x2803];
		stLocalControl.stEepromCfgData.byEnable2=CBYTE[0x2804];
		stLocalControl.stEepromCfgData.byPrio=CBYTE[0x2805];
		stLocalControl.stEepromCfgData.byRingTime=CBYTE[0x2806];
		stLocalControl.stEepromCfgData.byTalkTime=CBYTE[0x2807];
		stLocalControl.stEepromCfgData.byBroadTime=CBYTE[0x2808];
		stLocalControl.stEepromCfgData.byListenDelay=CBYTE[0x2809];
		stLocalControl.stEepromCfgData.byVolumStep=CBYTE[0x280A];
		stLocalControl.stEepromCfgData.byRingVol=CBYTE[0x280B];
		stLocalControl.stEepromCfgData.byCH0TalkedVol=CBYTE[0x280C];
		stLocalControl.stEepromCfgData.byCH1TalkedVol=CBYTE[0x280D];
		stLocalControl.stEepromCfgData.byBroadVol=CBYTE[0x280E];
		stLocalControl.stEepromCfgData.byNoteVol=CBYTE[0x280F];
		stLocalControl.stEepromCfgData.byMusicVol=CBYTE[0x2810];
		stLocalControl.stEepromCfgData.byCH0TalkVol=CBYTE[0x2811];
		stLocalControl.stEepromCfgData.byCH1TalkVol=CBYTE[0x2812];
		stLocalControl.stEepromCfgData.bySelfRingVol=CBYTE[0x2813];
		stLocalControl.stEepromCfgData.byMaxVol=CBYTE[0x2814];
		stLocalControl.stEepromCfgData.byMinVol=CBYTE[0x2815];
		stLocalControl.stEepromCfgData.byReserve1=CBYTE[0x2816];
		stLocalControl.stEepromCfgData.byReserve2=CBYTE[0x2817];
		stLocalControl.stEepromCfgData.byReserve3=CBYTE[0x2818];
		stLocalControl.stEepromCfgData.bySerialNum1=CBYTE[0x2819];
		stLocalControl.stEepromCfgData.bySerialNum2=CBYTE[0x281A];
		stLocalControl.stEepromCfgData.bySerialNum3=CBYTE[0x281B];
		stLocalControl.stEepromCfgData.byVersionHi=CBYTE[0x281C];
		stLocalControl.stEepromCfgData.byVersionLo=CBYTE[0x281D];
		byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
		byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	}
	//读取液晶配置参数
	byTemp = FmReadByte(LCD_INIT_ADDR);
	if(INIT_FLAG == byTemp)
	{ 	//已经初始化了
		Modify_SEC_Name	= FmReadByte(LCD_SEC_MSK_ADDR);
	    stLocalControl.uiShortType = LCD_SEC_ADDR;	            //开始读取颜色数据
        stLocalControl.uiShortType =stLocalControl.uiShortType+18;
	    Page_Color[0]=FmReadByte(stLocalControl.uiShortType++);//科室字体颜色 
	    Page_Color[1]=FmReadByte(stLocalControl.uiShortType++);//科室字体背景色,也是清屏色
	    Page_Color[2]=FmReadByte(stLocalControl.uiShortType++);//中间填充颜色
	    Page_Color[3]=FmReadByte(stLocalControl.uiShortType++);//几床到几床字体背景色
	    Page_Color[4]=FmReadByte(stLocalControl.uiShortType++);//几床到几床背景色,也是清屏色
	}
	else
	{	//没有初始化
		byTemp = FmReadByte(LCD_INIT_ADDR);
		if(INIT_FLAG == byTemp)
		{	//重新检查
			Modify_SEC_Name	= FmReadByte(LCD_SEC_MSK_ADDR);
			stLocalControl.uiShortType = LCD_SEC_ADDR;		              //开始读取颜色数据
            stLocalControl.uiShortType =stLocalControl.uiShortType+18;
	        Page_Color[0]=FmReadByte(stLocalControl.uiShortType++);//科室字体颜色 
	        Page_Color[1]=FmReadByte(stLocalControl.uiShortType++);//科室字体背景色,也是清屏色
	        Page_Color[2]=FmReadByte(stLocalControl.uiShortType++);//中间填充颜色
	        Page_Color[3]=FmReadByte(stLocalControl.uiShortType++);//几床到几床字体背景色
	        Page_Color[4]=FmReadByte(stLocalControl.uiShortType++);//几床到几床背景色,也是清屏色
			return;
		}
		FmWriteByte(LCD_INIT_ADDR, INIT_FLAG);
		FmWriteByte(LCD_SEC_MSK_ADDR, 0);
	}
	SerchRoom=FmReadByte(Serch_Room_ADDR);
	if(SerchRoom==1||SerchRoom==2||SerchRoom==3||SerchRoom==4||SerchRoom==5||SerchRoom==6||SerchRoom==7||SerchRoom==8||SerchRoom==9||SerchRoom==10||SerchRoom==11||SerchRoom==12||SerchRoom==13||SerchRoom==14||SerchRoom==15)
	{  
	 	SerchRoom=SerchRoom;
	}
	else
		SerchRoom=0;
	if(INIT_FLAG==FmReadByte(Serch_Room_Page_ADDR))
		Page_Flag=1;
	else
		Page_Flag=0;	
	Modify_Room_Number=FmReadByte(Modify_Room_Number_ADDR);
}
/**********************************************************
*函数名称			:Get_GT23_Data	
*函数描述        	:读取字库的点阵数据
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-3-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size)
{
	 uint32  xdata address1,address2,address3,address4;

	 if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <0x7E) //   0x20~0x7e  是ASCII字符
	 {
		 switch(word_size)
		 {
			 case 1: //16*8
			 default:
				 address1 = (lowbyte-0x20)*16; 
				 address3 = address1+A8BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,16);
				 break; 
				 
			 case 2: //24x12
				 address1 = (lowbyte-0x20)*48; 
				 address3 = address1+A12BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,48);
				 break; 				 
			 
			 case 3:	//32X16
				 address1 = (lowbyte-0x20)*64; 
				 address3 = address1+A16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,64);
				 break; 				 
/*				 case 4:   //96x48
				 if(lowbyte<=0x39&&lowbyte>=0x30)  //数字
				{
				 address1 = (lowbyte-0x21)*576; 
				 address3 = address1+D48BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,576); 				
				}
				break;*/
				 
		 }		 
	 
	 }
	 else if(highbyte >=0x81&&highbyte<=0xfe&&lowbyte>=0x40&&lowbyte<=0xfe)    //GBK内码 32X32
	 {
		switch(word_size)
		{
			case 1:
			default:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*32;				 //每个汉字占用32字节
				 address4 = address3+C16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,32);
				 break;
			case 3:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*128;				 //每个汉字占用128字节
				 address4 = address3+C32BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,128);
				 break; 			 
		}
	 }

}



/**********************************************************
*函数名称			:Get_GT23_Data1	
*函数描述        	:读取字库的点阵数据
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:熊坚强
*创建日期	 		:2011-3-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Get_GT23_Data1(uint8 *word,uint8 word_size)
{
	 uint32  xdata address1,address2,address3,address4;
	 uint8 xdata highbyte,lowbyte;
	 highbyte = *word;
	 word++;
	 lowbyte = *word;
	 if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <0x7E) //   0x20~0x7e  是ASCII字符
	 {
		 switch(word_size)
		 {
			 case 1: //16*8
			 default:
				 address1 = (lowbyte-0x20)*16; 
				 address3 = address1+A8BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,16);
				 break; 
				 
			 case 2: //24x12
				 address1 = (lowbyte-0x20)*48; 
				 address3 = address1+A12BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,48);
				 break; 				 
			 
			 case 3:	//32X16
				 address1 = (lowbyte-0x20)*64; 
				 address3 = address1+A16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,64);
				 break; 				 
/*				 case 4:   //96x48
				 if(lowbyte<=0x39&&lowbyte>=0x30)  //数字
				{
				 address1 = (lowbyte-0x21)*576; 
				 address3 = address1+D48BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,576); 				
				}
				break;*/
				 
		 }		 
	 
	 }
	 else if(highbyte >=0x81&&highbyte<=0xfe&&lowbyte>=0x40&&lowbyte<=0xfe)    //GBK内码 32X32
	 {
		switch(word_size)
		{
			case 1:
			default:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*32;				 //每个汉字占用32字节
				 address4 = address3+C16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,32);
				 break;
			case 3:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*128;				 //每个汉字占用128字节
				 address4 = address3+C32BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,128);
				 break; 			 
		}
	 }

}


/************************************************************************/
/* PROCEDURE: WREN														*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WREN()
{
	SST_CS=0;				/* enable device */
	SpiTransferByte(0x06);		/* send WREN command */
	SST_CS=1;				/* disable device */
	Wait_Busy();
}


/************************************************************************/
/* PROCEDURE: Sector_Erase												*/
/*																		*/
/* This procedure Sector Erases the Chip.								*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Sector_Erase(unsigned long Dst)
{


	SST_CS=0;					/* enable device */
	SpiTransferByte(0x20);			/* send Sector Erase command */
	SpiTransferByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SpiTransferByte(((Dst & 0xFFFF) >> 8));
	SpiTransferByte(Dst & 0xFF);
	SST_CS=1;					/* disable device */
}

/**********************************************************
*函数名称			:FRAM_TEST	
*函数描述        	:存储芯片测试操作
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
void FRAM_TEST(void)
{
	unsigned char i;
	
	FRAM_BUFF[0]=0x11;
	FRAM_BUFF[1]=0x22;
	FRAM_BUFF[2]=0x33;
	FRAM_BUFF[3]=0x44;
	FRAM_BUFF[4]=0x55;
	FRAM_BUFF[5]=0x66; 
	
	FRAM_BUFF[6]=0x77;
	FRAM_BUFF[7]=0x88;
	FRAM_BUFF[8]=0x99;
	FRAM_BUFF[9]=0xaa;
	FRAM_BUFF[10]=0xbb;
	
	
	FmWriteByte(0x3e9000,FRAM_BUFF[0]);
	FmWriteBytes1(0x3e9002,10,&(FRAM_BUFF[1]));
	
	for(i=0;i<12;i++) FRAM_BUFF[i]=0;
	Delayms(10);

	FRAM_BUFF[0]=FmReadByte(0x3e9000);
	Read_Cont(&(FRAM_BUFF[1]),0x3e9002,10);
	Delayms(10);

	Send_Data(FRAM_BUFF,11);


}



