#define _IN_VOICE_PLAY_

#include "custom.h"


const DataFlashFormat DataFlashTable[] = {
	{PCM_0_ADDR, PCM_1_ADDR-PCM_0_ADDR},
	{PCM_1_ADDR, PCM_2_ADDR-PCM_1_ADDR},
	{PCM_2_ADDR, PCM_3_ADDR-PCM_2_ADDR},
	{PCM_3_ADDR, PCM_4_ADDR-PCM_3_ADDR},
	{PCM_4_ADDR, PCM_5_ADDR-PCM_4_ADDR},
	{PCM_5_ADDR, PCM_6_ADDR-PCM_5_ADDR},
	{PCM_6_ADDR, PCM_7_ADDR-PCM_6_ADDR},
	{PCM_7_ADDR, PCM_8_ADDR-PCM_7_ADDR},
	{PCM_8_ADDR, PCM_9_ADDR-PCM_8_ADDR},
	{PCM_9_ADDR, PCM_DTMF0_ADDR-PCM_9_ADDR},
	{PCM_DTMF0_ADDR,PCM_DTMF1_ADDR-PCM_DTMF0_ADDR},
	{PCM_DTMF1_ADDR,PCM_DTMF2_ADDR-PCM_DTMF1_ADDR},
	{PCM_DTMF2_ADDR,PCM_DTMF3_ADDR-PCM_DTMF2_ADDR},
	{PCM_DTMF3_ADDR,PCM_DTMF4_ADDR-PCM_DTMF3_ADDR},
	{PCM_DTMF4_ADDR,PCM_DTMF5_ADDR-PCM_DTMF4_ADDR},
	{PCM_DTMF5_ADDR,PCM_DTMF6_ADDR-PCM_DTMF5_ADDR},
	{PCM_DTMF6_ADDR,PCM_DTMF7_ADDR-PCM_DTMF6_ADDR},
	{PCM_DTMF7_ADDR,PCM_DTMF8_ADDR-PCM_DTMF7_ADDR},
	{PCM_DTMF8_ADDR,PCM_DTMF9_ADDR-PCM_DTMF8_ADDR},
	{PCM_DTMF9_ADDR,PCM_DTMFPSOUND_ADDR-PCM_DTMF9_ADDR},
	{PCM_DTMFPSOUND_ADDR,PCM_DTMFSTART_ADDR-PCM_DTMFPSOUND_ADDR},
	{PCM_DTMFSTART_ADDR,PCM_Tock_ADDR-PCM_DTMFSTART_ADDR},
	{PCM_Tock_ADDR,PCM_GY_ADDR-PCM_Tock_ADDR},		
	{PCM_GY_ADDR, PCM_JIA_ADDR-PCM_GY_ADDR},				
	{PCM_JIA_ADDR, PCM_SHI_ADDR-PCM_JIA_ADDR},
	{PCM_SHI_ADDR, PCM_WSJ_ADDR-PCM_SHI_ADDR},
	{PCM_WSJ_ADDR, PCM_HAO_ADDR-PCM_WSJ_ADDR},
	{PCM_HAO_ADDR, PCM_HJ_ADDR-PCM_HAO_ADDR},
	{PCM_HJ_ADDR, PCM_WB_ADDR-PCM_HJ_ADDR},
	{PCM_WB_ADDR, PCM_CHUANG_ADDR-PCM_WB_ADDR},
	{PCM_CHUANG_ADDR, PCM_KSH_ADDR-PCM_CHUANG_ADDR},
	{PCM_KSH_ADDR, PCM_CHG_ADDR-PCM_KSH_ADDR},
	{PCM_CHG_ADDR, PCM_FANG_ADDR-PCM_CHG_ADDR},
	{PCM_FANG_ADDR, PCM_BAI_ADDR-PCM_FANG_ADDR},	
	{PCM_BAI_ADDR, PCM_SHY_ADDR-PCM_BAI_ADDR},
	{PCM_SHY_ADDR, PCM_HANG_TEL_ADDR-PCM_SHY_ADDR},
	{PCM_HANG_TEL_ADDR, PCM_CONNECTED_WIFI_ADDR-PCM_HANG_TEL_ADDR},
	{PCM_CONNECTED_WIFI_ADDR, PCM_CONNECTED_SERVER_ADDR-PCM_CONNECTED_WIFI_ADDR},
	{PCM_CONNECTED_SERVER_ADDR, PCM_END_ADDR-PCM_CONNECTED_SERVER_ADDR},
		
	{PCM_PAUSE_ADDR, 2000}
};



//�������Ŷ���


int8_t voice_buff[13312];




int16_t *pvoice_data;
int16_t value;
uint32_t voice_data_addr;
uint32_t voice_data_len;
uint8_t  cur_play_index=0;

uint16_t voice_seg_pause_time=0;


uint32_t voice_data_total_len;

volatile uint8_t		voice_play_sequence[20];
volatile uint8_t		voice_play_seq_cnt;

volatile uint8_t 		voice_play_turns;
volatile uint8_t		voice_play_times=4;

volatile uint8_t 		played_times;	//�ѱ��ű���
volatile uint8_t		played_turns;	//�ѱ�������
volatile uint16_t		voice_times_delay_time;	//���ű������ʱ��
volatile uint16_t		voice_beds_delay_time;	//���Ŵ�λ���ʱ��
volatile uint16_t		voice_turns_delay_time;	//�����������ʱ��



uint16_t voice_play_state=0;

/*#define VOICE_PALY_START 	0x01
#define VOICE_PLAY_END	 	0x02
#define VOICE_PLAY_NEXT_SEG 0x04*/

void init_voice_play_sequence(void);


void dac_pin_config(void)
{
	LPC_PINCON->PINSEL1 |= (0x02 << 20);
}



/*********************************************************************************************
*
*
* ��ȡ����ʱ����������
*********************************************************************************************/
void load_voice_play_sequence(uint8_t call_type,uint8_t room,uint8_t bed)
{

	uint8_t i,j,k,wcflg = 0;
//	uint8_t m,slatyp=0;
//	uint16_t p;

	init_voice_play_sequence();
	
	if(room==0 && bed==0) return;

	

	i = room/100;			//��λ
	j = (room%100)/10;		//ʮλ
	k = room%10;			//��λ


	if((0==i)&&(0==j)&&(0==k))
	{
		
	}
  	else
  	{
  		if(i!=0)
		{
			voice_play_sequence[voice_play_seq_cnt++] = i;
			voice_play_sequence[voice_play_seq_cnt++] = BAI_NUM;
			if(j!=0 || k!=0)
			{
				voice_play_sequence[voice_play_seq_cnt++] = j;
				if(j!=0)  	voice_play_sequence[voice_play_seq_cnt++] = SHI_NUM;

				if(k!=0)	voice_play_sequence[voice_play_seq_cnt++] = k;
				
			}
			
		}
		else
		{
			if(j != 0)	   //ʮλ��Ϊ��
			{
				if(1 != j)	//10 ~ 19 ������ʮλ��1
				{
					voice_play_sequence[voice_play_seq_cnt++]=j;
				}
				voice_play_sequence[voice_play_seq_cnt++]=SHI_NUM;	 //ʮ
				if(k != 0)	 //��λ��Ϊ��
				{
					voice_play_sequence[voice_play_seq_cnt++]=k;
				}
			}
			else			//ʮλΪ��
			{
				if(k != 0)	 //��λ��Ϊ��
				{
					voice_play_sequence[voice_play_seq_cnt++]=k;
				}
			}			
		}
		voice_play_sequence[voice_play_seq_cnt++] = FANG_NUM;
  	}

	i = bed/100;			//��λ
	j = (bed%100)/10;		//ʮλ
	k = bed%10;			//��λ

	if((0==i)&&(0==j)&&(0==k))
	{
		
	}
  	else
  	{
  		if(i!=0)
		{
			voice_play_sequence[voice_play_seq_cnt++] = i;
			voice_play_sequence[voice_play_seq_cnt++] = BAI_NUM;
			if(j!=0 || k!=0)
			{
				voice_play_sequence[voice_play_seq_cnt++] = j;
				if(j!=0)  	voice_play_sequence[voice_play_seq_cnt++] = SHI_NUM;

				if(k!=0)	voice_play_sequence[voice_play_seq_cnt++] = k;
				
			}
			
		}
		else
		{
			if(j != 0)	   //ʮλ��Ϊ��
			{
				if(1 != j)	//10 ~ 19 ������ʮλ��1
				{
					voice_play_sequence[voice_play_seq_cnt++]=j;
				}
				voice_play_sequence[voice_play_seq_cnt++]=SHI_NUM;	 //ʮ
				if(k != 0)	 //��λ��Ϊ��
				{
					voice_play_sequence[voice_play_seq_cnt++]=k;
				}
			}
			else			//ʮλΪ��
			{
				if(k != 0)	 //��λ��Ϊ��
				{
					voice_play_sequence[voice_play_seq_cnt++]=k;
				}
			}			
		}
		voice_play_sequence[voice_play_seq_cnt++] = CHUANG_NUM;
  	}

    voice_play_sequence[voice_play_seq_cnt]=HJ_NUM;	//����


/*	for(i=0;i<=voice_play_seq_cnt;i++)
	{
		uart2_send_byte(voice_play_sequence[i]);
	}*/

	voice_data_addr = 	DataFlashTable[voice_play_sequence[cur_play_index]].addr;
	voice_data_len  =   DataFlashTable[voice_play_sequence[cur_play_index]].len;



	sst_fast_read(voice_buff,voice_data_addr,voice_data_len);
	pvoice_data =(int16_t *)voice_buff; 


	voice_data_len /= 2;
	voice_play_state |= VOICE_PLAY_START;	
}		 

