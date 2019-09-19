   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  34                     ; 14 @far @interrupt void NonHandledInterrupt (void)
  34                     ; 15 {
  35                     .text:	section	.text,new
  36  0000               f_NonHandledInterrupt:
  41                     ; 19 	return;
  44  0000 80            	iret
  66                     ; 24 @far @interrupt void AWU_IT (void){
  67                     .text:	section	.text,new
  68  0000               f_AWU_IT:
  73                     ; 26   (void) AWU->CSR;
  75  0000 c650f0        	ld	a,20720
  76  0003 97            	ld	xl,a
  77                     ; 27   return;
  80  0004 80            	iret
 102                     ; 30 @far @interrupt void PortAInterrupt (void){
 103                     .text:	section	.text,new
 104  0000               f_PortAInterrupt:
 109                     ; 31 	return;
 112  0000 80            	iret
 134                     ; 34 @far @interrupt void PortCInterrupt (void){
 135                     .text:	section	.text,new
 136  0000               f_PortCInterrupt:
 141                     ; 35 	return;
 144  0000 80            	iret
 168                     ; 37 @far @interrupt void PortDInterrupt (void){
 169                     .text:	section	.text,new
 170  0000               f_PortDInterrupt:
 173  0000 3b0002        	push	c_x+2
 174  0003 be00          	ldw	x,c_x
 175  0005 89            	pushw	x
 176  0006 3b0002        	push	c_y+2
 177  0009 be00          	ldw	x,c_y
 178  000b 89            	pushw	x
 181                     ; 38 	if(GetGDO0()==0)
 183  000c c65010        	ld	a,20496
 184  000f a510          	bcp	a,#16
 185  0011 2609          	jrne	L16
 186                     ; 40 		UART1_SendData8(0X23);
 188  0013 a623          	ld	a,#35
 189  0015 cd0000        	call	_UART1_SendData8
 191                     ; 41 		rfirq=1; //无线中断
 193  0018 35010000      	mov	_rfirq,#1
 194  001c               L16:
 195                     ; 43 	return;
 198  001c 85            	popw	x
 199  001d bf00          	ldw	c_y,x
 200  001f 320002        	pop	c_y+2
 201  0022 85            	popw	x
 202  0023 bf00          	ldw	c_x,x
 203  0025 320002        	pop	c_x+2
 204  0028 80            	iret
 228                     ; 47 @far @interrupt void PortBInterrupt (void){
 229                     .text:	section	.text,new
 230  0000               f_PortBInterrupt:
 235                     ; 48 	audio_irq=1;
 237  0000 35010000      	mov	_audio_irq,#1
 238                     ; 49 	audio_play=0;//声音播放结束
 240  0004 3f00          	clr	_audio_play
 241                     ; 50 }
 244  0006 80            	iret
 267                     ; 53 @far @interrupt void TIM2UpdateInterrupt(void)
 267                     ; 54 {
 268                     .text:	section	.text,new
 269  0000               f_TIM2UpdateInterrupt:
 274                     ; 55 }
 277  0000 80            	iret
 303                     ; 57 @far @interrupt void TIM4UpdateInterrupt(void)
 303                     ; 58 {
 304                     .text:	section	.text,new
 305  0000               f_TIM4UpdateInterrupt:
 310                     ; 60 	TIM4->SR1=0x00;//?清除更新标志
 312  0000 725f5344      	clr	21316
 313                     ; 75 	if(Ir_timer)
 315  0004 3d00          	tnz	_Ir_timer
 316  0006 2702          	jreq	L311
 317                     ; 77 		Ir_timer--;
 319  0008 3a00          	dec	_Ir_timer
 320  000a               L311:
 321                     ; 81 	if(Ir_repeat_timer)
 323  000a 3d00          	tnz	_Ir_repeat_timer
 324  000c 2708          	jreq	L511
 325                     ; 83 		if(--Ir_repeat_timer == 0)
 327  000e 3a00          	dec	_Ir_repeat_timer
 328  0010 2604          	jrne	L511
 329                     ; 85 			SetIrState(IR_REPEAT);
 331  0012 72160000      	bset	_IrState,#3
 332  0016               L511:
 333                     ; 88 }
 336  0016 80            	iret
 338                     .const:	section	.text
 339  0000               __vectab:
 340  0000 82            	dc.b	130
 342  0001 00            	dc.b	page(__stext)
 343  0002 0000          	dc.w	__stext
 344  0004 82            	dc.b	130
 346  0005 00            	dc.b	page(f_NonHandledInterrupt)
 347  0006 0000          	dc.w	f_NonHandledInterrupt
 348  0008 82            	dc.b	130
 350  0009 00            	dc.b	page(f_NonHandledInterrupt)
 351  000a 0000          	dc.w	f_NonHandledInterrupt
 352  000c 82            	dc.b	130
 354  000d 00            	dc.b	page(f_AWU_IT)
 355  000e 0000          	dc.w	f_AWU_IT
 356  0010 82            	dc.b	130
 358  0011 00            	dc.b	page(f_NonHandledInterrupt)
 359  0012 0000          	dc.w	f_NonHandledInterrupt
 360  0014 82            	dc.b	130
 362  0015 00            	dc.b	page(f_PortAInterrupt)
 363  0016 0000          	dc.w	f_PortAInterrupt
 364  0018 82            	dc.b	130
 366  0019 00            	dc.b	page(f_PortBInterrupt)
 367  001a 0000          	dc.w	f_PortBInterrupt
 368  001c 82            	dc.b	130
 370  001d 00            	dc.b	page(f_PortCInterrupt)
 371  001e 0000          	dc.w	f_PortCInterrupt
 372  0020 82            	dc.b	130
 374  0021 00            	dc.b	page(f_PortDInterrupt)
 375  0022 0000          	dc.w	f_PortDInterrupt
 376  0024 82            	dc.b	130
 378  0025 00            	dc.b	page(f_NonHandledInterrupt)
 379  0026 0000          	dc.w	f_NonHandledInterrupt
 380  0028 82            	dc.b	130
 382  0029 00            	dc.b	page(f_NonHandledInterrupt)
 383  002a 0000          	dc.w	f_NonHandledInterrupt
 384  002c 82            	dc.b	130
 386  002d 00            	dc.b	page(f_NonHandledInterrupt)
 387  002e 0000          	dc.w	f_NonHandledInterrupt
 388  0030 82            	dc.b	130
 390  0031 00            	dc.b	page(f_NonHandledInterrupt)
 391  0032 0000          	dc.w	f_NonHandledInterrupt
 392  0034 82            	dc.b	130
 394  0035 00            	dc.b	page(f_NonHandledInterrupt)
 395  0036 0000          	dc.w	f_NonHandledInterrupt
 396  0038 82            	dc.b	130
 398  0039 00            	dc.b	page(f_NonHandledInterrupt)
 399  003a 0000          	dc.w	f_NonHandledInterrupt
 400  003c 82            	dc.b	130
 402  003d 00            	dc.b	page(f_TIM2UpdateInterrupt)
 403  003e 0000          	dc.w	f_TIM2UpdateInterrupt
 404  0040 82            	dc.b	130
 406  0041 00            	dc.b	page(f_NonHandledInterrupt)
 407  0042 0000          	dc.w	f_NonHandledInterrupt
 408  0044 82            	dc.b	130
 410  0045 00            	dc.b	page(f_NonHandledInterrupt)
 411  0046 0000          	dc.w	f_NonHandledInterrupt
 412  0048 82            	dc.b	130
 414  0049 00            	dc.b	page(f_NonHandledInterrupt)
 415  004a 0000          	dc.w	f_NonHandledInterrupt
 416  004c 82            	dc.b	130
 418  004d 00            	dc.b	page(f_NonHandledInterrupt)
 419  004e 0000          	dc.w	f_NonHandledInterrupt
 420  0050 82            	dc.b	130
 422  0051 00            	dc.b	page(f_NonHandledInterrupt)
 423  0052 0000          	dc.w	f_NonHandledInterrupt
 424  0054 82            	dc.b	130
 426  0055 00            	dc.b	page(f_NonHandledInterrupt)
 427  0056 0000          	dc.w	f_NonHandledInterrupt
 428  0058 82            	dc.b	130
 430  0059 00            	dc.b	page(f_NonHandledInterrupt)
 431  005a 0000          	dc.w	f_NonHandledInterrupt
 432  005c 82            	dc.b	130
 434  005d 00            	dc.b	page(f_NonHandledInterrupt)
 435  005e 0000          	dc.w	f_NonHandledInterrupt
 436  0060 82            	dc.b	130
 438  0061 00            	dc.b	page(f_NonHandledInterrupt)
 439  0062 0000          	dc.w	f_NonHandledInterrupt
 440  0064 82            	dc.b	130
 442  0065 00            	dc.b	page(f_TIM4UpdateInterrupt)
 443  0066 0000          	dc.w	f_TIM4UpdateInterrupt
 444  0068 82            	dc.b	130
 446  0069 00            	dc.b	page(f_NonHandledInterrupt)
 447  006a 0000          	dc.w	f_NonHandledInterrupt
 448  006c 82            	dc.b	130
 450  006d 00            	dc.b	page(f_NonHandledInterrupt)
 451  006e 0000          	dc.w	f_NonHandledInterrupt
 452  0070 82            	dc.b	130
 454  0071 00            	dc.b	page(f_NonHandledInterrupt)
 455  0072 0000          	dc.w	f_NonHandledInterrupt
 456  0074 82            	dc.b	130
 458  0075 00            	dc.b	page(f_NonHandledInterrupt)
 459  0076 0000          	dc.w	f_NonHandledInterrupt
 460  0078 82            	dc.b	130
 462  0079 00            	dc.b	page(f_NonHandledInterrupt)
 463  007a 0000          	dc.w	f_NonHandledInterrupt
 464  007c 82            	dc.b	130
 466  007d 00            	dc.b	page(f_NonHandledInterrupt)
 467  007e 0000          	dc.w	f_NonHandledInterrupt
 518                     	xdef	__vectab
 519                     	xref	__stext
 520                     	xdef	f_TIM4UpdateInterrupt
 521                     	xdef	f_TIM2UpdateInterrupt
 522                     	xdef	f_PortBInterrupt
 523                     	xref.b	_audio_irq
 524                     	xref.b	_audio_play
 525                     	xdef	f_PortDInterrupt
 526                     	xdef	f_PortCInterrupt
 527                     	xdef	f_PortAInterrupt
 528                     	xdef	f_AWU_IT
 529                     	xdef	f_NonHandledInterrupt
 530                     	xref.b	_Ir_repeat_timer
 531                     	xref.b	_Ir_timer
 532                     	xref.b	_IrState
 533                     	xref.b	_rfirq
 534                     	xref	_UART1_SendData8
 535                     	xref.b	c_x
 536                     	xref.b	c_y
 555                     	end
