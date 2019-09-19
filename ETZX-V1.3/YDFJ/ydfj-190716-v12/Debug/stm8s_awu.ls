   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     .const:	section	.text
   5  0000               _APR_Array:
   6  0000 00            	dc.b	0
   7  0001 1e            	dc.b	30
   8  0002 1e            	dc.b	30
   9  0003 1e            	dc.b	30
  10  0004 1e            	dc.b	30
  11  0005 1e            	dc.b	30
  12  0006 1e            	dc.b	30
  13  0007 1e            	dc.b	30
  14  0008 1e            	dc.b	30
  15  0009 1e            	dc.b	30
  16  000a 1e            	dc.b	30
  17  000b 1e            	dc.b	30
  18  000c 1e            	dc.b	30
  19  000d 3d            	dc.b	61
  20  000e 17            	dc.b	23
  21  000f 17            	dc.b	23
  22  0010 3e            	dc.b	62
  23  0011               _TBR_Array:
  24  0011 00            	dc.b	0
  25  0012 01            	dc.b	1
  26  0013 02            	dc.b	2
  27  0014 03            	dc.b	3
  28  0015 04            	dc.b	4
  29  0016 05            	dc.b	5
  30  0017 06            	dc.b	6
  31  0018 07            	dc.b	7
  32  0019 08            	dc.b	8
  33  001a 09            	dc.b	9
  34  001b 0a            	dc.b	10
  35  001c 0b            	dc.b	11
  36  001d 0c            	dc.b	12
  37  001e 0c            	dc.b	12
  38  001f 0e            	dc.b	14
  39  0020 0f            	dc.b	15
  40  0021 0f            	dc.b	15
  69                     ; 67 void AWU_DeInit(void)
  69                     ; 68 {
  71                     .text:	section	.text,new
  72  0000               _AWU_DeInit:
  76                     ; 69     AWU->CSR = AWU_CSR_RESET_VALUE;
  78  0000 725f50f0      	clr	20720
  79                     ; 70     AWU->APR = AWU_APR_RESET_VALUE;
  81  0004 353f50f1      	mov	20721,#63
  82                     ; 71     AWU->TBR = AWU_TBR_RESET_VALUE;
  84  0008 725f50f2      	clr	20722
  85                     ; 72 }
  88  000c 81            	ret
 250                     ; 82 void AWU_Init(AWU_Timebase_TypeDef AWU_TimeBase)
 250                     ; 83 {
 251                     .text:	section	.text,new
 252  0000               _AWU_Init:
 254  0000 88            	push	a
 255       00000000      OFST:	set	0
 258                     ; 86     assert_param(IS_AWU_TIMEBASE_OK(AWU_TimeBase));
 260                     ; 89     AWU->CSR |= AWU_CSR_AWUEN;
 262  0001 721850f0      	bset	20720,#4
 263                     ; 92     AWU->TBR &= (uint8_t)(~AWU_TBR_AWUTB);
 265  0005 c650f2        	ld	a,20722
 266  0008 a4f0          	and	a,#240
 267  000a c750f2        	ld	20722,a
 268                     ; 93     AWU->TBR |= TBR_Array[(uint8_t)AWU_TimeBase];
 270  000d 7b01          	ld	a,(OFST+1,sp)
 271  000f 5f            	clrw	x
 272  0010 97            	ld	xl,a
 273  0011 c650f2        	ld	a,20722
 274  0014 da0011        	or	a,(_TBR_Array,x)
 275  0017 c750f2        	ld	20722,a
 276                     ; 96     AWU->APR &= (uint8_t)(~AWU_APR_APR);
 278  001a c650f1        	ld	a,20721
 279  001d a4c0          	and	a,#192
 280  001f c750f1        	ld	20721,a
 281                     ; 97     AWU->APR |= APR_Array[(uint8_t)AWU_TimeBase];
 283  0022 7b01          	ld	a,(OFST+1,sp)
 284  0024 5f            	clrw	x
 285  0025 97            	ld	xl,a
 286  0026 c650f1        	ld	a,20721
 287  0029 da0000        	or	a,(_APR_Array,x)
 288  002c c750f1        	ld	20721,a
 289                     ; 99 }
 292  002f 84            	pop	a
 293  0030 81            	ret
 348                     ; 108 void AWU_Cmd(FunctionalState NewState)
 348                     ; 109 {
 349                     .text:	section	.text,new
 350  0000               _AWU_Cmd:
 354                     ; 110     if (NewState != DISABLE)
 356  0000 4d            	tnz	a
 357  0001 2706          	jreq	L331
 358                     ; 113         AWU->CSR |= AWU_CSR_AWUEN;
 360  0003 721850f0      	bset	20720,#4
 362  0007 2004          	jra	L531
 363  0009               L331:
 364                     ; 118         AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
 366  0009 721950f0      	bres	20720,#4
 367  000d               L531:
 368                     ; 120 }
 371  000d 81            	ret
 424                     	switch	.const
 425  0022               L41:
 426  0022 000003e8      	dc.l	1000
 427                     ; 135 void AWU_LSICalibrationConfig(uint32_t LSIFreqHz)
 427                     ; 136 {
 428                     .text:	section	.text,new
 429  0000               _AWU_LSICalibrationConfig:
 431  0000 5206          	subw	sp,#6
 432       00000006      OFST:	set	6
 435                     ; 138     uint16_t lsifreqkhz = 0x0;
 437  0002 1e03          	ldw	x,(OFST-3,sp)
 438                     ; 139     uint16_t A = 0x0;
 440  0004 1e05          	ldw	x,(OFST-1,sp)
 441                     ; 142     assert_param(IS_LSI_FREQUENCY_OK(LSIFreqHz));
 443                     ; 144     lsifreqkhz = (uint16_t)(LSIFreqHz / 1000); /* Converts value in kHz */
 445  0006 96            	ldw	x,sp
 446  0007 1c0009        	addw	x,#OFST+3
 447  000a cd0000        	call	c_ltor
 449  000d ae0022        	ldw	x,#L41
 450  0010 cd0000        	call	c_ludv
 452  0013 be02          	ldw	x,c_lreg+2
 453  0015 1f03          	ldw	(OFST-3,sp),x
 454                     ; 148     A = (uint16_t)(lsifreqkhz >> 2U); /* Division by 4, keep integer part only */
 456  0017 1e03          	ldw	x,(OFST-3,sp)
 457  0019 54            	srlw	x
 458  001a 54            	srlw	x
 459  001b 1f05          	ldw	(OFST-1,sp),x
 460                     ; 150     if ((4U * A) >= ((lsifreqkhz - (4U * A)) * (1U + (2U * A))))
 462  001d 1e05          	ldw	x,(OFST-1,sp)
 463  001f 58            	sllw	x
 464  0020 58            	sllw	x
 465  0021 1f01          	ldw	(OFST-5,sp),x
 466  0023 1e03          	ldw	x,(OFST-3,sp)
 467  0025 72f001        	subw	x,(OFST-5,sp)
 468  0028 1605          	ldw	y,(OFST-1,sp)
 469  002a 9058          	sllw	y
 470  002c 905c          	incw	y
 471  002e cd0000        	call	c_imul
 473  0031 1605          	ldw	y,(OFST-1,sp)
 474  0033 9058          	sllw	y
 475  0035 9058          	sllw	y
 476  0037 bf01          	ldw	c_x+1,x
 477  0039 90b301        	cpw	y,c_x+1
 478  003c 2509          	jrult	L561
 479                     ; 152         AWU->APR = (uint8_t)(A - 2U);
 481  003e 7b06          	ld	a,(OFST+0,sp)
 482  0040 a002          	sub	a,#2
 483  0042 c750f1        	ld	20721,a
 485  0045 2006          	jra	L761
 486  0047               L561:
 487                     ; 156         AWU->APR = (uint8_t)(A - 1U);
 489  0047 7b06          	ld	a,(OFST+0,sp)
 490  0049 4a            	dec	a
 491  004a c750f1        	ld	20721,a
 492  004d               L761:
 493                     ; 158 }
 496  004d 5b06          	addw	sp,#6
 497  004f 81            	ret
 520                     ; 165 void AWU_IdleModeEnable(void)
 520                     ; 166 {
 521                     .text:	section	.text,new
 522  0000               _AWU_IdleModeEnable:
 526                     ; 168     AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
 528  0000 721950f0      	bres	20720,#4
 529                     ; 171     AWU->TBR = (uint8_t)(~AWU_TBR_AWUTB);
 531  0004 35f050f2      	mov	20722,#240
 532                     ; 172 }
 535  0008 81            	ret
 579                     ; 180 FlagStatus AWU_GetFlagStatus(void)
 579                     ; 181 {
 580                     .text:	section	.text,new
 581  0000               _AWU_GetFlagStatus:
 585                     ; 182     return((FlagStatus)(((uint8_t)(AWU->CSR & AWU_CSR_AWUF) == (uint8_t)0x00) ? RESET : SET));
 587  0000 c650f0        	ld	a,20720
 588  0003 a520          	bcp	a,#32
 589  0005 2603          	jrne	L22
 590  0007 4f            	clr	a
 591  0008 2002          	jra	L42
 592  000a               L22:
 593  000a a601          	ld	a,#1
 594  000c               L42:
 597  000c 81            	ret
 632                     	xdef	_TBR_Array
 633                     	xdef	_APR_Array
 634                     	xdef	_AWU_GetFlagStatus
 635                     	xdef	_AWU_IdleModeEnable
 636                     	xdef	_AWU_LSICalibrationConfig
 637                     	xdef	_AWU_Cmd
 638                     	xdef	_AWU_Init
 639                     	xdef	_AWU_DeInit
 640                     	xref.b	c_lreg
 641                     	xref.b	c_x
 660                     	xref	c_imul
 661                     	xref	c_ludv
 662                     	xref	c_ltor
 663                     	end
