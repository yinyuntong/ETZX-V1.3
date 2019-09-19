/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:main.c
*�ļ�����    		:������
*������     		:������
*��������   		:2008-9-22
*�汾��       		:V1.0
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define  _IN_MAIN_
#include "config.h"

uint16	uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
STLocalControl xdata stLocalControl;
uint8 xdata TelNum[TEL_NUM_LEN+1]={0,0,0,0,0,0,0,0,0,0,0,0};


uint8 xdata byRecPosition=0;
uint8 xdata byRecTimeout=0;
uint8 xdata byUsart0RecBuf[UART0_REC_LEN];

bit bUartRecFinished =0;
bit bTimerOutDeal=0;
uint8 xdata RePeat =0;
uint8 xdata RecGsmDataResult=0;			//����GSM���ݵĽ��
/**********************************************************
*��������			:delay_nus	
*��������        	:���뼶��ʱ����
*�������   		:i:��ʱ��us��
*����ֵ				:   	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:����ͬ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void delay_nus(unsigned char i)
{
 	unsigned char j;

 	for(;i>0;i--)
 	{
 		for(j=2;j>0;j--);
 	}
	 
}

/*---------------------------------------------------------------------------
����ԭ��: void Delayms(uint16 ms)
����˵��: ms--��Ҫ��ʱ��ֵ
�� �� ֵ: ��
�������ܣ���ʱ����(����18.432M����ָ��������ʱ1mS)
----------------------------------------------------------------------------*/
void Delayms(uint16 ms)	  
{
   uint16 xdata i;
   for(;ms!=0;ms--)
		for(i=900;i!=0;i--);
}




void UsartInit(void)
{
	   SCON	=	0X50;	   //������ʽ1      8λ�ɱ䲨����
	   
	   TMOD	&=	0X0F;	   //TO��ʽ����
	   TMOD	|=	0X20;	   //;t1 ��ʽ2�������ʷ�����	//8λ�Զ�����
	
	
	   TL1 =   0XF6;	   //������4800  ��18.432M����������
//	    TL1 =   0XFC;	   //������9600  ��18.432M����������	
	   TH1 =   TL1;
//	   PCON    &=  ~SMOD;	//	;�����ʱ���ѡ�� 	   smod=0
		PCON	|=	SMOD;	//	;�����ʱ���ѡ�� 	   smod=1

	// AUXR    &=  0xBf;	   //��ͳ12��Ƶ�ٶ�
	   AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:4800*12*2=115200
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=1;
	   ES=1;   
}



void UsartInit_work(void)
{
	   SCON	=	0X50;	   //������ʽ1      8λ�ɱ䲨����
	   
	   TMOD	&=	0X0F;	   //TO��ʽ����
	   TMOD	|=	0X20;	   //;t1 ��ʽ2�������ʷ�����	//8λ�Զ�����
	
//	   TL1 =   0Xd8;	   //������19200/12   ��18.432M����������	
	    TL1 =   0XF6;	   //������57600/12  ��18.432M����������
//	    TL1 =   0XFC;	   //������115200/12  ��18.432M����������	
	   TH1 =   TL1;
	   PCON    &=  ~SMOD;	//	;�����ʱ���ѡ�� 	   smod=0
	   
	// AUXR    &=  0xBf;	   //��ͳ12��Ƶ�ٶ�
	   AUXR |= T1x12;	   //1Tģʽ
	   TF1=0;
	   TR1=1;
	   
	   RI=0;
	   TI=0;
	   REN=1;
	   ES=1;   
}


void UartISR(void ) interrupt UART_INTNO
{
	uint8 RecData;
	if(RI)
	{
		RI=0;
		RecData=SBUF;
				
		if(byRecPosition!=0) //������ʼ�ֽ�
		{
			if(byRecTimeout==0)
			{//��ʱ����
				byRecPosition=0;
				return;
			}
		}
		byRecTimeout=4;	//�ֿ�ʼ��ʱ80ms     ÿ�ֽ������80ms,��ʱ��Ϊ��֡����
		byUsart0RecBuf[byRecPosition++] = RecData;	
		if(byRecPosition>=UART0_REC_LEN)
		{
			byRecPosition=0;
			byRecTimeout=0;
			memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
		}
	}
	if(TI)
	{
		TI=0;
	}
}


