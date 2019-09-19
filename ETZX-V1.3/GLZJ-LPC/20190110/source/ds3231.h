
#ifndef _DS3231_H_
#define _DS3231_H_


/**************************** defines *******************************/
#define ADDRTC  0xd0    /* DS3231 slave address (write) */
#define ACK     0
#define NACK    1


#ifndef _IN_DS3231_
extern uint32_t read_ds3231_delay;
extern void start(void);
extern void stop(void);
extern void DS3231_pincfg(void);
extern uint8_t i2cwrite(uint8_t d);
extern uint8_t i2cread(int8_t b);
extern void writebyte(uint8_t Add,uint8_t Data);
extern void WriteDS3231_time(void);
extern void   ReadDS3231_time(void);
extern void    rd_temp(void);
extern void    frq_out_tog(void);
extern void    init_alrm(void);
extern void   DS3231_comm_init(void);
extern void DS3231_init(void);
extern uint8_t BCDtoHEX(uint8_t bcd_value);
extern uint16_t HEXtoBCD(uint8_t hex_value);

extern void init_black_light_status(void);
extern void check_little_voice_status(void);
extern void check_black_light_status(void);


#endif


#endif

