#define _IN_QNIO_
#include "config.h"

uint8 qnd_i2c;
uint8 qnd_i2c_timeout;
uint8 i2cMux = 0; //add this gloab temp to solve conflick of call timerRssicheck() and call 2-wire() at the same time

/*****************************************************************************************************
** Name:      Msdelay()
** Function:  time delay
*****************************************************************************************************/



void Msdelay(uint dly)
{ 
    uint8 i;
    for( ; dly>0; dly--)
    for(i=0; i<MS_DELAY_CONST; i++);
}


/*************************************************************************************************
** Name:      I2C_subfunction
** Function:  Software i2c code  
*****************************************************************************************************/
void Set_SCL(uint8 i)      
{
    if(i) 
    {
        SCL_1;
    }
    else  
    { 
        SCL_0;
    }
}

void Set_SDA(uint8 i)      
{
    if(i) 
	{
        SDA_1;
    } 
    else 
    {
        SDA_0;
    }
}

void Start(void)               
{ 
    Msdelay(1);
    Set_SCL(1);
    Msdelay(1);
    Set_SDA(1);
    Msdelay(1);
    Set_SDA(0);
    Msdelay(2);
    Set_SCL(0);
}

void Stop(void)              
{
    Msdelay(1);
    Set_SDA(0);
    Msdelay(1);
    Set_SCL(1);
    Msdelay(2);
    Set_SDA(1);
}

void Send_ACK(uint8 i) //i=0 for ACK and i=1 for Nack
{
    Set_SDA(i);
    Msdelay(1);
    Set_SCL(1);
    Msdelay(1);
    Set_SCL(0);
    Msdelay(1);
    Set_SDA(1);
}
 

void Check_ACK(void)
{
    Msdelay(1);
    Set_SCL(1);
    qnd_i2c_timeout = (uint8)SDA;
    Msdelay(1);
    Set_SCL(0);
}

void Write_Bit(uint8 i)
{
    Msdelay(1);
    Set_SDA(i);
    Msdelay(1);
    Set_SCL(1);
    Msdelay(2);
    Set_SCL(0);
}

void Write_Byte(uint8 Data)
{
    uint8 i;
    for( i = 0;i < 8;i++)
    {
        Write_Bit(Data>>7);
        Data<<=1;
    }
}

uint8 Read_Byte(void)
{
    uint8 Data1=0x00;
    uint8 j;
    for( j = 0;j < 8;j++)
    {
        Msdelay(1);
        Set_SCL(1);
        Data1 = (Data1<<1) | ((uint8)SDA & 0x01);
        Msdelay(1);
        Set_SCL(0);
     }
    return Data1;
}
 
/************************************************************************************************************
** Name: I2C_Write_1byte                         
** Funcation:write a data to a desired            
**           register through i2c bus 
** Description: Slave---------device address
**              Regis_Addr----register address
*************************************************************************************************************/
uint8 I2C_Write_1byte(uint8 Slave,uint8 Regis_Addr,uint8 Data)
{
    uint8 temp;
    temp=Slave;

    Start();
    Write_Byte(temp);
    Check_ACK();
    if (!qnd_i2c_timeout) 
    {
        Write_Byte(Regis_Addr);
        Check_ACK();
        if (!qnd_i2c_timeout) 
        {
            Write_Byte(Data);
            Check_ACK();
        }
    }
    Stop();
    return !qnd_i2c_timeout;
}
 
