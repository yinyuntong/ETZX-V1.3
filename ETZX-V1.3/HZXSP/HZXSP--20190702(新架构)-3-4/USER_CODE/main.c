/****************************************Copyright (c)****************************************************
����˵��:
�˳���Ϊ������һ��֮��ϵͳ����˫��32*128��ɫ������ʾ���ܣ�����ͨ�����߻����߷�ʽ���ñ�����Ϣ
����ʾʱ��,������Ϣ��������Ϣ��
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"
#include "HZXSP.h"
#include "spi.h"
#include "E2PROM.h"
#include "led.h"
#include "uart.h"
#include "singlebus.h"
#include "CC1101.h"
#include "Util.h"
#include "string.h"
#include "stdio.h"
#include "main.h"


#define EnableIRQ()  __enable_irq()
#define DisableIRQ() __disable_irq()

//extern INT8U DispColor;
uint8_t curDispNusre=0;
/*********************************************************************************************************
  ȫ�ֱ���
*********************************************************************************************************/
unsigned long    GulSysClock     =  12000000UL;                         /* ��¼������ʱ��Ƶ��           */

SYSTEM_INFO SystemInfo;


INT8U daysOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};   //һ�����·ݵ�����

INT8U KeyState=0;	//�������Ű���״̬
INT8U MySec=1;

INT8U ASCII_BCD[4]={0,0,0,0};

INT8U 	KeyDownDelay	=0x00;

INT8U	KeyAlwaysDownDelay = 0x00;	

INT8U  SaveSecAddrDelay =0X00;



INT16U Num;
INT8U buf[256];


extern STBusFreq stBusDealFreq;

//���뻤ʿ��Ϣ����ʿ��λ��Ϣ����
void InsertNurse(INT8U NO)
{
	INT8U i,j,buf[20];
	memset((void*)buf,0,20);
	j=0;
	buf[j++] = NO;
	for (i=0;SystemInfo.DisplayInfo.NurseBuf[i]!=0&&i<19;i++)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[i]!=NO)
		{
			buf[j++]=SystemInfo.DisplayInfo.NurseBuf[i];
		}
	}
	memcpy((void*)SystemInfo.DisplayInfo.NurseBuf,(void*)buf,20);
	if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)
	{
/*		if (SystemInfo.DisplayInfo.NurseShowLen>LED_WIDTH); //��Ҫ�ƶ�
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;*/
	}
	else
	{
		curDispNusre =0;
		SystemInfo.DisplayInfo.state |= DISPLAY_STATE_NURSE;
		SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
	}
}
//ɾ����ʿ��λ��Ϣ
void DeleteNurse(INT8U NO)
{
	INT8U i,j,buf[20];
	memset((void*)buf,0,20);
	j=0;
	for (i=0;SystemInfo.DisplayInfo.NurseBuf[i]!=0&&i<19;i++)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[i]!=NO)
		{
			buf[j++]=SystemInfo.DisplayInfo.NurseBuf[i];
		}
	}
	memcpy((void*)SystemInfo.DisplayInfo.NurseBuf,(void*)buf,20);
	if (buf[0]==0)
	{
		SystemInfo.DisplayInfo.state &= ~DISPLAY_STATE_NURSE;
		SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
	}
	else
	{
/*		if (SystemInfo.DisplayInfo.NurseShowLen>LED_WIDTH); //��Ҫ�ƶ�
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;*/
	}
}

void intMasterEnable(void);                                             /* �����ж�                   */

/*********************************************************************************************************
** Function name:        WDTInit
** Descriptions:        WDT��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void WDTInit (void)
{
    LPC_SYSCON->WDTCLKSEL      = 0x01;                                  /* ѡ��WDTʱ��Դ                */
    LPC_SYSCON->WDTCLKUEN      = 0x00;
    LPC_SYSCON->WDTCLKUEN      = 0x01;                                  /* ����ʹ��                     */
    LPC_SYSCON->WDTCLKDIV      = 0x01;                                  /* ��ƵΪ1                      */
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<15);                               /* ��WDTģ��                  */
    LPC_WDT->TC          = 0xFFFFFF;                                      /* ��ʱʱ��                     */
    LPC_WDT->MOD         = (0x01 << 0)                                  /* ʹ��WDT                      */
                          |(0x01 << 1);                                 /* ʹ��WDT�ж�                  */
    LPC_WDT->FEED        = 0xAA;                                        /* ι��                         */
    LPC_WDT->FEED        = 0x55;
}

/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void KeyGPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��           */
	//����ģʽ�˿ڳ�ʼ��
	LPC_IOCON->PIO0_1 &= ~0x07;		   //P0.1
	//��ʼ������
	LPC_GPIO0->DIR    &= ~(1<<1);                       	/* ��ʼ��Ϊ���빦��     */
}

