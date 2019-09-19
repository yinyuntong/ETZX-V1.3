/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Uart.c
*文件描述    		:串口驱动程序
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
#define _IN_USART_
#include "config.h"

//串口变量定义
STUsartCfg  xdata stUsartCfg;
uint8	xdata byUsart0RecBuf[UART0_RX_BUF_SIZE];
uint8   xdata byUsart0SndBuf[UART0_TX_BUF_SIZE];


bit bUsart0RecFinish= 0;
bit bRS485DataError = 0;

bit bUsartRecedDietData = 0;
bit bUsartRecedPrescriptionData = 0;
bit bUsartRecedPromptData =0;




extern STLocalControl xdata stLocalControl;
extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];


uint8 xdata Frame_No=0;		//帧号
uint8 xdata Frame_Type=0xff;	//帧数据的类型

uint16 xdata uiBedFaceFormLen=0;
uint16 xdata uiBedFaceDataLen=0;

bit bDispBedFace=0;
bit bDispBedForm=0;
bit bBus0AckOk=0;
bit bBus0AckFail=0;

uint8 CheckSum=0;	//整个床头卡信息的校验和
uint8 checkFormData(int16 datalen)
{
	uint32 dataaddr = BED_FACE_FORM_START_ADDR_2;
	STItemForm stItemForm;
	if(datalen%sizeof(STItemForm) ==0)
	{//个数符合要求
		if(datalen/sizeof(STItemForm)<MAX_ITEM_CODE)
		{
			for(;datalen>0;)
			{
				Read_Cont((uint8 *)&stItemForm,dataaddr,sizeof(STItemForm));
				if( stItemForm.ItemCode>=MAX_ITEM_CODE || stItemForm.StartPoint.x>=stItemForm.EndPoint.x ||stItemForm.StartPoint.y >=stItemForm.EndPoint.y || \
					stItemForm.EndPoint.x>=LCD_PIX_X || stItemForm.EndPoint.y >= LCD_PIX_Y || stItemForm.bcolor > COLOR_MAX_VAL || stItemForm.fcolor >COLOR_MAX_VAL)
						break;
				else
				{
					dataaddr+= sizeof(STItemForm);
					datalen -= sizeof(STItemForm);
				}
			}
			if(datalen ==0)
			{//数据校验全部正确
				return 1;
			}
			else return 0;
		}
		else return 0;
	}

	else return 0;

}


uint8 checkFaceData(int16 datalen)
{
	uint32 dataaddr = BED_FACE_DATA_START_ADDR_2;
	uint8 ItemData[2];   //ItemData[0]:项目代码   //ItemData[1]:项目数据长度
	if(datalen<2) return 0;
	
	for(;datalen>0;)
	{
		Read_Cont(ItemData,dataaddr,2);
		if(ItemData[0]>=MAX_ITEM_CODE ) return 0;			//项目代码超过最大极限

		dataaddr += (2+ ItemData[1]);
		
		if(datalen < (2+ItemData[1]))   return 0;			//项目长度超过总长度

		datalen -= (2+ ItemData[1]);

		if(datalen ==0) return 1;			//总长度是各项长度之和

		else if(datalen<2 ) return 0;
		
	}
}



uint8 crcCheckData(uint32 addr,uint16 length,uint8 crc)
{
	uint8 i;
	uint8 crcSum=0;

	OS_ENTER_CRITICAL();
	do
	{
		if(length<=128)
		{
			Read_Cont(GT23_Rec_Data,addr,length);
			for(i=0;i<length;i++)
			{
				crcSum += GT23_Rec_Data[i];
			}
			if(crc == crcSum) i=1;
			else 			  i=0;
			break;
		}
		else
		{
			
			Read_Cont(GT23_Rec_Data,addr,128);
			for(i=0;i<128;i++)
			{
				crcSum += GT23_Rec_Data[i];
			}
			addr+=128;
			length -= 128;
			
			//复位看门狗
			WDT_CONTR = 0x3e; 			
		}		
	}while(length>0);
	WDT_CONTR = 0x3e;	//喂狗
    OS_EXIT_CRITICAL();
	return i;
}