/*********************************************************************************************
*
*
*
*********************************************************************************************/
void init_voice_play_sequence()
{
  uint8_t i;
  for(i=0;i<20;i++)
  {
    voice_play_sequence[i] = 0;
  }
  cur_play_index = 0;
  voice_play_seq_cnt = 0;
  voice_play_state &= (~(VOICE_PLAY_START|VOICE_PLAY_NEXT_SEG));

}


uint8_t MakeCallVoiceData(STIndicationData *pstIndicationData)
{
	uint8_t i,j,k;
	uint8_t room=0;
	uint8_t bed=0;
	uint8_t cmd=0;

	room = pstIndicationData->byIndicationRoomAddr;
	bed  = pstIndicationData->byIndicationBedAddr;
	cmd = pstIndicationData->byCallCmd & 0x1f;

	init_voice_play_sequence();
	
	
	if(room==0 && bed==0) return 0;

	if(room == ADD_BED_FJ)
	{//�Ӵ��ֻ�
		
		voice_play_sequence[voice_play_seq_cnt++] = JIA_NUM;
		
		i = bed/100;			//��λ
		j = (bed%100)/10;		//ʮλ
		k = bed%10; 		//��λ
/*		if((0==i)&&(0==j)&&(0==k))
		{
			
		}
		else*/
		{
			if(i!=0)
			{
				voice_play_sequence[voice_play_seq_cnt++] = i;
				voice_play_sequence[voice_play_seq_cnt++] = BAI_NUM;
				if(j!=0 || k!=0)
				{
					voice_play_sequence[voice_play_seq_cnt++] = j;
					if(j!=0)	voice_play_sequence[voice_play_seq_cnt++] = SHI_NUM;
		
					if(k!=0)	voice_play_sequence[voice_play_seq_cnt++] = k;
					
				}
				
			}
			else
			{
				if(j != 0)	   //ʮλ��Ϊ��
				{
					if(1 != j)	//10 ~ 19 ������ʮλ��1
					{
						voice_play_sequence[voice_play_seq_cnt++]=j;
					}
					voice_play_sequence[voice_play_seq_cnt++]=SHI_NUM;	 //ʮ
					if(k != 0)	 //��λ��Ϊ��
					{
						voice_play_sequence[voice_play_seq_cnt++]=k;
					}
				}
				else			//ʮλΪ��
				{
					if(k != 0)	 //��λ��Ϊ��
					{
						voice_play_sequence[voice_play_seq_cnt++]=k;
					}
				}			
			}
			voice_play_sequence[voice_play_seq_cnt++] = CHUANG_NUM;
		}

		voice_play_sequence[voice_play_seq_cnt]=HJ_NUM; //����

		return 1;
			
	}
	


	switch(cmd)
	{
		case CMD_INFUSION_CALL:	//��ͷ�ֻ�����
		case CMD_SERVICE_CALL:	//��Һ��������Ϻ���		
			//if(voice_play_state & VOICE_PLAY_ROOM)
			if(stEepromCfgData.bPlayRoom)
			{
				i = room/100;			//��λ
				j = (room%100)/10;		//ʮλ
				k = room%10;			//��λ


				if((0==i)&&(0==j)&&(0==k))
				{
					
				}
			  	else
			  	{
			  		if(i!=0)
					{
						voice_play_sequence[voice_play_seq_cnt++] = i;
						voice_play_sequence[voice_play_seq_cnt++] = BAI_NUM;
						if(j!=0 || k!=0)
						{
							voice_play_sequence[voice_play_seq_cnt++] = j;
							if(j!=0)  	voice_play_sequence[voice_play_seq_cnt++] = SHI_NUM;

							if(k!=0)	voice_play_sequence[voice_play_seq_cnt++] = k;
							
						}
						
					}
					else
					{
						if(j != 0)	   //ʮλ��Ϊ��
						{
							if(1 != j)	//10 ~ 19 ������ʮλ��1
							{
								voice_play_sequence[voice_play_seq_cnt++]=j;
							}
							voice_play_sequence[voice_play_seq_cnt++]=SHI_NUM;	 //ʮ
							if(k != 0)	 //��λ��Ϊ��
							{
								voice_play_sequence[voice_play_seq_cnt++]=k;
							}
						}
						else			//ʮλΪ��
						{
							if(k != 0)	 //��λ��Ϊ��
							{
								voice_play_sequence[voice_play_seq_cnt++]=k;
							}
						}			
					}
					voice_play_sequence[voice_play_seq_cnt++] = FANG_NUM;
			  	}		
			}
			i = bed/100;			//��λ
			j = (bed%100)/10;		//ʮλ
			k = bed%10;			//��λ
			if((0==i)&&(0==j)&&(0==k))
			{
				
			}
			else
			{
				if(i!=0)
				{
					voice_play_sequence[voice_play_seq_cnt++] = i;
					voice_play_sequence[voice_play_seq_cnt++] = BAI_NUM;
					if(j!=0 || k!=0)
					{
						voice_play_sequence[voice_play_seq_cnt++] = j;
						if(j!=0)	voice_play_sequence[voice_play_seq_cnt++] = SHI_NUM;
			
						if(k!=0)	voice_play_sequence[voice_play_seq_cnt++] = k;
						
					}
					
				}
				else
				{
					if(j != 0)	   //ʮλ��Ϊ��
					{
						if(1 != j)	//10 ~ 19 ������ʮλ��1
						{
							voice_play_sequence[voice_play_seq_cnt++]=j;
						}
						voice_play_sequence[voice_play_seq_cnt++]=SHI_NUM;	 //ʮ
						if(k != 0)	 //��λ��Ϊ��
						{
							voice_play_sequence[voice_play_seq_cnt++]=k;
						}
					}
					else			//ʮλΪ��
					{
						if(k != 0)	 //��λ��Ϊ��
						{
							voice_play_sequence[voice_play_seq_cnt++]=k;
						}
					}			
				}
				voice_play_sequence[voice_play_seq_cnt++] = CHUANG_NUM;
			}
			if(cmd == CMD_INFUSION_CALL)
			{
				voice_play_sequence[voice_play_seq_cnt]=HJ_NUM;	//����
			}

			else if(cmd == CMD_SERVICE_CALL)
			{
				voice_play_sequence[voice_play_seq_cnt]=SHY_NUM;	//��Һ
				voice_play_sequence[voice_play_seq_cnt]=WB_NUM;	//���
			}			
			break;
		case CMD_EMERGENCY_CALL:	//���������
			i = room/100;			//��λ
			j = (room%100)/10;		//ʮλ
			k = room%10;			//��λ


			if((0==i)&&(0==j)&&(0==k))
			{
				
			}
		  	else
		  	{
		  		if(i!=0)
				{
					voice_play_sequence[voice_play_seq_cnt++] = i;
					voice_play_sequence[voice_play_seq_cnt++] = BAI_NUM;
					if(j!=0 || k!=0)
					{
						voice_play_sequence[voice_play_seq_cnt++] = j;
						if(j!=0)  	voice_play_sequence[voice_play_seq_cnt++] = SHI_NUM;

						if(k!=0)	voice_play_sequence[voice_play_seq_cnt++] = k;
						
					}
					
				}
				else
				{
					if(j != 0)	   //ʮλ��Ϊ��
					{
						if(1 != j)	//10 ~ 19 ������ʮλ��1
						{
							voice_play_sequence[voice_play_seq_cnt++]=j;
						}
						voice_play_sequence[voice_play_seq_cnt++]=SHI_NUM;	 //ʮ
						if(k != 0)	 //��λ��Ϊ��
						{
							voice_play_sequence[voice_play_seq_cnt++]=k;
						}
					}
					else			//ʮλΪ��
					{
						if(k != 0)	 //��λ��Ϊ��
						{
							voice_play_sequence[voice_play_seq_cnt++]=k;
						}
					}			
				}
				voice_play_sequence[voice_play_seq_cnt++] = FANG_NUM;
  			}
			voice_play_sequence[voice_play_seq_cnt]=HJ_NUM;	//����	
			break;
		default:
			return 0;
	}
	return 1;
}


