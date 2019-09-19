#ifndef _VOICE_PLAY_H_
#define _VOICE_PLAY_H_

/*
序号	声音			起始地址
0		0.pcm：		0x00000000
1		1.pcm：		0x0000258e
2		2.pcm：		0x000042f2
3		3.pcm：		0x000061f8
4		4.pcm：		0x00008406
5		5.pcm：		0x0000a486
6		6.pcm：		0x0000c4da
7		7.pcm：		0x0000e2da
8		8.pcm：		0x000102aa
9		9.pcm：		0x00011d10
10		DTMF0.pcm：	0x00014098
11		DTMF1.pcm：	0x00016ed8
12		DTMF2.pcm：	0x00019d10
13		DTMF3.pcm：	0x0001cb4a
14		DTMF4.pcm：	0x0001f964
15		DTMF5.pcm：	0x000227a0
16		DTMF6.pcm：	0x000255da
17		DTMF7.pcm：	0x00028410
18		DTMF8.pcm：	0x0002b25e
19		DTMF9.pcm：	0x0002e0ac
20		DTMFPSOUND.pcm：0x00030ee4
21		DTMFSTART.pcm：	0x00033d24
22		Tock.pcm：		0x00036b62
23		供氧.pcm：	0x000372e2
24		加.pcm：		0x0003a3ca
25		十.pcm：		0x0003bbfe
26		卫生间.pcm：0x0003e04c
27		号.pcm：		0x0004138a
28		呼叫.pcm：	0x000433fc
29		完毕.pcm：	0x000460fc
30		床.pcm：		0x000489e8
31		开始.pcm：	0x0004ae3a
32		成功.pcm：	0x0004db50
33		房.pcm：		0x0005066c
34		百.pcm：		0x00052082
35		输液 .pcm：	0x00053f0c
36        请挂好电话机.pcm：0x00056c58
37        已连上无线路由器.pcm：0x0005ce10
38       已连上服务器.pcm：0x00063c76

*/



#define DTMF0_NUM	10
#define DTMF1_NUM	11
#define DTMF2_NUM	12
#define DTMF3_NUM	13
#define DTMF4_NUM	14
#define DTMF5_NUM	15
#define DTMF6_NUM	16
#define DTMF7_NUM	17
#define DTMF8_NUM	18
#define DTMF9_NUM	19
#define DTMFPSOUND_NUM	20
#define DTMFSTART_NUM	21
#define TOCK_NUM	22
#define GY_NUM		23
#define JIA_NUM		24
#define SHI_NUM		25
#define WSJ_NUM		26
#define HAO_NUM		27
#define HJ_NUM		28
#define WB_NUM		29
#define CHUANG_NUM	30
#define KSH_NUM		31
#define CHG_NUM		32
#define FANG_NUM	33
#define BAI_NUM		34
#define SHY_NUM		35
#define HANG_TEL_NUM	36
#define CONNECTED_WIFI_NUM	37
#define CONNECTED_SERVER_NUM	38
#define MAX_NUM		39


//dataflash
//数据存储器数据结构映射表

#define PCM_0_ADDR		0x00000000
#define PCM_1_ADDR		0x0000258e
#define PCM_2_ADDR		0x000042f2	
#define	PCM_3_ADDR		0x000061f8
#define	PCM_4_ADDR		0x00008406
#define PCM_5_ADDR		0x0000a486
#define	PCM_6_ADDR		0x0000c4da	
#define	PCM_7_ADDR		0x0000e2da
#define	PCM_8_ADDR		0x000102aa
#define	PCM_9_ADDR		0x00011d10
#define	PCM_DTMF0_ADDR	0x00014098
#define	PCM_DTMF1_ADDR	0x00016ed8
#define	PCM_DTMF2_ADDR	0x00019d10
#define	PCM_DTMF3_ADDR	0x0001cb4a
#define PCM_DTMF4_ADDR	0x0001f964
#define PCM_DTMF5_ADDR	0x000227a0
#define PCM_DTMF6_ADDR	0x000255da
#define PCM_DTMF7_ADDR 	0x00028410
#define PCM_DTMF8_ADDR	0x0002b25e
#define PCM_DTMF9_ADDR	0x0002e0ac
#define PCM_DTMFPSOUND_ADDR 0x00030ee4
#define PCM_DTMFSTART_ADDR	0x00033d24
#define PCM_Tock_ADDR	0x00036b62
#define PCM_GY_ADDR		0x000372e2
#define PCM_JIA_ADDR    0x0003a3ca
#define PCM_SHI_ADDR	0x0003bbfe
#define PCM_WSJ_ADDR	0x0003e04c
#define PCM_HAO_ADDR	0x0004138a
#define PCM_HJ_ADDR		0x000433fc
#define PCM_WB_ADDR 	0x000460fc
#define PCM_CHUANG_ADDR 0x000489e8
#define PCM_KSH_ADDR	0x0004ae3a
#define PCM_CHG_ADDR    0x0004db50
#define PCM_FANG_ADDR 	0x0005066c
#define PCM_BAI_ADDR    0x00052082
#define PCM_SHY_ADDR 	0x00053f0c

