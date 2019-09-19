
#ifndef PLAYER_H
#define PLAYER_H






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
#define TEN_VOICE	10
#define ELEVEN_VOICE	11
#define TWELVE_VOICE	12
#define THIRTEEN_VOICE	13
#define FOURTEEN_VOICE	14
#define FIFTEEN_VOICE	15
#define SIXTEEN_VOICE	16
#define SEVENTEEN_VOICE	17
#define EIGHTEEN_VOICE	18
#define NINETEEN_VOICE	19
#define TWENTY_VOICE	20
#define THIRTY_VOICE	21
#define FOURTY_VOICE	22
#define FIFTY_VOICE		23
#define SIXTY_VOICE		24
#define SEVENTY_VOICE	25
#define EIGHTY_VOICE	26
#define NINETY_VOICE	27
#define THOUSAND_VOICE	28
#define MONTH_VOICE		29
#define	DAY_VOICE		30
#define HOUR_VOICE		31
#define MINUTE_VOICE	32
#define SECOND_VOICE	33

#define END_VOICE		34
#define PAST_VOICE		35

#define BATH_VOICE		36
#define BED_VOICE		37
#define CALL_VOICE		38
#define INFUSION_VOICE	39
#define NUMBER_VOICE	40
#define EXTRA_VOICE		41
#define EXTENSION_VOICE	42

#define YEAR_VOICE		43
#define ROOM_VOICE		44
#define HUNDRED_VOICE	45




//ISD1700◊¥Ã¨øÿ÷∆◊÷
#define		ISD_PLAYING		0x01								//”Ô“Ù–æ∆¨’˝‘⁄≤•“Ù
#define 	ISD_REPLAY		0x02								//”Ô“Ù–æ∆¨÷ÿ≤•“Ù
#define		ISD_TIME_OVER	0x04								//“ª¬÷≤•“Ù ±º‰µΩ¡À

#ifndef _IN_PLAYER_
extern uint32 code ulVoiceDataAddr[19][2];
extern void PlayManager(void);
#endif

#endif
