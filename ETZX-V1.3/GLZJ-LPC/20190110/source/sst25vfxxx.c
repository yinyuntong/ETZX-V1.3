#define _IN_SST25VFXXX_

#include"custom.h"


void erase_blk_64k(uint8_t);
void erase_sector_4k(uint16_t);
void init_sst25vfxxx_spi(void);
void erase_cp(void);
void erase_chip(void);
uint8_t spi_send_data_sim(uint8_t);
uint8_t spi_send_data_hard(uint8_t);
void init_sst25vf040_mode0(void);
void enable_write(void);
void AAI_program(uint8_t *,uint32_t,uint16_t);
void SST_func(void);
void test_sst25vfxxx(void);
uint8_t spi_get_statu(void);
void wait_internal_operation_finish(void);

uint16_t sst_read(uint8_t *,uint32_t,uint16_t);
uint16_t sst_write(uint8_t *,uint32_t,uint16_t);

void read_id(void);

uint8_t man_id=0;	//制造商ID
uint8_t dev_id=0;	//设备ID

/*********************************************************************************************
*sst25vf032 contant 2048 of 4k
*
*
*********************************************************************************************/
void erase_sector_4k(uint16_t Dst)
{
  SST_func();
  sst_cs_ctrl(0);
  short_soft_delay(200);
  enable_write();					
  sst_cs_ctrl(0);					/* enable devispiCe */
  short_soft_delay(200);
  spi_send_data_hard(ERASE_SECTOR_4K);			/* send 64 KByte Block Erase command */
  spi_send_data_hard(Dst>>4); 	/* send 3 address bytes */
  spi_send_data_hard(Dst<<4);
  spi_send_data_hard(0);
  sst_cs_ctrl(1);					/* disable devispiCe */
  //soft_delay_half_ms(60);
  wait_internal_operation_finish();
}

