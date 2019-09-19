#define _IN_PLAYER_

#include "config.h"




/*
0.mp3：0x00000000
1.mp3：0x000020ee
2.mp3：0x00003e49
3.mp3：0x00004bdb
4.mp3：0x00006cc9
5.mp3：0x00008db7
6.mp3：0x0000aea5
7.mp3：0x0000ce25
8.mp3：0x0000ee5c
9.mp3：0x00010c6e
供氧.mp3：0x00012e13
加.mp3：0x000172d2
十.mp3：0x00017992
卫生间.mp3：0x00019bed
号.mp3：0x0001c6db
呼叫.mp3：0x0001e712
完毕.mp3：0x00020d00
床.mp3：0x00024995
开始.mp3：0x00025aba
房.mp3：0x00029c35
百.mp3：0x0002b990
输液.mp3：0x0002d910
请稍候，护士马上就到.MP3:0x00031c2d
本系统服务已到期.MP3:0x000379d2	

*/


//语音数据起始地址定义
uint32 code ulVoiceDataAddr[24]= {
0x00000000,	//0		起始地址	     
0x000020ee,	// 1
0x00003e49,	// 2
0x00004bdb,	// 3
0x00006cc9,	// 4
0x00008db7,	// 5
0x0000aea5,	// 6
0x0000ce25,	//7 
0x0000ee5c,	//8
0x00010c6e,	//9
0x00012e13,	//供氧
0x000172d2,	//加
0x00017992,	//十
0x00019bed,	//卫生间
0x0001c6db, //号
0x0001e712,	//呼叫
0x00020d00,	//完毕
0x00024995,	// 床
0x00025aba,	//开始
0x00029c35,	//房
0x0002b990,	//百
0x0002d910,	//输液
0x00031c2d, //请稍候，护士马上就到
0x000379d2	//本系统服务已到期
};

//语音数据长度定义
uint32 code ulVoiceDataLen[24]= {
0x000020ee,	//0		      长度
0x00001d5b,	// 1
0x00000d92,	// 2
0x000020ee,	// 3
0x000020ee,	// 4
0x000020ee,	// 5
0x00001f80,	// 6
0x00002037,	//7 
0x00001e12,	//8
0x000021a5,	//9
0x000044bf,	//供氧
0x000006c0,	//加
0x0000225b,	//十
0x00002aee,	//卫生间
0x00000fb7,	//号
0x000025ee,	//呼叫
0x00003c95,	//完毕
0x00001125,	// 床
0x0000417b,	//开始
0x00001d5b,	//房
0x00001f80,	//百
0x0000431d,	//输液
0x00005da5, //请稍候，护士马上就到
0x00008437	//本系统服务已到期
};





/*void SetPlay(uint8 byPos)
{
	uint8 xdata i;
	SST25VFAddr = ulVoiceDataAddr[byPos][0];	//此音段的起始地址
	Send_Data((uint8 * )(&SST25VFAddr),4);
		
	while(1)
	{
		Read_Cont(SST25VFAddr,32,FRAM_BUFF);
		while(!MP3_DREQ)
		{
			OSWait(K_TMO,1);	
		}
		Mp3SelectData();	  // Now SPI writes go to SDI port 
		for(i=0;i<32;i++)
		{
			SPI_WriteByte(FRAM_BUFF[i]);
		}
		Mp3DeselectData();
		
		SST25VFAddr +=32;
		if(SST25VFAddr >= ulVoiceDataAddr[byPos][1]) break; //此音段的结束地址
	}
	Send_Data((uint8 * )(&ulVoiceDataAddr[byPos][1]),4);
	Send_Data((uint8 * )(&SST25VFAddr),4);
}
*/


void SetPlay(uint8 byPos)
{
	uint8 xdata i;
	int32 xdata VoiceDataLen;
	SST25VFAddr = ulVoiceDataAddr[byPos];	//此音段的起始地址
	VoiceDataLen = ulVoiceDataLen[byPos];
	
	Send_Data((uint8 * )(&SST25VFAddr),4);
			
	while(1)
	{

		Read_Cont(SST25VFAddr,32,FRAM_BUFF);
		while(!MP3_DREQ);
//		{
//			OSWait(K_TMO,1);	
//		}
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
	//VS1003FlushBuffer();
	Send_Data((uint8 * )(&VoiceDataLen),4);	
	Send_Data((uint8 * )(&SST25VFAddr),4);
	Mp3SoftReset();
	
}


void PlayManager(void)
{
	static uint8 byPos;
	static uint16 uiTimerOut;	
	while(true)
	{
VoicePlayStart:
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
			byPos = stLocalControl.stVoicePlay.byVoiceData[stLocalControl.stVoicePlay.byVoicePosition];

			SST25VFAddr = ulVoiceDataAddr[byPos];	//此音段的起始地址
			VoiceDataLen = ulVoiceDataLen[byPos];					
			while(1)
			{

				SpiInit();
				Read_Cont(SST25VFAddr,32,FRAM_BUFF);
				Vs1003SpiInit();
				while(!MP3_DREQ);
		//		{
		//			OSWait(K_TMO,1);	
		//		}
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
				if(bPlayVoiceMaintain==1)
				{
					bPlayVoiceMaintain =0;
					stLocalControl.stVoicePlay.byVoicePosition = 0x00;
					stLocalControl.stVoicePlay.byVoiceTimes=0;

					stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
					stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_MAINTAIN;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					stLocalControl.stVoicePlay.byVS1003State &= ~(ISD_REPLAY|ISD_PLAYING);	//清标志	
					goto VoicePlayStart;
				}

				else if(bPlayVoiceWait==1)
				{
					bPlayVoiceWait =0;
					stLocalControl.stVoicePlay.byVoicePosition = 0x00;
					stLocalControl.stVoicePlay.byVoiceTimes=0;

					stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
					stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_WAITE;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					stLocalControl.stVoicePlay.byVS1003State &= ~(ISD_REPLAY|ISD_PLAYING);	//清标志	
					goto VoicePlayStart;
				}				
				else
				{
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
}




