/***************************************************************************/
/* DEMO3231.C                                                              */
/***************************************************************************/
#define _IN_DS3231_
#include "Custom.h"


uint32_t read_ds3231_delay=0;


/**************************** functions ******************************/
void start(void);
void stop(void);
void DS3231_pincfg(void);
uint8_t i2cwrite(uint8_t d);
uint8_t i2cread(int8_t b);
void writebyte(uint8_t Add,uint8_t Data);
void WriteDS3231_time(void);
void   ReadDS3231_time(void);
void    rd_temp(void);
void    frq_out_tog(void);
void    init_alrm(void);
void   DS3231_comm_init(void);



void delay_nop(uint8_t i)
{
	for(;i>0;i--) 
	{
	}
}



uint8_t BCDtoHEX(uint8_t bcd_value)
{
	uint8_t hex_value;
	hex_value = (bcd_value/16)*10+(bcd_value&0x0f);
	return hex_value;
}

uint16_t HEXtoBCD(uint8_t hex_value)
{
	uint16_t bcd_value=0;
	bcd_value = (hex_value/100)*256;
	hex_value = hex_value%100;
	bcd_value +=  ((hex_value/10)*16 + (hex_value%10));
	return bcd_value;
}




void start(void)            /* --------- Initiate start condition ---------- */
{
	DS3231_SDA_OUTPUT();		//SDA方向为输出 
	delay_nop(100);
	Set_DS3231_SDA;  Set_DS3231_SCL;
	delay_nop(100);
	Clr_DS3231_SDA;
	delay_nop(100);
}
void stop(void)             /* ---------- Initiate stop condition ----------- */
{
	DS3231_SDA_OUTPUT();		//SDA方向为输出 
	delay_nop(50);
    Clr_DS3231_SDA;  Clr_DS3231_SDA;
	delay_nop(50);
    Set_DS3231_SCL;  Set_DS3231_SCL; 
	delay_nop(100);
	Set_DS3231_SDA;
}


void DS3231_pincfg(void)
{
	
   LPC_GPIO2->FIODIR |= (DS3231_SDA| DS3231_SCL|DS3231_RST);	//输出
   //默认全部使用上拉电阻
//   LPC_PINCON->PINMODE0 &= ~(0x03<<DS1302_IO_MOD);		//00:使用上拉电阻
   Set_DS3231_SCL;  
   Set_DS3231_SDA;
   Set_DS3231_RST;

}



//通过IIC写入一个字节的数据
//输入参数: d: 将写入的数据
//输出参数:0-表示从机应答      1-表示从机非应答
uint8_t i2cwrite(uint8_t d)         /* ----------------------------- */
{
	uint8_t i;
	DS3231_SDA_OUTPUT();		//SDA方向为输出 
    Clr_DS3231_SCL;
	delay_nop(50);
    for (i = 0;i < 8; i++)
    {
        if (d & 0x80)
			Set_DS3231_SDA; /* Send the msbits first */
        else
            Clr_DS3231_SDA;
		delay_nop(50);
        Clr_DS3231_SCL;
        Set_DS3231_SCL;
        d = d << 1;     /* do shift here to increase scl high time */
		delay_nop(100);
		Clr_DS3231_SCL;
		delay_nop(50);
    }
	DS3231_SDA_INPUT();		//SDA方向为输入
	delay_nop(50);
    Clr_DS3231_SCL;
    Set_DS3231_SCL;
	delay_nop(50);
    if(DS3231_SDA_status) i=1;
	else i=0;
	delay_nop(50);
    //if (i) printf("Ack bit missing  %02X\n",(unsigned int)d);
    Clr_DS3231_SCL;
    return(i);
}


//通过IIC读取一个字节的数据
//输入参数b=0:表示应答b=1:非应答
//输出参数:读取的数据
uint8_t i2cread(int8_t b)   /* ----------------------------------- */
{
	uint8_t i, d;
	DS3231_SDA_INPUT();		//SDA方向为输入
    d = 0;
    Clr_DS3231_SCL;
	delay_nop(100);
    for (i = 0; i < 8; i++) /* read the msb first */
    {
            Set_DS3231_SCL;
            d = d << 1;
			if(DS3231_SDA_status) d=d|0x01;
            //d = d | (unsigned char)sda;
            delay_nop(100);
            Clr_DS3231_SCL;
			delay_nop(100);
    }
	DS3231_SDA_OUTPUT();		//SDA方向为输出
	if(b)	Set_DS3231_SDA;
	else    Clr_DS3231_SDA;
    //sda = b;          /* low for ack, high for nack */
	delay_nop(50);
    Set_DS3231_SCL;
	delay_nop(100);
    Clr_DS3231_SCL;
    return d;
}