void SectorDataCopy(unsigned long dst,unsigned long src,uint16 length)
{
	OS_ENTER_CRITICAL();
	do
	{
		if(length<=128)
		{
			Read_Cont(GT23_Rec_Data,src,length);
			Write_Cont(dst,GT23_Rec_Data,length);
			break;
		}

		else
		{
			
			Read_Cont(GT23_Rec_Data,src,128);
			Write_Cont(dst,GT23_Rec_Data,128);
			src+=128;
			dst+=128;
			length -= 128;
			WDT_CONTR = 0x3e;	//喂狗
			
		}
	}while(length>0);
	WDT_CONTR = 0x3e;	//喂狗
    OS_EXIT_CRITICAL();

}


uint8 StoreDataCheck(unsigned long dst,uint16 length)
{
	uint8 i;
	uint8 crc=0;
	OS_ENTER_CRITICAL();
	while(1)
	{
		if(length<=128)
		{
			Read_Cont(GT23_Rec_Data,dst,length);
			for(i=0;i<length;i++) crc += GT23_Rec_Data[i];
		
			dst += length;
			if(Read(dst)== crc) i=1;	//最后校验和相等
			else i=0;
			OS_EXIT_CRITICAL();
			return i;
		}
		else
		{
			Read_Cont(GT23_Rec_Data,dst,128);
			for(i=0;i<128;i++) crc += GT23_Rec_Data[i];
			dst+=128;
			length -= 128;
			WDT_CONTR = 0x3e;	//喂狗
		}
	}
}

uint8 IsSingleItem(uint8 *startaddr,uint8 length)
{
	if( *(startaddr+2)==1 && *(startaddr+4)==1 )
	{//一串数据只有一帧数据
		startaddr +=5;
		length -=5;

		if( (length-2)== *(startaddr+1) ) return 1;		// 只有一项
		else return 0;
	}
	else return 0;
}


