   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 48 void BEEP_DeInit(void)
  32                     ; 49 {
  34                     	switch	.text
  35  0000               _BEEP_DeInit:
  39                     ; 50     BEEP->CSR = BEEP_CSR_RESET_VALUE;
  41  0000 351f50f3      	mov	20723,#31
  42                     ; 51 }
  45  0004 81            	ret
 110                     ; 61 void BEEP_Init(BEEP_Frequency_TypeDef BEEP_Frequency)
 110                     ; 62 {
 111                     	switch	.text
 112  0005               _BEEP_Init:
 114  0005 88            	push	a
 115       00000000      OFST:	set	0
 118                     ; 65     assert_param(IS_BEEP_FREQUENCY_OK(BEEP_Frequency));
 120                     ; 68     if ((BEEP->CSR & BEEP_CSR_BEEPDIV) == BEEP_CSR_BEEPDIV)
 122  0006 c650f3        	ld	a,20723
 123  0009 a41f          	and	a,#31
 124  000b a11f          	cp	a,#31
 125  000d 2610          	jrne	L15
 126                     ; 70         BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPDIV); /* Clear bits */
 128  000f c650f3        	ld	a,20723
 129  0012 a4e0          	and	a,#224
 130  0014 c750f3        	ld	20723,a
 131                     ; 71         BEEP->CSR |= BEEP_CALIBRATION_DEFAULT;
 133  0017 c650f3        	ld	a,20723
 134  001a aa0b          	or	a,#11
 135  001c c750f3        	ld	20723,a
 136  001f               L15:
 137                     ; 75     BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPSEL);
 139  001f c650f3        	ld	a,20723
 140  0022 a43f          	and	a,#63
 141  0024 c750f3        	ld	20723,a
 142                     ; 76     BEEP->CSR |= (uint8_t)(BEEP_Frequency);
 144  0027 c650f3        	ld	a,20723
 145  002a 1a01          	or	a,(OFST+1,sp)
 146  002c c750f3        	ld	20723,a
 147                     ; 78 }
 150  002f 84            	pop	a
 151  0030 81            	ret
 206                     ; 87 void BEEP_Cmd(FunctionalState NewState)
 206                     ; 88 {
 207                     	switch	.text
 208  0031               _BEEP_Cmd:
 212                     ; 89     if (NewState != DISABLE)
 214  0031 4d            	tnz	a
 215  0032 2706          	jreq	L101
 216                     ; 92         BEEP->CSR |= BEEP_CSR_BEEPEN;
 218  0034 721a50f3      	bset	20723,#5
 220  0038 2004          	jra	L301
 221  003a               L101:
 222                     ; 97         BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPEN);
 224  003a 721b50f3      	bres	20723,#5
 225  003e               L301:
 226                     ; 99 }
 229  003e 81            	ret
 282                     .const:	section	.text
 283  0000               L41:
 284  0000 000003e8      	dc.l	1000
 285                     ; 114 void BEEP_LSICalibrationConfig(uint32_t LSIFreqHz)
 285                     ; 115 {
 286                     	switch	.text
 287  003f               _BEEP_LSICalibrationConfig:
 289  003f 5206          	subw	sp,#6
 290       00000006      OFST:	set	6
 293                     ; 121     assert_param(IS_LSI_FREQUENCY_OK(LSIFreqHz));
 295                     ; 123     lsifreqkhz = (uint16_t)(LSIFreqHz / 1000); /* Converts value in kHz */
 297  0041 96            	ldw	x,sp
 298  0042 1c0009        	addw	x,#OFST+3
 299  0045 cd0000        	call	c_ltor
 301  0048 ae0000        	ldw	x,#L41
 302  004b cd0000        	call	c_ludv
 304  004e be02          	ldw	x,c_lreg+2
 305  0050 1f03          	ldw	(OFST-3,sp),x
 306                     ; 127     BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPDIV); /* Clear bits */
 308  0052 c650f3        	ld	a,20723
 309  0055 a4e0          	and	a,#224
 310  0057 c750f3        	ld	20723,a
 311                     ; 129     A = (uint16_t)(lsifreqkhz >> 3U); /* Division by 8, keep integer part only */
 313  005a 1e03          	ldw	x,(OFST-3,sp)
 314  005c 54            	srlw	x
 315  005d 54            	srlw	x
 316  005e 54            	srlw	x
 317  005f 1f05          	ldw	(OFST-1,sp),x
 318                     ; 131     if ((8U * A) >= ((lsifreqkhz - (8U * A)) * (1U + (2U * A))))
 320  0061 1e05          	ldw	x,(OFST-1,sp)
 321  0063 58            	sllw	x
 322  0064 58            	sllw	x
 323  0065 58            	sllw	x
 324  0066 1f01          	ldw	(OFST-5,sp),x
 325  0068 1e03          	ldw	x,(OFST-3,sp)
 326  006a 72f001        	subw	x,(OFST-5,sp)
 327  006d 1605          	ldw	y,(OFST-1,sp)
 328  006f 9058          	sllw	y
 329  0071 905c          	incw	y
 330  0073 cd0000        	call	c_imul
 332  0076 1605          	ldw	y,(OFST-1,sp)
 333  0078 9058          	sllw	y
 334  007a 9058          	sllw	y
 335  007c 9058          	sllw	y
 336  007e bf01          	ldw	c_x+1,x
 337  0080 90b301        	cpw	y,c_x+1
 338  0083 250c          	jrult	L331
 339                     ; 133         BEEP->CSR |= (uint8_t)(A - 2U);
 341  0085 7b06          	ld	a,(OFST+0,sp)
 342  0087 a002          	sub	a,#2
 343  0089 ca50f3        	or	a,20723
 344  008c c750f3        	ld	20723,a
 346  008f 2009          	jra	L531
 347  0091               L331:
 348                     ; 137         BEEP->CSR |= (uint8_t)(A - 1U);
 350  0091 7b06          	ld	a,(OFST+0,sp)
 351  0093 4a            	dec	a
 352  0094 ca50f3        	or	a,20723
 353  0097 c750f3        	ld	20723,a
 354  009a               L531:
 355                     ; 139 }
 358  009a 5b06          	addw	sp,#6
 359  009c 81            	ret
 372                     	xdef	_BEEP_LSICalibrationConfig
 373                     	xdef	_BEEP_Cmd
 374                     	xdef	_BEEP_Init
 375                     	xdef	_BEEP_DeInit
 376                     	xref.b	c_lreg
 377                     	xref.b	c_x
 396                     	xref	c_imul
 397                     	xref	c_ludv
 398                     	xref	c_ltor
 399                     	end