void UartSendByte(uint8 Data)
{ 
    ES=0;
   	SBUF=Data;
   	while(!TI);
	TI=0;
	ES=1;
}


/***********************************************************/
void Send_Data(uint8 *Databuf,uint8 l)
{ 
    uint8 i;
    ES=0;
	for(i=0;i<l;i++)
     {
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;
	 }
	ES=1;
}


void UartSendString(uint8 *Databuf)
{
	ES=0;
	while(*Databuf	!=	0)   //'\0'
	{
	   SBUF=*Databuf;
       while(!TI);
 	   TI=0;
	   Databuf++;		
	}
	ES=1;	
}


void IOInit(void)
{
	//����1���ͽ�����Ϊ��©��������ս�����Ϊ��������
	//P3^3,��������
	P3M0 |= ( Bin(0,0,0,0,1,0,0,0));
	P3M1 &= (~Bin(0,0,0,0,1,0,0,0));


	// P1^3   GSM_RST ��©���
	P1M0 |=  (Bin(0,0,0,0,1,0,0,0));
	P1M1 |=  (Bin(0,0,0,0,1,0,0,0));
	GSM_RST =1;	//����s
	GSM_ON_OFF =1;

	//P1^6,P1^7 �������
	P1M0 &=  (~(Bin(1,1,0,0,0,0,0,0)));
	P1M1 |=    (Bin(1,1,0,0,0,0,0,0));	
	XTA =0;
	XTD =0;
}



//���Σ��ҵ���������byUsart0RecBuf[]�е�λ��+1
uint8 UartRecTreat(uint8* pCmd,uint8 byCmdLen)	
{
	uint8 i,j,k;
	ES=0;
	for(i=0;i<byRecPosition;i++)
	{

		if(byUsart0RecBuf[i]==*pCmd)
		{//��1�����
			if(byCmdLen==1)
			{//����Ϊ1��ֱ���˳�
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;
				return(i+1);	//ʹ֮Ϊһ����Ϊ0����					
			}
			
			if((i+byCmdLen)>byRecPosition)	
			{//����������
				byRecPosition=0;
//;				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;			
				return 0;
			}
			
			//����û�г������ջ�����
			k=1;
			for(j=i+1;j<byCmdLen+i;j++)
			{
				if(byUsart0RecBuf[j]!= *(pCmd+k))	break;	//����ȣ��˳���ѭ��
				k++;
			}
			
			if(j>=byCmdLen+i)
			{//ȫ���������
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;			
				return(i+1);
			}
			//���������һ��λ�ÿ�ʼ����
		}
	}

	byRecPosition=0;
//	memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
	ES=1;
	return 0;	
}



//���̺���������UartRecTreat3����
uint8 UartRecTreat2(uint8* pCmd,uint8 byCmdLen)	
{
	uint8 i,j,k;
	ES=0;
	for(i=0;i<byRecPosition;i++)
	{

		if(byUsart0RecBuf[i]==*pCmd)
		{//��1�����
			if(byCmdLen==1)
			{//����Ϊ1��ֱ���˳�
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;
				return(i+1);	//ʹ֮Ϊһ����Ϊ0����					
			}
			
			if((i+byCmdLen)>byRecPosition)	
			{//����������
//				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
//				ES=1;			
				return 0;
			}
			
			//����û�г������ջ�����
			k=1;
			for(j=i+1;j<byCmdLen+i;j++)
			{
				if(byUsart0RecBuf[j]!= *(pCmd+k))	break;	//����ȣ��˳���ѭ��
				k++;
			}
			
			if(j>=byCmdLen+i)
			{//ȫ���������
				byRecPosition=0;
//				memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
				ES=1;			
				return(i+1);
			}
			//���������һ��λ�ÿ�ʼ����
		}
	}

//	byRecPosition=0;
//	memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
//	ES=1;
	return 0;	
}



