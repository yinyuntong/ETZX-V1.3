#define _IN_PLAYER_

#include "config.h"




/*
0.mp3��0x00000000
1.mp3��0x000020ee
2.mp3��0x00003e49
3.mp3��0x00004bdb
4.mp3��0x00006cc9
5.mp3��0x00008db7
6.mp3��0x0000aea5
7.mp3��0x0000ce25
8.mp3��0x0000ee5c
9.mp3��0x00010c6e
����.mp3��0x00012e13
��.mp3��0x000172d2
ʮ.mp3��0x00017992
������.mp3��0x00019bed
��.mp3��0x0001c6db
����.mp3��0x0001e712
���.mp3��0x00020d00
��.mp3��0x00024995
��ʼ.mp3��0x00025aba
��.mp3��0x00029c35
��.mp3��0x0002b990
��Һ.mp3��0x0002d910
���Ժ򣬻�ʿ���Ͼ͵�.MP3:0x00031c2d
��ϵͳ�����ѵ���.MP3:0x000379d2	

*/


//����������ʼ��ַ����
uint32 code ulVoiceDataAddr[24]= {
0x00000000,	//0		��ʼ��ַ	     
0x000020ee,	// 1
0x00003e49,	// 2
0x00004bdb,	// 3
0x00006cc9,	// 4
0x00008db7,	// 5
0x0000aea5,	// 6
0x0000ce25,	//7 
0x0000ee5c,	//8
0x00010c6e,	//9
0x00012e13,	//����
0x000172d2,	//��
0x00017992,	//ʮ
0x00019bed,	//������
0x0001c6db, //��
0x0001e712,	//����
0x00020d00,	//���
0x00024995,	// ��
0x00025aba,	//��ʼ
0x00029c35,	//��
0x0002b990,	//��
0x0002d910,	//��Һ
0x00031c2d, //���Ժ򣬻�ʿ���Ͼ͵�
0x000379d2	//��ϵͳ�����ѵ���
};

//�������ݳ��ȶ���
uint32 code ulVoiceDataLen[24]= {
0x000020ee,	//0		      ����
0x00001d5b,	// 1
0x00000d92,	// 2
0x000020ee,	// 3
0x000020ee,	// 4
0x000020ee,	// 5
0x00001f80,	// 6
0x00002037,	//7 
0x00001e12,	//8
0x000021a5,	//9
0x000044bf,	//����
0x000006c0,	//��
0x0000225b,	//ʮ
0x00002aee,	//������
0x00000fb7,	//��
0x000025ee,	//����
0x00003c95,	//���
0x00001125,	// ��
0x0000417b,	//��ʼ
0x00001d5b,	//��
0x00001f80,	//��
0x0000431d,	//��Һ
0x00005da5, //���Ժ򣬻�ʿ���Ͼ͵�
0x00008437	//��ϵͳ�����ѵ���
};





/*void SetPlay(uint8 byPos)
{
	uint8 xdata i;
	SST25VFAddr = ulVoiceDataAddr[byPos][0];	//�����ε���ʼ��ַ
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
		if(SST25VFAddr >= ulVoiceDataAddr[byPos][1]) break; //�����εĽ�����ַ
	}
	Send_Data((uint8 * )(&ulVoiceDataAddr[byPos][1]),4);
	Send_Data((uint8 * )(&SST25VFAddr),4);
}
*/


void SetPlay(uint8 byPos)
{
	uint8 xdata i;
	int32 xdata VoiceDataLen;
	SST25VFAddr = ulVoiceDataAddr[byPos];	//�����ε���ʼ��ַ
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
		if(VoiceDataLen<=0) break; //�����εĽ�����ַ

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
		{	//û����Ҫ���ŵ�����	
			OSWait(K_TMO, 2);			
			continue;			
		}
		
		
		//��������Ҫ����
		stLocalControl.stVoicePlay.byVS1003State &= ~ISD_REPLAY;
		stLocalControl.stVoicePlay.byVS1003State |= ISD_PLAYING;	
		//����Ҫ���ŵ�����
		stLocalControl.stVoicePlay.byVoicePosition = 0x00;
		while(1)
		{
PlayRestart:
			byPos = stLocalControl.stVoicePlay.byVoiceData[stLocalControl.stVoicePlay.byVoicePosition];

			SST25VFAddr = ulVoiceDataAddr[byPos];	//�����ε���ʼ��ַ
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
				if(VoiceDataLen<=0) break; //�����εĽ�����ַ

			}
			
//			Mp3SoftReset();
//			VS1003FlushBuffer();

	
//PlayEnd:
			//���β������
			if(stLocalControl.stVoicePlay.byVS1003State & ISD_REPLAY) goto VoicePlayStart;
			if(!(stLocalControl.stVoicePlay.byVS1003State & ISD_PLAYING)) goto VoicePlayStart;
			
			stLocalControl.stVoicePlay.byVoicePosition = 
				(stLocalControl.stVoicePlay.byVoicePosition + 1) % stLocalControl.stVoicePlay.byVoiceSize;
			if(0x00 == stLocalControl.stVoicePlay.byVoicePosition)
			{	//����һ�������
				if(bPlayVoiceMaintain==1)
				{
					bPlayVoiceMaintain =0;
					stLocalControl.stVoicePlay.byVoicePosition = 0x00;
					stLocalControl.stVoicePlay.byVoiceTimes=0;

					stLocalControl.stBusDealFreq.bySndSecAddr = 0x00;
					stLocalControl.stBusDealFreq.byCmd = CMD_VOICE_MAINTAIN;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					stLocalControl.stVoicePlay.byVS1003State &= ~(ISD_REPLAY|ISD_PLAYING);	//���־	
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
					stLocalControl.stVoicePlay.byVS1003State &= ~(ISD_REPLAY|ISD_PLAYING);	//���־	
					goto VoicePlayStart;
				}				
				else
				{
					stLocalControl.stVoicePlay.byVoicePosition = 0x00;
					uiTimerOut=80;	//���2��
					for(uiTimerOut;uiTimerOut>0;uiTimerOut--)
					{
						OSWait(K_TMO,5); //25ms
						if(stLocalControl.stVoicePlay.byVS1003State & ISD_REPLAY)	goto VoicePlayStart;
					}
					stLocalControl.stVoicePlay.byVoiceTimes--;
					if(stLocalControl.stVoicePlay.byVoiceTimes==0)
					{// 4 �α������
						stLocalControl.stBusDealFreq.byCmd = CMD_STOP_VOICE_PLAY;
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
						stLocalControl.stVoicePlay.byVS1003State &= ~(ISD_REPLAY|ISD_PLAYING);	//���־
						goto VoicePlayStart;
						
					}
					else goto PlayRestart;
				}
			}
		}
	}
}