void writebyte(uint8_t Add,uint8_t Data)        /* ----------------------------------------------- */
{
    start();
    i2cwrite(ADDRTC);
    i2cwrite(Add);
    i2cwrite(Data);
    stop();
}



void WriteDS3231_time(void)		//所有的时间都是2000年之后的
{
	uint8_t week=1;		//得不到有效的周数据，
	//时间数据都是BCD码

	if(stTime.byYearL>0x99 || stTime.byMonth>0x12 || stTime.byDay>0x31 || 
	   stTime.byHour>0x23 || stTime.byMinute>0x59 || stTime.bySecond>0x59)
	   return;		//有错的时间数据，直接返回

	
    start();
    i2cwrite(ADDRTC);       /* write slave address, write 1339 */
    i2cwrite(0x00); /* write register address, 1st clock register */
    i2cwrite(stTime.bySecond);
    i2cwrite(stTime.byMinute);
    i2cwrite(stTime.byHour);
    i2cwrite(week);	
    i2cwrite(stTime.byDay);
    i2cwrite(stTime.byMonth|0x80);  //所有的时间都是2000年之后的
    i2cwrite(stTime.byYearL);
 //   i2cwrite(0x10); /* enable sqw, 1hz output */
    stop();

}

uint8_t ReadByte(uint8_t Add)
{
	uint8_t dat;
	start();
	i2cwrite(ADDRTC);
	i2cwrite(Add);
	start();
	i2cwrite(ADDRTC | 1);
	dat = i2cread(NACK);
	stop();
	return dat;
}




//读取DS3231中的时间
void   ReadDS3231_time(void)    
{
	uint8_t month;
	uint8_t week;
	start();
	i2cwrite(ADDRTC);
	i2cwrite(0);
	start();
	i2cwrite(ADDRTC | 1);
	stTime.bySecond = (i2cread(ACK)&0x7f);
	stTime.byMinute = (i2cread(ACK)&0x7f);
	stTime.byHour = (i2cread(ACK)&0x3f);
	week = (i2cread(ACK)&0x07);	
	stTime.byDay = 	(i2cread(ACK)&0x3f);
	stTime.byMonth = (i2cread(ACK)&0x9f);
	stTime.byYearL = i2cread(NACK);
	stop();


	if(stTime.byMonth & 0x80) byYearH = 0x20;
	else byYearH = 0x19;


	stTime.byMonth &= 0x7f;		//将最高位清0


/*	uart2_send_byte(byYearH);
	uart2_send_byte(stTime.byYearL);
	uart2_send_byte(stTime.byMonth);
	uart2_send_byte(stTime.byDay);
	uart2_send_byte(stTime.byHour);
	uart2_send_byte(stTime.byMinute);
	uart2_send_byte(stTime.bySecond);*/

}



uint8_t ReadDS3231Status(void)
{
	uint8_t status;
	start();
	i2cwrite(ADDRTC);
	i2cwrite(0x0F);
	start();
	i2cwrite(ADDRTC | 1);
	status = i2cread(NACK);
	stop();
	return status;
}

//读取DS3231中的温度
void    rd_temp(void)       /* -------- display temperature -------- */
{
	int     itemp;
	float   ftemp;

    do
    {
        start();
        i2cwrite(ADDRTC);
        i2cwrite(0x0e);         /* address of control register */
        start();
        i2cwrite(ADDRTC + 1);   /* send the device address for read */
        itemp = i2cread(NACK);  /* get the control register value */
        stop();
    }while(itemp & 0x20);            /* wait until CNVT bit goes inactive */

    start();
    i2cwrite(ADDRTC);
    i2cwrite(0x11);                 /* address of temperature MSB */
    start();
    i2cwrite(ADDRTC + 1);           /* send the device address for read */
    itemp = ( (int) i2cread(ACK) << 5 );
    itemp += ( i2cread(NACK) >> 3);
    stop();
    if(itemp & 0x1000)      itemp += 0xe000;        /* if sign bit set, make 16 bit 2's comp */

    ftemp = 0.03125 * (float) itemp;        /* convert to degrees C */
    /* ftemp = ftemp * 9 / 5 + 32;  /* skip this if you don't want degrees F */

    //sprintf(str, "%5.2f", ftemp);
}



