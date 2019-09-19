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

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//系统时钟定时值
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//主线程消息队列
STLocalControl 	xdata stLocalControl;						//全局变量结构体

bit bComRecFrame =0;

uint8 data volumn =0;
#define COM_FREQ_SIZE		64

uint8 xdata ComRecBuff[COM_FREQ_SIZE];
uint8 xdata ComRecBuff1[COM_FREQ_SIZE];
uint8 xdata ComRecPost=0;


uint32	xdata SST25VFAddr=0x000000;

void UartInt(void ) interrupt UART_INTNO
{
	uint8 byRecData;
	if(RI==1)
	{
		RI=0;
		
//将接收到的数据放到接收缓冲区中,待满10个字节后,AAI写入到存储器中

		byRecData=SBUF;

		ComRecBuff[ComRecPost] = byRecData;
		ComRecPost++;
		if(ComRecPost == COM_FREQ_SIZE)
		{//一帧数据已满
			ComRecPost=0;
			bComRecFrame =1;
		}


/*		Byte_Write1(SST25VFAddr,byRecData);
		SST25VFAddr++;
//		if(SST25VFAddr==0x01000000) SST25VFAddr=0;
*/
	}
	else if(TI==1)
	{
		TI=0;
	}
}


/**********************************************************
*函数名称			:ToComDealBuff	
*函数描述        	:将串口收到的一帧数据发送到串口处理缓冲区中
*输入参数   		:pbyData:待发送的数据指针
*返回值				:TRUE:发送成功,FALSE:队列满,发送失败 	
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
uint8 ToComDealBuff(uint8* pbyData)
{
/*	uint8 byTemp = COM_FREQ_SIZE;

	OS_ENTER_CRITICAL();
	if(ComDealBuffLen() >= (COM_DEAL_BUFF_ZISE - 1))
	{	//没有空间存储了,失败	
		OS_EXIT_CRITICAL();
		if(bPrgState)
		{
			PRG_STATE =0;
			bPrgState =0;
		}
		else 
		{
			PRG_STATE =1;
			bPrgState =1;		
		}
		return(FALSE);
	}	
	while(byTemp--)
	{	//数据入队列
		ComDealBuff[byComDealBuffTail] = *pbyData++;
		IncComDealBuffPtr(byComDealBuffTail);
	}
	OS_EXIT_CRITICAL();	
	*/
	return(TRUE);	

}

/**********************************************************
*函数名称			:delay_nms	
*函数描述        	:毫秒级延时操作
*输入参数   		:i:延时的ms数
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
void delay_nms(uint16 ms)
{
   uint16 xdata i;
   for(;ms>0;ms--)
   {
		for(i=900;i>0;i--);
   }
}


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
void delay_nus(uint16 i)
{
 	uint8 xdata j;

 	for(;i>0;i--)
 	{
 		for(j=2;j>0;j--);
 	}
	 
}



void UsartInit(void)
{
	SCON=0X50;	   //工作方式1
	TMOD&=0X0F;	   //TO方式不变
	TMOD|=0X20;	   //;t1 方式2作波特率发生器

//	TL1 =   0XD8;	   //波特率1200  在18.432M晶振的情况下
	TL1 =   0XF6;	   //波特率4800  在18.432M晶振的情况下
//	TL1 =   0XFB;	   //波特率9600  在18.432M晶振的情况下
	TH1 =   TL1;
	PCON    &=  ~SMOD;	//	;波特率倍增选择 	   smod=0
	// AUXR    &=  0xBf;	   //传统12分频速度
	AUXR |= T1x12;	   //1T模式,最后波特率为:4800*12=57.6K
//	AUXR |= T1x12;	   //1T模式,最后波特率为:9600*12=115.200K


	TF1=0;
	TR1=1;

	RI=0;
	TI=0;
	REN=1;
	ES=1;   
}
/***********************************************************/

void Send_Data(uint8 *Databuf,uint8 l)
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


void Send_Data_Byte(uint8 SndData)
{ 
    ES=0;
	
	SBUF=SndData;
	while(!TI);
	TI=0;
	
	ES=1;
}


/** Output a character to console */
#define ConsolePutChar(c){SBUF = (c);while (!TI);TI = 0; }



