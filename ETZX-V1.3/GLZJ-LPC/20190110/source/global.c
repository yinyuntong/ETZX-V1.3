#define _IN_GLOBAL_

#include"custom.h"


volatile uint8_t		little_voice_effective=0;	 //小音量有效
volatile uint8_t		blight_off_state=0;		 	 //背光关状态


volatile uint16_t		ring_val;
volatile uint8_t		ring_fre_val;
volatile uint8_t		ring_tlp_chk;
volatile uint8_t		ring_tlp_picked;
volatile uint8_t		ring_tlp_release;

volatile uint8_t		mt8870_received=0;

volatile uint8_t 		voice_change_flag;


uint16_t para_rst_key_time= PARA_RST_KEY_TIME+1;
uint8_t  soft_rst_key_time= SOFT_RST_KEY_TIME+1;



uint8_t KeyState=0;			//LED与按键的状态


void short_soft_delay(uint16_t);
void init_my_val(void);
void init_scon1_rcvbuf(uint8_t);
void init_scon2_rcvbuf(uint8_t);
void init_scon3_rcvbuf(uint8_t);

void close_speak(void);
void open_speak(void);
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void short_soft_delay(uint16_t ch)	  //ch=190    10us
{
  for(;;)
  {
    ch--;
	if(0==ch)
	{
	  break;
	}
  }
}

/*********************************************************************************************
*
*
*延时0.05us
*********************************************************************************************/
void short_delay(uint16_t i)
{
  do{
  }while(--i);
}

/*********************************************************************************************
*																								 
*
*
*********************************************************************************************/
void soft_delay_half_ms(uint16_t ch)
{
  	uint8_t i;
  	do
	{ 
  		for(i=0;i<50;i++)
    		short_soft_delay(190);

		#ifdef	enable_wdt
    	feed_wdt();
    	#endif
  	}while(--ch);
}

/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_my_val()
{
  scon1_time = 0;
  scon1_tov = 0;
  scon1_received = 0;
  scon1_len = 0;

  scon2_time = 0;
  scon2_tov = 0;
  scon2_received = 0;
  scon2_len = 0;

  scon3_time = 0;
  scon3_tov = 0;
  scon3_received = 0;
  scon3_len = 0;

  uart3_auto_time = 0; 
  
  voice_turns_delay_time = 0;

  init_scon1_rcvbuf(0);
  init_scon2_rcvbuf(0);
  init_scon3_rcvbuf(0);

  voice_play_turns = 0;
  voice_play_times = 0;

  played_times = 0;
  played_turns = 0;

  ring_tlp_chk = 0;
  ring_tlp_picked = 0;
  ring_tlp_release = 0;

}




/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_scon1_rcvbuf(uint8_t ch)
{
  uint16_t i;
  for(i=0;i<256;i++)
  {
    scon1_Rcvbuf[i] = ch;
  }
  scon1_ptr = scon1_Rcvbuf;
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_scon2_rcvbuf(uint8_t ch)
{
  uint16_t i;
  for(i=0;i<256;i++)
  {
    scon2_Rcvbuf[i] = ch;
  }
  scon2_ptr = scon2_Rcvbuf;
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_scon3_rcvbuf(uint8_t ch)
{
  uint16_t i;
  for(i=0;i<256;i++)
  {
    scon3_Rcvbuf[i] = ch;
  }
  scon3_ptr = scon3_Rcvbuf;
}



void channel_pin_config(void)
{
	//信号线上24V电源
	LPC_PINCON->PINSEL3 &= (~(0x03 << 18)); 			// P1.25  power_en 常态为1
	LPC_GPIO1->FIODIR    |= power_en_pin;
	power_en_ctrl(1);

	//广播时增加负载
	LPC_PINCON->PINSEL7 &= (~(0x03 << 20)); 			// P3.26  gb_fz   常态为0
	LPC_GPIO3->FIODIR    |= gb_fz_pin;
	gb_fz_ctrl(0);	

	//通话时SD通道打开
	LPC_PINCON->PINSEL7 &= (~(0x03 << 18)); 			// P3.25  xtd   常态为0
	LPC_GPIO3->FIODIR    |= xtd_pin;
	xtd_ctrl(0);		

	//通话时SA通道打开
	LPC_PINCON->PINSEL3 &= (~(0x03 << 8)); 				// P1.20  xta  常态为0
	LPC_GPIO1->FIODIR    |= xta_pin;
	xta_ctrl(0);

	//播放背景音乐时打开SA 通道
	LPC_PINCON->PINSEL3 &= (~(0x03 << 10)); 			// P1.21  KBD  常态为0
	LPC_GPIO1->FIODIR    |= kbd_pin;
	kbd_ctrl(0);

	//语音报号时打开SD 通道
	LPC_PINCON->PINSEL3 &= (~(0x03 << 14)); 			// P1.23  KGB  常态为0
	LPC_GPIO1->FIODIR    |= kgb_pin;
	kgb_ctrl(0);	

	//语音报号时打开SA 通道
	LPC_PINCON->PINSEL3 &= (~(0x03 << 16)); 			// P1.24  KBC  常态为0
	LPC_GPIO1->FIODIR    |= kbc_pin;
	kbc_ctrl(0);	
		
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void open_speak()
{

}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void close_speak()
{	

}
