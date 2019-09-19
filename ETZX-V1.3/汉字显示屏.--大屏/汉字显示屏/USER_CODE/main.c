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


#define EnableIRQ()  __enable_irq()
#define DisableIRQ() __disable_irq()
/*********************************************************************************************************
  ȫ�ֱ���
*********************************************************************************************************/
unsigned long    GulSysClock     =  12000000UL;                         /* ��¼������ʱ��Ƶ��           */

SYSTEM_INFO SystemInfo;


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
		if (SystemInfo.DisplayInfo.NurseShowLen>screen.width); //��Ҫ�ƶ�
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
	}
	else
	{
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
		if (SystemInfo.DisplayInfo.NurseShowLen>screen.width); //��Ҫ�ƶ�
		else SystemInfo.DisplayInfo.state |= DISPLAY_STATE_CHANGED;
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

#define GetTestMode() (LPC_GPIO0->DATA&(1<<1))
/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��           */
	//����ģʽ�˿ڳ�ʼ��
	LPC_IOCON->PIO0_1 &= ~0x07;		   //P0.1
	//��ʼ������
	LPC_GPIO0->DIR    &= ~(1<<1);                       	/* ��ʼ��Ϊ���빦��     */
}

INT8U daysOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};   //һ�����·ݵ�����

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
	//100ms��ʱ����

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
	SystemInfo.state|=SYSTEM_STATE_MOVING; //ִ֪ͨ���ƶ�
	if (stUartControl.byTimerout>1) stUartControl.byTimerout--;	
}

/*********************************************************************************************************
** Function name:        timer0Init
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
    LPC_TMR32B1->MR0     = SystemFrequency/30000;                       /* ����1S�ж�ʱ��             */
    LPC_TMR32B1->TCR     = 0x01;                                        /* ������ʱ��                   */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);                                    /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER_32_1_IRQn, 3);
}

