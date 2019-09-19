unsigned char code ParameterData[39]={0x5a,	//初始化标志
									0x01,	//1-SelfSecAddr
									0x02,	//2-SelfRoomAddr
									0x03,	//3-SelfBedAddr:
									0xf7,	//4-cEnable1		//bEnSoundNote=0;   移叫转移不语音提示
									0xff,	//5-cEnable2
									0x60,	//6-cPrio
									0x0e,	//7-cRingTime
									0x40,	//8-cTalkTime
									0x80,	//9-cBroadTime
									0x03,	//10-cListenDelay
									0x04,	//11-cVolumStep
									0xd0,	//12-cRingVol
									0xd0,	//13-cCH0TalkedVol
									0xd0,	//14-cCH1TalkedVol
									0xd0,	//15-cBroadVol
									0xd0,	//16-cNoteVol
									0xd0,	//17-cMusicVol
									0xd0,	//18-cCH0TalkVol
									0xd0,	//19-cCH1TalkVol
									0xd0,	//20-cSelfRingVol
									0xf9,	//21-cMaxVol
									0x0a,	//22-cMinVol
									0xff,	//23-cReserve1
									0xff,	//24-cReserve2
									0xff,	//25-cReserve3
									0xff,	//26-cSerialNum1
									0xff,	//27-cSerialNum2
									0xff,	//28-cSerialNum3
									0x10,	//29-cVersionHi
									0x0a,	//30-cVersionLo
									0x00,	//31-输液报警器
									0x00,	//32-输液报警器
									0x00,	//33-输液报警器
									0x00,	//34-输液报警器
									0x00,	//35-输液报警器
									0x00,	//36-输液报警器
									0x02,	//37-床头分机相关标志	
											//bit0:  代表病人出院标志  0:该床位有病人  1:病人已出院
											//bit1:  代表背光显示状态标志  0:背光关       1:背关开
											//bit2:  代表是否处于供氧计时状态  0：未供氧计时  1：处于供氧计时状态
											//bit3:代表床头分机是否语音报号0:不语音报号　　1:语语报号(默认不报号)
											//bit4:床头分机显示模式  0:供氧信息显示模式，1，色块模式
											//bit5:接收床头卡、清除床头卡是否需要判断区号房号0:不需要判断，1:需要判断
									0x00	//CRC	
									};

