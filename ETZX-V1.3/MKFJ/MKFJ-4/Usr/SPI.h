/*
************************Copyright(c)************************
*	  			   ����������Ƽ����޹�˾
*			       	All Rights Reserved
*			     		 
*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:SPI.h
*�ļ�����    		:SPI������������ͷ�ļ�
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
//����Ƭѡ�ź�
//ԭ����
//sbit SST_CS  = P4^1;											//����洢��Ƭѡ�ź���
//sbit GT23_CS = P4^5;									        //�ֿ�оƬƬѡ�ź���


//���ڵ�
sbit SST_CS  = P4^5;											//����洢��Ƭѡ�ź���
sbit GT23_CS = P4^1;									        //�ֿ�оƬƬѡ�ź���

//�洢������������
#define   SST_WRSR          0x01								//д״̬�Ĵ���
#define   SST_WRITE		    0x02 								//д����
#define   SST_READ			0x03								//������
#define   SST_WRDI			0x04								//д��ֹ
#define   SST_WREN			0x06								//д����
#define   SST_SECTOR_EARSE  0x20 								//����������� ,ÿ������Ϊ4KB�ֽڿռ�
#define   SST_EWSR          0x50								//����д״̬�Ĵ���
#define   SST_AAI_S         0xAF 								//SST��ַ�Զ�����
#define   SST_AAI_P         0xAD 								//PCT��ַ�Զ�����
#define   SST_READ_ID       0x90 								//��д�洢оƬID��

/*#define  C24BaseAdd        0x68190;  							//24*24 ����
#define   C32BaseAdd  		0xEDF00;  							//32*32 ����
#define   A8BaseAdd   		0x1DD780;  							//8*16 ASCII
#define   A12BaseAdd  		0x1DFF00;  							//16*24 ASCII
#define   A16BaseAdd  		0x1E5A50;  							//16*32 ASCII*/


//�ֿ������ƫ�Ƶ�ַ�궨��
	

#define  A8BaseAdd    0x00000000  //16*8ASCI
#define  A12BaseAdd   0x000005f0  //16*8ASCI
#define  A16BaseAdd   0x000017c0  //32*16ASCI

#define  D48BaseAdd   0x00002f80   //96*48����

#define  C16BaseAdd  0x00004600;  //16*16 ����
#define  C32BaseAdd  0x000c0640;  //32*32 ����
	//--------------------------------------

#ifndef _IN_SPI_
#define _IN_SPI_
uint8 SpiTransferByte(uint8 byData);
uint8 FmReadByte(uint32 uiAddr);
uint8 Read_Status_Register(void);
void  SpiInit(void);
void  Wait_Busy(void);
void  Delay_nus(uint8 xdata i);
void  FmWriteByte(uint32 uiAddr, uint8 byData);
void  FmWriteBytes(uint32 uiAddr, uint16 uiLen, uint8* pbyData);
void  ParameterInit(void);
void  SaveParameter(void);
void  Get_GT23_Data(uint8 highbyte,uint8 lowbyte,uint8 word_size);
void Get_GT23_Data1(uint8 *word,uint8 word_size);
void Read_Cont(uint8 *pBuffer,unsigned long Dst, unsigned long no_bytes);

//void Get_GT23_Data1(uint8 *word,uint8 word_size);
uint8 READ_ID(void);
void FRAM_TEST(void);

#endif
