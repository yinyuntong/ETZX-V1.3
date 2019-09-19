/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:main.c
*文件描述    		:主程序
*创建人     		:尹运同
*创建日期   		:2009-4-9
*版本号       		:V1.0
*注释	     		:					
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define _IN_MAIN_
#include "config.h"

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值

//动态管理内存大小定义
#define MALLOC_MEM_SIZE		300
uint8 			xdata 			byMallocMem[MALLOC_MEM_SIZE];
//变量定义
STLocalControl  xdata 			stLocalControl;
Queue DataQueue;//定义一个数据队列

void LedInit(void);
void Delayms(uint16 ms);


bit bLedState=0;
bit bPowerEnState;
bit	bPCState=0;
bit PowerSwPrePress=0;
bit PowerSwPressed=0;
bit PowerSwPreUp=1;
bit PowerSwUped=1;

bit bKeyDownTreat=0;


uint8 mSecond=0;
uint8 Second=0;

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
//	CMOD	&=	!ECF;			//禁止 PCA计数器溢出中断   
//	CCAPM0	&=	!PCA0_ECCF;		//禁止CCF0中断
//	CCAPM1	&=	!PCA1_ECCF;		//禁止CCF1中断	
//}

void SpiInt(void) interrupt SPI_INTNO		//9号中断
{
	IE2 &= ~ESPI;
}

#endif


void Mp3Rst(void)
{
	MP3_RST =1;		//高电平复位
	Delayms(1000);
	MP3_RST =0;	
}


/*---------------------------------------------------------------------------
函数原型: void Delayms(uint16 ms)
参数说明: ms--需要延时的值
返 回 值: 无
函数功能：延时程序(对于18.432M晶振单指令周期延时1mS)
----------------------------------------------------------------------------*/
void Delayms(uint16 ms)	  
{
   uint16 xdata i;
   for(;ms!=0;ms--)
		for(i=900;i!=0;i--);
}


uint8 ParaCrcCheck(uint16 addr)
{
	uint8 crcResult=0;
	uint8 crcData =0;
	uint8 i;
	uint8 paraData;
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		paraData = IapReadByte(addr);
		crcResult +=paraData; /*IapReadByte(addr)*/;
		addr++;
	}
	crcData = IapReadByte(addr);
	DisableIAP();
	
	if(crcData == crcResult) return 1;
	else return 0;
}



uint8 CalcParaCrc(void)
{
	uint8 crcResult=0;
	uint8 i;
	uint8 *addr= &(stLocalControl.stEepromCfgData.byInitFlag);
	EnableIAP(IAP_READ);
	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
	{
		crcResult += *addr;
		addr++;
	}
	DisableIAP();
	return crcResult;
}



