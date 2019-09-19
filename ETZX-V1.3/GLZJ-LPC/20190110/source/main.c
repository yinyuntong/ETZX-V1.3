#include "custom.h"

uint8_t gb_fz_status=0;

void int2str(int n, char *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    int temp = n < 0 ? -n: n;  // tempΪn�ľ���ֵ

   if (str == NULL)
   {
       return;
   }
   while(temp)
   {
       buf[i++] = (temp % 10) + '0';  //��temp��ÿһλ�ϵ�������buf
       temp = temp / 10;
   }

   len = n < 0 ? ++i: i;  //���n�Ǹ����������Ҫһλ���洢����
   str[i] = 0;            //ĩβ�ǽ�����0
   while(1)
   {
       i--;
       if (buf[len-i-1] ==0)
       {
           break;
       }
       str[i] = buf[len-i-1];  //��buf��������ַ������ַ���
   }
   if (i == 0 )
   {
       str[i] = '-';          //����Ǹ��������һ������
   }
}



void led_pin_config(void)
{
	//WIFI AP LED
	LPC_PINCON->PINSEL0 &= (~(0x03 << 14)); 			// P0.7  ��Ϊwifi  ap_led
	LPC_GPIO0->FIODIR	 |= wifi_ap_led_pin;				//�������
	
	wifi_ap_led_ctrl(AP_LED_OFF);
	ClrWpaState(WIFI_AP_LED);

	//WIFI SERVER
	LPC_PINCON->PINSEL0 &= (~(0x03 << 12)); 			// P0.6  ��Ϊwifi server_led
	LPC_GPIO0->FIODIR	 |= wifi_server_led_pin;				//�������

	wifi_server_led_ctrl(SERVER_LED_OFF);
	ClrWpaState(WIFI_SERVER_LED);
	
}


/*********************************************************************************************************
** Function name:       main
** Descriptions:        ���жϷ�ʽ������λ�����ݲ������յ����ݻ��͸���λ�����̽�JP14,�������ڵ��������
**                      �����ò�����Ϊ9600��8λ����λ��1λֹͣλ��ע�ⲻҪѡ��"HEX��ʾ"
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main (void)
{ 
	uint32_t i;
	uint8_t j;
	
	SystemInit();                     //ϵͳ��ʼ�� 
	esp8266_pin_config();

	led_pin_config();

	//�绰����ѹѡ��
	LPC_PINCON->PINSEL0 &= (~(0x03 << 10)); 			// P0.5  KBC  ��̬Ϊ0
	LPC_GPIO0->FIODIR    |= tel_ctr_pin;
	kbc_ctrl(DC60V);


	LPC_PINCON->PINSEL3 &= (~(0x03 << 26)); 			// P1.29  cgb   ��̬Ϊ0  ������ֹͨ��
	LPC_PINCON->PINMODE3 |= (0X02<<26);					//������Ƭ������������
	LPC_PINCON->PINMODE_OD1 |= cgb_pin;					//��©ģʽ	
	LPC_GPIO1->FIODIR    |= cgb_pin;					//�����ʽ
	cgb_ctrl(0);


	LPC_PINCON->PINSEL0 &= (~(0x03 << 0)); 			// P0.0   cbd   ��̬Ϊ0  ������ֹͨ��
	LPC_PINCON->PINMODE0 |= (0X02<<0);					//������Ƭ������������
	LPC_PINCON->PINMODE_OD0 |= cbd_pin;					//��©ģʽ
	LPC_GPIO0->FIODIR    |= cbd_pin;					//�����ʽ
	cbd_ctrl(0);


	fm62429_pincfg();

  
	stCallPointer.pstHead = NULL;
	stCallPointer.pstTail = NULL;
	
	stIndicationData.pstNext = NULL;

	init_uart0(57600);					//����485����
	init_uart1(115200);					//WIFI
	init_uart2(57600);					//��λ��
	init_uart3(1200);					//������ʾоƬ


	AM79R70_pincfg();
/*	for(i=0;i<10;i++)	byUsart0SndBuf[i]=i;
	stUsartCfg[0].uiSndLen =10;
	stUsartCfg[0].uiSndPos = 0;
	LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
	LPC_UART0->IER   |= 0x02;*/


	init_sst25vfxxx_spi();  			//�ⲿE2P�ӿڳ�ʼ��
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
  	init_uart0(stEepromCfgData.uiRs485Brt);					//����485����


	
	voice_play_word(DTMFSTART_NUM);
	//load_voice_play_sequence(0,101,23);


