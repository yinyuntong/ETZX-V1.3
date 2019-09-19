#include "main.h"
#include "wtvxxx.h"


//ISD1700命令定义
#define		CMD_STOP_PLAY	0xFE        //停止播放
#define		CMD_LOOP_PLAY   0xF2		//循环播放
#define     CMD_OPEN_PA     0xF1        //打开功放
#define     CMD_CLOSE_PA    0xF0        //关闭功放
#define     CMD_VOL_SET     0xE0        //音量调节 E0~EF  16级
#define     CMD_PLAY        0x00        //单次播放 00~7F  128段语音

extern unsigned char voml;     //音量暂存器
extern bool audio_irq;
bool audio_play=0; //正在放音
#define MAX_VOICE_ROWS 22

INT8U play_buf[30]; //播放音乐区
INT8U play_start=0,play_end=0;//播放声音开始与结束

//语音数据定义
const uint8 byVoiceDataAddr[23]= 
                            {
                               0x00,//语音0；     0
                               0x01,//语音1；     1
                               0x02,//语音2；     2
                               0x03,//语音3；     3
                               0x04,//语音4；     4
                               0x05,//语音5；     5
                               0x06,//语音6；     6
                               0x07,//语音7；     7
                               0x08,//语音8；     8
                               0x09,//语音9；     9
                               0x0a,//语音十；    10
                               0x0b,//语音百；    11
                               0x0c,//语音  呼叫；12
                               0x0d,//语音  输液；  13
                               0x0e,//语音  完毕；  14
                               0x0f,//语音  床；    15
                               0x10,//语音  号；    16
                               0x11,//语音  房；    17
                               0x12,//语音  卫生间；18
                               0x13,//语音  成功；  19
                               0x14, //语音  失败； 20
							   0x15, //语音  加；   21
							   0x16, //语音  嘀；   22
							 };

/*---------------------------------------------------------------------------
函数原型: void Delayms(uint ms)
参数说明: ms--需要延时的值
返 回 值: 无
函数功能：延时程序(对于18.432M晶振单指令周期延时1mS)
----------------------------------------------------------------------------*/
void Delayms(uint16 ms)	  
{
   uint16 i;
   while(ms--)
		for(i=300;i>0;i--);
}

void Delayus(uint16 us)	  
{
 while(us--){nop();nop();nop();};
}

void WTVxxxInit(unsigned char voml)
{		
	//复位语音芯片ISD17XX
	SetAUDRST(0);
	Delayms(5); 		
	SetAUDRST(1);
	Delayms(10);
}
void StopPlay(void)
{
	SPI_Cmd(DISABLE);
	Delayms(2);
	SetSCK(1);
  SetAUDCS(0);
	Delayms(2);
	SPITransferByte(CMD_CLOSE_PA);
	SetAUDCS(1);
	SPI_Cmd(ENABLE);
	play_end=play_start;
	audio_play=0;
}



void StartPlay(void)
{   
	if (audio_play) return;
	SPI_Cmd(DISABLE);
	Delayms(1);
	SetSCK(1);
  SetAUDCS(0);
	Delayms(1);
	SPITransferByte(CMD_OPEN_PA);
	SPITransferByte(CMD_VOL_SET+voml); 
	SPITransferByte(CMD_PLAY+play_buf[play_start]);
	play_start = (play_start+1)%30;
	//SPITransferByte(CMD_CLOSE_PA);
	SetAUDCS(1);
	SPI_Cmd(ENABLE);
	Delayms(1);
	audio_irq=0;
	audio_play=1;
}

void CheckPlay(void)
{
	//检查报号状态
	if (play_end!=play_start) StartPlay();
	else StopPlay();	
}
//播放声音
void PlaySound(INT8U pos)
{
	play_buf[play_end] = pos;
	if (play_start==play_end) StartPlay();
	play_end = (play_end+1)%30;
}


//dat 房号 dat1 床头号  dat2 呼叫类型
/*void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
{ 
	if((dat2==6)||(dat2==7))  //床头呼叫或输液完毕
	{
		if (dat==250) PlaySound(21);//加床
		dat = dat1;
		if (dat1>=100)   //
		{
			PlaySound(dat1/100);
			PlaySound(11);
			dat1 %= 100;
		}
		if (dat1>=10)
		{
			if (dat1>19||dat>100) PlaySound(dat1/10);
			PlaySound(10);
			dat1 %= 10;
		} else if(dat>100) PlaySound(0);
		if (dat1>0)	PlaySound(dat1);
		PlaySound(15);//床
		if (dat2==6) PlaySound(12);//呼叫
		else {
			PlaySound(13);
			PlaySound(14);//输液完毕
		}
	}
	else //卫生间呼叫
	{
		if (dat>=10) //
		{
			if (dat>19) PlaySound(dat/10);
			PlaySound(10);
			dat %= 10;
		}
		if (dat>0) PlaySound(dat);
		PlaySound(17); //房
		PlaySound(18);//卫生间
		PlaySound(12);//呼叫
	}
}*/


//dat 房号 dat1 床头号  dat2 呼叫类型

void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
{ 
	if((dat2==BED_CALL_TYPE) ||(dat2==BED_INFUSION_TYPE))//床头呼叫或输液完毕,都报"xx床呼叫"
	{
		if(dat == ADD_FJ) PlaySound(EXTRA_VOC_ADDR);	//加床

		PlaySound(BED_VOC_ADDR);
		PlaySound(EXTENSION_VOC_ADDR);
		PlaySound(NUMBER_VOC_ADDR);

	}
	else if(dat2==WC_CALL_TYPE)
	{
		PlaySound(BATHROOM_VOC_ADDR);
		PlaySound(EXTENSION_VOC_ADDR);
		PlaySound(NUMBER_VOC_ADDR);

		dat1 = dat;		//卫生间数据在房号	
	}
	else return;

	if(dat1>=100)
	{
		PlaySound((dat1/100)/*-1*/);
		PlaySound(HUNDRED_VOC_ADDR);
		if(dat1%100 )
		{
			PlaySound(AND_VOC_ADDR);
			dat1%=100;


		}
	}

	if(dat1>=20)
	{
		switch(dat1/10)
		{
			case 2:
				PlaySound(TWENTY_VOC_ADDR);
				break;
			case 3:
				PlaySound(THIRTY_VOC_ADDR);
				break;	
			case 4:
				PlaySound(FOURTY_VOC_ADDR);
				break;	
			case 5:
				PlaySound(FIFTY_VOC_ADDR);
				break;
			case 6:
				PlaySound(SIXTY_VOC_ADDR);
				break;	
			case 7:
				PlaySound(SEVENTY_VOC_ADDR);
				break;	
			case 8:
				PlaySound(EIGHTY_VOC_ADDR);
				break;	
			case 9:
				PlaySound(NINETY_VOC_ADDR);
				break;
			default:
				break;
		}

		dat1 %=10;
		if(dat1!=0)
		{
			PlaySound((dat1%10)/*-1*/);
		}
	}
	else //少于20
	{
	
		if(dat1!=0)	PlaySound(dat1/*-1*/);
	}

	
	PlaySound(CALLING_VOC_ADDR);

	
}





void playtest(INT8U start_seg,INT8U end_seg)
{
	INT8U i;
	for(i=start_seg;i<=end_seg;i++)
	{
		PlaySound(i);
	}
}

