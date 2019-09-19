/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:main.c
*文件描述    		:主函数
*创建人     		:尹运同
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
#define _IN_MAIN
#include "config.h"

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//主线程消息队列
STLocalControl 	xdata stLocalControl;						//全局变量结构体

bit bWireLessRec=0;
bit bEnableOxTimer=0;				//如果没收到供氧开始不充许计时

uint8 xdata byWireLessRecBuff[10];
uint8 xdata byWireLessRecLen;


uint8 xdata byWireLessSndBuff[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};

uint8 code  	run[12]={31,29,31,30,31,30,31,31,30,31,30,31};   
uint8 code   notrun[12]={31,28,31,30,31,30,31,31,30,31,30,31};


bit bOXSupplyState=0;		//供氧状态标志



void Ex0Int(void)	interrupt X0_INTNO		//0号中断
{
	EX0=0;
}

void TIMER1Int(void) interrupt T1_INTNO		//3号中断
{
	ET1=0;
}

#if STC12C5A==true
void AdcInt(void)	interrupt ADC_INTNO		//5号中断
{
	EADC=0;
}

void LvdInt(void) interrupt LVD_INTNO		//6号中断
{
	ELVD=0;
}

//void PcaInt(void) interrupt PCA_INTNO		//7号中断
//{
//	CMOD	&=	~ECF;			//禁止 PCA计数器溢出中断   
//	CCAPM0	&=	~PCA0_ECCF;		//禁止CCF0中断
//	CCAPM1	&=	~PCA1_ECCF;		//禁止CCF1中断	
//}

#endif
/***********************************************************/
void UserTickTimer(void)   /* 系统定时中断中调用的用户函 */
{ 
	#if STC90 == 1  
		TF2=0;
	#endif
	#if STC12C5A==1
		uiIsrTimerCount+=ISR_INC_COUNT;
		CCAP0L = (uint8)uiIsrTimerCount;
		CCAP0H = (uint8)(uiIsrTimerCount>>8);
		CCF0=0;
	#endif
	if(stUsartCfg.byRecTimeout!=0) stUsartCfg.byRecTimeout--;
}



void Send_Data(uint8 *Databuf,uint8 xdata l)
{ 
    uint8 xdata i;
	Databuf=Databuf;
	l=l;
	i=i;
#if DEBUG==1
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	WDT_CONTR = 0x3e;	//喂狗
    ES=0;
	for(i=0;i<l;i++)
     {
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;
	 }
	ES=1;
#endif

/*#if DEBUG==1
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	stUsartCfg.uiSndLen=l;
	stUsartCfg.uiSndPos=0;
	stUsartCfg.pbySndBuf=Databuf;
	SBUF=stUsartCfg.pbySndBuf[0];
#endif
*/
}


void Send_Data_Byte(uint8 SendData)
{ 
	uint8 xdata i;
	SendData=SendData;
	i=i;
#if DEBUG ==1
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	WDT_CONTR = 0x3e;	//喂狗
	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
#endif

/*#if DEBUG==1	
	i=1000;
	while(stUsartCfg.uiSndLen)
	{
		delay_nms(1);
		if(--i ==0) break;
	}
	stUsartCfg.uiSndLen=1;
	stUsartCfg.uiSndPos=0; 
	SBUF=SendData;	
#endif
*/
}


void RD_ID(void)
{
	uint8 idata *p;
	p=0xf1;
	Send_Data(p,7);
}

bit DirRFSerialCompare()	//序列号最高位忽略
{
	if(stLocalControl.stEepromCfgData.byRFSerialNum1!= byWireLessRecBuff[1])
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum2!= byWireLessRecBuff[2])
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum3!= byWireLessRecBuff[3])
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum4!= byWireLessRecBuff[4])
	{	
		return(0);
	} 
	if(stLocalControl.stEepromCfgData.byRFSerialNum5!= byWireLessRecBuff[5])
	{	
		return(0);
	}
	if(stLocalControl.stEepromCfgData.byRFSerialNum6!= byWireLessRecBuff[6])
	{	
		return(0);
	}
	
	return(1); 	
}


uint16   allday(STTime   a)  	//返回多少天 
{    
	uint16 xdata  x; 
	uint8 xdata i;
	x=(a.byYear)*365+a.byYear/4;   
	if((a.byYear%4)==0)  
	{   
		for(i=0;i<a.byMonth;i++)   
		{   
			if(i>0)   
			{   
				 x=x+run[i-1];   
			}   
		}   
	}   
	else   
	{   
		for(i=0;i<a.byMonth;i++)   
		{   
			if(i>0)   
			{   
				x=x+notrun[i-1];   
			}   
		}   
	}   
	x=x+a.byDay;   
	return   x;   
}  

