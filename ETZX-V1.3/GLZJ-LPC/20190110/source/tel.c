
#define _IN_TEL_

#include"custom.h"


uint16_t TelState=0;								//电话处理相关标志
/*#define  bTelRing			(1<<1)				//电话机振铃状态
#define  bTelRingOut		(1<<2)				//电话振铃输出高电平
#define  bTelTlpEnChk		(1<<3)				//电话机TLP允许检测
#define  bTelHungDowned		(1<<4)				//电话机挂机动作
#define  bTelPickUped		(1<<5)				//电话机摘机动作
#define  bOriTelTlp			(1<<6)				//原tlp状态
#define  bCID				(1<<7)				//来电显示状态
#define  bCID_CMD			(1<<8)				//发送CID命令


#define SetTelState(x) (TelState|=(x))			//置位操作
#define ClrTelState( x) (TelState&=~(x))			//清零操作
#define GetTelState(x) (TelState&(x))			//获取位状态*/


uint16_t tel_ring_time=0;			//电话振铃时音
uint16_t tel_tlp_dis_chk=0;			//禁止检测时间
uint16_t tel_tlp_chk_delay=0;		//电话检测延时时间
uint16_t CID_marks_time =0;			//标志信号延时时间
uint16_t CID_txe_delay=0;			//发送允许延时，时间到发送禁止
uint8_t cid_data[30];


uint8_t		phone_key_buf[8];


uint16_t    tel_pickup_time=0;


void AM79R70_active(void);
void AM79R70_standby(void);
void AM79R70_ringing(void);
void AM79R70_pincfg(void);



void init_phone_key_buf(void)
{
	uint8_t i;
	for(i=0;i<8;i++) phone_key_buf[i]=0x00;
}


void mt8870_received_treat(void)
{
	uint16_t i;
	
	if((GetState(bHostTalk)==0) && (GetState(bHostTalk1)==0) )
	{//主机不在对讲状态
		if((mt8870_received>=1)&&(mt8870_received<=10))   //0~9
		{
			if(mt8870_received == 10) mt8870_received = 0;
		
			phone_key_buf[3] = phone_key_buf[2];
			phone_key_buf[2] = phone_key_buf[1];
			phone_key_buf[1] = phone_key_buf[0];
			phone_key_buf[0] = mt8870_received; 
	
	
		}	
		else if(mt8870_received == 11)	 //"	*	"
		{
			i = phone_key_buf[2]*100;
			i += phone_key_buf[1]*10;
			i += phone_key_buf[0];
//			uart2_send_byte(i);
			if(i==255)
			{//电话广播
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr= stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_BROADCAST3;		//全区广播
				stBusFreq.byRecSecAddr = 0xff;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = 0xff;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));

				tel_pickup_time =0;
			}
	
			else if(i==0)
			{
			
			}
			else if(i<=200)
			{//床头分机
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr= stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_COMM_CALL;
				stBusFreq.byRecSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.byRecRoomAddr = 0xff;
				stBusFreq.byRecBedAddr = i;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));

				tel_pickup_time =0;
				
			}
			else if(i<240)
			{//医护分机
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr= stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_COMM_CALL;
				stBusFreq.byRecSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.byRecRoomAddr = YHFJ_ADDR2;
				stBusFreq.byRecBedAddr = i-200;
				Bus0OutputData(&(stBusFreq.bySndSecAddr));	

				tel_pickup_time =0;
			}
			init_phone_key_buf();
		}
		else if(mt8870_received == 12)	//"#"
		{
			init_phone_key_buf();
		}
	}

}



void start_tel_ring(void)
{
	SetTelState(bTelRing);
	SetTelState(bTelRingOut);
	tel_ctrl(AC36V);
	tel_ring_time = 0;

	SetTelState(bCID);
	
	phone_ring_ctrl(1);
	SetTelState(bPhoneRingState);

	AM79R70_ringing();
}