uint8 UartRecTreat3(void)		//���յ���һ֡�����в����Ƿ��йؼ��ַ�
{

	ES=0;
	if(UartRecTreat2("+CPIN: READY",(uint8)strlen("+CPIN: READY")/*12*/))
	{
		return CPIN_READY;
	}
	else if(UartRecTreat2("+COPS: 0,0,\"CHINA MOBILE\"",/*(uint8)strlen("+COPS: 0,0,\"CHINA MOBILE\"")*/24))
	{
		return COPS_OK;
	}
	else if(UartRecTreat2("^CONF: 1",(uint8)strlen("^CONF: 1")))
	{
		return GSM_CALL_OK;
	}
	else if(UartRecTreat2("^CONN: 1,0",(uint8)strlen("^CONN: 1,0")))
	{
		return TEL_HANDLE;
	}

	else if(UartRecTreat2("^CEND: 1,0, 67, 21\r\n\r\nNO CARRIER",(uint8)strlen("^CEND: 1,0, 67, 21\r\n\r\nNO CARRIER")))
	{//�ƶ��绰���˺��Լ���Լ17����յ�  ��֡����
		return TEL_IGNORE;
	}

	else if(UartRecTreat2("^CEND: 1,0, 67, 17\r\n\r\nBUSY",(uint8)strlen("^CEND: 1,0, 67, 21\r\n\r\nBUSY")))
	{//�ƶ��绰����æ���������������绰�����򣬻�������ͨ���У�Լ18����յ�  ��֡����
		return TEL_BUSY;
	}
	else if(UartRecTreat2("NO CARRIER",(uint8)strlen("NO CARRIER")))
	{//�绰��֮ͨ��Ĺһ����յ�������Ϊ"^CEND: 1,9,67,16\r\n\r\nNO CARRIER",��ڶ�λ���ֲ�������ʱΪ9����ʱ��Ϊ12����ʱΪ61
//GSM_STATE_LED = LED_ON;
		return TEL_HAND_UP;
	}
	else if(UartRecTreat2("^CEND:1,0,48",(uint8)strlen("^CEND:1,0,48")))
	{
		return TEL_NO_HANDLE;
	}
	else if(UartRecTreat2("OK",(uint8)strlen("OK")))
	{
		return COMM_OK;
	}
	else 
	{
		byRecPosition=0;
		ES=1;
		return 0;
	}
	
}




void PCA0TimerISR(void) interrupt PCA_LVD_INTNO	//20ms��ʱ
{
	if(CCF0 ==1)
	{
		CCF0=0;
		WDT_CONTR = 0x3a; 	//ι��
		uiIsrTimerCount+=ISR_INC_COUNT;
		CCAP0L = (uint8)uiIsrTimerCount;
		CCAP0H = (uint8)(uiIsrTimerCount>>8);	

		bTimerOutDeal =1;

		if(byRecTimeout)
		{
			if(--byRecTimeout ==0)
			{//һ֡���ݽ������
				bUartRecFinished=1;
				//GSM_STATE_LED = LED_ON;
			}
		}
	}
	else if(CCF1 ==1)
	{
		CCF1=0;
	}
	else if(CCF2 ==1)
	{
		CCF2=0;
	}
	else if(CCF3 ==1)
	{
		CCF3=0;
	}

}





/**********************************************************
*��������			:Init	
*��������        	:Ӳ����ʼ������
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
void Init(void)
{
	uint16 i;

/*	char *string1 = "abcdefghijklmnopqrstuvwxyz";
	char *string2 = "onm";
	char *ptr;
	char *str;
*/

//	CMOD = 0x02;	// 2��Ƶ
	CMOD =PCACLK0;	//00 ==PCAʱ��12��Ƶ
	CCON = 0x00;	
	CL = 0x00;
	CH = 0x00;
	CCAP0L = (uint8)uiIsrTimerCount;
	CCAP0H = (uint8)(uiIsrTimerCount>>8);
	//����PCAģ��0Ϊ16λ�����ʱ����ECCF0=1����PCAģ��0�ж�
	CCAPM0 = 0x49;	
	//��PCA�жϺ�LVD(��ѹ���)�жϹ�������жϿ���λ
	EPCA_LVD = 1;
	//����PCA����������
	CR = 1;	

	IOInit();
	GSMInit();
	UsartInit();

/*	while(1)
	{
		Delayms(500);
		Send_Data_Byte(0X55);
	}*/

	//�����߳�ʼ�� 
	SingleBusInit(); 
	
