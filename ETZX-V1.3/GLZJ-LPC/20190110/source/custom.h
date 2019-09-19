#include "LPC17xx.h"                                                    /* LPC17xx外设寄存器            */
#include "lpc1700_reg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
//---------------------------------------------------------------
#define MALLOC_BUF	0


#define INIT_FLAG 0x5a

#define PARA_ADDR	0x3ff000	//4M 字节最后一个扇区
#define FLASH_TEST_ADDR	0x3ff000


//本机配置字结构体
typedef struct _STEepromCfgData
{
	uint8_t  byInitFlag;									//初始化标志
    uint8_t  bySelfSecAddr;									//区地址
    uint8_t  bySelfRoomAddr;								//房地址  
    uint8_t  bySelfBedAddr;									//床地址
    uint16_t uiRs485Brt;									//RS485波特率

    uint8_t  byBLOffStartHour;								//背光关起始小时
    uint8_t  byBLOffStartMin;								//背光关起始分钟
    uint8_t  byBLOffEndHour;								//背光关结束小时
    uint8_t  byBLOffEndMin;									//背光关结束分钟
    uint8_t  byLVoiceStartHour;								//小音量时段起始小时
    uint8_t  byLVoiceStartMin;								//小音量时段起始分钟
    uint8_t  byLVoiceEndHour;								//小音量时段结束小时
    uint8_t  byLVoiceEndMin;								//小音量时段结束分钟
    uint8_t	 byLVoiceVal;									//小音量值

    uint8_t  bPlayRoom;										//播放房号标志

	uint8_t  byHostPlayVol;									//主机报号音量
	uint8_t  bySlaPlayVol;									//分机报号音量

	uint8_t	 byHostPlayLittleVol;							//主机报号小音量
	uint8_t  bySlaPlayLittleVol;							//分机报号小音量

	
	uint8_t  byHostTalkVol;									//主机通话音量
	uint8_t  bySlaTalkVol;									//分机通话音量
    
    uint8_t  byVersionH;									//版本高位
    uint8_t  byVersionL;									//版本低位

	uint8_t	 bySysMode;										//系统模式
	//高4位WIFI相关，低4位串口相关
/*	#define USE_WIFI_MODE		0x80						//使用WIFI方式进行床头卡信息发送
	#define USE_WIFI_DEBUG		0x40						//将WIFI连接过程中及检测过程中的信息打印出来,须依赖USE_WIFI_MODE=1
	#define USE_WIFI_REC_DEBUG	0x20						//将WIFI接收床头卡的信息打印出来,须依赖USE_WIFI_MODE=1

	#define USE_USART2_MODE		0x08						//使用串口接收床头卡信息(包括更改区号)
	#define USE_USART2_SD_DATA	0x04						//使用串口发送应答、结果数据
	#define USE_TIME_DEBUG			0X01						//监测管理主机时间数据正不正确

*/
	
} STEepromCfgData, *pSTEepromCfgData;




/*数据帧格式
字节1：发送方区地址（一个字节）
字节2：发送方房地址（一个字节）
字节3：发送方床地址（一个字节）
字节4：命令代码（一个字节）
字节5：接收方区地址（一个字节）
字节6：接收方房地址（一个字节）
字节7：接收方床地址（一个字节）
注:数据桢在不同的命令中可能有不同含义
*/

/*单总线协议
  命令代码:一字节
  数据代码:6字节(根据命令的不同数据格式)
*/
typedef struct
{
	uint8_t bySndSecAddr;			/*主动发送方区地址*/
	uint8_t bySndRoomAddr;		/*主动发送方房地址*/
	uint8_t bySndBedAddr;			/*主动发送方床地址*/
	uint8_t byCmd;				/*命令*/
	uint8_t byRecSecAddr;			/*接收方区地址*/
	uint8_t byRecRoomAddr;		/*接收方房地址*/
	uint8_t byRecBedAddr;			/*接收方床地址*/	
} STBusFreq, *pSTBusFreq;	