void MakeRingVoiceData(STIndicationData *pstIndicationData)
{

}

void StartVoicePlay(void)
{
	NVIC_DisableIRQ(TIMER2_IRQn);	//�����жϲ�ʹ��

	voice_data_addr =	DataFlashTable[voice_play_sequence[cur_play_index]].addr;
	voice_data_len	=	DataFlashTable[voice_play_sequence[cur_play_index]].len;


	sst_fast_read(voice_buff,voice_data_addr,voice_data_len);
	pvoice_data =(int16_t *)voice_buff;	


	voice_data_len /= 2;


	voice_play_state |= VOICE_PLAY_START;

	voice_data_total_len =0;
	voice_play_state &= (~VOICE_PLAY_ONE_WORD);
	
	NVIC_EnableIRQ(TIMER2_IRQn);	//�����жϲ�ʹ��

}


void StopVoicePlay(void)
{
	voice_play_state &= (~VOICE_PLAY_START);
	voice_play_state &= (~(VOICE_PLAY_NEXT_SEG|VOICE_PLAY_NEXT_TIMES|VOICE_PLAY_NEXT_BED));
	cur_play_index = 0;
	voice_play_seq_cnt = 0;
	voice_data_len =0;




	played_times =0;
	played_turns =0;
	voice_times_delay_time =0;
	voice_beds_delay_time =0;
	
	
}