/**********************************************************
*函数名称			:UsartInit	
*函数描述        	:串口初始化函数
*输入参数   		:
*返回值				:
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void UsartInit(void)
{
	AUXR	&=	~BRTR;		//停止独立波特率发生器 

	//P3.1 TXD 开漏输出：因连着床头分机的MK，经过10k电阻至5V
	P3M0 |= 0x02;
	P3M1 |= 0x02;
	TXD =1;

	//串口0初始化
	stUsartCfg.pbyRecBuf = byUsart0RecBuf;
	stUsartCfg.pbySndBuf = byUsart0SndBuf;	
	stUsartCfg.byRecState=0;
	stUsartCfg.uiRecPosition=0;
	stUsartCfg.uiRecLen=0;
	stUsartCfg.byRecCount=0;
	stUsartCfg.uiSndLen  = stUsartCfg.uiSndPos  = 0;
	
	SCON=0X50;		//工作方式1
    PCON&=	~SMOD;   //  ;波特率倍增选择		smod=0
    AUXR	|=	S1BRS;	//特用波特率发生器


	if(stLocalControl.stEepromCfgData.uiRs485Brt>=2400)
	{
		AUXR	|=	BRTx12; 	//BRTX12=1; 1T模式,最后波特率为:4800*12=57.6K
		BRT= 256-(Fosc/32/stLocalControl.stEepromCfgData.uiRs485Brt	);
		
	}
	
	else
	{
		AUXR	&=	~BRTx12; 	//BRTX12=0; 12T模式
		BRT= 256-(Fosc/32/12/stLocalControl.stEepromCfgData.uiRs485Brt	);		
	}


	
	AUXR	|=	BRTR;		//启动独立波特率发生器   
	
	RI=0;
	TI=0;
	REN=1;
	ES=1;

}



void UsartInitConfig(uint16 boudRate)
{
	AUXR	&=	~BRTR;		//停止独立波特率发生器 

	//P3.1 TXD 开漏输出：因连着床头分机的MK，经过10k电阻至5V
	P3M0 |= 0x02;
	P3M1 |= 0x02;
	TXD =1;

	//串口0初始化
	stUsartCfg.pbyRecBuf = byUsart0RecBuf;
	stUsartCfg.pbySndBuf = byUsart0SndBuf;	
	stUsartCfg.byRecState=0;
	stUsartCfg.uiRecPosition=0;
	stUsartCfg.uiRecLen=0;
	stUsartCfg.byRecCount=0;
	stUsartCfg.uiSndLen  = stUsartCfg.uiSndPos  = 0;
	
	SCON=0X50;		//工作方式1
    PCON&=	~SMOD;   //  ;波特率倍增选择		smod=0
    AUXR	|=	S1BRS;	//特用波特率发生器

	if(boudRate>=2400)
	{
		AUXR	|=	BRTx12; 	//BRTX12=1; 1T模式,最后波特率为:4800*12=57.6K
		BRT= 256-(Fosc/32/boudRate);
		
	
	}
	else
	{
		AUXR	&=	~BRTx12; 	//BRTX12=0; 12T模式
		BRT= 256-(Fosc/32/12/boudRate);		
	}
	
	AUXR	|=	BRTR;		//启动独立波特率发生器   
	RI=0;
	TI=0;
	REN=1;
	ES=1;
}



/**********************************************************
*函数名称			:UART0(void)	
*函数描述        	:串口0中断函数
*输入参数   		:
*返回值				:
*全局变量			:stUsartCfg
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
#pragma disable
void UART0(void) interrupt UART_INTNO
{
	uint8 xdata byTemp;
	STUartFreq *xdata pstDataFreq;
	
	if(RI==1)
	{  
		RI=0;
		byTemp=SBUF;
		if(stUsartCfg.uiRecPosition!=0)	//不是起始字节
		{
			if(stUsartCfg.byRecTimeout==0)
			{//超时到了
			 	stUsartCfg.byRecState=0;
				stUsartCfg.uiRecPosition=0;
				return;
			}
		}
		stUsartCfg.byRecTimeout=5;	//又开始定时
		byUsart0RecBuf[stUsartCfg.uiRecPosition++] = byTemp;		
		switch(stUsartCfg.byRecState)
		{
			case 0: 							
				if(byTemp == START_BYTE1)
				{
					stUsartCfg.byRecState = 1;
//					byCount = 7;
					stUsartCfg.byRecCount = 6;
				}
				else
				{
					stUsartCfg.uiRecPosition = 0;
				}
				break;
			case 1: 							
				if(--stUsartCfg.byRecCount == 0)
				{	//收完地址数据
					
					pstDataFreq =(STUartFreq *)byUsart0RecBuf;
					if((pstDataFreq->byRecSecAddr != 0xff) && (pstDataFreq->byRecSecAddr != stLocalControl.stEepromCfgData.bySelfSecAddr))
					{//区号不相符	
						stUsartCfg.byRecState=0;
						stUsartCfg.uiRecPosition=0;
						return;

					} 

					if(stLocalControl.stEepromCfgData.byRs485SecRoom)//需要判断房号
					{
						if((pstDataFreq->byRecRoomAddr != 0xff) && (pstDataFreq->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr))
						{//房号不相符	
							stUsartCfg.byRecState=0;
							stUsartCfg.uiRecPosition=0;
							return;
						}
					}//不需要判断房号就不管
					
					if((pstDataFreq->byRecBedAddr != 0xff) && (pstDataFreq->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr))
					{//床号不相符	
						stUsartCfg.byRecState=0;
						stUsartCfg.uiRecPosition=0;
						return;
					}				

					stUsartCfg.byRecState = 2;
					stUsartCfg.byRecCount = 3;	
				}
				break;				
			case 2: 			
				stUsartCfg.byRecCount--;
				if(stUsartCfg.byRecCount == 1)
				{	
					stUsartCfg.uiRecLen = byTemp<<8; 
				}
				else if(stUsartCfg.byRecCount == 0)
				{	//接收完数据长度
					//stUsartCfg[0].uiRecLen += (byTemp << 8); 			//获取数据域长度值
					stUsartCfg.uiRecLen += byTemp;
					if(stUsartCfg.uiRecLen <= MAX_UART0_DATA_LEN)					
					{	//数据长度符合要求
						stUsartCfg.byRecState = 3;
						stUsartCfg.uiRecLen += 2; 
					}
					else
					{
						stUsartCfg.byRecState = 0;
						stUsartCfg.uiRecPosition = 0;
					}										
				}
				break;			
			case 3: 			
				if(--stUsartCfg.uiRecLen == 0)
				{	//接收完所有数据
					if(END_BYTE == byTemp)
					{	//结束位正确
						bUsart0RecFinish=1;	
					}
					stUsartCfg.byRecState = 0;
					stUsartCfg.uiRecPosition = 0; 			
				}
				break;			
			default:
				stUsartCfg.byRecState = 0;
				stUsartCfg.uiRecPosition = 0;
				break;		
		}
	}
	if(TI==1)
	{
		TI=0;

		pstDataFreq = (STUartFreq *)byUsart0SndBuf;
		stUsartCfg.uiSndLen--;
		stUsartCfg.uiSndPos++;		
		if(stUsartCfg.uiSndLen!=0)
		{    

			SBUF = stUsartCfg.pbySndBuf[stUsartCfg.uiSndPos];			
		}
		else
		{//一帧数据发送完成			
			switch(pstDataFreq->byData[1])	//信息类型
			{
			case DIET_TYPE:
				stLocalControl.byUsart1SdDelayTime =150;
				break;
				
			case PRESCRIPTION_TYPE:	
				stLocalControl.byUsart1SdDelayTime =150;
				break;
			case PROMPT_INFO_TYPE:
				stLocalControl.byUsart1SdDelayTime =150;
				break;
			}		
		}
	}
} 

/**********************************************************
*函数名称			:GetPskCrc	
*函数描述        	:计算数据桢累加校验和
*输入参数   		:pstDataFreq:待计算的数据桢指针
*返回值				:累加校验和
*全局变量			:stUsartCfg
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 GetPskCrc(STUartFreq *pstDataFreq)
{
	uint8   *xdata pbyData;
	uint16 xdata uiLen;
	uint8  xdata byCrc = 0;
	
//	pbyData = &(pstDataFreq->byRecSecAddr);
	pbyData = &(pstDataFreq->bySndSecAddr);
	uiLen = pstDataFreq->uiLen + HEAD_SIZE;
	while(uiLen--)
	{
		byCrc += *pbyData++;
	}	
	return(byCrc);
}
/**********************************************************
*函数名称			:CheckPsk	
*函数描述        	:检验数据包的正确型
*输入参数   		:pstDataFreq:待校验的数据桢指针
*返回值				:true:数据包正确,false:数据包有误
*全局变量			:
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
uint8 CheckPsk(pSTUartFreq pstDataFreq)
{
	uint8 *xdata pbyData;
	uint16 xdata uiLen;
	uint8  xdata byCrc = 0;
	
//	pbyData = &(pstDataFreq->byRecSecAddr);
	pbyData = &(pstDataFreq->bySndSecAddr);
	uiLen = pstDataFreq->uiLen + HEAD_SIZE;
	while(uiLen--)
	{
		byCrc += *pbyData++;
	}
	if(byCrc != *pbyData)
	{
		return(false);
	}
	return(true);	
}

/**********************************************************
*函数名称			:Usart0Manager	
*函数描述        	:串口0管理函数
*输入参数   		:
*返回值				:
*全局变量			:stUsartCfg,stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void Usart0RecDeal(void)
{	
	STUartFreq *xdata pstDataFreq;
	uint8 xdata temp;    //bit4=1:代表广播
	uint16 xdata uiDataLen;


	SCON  &= 0XEF;      //关闭REN
	uiDataLen = byUsart0RecBuf[LEN_HIGH_POSITION]*256+byUsart0RecBuf[LEN_LOW_POSITION];
	bUsart0RecFinish=0;

	pstDataFreq = (pSTUartFreq)byUsart0RecBuf;
	

	
	if(stLocalControl.stEepromCfgData.byRs485SecRoom==0x00)//不需要判断房号
	{
		if(pstDataFreq->byRecBedAddr  == stLocalControl.stEepromCfgData.bySelfBedAddr)	//地址相符
			temp =0x01;	//直接地址
		else
		{
			if(pstDataFreq->byRecBedAddr==0xff)
			{//床号是广播地址
				if(pstDataFreq->byRecRoomAddr==0xff)
				{//房号是广播地址	
					temp =0x11;		//广播地址
				}
				else if(pstDataFreq->byRecRoomAddr<240)
				{//房号暂时规定只到240
					temp =0x11;    //广播地址
				}
				else   temp=0x00;
			}
			else temp =0x00;
		}
	}
	else	//需要判断
	{

		stLocalControl.stBusDealFreq.byRecSecAddr = pstDataFreq->byRecSecAddr;
		stLocalControl.stBusDealFreq.byRecRoomAddr = pstDataFreq->byRecRoomAddr;
		stLocalControl.stBusDealFreq.byRecBedAddr = pstDataFreq->byRecBedAddr;	
		if(AddrCompare(&(stLocalControl.stBusDealFreq)))
		{
			if((stLocalControl.stBusDealFreq.byRecSecAddr == stLocalControl.stEepromCfgData.bySelfSecAddr)&&
			   (stLocalControl.stBusDealFreq.byRecRoomAddr == stLocalControl.stEepromCfgData.bySelfRoomAddr)&&
			   (stLocalControl.stBusDealFreq.byRecBedAddr == stLocalControl.stEepromCfgData.bySelfBedAddr))
				  temp =0x01;   //直接地址
			else temp =0x11;    //广播地址
		}
		else temp=0;
	}


	if(temp&0x01)
	{//是本机数据
	
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;	

		
		if(uiDataLen>5)
		{//数据域中的数据长度符合要求		
			if(!CheckPsk(pstDataFreq))	
			{//数据帧出错	
				if(byUsart0RecBuf[DATA_POSITION4]==0)
				{//不是结束帧数据
					bRS485DataError	=1;
					//Send_Data_Byte(0x01);
				}
				else
				{//是最后一帧
					//Send_Data_Byte(0x02);
					if( temp!=0x11)
					{//不是广播地址
						stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						
					}
					bRS485DataError	=0;		//标志恢复
					Frame_No =0;
					Frame_Type = 0xff;					
				}
			}

			
			else
			{//校验和正确
				//BL_SW=1;	//打开显示屏背光
				BL_ON();
				stLocalControl.uiLcdDisplayTime= 500;	//亮10秒钟


				if(byUsart0RecBuf[DATA_POSITION2] == 0x01)
				{//第一帧数据
					bRS485DataError = 0;
					Frame_No =0;
					Frame_Type =byUsart0RecBuf[DATA_POSITION1];
				}
								
				if(bRS485DataError==0)   
				{//未曾出错

					if( pstDataFreq->byCmd == CMD_DISPLAY_DATA ) 
					{	//本机床头液晶显示数据			

						if(byUsart0RecBuf[DATA_POSITION1]==BED_CARD_TYPE)	//BED_CARD_TYPE 在这里代表长名字的床头卡信息
						{
						
							if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
							{//第一帧数据

								//计数出数据域有效数据的校验和
								CheckSum = *(pstDataFreq->byData + pstDataFreq->uiLen);
								for(temp =0;temp<14;temp++) 	//地址6字节+命令1字节+长度2字节+附属数据5字节(共14字节)
								{
									CheckSum -= byUsart0RecBuf[temp+1];		//从sndsec开始
								}	
								
								Frame_No =1;
								Frame_Type = byUsart0RecBuf[DATA_POSITION1];								
								Write_Cont(BED_FACE_DATA_START_ADDR_2,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
								uiBedFaceDataLen = uiDataLen-5;

							}
							
							else 
							{								
							/*	if(Frame_No >= byUsart0RecBuf[DATA_POSITION2]) 
								{//后续帧号比前帧号小,不影响原数据的接收
									goto Usart0RecDealRet;
								}*/
										
								if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
								{//没有丢帧，是按顺序来的
									if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
									{//信息类型一致
										Frame_No ++;
									    //不进行擦除写
										if(uiBedFaceDataLen < LENGTH_4K)
										{
											//计数出数据域有效数据的校验和
											CheckSum += *(pstDataFreq->byData + pstDataFreq->uiLen);
											for(temp =0;temp<14;temp++) 	//地址6字节+命令1字节+长度2字节+附属数据5字节(共14字节)
											{
												CheckSum -= byUsart0RecBuf[temp+1];		//从sndsec开始
											}	
											
											Write_Cont(BED_FACE_DATA_START_ADDR_2+uiBedFaceDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
											uiBedFaceDataLen += (uiDataLen-5);
										}
										else  bRS485DataError=1;   //床头卡总长度超限	

									}
									else  bRS485DataError=1;    //信息类型不一致											
								}
								
								else   bRS485DataError=1;    //中间有丢帧

							}
							
							if(byUsart0RecBuf[DATA_POSITION4])	
							{//一屏数据结束,立即显示
								if(bRS485DataError ==0)
								{//未曾出错
									if(checkFaceData(uiBedFaceDataLen))
									{
										if(crcCheckData(BED_FACE_DATA_START_ADDR_2,uiBedFaceDataLen,CheckSum)==1)
										{//预存区域数据校验和正确

											Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&uiBedFaceDataLen,2);		//写长度
											SectorDataCopy(BED_FACE_DATA_START_ADDR,BED_FACE_DATA_START_ADDR_2,uiBedFaceDataLen); 	//写数据
											CheckSum += uiBedFaceDataLen>>8;
											CheckSum += (uiBedFaceDataLen&0x00ff);
											Write_Cont(BED_FACE_DATA_START_ADDR+uiBedFaceDataLen,&CheckSum,1);	//写校验和
											//再进行校验
											if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,uiBedFaceDataLen+2))	//将数据长度也包含进去	
											{
												stLocalControl.uiBedFaceDataLen = uiBedFaceDataLen;										
	usart0RecDisp:	
												bDispBedFace=1;
												bBus0AckOk =1;
												stLocalControl.bySlaveState &= ~PATIENT_DISCHARGE; //病人入院
											}
											else bBus0AckFail=1;
										}
										else bBus0AckFail =1;
									}
									else bBus0AckFail =1;
								}
								else 
								{
									bBus0AckFail=1;	
									goto usart0RecFailRet;	//曾经出过错强制退出
								}
							}	
						}
					}

					else if(pstDataFreq->byCmd ==  CMD_SB_LCD_DISPLAY_DATA)
					{//是手柄数据			
						switch(byUsart0RecBuf[DATA_POSITION1])	//信息类型
					 	{
						 	case DIET_TYPE:
								if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
								{//第一帧数据
									Frame_No =1;
									Frame_Type = byUsart0RecBuf[DATA_POSITION1];
								/*	WREN();
									Block_Erase_32K(DIET_DATA_START_ADDR);
									Wait_Busy();*/
									Write_Cont(DIET_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
															
									stLocalControl.uiDietDataLen = (uiDataLen-5);

								}
								else 
								{										
									if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
									{//没有丢帧，是按顺序来的
										if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
										{//信息类型一致
											Frame_No ++;
											if(stLocalControl.uiDietDataLen <LENGTH_32K)
											{
												Write_Cont(DIET_DATA_START_ADDR+stLocalControl.uiDietDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
												stLocalControl.uiDietDataLen  += (uiDataLen-5);		//加数据长度
											}
											else  bRS485DataError=1; //总长度超限
										}
										else  bRS485DataError=1; //信息类型不一致											
									}
									else  bRS485DataError=1; //中间有丢帧														
								}

								if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
								{//结束标志
									if(bRS485DataError ==0)
									{//未曾出错								
										Write_Cont(DIET_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiDietDataLen)),2);	//保存数据长度								    	
										bUsartRecedDietData =1;
										bBus0AckOk =1;
									}
									else 
									{
										bBus0AckFail=1;	
										goto usart0RecFailRet;	//曾经出过错强制退出
									}									
								}
								break;

							case PRESCRIPTION_TYPE:
								if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
								{//第一帧数据
									Frame_No =1;
									Frame_Type = byUsart0RecBuf[DATA_POSITION1];
									/*WREN();
									Block_Erase_32K(PRESCRIPTION_DATA_START_ADDR);
									Wait_Busy();*/
									Write_Cont(PRESCRIPTION_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);	//每类型数据可达32K
									stLocalControl.uiPrescriptionDataLen = uiDataLen-5;
								}
								else 
								{	
									if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
									{
										if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
										{//信息类型一致	
											Frame_No++;
											//不进行擦除写
											if(stLocalControl.uiPrescriptionDataLen< LENGTH_32K)
											{
												Write_Cont(PRESCRIPTION_DATA_START_ADDR+stLocalControl.uiPrescriptionDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
												stLocalControl.uiPrescriptionDataLen  += (uiDataLen-5);		//加数据长度
											}
											else bRS485DataError=1; //总长度超限
										}
										else   bRS485DataError=1; //信息类型不一致									
									}
									else bRS485DataError=1; //中间有丢帧
								}

								if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
								{//结束标志
									if(bRS485DataError ==0)
									{//未曾出错								
										Write_Cont(PRESCRIPTION_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPrescriptionDataLen)),2);	//保存数据长度									
										bUsartRecedPrescriptionData =1;	
										bBus0AckOk =1;
									}
									else 
									{
										bBus0AckFail=1;	
										goto usart0RecFailRet;	//曾经出过错强制退出
									}									
								}
								break;
							case PROMPT_INFO_TYPE:
								if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
								{//第一帧数据
									Frame_No =1;
									Frame_Type = byUsart0RecBuf[DATA_POSITION1];
								/*	WREN();
									Block_Erase_32K(PROMPT_DATA_START_ADDR);
									Wait_Busy();*/
									Write_Cont(PROMPT_DATA_START_ADDR,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);      	//每类型数据可达32K																						
									stLocalControl.uiPromptDataLen = uiDataLen-5;
								}
								else 
								{	
									if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
									{	
										if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
										{//信息类型一致	
											Frame_No++;
											//不进行擦除写
											if(stLocalControl.uiPromptDataLen < LENGTH_32K)
											{
												Write_Cont(PROMPT_DATA_START_ADDR+stLocalControl.uiPromptDataLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
												stLocalControl.uiPromptDataLen  += (uiDataLen-5);		//加数据长度
											}
											else bRS485DataError=1; //总长度超限
										}
										else  bRS485DataError=1; //信息类型不一致										
									}
									else    bRS485DataError=1; //中间有丢帧
								}
								
								if(byUsart0RecBuf[DATA_POSITION4] == 0x01)
								{//结束标志
									if(bRS485DataError ==0)
									{//未曾出错								
										Write_Cont(PROMPT_LENGTH_ADDR,(uint8 *)(&(stLocalControl.uiPromptDataLen)),2);	//保存数据长度     
										bUsartRecedPromptData =1;
										bBus0AckOk =1;
									}
									else 
									{
										bBus0AckFail=1;	
										goto usart0RecFailRet;	//曾经出过错强制退出
									}									
								}
								break;
								
					 	}	
					}
					else if(pstDataFreq->byCmd == CMD_DISPLAY_FORM)
					{
						if(byUsart0RecBuf[DATA_POSITION1]==BED_CARD_TYPE)	//BED_CARD_TYPE 在这里代表长名字的床头卡信息
						{
							if(byUsart0RecBuf[DATA_POSITION2] == 0x01)	
							{//第一帧数据
								//计数出数据域有效数据的校验和
								CheckSum = *(pstDataFreq->byData + pstDataFreq->uiLen);
								for(temp =0;temp<14;temp++) 	//地址6字节+命令1字节+长度2字节+附属数据5字节(共14字节)
								{
									CheckSum -= byUsart0RecBuf[temp+1];		//从sndsec开始
								}
								
								Frame_No =1;
								Frame_Type = byUsart0RecBuf[DATA_POSITION1];							
								Write_Cont(BED_FACE_FORM_START_ADDR_2,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
								uiBedFaceFormLen = uiDataLen-5;
							}
							
							else 
							{									
								if((Frame_No+1) == byUsart0RecBuf[DATA_POSITION2])
								{	
									if(Frame_Type == byUsart0RecBuf[DATA_POSITION1])
									{//信息类型一致	
										Frame_No++;
										//不进行擦除写	
										if(uiBedFaceFormLen <LENGTH_4K)
										{
											//计数出数据域有效数据的校验和
											CheckSum += *(pstDataFreq->byData + pstDataFreq->uiLen);
											for(temp =0;temp<14;temp++) 	//地址6字节+命令1字节+长度2字节+附属数据5字节(共14字节)
											{
												CheckSum -= byUsart0RecBuf[temp+1];		//从sndsec开始
											}
											
											Write_Cont(BED_FACE_FORM_START_ADDR_2+uiBedFaceFormLen,&(byUsart0RecBuf[DATA_POSITION5]),uiDataLen-5);
											uiBedFaceFormLen += (uiDataLen-5);
										}
										else bRS485DataError=1; //总长度超限
									}
									else  bRS485DataError=1; //信息类型不一致									
								}
								else  bRS485DataError=1; //中间有丢帧
							}
							
							if(byUsart0RecBuf[DATA_POSITION4])	
							{//一屏格式数据结束,保存数据长度
								if(bRS485DataError ==0)
								{//未曾出错
									if(checkFormData(uiBedFaceFormLen))
									{
										if(crcCheckData(BED_FACE_FORM_START_ADDR_2,uiBedFaceFormLen,CheckSum)==1)
										{//预存区域数据校验和正确
	
											Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&uiBedFaceFormLen,2);		//写长度
											SectorDataCopy(BED_FACE_FORM_START_ADDR,BED_FACE_FORM_START_ADDR_2,uiBedFaceFormLen);	//写数据
											CheckSum += uiBedFaceFormLen>>8;
											CheckSum += (uiBedFaceFormLen&0x00ff);
											Write_Cont(BED_FACE_FORM_START_ADDR+uiBedFaceFormLen,&CheckSum,1);	//写校验和
											//再进行校验
											if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,uiBedFaceFormLen+2))	//将数据长度也包含进去	
											{
												stLocalControl.uiBedFaceFormLen = uiBedFaceFormLen; 									
												bDispBedForm=1;
											}
										}
									}
								
								}
							}
						}
					}
				}
				else
				{//曾经出过错
					if(byUsart0RecBuf[DATA_POSITION4]==1)
					{//最后一帧数据
						if(temp != 0x11)
						{//不是广播地址
							bBus0AckFail=1;
							goto usart0RecFailRet;
						}
					}
				}