/*********************************************************************************************
*sst25vf032 contant 128 of 64k
*
*
*********************************************************************************************/
void erase_blk_64k(uint8_t Dst)
{
  SST_func();
  sst_cs_ctrl(0);
  short_soft_delay(200);
  enable_write();					
  sst_cs_ctrl(0);					/* enable devispiCe */
  short_soft_delay(200);
  spi_send_data_hard(BLOCK_ERASE_64K);			/* send 64 KByte Block Erase command */
  spi_send_data_hard(Dst); 	/* send 3 address bytes */
  spi_send_data_hard(0);
  spi_send_data_hard(0);
  sst_cs_ctrl(1);					/* disable devispiCe */
  //soft_delay_half_ms(200);
  wait_internal_operation_finish();
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void erase_cp()	  //扇区檫除只有在BPx全部为零时才能进行	  上电后BPx全部为1
{	
  enable_write();					
  sst_cs_ctrl(0);				
  spi_send_data_hard(ERASE_CHIP);			//60h or C7h
  sst_cs_ctrl(1);					/* disable devispiCe */
  //soft_delay_half_ms(250);
  wait_internal_operation_finish();
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void erase_chip()
{
  SST_func();
  erase_cp();
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void SST_func()
{
  sst_cs_ctrl(0);
  short_soft_delay(200);
  spi_send_data_hard(EWSR);   //状态寄存器写使能
  sst_cs_ctrl(1);
  short_soft_delay(200);

  sst_cs_ctrl(0);
  short_soft_delay(200);
  spi_send_data_hard(WRSR);   //写状态寄存器
  spi_send_data_hard(0x00);   //将全部存储空间设置为可写
  sst_cs_ctrl(1);
  short_soft_delay(200);
}
/******************************************************************************
**SPI数据发送
**data是要发送的数据
**成功返回1，失败返回0
******************************************************************************/
uint8_t spi_send_data_hard(uint8_t data)
{
    uint8_t temp;
	LPC_SPI->SPDR = data;                                               // 发送该数据用以产生时钟         
    while ( 0 == (LPC_SPI->SPSR & 0x80));                               // 等待数据发送完毕               
    temp = LPC_SPI->SPSR;                                               // 清除SPIF标志                   
	temp = temp;														//消除编译器warning
    return LPC_SPI->SPDR;                                      			// 返回接收到的数据               
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
/*取得状态寄存器的值*/
uint8_t spi_get_statu()
{
  uint8_t ch;
  sst_cs_ctrl(0);
  spi_send_data_hard(RDSR);		/* send RDSR command */	  //读状态寄存器
  ch=spi_send_data_hard(0xff);
  sst_cs_ctrl(1);
  return ch;
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void enable_write()
{
  sst_cs_ctrl(0);
  short_soft_delay(200);
  spi_send_data_hard(WREN);	  //写使能
  sst_cs_ctrl(1);
  short_soft_delay(200);
}

/************************************************************************/
/* PROCEDURE: read_id													*/
/*																		*/
/* This procedure Reads the manufacturer's ID and device ID.  It will 	*/
/* use 90h or ABh as the command to read the ID (90h in this sample).   */
/* It is up to the user to give the last byte ID_addr to determine      */
/* whether the device outputs manufacturer's ID first, or device ID 	*/
/* first.  Please see the product datasheet for details.  Returns ID in */
/* variable byte.														*/
/*																		*/
/* Input:																*/
/*		ID_addr															*/
/*																		*/
/* Returns:																*/
/*		byte:	ID1(Manufacture's ID = BFh or Device ID = 8Dh)			*/
/*																		*/
/************************************************************************/
void read_id(void)
{
	sst_cs_ctrl(0);				/* enable device */
	spi_send_data_hard(0x90);		/* send read ID command (90h or ABh) */
    spi_send_data_hard(0x00);		/* send address */
	spi_send_data_hard(0x00);		/* send address */
	spi_send_data_hard(0);		/* send address - either 00H or 01H */
	man_id = spi_send_data_hard(0xff);		/* receive byte */
	dev_id = spi_send_data_hard(0xff);
	sst_cs_ctrl(1);				/* disable device */
}



/*********************************************************************************************
*
*
*
*********************************************************************************************/
void AAI_program(uint8_t *ptr,uint32_t addr,uint16_t lenth)	 //	 must over 5Byte in this Program
{
	uint8_t i;
	uint16_t j;
  if(man_id==0xbf)		//sst系列芯片
  {
		if(dev_id == 0x4a)	//SST25VF032芯片
		{
			enable_write();

			sst_cs_ctrl(0);
			short_soft_delay(200);
			spi_send_data_hard(AAI);
			spi_send_data_hard(addr>>16);
			spi_send_data_hard(addr>>8);
			spi_send_data_hard(addr);

			spi_send_data_hard(*ptr++);
			spi_send_data_hard(*ptr++);   
			sst_cs_ctrl(1);
			lenth -= 2;
			short_soft_delay(200);	  

			do
			{
				sst_cs_ctrl(0);			//
				short_soft_delay(200);
				spi_send_data_hard(AAI);
				spi_send_data_hard(*ptr++);

				lenth--;
				if(lenth)	  spi_send_data_hard(*ptr++);		  //奇偶数判断
				else	   								  //如果是奇数到此处是已为0写如0xff
				{  
					spi_send_data_hard(0xff);  
					sst_cs_ctrl(1);
					short_soft_delay(200);
					break;	  
				}
				sst_cs_ctrl(1);
				short_soft_delay(200);
			}while(--lenth);

			sst_cs_ctrl(0);
			short_soft_delay(4);
			spi_send_data_hard(WRDI);
			sst_cs_ctrl(1);
			//soft_delay_half_ms(200);
			wait_internal_operation_finish();
		}

		else if(dev_id == 0x43)	//SST25VF020芯片
		{
			enable_write();
			sst_cs_ctrl(0);
			short_soft_delay(200);
			spi_send_data_hard(0xaf);
			spi_send_data_hard(addr>>16);
			spi_send_data_hard(addr>>8);
			spi_send_data_hard(addr);

			spi_send_data_hard(*ptr++);  
			sst_cs_ctrl(1);
			lenth --;
			short_soft_delay(200);		
			  
			do
			{
				sst_cs_ctrl(0); 		  //
				short_soft_delay(200);
				spi_send_data_hard(0xaf);
				spi_send_data_hard(*ptr++);
				lenth--;
				sst_cs_ctrl(1);
				short_soft_delay(200);
			}while(lenth);

			sst_cs_ctrl(0);
			short_soft_delay(4);
			spi_send_data_hard(WRDI);
			sst_cs_ctrl(1);
			//soft_delay_half_ms(200);
			wait_internal_operation_finish();
		}
		else
		{
			for(;lenth>0;lenth--)
			{
				enable_write();
				sst_cs_ctrl(0);					/* enable device */
				spi_send_data_hard(BYTE_PROGRAM);			/* send Byte Program command */
				spi_send_data_hard(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
				spi_send_data_hard(((addr & 0xFFFF) >> 8));
				spi_send_data_hard(addr & 0xFF);
				spi_send_data_hard(*ptr);			/* send byte to be programmed */
				sst_cs_ctrl(1);					/* disable device */

				wait_internal_operation_finish();
				addr++;
				ptr++;
			}	
		}
  	}
    else if(man_id == 0xEF)	//华邦系列存储器
    {
    	if(addr%256==0)		//起始地址从页地址开始
		{
			if(lenth<=256)	//长度一页之内
			{
onePageWrite:			
				enable_write();
				sst_cs_ctrl(0);					/* enable device */
				spi_send_data_hard(BYTE_PROGRAM);			/* send Byte Program command */
				spi_send_data_hard(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
				spi_send_data_hard(((addr & 0xFFFF) >> 8));
				spi_send_data_hard(addr & 0xFF);
				for(;lenth>0;lenth--)
				{

					spi_send_data_hard(*ptr);			/* send byte to be programmed */
					ptr++;	
				}
				sst_cs_ctrl(1);					/* disable device */
				wait_internal_operation_finish();				
			}
			else	//长度超 过一页
			{

somePageWrite:
				do
				{
					enable_write();
					sst_cs_ctrl(0);					/* enable device */
					spi_send_data_hard(BYTE_PROGRAM);			/* send Byte Program command */
					spi_send_data_hard(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
					spi_send_data_hard(((addr & 0xFFFF) >> 8));
					spi_send_data_hard(addr & 0xFF);
					j=256;
					for(;j>0;j--)
					{

						spi_send_data_hard(*ptr);			/* send byte to be programmed */
						ptr++;	
					}
					sst_cs_ctrl(1);					/* disable device */
					wait_internal_operation_finish();
					addr += 256;
					lenth -= 256;
					if(lenth<256) goto onePageWrite;
				}while(lenth>=256);
			}
		}
		else	//写地址不是从页起始地址开始
		{
			i= 256-addr%256;	//最开始页可以写的字节个数
			if(lenth<=i) goto onePageWrite;  	//在一页内写完
			else
			{//需要换页
				j=i;
				
				enable_write();
				sst_cs_ctrl(0);					/* enable device */
				spi_send_data_hard(BYTE_PROGRAM);			/* send Byte Program command */
				spi_send_data_hard(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
				spi_send_data_hard(((addr & 0xFFFF) >> 8));
				spi_send_data_hard(addr & 0xFF);				
				for(;i>0;i--)
				{
					spi_send_data_hard(*ptr);			/* send byte to be programmed */
					ptr++;					
				}
				sst_cs_ctrl(1);					/* disable device */
				wait_internal_operation_finish();

				addr += j;
				lenth -= j;

				if(lenth<=256) goto onePageWrite;
				else goto somePageWrite;
			}
		}
	}
	else
	{
		for(;lenth>0;lenth--)
		{
			enable_write();
			sst_cs_ctrl(0);					/* enable device */
			spi_send_data_hard(BYTE_PROGRAM);			/* send Byte Program command */
			spi_send_data_hard(((addr & 0xFFFFFF) >> 16));	/* send 3 address bytes */
			spi_send_data_hard(((addr & 0xFFFF) >> 8));
			spi_send_data_hard(addr & 0xFF);
			spi_send_data_hard(*ptr);			/* send byte to be programmed */
			sst_cs_ctrl(1);					/* disable device */

			wait_internal_operation_finish();
			addr++;
			ptr++;
		}
	}



}


/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_sst25vfxxx_spi()	
{
//#define		sst_cs			(1ul<<16)		//p0.16
//#define		sst_sck			(1ul<<15)		//p0.15
//#define		sst_miso		(1ul<<17)		//p0.17
//#define		sst_mosi		(1ul<<18)		//p0.18

  // 设置硬件SPI的通讯脚  
     LPC_PINCON->PINSEL1  &= ~(0x03ul << 0); 	 			//p0.16  set to gpio
     LPC_PINCON->PINSEL0  |=  (0x03ul << 30);    			// 设置P0.15脚为SCK脚           
     LPC_PINCON->PINSEL1   =  (0x03 << 2) | (0x03 << 4);	//设置P0.17、P0.18引脚为SPI引脚

     // 设置sst25vfxxx的片选引脚 
     LPC_GPIO0->FIODIR    |= sst_cs;
	 sst_cs_ctrl(1);

//     LPC_SPI->SPCCR =  0x40;                                            // 设置SPI时钟分频,可按需求配置    
     LPC_SPI->SPCCR =  0x08;                                            // 设置SPI时钟分频,可按需求配置
     // 设置SPI的工作方式  
     LPC_SPI->SPCR  =   (0 << 2) |                                      // SPI控制器每次传输8位数据     
                        (0 << 3) |                                      // CPHA = 0, SCK第一个时钟沿采样
                        (0 << 4) |                                      // CPOL = 0, SCK 为低有效       
                        (1 << 5) |                                      // MSTR = 1, SPI 处于主模式     
                        (0 << 6) |                                      // LSBF = 0, MSB (位7)在先      
                        (0 << 7);                                       // SPIE = 0, SPI 中断被禁止     
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
uint16_t sst_read(uint8_t *ptr,uint32_t addr,uint16_t len)
{
  uint16_t i;

  i = len;

  sst_cs_ctrl(0);
  short_soft_delay(200);
  spi_send_data_hard(READ);
  spi_send_data_hard(addr>>16);
  spi_send_data_hard(addr>>8);
  spi_send_data_hard(addr);
  do
  {
    *ptr++ = spi_send_data_hard(0xff);
  }while(--i);
  sst_cs_ctrl(1);
  short_soft_delay(200);

  return len;
}

/*********************************************************************************************
*
*
*
*********************************************************************************************/
uint16_t sst_fast_read(uint8_t *ptr,uint32_t addr,uint16_t len)
{
  uint16_t i;

  i = len;

  sst_cs_ctrl(0);
  short_soft_delay(200);
  spi_send_data_hard(FAST_READ);
  spi_send_data_hard(addr>>16);
  spi_send_data_hard(addr>>8);
  spi_send_data_hard(addr);
  spi_send_data_hard(0xff);
  do
  {
  	*ptr++ = spi_send_data_hard(0xff);
  }while(--i);
  sst_cs_ctrl(1);
  short_soft_delay(200);

  return len;
}


/*********************************************************************************************
*
*
*
*********************************************************************************************/
uint16_t sst_write(uint8_t *ptr,uint32_t addr,uint16_t len)
{
  SST_func();
  AAI_program(ptr,addr,len);
  sst_cs_ctrl(1);
  return len;
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void wait_internal_operation_finish()
{
  uint32_t limn=0;
  for(;;)
  {
    if(spi_get_statu() & 0x1){short_soft_delay(190);}
	else		{break;}
	limn++;
	if(limn==0xffffff)
	{
	  break;
	}
	//add//feed_wdt();
  }
}

/*********************************************************************************************
*
*
*
*********************************************************************************************/
void test_sst25vfxxx(void)
{ 
 	uint16_t j;
	uint8_t ram_buf[32];
	
	sst_read(ram_buf,0x00,32);
	for(j=0;j<32;j++)
	{
		uart0_send_byte(ram_buf[j]);
	}
	
}

