unsigned char code ParameterData[39]={0x5a,	//��ʼ����־
									0x01,	//1-SelfSecAddr
									0x02,	//2-SelfRoomAddr
									0x03,	//3-SelfBedAddr:
									0xf7,	//4-cEnable1		//bEnSoundNote=0;   �ƽ�ת�Ʋ�������ʾ
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
									0x00,	//31-��Һ������
									0x00,	//32-��Һ������
									0x00,	//33-��Һ������
									0x00,	//34-��Һ������
									0x00,	//35-��Һ������
									0x00,	//36-��Һ������
									0x02,	//37-��ͷ�ֻ���ر�־	
											//bit0:  �����˳�Ժ��־  0:�ô�λ�в���  1:�����ѳ�Ժ
											//bit1:  ��������ʾ״̬��־  0:�����       1:���ؿ�
											//bit2:  �����Ƿ��ڹ�����ʱ״̬  0��δ������ʱ  1�����ڹ�����ʱ״̬
											//bit3:����ͷ�ֻ��Ƿ���������0:���������š���1:���ﱨ��(Ĭ�ϲ�����)
											//bit4:��ͷ�ֻ���ʾģʽ  0:������Ϣ��ʾģʽ��1��ɫ��ģʽ
											//bit5:���մ�ͷ���������ͷ���Ƿ���Ҫ�ж����ŷ���0:����Ҫ�жϣ�1:��Ҫ�ж�
									0x00	//CRC	
									};