usart0RecFailRet:
				if(byUsart0RecBuf[DATA_POSITION4]==1)
				{//最后一帧数据
					bRS485DataError=0;	//清标志
					Frame_No =0;
					Frame_Type= 0xff;
				}

				
				if(bBus0AckOk==1)
				{
					bBus0AckOk=0;
					stLocalControl.stBusDealFreq.byCmd = CMD_BUS_ANSWER; 
					stLocalControl.stBusDealFreq.byRecSecAddr = pstDataFreq->byCmd;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));					
				}

				if(bBus0AckFail==1)
				{
					bBus0AckFail=0;
					stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));				
				}
			}
		}
		else
		{//数据长度不符
			if(temp	!=0x11)
			{//不是广播地址
				stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;
				Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				//bRS485DataError	=0;		//标志恢复
			}
		}		
	}
Usart0RecDealRet:	
	SCON  |= 0X10;    //打开REN
}


/**********************************************************
*函数名称			:UsartManager
*函数描述        	:单总线0管理线程
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2009-4-7
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void UsartManager(void)	 
{	
	while(true)
	{
		if(bUsart0RecFinish)									//串口0收到一帧数据
		{ 	

//			OSQPost(byMainCmdQ,UART0_CMD);	
		}
 
		OSWait(K_TMO, 1);
		WDT_CONTR = 0x3e;	//喂狗			
	}

}