/**********************************************************
*函数名称			:ReadParameter	
*函数描述        	:将Flash中的配置数据读入到内存中
*输入参数   		:
*返回值				:
*全局变量			:stLocalControl
*调用模块  			:EnableIAP,IapReadByte,DisableIAP
***********************************************************
*创建人	      		:尹运同
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void ReadParameter(uint16 addr)
{

	uint8 i=0;
	uint8 *pbyData = &(stLocalControl.stEepromCfgData.byInitFlag);
	
	EnableIAP(IAP_READ); 
	for(i=0;i<sizeof(STEepromCfgData);i++)
	{
		*pbyData = IapReadByte(addr);
		pbyData++;
		addr++;
	}
	DisableIAP();

}


void SaveParameter(uint16 addr)
{
	uint8 i;
	uint16 addr_temp=addr;
	uint8 *pbyData;
	uint8 readtimes;
	uint8 writetimes;
	uint8 crc_temp;

	stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();
	WDT_CONTR = 0x3d;	//喂狗
	writetimes =3;
	do
	{
		EnableIAP(IAP_ERASE);
		IapErase(addr); 

		
		EnableIAP(IAP_WRITE); 
		addr = addr_temp;
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}

		//地址紧跟前一数据区
		pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
		for(i=0;i<sizeof(STEepromCfgData);i++)
		{
			IapWriteByte(addr,*pbyData);
			pbyData++;
			addr++;
		}
		

		EnableIAP(IAP_READ);
		readtimes=3;
		do
		{
			pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			addr = addr_temp;


			for(i=0;i<(sizeof(STEepromCfgData))*2;)
			{
				if(*pbyData != IapReadByte(addr)) break;
				
				pbyData++;
				addr++;
				i++;
				if(i== sizeof(STEepromCfgData)) pbyData=&(stLocalControl.stEepromCfgData.byInitFlag);
			}	

			if(i>= (sizeof(STEepromCfgData)*2))
			{//写进去的数据与读出来的数据相同
				break;
			}
		
		}while(--readtimes);

		if(readtimes)
		{
			break;	//相同
		}
	}while(--writetimes);

	
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
*创建人	      		:尹运同
*创建日期	 		:2009-4-6
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void ParameterInit(void)
{
	uint8 flagData;
	uint8 readtimes=3;
CheckIapSec0:	

	do
	{

		if(ParaCrcCheck(IAP0_ADDR) ==1)  break;
			

	}while(--readtimes);


	if(readtimes)
	{//第一块校验和正确

		EnableIAP(IAP_READ);
		flagData = IapReadByte(IAP0_ADDR);
		DisableIAP();
		
		if(flagData == INIT_FLAG)
		{//数据已经初始化完毕了,读取数据参数
			ReadParameter(IAP0_ADDR);
			return;
		}
		else	goto CheckIapSec1;	
	}

	else
	{
CheckIapSec1:	
		readtimes=3;	
		do
		{
			if(ParaCrcCheck(IAP0_ADDR+sizeof(STEepromCfgData)) ==1)  break;
				
		}while(--readtimes);
	
	
		if(readtimes)
		{//第二块校验和正确
			EnableIAP(IAP_READ);
			flagData = IapReadByte(IAP0_ADDR+sizeof(STEepromCfgData));
			DisableIAP();
			
			if(flagData == INIT_FLAG)
			{//数据已经初始化完毕了,读取数据参数
				ReadParameter(IAP0_ADDR+sizeof(STEepromCfgData));
				SaveParameter(IAP0_ADDR);
				return;
			}
			else	goto UseInitValue;	
		}
		else
		{//第一块和第二块存储区3次读取都失败或者未初始化，应用初始值，但不保存到内部EEPROM中去
UseInitValue:
			stLocalControl.stEepromCfgData.byInitFlag = INIT_FLAG;
			stLocalControl.stEepromCfgData.bySelfSecAddr = 0x01;
			stLocalControl.stEepromCfgData.bySelfRoomAddr = GLZJ_ADDR2;
			stLocalControl.stEepromCfgData.bySelfBedAddr = GLZJ_ADDR3;
			stLocalControl.stEepromCfgData.uiRs485Brt	 = 57600;		//默认波特率57600
			stLocalControl.stEepromCfgData.byCrc = CalcParaCrc();

		}	
	}
	DisableIAP();
}


void PCATimerInt(void) interrupt 7
{
	EA=0;

#if STC12C5A==1
		uiIsrTimerCount+=ISR_INC_COUNT;
		CCAP0L = (uint8)uiIsrTimerCount;
		CCAP0H = (uint8)(uiIsrTimerCount>>8);
		CCF0=0;
#endif
	
	
#if USE_COM0==1
		if(stUsartCfg[0].byRecTimeout) 
		{
			stUsartCfg[0].byRecTimeout--;
		}
#endif
	
#if USE_COM1==1
		if(stUsartCfg[1].byRecTimeout) 
		{
			stUsartCfg[1].byRecTimeout--;
			if(stUsartCfg[1].byRecTimeout ==0)
			{
				stUsartCfg[1].byRecState=0;
				stUsartCfg[1].uiRecPosition=0;		
			}
		}
#endif


	TimerOutDeal();

	if(POWER_SW ==0)
	{
		if(PowerSwPrePress==0)   PowerSwPrePress=1;//按键初步按下
		else 
		{//已初步按下
			if(PowerSwPressed==0)
			{
				PowerSwPressed=1;
				bKeyDownTreat=1;    //按键消抖后按下

				PowerSwPreUp=0;
				PowerSwUped=0;
			}
			else
			{//按键已按下
			}
		}
	}


	else
	{
		if(PowerSwPreUp==0)  PowerSwPreUp=1;
		else 
		{//按键已初步弹起
			if(PowerSwUped==0)
			{
				PowerSwUped=1;
				
				PowerSwPrePress=0;
				PowerSwPressed=0;				
			}
			else 
			{//按键消抖后已弹起

			}
		}
	}

	if(++mSecond == 50)
	{
		mSecond=0;
		if(++Second ==30)
		{
			Second=0;
			DTimeState |=bDataTimeUpdate;
						
		}
	}
	EA=1;
}

/**********************************************************
*函数名称			:IoInit	
*函数描述        	:系统IO口初始化
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
void IoInit(void)
{
	//IO口配置
	//STC12C52/5A系列:M0=0,M1=0:准双向，M0=0,M1=1:高阻，M0=1,M1=0:推挽，M0=1,M1=1:开漏
	
//KBC,KBD 推挽输出
	P5M0 |= Bin(0,0,0,0,0,0,1,1);
	P5M1 &= (~Bin(0,0,0,0,0,0,1,1));

//KGB推挽输出
	P2M0 |= Bin(0,0,1,0,0,0,0,0);
	P2M1 &= (~Bin(0,0,1,0,0,0,0,0));

	
	KBC	=0;
	KBD =0;
	KGB =0;
	RE0 =1;			//禁止RS485接收
//	DE0 =0;			//禁止RS485发送
	DE0 =1;
	SS	=1;

//P5.3推挽输出		用于控制喇叭时的负载
	P5M0 |=   Bin(0,0,0,0,1,0,0,0);
	P5M1 &= (~Bin(0,0,0,0,1,0,0,0));
	GB_FZ=0;

//p1.7 推挽输出            用于24V电源控制
	P1M0 |=   Bin(1,0,0,0,0,0,0,0);
	P1M1 &= (~Bin(1,0,0,0,0,0,0,0));
	POWER_EN=1;			//24v电源打开
	bPowerEnState=1;		//24V电源状态标志置1	


	POWER_SW = 1;											//按键设置成弱上拉
}
/**********************************************************
*函数名称			:Init	
*函数描述        	:软件数据初始化函数
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
void Init(void)
{	
	//状态初始化
	memset(&(stLocalControl.byWaitAckTimeLanding),0x00,sizeof(STLocalControl));
	
	stLocalControl.uiDevState = FLAG_LANDING;	
	stLocalControl.stIndicationData.pstNext = NULL;
	stLocalControl.stVoicePlay.byISD1700State = 0x00;

#if	STC12C5A==1
//	CMOD = 0x02;	//PCA 定时FOSC/2 
	CMOD = 0x00;	//PCA 定时FOSC/12 
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块0为16位软件定时器,ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;
	
	//启动PCA计数器计数
	CR = 1;	

	//复位看门狗                                                                                                                                                                                                                                                                                     
//	WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M
#endif
	//IO口初始化
	IoInit();

	//mp3_rst mp3控制芯片初始化
	Mp3Rst();
				
	//电源灯、状态灯初始化
	LedInit();
	//单总线初始化		
	SingleBusInit();
	EA=1;
	Delayms(200);
	//其他参数初始化
	ParameterInit();

	//串口初始化
	Usart0Init(stLocalControl.stEepromCfgData.uiRs485Brt);
	Usart1Init();

	//OSMemInit(byMallocMem,MALLOC_MEM_SIZE);
	init_mempool(byMallocMem,MALLOC_MEM_SIZE);  	//初始化内存堆
	//链表初始化
	stLocalControl.stCallPointer.pstHead = 
	stLocalControl.stCallPointer.pstTail = NULL;

//	AddCallNod((pSTBusFreq)(&(stLocalControl.stBusFreq.bySndSecAddr)));


	MakeCH0TimerOut(1, 0);

	stLocalControl.byChar =0x00;	//系统忙
	Usart1SndCmd(CMD_SYSTEM_STATE,1,&(stLocalControl.byChar));	//报告系统忙状态



 //复位看门狗																																																																					  
 	WDT_CONTR = 0x3e;		 //看门狗定时2.5s@20M

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
*创建日期	 		:2009-1-4
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
int main(void)
{	
	P4SW = (P44EN|P45EN|P46EN);	//用作IO 口
	Delayms(100);

	//系统硬件初始化
	Init();	

	P8563_Read();
	byMinute=stTime.byMinute;
	stLocalControl.stBusFreq.bySndSecAddr = stTime.bySecond;
	stLocalControl.stBusFreq.bySndRoomAddr= stTime.byMinute;
	stLocalControl.stBusFreq.bySndBedAddr =	stTime.byHour;
	stLocalControl.stBusFreq.byCmd		=CMD_DATE_SEND;
	stLocalControl.stBusFreq.byRecSecAddr = stTime.byDay;
	stLocalControl.stBusFreq.byRecRoomAddr= stTime.byMonth;
	stLocalControl.stBusFreq.byRecBedAddr =	stTime.byYear;
	Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

	while(1)
	{
		//复位看门狗                                                                                                                                                                                                                                                                                     
		WDT_CONTR = 0x3e; 		//看门狗定时2.5s@20M       
		if(bUsart1RecFinish) Usart1RecDeal();

		if(bBus0RecFinish)									//总线0收到数据
		{ 	
			Bus0RecDeal();
		}
		
		if(bBus0SendFinish)									//总线0发送完数据帧
		{
			Bus0SendFinishDeal();		
		} 
		if(bBus1RecFinish)									//总线1收到数据
		{ 	
			Bus1RecDeal();	
		}
		if(bBus1SendFinish)									//总线1发送完数据帧
		{
			Bus1SendFinishDeal();		
		} 		

		if(bKeyDownTreat)
		{
			bKeyDownTreat=0;
			KeyDown();
		}

		if(DTimeState&bCH0TimerOutDeal)
		{
			DTimeState &= ~bCH0TimerOutDeal;
			if(stLocalControl.uiDevState & FLAG_LANDING)
			{	//上电登记状态，复位所有分机						
				stLocalControl.stBusFreq.bySndSecAddr = 0x00;
				stLocalControl.stBusFreq.bySndRoomAddr = 0x00;
				stLocalControl.stBusFreq.bySndBedAddr = 0x00;
				stLocalControl.stBusFreq.byCmd = CMD_POWER_ON;
				stLocalControl.stBusFreq.byRecSecAddr = 0xff;
				stLocalControl.stBusFreq.byRecRoomAddr = 0xff;
				stLocalControl.stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));

				Usart1SndCmd(CMD_LANDING,0x00,NULL);
				stLocalControl.byWaitAckTimeLanding = 250;	//等待5S
				
			}
			else if(stLocalControl.uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN|
				FLAG_TALK|FLAG_BROADCAST))
			{	//等待应答超时到
				if((stLocalControl.uiDevState & (FLAG_WAIT_ACK|FLAG_WAIT_LISTEN))&&
					((byUsart1RecBuf[CMD_POSITION] ==CMD_INFUSION_ANSWER)||(byUsart1RecBuf[CMD_POSITION] ==CMD_COMM_CALL)) )
				{//如果在等待应答或者等待接听状态下,且串口中的命令为处理输液呼叫或普通呼叫,将发操作不成功码
					Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);
					byUsart1RecBuf[CMD_POSITION] =0x00;		//清空此数据
					
				}				
				stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stCallAddr.bySndSecAddr;
				stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stCallAddr.bySndRoomAddr;
				stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stCallAddr.bySndBedAddr;
				stLocalControl.stBusFreq.byCmd = CMD_SYSTERM_RESET;
				stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stCallAddr.byRecSecAddr;
				stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stCallAddr.byRecRoomAddr;
				stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stCallAddr.byRecBedAddr;;					
				Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
			}
			else if(stLocalControl.uiDevState & FLAG_INDICATION)
			{
/*					//向语音模块发送停止报号命令
				stLocalControl.stBusFreq.byCmd = CMD_STOP_VOICE_PLAY;
				Bus1OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));
				if(0x00 == (stLocalControl.uiDevState & FLAG_STOP_EXIST))
				{	//没有正在发送停止命令
//						stLocalControl.stVoicePlay.byISD1700State |= ISD_TIME_OVER;

					stLocalControl.stBusFreq.byCmd = CMD_STOP_INDICATION;						
					if(true == Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)))
					{
						stLocalControl.uiDevState |= FLAG_STOP_EXIST;
					}						
				}

				*/
			}				
		}
		if(DTimeState &bCH1TimerOutDeal)
		{
		    DTimeState &= ~bCH1TimerOutDeal;
			stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stCH1CallAddr.bySndSecAddr;
			stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stCH1CallAddr.bySndRoomAddr;
			stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stCH1CallAddr.bySndBedAddr;
			stLocalControl.stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
			stLocalControl.stBusFreq.byRecSecAddr = stLocalControl.stCH1CallAddr.byRecSecAddr;
			stLocalControl.stBusFreq.byRecRoomAddr = stLocalControl.stCH1CallAddr.byRecRoomAddr;
			stLocalControl.stBusFreq.byRecBedAddr = stLocalControl.stCH1CallAddr.byRecBedAddr;			
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr));		
		}

		if(DTimeState & bDataTimeUpdate)
		{
			DTimeState &= ~bDataTimeUpdate;
		 	P8563_Read();
			if(byMinute!=stTime.byMinute)
			{
				stTime.bySecond &= 0x7f;	//bit7
				if(stTime.bySecond < 0x60)  
				{
					stTime.byMinute &= 0x7f;	//bit7
					if(stTime.byMinute < 0x60)  
					{
						stTime.byHour &= 0x3f;		//bit7 bit6
						if(stTime.byHour < 0x24)    
						{
							stTime.byDay &= 0x3f;		//bit7 bit6
							if(stTime.byDay < 0x32)   	
							{							
								stTime.byMonth &= 0x1f;		//bit7... bit5
								if(stTime.byMonth < 0x13)  		
								{
									if(stTime.byYear <= 0x99)  
									{
										byMinute=stTime.byMinute;
										stLocalControl.stBusFreq.bySndSecAddr = stTime.bySecond;
										stLocalControl.stBusFreq.bySndRoomAddr= stTime.byMinute;
										stLocalControl.stBusFreq.bySndBedAddr = stTime.byHour;
										stLocalControl.stBusFreq.byCmd		=CMD_DATE_SEND;
										stLocalControl.stBusFreq.byRecSecAddr = stTime.byDay;
										stLocalControl.stBusFreq.byRecRoomAddr= stTime.byMonth;
										stLocalControl.stBusFreq.byRecBedAddr = stTime.byYear;
										Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)); 
									}
								}
							}
						}
					}
				}
			}		
		
		}

		if(DTimeState &bWaitCommAckDTime)
		{
			DTimeState &= ~bWaitCommAckDTime;
			stLocalControl.uiDevState &= ~(FLAG_WAIT_COMM_ACK);
			Usart1SndResult(byUsart1RecBuf[CMD_POSITION],FALSE);
	   	
		}

		if(DTimeState & bWaitRS485AckDTime)
		{
			DTimeState &= ~bWaitRS485AckDTime;
			Usart1SndRS485Result(byUsart0SndBuf[CMD_POSITION],FALSE);
		}

	   	if(DTimeState &bWaitAckTimeLand)
		{	
			DTimeState &= ~bWaitAckTimeLand;
			stLocalControl.uiDetectPcTime = 30000;	//侦听PC机间隔10分钟
			stLocalControl.stBusFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusFreq.byCmd = CMD_PC_CLOSE;
			stLocalControl.stBusFreq.byRecSecAddr = 0xff;
			stLocalControl.stBusFreq.byRecRoomAddr =0xff;
			stLocalControl.stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stLocalControl.stBusFreq.bySndSecAddr)); 		
		}

		if(DTimeState & bDetectPcTime)
		{
			DTimeState &= ~bDetectPcTime;
			Usart1SndCmd(CMD_LANDING,0x00,NULL);
			stLocalControl.byWaitAckTimeLanding = 250;	//等待5S	
		}

		
		if(DTimeState & bUsart1SdResultDTime)
		{
			DTimeState &= ~bUsart1SdResultDTime;
			Usart1SndRS485Result(byUsart0SndBuf[CMD_POSITION],TRUE);	//发送结果码
		}	


		if(bRS485DataTreat)
		{
			if(stUsartCfg[0].uiSndLen==0) 
			{//RS485没有数据正在发送
				pSTUartFreq pstDataFreq=byUsart1RecBuf;
				if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_DISPLAY_FORM))
				{
					DE0=1;
//					Delayms(1);
					EA=0;
					memcpy(byUsart0SndBuf,byUsart1RecBuf,pstDataFreq->uiLen+EXT_SIZE);
					EA=1;

					pstDataFreq=byUsart0SndBuf;	
					stUsartCfg[0].pbySndBuf = byUsart0SndBuf;
					stUsartCfg[0].uiSndLen = pstDataFreq->uiLen+EXT_SIZE;
					stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	
					TI=0;
					SBUF= stUsartCfg[0].pbySndBuf[stUsartCfg[0].uiSndPos];

					if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA))
					{
						//保存分机地址					
						stLocalControl.stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
						stLocalControl.stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
						stLocalControl.stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

						//设置等待应答超时2S  在波特率为57.6K的情况下,100字节数据只要17.3ms
						stLocalControl.byWaitRS485AckDTime = 95;		//等待1.9S	

					}
				}
				bRS485DataTreat =0;
			}
		}
	}	
}
	
	
	
void LedStateFlash(void)
{
	if(bLedState==0)
	{
		LED_STATE=1;
		bLedState=1;
	}
	else
	{
		LED_STATE=0;
		bLedState=0;
	}
}

 void LedInit(void)
{
 
	 LED_STATE=0;	//状态指示灯打开
	 bLedState=0;

	 LED_POWER=0;	//24v电源指灯打开
}

