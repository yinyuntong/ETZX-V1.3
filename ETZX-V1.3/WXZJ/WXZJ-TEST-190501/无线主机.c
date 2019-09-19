/*
管理主机无线主机模块程序
修改：将原来转发呼叫指令改为接收总线指示指令并转换为呼叫指令  20140516

*/
#include "cc1101.h"
#include "uart.h"
#include "单总线接口.h"
//#define  cont 6
#define  cont 3
sbit   led0 = P1^0;
sbit   led1 = P1^1;
unsigned char int0_buf;
unsigned char area=0xff; //所属区域

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
   halSpiStrobe(CCxxx0_SRX);		//进入接收状态
   TxBuf[7]=0;uart_buf=0;TI=0;RI=0;ES=1;EA=1;
   while(1)
   { 
   	 x++;
     WDT_CONTR =0x3f;
	 if (timer>12000) //500us*6000 //3s超时，则自动清除备份数据
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
			//如果没有从总线获取过区域编号，则允许任何符合规则的数据进入系统，否则只允许相同区域的数据进入系统
			if (area==0xff||area==RxBuf[0])
			{
			  	//断定是否与上一帧数据相同
				for (i=0;i<8;i++)
				{
					if (RxBak[i]!=RxBuf[i]) break;
				}
				if (i<8) //如果与备份数据相同则不发送
				{
					//for(i=0;i<leng;i++) uart_send_one(RxBuf[i]);
					timer=0;
					Bus0OutputData(RxBuf);
					for (i=0;i<8;i++) RxBak[i]=RxBuf[i]; //更新备份数据
					//如果是加床呼叫，则需要转发到移动分机报号
					if (RxBuf[1]==250&&RxBuf[3]==0x06)
					{
					   RxBuf[7]=0xaa;
					   for(i=0;i<cont;i++)
					   { 
					   	 WDT_CONTR =0x3f;
						 halSpiStrobe(CCxxx0_SIDLE);
					   	 halRfSendPacket(RxBuf,8);}
					   }  
					   halSpiStrobe(CCxxx0_SRX);//delay(1000);		       //进入接收状态
				   }
				}
			
		  }
		}
		int0_buf=0;
		WDT_CONTR =0x3f;
		halSpiStrobe(CCxxx0_SRX);		//进入接收状态
		led0=1;
		EX1=1;
	  }
	 if(x>120000)
	 {
	 	x=0;
		//Bus0OutputData(RxBuf);
	  //	 for(i=0;i<cont;i++){TxBuf[0]=0x01;TxBuf[7]=0xaa;TxBuf[3]=0x06;TxBuf[4]=0x01;TxBuf[5]=0x01;TxBuf[4]++;
		//	  halRfSendPacket(TxBuf,8);}
		//	  halSpiStrobe(CCxxx0_SRX);		       //进入接收状态
		//  EX1=1;
	 }
	 if(bBus0SendFinish==1)bBus0SendFinish=0;      //如果数据发送完毕，发送标志清0;
	 if(bBus0RecFinish==1)                         //如果接收到数据，
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
              //指示码转为呼叫码
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
		halSpiStrobe(CCxxx0_SRX);//delay(1000);		       //进入接收状态
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
		  
		  halSpiStrobe(CCxxx0_SRX);		       //进入接收状态
          uart_buf=0;
		  EX1=1;
		}*/
   }
}