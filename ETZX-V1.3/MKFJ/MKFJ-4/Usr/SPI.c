/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SPI.c
*�ļ�����    		:SPI������������
*������     		:�ܼ�ǿ
*��������   		:2008-9-22
*�汾��       		:V1.1
*ע��	     		:					
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#define _IN_SPI_
#include "config.h"

extern STLocalControl 	xdata stLocalControl;

uint8    xdata  GT23_Rec_Data[128];
uint8    xdata  GT23_Rec_Length;
uint16   xdata  GT23_16LAddress;
uint16   xdata  GT23_16HAddress;
uint32   xdata  GT23_32Address; 
extern uint8   xdata SerchRoom;
extern bit     Page_Flag;
uint8  xdata   Modify_Room_Number=0;
uint8  xdata   Modify_SEC_Name=0;
extern uint8 xdata  Page_Color[5];
uint8  xdata  ID_Number=0;
uint8  xdata  ID_Number_Flag=0;

uint8 xdata 
FRAM_BUFF[12];

void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes);
extern void Delayms(uint16 ms);

/**********************************************************
*��������			:Delay_nus	
*��������        	:��ʱ����
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Delay_nus(uint8 xdata i)
{
 	uint8 xdata j;
 	for(;i>0;i--)
 	{
 	  for(j=2;j>0;j--);
 	}
}
/**********************************************************
*��������			:SpiTransferByte	
*��������        	:SPI���ߴ���һ���ֽ�����,�����ض�����һ���ֽ�����
*�������   		:byData:�����������
*����ֵ				:SPDAT:��ȡ�������� 	
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
uint8 SpiTransferByte(uint8 byData)
{
	SPDAT = byData;
	while((SPSTAT&0x80) == 0x00);				//�ȴ��������
	SPSTAT = 0xC0;
	return(SPDAT);
}
uint8 READ_ID(void)
{
   	uint8 byRet;
	SST_CS = 0;	 	
	SpiTransferByte(SST_READ_ID); 		    //�����дID������
	SpiTransferByte(0x00);	
	SpiTransferByte(0x00);
	SpiTransferByte(0x01);	
	byRet = SpiTransferByte(0xFF); 	       //����ID������ 	
	SST_CS = 1;		
	return(byRet);
}
/**********************************************************
*��������			:SpiInit	
*��������        	:SPI�ӿڳ�ʼ��
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
void SpiInit(void)
{
	P4M0  &= 0xDD;
	P4M1  &= 0xDD;
	SPCTL  = 0xDC;											//����SS����,ʹ��SPI�ӿ�,��λ��ǰ,Fosc/4	
	SPSTAT = 0xC0;											//�����־ 
	SST_CS=0;
	SpiTransferByte(SST_EWSR);
	SST_CS=1;
	SST_CS=0;
	SpiTransferByte(SST_WRSR);		
	SpiTransferByte(0x00);		
	SST_CS=1;
	ID_Number=READ_ID();
	if(ID_Number==0x8C)      //PCT
	ID_Number_Flag=1;
	else if(ID_Number==0x43) //SST
	ID_Number_Flag=2;
}
/**********************************************************
*��������			:Read_Status_Register	
*��������        	:��ȡSST�洢��״̬�Ĵ����е�����
*�������   		:
*����ֵ				:byRet:��ȡ�������� 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
uint8 Read_Status_Register(void)
{
	uint8 byRet;
	SST_CS=0;				
	SpiTransferByte(0x05);		
	byRet = SpiTransferByte(0xFF); 	
	SST_CS=1;				
	return byRet;
}
/**********************************************************
*��������			:Wait_Busy	
*��������        	:�ȴ�д���ݽ���
*�������   		:
*����ֵ				: 	
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Wait_Busy(void)
{
   while((Read_Status_Register()&0x01) == 0x01)
   {
     Delay_nus(10);
   }
}
/**********************************************************
*��������			:FmReadByte	
*��������        	:��ȡSST�洢��ָ����ַ��һ���ֽ�����
*�������   		:uiAddr:��ַ
*����ֵ				:byRet:��ȡ�������� 	
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
uint8 FmReadByte(uint32 uiAddr)
{
	uint8 byRet;
	SST_CS = 0;	 	
	SpiTransferByte(SST_READ); 		        //���������
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);	
	byRet = SpiTransferByte(0xFF); 	       //�������� 	
	SST_CS = 1;		
	return(byRet);
}

/************************************************************************/
/* PROCEDURE:	Read_Cont												*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*          SST25VF020:Dst:	Destination Address 000000H - 03FFFFH              	*/
/*      no_bytes	Number of bytes to read	(max = 128)					*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes)
{
	unsigned long i = 0;
	SST_CS = 0;				/* enable device */
	SpiTransferByte(0x03); 			/* read command */
	SpiTransferByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SpiTransferByte(((Dst & 0xFFFF) >> 8));
	SpiTransferByte(Dst & 0xFF);
	for (i = 0; i < no_bytes; i++)		/* read until no_bytes is reached */
	{
		pBuffer[i] = SpiTransferByte(0xFF);	/* receive byte and store at address 80H - FFH */
	}
	SST_CS = 1;					/* disable device */

}