void stop_tel_ring(void)
{
	ClrTelState(bTelRing);
	ClrTelState(bTelRingOut);
	tel_ctrl(DC60V);
	tel_ring_time = 0;

	phone_ring_ctrl(0);
	ClrTelState(bPhoneRingState);
}


/*********************************************************************************************
*
*
*
*********************************************************************************************/
void AM79R70_pincfg(void)
{

  	LPC_GPIO1->FIODIR |= (AM79R70_c1_pin| AM79R70_c2_pin |AM79R70_c3_pin);


	LPC_PINCON->PINMODE3 |= (0X03<<4);					//使能p1.18下拉

	LPC_PINCON->PINMODE3 |= (0X03<<6);					//使能p1.19下拉	

	LPC_PINCON->PINMODE3 |= (0X03<<12);					//使能p1.22下拉

	AM79R70_standby();
}


//振铃模式
void AM79R70_ringing(void)
{
	AM79R70_c3_ctrl(0);
	AM79R70_c2_ctrl(0);
	AM79R70_c1_ctrl(1);	

}



//待机模式
void AM79R70_standby(void)
{
	AM79R70_c3_ctrl(1);
	AM79R70_c2_ctrl(0);
	AM79R70_c1_ctrl(1);	
}


//通话模式
void AM79R70_active(void)
{
	AM79R70_c3_ctrl(0);
	AM79R70_c2_ctrl(1);
	AM79R70_c1_ctrl(0);	

}


void init_msm7512(void)
{
	//msm　来电显示芯片发送禁止
	LPC_PINCON->PINSEL0 &= (~(0x03 << 8)); 			// P0.4   =1:禁止发送
	LPC_GPIO0->FIODIR    |= msm_txe_pin;
	msm_txe_ctrl(1);

}



void init_mt8870(void)
{
	LPC_PINCON->PINSEL1 &= (~(0x03 << 6)); 			    // P0.19  mtt_d1  
	LPC_GPIO0->FIODIR	&= ~mt8870_dq1;


	LPC_PINCON->PINSEL1 &= (~(0x03 << 8)); 			    // P0.20  mtt_d2
	LPC_GPIO0->FIODIR	&= ~mt8870_dq2;

	LPC_PINCON->PINSEL1 &= (~(0x03 << 10)); 			    // P0.21  mtt_d3
	LPC_GPIO0->FIODIR	&= ~mt8870_dq3;


	LPC_PINCON->PINSEL1 &= (~(0x03 << 12)); 			    // P0.22  mtt_d4
	LPC_GPIO0->FIODIR	&= ~mt8870_dq4;

}


//奇校验: 奇数个1
uint8_t get_odd_parity(uint8_t ori_data)
{
	uint8_t i;
	uint8_t number=0;
	uint8_t data_temp;

	ori_data &= 0x7f;		//将最高位清0
	data_temp = ori_data;
	
	for(i=0;i<7;i++)
	{
		if(ori_data & 0x01) number++;
		ori_data >>= 1;	
	}

	if(number%2 == 0)	//原数据有偶数个1，需在最高位置1，形成奇数个1
	{
		data_temp |= 0x80;
	}
	return(data_temp);
}


uint8_t get_complement(uint8_t ori_data)
{
/*	if(ori_data & 0x80) //负数
	{
		ori_data ^= 0x7f;		//反码
		ori_data ++;			//补码
	}*/

	ori_data ^= 0xff;		//反码
	ori_data ++;			//补码

	return ori_data;
}


uint8_t get_checksum(uint8_t *p,uint8_t len)
{
	uint8_t sum=0;
	//while(*p !=NULL)
	for(;len>0;len--)
	{
		sum += *p;
		p++;
	}
	return sum;
}





