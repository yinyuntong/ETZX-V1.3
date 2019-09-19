#ifndef _SPI_H_
#define _SPI_H_


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



//SPI接口定义
#define SPI_CS0	(1<<4)   // 字库片选脚 P2.4
#define SPI_CS1	(1<<5)   // 存储器片选脚 P3.5
#define SPI_MISO (1<<5)	 //P2.5
#define SPI_MOSI (1<<4)	 //P3.4
#define SPI_SCK	(1<<9)   //P1.9
//SPI接口操作定义
#define SetSpiCS0() (LPC_GPIO2->DATA|=SPI_CS0)
#define ClrSpiCS0() (LPC_GPIO2->DATA&=~SPI_CS0)
#define SetSpiCS1() (LPC_GPIO3->DATA|=SPI_CS1)
#define ClrSpiCS1() (LPC_GPIO3->DATA&=~SPI_CS1)
#define SetSpiMOSI() (LPC_GPIO3->DATA|=SPI_MOSI)
#define ClrSpiMOSI() (LPC_GPIO3->DATA&=~SPI_MOSI)
#define SetSpiSCK() (LPC_GPIO1->DATA|=SPI_SCK)
#define ClrSpiSCK() (LPC_GPIO1->DATA&=~SPI_SCK)
#define GetSpiMISO() (LPC_GPIO2->DATA&SPI_MISO)

//SPI操作函数定义
extern void SPI_Init(void);
extern void Enable_GT23L32S4W(void);
extern void Disable_GT23L32S4W(void);
extern void Enable_Flash(void);
extern void Disable_Flash(void);
extern void SPI_PutByte(INT8U data);
extern void SPI_PutBytes(INT8U * data,INT8U len);
extern INT8U SPI_GetByte(void);
extern void SPI_GetBytes(INT8U * buf,INT8U len);
extern void Read_GL23L32S4W(INT32U ADD,INT8U ReadByteNum,INT8U *DataBuffer);
extern void Write_Cont(unsigned long Dst, INT8U *p,INT16U no_bytes);
extern unsigned char Read(unsigned long Dst);
extern void Read_Cont(INT8U *pBuffer,unsigned long Dst, unsigned long no_bytes);
extern void SST25VF_Init(void);	
extern unsigned char Read_ID(uint8_t ID_addr);
extern void Flash_Lock(void);
extern void Flash_Unlock(void);

extern void FRAM_TEST(void);

extern uint8_t manID,devID;

#endif


