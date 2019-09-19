#define _IN_INTERRUPT_

#include"custom.h"

uint16_t ms_unit=0;							//毫秒单位，30 000 ms = 30秒
uint16_t sec_unit=0;						//秒单位
uint8_t flag_readtime=0;					//读时间标志

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
* Description:          NMI 中断处理函数
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
* Description:          HardFault_Handler 中断处理函数
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
* Description:          MemManage_Handler 中断处理函数
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
* Description:          UART0 中断处理函数
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART0_IRQHandler (void)
{
    uint8_t i = 0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART0->IIR) & 0x01) == 0) 			 /*  判断是否有中断挂起   0:有中断挂起       */
	{                              
        switch (IIR & 0x0E)									/*  判断中断标志                */
		{                                 
        
            case 0x04:                                                  /*  接收数据中断                */
                scon0_time = 50;
                for (i = 0; i < 8; i++)					/*  连续接收8个字节             */
				{                 
                    *scon0_ptr++ = LPC_UART0->RBR;
                    scon0_len++;
					if(scon0_len>UART0_RX_BUF_SIZE)	scon0_ptr=scon0_Rcvbuf;                	
                }
                break;
            
            case 0x0C:                                                  /*  字符超时中断                */
                scon0_time = 50;
                while ((LPC_UART0->LSR & 0x01) == 0x01)					 /*  判断数据是否接收完毕        */ 
				{               
					*scon0_ptr++ = LPC_UART0->RBR;
					scon0_len++;
					if(scon0_len>UART0_RX_BUF_SIZE)	scon0_ptr=scon0_Rcvbuf;

                }
                break;
            case 0x02:	//THRE中断
				stUsartCfg[0].uiSndLen--;
				stUsartCfg[0].uiSndPos++;
				if(stUsartCfg[0].uiSndLen!=0)
				{
					
					LPC_UART0->THR = byUsart0SndBuf[stUsartCfg[0].uiSndPos];				
					
				}
				else 
				{//一帧数据发送完成
				
					LPC_UART0->IER  &= (~0X02);		//禁止发送中断
					if((byUsart0SndBuf[REC_ROOM_POSITION]== ZLDIS_ADDR2) || (byUsart0SndBuf[REC_ROOM_POSITION]== YHFJ_ADDR2))	//走廊显示屏的数据或医护分机的数据
					{
						byWaitRS485AckDTime = 0;		//广播数据不再等侍
						byUsart0SdResultDTime = 50;		//50*20 = 1000ms		//1000ms后开始发送结果命令
					}
					else
					{
						if(byUsart0SndBuf[DATA4_POSITION]==0x00)	//不是结束帧
						{

							byWaitRS485AckDTime = 0;
							if(byUsart0SndBuf[DATA2_POSITION]==1)
							{
								//第一帧数据要清存储器 ，擦除32K典型18ms
								byUsart0SdResultDTime = 20;	//20*20 = 400ms	 400ms后开始发送结果命令
							}
									
							else byUsart0SdResultDTime = 10;	//10*20 = 200ms  200ms后开始发送结果命令
						}

						else 
						{//是结束帧	
							if(byUsart0SndBuf[CMD_POSITION]==CMD_DISPLAY_FORM)
							{//是发送床头分机格式
								byUsart0SdResultDTime = 10;	//10*20 = 200ms   为分机保存数据而保留时间以免串口接收缓冲区的数据被盖
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
* Description:          UART1 中断处理函数
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART1_IRQHandler (void)
{
	uint8_t i=0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART1->IIR) & 0x01) == 0) 			 /*  判断是否有中断挂起   0:有中断挂起       */
	{                               
        switch (LPC_UART1->IIR & 0x0E)									 /*  判断中断标志                */
		{                                
        
            case 0x04:                                                  /*  接收数据中断                */
				scon1_time = 50;
				for(i=0;i<8;i++)
				{
                    *scon1_ptr++ = LPC_UART1->RBR;
                    scon1_len++;
					if(scon1_len>UART1_RX_BUF_SIZE) 
					{
						scon1_time = 1;			//立即结束
						scon1_len--;
						scon1_ptr=scon1_Rcvbuf;
//						LPC_UART1->IER	&= (~0x01);					//超限禁止接收中断，待处理完后开放
						break;
						
					}
				
				}
				if(GetWpaState(CONNECTED_SERVER))
				{
					wifi_server_led_flash();
				}
                break;
            
            case 0x0C:                                                  /*  字符超时中断                */
                scon1_time = 50;
                while ((LPC_UART1->LSR & 0x01) == 0x01)                 /*  判断数据是否接收完毕        */ 
				{
                    *scon1_ptr++ = LPC_UART1->RBR;
                    scon1_len++;
					if(scon1_len>UART1_RX_BUF_SIZE)
					{
						scon1_time = 1;			//立即结束
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
* Description:          UART2 中断处理函数
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART2_IRQHandler (void)
{
	uint8_t i=0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART2->IIR) & 0x01) == 0) 			 /*  判断是否有中断挂起   0:有中断挂起       */
	{                              
        switch (IIR & 0x0E)									/*  判断中断标志                */
		{                                 
        
            case 0x04:                                                  /*  接收数据中断                */
				scon2_time = 100;
				for(i=0;i<8;i++)
				{
                    *scon2_ptr++ = LPC_UART2->RBR;
                    scon2_len++;
					if(scon2_len>UART2_RX_BUF_SIZE)	scon2_ptr=scon2_Rcvbuf;				
				}
                break;
            
            case 0x0C:                                                  /*  字符超时中断                */
                scon2_time = 100;
                while ((LPC_UART2->LSR & 0x01) == 0x01)                 /*  判断数据是否接收完毕        */ 
				{
                    *scon2_ptr++ = LPC_UART2->RBR;
                    scon2_len++;
					if(scon2_len>UART2_RX_BUF_SIZE)	scon2_ptr=scon2_Rcvbuf;	
                }
                break;
		
            case 0x02:	//THRE中断
				stUsartCfg[2].uiSndLen--;
				stUsartCfg[2].uiSndPos++;		
				if(stUsartCfg[2].uiSndLen!=0)
				{    

					LPC_UART2->THR = byUsart2SndBuf[stUsartCfg[2].uiSndPos];			
				}
				else
				{//一帧数据发送完成
					LPC_UART2->IER  &= (~0X02);		//禁止发送中断				
				}
            	break; 
           
            default:
                break;
        }
    } 
}
/*********************************************************************************************************
* Function Name:        UART3_IRQHandler
* Description:          UART3 中断处理函数
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART3_IRQHandler (void)
{

    uint8_t i = 0;
    uint32_t IIR=0;
    while (((IIR = (uint32_t)LPC_UART3->IIR) & 0x01) == 0) 			 /*  判断是否有中断挂起   0:有中断挂起       */
	{                              
        switch (IIR & 0x0E)								  /*  判断中断标志                */
		{                                 
        
            case 0x04:                                                  /*  接收数据中断                */
				scon3_time = 100;
				for(i=0;i<8;i++)
				{
                    *scon3_ptr++ = LPC_UART3->RBR;
                    scon3_len++;
					if(scon3_len>UART3_RX_BUF_SIZE)	scon3_ptr=scon3_Rcvbuf;				
				}
	
                break;
            
            case 0x0C:                                                  /*  字符超时中断                */
                scon3_time = 100;
                while ((LPC_UART3->LSR & 0x01) == 0x01)                 /*  判断数据是否接收完毕        */ 
				{
                    *scon3_ptr++ = LPC_UART3->RBR;
                    scon3_len++;
					if(scon3_len>UART3_RX_BUF_SIZE)	scon3_ptr=scon3_Rcvbuf;
                }
                break;
				
            case 0x02:	//THRE中断
				stUsartCfg[3].uiSndLen--;
				stUsartCfg[3].uiSndPos++;		
				if(stUsartCfg[3].uiSndLen!=0)
				{    

					LPC_UART3->THR = byUsart3SndBuf[stUsartCfg[3].uiSndPos];			
				}
				else 
				{//一帧数据发送完成
					LPC_UART0->IER  &= (~0X02);		//禁止发送中断
					if(GetTelState(bCIDOccupy))
					{
						ClrTelState(bCIDOccupy);
						CID_marks_time = 360;
					}
					else if(GetTelState(bCIDData))
					{//数据发送完
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
* Description:          TIMER0 中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void TIMER0_IRQHandler (void)			 //0.5ms interrupt
{
	uint8_t i;
    LPC_TIM0->IR         = 0x01;  	  //清除中断



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
			msm_txe_ctrl(1);		//禁止发送
		}
	}


	if(CID_marks_time)
	{
		CID_marks_time--;
		if(CID_marks_time == 0)
		{
			ClrTelState(bCIDMarks);
			SetTelState(bCIDData);	//进入发送数据状态

			stUsartCfg[3].uiSndLen = cid_data[1]+3;
			stUsartCfg[3].uiSndPos = 0;
			memcpy(byUsart3SndBuf,cid_data,stUsartCfg[3].uiSndLen);
			LPC_UART3->THR = byUsart3SndBuf[stUsartCfg[3].uiSndPos];
			LPC_UART3->IER   |= 0x02;	//允许发送中断				
		}
	}

	//使用AC－36 驱动电话振铃
/*	if(GetTelState(bTelRing))
	{
		if(GetTelState(bTelRingOut))
		{
			tel_ring_time++;
			if(tel_ring_time>=2000)	//响1秒
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
//			if(tel_ring_time>=12000)	//最长可停6秒
			if(tel_ring_time>=10000)	//暂使用为5秒
			{
				tel_ring_time =0;
				SetTelState(bTelRingOut);
				tel_ctrl(AC36V);
			}			
		}
	}	*/

	//使用AM79R70 芯片驱动电话振铃
	if(GetTelState(bTelRing))
	{
		if(GetTelState(bTelRingOut))
		{//芯片TRING脚交替输出高低电平,响1秒的状态
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
		{//停4秒的状态
			tel_ring_time++;
//			if(tel_ring_time>=12000)	//最长可停6秒
			if(tel_ring_time>=8000)	//暂使用为4秒
			{//又开始下一轮的振铃
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
			{//摘机
				if(GetTelState(bOriTelTlp) )		//原是挂机状态
				{
					SetTelState(bTelPickUped);			//电话摘机
					ClrTelState(bOriTelTlp);
					
				}
			}
			else 
			{//挂机
				if(GetTelState(bOriTelTlp)==0)		//原是摘机状态
				{
					SetTelState(bTelHungDowned);			//电话挂机
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
	{//键之前没有按下
		if(para_reset_key_status ==0)
		{
			SetKeyState(PARA_RST_KEY_PRESS);	//置键按下标志
			para_rst_key_time =0;
		}
	}

	if(0==GetKeyState(SOFT_RST_KEY_PRESS))
	{//键之前没有按下
		if(soft_reset_key_status ==0)
		{
			SetKeyState(SOFT_RST_KEY_PRESS);	//置键按下标志
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
* Description:          TIMER2 中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void TIMER2_IRQHandler()		//125us
{


	// LPC_TIM2->TCR  = 0x02;	 //复位	为1时，定时器计数器和预分频计数器使能计数,为0时,计数器被禁止
	LPC_TIM2->IR         = 0x01; 		  //清除中断


	if(voice_play_state & VOICE_PLAY_START)
	{
		if(cur_play_index > voice_play_seq_cnt)
		{//全部语音报号完成

			voice_play_state &= (~VOICE_PLAY_START);
			cur_play_index =0;


		}

		else if(voice_data_len==0)
		{//一个语音报号完成,启动下 一个语音的报号

			voice_play_state &= (~VOICE_PLAY_START);


			if(voice_data_total_len !=0 )
			{
				voice_play_state |= VOICE_PLAY_READ_EEPROM_DATA;
				//return;
			}
			else
			{
				if(voice_play_state & VOICE_PLAY_ONE_WORD)
				{//只播放一个单词
					voice_play_state &= (~VOICE_PLAY_ONE_WORD);
				}
				
				else
				{
					cur_play_index++;
					if(cur_play_index >voice_play_seq_cnt)
					{//一句语音报号完成

						cur_play_index =0;
						played_times++;
						if (played_times >= voice_play_times)
						{//设定的报号遍数完成
							
							voice_beds_delay_time = 4000;	//停顿2秒
							//voice_play_state |= VOICE_PLAY_NEXT_BED;					
						}
						else
						{
							voice_times_delay_time = 2000;	//停顿1秒
							//voice_play_state |= VOICE_PLAY_NEXT_TIMES;
						}
					}
					else
					{
						//播报下一个字段
						voice_play_state |= VOICE_PLAY_NEXT_SEG;
					
					}
				}
			}
		}
		else 
		{//125uS时间到

			value = *pvoice_data;
			pvoice_data++;

			value/=64;			// ADC为10位，只取高位10位
			
			
			value = value*8/16; //音量控制	
			
			value += 0x200;
			Chip_DAC_UpdateValue(value);

			voice_data_len--;		
		}
	}



}
/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          外部中断0中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void EINT0_IRQHandler (void)
{
  //uint32_t	i;
	NVIC_DisableIRQ(EINT0_IRQn);


	LPC_SC->EXTINT |= 0x01;		// 清除外部中断标志       
	
	NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_SetPriority(EINT0_IRQn, 8);                                    // 设置外部中断并使能 */          
}


#define eint1_fall_trigger()   LPC_SC->EXTPOLAR &=  0xFD;				// 下降沿触发中断
#define eint1_rise_trigger()   LPC_SC->EXTPOLAR |=  0x02;				// 上升沿触发中断 

/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          外部中断0中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void EINT1_IRQHandler (void)	// 已没用
{
	uint32_t i;
	NVIC_DisableIRQ(EINT1_IRQn);
    LPC_SC->EXTINT |= 0x02;			// 清除外部中断标志  


	NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_SetPriority(EINT1_IRQn, 1);			// 设置外部中断并使能 
}
/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          外部中断0中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void EINT2_IRQHandler (void)
{
	NVIC_DisableIRQ(EINT2_IRQn);

    LPC_SC->EXTINT |= 0x04;	// 清除外部中断标志     

	mt8870_received = 0;
	if(mt8870_dq4)mt8870_received++;
	mt8870_received<<=1;
	
	if(mt8870_dq3)mt8870_received++;
	mt8870_received<<=1;
	
	if(mt8870_dq2)mt8870_received++;
	mt8870_received<<=1;
	
	if(mt8870_dq1)mt8870_received++;


	NVIC_EnableIRQ(EINT2_IRQn);
    NVIC_SetPriority(EINT2_IRQn, 9);                                    /* 设置外部中断并使能           */ 

	
}
/*********************************************************************************************************
* Function Name:        EINT0_IRQHandler
* Description:          外部中断0中断处理函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void EINT3_IRQHandler (void)
{ 
//	NVIC_DisableIRQ(EINT3_IRQn);
	
	eint3Isr();                                              // GPIO中断清零
//    LPC_SC->EXTINT |= 0x08;	// 清除外部中断标志 

//	NVIC_EnableIRQ(EINT3_IRQn);
//  	NVIC_SetPriority(EINT3_IRQn, 1); // 设置外部中断并使能        
}



