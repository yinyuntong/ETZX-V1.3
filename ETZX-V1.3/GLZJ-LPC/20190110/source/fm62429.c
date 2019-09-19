#define _IN_FM62429_

#include"custom.h"


void fm62429_pincfg(void);
void set_play_voice_volue(uint8_t ch,uint8_t vol_val);
void set_talk_voice_volue(uint8_t ch,uint8_t vol_val);
/*********************************************************************************************
*
*
*
*********************************************************************************************/
void fm62429_pincfg()
{
  	LPC_GPIO2->FIODIR |= (fm624_clk1_pin| fm624_dat1_pin);
	fm624_clk1_ctrl(1);
	fm624_dat1_ctrl(1);

	LPC_GPIO0->FIODIR |= (fm624_clk2_pin| fm624_dat2_pin);
	fm624_clk2_ctrl(1);
	fm624_dat2_ctrl(1);	
}
/*********************************************************************************************
*
*�����õ�������Χ1-28,��ֵԽ��,����Խ��
*ch=0: ��������������ch=1:�ֻ���������
*
*********************************************************************************************/
void set_play_voice_volue(uint8_t ch,uint8_t vol_val)
{	
	//read data rising edge of clk singal           latch data at falling edge
	uint8_t i=11;
	uint16_t j;
	//uint16_t j=0x601;
	//d0=0 ��һ��������		d0=1	�ڶ���������
	//d1=0	��������һ�����	d1=1	��������һ������
	//uint16_t j=0x603;
	ch &= 1;
	j=0x602+ch;

	if(vol_val>28)	vol_val = 28;

	vol_val *= 3;
	vol_val = 84 - vol_val;

	
	fm624_dat1_ctrl(1);
	short_delay(3000);

	fm624_clk1_ctrl(0);
	short_delay(2000);

	j  |=((3-(vol_val%4))<<7) | ((21-(vol_val/4))<<2);
	do
	{
		fm624_dat1_ctrl(0);
		short_delay(1500);

		fm624_clk1_ctrl(0);
		short_delay(1500);

		if(j&0x01) 	fm624_dat1_ctrl(1);
		else 			fm624_dat1_ctrl(0);	
		short_delay(1500);

		fm624_clk1_ctrl(1);
		short_delay(1500);

		j>>=1;
	}while(--i);
		
	fm624_dat1_ctrl(1);	
	short_delay(2000);

	fm624_clk1_ctrl(0);
	short_delay(2000);	

	fm624_clk1_ctrl(1);
	short_delay(2000);	
}


/*********************************************************************************************
*
*�����õ�������Χ1-28,��ֵԽ��,����Խ��
*ch=0: ͨ��ʱ�绰��������ch=1:ͨ��ʱ�ֻ�����
*
*********************************************************************************************/
void set_talk_voice_volue(uint8_t ch,uint8_t vol_val)
{	
	//read data rising edge of clk singal           latch data at falling edge
	uint8_t i=11;
	uint16_t j;
	//uint16_t j=0x601;
	//d0=0 ��һ��������		d0=1	�ڶ���������
	//d1=0	��������һ�����	d1=1	��������һ������
	//uint16_t j=0x603;
	ch &= 1;
	j=0x602+ch;

	if(vol_val>28)	vol_val = 28;

	vol_val *= 3;
	vol_val = 84 - vol_val;

	
	fm624_dat2_ctrl(1);
	short_delay(3000);

	fm624_clk2_ctrl(0);
	short_delay(2000);

	j  |=((3-(vol_val%4))<<7) | ((21-(vol_val/4))<<2);
	do
	{
		fm624_dat2_ctrl(0);
		short_delay(1500);

		fm624_clk2_ctrl(0);
		short_delay(1500);

		if(j&0x01) 		fm624_dat2_ctrl(1);
		else 			fm624_dat2_ctrl(0);	
		short_delay(1500);

		fm624_clk2_ctrl(1);
		short_delay(1500);

		j>>=1;
	}while(--i);
		
	fm624_dat2_ctrl(1);	
	short_delay(2000);

	fm624_clk2_ctrl(0);
	short_delay(2000);	

	fm624_clk2_ctrl(1);
	short_delay(2000);	
}



/*********************************************************************************************
*
*
*
*********************************************************************************************/
/*
ATT1		D10		D9		D8		D7		D6		D5		D4		D3		D2		D1		D0
0			1		1						1		0		1		0		1		0		1
-4											1		0		1		0		0
-8											1		0		0		1		1
-12											1		0		0		1		0
-16											1		0		0		0		1
-20											1		0		0		0		0
-24											0		1		1		1		1
-28											0		1		1		1		0
-32											0		1		1		0		1
-36											1		1		1		0	   	0
-40											0		1		0		1		1
-44											0		1		0		1		0
-48											0		1		0		0		1
-52											0		1		0		0		0
-56											0		0		1		1		1
-60											0		0		1		1		0
-64											0		0		1		0		1
-68											0		0		1		0		0
-72											0		0		0		1 		1
-76											0		0		0		1		0
-80											0		0		0		0		1

ATT2						D8		D7
0							1		1
-1							1		0
-2							0		1
-3							0		0

((21-val/4)<<2) | ((3-val%4)<<7)	   1	11 1010101 01
*/
/*********************************************************************************************
*
*
*
*********************************************************************************************/