//双向链表节点
typedef struct _STNod
{
    uint8_t  byCallSecAddr;									//呼叫方区地址
    uint8_t  byCallRoomAddr;									//呼叫方房地址
    uint8_t  byCallBedAddr;									//呼叫方床地址
    uint8_t  byCallPriority;									//呼叫优先级
    struct _STNod *pstPrev;
    struct _STNod *pstNext;    
} STNod, *pSTNod;
//双向链表结构体
typedef struct _STPointer
{
    struct _STNod *pstHead;									//链表头节点
    struct _STNod *pstTail;									//链表尾节点    
} STPointer, *pSTPointer;
//信息指示方结构体
typedef struct _STIndicationData
{
    uint8_t  byIndicationSecAddr;								//指示方区地址
    uint8_t  byIndicationRoomAddr;							//指示方房地址  
    uint8_t  byIndicationBedAddr;								//指示方床地址 
    uint8_t  byCallCmd;										//指示的命令
    uint8_t  byState;											//该呼叫信息的状态
    struct _STNod *pstNext;    								//下一条呼叫信息指针
} STIndicationData, *pSTIndicationData;
//超时管理结构体
typedef struct _STTimerout
{	
	uint8_t  byTimerOutSet;									//一次超时时间设置(20ms单位)
	uint8_t  byTimerOut;										//一次超时时间(20ms单位)
	uint8_t  byTimerOutCount;									//允许超时次数	
} STTimerout, *pSTTimerout;
//地址结构体
typedef struct
{
	uint8_t  bySndSecAddr;										//发送方区地址
    uint8_t  bySndRoomAddr;										//发送方房地址  
    uint8_t  bySndBedAddr;										//发送方床地址
    uint8_t  byRecSecAddr;
	uint8_t  byRecRoomAddr;
	uint8_t  byRecBedAddr;

    uint8_t  byCmd;											//原命令
} STAddr, *pSTAddr;


//地址结构体
typedef struct
{
	uint8_t  bySecAddr;										//区地址
    uint8_t  byRoomAddr;									//房地址  
    uint8_t  byBedAddr;										//床地址									
} STAddr_S, *pSTAddr_S;


typedef struct queue
{
	uint16_t queuesize;		//数组的大小
	uint16_t head,tail;		//队列的头和尾下标
	uint8_t *q;				//数组头指针
	
}Queue;



//时间结构体
typedef struct
{
uint8_t bySecond;
uint8_t byMinute;
uint8_t byHour;
//uint8_t byWeek;
uint8_t byDay;
uint8_t byMonth;
uint8_t byYearL;
//uint8_t byYearH;
}STTime;



typedef struct{
  uint16_t		Year;
  uint8_t 		Month;
  uint8_t		Date;
  uint8_t		Week;
  uint8_t		Hour;
  uint8_t		Min;
  uint8_t		Sec;
}Rtc_struct;


//串口结构体定义
typedef struct
{
	uint16_t uiSndLen;		/*发送数据长度*/
	uint16_t uiSndPos;		/*发送数据位置*/	
} STUsartCfg, pSTUsartCfg;



typedef struct
{
	uint8_t	byStartByte1;					//数据帧起始字符1
	uint8_t 	bySndSecAddr;					//主动发送方区地址
	uint8_t 	bySndRoomAddr;					//主动发送方房地址
	uint8_t 	bySndBedAddr;					//主动发送方床地址	
	uint8_t 	byRecSecAddr;					//收方区地址
	uint8_t 	byRecRoomAddr;					//收方房地址
	uint8_t 	byRecBedAddr;					//收方床地址
	uint8_t 	byCmd;							//命令
	uint8_t		byLenH;							//长度高字节
	uint8_t 	byLenL;							//长度低字节
	uint8_t   byData[1];						//链路数据	
} STUartFreq, *pSTUartFreq;


#include "uart.h"
//#include "sst25vfxxx.h"
#include "pct25vf.h"
#include "global.h"
#include "wdt.h"
#include "voice_play.h"
#include "timer.h"
#include "interrupt.h"
#include "Extint.h"
#include "SingleBus.h"
#include "tel.h"
#include "ds3231.h"
#include "wpa_cli.h"
#include "fm62429.h"


#define	enable_wdt				


//高4位WIFI相关，低4位串口相关
#define USE_WIFI_MODE		0x80						//使用WIFI方式进行床头卡信息发送
#define USE_WIFI_DEBUG		0x40						//将WIFI连接过程中及检测过程中的信息打印出来,须依赖USE_WIFI_MODE=1
#define USE_WIFI_REC_DEBUG	0x20						//将WIFI接收床头卡的信息打印出来,须依赖USE_WIFI_MODE=1