uint8_t make_msm7512_data(uint8_t room,uint8_t bed)
{
	uint8_t check;

	uint8_t i,j,k;


	for(i=0;i<30;i++) cid_data[i] = 0x00;


	if(room==0 && bed==0)	return 0;	


	cid_data[0] = 0x04;  	//消息头－消息类型


//以下为消息中的日期、时间数据
	cid_data[2] =	(stTime.byMonth/0x10)+0x30;
	cid_data[3] = 	(stTime.byMonth%0x10)+0x30;

	cid_data[4] =	(stTime.byDay/0x10)+0x30;
	cid_data[5] = 	(stTime.byDay%0x10)+0x30;

	cid_data[6] = 	(stTime.byHour/0x10)+0x30;
	cid_data[7] = 	(stTime.byHour%0x10)+0x30;

	cid_data[8] = 	(stTime.byMinute/0x10)+0x30;
	cid_data[9] = 	(stTime.byMinute%0x10)+0x30;


	if(bed==0)
	{//卫生间分机呼叫
		i = room/100;			//百位
		j = (room%100)/10;		//十位
		k = room%10;			//个位		
	}
	else
	{
		i = bed/100;			//百位
		j = (bed%100)/10;		//十位
		k = bed%10;			//个位	
	}
	if(i != 0) 
	{
		cid_data[1] = 11;  		//消息头－消息长度字
		cid_data[10] = 0x30+i;	//分机号码数据
		cid_data[11] = 0x30+j;
		cid_data[12] = 0x30+k;
	}
	else if(j != 0)
	{
		cid_data[1] = 10;  		//消息头－消息长度字
		cid_data[10] = 0x30+j;	//分机号码数据
		cid_data[11] = 0x30+k;	
	}
	else 
	{
		cid_data[1] = 9;  		//消息头－消息长度字
		cid_data[10] = 0x30+k;	//分机号码数据	
	}

	for(i=2;i<cid_data[1]+2;i++)
	{
		cid_data[i] = get_odd_parity(cid_data[i]);	//消息头不需要奇校验
	}

	check = get_checksum(cid_data,cid_data[1]+2); //从消息头开始校验
	check = get_complement(check);
	cid_data[cid_data[1]+2] = check;	//校验位
	return 1;
}



