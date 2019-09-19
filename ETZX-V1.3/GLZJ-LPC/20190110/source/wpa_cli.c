#define _IN_WPA_CLI_

#include"custom.h"


const unsigned int crc_32_tab[] = { /* CRC polynomial 0xedb88320 */
0x2bf32d7a,0x64806f53,0x57197f37,0x3ffe0bf2,0x102e5263,
0x497b6bb9,0x6c5c7a90,0x0d0d15f7,0x130609a6,0x7ccd7cf1,
0x13ac5e36,0x15da1ed7,0x530f6c0a,0x56a132f7,0x53967301,
0x6a5b3731,0x5cf1181a,0x2dd671ab,0x46fc14ce,0x71793ac6,
0x6be670a3,0x1c832350,0x059278fe,0x15934afb,0x7a3d6b38,
0x146d7b0d,0x5c6204f5,0x40fb595a,0x21da6eb1,0x4392476f,
0x5648349c,0x2a774211,0x4cb441a8,0x48a254e6,0x147610e0,
0x71c17671,0x355608fb,0x37806044,0x76f13631,0x0c083770,
0x375f564d,0x6dc51941,0x15126b04,0x006d6028,0x70014685,
0x373e34f9,0x06064651,0x74874056,0x2e85304d,0x4269106b,
0x794f6615,0x08df06dd,0x34c66294,0x64fc62fc,0x1f4135d2,
0x05743c40,0x47b2730c,0x305e00f3,0x6186450a,0x56384d2b,
0x0fcc1cad,0x562c2dfb,0x6b9a2dc8,0x579124ea,0x49ac2e30,
0x5cb57fa4,0x034f0b17,0x72da57f0,0x467d0895,0x237e4fe2,
0x314a7f88,0x0ae05e52,0x3c7f2d9e,0x37bf3f8d,0x39844bb0,
0x38655e49,0x67835e52,0x78e448df,0x63ca74a8,0x56a006b4,
0x7971497b,0x0d0d61f8,0x143d7a8b,0x5fba7994,0x5ac54a27,
0x258f2f5d,0x33d31efc,0x1953468e,0x3b0d362b,0x13704675,
0x68d53b9e,0x10fb756c,0x716d3c9b,0x2bef000b,0x1f5f2899,
0x68b9209c,0x2d1c5453,0x7cfa5acf,0x61e3331c,0x73881fb0,
0x75e43a72,0x51b66dd5,0x01a623d5,0x21d541dc,0x7ca600da,
0x06fa6b15,0x0f41557b,0x3e02187b,0x3bd32cb1,0x2fd431e9,
0x23215b45,0x78e6518a,0x67f464e2,0x0a037192,0x76073770,
0x464b222b,0x3023309d,0x427b5bb2,0x498921bc,0x096e4124,
0x4e9045d3,0x09e466d3,0x208f5def,0x1183715e,0x68fd67bb,
0x791b20df,0x47d72251,0x662e1c66,0x41425550,0x45b011a7,
0x2e720e2f,0x640b7cef,0x35ca47d1,0x46662d5d,0x0e453059,
0x2927594e,0x1214013f,0x47b65719,0x64891da4,0x51844dfd,
0x2f336ae2,0x304d7357,0x3a4b1768,0x039a2818,0x644e43c5,
0x75890a77,0x3cec4202,0x4cd04fe3,0x47215f35,0x60007b3c,
0x26a26b53,0x4e074342,0x3c7a7346,0x1ff93d4e,0x76f5613f,
0x38102040,0x58bd774e,0x31f133b8,0x7b435830,0x18303f89,
0x10601268,0x13170f6a,0x273d6191,0x74a35eef,0x387019f2,
0x45d041b8,0x3d677253,0x39165be9,0x570a200f,0x26347ed9,
0x479c796b,0x732c7e0f,0x43e21226,0x0e8f506a,0x5f880651,
0x5efa519f,0x18c06761,0x4be137e9,0x472c7f83,0x438b11fb,
0x71fe5f2f,0x625e613a,0x515e6900,0x1f5d2429,0x7f0c4fc3,
0x21ef6524,0x1a463ace,0x24e22152,0x62ec0493,0x0aaf1bd5,
0x0fe24774,0x07007143,0x64b62cd9,0x016f7f5a,0x4a823280,
0x159174ef,0x70145a15,0x54331e28,0x354d7703,0x29e340eb,
0x65c70e89,0x0eb31d96,0x06ba7e10,0x633967f1,0x3c215bbc,
0x1eda4c61,0x09184541,0x3349555e,0x6f8910f8,0x494a4526,
0x33071532,0x3abc37b3,0x42fa79cc,0x6dd620eb,0x0ffd4803,
0x75b17b18,0x50842c8a,0x1c063997,0x78c21fda,0x383a53d4,
0x69ca68ca,0x77977c76,0x1bf42d5f,0x7add58d6,0x488205e5,
0x6cf94cb6,0x5683463e,0x77142e30,0x0ef8717b,0x35cf75ec,
0x5c360ba9,0x13cd3f96,0x188b3ded,0x2d732e8f,0x302400d4,
0x21eb0ae3,0x1a2e40e4,0x756f2a7c,0x6d3c3177,0x68c47a95,
0x12e941c9,0x6a043d71,0x5ebe684f,0x02a1504c,0x0c56384a,
0x66a70d92,0x68bf759d,0x09411f4f,0x2b241adb,0x5a7f55e6,
0x40a905a5,0x12565707,0x0f973d33,0x2ce938db,0x75b90730,
0x7c065d81,0x06063ecb,0x71f422be,0x497f220b,0x096c39f0,
0x075b0962,0x6ee53c44,0x58623d86,0x4f154f29,0x09975f7c,
0x6eb46d05,0x121916f5,0x6f832626,0x204b7ade,0x668d6dfb,
0x02336833,0x27ac4884,0x6d203b14,0x6a506c07,0x68901e1a,
0x2b7c2b05,0x605063eb,0x71113a6c,0x21e51f0c,0x524f280e,
0x492b6bf7,0x69991552,0x603c216e,0x13780a29,0x36935b0e,
0x1ee37444,0x33721227,0x52be2685,0x278038cb,0x6ca06cb3,
0x61b70b25,0x7cd90771,0x0b7d0b0c,0x43b42471,0x101159d8,
0x73f926e5,0x13296475,0x7eb65436,0x3ada15b6,0x4c3f38f9,
0x50b702eb,0x06745a1f,0x7e3d46b5,0x5842066b,0x286f7e19,
0x466d5c2a,0x2ebd1bd4,0x058c571e,0x112a05ee,0x71557abb,
0x41ae3192,0x091a6c96,0x24d9311d,0x1594131a,0x05ba6875,
0x5be00c7c,0x00ff5b9c,0x01c711f2,0x2b5d4b85,0x07421b20,
0x77331526,0x793d55d3,0x1d6a06ce,0x419c45f2,0x0d9f1fb9,
0x387c21b2,0x29846de2,0x5abb3e08,0x4f837e18,0x49ba1f64,
0x78a51254,0x7861029c,0x6db3124a,0x29647226,0x579b6a3e,
0x22c53b8b,0x2f144c19,0x3a9b5918,0x4f8506bc,0x211123d7,
0x31236794,0x0bb161be,0x0e5a5a6e,0x25e30425,0x786f3b8d,
0x6ab02a7e,0x1f630ffa,0x4bcc3d40,0x511a181c,0x49d43524,
0x73bd7088,0x61d46294,0x7f5c351a,0x740c4960,0x3ad85a45,
0x07f464b4,0x400030a7,0x117d6747,0x0afa46e1,0x1b9a4bd4,
0x18e51da5,0x7bff49ce,0x59bc6211,0x74280250,0x797d223b,
0x639e7953,0x448b300d,0x6f762f23,0x2b512653,0x431a6e14,
0x29294412,0x4e4c2070,0x127c121b,0x246742b5,0x0e2a06e1,
0x60e845fd,0x3e2c1c88,0x352e76be,0x5b780d3e,0x037e3bb3,
0x492429ce,0x2b4a411a,0x62cf7e72,0x6ab36420,0x0a3d39e9,
0x794c729a,0x38410aca,0x402112f5,0x14e06718,0x52f86870,
0x5ab37216,0x0ed64788,0x098d745d,0x01c50eb4,0x2c5e1a2b,
0x7fd57753,0x36576bfe,0x096172e3,0x3b4f3562,0x66936873,
0x07664e07,0x1c2a665b,0x59e87916,0x1d7b1c2a,0x47744ec5,
0x5fb630e0,0x075d642a,0x326b495e,0x3050254c,0x014d531a,
};



