/*
************************Copyright(c)************************
*	  			   湖南一特电子股份有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:MB85RS256.h
*文件描述    		:MB85RS256铁电存储器头文件
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


/*#define WREN    0x06
#define WRITE   0x02
#define READ    0x03
#define RDSR	0x05
#define EWSR    0X50
#define WRSR	0x01
#define WRDI	0x04
#define SECTOR_ERASE  0X20
#define BLOCK_ERASE   0X52
#define CHIP_ERASE    0X60
#define AAI_WRITE	  0XAF
#define READ_ID1      0X90
#define READ_ID2      0XAB*/


#ifndef _IN_SST25VF_
#define _IN_SST25VF_

extern unsigned char xdata FRAM_BUFF[];

extern void SST25VF_Init();
extern void Send_Byte(unsigned char out);
extern unsigned char Get_Byte();
extern void Poll_SO();
extern void CE_High();
extern void CE_Low();
extern void Hold_Low();
extern void Unhold();
extern void WP_Low();
extern void UnWP();
extern unsigned char Read_Status_Register();
extern void EWSR();
extern void WRSR(byte);
extern void WREN();
extern void WRDI();
extern void EBSY();
extern void DBSY();
extern unsigned char Read_ID(ID_addr);
extern unsigned long Jedec_ID_Read(); 
extern unsigned char Read(unsigned long Dst);
extern void Read_Cont(unsigned long Dst, unsigned long no_bytes,uint8 *pBuffer);
extern unsigned char HighSpeed_Read(unsigned long Dst); 
extern void HighSpeed_Read_Cont(unsigned long Dst, unsigned long no_bytes,uint8 *pBuffer);
extern void Byte_Program(unsigned long Dst, unsigned char byte);
extern void Auto_Add_IncA(unsigned long Dst, unsigned char byte1, unsigned char byte2);
extern void Auto_Add_IncB(unsigned char byte1, unsigned char byte2);
extern void Auto_Add_IncA_EBSY(unsigned long Dst, unsigned char byte1, unsigned char byte2);
extern void Auto_Add_IncB_EBSY(unsigned char byte1, unsigned char byte2);
extern void Chip_Erase();
extern void Sector_Erase(unsigned long Dst);
extern void Block_Erase_32K(unsigned long Dst);
extern void Block_Erase_64K(unsigned long Dst);
extern void Wait_Busy();
extern void Wait_Busy_AAI();
extern void WREN_Check();
extern void WREN_AAI_Check();

extern void Verify(unsigned char byte, unsigned char cor_byte);
extern void Byte_Write(unsigned long Dst, unsigned char byte);
extern void AAI_Write(unsigned long Dst, uint8 *p,uint8 no_bytes);
extern void Byte_Write1(unsigned long Dst, unsigned char byte);
extern void AAI_Write1(unsigned long Dst, uint8 *p,uint8 no_bytes);
extern void delay_nus(unsigned char i);
extern void delay_nms(unsigned char i);
extern void FRAM_TEST(void);



#endif

