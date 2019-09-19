#ifndef _WPA_CLI_H_
#define _WPA_CLI_H_


#define AP_LED_ON	0
#define AP_LED_OFF	1

#define SERVER_LED_ON		0
#define SERVER_LED_OFF	1




extern uint16_t WpaState;

#define EXIST_SSID			(1<<0)			//搜寻到正确的SSID
#define CONNECTED_WIFI		(1<<1)			//已正确连上,且获得IP地址
#define SERIA_NET			(1<<2)			//透传模式
#define CONNECTED_SERVER 	(1<<3)			//已连上服务器
#define CHECK_ESP8266		(1<<5)			//检查WIFI模块
#define WIFI_REC 			(1<<6)			//WIFI接收到数据


#define WIFI_AP_LED			(1<<8)			//是否连上wifi  AP (路由器)指示灯
#define WIFI_SERVER_LED		(1<<9)			//是否连上wifi server(APP软件)指示灯



#define SetWpaState(x) (WpaState|=(x))			//置位操作
#define ClrWpaState(x) (WpaState&=~(x))		//清零操作
#define GetWpaState(x) (WpaState&(x))			//获取位状态



#define CHECK_WIFI_TIME	60000		//30秒检测一次wifi
#define CHECK_WIFI_SHORT_TIME 20000	//非正常时10秒检测一次wifi



#define IP_ADDR		192.168.88.229
#define PORT		9999

#ifndef _IN_WPA_CLI_

extern unsigned char ssid[];
extern unsigned char pwd[];

extern uint16_t check_esp8266_time;

extern void init_pwd(void);
extern void init_ssid(void);
extern void init_esp8266(void);
extern void check_esp8266(void);
extern void get_right_ssid(char * ssid ,int area,int room,int bed);
extern uint8_t wifi_send_data(uint8_t *pdata,uint8_t data_len);
extern uint8_t wifi_send_cmd(STAddr_S sur_addr,uint8_t Cmd,uint8_t byLen,uint8_t *pData);
extern void wifi_rec_deal(uint8_t *p,uint16_t len);
extern unsigned int crc32(unsigned int crc, unsigned char *buffer, int size);
extern void wifi_server_led_flash(void);
extern void wifi_ap_led_flash(void);
#endif





#endif




