unsigned char ssid[30];
unsigned char pwd[10];

uint16_t WpaState=0;		//WPA 状态字


uint16_t check_esp8266_time=0;


void init_ssid(void)
{
	unsigned char i;
	for(i=0;i<30;i++) ssid[i]=0x00;
}

void init_pwd(void)
{
	unsigned char i;
	for(i=0;i<10;i++) pwd[i]=0x00;
}


unsigned int crc32(unsigned int crc, unsigned char *buffer, int size)
{
    unsigned int i;
    for (i = 0; i < size; i++) {
        crc = crc_32_tab[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
    }
    return crc ;
}

static char *sub_string(char *str, int start, int end)
{
	static char * st = NULL;
	int i = start, j = 0;
	st ? free(st) : 0;
	st = (char *)malloc(sizeof(char) * (end - start + 1));
	while(i < end){
		st[j++] = str[i++];
	}
	st[j] = '\0';
	return st;
}

int ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr)
{
        int  StringLen;
        char caNewString[2048];

        char *FindPos = strstr(sSrc, sMatchStr);
        if( (!FindPos) || (!sMatchStr) )
                return -1;

        while( FindPos )
        {
                memset(caNewString, 0, sizeof(caNewString));
                StringLen = FindPos - sSrc;
                strncpy(caNewString, sSrc, StringLen);
                strcat(caNewString, sReplaceStr);
                strcat(caNewString, FindPos + strlen(sMatchStr));
                strcpy(sSrc, caNewString);

                FindPos = strstr(sSrc, sMatchStr);
        }

        return 0;
}


struct ssid_list{
	char bssid[50];
	char ssid[50];
	char psk[20];
	int level;
};

void wifi_server_led_flash(void)
{
	if(GetWpaState(WIFI_SERVER_LED))
	{
		wifi_server_led_ctrl(SERVER_LED_OFF);
		ClrWpaState(WIFI_SERVER_LED);
	}
	else
	{
		wifi_server_led_ctrl(SERVER_LED_ON);
		SetWpaState(WIFI_SERVER_LED);						
	}
}


void wifi_ap_led_flash(void)
{
	if(GetWpaState(WIFI_AP_LED))
	{
		wifi_ap_led_ctrl(AP_LED_OFF);
		ClrWpaState(WIFI_AP_LED);
	}
	else
	{
		wifi_ap_led_ctrl(AP_LED_ON);
		SetWpaState(WIFI_AP_LED);						
	}
}



void get_right_ssid(char * ssid ,int area,int room,int bed)
{
	sprintf(ssid,"ETZW-%02X%02X%02X-",area,room,bed);
	unsigned int crc = crc32(0x78943251,(unsigned char *)ssid,12);
	sprintf((char*)(ssid+12),"%04X",crc>>16);
}


void esp8266_pin_config(void)
{
	LPC_PINCON->PINSEL0 &= (~(0x03 << 18)); 			// P0.9  esp_rst  复位脚
	LPC_GPIO0->FIODIR    |= esp_rst_pin;				//输出口
	esp_rst_ctrl(0);									//低电平复位

}


void esp8266_rst(void)
{
	esp_rst_ctrl(0);
	soft_delay_half_ms(1000);
	esp_rst_ctrl(1);

}


void init_esp8266(void)
{

	if(stEepromCfgData.bySysMode & USE_WIFI_MODE)
	{

	uint16_t i,j;
	uint8_t string[40];
			
	wifi_ap_led_ctrl(AP_LED_OFF);
	ClrWpaState(WIFI_AP_LED);
	ClrWpaState(CONNECTED_WIFI);
	

	wifi_server_led_ctrl(SERVER_LED_OFF);
	ClrWpaState(WIFI_SERVER_LED);	
	ClrWpaState(CONNECTED_SERVER);
	
	
	uart1_send_str("AT");	//测试AT启动
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行

	for(i=0;i<5000;i++)	//延时2.5秒
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收wifi模块模式数据
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;			
			if(strstr(scon1_Rcvbuf,"OK"))
			{
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 AT START OK!");
					uart2_send_byte(0x0a);	//换行
				}
				init_scon1_Rcvbuf();
				break;	//响应		
			}			
			init_scon1_Rcvbuf();
		}
	}
	if(i>=5000)
	{
		voice_play_word(DTMF0_NUM);
		
		esp8266_rst();	
		if(stEepromCfgData.bySysMode & USE_USART2_MODE) goto init_cwmode;
		else while(1);		//系统重启
	}

	

init_cwmode:	
	uart1_send_str("AT+CWMODE?");	//获取WIFI模块的模式
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行	
	for(i=0;i<10000;i++)	//延时5秒
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收wifi模块模式数据
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;			
			if(strstr(scon1_Rcvbuf,"+CWMODE:1"))  
			{

				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 CWMODE:1 STATION");
					uart2_send_byte(0x0a);	//换行
				}
				init_scon1_Rcvbuf();
				break;	//站点模式	
			}
			else if(strstr(scon1_Rcvbuf,"+CWMODE:2") || strstr(scon1_Rcvbuf,"+CWMODE:3"))	//AP模式
			{
				uart1_send_str("AT+CWMODE=1");	//设置WIFI为站点模式
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行

				soft_delay_half_ms(500);
				
				uart1_send_str("AT+RST");	//需重启后生效
				uart1_send_byte(0x0d);	//回车
				uart1_send_byte(0x0a);	//换行


				init_scon1_Rcvbuf();
				i=10000;
				break;
			}
			init_scon1_Rcvbuf();
		}	
	}
	
	if(i>=10000)
	{
		esp8266_rst();	
		if(stEepromCfgData.bySysMode & USE_USART2_MODE)		goto init_judge_ap;
		else while(1);		//系统重启

	}



init_judge_ap:
	soft_delay_half_ms(2000);
	uart1_send_str("AT+CWJAP?");	//判断是否真连上了
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行


	for(i=0;i<5000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收连上信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;

			if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
			{
				uart2_send_str(scon1_Rcvbuf);
			}
			
			if(strstr(scon1_Rcvbuf,"+CWJAP:"))
			{//是正确回复
				if(strstr(scon1_Rcvbuf,ssid))
				{//已连上正确的SSID
					SetWpaState(CONNECTED_WIFI);
					SetWpaState(EXIST_SSID);

					if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
					{
						uart2_send_byte(0x0a);	//换行
						uart2_send_str("esp8266 CONNECTED WIFI:");
						uart2_send_str(ssid);
						uart2_send_byte(0x0a);	//换行
					}

					wifi_ap_led_ctrl(AP_LED_ON);
					SetWpaState(WIFI_AP_LED);	

					set_play_voice_volue(PLAY_HOST_CH,28);
					voice_play_word(CONNECTED_WIFI_NUM);	


					while( voice_play_state & VOICE_PLAY_ONE_WORD)
					{//存在有多段(>=3)的情况下

						j=5000;
						while(!(voice_play_state & VOICE_PLAY_READ_EEPROM_DATA))
						{//等待一段播放完成
							soft_delay_half_ms(1);
							if(--j ==0) break;

						}

						voice_continue_play_word();

						if(voice_data_total_len== 0) break;
						
					}

					j=5000;	//一段最长1.5S
					while( voice_play_state& VOICE_PLAY_START)
					{//等待最后一段放完
						soft_delay_half_ms(1);
						if(--j ==0) break;
					}	
					init_scon1_Rcvbuf();
					goto init_connect_server;
				}
				else
				{//连上错误的SSID
					init_scon1_Rcvbuf();
					goto init_connect_ap;
				}
			
			}
			else if(strstr(scon1_Rcvbuf,"No AP"))
			{//没有连上
				init_scon1_Rcvbuf();
				goto init_connect_ap;
			}
			init_scon1_Rcvbuf();
		}
	}
	if(i>=5000)		//没有回复重启
	{
		esp8266_rst();	
		if(stEepromCfgData.bySysMode & USE_USART2_MODE) goto init_connect_ap;
		else while(1);		//系统重启
	}