//	//��λ���Ź�
//	WDT_CONTR = 0x3a; 		
	//������ʼ��
	InitParameter();


	stLocalControl.stBusDealFreq.bySndSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr  = stLocalControl.stEepromCfgData.bySelfBedAddr;
	stLocalControl.stBusDealFreq.byCmd         = CMD_LANDING;
	stLocalControl.stBusDealFreq.byRecSecAddr  = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.byRecRoomAddr  = GLZJ_ADDR;
	stLocalControl.stBusDealFreq.byRecBedAddr   = 0x00;
	Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));

	stLocalControl.byLedFlashTime = 50;
	stLocalControl.byLedTimeSet   = 50;

	byDevState1 = 0x00;
	byDevState2 = 0x00;
	byDevState2 = 0x00;



//	bLanding = 1;
	SYS_STATE_LED = LED_ON;
	bLanding = 0;		//���������������ϵ�Ǽ�����
	EA=1;


RE_GSM_UART_AUTOBRT_SET:	
	GSMUartAutoBrtSet();	//��������Ӧ������
	i=100;		//��ʱ2S
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}
	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",2))
		{//��������
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;									
			Delayms(1000);
			goto RE_GSM_UART_AUTOBRT_SET;		
		}		
	}
	else
	{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_UART_AUTOBRT_SET;	
	}	

	
	TR1=0;
 	UsartInit_work();
	Delayms(1000);


RE_AFFIRM_UART_COM:
	AffirmUartCom();
	i=100;		//��ʱ2S
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}
	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",2))
		{//��������
//			GSM_STATE_LED = LED_ON;

		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_AFFIRM_UART_COM;		
		}		
	}
	else
	{
//			GSM_STATE_LED = LED_OFF;	
			Delayms(1000);
			goto RE_AFFIRM_UART_COM;	
	}


RE_LOOP_DISP_OFF:
	LoopDispOff();	//�رջ���
	i=100;		//��ʱ2S
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}
	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",2))
		{//��������
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_LOOP_DISP_OFF;		
		}		
	}
	else
	{
//			GSM_STATE_LED = LED_OFF;	
			Delayms(1000);
			goto RE_LOOP_DISP_OFF;	
	}	
	

RE_SIM_STATE_QUERY:
	SIMStateQuery();
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("+CPIN: READY",(uint8)strlen("+CPIN: READY")/*12*/))
		{//��������
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_SIM_STATE_QUERY;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_SIM_STATE_QUERY;
	}






RE_GSM_SWPATH_SET:
	GSMSWPathSet(0x00);	//��ǰ����ͨ������Ϊ��һ·
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//��������
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_SWPATH_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_GSM_SWPATH_SET;
	}


RE_ECHO_REST_SET:
	EchoRestrainSet(0x01);	//�򿪻������ƹ��� ������˻��
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//��������
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_ECHO_REST_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_ECHO_REST_SET;
	}


RE_GSM_SPK_VOL_SET:
	GSMSPKVoiceSet(0x01);	//����SPK����       5:Ϊ�������
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//��������
//			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_SPK_VOL_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_GSM_SPK_VOL_SET;
	}

RE_GSM_MIC_GAIN_SET:
	GSMMICGainSet(0x00);	//����MIC����       
	i=250;
	while(!bUartRecFinished)
	{
		Delayms(20);
		if(--i==0)  break;
	}

	if(bUartRecFinished)
	{
		bUartRecFinished=0;
		if(UartRecTreat("OK",(uint8)strlen("OK")))
		{//��������
			GSM_STATE_LED = LED_ON;
		}
		else 
		{
//			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_GSM_MIC_GAIN_SET;		
		}
	}
	else 
	{	
//		GSM_STATE_LED = LED_OFF;
		Delayms(1000);
		goto RE_GSM_MIC_GAIN_SET;
	}


RE_NETOPS_QUERY:
		NetOPSQuery();
		i=250;
		while(!bUartRecFinished)
		{
			Delayms(20);
			if(--i==0)	break;
		}
	
		if(bUartRecFinished)
		{
			bUartRecFinished=0;
			if(UartRecTreat("+COPS: 0,0,"/*\"CHINA MOBILE\""*/,(uint8)strlen("+COPS: 0,0,") /*24*/))
			{//����������
				GSM_STATE_LED = LED_ON;
			}
			else 
			{
				GSM_STATE_LED = LED_OFF;
				Delayms(1000);
				goto RE_NETOPS_QUERY;		
			}
		}
		else 
		{	
			GSM_STATE_LED = LED_OFF;
			Delayms(1000);
			goto RE_NETOPS_QUERY;
		}
}



