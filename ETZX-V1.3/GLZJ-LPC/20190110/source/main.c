#include "custom.h"

uint8_t gb_fz_status=0;

void int2str(int n, char *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    int temp = n < 0 ? -n: n;  // temp为n的绝对值

   if (str == NULL)
   {
       return;
   }
   while(temp)
   {
       buf[i++] = (temp % 10) + '0';  //把temp的每一位上的数存入buf
       temp = temp / 10;
   }

   len = n < 0 ? ++i: i;  //如果n是负数，则多需要一位来存储负号
   str[i] = 0;            //末尾是结束符0
   while(1)
   {
       i--;
       if (buf[len-i-1] ==0)
       {
           break;
       }
       str[i] = buf[len-i-1];  //把buf数组里的字符拷到字符串
   }
   if (i == 0 )
   {
       str[i] = '-';          //如果是负数，添加一个负号
   }
}



void led_pin_config(void)
{
	//WIFI AP LED
	LPC_PINCON->PINSEL0 &= (~(0x03 << 14)); 			// P0.7  作为wifi  ap_led
	LPC_GPIO0->FIODIR	 |= wifi_ap_led_pin;				//用作输出
	
	wifi_ap_led_ctrl(AP_LED_OFF);
	ClrWpaState(WIFI_AP_LED);

	//WIFI SERVER
	LPC_PINCON->PINSEL0 &= (~(0x03 << 12)); 			// P0.6  作为wifi server_led
	LPC_GPIO0->FIODIR	 |= wifi_server_led_pin;				//用作输出

	wifi_server_led_ctrl(SERVER_LED_OFF);
	ClrWpaState(WIFI_SERVER_LED);
	
}


/*********************************************************************************************************
** Function name:       main
** Descriptions:        以中断方式接收上位机数据并将接收的数据回送给上位机。短接JP14,借助串口调试软件，
**                      并设置波特率为9600，8位数据位，1位停止位，注意不要选中"HEX显示"
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main (void)
{ 
	uint32_t i;
	uint8_t j;
	
	SystemInit();                     //系统初始化 
	esp8266_pin_config();

	led_pin_config();

	//电话机电压选择
	LPC_PINCON->PINSEL0 &= (~(0x03 << 10)); 			// P0.5  KBC  常态为0
	LPC_GPIO0->FIODIR    |= tel_ctr_pin;
	kbc_ctrl(DC60V);


	LPC_PINCON->PINSEL3 &= (~(0x03 << 26)); 			// P1.29  cgb   常态为0  　　禁止通道
	LPC_PINCON->PINMODE3 |= (0X02<<26);					//不允许片内上拉或下拉
	LPC_PINCON->PINMODE_OD1 |= cgb_pin;					//开漏模式	
	LPC_GPIO1->FIODIR    |= cgb_pin;					//输出方式
	cgb_ctrl(0);


	LPC_PINCON->PINSEL0 &= (~(0x03 << 0)); 			// P0.0   cbd   常态为0  　　禁止通道
	LPC_PINCON->PINMODE0 |= (0X02<<0);					//不允许片内上拉或下拉
	LPC_PINCON->PINMODE_OD0 |= cbd_pin;					//开漏模式
	LPC_GPIO0->FIODIR    |= cbd_pin;					//输出方式
	cbd_ctrl(0);


	fm62429_pincfg();

  
	stCallPointer.pstHead = NULL;
	stCallPointer.pstTail = NULL;
	
	stIndicationData.pstNext = NULL;

	init_uart0(57600);					//发送485数据
	init_uart1(115200);					//WIFI
	init_uart2(57600);					//上位机
	init_uart3(1200);					//来电显示芯片


	AM79R70_pincfg();
/*	for(i=0;i<10;i++)	byUsart0SndBuf[i]=i;
	stUsartCfg[0].uiSndLen =10;
	stUsartCfg[0].uiSndPos = 0;
	LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
	LPC_UART0->IER   |= 0x02;*/


	init_sst25vfxxx_spi();  			//外部E2P接口初始化
	read_id();
	
	dac_pin_config();

	set_play_voice_volue(PLAY_HOST_CH,28);
	set_play_voice_volue(PLAY_SLAVE_CH,0);

	set_talk_voice_volue(TALK_HOST_CH,0);
	set_talk_voice_volue(TALK_SLAVE_CH,0);

	init_timer0();
	init_timer1();
	init_timer2();

	channel_pin_config();

	SingleBusInit();


//	frame_test();

	init_parameter();
  	init_uart0(stEepromCfgData.uiRs485Brt);					//发送485数据


	
	voice_play_word(DTMFSTART_NUM);
	//load_voice_play_sequence(0,101,23);


/*	uart2_send_byte(0xaa);

	uart1_send_byte(0x0a);	//换行
	uart1_send_str("AT+GMR");
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行*/