init_connect_ap:
	soft_delay_half_ms(2000);
	for(i=0;i<40;i++) string[i]=0x00;
	strcpy(string,"AT+CWJAP=\"");
	strcat(string,ssid);
	strcat(string,"\",\"");
	strcat(string,pwd);
	strcat(string,"\"");


	uart1_send_str(string);	//连接当前可用AP
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行		

	for(i=0;i<10000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收到信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;
			
			if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
			{
				uart2_send_str(scon1_Rcvbuf);
			}

			if(strstr(scon1_Rcvbuf,"GOT IP"))
			{//获得IP
				SetWpaState(CONNECTED_WIFI);

				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 GOT IP!");
					uart2_send_byte(0x0a);	//换行
				}

				wifi_ap_led_ctrl(AP_LED_ON);
				SetWpaState(WIFI_AP_LED);	

				set_play_voice_volue(PLAY_HOST_CH,28);
				voice_play_word(CONNECTED_WIFI_NUM);

				while( voice_play_state & VOICE_PLAY_ONE_WORD)
				{//存在有多段(>=3)的情况下

					j=5000;
					while(!(voice_play_state & VOICE_PLAY_READ_EEPROM_DATA))
					{//等待一段播放完成
						soft_delay_half_ms(1);
						if(--j ==0) break;

					}

					voice_continue_play_word();

					if(voice_data_total_len== 0) break;
					
				}

				j=5000;	//一段最长1.5S
				while( voice_play_state& VOICE_PLAY_START)
				{//等待最后一段放完
					soft_delay_half_ms(1);
					if(--j ==0) break;
				}	
				init_scon1_Rcvbuf();
				goto init_connect_server;
			}
			init_scon1_Rcvbuf();
		}
	}
	if(i>=10000) return;
	



init_connect_server:

	soft_delay_half_ms(2000);
	//AT+CIPSTART="TCP","192.168.88.254",9999
	uart1_send_str("AT+CIPSTART=\"TCP\",\"192.168.88.254\",9999");	//设备连入服务器
	
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行			


	for(i=0;i<10000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收到信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;
			

			if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
			{
				uart2_send_str(scon1_Rcvbuf);
			}

			if(strstr(scon1_Rcvbuf,"ALREADY CONNECTED"))
			{
				SetWpaState(CONNECTED_SERVER);
	
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 ALREADY CONNECTED SERVER!");
					uart2_send_byte(0x0a);	//换行
				}

				wifi_server_led_ctrl(SERVER_LED_ON);
				SetWpaState(WIFI_SERVER_LED);

				wifi_ap_led_ctrl(AP_LED_OFF);
				ClrWpaState(WIFI_AP_LED);


//				set_play_voice_volue(PLAY_HOST_CH,28);

				voice_play_word(CONNECTED_SERVER_NUM);
				
				while( voice_play_state & VOICE_PLAY_ONE_WORD)
				{//存在有多段(>=3)的情况下

					j=5000;
					while(!(voice_play_state & VOICE_PLAY_READ_EEPROM_DATA))
					{//等待一段播放完成
						soft_delay_half_ms(1);
						if(--j ==0) break;

					}

					voice_continue_play_word();

					if(voice_data_total_len== 0) break;
					
				}

				j=5000;	//一段最长1.5S
				while( voice_play_state& VOICE_PLAY_START)
				{//等待最后一段放完
					soft_delay_half_ms(1);
					if(--j ==0) break;
				}	
				init_scon1_Rcvbuf();
				break;
			}
			else if(strstr(scon1_Rcvbuf,"CONNECT"))
			{//接入服务器
				SetWpaState(CONNECTED_SERVER);
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{					
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 CONNECTED SERVER!");
					uart2_send_byte(0x0a);	//换行
				}
				
				wifi_server_led_ctrl(SERVER_LED_ON);
				SetWpaState(WIFI_SERVER_LED);

				wifi_ap_led_ctrl(AP_LED_OFF);
				ClrWpaState(WIFI_AP_LED);					
//					set_play_voice_volue(PLAY_HOST_CH,28);

				voice_play_word(CONNECTED_SERVER_NUM);	

				while( voice_play_state & VOICE_PLAY_ONE_WORD)
				{//存在有多段(>=3)的情况下

					j=5000;
					while(!(voice_play_state & VOICE_PLAY_READ_EEPROM_DATA))
					{//等待一段播放完成
						soft_delay_half_ms(1);
						if(--j ==0) break;

					}

					voice_continue_play_word();

					if(voice_data_total_len== 0) break;
					
				}

				j=5000;	//一段最长1.5S
				while( voice_play_state& VOICE_PLAY_START)
				{//等待最后一段放完
					soft_delay_half_ms(1);
					if(--j ==0) break;
				}	
				init_scon1_Rcvbuf();
				break;
			}
			init_scon1_Rcvbuf();
		}
	}

	if(i>=10000)
	{
		if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
		{
			uart2_send_byte(0x0a);	//换行
			uart2_send_str("esp8266 connect server timeout !");
			uart2_send_byte(0x0a);	//换行
		}
	}
	}
}







