/*
  --- STC International Limited ---------------- 
һ��������EEPROM ���Գ����ú꾧�����ذ����ֱ�Ӳ���

STC12C5AxxAD ϵ�е�Ƭ�� EEPROM/IAP ���ܲ��Գ�����ʾ
STC12C52xxAD ϵ�е�Ƭ�� EEPROM/IAP ���ܲ��Գ�����ʾ
STC11xx ϵ�е�Ƭ�� EEPROM/IAP ���ܲ��Գ�����ʾ
STC10xx ϵ�е�Ƭ�� EEPROM/IAP ���ܲ��Գ�����ʾ
 --- STC International Limited ------------------
 --- �꾧�ɷ�  ��� 2009/1/12 V1.0 --------------
 --- Mobile: 13922805190 ------------------------
 --- Fax: 0755-82944243 -------------------------
 --- Tel: 0755-82948412 -------------------------
 --- Web: www.MCU-Memory.com --------------------
����ʾ������STC-ISP Ver 3.0A.PCB �����ر�̹����ϲ���ͨ��,EEPROM ������
��P1 ������ʾ, ���Ҫ�ڳ�����ʹ�û������������øó���,���ڳ����л�������
ע��ʹ���˺꾧�Ƽ������ϼ�����
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

//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0x80 //ϵͳ����ʱ��<30MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
#define ENABLE_ISP 0x82 //ϵͳ����ʱ��<20MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x83 //ϵͳ����ʱ��<12MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x84 //ϵͳ����ʱ��<6MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x85 //ϵͳ����ʱ��<3MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x86 //ϵͳ����ʱ��<2MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x87 //ϵͳ����ʱ��<1MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ

#define DEBUG_DATA               0x5A  //�����Գ������մ洢�� EEPROM ��Ԫ����ֵ
#define DATA_FLASH_START_ADDRESS 0x00  //STC5Axx ϵ�� EEPROM ������ʼ��ַ


extern INT8U Byte_Read(INT16U add);              //��һ�ֽڣ�����ǰ���IAP ����
extern void Byte_Program(INT16U add, INT8U ch);  //�ֽڱ�̣�����ǰ���IAP ����
extern void Sector_Erase(INT16U add);            //��������
extern void IAP_Disable();                       //�ر�IAP ����
extern void Delay();


#endif