/**********************************************************
*��������			:FmWriteByte	
*��������        	:��SST�洢��ָ����ַд��һ���ֽ�����
*�������   		:uiAddr:��ַ,byData:��д�������
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
void FmWriteByte(uint32 uiAddr, uint8 byData)
{ 
    SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //����дʹ������
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_SECTOR_EARSE); //�����������	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);			
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //����дʹ������
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;	 	
	SpiTransferByte(SST_WRITE);       //����д����	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);		
	SpiTransferByte(byData); 	      //д������  	
	SST_CS = 1;
	Wait_Busy();
}


void FmWriteByte1(uint32 uiAddr, uint8 byData)
{ 

	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //����дʹ������
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;	 	
	SpiTransferByte(SST_WRITE);       //����д����	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));	
	SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
	SpiTransferByte(uiAddr & 0xFF);		
	SpiTransferByte(byData); 	      //д������  	
	SST_CS = 1;
	Wait_Busy();
}


/**********************************************************
*��������			:FmWriteBytes	
*��������        	:��SST�洢��ָ����ַ�����ֽ�����
*�������   		:uiAddr:��ַ,uiLen:��д������ݳ���,pbyData:��д�������׵�ַ
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
#if 1
void FmWriteBytes(uint32 uiAddr, uint16 uiLen, uint8* pbyData)
{
	if(0 == uiLen)
	{
		return;
	} 
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //����дʹ������
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_SECTOR_EARSE); //�����������	
	SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));                            /* ����3���ֽڵĵ�ַ��Ϣ        */
    SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
    SpiTransferByte(uiAddr & 0xFF);		
	SST_CS = 1;
	Wait_Busy();
	while (uiLen--)
	{
		SST_CS = 0;
        SpiTransferByte(SST_WREN);                                                    /* ����дʹ������               */
        SST_CS = 1;
    	Wait_Busy();
        SST_CS = 0;
        SpiTransferByte(0x02);                                                    /* �����ֽ�������д����         */
        SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));                            /* ����3���ֽڵĵ�ַ��Ϣ        */
        SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
        SpiTransferByte(uiAddr & 0xFF);
        SpiTransferByte(*pbyData++);                                             /* ���ͱ���д������             */
		uiAddr++;
        SST_CS = 1;
    	Wait_Busy();
    }
	SST_CS=0;
	SpiTransferByte(SST_WRDI);		
	SST_CS = 1;  
	Wait_Busy();
}
#else
void FmWriteBytes(uint32 uiAddr, uint16 uiLen, uint8* pbyData)
{
	uint16  xdata highaddr;
	uint16  xdata lowaddr;	
	highaddr = uiAddr/65536;
    lowaddr  = uiAddr%65536;  
	if(0 == uiLen)
	{
		return;
	} 
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //����дʹ������
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_SECTOR_EARSE); //�����������	
	SpiTransferByte(highaddr%256);
	SpiTransferByte(lowaddr/256);
	SpiTransferByte(lowaddr%256);		
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	SpiTransferByte(SST_WREN);	      //����дʹ������
	SST_CS = 1;
	Wait_Busy();
	SST_CS = 0;
	if(ID_Number_Flag==0x01)
	SpiTransferByte(SST_AAI_P);         //�����ַ�Զ��������� 
	else if(ID_Number_Flag==0x02)
	SpiTransferByte(SST_AAI_S);	
	SpiTransferByte(highaddr%256);
	SpiTransferByte(lowaddr/256);
	SpiTransferByte(lowaddr%256);		
	SpiTransferByte(*pbyData);
	if(ID_Number_Flag==0x01)
	{
	  pbyData++;
	  SpiTransferByte(*pbyData);
	}
	SST_CS=1;
	Wait_Busy();
	uiLen--;
    if(ID_Number_Flag==0x01)
	uiLen--;
    pbyData++;
	for(;uiLen>0;)				      //д������ 
	{	
		SST_CS=0;
	    if(ID_Number_Flag==0x01)
		SpiTransferByte(SST_AAI_P);     //�����ַ�Զ��������� 
		else if(ID_Number_Flag==0x02)
		SpiTransferByte(SST_AAI_S);
		SpiTransferByte(*pbyData);
	    if(ID_Number_Flag==0x01)
		{
		  pbyData++;
		  SpiTransferByte(*pbyData);
		}
		SST_CS=1;
		Wait_Busy();
		uiLen--;
	    if(ID_Number_Flag==0x01)
		uiLen--;
		pbyData++;
	}	
	SST_CS=0;
	SpiTransferByte(SST_WRDI);		
	SST_CS = 1;  
	Wait_Busy();
}
#endif