//正常情况下每30秒钟或者wifi模块无响应时10秒检查1次ESP8266状态
void check_esp8266(void)
{

	if(stEepromCfgData.bySysMode & USE_WIFI_MODE)
	{
	uint16_t i;
	uint8_t string[40];	
	uint8_t send_result;

get_server_state:
//  STATUS:2	服务器未打开、服务器打开但未连接、服务器连上别的AP	
//  STATUS:3     	已连上服务器、或者连上后服务器连上别的AP或关闭无线连接
//  STATUS:4	服务器连接后已关闭
//  STATUS:5	本机未连上AP的状态码



	uart1_send_str("AT+CIPSTATUS");	//获得连接服务器状态
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行		

	for(i=0;i<4000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收到信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;

			if(strstr(scon1_Rcvbuf,"STATUS:2"))
			{//未连上服务器的状态码

				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 link is not valid!");
					uart2_send_byte(0x0a);	//换行
				}
				ClrWpaState(CONNECTED_SERVER);

				wifi_server_led_ctrl(SERVER_LED_OFF);
				ClrWpaState(WIFI_SERVER_LED);


				init_scon1_Rcvbuf();
				goto judge_ap;
			}



			else if(strstr(scon1_Rcvbuf,"STATUS:3"))
			{//已连上服务器或曾连上服务器但已断开

				STAddr_S source_addr;
				init_scon1_Rcvbuf();
				
				source_addr.bySecAddr = 	stEepromCfgData.bySelfSecAddr;
				source_addr.byRoomAddr =	stEepromCfgData.bySelfRoomAddr;
				source_addr.byBedAddr = 	stEepromCfgData.bySelfBedAddr;
				send_result = wifi_send_cmd(source_addr,CMD_LANDING,0,NULL);


				if(send_result == 0)
				{
	
					if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
					{
						uart2_send_byte(0x0a);	//换行
						uart2_send_str("esp8266 link is valid,send data ok!");
						uart2_send_byte(0x0a);	//换行
					}

					SetWpaState(CONNECTED_SERVER);

					wifi_server_led_ctrl(SERVER_LED_ON);
					SetWpaState(WIFI_SERVER_LED);


					wifi_ap_led_ctrl(AP_LED_OFF);	//服务指示灯亮,就把连入无线路由器指示灯熄 灭
					ClrWpaState(WIFI_AP_LED);
					return;
				}
				else 
				{
					if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
					{
						uart2_send_byte(0x0a);	//换行
						uart2_send_str("esp8266 link is invalid,send data fail!");
						uart2_send_byte(0x0a);	//换行
					}
					
					ClrWpaState(CONNECTED_SERVER);

					wifi_server_led_ctrl(SERVER_LED_OFF);
					ClrWpaState(WIFI_SERVER_LED);
					goto judge_ap;				
				}

			}

			else if(strstr(scon1_Rcvbuf,"STATUS:4"))
			{//服务器连接已关闭，须重连
				
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 link is closed!");
					uart2_send_byte(0x0a);	//换行
				}


				ClrWpaState(CONNECTED_SERVER);
				wifi_server_led_ctrl(SERVER_LED_OFF);
				ClrWpaState(WIFI_SERVER_LED);

				init_scon1_Rcvbuf();
				goto judge_ap;	
			}

			
			else if(strstr(scon1_Rcvbuf,"STATUS:5"))
			{//未连上AP的状态码
					
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 colsed,wifi disconnect!");
					uart2_send_byte(0x0a);	//换行
				}

				ClrWpaState(CONNECTED_WIFI);
				wifi_ap_led_ctrl(AP_LED_OFF);
				ClrWpaState(WIFI_AP_LED);	

				ClrWpaState(CONNECTED_SERVER);
				wifi_server_led_ctrl(SERVER_LED_OFF);
				ClrWpaState(WIFI_SERVER_LED);	
				init_scon1_Rcvbuf();
				goto connect_ap;
			}
			init_scon1_Rcvbuf();
		}
	}		

	if(i>=4000)
	{//时间到未收到回复
		esp8266_rst();
		check_esp8266_time = CHECK_WIFI_SHORT_TIME; 	//10秒之后再次检测
		
		ClrWpaState(CONNECTED_SERVER);
		wifi_server_led_ctrl(SERVER_LED_OFF);
		ClrWpaState(WIFI_SERVER_LED);	

		ClrWpaState(CONNECTED_WIFI);
		wifi_ap_led_ctrl(AP_LED_OFF);
		ClrWpaState(WIFI_AP_LED);

	
		if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
		{
			uart2_send_byte(0x0a);	//换行
			uart2_send_str("esp8266 check server status 2 seccond timeout!");
			uart2_send_byte(0x0a);	//换行
		}
		
		return;
	}




judge_ap:
	uart1_send_str("AT+CWJAP?");	//判断是否真连上了路由器
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行


	for(i=0;i<4000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收连上信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;
			
			if(strstr(scon1_Rcvbuf,"+CWJAP:"))
			{//是正确回复
				if(strstr(scon1_Rcvbuf,ssid))
				{//已连上正确的SSID
	
					if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
					{
						uart2_send_byte(0x0a);	//换行
						uart2_send_str("check esp8266 +CWJAP:");
						uart2_send_str(ssid);
						uart2_send_byte(0x0a);	//换行
					}

					wifi_ap_led_ctrl(AP_LED_ON);
					SetWpaState(WIFI_AP_LED);
					SetWpaState(CONNECTED_WIFI);					

					init_scon1_Rcvbuf();
					goto connect_server;
				}
				else
				{//连上错误的SSID
					if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
					{					
						uart2_send_byte(0x0a);	//换行
						uart2_send_str("check esp8266 AT+CWJAP?: ERROR ssid");
						uart2_send_byte(0x0a);	//换行
					}
			
					wifi_ap_led_ctrl(AP_LED_OFF);
					ClrWpaState(WIFI_AP_LED);
					ClrWpaState(CONNECTED_WIFI);	//先将此状态标志清0
					init_scon1_Rcvbuf();

					uart1_send_str("AT+CWQAP");	//退出错误的AP
					uart1_send_byte(0x0d);	//回车
					uart1_send_byte(0x0a);	//换行
					soft_delay_half_ms(2000);
					goto connect_ap;
				}
			
			}
			else if(strstr(scon1_Rcvbuf,"No AP"))
			{//没有连上	
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("check esp8266 AT+CWJAP?: No AP!");
					uart2_send_byte(0x0a);	//换行
				}
			
				wifi_ap_led_ctrl(AP_LED_OFF);
				ClrWpaState(WIFI_AP_LED);
				ClrWpaState(CONNECTED_WIFI);	//先将此状态标志清0
				init_scon1_Rcvbuf();
				
				goto connect_ap;
			}
			init_scon1_Rcvbuf();			
		}
	}	
	if(i>=4000)
	{//时间到未收到回复
		esp8266_rst();
		wifi_ap_led_ctrl(AP_LED_OFF);
		ClrWpaState(WIFI_AP_LED);
		ClrWpaState(CONNECTED_WIFI);	//先将此状态标志清0		
		check_esp8266_time = CHECK_WIFI_SHORT_TIME; 	//10秒之后再次检测	

		if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
		{
			uart2_send_byte(0x0a);	//换行
			uart2_send_str("check esp8266 AT+CWJAP? 2 second timerout!");
			uart2_send_byte(0x0a);	//换行
		}
			
		return;
	}


	
connect_ap:
	for(i=0;i<40;i++) string[i]=0x00;
	strcpy(string,"AT+CWJAP=\"");
	strcat(string,ssid);
	strcat(string,"\",\"");
	strcat(string,pwd);
	strcat(string,"\"");


	uart1_send_str(string);	//连接当前可用AP
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行		

	for(i=0;i<10000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收到信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;


			if(strstr(scon1_Rcvbuf,"GOT IP"))
			{//获得IP


				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 AT+CWJAP:");
					uart2_send_str(ssid);
					uart2_send_str("GOT IP!");
					uart2_send_byte(0x0a);	//换行
				}

				wifi_ap_led_ctrl(AP_LED_ON);
				SetWpaState(WIFI_AP_LED);
				SetWpaState(CONNECTED_WIFI);
				init_scon1_Rcvbuf();
				
				goto connect_server;
			}
			init_scon1_Rcvbuf();			
		}
	}
	if(i>=10000)
	{//时间到未收到回复
		esp8266_rst();
		check_esp8266_time = CHECK_WIFI_SHORT_TIME; 	//10秒之后再次检测
	
		if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
		{
			uart2_send_byte(0x0a);	//换行
			uart2_send_str("esp8266 AT+CWJAP:");
			uart2_send_str(ssid);
			uart2_send_str(" 5 second timeout!");
			uart2_send_byte(0x0a);	//换行
		}
		
		return;
	}			