void    frq_out_tog(void)   /* --- toggle en32khz bit to enable/disable sqw --- */
{
	uint8_t   val;

    start();
    i2cwrite(ADDRTC);
    i2cwrite(0x0f);                 /* control/status reg address */
    start();
    i2cwrite(ADDRTC + 1);           /* send the device address for read */
    val = i2cread(NACK);
    stop();
    val ^= 0x08;    /* toggle en32khz bit */
    start();
    i2cwrite(ADDRTC);
    i2cwrite(0x0f);                 /* control/status reg address */
    i2cwrite(val);
    stop();
}





void    init_alrm(void)     /* --- enable alarm 1 for once-per-second --- */
{
    start();
    i2cwrite(ADDRTC);
    i2cwrite(7);            /* 1st alarm 1 reg address */
    i2cwrite(0x80); /* mask alarm register */
    i2cwrite(0x80);
    i2cwrite(0x80);
    i2cwrite(0x80);
    stop();

    start();
    i2cwrite(ADDRTC);
    i2cwrite(0x0e); /* control/status reg address */
    i2cwrite(0x05); /* enable interrupts, alarm 1 output */ 
}



void   DS3231_comm_init(void)     /* ------ reset DS3231 comm interface ------ */
{
	
    do /* because the DS3231 I2C interface is active for both supplies */
    {  /*  after a micro reset, we must get the comm into a known state */
    	DS3231_SDA_OUTPUT();		//SDA方向为输出
        Set_DS3231_SDA;        /* make sure master has released SDA */
        Set_DS3231_SCL;
		DS3231_SDA_INPUT();		//SDA方向为输入
		if(DS3231_SDA_status)
        //if(sda) /* if sda is high, generate a start */
        {
        	DS3231_SDA_OUTPUT();		//SDA方向为输出
            Clr_DS3231_SDA;        /* The DS3231 will recognize a valid start */
            Set_DS3231_SDA;        /*  condition anywhere in a I2C data transfer */
        }
        Clr_DS3231_SCL;
		DS3231_SDA_INPUT();		//SDA方向为输入
    //}while(sda == 0);        /* if the DS3231 is holding sda low, try again */
    }while(DS3231_SDA_status == 0);        /* if the DS3231 is holding sda low, try again */
}



void DS3231_init(void)
{
	DS3231_pincfg();
	DS3231_comm_init();
	
	ReadDS3231Status();	
	
	//读取DS3231中的时间
	ReadDS3231_time();	

	start();
	i2cwrite(ADDRTC);
	i2cwrite(0x0f);
	i2cwrite(0);			/* clear alarm flags */
	stop();


		
	if((stTime.byYearL > 0x99)||(stTime.byMonth > 0x12)||(stTime.byMonth==0x00)\
	||(stTime.byDay > 0x31)||(stTime.byDay == 0x00)||(stTime.byHour > 0x23) \
	||(stTime.byMinute >0x59)||(stTime.bySecond >0x59))
	{		

		byYearH = 0x20;
		stTime.byYearL = 0x18;
		stTime.byMonth = 0x01;
		stTime.byDay = 0x29;
		stTime.byHour = 0x15;
		stTime.byMinute = 0x08;
		stTime.bySecond	= 0x58;  //初始化系统为2018.1.29.15:08:58
		
		WriteDS3231_time();
		
	}	


	read_ds3231_delay= (60- BCDtoHEX(stTime.bySecond))*2000;
}


/*********************************************************************************************/
/*                                                                                                                                                                                                        */
/* 检查小音量时段　                                                                                                                                                    */
/*********************************************************************************************/
void check_little_voice_status()
{

	uint16_t		little_voice_start; 	  //小音量起始时间
	uint16_t		little_voice_end;		  //小音量结束时间

	uint16_t 		current_time;				//当前时间
	
	uint8_t result=0;

	little_voice_effective = 0;	 //小音量值标志

	//小音量时段起始、结束时间都是BCD码，实时时间也是BCD码	
	little_voice_start = BCDtoHEX(stEepromCfgData.byLVoiceStartHour)*60 + BCDtoHEX(stEepromCfgData.byLVoiceStartMin);
	little_voice_end  = BCDtoHEX(stEepromCfgData.byLVoiceEndHour)*60 + BCDtoHEX(stEepromCfgData.byLVoiceEndMin);

	if(little_voice_start != little_voice_end)
	{
		current_time= BCDtoHEX(stTime.byHour)*60+ BCDtoHEX(stTime.byMinute);
		
		if(little_voice_start>little_voice_end)
		{
			if( ((current_time>=0)&&(current_time<little_voice_end)) || ((current_time>=little_voice_start)&&(current_time<=1439)) )
			{
				result = 1;
			}
		}
		else
		{
			if((current_time>=little_voice_start)&&(current_time<little_voice_end)) 
			{
				result = 1;
			}
		}
		if(0 != result)
		{
			little_voice_effective = 1;

		}		
	}

}


