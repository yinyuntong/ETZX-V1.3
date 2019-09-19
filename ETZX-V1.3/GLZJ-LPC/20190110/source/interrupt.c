#define _IN_INTERRUPT_

#include"custom.h"

uint16_t ms_unit=0;							//���뵥λ��30 000 ms = 30��
uint16_t sec_unit=0;						//�뵥λ
uint8_t flag_readtime=0;					//��ʱ���־

void UART0_IRQHandler (void);
void NMI_Handler(void);
void HardFault_Handler(void);
void UART2_IRQHandler (void);
void TIMER0_IRQHandler (void);
void EINT0_IRQHandler (void);

void WDT_IRQHandler(void);


extern uint8_t gb_fz_status;
/*********************************************************************************************************
* Function Name:        NMI_Handler
* Description:          NMI �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void NMI_Handler(void)
{
  for(;;);
}
/*********************************************************************************************************
* Function Name:        HardFault_Handler
* Description:          HardFault_Handler �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void HardFault_Handler(void)
{
  for(;;);
}
/*********************************************************************************************************
* Function Name:        MemManage_Handler
* Description:          MemManage_Handler �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void MemManage_Handler(void)
{
  for(;;);
}
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void WDT_IRQHandler(void)
{
  uint32_t wdt_test_i;
  wdt_test_i = LPC_WDT -> WDTV;
  for(;;);
}


 /*********************************************************************************************************
* Function Name:        UART0_IRQHandler
* Description:          UART0 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART0_IRQHandler (void)
{
    uint8_t i = 0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART0->IIR) & 0x01) == 0) 			 /*  �ж��Ƿ����жϹ���   0:���жϹ���       */
	{                              
        switch (IIR & 0x0E)									/*  �ж��жϱ�־                */
		{                                 
        
            case 0x04:                                                  /*  ���������ж�                */
                scon0_time = 50;
                for (i = 0; i < 8; i++)					/*  ��������8���ֽ�             */
				{                 
                    *scon0_ptr++ = LPC_UART0->RBR;
                    scon0_len++;
					if(scon0_len>UART0_RX_BUF_SIZE)	scon0_ptr=scon0_Rcvbuf;                	
                }
                break;
            
            case 0x0C:                                                  /*  �ַ���ʱ�ж�                */
                scon0_time = 50;
                while ((LPC_UART0->LSR & 0x01) == 0x01)					 /*  �ж������Ƿ�������        */ 
				{               
					*scon0_ptr++ = LPC_UART0->RBR;
					scon0_len++;
					if(scon0_len>UART0_RX_BUF_SIZE)	scon0_ptr=scon0_Rcvbuf;

                }
                break;
            case 0x02:	//THRE�ж�
				stUsartCfg[0].uiSndLen--;
				stUsartCfg[0].uiSndPos++;
				if(stUsartCfg[0].uiSndLen!=0)
				{
					
					LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];				
					
				}
				else 
				{//һ֡���ݷ������
				
					LPC_UART0->IER  &= (~0X02);		//��ֹ�����ж�
					if((byUsart0SndBuf[REC_ROOM_POSITION]== ZLDIS_ADDR2) || (byUsart0SndBuf[REC_ROOM_POSITION]== YHFJ_ADDR2))	//������ʾ�������ݻ�ҽ���ֻ�������
					{
						byWaitRS485AckDTime = 0;		//�㲥���ݲ��ٵ���
						byUsart0SdResultDTime = 50;		//50*20 = 1000ms		//1000ms��ʼ���ͽ������
					}
					else
					{
						if(byUsart0SndBuf[DATA4_POSITION]==0x00)	//���ǽ���֡
						{

							byWaitRS485AckDTime = 0;
							if(byUsart0SndBuf[DATA2_POSITION]==1)
							{
								//��һ֡����Ҫ��洢�� ������32K����18ms
								byUsart0SdResultDTime = 20;	//20*20 = 400ms	 400ms��ʼ���ͽ������
							}
									
							else byUsart0SdResultDTime = 10;	//10*20 = 200ms  200ms��ʼ���ͽ������
						}

						else 
						{//�ǽ���֡	
							if(byUsart0SndBuf[CMD_POSITION]==CMD_DISPLAY_FORM)
							{//�Ƿ��ʹ�ͷ�ֻ���ʽ
								byUsart0SdResultDTime = 10;	//10*20 = 200ms   Ϊ�ֻ��������ݶ�����ʱ�����⴮�ڽ��ջ����������ݱ���
							}
						}
					}
				}           
            	break;
            default:
                break;
        }
    } 
}


