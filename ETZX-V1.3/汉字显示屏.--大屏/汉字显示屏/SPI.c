/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SPI.c
*�ļ�����    		:SPI������������
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
#define _IN_SPI_
#include "config.h"

extern   STLocalControl xdata stLocalControl;
extern   uint8  xdata  DotArrayRedData[192];
extern   uint8  xdata  DotArrayBlueData[192];
uint8    xdata  GT23_Rec_Data[32];
uint8    xdata  GT23_Rec_Length;
uint16   xdata  GT23_16LAddress;
uint16   xdata  GT23_16HAddress;
uint32   xdata  GT23_32Address; 
uint8    xdata  ID_Number=0;
uint8    xdata  ID_Number_Flag=0;
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
*������	      		:�ܼ�ǿ
*��������	 		:2008-9-22
***********************************************************
*�޸���	  			:
*�޸�����    		:
*ע��		      	:
**********************************************************/
void SpiInit(void)
{
	 P1M0 &= 0xF5;
	 P1M1 &= 0xF5;
	 SPCTL = 0xDC;				//����SS���ţ�ʹ��SPI�ӿڣ���λ��ǰ��Fosc/4	
	 SPSTAT = 0xC0;				//�����־ 
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
*������	      		:�ܼ�ǿ
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
/**********************************************************
*��������			:FmWriteByte	
*��������        	:��SST�洢��ָ����ַд��һ���ֽ�����
*�������   		:uiAddr:��ַ,byData:��д�������
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
/**********************************************************
*��������			:FmWriteBytes	
*��������        	:��SST�洢��ָ����ַ����ֽ�����
*�������   		:uiAddr:��ַ,uiLen:��д������ݳ���,pbyData:��д�����ݵĴ洢��ַ
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
/**********************************************************
*��������			:FmWriteConstBytes	
*��������        	:��SST�洢��ָ����ַ����ֽ�����
*�������   		:uiAddr:��ַ,uiLen:��д������ݳ���,pbyData:��д�����ݵĴ洢��ַ
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
void FmWriteConstBytes(uint32 uiAddr, uint16 uiLen, uint8 const* pbyData)
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
/**********************************************************
*��������			:ParameterInit	
*��������        	:������ʼ��
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
void ParameterInit(void)
{
    uint8 i;
	uint8 byTemp;
	uint32 xdata tempaddr;
	EnableIAP(IAP_READ);
	stLocalControl.stEepromCfgData.bySelfSecAddr=CBYTE[0x2800];
	stLocalControl.stEepromCfgData.bySelfRoomAddr=CBYTE[0x2801];
	stLocalControl.stEepromCfgData.bySelfBedAddr=CBYTE[0x2802];
	stLocalControl.stEepromCfgData.byLight=CBYTE[0x2803];
	stLocalControl.stEepromCfgData.byDispTime=CBYTE[0x2804];
	if(IapReadByte(0x0200)==INIT_FLAG)
	stLocalControl.stEepromCfgData.byEnable=0xF4;
	else
	stLocalControl.stEepromCfgData.byEnable=CBYTE[0x2805];
	stLocalControl.stEepromCfgData.uiBroadEnable=((uint16)CBYTE[0x2806])<<8;
	stLocalControl.stEepromCfgData.uiBroadEnable|=CBYTE[0x2807];
    stLocalControl.stEepromCfgData.uiBroadExist=((uint16)CBYTE[0x2808])<<8;
	stLocalControl.stEepromCfgData.uiBroadExist|=CBYTE[0x2809];
	stLocalControl.stEepromCfgData.byReserve1=CBYTE[0x280A];
	stLocalControl.stEepromCfgData.byReserve2=CBYTE[0x280B];
	stLocalControl.stEepromCfgData.byReserve3=CBYTE[0x280C];
	stLocalControl.stEepromCfgData.bySerialNum1=CBYTE[0x280D];
	stLocalControl.stEepromCfgData.bySerialNum2=CBYTE[0x280E];
	stLocalControl.stEepromCfgData.bySerialNum3=CBYTE[0x280F];
	stLocalControl.stEepromCfgData.byVersionHi=CBYTE[0x2810];
	stLocalControl.stEepromCfgData.byVersionLo=CBYTE[0x2811]; 
	DisableIAP();
	//----------------------------------------------------
	//----------------------------------------------------
	//��ȡҺ�����ò���
	byTemp = FmReadByte(LCD_INIT_ADDR);
	if(INIT_FLAG == byTemp)	    //�Ѿ���ʼ����
	{ 	
	    tempaddr=LOGO_ADDR_RED;
		stLocalControl.pbyData=0;
		for(i=0;i<12;i++)
		{
		  *stLocalControl.pbyData=FmReadByte(tempaddr);
		  stLocalControl.pbyData++;
		  tempaddr++; 
		}
		stLocalControl.pbyData=0;
		GBdatatoDotarray();
		for(i=0;i<192;i++)
    	DotArrayRedData[i]=DotArrayBlueData[i];
	    tempaddr=LOGO_ADDR_BLUE;
		stLocalControl.pbyData=0;
		for(i=0;i<12;i++)
		{
		  *stLocalControl.pbyData=FmReadByte(tempaddr);
		  stLocalControl.pbyData++;
		  tempaddr++; 
		}
		stLocalControl.pbyData=0;
		GBdatatoDotarray();	
	}
	else  //û�г�ʼ��
	{	
		FmWriteConstBytes(LOGO_ADDR_RED,12,(uint8 const*)(0x2A00));	//�洢��ɫLOGO����			
		tempaddr=LOGO_ADDR_RED;
		stLocalControl.pbyData=0;
		for(i=0;i<12;i++)
		{
		  *stLocalControl.pbyData=FmReadByte(tempaddr);
		  stLocalControl.pbyData++;
		  tempaddr++; 
		}
		stLocalControl.pbyData=0;
		GBdatatoDotarray();
		for(i=0;i<192;i++)
    	DotArrayRedData[i]=DotArrayBlueData[i];
		FmWriteConstBytes(LOGO_ADDR_BLUE,12,(uint8 const*)(0x2B00));//�洢��ɫLOGO����
		tempaddr=LOGO_ADDR_BLUE;
		stLocalControl.pbyData=0;
		for(i=0;i<12;i++)
		{
		  *stLocalControl.pbyData=FmReadByte(tempaddr);
		  stLocalControl.pbyData++;
		  tempaddr++; 
		}
		stLocalControl.pbyData=0;
		GBdatatoDotarray();	
		FmWriteByte(LCD_INIT_ADDR, INIT_FLAG);			
	}		
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Get_GT23_Data(uint8 highbyte,uint8 lowbyte)
{
    uint32  xdata address1,address2,address3,address4;
    if(highbyte >=0xB0&&highbyte<=0xF7&&lowbyte>=0xA1)
	 {
		  address1 = (highbyte-0xB0)*94; 
	      address2 = lowbyte-0xA1+846+address1;
	      address3 = address2*32;
	      address4 = address3+C16BaseAdd; 
	 }
   else if(highbyte >=0xA1&&highbyte<=0xA9&&lowbyte>=0xA1)
	{
		  address1 = (highbyte-0xA1)*94; 
	      address2 = lowbyte-0xA1+address1;
	      address3 = address2*32;
	      address4 = address3+C16BaseAdd; 
	 }
	else if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <= 0x7E)	  //�����ASCII
	{ 
	      address1 = (lowbyte-0x20)*16; 
	      address4 = address1+A8BaseAdd;
	}
   GT23_32Address=address4;
   GT23_16HAddress=GT23_32Address/65536;
   GT23_16LAddress=GT23_32Address%65536;
   GT23_CS=0;
   SpiTransferByte(0x03);
   SpiTransferByte(GT23_16HAddress%256);
   SpiTransferByte(GT23_16LAddress/256);
   SpiTransferByte(GT23_16LAddress%256);
   if(highbyte >=0xA1&&highbyte<=0xF7&&lowbyte>=0xA1)
   {
	   for(GT23_Rec_Length=0;GT23_Rec_Length<32;GT23_Rec_Length++)
       GT23_Rec_Data[GT23_Rec_Length]=SpiTransferByte(0xFF);
   }
   else if(highbyte==0x00&&lowbyte>=0x20&&lowbyte <= 0x7E)	
   {
        for(GT23_Rec_Length=0;GT23_Rec_Length<16;GT23_Rec_Length++)
        GT23_Rec_Data[GT23_Rec_Length]=SpiTransferByte(0xFF);
	}	   
   GT23_CS=1;
}