/**********************************************************
*��������			:PIOINT0_IRQHandler	
*��������        	:KEY�������ڵĶ˿�0�ж�
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:
*��������	 		:
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void PIOINT0_IRQHandler(void)
{
	LPC_GPIO0->IC |= KeyPin;		//��λ�жϱ�־
					          
}



/**********************************************************
*��������			:Timer0Int	
*��������        	:��ʱ��0����ж�,��ʱ��ÿ93.75us�ж�һ��
					 
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
#if 0
void TIMER32_0_IRQHandler (void)
{ 
	static INT16U display_tick=0;
    LPC_TMR32B0->IR = 0x01;                                             /* ����жϱ�־                 */
	//��ʾˢ����ʱ����
	display_tick++;
	if (display_tick>=9) //�����1ms �Ķ�ʱ
	{
		display_tick=0;
	   //������ʾ����
	   ShowNextLine();
	   PutDataToSSP();
	}
	else
	{
		if (first ==0 &&(LPC_SSP0->SR&0x10)==0)
			PutDataToSSP();
		
		else if (screen.DISP_first==1&&(LPC_SSP0->SR&0x10)==0)		//tx FIFOΪ��ʱ��busyλΪ0
		{
			PutDataToSSP();
		}
	}


	if(KeyState & KEY_UP)
	{
		if(GetKeyValue() ==0)
		{
			KeyState &= ~KEY_UP;
			KeyState |= KEY_DOWN_ACT;
			KeyDownDelay = KEY_DOWN_DELAY;
		}
	}

	else if(KeyState & KEY_DOWN_ACT)
	{
		if(KeyDownDelay)
		{
			KeyDownDelay--;
			if(KeyDownDelay==0)
			{
				KeyState &= ~KEY_DOWN_ACT;
				if(GetKeyValue()==0)
				{
					KeyState |= KEY_DOWNED;

					KeyState |= KEY_DOWNED_HANDLER;
					
					KeyAlwaysDownDelay= KEY_ALWAYS_DOWN_DELAY;
					KeyDownDelay  =  KEY_DOWN_DELAY;

					ClsScreen(YELLOW);

				}
				else
				{
					KeyState &= ~(KEY_DOWN_ACT|KEY_DOWNED|KEY_ALWAYS_DOWN);
					KeyState |= KEY_UP;
				}
				#if 0   //δ���жϷ�ʽ
				NVIC_EnableIRQ(EINT0_IRQn);                                    /* �����жϲ�ʹ��               */
				NVIC_SetPriority(EINT0_IRQn, 2);	
				#endif
			}
		}
	}
	else if(KeyState & KEY_DOWNED)
	{
		if(KeyDownDelay)
		{
			KeyDownDelay --;
			if(KeyDownDelay==0)
			{
				
				if(GetKeyValue()==0)
				{
					KeyDownDelay = KEY_DOWN_DELAY;
					if(KeyAlwaysDownDelay)
					{
						KeyAlwaysDownDelay--;
						if(KeyAlwaysDownDelay==0)
						{
							KeyState &= ~ KEY_DOWNED;
							KeyState |= KEY_ALWAYS_DOWN;
							KeyDownDelay = 200;
							KeyAlwaysDownDelay = 25;	//��0.5����
							KeyState |= KEY_ALWAYS_DOWN_HANDLER;

							ClsScreen(YELLOW);
						}
					}
				}
				else
				{
					KeyState &= ~(KEY_DOWN_ACT|KEY_DOWNED|KEY_ALWAYS_DOWN);
					KeyState  |=  KEY_UP;
					
					ClsScreen(YELLOW);
				}
			}
		}
	}

	else if(KeyState & KEY_ALWAYS_DOWN)
	{
		if(KeyDownDelay)
		{
			KeyDownDelay --;
			if(KeyDownDelay==0)
			{
				if(GetKeyValue()==0)
				{
					KeyDownDelay = 200;
					if(KeyAlwaysDownDelay)
					{
						KeyAlwaysDownDelay--;
						if(KeyAlwaysDownDelay==0)
						{

							KeyAlwaysDownDelay = 25;	//��0.5����
							KeyState |= KEY_ALWAYS_DOWN_HANDLER;
							
						}
					}
				}
				else
				{
					KeyState &= ~(KEY_DOWN_ACT|KEY_DOWNED|KEY_ALWAYS_DOWN);
					KeyState  |=  KEY_UP;
					
					ClsScreen(YELLOW);
				}				
			}
		}
	}


}