void send_msm7512_test(void)
{
	uint8_t i;

	uint8_t check;

	
	static uint8_t number=0;
	uint8_t cid_data[30];

	for(i=0;i<30;i++) cid_data[i] = 0x00;

	cid_data[0] = 0x04;  	//消息头－消息类型	
	cid_data[1] = 12;  		//消息头－消息长度字 


	cid_data[2] = '0';
	cid_data[3] = '8';

	cid_data[4] = '2';
	cid_data[5] = '2';

	cid_data[6] = '1';
	cid_data[7] = '4';

	cid_data[8] = '2';
	cid_data[9] = '5';

	
	cid_data[10] = '8';
	cid_data[11] = '0';
	cid_data[12] = '7';
	cid_data[13] = '0'+number;
	
	
	number++;
	if(number>=10) number=0;

	i=2;
	while(cid_data[i] != 0)
	{
		cid_data[i] = get_odd_parity(cid_data[i]);	//消息头不需要奇校验
		i++;
	}

	
	check = get_checksum(cid_data,cid_data[1]+2); //从消息头开始校验
	check = get_complement(check);


	
	msm_txe_ctrl(0);		//允许发送
	for(i=0;i<30;i++)	uart3_send_byte(0x55);	//信道占用信号

	soft_delay_half_ms(360);	//标志信号


	i=0;
	while(cid_data[i] != 0)
	{
		uart3_send_byte(cid_data[i]);
		i++;
	}

	uart3_send_byte(check); //校验和
	
	
	soft_delay_half_ms(20);
	msm_txe_ctrl(1);		//禁止发送





#if 0
	uint8_t i;

	uint8_t check;

	static uint8_t number=0;
	uint8_t cid_data[30];

	for(i=0;i<30;i++) cid_data[i] = 0x00;

	cid_data[0] = 0x04;  //消息头－消息类型
//	cid_data[1] = 0x13;  //消息头－消息长度字	
	cid_data[1] = 12;  //消息头－消息长度字	


	cid_data[2] = '0';
	cid_data[3] = '8';

	cid_data[4] = '2';
	cid_data[5] = '2';

	cid_data[6] = '1';
	cid_data[7] = '4';

	cid_data[8] = '2';
	cid_data[9] = '5';

	

/*	cid_data[10] = '1';
	cid_data[11] = '8';
	cid_data[12] = '8';
	cid_data[13] = '8';
	cid_data[14] = '2';
	cid_data[15] = '3';
	cid_data[16] = '4';
	cid_data[17] = '5';
	cid_data[18] = '6';
	cid_data[19] = '7';
	cid_data[20] = '2';
	cid_data[21] = 0;*/


	cid_data[10] = '8';
	cid_data[11] = '0';
	cid_data[12] = '7';
	cid_data[13] = '0'+number;


	number++;
	if(number>=10) number=0;

	i=2;
	while(cid_data[i] != 0)
	{
		cid_data[i] = get_odd_parity(cid_data[i]);	//消息头不需要奇校验
		i++;
	}

	
	check = get_checksum(cid_data);	//从消息头开始校验
	check = get_complement(check);


	
	msm_txe_ctrl(0);		//允许发送
	for(i=0;i<30;i++) 	uart3_send_byte(0x55);	//信道占用信号

	soft_delay_half_ms(360);	//标志信号


	i=0;
	while(cid_data[i] != 0)
	{
		uart3_send_byte(cid_data[i]);
		i++;
	}

	uart3_send_byte(check); //校验和



/*	uart3_send_byte(0x04);   //消息头－消息类型
//	soft_delay_half_ms(20);
	
	uart3_send_byte(0x13);   //消息头－消息长度字		
//	soft_delay_half_ms(20);*/


	//时间:月日时分
/*	
	uart3_send_byte(0xB0);   //消息体:月高位
	uart3_send_byte(0x38);   //月低位
	uart3_send_byte(0x32);   //日高位
	uart3_send_byte(0x32);   //日低位
	uart3_send_byte(0x31);	//时高位
	uart3_send_byte(0x34);	//时低位
	uart3_send_byte(0x32);	//分高位
	uart3_send_byte(0xB5);	//分低位35最高位为奇偶校验位*/

	
//电话号码		  
/*	uart3_send_byte(0x31); 
	uart3_send_byte(0x38);
	uart3_send_byte(0x38);
	uart3_send_byte(0x38);  
	uart3_send_byte(0x32);
	uart3_send_byte(0xB3);
	uart3_send_byte(0x34);
	uart3_send_byte(0xB5);
	uart3_send_byte(0xB6);
	uart3_send_byte(0x37);
	uart3_send_byte(0x32);*/

//	soft_delay_half_ms(20);
//	uart3_send_byte(0x8B);	  //校验和高位不需要奇校验从04－32　模256的和取补即intelhex校验和
	soft_delay_half_ms(20);
	msm_txe_ctrl(1);		//禁止发送
#endif
}

void init_tel(void)
{

	//电话机电压选择
	LPC_PINCON->PINSEL0 &= (~(0x03 << 10)); 			// P0.5  KBC  常态为0
	LPC_GPIO0->FIODIR    |= tel_ctr_pin;
	kbc_ctrl(DC60V);	


	//tlq电话对讲声音通道开关1:关闭　0:打开
	LPC_PINCON->PINSEL3 &= (~(0x03 << 24)); 			// P1.28  
	LPC_GPIO1->FIODIR    |= phone_tlq;
	phone_tlq_ctrl(1);		//关闭


	//ring 
	LPC_PINCON->PINSEL3 &= (~(0x03 << 30)); 			// P1.31  
	LPC_GPIO1->FIODIR	 |= phone_ring;
	phone_ring_ctrl(0);


	SetTelState(bTelTlpEnChk);	//允许检测
	tel_tlp_dis_chk =0;
	tel_ring_time =0;

	ClrTelState(bTelHungDowned);
	ClrTelState(bTelPickUped);


tel_chk:
	if(phone_tlp_status)
	{
		soft_delay_half_ms(40);
		if(phone_tlp_status)  
		{
			SetTelState(bOriTelTlp);
		}
		else goto tel_chk;
	}
	else 
	{
		soft_delay_half_ms(40);
		if(phone_tlp_status ==0) 
		{
			ClrTelState(bOriTelTlp);
		}
		else goto tel_chk;
	}

	
}



