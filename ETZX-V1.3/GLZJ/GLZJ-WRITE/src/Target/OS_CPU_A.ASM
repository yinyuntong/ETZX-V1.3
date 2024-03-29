;*********************************************************************************************************
;**                                            Small RTOS 51 
;**                                   The Real-Time Kernel For Keil c51
;**
;**                                  (c) Copyright 2002-2003, chenmingji
;**                                           All Rights Reserved
;**
;**                                                  V1.12.1
;**
;**
;**--------------------文件信息---------------------------------------------------------------------------
;**文   件   名: OS_CPU_A.ASM
;**创   建   人: 陈明计
;**版        本: V1.12.1
;**最后修改日期:  2002年2月5日
;**描　      述:  Small RTOS 51 与CPU(8051系列)相关的汇编程序
;**---------------------历史版本信息----------------------------------------------------------------------
;** 创建人: 陈明计
;** 版  本:V0.50
;** 日　期: 2002年2月22日
;** 描　述: 原始版本
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.00
;** 日　期: 2002年6月10日
;** 描　述: 支持软的非屏蔽中断
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.10.3
;** 日　期: 2002年9月16日
;** 描　述: 修改了LoadCtx代码使之执行更快，代码更小
;**         
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.10.4
;** 日　期: 2002年10月5日
;** 描　述: 将OS_CPU_A.ASM和OS_CPU_A_task16.ASM合并
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.11.0
;** 日　期: 2002年12月2日
;** 描　述: 根据新版本要求使任务堆栈包含Os_Enter_Sum，使优先级最低
;**         的任务只保存少量寄存器；增加注释
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.12.0
;** 日　期: 2002年12月30日
;** 描　述: 根据新版本要求更改少量代码
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.12.1
;** 日　期: 2002年2月5日
;** 描　述: 更正LoadCtx中OS_MAX_TASKS为8或16的bug
;**---------------------当前版本修订-----------------------------------------------------------------------
;** 修改人:
;** 日　期:
;** 描　述:
;**
;**------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
#include "OS_CPU.H"
#include "OS_CFG.H"

#if EN_REENTRANT > 0
#define SP_ADD_BYTE  5
#else
#define SP_ADD_BYTE  3
#endif


    NAME    OS_CPU_A_ASM

?PR?OSCtxSw?OS_CPU_A                     SEGMENT CODE
?PR?OSIntCtxSw?OS_CPU_A                  SEGMENT CODE
?PR?LoadCtx?OS_CPU_A                     SEGMENT CODE
?PR?C_OSCtxSw?OS_CPU_A                   SEGMENT CODE 
?PR?_OSTaskStkInit?OS_CPU_A              SEGMENT CODE 


    EXTRN   CODE (OSMapTbl)
    EXTRN   DATA (OSFastSwap)
    EXTRN   DATA (OSTaskID)
    EXTRN   DATA (OSNextTaskID)
    EXTRN   DATA (OSTsakStackBotton)
    EXTRN   DATA (Os_Enter_Sum)
IF EN_SP2  <> 0
    EXTRN   IDATA (Sp2)
ENDIF
IF EN_REENTRANT > 0
    EXTRN   DATA (?C_XBP)
ENDIF

    PUBLIC  _OSTaskStkInit
    PUBLIC  LoadCtx
    PUBLIC  OSIntCtxSw
    PUBLIC  OSCtxSw
    PUBLIC  STACK
    PUBLIC  C_OSCtxSw

;****************************************************************************************
;?STACK SEGMENT IDATA

?STACK      SEGMENT   IDATA

        RSEG    ?STACK
STACK:                                          ;堆栈
        DS  1

; /*********************************************************************************************************
; ** 函数名称: OSTaskStkInit
; ** 功能描述: 任务堆栈初始化
; ** 输　入: 无
; ** 输　出 : 无
; ** 全局变量: OSTaskID,OSTsakStackBotton,SP
; ** 调用模块: LoadCtx
; ** 
; ** 作　者: 陈明计
; ** 日　期: 2002年2月22日
; **-------------------------------------------------------------------------------------------------------
; ** 修改人:
; ** 日　期:
; **-------------------------------------------------------------------------------------------------------
; ********************************************************************************************************/
 
    RSEG  ?PR?_OSTaskStkInit?OS_CPU_A
