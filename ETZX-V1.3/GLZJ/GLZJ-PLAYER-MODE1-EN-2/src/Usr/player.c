#define _IN_PLAYER_

#include "config.h"




/*
00-ero.mp3：0x00000000
01-one.mp3：0x000019b6
02-two.mp3：0x00002db6
03-three.mp3：0x00004324
04-four.mp3：0x000059fe
05-five.mp3：0x000070da
06-six.mp3：0x00008700
07-seven.mp3：0x00009bb8
08-eight.mp3：0x0000b126
09-nine.mp3：0x0000c526
10-ten.mp3：0x0000db4c
11-eleven.mp3：0x0000ee96
12-twelve.mp3：0x000104bc
13-thirteen.mp3：0x00011ae2
14-fourteen.mp3：0x0001349a
15-fifteen.mp3：0x00014fc0
16-sixteen.mp3：0x00016ae6
17-seventeen.mp3：0x000188e6
18-eihgteen.mp3：0x0001a40c
19-nineteen.mp3：0x0001bc56
20-twenty.mp3：0x0001d4a0
21-thirty.mp3：0x0001ea0e
22-forty.mp3：0x0001ff7c
23-fifty.mp3：0x000214ea
24-sixty.mp3：0x0002277c
25-seventy.mp3：0x00023e58
26-eighty.mp3：0x000255ea
27-ninety.mp3：0x00026934
28-thousand.mp3：0x00028010
29-month.mp3：0x0002985a
30-day.mp3：0x0002a812
31-hour.mp3：0x0002bd80
32-minute.mp3：0x0002d0ca
33-second.mp3：0x0002e2a6
34-to.mp3：0x0002f8cc
35-past.mp3：0x00030b5e
36-bath.mp3：0x000320cc
37-bed.mp3：0x000332a8
38-call.mp3：0x00034484
39-infusion.mp3：0x00035884
40-number.mp3：0x000373aa
41-extra.mp3：0x000389d0
42-extension.mp3：0x00039f3e
43-year.mp3：0x0003bc88
44-room.mp3：0x0003cdae
45-hundred.mp3：0x0003e0f8

*/


//语音数据起始地址定义
uint32 code ulVoiceDataAddr[46]= {
0x00000000,	//0---zero		起始地址	   0    
0x000019b6,	//1--one
0x00002db6,	// 2--two
0x00004324,	// 3----three
0x000059fe,	//4---four
0x000070da,	// 5---five
0x00008700,	//6----six
0x00009bb8,	//7----seven
0x0000b126,	//8----eight
0x0000c526,	//9-----nine
0x0000db4c,	//10---ten
0x0000ee96,	//11
0x000104bc,	//12
0x00011ae2,	//13
0x0001349a, //14
0x00014fc0,	//15
0x00016ae6,	//16
0x000188e6,	// 17
0x0001a40c,	//18
0x0001bc56,	//19
0x0001d4a0,	//20
0x0001ea0e,	//21-----thirty
0x0001ff7c,	//22-----forty
0x000214ea,	//23-----fifty
0x0002277c,	//24-----sixty
0x00023e58,	//25-----seventy
0x000255ea,	//26-----eight
0x00026934,	//27-----ninety
0x00028010,	//28-----thousand
0x0002985a,	//29-------month
0x0002a812,	//30--------day
0x0002bd80,	//31-------hour
0x0002d0ca,	//32-------minute
0x0002e2a6,	//33---second


0x0002f8cc,	//34------end
0x0003093a,	//35------past
0x00031ea8,	//36------bath

0x00033084,	//37-------bed
0x00034260,	//38-------call
0x00035660,	//39-------infusion

0x00037186,	//40-------number
0x000387ac,	//41-------extra
0x00039d1a,	//42-------extension

0x0003ba64,	//43-------year
0x0003cb8a,	//44-------room
0x0003ded4,	//45-------hundred
};

