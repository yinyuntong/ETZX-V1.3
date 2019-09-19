#ifndef _SST25VFXXX_H_
#define _SST25VFXXX_H_

#define record_start_addr
#define max

#define READ            0x03
#define FAST_READ		0x0b
#define ERASE_SECTOR_4K	0x20
#define BLOCK_ERASE_32K 0x52
#define BLOCK_ERASE_64K	0xd8

#define ERASE_CHIP      0x60
#define BYTE_PROGRAM    0x02
#define AAI             0xad
#define RDSR            0x05
#define EWSR            0x50
#define WRSR            0x01
#define WREN            0x06
#define WRDI            0x04
#define EBSY            0x70
#define DBSY            0x80
#define Read_ID			0x90


#ifndef _IN_SST25VFXXX_
extern void erase_blk_64k(uint8_t);
extern void erase_sector_4k(uint16_t);
extern void init_sst25vfxxx_spi(void);
extern void erase_cp(void);
extern void erase_chip(void);
extern uint8_t spi_send_data_sim(uint8_t);
extern uint8_t spi_send_data_hard(uint8_t);
extern void init_sst25vf040_mode0(void);
extern void enable_write(void);
extern void AAI_program(uint8_t *,uint32_t,uint16_t);
extern void SST_func(void);
extern void test_sst25vfxxx(void);
extern uint8_t spi_get_statu(void);
extern void wait_internal_operation_finish(void);

extern uint16_t sst_read(uint8_t *,uint32_t,uint16_t);
extern uint16_t sst_fast_read(uint8_t *ptr,uint32_t addr,uint16_t len);
extern uint16_t sst_write(uint8_t *,uint32_t,uint16_t);
extern void read_id(void);

extern uint8_t man_id;	//制造商ID
extern uint8_t dev_id;	//设备ID


#endif




#endif
