   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     	bsct
   5  0000               _sm_state:
   6  0000 00            	dc.b	0
   7  0001               _IrState:
   8  0001 00            	dc.b	0
   9  0002               _Ir_timer:
  10  0002 00            	dc.b	0
  11  0003               _Ir_repeat_timer:
  12  0003 00            	dc.b	0
  61                     ; 13 void IrDi_800us(void)
  61                     ; 14 {
  63                     .text:	section	.text,new
  64  0000               _IrDi_800us:
  66  0000 88            	push	a
  67       00000001      OFST:	set	1
  70                     ; 15 	unsigned char i=240;
  72  0001 a6f0          	ld	a,#240
  73  0003 6b01          	ld	(OFST+0,sp),a
  75  0005 2009          	jra	L33
  76  0007               L72:
  77                     ; 16 	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
  80  0007 9d            nop
  86  0008 9d            nop
  92  0009 9d            nop
  98  000a 9d            nop
 104  000b 9d            nop
 110  000c 9d            nop
 116  000d 9d            nop
 122  000e 9d            nop
 128  000f 9d            nop
 131  0010               L33:
 134  0010 7b01          	ld	a,(OFST+0,sp)
 135  0012 0a01          	dec	(OFST+0,sp)
 136  0014 4d            	tnz	a
 137  0015 26f0          	jrne	L72
 138                     ; 17 }
 141  0017 84            	pop	a
 142  0018 81            	ret
 185                     ; 20 void IrDi_600us(void)
 185                     ; 21 {
 186                     .text:	section	.text,new
 187  0000               _IrDi_600us:
 189  0000 88            	push	a
 190       00000001      OFST:	set	1
 193                     ; 22 	unsigned char i=168;
 195  0001 a6a8          	ld	a,#168
 196  0003 6b01          	ld	(OFST+0,sp),a
 198  0005 2009          	jra	L16
 199  0007               L55:
 200                     ; 23 	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
 203  0007 9d            nop
 209  0008 9d            nop
 215  0009 9d            nop
 221  000a 9d            nop
 227  000b 9d            nop
 233  000c 9d            nop
 239  000d 9d            nop
 245  000e 9d            nop
 251  000f 9d            nop
 254  0010               L16:
 257  0010 7b01          	ld	a,(OFST+0,sp)
 258  0012 0a01          	dec	(OFST+0,sp)
 259  0014 4d            	tnz	a
 260  0015 26f0          	jrne	L55
 261                     ; 24 }
 264  0017 84            	pop	a
 265  0018 81            	ret
 288                     ; 32 void IrDi_init(void) //初始化PWM输出用作数据发送，FSK 为38KHZ
 288                     ; 33 {
 289                     .text:	section	.text,new
 290  0000               _IrDi_init:
 294                     ; 34 	TIM2->CR1 &= ~1; //关闭TIM2
 296  0000 72115300      	bres	21248,#0
 297                     ; 35 	TIM2->PSCR = 0;    //不分频
 299  0004 725f530e      	clr	21262
 300                     ; 36 	TIM2->ARRH = (F_OSC/FPWM)>>8; 
 302  0008 3501530f      	mov	21263,#1
 303                     ; 37 	TIM2->ARRL = (F_OSC/FPWM)&0xff;
 305  000c 35a55310      	mov	21264,#165
 306                     ; 45 	TIM2->CCR2H = (F_OSC/FPWM/4)>>8;
 308  0010 725f5313      	clr	21267
 309                     ; 46 	TIM2->CCR2L = (F_OSC/FPWM/4)&0xff;	//占空比1/4
 311  0014 35695314      	mov	21268,#105
 312                     ; 49 	TIM2->CCMR2 = 0x60;  //PWM模式1
 314  0018 35605308      	mov	21256,#96
 315                     ; 55 	SetIR(0);
 317  001c 7217500f      	bres	20495,#3
 318                     ; 59 }
 322  0020 81            	ret
 377                     ; 60 void IRCmd(FunctionalState NewState)
 377                     ; 61 {
 378                     .text:	section	.text,new
 379  0000               _IRCmd:
 383                     ; 63 	if (NewState==DISABLE) 
 385  0000 4d            	tnz	a
 386  0001 260e          	jrne	L321
 387                     ; 65 		TIM2->CCER1 &= ~(1<<4); //禁止OC4
 389  0003 7219530a      	bres	21258,#4
 390                     ; 66 		TIM2->CR1 &= ~(1<<0);    //禁止TIM2
 392  0007 72115300      	bres	21248,#0
 393                     ; 71 		SetIR(0);
 395  000b 7217500f      	bres	20495,#3
 398  000f 200c          	jra	L521
 399  0011               L321:
 400                     ; 77 		TIM2->CCER1 |= 1<<4; //使能OC4
 402  0011 7218530a      	bset	21258,#4
 403                     ; 78 		TIM2->CR1 |= 1;    //使能TIM2
 405  0015 72105300      	bset	21248,#0
 406                     ; 79 		SetIR(1);
 408  0019 7216500f      	bset	20495,#3
 409  001d               L521:
 410                     ; 81 }
 413  001d 81            	ret
 508                     ; 84 void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,bool mode)
 508                     ; 85 { 
 509                     .text:	section	.text,new
 510  0000               _IrDi_send_data:
 512  0000 89            	pushw	x
 513  0001 89            	pushw	x
 514       00000002      OFST:	set	2
 517                     ; 88 	SetIRCNT(mode)		//按键时输出0:禁止正面红外灯发射；定位自动发送此端口为输入，允许两个红外管外射
 519  0002 0d08          	tnz	(OFST+6,sp)
 520  0004 2706          	jreq	L571
 523  0006 72175002      	bres	20482,#3
 525  000a 2004          	jra	L771
 526  000c               L571:
 529  000c 72165002      	bset	20482,#3
 530  0010               L771:
 531                     ; 89 	IrDi_init();
 533  0010 cd0000        	call	_IrDi_init
 535                     ; 90 	for (k=0;k<size;k++)
 537  0013 0f02          	clr	(OFST+0,sp)
 539  0015 204d          	jra	L502
 540  0017               L102:
 541                     ; 93 		IRCmd(ENABLE);
 543  0017 a601          	ld	a,#1
 544  0019 cd0000        	call	_IRCmd
 546                     ; 94 	  IrDi_800us();
 548  001c cd0000        	call	_IrDi_800us
 550                     ; 96 	  for(i=0;i<8;i++)
 552  001f 0f01          	clr	(OFST-1,sp)
 553  0021               L112:
 554                     ; 98 			if((IrDa_data[k]&0x01)==0x01)
 556  0021 7b03          	ld	a,(OFST+1,sp)
 557  0023 97            	ld	xl,a
 558  0024 7b04          	ld	a,(OFST+2,sp)
 559  0026 1b02          	add	a,(OFST+0,sp)
 560  0028 2401          	jrnc	L61
 561  002a 5c            	incw	x
 562  002b               L61:
 563  002b 02            	rlwa	x,a
 564  002c f6            	ld	a,(x)
 565  002d a401          	and	a,#1
 566  002f a101          	cp	a,#1
 567  0031 260a          	jrne	L712
 568                     ; 100 				IRCmd(ENABLE);
 570  0033 a601          	ld	a,#1
 571  0035 cd0000        	call	_IRCmd
 573                     ; 101 				IrDi_800us();
 575  0038 cd0000        	call	_IrDi_800us
 578  003b 2007          	jra	L122
 579  003d               L712:
 580                     ; 105 				IRCmd(DISABLE);
 582  003d 4f            	clr	a
 583  003e cd0000        	call	_IRCmd
 585                     ; 106 				IrDi_800us();
 587  0041 cd0000        	call	_IrDi_800us
 589  0044               L122:
 590                     ; 108 			IrDa_data[k]>>=1;
 592  0044 7b03          	ld	a,(OFST+1,sp)
 593  0046 97            	ld	xl,a
 594  0047 7b04          	ld	a,(OFST+2,sp)
 595  0049 1b02          	add	a,(OFST+0,sp)
 596  004b 2401          	jrnc	L02
 597  004d 5c            	incw	x
 598  004e               L02:
 599  004e 02            	rlwa	x,a
 600  004f 74            	srl	(x)
 601                     ; 96 	  for(i=0;i<8;i++)
 603  0050 0c01          	inc	(OFST-1,sp)
 606  0052 7b01          	ld	a,(OFST-1,sp)
 607  0054 a108          	cp	a,#8
 608  0056 25c9          	jrult	L112
 609                     ; 111 		IRCmd(DISABLE);
 611  0058 4f            	clr	a
 612  0059 cd0000        	call	_IRCmd
 614                     ; 112 	  IrDi_800us();
 616  005c cd0000        	call	_IrDi_800us
 618                     ; 113 		IrDi_800us();
 620  005f cd0000        	call	_IrDi_800us
 622                     ; 90 	for (k=0;k<size;k++)
 624  0062 0c02          	inc	(OFST+0,sp)
 625  0064               L502:
 628  0064 7b02          	ld	a,(OFST+0,sp)
 629  0066 1107          	cp	a,(OFST+5,sp)
 630  0068 25ad          	jrult	L102
 631                     ; 115 	SetIRCNT(1);
 633  006a 72175002      	bres	20482,#3
 634                     ; 116 }
 638  006e 5b04          	addw	sp,#4
 639  0070 81            	ret
 684                     ; 120 void Ir_send_start(void)
 684                     ; 121 {
 685                     .text:	section	.text,new
 686  0000               _Ir_send_start:
 688  0000 5203          	subw	sp,#3
 689       00000003      OFST:	set	3
 692                     ; 123 	uint16_t x=0;
 694  0002 5f            	clrw	x
 695  0003 1f01          	ldw	(OFST-2,sp),x
 696                     ; 143 	IRCmd(ENABLE);
 698  0005 a601          	ld	a,#1
 699  0007 cd0000        	call	_IRCmd
 701                     ; 144 	for(i=0;i<15;i++)
 703  000a 0f03          	clr	(OFST+0,sp)
 704  000c               L542:
 705                     ; 146 		IrDi_600us();
 707  000c cd0000        	call	_IrDi_600us
 709                     ; 144 	for(i=0;i<15;i++)
 711  000f 0c03          	inc	(OFST+0,sp)
 714  0011 7b03          	ld	a,(OFST+0,sp)
 715  0013 a10f          	cp	a,#15
 716  0015 25f5          	jrult	L542
 717                     ; 148 	IRCmd(DISABLE);
 719  0017 4f            	clr	a
 720  0018 cd0000        	call	_IRCmd
 722                     ; 150 	for(i=0;i<8;i++)
 724  001b 0f03          	clr	(OFST+0,sp)
 725  001d               L352:
 726                     ; 152 		IrDi_600us();
 728  001d cd0000        	call	_IrDi_600us
 730                     ; 150 	for(i=0;i<8;i++)
 732  0020 0c03          	inc	(OFST+0,sp)
 735  0022 7b03          	ld	a,(OFST+0,sp)
 736  0024 a108          	cp	a,#8
 737  0026 25f5          	jrult	L352
 738                     ; 158 }
 741  0028 5b03          	addw	sp,#3
 742  002a 81            	ret
 805                     ; 163 void Ir_send_data(uint8_t send_data)	
 805                     ; 164 {
 806                     .text:	section	.text,new
 807  0000               _Ir_send_data:
 809  0000 88            	push	a
 810  0001 5204          	subw	sp,#4
 811       00000004      OFST:	set	4
 814                     ; 166 	uint16_t x=0;
 816  0003 5f            	clrw	x
 817  0004 1f01          	ldw	(OFST-3,sp),x
 818                     ; 197 	for(i=0;i<8;i++)
 820  0006 0f03          	clr	(OFST-1,sp)
 821  0008               L313:
 822                     ; 199 		IRCmd(ENABLE);
 824  0008 a601          	ld	a,#1
 825  000a cd0000        	call	_IRCmd
 827                     ; 200 		IrDi_600us();
 829  000d cd0000        	call	_IrDi_600us
 831                     ; 201 		IRCmd(DISABLE);
 833  0010 4f            	clr	a
 834  0011 cd0000        	call	_IRCmd
 836                     ; 204 		if(send_data &0x01)
 838  0014 7b05          	ld	a,(OFST+1,sp)
 839  0016 a501          	bcp	a,#1
 840  0018 270f          	jreq	L123
 841                     ; 206 			for(j=0;j<3;j++)
 843  001a 0f04          	clr	(OFST+0,sp)
 844  001c               L323:
 845                     ; 208 				IrDi_600us();
 847  001c cd0000        	call	_IrDi_600us
 849                     ; 206 			for(j=0;j<3;j++)
 851  001f 0c04          	inc	(OFST+0,sp)
 854  0021 7b04          	ld	a,(OFST+0,sp)
 855  0023 a103          	cp	a,#3
 856  0025 25f5          	jrult	L323
 858  0027 2003          	jra	L133
 859  0029               L123:
 860                     ; 214 			IrDi_600us();		
 862  0029 cd0000        	call	_IrDi_600us
 864  002c               L133:
 865                     ; 217 		send_data >>=1;
 867  002c 0405          	srl	(OFST+1,sp)
 868                     ; 197 	for(i=0;i<8;i++)
 870  002e 0c03          	inc	(OFST-1,sp)
 873  0030 7b03          	ld	a,(OFST-1,sp)
 874  0032 a108          	cp	a,#8
 875  0034 25d2          	jrult	L313
 876                     ; 220 }
 879  0036 5b05          	addw	sp,#5
 880  0038 81            	ret
 916                     ; 224 void Ir_send_end(void)
 916                     ; 225 {
 917                     .text:	section	.text,new
 918  0000               _Ir_send_end:
 920  0000 89            	pushw	x
 921       00000002      OFST:	set	2
 924                     ; 226 	uint16_t x=0;
 926  0001 5f            	clrw	x
 927  0002 1f01          	ldw	(OFST-1,sp),x
 928                     ; 235 	IRCmd(ENABLE);
 930  0004 a601          	ld	a,#1
 931  0006 cd0000        	call	_IRCmd
 933                     ; 236 	IrDi_600us();	
 935  0009 cd0000        	call	_IrDi_600us
 937                     ; 237 	IRCmd(DISABLE); 
 939  000c 4f            	clr	a
 940  000d cd0000        	call	_IRCmd
 942                     ; 239 }
 945  0010 85            	popw	x
 946  0011 81            	ret
 991                     ; 242 void Ir_send_repeat(void)
 991                     ; 243 {
 992                     .text:	section	.text,new
 993  0000               _Ir_send_repeat:
 995  0000 5203          	subw	sp,#3
 996       00000003      OFST:	set	3
 999                     ; 244 	uint16_t x=0;
1001  0002 5f            	clrw	x
1002  0003 1f01          	ldw	(OFST-2,sp),x
1003                     ; 265 	IRCmd(ENABLE);
1005  0005 a601          	ld	a,#1
1006  0007 cd0000        	call	_IRCmd
1008                     ; 266 	for(i=0;i<15;i++) IrDi_600us();
1010  000a 0f03          	clr	(OFST+0,sp)
1011  000c               L373:
1014  000c cd0000        	call	_IrDi_600us
1018  000f 0c03          	inc	(OFST+0,sp)
1021  0011 7b03          	ld	a,(OFST+0,sp)
1022  0013 a10f          	cp	a,#15
1023  0015 25f5          	jrult	L373
1024                     ; 267 	IRCmd(DISABLE);	
1026  0017 4f            	clr	a
1027  0018 cd0000        	call	_IRCmd
1029                     ; 269 	for(i=0;i<4;i++) IrDi_600us();
1031  001b 0f03          	clr	(OFST+0,sp)
1032  001d               L104:
1035  001d cd0000        	call	_IrDi_600us
1039  0020 0c03          	inc	(OFST+0,sp)
1042  0022 7b03          	ld	a,(OFST+0,sp)
1043  0024 a104          	cp	a,#4
1044  0026 25f5          	jrult	L104
1045                     ; 271 	IRCmd(ENABLE);
1047  0028 a601          	ld	a,#1
1048  002a cd0000        	call	_IRCmd
1050                     ; 272 	IrDi_600us();
1052  002d cd0000        	call	_IrDi_600us
1054                     ; 273 	IRCmd(DISABLE);		
1056  0030 4f            	clr	a
1057  0031 cd0000        	call	_IRCmd
1059                     ; 276 }
1062  0034 5b03          	addw	sp,#3
1063  0036 81            	ret
1114                     	xdef	_IRCmd
1115                     	xdef	_IrDi_init
1116                     	xdef	_IrDi_800us
1117                     	xdef	_IrDi_600us
1118                     	xdef	_Ir_send_end
1119                     	xdef	_Ir_send_repeat
1120                     	xdef	_Ir_send_data
1121                     	xdef	_Ir_send_start
1122                     	xdef	_IrDi_send_data
1123                     	xdef	_Ir_repeat_timer
1124                     	xdef	_Ir_timer
1125                     	xdef	_sm_state
1126                     	xdef	_IrState
1145                     	end