void tel_pickup_treat(void)
{
	if(GetDevState(FLAG_INDICATION))
	{	//如果正在报号,发送处理呼叫命令(紧急呼叫发送清除命令)		

		stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
		stBusFreq.bySndRoomAddr = stEepromCfgData.bySelfRoomAddr;
		stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
		stBusFreq.byRecSecAddr = stIndicationData.byIndicationSecAddr;
		stBusFreq.byRecRoomAddr = stIndicationData.byIndicationRoomAddr;
		stBusFreq.byRecBedAddr = stIndicationData.byIndicationBedAddr;				
		if(CMD_EMERGENCY_CALL == (stIndicationData.byCallCmd & 0x1f))
		{
			stBusFreq.byCmd = CMD_EMERGENCY_CLEAR;
		}
		else
		{	//不是紧急呼叫,置听筒接听标志,发送处理命令									
			stBusFreq.byCmd = stIndicationData.byCallCmd & 0x1f;
			stBusFreq.byCmd += 0x06; 
		}		
		Bus0OutputData(&(stBusFreq.bySndSecAddr));

		tel_pickup_time =0;

	}

	else
	{
		tel_pickup_time = TEL_PICKUP_TIME;
	}

}



void tel_hungdown_treat(void)
{

	tel_pickup_time =0;
	if(GetState(bHostTalk))
	{
		
		ClrState(bHostTalk);
		ClrDevState(FLAG_TALK);
		
		VoiceChannelCtx();

		memcpy(&(stBusFreq.bySndSecAddr),&(stCallAddr.bySndSecAddr),3);
		memcpy(&(stBusFreq.byRecSecAddr),&(stCallAddr.byRecSecAddr),3);
		stBusFreq.byCmd = CMD_SYSTERM_RESET;
		Bus0OutputData(&(stBusFreq.bySndSecAddr));

					
		set_talk_voice_volue(TALK_HOST_CH,0);
		set_talk_voice_volue(TALK_SLAVE_CH,0);
	}

	else if(GetState(bHostTalk1))
	{
		ClrState(bHostTalk1);
		ClrDevState(FLAG_TALK1);
		
		VoiceChannelCtx();

		memcpy(&(stBusFreq.bySndSecAddr),&(stCH1CallAddr.bySndSecAddr),3);
		memcpy(&(stBusFreq.byRecSecAddr),&(stCH1CallAddr.byRecSecAddr),3);
		stBusFreq.byCmd = CMD_CHANNEL_CLOSE;
		Bus0OutputData(&(stBusFreq.bySndSecAddr));
		set_talk_voice_volue(TALK_HOST_CH,0);
		set_talk_voice_volue(TALK_SLAVE_CH,0);			
	}

	else if(uiDevState & FLAG_BROADCAST)
	{
		memcpy(&(stBusFreq.bySndSecAddr),&(stCallAddr.bySndSecAddr),3);
		memcpy(&(stBusFreq.byRecSecAddr),&(stCallAddr.byRecSecAddr),3);
		stBusFreq.byCmd = CMD_SYSTERM_RESET;
		Bus0OutputData(&(stBusFreq.bySndSecAddr));	
		set_talk_voice_volue(TALK_SLAVE_CH,0);
	}

	AM79R70_standby();
}
