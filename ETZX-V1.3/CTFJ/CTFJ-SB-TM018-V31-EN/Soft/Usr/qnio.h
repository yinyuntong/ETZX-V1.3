#ifndef _QNIO_H_
#define _QNIO_H_



/* modify this according to I2C device address when you use standard I2C function like I2C_XXXX except AI2C_XXXX*/
#define I2C_DEV0_ADDRESS 0x58				//qN8027–¥µÿ÷∑
#define I2C_TIMEOUT_TIME    10
#define I2C_TIMEOUT_COUNT    8
#define MS_DELAY_CONST    40
/************end*********************/



/****************************************************************************************************************************
** The following routines is for LPC214X ARM I/O definition. It is just as reference for program using 2-wire and 3-wire bus.
** Pls make a further revision for your routines according to the following type.
****************************************************************************************************************************/
#define SCL_1   (SCL=1)
#define SCL_0   (SCL=0)

#define SDA_1   (SDA=1)
#define SDA_0   (SDA=0)

//#define CEN_1   (CEN=1)
//#define CEN_0   (CEN=0)

/***************************************************************************************************/
extern void Msdelay(uint dly);
extern uint8 ChipReset(uint8 Slave) ;



extern uint8 QND_ReadReg(uint8 adr);
extern uint8 QND_WriteReg(uint8 adr, uint8 value);




/** the following functions is for other I2C devices rather than QN800X ***/
extern uint8 QND_I2C_WRITE(uint8 Regis_Addr,uint8 Data);
extern uint8 QND_I2C_READ(uint8 Regis_Addr);
extern uint8 QND_I2C_NREAD(uint8 Regis_Addr, uint8 *buf, uint8 n);
extern uint8 QND_I2C_NWRITE(uint8 Regis_Addr, uint8 *buf, uint8 n);

#endif