#endif
/*********************************************************************************************************
* Function Name:        TIMER32_0_IRQHandler
* Description:          TIMER0 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER32_1_IRQHandler (void)
{
	static INT8U stick=0;
    LPC_TMR32B1->IR = 0x01;                                             /* ����жϱ�־                 */
	stick++;
	//33.3ms��ʱ����

	if (stick>=30)	  //�뼶��ʱ
	{
		SystemInfo.error++;
		SystemInfo.DisplayInfo.ChangeDelay++;
		stick=0;
		//ʱ�䴦�� 
		SystemInfo.sTime.second++;
		if(SystemInfo.sTime.second >= 60)
		{
			SystemInfo.sTime.second = 0;
			//��������
			SystemInfo.sTime.minute++;
			if(SystemInfo.sTime.minute >= 60)
			{
				SystemInfo.sTime.minute = 0;
				//Сʱ����
				SystemInfo.sTime.hour++;							
				if(SystemInfo.sTime.hour >= 24)
				{							
					SystemInfo.sTime.hour = 0;
					SystemInfo.sTime.day++;
					if ((SystemInfo.sTime.month==2&&SystemInfo.sTime.year%4==0&&SystemInfo.sTime.day>(daysOfMonth[SystemInfo.sTime.month]+1))||
						(SystemInfo.sTime.day>daysOfMonth[SystemInfo.sTime.month])) 
					{
						SystemInfo.sTime.day = 1;
						SystemInfo.sTime.month++;
						if (SystemInfo.sTime.month>12)
						{
							SystemInfo.sTime.month = 1;
							SystemInfo.sTime.year = (SystemInfo.sTime.year+1)%100;
						}
					}
	
				}
			}
		}
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_TIME_CHANGED; //ʱ���Ѹı�
	}
	
//	if(stick%2==0)
	{
		SystemInfo.state|=SYSTEM_STATE_MOVING; //ִ֪ͨ���ƶ�
	}
	
	if (stUartControl.byTimerout>1) stUartControl.byTimerout--;	

	if(SaveSecAddrDelay)
	{
		SaveSecAddrDelay --;
		if(SaveSecAddrDelay ==0)
		{
			KeyState &= ~FIRST_KEY_HANDLED;
			if(MySec != SystemInfo.LOCAL_ADDR)
			{
				SystemInfo.LOCAL_ADDR = MySec;
				//eepromWrite((INT8U*)&SystemInfo); //�����и��ģ��������ŵ�ַ
				Write_Cont(SYS_INFO_ADD,(INT8U*)&SystemInfo,sizeof(SystemInfo));
			}
			ClsScreen(YELLOW);		
		}
	}	
}

/*********************************************************************************************************
** Function name:        timer0Init
** Descriptions:        32λ��ʱ��0��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void timer0Init (void)
{
	//��ʼ����ʱ��T0 ��Ϊ��ʾɨ��ʹ��
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);								/* �򿪶�ʱ��ģ��				*/

	LPC_TMR32B0->IR 	 = 1;
	LPC_TMR32B0->PR 	 = 1000;										/* ���÷�Ƶϵ�� 				*/
	LPC_TMR32B0->MCR	 = 3;											/* ����MR0ƥ���λTC�������ж�*/
	LPC_TMR32B0->MR0	 = SystemFrequency/10000000;					   /* ����100uS�ж�ʱ�� 			*/
//	LPC_TMR32B0->MR0	 = SystemFrequency/1000000;					    /* ����1mS�ж�ʱ�� 			*/
	LPC_TMR32B0->TCR	 = 0x01;										/* ������ʱ��					*/
	NVIC_EnableIRQ(TIMER_32_0_IRQn);									/* �����жϲ�ʹ��				*/
	NVIC_SetPriority(TIMER_32_0_IRQn, 1);
}


/*********************************************************************************************************
** Function name:        timer1Init
** Descriptions:        32λ��ʱ��1��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void timer1Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);                              /* �򿪶�ʱ��ģ��               */

    LPC_TMR32B1->IR      = 1;
    LPC_TMR32B1->PR      = 1000;                                        /* ���÷�Ƶϵ��                 */
    LPC_TMR32B1->MCR     = 3;                                           /* ����MR0ƥ���λTC�������ж�*/
    LPC_TMR32B1->MR0     = SystemFrequency/30000;                       /* ����33.3mS�ж�ʱ��             */
    LPC_TMR32B1->TCR     = 0x01;                                        /* ������ʱ��                   */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);                                    /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER_32_1_IRQn, 3);
}

void SaveSystemInfo()
{
	
}

void ShowVersion(INT8U *buf)
{
	sprintf(buf,"V%d.%d",VERSION_HI,VERSION_LO);

	TextOutRed(16,0,buf,FONT_BOLD,16);

}


