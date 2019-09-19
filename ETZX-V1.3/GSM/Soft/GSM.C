#define _IN_GSM_
#include "config.h"

void AffirmUartCom(void)
{
	UartSendString("AT");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行
}


void LoopDispOff(void)	//回显关闭
{
	UartSendString("ATE0");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}

void LoopDispOn(void)	//回显打开
{
	UartSendString("ATE1");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}


void SIMStateQuery(void)	//SIM状态查询
{
	UartSendString("AT+CPIN?");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行
}


void SignalIntensionQuery(void)	//网络信号强度查询
{
	UartSendString("AT+CSQ");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}


void NetOPSQuery(void)		//网络运营商注册查询
{
	UartSendString("AT+COPS?");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	

}


void GSMModuleInfoQuery(void)	//模块信息查询
{
	UartSendString("ATI");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	

}

void GSMUartAutoBrtSet(void)	//自适应波特率设置
{
	UartSendString("AT+IPR=0");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}



void GSMUartBrtSet(uint16 UartBrt)	//设置GSM模块波特率UartBrt=0:为自适应波特率
{										//最高为57600	//再高单片机串口收发数据不正确
	uint8 BaudRate[5];
	BaudRate[0]	= UartBrt/10000+0x30;
	UartBrt		= UartBrt%10000;
	BaudRate[1]	= UartBrt/1000+0x30;
	UartBrt		= UartBrt%1000;
	BaudRate[2]	= UartBrt/100+0x30;
	UartBrt		= UartBrt%100;
	BaudRate[3]	= UartBrt/10+0x30;
	UartBrt		= UartBrt%10;
	BaudRate[4]	= UartBrt+0x30;

	
	UartSendString("AT+IPR=");
	Send_Data(BaudRate,5);
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行

}


void GSMPathQuery(void)
{
	UartSendString("AT^SWSPATH?");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}

void EchoRestrainQuery(void)	//回声抑制功能查询
{
	UartSendString("AT^ECHO?");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行		
}


void EchoRestrainSet(uint8 Parameter)	//回声抑制功能设置
{
	UartSendString("AT^ECHO=");
	UartSendByte(0x30+Parameter);	
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行		
}




void GSMSWPathSet(uint8 path)
{
	if(path>=2) return;

	UartSendString("AT^SWSPATH=");
	UartSendByte(0x30+path);	
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行		
}


void GSMSPKVoiceSet(uint8 Voice)
{
	
	UartSendString("AT+CLVL=");
	UartSendByte(0x30+Voice);	//设置扬声器音量
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}


void GSMMICGainSet(uint8 Gain)
{
	UartSendString("AT+CMIC=");
	UartSendByte(0x30+Gain);	//设置麦克风增益
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}



void GSMCall(void)	//模块主叫
{
	uint8 TelPhoneNum[TEL_NUM_LEN+1];
	uint8 i;
	memcpy(TelPhoneNum,stLocalControl.stEepromCfgData.byTelNum,stLocalControl.stEepromCfgData.byTelNumValidLen);
	for(i=0;i<stLocalControl.stEepromCfgData.byTelNumValidLen;i++) TelPhoneNum[i]=TelPhoneNum[i]+0x30;
	
	UartSendString("ATD");
	for(i=0;i<stLocalControl.stEepromCfgData.byTelNumValidLen;i++)
	{
		 UartSendByte(TelPhoneNum[i]);
	}
//	Send_Data(TelPhoneNum,TEL_NUM_LEN);	//发送电话号码
	UartSendByte(';');	
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行	
}

void GSMHandUp(void)	//GSM模块主动挂断
{
	UartSendString("AT+CHUP");
	UartSendByte(0x0d);		//回车
	UartSendByte(0x0a);		//换行		
}


void GSMInit(void)
{
	GSM_PWR_SW =1;
	GSM_ON_OFF =1;	
	Delayms(500);		//上电时间最小值250ms

	GSM_ON_OFF =0;
	Delayms(750);		//复位时间 750ms
	GSM_ON_OFF =1;
}


void GSMRestar(void)	//GSM模块重启
{
	GSM_RST =0;
	Delayms(50);
	GSM_RST =1;
}


uint8 GSMCallTreat(void)
{
	uint16 i;
	if(((stLocalControl.stEepromCfgData.byTelFuncState&0x01)==0)||(stLocalControl.stEepromCfgData.byTelNumValidLen==0)) return 0;
	RePeat=0;
RE_GSM_CALL:	
	while(RePeat<3)
	{
		GSMCall();
		i=5000; 	//延时5S
		while(!bUartRecFinished)
		{
			Delayms(1);
			if(--i==0)	break;
		}

		if(bUartRecFinished)
		{
			bUartRecFinished=0;
			if(UartRecTreat("^ORIG: 1,0\r\n\r\nOK",(uint8)strlen("^ORIG: 1,0\r\n\r\nOK")))
			{//收到第一步
			
				i=2000; //延时2S
				while(!bUartRecFinished)
				{
					Delayms(1);
					if(--i==0)	break;
				}
				if(bUartRecFinished)
				{
					bUartRecFinished=0; 
					if(UartRecTreat("^CONF: 1",(uint8)strlen("^CONF: 1")))
					{//收到第二步数据^CONF: 1
					
						GSM_STATE_LED = LED_ON;
						//break;
						return 1;
					}

				}
			}
		}
//		else 
//		{
//未收到任何数据
			Delayms(2000);
			RePeat++;
			goto RE_GSM_CALL;
//		}
	}
	return 0;
}


uint8 GSMHandUpTreat(void)
{
	uint16 i;
	if(((stLocalControl.stEepromCfgData.byTelFuncState&0x01)==0)||(stLocalControl.stEepromCfgData.byTelNumValidLen==0)) return 0;	
	RePeat=0;
RE_GSM_HUNDUP:	
	while(RePeat<3)
	{
		GSMHandUp();
		i=2000; 	//延时2S
		while(!bUartRecFinished)
		{
			Delayms(1);
			if(--i==0)	break;
		}

		if(bUartRecFinished)
		{
			bUartRecFinished=0;
			if(UartRecTreat("OK",(uint8)strlen("OK")))
			{//收到第一步
				//GSM_STATE_LED = LED_OFF;
				//break;
				return 1;
				
			}
		}
		Delayms(2000);
		RePeat++;
		goto RE_GSM_HUNDUP;
	}
	return 0;

}



