void SaveSystemInfo()
{
	
}
//��ʾʱ��
void ShowTime(INT8U *buf)
{
	if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)!=0
		||(SystemInfo.DisplayInfo.state&0x1f)==0)
	{
		sprintf(buf,"%02d:%02d:%02d",SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOut(15,8,buf,FONT_ARIAL,24);

		sprintf(buf,"20%02d-%02d-%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day);
		TextOut(32,1,buf,FONT_DEFAULT,7);

		//��ʾϵͳ״̬
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
		//��ʾ������Ϣ
		
		if (SystemInfo.state&SYSTEM_STATE_BUS_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_buserror,8);
		}
		else if (SystemInfo.state&SYSTEM_STATE_LOCAL_ERROR)
		{
			SetArea(120,0,127,7);
			PutPixelDatas((INT8U*)Icon_failure,8);
		}
	}
	else
	{
		sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",SystemInfo.sTime.year,SystemInfo.sTime.month,SystemInfo.sTime.day,SystemInfo.sTime.hour,SystemInfo.sTime.minute,SystemInfo.sTime.second);
		TextOut(10,1,buf,FONT_DEFAULT,7);
		//��ʾϵͳ״̬
		SetArea(0,0,7,7);
		if (SystemInfo.state&SYSTEM_STATE_ONLINE)
			PutPixelDatas((INT8U*)Icon_ok,8);
		else
			PutPixelDatas((INT8U*)Icon_error,8);
	}
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
			sprintf(buf,"��%d������",SystemInfo.DisplayInfo.CallBuf[3]);
		else
			sprintf(buf,"%d������",SystemInfo.DisplayInfo.CallBuf[3]);
		break;
	case 0x07: //��Һ����
		sprintf(buf,"%d����Һ���",SystemInfo.DisplayInfo.CallBuf[3]);
		break;
	case 0x08: //���������
		sprintf(buf,"%d�����������",SystemInfo.DisplayInfo.CallBuf[2]);
		break;	
	}
	len = GetTextShowLength(buf,16);
	size=16;	
	
	if (len>screen.width)
	{
		  SystemInfo.DisplayInfo.CallShowLen = len;
		  if (SystemInfo.DisplayInfo.CallIndex==0)ClsScreenForLine(8,31);
		  TextOut(screen.width-SystemInfo.DisplayInfo.CallIndex,(24-size)/2+8,buf,FONT_ARIAL,size);
		  SystemInfo.DisplayInfo.CallIndex = (SystemInfo.DisplayInfo.CallIndex+1)%(SystemInfo.DisplayInfo.CallShowLen+screen.width);
	}
	else TextOut((screen.width-len)/2,(24-size)/2+8,buf,FONT_ARIAL,size);
}
//�����������
extern void ClearRect(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
//��ʾ��ʿ����Ϣ
void ShowNurseInfo(INT8U * buf)
{
	INT16U len;
	INT8U size,i,*tmp;
	strcpy(buf,"��ʿ��:");
	tmp = buf;
	while(*tmp++);
	tmp--;
	//uartSendByte(*(tmp-2));
	for (i=0;i<20;i++)
	{
		if (SystemInfo.DisplayInfo.NurseBuf[i]!=0)
		{
			sprintf(tmp,"%d,",SystemInfo.DisplayInfo.NurseBuf[i]);
			while(*tmp++);
			tmp--;
		}
		else 
			break;
	}
	
	if (SystemInfo.DisplayInfo.NurseBuf[0]==0) return;
	tmp--;
	sprintf(tmp,"%s","��");
	while(*tmp++);
	tmp--;
	size=16;
	len = 	GetTextShowLength(buf,16);
	
	if (len>screen.width)
	{
		SystemInfo.DisplayInfo.NurseShowLen = len;
		if (SystemInfo.DisplayInfo.NurseIndex==0)ClsScreenForLine(8,31);
		if (screen.width-SystemInfo.DisplayInfo.NurseIndex>0)	ClearRect(0,(24-size)/2+8,screen.width-SystemInfo.DisplayInfo.NurseIndex,(24-size)/2+8+size);
		if (screen.width+len-SystemInfo.DisplayInfo.NurseIndex<screen.width-1)	ClearRect(screen.width+len-SystemInfo.DisplayInfo.NurseIndex,(24-size)/2+8,screen.width-1,(24-size)/2+8+size);
		TextOut(screen.width-SystemInfo.DisplayInfo.NurseIndex,(24-size)/2+8,buf,FONT_ARIAL,size);
		SystemInfo.DisplayInfo.NurseIndex = (SystemInfo.DisplayInfo.NurseIndex+1)%(SystemInfo.DisplayInfo.NurseShowLen+screen.width);
	}
	else
	{ 
		ClearRect(0,(24-size)/2+8,(screen.width-len)/2,(24-size)/2+8+size);
		ClearRect(screen.width-(screen.width-len)/2,(24-size)/2+8,screen.width-1,(24-size)/2+8+size);
		TextOut((screen.width-len)/2,(24-size)/2+8,buf,FONT_ARIAL,size);	
	}
}
//��ʾ������Ϣ
void ShowMessageInfo(INT8U * buf)
{
	if (SystemInfo.DisplayInfo.MessageShowLen==0)
	{
		 SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16);
	}
	if (SystemInfo.DisplayInfo.MessageShowLen>screen.width)
	{
		TextOut(screen.width-SystemInfo.DisplayInfo.MessageIndex,13,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
		SystemInfo.DisplayInfo.MessageIndex = (SystemInfo.DisplayInfo.MessageIndex+1)%(SystemInfo.DisplayInfo.MessageShowLen+screen.width);
	}
	else
	{
		TextOut((screen.width-SystemInfo.DisplayInfo.MessageShowLen)/2,13,SystemInfo.DisplayInfo.MessageBuf,FONT_DEFAULT,16);
	}
}

int main(void)
{
	INT8U i,test;
	INT8U buf[256];
  SystemInit();	                                                    /* ϵͳʱ�ӳ�ʼ��               */
	GPIOInit();
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
  intMasterEnable();                                                  /* ʹ���ж��ܿ���               */
	SingleBusInit();

	//��ȡϵͳ������Ϣ
	eepromRead(0x0, (INT8U*)&SystemInfo, SYSTEM_SAVE_SIZE);                                         /* ��EEPROM 0x100��ַ�������� */
	if (SystemInfo.UartBaud>10)																                                                              
	{
		//��ʼ��ΪĬ��ֵ
		SystemInfo.UartBaud = 9; //Ĭ��Ϊ57600
		SystemInfo.LOCAL_ADDR=0;
		SystemInfo.DisplayInfo.MessageBuf[0]=0;
		eepromWrite((INT8U*)&SystemInfo); //�������
		eepromRead(0x0, (INT8U*)&SystemInfo, SYSTEM_SAVE_SIZE);
	}
	//��ʼ������ 
	UartInit(SystemInfo.UartBaud);

	if (SystemInfo.DisplayInfo.MessageBuf[0]==0) SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_MESSAGE;
	else
	{
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_MESSAGE;
		SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
	}
	i=0;
	while(SystemInfo.DisplayInfo.MessageBuf[i])
	{
		uartSendByte(SystemInfo.DisplayInfo.MessageBuf[i++]);
	}
	
	//��ʼ��ģ��SPI��
	SPI_Init();
	//��ʼ����ʾ����ӿ�
	LED_Init();
	  
	//strcpy(SystemInfo.DisplayInfo.MessageBuf,"������2009������ֵ�����ҵ����ӣ�ȫ����������2000��������������΢��Щ��׳��");
	//SystemInfo.DisplayInfo.MessageShowLen = GetTextShowLength(SystemInfo.DisplayInfo.MessageBuf,16)+screen.width;
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

	SetArea(0,0,127,31);
	PutPixelDatas((INT8U*)Eter_logo,128*32/8);
	Delay_Ms(5000);
	WDTInit();
	ClsScreen();
	SetOperateFrame(1);
	ClsScreen();
	SetOperateFrame(0);
	while (1) {
		//DisableIRQ();                                                   /* ���ж�                       */
    LPC_WDT->FEED  = 0xAA;                                          /* ι��                         */
    LPC_WDT->FEED  = 0x55;
    //EnableIRQ();   
		//��ѯ���߽���״̬
		Bus0Manager();
		//��ѯ�������ݽ���
		UartManager();                                                 /* ���ж�                       */
		
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
				if (test==0) ClsScreen();
				DrawRect(test,test,screen.width-test-1,screen.height-test-1);
				test = (test+1)%(screen.height/2);
			}
		}
		else
		{
			if (SystemInfo.state&SYSTEM_STATE_TESTMODE)
			{
				SystemInfo.state&= ~SYSTEM_STATE_TESTMODE;
				ClsScreen();	
			}

			//�л�����
			if (SystemInfo.DisplayInfo.ChangeDelay>30)
			{
				if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL)==0&&(SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE)==0) //�ں��к�λ����Ϣʱ���л�
				{
					if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING)
					{
						SystemInfo.DisplayInfo.ChangeDelay=0;
						SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //����л�״̬
						SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;						
					}
					else
					{
						if ((SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)!=0&&SystemInfo.DisplayInfo.MessageIndex==0)
						{
							SystemInfo.DisplayInfo.ChangeDelay=0;
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGEING; //����л�״̬
							SystemInfo.DisplayInfo.state|=DISPLAY_STATE_CHANGED;
						}
					}
				}
			}

			//������ߴ���
			if (SystemInfo.error>4)
			{
				if ((SystemInfo.state&SYSTEM_STATE_BUS_ERROR)==0)
				{
					//�����Ƿ�Ϊ��������
					SystemInfo.state|= SYSTEM_STATE_TESTMODE; //��Ϊ����ģʽ���Խ�ֹ���߽�������
					ClrBus0SendPin();
					SetProtectPin();
					Delay_Ms(10);
					if (GetBus0RecPin()==0) //�͵�ƽ��ʾ��������
					{
						SystemInfo.state|=SYSTEM_STATE_BUS_ERROR;  //�ô���״̬
						SystemInfo.state&=~SYSTEM_STATE_ONLINE; //������״̬
						ClrProtectPin();
						Delay_Ms(10);
					}
					else
					{
						SystemInfo.state|=SYSTEM_STATE_BUS_ERROR;  //�ô���״̬
						SystemInfo.state&=~SYSTEM_STATE_ONLINE; //������״̬
						//��������
						SystemInfo.state|=SYSTEM_STATE_LOCAL_ERROR;  //�ô���״̬
					}
					SystemInfo.state&= ~SYSTEM_STATE_TESTMODE;
				}
			}
			else
			{
				if (SystemInfo.state&SYSTEM_STATE_BUS_ERROR)
				{
					SystemInfo.state&=~SYSTEM_STATE_BUS_ERROR;  //�����״̬
					SystemInfo.state&=~SYSTEM_STATE_LOCAL_ERROR;  //�����״̬
					ClrProtectPin();
					//SystemInfo.state&=~SYSTEM_STATE_ONLINE; //������״̬
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
					 i = GetNextDisplayFrame();
					 SetOperateFrame(i);
					 ClsScreen();
					 ShowCallInfo(buf);
					 ShowTime(buf);
					 SetDisplayFrame(i);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //�л�ʿ��λ��Ϣ
				{
					SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_CHANGEING; //����л�״̬
					SystemInfo.DisplayInfo.NurseIndex=0;
					SystemInfo.DisplayInfo.NurseShowLen=0;
					 i = GetNextDisplayFrame();
					 SetOperateFrame(i);
					 ClsScreen();
					 ShowNurseInfo(buf);
					 ShowTime(buf);
					 SetDisplayFrame(i);
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //�л���ʾΪʱ����Ϣ
				{
					i = GetNextDisplayFrame();
					SetOperateFrame(i);
					ClsScreen();
					ShowTime(buf);
					SetDisplayFrame(i);	
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//����Ϣ��ʾ
				{
					SystemInfo.DisplayInfo.MessageIndex=0;
					SystemInfo.DisplayInfo.MessageShowLen=0;
					i = GetNextDisplayFrame();
					SetOperateFrame(i);
					ClsScreen();
					ShowMessageInfo(buf);
					ShowTime(buf);
					SetDisplayFrame(i);
				}
				else
				{
					i = GetNextDisplayFrame();
					SetOperateFrame(i);
					ClsScreen();
					ShowTime(buf);
					SetDisplayFrame(i);
				}
			}
			//������߽���״̬
	
			//���������ƶ�
			if (SystemInfo.state&SYSTEM_STATE_MOVING)
			{
				SystemInfo.state&=~SYSTEM_STATE_MOVING;
				if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CALL) //�к�����Ϣ
				{
					if (SystemInfo.DisplayInfo.CallShowLen>screen.width) //��Ҫ�ƶ�
					{
						ShowCallInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_NURSE) //�л�ʿ��λ��Ϣ
				{
					if (SystemInfo.DisplayInfo.NurseShowLen>screen.width) //��Ҫ�ƶ�
					{
						ShowNurseInfo(buf);
					}
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_CHANGEING) //�л���ʾ��������
				{
				}
				else if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_MESSAGE)//����Ϣ��ʾ
				{
					if (SystemInfo.DisplayInfo.MessageShowLen>screen.width) //��Ҫ�ƶ�
					{
						ShowMessageInfo(buf);
					}
				}
			}
		
			if (SystemInfo.DisplayInfo.state&DISPLAY_STATE_TIME_CHANGED)
			{
				SystemInfo.DisplayInfo.state&=~DISPLAY_STATE_TIME_CHANGED;
				ShowTime(buf);
			}
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