#define USE_USART2_MODE 	0x08						//使用串口接收床头卡信息(包括更改区号)

#define USE_TIME_DEBUG		0X01						//监测管理主机时间数据正不正确

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef true
#define true  1
#endif
#ifndef false
#define false 0
#endif


#define PLAY_HOST_CH	0
#define PLAY_SLAVE_CH	1

#define TALK_HOST_CH	0
#define TALK_SLAVE_CH	1

/////////////////////////////////////////////////////////////////////////////
#define		sst_cs			(1ul<<16)		//p0.16
#define		sst_sck			(1ul<<15)		//p0.15
#define		sst_miso		(1ul<<17)		//p0.17
#define		sst_mosi		(1ul<<18)		//p0.18

#define		sst_cs_ctrl(x)			(x?(FIO0SET = sst_cs):(FIO0CLR = sst_cs))
#define 	sst_sck_ctrl(x)			(x?(FIO0SET = sst_sck):(FIO0CLR = sst_sck))
#define 	sst_mosi_ctrl(x)		(x?(FIO0SET = sst_mosi):(FIO0CLR = sst_mosi))
#define 	sst_miso_status			(FIO0PIN&sst_miso)		
/////////////////////////////////////////////////////////////////////////////
#define		int0_status				(FIO2PIN&(1ul<<12))
#define		MTT_TD_pin				(1ul<<12)		//p2.12
#define		MTT_TD_pin_status		(FIO2PIN&MTT_TD_pin)
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#define		sd_rx_pin				(1ul<<1)		//p0.1
#define		sd_rx_pin_status		(FIO0PIN&sd_rx_pin)

#define 	sd_tx_pin				(1ul<<13)		//p2.13
#define		sd_tx_ctrl(x)			(x?(FIO2SET = sd_tx_pin):(FIO2CLR = sd_tx_pin))	

/////////////////////////////////////////////////////////////////////////////
#define 	P0Int    1					//PORT0 GPIO中断挂起状态
#define 	P2Int    (1ul<<2)			//PORT2 GPIO中断挂起状态
/////////////////////////////////////////////////////////////////////////////
#define     tlp_irq_pin				(1ul<<23)
#define 	phone_tlp				(1ul<<23)		//p0.23 电话摘机
#define 	phone_tlp_status		(FIO0PIN&phone_tlp)

#define		phone_ring				(1ul<<31)		//p1.31
#define		phone_ring_ctrl(x)		(x?(FIO1SET = phone_ring):(FIO1CLR = phone_ring))


#define		phone_tlq				(1ul<<28)	   	//p1.28 电话开关
#define		phone_tlq_ctrl(x)		(x?(FIO1SET = phone_tlq):(FIO1CLR = phone_tlq))	


#define 	tel_ctr_pin				(1ul<<5)		//p0.5   电压选择
#define 	AC36V					1				//交流振铃
#define 	DC60V					0				//待机、通话直流电压
#define		tel_ctrl(x)				(x?(FIO0SET = tel_ctr_pin):(FIO0CLR = tel_ctr_pin))	

#define 	AM79R70_c1_pin			(1ul<<18)		//p1.18
#define 	AM79R70_c1_ctrl(x)		(x?(FIO1SET = AM79R70_c1_pin):(FIO1CLR = AM79R70_c1_pin))


#define 	AM79R70_c2_pin			(1ul<<19)		//p1.19
#define 	AM79R70_c2_ctrl(x)		(x?(FIO1SET = AM79R70_c2_pin):(FIO1CLR = AM79R70_c2_pin))


#define 	AM79R70_c3_pin			(1ul<<22)		//p1.22
#define 	AM79R70_c3_ctrl(x)		(x?(FIO1SET = AM79R70_c3_pin):(FIO1CLR = AM79R70_c3_pin))

/////////////////////////////////////////////////////////////////////////////
#define mt8870_dq1		(FIO0PIN&(1<<19))
#define mt8870_dq2		(FIO0PIN&(1<<20))
#define mt8870_dq3		(FIO0PIN&(1<<21))
#define mt8870_dq4		(FIO0PIN&(1<<22))
/////////////////////////////////////////////////////////////////////////////
//ds3231芯片
#define DS3231_SCL			(1<<3)	//p2.3
#define DS3231_SDA			(1<<2)	//p2.2
#define DS3231_RST          (1<<4)  //P2.4