/** Write a Code Constant string to console */
void ConsoleWrite(char code *str){
  ES=0;
  while (*str)
    {
      ConsolePutChar (*str++);
    }
  ES=1;
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
	int8 i;
	Mp3PutInReset();
	
	CMOD = 0x02;
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

	//串口初始化
	UsartInit();


	//SPI初始化
	SpiInit();

	//SST25VF020初始化
	SST25VF_Init();


//	FRAM_TEST();


	//MP3复位
	VS1003Init();

	//擦除整个芯片
	WREN();
	Chip_Erase();	
	Wait_Busy();
	delay_nms(100);


	delay_nms(1000);
	PRG_STATE =0;	//指示灯亮
	bPrgState =0;
	EA=1;


	SST25VFAddr=0x000000;
	while(1)
	{

/*	
	//从SST25VF032中取出数据放音代码
		Read_Cont(SST25VFAddr,32,FRAM_BUFF);
		//Send_Data(FRAM_BUFF,32);
		while(!MP3_DREQ);
		Mp3SelectData();	  // Now SPI writes go to SDI port 
		for(i=0;i<32;i++)
		{
  			SPI_WriteByte(FRAM_BUFF[i]);
		}
		Mp3DeselectData();
		
		SST25VFAddr +=32;
		if(SST25VFAddr >= 0x07fff0)
		{
			SST25VFAddr=0;
			if(bPrgState)
			{
				PRG_STATE =0;
				bPrgState =0;
			}
			else 
			{
				PRG_STATE =1;
				bPrgState =1;		
			}
			VS1003FlushBuffer();			
		}
*/


	
//接收串口MP3数据写入SST25vF032芯片	
		if(bComRecFrame)
		{
			bComRecFrame = 0;
			ES=0;
			memcpy(ComRecBuff1,ComRecBuff,COM_FREQ_SIZE);
			ES=1;
			
			AAI_Write1(SST25VFAddr,ComRecBuff1,COM_FREQ_SIZE);
			SST25VFAddr += COM_FREQ_SIZE;
		}


//与上面联合测试SST25vf032芯片
/*		
		if(SST25VFAddr>=59)
		{
			if(bPrgState)
			{
				PRG_STATE =0;
				bPrgState =0;
			}
			else 
			{
				PRG_STATE =1;
				bPrgState =1;		
			}		
			 Read_Cont(0x000000,60,FRAM_BUFF);	 
			 Send_Data(FRAM_BUFF,60);
			 WREN();
			 Chip_Erase();			 
//			 Sector_Erase(0x000000);
			 Wait_Busy();
			 SST25VFAddr=0x000000;
			 ComRecPost=0;
		}
*/
	}
	


//正弦波测试
//	VsSineTest();
	while(1);

	
	
//	//复位看门狗
//	WDT_CONTR = 0x3a; 	


	//参数初始化
	InitParameter();

	//单总线初始化 
	SingleBusInit(); 	

	MakeCH0TimerOut(0, 0);		
}
/**********************************************************
*函数名称			:GetMessage	
*函数描述        	:获取主线程消息队列中的消息(无消息则挂起自身)
*输入参数   		:Msg:存储消息指针
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
void GetMessage(uint8 data *Msg)
{
	OSQPend(Msg, byMainCmdQ, 0);
}
/**********************************************************
*函数名称			:DispatchMessage	
*函数描述        	:分发处理获取的主线程消息
*输入参数   		:Msg:消息,高3位是命令类型,低5位是命令数据
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
void DispatchMessage(uint8 Msg)
{
	switch(Msg&0xe0)
	{
		case BUS0_REC:									//总线0收到数据帧
			Bus0RecDeal();
			break;
		case BUS0_SND:	 								//总线0数据发送完成
			Bus0SendDeal();
			break;		
		case KEY_DOWN: 									//按键按下处理					
			KeyDownDeal(Msg & 0x1f);
			break;
		case KEY_UP:									//按键弹起处理
			KeyUpDeal(Msg & 0x1f);
			break;
		case TIMER_OUT:	 								//超时处理
			TimerOutDeal();					
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
*创建人	      		:陈卫国
*创建日期	 		:2008-9-22
***********************************************************
*修改人	  			:
*修改日期    		:
*注释		      	:
**********************************************************/
void MainTask(void)
{
	static uint8 data Msg;  
	
	//系统硬件初始化
	Init();
	//创建主线程消息队列	
	OSQCreate(byMainCmdQ, 16);
	//以下为创建任务线程	
	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(KeyManager, NULL, 2);
	OSTaskCreate(TimerOutManager, NULL, 3);
	
	//进入消息循环
	while(TRUE)
	{
		GetMessage(&Msg);
		DispatchMessage(Msg);
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
	//操作系统初始化			
	OSInit();	
	//创建系统主线程
	OSTaskCreate(MainTask, NULL, 0); 	
	while(1)
    {
       PCON = PCON | 0x01;                    //CPU进入休眠状态	    
    }	 
}