//��ʾʱ��
void ShowTime(INT8U *buf)
{
	if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)!=0
		||(SystemInfo.DisplayInfo.state&0x1f)==0)
	{
		if((KeyState&(KEY_DOWNED|KEY_ALWAYS_DOWN))  || SaveSecAddrDelay)
		{// �м�����״̬�����ϲ���ʾ


		}
		else
		{
			sprintf(buf,"%02d",SystemInfo.sTime.hour);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"ʱ");
			TextOutGreen_DotData(16,0,buf,FONT_DEFAULT,16);	

			sprintf(buf,"%02d",SystemInfo.sTime.minute);
			TextOutRed(32,0,buf,FONT_BOLD,16);
			sprintf(buf,"��");
			TextOutGreen_DotData(48,0,buf,FONT_DEFAULT,16);			

			sprintf(buf,"%02d",SystemInfo.sTime.second);
			TextOutRed(64,0,buf,FONT_BOLD,16);
			sprintf(buf,"��");
			TextOutGreen_DotData(80,0,buf,FONT_DEFAULT,16);	
			

			
/*			sprintf(buf,"%02d  %02d  %02d  ",SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
			ClearRect(0,0,15,1,RED);	//���������ʾ���
			TextOutRed(0,2,buf,FONT_DEFAULT,16);

			sprintf(buf,"  ʱ  ��  ��");
			TextOutGreen(0,0,buf,FONT_DEFAULT,16);	*/		
		}
//		sprintf(buf,"20%02d-%02d-%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day);
//		TextOut(32,1,buf,FONT_DEFAULT,7);

#if 0
		//��ʾϵͳ״̬
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
		//��ʾ������Ϣ
#endif


#if 0
		if (SystemInfo.state&SYSTEM_STATE_BUS_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_buserror,8);
		}
		else if (SystemInfo.state&SYSTEM_STATE_LOCAL_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_failure,8);
#endif

	}
#if 0		
	else
	{
		sprintf(buf,"%02d:%02d:%02d",SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOutRed(16,2,buf,FONT_DEFAULT,16);
	
		sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day,SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOut(10,1,buf,FONT_DEFAULT,7);
		//��ʾϵͳ״̬
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
	
	}
#endif		
}


	
//��ʾ������Ϣ
void ShowCallInfo(INT8U * buf)
{
	INT16U len;INT8U size;
	buf[0]=0;


	switch(SystemInfo.DisplayInfo.CallBuf[0])
	{
	case 0x06: //��ͷ����
		if (SystemInfo.DisplayInfo.CallBuf[2]==250)//�Ӵ�����
		{		
			sprintf(buf,"��");
			TextOutGreen(0,0,buf,FONT_DEFAULT,16);
			if(SystemInfo.DisplayInfo.CallBuf[3]<100)
			{
	            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
				TextOutRed(16,0,buf,FONT_BOLD,16);
				sprintf(buf,"������");
				TextOutGreen(32,0,buf,FONT_DEFAULT,16);
			}
			else
			{
	            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
				TextOutRed(16,0,buf,FONT_BOLD,16);
				sprintf(buf,"������");
				TextOutGreen(40,0,buf,FONT_DEFAULT,16);			
			}
		}
		else if(SystemInfo.DisplayInfo.CallBuf[3] == 0x00)
		{
			//sprintf(buf,"%d���ſں���",SystemInfo.DisplayInfo.CallBuf[2]);
			if(SystemInfo.DisplayInfo.CallBuf[2]<100)
			{
				sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
				TextOutRed(0,0,buf,FONT_BOLD,16);
				sprintf(buf,"���ſں���");
				TextOutGreen(16,0,buf,FONT_DEFAULT,16);
			}
			else
			{
	            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
				TextOutRed(0,0,buf,FONT_BOLD,16);
				sprintf(buf,"�ſں���");
				TextOutGreen(24,0,buf,FONT_DEFAULT,16);				
			}
		}
		
		else 
		{
			//sprintf(buf,"%d������",SystemInfo.DisplayInfo.CallBuf[3]);
			if(SystemInfo.DisplayInfo.CallBuf[2]>=100)
			{
				if(SystemInfo.DisplayInfo.CallBuf[3]>=100)
				{//XXX��xxx����
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"��");
					TextOutGreen(24,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(40,0,buf,FONT_BOLD,16);
					sprintf(buf,"����");
					TextOutGreen(64,0,buf,FONT_DEFAULT,16);						
				}
				else
				{//xxx-xx������
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"-");
					TextOutGreen(24,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(32,0,buf,FONT_BOLD,16);
					sprintf(buf,"������");
					TextOutGreen(48,0,buf,FONT_DEFAULT,16);					
				}
			}
			else
			{
				if(SystemInfo.DisplayInfo.CallBuf[3]>=100)
				{//XX-xxx����
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"-");
					TextOutGreen(16,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(24,0,buf,FONT_BOLD,16);
					sprintf(buf,"������");
					TextOutGreen(48,0,buf,FONT_DEFAULT,16);						
				}
				else
				{//xx��xx������
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
					TextOutRed(0,0,buf,FONT_BOLD,16);
					sprintf(buf,"��");
					TextOutGreen(16,0,buf,FONT_DEFAULT,16);	
		            sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
					TextOutRed(32,0,buf,FONT_BOLD,16);
					sprintf(buf,"������");
					TextOutGreen(48,0,buf,FONT_DEFAULT,16);					
				}				
			}
		}
		break;
	case 0x07: //��Һ����
		//sprintf(buf,"%d����Һ���",SystemInfo.DisplayInfo.CallBuf[3]);
		if(SystemInfo.DisplayInfo.CallBuf[3]<100)
		{
			sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[3]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"����Һ���");
			TextOutGreen(16,0,buf,FONT_DEFAULT,16);
		}
		else
		{
            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[3]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"����Һ��");
			TextOutGreen(24,0,buf,FONT_DEFAULT,16);				
		}
		break;
	case 0x08: //���������
		//sprintf(buf,"%d������",SystemInfo.DisplayInfo.CallBuf[2]);
		if(SystemInfo.DisplayInfo.CallBuf[2]<100)
		{
			sprintf(buf,"%2d",SystemInfo.DisplayInfo.CallBuf[2]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"������ ");
			TextOutGreen(16,0,buf,FONT_DEFAULT,16);
		}
		else
		{
            sprintf(buf,"%3d",SystemInfo.DisplayInfo.CallBuf[2]);
			TextOutRed(0,0,buf,FONT_BOLD,16);
			sprintf(buf,"������");
			TextOutGreen(32,0,buf,FONT_DEFAULT,16);				
		}		
		break;	
	}	
}




