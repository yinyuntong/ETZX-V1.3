/*
����������������ģ�����
�޸ģ���ԭ��ת������ָ���Ϊ��������ָʾָ�ת��Ϊ����ָ��  20140516

*/
#include "cc1101.h"
#include "uart.h"
#include "�����߽ӿ�.h"
//#define  cont 6
#define  cont 3
sbit   led0 = P1^0;
sbit   led1 = P1^1;
unsigned char int0_buf;
unsigned char area=0xff; //��������

extern unsigned int data timer;
//unsigned char cont =8;
void int0() interrupt 2
{
  EX1=0;int0_buf=1;
}
void main(void)
{  unsigned long x;
   unsigned char xdata TxBuf[8],RxBuf[24],RxBak[8];


//   unsigned char xdata LastCallBuf[8];
   unsigned char leng,i;
   led0=0;led1=0;
   WDT_CONTR=0x3f;
   CpuInit();
   led0=1;
   x=0;TxBuf[2]=1;
   timer=6001;
   UART_init(9600);
   uart_send_one(0xaa);
   SingleBusInit();
   //halRfSendPacket(TxBuf,8);}
   EA=1;
   int0_buf=0;
   ES=1;
   EX1=1;
   IT1=1;
   halSpiStrobe(CCxxx0_SRX);		//�������״̬
   TxBuf[7]=0;uart_buf=0;TI=0;RI=0;ES=1;EA=1;
   while(1)
   { 
   	 x++;
     WDT_CONTR =0x3f;
	 if (timer>12000) //500us*6000 //3s��ʱ�����Զ������������
	 {
	 	for (i=0;i<8;i++) RxBak[i]=0;
		timer=0;
	 }
     if(int0_buf==1)
	 { 
		led0=0;
//	 	uart_send_one(0xaa);
		leng = 24;
		for (i=0;i<8;i++) RxBuf[i]=0;
        if(halRfReceivePacket(RxBuf,&leng))
		{
//		  for (i=0;i<leng;i++) uart_send_one(RxBuf[i]);
 		  WDT_CONTR =0x3f;
		  if(RxBuf[7]==0x55)
		  {
			//���û�д����߻�ȡ�������ţ��������κη��Ϲ�������ݽ���ϵͳ������ֻ������ͬ��������ݽ���ϵͳ
			if (area==0xff||area==RxBuf[0])
			{
			  	//�϶��Ƿ�����һ֡������ͬ
				for (i=0;i<8;i++)
				{
					if (RxBak[i]!=RxBuf[i]) break;
				}
				if (i<8) //����뱸��������ͬ�򲻷���
				{
					//for(i=0;i<leng;i++) uart_send_one(RxBuf[i]);
					timer=0;
					Bus0OutputData(RxBuf);
					for (i=0;i<8;i++) RxBak[i]=RxBuf[i]; //���±�������
					//����ǼӴ����У�����Ҫת�����ƶ��ֻ�����
					if (RxBuf[1]==250&&RxBuf[3]==0x06)
					{
					   RxBuf[7]=0xaa;
					   for(i=0;i<cont;i++)
					   { 
					   	 WDT_CONTR =0x3f;
						 halSpiStrobe(CCxxx0_SIDLE);
					   	 halRfSendPacket(RxBuf,8);}
					   }  
					   halSpiStrobe(CCxxx0_SRX);//delay(1000);		       //�������״̬
				   }
				}
			
		  }
		}
		int0_buf=0;
		WDT_CONTR =0x3f;
		halSpiStrobe(CCxxx0_SRX);		//�������״̬
		led0=1;
		EX1=1;
	  }
	 if(x>120000)
	 {
	 	x=0;
		//Bus0OutputData(RxBuf);
	  //	 for(i=0;i<cont;i++){TxBuf[0]=0x01;TxBuf[7]=0xaa;TxBuf[3]=0x06;TxBuf[4]=0x01;TxBuf[5]=0x01;TxBuf[4]++;
		//	  halRfSendPacket(TxBuf,8);}
		//	  halSpiStrobe(CCxxx0_SRX);		       //�������״̬
		//  EX1=1;
	 }
	 if(bBus0SendFinish==1)bBus0SendFinish=0;      //������ݷ�����ϣ����ͱ�־��0;
	 if(bBus0RecFinish==1)                         //������յ����ݣ�
	 {  
		led0=0;
	 	bBus0RecFinish=0; 
		EA=0;
  	    for(i=0;i<7;i++)
		{ 
			TxBuf[i]=byBus0RecData[i];//uart_send_one(TxBuf[i]);
		}
		EA=1;
		if((TxBuf[3]==0x5a)||(TxBuf[3]==0x2f))
        {
		   if (TxBuf[3]==0x2f)
		   {
              //ָʾ��תΪ������
			  TxBuf[3] = TxBuf[4]&0x1f;	
			  TxBuf[4] = 0;
		   }
		   if (TxBuf[3]==0x06)area = TxBuf[0];

		   TxBuf[7]=0xaa;
		   for(i=0;i<cont;i++)
		   { 
		   	 WDT_CONTR =0x3f;
			 halSpiStrobe(CCxxx0_SIDLE);
			 
		   	 halRfSendPacket(TxBuf,8);

			 //delay(0xffff);
		   }


/*
			delay(0xffff);
			TxBuf[3] = 0x5d;
			TxBuf[7]=0xaa;
			WDT_CONTR =0x3f;
			halSpiStrobe(CCxxx0_SIDLE);
			halRfSendPacket(TxBuf,8);*/		   
		   
		}  
		halSpiStrobe(CCxxx0_SRX);//delay(1000);		       //�������״̬
	}
    led0=1;
	 /*if(uart_buf>0)
	    { //uart_send_one(0x5a);
		  TxBuf[0]=0x01;
		  TxBuf[1]=01;
		 // TxBuf[2]=1;
		  TxBuf[3]=0x07;
          TxBuf[4]=0x01;
		  TxBuf[5]=255;
          TxBuf[6]=1;
		  TxBuf[7]=0xaa;TxBuf[2]++;
		  
		  //for(i=0;i<cont;i++){
		  halRfSendPacket(TxBuf,8);delay(1000);
		 // }
		  
		  halSpiStrobe(CCxxx0_SRX);		       //�������״̬
          uart_buf=0;
		  EX1=1;
		}*/
   }
}