STOXTime   timeInterval(STTime a,STTime   b)  	//a:结束时间  b:起始时间  
{   
  uint16   xdata x,y;   
  STOXTime   xdata sum;   
  x=allday(a);   
  y=allday(b);   
  sum.uiHour=(x-y)*24+a.byHour-b.byHour;   
  if((a.byMin)<b.byMin)   
  {   
  	sum.byMin=a.byMin+60-b.byMin;
	sum.uiHour=sum.uiHour-1;   
  }
  else  
  {   
	sum.byMin=a.byMin-b.byMin;   
  } 

  if((a.bySec)<b.bySec)   
  {   
  	sum.bySec=a.bySec+60-b.bySec;
	sum.byMin=sum.byMin-1;   
  }
  else  
  {   
	sum.bySec=a.bySec-b.bySec;   
  }
  return   sum;   
 }   

/**********************************************************
*函数名称			:Init	
*函数描述        	:硬件初始化操作
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
void Init(void)
{
	CMOD = 0x02;
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块0为16位软件定时器,ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;
 	 
//	TF1=0;
//	CCAPM1 = (PCA1_CAPN|PCA1_ECCF);		//PCA1捕获中断
//	AUXR1 |= PCA_P4;		//映射到P4口

	//启动PCA计数器计数
	CR = 1;	
	

	//IO口配置
	//STC12C52/5A系列:M0=0,M1=0:准双向，M0=0,M1=1:高阻，M0=1,M1=0:推挽，M0=1,M1=1:开漏
	
	//特别注意:因输入口线上外围未配置上拉电阻，不能配置成高阻输入，只能配置成准双向，否则该脚在高电平时处于悬浮状态


	P4SW=P46EN|P45EN|P44EN;	//P4口设置成IO口


	memset(&(stLocalControl.stBusDealFreq.bySndSecAddr),0x00,sizeof(STLocalControl));		//将变量数据清0

	//IO初始化
	SSD1963_CS	= 1;
	SST25VF_CS = 1;
	GT23L_CS = 1;
	ADS7843_CS = 1;
	EPH1660_CS = 0;

//	UsartInitConfig(57600);
//	Send_Data_Byte(0x33);
	Delayms(200);

	//读取系统配置字
	InitParameter();

  	//串口初始化
  	UsartInit();
	Send_Data_Byte(0x88);	


	//复位EPH1660
//	ResetEPH1660();

	//设置MCU为SPI主方
	MCUMasterSPI();
	

//	RD_ID();

	//存储器初始化

    manID=Read_ID(0x00);
	devID=Read_ID(0X01);

	Flash_Lock();

	//测试存储器
//	FRAM_TEST();


	//单总线初始化 	
	SingleBusInit();
	
	//RGB驱动芯片初始化
	InitSSD1963();
	page=0;
	ShowPowerUpFace();


#if CONFIG_CC1101==1	
	CC1101Init();
#endif
		
	//设置上电登记标志,灯闪烁时间初始化,超时设置(超时后自动发送上电登记命令)	
	bLanding = 1;	
	MakeCH0TimerOut(50, 0);	


	stLocalControl.stSupplyOxTotalTime.uiHour=stLocalControl.stEepromCfgData.stSupplyOxTotalTime.uiHour;
	stLocalControl.stSupplyOxTotalTime.byMin=stLocalControl.stEepromCfgData.stSupplyOxTotalTime.byMin;

	memset(&(stLocalControl.stTime.byYear),0x00,6);	//清当前时间数据

	stLocalControl.byDispNumSetOkTime=0;


#if CONFIG_CC1101==1
//移到最后,以免cc1101初始化时产生中断造成误读造成读空间溢出
	TF1=0;
	CCAPM1 = (PCA1_CAPN|PCA1_ECCF);		//PCA1捕获中断
	AUXR1 |= PCA_P4;	     	//映射到P4口	
	halSpiStrobe(CCxxx0_SRX);	//允许接收
#endif

	//床头界面存储到显示内存中
	page=1;
	ShowBedFace();

	memset(BedDataBuff,0x00,SEGMENT_DATA_LEN);
	memset(byUsart0SndBuf,0x00,UART0_TX_BUF_SIZE);

	stLocalControl.uiCC1101ReReadTime = CC1101_REREAD_TIME;

	//复位看门狗
	WDT_CONTR = 0x3e; 	

    Set_VisualPage(1);	
	stLocalControl.byDisplayFace=BED_INFO_FACE;

	fcolor=BLUE;
	page=0;
	Lcd_Clear();		//将第0页清屏（用于呼叫信息显示)		
}
/**********************************************************
*函数名称			:GetMessage	
*函数描述        	:获取主线程消息队列中的消息(无消息则挂起自身)
*输入参数   		:Msg:存储消息指针
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
void GetMessage(uint8 data *Msg)
{
	OSQPend(Msg, byMainCmdQ, 0);
}
/**********************************************************
*函数名称			:DispatchMessage	
*函数描述        	:分发处理获取的主线程消息
*输入参数   		:Msg:消息,高4位是命令类型,低4位是命令数据
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
void DispatchMessage(uint8 Msg)
{
	switch(Msg&0xf0)
	{
		case BUS0_REC:										//总线0收到数据帧
			Bus0RecDeal();
			break;
		case BUS0_SND:	 									//总线0数据发送完成
			Bus0SendDeal();
			break;			
		case TIMER_OUT:	 									//超时处理
			TimerOutDeal();
			break;	
		default:
			break;
	}	
}
/**********************************************************
*函数名称			:MainTask	
*函数描述        	:系统主线程,负责整个系统的消息处理
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
void MainTask(void)
{
	static uint8 data Msg;  
	uint8 i;
	
	//系统硬件初始化
	Init();	
	//创建主线程消息队列	
	OSQCreate(byMainCmdQ, 16);
	//以下为创建任务线程	
	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(TimerOutManager, NULL, 2);	
	//进入消息循环	

//	RD_ID();

	while(TRUE)
	{
		//复位看门狗
		WDT_CONTR = 0x3e; 
		GetMessage(&Msg);
		DispatchMessage(Msg);
		if(bUsart0RecFinish)									//串口0收到一帧数据
		{ 	
			bUsart0RecFinish=0;
			Usart0RecDeal();	
		}

#if CONFIG_CC1101==1
		if(bWireLessRec==1)
		{
			//Send_Data(byWireLessRecBuff,byWireLessRecLen);
			bWireLessRec=0;
			byWireLessRecLen=8;	//要读取数据的最大长度
			if(halRfReceivePacket(byWireLessRecBuff,&byWireLessRecLen)==CRC_OK)
			{
				if(byWireLessRecLen==8)
				{//输液报警器上来的信号
					if(byWireLessRecBuff[7]==CMD_NUMBER_SET)	
					{//是对码命令
						memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(byWireLessRecBuff[1]),3);
						stLocalControl.stBusDealFreq.byCmd = CMD_RF_NUMSET_START;
						memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(byWireLessRecBuff[4]),3);
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
					else 
					{
						if(byWireLessRecBuff[7]==CMD_INFUSION_CALL)
						{
							if(DirRFSerialCompare())	//是相应的输液报警器
							{
								memcpy(&(stLocalControl.stBusDealFreq.bySndSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
								stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CALL;
								Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
							}						
						}
					
					}
				}
				else if(byWireLessRecLen==6)
				{//移动分机上来的信号
					if(byWireLessRecBuff[0]==0x05)
					{//起始位正确
						if(byWireLessRecBuff[5]==(uint8)(byWireLessRecBuff[0]+byWireLessRecBuff[1]+byWireLessRecBuff[2]+byWireLessRecBuff[3]+byWireLessRecBuff[4]))
						{//校验和相等
							if((byWireLessRecBuff[1]== stLocalControl.stEepromCfgData.bySelfSecAddr) && (byWireLessRecBuff[2]== 251))
							{//区号相等且为移动分机
								if(byWireLessRecBuff[4] ==0x01)
								{//命令是护士到位命令
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
									stLocalControl.stBusDealFreq.bySndBedAddr = byWireLessRecBuff[3];	//移动分机号
									stLocalControl.stBusDealFreq.byCmd = CMD_NURSE_IN;
									memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState &PATIENT_DISCHARGE)) //黑屏的情况下
									{
										BL_ON();//打开显示屏
									}								
									
								}
								else if(byWireLessRecBuff[4] ==0x02)
								{//命令是查房命令
									stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
									stLocalControl.stBusDealFreq.bySndRoomAddr = 251;
									stLocalControl.stBusDealFreq.bySndBedAddr = byWireLessRecBuff[3];	//移动分机号
									stLocalControl.stBusDealFreq.byCmd = CMD_INSPECTOR_CALL;
									memcpy(&(stLocalControl.stBusDealFreq.byRecSecAddr),&(stLocalControl.stEepromCfgData.bySelfSecAddr),3);
									Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
									if( !(stLocalControl.bySlaveState &LCD_ON) || (stLocalControl.bySlaveState &PATIENT_DISCHARGE)) //黑屏的情况下
									{
										BL_ON();//打开显示屏
									}								
									
								}
							}
						}
					}
				}								
			}
//			Send_Data(byWireLessRecBuff,byWireLessRecLen);
			halSpiStrobe(CCxxx0_SRX);	//允许接收
			//CCF1=0;     //不清此中断，以免在此时间段内有中断产生不能再次触发中断
			CCAPM1 |= PCA1_ECCF;	//开启中断
		
		}
#endif
		
		if(bDispBedFace==1)
		{
			bDispBedFace =0;
			if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)
			{				
				page =1;
				ShowItem(BED_NUMBER);	//显示床号
				ShowPatientBaseInfo();		//显示病人基本信息
				ShowItem(OX_START);
				ShowItem(OX_END);
				ShowItem(OX_SUBTOTAL);	
				ShowItem(OX_TOTAL);
				Set_VisualPage(1);
				stLocalControl.byDisplayFace = BED_INFO_FACE;


				for(i=0;i<5;i++)
				{
					SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);	//拷贝至另一备份区				
					if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)==1)  break;
						
				}
				
			}
			else
			{
				stLocalControl.uiBedFaceDataLen =0;			
				Write_Cont(BED_FACE_DATA_LENGTH_ADDR,&(stLocalControl.uiBedFaceDataLen),2);	//将数据长度清0
				Write_Cont(BED_FACE_DATA_BAK,&(stLocalControl.uiBedFaceDataLen),2);
				
			}
		}

		if(bDispBedForm ==1)
		{
			bDispBedForm =0;
			if(StoreDataCheck(BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+2))	//将数据长度也包含进去
			{
				ShowBedFaceForm();	
				ShowItem(BED_NUMBER);
				Read_Cont((uint8*)&(stLocalControl.uiBedFaceDataLen),BED_FACE_DATA_LENGTH_ADDR,2);	//读床头卡数据总个数
				if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)) //将数据长度也包含进去
				{
					ShowPatientBaseInfo();	

					ShowItem(OX_START);
					ShowItem(OX_END);
					ShowItem(OX_SUBTOTAL);	
					ShowItem(OX_TOTAL);					
					if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)!=1)
					{//如果备份数据区不正确
						for(i=0;i<5;i++)
						{
							SectorDataCopy(BED_FACE_DATA_BAK,BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+3);	//拷贝至另一备份区				
							if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2)==1)	break;	
						}		
					}
				}
				else
				{//主数据区不正确
					if(StoreDataCheck(BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+2))
					{//备份区正确
						while(1)
						{
							SectorDataCopy(BED_FACE_DATA_LENGTH_ADDR,BED_FACE_DATA_BAK,stLocalControl.uiBedFaceDataLen+3);	//拷贝至主数据区				
							if(StoreDataCheck(BED_FACE_DATA_LENGTH_ADDR,stLocalControl.uiBedFaceDataLen+2)==1)  break; 				
						}
						ShowPatientBaseInfo();
						ShowItem(OX_START);
						ShowItem(OX_END);
						ShowItem(OX_SUBTOTAL);	
						ShowItem(OX_TOTAL);						
					}
					else 	//主数据区不正确，备份数据区也不正确。
					{
					}
				}
		
				Set_VisualPage(1);

				for(i=0;i<5;i++)
				{
					SectorDataCopy(BED_FACE_FORM_BAK,BED_FACE_FORM_LENGTH_ADDR,stLocalControl.uiBedFaceFormLen+3);	//拷贝至另一备份区				
					if(StoreDataCheck(BED_FACE_FORM_BAK,stLocalControl.uiBedFaceFormLen+2)==1)  break;
						
				}				
			}
			else
			{
				stLocalControl.uiBedFaceFormLen =0;			
				Write_Cont(BED_FACE_FORM_LENGTH_ADDR,&(stLocalControl.uiBedFaceFormLen),2);	//将数据长度清0
				Write_Cont(BED_FACE_FORM_BAK,&(stLocalControl.uiBedFaceFormLen),2);	//将数据长度清0
			}
				
		}
	}
} 
/**********************************************************
*函数名称			:main	
*函数描述        	:系统主函数,整个软件的入口
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
void main(void)
{  	
	//RD_ID();
	P4SW=0X70;
	EPH1660_CS=0;	//禁止
	Delayms(200);
	//操作系统初始化	
	OSInit();

	led0=1;
	//创建系统主线程
	OSTaskCreate(MainTask, NULL, 0); 
	
	while(1)
    {
		_nop_();
 //      PCON = PCON | 0x01;                    //CPU进入休眠状态	    
    }	 
}
