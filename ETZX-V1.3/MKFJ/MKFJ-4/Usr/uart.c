/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:uart.c
*文件描述    		:串口驱动程序
*创建人     		:陈卫国
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_UART_
#include "config.h"

STUartControl idata stUartControl;							//串口控制结构体
uint8 xdata byUartBuff[UART0_BUF_SIZE];        				//给读串口消息队列分配的队列空间	
uint8 data byUsartData;										//串口缓冲区暂存
 
extern STLocalControl 	xdata stLocalControl;
uint8  xdata Page_Color[5]={0x00,0x00,0x00,0x00,0x00};
extern uint8  xdata  Modify_SEC_Name;
uint32 xdata Fbaud;   //波特率的数值
uint8  xdata Fbaud_Type;

extern bit bUsartRecFrame;

extern void Delayms(uint16 ms);
/**********************************************************
*函数名称			:UartInit	
*函数描述        	:串口初始化
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
void UartInit(void)
{
	//控制字初始化
	stUartControl.byState  = 0x00;
	stUartControl.byRxTail = 0x00; 
	stUartControl.byCrc    = 0x00;
	//波特率设置
	AUXR |= 0x40;											//1T模式 
	SCON = 0x50;
	PCON &= 0x7F;
	TMOD |= 0x20;
	//TH1 = 0xF6;
	//TL1 = 0xF6;												//波特率设置为57600		
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	//2012/4/9波特率可变
	if(INIT_FLAG==FmReadByte(Set_Baud_ADDR))
	{
	   Fbaud_Type=FmReadByte(System_Baud_ADDR);
	   switch(Fbaud_Type)
	   {
	     case 1:
		 Fbaud=1200;
		 break;
		 case 2:
		 Fbaud=2400;
		 break;
		 case 3:
		 Fbaud=4800;
		 break;
		 case 4:
		 Fbaud=9600;
		 break;
		 case 5:
		 Fbaud=14400;
		 break;
		 case 6:
		 Fbaud=19200;
		 break;
		 case 7:
		 Fbaud=28800;
		 break;
		 case 8:
		 Fbaud=38400;
		 break;
		 case 9:
		 Fbaud=57600;
		 break;
		 case 10:
		 Fbaud=115200;
		 break;
	   }
	}
	else
	{
	  Fbaud=57600;
	}
	if(Fbaud>2000)
	{
	  TL1=TH1=256-(Fosc/32/Fbaud);
	}
	else
	{
	  AUXR &= 0xBF;
	  TL1=TH1=256-(Fosc/32/Fbaud/12);
	}
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------	
	TR1 = 1;
	ES = 1;	
}
/**********************************************************
*函数名称			:UartDeal	
*函数描述        	:串口接收到一帧数据处理函数
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
void UartDeal(void)
{ 	
    //-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	//增加自动检测命令
    if(CMD_RS485_CHECK == ((STUartFreq xdata*)byUartBuff)->byCmd)//如果是自动检测命令
	{
	    stLocalControl.stBusDealFreq.byRecSecAddr = CMD_RS485_CHECK;
	    stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	    stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
	    stLocalControl.stBusDealFreq.byCmd = 0x02;
	    stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
	    stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
	    stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));


		return;
	}
	//-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	if(((STUartFreq xdata*)byUartBuff)->byRecAreaAddr != stLocalControl.stEepromCfgData.bySelfSecAddr)
	{	//地址不是到本机的 		
		return;
	}
	if(((STUartFreq xdata*)byUartBuff)->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr)
	{	//地址不是到本机的		
		return;
	}
	if(((STUartFreq xdata*)byUartBuff)->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr)
	{	//地址不是到本机的			
		return;
	} 	  
    if(CMD_DISPLAY_DATA == ((STUartFreq xdata*)byUartBuff)->byCmd) //发送科室配置信息
	{	
		stLocalControl.pbyData = ((STUartFreq xdata*)byUartBuff)->byData;
		//stLocalControl.pbyData++;
		stLocalControl.pbyData+=3;
		if(0x00 != *(stLocalControl.pbyData))	   //...是否为内码数据
		{	
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;			
			goto ExitDeal;
		}
		stLocalControl.pbyData++;
	    if(0x04 != *(stLocalControl.pbyData))    //...是否为门口分机界面
		{	
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;			
			goto ExitDeal;
		}
		stLocalControl.pbyData++;
		if(*(stLocalControl.pbyData) > 0)      //...屏号数据为0
		{	//超过最大屏数目了 
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;					
			goto ExitDeal;	
		}
		stLocalControl.uiShortType = stLocalControl.byChar1 = *(stLocalControl.pbyData);   //屏号放在stLocalControl.byChar1中,开始存储到铁电存储器中
		stLocalControl.uiShortType += LCD_SEC_ADDR; 	
		stLocalControl.pbyData ++;
		if(*(stLocalControl.pbyData) > 0)	 //只接收一帧数据，帧号数据为0
		{	//超过最大行数目了
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;					
			goto ExitDeal;	
		}
		stLocalControl.byChar2 = *(stLocalControl.pbyData);	  //帧号放在stLocalControl.byChar2中
		
		//返回正确应答
		stLocalControl.stBusDealFreq.byRecSecAddr = CMD_DISPLAY_DATA;
		stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
		stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
		stLocalControl.stBusDealFreq.byCmd = 0x02;
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		   
		if(stLocalControl.byChar2==0x00)                      //帧号为0表示发送的是科室名称、几床到几床、颜色数据
		{
		  stLocalControl.uiShortType += stLocalControl.byChar2 * (((STUartFreq xdata*)byUartBuff)->byLen-8);	 //总共41个字节存储(科室14+床号4+颜色5+责任医生8+责任护士8+床号2)
		  stLocalControl.byChar2 = (1 << stLocalControl.byChar1);		
		  stLocalControl.pbyData++;
		  if(*(stLocalControl.pbyData) == 0x01)	//数据为结束行
		  {
			Modify_SEC_Name	= 0x01;
			FmWriteByte(LCD_SEC_MSK_ADDR,0x01); 
			stLocalControl.byChar2=0x01;
		  }  
		  //所有数据正确，写入数据
		  stLocalControl.pbyData++;
		  FmWriteBytes(stLocalControl.uiShortType,((STUartFreq xdata*)byUartBuff)->byLen-8,stLocalControl.pbyData);	//开始存储发送屏数据	
	      stLocalControl.uiShortType = LCD_SEC_ADDR;	   //...开始读取颜色数据
          stLocalControl.uiShortType =stLocalControl.uiShortType+18;
		  Page_Color[0]=FmReadByte(stLocalControl.uiShortType++);//科室字体颜色 
	      Page_Color[1]=FmReadByte(stLocalControl.uiShortType++);//科室字体背景色,也是清屏色
	      Page_Color[2]=FmReadByte(stLocalControl.uiShortType++);//中间填充颜色
		  Page_Color[3]=FmReadByte(stLocalControl.uiShortType++);//几床到几床字体背景色
	      Page_Color[4]=FmReadByte(stLocalControl.uiShortType++);//几床到几床背景色,也是清屏色
		  if(stLocalControl.byChar2 == 0x01)
		  {	
		    ReadPageColour();
			SetLcdState(1);	
			ShowPage();	
		  }
		}
		return;  	
	}
	stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;		
ExitDeal:
	stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
    stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
} 
/**********************************************************
*函数名称			:UartInt	
*函数描述        	:串口中断函数
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
#pragma disable
void comm(void) interrupt 4
{
	if(RI == 1)
	{       
		RI = 0;
		//取出串口数据
		byUsartData = SBUF;
		//设置接收超时				
		MakeCH0TimerOut(5, 0);	
		switch(stUartControl.byState)
		{
			/*case 0:											//起始阶段					
				if(byUsartData == START_BYTE1)
				{	//起始字符正确,起始字符不存储					
					stUartControl.byState = 1;
					stUartControl.byLen = 7; 
					stUartControl.byRxTail = 0;	
					stUartControl.byCrc = 0;																										
				}
				else
				{
					stUartControl.byRxTail = 0;
				} 			
				break;
			case 1:
				stUartControl.byCrc += byUsartData;	
				byUartBuff[stUartControl.byRxTail++] = byUsartData;			
				if(--stUartControl.byLen == 0x00)
				{	
				    stUartControl.byState = 2;
					stUartControl.byLen = 2;
				}					
				break;
			case 2:	
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.byLen == 0x00)
				{
				  if(MAX_FREQ_DATA_LEN == byUsartData)
				  {	//长度值正确
				    stUartControl.byLen = MAX_FREQ_DATA_LEN;
				    stUartControl.byState = 3;
			 	 }
				else
				{	//长度值错误,退出接收
				   stUartControl.byState = 0;
				   stUartControl.byRxTail = 0;
				}
			   }					
				break; 				
			case 3:
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.byLen == 0x00) 
				{	//接收完所有数据
					stUartControl.byState = 4; 									
				}
				break;
			case 4:
				if(stUartControl.byCrc == byUsartData)
				{	//校验和正确
					stUartControl.byState = 5;
				}
				else
				{	//校验和错误,退出接收
					stUartControl.byState = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 5:	
				stUartControl.byState = 0;			
				if(END_BYTE == byUsartData)
				{	//结束字符正确 						
					OS_INT_ENTER();
					OSQIntPost(byMainCmdQ, USART_REC); 				
					OSIntExit();
				}				
				break;		
		   	default:
				stUartControl.byState = 0;
				stUartControl.byRxTail = 0;
				break;*/
		    //2012-3-14增加检测板自动检测功能
		 	case 0:											//起始阶段					
				if(byUsartData == START_BYTE1)
				{	//起始字符正确,起始字符不存储					
					stUartControl.byState = 1;
					stUartControl.byLen = 7; 
					stUartControl.byRxTail = 0;	
					stUartControl.byCrc = 0;
																															
				}
				else
				{
					stUartControl.byRxTail = 0;
				} 			
				break;
			case 1:
				stUartControl.byCrc += byUsartData;	
				byUartBuff[stUartControl.byRxTail++] = byUsartData;			
				if(--stUartControl.byLen == 0x00)
				{	
				    stUartControl.byState = 2;
					stUartControl.byLen = 2;
					//--------------------------------------------------
				    //--------------------------------------------------
				    //增加自动检测功能
                    if(CMD_RS485_CHECK == ((STUartFreq xdata*)byUartBuff)->byCmd)//如果是自动检测命令
				    {
	
				      stUartControl.byState = 0x20;
				      stUartControl.byLen = 7; 
				    }
				   //--------------------------------------------------
				   //--------------------------------------------------
				}					
				break;
			case 0x20:
			  	stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.byLen == 0x00)
				{
				  stUartControl.byState = 4;
				  
			    }
			    break;
			case 2:	
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;	
				
				stUartControl.byLen--;
				if(stUartControl.byLen == 1)
				{	
					stUartControl.uiRecLen = byUsartData<<8; 
				}				
	
				
				else if(stUartControl.byLen == 0x00)
				{
					stUartControl.uiRecLen += byUsartData;
					if(stUartControl.uiRecLen <= MAX_FREQ_DATA_LEN)					
					{	//数据长度符合要求
						stUartControl.byState = 3;
						//stUartControl.uiRecLen += 2; 
					}				

					else
					{	//长度值错误,退出接收
					   stUartControl.byState = 0;
					   stUartControl.byRxTail = 0;
					}
			   }					
				break; 				
			case 3:
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.uiRecLen == 0x00) 
				{	//接收完所有数据
					stUartControl.byState = 4; 									
				}
				break;
			case 4:
				if(stUartControl.byCrc == byUsartData)
				{	//校验和正确
					stUartControl.byState = 5;
				}
				else
				{	//校验和错误,退出接收
					stUartControl.byState = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 5:	
				stUartControl.byState = 0;		
				if(END_BYTE == byUsartData)
				{	//结束字符正确 	
					bUsartRecFrame=1;
								     
				}				
				break;		
		   	default:
				stUartControl.byState = 0;
				stUartControl.byRxTail = 0;
				break;							
		}	 
	}
	else
	{
		TI = 0;		
	}
}


void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
    uint8 xdata i;
	
    ES=0;
	for(i=0;i<l;i++)
     {
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;
	 }
	ES=1;
}


void Send_Data_Byte(uint8 SendData)
{ 
	uint8 xdata i;
	
	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
}

