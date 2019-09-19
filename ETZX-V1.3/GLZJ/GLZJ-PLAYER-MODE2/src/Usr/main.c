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

uint16	xdata uiIsrTimerCount = ISR_INC_COUNT;					//ϵͳʱ�Ӷ�ʱֵ
uint8	OS_Q_MEM_SEL	byMainCmdQ[16];						//���߳���Ϣ����
STLocalControl 	xdata stLocalControl;						//ȫ�ֱ����ṹ��

bit bComRecFrame =0;

uint8 data volumn =0;
#define COM_DEAL_BUFF_ZISE 32+1
#define COM_FREQ_SIZE		10

uint8 xdata ComRecBuff[COM_FREQ_SIZE];
uint8 xdata ComRecBuff1[COM_FREQ_SIZE];
uint8 xdata ComRecPost=0;

uint32	xdata SST25VFAddr;
int32  xdata VoiceDataLen;

uint8 xdata i;

void UartInt(void ) interrupt UART_INTNO
{
	uint8 byRecData;
	if(RI==1)
	{
		RI=0;

	}
	else if(TI==1)
	{
		TI=0;
	}
}

/**********************************************************
*��������			:delay_nms	
*��������        	:���뼶��ʱ����
*�������   		:i:��ʱ��ms��
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
void delay_nms(uint16 ms)
{
   uint16 xdata i;
   for(;ms>0;ms--)
   {
		for(i=900;i>0;i--);
   }
}


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
void delay_nus(uint16 i)
{
 	uint8 xdata j;

 	for(;i>0;i--)
 	{
 		for(j=2;j>0;j--);
 	}
	 
}



void UsartInit(void)
{
	SCON=0X50;	   //������ʽ1
	TMOD&=0X0F;	   //TO��ʽ����
	TMOD|=0X20;	   //;t1 ��ʽ2�������ʷ�����

//	TL1 =   0XD8;	   //������1200  ��18.432M����������
	TL1 =   0XF6;	   //������4800  ��18.432M����������
//	TL1 =   0XFB;	   //������9600  ��18.432M����������
	TH1 =   TL1;
	PCON    &=  ~SMOD;	//	;�����ʱ���ѡ�� 	   smod=0
	// AUXR    &=  0xBf;	   //��ͳ12��Ƶ�ٶ�
	AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:4800*12=57.6K
//	AUXR |= T1x12;	   //1Tģʽ,�������Ϊ:9600*12=115.200K


	TF1=0;
	TR1=1;

	RI=0;
	TI=0;
	REN=1;
	ES=1;   
}
/***********************************************************/

