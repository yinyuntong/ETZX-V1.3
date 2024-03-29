/*********************************************************************************************************
**				                               Small RTOS 51
**                                   The Real-Time Kernel For Keil c51
**
**                                  (c) Copyright 2002-2003, chenmingji
**                                           All Rights Reserved
**
**                                                  V1.20.0
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: OS_CPU.h
**创   建   人: 陈明计
**最后修改日期:  2003年8月17日
**描　      述: Small RTOS 51与CPU(既8051系列)相关的C语言头文件,定义一些与CPU相关的宏。
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: V0.50
** 日　期: 2002年2月22日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.00
** 日　期: 2002年6月20日
** 描　述: 无变化
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.10~1.12.1
** 日　期: 2002年9月1日~2002年12月2日
** 描　述: 根据新版本需要增加一些宏定义
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.20.0
** 日　期: 2003年8月17日
** 描　述: 根据新版本需要增加EN_REENTRANT
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#define EN_SP2        0                                         /* 禁止(0)或允许(1)软非屏蔽中断               */
#define EN_REENTRANT  0                                         /* 禁止(0)或允许(1)管理重入栈                 */

#ifdef __C51__

//#define idata                                   /* 非keil c51时必须加止这一句                               */
//#define data                                    /* 非keil c51时必须加止这一句                               */
//#define xdata                                   /* 非keil c51时必须加止这一句                               */
//#define reentrant                               /* 非keil c51时必须加止这一句                               */
//#define small                                   /* 非keil c51时必须加止这一句                               */
//#define code                                    /* 非keil c51时必须加止这一句                               */

typedef unsigned char  uint8;                                   /* 定义可移植的无符号8位整数关键字            */
typedef signed   char  int8;                                    /* 定义可移植的有符号8位整数关键字            */
typedef unsigned int   uint16;                                  /* 定义可移植的无符号16位整数关键字           */
typedef signed   int   int16;                                   /* 定义可移植的有符号16位整数关键字           */
typedef unsigned long  uint32;                                  /* 定义可移植的无符号32位整数关键字           */
typedef signed   long  int32;                                   /* 定义可移植的有符号32位整数关键字           */

extern void OSCPUInit(void) small;
extern void OSTaskStkInit(void (code * task)(void), void xdata *ptos, uint8 TaskID);
extern void OSTaskStkDel(uint8 TaskID) small;

#ifndef NULL
#define NULL 0
#endif

#define  OS_INT_ENTER() OSIntNesting++, EA = 1

#define  OS_ENTER_CRITICAL()  EA = 0,Os_Enter_Sum++      	/* 禁止中断                                 	*/	
#define  OS_EXIT_CRITICAL()   if (--Os_Enter_Sum==0) EA = 1  /* 允许中断                                 	*/	 

#define  HIGH_BYTE  0                                           /* uint16的高位字节                              */
#define  LOW_BYTE   1                                           /* uint16的低位字节                              */

#define  OS_TASK_SW()         OSCtxSw()                         /* 任务切换函数                                  */


#define  IDATA_RAM_SIZE  0x100                                  /* idata大小                                    */

extern void OSCtxSw(void);
extern void OSIntCtxSw(void);
#ifndef IN_OS_CPU_C
extern void OSStartHighRdy(void)  small;
#endif

#define  Sp2Space       8                                       /* 高级中断（软非屏蔽中断）堆栈大小 EN_SP2为0时无效*/

#define OS_TIME_ISR     6                                       /* 系统定时器使用的中断                         */

#endif



#ifndef __C51__	  
SET_EA   MACRO
             SETB     EA
         ENDM
         
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
