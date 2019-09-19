   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 47 void EXTI_DeInit(void)
  32                     ; 48 {
  34                     .text:	section	.text,new
  35  0000               _EXTI_DeInit:
  39                     ; 49     EXTI->CR1 = EXTI_CR1_RESET_VALUE;
  41  0000 725f50a0      	clr	20640
  42                     ; 50     EXTI->CR2 = EXTI_CR2_RESET_VALUE;
  44  0004 725f50a1      	clr	20641
  45                     ; 51 }
  48  0008 81            	ret
 173                     ; 64 void EXTI_SetExtIntSensitivity(EXTI_Port_TypeDef Port, EXTI_Sensitivity_TypeDef SensitivityValue)
 173                     ; 65 {
 174                     .text:	section	.text,new
 175  0000               _EXTI_SetExtIntSensitivity:
 177  0000 89            	pushw	x
 178       00000000      OFST:	set	0
 181                     ; 68     assert_param(IS_EXTI_PORT_OK(Port));
 183                     ; 69     assert_param(IS_EXTI_SENSITIVITY_OK(SensitivityValue));
 185                     ; 72     switch (Port)
 187  0001 9e            	ld	a,xh
 189                     ; 94     default:
 189                     ; 95         break;
 190  0002 4d            	tnz	a
 191  0003 270e          	jreq	L12
 192  0005 4a            	dec	a
 193  0006 271d          	jreq	L32
 194  0008 4a            	dec	a
 195  0009 272e          	jreq	L52
 196  000b 4a            	dec	a
 197  000c 2742          	jreq	L72
 198  000e 4a            	dec	a
 199  000f 2756          	jreq	L13
 200  0011 2064          	jra	L311
 201  0013               L12:
 202                     ; 74     case EXTI_PORT_GPIOA:
 202                     ; 75         EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PAIS);
 204  0013 c650a0        	ld	a,20640
 205  0016 a4fc          	and	a,#252
 206  0018 c750a0        	ld	20640,a
 207                     ; 76         EXTI->CR1 |= (uint8_t)(SensitivityValue);
 209  001b c650a0        	ld	a,20640
 210  001e 1a02          	or	a,(OFST+2,sp)
 211  0020 c750a0        	ld	20640,a
 212                     ; 77         break;
 214  0023 2052          	jra	L311
 215  0025               L32:
 216                     ; 78     case EXTI_PORT_GPIOB:
 216                     ; 79         EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PBIS);
 218  0025 c650a0        	ld	a,20640
 219  0028 a4f3          	and	a,#243
 220  002a c750a0        	ld	20640,a
 221                     ; 80         EXTI->CR1 |= (uint8_t)((uint8_t)(SensitivityValue) << 2);
 223  002d 7b02          	ld	a,(OFST+2,sp)
 224  002f 48            	sll	a
 225  0030 48            	sll	a
 226  0031 ca50a0        	or	a,20640
 227  0034 c750a0        	ld	20640,a
 228                     ; 81         break;
 230  0037 203e          	jra	L311
 231  0039               L52:
 232                     ; 82     case EXTI_PORT_GPIOC:
 232                     ; 83         EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PCIS);
 234  0039 c650a0        	ld	a,20640
 235  003c a4cf          	and	a,#207
 236  003e c750a0        	ld	20640,a
 237                     ; 84         EXTI->CR1 |= (uint8_t)((uint8_t)(SensitivityValue) << 4);
 239  0041 7b02          	ld	a,(OFST+2,sp)
 240  0043 97            	ld	xl,a
 241  0044 a610          	ld	a,#16
 242  0046 42            	mul	x,a
 243  0047 9f            	ld	a,xl
 244  0048 ca50a0        	or	a,20640
 245  004b c750a0        	ld	20640,a
 246                     ; 85         break;
 248  004e 2027          	jra	L311
 249  0050               L72:
 250                     ; 86     case EXTI_PORT_GPIOD:
 250                     ; 87         EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PDIS);
 252  0050 c650a0        	ld	a,20640
 253  0053 a43f          	and	a,#63
 254  0055 c750a0        	ld	20640,a
 255                     ; 88         EXTI->CR1 |= (uint8_t)((uint8_t)(SensitivityValue) << 6);
 257  0058 7b02          	ld	a,(OFST+2,sp)
 258  005a 97            	ld	xl,a
 259  005b a640          	ld	a,#64
 260  005d 42            	mul	x,a
 261  005e 9f            	ld	a,xl
 262  005f ca50a0        	or	a,20640
 263  0062 c750a0        	ld	20640,a
 264                     ; 89         break;
 266  0065 2010          	jra	L311
 267  0067               L13:
 268                     ; 90     case EXTI_PORT_GPIOE:
 268                     ; 91         EXTI->CR2 &= (uint8_t)(~EXTI_CR2_PEIS);
 270  0067 c650a1        	ld	a,20641
 271  006a a4fc          	and	a,#252
 272  006c c750a1        	ld	20641,a
 273                     ; 92         EXTI->CR2 |= (uint8_t)(SensitivityValue);
 275  006f c650a1        	ld	a,20641
 276  0072 1a02          	or	a,(OFST+2,sp)
 277  0074 c750a1        	ld	20641,a
 278                     ; 93         break;
 280  0077               L33:
 281                     ; 94     default:
 281                     ; 95         break;
 283  0077               L311:
 284                     ; 97 }
 287  0077 85            	popw	x
 288  0078 81            	ret
 346                     ; 106 void EXTI_SetTLISensitivity(EXTI_TLISensitivity_TypeDef SensitivityValue)
 346                     ; 107 {
 347                     .text:	section	.text,new
 348  0000               _EXTI_SetTLISensitivity:
 352                     ; 109     assert_param(IS_EXTI_TLISENSITIVITY_OK(SensitivityValue));
 354                     ; 112     EXTI->CR2 &= (uint8_t)(~EXTI_CR2_TLIS);
 356  0000 721550a1      	bres	20641,#2
 357                     ; 113     EXTI->CR2 |= (uint8_t)(SensitivityValue);
 359  0004 ca50a1        	or	a,20641
 360  0007 c750a1        	ld	20641,a
 361                     ; 114 }
 364  000a 81            	ret
 410                     ; 121 EXTI_Sensitivity_TypeDef EXTI_GetExtIntSensitivity(EXTI_Port_TypeDef Port)
 410                     ; 122 {
 411                     .text:	section	.text,new
 412  0000               _EXTI_GetExtIntSensitivity:
 414  0000 88            	push	a
 415       00000001      OFST:	set	1
 418                     ; 123     uint8_t value = 0;
 420  0001 0f01          	clr	(OFST+0,sp)
 421                     ; 126     assert_param(IS_EXTI_PORT_OK(Port));
 423                     ; 128     switch (Port)
 426                     ; 145     default:
 426                     ; 146         break;
 427  0003 4d            	tnz	a
 428  0004 270e          	jreq	L341
 429  0006 4a            	dec	a
 430  0007 2714          	jreq	L541
 431  0009 4a            	dec	a
 432  000a 271c          	jreq	L741
 433  000c 4a            	dec	a
 434  000d 2725          	jreq	L151
 435  000f 4a            	dec	a
 436  0010 2730          	jreq	L351
 437  0012 2035          	jra	L302
 438  0014               L341:
 439                     ; 130     case EXTI_PORT_GPIOA:
 439                     ; 131         value = (uint8_t)(EXTI->CR1 & EXTI_CR1_PAIS);
 441  0014 c650a0        	ld	a,20640
 442  0017 a403          	and	a,#3
 443  0019 6b01          	ld	(OFST+0,sp),a
 444                     ; 132         break;
 446  001b 202c          	jra	L302
 447  001d               L541:
 448                     ; 133     case EXTI_PORT_GPIOB:
 448                     ; 134         value = (uint8_t)((uint8_t)(EXTI->CR1 & EXTI_CR1_PBIS) >> 2);
 450  001d c650a0        	ld	a,20640
 451  0020 a40c          	and	a,#12
 452  0022 44            	srl	a
 453  0023 44            	srl	a
 454  0024 6b01          	ld	(OFST+0,sp),a
 455                     ; 135         break;
 457  0026 2021          	jra	L302
 458  0028               L741:
 459                     ; 136     case EXTI_PORT_GPIOC:
 459                     ; 137         value = (uint8_t)((uint8_t)(EXTI->CR1 & EXTI_CR1_PCIS) >> 4);
 461  0028 c650a0        	ld	a,20640
 462  002b a430          	and	a,#48
 463  002d 4e            	swap	a
 464  002e a40f          	and	a,#15
 465  0030 6b01          	ld	(OFST+0,sp),a
 466                     ; 138         break;
 468  0032 2015          	jra	L302
 469  0034               L151:
 470                     ; 139     case EXTI_PORT_GPIOD:
 470                     ; 140         value = (uint8_t)((uint8_t)(EXTI->CR1 & EXTI_CR1_PDIS) >> 6);
 472  0034 c650a0        	ld	a,20640
 473  0037 a4c0          	and	a,#192
 474  0039 4e            	swap	a
 475  003a 44            	srl	a
 476  003b 44            	srl	a
 477  003c a403          	and	a,#3
 478  003e 6b01          	ld	(OFST+0,sp),a
 479                     ; 141         break;
 481  0040 2007          	jra	L302
 482  0042               L351:
 483                     ; 142     case EXTI_PORT_GPIOE:
 483                     ; 143         value = (uint8_t)(EXTI->CR2 & EXTI_CR2_PEIS);
 485  0042 c650a1        	ld	a,20641
 486  0045 a403          	and	a,#3
 487  0047 6b01          	ld	(OFST+0,sp),a
 488                     ; 144         break;
 490  0049               L551:
 491                     ; 145     default:
 491                     ; 146         break;
 493  0049               L302:
 494                     ; 149     return((EXTI_Sensitivity_TypeDef)value);
 496  0049 7b01          	ld	a,(OFST+0,sp)
 499  004b 5b01          	addw	sp,#1
 500  004d 81            	ret
 536                     ; 157 EXTI_TLISensitivity_TypeDef EXTI_GetTLISensitivity(void)
 536                     ; 158 {
 537                     .text:	section	.text,new
 538  0000               _EXTI_GetTLISensitivity:
 540  0000 88            	push	a
 541       00000001      OFST:	set	1
 544                     ; 160     uint8_t value = 0;
 546  0001 0f01          	clr	(OFST+0,sp)
 547                     ; 163     value = (uint8_t)(EXTI->CR2 & EXTI_CR2_TLIS);
 549  0003 c650a1        	ld	a,20641
 550  0006 a404          	and	a,#4
 551  0008 6b01          	ld	(OFST+0,sp),a
 552                     ; 165     return((EXTI_TLISensitivity_TypeDef)value);
 554  000a 7b01          	ld	a,(OFST+0,sp)
 557  000c 5b01          	addw	sp,#1
 558  000e 81            	ret
 571                     	xdef	_EXTI_GetTLISensitivity
 572                     	xdef	_EXTI_GetExtIntSensitivity
 573                     	xdef	_EXTI_SetTLISensitivity
 574                     	xdef	_EXTI_SetExtIntSensitivity
 575                     	xdef	_EXTI_DeInit
 594                     	end