connect_server:
	ClrWpaState(CONNECTED_SERVER);
	wifi_server_led_ctrl(SERVER_LED_OFF);
	ClrWpaState(WIFI_SERVER_LED);

	uart1_send_str("AT+CIPSTART=\"TCP\",\"192.168.88.254\",9999");	//设备连入服务器
	uart1_send_byte(0x0d);	//回车
	uart1_send_byte(0x0a);	//换行			
	
	
	for(i=0;i<10000;i++)
	{
		soft_delay_half_ms(1);
		if(scon1_GucRcvNew == 0xff)
		{//接收到信息
		
			scon1_GucRcvNew=0;
			LPC_UART1->IER	= 0x01;
	
			if(strstr(scon1_Rcvbuf,"ALREADY CONNECTED"))
			{

	
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 AT+CIPSTART=\"TCP\",\"192.168.88.254\",9999, ALREADY CONNECTED!");
					uart2_send_byte(0x0a);	//换行
				}


				wifi_ap_led_ctrl(AP_LED_OFF);
				ClrWpaState(WIFI_AP_LED);
				
				SetWpaState(CONNECTED_SERVER);
				wifi_server_led_ctrl(SERVER_LED_ON);
				SetWpaState(WIFI_SERVER_LED); 
				init_scon1_Rcvbuf();
				return;
			}
			else if(strstr(scon1_Rcvbuf,"CONNECT"))
			{//接入服务器
	
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 AT+CIPSTART=\"TCP\",\"192.168.88.254\",9999, CONNECT!");
					uart2_send_byte(0x0a);	//换行
				}

				wifi_ap_led_ctrl(AP_LED_OFF);
				ClrWpaState(WIFI_AP_LED);

				SetWpaState(CONNECTED_SERVER);
				wifi_server_led_ctrl(SERVER_LED_ON);
				SetWpaState(WIFI_SERVER_LED); 
				init_scon1_Rcvbuf();
				return;
			}
			else if(strstr(scon1_Rcvbuf,"CLOSED"))
			{//服务器已关闭
	
				if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
				{
					uart2_send_byte(0x0a);	//换行
					uart2_send_str("esp8266 AT+CIPSTART=\"TCP\",\"192.168.88.254\",9999, SERVER CLOSED!");
					uart2_send_byte(0x0a);	//换行
				}

				init_scon1_Rcvbuf();
				return;
			}
			init_scon1_Rcvbuf();
		}
	}
	if(i>=10000)
	{//时间到未收到连接服务器回复
		check_esp8266_time = CHECK_WIFI_SHORT_TIME; 	//10秒之后再次检测	

		if(stEepromCfgData.bySysMode & USE_WIFI_DEBUG)
		{
			uart2_send_byte(0x0a);	//换行
			uart2_send_str("esp8266 AT+CIPSTART=\"TCP\",\"192.168.88.254\",9999,5 second timeout !");
			uart2_send_byte(0x0a);	//换行
		}		
		return;
	}	
	}
}









//返回0: 数据成功发送，且收到SEND OK
//返回1:WIFI模块未连上APP
//返回2:发送AT+CIPSEND命令后未收到OK，实际数据未发送
//返回3:发送数据后未收到SEND OK
uint8_t wifi_send_data(uint8_t *pdata,uint8_t data_len)
{

	if(GetWpaState(CONNECTED_SERVER)==0)  return 1;		//没有连上服务器，直接返回

	else
	{
		uint8_t string[40];
		uint16_t i;
		
		for(i=0;i<40;i++) string[i]=0x00;

		i = sprintf(string,"%s","AT+CIPSEND=");
		sprintf(string+i,"%d",data_len);
		


		uart1_send_str(string);	//连接当前可用AP
		uart1_send_byte(0x0d);	//回车
		uart1_send_byte(0x0a);	//换行	


		for(i=0;i<2000;i++)
		{
			soft_delay_half_ms(1);
			if(scon1_GucRcvNew == 0xff)
			{//接收到信息
			
				scon1_GucRcvNew=0;
				LPC_UART1->IER	= 0x01;
		
				if(strstr(scon1_Rcvbuf,"OK"))
				{//可以发送数据了
					init_scon1_Rcvbuf();
					uart1_send_str_len(pdata,data_len);	//发送数据

					for(i=0;i<2000;i++)
					{
						soft_delay_half_ms(1);
						if(scon1_GucRcvNew == 0xff)
						{//接收到信息
							scon1_GucRcvNew=0;
							LPC_UART1->IER	= 0x01;
					
							if(strstr(scon1_Rcvbuf,"SEND OK"))	
							{//发送完成
								init_scon1_Rcvbuf();
								return 0;
							}
							init_scon1_Rcvbuf();
						}
					}
					return 3;
				}
				init_scon1_Rcvbuf();
			}
		}		
		return 2;
	}
}


//返回0: 数据成功发送，且收到SEND OK
//返回1:WIFI模块未连上APP
//返回2:发送AT+CIPSEND命令后未收到OK，实际数据未发送
//返回3:发送数据后未收到SEND OK

uint8_t wifi_send_cmd(STAddr_S sur_addr,uint8_t Cmd,uint8_t byLen,uint8_t *pData)
{
	uint8_t result;
	if(stEepromCfgData.bySysMode & USE_WIFI_MODE)
	{	

		pSTUartFreq pstDataFreq;
		pstDataFreq = (pSTUartFreq)byUsart1SndBuf;
		
		pstDataFreq->byStartByte1   =START_BYTE1;
		pstDataFreq->bySndSecAddr   =sur_addr.bySecAddr;
		pstDataFreq->bySndRoomAddr  =sur_addr.byRoomAddr;
		pstDataFreq->bySndBedAddr   =sur_addr.byBedAddr;
		pstDataFreq->byRecSecAddr   =stEepromCfgData.bySelfSecAddr;
		pstDataFreq->byRecRoomAddr  =PC_ADDR2;
		pstDataFreq->byRecBedAddr   =PC_ADDR3;	
		pstDataFreq->byCmd			=Cmd;		//命令
		pstDataFreq->byLenH			=0x00;
		pstDataFreq->byLenL			=byLen;
		if(byLen)
		{
			memcpy(pstDataFreq->byData,pData,byLen);	//复制数据
		}

		
		pstDataFreq->byData[byLen] = GetPskCrc(pstDataFreq);
		pstDataFreq->byData[byLen+1] = END_BYTE;

		result=wifi_send_data(byUsart1SndBuf,byLen+EXT_SIZE);
		return result;
	}
	else return 0;	
}



void wifi_send_cmd_test(void)
{
	byUsart1SndBuf[0]=0x5a;
	byUsart1SndBuf[1]=0x01;
	byUsart1SndBuf[2]=0xfe;
	byUsart1SndBuf[3]=0x00;

	
	byUsart1SndBuf[4]=0x01;
	byUsart1SndBuf[5]=0x00;
	byUsart1SndBuf[6]=0x00;


	byUsart1SndBuf[7]=0x06;
	byUsart1SndBuf[8]=0x00;
	byUsart1SndBuf[9]=0x03;


	byUsart1SndBuf[10]=0x01;
	byUsart1SndBuf[11]=0x02;
	byUsart1SndBuf[12]=0x03;

	byUsart1SndBuf[13]=0x0f;
	byUsart1SndBuf[14]=0x16;

	wifi_send_data(byUsart1SndBuf,15);

}


STAddr_S wifi_result_sur_addr;	//回复WIFI结果源地址(相当于电脑发过来的目标地址)