void voice_play_word(uint8_t index)
{

	uint16_t i=0,j=0;


	NVIC_DisableIRQ(TIMER2_IRQn);	//�����жϲ�ʹ��

	cur_play_index=0;
	voice_play_seq_cnt =0;
	voice_data_addr = DataFlashTable[index].addr;
	voice_data_len  = DataFlashTable[index].len;

	
	voice_data_total_len =  voice_data_len;


	if(voice_data_total_len <= VOICE_BUFF_MAX)
	{
		voice_data_total_len =0;
	}
	else	//һ������Ƶ���ݱȽϴ�,������󻺳���
	{
		voice_data_total_len  -= VOICE_BUFF_MAX;
		voice_data_len = VOICE_BUFF_MAX;


	}


	sst_read((uint8_t *)voice_buff,voice_data_addr,voice_data_len);
	pvoice_data =(int16_t *)voice_buff;


	voice_data_addr += VOICE_BUFF_MAX;
	

	voice_play_state |= VOICE_PLAY_START;
	voice_data_len /= 2;

	voice_play_state |= VOICE_PLAY_ONE_WORD;

	NVIC_EnableIRQ(TIMER2_IRQn);    //�����жϲ�ʹ��	
}




void voice_continue_play_word(void)
{

	NVIC_DisableIRQ(TIMER2_IRQn);	//�����жϲ�ʹ��

	voice_play_state &= (~VOICE_PLAY_READ_EEPROM_DATA);


	if(voice_data_total_len < VOICE_BUFF_MAX)
	{
		voice_data_len = voice_data_total_len;
		voice_data_total_len =0;

	}
	else	//һ������Ƶ���ݱȽϴ�,������󻺳���
	{
		voice_data_total_len  -= VOICE_BUFF_MAX;
		voice_data_len = VOICE_BUFF_MAX;
	}


	sst_read((uint8_t *)voice_buff,voice_data_addr,voice_data_len);
	pvoice_data =(int16_t *)voice_buff;



	voice_data_addr += VOICE_BUFF_MAX;


	voice_play_state |= VOICE_PLAY_START;
	voice_data_len /= 2;

	voice_play_state |= VOICE_PLAY_ONE_WORD;	
	NVIC_EnableIRQ(TIMER2_IRQn);	//�����жϲ�ʹ��

}


void voice_play_treat(void)
{
	if(voice_play_state & VOICE_PLAY_NEXT_SEG)
	{
		voice_play_state &= (~VOICE_PLAY_NEXT_SEG);
		StartVoicePlay();
	}

	else if(voice_play_state & VOICE_PLAY_NEXT_TIMES)
	{//ͬһ��λ����һ��
		voice_play_state &= (~VOICE_PLAY_NEXT_TIMES);
		StartVoicePlay();
	}
	
	else if(voice_play_state & VOICE_PLAY_NEXT_BED)
	{//��һ��λ
		voice_play_state &= (~VOICE_PLAY_NEXT_BED);
		if(uiDevState & FLAG_INDICATION)
		{
			if(0x00 == (uiDevState & FLAG_STOP_EXIST))
			{	//û�����ڷ���ֹͣ����
		
				stBusFreq.byCmd = CMD_STOP_INDICATION;						
				if(true == Bus0OutputData(&(stBusFreq.bySndSecAddr)))
				{
					uiDevState |= FLAG_STOP_EXIST;
				}						
			}
		}

	}

}


