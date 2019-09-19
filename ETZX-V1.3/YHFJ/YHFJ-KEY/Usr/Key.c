/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:Key.c
*文件描述    		:医护分机键盘处理程序
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
#define _IN_KEY_
#include "config.h"

#define KeyScanWait()	{_nop_();_nop_();}
//32个按键状态联合体	 
typedef union 
{
	uint8 	byKeyValue[4];
	uint32 	ulKeyValue;
} UNKeyValue, *pUNKeyValue;	


//按键扫描引脚定义
sbit 	H1 	= P1^0;
sbit    H2	= P1^1;
sbit	H3 	= P1^2;
sbit	H4	= P1^3;	
sbit	H5	= P1^4;	

sbit	L1	= P3^0;
sbit	L2	= P3^1;
//sbit	L1	= P1^7;
//sbit	L2	= P1^6;
sbit	L3	= P3^2;
sbit	L4	= P3^5;
sbit	L5	= P3^7;


extern uint8	OS_Q_MEM_SEL	byMainCmdQ[];	  
extern STLocalControl 	xdata stLocalControl;

/**********************************************************
*函数名称			:KeyScan	
*函数描述        	:键盘扫描函数
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
uint8 KeyScan(void)
{
    H1=1;
	H2=1;
    H3=1;
	H4=1;	
 	H5=1;

	
	H1=0;
//	KeyScanWait();
	delay_nus(100);
	if(L1==0)  return(KEY_3);
	else if(L2==0)  return(KEY_2);
	else if(L3==0)  return(KEY_1);
	else if(L4==0)  return(KEY_SEARCH);
	else if(L5==0)  return(KEY_ADDBED);

			
	H1 = 1;
	H2 = 0;
//	KeyScanWait();
	delay_nus(100);
	if(L1==0)  return(KEY_6);
	else if(L2==0)  return(KEY_5);
	else if(L3==0)  return(KEY_4);
	else if(L4==0)  return(KEY_CALCULATE);
	else if(L5==0)  return(KEY_SEC);

	H2 = 1;
	H3 = 0;
//	KeyScanWait();
	delay_nus(100);
	if(L1==0)  return(KEY_9);
	else if(L2==0)  return(KEY_8);
	else if(L3==0)  return(KEY_7);
//	else if(L4==0)  return(KEY_MODE);
	else if(L4==0)	return(KEY_BROAD);
//	else if(L5==0)  return(KEY_ROOM);
	else if(L5==0)  return(KEY_MS);


	H3 = 1;
	H4 = 0;
//	KeyScanWait();
	delay_nus(100);
	if(L1==0)  return(KEY_ENTER);
	else if(L2==0)  return(KEY_0);
	else if(L3==0)  return(KEY_CLEAR);
//	else if(L4==0)  return(KEY_BROAD);
	else if(L4==0)	return(KEY_MODE);
//	else if(L5==0)  return(KEY_MS);
	else if(L5==0)	return(KEY_ROOM);


	H4 = 1;
	H5 = 0;
//	KeyScanWait();
	delay_nus(100);
	if(L1==0)  return(KEY_VOICEDEC);
	else if(L2==0)  return(KEY_RESERVE);
	else if(L3==0)  return(KEY_VOICEINC);
	else if(L4==0)  return(KEY_MUSIC);
	else if(L5==0)  return(KEY_MR);	
	H5 = 1;
	return(NO_KEY);
}
/**********************************************************
*函数名称			:KeyManager	
*函数描述        	:键盘监控程序
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
void KeyManager(void)
{ 	
	static uint8 byKeyValue;	

	L1=1;
	L2=1;
	L3=1;
	L4=1;	
	L5=1;	
    while (TRUE)
    {
//        OSWait(K_TMO, OS_TICKS_PER_SEC/50);           		//延时20ms
        OSWait(K_TMO, 10);           		//延时100ms   
        byKeyValue = KeyScan();              				//获当前按键状态
        if(byKeyValue == NO_KEY)
        {        	
       		continue;
        }        
//        OSWait(K_TMO, OS_TICKS_PER_SEC/50);           		//延时20ms 
        OSWait(K_TMO, 10);           		//延时100ms 
        if(byKeyValue != KeyScan())
        {
            continue;
        }
        OSQPost(byMainCmdQ, KEY_DOWN|byKeyValue);			//处理按键闭合事件
        if(byKeyValue == KEY_CLEAR)
        {
        	OSWait(K_TMO, 200);					//键按下之后延时1000ms,避免频凡按键
        }

		//等待按键松开                                                  
        while(byKeyValue == KeyScan())
        {	//处理按键连击事件
//			OSWait(K_TMO, OS_TICKS_PER_SEC/50);    
	        OSWait(K_TMO, 10);           		//延时100ms 
        }
		OSQPost(byMainCmdQ, KEY_UP|byKeyValue);				//处理按键弹起事件			
    }
}
/**********************************************************
*函数名称			:KeyDownDeal	
*函数描述        	:按键按下处理程序
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
void KeyDownDeal(uint8 byKey)
{
	stLocalControl.stBusDealFreq.bySndSecAddr = byKey;
	stLocalControl.stBusDealFreq.bySndRoomAddr = 0x00;
	stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;
	stLocalControl.stBusDealFreq.byCmd = CMD_KEY_DOWN;
	stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
	stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
	stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
	Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));  
}
/**********************************************************
*函数名称			:KeyUpDeal	
*函数描述        	:按键弹起处理程序
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
void KeyUpDeal(uint8 byKey)
{
	stLocalControl.stBusDealFreq.bySndSecAddr = byKey;
	stLocalControl.stBusDealFreq.bySndRoomAddr = 0x00;
	stLocalControl.stBusDealFreq.bySndBedAddr = 0x00;
	stLocalControl.stBusDealFreq.byCmd = CMD_KEY_UP;
	stLocalControl.stBusDealFreq.byRecSecAddr = 0xff;
	stLocalControl.stBusDealFreq.byRecRoomAddr = 0xff;
	stLocalControl.stBusDealFreq.byRecBedAddr = 0xff;
	Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));  
}