void wifi_rec_deal(uint8_t *p,uint16_t len)
{
	if(stEepromCfgData.bySysMode & USE_WIFI_MODE)
	{
	uint16_t data_len;
	STAddr_S source_addr;
	uint8_t  data_buff[10];

	uint8_t *wifi_deal_buff;

	pSTUartFreq pstDataFreq=(pSTUartFreq)p;

		//数据长度
	data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
	if( data_len> MAX_UART2_DATA_LEN) goto wifi_rec_deal_ret;

	//结束位
	if(pstDataFreq->byData[data_len+1] != END_BYTE) goto wifi_rec_deal_ret;

	//校验和
	if(!(CheckPsk(pstDataFreq))) 	goto wifi_rec_deal_ret;


	if(wifi_deal_buff[CMD_POSITION]  == CMD_ENTER_PC)
	{		
		goto wifi_rec_deal_ret;	//收到PC机的确认命令
	}


	wifi_deal_buff =malloc(len);

	if(wifi_deal_buff ==NULL) goto wifi_rec_deal_ret;

	//获得空间
	memcpy(wifi_deal_buff,p,len);
	LPC_UART1->IER	= 0x01;		//WIFI模块通迅允许接收中断


	if(stEepromCfgData.bySysMode & USE_WIFI_REC_DEBUG)
	{
		for(data_len =0;data_len<len;data_len++) uart2_send_byte(wifi_deal_buff[data_len]);
	}
	pstDataFreq = (pSTUartFreq)wifi_deal_buff;



	memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
	data_buff[0] = wifi_deal_buff[CMD_POSITION];
	data_buff[1] = TRUE;
	wifi_send_cmd(source_addr,CMD_ENTER_PC,2,data_buff); //发送确认命令	

	pstDataFreq = (pSTUartFreq)wifi_deal_buff;

	if((pstDataFreq->byCmd!= CMD_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_HANDLE_DISPLAY_DATA) && (pstDataFreq->byCmd!= CMD_DISPLAY_FORM))
	{
		ClrState(bRS485DataTreat);		//因为数据可能已覆盖，将485数据处理标志清0
	}
	
	switch(pstDataFreq->byCmd)
	{
		case CMD_VOICE_PLAY_ROOM:	//报号是否播报房号

			stEepromCfgData.bPlayRoom= pstDataFreq->byData[0];
			save_parameter();
			memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_VOICE_PLAY_ROOM;
			data_buff[1] = TRUE;
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作成功命令
			break;
		
		case CMD_DATE_SEND:		//校时数据
			memcpy(&(stTime.bySecond),&(wifi_deal_buff[DATA0_POSITION]),6);
			
			//构建总线上的校时数据包   
			if((stTime.bySecond>=0x60)||(stTime.byMinute>=0x60)
			 ||(stTime.byHour>=0x24)||(stTime.byDay>=0x32)
			 ||(stTime.byMonth>=0x13)||(stTime.byYearL>0x99))
			{
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_DATE_SEND;
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作失败命令			
				break;
			}
			WriteDS3231_time();
			
			memcpy(&(stBusFreq.bySndSecAddr),&(stTime.bySecond),3);
		    memcpy(&(stBusFreq.byRecSecAddr),&(stTime.byDay),3);
		    stBusFreq.byCmd = CMD_DATE_SEND;   
		   	Bus0OutputData(&(stBusFreq.bySndSecAddr));
			
			memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_DATE_SEND;
			data_buff[1] = TRUE;
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作成功命令				
			break;
		case CMD_POWER_ON:
  			if((pstDataFreq->byData[0] == 0xff) ||(pstDataFreq->byData[1] == 0xff) ||(pstDataFreq->byData[2] == 0xff)||
				(pstDataFreq->byRecRoomAddr ==GLZJ_ADDR2)   )
  			{//全体设备重启
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_POWER_ON;
				data_buff[1] = TRUE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); //发送操作成功命令 

				while(stUsartCfg[2].uiSndLen!=0)	
				{//有数据正在发送中,等待结果码发送完成

				}

	    		while(1);
  			}
			else
			{//某个单一设备重启
				stBusFreq.byCmd = CMD_POWER_ON;
				stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
				stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
				stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
    		break;
		case CMD_DISPLAY_DATA:	 //一帧液晶屏数据
		case CMD_HANDLE_DISPLAY_DATA:	//发送一帧手柄 液晶屏数据
		
			if(stUsartCfg[0].uiSndLen )
			{//RS485有数据正在发送
				SetState(bRS485DataTreat);
			}

			else
			{
				pstDataFreq = (pSTUartFreq)wifi_deal_buff;
				data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
				__disable_irq();
				memcpy(byUsart0SndBuf,wifi_deal_buff,data_len+EXT_SIZE);
				__enable_irq();
							
				stUsartCfg[0].uiSndLen = data_len+EXT_SIZE;
				stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	
				
				LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
				LPC_UART0->IER  |= 0x02;		//允许串口0发送中断
				
				//保存分机地址					
				stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
				stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
				stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;

				//设置等待应答超时2S  在波特率为57.6K的情况下,100字节数据只要17.3ms
				byWaitRS485AckDTime = 95;		//等待1.9S	
			}
			break;
			
		case CMD_DISPLAY_FORM:	//发送一帧床头分机格式
			if(stUsartCfg[0].uiSndLen )
			{//RS485有数据正在发送
			
				SetState(bRS485DataTreat);
			}

			else
			{
				pstDataFreq = (pSTUartFreq)wifi_deal_buff;
				data_len = (pstDataFreq->byLenH<<8)+pstDataFreq->byLenL;
				__disable_irq();
				memcpy(byUsart0SndBuf,wifi_deal_buff,data_len+EXT_SIZE);
				__enable_irq();

							
				stUsartCfg[0].uiSndLen = data_len+EXT_SIZE;
				stUsartCfg[0].uiSndPos = 0; 	//从数据帧头部开始发送数据	

				LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];
				LPC_UART0->IER  |= 0x02;		//允许串口0发送中断

				//保存分机地址					
				stRS485Addr.bySecAddr = pstDataFreq->byRecSecAddr;
				stRS485Addr.byRoomAddr = pstDataFreq->byRecRoomAddr;
				stRS485Addr.byBedAddr = pstDataFreq->byRecBedAddr;	
			}
			break;
	
		case CMD_INFUSION_ANSWER:
		case CMD_SERVICE_ANSWER:
		case CMD_EMERGENCY_ANSWER:
		case CMD_HELP_ANSWER:
			if(uiDevState & FLAG_BUSY)
			{	//忙，退出
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = wifi_deal_buff[CMD_POSITION];
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}
			memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			stBusFreq.byCmd = pstDataFreq->byCmd;	//原命令
			memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
		case CMD_INFUSION_CLEAR:
		case CMD_SERVICE_CLEAR:
		case CMD_EMERGENCY_CLEAR:
		case CMD_HELP_CLEAR:
			memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
			stBusFreq.byCmd = pstDataFreq->byCmd;	//原命令
			memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;
		case CMD_COMM_CALL:
			if(uiDevState & FLAG_BUSY)
			{	//系统忙
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = wifi_deal_buff[CMD_POSITION];
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 

			}
			else
			{
				memcpy(&(stBusFreq.bySndSecAddr),&(stEepromCfgData.bySelfSecAddr),3);
				stBusFreq.byCmd = CMD_COMM_CALL;	//原命令
				memcpy(&(stBusFreq.byRecSecAddr),&(pstDataFreq->byData[0]),3);
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
			break;	
		case CMD_TALK_CLEAR:
			if(0x0000 == (uiDevState & (FLAG_TALK1|FLAG_TALK)))
			{//通道0与通道1都未处于对讲状态
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = wifi_deal_buff[CMD_POSITION];
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 

				break;
			}
			
			if(uiDevState & FLAG_CHANNEL_CHANGE_EXIST)	
			{//有通道切换在总线发送队列中
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = wifi_deal_buff[CMD_POSITION];
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}
			
			if((uiDevState & FLAG_TALK))
			{//通道0处于对讲状态
				if(((wifi_deal_buff[DATA0_POSITION] == stCallAddr.bySndSecAddr)&&
					(wifi_deal_buff[DATA1_POSITION] == stCallAddr.bySndRoomAddr)&&
					(wifi_deal_buff[DATA2_POSITION] == stCallAddr.bySndBedAddr))||
				   ((wifi_deal_buff[DATA0_POSITION] == stCallAddr.byRecSecAddr)&&
					(wifi_deal_buff[DATA1_POSITION] == stCallAddr.byRecRoomAddr)&&
					(wifi_deal_buff[DATA2_POSITION] == stCallAddr.byRecBedAddr))||				   
				   ((wifi_deal_buff[DATA3_POSITION] == stCallAddr.bySndSecAddr)&&
				    (wifi_deal_buff[DATA4_POSITION] == stCallAddr.bySndRoomAddr)&&
				    (wifi_deal_buff[DATA5_POSITION] == stCallAddr.bySndBedAddr))||
				   ((wifi_deal_buff[DATA3_POSITION] == stCallAddr.byRecSecAddr)&&
				    (wifi_deal_buff[DATA4_POSITION] == stCallAddr.byRecRoomAddr)&&
				    (wifi_deal_buff[DATA5_POSITION] == stCallAddr.byRecBedAddr)))
				{//是通道0中的分机,发送系统复位命令
					MakeCH0TimerOut(5,0);//	缩短超时时间,准备退出
					memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
					data_buff[0] = CMD_TALK_CLEAR;
					data_buff[1] = TRUE;
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
					break;
				}
			}
			
			if((uiDevState & FLAG_TALK1))
			{//通道1处于对讲状态
				if(((wifi_deal_buff[DATA0_POSITION] == stCH1CallAddr.bySndSecAddr)&&
					(wifi_deal_buff[DATA1_POSITION] == stCH1CallAddr.bySndRoomAddr)&&
					(wifi_deal_buff[DATA2_POSITION] == stCH1CallAddr.bySndBedAddr))||
				   ((wifi_deal_buff[DATA0_POSITION] == stCH1CallAddr.byRecSecAddr)&&
					(wifi_deal_buff[DATA1_POSITION] == stCH1CallAddr.byRecRoomAddr)&&
					(wifi_deal_buff[DATA2_POSITION] == stCH1CallAddr.byRecBedAddr))||					
				   ((wifi_deal_buff[DATA3_POSITION] == stCH1CallAddr.bySndSecAddr)&&
					(wifi_deal_buff[DATA4_POSITION] == stCH1CallAddr.bySndRoomAddr)&&
					(wifi_deal_buff[DATA5_POSITION] == stCH1CallAddr.bySndBedAddr))||
				   ((wifi_deal_buff[DATA3_POSITION] == stCH1CallAddr.byRecSecAddr)&&
					(wifi_deal_buff[DATA4_POSITION] == stCH1CallAddr.byRecRoomAddr)&&
					(wifi_deal_buff[DATA5_POSITION] == stCH1CallAddr.byRecBedAddr)))


				{//是通道1中的分机,发送系统复位命令
					MakeCH1TimerOut(5,0);//	缩短超时时间,准备退出
					memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
					data_buff[0] = CMD_TALK_CLEAR;
					data_buff[1] = TRUE;
					wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
					break;

				}
			}

			//地址不对
			memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
			data_buff[0] = CMD_TALK_CLEAR;
			data_buff[1] = FALSE;
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			break;
			
		case CMD_NUMBER_SET:		//根据地址修改管理主机地址
			if(wifi_deal_buff[REC_ROOM_POSITION]==GLZJ_ADDR2)
			{//给管理主机编区号,同时将此区号发送给其它下位机设备
				stEepromCfgData.bySelfSecAddr = wifi_deal_buff[DATA0_POSITION];
				save_parameter();
				
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr =	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd		  =	CMD_SYSTEM_SEC_SET;
				stBusFreq.byRecSecAddr =	stEepromCfgData.bySelfSecAddr;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr  = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
			break;	

		case CMD_QUEST:
			stBusFreq.byCmd = wifi_deal_buff[CMD_POSITION];
			stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;	
			
		case CMD_SCHEDULE_SEND:	//护理级别修改命令		
			stBusFreq.bySndSecAddr = wifi_deal_buff[DATA3_POSITION]; //ADDR或data
			stBusFreq.byCmd = wifi_deal_buff[CMD_POSITION];
			stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;	
		case CMD_OPEN_LCD:
		case CMD_CLOSE_LCD:
		case CMD_CLEAR_LCD:
		case CMD_CLEAR_HANDLE_LCD:
		case CMD_CLEAR_MK_LCD:
			stBusFreq.byCmd = wifi_deal_buff[CMD_POSITION];
			stBusFreq.byRecSecAddr = pstDataFreq->byData[0];
			stBusFreq.byRecRoomAddr = pstDataFreq->byData[1];
			stBusFreq.byRecBedAddr = pstDataFreq->byData[2];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case CMD_STOP_VOICE:			
			stBusFreq.byCmd = wifi_deal_buff[CMD_POSITION];
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			ClrState(bOriginMusic);
			break;

		case CMD_START_VOICE:			
		case CMD_BROADCAST1:
		case CMD_BROADCAST2:
		case CMD_BROADCAST3:
			if((uiDevState & FLAG_BUSY)||(uiDevState & FLAG_TALK1))
			{	//系统忙
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = wifi_deal_buff[CMD_POSITION];
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}

			stBusFreq.byCmd = wifi_deal_buff[CMD_POSITION];
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case CMD_MUSIC_PLAY:			
			if((uiDevState & FLAG_BUSY)||(uiDevState & FLAG_TALK1))
			{	//系统忙
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_MUSIC_PLAY;
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				break;
			}

			stBusFreq.byCmd = CMD_MUSIC_PLAY;
			stBusFreq.byRecSecAddr = 0xff;
			stBusFreq.byRecRoomAddr = 0xff;
			stBusFreq.byRecBedAddr = 0xff;
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			SetState(bOriginMusic);
			break;

			
		case CMD_VOICE_TIMER_SET:
			if((wifi_deal_buff[DATA0_POSITION]>0x23) || (wifi_deal_buff[DATA1_POSITION]>0x59) ||
			   (wifi_deal_buff[DATA2_POSITION]>0x23) || (wifi_deal_buff[DATA3_POSITION]>0x59) ||
			   (wifi_deal_buff[DATA4_POSITION]>7))
			{

			}
			
			else
			{
				stEepromCfgData.byLVoiceStartHour =  wifi_deal_buff[DATA0_POSITION]; //起始小时
				stEepromCfgData.byLVoiceStartMin =  wifi_deal_buff[DATA1_POSITION];	//起始分钟
				stEepromCfgData.byLVoiceEndHour  = wifi_deal_buff[DATA2_POSITION]; //结束小时
				stEepromCfgData.byLVoiceEndMin = wifi_deal_buff[DATA3_POSITION];	//结束分钟
				stEepromCfgData.byLVoiceVal =  wifi_deal_buff[DATA4_POSITION]; //音量值
				save_parameter();
				
				stBusFreq.bySndSecAddr = stEepromCfgData.byLVoiceStartHour; //起始小时
				stBusFreq.bySndRoomAddr = stEepromCfgData.byLVoiceStartMin;	//起始分钟
				stBusFreq.bySndBedAddr = 0x00;
				stBusFreq.byCmd = CMD_VOICE_TIMER_SET;
				stBusFreq.byRecSecAddr = stEepromCfgData.byLVoiceEndHour; //结束小时
				stBusFreq.byRecRoomAddr = stEepromCfgData.byLVoiceEndMin;	//结束分钟
				stBusFreq.byRecBedAddr = stEepromCfgData.byLVoiceVal; //音量值
				Bus0OutputData(&(stBusFreq.bySndSecAddr));

				check_little_voice_status();				//检查是否设置了小音量时段,如果设置了则置标志位,并对起始和结束时间赋值
			}
			break;
			
		case CMD_BL_TIMER_SET:
			if((wifi_deal_buff[DATA0_POSITION]>0x23) || (wifi_deal_buff[DATA1_POSITION]>0x59) ||
			   (wifi_deal_buff[DATA2_POSITION]>0x23) || (wifi_deal_buff[DATA3_POSITION]>0x59) )	//时间信息不对
			  
			{

			}
			else
			{
				stEepromCfgData.byBLOffStartHour = 	wifi_deal_buff[DATA0_POSITION];	//起始小时
				stEepromCfgData.byBLOffStartMin = 	wifi_deal_buff[DATA1_POSITION];	//起始分钟
				stEepromCfgData.byBLOffEndHour = 	wifi_deal_buff[DATA2_POSITION]; //结束小时
				stEepromCfgData.byBLOffEndMin =  	wifi_deal_buff[DATA3_POSITION];	//结束分钟
				save_parameter();
				
				stBusFreq.bySndSecAddr = stEepromCfgData.byBLOffStartHour; //起始小时
				stBusFreq.bySndRoomAddr = stEepromCfgData.byBLOffStartMin;	//起始分钟
				stBusFreq.bySndBedAddr = 0x00;
				stBusFreq.byCmd = CMD_BL_TIMER_SET;
				stBusFreq.byRecSecAddr = stEepromCfgData.byBLOffEndHour; //结束小时
				stBusFreq.byRecRoomAddr = stEepromCfgData.byBLOffEndMin;	//结束分钟
				stBusFreq.byRecBedAddr = 0x00; //音量值
				Bus0OutputData(&(stBusFreq.bySndSecAddr));

				init_black_light_status();
			}
			break;	

			
		case CMD_ENTER:	//来自PC机上的登记确认命令
			if(byWaitAckTimeLanding)
			{//正在等待PC机上的登记确认命令
				byWaitAckTimeLanding = 0;
				uiDetectPcTime = 30000;	//侦听PC机间隔10分钟
				
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;	
				stBusFreq.byCmd = CMD_PC_OPEN;
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr =0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				SetState(bPCOpen);   //集成主机开机状态
			}
			break;
			
		case CMD_RS485_BRT_SET:
			if(wifi_deal_buff[DATA0_POSITION] >0 && wifi_deal_buff[DATA0_POSITION] <10)
			{
				switch(wifi_deal_buff[DATA0_POSITION])
				{
					case 1:
						stEepromCfgData.uiRs485Brt	=1200;
						break;
					case 2:
						stEepromCfgData.uiRs485Brt	= 2400;
						break;
					case 3:
						stEepromCfgData.uiRs485Brt	= 4800;
						break;
					case 4:
						stEepromCfgData.uiRs485Brt	= 9600;
						break;
					case 5:
						stEepromCfgData.uiRs485Brt	= 14400;
						break;
					case 6:
						stEepromCfgData.uiRs485Brt	= 19200;
						break;
					case 7:
						stEepromCfgData.uiRs485Brt	= 28800;
						break;
					case 8:
						stEepromCfgData.uiRs485Brt	= 38400;
						break;
					case 9:
						stEepromCfgData.uiRs485Brt	= 57600;
						break;
					default:
						stEepromCfgData.uiRs485Brt	=1200;
						break;				
				}					
				//同时将RS485总线的波特（串口0）进行设置
				init_uart0(stEepromCfgData.uiRs485Brt);
				save_parameter();				
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_RS485_BRT_SET;
				stBusFreq.byRecSecAddr	=	wifi_deal_buff[DATA0_POSITION];	//波特率代码
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndBrtTimes = RE_SND_TIMES;
			}
			else 
			{
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_RS485_BRT_SET;
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			}
			break;

		case CMD_RS485_INFO_SEC_ROOM:
			if(wifi_deal_buff[DATA0_POSITION] ==0 || wifi_deal_buff[DATA0_POSITION] ==1)
			{
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_RS485_INFO_SEC_ROOM;
				stBusFreq.byRecSecAddr	=	wifi_deal_buff[DATA0_POSITION];	//床头分机接怍RS485信息是否需要判断房号
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndRs485RoomTimes = RE_SND_TIMES;			//重复发送三次
			}
			else
			{
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_RS485_INFO_SEC_ROOM;
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			}
			break;

		case CMD_BED_VOICE_PLAY:
			if(wifi_deal_buff[DATA0_POSITION] ==0 || wifi_deal_buff[DATA0_POSITION] ==1)
			{
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_BED_VOICE_PLAY;
				stBusFreq.byRecSecAddr	=	wifi_deal_buff[DATA0_POSITION];	//床头分机是否需要语音报号
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
				byReSndBedVoiPlaTimes = RE_SND_TIMES;			//重复发送三次	
			}
			else 
			{
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_BED_VOICE_PLAY;
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
			}
			break;
			
		case CMD_DISP_MODE:
			if(wifi_deal_buff[DATA0_POSITION] ==0 || wifi_deal_buff[DATA0_POSITION] ==1)
			{			
				stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd			=	CMD_DISP_MODE;	
				stBusFreq.byRecSecAddr	=	wifi_deal_buff[DATA0_POSITION];
				stBusFreq.byRecRoomAddr	=	wifi_deal_buff[DATA1_POSITION];
				stBusFreq.byRecBedAddr	=	wifi_deal_buff[DATA2_POSITION];
				Bus0OutputData(&(stBusFreq.bySndSecAddr));
			}
			else 
			{
				memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
				data_buff[0] = CMD_DISP_MODE;
				data_buff[1] = FALSE;
				wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 
				
			}
			break;

		case CMD_COLOR_CLEAR:
			stBusFreq.bySndSecAddr 	= 	stEepromCfgData.bySelfSecAddr;
			stBusFreq.bySndRoomAddr	=	stEepromCfgData.bySelfRoomAddr;
			stBusFreq.bySndBedAddr	=	stEepromCfgData.bySelfBedAddr;
			stBusFreq.byCmd			=	CMD_COLOR_CLEAR;	
			stBusFreq.byRecSecAddr	=	wifi_deal_buff[DATA0_POSITION];
			stBusFreq.byRecRoomAddr	=	wifi_deal_buff[DATA1_POSITION];
			stBusFreq.byRecBedAddr	=	wifi_deal_buff[DATA2_POSITION];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case 0x5b:	//给下位机设备编号
			memcpy(&(stBusFreq.bySndSecAddr), &(wifi_deal_buff[DATA0_POSITION]),3);
			stBusFreq.byCmd = CMD_NUMBER_SET;
			stBusFreq.byRecSecAddr =  wifi_deal_buff[DATA3_POSITION];
			stBusFreq.byRecRoomAddr = wifi_deal_buff[DATA4_POSITION];
			Bus0OutputData(&(stBusFreq.bySndSecAddr));
			break;

		case CMD_VOLUME_SET:
			switch(wifi_deal_buff[DATA0_POSITION])
			{
				case 1://语音报号主机音量
					stEepromCfgData.byHostPlayVol = wifi_deal_buff[DATA1_POSITION];
					save_parameter();
					break;
				case 2://语音报号分机音量
					stEepromCfgData.bySlaPlayVol = wifi_deal_buff[DATA1_POSITION];
					save_parameter();
					break;				
				case 3://对讲通话音量
					stEepromCfgData.byHostTalkVol = wifi_deal_buff[DATA1_POSITION];
					save_parameter();
					break;
				case 4://对讲分机音量
					stEepromCfgData.bySlaTalkVol = wifi_deal_buff[DATA1_POSITION];
					save_parameter();
					break;						
			}
			//break;
			//继default发送结果命令
		default:
			memcpy(&(source_addr.bySecAddr),&(wifi_deal_buff[REC_SEC_POSITION]),3);
			data_buff[0] = wifi_deal_buff[CMD_POSITION];
			data_buff[1] = TRUE;
			wifi_send_cmd(source_addr,CMD_RESULT_PC,2,data_buff); 		//其它命令都 发结果成功，以免引起重发
			break;
	}
	free(wifi_deal_buff);
wifi_rec_deal_ret:
	LPC_UART1->IER	= 0x01;		//WIFI模块通迅允许接收中断	
	}
	init_scon1_Rcvbuf();
}




