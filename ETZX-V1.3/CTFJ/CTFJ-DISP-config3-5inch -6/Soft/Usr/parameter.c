unsigned char code ParameterData[22]={0x5b,	//初始化标志
									0x01,	//1-SelfSecAddr
									0x02,	//2-SelfRoomAddr
									0x03,	//3-SelfBedAddr:
									0xff,	//4-cSerialNum1
									0xff,	//5-cSerialNum2
									0xff,	//6-cSerialNum3
									0x04,	//7-cVersionHi
									0x06,	//8-cVersionLo
									0x00,	//9-stSupplyOxTotalTime.uiHour
									0x00,
									0x00,	//11-stSupplyOxTotalTime.byMin
									0x00,	//12-输液报警器
									0x00,	//13-输液报警器
									0x00,	//14-输液报警器
									0x00,	//15-输液报警器
									0x00,	//16-输液报警器
									0x00,	//17-输液报警器
									0xe1,	//18-RS485波特率的高位
									0x00,	//19-RS485波特率的低位默认波特率57600
									0x00,	//20--   0x00:床头分机接收RS485信息时不判断区号、房号
												 //0x01:需判断
									0x46,	//21--byCRC 数据校验
									};

