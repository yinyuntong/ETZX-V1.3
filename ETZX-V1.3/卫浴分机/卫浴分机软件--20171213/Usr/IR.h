/*
*-----------------------文件信息---------------------------
*文件名称     		:IR.h
*文件描述    		:红外接收程序头文件
*创建人     		:熊坚强
*创建日期   		:2011-2-16
*版本号       		:
*注释	     		:
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#ifndef _IR_H_
#define _IR_H_

#define IrDA_CMD_NURSE_IN   0x01							//红外接口护士到位命令
#define IrDA_CMD_INSPECTION	0x02							//红外接口查房命令


#define NURSE_IN_TIME	350								//护士到位信号有效时间
#define MOVE_FJ				251								//移动分机



#ifndef _IN_IR_

extern uint8 xdata MoveFjNum;		//移动分机号码

extern uint8 xdata UartRecData;
extern uint8 xdata IrDataPosit;

extern uint16  uiNurseInTime;						//检测护士到位定时时间
extern uint8 	byIrDataIntervalTimer;				//红外数据接收过程中字节之间间隔时间
extern uint8 	byNumSetTime;						//编号时间

extern uint8 xdata byIrDARecBuff[], byIrDADealBuff[];		//红外接收区、处理缓冲区

extern bit bFourByteRec;					//串口接收到4字节命令
extern bit bSixByteRec;					//串口接收到6字节命�


extern bit	bSetKeyDown ;				//设置键按下状态
extern bit	bNurseIn;	 			//护士到位标志


extern void IRInit(void);
extern void IrDATreat(void);	//数据处理函数
extern void IrDANumberSet(void);	//数据处理函数

#endif



#endif