void ShowNurseInfo(INT8U *buf)
{
	INT16U len;
	INT8U size,*tmp;

	strcpy(buf,"��ʿλ��");
	tmp = buf;
	while(*tmp++);
	tmp--;	
	
	if(curDispNusre==0)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[curDispNusre]!=0)
		{
			sprintf(tmp,"%02d",SystemInfo.DisplayInfo.NurseBuf[curDispNusre]);
			while(*tmp++);
			tmp--;

			sprintf(tmp,"%s","��");
			while(*tmp++);
			tmp--;

			Num = GetNextDisplayFrame();
			SetOperateFrame(Num);
			ClsScreen(YELLOW);
			TextOutRed(0,0,buf,FONT_DEFAULT,16);
			SetDisplayFrame(Num);	

			curDispNusre++;
		}
		else  return;		
	}

	else
	{
		if (SystemInfo.DisplayInfo.NurseBuf[curDispNusre]!=0)
		{
			sprintf(tmp,"%02d",SystemInfo.DisplayInfo.NurseBuf[curDispNusre]);
			while(*tmp++);
			tmp--;

			sprintf(tmp,"%s","��");
			while(*tmp++);
			tmp--;

			Num = GetNextDisplayFrame();
			SetOperateFrame(Num);
			ClsScreen(YELLOW);
			TextOutRed(0,0,buf,FONT_DEFAULT,16);
			SetDisplayFrame(Num);	

			curDispNusre++;

		}
		else
		{
			curDispNusre=0;
			
			if (SystemInfo.DisplayInfo.NurseBuf[curDispNusre]!=0)
			{
				sprintf(tmp,"%02d",SystemInfo.DisplayInfo.NurseBuf[curDispNusre]);
				while(*tmp++);
				tmp--;
				
				sprintf(tmp,"%s","��");
				while(*tmp++);
				tmp--;
				
				Num = GetNextDisplayFrame();
				SetOperateFrame(Num);
				ClsScreen(YELLOW);
				TextOutRed(0,0,buf,FONT_DEFAULT,16);
				SetDisplayFrame(Num); 
				
				curDispNusre++;

			}
			else return;
		}
	}
	
}



