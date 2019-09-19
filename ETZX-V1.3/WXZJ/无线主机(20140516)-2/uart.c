#include    "stc12c5624.h"
////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////
unsigned char uart_data[10];
unsigned char uart_buf;
unsigned char int_buf;
void UART_init(unsigned long  band)
{  
  /* switch(band)
   {  case 1200:    TH1  = TL1  = 0xd8;
      case 2400:    TH1  = TL1  = 0xec;
      case 4800:    TH1  = TL1  = 0xf6;
	  case 9600:    TH1  = TL1  = 0xfb;
	  case 19200:   AUXR = 0x20;TH1  = TL1  = 0xf1;
	  case 38400:   AUXR = 0x20;TH1  = TL1  = 0xf8;
	  case 57600:   AUXR = 0x20;TH1  = TL1  = 0xfb;
	  case 115200:  AUXR = 0x20;TH1  = TL1  = 0xfd;
   }
   */
   TH1=TL1=-(26000000/32/12/band);
   PCON = 0x00;
   TMOD = 0x21;
   SCON = 0x50;
   TI   = 0;
   RI   = 0;
   TR1  = 1;
   //ES   = 1;
} 
/**********************************************************
**
**
/**********************************************************/

void UART (void) interrupt 4
{      if(RI)
	   { RI = 0;
	     uart_data[uart_buf]=SBUF;
		 uart_buf++;
	     if(uart_buf>9){uart_buf=9;};
	   }
	 TI = 0;
}
/**********************************************************
**
/**********************************************************/
void uart_send_one(unsigned char x)
{ 
  TI = 0;
  SBUF =x; 
  while(TI==0);TI=0;
}///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////

