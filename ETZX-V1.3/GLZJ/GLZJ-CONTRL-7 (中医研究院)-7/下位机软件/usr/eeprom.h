/*
  --- STC International Limited ---------------- 
一个完整的EEPROM 测试程序，用宏晶的下载板可以直接测试

STC12C5AxxAD 系列单片机 EEPROM/IAP 功能测试程序演示
STC12C52xxAD 系列单片机 EEPROM/IAP 功能测试程序演示
STC11xx 系列单片机 EEPROM/IAP 功能测试程序演示
STC10xx 系列单片机 EEPROM/IAP 功能测试程序演示
 --- STC International Limited ------------------
 --- 宏晶股份  设计 2009/1/12 V1.0 --------------
 --- Mobile: 13922805190 ------------------------
 --- Fax: 0755-82944243 -------------------------
 --- Tel: 0755-82948412 -------------------------
 --- Web: www.MCU-Memory.com --------------------
本演示程序在STC-ISP Ver 3.0A.PCB 的下载编程工具上测试通过,EEPROM 的数据
在P1 口上显示, 如果要在程序中使用或在文章中引用该程序,请在程序中或文章中
注明使用了宏晶科技的资料及程序
*/

#ifndef _EEPROM_H_
#define _EEPROM_H_

typedef unsigned char  INT8U;
typedef unsigned int   INT16U;


#if STC90==1
#define IAP_DATA	ISP_DATA
#define IAP_CONTR	ISP_CONTR
#define IAP_CMD		ISP_CMD
#define IAP_ADDRH	ISP_ADDRH
#define IAP_ADDRL	ISP_ADDRL
#define IAP_TRIG	ISP_TRIG
#endif	

//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

#define DEBUG_DATA               0x5A  //本测试程序最终存储在 EEPROM 单元的数值
#define DATA_FLASH_START_ADDRESS 0x00  //STC5Axx 系列 EEPROM 测试起始地址


extern INT8U Byte_Read(INT16U add);              //读一字节，调用前需打开IAP 功能
extern void Byte_Program(INT16U add, INT8U ch);  //字节编程，调用前需打开IAP 功能
extern void Sector_Erase(INT16U add);            //擦除扇区
extern void IAP_Disable();                       //关闭IAP 功能
extern void Delay();


#endif