/***********************************************************************************************************
** Name: I2C_Read_1byte                          
** Function: Read a data from a desired register 
**           through i2c bus 
** Description: Slave---------device address
**              Regis_Addr----register address
************************************************************************************************************/
uint8 I2C_Read_1byte(uint8 Slave,uint8 Regis_Addr)
{ 
    uint8 Data=0x00;
    uint8 temp;
    temp =Slave | 0x01;

    Start();
    Write_Byte(Slave);
    Check_ACK();
    if (!qnd_i2c_timeout) 
    {
        Write_Byte(Regis_Addr);
        Check_ACK();
        if(!qnd_i2c_timeout) 
        {
            //stop();
            Start();
            Write_Byte(temp);
            Check_ACK();
            if (!qnd_i2c_timeout) 
            {
                Data = Read_Byte();
                Send_ACK(1);
            }
        }
    }
    Stop();
    return Data;
}

  /***********************************************************************************************************
  ** Name: I2C_Read_nbyte                            
  ** Function: Read n data from a desired register    
  **           through i2c bus 
  ** Description: Slave---------device address
  **              Regis_Addr----register address
  ************************************************************************************************************/
  uint8 I2C_Read_nbyte(uint8 Slave,uint8 Regis_Addr, uint8 *buf, uint8 n)
  { 
    uint8 Data=0x00;
    uint8 temp;
    uint8 i;
    temp =Slave | 0x01;

    Start();
    Write_Byte(Slave);
    Check_ACK();
    if(!qnd_i2c_timeout) 
    {
        Write_Byte(Regis_Addr);
        Check_ACK();
        if (!qnd_i2c_timeout)
        {
            //stop();
            Start();
            Write_Byte(temp);
            Check_ACK();
            if (!qnd_i2c_timeout)
            {
                for(i=0; i<n-1; i++) {
                    buf[i] = Read_Byte();
                    Send_ACK(0);
                }
                buf[i] = Read_Byte();
                Send_ACK(1);
            }
        }
    }
    Stop();
    return !qnd_i2c_timeout;
}

  /***********************************************************************************************************
    ** Name: I2C_Write_nbyte                
    ** Function: Write n bytes data to register 
    **             through i2c bus 
    ** Description: Slave---------device address
    **                Regis_Addr----register address
    ************************************************************************************************************/
uint8 I2C_Write_nbyte(uint8 Slave,uint8 Regis_Addr, uint8 *buf, uint8 n)
{ 
    uint8 temp;
    uint8 i;
    temp=Slave;

    Start();
    Write_Byte(temp);
    Check_ACK();
    if(!qnd_i2c_timeout) 
    {
        Write_Byte(Regis_Addr);
        Check_ACK();
        for(i=0; (i<n) && !qnd_i2c_timeout; i++)
        {
            Write_Byte(buf[i]);
            Check_ACK();
        }
    }
    Stop();
    return !qnd_i2c_timeout;
}




extern uint8 qnd_DevIndex;


uint8 QND_WriteReg(uint8 Regis_Addr,uint8 Data)
{


    return QND_I2C_WRITE(Regis_Addr,Data);

    return 1;
}


uint8 QND_ReadReg(uint8 Regis_Addr)
{
    uint8 Data;



    Data = QND_I2C_READ(Regis_Addr);

    return Data;
}




uint8 QND_I2C_WRITE(uint8 Regis_Addr,uint8 Data)
{
    uint8 ret;
    uint8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
        ret = I2C_Write_1byte(I2C_DEV0_ADDRESS, Regis_Addr, Data);
        if(ret) break;
    }
    if(!tryCount) {
    SPRINTF(("QND_I2C_WRITE error!\n"));    
        return 0;
    }
    qnd_i2c = 0;
    return 1;
}

uint8 QND_I2C_READ(uint8 Regis_Addr)
{
    uint8 ret;
    uint8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
    ret = I2C_Read_1byte(I2C_DEV0_ADDRESS, Regis_Addr);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) {
    SPRINTF(("QND_I2C_READ error!\n"));    
    }
    qnd_i2c = 0;
    return ret;
}

uint8 QND_I2C_NREAD(uint8 Regis_Addr, uint8 *buf, uint8 n)
{
    uint8 ret;
    uint8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
        ret = I2C_Read_nbyte(I2C_DEV0_ADDRESS, Regis_Addr, buf, n);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) {
    SPRINTF(("QND_I2C_READ error!\n"));    
    }
    qnd_i2c = 0;
    return ret;
}

uint8 QND_I2C_NWRITE(uint8 Regis_Addr, uint8 *buf, uint8 n)
{
    uint8 ret;
    uint8 tryCount = I2C_TIMEOUT_COUNT;
    qnd_i2c = 1;
    while(--tryCount) {
        ret = I2C_Write_nbyte(I2C_DEV0_ADDRESS, Regis_Addr, buf, n);
        if(!qnd_i2c_timeout) break;
    }
    if(!tryCount) {
        SPRINTF(("QND_I2C_READ error!\n"));    
    }
    qnd_i2c = 0;
    return ret;
}



/*************************************************************************************************************
                              end file
 ************************************************************************************************************/