#define PCM_HANG_TEL_ADDR	0x00056c58
#define PCM_CONNECTED_WIFI_ADDR 0x0005ce10
#define PCM_CONNECTED_SERVER_ADDR 0x00063c76


#define PCM_END_ADDR    429536 //结束地址
#define PCM_PAUSE_ADDR 	PCM_END_ADDR


#define VOICE_PLAY_TIMES	4


#define VOICE_BUFF_MAX  13312

typedef enum DATAFLASH_INDEX{PCM_0,PCM_1,PCM_2,PCM_3,PCM_4,PCM_5,PCM_6,PCM_7,PCM_8,PCM_9,
	PCM_DTMF0,PCM_DTMF1,PCM_DTMF2,PCM_DTMF3,PCM_DTMF4,PCM_DTMF5,PCM_DTMF6,PCM_DTMF7,PCM_DTMF8,PCM_DTMF9,PCM_DTMFPSOUND,PCM_DTMFSTART,
	PCM_Tock,PCM_GY,PCM_JIA,PCM_SHI,PCM_WSJ,PCM_HAO,PCM_HJ,PCM_WB,PCM_CHUANG,PCM_KSH,PCM_CHG,PCM_FANG,PCM_BAI,PCM_SHY,
	PCM_HANG_TEL,PCM_CONNECTED_WIFI,PCM_CONNECTED_SERVER,PCM_END} dataFlashIndex; //结构表位置定义

#define PCM_PAUSE PCM_END

typedef struct {
	uint32_t addr;
	uint32_t len;	
}DataFlashFormat;


//uint16_t voice_play_state=0;
#define VOICE_PLAY_START 		0x0001
#define VOICE_PLAY_END	 		0x0002
#define VOICE_PLAY_NEXT_SEG 	0x0004
#define VOICE_PLAY_NEXT_TIMES	0x0008	//一个床位开始报号下一遍
#define VOICE_PLAY_NEXT_BED		0x0010	//开始播报下一个床位
#define VOICE_PLAY_NEXT_TURNS	0x0020	//开始播报下一轮
#define VOICE_PLAY_ROOM			0x0040	//播放房号
#define VOICE_PLAY_ONE_WORD		0x0080	//单独播放一个单词
#define VOICE_PLAY_READ_EEPROM_DATA	0x0100	//读E2 语音数据标志
#define VOICE_PLAY_HANG_TEL			0x0200	//语音播放挂好电话机


#ifndef _IN_VOICE_PLAY_
extern void dac_pin_config(void);
extern void voice_play_word(uint8_t index);
extern void load_voice_play_sequence(uint8_t call_type,uint8_t room,uint8_t bed);
extern void StopVoicePlay(void);
extern uint8_t MakeCallVoiceData(STIndicationData *pstIndicationData);
extern void MakeRingVoiceData(STIndicationData *pstIndicationData);

extern void StartVoicePlay(void);
extern void voice_play_treat(void);
extern void voice_continue_play_word(void);

extern const DataFlashFormat DataFlashTable[];


extern int8_t voice_buff[];


extern int16_t *pvoice_data;
extern int16_t value;
extern uint32_t voice_data_addr;
extern uint32_t voice_data_len;
extern uint8_t  cur_play_index;

extern uint16_t voice_seg_pause_time;
extern uint32_t voice_data_total_len;



extern volatile uint8_t		voice_play_sequence[];
extern volatile uint8_t		voice_play_seq_cnt;

extern volatile uint8_t 	voice_play_turns;
extern volatile uint8_t		voice_play_times;

extern volatile uint8_t 	played_times;
extern volatile uint8_t		played_turns;
extern volatile uint16_t	voice_times_delay_time;	//报号遍数间隔时间
extern volatile uint16_t	voice_beds_delay_time;	//报号床位间隔时间
extern volatile uint16_t	voice_turns_delay_time;	//报号轮数间隔时间



extern uint16_t voice_play_state;



#endif


#endif


