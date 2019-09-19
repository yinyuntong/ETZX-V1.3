/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:main.c
*文件描述    		:主函数
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
#define  _IN_MAIN_
#include "config.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值
STLocalControl xdata stLocalControl;
uint8 xdata TelNum[TEL_NUM_LEN+1]={0,0,0,0,0,0,0,0,0,0,0,0};


uint8 xdata byRecPosition=0;
uint8 xdata byRecTimeout=0;
uint8 xdata byUsart0RecBuf[UART0_REC_LEN];

bit bUartRecFinished =0;
bit bTimerOutDeal=0;
uint8 xdata RePeat =0;
uint8 xdata RecGsmDataResult=0;			//接收GSM数据的结果
/**********************************************************
*函数名称			:delay_nus	
*函数描述        	:纳秒级延时操作
*输入参数   		:i:延时的us数
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
void delay_nus(unsigned char i)
{
 	unsigned char j;

 	for(;i>0;i--)
 	{
 		for(j=2;j>0;j--);
 	}
	 
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




void UsartInit(void)
{
	   SCON	=	0X50;	   //工作方式1      8位可变波特率
	   
	   TMOD	&=	0X0F;	   //TO方式不变
	   TMOD	|=	0X20;	   //;t1 方式2作波特率发生器	//8位自动重载
	
	
	   TL1 =   0XF6;	   //波特率4800  在18.432M晶振的情况下
//	    TL1 =   0XFC;	   //波特率9600  在18.432M晶振的情况下	
	   TH1 =   TL1;
//	   PCON    &=  ~SMOD;	//	;波特率倍增选择 	   smod=0
		PCON	|=	SMOD;	//	;波特率倍增选择 	   smod=1

	// AUXR    &=  0xBf;	   //传统12分频速度
	   AUXR |= T1x12;	   //1T模式,最后波特率为:4800*12*2=115200
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=1;
	   ES=1;   
}



void UsartInit_work(void)
{
	   SCON	=	0X50;	   //工作方式1      8位可变波特率
	   
	   TMOD	&=	0X0F;	   //TO方式不变
	   TMOD	|=	0X20;	   //;t1 方式2作波特率发生器	//8位自动重载
	
//	   TL1 =   0Xd8;	   //波特率19200/12   在18.432M晶振的情况下	
	    TL1 =   0XF6;	   //波特率57600/12  在18.432M晶振的情况下
//	    TL1 =   0XFC;	   //波特率115200/12  在18.432M晶振的情况下	
	   TH1 =   TL1;
	   PCON    &=  ~SMOD;	//	;波特率倍增选择 	   smod=0
	   
	// AUXR    &=  0xBf;	   //传统12分频速度
	   AUXR |= T1x12;	   //1T模式
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=1;
	   ES=1;   
}


void UartISR(void ) interrupt UART_INTNO
{
	uint8 RecData;
	if(RI)
	{
		RI=0;
		RecData=SBUF;
				
		if(byRecPosition!=0) //不是起始字节
		{
			if(byRecTimeout==0)
			{//超时到了
				byRecPosition=0;
				return;
			}
		}
		byRecTimeout=4;	//又开始定时80ms     每字节最大间隔80ms,超时即为另帧数据
		byUsart0RecBuf[byRecPosition++] = RecData;	
		if(byRecPosition>=UART0_REC_LEN)
		{
			byRecPosition=0;
			byRecTimeout=0;
			memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
		}
	}
	if(TI)
	{
		TI=0;
	}
}


void UartSendByte(uint8 Data)
{ 
    ES=0;
   	SBUF=Data;
   	while(!TI);
	TI=0;
	ES=1;
}


/***********************************************************/
void Send_Data(uint8 *Databuf,uint8 l)
{ 
    uint8 i;
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


void UartSendString(uint8 *Databuf)
{
	ES=0;
	while(*Databuf	!=	0)   //'\0'
	{
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;		
	}
	ES=1;	
}


void IOInit(void)
{
	//总线1发送脚设置为开漏输出，接收脚设置为高阻输入
	//P3^3,高阻输入
	P3M0 |= ( Bin(0,0,0,0,1,0,0,0));
	P3M1 &= (~Bin(0,0,0,0,1,0,0,0));


	// P1^3   GSM_RST 开漏输出
	P1M0 |=  (Bin(0,0,0,0,1,0,0,0));
	P1M1 |=  (Bin(0,0,0,0,1,0,0,0));
	GSM_RST =1;	//高阻s
	GSM_ON_OFF =1;

	//P1^6,P1^7 推挽输出
	P1M0 &=  (~(Bin(1,1,0,0,0,0,0,0)));
	P1M1 |=    (Bin(1,1,0,0,0,0,0,0));	
	XTA =0;
	XTD =0;
}



//出参：找到的数据在byUsart0RecBuf[]中的位置+1
uint8 UartRecTreat(uint8* pCmd,uint8 byCmdLen)	
{
	uint8 i,j,k;
	ES=0;
	for(i=0;i<byRecPosition;i++)
	{

		if(byUsart0RecBuf[i]==*pCmd)
		{//第1个相符
			if(byCmdLen==1)
			{//长度为1，直接退出
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;
				return(i+1);	//使之为一个不为0的数					
			}
			
			if((i+byCmdLen)>byRecPosition)	
			{//超过缓冲区
				byRecPosition=0;
//;				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;			
				return 0;
			}
			
			//后续没有超出接收缓冲区
			k=1;
			for(j=i+1;j<byCmdLen+i;j++)
			{
				if(byUsart0RecBuf[j]!= *(pCmd+k))	break;	//不相等，退出内循环
				k++;
			}
			
			if(j>=byCmdLen+i)
			{//全部数据相符
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;			
				return(i+1);
			}
			//不相符从下一个位置开始查找
		}
	}

	byRecPosition=0;
//	memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
	ES=1;
	return 0;	
}



//过程函数，用于UartRecTreat3（）
uint8 UartRecTreat2(uint8* pCmd,uint8 byCmdLen)	
{
	uint8 i,j,k;
	ES=0;
	for(i=0;i<byRecPosition;i++)
	{

		if(byUsart0RecBuf[i]==*pCmd)
		{//第1个相符
			if(byCmdLen==1)
			{//长度为1，直接退出
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;
				return(i+1);	//使之为一个不为0的数					
			}
			
			if((i+byCmdLen)>byRecPosition)	
			{//超过缓冲区
//				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
//				ES=1;			
				return 0;
			}
			
			//后续没有超出接收缓冲区
			k=1;
			for(j=i+1;j<byCmdLen+i;j++)
			{
				if(byUsart0RecBuf[j]!= *(pCmd+k))	break;	//不相等，退出内循环
				k++;
			}
			
			if(j>=byCmdLen+i)
			{//全部数据相符
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;			
				return(i+1);
			}
			//不相符从下一个位置开始查找
		}
	}

//	byRecPosition=0;
//	memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
//	ES=1;
	return 0;	
}



uint8 UartRecTreat3(void)		//从收到的一帧数据中查找是否有关键字符
{

	ES=0;
	if(UartRecTreat2("+CPIN: READY",(uint8)strlen("+CPIN: READY")/*12*/))
	{
		return CPIN_READY;
	}
	else if(UartRecTreat2("+COPS: 0,0,\"CHINA MOBILE\"",/*(uint8)strlen("+COPS: 0,0,\"CHINA MOBILE\"")*/24))
	{
		return COPS_OK;
	}
	else if(UartRecTreat2("^CONF: 1",(uint8)strlen("^CONF: 1")))
	{
		return GSM_CALL_OK;
	}
	else if(UartRecTreat2("^CONN: 1,0",(uint8)strlen("^CONN: 1,0")))
	{
		return TEL_HANDLE;
	}

	else if(UartRecTreat2("^CEND: 1,0, 67, 21\r\n\r\nNO CARRIER",(uint8)strlen("^CEND: 1,0, 67, 21\r\n\r\nNO CARRIER")))
	{//移动电话按了忽略键，约17秒后收到  此帧数据
		return TEL_IGNORE;
	}

	else if(UartRecTreat2("^CEND: 1,0, 67, 17\r\n\r\nBUSY",(uint8)strlen("^CEND: 1,0, 67, 21\r\n\r\nBUSY")))
	{//移动电话按了忙键，或者有其它电话正拨打，或者正在通话中，约18秒后收到  此帧数据
		return TEL_BUSY;
	}
	else if(UartRecTreat2("NO CARRIER",(uint8)strlen("NO CARRIER")))
	{//电话接通之后的挂机，收到的字束为"^CEND: 1,9,67,16\r\n\r\nNO CARRIER",因第二位数字不定，有时为9，有时间为12，有时为61
//GSM_STATE_LED = LED_ON;
		return TEL_HAND_UP;
	}
	else if(UartRecTreat2("^CEND:1,0,48",(uint8)strlen("^CEND:1,0,48")))
	{
		return TEL_NO_HANDLE;
	}
	else if(UartRecTreat2("OK",(uint8)strlen("OK")))
	{
		return COMM_OK;
	}
	else 
	{
		byRecPosition=0;
		ES=1;
		return 0;
	}
	
}




void PCA0TimerISR(void) interrupt PCA_LVD_INTNO	//20ms定时
{
	if(CCF0 ==1)
	{
		CCF0=0;
		WDT_CONTR = 0x3a; 	//喂狗
		uiIsrTimerCount+=ISR_INC_COUNT;
		CCAP0L = (uint8)uiIsrTimerCount;
		CCAP0H = (uint8)(uiIsrTimerCount>>8);	

		bTimerOutDeal =1;

		if(byRecTimeout)
		{
			if(--byRecTimeout ==0)
			{//一帧数据接收完成
				bUartRecFinished=1;
				//GSM_STATE_LED = LED_ON;
			}
		}
	}
	else if(CCF1 ==1)
	{
		CCF1=0;
	}
	else if(CCF2 ==1)
	{
		CCF2=0;
	}
	else if(CCF3 ==1)
	{
		CCF3=0;
	}

}





/**********************************************************
*函数名称			:Init	
*函数描述        	:硬件初始化操作
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
void Init(void)
{
	uint16 i;

/*	char *string1 = "abcdefghijklmnopqrstuvwxyz";
	char *string2 = "onm";
	char *ptr;
	char *str;
*/

//	CMOD = 0x02;	// 2分频
	CMOD =PCACLK0;	//00 ==PCA时钟12分频
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//设置PCA模块0为16位软件定时器，ECCF0=1允许PCA模块0中断
	CCAPM0 = 0x49;	
	//开PCA中断和LVD(低压检测)中断共享的总中断控制位
	EPCA_LVD = 1;
	//启动PCA计数器计数
	CR = 1;	

	IOInit();
	GSMInit();
	UsartInit();

/*	while(1)
	{
		Delayms(500);
		Send_Data_Byte(0X55);
	}*/

	//单总线初始化 
	SingleBusInit(); 
	
//	//复位看门狗
//	WDT_CONTR = 0x3a; 		
	//参数初始化
	InitParameter();


	stLocalControl.stBusDealFreq.bySndSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr  = stLocalControl.stEepromCfgData.bySelfBedAddr;
	stLocalControl.stBusDealFreq.byCmd         = CMD_LANDING;
	stLocalControl.stBusDealFreq.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr  = GLZJ_ADDR;
	stLocalControl.stBusDealFreq.byRecBedAddr   = 0x00;
	Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

	stLocalControl.byLedFlashTime = 50;
	stLocalControl.byLedTimeSet   = 50;

	byDevState1 = 0x00;
	byDevState2 = 0x00;
	byDevState2 = 0x00;



//	bLanding = 1;
	SYS_STATE_LED = LED_ON;
	bLanding = 0;		//管理主机不发送上电登记命令
	EA=1;


RE_GSM_UART_AUTOBRT_SET:	
	GSMUartAutoBrtSet();	//设置自适应波特率
	i=100;		//延时2S
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}
	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",2))
		{//串口正常
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;									
			Delayms(1000);
			goto RE_GSM_UART_AUTOBRT_SET;		
		}		
	}
	else
	{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_UART_AUTOBRT_SET;	
	}	

	
	TR1=0;
 	UsartInit_work();
	Delayms(1000);