#define DS3231_SDA_MOD		4

#define Set_DS3231_SDA      FIO2SET = DS3231_SDA
#define Clr_DS3231_SDA	    FIO2CLR = DS3231_SDA

#define Set_DS3231_SCL  	FIO2SET = DS3231_SCL
#define Clr_DS3231_SCL  	FIO2CLR = DS3231_SCL

#define Set_DS3231_RST  	FIO2SET = DS3231_RST
#define Clr_DS3231_RST      FIO2CLR = DS3231_RST


#define DS3231_SDA_status	(FIO2PIN&DS3231_SDA)		//结果不为0，IO状态为高电平


#define DS3231_DELAY   5

#define DS3231_SDA_INPUT()    LPC_GPIO2->FIODIR &= ~DS3231_SDA		//该位为0，用作输入
#define DS3231_SDA_OUTPUT()   LPC_GPIO2->FIODIR |= DS3231_SDA

/////////////////////////////////////////////////////////////////////////////
#define gb_fz_pin					(1ul<<26)		//P3.26 	广播负载
#define	gb_fz_ctrl(x)				(x?(FIO3SET = gb_fz_pin):(FIO3CLR = gb_fz_pin))	

#define xtd_pin						(1ul<<25)		//p3.25	对讲时SD通道控制
#define	xtd_ctrl(x)					(x?(FIO3SET = xtd_pin):(FIO3CLR = xtd_pin))

#define xta_pin						(1ul<<20)		//p1.20	对讲时SA通道控制
#define	xta_ctrl(x)					(x?(FIO1SET = xta_pin):(FIO1CLR = xta_pin))


#define kbd_pin						(1ul<<21)		//p1.21	播放背景音乐SA通道控制
#define	kbd_ctrl(x)					(x?(FIO1SET = kbd_pin):(FIO1CLR = kbd_pin))

#define kgb_pin						(1ul<<23)		//p1.23	语音报号时SD通道控制
#define	kgb_ctrl(x)					(x?(FIO1SET = kgb_pin):(FIO1CLR = kgb_pin))

#define kbc_pin						(1ul<<24)		//p1.24	语音报号时SA通道控制
#define	kbc_ctrl(x)					(x?(FIO1SET = kbc_pin):(FIO1CLR = kbc_pin))


#define cgb_pin						(1ul<<29)		//p1.29	通话时电话端SD控制
#define	cgb_ctrl(x)					(x?(FIO1SET = cgb_pin):(FIO1CLR = cgb_pin))

#define cbd_pin						(1ul<<0)		//p0.0	通话时电话端SA控制
#define	cbd_ctrl(x)					(x?(FIO0SET = cbd_pin):(FIO0CLR = cbd_pin))


/////////////////////////////////////////////////////////////////////////////

#define power_en_pin				(1ul<<25)		//p1.25  lm317电源控制
#define	power_en_ctrl(x)			(x?(FIO1SET = power_en_pin):(FIO1CLR = power_en_pin))

#define para_reset_key				(1ul<<26)		//p1.26   para_rst _key  参数复位按键 
#define	para_reset_key_status		(FIO1PIN&para_reset_key)

#define soft_reset_key				(1ul<<8)		//p0.8	用于系统软重启
#define soft_reset_key_status		(FIO0PIN&soft_reset_key)

#define wifi_ap_led_pin				(1ul<<7)		//p0.7	power led 显示
#define	wifi_ap_led_ctrl(x)			(x?(FIO0SET = wifi_ap_led_pin):(FIO0CLR = wifi_ap_led_pin))

#define wifi_server_led_pin				(1ul<<6)		//p0.6	wifi  led 显示
#define	wifi_server_led_ctrl(x)			(x?(FIO0SET = wifi_server_led_pin):(FIO0CLR = wifi_server_led_pin))
/////////////////////////////////////////////////////////////////////////////

#define msm_txe_pin					(1ul<<4)		//p0.4	来电显示模块发送允许
#define	msm_txe_ctrl(x)				(x?(FIO0SET = msm_txe_pin):(FIO0CLR = msm_txe_pin))


#define msm_xd_pin					(1ul<<28)		//p4.28	来电显示模块发送
#define	msm_xd_ctrl(x)				(x?(FIO4SET = msm_xd_pin):(FIO4CLR = msm_xd_pin))

