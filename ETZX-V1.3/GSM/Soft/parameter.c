unsigned char code ParameterData[26]={0x5a,	//初始化标志
									0x01,	//1-SelfSecAddr
									252,	//2-SelfRoomAddr
									0x01,	//3-SelfBedAddr:
									0xff,	//4-cSerialNum1
									0xff,	//5-cSerialNum2
									0xff,	//6-cSerialNum3
									0x10,	//7-cVersionHi
									0x0a,	//8-cVersionLo
									0x00,	//9-  电话号码第1个数字(从前住后）
									0x00,	//10-电话号码第2个数字
									0x00,	//11-电话号码第3个数字
									0x00,	//12-电话号码第4个数字
									0x00,	//13-电话号码第5个数字
									0x00,	//14-电话号码第6个数字
									0x00,	//15-电话号码第7个数字
									0x00,	//16-电话号码第8个数字
									0x00,	//17-电话号码第9个数字
									0x00,	//18-电话号码第10个数字
									0x00,	//19-电话号码第11个数字
									0x0f,	//20-电话号码结束符
									0x00,	//21-电话号码有效长度
									0x01,	//22-功能相关标志	
											//bit0:  代表移动电话功能标志  0:关闭  1:打开
											//bit1:  
									0x0e,	//23-cRingTime
									0x40,	//24-cTalkTime
									0x80	//25-cBroadTime											
									};

