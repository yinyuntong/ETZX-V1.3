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



#define WINBOND_FLASH  0XEF
#define W25Q80			0X13
#define W25Q16			0X14
#define W25Q32			0X15

#define SST_FLASH		0XBF
#define SST25VF032		0X4A


#define DATA_TYPE_SIZE			0X1000	//4k




#define FLASH_PageSize      256                                  //页大小
#define FLASH_PerWritePageSize      256                          //每页大小

/*W25X指令定义*/
#define W25X_WriteEnable		      0x06                            //写使能指令
#define W25X_WriteDisable		      0x04                            //写失能指令
#define W25X_ReadStatusReg		      0x05                            //读状态寄存器
#define W25X_WriteStatusReg		      0x01                            //写状态寄存器
#define W25X_ReadData			      0x03                            //读数据指令
#define W25X_FastReadData		      0x0B                            //快速读模式指令
#define W25X_FastReadDual		      0x3B                            //快读双输出指令    
#define W25X_PageProgram		      0x02                            //页写入指令
#define W25X_BlockErase			      0xD8                            //块擦除指令
#define W25X_SectorErase		      0x20                            //扇区擦除指令
#define W25X_ChipErase			      0xC7                            //片擦除指令
#define W25X_PowerDown			      0xB9                            //进入掉电模式指令
#define W25X_ReleasePowerDown	      0xAB                            //退出掉电模式
#define W25X_DeviceID			      0xAB                            //读取芯片ID
#define W25X_ManufactDeviceID   	  0x90                            //读取制造ID
#define W25X_JedecDeviceID		      0x9F 

#define WIP_Flag                      0x01                            //写入忙标志位
#define Dummy_Byte                    0xFF                            //空数据





#ifndef _IN_SST25VF_
#define _IN_SST25VF_

extern uint8 xdata manID,devID;




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
extern void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes);
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
extern void AAI_Write(unsigned long Dst, uint8 *p,uint16 no_bytes);
extern void delay_nus(unsigned int i);
extern void delay_nms(unsigned int i);
extern void FRAM_TEST(void);

extern void Write_Cont(unsigned long Dst, uint8 *p,uint16 no_bytes);
extern void Flash_Lock(void);

#endif