//语音数据长度定义
uint32 code ulVoiceDataLen[46]= {
6582,	//zero		      长度
5120,	// one
5486,	// two
5850,	// three
5852,	//four
5670,	//five
5304,	//six
5486,	//seven
5120,	//eight
5670,	//nine
4938,	//ten
5670,	//eleven
5670,	//twelve
6584,	//thirteen
6950,	//fourteen
6950,	//fifteen
7680,	//sixteen
6950,	// seventeen
6218,	//eighteen
6218,	//nineteen
5486,	//twenty
5486,	//thirty
5486,	//fourty
4754,	//fifty
5852,	//sixty
6034,	//seventy
4938,	//eighty
5852,	//ninty
6218,	//thousand
4024,	//month
5486,	//day
4938,	//hour
4572,	//minute
5670,	//second

4206,	//end
5486,	//past
4572,	//bath
4572,	//bed
5120,	//call
6950,	//infusion
5670,	//number
5486,	//extra
7498,	//extension

4390,	//year
4938,	//room
5120,	//hundred
};






void PlayManager(void)
{
	static uint8 byPos;
	static uint16 uiTimerOut;
	
	SpiInit();
	
	Vs1003SpiInit();
	while(!MP3_DREQ);	

	
	while(true)
	{

VoicePlayStart:


	
		//复位看门狗
		WDT_CONTR = 0x3e; 	//2.5s@20M		
		if(0x00 == (stLocalControl.stVoicePlay.byVS1003State&ISD_REPLAY))
		{	//没有需要播放的语音	
			OSWait(K_TMO, 2);			
			continue;			
		}
		
		
		//有语音需要播放
		stLocalControl.stVoicePlay.byVS1003State &= ~ISD_REPLAY;
		stLocalControl.stVoicePlay.byVS1003State |= ISD_PLAYING;	
		//有需要播放的语音
		stLocalControl.stVoicePlay.byVoicePosition = 0x00;
		while(1)
		{
PlayRestart:
			//复位看门狗
			WDT_CONTR = 0x3e; 	//2.5s@20M		
			byPos = stLocalControl.stVoicePlay.byVoiceData[stLocalControl.stVoicePlay.byVoicePosition];

			SST25VFAddr = ulVoiceDataAddr[byPos];	//此音段的起始地址
			VoiceDataLen = ulVoiceDataLen[byPos];					
			while(1)
			{
				//复位看门狗
				WDT_CONTR = 0x3e;	//2.5s@20M	

				Read_Cont(SST25VFAddr,32,FRAM_BUFF);
				while(!MP3_DREQ);

				Mp3SelectData();	  // Now SPI writes go to SDI port 
				for(i=0;i<32;i++)
				{
					SPI_WriteByte(FRAM_BUFF[i]);
				}
				Mp3DeselectData();
				
				SST25VFAddr +=32;
				VoiceDataLen -=32;
				if(VoiceDataLen<=0) break; //此音段的结束地址

			}			
//			Mp3SoftReset();
//			VS1003FlushBuffer();

	
//PlayEnd:
			//本次播音完成
			if(stLocalControl.stVoicePlay.byVS1003State & ISD_REPLAY) goto VoicePlayStart;
			if(!(stLocalControl.stVoicePlay.byVS1003State & ISD_PLAYING)) goto VoicePlayStart;
			
			stLocalControl.stVoicePlay.byVoicePosition = 
				(stLocalControl.stVoicePlay.byVoicePosition + 1) % stLocalControl.stVoicePlay.byVoiceSize;
			if(0x00 == stLocalControl.stVoicePlay.byVoicePosition)
			{	//报号一次完成了
				stLocalControl.stVoicePlay.byVoicePosition = 0x00;
				uiTimerOut=80;	//间隔2秒
				for(uiTimerOut;uiTimerOut>0;uiTimerOut--)
				{
					OSWait(K_TMO,5); //25ms
					if(stLocalControl.stVoicePlay.byVS1003State & ISD_REPLAY)	goto VoicePlayStart;
				}
				stLocalControl.stVoicePlay.byVoiceTimes--;
				if(stLocalControl.stVoicePlay.byVoiceTimes==0)
				{// 4 次报号完成
					stLocalControl.stBusDealFreq.byCmd = CMD_STOP_VOICE_PLAY;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					stLocalControl.stVoicePlay.byVS1003State &= ~(ISD_REPLAY|ISD_PLAYING);	//清标志
					goto VoicePlayStart;
					
				}
				else goto PlayRestart;

			}
		}
	}
}




