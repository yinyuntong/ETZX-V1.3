
#ifndef PLAYER_H
#define PLAYER_H


/*
0	0.mp3��0x00000000
1	1.mp3��0x000028c9
2	2.mp3��0x00004f6e
3	3.mp3��0x00007613
4	4.mp3��0x00009e25
5	5.mp3��0x0000c580
6	6.mp3��0x0000ed92
7	7.mp3��0x0001115b
8	8.mp3��0x0001396d
9	9.mp3��0x00015ded
10	����.mp3��0x00018548
11  ��.mp3
12	ʮ.mp3��0x0001f61c
13	������.mp3��0x00021e2e
14	��.mp3��0x00024a89
15	����.mp3��0x00025dd2
16	���.mp3��0x000288c0
17	��.mp3��0x0002f994
18	��ʼ.mp3��0x00030e4b
19	��.mp3��0x00037f1f
20	��.mp3��0x00039c7a
21	��Һ.mp3��0x0003b131
22  ���Ժ�.mp3
23  ϵͳ����.mp3
*/


#define ZERO_VOICE	0
#define ONE_VOICE	1
#define TOW_VOICE	2
#define THREE_VOICE	3
#define FOUR_VOICE	4
#define FIVE_VOICE	5
#define SIX_VOICE	6
#define SEVEN_VOICE	7
#define EIGHT_VOICE	8
#define NINE_VOICE	9
#define OX_SUPPLY_VOICE	10
#define ADD_VOICE	11
#define TEN_VOICE	12
#define TOILET_VOICE	13
#define NUM_VOICE	14
#define CALL_VOICE	15
#define END_VOICE	16
#define BED_VOICE	17
#define START_VOICE	18
#define ROOM_VOICE	19
#define HUNDRED_VOICE 20
#define INFUSION_VOICE 21
#define WAIT_VOICE   22
#define MAINTAIN_VOICE 23



//ISD1700״̬������
#define		ISD_PLAYING		0x01								//����оƬ���ڲ���
#define 	ISD_REPLAY		0x02								//����оƬ�ز���
#define		ISD_TIME_OVER	0x04								//һ�ֲ���ʱ�䵽��

#ifndef _IN_PLAYER_
extern uint32 code ulVoiceDataAddr[19][2];
extern void SetPlay(uint8 byPos);
extern void PlayManager(void);
#endif

#endif