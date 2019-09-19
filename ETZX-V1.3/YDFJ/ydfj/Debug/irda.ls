   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  52                     ; 3 void IrDi_800us(void)
  52                     ; 4 {
  54                     	switch	.text
  55  0000               _IrDi_800us:
  57  0000 88            	push	a
  58       00000001      OFST:	set	1
  61                     ; 5 	unsigned char i=240;
  63  0001 a6f0          	ld	a,#240
  64  0003 6b01          	ld	(OFST+0,sp),a
  66  0005 2009          	jra	L33
  67  0007               L72:
  68                     ; 6 	while(i--){nop();nop();nop();nop();nop();nop();nop();nop();nop();}
  71  0007 9d            nop
  77  0008 9d            nop
  83  0009 9d            nop
  89  000a 9d            nop
  95  000b 9d            nop
 101  000c 9d            nop
 107  000d 9d            nop
 113  000e 9d            nop
 119  000f 9d            nop
 122  0010               L33:
 125  0010 7b01          	ld	a,(OFST+0,sp)
 126  0012 0a01          	dec	(OFST+0,sp)
 127  0014 4d            	tnz	a
 128  0015 26f0          	jrne	L72
 129                     ; 7 }
 132  0017 84            	pop	a
 133  0018 81            	ret
 156                     ; 10 void IrDi_init(void) //初始化PWM输出用作数据发送，FSK 为38KHZ
 156                     ; 11 {
 157                     	switch	.text
 158  0019               _IrDi_init:
 162                     ; 12 	TIM2->CR1 &= ~1; //关闭TIM1
 164  0019 72115300      	bres	21248,#0
 165                     ; 13 	TIM2->PSCR = 0;    //不分频
 167  001d 725f530e      	clr	21262
 168                     ; 14 	TIM2->ARRH = (F_OSC/FPWM)>>8; 
 170  0021 3501530f      	mov	21263,#1
 171                     ; 15 	TIM2->ARRL = (F_OSC/FPWM)&0xff;
 173  0025 35a55310      	mov	21264,#165
 174                     ; 16 	TIM2->CCR2H = (F_OSC/FPWM/2)>>8;
 176  0029 725f5313      	clr	21267
 177                     ; 17 	TIM2->CCR2L = (F_OSC/FPWM/2)&0xff;  //占空比50%
 179  002d 35d25314      	mov	21268,#210
 180                     ; 18 	TIM2->CCMR2 = 0x60;  //PWM模式1
 182  0031 35605308      	mov	21256,#96
 183                     ; 20 	SetIR(0);
 185  0035 7217500f      	bres	20495,#3
 186                     ; 23 }
 190  0039 81            	ret
 245                     ; 24 void IRCmd(FunctionalState NewState)
 245                     ; 25 {
 246                     	switch	.text
 247  003a               _IRCmd:
 251                     ; 27 	if (NewState==DISABLE) 
 253  003a 4d            	tnz	a
 254  003b 260e          	jrne	L57
 255                     ; 29 		TIM2->CCER1 &= ~(1<<4); //禁止OC4
 257  003d 7219530a      	bres	21258,#4
 258                     ; 30 		TIM2->CR1 &= ~(1<<0);    //禁止TIM1
 260  0041 72115300      	bres	21248,#0
 261                     ; 31 		SetIR(0);
 263  0045 7217500f      	bres	20495,#3
 266  0049 200c          	jra	L77
 267  004b               L57:
 268                     ; 35 		TIM2->CCER1 |= 1<<4; //使能OC4
 270  004b 7218530a      	bset	21258,#4
 271                     ; 36 		TIM2->CR1 |= 1;    //使能TIM1
 273  004f 72105300      	bset	21248,#0
 274                     ; 37 		SetIR(1);
 276  0053 7216500f      	bset	20495,#3
 277  0057               L77:
 278                     ; 39 }
 281  0057 81            	ret
 376                     ; 42 void IrDi_send_data(unsigned char * IrDa_data,unsigned char size,bool mode)
 376                     ; 43 { 
 377                     	switch	.text
 378  0058               _IrDi_send_data:
 380  0058 89            	pushw	x
 381  0059 89            	pushw	x
 382       00000002      OFST:	set	2
 385                     ; 46 	SetIRCNT(mode)
 387  005a 0d08          	tnz	(OFST+6,sp)
 388  005c 2706          	jreq	L741
 391  005e 72175002      	bres	20482,#3
 393  0062 2004          	jra	L151
 394  0064               L741:
 397  0064 72165002      	bset	20482,#3
 398  0068               L151:
 399                     ; 47 	IrDi_init();
 401  0068 adaf          	call	_IrDi_init
 403                     ; 48 	for (k=0;k<size;k++)
 405  006a 0f02          	clr	(OFST+0,sp)
 407  006c 2048          	jra	L751
 408  006e               L351:
 409                     ; 51 		IRCmd(ENABLE);
 411  006e a601          	ld	a,#1
 412  0070 adc8          	call	_IRCmd
 414                     ; 52 	  IrDi_800us();
 416  0072 ad8c          	call	_IrDi_800us
 418                     ; 54 	  for(i=0;i<8;i++)
 420  0074 0f01          	clr	(OFST-1,sp)
 421  0076               L361:
 422                     ; 56 			if((IrDa_data[k]&0x01)==0x01)
 424  0076 7b03          	ld	a,(OFST+1,sp)
 425  0078 97            	ld	xl,a
 426  0079 7b04          	ld	a,(OFST+2,sp)
 427  007b 1b02          	add	a,(OFST+0,sp)
 428  007d 2401          	jrnc	L41
 429  007f 5c            	incw	x
 430  0080               L41:
 431  0080 02            	rlwa	x,a
 432  0081 f6            	ld	a,(x)
 433  0082 a401          	and	a,#1
 434  0084 a101          	cp	a,#1
 435  0086 2609          	jrne	L171
 436                     ; 58 				IRCmd(ENABLE);
 438  0088 a601          	ld	a,#1
 439  008a adae          	call	_IRCmd
 441                     ; 59 				IrDi_800us();
 443  008c cd0000        	call	_IrDi_800us
 446  008f 2006          	jra	L371
 447  0091               L171:
 448                     ; 63 				IRCmd(DISABLE);
 450  0091 4f            	clr	a
 451  0092 ada6          	call	_IRCmd
 453                     ; 64 				IrDi_800us();
 455  0094 cd0000        	call	_IrDi_800us
 457  0097               L371:
 458                     ; 66 			IrDa_data[k]>>=1;
 460  0097 7b03          	ld	a,(OFST+1,sp)
 461  0099 97            	ld	xl,a
 462  009a 7b04          	ld	a,(OFST+2,sp)
 463  009c 1b02          	add	a,(OFST+0,sp)
 464  009e 2401          	jrnc	L61
 465  00a0 5c            	incw	x
 466  00a1               L61:
 467  00a1 02            	rlwa	x,a
 468  00a2 74            	srl	(x)
 469                     ; 54 	  for(i=0;i<8;i++)
 471  00a3 0c01          	inc	(OFST-1,sp)
 474  00a5 7b01          	ld	a,(OFST-1,sp)
 475  00a7 a108          	cp	a,#8
 476  00a9 25cb          	jrult	L361
 477                     ; 69 		IRCmd(DISABLE);
 479  00ab 4f            	clr	a
 480  00ac ad8c          	call	_IRCmd
 482                     ; 70 	  IrDi_800us();
 484  00ae cd0000        	call	_IrDi_800us
 486                     ; 71 		IrDi_800us();
 488  00b1 cd0000        	call	_IrDi_800us
 490                     ; 48 	for (k=0;k<size;k++)
 492  00b4 0c02          	inc	(OFST+0,sp)
 493  00b6               L751:
 496  00b6 7b02          	ld	a,(OFST+0,sp)
 497  00b8 1107          	cp	a,(OFST+5,sp)
 498  00ba 25b2          	jrult	L351
 499                     ; 73 	SetIRCNT(1);
 501  00bc 72175002      	bres	20482,#3
 502                     ; 74 }
 506  00c0 5b04          	addw	sp,#4
 507  00c2 81            	ret
 520                     	xdef	_IRCmd
 521                     	xdef	_IrDi_init
 522                     	xdef	_IrDi_800us
 523                     	xdef	_IrDi_send_data
 542                     	end