//��ʾ������Ϣ
void ShowMessageInfo(INT8U * buf)
{
	if (SystemInfo.DisplayInfo.MessageShowLen==0)
	{
		 SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16);

	}
	if (SystemInfo.DisplayInfo.MessageShowLen>LED_WIDTH)
	{	
		switch(SystemInfo.DisplayInfo.color)
		{
			case COLOR_RED:
				default:
					TextOutRed(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					break;
			case COLOR_GREEN:
					TextOutGreen(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					break;				
			case COLOR_ORANGE:
					TextOutRed(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					TextOutGreen(LED_WIDTH-SystemInfo.DisplayInfo.MessageIndex,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
					break;				
					
		}
		SystemInfo.DisplayInfo.MessageIndex = (SystemInfo.DisplayInfo.MessageIndex+1)%(SystemInfo.DisplayInfo.MessageShowLen+LED_WIDTH);

	}
	else
	{
		switch(SystemInfo.DisplayInfo.color)
		{
			case COLOR_RED:
			default:
				TextOutRed((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				break;
			case COLOR_GREEN:
				TextOutGreen((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				break;
			case COLOR_ORANGE:
				TextOutRed((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				TextOutGreen((LED_WIDTH-SystemInfo.DisplayInfo.MessageShowLen)/2,0,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);	
				break;				
		}
	}
}


void CC1101_Exint1_Enable(void)
{
	LPC_GPIO1->IE |= cc1101RecPin; //�����ж�
	NVIC_EnableIRQ(EINT1_IRQn); 								   /* �����жϲ�ʹ��			   */
	NVIC_SetPriority(EINT1_IRQn, 2);
}



void hex_convert(INT8U HexData)
{
	INT8U i;

	ASCII_BCD[0]=HexData/100 +0x30;
	ASCII_BCD[1]=(HexData%100)/10 +0x30;
	ASCII_BCD[2]=(HexData%100)%10 +0x30;
	ASCII_BCD[3]=0;



	if(ASCII_BCD[0]==0x30)
	{
		ASCII_BCD[0]= ASCII_BCD[1];
		ASCII_BCD[1]= ASCII_BCD[2];
		ASCII_BCD[2]=0;	
		if(ASCII_BCD[0]==0x30)
		{
			ASCII_BCD[0]= ASCII_BCD[1];
			ASCII_BCD[1]= 0;			
		}		
	}

}

int main(void)
{

	uint8_t i;
	uint32_t addr;
  	SystemInit();	                                                    /* ϵͳʱ�ӳ�ʼ��               */
	KeyGPIOInit();
	KeyState |= KEY_UP;
	memset((void*)&SystemInfo,0,sizeof(SYSTEM_INFO));
	SystemInfo.sTime.year = 12;
	SystemInfo.sTime.month =12;
	SystemInfo.sTime.day = 25;
	SystemInfo.sTime.hour = 14;
	SystemInfo.sTime.minute = 36;
	SystemInfo.sTime.second = 0;
	SystemInfo.DisplayInfo.state =0;
	SystemInfo.DisplayInfo.state |= DISPLAY_STATE_TIME_CHANGED;

	timer1Init();

	//��ʼ��ģ��SPI��
	SPI_Init();
    manID=Read_ID(0x00);
	devID=Read_ID(0X01);

	Flash_Lock();


	//��ȡϵͳ������Ϣ
	Read_Cont((INT8U*)&SystemInfo,SYS_INFO_ADD,SYSTEM_SAVE_SIZE); 
	
	if (SystemInfo.UartBaud>10)																                                                              
	{
		//��ʼ��ΪĬ��ֵ
		SystemInfo.UartBaud = 9; //Ĭ��Ϊ57600
		SystemInfo.LOCAL_ADDR=1;
		SystemInfo.DisplayInfo.MessageBuf[0]=0;
		Write_Cont(SYS_INFO_ADD,(INT8U*)&SystemInfo,sizeof(SystemInfo));
		Read_Cont((INT8U*)&SystemInfo,SYS_INFO_ADD,SYSTEM_SAVE_SIZE);
	}
	MySec = SystemInfo.LOCAL_ADDR;
	//��ʼ������
//	SystemInfo.UartBaud = 9; //Ĭ��Ϊ57600
	UartInit(SystemInfo.UartBaud);
	uartSendByte(0X55);

/*	uartSendByte(manID);
	uartSendByte(devID);
	FRAM_TEST();*/

	if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
	else
	{
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
	}

	
	cc1101Pin_init();
	CC1101_Init();
	

	//��ʼ����ʾ����ӿ�
	LED_Init();
                                         
	//strcpy(SystemInfo.DisplayInfo.MessageBuf,"������2009������ֵ�����ҵ����ӣ�ȫ����������2000��������������΢��Щ��׳��");
	//SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16)+LED_WIDTH;
	//SystemInfo.DisplayInfo.state |= DISPLAY_STATE_MESSAGE;
	/*SystemInfo.DisplayInfo.CallBuf[0]=8;
	SystemInfo.DisplayInfo.CallBuf[1]=1;
	SystemInfo.DisplayInfo.CallBuf[2]=250;
	SystemInfo.DisplayInfo.CallBuf[3]=153;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CALL;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;*/
	//SystemInfo.DisplayInfo.state=0;

	/*SystemInfo.DisplayInfo.NurseBuf[0]=100;
	SystemInfo.DisplayInfo.NurseBuf[1]=10;
	SystemInfo.DisplayInfo.NurseBuf[2]=3;
	SystemInfo.DisplayInfo.NurseBuf[3]=14;
	SystemInfo.DisplayInfo.NurseBuf[4]=0;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_NURSE;
	SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
	*/

	SetArea(0,0,95,15);
//	PutPixelDatas((INT8U*)Eter_logo,128*32/8);
	
	SetOperateFrame(0);
	ClsScreen(YELLOW);	
	SetOperateFrame(1);	
	ClsScreen(YELLOW);
	SetOperateFrame(0);

	intMasterEnable();        /* ʹ���ж��ܿ���               */  
	Delay_Ms(3000);
	SingleBusInit();


	SetOperateFrame(0);
	FullScreen(RED);
	SetDisplayFrame(0);
	Delay_Ms(3000);
	ClsScreen(RED);

	
	SetOperateFrame(0);
	FullScreen(GREEN);
	SetDisplayFrame(0);	
	Delay_Ms(3000);
	ClsScreen(GREEN);


	SetOperateFrame(0);
	ShowVersion(buf);
	SetDisplayFrame(0); 
	Delay_Ms(3000);
	ClsScreen(RED);
/*	SystemInfo.DisplayInfo.MessageBuf[0]=0xaa;
	Write_Cont(SYS_INFO_ADD,SystemInfo.DisplayInfo.MessageBuf,1);
	uartSendByte(Read(SYS_INFO_ADD));*/


#if 0
	SetOperateFrame(0);
	FullScreen(YELLOW);
	SetDisplayFrame(0);	
	Delay_Ms(3000);
	ClsScreen(YELLOW);
#endif


	first=1;


#if 0
	SetOperateFrame(0);
	DrawLine(3,0,46,0,RED);
	SetDisplayFrame(0);	
	Delay_Ms(2000);	
	ClsScreenForLine(0,1,RED);

	SetOperateFrame(0);
	DrawRect(0,0,39,8,GREEN);
	SetDisplayFrame(0);	
	Delay_Ms(2000);	
	ClearRect(0,0,39,8,GREEN);


	SetArea(3,0,10,7);
	SetOperateFrame(0);
	PutPixelDatas((INT8U*)Icon_ok,8,RED);

	SetArea(12,0,19,7);
	SetOperateFrame(0);
	PutPixelDatas((INT8U*)Icon_ok,8,GREEN);	
	SetDisplayFrame(0);
	Delay_Ms(2000);
	ClearRect(3,0,19,7,YELLOW);
#endif


	CC1101_Exint1_Enable();
	halSpiStrobe(CCxxx0_SRX);	//�������

//����
/*	SystemInfo.DisplayInfo.color = GREEN;
	SystemInfo.DisplayInfo.state |=DISPLAY_STATE_MESSAGE;
	sprintf(SystemInfo.DisplayInfo.MessageBuf,"��ף��Һ����´����֣��������⣬����˳��!");
	SystemInfo.DisplayInfo.ChangeDelay=31;*/


	WDTInit();
	while (1) 
	{
		//DisableIRQ();                                                   /* ���ж�                       */
	    LPC_WDT->FEED  = 0xAA;                                         // ι��                     
	    LPC_WDT->FEED  = 0x55;
	    //EnableIRQ();   

		#if 0
		//��ѯ�������ݽ���
		UartManager();                                                
		#endif


		#if 0
		if (!GetTestMode()) //�Ƿ�Ҫ�������ģʽ
		{
			if ((SystemInfo.state&SYSTEM_STATE_TESTMODE)==0)
			{
				SystemInfo.state|= SYSTEM_STATE_TESTMODE;
				test=0;
			}
			if (SystemInfo.state&SYSTEM_STATE_MOVING)
			{
				SystemInfo.state&=~SYSTEM_STATE_MOVING;
				if (test==0) ClsScreen(YELLOW);
				DrawRect(test,test,LED_WIDTH-test-1,LED_HEIGHT-test-1,RED);
				test = (test+1)%(LED_HEIGHT/2);
			}
		}
		else
		#endif
		
		{
			if (SystemInfo.state&SYSTEM_STATE_TESTMODE)
			{
				SystemInfo.state&= ~SYSTEM_STATE_TESTMODE;
				ClsScreen(YELLOW);	
			}

			//�л�����:��̬ʱ��ʱ��������Ϣ������л���
			if (SystemInfo.DisplayInfo.ChangeDelay>MESSAGE_CHANGED_DELAY /*5*/)	//5��
			{
				if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL)==0/*&&(SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)==0*/) //�ں��к�λ����Ϣʱ���л�
				{
					if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)
					{
						SystemInfo.DisplayInfo.ChangeDelay=0;
						SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //����л�״̬
						SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;						
					}
					else
					{
						if((SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)!=0)
						{
							SystemInfo.DisplayInfo.ChangeDelay=MESSAGE_CHANGED_DELAY-1;
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;							
						}
						else if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)!=0&&SystemInfo.DisplayInfo.MessageIndex==0)
						{
							SystemInfo.DisplayInfo.ChangeDelay=0;
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGEING; //���л���ʾ״̬
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
						}
					}
				}
			}

	

			if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGED) //�����ݷ����仯
			{
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGED;
				if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL) //�к�����Ϣ
				{
					SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //����л�״̬
					SystemInfo.DisplayInfo.CallIndex=0;
					SystemInfo.DisplayInfo.CallShowLen=0;
					 Num = GetNextDisplayFrame();
					 SetOperateFrame(Num);
					 ClsScreen(YELLOW);
					 ShowCallInfo(buf);
					 SetDisplayFrame(Num);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //�л�ʿ��λ��Ϣ
				{
					SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //����л�״̬
					SystemInfo.DisplayInfo.NurseIndex=0;
					SystemInfo.DisplayInfo.NurseShowLen=0;
					 ShowNurseInfo(buf);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //�л���ʾΪʱ����Ϣ
				{
/*					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowTime(buf);
					SetDisplayFrame(Num);	
*/
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowMessageInfo(buf);
					SetDisplayFrame(Num);

				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//����Ϣ��ʾ
				{
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowMessageInfo(buf);
					SetDisplayFrame(Num);
					
				}
				else
				{
/*					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowTime(buf);
					SetDisplayFrame(Num);
*/
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					Num = GetNextDisplayFrame();
					SetOperateFrame(Num);
					ClsScreen(YELLOW);
					ShowMessageInfo(buf);
					SetDisplayFrame(Num);

				}
			}


			
			//������߽���״̬
			cc1101Hander();
			Bus0Manager();
			UartManager();

			if(TimerOutDealFlag==1)
			{
				TimerOutDealFlag=0;
				TimerOutDeal();
			}

			if(KeyState	&	KEY_DOWNED_HANDLER)
			{
				KeyState &= ~(KEY_DOWNED_HANDLER);
				SaveSecAddrDelay = SAVE_SEC_ADDR_DELAY;		//�������Ѵ�����ʱ���������1S�����ް����������򱣴�����
				uartSendByte(KeyState);
				if(KeyState & FIRST_KEY_HANDLED)
				{//�е�һ�μ�����
					MySec ++;
					if(MySec==16) MySec=0;
					
				}
				else KeyState |= FIRST_KEY_HANDLED;			//��1�μ����²����Ѵ���

				uartSendByte(KeyState);
				hex_convert(MySec);
				ClearRect(0,0,15,15,RED);
				TextOutRed(0,0,ASCII_BCD,FONT_DEFAULT,8);
			}

			else if(KeyState&(KEY_ALWAYS_DOWN_HANDLER))
			{
				KeyState &= ~(KEY_ALWAYS_DOWN_HANDLER);

				SaveSecAddrDelay = SAVE_SEC_ADDR_DELAY;
				MySec ++;
				if(MySec==16) MySec=0;
				hex_convert(MySec);
				ClearRect(0,0,15,15,RED);
				TextOutRed(0,0,ASCII_BCD,FONT_DEFAULT,8);
			}

			//���������ƶ�
			if (SystemInfo.state&SYSTEM_STATE_MOVING)
			{
				SystemInfo.state&=~SYSTEM_STATE_MOVING;
				if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL) //�к�����Ϣ
				{
					if (SystemInfo.DisplayInfo.CallShowLen>LED_WIDTH) //��Ҫ�ƶ�
					{
						ShowCallInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //�л�ʿ��λ��Ϣ
				{
					if (SystemInfo.DisplayInfo.NurseShowLen>LED_WIDTH) //��Ҫ�ƶ�
					{
						ShowNurseInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //��ʾʱ��״̬�£�������
				{
					if (SystemInfo.DisplayInfo.MessageShowLen>LED_WIDTH) //��Ҫ�ƶ� //������
					{
						ShowMessageInfo(buf);
					}				
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//����Ϣ��ʾ
				{
					if (SystemInfo.DisplayInfo.MessageShowLen>LED_WIDTH) //��Ҫ�ƶ�
					{
						ShowMessageInfo(buf);
					}
				}
			}

		
/*			if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_TIME_CHANGED)
			{
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_TIME_CHANGED;
				ShowTime(buf);
			
			}
*/
		}
    }                                                                   /* End while (1)                */
}

__asm void intMasterEnable() {
    CPSIE   I                                                           /* �����ж�                   */
    BX      LR
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