#define msm_rx_pin					(1ul<<29)		//p4.29	来电显示模块接收
#define	msm_rx_ctrl(x)				(FIO4PIN&msm_rx_pin)

/////////////////////////////////////////////////////////////////////////////

#define esp_rst_pin					(1ul<<9)		//p0.9	WIFI模块复位脚
#define	esp_rst_ctrl(x)				(x?(FIO0SET = esp_rst_pin):(FIO0CLR = esp_rst_pin))


/////////////////////////////////////////////////////////////////////////////
#define fm624_dat1_pin				(1ul<<9)		//p2.9	语音报号 音量电位器
#define	fm624_dat1_ctrl(x)			(x?(FIO2SET = fm624_dat1_pin):(FIO2CLR = fm624_dat1_pin))


#define fm624_clk1_pin				(1ul<<8)		//p2.8	语音报号 音量电位器
#define	fm624_clk1_ctrl(x)			(x?(FIO2SET = fm624_clk1_pin):(FIO2CLR = fm624_clk1_pin))



#define fm624_dat2_pin				(1ul<<27)		//p0.27	通话音量电位器
#define	fm624_dat2_ctrl(x)			(x?(FIO0SET = fm624_dat2_pin):(FIO0CLR = fm624_dat2_pin))


#define fm624_clk2_pin				(1ul<<28)		//p0.28	通话音量电位器
#define	fm624_clk2_ctrl(x)			(x?(FIO0SET = fm624_clk2_pin):(FIO0CLR = fm624_clk2_pin))



/////////////////////////////////////////////////////////////////////////////
#define NULL			0


#define	add			1
#define delete		0
#define MAX_VICS                    51                                  /*  异常向量表表项数目          */

/*********************************************************************************************************
  返回值定义
*********************************************************************************************************/
#define     ZY_OK               0                                       /*  操作成功                    */
#define     ZY_NOT_OK           1                                       /*  操作失败                    */
#define     ZY_PARAMETER_ERR    2                                       /*  参数错误                    */
#define     ZY_NO_FIND_FUNCTION 6                                       /*  没有发现指定函数            */
#define     ZY_NO_MEMORY        12                                      /*  内存不足                    */
#define     ZY_TIME_OUT         13                                      /*  超时                        */
#define     ZY_NO_FIND_OBJECT   16                                      /*  没有发现对象                */


#define IAP_ENTER_ADR   0x1FFF1FF1                                      // IAP入口地址定义              /

/* 
*  定义IAP命令字
*/                                     
#define IAP_Prepare             50                                      // 选择扇区                     /
                                                                        // 【起始扇区号、结束扇区号】   /                    
#define IAP_RAMTOFLASH          51                                      // 拷贝数据 FLASH目标地址       /
                                                                        // RAM源地址    【、写入字节数  /
                                                                        // 系统时钟频率】               /
#define IAP_ERASESECTOR         52                                      // 擦除扇区    【起始扇区号     /
                                                                        // 结束扇区号、系统时钟频率】   /
#define IAP_BLAND_CHK            53                                      // 查空扇区    【起始扇区号、   /
                                                                        // 结束扇区号】                 
#define IAP_READPARTID          54                                      // 读器件ID    【无】           /
#define IAP_BOOTCODEID          55  									// 读Boot版本号【无】           /
#define IAP_COMPARE             56 										// 比较命令    【Flash起始地址RAM起始地址、需要比较的字节数】 

#define Read_chip_serial		58

//定义IAP返回状态字
#define CMD_SUCCESS                                0
#define INVALID_COMMAND                            1
#define SRC_ADDR_ERROR                             2 
#define DST_ADDR_ERROR                             3
#define SRC_ADDR_NOT_MAPPED                        4
#define DST_ADDR_NOT_MAPPED                        5
#define COUNT_ERROR                                6
#define INVALID_SECTOR                             7
#define SECTOR_NOT_BLANK                           8
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION    9
#define COMPARE_ERROR                              10
#define BUSY                                       11


//定义CCLK值大小，单位为KHz 
#define IAP_FCCLK            (25000)

//定义函数指针  
extern void (*IAP_Entry) (uint32_t ulParam_tab[], uint32_t ulPesult_tab[]);	










