   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 44 uint8_t ITC_GetCPUCC(void)
  32                     ; 45 {
  34                     .text:	section	.text,new
  35  0000               _ITC_GetCPUCC:
  39                     ; 47   _asm("push cc");
  42  0000 8a            push cc
  44                     ; 48   _asm("pop a");
  47  0001 84            pop a
  49                     ; 49   return; /* Ignore compiler warning, the returned value is in A register */
  52  0002 81            	ret
  75                     ; 75 void ITC_DeInit(void)
  75                     ; 76 {
  76                     .text:	section	.text,new
  77  0000               _ITC_DeInit:
  81                     ; 77     ITC->ISPR1 = ITC_SPRX_RESET_VALUE;
  83  0000 35ff7f70      	mov	32624,#255
  84                     ; 78     ITC->ISPR2 = ITC_SPRX_RESET_VALUE;
  86  0004 35ff7f71      	mov	32625,#255
  87                     ; 79     ITC->ISPR3 = ITC_SPRX_RESET_VALUE;
  89  0008 35ff7f72      	mov	32626,#255
  90                     ; 80     ITC->ISPR4 = ITC_SPRX_RESET_VALUE;
  92  000c 35ff7f73      	mov	32627,#255
  93                     ; 81     ITC->ISPR5 = ITC_SPRX_RESET_VALUE;
  95  0010 35ff7f74      	mov	32628,#255
  96                     ; 82     ITC->ISPR6 = ITC_SPRX_RESET_VALUE;
  98  0014 35ff7f75      	mov	32629,#255
  99                     ; 83     ITC->ISPR7 = ITC_SPRX_RESET_VALUE;
 101  0018 35ff7f76      	mov	32630,#255
 102                     ; 84     ITC->ISPR8 = ITC_SPRX_RESET_VALUE;
 104  001c 35ff7f77      	mov	32631,#255
 105                     ; 85 }
 108  0020 81            	ret
 133                     ; 92 uint8_t ITC_GetSoftIntStatus(void)
 133                     ; 93 {
 134                     .text:	section	.text,new
 135  0000               _ITC_GetSoftIntStatus:
 139                     ; 94     return (uint8_t)(ITC_GetCPUCC() & CPU_CC_I1I0);
 141  0000 cd0000        	call	_ITC_GetCPUCC
 143  0003 a428          	and	a,#40
 146  0005 81            	ret
 395                     .const:	section	.text
 396  0000               L22:
 397  0000 0028          	dc.w	L14
 398  0002 0028          	dc.w	L14
 399  0004 0028          	dc.w	L14
 400  0006 0028          	dc.w	L14
 401  0008 0031          	dc.w	L34
 402  000a 0031          	dc.w	L34
 403  000c 0031          	dc.w	L34
 404  000e 0031          	dc.w	L34
 405  0010 0065          	dc.w	L302
 406  0012 0065          	dc.w	L302
 407  0014 003a          	dc.w	L54
 408  0016 003a          	dc.w	L54
 409  0018 0043          	dc.w	L74
 410  001a 0043          	dc.w	L74
 411  001c 0043          	dc.w	L74
 412  001e 0043          	dc.w	L74
 413  0020 004c          	dc.w	L15
 414  0022 004c          	dc.w	L15
 415  0024 004c          	dc.w	L15
 416  0026 004c          	dc.w	L15
 417  0028 0065          	dc.w	L302
 418  002a 0065          	dc.w	L302
 419  002c 0055          	dc.w	L35
 420  002e 0055          	dc.w	L35
 421  0030 005e          	dc.w	L55
 422                     ; 102 ITC_PriorityLevel_TypeDef ITC_GetSoftwarePriority(ITC_Irq_TypeDef IrqNum)
 422                     ; 103 {
 423                     .text:	section	.text,new
 424  0000               _ITC_GetSoftwarePriority:
 426  0000 88            	push	a
 427  0001 89            	pushw	x
 428       00000002      OFST:	set	2
 431                     ; 105     uint8_t Value = 0;
 433  0002 0f02          	clr	(OFST+0,sp)
 434                     ; 106     uint8_t Mask = 0;
 436  0004 7b01          	ld	a,(OFST-1,sp)
 437  0006 97            	ld	xl,a
 438                     ; 109     assert_param(IS_ITC_IRQ_OK((uint8_t)IrqNum));
 440                     ; 112     Mask = (uint8_t)(0x03U << (((uint8_t)IrqNum % 4U) * 2U));
 442  0007 7b03          	ld	a,(OFST+1,sp)
 443  0009 a403          	and	a,#3
 444  000b 48            	sll	a
 445  000c 5f            	clrw	x
 446  000d 97            	ld	xl,a
 447  000e a603          	ld	a,#3
 448  0010 5d            	tnzw	x
 449  0011 2704          	jreq	L41
 450  0013               L61:
 451  0013 48            	sll	a
 452  0014 5a            	decw	x
 453  0015 26fc          	jrne	L61
 454  0017               L41:
 455  0017 6b01          	ld	(OFST-1,sp),a
 456                     ; 114     switch (IrqNum)
 458  0019 7b03          	ld	a,(OFST+1,sp)
 460                     ; 184     default:
 460                     ; 185         break;
 461  001b a119          	cp	a,#25
 462  001d 2407          	jruge	L02
 463  001f 5f            	clrw	x
 464  0020 97            	ld	xl,a
 465  0021 58            	sllw	x
 466  0022 de0000        	ldw	x,(L22,x)
 467  0025 fc            	jp	(x)
 468  0026               L02:
 469  0026 203d          	jra	L302
 470  0028               L14:
 471                     ; 116     case ITC_IRQ_TLI: /* TLI software priority can be read but has no meaning */
 471                     ; 117     case ITC_IRQ_AWU:
 471                     ; 118     case ITC_IRQ_CLK:
 471                     ; 119     case ITC_IRQ_PORTA:
 471                     ; 120         Value = (uint8_t)(ITC->ISPR1 & Mask); /* Read software priority */
 473  0028 c67f70        	ld	a,32624
 474  002b 1401          	and	a,(OFST-1,sp)
 475  002d 6b02          	ld	(OFST+0,sp),a
 476                     ; 121         break;
 478  002f 2034          	jra	L302
 479  0031               L34:
 480                     ; 122     case ITC_IRQ_PORTB:
 480                     ; 123     case ITC_IRQ_PORTC:
 480                     ; 124     case ITC_IRQ_PORTD:
 480                     ; 125     case ITC_IRQ_PORTE:
 480                     ; 126         Value = (uint8_t)(ITC->ISPR2 & Mask); /* Read software priority */
 482  0031 c67f71        	ld	a,32625
 483  0034 1401          	and	a,(OFST-1,sp)
 484  0036 6b02          	ld	(OFST+0,sp),a
 485                     ; 127         break;
 487  0038 202b          	jra	L302
 488  003a               L54:
 489                     ; 137     case ITC_IRQ_SPI:
 489                     ; 138     case ITC_IRQ_TIM1_OVF:
 489                     ; 139         Value = (uint8_t)(ITC->ISPR3 & Mask); /* Read software priority */
 491  003a c67f72        	ld	a,32626
 492  003d 1401          	and	a,(OFST-1,sp)
 493  003f 6b02          	ld	(OFST+0,sp),a
 494                     ; 140         break;
 496  0041 2022          	jra	L302
 497  0043               L74:
 498                     ; 141     case ITC_IRQ_TIM1_CAPCOM:
 498                     ; 142 #ifdef STM8S903
 498                     ; 143     case ITC_IRQ_TIM5_OVFTRI:
 498                     ; 144     case ITC_IRQ_TIM5_CAPCOM:
 498                     ; 145 #else
 498                     ; 146     case ITC_IRQ_TIM2_OVF:
 498                     ; 147     case ITC_IRQ_TIM2_CAPCOM:
 498                     ; 148 #endif /*STM8S903*/
 498                     ; 149 
 498                     ; 150     case ITC_IRQ_TIM3_OVF:
 498                     ; 151         Value = (uint8_t)(ITC->ISPR4 & Mask); /* Read software priority */
 500  0043 c67f73        	ld	a,32627
 501  0046 1401          	and	a,(OFST-1,sp)
 502  0048 6b02          	ld	(OFST+0,sp),a
 503                     ; 152         break;
 505  004a 2019          	jra	L302
 506  004c               L15:
 507                     ; 153     case ITC_IRQ_TIM3_CAPCOM:
 507                     ; 154     case ITC_IRQ_UART1_TX:
 507                     ; 155     case ITC_IRQ_UART1_RX:
 507                     ; 156     case ITC_IRQ_I2C:
 507                     ; 157         Value = (uint8_t)(ITC->ISPR5 & Mask); /* Read software priority */
 509  004c c67f74        	ld	a,32628
 510  004f 1401          	and	a,(OFST-1,sp)
 511  0051 6b02          	ld	(OFST+0,sp),a
 512                     ; 158         break;
 514  0053 2010          	jra	L302
 515  0055               L35:
 516                     ; 171     case ITC_IRQ_ADC1:
 516                     ; 172 #endif /*STM8S105, STM8S103 or STM8S905 or STM8AF626x */
 516                     ; 173 
 516                     ; 174 #ifdef STM8S903
 516                     ; 175     case ITC_IRQ_TIM6_OVFTRI:
 516                     ; 176 #else
 516                     ; 177     case ITC_IRQ_TIM4_OVF:
 516                     ; 178 #endif /*STM8S903*/
 516                     ; 179         Value = (uint8_t)(ITC->ISPR6 & Mask); /* Read software priority */
 518  0055 c67f75        	ld	a,32629
 519  0058 1401          	and	a,(OFST-1,sp)
 520  005a 6b02          	ld	(OFST+0,sp),a
 521                     ; 180         break;
 523  005c 2007          	jra	L302
 524  005e               L55:
 525                     ; 181     case ITC_IRQ_EEPROM_EEC:
 525                     ; 182         Value = (uint8_t)(ITC->ISPR7 & Mask); /* Read software priority */
 527  005e c67f76        	ld	a,32630
 528  0061 1401          	and	a,(OFST-1,sp)
 529  0063 6b02          	ld	(OFST+0,sp),a
 530                     ; 183         break;
 532  0065               L75:
 533                     ; 184     default:
 533                     ; 185         break;
 535  0065               L302:
 536                     ; 188     Value >>= (uint8_t)(((uint8_t)IrqNum % 4u) * 2u);
 538  0065 7b03          	ld	a,(OFST+1,sp)
 539  0067 a403          	and	a,#3
 540  0069 48            	sll	a
 541  006a 5f            	clrw	x
 542  006b 97            	ld	xl,a
 543  006c 7b02          	ld	a,(OFST+0,sp)
 544  006e 5d            	tnzw	x
 545  006f 2704          	jreq	L42
 546  0071               L62:
 547  0071 44            	srl	a
 548  0072 5a            	decw	x
 549  0073 26fc          	jrne	L62
 550  0075               L42:
 551  0075 6b02          	ld	(OFST+0,sp),a
 552                     ; 190     return((ITC_PriorityLevel_TypeDef)Value);
 554  0077 7b02          	ld	a,(OFST+0,sp)
 557  0079 5b03          	addw	sp,#3
 558  007b 81            	ret
 622                     	switch	.const
 623  0032               L44:
 624  0032 003c          	dc.w	L502
 625  0034 003c          	dc.w	L502
 626  0036 003c          	dc.w	L502
 627  0038 003c          	dc.w	L502
 628  003a 004e          	dc.w	L702
 629  003c 004e          	dc.w	L702
 630  003e 004e          	dc.w	L702
 631  0040 004e          	dc.w	L702
 632  0042 00b8          	dc.w	L162
 633  0044 00b8          	dc.w	L162
 634  0046 0060          	dc.w	L112
 635  0048 0060          	dc.w	L112
 636  004a 0072          	dc.w	L312
 637  004c 0072          	dc.w	L312
 638  004e 0072          	dc.w	L312
 639  0050 0072          	dc.w	L312
 640  0052 0084          	dc.w	L512
 641  0054 0084          	dc.w	L512
 642  0056 0084          	dc.w	L512
 643  0058 0084          	dc.w	L512
 644  005a 00b8          	dc.w	L162
 645  005c 00b8          	dc.w	L162
 646  005e 0096          	dc.w	L712
 647  0060 0096          	dc.w	L712
 648  0062 00a8          	dc.w	L122
 649                     ; 207 void ITC_SetSoftwarePriority(ITC_Irq_TypeDef IrqNum, ITC_PriorityLevel_TypeDef PriorityValue)
 649                     ; 208 {
 650                     .text:	section	.text,new
 651  0000               _ITC_SetSoftwarePriority:
 653  0000 89            	pushw	x
 654  0001 89            	pushw	x
 655       00000002      OFST:	set	2
 658                     ; 210     uint8_t Mask = 0;
 660  0002 7b01          	ld	a,(OFST-1,sp)
 661  0004 97            	ld	xl,a
 662                     ; 211     uint8_t NewPriority = 0;
 664  0005 7b02          	ld	a,(OFST+0,sp)
 665  0007 97            	ld	xl,a
 666                     ; 214     assert_param(IS_ITC_IRQ_OK((uint8_t)IrqNum));
 668                     ; 215     assert_param(IS_ITC_PRIORITY_OK(PriorityValue));
 670                     ; 218     assert_param(IS_ITC_INTERRUPTS_DISABLED);
 672                     ; 222     Mask = (uint8_t)(~(uint8_t)(0x03U << (((uint8_t)IrqNum % 4U) * 2U)));
 674  0008 7b03          	ld	a,(OFST+1,sp)
 675  000a a403          	and	a,#3
 676  000c 48            	sll	a
 677  000d 5f            	clrw	x
 678  000e 97            	ld	xl,a
 679  000f a603          	ld	a,#3
 680  0011 5d            	tnzw	x
 681  0012 2704          	jreq	L23
 682  0014               L43:
 683  0014 48            	sll	a
 684  0015 5a            	decw	x
 685  0016 26fc          	jrne	L43
 686  0018               L23:
 687  0018 43            	cpl	a
 688  0019 6b01          	ld	(OFST-1,sp),a
 689                     ; 225     NewPriority = (uint8_t)((uint8_t)(PriorityValue) << (((uint8_t)IrqNum % 4U) * 2U));
 691  001b 7b03          	ld	a,(OFST+1,sp)
 692  001d a403          	and	a,#3
 693  001f 48            	sll	a
 694  0020 5f            	clrw	x
 695  0021 97            	ld	xl,a
 696  0022 7b04          	ld	a,(OFST+2,sp)
 697  0024 5d            	tnzw	x
 698  0025 2704          	jreq	L63
 699  0027               L04:
 700  0027 48            	sll	a
 701  0028 5a            	decw	x
 702  0029 26fc          	jrne	L04
 703  002b               L63:
 704  002b 6b02          	ld	(OFST+0,sp),a
 705                     ; 227     switch (IrqNum)
 707  002d 7b03          	ld	a,(OFST+1,sp)
 709                     ; 311     default:
 709                     ; 312         break;
 710  002f a119          	cp	a,#25
 711  0031 2407          	jruge	L24
 712  0033 5f            	clrw	x
 713  0034 97            	ld	xl,a
 714  0035 58            	sllw	x
 715  0036 de0032        	ldw	x,(L44,x)
 716  0039 fc            	jp	(x)
 717  003a               L24:
 718  003a 207c          	jra	L162
 719  003c               L502:
 720                     ; 230     case ITC_IRQ_TLI: /* TLI software priority can be written but has no meaning */
 720                     ; 231     case ITC_IRQ_AWU:
 720                     ; 232     case ITC_IRQ_CLK:
 720                     ; 233     case ITC_IRQ_PORTA:
 720                     ; 234         ITC->ISPR1 &= Mask;
 722  003c c67f70        	ld	a,32624
 723  003f 1401          	and	a,(OFST-1,sp)
 724  0041 c77f70        	ld	32624,a
 725                     ; 235         ITC->ISPR1 |= NewPriority;
 727  0044 c67f70        	ld	a,32624
 728  0047 1a02          	or	a,(OFST+0,sp)
 729  0049 c77f70        	ld	32624,a
 730                     ; 236         break;
 732  004c 206a          	jra	L162
 733  004e               L702:
 734                     ; 238     case ITC_IRQ_PORTB:
 734                     ; 239     case ITC_IRQ_PORTC:
 734                     ; 240     case ITC_IRQ_PORTD:
 734                     ; 241     case ITC_IRQ_PORTE:
 734                     ; 242         ITC->ISPR2 &= Mask;
 736  004e c67f71        	ld	a,32625
 737  0051 1401          	and	a,(OFST-1,sp)
 738  0053 c77f71        	ld	32625,a
 739                     ; 243         ITC->ISPR2 |= NewPriority;
 741  0056 c67f71        	ld	a,32625
 742  0059 1a02          	or	a,(OFST+0,sp)
 743  005b c77f71        	ld	32625,a
 744                     ; 244         break;
 746  005e 2058          	jra	L162
 747  0060               L112:
 748                     ; 254     case ITC_IRQ_SPI:
 748                     ; 255     case ITC_IRQ_TIM1_OVF:
 748                     ; 256         ITC->ISPR3 &= Mask;
 750  0060 c67f72        	ld	a,32626
 751  0063 1401          	and	a,(OFST-1,sp)
 752  0065 c77f72        	ld	32626,a
 753                     ; 257         ITC->ISPR3 |= NewPriority;
 755  0068 c67f72        	ld	a,32626
 756  006b 1a02          	or	a,(OFST+0,sp)
 757  006d c77f72        	ld	32626,a
 758                     ; 258         break;
 760  0070 2046          	jra	L162
 761  0072               L312:
 762                     ; 260     case ITC_IRQ_TIM1_CAPCOM:
 762                     ; 261 #ifdef STM8S903
 762                     ; 262     case ITC_IRQ_TIM5_OVFTRI:
 762                     ; 263     case ITC_IRQ_TIM5_CAPCOM:
 762                     ; 264 #else
 762                     ; 265     case ITC_IRQ_TIM2_OVF:
 762                     ; 266     case ITC_IRQ_TIM2_CAPCOM:
 762                     ; 267 #endif /*STM8S903*/
 762                     ; 268 
 762                     ; 269     case ITC_IRQ_TIM3_OVF:
 762                     ; 270         ITC->ISPR4 &= Mask;
 764  0072 c67f73        	ld	a,32627
 765  0075 1401          	and	a,(OFST-1,sp)
 766  0077 c77f73        	ld	32627,a
 767                     ; 271         ITC->ISPR4 |= NewPriority;
 769  007a c67f73        	ld	a,32627
 770  007d 1a02          	or	a,(OFST+0,sp)
 771  007f c77f73        	ld	32627,a
 772                     ; 272         break;
 774  0082 2034          	jra	L162
 775  0084               L512:
 776                     ; 274     case ITC_IRQ_TIM3_CAPCOM:
 776                     ; 275     case ITC_IRQ_UART1_TX:
 776                     ; 276     case ITC_IRQ_UART1_RX:
 776                     ; 277     case ITC_IRQ_I2C:
 776                     ; 278         ITC->ISPR5 &= Mask;
 778  0084 c67f74        	ld	a,32628
 779  0087 1401          	and	a,(OFST-1,sp)
 780  0089 c77f74        	ld	32628,a
 781                     ; 279         ITC->ISPR5 |= NewPriority;
 783  008c c67f74        	ld	a,32628
 784  008f 1a02          	or	a,(OFST+0,sp)
 785  0091 c77f74        	ld	32628,a
 786                     ; 280         break;
 788  0094 2022          	jra	L162
 789  0096               L712:
 790                     ; 294     case ITC_IRQ_ADC1:
 790                     ; 295 #endif /*STM8S105, STM8S103 or STM8S905 or STM8AF626x */
 790                     ; 296 
 790                     ; 297 #ifdef STM8S903
 790                     ; 298     case ITC_IRQ_TIM6_OVFTRI:
 790                     ; 299 #else
 790                     ; 300     case ITC_IRQ_TIM4_OVF:
 790                     ; 301 #endif /*STM8S903*/
 790                     ; 302         ITC->ISPR6 &= Mask;
 792  0096 c67f75        	ld	a,32629
 793  0099 1401          	and	a,(OFST-1,sp)
 794  009b c77f75        	ld	32629,a
 795                     ; 303         ITC->ISPR6 |= NewPriority;
 797  009e c67f75        	ld	a,32629
 798  00a1 1a02          	or	a,(OFST+0,sp)
 799  00a3 c77f75        	ld	32629,a
 800                     ; 304         break;
 802  00a6 2010          	jra	L162
 803  00a8               L122:
 804                     ; 306     case ITC_IRQ_EEPROM_EEC:
 804                     ; 307         ITC->ISPR7 &= Mask;
 806  00a8 c67f76        	ld	a,32630
 807  00ab 1401          	and	a,(OFST-1,sp)
 808  00ad c77f76        	ld	32630,a
 809                     ; 308         ITC->ISPR7 |= NewPriority;
 811  00b0 c67f76        	ld	a,32630
 812  00b3 1a02          	or	a,(OFST+0,sp)
 813  00b5 c77f76        	ld	32630,a
 814                     ; 309         break;
 816  00b8               L322:
 817                     ; 311     default:
 817                     ; 312         break;
 819  00b8               L162:
 820                     ; 316 }
 823  00b8 5b04          	addw	sp,#4
 824  00ba 81            	ret
 837                     	xdef	_ITC_GetSoftwarePriority
 838                     	xdef	_ITC_SetSoftwarePriority
 839                     	xdef	_ITC_GetSoftIntStatus
 840                     	xdef	_ITC_DeInit
 841                     	xdef	_ITC_GetCPUCC
 860                     	end