/**********************************************************
*��������			:main	
*��������        	:ϵͳ������,������������
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
void main(void)
{  	
	//ϵͳ��ʼ��			
	Init();	
	UartSendByte(0xAA);
	while(1)
    {
	    if(bBus0RecFinish)
    	{
    		bBus0RecFinish =0;
			Bus0RecDeal();
			
    	}
		if(bBus0SendFinish)
		{
			bBus0SendFinish=0;
			Bus0SendDeal();
		}

		if(bUartRecFinished)
		{

			bUartRecFinished=0;
			RecGsmDataResult = UartRecTreat3();
			stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
			stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
			stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
			stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr; 			
			if(TEL_HANDLE == RecGsmDataResult)
			{//�绰����
				RecGsmDataResult=0;
				UartSendByte(0xbb);
				bDealKeyDown = 1;	
				//�������ָʾ�����ֻ��ĺ���,���������ֻ�����
				if((bIndicatingOther))
				{				
					stLocalControl.stBusDealFreq.byRecSecAddr 	= stLocalControl.stIndicationData.bySndSecAddr;
					stLocalControl.stBusDealFreq.byRecRoomAddr 	= stLocalControl.stIndicationData.bySndRoomAddr;
					stLocalControl.stBusDealFreq.byRecBedAddr 	= stLocalControl.stIndicationData.bySndBedAddr;
					if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
					{	//���������,����Ҫ����,ֱ�����
						stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
					}
					else
					{	//�������ͺ���,����
						if(stLocalControl.stIndicationData.bySndRoomAddr==ADD_BED_FJ_ADDR)
						{//�Ӵ��ֻ�����,����Ҫ����,ֱ�����
							stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
						}
						else
						{
							stLocalControl.stBusDealFreq.byCmd = stLocalControl.stIndicationData.byCmd & 0x1f;
							stLocalControl.stBusDealFreq.byCmd += 0x06;
						}
					} 				
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}				
			}
			else if(TEL_HAND_UP == RecGsmDataResult)
			{//�绰��֮ͨ��ĹҶ�    �յ���NO CARRIER��

				RecGsmDataResult =0;
				//������ڱ����ĺ���,�յ��绰�Ҷ�,��������ĺ���	
/*				if(bConfusionNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bServiceNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bHelpNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_HELP_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else if(bEmergencyNoting)
				{
					stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
					Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
				}
				else */
				{

					GSM_STATE_LED= LED_OFF;
					bDealKeyDown = 0; 								//���洦�ü���״̬	  			
					if((bWaitAck|bWaitListen|bChannel0Talk))
					{	//ͨ��0�������л���ͨ��״̬,���ٸ�ͨ����ʱʱ��				
						MakeCH0TimerOut(5, 0);
					}
					else if(bChannel1Talk)
					{	//ͨ��1����ͨ��״̬,���ٸ�ͨ����ʱʱ��
						MakeCH1TimerOut(5, 0);
					}	

					else if(( bIndicatingOther))
					{	
						//GSM_STATE_LED= LED_OFF;
						 bIndicatingOther =0;
						stLocalControl.stBusDealFreq.byRecSecAddr 	= stLocalControl.stIndicationData.bySndSecAddr;
						stLocalControl.stBusDealFreq.byRecRoomAddr 	= stLocalControl.stIndicationData.bySndRoomAddr;
						stLocalControl.stBusDealFreq.byRecBedAddr 	= stLocalControl.stIndicationData.bySndBedAddr;
						if(CMD_EMERGENCY_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
						{	//���������,����Ҫ����,ֱ�����
							stLocalControl.stBusDealFreq.byCmd = CMD_EMERGENCY_CLEAR;
						}
						else if(CMD_INFUSION_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
						{		//��Һ����
							stLocalControl.stBusDealFreq.byCmd = CMD_INFUSION_CLEAR;
						}								
						else if(CMD_SERVICE_CALL == (stLocalControl.stIndicationData.byCmd & 0x1f))
						{		//��Һ����
							stLocalControl.stBusDealFreq.byCmd = CMD_SERVICE_CLEAR;
						}					
						Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
					}
				}

			}
//			byRecPosition=0;			
//			memset(byUsart0RecBuf,0x00,UART0_REC_LEN);
		}

		if(bTimerOutDeal)
		{
			bTimerOutDeal=0;
			TimerOutDeal();
		}
    }	 
}