#ifdef	  enable_wdt				//开狗
	init_wdt();
#endif

	init_tel();
	init_msm7512();

	init_mt8870();
	eint2Init();			//用于MT8870信号输入

	DS3231_init();

	init_phone_key_buf();

	init_black_light_status();

	if(stEepromCfgData.bySysMode & USE_USART2_MODE)
	{//只允许串口，亮黄灯
		wifi_ap_led_ctrl(AP_LED_ON);
		wifi_server_led_ctrl(SERVER_LED_ON);
	}

	if(stEepromCfgData.bySysMode & USE_WIFI_MODE)
	{
		wifi_ap_led_ctrl(AP_LED_OFF);
		wifi_server_led_ctrl(SERVER_LED_OFF);	
		init_ssid();
		get_right_ssid(ssid,stEepromCfgData.bySelfSecAddr,0x00,0x00);
		uart2_send_str(ssid);


		ssid[16]=8;		
		ssid[17]=2;		
		ssid[18]=7;		
		ssid[19]=3;		
		ssid[20]=6;		
		ssid[21]=5;		
		ssid[22]=9;		
		ssid[23]=6;	
		unsigned int crc = crc32(0xffffffff,(unsigned char *)ssid,24);		
		ssid[16]=0;	

		init_pwd();
		sprintf(pwd,"%08X",crc);
		uart2_send_str(pwd);
		uart2_send_byte(0x0d);	//回车
		uart2_send_byte(0x0a);	//换行	


		esp8266_rst();
		soft_delay_half_ms(1000);

		
		init_esp8266();

		check_esp8266_time=CHECK_WIFI_TIME;
	}


/*	for(i=0;i<10;i++)
	{
		for(j=0;j<20;j++) byUsart1SndBuf[j]=i*0x20+j;	
		wifi_send_data(byUsart1SndBuf,20);
	}*/