/*	uart2_send_byte(0xaa);

	uart1_send_byte(0x0a);	//����
	uart1_send_str("AT+GMR");
	uart1_send_byte(0x0d);	//�س�
	uart1_send_byte(0x0a);	//����*/

#ifdef	  enable_wdt				//����
	init_wdt();
#endif

	init_tel();
	init_msm7512();

	init_mt8870();
	eint2Init();			//����MT8870�ź�����

	DS3231_init();

	init_phone_key_buf();

	init_black_light_status();

	if(stEepromCfgData.bySysMode & USE_USART2_MODE)
	{//ֻ�����ڣ����Ƶ�
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
		uart2_send_byte(0x0d);	//�س�
		uart2_send_byte(0x0a);	//����	


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
	//��ȫ���豸��λ
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
			else	//һ������Ƶ���ݱȽϴ�,������󻺳���
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

		if (GetKeyState(PARA_RST_KEY_PRESS))	//��֮ǰ�ǰ��µ�
		{
			if(para_rst_key_time< PARA_RST_KEY_TIME) para_rst_key_time++;
			
			if(para_reset_key_status ==0)	//��ʵ�ʰ���
			{
				if (PARA_RST_KEY_TIME ==para_rst_key_time)	//����5000ms(5��)
				{
					reset_parameter();
					while(1);					
					para_rst_key_time++;   //ʹ��λ�������ٴ���

				}
			}
			else //��������
			{
				ClrKeyState(PARA_RST_KEY_PRESS);
				para_rst_key_time=0;
			}
		}



		if (GetKeyState(SOFT_RST_KEY_PRESS))	//��֮ǰ�ǰ��µ�
		{
			if(soft_rst_key_time< SOFT_RST_KEY_TIME) soft_rst_key_time++;
			
			if(soft_reset_key_status ==0)	//��ʵ�ʰ���
			{
				if (SOFT_RST_KEY_TIME ==soft_rst_key_time)	//����20m��
				{
					while(1);
					
					soft_rst_key_time++;   //ʹ�����λ���ٴ���
				}
			}
			else //��������
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
		if(mt8870_received)		  //�绰��������
		{
			mt8870_received_treat();
			mt8870_received = 0;
		}


		if(GetTelState(bCID_CMD))
		{
			ClrTelState(bCID_CMD);

			if(make_msm7512_data(stIndicationData.byIndicationRoomAddr,stIndicationData.byIndicationBedAddr))
			{
				msm_txe_ctrl(0);		//������
				SetTelState(bCIDOccupy);		//�ŵ�ռ���ź�״̬
				stUsartCfg[3].uiSndLen = 30;
				stUsartCfg[3].uiSndPos = 0;
				for(i=0;i<30;i++)
				{
					byUsart3SndBuf[i] = 0x55;
				}
				LPC_UART3->THR = byUsart3SndBuf[stUsartCfg[3].uiSndPos];
				LPC_UART3->IER   |= 0x02;	//�������ж�			
			}

		}


		if(tel_tlp_chk_delay == 0)
		{
			if(phone_tlp_status ==0)
			{//��ժ��
				if(GetTelState(bOriTelTlp) )		//ԭ�ǹһ�״̬
				{
					tel_tlp_chk_delay=500;		//��ʱ250ms
				}
			}
			else 
			{//�һ�
				if(GetTelState(bOriTelTlp)==0)		//ԭ��ժ��״̬
				{
					tel_tlp_chk_delay = 500;
				}
			}
		}
		

		if(GetTelState(bTelHungDowned))
		{//�绰�һ�����
			ClrTelState(bTelHungDowned);
			tel_hungdown_treat();

		}

		if(GetTelState(bTelPickUped))
		{//�绰ժ������
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
			{//RS485û���������ڷ���
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
					stUsartCfg[0].uiSndPos = 0; 	//������֡ͷ����ʼ��������	

					LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
					LPC_UART0->IER	|= 0x02;		//������0�����ж�

					if((pstDataFreq->byCmd==CMD_DISPLAY_DATA) || (pstDataFreq->byCmd==CMD_HANDLE_DISPLAY_DATA))
					{
						//����ֻ���ַ					
						stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
						stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
						stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

						//���õȴ�Ӧ��ʱ2S  �ڲ�����Ϊ57.6K�������,100�ֽ�����ֻҪ17.3ms
						byWaitRS485AckDTime = 95;		//�ȴ�1.9S	

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
		{//����wifiģ������

			scon1_GucRcvNew=0;

			
/*			char str[25];
			for(i=0;i<25;i++) str[i]=0;

			
			int2str(scon1_received,str);
			uart2_send_str(str);		//�ַ�����
			uart2_send_byte(0x0a);		//����	
//			uart2_send_byte(scon1_received);
			for(i=0;i<scon1_received;i++)  uart2_send_byte(scon1_Rcvbuf[i]);
			
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;	*/


			if(i=strstr(scon1_Rcvbuf,"+IPD,"))
			{//��������ǰ��д
				uint16_t len;
				i +=5;
				len= atoi(i);	//���ݳ���
				if(len<=255)
				{
					if(len<10) 		i +=2;	//����:
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
		{//������λ��������Ϣ
			scon0_GucRcvNew=0;
			//uart2_send_byte(scon0_received);
			//for(i=0;i<scon0_received;i++)  uart2_send_byte(scon0_Rcvbuf[i]);
			scon0_Rcvbuf[scon0_received]=0;
			if(strstr(scon0_Rcvbuf,"AT+GMR")!=NULL)
			{//�鿴�汾��Ϣ

				uart1_send_str("AT+GMR");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����	
			}

			else if(strstr(scon0_Rcvbuf,"AT+RST")!=NULL)
			{//����ģ��

				uart1_send_str("AT+RST");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����
			}
			else if(strstr(scon0_Rcvbuf,"AT+CWMODE=1")!=NULL)
			{//STAģʽ

				uart1_send_str("AT+CWMODE=1");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����
			}
			else if(strstr(scon0_Rcvbuf,"AT+CWMODE=2")!=NULL)
			{//APģʽ

				uart1_send_str("AT+CWMODE=2");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����
			}	
			else if(strstr(scon0_Rcvbuf,"AT+CWMODE=3")!=NULL)
			{//STA+APģʽ

				uart1_send_str("AT+CWMODE=3");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����
			}	
			else if(strstr(scon0_Rcvbuf,"AT+CWLAP")!=NULL)
			{//�г�·�����б�
				uart1_send_str("AT+CWLAP");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����	
			}

			else if(strstr(scon0_Rcvbuf,"AT+CWJAP=")!=NULL)
			{//��ģ�������Լ���·����
				uart1_send_str(scon0_Rcvbuf);
			}
			
			else if(strstr(scon0_Rcvbuf,"AT+CWJAP?")!=NULL)
			{//����Ƿ���������
				uart1_send_str("AT+CWJAP?");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����	
			}
			
			else if(strstr(scon0_Rcvbuf,"AT+CIPMUX=1")!=NULL)
			{//������ģ������
				uart1_send_str("AT+CIPMUX=1");
				uart1_send_byte(0x0d);	//�س�
				uart1_send_byte(0x0a);	//����	
			}
			else if(strstr(scon0_Rcvbuf,"AT+CIPSTART=")!=NULL)
			{//��ģ������Զ��TCP������
				uart1_send_str(scon0_Rcvbuf);				
			}
			else if(strstr(scon0_Rcvbuf,"AT+CIPSEND=")!=NULL)
			{//��ģ�鷢�����ݵ�Զ��TCP������
				uart1_send_str(scon0_Rcvbuf);				
			}

			else if(strstr(scon0_Rcvbuf,"AT+CIPSERVER=")!=NULL)
			{//����ģ�鱾�ص�TCP������
				uart1_send_str(scon0_Rcvbuf);				
			}
			
			else if(strstr(scon0_Rcvbuf,"AT+CIPSR")!=NULL)
			{//��һ��ģ���IP ��ַ
				uart1_send_str(scon0_Rcvbuf);				
			}
			else if(strstr(scon0_Rcvbuf,"singlesend")!=NULL)	//�����߷���
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
			else if(strstr(scon0_Rcvbuf,"msm_test")!=NULL)		//MSM7512����
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