RE_AFFIRM_UART_COM:
	AffirmUartCom();
	i=100;		//延时2S
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}
	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",2))
		{//串口正常
//			GSM_STATE_LED = LED_ON;

		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_AFFIRM_UART_COM;		
		}		
	}
	else
	{
//			GSM_STATE_LED = LED_OFF;	
			Delayms(1000);
			goto RE_AFFIRM_UART_COM;	
	}


RE_LOOP_DISP_OFF:
	LoopDispOff();	//关闭回显
	i=100;		//延时2S
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}
	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",2))
		{//串口正常
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_LOOP_DISP_OFF;		
		}		
	}
	else
	{
//			GSM_STATE_LED = LED_OFF;	
			Delayms(1000);
			goto RE_LOOP_DISP_OFF;	
	}	
	

RE_SIM_STATE_QUERY:
	SIMStateQuery();
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("+CPIN: READY",(uint8)strlen("+CPIN: READY")/*12*/))
		{//串口正常
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_SIM_STATE_QUERY;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_SIM_STATE_QUERY;
	}






RE_GSM_SWPATH_SET:
	GSMSWPathSet(0x00);	//当前语音通道设置为第一路
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//串口正常
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_SWPATH_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_GSM_SWPATH_SET;
	}


RE_ECHO_REST_SET:
	EchoRestrainSet(0x01);	//打开回声抑制功能 不产生嘶叫
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//串口正常
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_ECHO_REST_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_ECHO_REST_SET;
	}