void Send_Data(uint8 *Databuf,uint8 l)
{ 
    uint8 xdata i;
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


void Send_Data_Byte(uint8 SndData)
{ 
    ES=0;
	
	SBUF=SndData;
	while(!TI);
	TI=0;
	
	ES=1;
}


/** Output a character to console */
#define ConsolePutChar(c){SBUF = (c);while (!TI);TI = 0; }



/** Write a Code Constant string to console */
void ConsoleWrite(char code *str){
  ES=0;
  while (*str)
    {
      ConsolePutChar (*str++);
    }
  ES=1;
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
	Mp3PutInReset();
	
	CMOD = 0x02;
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

	//��������0
	memset(&(stLocalControl.byChar1),0x00,sizeof(STLocalControl));

	//���ڳ�ʼ��
	UsartInit();


	//SPI��ʼ��
	Vs1003SpiInit();

	//SST25VF020��ʼ��
	SST25VF_Init();

	//����
//	FRAM_TEST();

	//MP3��λ
	VS1003Init();
	
	
	//��λ���Ź�
//	WDT_CONTR = 0x3a; 	

	//������ʼ��
	InitParameter();

	//�����߳�ʼ�� 
	SingleBusInit(); 	

	MakeCH0TimerOut(0, 0);		
}
/**********************************************************
*��������			:GetMessage	
*��������        	:��ȡ���߳���Ϣ�����е���Ϣ(����Ϣ���������)
*�������   		:Msg:�洢��Ϣָ��
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
void GetMessage(uint8 data *Msg)
{
	OSQPend(Msg, byMainCmdQ, 0);
}
/**********************************************************
*��������			:DispatchMessage	
*��������        	:�ַ������ȡ�����߳���Ϣ
*�������   		:Msg:��Ϣ,��3λ����������,��5λ����������
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
void DispatchMessage(uint8 Msg)
{
	switch(Msg&0xe0)
	{
		case BUS0_REC:									//����0�յ�����֡
			Bus0RecDeal();
			break;
		case BUS0_SND:	 								//����0���ݷ������
			Bus0SendDeal();
			break;		
		case KEY_DOWN: 									//�������´���					
			KeyDownDeal(Msg & 0x1f);
			break;
		case KEY_UP:									//����������
			KeyUpDeal(Msg & 0x1f);
			break;
		case TIMER_OUT:	 								//��ʱ����
			TimerOutDeal();					
			break;
	}	
}
/**********************************************************
*��������			:MainTask	
*��������        	:ϵͳ���߳�,��������ϵͳ����Ϣ����
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
void MainTask(void)
{
	static uint8 data Msg;  
	
	//ϵͳӲ����ʼ��
	Init();
	//�������߳���Ϣ����	
	OSQCreate(byMainCmdQ, 16);
	//����Ϊ���������߳�	
	OSTaskCreate(Bus0Manage, NULL, 1);	
	OSTaskCreate(KeyManager, NULL, 2);
	OSTaskCreate(TimerOutManager, NULL, 3);
	OSTaskCreate(PlayManager, NULL, 4);	
	
	//������Ϣѭ��
	while(TRUE)
	{
		GetMessage(&Msg);
		DispatchMessage(Msg);
/*
		while(1)
		{
			SST25VFAddr =0x00ad3d;
			while(1)
			{
				Read_Cont(SST25VFAddr,32,FRAM_BUFF);
				while(!MP3_DREQ);
		//		{
		//			OSWait(K_TMO,1);	
		//		}
				Mp3SelectData();	  // Now SPI writes go to SDI port 
				for(i=0;i<32;i++)
				{
					SPI_WriteByte(FRAM_BUFF[i]);
				}
				Mp3DeselectData();
				
				SST25VFAddr +=32;
				if(	SST25VFAddr > 0x0000e31d) break;
			}


			SST25VFAddr =0x0003b831;
			while(1)
			{
				Read_Cont(SST25VFAddr,32,FRAM_BUFF);
				while(!MP3_DREQ);
		//		{
		//			OSWait(K_TMO,1);	
		//		}
				Mp3SelectData();	  // Now SPI writes go to SDI port 
				for(i=0;i<32;i++)
				{
					SPI_WriteByte(FRAM_BUFF[i]);
				}
				Mp3DeselectData();
				
				SST25VFAddr +=32;
				if( SST25VFAddr > 0x0003e789) break;
			}


			SST25VFAddr =0x00031008;
			while(1)
			{
				Read_Cont(SST25VFAddr,32,FRAM_BUFF);
				while(!MP3_DREQ);
		//		{
		//			OSWait(K_TMO,1);	
		//		}
				Mp3SelectData();	  // Now SPI writes go to SDI port 
				for(i=0;i<32;i++)
				{
					SPI_WriteByte(FRAM_BUFF[i]);
				}
				Mp3DeselectData();
				
				SST25VFAddr +=32;
				if( SST25VFAddr > 0x00034c6f) break;
			}
			
		}
*/		
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
	//����ϵͳ��ʼ��			
	OSInit();	
	//����ϵͳ���߳�
	OSTaskCreate(MainTask, NULL, 0); 	
	while(1)
    {
       PCON = PCON | 0x01;                    //CPU��������״̬	    
    }	 
}

