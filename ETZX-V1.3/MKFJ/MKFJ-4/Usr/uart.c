/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:uart.c
*�ļ�����    		:������������
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
#define _IN_UART_
#include "config.h"

STUartControl idata stUartControl;							//���ڿ��ƽṹ��
uint8 xdata byUartBuff[UART0_BUF_SIZE];        				//����������Ϣ���з���Ķ��пռ�	
uint8 data byUsartData;										//���ڻ������ݴ�
 
extern STLocalControl 	xdata stLocalControl;
uint8  xdata Page_Color[5]={0x00,0x00,0x00,0x00,0x00};
extern uint8  xdata  Modify_SEC_Name;
uint32 xdata Fbaud;   //�����ʵ���ֵ
uint8  xdata Fbaud_Type;

extern bit bUsartRecFrame;

extern void Delayms(uint16 ms);
/**********************************************************
*��������			:UartInit	
*��������        	:���ڳ�ʼ��
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
void UartInit(void)
{
	//�����ֳ�ʼ��
	stUartControl.byState  = 0x00;
	stUartControl.byRxTail = 0x00; 
	stUartControl.byCrc    = 0x00;
	//����������
	AUXR |= 0x40;											//1Tģʽ 
	SCON = 0x50;
	PCON &= 0x7F;
	TMOD |= 0x20;
	//TH1 = 0xF6;
	//TL1 = 0xF6;												//����������Ϊ57600		
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	//2012/4/9�����ʿɱ�
	if(INIT_FLAG==FmReadByte(Set_Baud_ADDR))
	{
	   Fbaud_Type=FmReadByte(System_Baud_ADDR);
	   switch(Fbaud_Type)
	   {
	     case 1:
		 Fbaud=1200;
		 break;
		 case 2:
		 Fbaud=2400;
		 break;
		 case 3:
		 Fbaud=4800;
		 break;
		 case 4:
		 Fbaud=9600;
		 break;
		 case 5:
		 Fbaud=14400;
		 break;
		 case 6:
		 Fbaud=19200;
		 break;
		 case 7:
		 Fbaud=28800;
		 break;
		 case 8:
		 Fbaud=38400;
		 break;
		 case 9:
		 Fbaud=57600;
		 break;
		 case 10:
		 Fbaud=115200;
		 break;
	   }
	}
	else
	{
	  Fbaud=57600;
	}
	if(Fbaud>2000)
	{
	  TL1=TH1=256-(Fosc/32/Fbaud);
	}
	else
	{
	  AUXR &= 0xBF;
	  TL1=TH1=256-(Fosc/32/Fbaud/12);
	}
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------	
	TR1 = 1;
	ES = 1;	
}
/**********************************************************
*��������			:UartDeal	
*��������        	:���ڽ��յ�һ֡���ݴ�����
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
void UartDeal(void)
{ 	
    //-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	//�����Զ��������
    if(CMD_RS485_CHECK == ((STUartFreq xdata*)byUartBuff)->byCmd)//������Զ��������
	{
	    stLocalControl.stBusDealFreq.byRecSecAddr = CMD_RS485_CHECK;
	    stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	    stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
	    stLocalControl.stBusDealFreq.byCmd = 0x02;
	    stLocalControl.stBusDealFreq.bySndSecAddr  = 0x01;
	    stLocalControl.stBusDealFreq.bySndRoomAddr = 0x02;
	    stLocalControl.stBusDealFreq.bySndBedAddr  = 0x03;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));


		return;
	}
	//-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	if(((STUartFreq xdata*)byUartBuff)->byRecAreaAddr != stLocalControl.stEepromCfgData.bySelfSecAddr)
	{	//��ַ���ǵ������� 		
		return;
	}
	if(((STUartFreq xdata*)byUartBuff)->byRecRoomAddr != stLocalControl.stEepromCfgData.bySelfRoomAddr)
	{	//��ַ���ǵ�������		
		return;
	}
	if(((STUartFreq xdata*)byUartBuff)->byRecBedAddr != stLocalControl.stEepromCfgData.bySelfBedAddr)
	{	//��ַ���ǵ�������			
		return;
	} 	  
    if(CMD_DISPLAY_DATA == ((STUartFreq xdata*)byUartBuff)->byCmd) //���Ϳ���������Ϣ
	{	
		stLocalControl.pbyData = ((STUartFreq xdata*)byUartBuff)->byData;
		//stLocalControl.pbyData++;
		stLocalControl.pbyData+=3;
		if(0x00 != *(stLocalControl.pbyData))	   //...�Ƿ�Ϊ��������
		{	
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;			
			goto ExitDeal;
		}
		stLocalControl.pbyData++;
	    if(0x04 != *(stLocalControl.pbyData))    //...�Ƿ�Ϊ�ſڷֻ�����
		{	
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;			
			goto ExitDeal;
		}
		stLocalControl.pbyData++;
		if(*(stLocalControl.pbyData) > 0)      //...��������Ϊ0
		{	//�����������Ŀ�� 
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;					
			goto ExitDeal;	
		}
		stLocalControl.uiShortType = stLocalControl.byChar1 = *(stLocalControl.pbyData);   //���ŷ���stLocalControl.byChar1��,��ʼ�洢������洢����
		stLocalControl.uiShortType += LCD_SEC_ADDR; 	
		stLocalControl.pbyData ++;
		if(*(stLocalControl.pbyData) > 0)	 //ֻ����һ֡���ݣ�֡������Ϊ0
		{	//�����������Ŀ��
			stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;					
			goto ExitDeal;	
		}
		stLocalControl.byChar2 = *(stLocalControl.pbyData);	  //֡�ŷ���stLocalControl.byChar2��
		
		//������ȷӦ��
		stLocalControl.stBusDealFreq.byRecSecAddr = CMD_DISPLAY_DATA;
		stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
		stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
		stLocalControl.stBusDealFreq.byCmd = 0x02;
		stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
		stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
		stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
		Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));
		   
		if(stLocalControl.byChar2==0x00)                      //֡��Ϊ0��ʾ���͵��ǿ������ơ���������������ɫ����
		{
		  stLocalControl.uiShortType += stLocalControl.byChar2 * (((STUartFreq xdata*)byUartBuff)->byLen-8);	 //�ܹ�41���ֽڴ洢(����14+����4+��ɫ5+����ҽ��8+���λ�ʿ8+����2)
		  stLocalControl.byChar2 = (1 << stLocalControl.byChar1);		
		  stLocalControl.pbyData++;
		  if(*(stLocalControl.pbyData) == 0x01)	//����Ϊ������
		  {
			Modify_SEC_Name	= 0x01;
			FmWriteByte(LCD_SEC_MSK_ADDR,0x01); 
			stLocalControl.byChar2=0x01;
		  }  
		  //����������ȷ��д������
		  stLocalControl.pbyData++;
		  FmWriteBytes(stLocalControl.uiShortType,((STUartFreq xdata*)byUartBuff)->byLen-8,stLocalControl.pbyData);	//��ʼ�洢����������	
	      stLocalControl.uiShortType = LCD_SEC_ADDR;	   //...��ʼ��ȡ��ɫ����
          stLocalControl.uiShortType =stLocalControl.uiShortType+18;
		  Page_Color[0]=FmReadByte(stLocalControl.uiShortType++);//����������ɫ 
	      Page_Color[1]=FmReadByte(stLocalControl.uiShortType++);//�������屳��ɫ,Ҳ������ɫ
	      Page_Color[2]=FmReadByte(stLocalControl.uiShortType++);//�м������ɫ
		  Page_Color[3]=FmReadByte(stLocalControl.uiShortType++);//�������������屳��ɫ
	      Page_Color[4]=FmReadByte(stLocalControl.uiShortType++);//��������������ɫ,Ҳ������ɫ
		  if(stLocalControl.byChar2 == 0x01)
		  {	
		    ReadPageColour();
			SetLcdState(1);	
			ShowPage();	
		  }
		}
		return;  	
	}
	stLocalControl.stBusDealFreq.byCmd = CMD_DATA_ERROR;		
ExitDeal:
	stLocalControl.stBusDealFreq.byRecSecAddr = stLocalControl.stBusDealFreq.bySndSecAddr;
    stLocalControl.stBusDealFreq.byRecRoomAddr = stLocalControl.stBusDealFreq.bySndRoomAddr;
	stLocalControl.stBusDealFreq.byRecBedAddr = stLocalControl.stBusDealFreq.bySndBedAddr;
	stLocalControl.stBusDealFreq.bySndSecAddr = stLocalControl.stEepromCfgData.bySelfSecAddr;
	stLocalControl.stBusDealFreq.bySndRoomAddr = stLocalControl.stEepromCfgData.bySelfRoomAddr;
	stLocalControl.stBusDealFreq.bySndBedAddr = stLocalControl.stEepromCfgData.bySelfBedAddr;
	Bus0OutputData(&(stLocalControl.stBusDealFreq.bySndSecAddr));	
} 
/**********************************************************
*��������			:UartInt	
*��������        	:�����жϺ���
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
#pragma disable
void comm(void) interrupt 4
{
	if(RI == 1)
	{       
		RI = 0;
		//ȡ����������
		byUsartData = SBUF;
		//���ý��ճ�ʱ				
		MakeCH0TimerOut(5, 0);	
		switch(stUartControl.byState)
		{
			/*case 0:											//��ʼ�׶�					
				if(byUsartData == START_BYTE1)
				{	//��ʼ�ַ���ȷ,��ʼ�ַ����洢					
					stUartControl.byState = 1;
					stUartControl.byLen = 7; 
					stUartControl.byRxTail = 0;	
					stUartControl.byCrc = 0;																										
				}
				else
				{
					stUartControl.byRxTail = 0;
				} 			
				break;
			case 1:
				stUartControl.byCrc += byUsartData;	
				byUartBuff[stUartControl.byRxTail++] = byUsartData;			
				if(--stUartControl.byLen == 0x00)
				{	
				    stUartControl.byState = 2;
					stUartControl.byLen = 2;
				}					
				break;
			case 2:	
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.byLen == 0x00)
				{
				  if(MAX_FREQ_DATA_LEN == byUsartData)
				  {	//����ֵ��ȷ
				    stUartControl.byLen = MAX_FREQ_DATA_LEN;
				    stUartControl.byState = 3;
			 	 }
				else
				{	//����ֵ����,�˳�����
				   stUartControl.byState = 0;
				   stUartControl.byRxTail = 0;
				}
			   }					
				break; 				
			case 3:
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.byLen == 0x00) 
				{	//��������������
					stUartControl.byState = 4; 									
				}
				break;
			case 4:
				if(stUartControl.byCrc == byUsartData)
				{	//У�����ȷ
					stUartControl.byState = 5;
				}
				else
				{	//У��ʹ���,�˳�����
					stUartControl.byState = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 5:	
				stUartControl.byState = 0;			
				if(END_BYTE == byUsartData)
				{	//�����ַ���ȷ 						
					OS_INT_ENTER();
					OSQIntPost(byMainCmdQ, USART_REC); 				
					OSIntExit();
				}				
				break;		
		   	default:
				stUartControl.byState = 0;
				stUartControl.byRxTail = 0;
				break;*/
		    //2012-3-14���Ӽ����Զ���⹦��
		 	case 0:											//��ʼ�׶�					
				if(byUsartData == START_BYTE1)
				{	//��ʼ�ַ���ȷ,��ʼ�ַ����洢					
					stUartControl.byState = 1;
					stUartControl.byLen = 7; 
					stUartControl.byRxTail = 0;	
					stUartControl.byCrc = 0;
																															
				}
				else
				{
					stUartControl.byRxTail = 0;
				} 			
				break;
			case 1:
				stUartControl.byCrc += byUsartData;	
				byUartBuff[stUartControl.byRxTail++] = byUsartData;			
				if(--stUartControl.byLen == 0x00)
				{	
				    stUartControl.byState = 2;
					stUartControl.byLen = 2;
					//--------------------------------------------------
				    //--------------------------------------------------
				    //�����Զ���⹦��
                    if(CMD_RS485_CHECK == ((STUartFreq xdata*)byUartBuff)->byCmd)//������Զ��������
				    {
	
				      stUartControl.byState = 0x20;
				      stUartControl.byLen = 7; 
				    }
				   //--------------------------------------------------
				   //--------------------------------------------------
				}					
				break;
			case 0x20:
			  	stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.byLen == 0x00)
				{
				  stUartControl.byState = 4;
				  
			    }
			    break;
			case 2:	
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;	
				
				stUartControl.byLen--;
				if(stUartControl.byLen == 1)
				{	
					stUartControl.uiRecLen = byUsartData<<8; 
				}				
	
				
				else if(stUartControl.byLen == 0x00)
				{
					stUartControl.uiRecLen += byUsartData;
					if(stUartControl.uiRecLen <= MAX_FREQ_DATA_LEN)					
					{	//���ݳ��ȷ���Ҫ��
						stUartControl.byState = 3;
						//stUartControl.uiRecLen += 2; 
					}				

					else
					{	//����ֵ����,�˳�����
					   stUartControl.byState = 0;
					   stUartControl.byRxTail = 0;
					}
			   }					
				break; 				
			case 3:
				stUartControl.byCrc += byUsartData;
				byUartBuff[stUartControl.byRxTail++] = byUsartData;
				if(--stUartControl.uiRecLen == 0x00) 
				{	//��������������
					stUartControl.byState = 4; 									
				}
				break;
			case 4:
				if(stUartControl.byCrc == byUsartData)
				{	//У�����ȷ
					stUartControl.byState = 5;
				}
				else
				{	//У��ʹ���,�˳�����
					stUartControl.byState = 0;
					stUartControl.byRxTail = 0;
				}
				break;
			case 5:	
				stUartControl.byState = 0;		
				if(END_BYTE == byUsartData)
				{	//�����ַ���ȷ 	
					bUsartRecFrame=1;
								     
				}				
				break;		
		   	default:
				stUartControl.byState = 0;
				stUartControl.byRxTail = 0;
				break;							
		}	 
	}
	else
	{
		TI = 0;		
	}
}


void Send_Data(uint8 *Databuf,uint8 xdata l)
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


void Send_Data_Byte(uint8 SendData)
{ 
	uint8 xdata i;
	
	ES=0;
    SBUF=SendData;
    while(!TI);
    TI=0;
	ES=1;
}