void FmWriteBytes1(uint32 uiAddr, uint16 uiLen, uint8* pbyData)
{
	if(0 == uiLen)
	{
		return;
	} 
 
	while (uiLen--)
	{
		SST_CS = 0;
        SpiTransferByte(SST_WREN);                                                    /* ����дʹ������               */
        SST_CS = 1;
    	Wait_Busy();
        SST_CS = 0;
        SpiTransferByte(0x02);                                                    /* �����ֽ�������д����         */
        SpiTransferByte(((uiAddr & 0xFFFFFF) >> 16));                            /* ����3���ֽڵĵ�ַ��Ϣ        */
        SpiTransferByte(((uiAddr & 0xFFFF) >> 8));
        SpiTransferByte(uiAddr & 0xFF);
        SpiTransferByte(*pbyData++);                                             /* ���ͱ���д������             */
		uiAddr++;
        SST_CS = 1;
    	Wait_Busy();
    }
	SST_CS=0;
	SpiTransferByte(SST_WRDI);		
	SST_CS = 1;  
	Wait_Busy();
}

/**********************************************************
*��������			:ReadParameter	
*��������        	:��ȡ���ò���
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
void ReadParameter(void)
{
	stLocalControl.uiShortType = IAP_ADDR;
	stLocalControl.pbyData= & (stLocalControl.stEepromCfgData.bySelfSecAddr);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	EnableIAP(IAP_READ); 		
	while(stLocalControl.byChar1--)
	{ 		
		*stLocalControl.pbyData++ = IapReadByte(stLocalControl.uiShortType++);		
	}
	DisableIAP(); 
	//����ʹ������
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
}
/**********************************************************
*��������			:SaveParameter	
*��������        	:�洢���ò���,�洢����ǰ�Ȳ�����������,
					 Ȼ�����в�������д������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:stLocalControl
*����ģ��  			:EnableIAP,IapErase,IapWriteByte,DisableIAP
***********************************************************
*������	      		:������
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SaveParameter(void)
{  
	stLocalControl.uiShortType = IAP_ADDR;	
	stLocalControl.pbyData = &(stLocalControl.stEepromCfgData.bySelfSecAddr);
	stLocalControl.byChar1 = sizeof(STEepromCfgData);
	EnableIAP(IAP_ERASE);
	IapErase(IAP_ADDR); 	
	EnableIAP(IAP_WRITE); 
	while(stLocalControl.byChar1--)
	{
		IapWriteByte(stLocalControl.uiShortType++, *stLocalControl.pbyData++);
	}
	//�������д���������е��øú���,���¼���ʹ������
	byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
	byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	DisableIAP();	
	EnableIAP(IAP_ERASE);
	IapErase(0x0000); 	
	EnableIAP(IAP_WRITE);
	IapWriteByte(0x0000,INIT_FLAG);
	DisableIAP();
}
/**********************************************************
*��������			:ParameterInit	
*��������        	:������ʼ��
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
void ParameterInit(void)
{ 
	uint8 byTemp;
	//��ʼ��ȡϵͳ����
    EnableIAP(IAP_READ);
	if(IapReadByte(0x0000)==INIT_FLAG)
	{
		ReadParameter();
	}
	else
	{
		stLocalControl.stEepromCfgData.bySelfSecAddr=CBYTE[0x2800];
		stLocalControl.stEepromCfgData.bySelfRoomAddr=CBYTE[0x2801];
		stLocalControl.stEepromCfgData.bySelfBedAddr=CBYTE[0x2802];
		if(FmReadByte(Close_EnSoundNote_ADDR)==0x01)
			stLocalControl.stEepromCfgData.byEnable1=0xF7;
		else
			stLocalControl.stEepromCfgData.byEnable1=CBYTE[0x2803];
		stLocalControl.stEepromCfgData.byEnable2=CBYTE[0x2804];
		stLocalControl.stEepromCfgData.byPrio=CBYTE[0x2805];
		stLocalControl.stEepromCfgData.byRingTime=CBYTE[0x2806];
		stLocalControl.stEepromCfgData.byTalkTime=CBYTE[0x2807];
		stLocalControl.stEepromCfgData.byBroadTime=CBYTE[0x2808];
		stLocalControl.stEepromCfgData.byListenDelay=CBYTE[0x2809];
		stLocalControl.stEepromCfgData.byVolumStep=CBYTE[0x280A];
		stLocalControl.stEepromCfgData.byRingVol=CBYTE[0x280B];
		stLocalControl.stEepromCfgData.byCH0TalkedVol=CBYTE[0x280C];
		stLocalControl.stEepromCfgData.byCH1TalkedVol=CBYTE[0x280D];
		stLocalControl.stEepromCfgData.byBroadVol=CBYTE[0x280E];
		stLocalControl.stEepromCfgData.byNoteVol=CBYTE[0x280F];
		stLocalControl.stEepromCfgData.byMusicVol=CBYTE[0x2810];
		stLocalControl.stEepromCfgData.byCH0TalkVol=CBYTE[0x2811];
		stLocalControl.stEepromCfgData.byCH1TalkVol=CBYTE[0x2812];
		stLocalControl.stEepromCfgData.bySelfRingVol=CBYTE[0x2813];
		stLocalControl.stEepromCfgData.byMaxVol=CBYTE[0x2814];
		stLocalControl.stEepromCfgData.byMinVol=CBYTE[0x2815];
		stLocalControl.stEepromCfgData.byReserve1=CBYTE[0x2816];
		stLocalControl.stEepromCfgData.byReserve2=CBYTE[0x2817];
		stLocalControl.stEepromCfgData.byReserve3=CBYTE[0x2818];
		stLocalControl.stEepromCfgData.bySerialNum1=CBYTE[0x2819];
		stLocalControl.stEepromCfgData.bySerialNum2=CBYTE[0x281A];
		stLocalControl.stEepromCfgData.bySerialNum3=CBYTE[0x281B];
		stLocalControl.stEepromCfgData.byVersionHi=CBYTE[0x281C];
		stLocalControl.stEepromCfgData.byVersionLo=CBYTE[0x281D];
		byEnable1 = stLocalControl.stEepromCfgData.byEnable1;
		byEnable2 = stLocalControl.stEepromCfgData.byEnable2;
	}
	//��ȡҺ�����ò���
	byTemp = FmReadByte(LCD_INIT_ADDR);
	if(INIT_FLAG == byTemp)
	{ 	//�Ѿ���ʼ����
		Modify_SEC_Name	= FmReadByte(LCD_SEC_MSK_ADDR);
	    stLocalControl.uiShortType = LCD_SEC_ADDR;	            //��ʼ��ȡ��ɫ����
        stLocalControl.uiShortType =stLocalControl.uiShortType+18;
	    Page_Color[0]=FmReadByte(stLocalControl.uiShortType++);//����������ɫ 
	    Page_Color[1]=FmReadByte(stLocalControl.uiShortType++);//�������屳��ɫ,Ҳ������ɫ
	    Page_Color[2]=FmReadByte(stLocalControl.uiShortType++);//�м������ɫ
	    Page_Color[3]=FmReadByte(stLocalControl.uiShortType++);//�������������屳��ɫ
	    Page_Color[4]=FmReadByte(stLocalControl.uiShortType++);//��������������ɫ,Ҳ������ɫ
	}
	else
	{	//û�г�ʼ��
		byTemp = FmReadByte(LCD_INIT_ADDR);
		if(INIT_FLAG == byTemp)
		{	//���¼��
			Modify_SEC_Name	= FmReadByte(LCD_SEC_MSK_ADDR);
			stLocalControl.uiShortType = LCD_SEC_ADDR;		              //��ʼ��ȡ��ɫ����
            stLocalControl.uiShortType =stLocalControl.uiShortType+18;
	        Page_Color[0]=FmReadByte(stLocalControl.uiShortType++);//����������ɫ 
	        Page_Color[1]=FmReadByte(stLocalControl.uiShortType++);//�������屳��ɫ,Ҳ������ɫ
	        Page_Color[2]=FmReadByte(stLocalControl.uiShortType++);//�м������ɫ
	        Page_Color[3]=FmReadByte(stLocalControl.uiShortType++);//�������������屳��ɫ
	        Page_Color[4]=FmReadByte(stLocalControl.uiShortType++);//��������������ɫ,Ҳ������ɫ
			return;
		}
		FmWriteByte(LCD_INIT_ADDR, INIT_FLAG);
		FmWriteByte(LCD_SEC_MSK_ADDR, 0);
	}
	SerchRoom=FmReadByte(Serch_Room_ADDR);
	if(SerchRoom==1||SerchRoom==2||SerchRoom==3||SerchRoom==4||SerchRoom==5||SerchRoom==6||SerchRoom==7||SerchRoom==8||SerchRoom==9||SerchRoom==10||SerchRoom==11||SerchRoom==12||SerchRoom==13||SerchRoom==14||SerchRoom==15)
	{  
	 	SerchRoom=SerchRoom;
	}
	else
		SerchRoom=0;
	if(INIT_FLAG==FmReadByte(Serch_Room_Page_ADDR))
		Page_Flag=1;
	else
		Page_Flag=0;	
	Modify_Room_Number=FmReadByte(Modify_Room_Number_ADDR);
}
/**********************************************************
*��������			:Get_GT23_Data	
*��������        	:��ȡ�ֿ�ĵ�������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2011-3-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size)
{
	 uint32  xdata address1,address2,address3,address4;

	 if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <0x7E) //   0x20~0x7e  ��ASCII�ַ�
	 {
		 switch(word_size)
		 {
			 case 1: //16*8
			 default:
				 address1 = (lowbyte-0x20)*16; 
				 address3 = address1+A8BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,16);
				 break; 
				 
			 case 2: //24x12
				 address1 = (lowbyte-0x20)*48; 
				 address3 = address1+A12BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,48);
				 break; 				 
			 
			 case 3:	//32X16
				 address1 = (lowbyte-0x20)*64; 
				 address3 = address1+A16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,64);
				 break; 				 
/*				 case 4:   //96x48
				 if(lowbyte<=0x39&&lowbyte>=0x30)  //����
				{
				 address1 = (lowbyte-0x21)*576; 
				 address3 = address1+D48BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,576); 				
				}
				break;*/
				 
		 }		 
	 
	 }
	 else if(highbyte >=0x81&&highbyte<=0xfe&&lowbyte>=0x40&&lowbyte<=0xfe)    //GBK���� 32X32
	 {
		switch(word_size)
		{
			case 1:
			default:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*32;				 //ÿ������ռ��32�ֽ�
				 address4 = address3+C16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,32);
				 break;
			case 3:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*128;				 //ÿ������ռ��128�ֽ�
				 address4 = address3+C32BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,128);
				 break; 			 
		}
	 }

}



