#include "config.h"
unsigned char code MyParameter[49]={INIT_FLAG,	//��ʼ����־
								  0x01,	//1-SelfSecAddr
								  253,	//2-SelfRoomAddr
								  0x01,	//3-SelfBedAddr
								  0xff,	//4-cEnable1
								  0xff,	//5-cEnable2
								  0x60,	//6-cPrio
								  0x0f,	//7-cRingTime
								  0x40,	//8-cTalkTime
								  0x80,	//9-cBroadTime
								  100,	//10-cListenDelay
								  0x04,	//11-cVolumStep
								  0x3f,	//12-cHDwnRingVol
								  0x3f,	//13-cHUpRingVol
								  0x3f,	//14-cCH0HDwnTalkedVol
								  0x3f,	//15-cCH1HDwnTalkedVol
								  0x3f,	//16-cCH0HUpTalkedVol
								  0x3f,	//17-cCH1HUpTalkedVol
								  0x3f,	//18-cHDwnBroadVol
								  0x3f,	//19-cHUpBroadVol
								  0x3f,	//20-cHDwnNoteVol
								  0x3f,	//21-cHUpNoteVol
								  0x3f,	//22-cHDwnMusicVol
								  0x3f,	//23-cHUpMusicVol
								  0x3f,	//24-cCH0HDwnTalkVol
								  0x3f,	//25-cCH1HDwnTalkVol
								  0x3f,	//26-cCH0HUpTalkVol
								  0x3f,	//27-cCH1HUpTalkVol
								  0x3f,	//28-cHDwnSelfRingVol
								  0x3f,	//29-cHUpSelfRingVol
								  0x7f,	//30-cMaxVol
								  0x00,	//31-cMinVol
								  0xff,	//32-cReserve1
								  0xff,	//33-cReserve2
								  0xff,	//34-cReserve3
								  0xff,	//35-cSerialNum1
								  0xff,	//36-cSerialNum2
								  0xff,	//37-cSerialNum3
								  0x20,	//38-�������ʼСʱ
								  0x00,	//39- �������ʼ����
								  0x06,	//40- ����ؽ���Сʱ
								  0x00,	//41- ����ؽ�������
								  0x20, //42-С������ʼСʱ
								  0x00, //43-С������ʼ����
								  0x06,	//44-С��������Сʱ
								  0x00,	//45-С������������
								  0x03,	//46-С����ֵ
								  0x02,	//47-cVersionHi
								  0x01	//48-cVersionLo
};





