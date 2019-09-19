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
  71                     	switch	.text
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
 251                     	switch	.text
 252  000d               _AWU_Init:
 254  000d 88            	push	a
 255       00000000      OFST:	set	0
 258                     ; 86     assert_param(IS_AWU_TIMEBASE_OK(AWU_TimeBase));
 260                     ; 89     AWU->CSR |= AWU_CSR_AWUEN;
 262  000e 721850f0      	bset	20720,#4
 263                     ; 92     AWU->TBR &= (uint8_t)(~AWU_TBR_AWUTB);
 265  0012 c650f2        	ld	a,20722
 266  0015 a4f0          	and	a,#240
 267  0017 c750f2        	ld	20722,a
 268                     ; 93     AWU->TBR |= TBR_Array[(uint8_t)AWU_TimeBase];
 270  001a 7b01          	ld	a,(OFST+1,sp)
 271  001c 5f            	clrw	x
 272  001d 97            	ld	xl,a
 273  001e c650f2        	ld	a,20722
 274  0021 da0011        	or	a,(_TBR_Array,x)
 275  0024 c750f2        	ld	20722,a
 276                     ; 96     AWU->APR &= (uint8_t)(~AWU_APR_APR);
 278  0027 c650f1        	ld	a,20721
 279  002a a4c0          	and	a,#192
 280  002c c750f1        	ld	20721,a
 281                     ; 97     AWU->APR |= APR_Array[(uint8_t)AWU_TimeBase];
 283  002f 7b01          	ld	a,(OFST+1,sp)
 284  0031 5f            	clrw	x
 285  0032 97            	ld	xl,a
 286  0033 c650f1        	ld	a,20721
 287  0036 da0000        	or	a,(_APR_Array,x)
 288  0039 c750f1        	ld	20721,a
 289                     ; 99 }
 292  003c 84            	pop	a
 293  003d 81            	ret
 348                     ; 108 void AWU_Cmd(FunctionalState NewState)
 348                     ; 109 {
 349                     	switch	.text
 350  003e               _AWU_Cmd:
 354                     ; 110     if (NewState != DISABLE)
 356  003e 4d            	tnz	a
 357  003f 2706          	jreq	L331
 358                     ; 113         AWU->CSR |= AWU_CSR_AWUEN;
 360  0041 721850f0      	bset	20720,#4
 362  0045 2004          	jra	L531
 363  0047               L331:
 364                     ; 118         AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
 366  0047 721950f0      	bres	20720,#4
 367  004b               L531:
 368                     ; 120 }
 371  004b 81            	ret
 424                     	switch	.const
 425  0022               L41:
 426  0022 000003e8      	dc.l	1000
 427                     ; 135 void AWU_LSICalibrationConfig(uint32_t LSIFreqHz)
 427                     ; 136 {
 428                     	switch	.text
 429  004c               _AWU_LSICalibrationConfig:
 431  004c 5206          	subw	sp,#6
 432       00000006      OFST:	set	6
 435                     ; 138     uint16_t lsifreqkhz = 0x0;
 437  004e 1e03          	ldw	x,(OFST-3,sp)
 438                     ; 139     uint16_t A = 0x0;
 440  0050 1e05          	ldw	x,(OFST-1,sp)
 441                     ; 142     assert_param(IS_LSI_FREQUENCY_OK(LSIFreqHz));
 443                     ; 144     lsifreqkhz = (uint16_t)(LSIFreqHz / 1000); /* Converts value in kHz */
 445  0052 96            	ldw	x,sp
 446  0053 1c0009        	addw	x,#OFST+3
 447  0056 cd0000        	call	c_ltor
 449  0059 ae0022        	ldw	x,#L41
 450  005c cd0000        	call	c_ludv
 452  005f be02          	ldw	x,c_lreg+2
 453  0061 1f03          	ldw	(OFST-3,sp),x
 454                     ; 148     A = (uint16_t)(lsifreqkhz >> 2U); /* Division by 4, keep integer part only */
 456  0063 1e03          	ldw	x,(OFST-3,sp)
 457  0065 54            	srlw	x
 458  0066 54            	srlw	x
 459  0067 1f05          	ldw	(OFST-1,sp),x
 460                     ; 150     if ((4U * A) >= ((lsifreqkhz - (4U * A)) * (1U + (2U * A))))
 462  0069 1e05          	ldw	x,(OFST-1,sp)
 463  006b 58            	sllw	x
 464  006c 58            	sllw	x
 465  006d 1f01          	ldw	(OFST-5,sp),x
 466  006f 1e03          	ldw	x,(OFST-3,sp)
 467  0071 72f001        	subw	x,(OFST-5,sp)
 468  0074 1605          	ldw	y,(OFST-1,sp)
 469  0076 9058          	sllw	y
 470  0078 905c          	incw	y
 471  007a cd0000        	call	c_imul
 473  007d 1605          	ldw	y,(OFST-1,sp)
 474  007f 9058          	sllw	y
 475  0081 9058          	sllw	y
 476  0083 bf01          	ldw	c_x+1,x
 477  0085 90b301        	cpw	y,c_x+1
 478  0088 2509          	jrult	L561
 479                     ; 152         AWU->APR = (uint8_t)(A - 2U);
 481  008a 7b06          	ld	a,(OFST+0,sp)
 482  008c a002          	sub	a,#2
 483  008e c750f1        	ld	20721,a
 485  0091 2006          	jra	L761
 486  0093               L561:
 487                     ; 156         AWU->APR = (uint8_t)(A - 1U);
 489  0093 7b06          	ld	a,(OFST+0,sp)
 490  0095 4a            	dec	a
 491  0096 c750f1        	ld	20721,a
 492  0099               L761:
 493                     ; 158 }
 496  0099 5b06          	addw	sp,#6
 497  009b 81            	ret
 520                     ; 165 void AWU_IdleModeEnable(void)
 520                     ; 166 {
 521                     	switch	.text
 522  009c               _AWU_IdleModeEnable:
 526                     ; 168     AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
 528  009c 721950f0      	bres	20720,#4
 529                     ; 171     AWU->TBR = (uint8_t)(~AWU_TBR_AWUTB);
 531  00a0 35f050f2      	mov	20722,#240
 532                     ; 172 }
 535  00a4 81            	ret
 579                     ; 180 FlagStatus AWU_GetFlagStatus(void)
 579                     ; 181 {
 580                     	switch	.text
 581  00a5               _AWU_GetFlagStatus:
 585                     ; 182     return((FlagStatus)(((uint8_t)(AWU->CSR & AWU_CSR_AWUF) == (uint8_t)0x00) ? RESET : SET));
 587  00a5 c650f0        	ld	a,20720
 588  00a8 a520          	bcp	a,#32
 589  00aa 2603          	jrne	L22
 590  00ac 4f            	clr	a
 591  00ad 2002          	jra	L42
 592  00af               L22:
 593  00af a601          	ld	a,#1
 594  00b1               L42:
 597  00b1 81            	ret
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