/*********************************************************************************************************
* Function Name:        UART1_IRQHandler
* Description:          UART1 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART1_IRQHandler (void)
{
	uint8_t i=0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART1->IIR) & 0x01) == 0) 			 /*  �ж��Ƿ����жϹ���   0:���жϹ���       */
	{                               
        switch (LPC_UART1->IIR & 0x0E)									 /*  �ж��жϱ�־                */
		{                                
        
            case 0x04:                                                  /*  ���������ж�                */
				scon1_time = 50;
				for(i=0;i<8;i++)
				{
                    *scon1_ptr++ = LPC_UART1->RBR;
                    scon1_len++;
					if(scon1_len>UART1_RX_BUF_SIZE) 
					{
						scon1_time = 1;			//��������
						scon1_len--;
						scon1_ptr=scon1_Rcvbuf;
//						LPC_UART1->IER	&= (~0x01);					//���޽�ֹ�����жϣ���������󿪷�
						break;
						
					}
				
				}
				if(GetWpaState(CONNECTED_SERVER))
				{
					wifi_server_led_flash();
				}
                break;
            
            case 0x0C:                                                  /*  �ַ���ʱ�ж�                */
                scon1_time = 50;
                while ((LPC_UART1->LSR & 0x01) == 0x01)                 /*  �ж������Ƿ�������        */ 
				{
                    *scon1_ptr++ = LPC_UART1->RBR;
                    scon1_len++;
					if(scon1_len>UART1_RX_BUF_SIZE)
					{
						scon1_time = 1;			//��������
						scon1_len--;
						scon1_ptr=scon1_Rcvbuf;
//						LPC_UART1->IER	&= (~0x01);
						break;						
					}
                }
				if(GetWpaState(CONNECTED_SERVER))
				{
					wifi_server_led_flash();
				}
                break;

                
            default:
                break;
        }
    } 
}
/*********************************************************************************************************
* Function Name:        UART2_IRQHandler
* Description:          UART2 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART2_IRQHandler (void)
{
	uint8_t i=0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART2->IIR) & 0x01) == 0) 			 /*  �ж��Ƿ����жϹ���   0:���жϹ���       */
	{                              
        switch (IIR & 0x0E)									/*  �ж��жϱ�־                */
		{                                 
        
            case 0x04:                                                  /*  ���������ж�                */
				scon2_time = 100;
				for(i=0;i<8;i++)
				{
                    *scon2_ptr++ = LPC_UART2->RBR;
                    scon2_len++;
					if(scon2_len>UART2_RX_BUF_SIZE)	scon2_ptr=scon2_Rcvbuf;				
				}
                break;
            
            case 0x0C:                                                  /*  �ַ���ʱ�ж�                */
                scon2_time = 100;
                while ((LPC_UART2->LSR & 0x01) == 0x01)                 /*  �ж������Ƿ�������        */ 
				{
                    *scon2_ptr++ = LPC_UART2->RBR;
                    scon2_len++;
					if(scon2_len>UART2_RX_BUF_SIZE)	scon2_ptr=scon2_Rcvbuf;	
                }
                break;
		
            case 0x02:	//THRE�ж�
				stUsartCfg[2].uiSndLen--;
				stUsartCfg[2].uiSndPos++;		
				if(stUsartCfg[2].uiSndLen!=0)
				{    

					LPC_UART2->THR = byUsart2SndBuf[stUsartCfg[2].uiSndPos];			
				}
				else
				{//һ֡���ݷ������
					LPC_UART2->IER  &= (~0X02);		//��ֹ�����ж�				
				}
            	break; 
           
            default:
                break;
        }
    } 
}
/*********************************************************************************************************
* Function Name:        UART3_IRQHandler
* Description:          UART3 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART3_IRQHandler (void)
{

    uint8_t i = 0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART3->IIR) & 0x01) == 0) 			 /*  �ж��Ƿ����жϹ���   0:���жϹ���       */
	{                              
        switch (IIR & 0x0E)								  /*  �ж��жϱ�־                */
		{                                 
        
            case 0x04:                                                  /*  ���������ж�                */
				scon3_time = 100;
				for(i=0;i<8;i++)
				{
                    *scon3_ptr++ = LPC_UART3->RBR;
                    scon3_len++;
					if(scon3_len>UART3_RX_BUF_SIZE)	scon3_ptr=scon3_Rcvbuf;				
				}
	
                break;
            
            case 0x0C:                                                  /*  �ַ���ʱ�ж�                */
                scon3_time = 100;
                while ((LPC_UART3->LSR & 0x01) == 0x01)                 /*  �ж������Ƿ�������        */ 
				{
                    *scon3_ptr++ = LPC_UART3->RBR;
                    scon3_len++;
					if(scon3_len>UART3_RX_BUF_SIZE)	scon3_ptr=scon3_Rcvbuf;
                }
                break;
				
            case 0x02:	//THRE�ж�
				stUsartCfg[3].uiSndLen--;
				stUsartCfg[3].uiSndPos++;		
				if(stUsartCfg[3].uiSndLen!=0)
				{    

					LPC_UART3->THR = byUsart3SndBuf[stUsartCfg[3].uiSndPos];			
				}
				else 
				{//һ֡���ݷ������
					LPC_UART0->IER  &= (~0X02);		//��ֹ�����ж�
					if(GetTelState(bCIDOccupy))
					{
						ClrTelState(bCIDOccupy);
						CID_marks_time = 360;
					}
					else if(GetTelState(bCIDData))
					{//���ݷ�����
						ClrTelState(bCIDData);
						CID_txe_delay= 20;
					}
				}
            	break; 

			
            default:
                break;
        }
    } 
}
/*********************************************************************************************************
* Function Name:        TIMER0_IRQHandler
* Description:          TIMER0 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER0_IRQHandler (void)			 //0.5ms interrupt
{
	uint8_t i;
    LPC_TIM0->IR         = 0x01;  	  //����ж�



	if(read_ds3231_delay)
	{
		read_ds3231_delay--;
		if(read_ds3231_delay == 0)
		{
			SetState(bReadTime);
		}
	}

	if(CID_txe_delay)
	{
		CID_txe_delay--;
		if(CID_txe_delay == 0)
		{
			msm_txe_ctrl(1);		//��ֹ����
		}
	}


	if(CID_marks_time)
	{
		CID_marks_time--;
		if(CID_marks_time == 0)
		{
			ClrTelState(bCIDMarks);
			SetTelState(bCIDData);	//���뷢������״̬

			stUsartCfg[3].uiSndLen = cid_data[1]+3;
			stUsartCfg[3].uiSndPos = 0;
			memcpy(byUsart3SndBuf,cid_data,stUsartCfg[3].uiSndLen);
			LPC_UART3->THR = byUsart3SndBuf[stUsartCfg[3].uiSndPos];
			LPC_UART3->IER   |= 0x02;	//�������ж�				
		}
	}

	//ʹ��AC��36 �����绰����
/*	if(GetTelState(bTelRing))
	{
		if(GetTelState(bTelRingOut))
		{
			tel_ring_time++;
			if(tel_ring_time>=2000)	//��1��
			{
				tel_ring_time =0;
				ClrTelState(bTelRingOut);
				tel_ctrl(DC60V);

				if(GetTelState(bCID))
				{
					ClrTelState(bCID);
					SetTelState(bCID_CMD);
				}
			}
		}
		else
		{
			tel_ring_time++;
//			if(tel_ring_time>=12000)	//���ͣ6��
			if(tel_ring_time>=10000)	//��ʹ��Ϊ5��
			{
				tel_ring_time =0;
				SetTelState(bTelRingOut);
				tel_ctrl(AC36V);
			}			
		}
	}	*/

	//ʹ��AM79R70 оƬ�����绰����
	if(GetTelState(bTelRing))
	{
		if(GetTelState(bTelRingOut))
		{//оƬTRING�Ž�������ߵ͵�ƽ,��1���״̬
			tel_ring_time++;
			if(tel_ring_time<2000)
			{
				if((tel_ring_time%50) ==0)
				{
					if(GetTelState(bPhoneRingState))
					{
						phone_ring_ctrl(0);
						ClrTelState(bPhoneRingState);
					}
					else
					{
						phone_ring_ctrl(1);
						SetTelState(bPhoneRingState);
					}					
				}
			}

			else
			{
				tel_ring_time =0;
				ClrTelState(bTelRingOut);
				tel_ctrl(DC60V);

				phone_ring_ctrl(0);
				ClrTelState(bPhoneRingState);

				if(GetTelState(bCID))
				{
					ClrTelState(bCID);
					SetTelState(bCID_CMD);
				}
			}
		}
		else
		{//ͣ4���״̬
			tel_ring_time++;
//			if(tel_ring_time>=12000)	//���ͣ6��
			if(tel_ring_time>=8000)	//��ʹ��Ϊ4��
			{//�ֿ�ʼ��һ�ֵ�����
				tel_ring_time =0;
				SetTelState(bTelRingOut);
				tel_ctrl(AC36V);

				phone_ring_ctrl(1);
				SetTelState(bPhoneRingState);				
			}			
		}

		if(NULL == (uint8_t *)(stCallPointer.pstHead)) stop_tel_ring();
	}



	if(tel_tlp_chk_delay)
	{
		if(--tel_tlp_chk_delay ==0)
		{
			if(phone_tlp_status ==0)
			{//ժ��
				if(GetTelState(bOriTelTlp) )		//ԭ�ǹһ�״̬
				{
					SetTelState(bTelPickUped);			//�绰ժ��
					ClrTelState(bOriTelTlp);
					
				}
			}
			else 
			{//�һ�
				if(GetTelState(bOriTelTlp)==0)		//ԭ��ժ��״̬
				{
					SetTelState(bTelHungDowned);			//�绰�һ�
					SetTelState(bOriTelTlp);
				}
			}		
		}
	}

	if(voice_times_delay_time)
	{
		if (--voice_times_delay_time == 0)
		{
			voice_play_state |= VOICE_PLAY_NEXT_TIMES;		
		}
	}
	
	if(voice_beds_delay_time)
	{
		if (--voice_beds_delay_time == 0)
		{
			voice_play_state |= VOICE_PLAY_NEXT_BED;		
		}
	}	



	if(check_esp8266_time)
	{
		if(--check_esp8266_time==0)
		{
			SetWpaState(CHECK_ESP8266);
			check_esp8266_time = CHECK_WIFI_TIME;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if(scon0_time)
	{
		scon0_time--;
		if(scon0_time==0)
		{
			scon0_received = scon0_len;
			scon0_time = 0;
			scon0_ptr = scon0_Rcvbuf;
			scon0_len = 0;
			scon0_GucRcvNew = 0xff;
		}
	}	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if(scon1_time)
	{
		scon1_time--;
		if(scon1_time==0)
		{
			scon1_received = scon1_len;
			scon1_time = 0;
			scon1_ptr = scon1_Rcvbuf;
			scon1_len = 0;
			scon1_GucRcvNew = 0xff;

			if(GetWpaState(CONNECTED_SERVER))
			{
				wifi_server_led_ctrl(SERVER_LED_ON);
				SetWpaState(WIFI_SERVER_LED);	
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if(scon2_time)
	{
		scon2_time--;
		if(scon2_time == 0)
		{
			scon2_received = scon2_len;
			scon2_time = 0;
			scon2_ptr = scon2_Rcvbuf;
			scon2_len = 0;
			scon2_GucRcvNew = 0xff;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if(scon3_time)
	{
		scon3_time--;
		if(scon3_time == 0)
		{
			scon3_received = scon3_len;
			scon3_time = 0;
			scon3_ptr = scon3_Rcvbuf;
			scon3_len = 0;
		}
	}
	if(0==GetKeyState(PARA_RST_KEY_PRESS))
	{//��֮ǰû�а���
		if(para_reset_key_status ==0)
		{
			SetKeyState(PARA_RST_KEY_PRESS);	//�ü����±�־
			para_rst_key_time =0;
		}
	}

	if(0==GetKeyState(SOFT_RST_KEY_PRESS))
	{//��֮ǰû�а���
		if(soft_reset_key_status ==0)
		{
			SetKeyState(SOFT_RST_KEY_PRESS);	//�ü����±�־
			soft_rst_key_time =0;
		}
	}	


	if(tel_pickup_time)
	{
		if(--tel_pickup_time == 0)
		{
			voice_play_state |= VOICE_PLAY_HANG_TEL;
		}
	}
}

#define DAC_BIAS_EN         ((uint32_t) (1 << 16))

#define DAC_VALUE(n)        ((uint32_t) ((n & 0x3FF) << 6))


/* Update value to DAC buffer*/
void Chip_DAC_UpdateValue( uint32_t dac_value)
{
	uint32_t tmp;

	tmp = LPC_DAC->DACR & DAC_BIAS_EN;
	tmp |= DAC_VALUE(dac_value);
	/* Update value */
	LPC_DAC->DACR = tmp;
}



/*********************************************************************************************************
* Function Name:        TIMER2_IRQHandler
* Description:          TIMER2 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER2_IRQHandler()		//125us
{


	// LPC_TIM2->TCR  = 0x02;	 //��λ	Ϊ1ʱ����ʱ����������Ԥ��Ƶ������ʹ�ܼ���,Ϊ0ʱ,����������ֹ
	LPC_TIM2->IR         = 0x01; 		  //����ж�


	if(voice_play_state & VOICE_PLAY_START)
	{
		if(cur_play_index > voice_play_seq_cnt)
		{//ȫ�������������

			voice_play_state &= (~VOICE_PLAY_START);
			cur_play_index =0;


		}

		else if(voice_data_len==0)
		{//һ�������������,������ һ�������ı���

			voice_play_state &= (~VOICE_PLAY_START);


			if(voice_data_total_len !=0 )
			{
				voice_play_state |= VOICE_PLAY_READ_EEPROM_DATA;
				//return;
			}
			else
			{
				if(voice_play_state & VOICE_PLAY_ONE_WORD)
				{//ֻ����һ������
					voice_play_state &= (~VOICE_PLAY_ONE_WORD);
				}
				
				else
				{
					cur_play_index++;
					if(cur_play_index >voice_play_seq_cnt)
					{//һ�������������

						cur_play_index =0;
						played_times++;
						if (played_times >= voice_play_times)
						{//�趨�ı��ű������
							
							voice_beds_delay_time = 4000;	//ͣ��2��
							//voice_play_state |= VOICE_PLAY_NEXT_BED;					
						}
						else
						{
							voice_times_delay_time = 2000;	//ͣ��1��
							//voice_play_state |= VOICE_PLAY_NEXT_TIMES;
						}
					}
					else
					{
						//������һ���ֶ�
						voice_play_state |= VOICE_PLAY_NEXT_SEG;
					
					}
				}
			}
		}
		else 
		{//125uSʱ�䵽

			value = *pvoice_data;
			pvoice_data++;

			value/=64;			// ADCΪ10λ��ֻȡ��λ10λ
			
			
			value = value*8/16; //��������	
			
			value += 0x200;
			Chip_DAC_UpdateValue(value);

			voice_data_len--;		
		}
	}



}
/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          �ⲿ�ж�0�жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void EINT0_IRQHandler (void)
{
  //uint32_t	i;
	NVIC_DisableIRQ(EINT0_IRQn);


	LPC_SC->EXTINT |= 0x01;		// ����ⲿ�жϱ�־       
	
	NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_SetPriority(EINT0_IRQn, 8);                                    // �����ⲿ�жϲ�ʹ�� */          
}


#define eint1_fall_trigger()   LPC_SC->EXTPOLAR &=  0xFD;				// �½��ش����ж�
#define eint1_rise_trigger()   LPC_SC->EXTPOLAR |=  0x02;				// �����ش����ж� 

/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          �ⲿ�ж�0�жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void EINT1_IRQHandler (void)	// ��û��
{
	uint32_t i;
	NVIC_DisableIRQ(EINT1_IRQn);
    LPC_SC->EXTINT |= 0x02;			// ����ⲿ�жϱ�־  


	NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_SetPriority(EINT1_IRQn, 1);			// �����ⲿ�жϲ�ʹ�� 
}
/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          �ⲿ�ж�0�жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void EINT2_IRQHandler (void)
{
	NVIC_DisableIRQ(EINT2_IRQn);

    LPC_SC->EXTINT |= 0x04;	// ����ⲿ�жϱ�־     

	mt8870_received = 0;
	if(mt8870_dq4)mt8870_received++;
	mt8870_received<<=1;
	
	if(mt8870_dq3)mt8870_received++;
	mt8870_received<<=1;
	
	if(mt8870_dq2)mt8870_received++;
	mt8870_received<<=1;
	
	if(mt8870_dq1)mt8870_received++;


	NVIC_EnableIRQ(EINT2_IRQn);
    NVIC_SetPriority(EINT2_IRQn, 9);                                    /* �����ⲿ�жϲ�ʹ��           */ 

	
}
/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          �ⲿ�ж�0�жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void EINT3_IRQHandler (void)
{ 
//	NVIC_DisableIRQ(EINT3_IRQn);
	
	eint3Isr();                                              // GPIO�ж�����
//    LPC_SC->EXTINT |= 0x08;	// ����ⲿ�жϱ�־ 

//	NVIC_EnableIRQ(EINT3_IRQn);
//  	NVIC_SetPriority(EINT3_IRQn, 1); // �����ⲿ�жϲ�ʹ��        
}