RE_GSM_SPK_VOL_SET:
	GSMSPKVoiceSet(0x01);	//设置SPK音量       5:为最大音量
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//串口正常
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_SPK_VOL_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_GSM_SPK_VOL_SET;
	}

RE_GSM_MIC_GAIN_SET:
	GSMMICGainSet(0x00);	//设置MIC增益       
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//串口正常
			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_MIC_GAIN_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_GSM_MIC_GAIN_SET;
	}


RE_NETOPS_QUERY:
		NetOPSQuery();
		i=250;
		while(!bUartRecFinished)
		{
			Delayms(20);
			if(--i==0)	break;
		}
	
		if(bUartRecFinished)
		{
			bUartRecFinished=0;
			if(UartRecTreat("+COPS: 0,0,"/*\"CHINA MOBILE\""*/,(uint8)strlen("+COPS: 0,0,") /*24*/))
			{//已连接网络
				GSM_STATE_LED = LED_ON;
			}
			else 
			{
				GSM_STATE_LED = LED_OFF;
				Delayms(1000);
				goto RE_NETOPS_QUERY;		
			}
		}
		else 
		{	
			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_NETOPS_QUERY;
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void main(void)
{  	
	//系统初始化			
	Init();	
	UartSendByte(0xAA);
	while(1)
    {
	    if(bBus0RecFinish)
    	{
    		bBus0RecFinish =0;
			Bus0RecDeal();
			
    	}
		if(bBus0SendFinish)
		{
			bBus0SendFinish=0;
			Bus0SendDeal();
		}

		if(bUartRecFinished)
		{

			bUartRecFinished=0;
			RecGsmDataResult = UartRecTreat3();
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr; 			
			if(TEL_HANDLE == RecGsmDataResult)
			{//电话接听
				RecGsmDataResult=0;
				UartSendByte(0xbb);
				bDealKeyDown = 1;	
				//如果正在指示其他分机的呼叫,处理其他分机呼叫
				if((bIndicatingOther))
				{				
					stLocalControl.stBusDealFreq.byRecSecAddr 	= stLocalControl.stIndicationData.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr 	= stLocalControl.stIndicationData.bySndRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr 	= stLocalControl.stIndicationData.bySndBedAddr;
					if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
					{	//卫生间呼叫,不需要处理,直接清除
						stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
					}
					else
					{	//其他类型呼叫,处理
						if(stLocalControl.stIndicationData.bySndRoomAddr==ADD_BED_FJ_ADDR)
						{//加床分机呼叫,不需要处理,直接清除
							stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
						}
						else
						{
							stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCmd & 0x1f;
							stLocalControl.stBusDealFreq.byCmd += 0x06;
						}
					} 				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}				
			}
			else if(TEL_HAND_UP == RecGsmDataResult)
			{//电话接通之后的挂断    收到“NO CARRIER”

				RecGsmDataResult =0;
				//如果存在本机的呼叫,收到电话挂断,清除本机的呼叫	
/*				if(bConfusionNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bServiceNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bHelpNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_HELP_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bEmergencyNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else */
				{

					GSM_STATE_LED= LED_OFF;
					bDealKeyDown = 0; 								//保存处置键的状态	  			
					if((bWaitAck|bWaitListen|bChannel0Talk))
					{	//通道0主动呼叫或者通话状态,减少该通道超时时间				
						MakeCH0TimerOut(5, 0);
					}
					else if(bChannel1Talk)
					{	//通道1主动通话状态,减少该通道超时时间
						MakeCH1TimerOut(5, 0);
					}	

					else if(( bIndicatingOther))
					{	
						//GSM_STATE_LED= LED_OFF;
						 bIndicatingOther =0;
						stLocalControl.stBusDealFreq.byRecSecAddr 	= stLocalControl.stIndicationData.bySndSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr 	= stLocalControl.stIndicationData.bySndRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr 	= stLocalControl.stIndicationData.bySndBedAddr;
						if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
						{	//卫生间呼叫,不需要处理,直接清除
							stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
						}
						else if(CMD_INFUSION_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
						{		//输液呼叫
							stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
						}								
						else if(CMD_SERVICE_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
						{		//输液呼叫
							stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;
						}					
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
				}

			}
//			byRecPosition=0;			
//			memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
		}

		if(bTimerOutDeal)
		{
			bTimerOutDeal=0;
			TimerOutDeal();
		}
    }	 
}
