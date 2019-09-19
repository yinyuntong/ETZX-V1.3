
#define _IN_TEL_

#include"custom.h"


uint16_t TelState=0;								//�绰������ر�־
/*#define  bTelRing			(1<<1)				//�绰������״̬
#define  bTelRingOut		(1<<2)				//�绰��������ߵ�ƽ
#define  bTelTlpEnChk		(1<<3)				//�绰��TLP������
#define  bTelHungDowned		(1<<4)				//�绰���һ�����
#define  bTelPickUped		(1<<5)				//�绰��ժ������
#define  bOriTelTlp			(1<<6)				//ԭtlp״̬
#define  bCID				(1<<7)				//������ʾ״̬
#define  bCID_CMD			(1<<8)				//����CID����


#define SetTelState(x) (TelState|=(x))			//��λ����
#define ClrTelState( x) (TelState&=~(x))			//�������
#define GetTelState(x) (TelState&(x))			//��ȡλ״̬*/


uint16_t tel_ring_time=0;			//�绰����ʱ��
uint16_t tel_tlp_dis_chk=0;			//��ֹ���ʱ��
uint16_t tel_tlp_chk_delay=0;		//�绰�����ʱʱ��
uint16_t CID_marks_time =0;			//��־�ź���ʱʱ��
uint16_t CID_txe_delay=0;			//����������ʱ��ʱ�䵽���ͽ�ֹ
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
	{//�������ڶԽ�״̬
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
			{//�绰�㲥
				stBusFreq.bySndSecAddr = stEepromCfgData.bySelfSecAddr;
				stBusFreq.bySndRoomAddr= stEepromCfgData.bySelfRoomAddr;
				stBusFreq.bySndBedAddr = stEepromCfgData.bySelfBedAddr;
				stBusFreq.byCmd = CMD_BROADCAST3;		//ȫ���㲥
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
			{//��ͷ�ֻ�
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
			{//ҽ���ֻ�
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


	LPC_PINCON->PINMODE3 |= (0X03<<4);					//ʹ��p1.18����

	LPC_PINCON->PINMODE3 |= (0X03<<6);					//ʹ��p1.19����	

	LPC_PINCON->PINMODE3 |= (0X03<<12);					//ʹ��p1.22����

	AM79R70_standby();
}


//����ģʽ
void AM79R70_ringing(void)
{
	AM79R70_c3_ctrl(0);
	AM79R70_c2_ctrl(0);
	AM79R70_c1_ctrl(1);	

}



//����ģʽ
void AM79R70_standby(void)
{
	AM79R70_c3_ctrl(1);
	AM79R70_c2_ctrl(0);
	AM79R70_c1_ctrl(1);	
}


//ͨ��ģʽ
void AM79R70_active(void)
{
	AM79R70_c3_ctrl(0);
	AM79R70_c2_ctrl(1);
	AM79R70_c1_ctrl(0);	

}


void init_msm7512(void)
{
	//msm��������ʾоƬ���ͽ�ֹ
	LPC_PINCON->PINSEL0 &= (~(0x03 << 8)); 			// P0.4   =1:��ֹ����
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


//��У��: ������1
uint8_t get_odd_parity(uint8_t ori_data)
{
	uint8_t i;
	uint8_t number=0;
	uint8_t data_temp;

	ori_data &= 0x7f;		//�����λ��0
	data_temp = ori_data;
	
	for(i=0;i<7;i++)
	{
		if(ori_data & 0x01) number++;
		ori_data >>= 1;	
	}

	if(number%2 == 0)	//ԭ������ż����1���������λ��1���γ�������1
	{
		data_temp |= 0x80;
	}
	return(data_temp);
}


uint8_t get_complement(uint8_t ori_data)
{
/*	if(ori_data & 0x80) //����
	{
		ori_data ^= 0x7f;		//����
		ori_data ++;			//����
	}*/

	ori_data ^= 0xff;		//����
	ori_data ++;			//����

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


	cid_data[0] = 0x04;  	//��Ϣͷ����Ϣ����


//����Ϊ��Ϣ�е����ڡ�ʱ������
	cid_data[2] =	(stTime.byMonth/0x10)+0x30;
	cid_data[3] = 	(stTime.byMonth%0x10)+0x30;

	cid_data[4] =	(stTime.byDay/0x10)+0x30;
	cid_data[5] = 	(stTime.byDay%0x10)+0x30;

	cid_data[6] = 	(stTime.byHour/0x10)+0x30;
	cid_data[7] = 	(stTime.byHour%0x10)+0x30;

	cid_data[8] = 	(stTime.byMinute/0x10)+0x30;
	cid_data[9] = 	(stTime.byMinute%0x10)+0x30;


	if(bed==0)
	{//������ֻ�����
		i = room/100;			//��λ
		j = (room%100)/10;		//ʮλ
		k = room%10;			//��λ		
	}
	else
	{
		i = bed/100;			//��λ
		j = (bed%100)/10;		//ʮλ
		k = bed%10;			//��λ	
	}
	if(i != 0) 
	{
		cid_data[1] = 11;  		//��Ϣͷ����Ϣ������
		cid_data[10] = 0x30+i;	//�ֻ���������
		cid_data[11] = 0x30+j;
		cid_data[12] = 0x30+k;
	}
	else if(j != 0)
	{
		cid_data[1] = 10;  		//��Ϣͷ����Ϣ������
		cid_data[10] = 0x30+j;	//�ֻ���������
		cid_data[11] = 0x30+k;	
	}
	else 
	{
		cid_data[1] = 9;  		//��Ϣͷ����Ϣ������
		cid_data[10] = 0x30+k;	//�ֻ���������	
	}

	for(i=2;i<cid_data[1]+2;i++)
	{
		cid_data[i] = get_odd_parity(cid_data[i]);	//��Ϣͷ����Ҫ��У��
	}

	check = get_checksum(cid_data,cid_data[1]+2); //����Ϣͷ��ʼУ��
	check = get_complement(check);
	cid_data[cid_data[1]+2] = check;	//У��λ
	return 1;
}



void send_msm7512_test(void)
{
	uint8_t i;

	uint8_t check;

	
	static uint8_t number=0;
	uint8_t cid_data[30];

	for(i=0;i<30;i++) cid_data[i] = 0x00;

	cid_data[0] = 0x04;  	//��Ϣͷ����Ϣ����	
	cid_data[1] = 12;  		//��Ϣͷ����Ϣ������ 


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
		cid_data[i] = get_odd_parity(cid_data[i]);	//��Ϣͷ����Ҫ��У��
		i++;
	}

	
	check = get_checksum(cid_data,cid_data[1]+2); //����Ϣͷ��ʼУ��
	check = get_complement(check);


	
	msm_txe_ctrl(0);		//������
	for(i=0;i<30;i++)	uart3_send_byte(0x55);	//�ŵ�ռ���ź�

	soft_delay_half_ms(360);	//��־�ź�


	i=0;
	while(cid_data[i] != 0)
	{
		uart3_send_byte(cid_data[i]);
		i++;
	}

	uart3_send_byte(check); //У���
	
	
	soft_delay_half_ms(20);
	msm_txe_ctrl(1);		//��ֹ����





#if 0
	uint8_t i;

	uint8_t check;

	static uint8_t number=0;
	uint8_t cid_data[30];

	for(i=0;i<30;i++) cid_data[i] = 0x00;

	cid_data[0] = 0x04;  //��Ϣͷ����Ϣ����
//	cid_data[1] = 0x13;  //��Ϣͷ����Ϣ������	
	cid_data[1] = 12;  //��Ϣͷ����Ϣ������	


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
		cid_data[i] = get_odd_parity(cid_data[i]);	//��Ϣͷ����Ҫ��У��
		i++;
	}

	
	check = get_checksum(cid_data);	//����Ϣͷ��ʼУ��
	check = get_complement(check);


	
	msm_txe_ctrl(0);		//������
	for(i=0;i<30;i++) 	uart3_send_byte(0x55);	//�ŵ�ռ���ź�

	soft_delay_half_ms(360);	//��־�ź�


	i=0;
	while(cid_data[i] != 0)
	{
		uart3_send_byte(cid_data[i]);
		i++;
	}

	uart3_send_byte(check); //У���



/*	uart3_send_byte(0x04);   //��Ϣͷ����Ϣ����
//	soft_delay_half_ms(20);
	
	uart3_send_byte(0x13);   //��Ϣͷ����Ϣ������		
//	soft_delay_half_ms(20);*/


	//ʱ��:����ʱ��
/*	
	uart3_send_byte(0xB0);   //��Ϣ��:�¸�λ
	uart3_send_byte(0x38);   //�µ�λ
	uart3_send_byte(0x32);   //�ո�λ
	uart3_send_byte(0x32);   //�յ�λ
	uart3_send_byte(0x31);	//ʱ��λ
	uart3_send_byte(0x34);	//ʱ��λ
	uart3_send_byte(0x32);	//�ָ�λ
	uart3_send_byte(0xB5);	//�ֵ�λ35���λΪ��żУ��λ*/

	
//�绰����		  
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
//	uart3_send_byte(0x8B);	  //У��͸�λ����Ҫ��У���04��32��ģ256�ĺ�ȡ����intelhexУ���
	soft_delay_half_ms(20);
	msm_txe_ctrl(1);		//��ֹ����
#endif
}

void init_tel(void)
{

	//�绰����ѹѡ��
	LPC_PINCON->PINSEL0 &= (~(0x03 << 10)); 			// P0.5  KBC  ��̬Ϊ0
	LPC_GPIO0->FIODIR    |= tel_ctr_pin;
	kbc_ctrl(DC60V);	


	//tlq�绰�Խ�����ͨ������1:�رա�0:��
	LPC_PINCON->PINSEL3 &= (~(0x03 << 24)); 			// P1.28  
	LPC_GPIO1->FIODIR    |= phone_tlq;
	phone_tlq_ctrl(1);		//�ر�


	//ring 
	LPC_PINCON->PINSEL3 &= (~(0x03 << 30)); 			// P1.31  
	LPC_GPIO1->FIODIR	 |= phone_ring;
	phone_ring_ctrl(0);


	SetTelState(bTelTlpEnChk);	//������
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
	{	//������ڱ���,���ʹ����������(�������з����������)		

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
		{	//���ǽ�������,����Ͳ������־,���ʹ�������									
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