_OSTaskStkInit:
    USING   0
;---- Variable 'cp?147' assigned to Register 'R0' ----
;---- Variable 'cp?146' assigned to Register 'R1' ----
;---- Variable 'i?145' assigned to Register 'R2' ----
;---- Variable 'TaskID?144' assigned to Register 'R3' ----
;---- Variable 'ptos?143' assigned to Register 'R4/R5' ----
    
    PUSH    AR6
    PUSH    AR7
; {
#if OS_MAX_TASKS < 9
;         OSFastSwap[0] &= ~OSMapTbl[TaskID];		  //清0  
    MOV     A,R3
    MOV     DPTR,#OSMapTbl
    MOVC    A,@A+DPTR
    ORL     OSFastSwap,A
#else
;         if (TaskID < 8)
    MOV     A,R3
    CLR     C
    SUBB    A,#08H
    MOV     A,R3
    JNC     StkInit07
;         {
;             OSFastSwap[1] &= ~OSMapTbl[TaskID];
    MOV     DPTR,#OSMapTbl
    MOVC    A,@A+DPTR
    ORL     OSFastSwap+1,A
;         }
    SJMP    StkInit08
StkInit07:
;         else
;         {
;             OSFastSwap[0] &= ~OSMapTbl[TaskID & 0x07];
    ANL     A,#07H
    MOV     DPTR,#OSMapTbl
    MOVC    A,@A+DPTR
    ORL     OSFastSwap,A
;         }
StkInit08:
#endif

;     if (TaskID < OSRunningTaskID())
    MOV     A,R3
    CLR     C
    SUBB    A,OSTaskID
    JNC     StkInit5
;     {
;         i =  OSRunningTaskID() - TaskID;
    CLR     C
    MOV     A,OSTaskID
    SUBB    A,R3
    MOV     R2,A
;         cp = (uint8 idata *)(&(OSTsakStackBotton[TaskID + 1]));
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,R3
    MOV     R1,A
StkInit8:
;         do
;         {
;             *cp += SP_ADD_BYTE;
    MOV     A,#SP_ADD_BYTE
    ADD     A,@R1
    MOV     @R1,A
;             cp++;
    INC     R1
;         } while (--i != 0);
    DJNZ    R2,StkInit8

;         cp1 = (uint8 idata *)SP;
    MOV     R0,SP
;         SP = SP + SP_ADD_BYTE;
            ; SOURCE LINE # 172
    MOV     A,#SP_ADD_BYTE
    ADD     A,SP
    MOV     SP,A
;         i = SP - (uint8)(OSTsakStackBotton[TaskID + 1]) + 1;
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,R3
    MOV     R1,A
    MOV     AR7,@R1
    CLR     C
    MOV     A,SP
    SUBB    A,R7
    MOV     R2,A
    INC     R2
;         cp = (uint8 idata *)SP;
    MOV     R1,SP
StkInit11:
;         do
;         {
;             *cp-- = *cp1--;
    MOV     A,@R0
    MOV     @R1,A
    DEC     R0
    DEC     R1
;         } while (--i != 0);
    DJNZ    R2,StkInit11
;     }
    SJMP    StkInit12
StkInit5:
;     else
;     {
;         cp1 = (uint8 idata *)(&(OSTsakStackBotton[OSRunningTaskID() + 1]));
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSTaskID
    MOV     R0,A
;         i = TaskID - OSRunningTaskID();
    CLR     C
    MOV     A,R3
    SUBB    A,OSTaskID
    MOV     R2,A
StkInit15:
;         do
;         {
;             *cp1 -= SP_ADD_BYTE;
    MOV     A,#(-SP_ADD_BYTE)
    ADD     A,@R0
    MOV     @R0,A
;             cp1++;
    INC     R0
;         } while (--i != 0);
    DJNZ    R2,StkInit15
;         
;         cp = OSTsakStackBotton[OSRunningTaskID() + 1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSTaskID
    MOV     R0,A
    MOV     A,@R0
    MOV     R1,A
;         i = OSTsakStackBotton[TaskID] - cp - SP_ADD_BYTE;
    MOV     A,R1
    ADD     A,#(-SP_ADD_BYTE)
    MOV     R7,A

    MOV     A,#LOW (OSTsakStackBotton)
    ADD     A,R3
    MOV     R0,A
    MOV     A,@R0
    CLR     C
    SUBB    A,R7
    MOV     R2,A
;         cp1 = cp - SP_ADD_BYTE;
    MOV     R0,AR7
StkInit18:
;         do
;         {
;             *cp1++ = *cp++;
    MOV     A,@R1
    MOV     @R0,A
    INC     R0
    INC     R1
;         } while (--i != 0);
    DJNZ    R2,StkInit18
;     }
StkInit12:
;     cp = OSTsakStackBotton[TaskID];
    MOV     A,#LOW (OSTsakStackBotton)
    ADD     A,R3
    MOV     R0,A
    MOV     A,@R0
    MOV     R1,A
;     *cp++ = (uint16)task % 256;
    pop     ACC
    MOV     @R1,A
    INC     R1
;     *cp++ = (uint16)task / 256;
    pop     ACC
    MOV     @R1,A
    INC     R1
#if EN_REENTRANT > 0
;     *cp++ = (uint16)ptos / 256;
    MOV     @R1,AR4
    INC     R1
;     *cp++ = (uint16)ptos % 256;
    MOV     @R1,AR5
    INC     R1
#endif
;     *cp = 0;
    CLR     A
    MOV     @R1,A
; }
    RET     

;/*********************************************************************************************************
;** 函数名称: LoadCtx
;** 功能描述: 任务环境恢复函数
;** 输　入: OSTaskID,OSFastSwap
;** 输　出 : 无
;** 全局变量: 无
;** 调用模块: 无
;** 
;** 作　者: 陈明计
;** 日　期: 2002年2月22日
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 陈明计
;** 日　期: 2002年12月2日
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 陈明计
;** 日　期: 2003年2月5日
;**-------------------------------------------------------------------------------------------------------
;** 修　改:
;** 日　期:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/

    RSEG  ?PR?LoadCtx?OS_CPU_A
LoadCtx:
    USING   0
   
    POP     Os_Enter_Sum            ;恢复关中断计数器

#if EN_REENTRANT > 0
    POP     ?C_XBP+1
    POP     ?C_XBP
#endif

                                    ;判断是否需要恢复所有寄存器
    MOV     A,OSTaskID
    CJNE    A,#OS_MAX_TASKS,LoadCtx_0
    SJMP    LoadCtx_2
LoadCtx_0:
    MOV     DPTR,#OSMapTbl
#if OS_MAX_TASKS < 9
    MOVC    A,@A+DPTR
    ANL     A,OSFastSwap
#else
    MOV     R6,OSFastSwap
    CLR     C
    SUBB    A,#8
    JNC     LoadCtx_1
    MOV     R6,OSFastSwap + 1
    MOV     A,OSTaskID
LoadCtx_1:
    MOVC    A,@A+DPTR
    ANL     A,R6
#endif
    JNZ     LoadCtx_2
                                    ;恢复寄存器
    POP     7
    POP     6
    POP     5
    POP     4
    POP     3
    POP     2
    POP     1
    POP     0
    POP     PSW
    POP     DPL
    POP     DPH
    POP     B
    POP     ACC
LoadCtx_2:
                                    ;判断是否需要开中断
    INC     Os_Enter_Sum
    djnz    Os_Enter_Sum,LoadCtx_3
    SET_EA                          ;开中断
LoadCtx_3:
    RET


;/*********************************************************************************************************
;** 函数名称: OSCtxSw
;** 功能描述: 任务主动放弃CPU环境保存函数
;** 输　入: OSTaskID
;** 输　出 : 无
;** 全局变量: OSFastSwap
;** 调用模块: 无
;** 
;** 作　者: 陈明计
;** 日　期: 2002年2月22日
;**-------------------------------------------------------------------------------------------------------
;** 修　改:
;** 日　期:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    RSEG  ?PR?OSCtxSw?OS_CPU_A
OSCtxSw:
    USING   0

#if EN_REENTRANT > 0
    PUSH     ?C_XBP
    PUSH     ?C_XBP+1
#endif

    PUSH    Os_Enter_Sum            ;保存关中断计数器
                                    ;设置标志：任务再次恢复运行时不必恢复所有寄存器
    MOV     DPTR,#OSMapTbl
    MOV     A,OSTaskID
#if OS_MAX_TASKS < 9
    MOVC    A,@A+DPTR
    ORL     OSFastSwap,A
#else
    CLR     C
    SUBB    A,#8
    JC      OSCtxSw_1
    MOVC    A,@A+DPTR
    ORL     OSFastSwap,A
    LJMP    C_OSCtxSw
OSCtxSw_1:
    MOV     A,OSTaskID
    MOVC    A,@A+DPTR
    ORL     OSFastSwap+1,A
#endif
    LJMP    C_OSCtxSw
;****************************************************************************************
;/*********************************************************************************************************
;** 函数名称: C_OSCtxSw
;** 功能描述: 堆栈处理函数
;** 输　入: 无
;** 输　出 : 无
;** 全局变量: OSTaskID,OSTsakStackBotton,SP
;** 调用模块: LoadCtx
;** 
;** 作　者: 陈明计
;** 日　期: 2002年2月22日
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 陈明计
;** 日　期: 2002年12月2日
;**-------------------------------------------------------------------------------------------------------
;** 修　改:
;** 日　期:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    RSEG  ?PR?C_OSCtxSw?OS_CPU_A
C_OSCtxSw:

    mov     r2,sp
    
;     cp1 = (unsigned char idata *)SP +1;
    MOV     R0,SP

IF EN_SP2  <> 0
    mov     sp,#(Sp2-1)             ;堆栈指向临时空间，允许“软非屏蔽中断”
ENDIF

    INC     R0
;     temp = (unsigned char )OSTsakStackBotton[OSNextTaskID+1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSNextTaskID
    MOV     R1,A
    MOV     A,@R1
    MOV     R7,A
;     cp2 = OSTsakStackBotton[OSTaskID+1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSTaskID
    MOV     R1,A
    MOV     A,@R1
    MOV     R1,A
;     if( OSNextTaskID > OSTaskID)
    MOV     A,OSNextTaskID
    SETB    C
    SUBB    A,OSTaskID
    JC      ?C0001
;     {
;         while(cp2 != (unsigned char idata *)temp)
;         {
;             *cp1++ = *cp2++;
;         }
    MOV     A,R7
    CLR     C
    SUBB    A,R1
    MOV     R6,A
?C0002:
    MOV     A,@R1
    MOV     @R0,A
    INC     R0
    INC     R1
    DJNZ    R6,?C0002
?C0003:
;         temp = OSTsakStackBotton[OSTaskID+1] - (unsigned char idata *)SP-1;
    MOV     A,#LOW (OSTsakStackBotton+1)
    ADD     A,OSTaskID
    MOV     R1,A
    MOV     A,@R1
    SETB    C
    ;SUBB    A,sp
    SUBB    A,r2
    MOV     R7,A
;         SP = (unsigned char )cp1 - 1;
    DEC     R0;
    MOV     SP,R0
;         for(i = OSTaskID+1;i < OSNextTaskID+1; i++)
;         {
;             OSTsakStackBotton[i] -= temp;
;         }
    MOV     A,OSNextTaskID
    CLR     C
    SUBB    A,OSTaskID
    MOV     R6,A
    JZ      ?C0005

    MOV     A,#LOW (OSTsakStackBotton)
    ADD     A,OSTaskID
    MOV     R1,A    
    MOV     A,R7
    CPL     A
    INC     A
    MOV     R7,A
?C0004:
    INC     R1
    MOV     A,R7
    ADD     A,@R1    
    MOV     @R1,A
    DJNZ    R6,?C0004
?C0005:
;         OSTaskID = OSNextTaskID;
    MOV     OSTaskID,OSNextTaskID
;         LoadCtx();    
    LJMP    LoadCtx
;     }
?C0001:
; 
;     if( OSNextTaskID != OSTaskID)
    MOV     A,OSNextTaskID
    XRL     A,OSTaskID
    JZ      ?C000r
;     {
;          cp2--;
;          cp1--;
;         while(cp2 != (unsigned char idata *)temp)
;         {
;             *cp2-- = *cp1--;
;         }
    ;MOV     A,R7
    ;CLR     C
    ;SUBB    A,R1
    ;MOV     R6,A
    mov     a,r0
    clr     c
    subb    a,r7
    mov     r6,a
?C0008:
    DEC     R0
    DEC     R1
    MOV     A,@R0
    MOV     @R1,A
    DJNZ    R6,?C0008
?C0009:
;         temp = OSTsakStackBotton[OSTaskID+1] - (unsigned char idata *)SP-1;
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSTaskID
    MOV     R1,A
    MOV     A,@R1
    SETB    C
    ;SUBB    A,SP
    SUBB    A,r2
    MOV     R7,A
;         SP = (unsigned char )OSTsakStackBotton[OSNextTaskID+1];
    MOV     A,#LOW (OSTsakStackBotton+01H)
    ADD     A,OSNextTaskID
    MOV     R1,A
    MOV     A,@R1
    MOV     SP,A
;         for(i = OSNextTaskID+1;i < OSTaskID+1; i++)
;         {
;             OSTsakStackBotton[i] += temp;
;         }

    MOV     A,OSTaskID
    CLR     C
    SUBB    A,OSNextTaskID
    JZ      ?C0011

    MOV     R6,A
    MOV     A,#LOW (OSTsakStackBotton)
    ADD     A,OSNextTaskID
    MOV     R1,A    
?C0010:
    INC     R1
    MOV     A,R7
    ADD     A,@R1    
    MOV     @R1,A
    DJNZ    R6,?C0010

?C0011:
;         OSTaskID = OSNextTaskID;        
    MOV     OSTaskID,OSNextTaskID
;         SP--;
    DEC     SP
;     }
?C0007:
;     LoadCtx();
    LJMP    LoadCtx
?C000r:
IF EN_SP2  <> 0
    mov     SP,r2
ENDIF
    LJMP    LoadCtx
;****************************************************************************************
;/*********************************************************************************************************
;** 函数名称: OSIntCtxSw
;** 功能描述: 中断使任务放弃CPU环境保存函数
;** 输　入: OSTaskID
;** 输　出 : 无
;** 全局变量: OSFastSwap
;** 调用模块: 无
;** 
;** 作　者: 陈明计
;** 日　期: 2002年2月22日
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 陈明计
;** 日　期: 2002年12月2日
;**-------------------------------------------------------------------------------------------------------
;** 修　改:
;** 日　期:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    RSEG  ?PR?OSIntCtxSw?OS_CPU_A
OSIntCtxSw:
    USING   0
                                        ;是否是优先级最低任务
    MOV     A,#OS_MAX_TASKS
    XRL     A,OSTaskID
    JNZ     OSIntCtxSw_0
                                        ;是则不需要保存所有寄存器
;SP=SP-13-4                             ;4:两层函数调用堆栈，13：寄存器数目
    MOV     A,#(-17)
    ADD     A,SP
    MOV     SP,A
                                        ;跳转到OSCtxSw，同时通知CPU中断处理完成
    MOV     A, #LOW  OSCtxSw
    PUSH    ACC
    MOV     A, #HIGH OSCtxSw
    PUSH    ACC
    RETI
                                        ;需要保存所有寄存器
OSIntCtxSw_0:
;SP=SP-4                                ;4:两层函数调用堆栈
    MOV     A,#0FCH
    ADD     A,SP
    MOV     SP,A
                                        ;设置标志：任务再次恢复运行时需要恢复所有寄存器
    MOV     DPTR,#OSMapTbl
    MOV     A,OSTaskID
#if OS_MAX_TASKS < 9
    MOVC    A,@A+DPTR
    CPL     A
    ANL     OSFastSwap,A
#else
    CLR     C
    SUBB    A,#8
    JC      OSIntCtxSw_1    
    MOVC    A,@A+DPTR
    CPL     A    
    ANL     OSFastSwap,A
    SJMP    OSIntCtxSw_2
OSIntCtxSw_1:
    MOV     A,OSTaskID
    MOVC    A,@A+DPTR
    CPL     A    
    ANL     OSFastSwap+1,A
OSIntCtxSw_2:
#endif

#if EN_REENTRANT > 0
    PUSH     ?C_XBP
    PUSH     ?C_XBP+1
#endif
    PUSH    Os_Enter_Sum            ;保存关中断计数器
                                        ;跳转到堆栈处理，同时通知CPU中断处理完成
    MOV     A, #LOW  C_OSCtxSw
    PUSH    ACC
    MOV     A, #HIGH C_OSCtxSw
    PUSH    ACC
    RETI

;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
    END