/**********************************************************
*��������			:Get_GT23_Data1	
*��������        	:��ȡ�ֿ�ĵ�������
*�������   		:
*����ֵ				:
*ȫ�ֱ���			:
*����ģ��  			:
***********************************************************
*������	      		:�ܼ�ǿ
*��������	 		:2011-3-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void Get_GT23_Data1(uint8 *word,uint8 word_size)
{
	 uint32  xdata address1,address2,address3,address4;
	 uint8 xdata highbyte,lowbyte;
	 highbyte = *word;
	 word++;
	 lowbyte = *word;
	 if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <0x7E) //   0x20~0x7e  ��ASCII�ַ�
	 {
		 switch(word_size)
		 {
			 case 1: //16*8
			 default:
				 address1 = (lowbyte-0x20)*16; 
				 address3 = address1+A8BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,16);
				 break; 
				 
			 case 2: //24x12
				 address1 = (lowbyte-0x20)*48; 
				 address3 = address1+A12BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,48);
				 break; 				 
			 
			 case 3:	//32X16
				 address1 = (lowbyte-0x20)*64; 
				 address3 = address1+A16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,64);
				 break; 				 
/*				 case 4:   //96x48
				 if(lowbyte<=0x39&&lowbyte>=0x30)  //����
				{
				 address1 = (lowbyte-0x21)*576; 
				 address3 = address1+D48BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address3,576); 				
				}
				break;*/
				 
		 }		 
	 
	 }
	 else if(highbyte >=0x81&&highbyte<=0xfe&&lowbyte>=0x40&&lowbyte<=0xfe)    //GBK���� 32X32
	 {
		switch(word_size)
		{
			case 1:
			default:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*32;				 //ÿ������ռ��32�ֽ�
				 address4 = address3+C16BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,32);
				 break;
			case 3:
				 address1 = (highbyte-0x81)*191; 
				 address2 = (lowbyte-0x40)+address1;
				 address3 = address2*128;				 //ÿ������ռ��128�ֽ�
				 address4 = address3+C32BaseAdd; 
				 Read_Cont(GT23_Rec_Data,address4,128);
				 break; 			 
		}
	 }

}