//	set_play_voice_volue(PLAY_HOST_CH,0);
	//对全体设备复位
	memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
	stBusFreq.byCmd = CMD_POWER_ON;
	stBusFreq.byRecSecAddr = 0xff;
	stBusFreq.byRecRoomAddr = 0xff;
	stBusFreq.byRecBedAddr = 0xff;
	Bus0OutputData(&(stBusFreq.bySndSecAddr));

	for(;;)
	{	

		#ifdef	enable_wdt
	    feed_wdt();
	    #endif	

		if(voice_play_state & VOICE_PLAY_READ_EEPROM_DATA)
		{
			voice_play_state &= (~VOICE_PLAY_READ_EEPROM_DATA);


			if(voice_data_total_len < VOICE_BUFF_MAX)
			{
				voice_data_len = voice_data_total_len;
				voice_data_total_len =0;
			
			}
			else	//一个字音频数据比较大,超过最大缓冲器
			{
				voice_data_total_len  -= VOICE_BUFF_MAX;
				voice_data_len = VOICE_BUFF_MAX;
			}
			

			sst_read((uint8_t *)voice_buff,voice_data_addr,voice_data_len);
			pvoice_data =(int16_t *)voice_buff;
			

	
			voice_data_addr += VOICE_BUFF_MAX;
			
			
			voice_play_state |= VOICE_PLAY_START;
			voice_data_len /= 2;
			
			voice_play_state |= VOICE_PLAY_ONE_WORD;	
		}



		if(voice_play_state & VOICE_PLAY_HANG_TEL)
		{

			voice_play_state &= (~VOICE_PLAY_HANG_TEL);
			set_play_voice_volue(PLAY_HOST_CH,28);
			voice_play_word(HANG_TEL_NUM);	

			tel_pickup_time = TEL_PICKUP_TIME;		
		}

		if (GetKeyState(PARA_RST_KEY_PRESS))	//键之前是按下的
		{
			if(para_rst_key_time< PARA_RST_KEY_TIME) para_rst_key_time++;
			
			if(para_reset_key_status ==0)	//键实际按下
			{
				if (PARA_RST_KEY_TIME ==para_rst_key_time)	//长按5000ms(5秒)
				{
					reset_parameter();
					while(1);					
					para_rst_key_time++;   //使复位参数不再处理

				}
			}
			else //按键弹起
			{
				ClrKeyState(PARA_RST_KEY_PRESS);
				para_rst_key_time=0;
			}
		}



		if (GetKeyState(SOFT_RST_KEY_PRESS))	//键之前是按下的
		{
			if(soft_rst_key_time< SOFT_RST_KEY_TIME) soft_rst_key_time++;
			
			if(soft_reset_key_status ==0)	//键实际按下
			{
				if (SOFT_RST_KEY_TIME ==soft_rst_key_time)	//长按20m秒
				{
					while(1);
					
					soft_rst_key_time++;   //使软件复位不再处理
				}
			}
			else //按键弹起
			{
				ClrKeyState(SOFT_RST_KEY_PRESS);
				soft_rst_key_time=0;
			}
		}

		if(GetWpaState(CHECK_ESP8266))
		{
			ClrWpaState(CHECK_ESP8266);
			check_esp8266();
		}

		if(GetState(bReadTime))
		{
			ClrState(bReadTime) ;
			ReadDS3231_time();
			check_black_light_status();
			if(stTime.bySecond < 0x60)
				read_ds3231_delay = (60-BCDtoHEX(stTime.bySecond))*2000;
			else read_ds3231_delay = 120000;

			memcpy(&(stBusFreq.bySndSecAddr),&(stTime.bySecond),3);
		    memcpy(&(stBusFreq.byRecSecAddr),&(stTime.byDay),3);
		    stBusFreq.byCmd = CMD_DATE_SEND;   
		   	Bus0OutputData(&(stBusFreq.bySndSecAddr));			
		}


/*
		if(GetBusErrorState(bStartBitError))
		{
			ClrBusErrorState(bStartBitError);
			uart2_send_byte(0xe0);
		}

		if(GetBusErrorState(bNightBitError))
		{
			ClrBusErrorState(bNightBitError);
			uart2_send_byte(0xe9);
		}

		if(GetBusErrorState(bEndBitError))
		{
			ClrBusErrorState(bEndBitError);
			uart2_send_byte(0xea);
		}

*/
		if(mt8870_received)		  //电话按键数据
		{
			mt8870_received_treat();
			mt8870_received = 0;
		}


		if(GetTelState(bCID_CMD))
		{
			ClrTelState(bCID_CMD);

			if(make_msm7512_data(stIndicationData.byIndicationRoomAddr,stIndicationData.byIndicationBedAddr))
			{
				msm_txe_ctrl(0);		//允许发送
				SetTelState(bCIDOccupy);		//信道占用信号状态
				stUsartCfg[3].uiSndLen = 30;
				stUsartCfg[3].uiSndPos = 0;
				for(i=0;i<30;i++)
				{
					byUsart3SndBuf[i] = 0x55;
				}
				LPC_UART3->THR = byUsart3SndBuf[stUsartCfg[3].uiSndPos];
				LPC_UART3->IER   |= 0x02;	//允许发送中断			
			}

		}


		if(tel_tlp_chk_delay == 0)
		{
			if(phone_tlp_status ==0)
			{//已摘机
				if(GetTelState(bOriTelTlp) )		//原是挂机状态
				{
					tel_tlp_chk_delay=500;		//延时250ms
				}
			}
			else 
			{//挂机
				if(GetTelState(bOriTelTlp)==0)		//原是摘机状态
				{
					tel_tlp_chk_delay = 500;
				}
			}
		}
		

		if(GetTelState(bTelHungDowned))
		{//电话挂机处理
			ClrTelState(bTelHungDowned);
			tel_hungdown_treat();

		}

		if(GetTelState(bTelPickUped))
		{//电话摘机处理
			ClrTelState(bTelPickUped);
			stop_tel_ring();
			tel_pickup_treat();

		}
			
		
		Bus0Manager();
		if(GetState(bTimerOutDeal))
		{
			ClrState(bTimerOutDeal);
			TimerOutDeal();
		}


		if(GetState(bRS485DataTreat))
		{
			if(stUsartCfg[0].uiSndLen==0) 
			{//RS485没有数据正在发送
				pSTUartFreq pstDataFreq=(pSTUartFreq)scon2_Rcvbuf;
				
				if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_DISPLAY_FORM))
				{
					uint16_t uiLen;
					uiLen = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
					__disable_irq();
					memcpy(byUsart0SndBuf,scon2_Rcvbuf,uiLen+EXT_SIZE);
					__enable_irq();

					pstDataFreq = (pSTUartFreq)byUsart0SndBuf;	
					stUsartCfg[0].uiSndLen =uiLen+EXT_SIZE;
					stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	

					LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
					LPC_UART0->IER	|= 0x02;		//允许串口0发送中断

					if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA))
					{
						//保存分机地址					
						stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
						stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
						stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

						//设置等待应答超时2S  在波特率为57.6K的情况下,100字节数据只要17.3ms
						byWaitRS485AckDTime = 95;		//等待1.9S	

					}
				}
				ClrState(bRS485DataTreat);
			}		
		}


		voice_play_treat();


		if(scon2_GucRcvNew == 0xff)
		{
			scon2_GucRcvNew=0;
			//uart2_send_byte(scon2_received);
			//for(i=0;i<scon2_received;i++)  uart2_send_byte(scon2_Rcvbuf[i]);
			Usart2RecDeal();
		}

		if(scon1_GucRcvNew == 0xff)
		{//接收wifi模块数据

			scon1_GucRcvNew=0;

			
/*			char str[25];
			for(i=0;i<25;i++) str[i]=0;

			
			int2str(scon1_received,str);
			uart2_send_str(str);		//字符个数
			uart2_send_byte(0x0a);		//换行	
//			uart2_send_byte(scon1_received);
			for(i=0;i<scon1_received;i++)  uart2_send_byte(scon1_Rcvbuf[i]);
			
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;	*/


			if(i=strstr(scon1_Rcvbuf,"+IPD,"))
			{//接收数据前导写
				uint16_t len;
				i +=5;
				len= atoi(i);	//数据长度
				if(len<=255)
				{
					if(len<10) 		i +=2;	//包括:
					else if(len<100) 	i +=3;
					else if(len<1000)	i +=4;				
					if(*(uint8_t *)i == START_BYTE1)
					{	
						wifi_rec_deal(i,len);
					}
				}
			}
			LPC_UART1->IER	= 0x01;	
		
		}



		if(scon0_GucRcvNew == 0xff)
		{//接收上位机调试信息
			scon0_GucRcvNew=0;
			//uart2_send_byte(scon0_received);
			//for(i=0;i<scon0_received;i++)  uart2_send_byte(scon0_Rcvbuf[i]);
			scon0_Rcvbuf[scon0_received]=0;
			if(strstr(scon0_Rcvbuf,"AT+GMR")!=NULL)
			{//查看版本信息

				uart1_send_str("AT+GMR");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行	
			}

			else if(strstr(scon0_Rcvbuf,"AT+RST")!=NULL)
			{//重启模块

				uart1_send_str("AT+RST");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行
			}
			else if(strstr(scon0_Rcvbuf,"AT+CWMODE=1")!=NULL)
			{//STA模式

				uart1_send_str("AT+CWMODE=1");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行
			}
			else if(strstr(scon0_Rcvbuf,"AT+CWMODE=2")!=NULL)
			{//AP模式

				uart1_send_str("AT+CWMODE=2");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行
			}	
			else if(strstr(scon0_Rcvbuf,"AT+CWMODE=3")!=NULL)
			{//STA+AP模式

				uart1_send_str("AT+CWMODE=3");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行
			}	
			else if(strstr(scon0_Rcvbuf,"AT+CWLAP")!=NULL)
			{//列出路由器列表
				uart1_send_str("AT+CWLAP");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行	
			}

			else if(strstr(scon0_Rcvbuf,"AT+CWJAP=")!=NULL)
			{//让模块连上自己的路由器
				uart1_send_str(scon0_Rcvbuf);
			}
			
			else if(strstr(scon0_Rcvbuf,"AT+CWJAP?")!=NULL)
			{//检测是否真连上了
				uart1_send_str("AT+CWJAP?");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行	
			}
			
			else if(strstr(scon0_Rcvbuf,"AT+CIPMUX=1")!=NULL)
			{//启动多模块连接
				uart1_send_str("AT+CIPMUX=1");
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行	
			}
			else if(strstr(scon0_Rcvbuf,"AT+CIPSTART=")!=NULL)
			{//让模块连上远程TCP服务器
				uart1_send_str(scon0_Rcvbuf);				
			}
			else if(strstr(scon0_Rcvbuf,"AT+CIPSEND=")!=NULL)
			{//让模块发送数据到远程TCP服务器
				uart1_send_str(scon0_Rcvbuf);				
			}

			else if(strstr(scon0_Rcvbuf,"AT+CIPSERVER=")!=NULL)
			{//开启模块本地的TCP服务器
				uart1_send_str(scon0_Rcvbuf);				
			}
			
			else if(strstr(scon0_Rcvbuf,"AT+CIPSR")!=NULL)
			{//查一下模块的IP 地址
				uart1_send_str(scon0_Rcvbuf);				
			}
			else if(strstr(scon0_Rcvbuf,"singlesend")!=NULL)	//单总线发送
			{
				stBusFreq.bySndSecAddr = 0x55;
				stBusFreq.bySndRoomAddr = 0x55;
				stBusFreq.bySndBedAddr = 0x55;			
				stBusFreq.byCmd = 0x01;
				stBusFreq.byRecSecAddr = 0xaa;
				stBusFreq.byRecRoomAddr = 0xaa;
				stBusFreq.byRecBedAddr = 0xaa;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));				
			}
			else if(strstr(scon0_Rcvbuf,"msm_test")!=NULL)		//MSM7512测试
			{
				send_msm7512_test();
			}
			else
			{
				uart1_send_str(scon0_Rcvbuf);			
			}
			
			init_scon0_Rcvbuf();
		}		

		soft_delay_half_ms(1);		

	}	 //end main loop
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
