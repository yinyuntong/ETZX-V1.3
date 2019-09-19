
#include "intrins.h"

#define uchar unsigned char
#define uint  unsigned int

#define DEBUG 1


#define KEY0_UP			0
#define KEY1_UP			1
#define KEY2_UP			2
#define KEY3_UP			3
#define KEY4_UP			4
#define KEY5_UP			5
#define KEY6_UP			6
#define KEY7_UP			7
#define KEY8_UP			8
#define KEY9_UP			9
#define KEY_ENTER_UP	0x0a
#define KEY_ESC_UP		0x0b
#define KEY_CALL_UP 	0x0c

#define KEY0_DOWN			0x10
#define KEY1_DOWN			0x11
#define KEY2_DOWN			0x12
#define KEY3_DOWN			0x13
#define KEY4_DOWN			0x14
#define KEY5_DOWN			0x15
#define KEY6_DOWN			0x16
#define KEY7_DOWN			0x17
#define KEY8_DOWN			0x18
#define KEY9_DOWN			0x19
#define KEY_ENTER_DOWN		0x1a
#define KEY_ESC_DOWN		0x1b
#define KEY_CALL_DOWN 		0x1c


#define NO_KEY				0xff


#define DISP_ON_TIME		30


#include <STC12C5A.H>
#include <ascii.h>
#include <display.h>
#include <main.h>


//Î»Ñ¡ÐÅºÅ

sbit DB0		=	P0^0;
sbit DB1		=	P0^1;
sbit DB2		=	P0^2;
sbit DB3		=	P0^3;
sbit DB4		=	P0^4;
sbit DB5		=	P0^5;
sbit DB6		=	P0^6;
sbit DB7		=	P0^7;




sbit Row1		=	P1^0;
sbit Row2		=	P1^1;
sbit Row3		=	P1^2;
sbit Col1		=	P1^3;
sbit Col2		=	P1^4;
sbit Col3		=	P1^5;
sbit Col4		=	P1^6;


sbit ir			=	P3^4;



#define LCD_DATA  P0

sbit LCD_WR  = P5^2;
sbit LCD_RD  = P5^3;
sbit LCD_A0  = P4^6;
sbit LCD_RST = P4^1;
sbit LCD_BLK = P3^5;  