/***********************************************************************************************/
/*																										*/
/*
/*检查背光关时间段　																			*/																		
/**********************************************************************************************/
void check_black_light_status()		
{

	uint16_t		blight_off_start; 	  //背光关起始时间
	uint16_t		blight_off_end;		  //背光关结束时间

	uint16_t		current_time;				//当前时间
	
	uint8_t 		result=0;


	//小音量时段起始、结束时间都是BCD码，实时时间也是BCD码		
	blight_off_start = BCDtoHEX(stEepromCfgData.byBLOffStartHour)*60 + BCDtoHEX(stEepromCfgData.byBLOffStartMin);
	blight_off_end  = BCDtoHEX(stEepromCfgData.byBLOffEndHour)*60 + BCDtoHEX(stEepromCfgData.byBLOffEndMin);

	if(blight_off_start != blight_off_end)
	{
		current_time= BCDtoHEX(stTime.byHour)*60+ BCDtoHEX(stTime.byMinute);

		if(blight_off_start > blight_off_end)
		{
			if( ((current_time>=0)&&(current_time<blight_off_end)) || ((current_time>=blight_off_start)&&(current_time<=1439)) )
			{
				result = 1;
			}
		}
		else
		{
			if((current_time>=blight_off_start)&&(current_time<blight_off_end)) 
			{
				result = 1;
			}
		}
		if(0 != result)
		{//在背光关时段内
			if(blight_off_state ==0)
			{//之前是打开状态，发送一次关闭命令
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_CLOSE_LCD;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));				
			}
		}
		else
		{//不在背光关时段内
			if(blight_off_state ==1)
			{//之前是关闭状态，发送一次打开命令
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_OPEN_LCD;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));				
			}

		}
	}
	else 
	{
		if(blight_off_state ==1)
		{//之前是关闭状态，发送一次打开命令
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = CMD_OPEN_LCD;
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));				
		}	
	}

}




void init_black_light_status()		
{

	uint16_t		blight_off_start; 	  //背光关起始时间
	uint16_t		blight_off_end;		  //背光关结束时间

	uint16_t		current_time;				//当前时间
	
	uint8_t 		result=0;


	//小音量时段起始、结束时间都是BCD码，实时时间也是BCD码		
	blight_off_start = BCDtoHEX(stEepromCfgData.byBLOffStartHour)*60 + BCDtoHEX(stEepromCfgData.byBLOffStartMin);
	blight_off_end  = BCDtoHEX(stEepromCfgData.byBLOffEndHour)*60 + BCDtoHEX(stEepromCfgData.byBLOffEndMin);

	if(blight_off_start != blight_off_end)
	{
		current_time= BCDtoHEX(stTime.byHour)*60+ BCDtoHEX(stTime.byMinute);

		if(blight_off_start > blight_off_end)
		{
			if( ((current_time>=0)&&(current_time<blight_off_end)) || ((current_time>=blight_off_start)&&(current_time<=1439)) )
			{
				result = 1;
			}
		}
		else
		{
			if((current_time>=blight_off_start)&&(current_time<blight_off_end)) 
			{
				result = 1;
			}
		}
		if(0 != result)
		{//在背光关时段内
			//发送关闭命令
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = CMD_CLOSE_LCD;
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));		
		}
		else
		{//不在背光关时段内
			//发送打开命令
			stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd = CMD_OPEN_LCD;
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));				

		}
	}
	else
	{
		//发送打开命令
		stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
		stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
		stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
		stBusFreq.byCmd = CMD_OPEN_LCD;
		stBusFreq.byRecSecAddr = 0xff;
		stBusFreq.byRecRoomAddr = 0xff;
		stBusFreq.byRecBedAddr = 0xff;
		Bus0OutputData(&(stBusFreq.bySndSecAddr));	
	}

}





