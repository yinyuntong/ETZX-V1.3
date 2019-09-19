#include "main.h"
#include "wtvxxx.h"


//ISD1700�����
#define		CMD_STOP_PLAY	0xFE        //ֹͣ����
#define		CMD_LOOP_PLAY   0xF2		//ѭ������
#define     CMD_OPEN_PA     0xF1        //�򿪹���
#define     CMD_CLOSE_PA    0xF0        //�رչ���
#define     CMD_VOL_SET     0xE0        //�������� E0~EF  16��
#define     CMD_PLAY        0x00        //���β��� 00~7F  128������

extern unsigned char voml;     //�����ݴ���
extern bool audio_irq;
bool audio_play=0; //���ڷ���
#define MAX_VOICE_ROWS 22

INT8U play_buf[30]; //����������
INT8U play_start=0,play_end=0;//����������ʼ�����

//�������ݶ���
const uint8 byVoiceDataAddr[23]= 
                            {
                               0x00,//����0��     0
                               0x01,//����1��     1
                               0x02,//����2��     2
                               0x03,//����3��     3
                               0x04,//����4��     4
                               0x05,//����5��     5
                               0x06,//����6��     6
                               0x07,//����7��     7
                               0x08,//����8��     8
                               0x09,//����9��     9
                               0x0a,//����ʮ��    10
                               0x0b,//�����٣�    11
                               0x0c,//����  ���У�12
                               0x0d,//����  ��Һ��  13
                               0x0e,//����  ��ϣ�  14
                               0x0f,//����  ����    15
                               0x10,//����  �ţ�    16
                               0x11,//����  ����    17
                               0x12,//����  �����䣻18
                               0x13,//����  �ɹ���  19
                               0x14, //����  ʧ�ܣ� 20
							   0x15, //����  �ӣ�   21
							   0x16, //����  �֣�   22
							 };

/*---------------------------------------------------------------------------
����ԭ��: void Delayms(uint ms)
����˵��: ms--��Ҫ��ʱ��ֵ
�� �� ֵ: ��
�������ܣ���ʱ����(����18.432M����ָ��������ʱ1mS)
----------------------------------------------------------------------------*/
void Delayms(uint16 ms)	  
{
   uint16 i;
   while(ms--)
		for(i=300;i>0;i--);
}

void Delayus(uint16 us)	  
{
 while(us--){nop();nop();nop();};
}

void WTVxxxInit(unsigned char voml)
{		
	//��λ����оƬISD17XX
	SetAUDRST(0);
	Delayms(5); 		
	SetAUDRST(1);
	Delayms(10);
}
void StopPlay(void)
{
	SPI_Cmd(DISABLE);
	Delayms(2);
	SetSCK(1);
  SetAUDCS(0);
	Delayms(2);
	SPITransferByte(CMD_CLOSE_PA);
	SetAUDCS(1);
	SPI_Cmd(ENABLE);
	play_end=play_start;
	audio_play=0;
}



void StartPlay(void)
{   
	if (audio_play) return;
	SPI_Cmd(DISABLE);
	Delayms(1);
	SetSCK(1);
  SetAUDCS(0);
	Delayms(1);
	SPITransferByte(CMD_OPEN_PA);
	SPITransferByte(CMD_VOL_SET+voml); 
	SPITransferByte(CMD_PLAY+play_buf[play_start]);
	play_start = (play_start+1)%30;
	//SPITransferByte(CMD_CLOSE_PA);
	SetAUDCS(1);
	SPI_Cmd(ENABLE);
	Delayms(1);
	audio_irq=0;
	audio_play=1;
}

void CheckPlay(void)
{
	//��鱨��״̬
	if (play_end!=play_start) StartPlay();
	else StopPlay();	
}
//��������
void PlaySound(INT8U pos)
{
	play_buf[play_end] = pos;
	if (play_start==play_end) StartPlay();
	play_end = (play_end+1)%30;
}


//dat ���� dat1 ��ͷ��  dat2 ��������
/*void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
{ 
	if((dat2==6)||(dat2==7))  //��ͷ���л���Һ���
	{
		if (dat==250) PlaySound(21);//�Ӵ�
		dat = dat1;
		if (dat1>=100)   //
		{
			PlaySound(dat1/100);
			PlaySound(11);
			dat1 %= 100;
		}
		if (dat1>=10)
		{
			if (dat1>19||dat>100) PlaySound(dat1/10);
			PlaySound(10);
			dat1 %= 10;
		} else if(dat>100) PlaySound(0);
		if (dat1>0)	PlaySound(dat1);
		PlaySound(15);//��
		if (dat2==6) PlaySound(12);//����
		else {
			PlaySound(13);
			PlaySound(14);//��Һ���
		}
	}
	else //���������
	{
		if (dat>=10) //
		{
			if (dat>19) PlaySound(dat/10);
			PlaySound(10);
			dat %= 10;
		}
		if (dat>0) PlaySound(dat);
		PlaySound(17); //��
		PlaySound(18);//������
		PlaySound(12);//����
	}
}*/


//dat ���� dat1 ��ͷ��  dat2 ��������

void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
{ 
	if((dat2==BED_CALL_TYPE) ||(dat2==BED_INFUSION_TYPE))//��ͷ���л���Һ���,����"xx������"
	{
		if(dat == ADD_FJ) PlaySound(EXTRA_VOC_ADDR);	//�Ӵ�

		PlaySound(BED_VOC_ADDR);
		PlaySound(EXTENSION_VOC_ADDR);
		PlaySound(NUMBER_VOC_ADDR);

	}
	else if(dat2==WC_CALL_TYPE)
	{
		PlaySound(BATHROOM_VOC_ADDR);
		PlaySound(EXTENSION_VOC_ADDR);
		PlaySound(NUMBER_VOC_ADDR);

		dat1 = dat;		//�����������ڷ���	
	}
	else return;

	if(dat1>=100)
	{
		PlaySound((dat1/100)/*-1*/);
		PlaySound(HUNDRED_VOC_ADDR);
		if(dat1%100 )
		{
			PlaySound(AND_VOC_ADDR);
			dat1%=100;


		}
	}

	if(dat1>=20)
	{
		switch(dat1/10)
		{
			case 2:
				PlaySound(TWENTY_VOC_ADDR);
				break;
			case 3:
				PlaySound(THIRTY_VOC_ADDR);
				break;	
			case 4:
				PlaySound(FOURTY_VOC_ADDR);
				break;	
			case 5:
				PlaySound(FIFTY_VOC_ADDR);
				break;
			case 6:
				PlaySound(SIXTY_VOC_ADDR);
				break;	
			case 7:
				PlaySound(SEVENTY_VOC_ADDR);
				break;	
			case 8:
				PlaySound(EIGHTY_VOC_ADDR);
				break;	
			case 9:
				PlaySound(NINETY_VOC_ADDR);
				break;
			default:
				break;
		}

		dat1 %=10;
		if(dat1!=0)
		{
			PlaySound((dat1%10)/*-1*/);
		}
	}
	else //����20
	{
	
		if(dat1!=0)	PlaySound(dat1/*-1*/);
	}

	
	PlaySound(CALLING_VOC_ADDR);

	
}





void playtest(INT8U start_seg,INT8U end_seg)
{
	INT8U i;
	for(i=start_seg;i<=end_seg;i++)
	{
		PlaySound(i);
	}
}