/************************************************************************/
/* PROCEDURE: WREN														*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WREN()
{
	SST_CS=0;				/* enable device */
	SpiTransferByte(0x06);		/* send WREN command */
	SST_CS=1;				/* disable device */
	Wait_Busy();
}


/************************************************************************/
/* PROCEDURE: Sector_Erase												*/
/*																		*/
/* This procedure Sector Erases the Chip.								*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 07FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Sector_Erase(unsigned long Dst)
{


	SST_CS=0;					/* enable device */
	SpiTransferByte(0x20);			/* send Sector Erase command */
	SpiTransferByte(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SpiTransferByte(((Dst & 0xFFFF) >> 8));
	SpiTransferByte(Dst & 0xFF);
	SST_CS=1;					/* disable device */
}

/**********************************************************
*��������			:FRAM_TEST	
*��������        	:�洢оƬ���Բ���
*�������   		:
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
void FRAM_TEST(void)
{
	unsigned char i;
	
	FRAM_BUFF[0]=0x11;
	FRAM_BUFF[1]=0x22;
	FRAM_BUFF[2]=0x33;
	FRAM_BUFF[3]=0x44;
	FRAM_BUFF[4]=0x55;
	FRAM_BUFF[5]=0x66; 
	
	FRAM_BUFF[6]=0x77;
	FRAM_BUFF[7]=0x88;
	FRAM_BUFF[8]=0x99;
	FRAM_BUFF[9]=0xaa;
	FRAM_BUFF[10]=0xbb;
	
	
	FmWriteByte(0x3e9000,FRAM_BUFF[0]);
	FmWriteBytes1(0x3e9002,10,&(FRAM_BUFF[1]));
	
	for(i=0;i<12;i++) FRAM_BUFF[i]=0;
	Delayms(10);

	FRAM_BUFF[0]=FmReadByte(0x3e9000);
	Read_Cont(&(FRAM_BUFF[1]),0x3e9002,10);
	Delayms(10);

	Send_Data(FRAM_BUFF,11);


}



