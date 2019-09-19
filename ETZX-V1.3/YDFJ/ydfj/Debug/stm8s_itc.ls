   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 44 uint8_t ITC_GetCPUCC(void)
  32                     ; 45 {
  34                     	switch	.text
  35  0000               _ITC_GetCPUCC:
  39                     ; 47   _asm("push cc");
  42  0000 8a            push cc
  44                     ; 48   _asm("pop a");
  47  0001 84            pop a
  49                     ; 49   return; /* Ignore compiler warning, the returned value is in A register */
  52  0002 81            	ret
  75                     ; 75 void ITC_DeInit(void)
  75                     ; 76 {
  76                     	switch	.text
  77  0003               _ITC_DeInit:
  81                     ; 77     ITC->ISPR1 = ITC_SPRX_RESET_VALUE;
  83  0003 35ff7f70      	mov	32624,#255
  84                     ; 78     ITC->ISPR2 = ITC_SPRX_RESET_VALUE;
  86  0007 35ff7f71      	mov	32625,#255
  87                     ; 79     ITC->ISPR3 = ITC_SPRX_RESET_VALUE;
  89  000b 35ff7f72      	mov	32626,#255
  90                     ; 80     ITC->ISPR4 = ITC_SPRX_RESET_VALUE;
  92  000f 35ff7f73      	mov	32627,#255
  93                     ; 81     ITC->ISPR5 = ITC_SPRX_RESET_VALUE;
  95  0013 35ff7f74      	mov	32628,#255
  96                     ; 82     ITC->ISPR6 = ITC_SPRX_RESET_VALUE;
  98  0017 35ff7f75      	mov	32629,#255
  99                     ; 83     ITC->ISPR7 = ITC_SPRX_RESET_VALUE;
 101  001b 35ff7f76      	mov	32630,#255
 102                     ; 84     ITC->ISPR8 = ITC_SPRX_RESET_VALUE;
 104  001f 35ff7f77      	mov	32631,#255
 105                     ; 85 }
 108  0023 81            	ret
 133                     ; 92 uint8_t ITC_GetSoftIntStatus(void)
 133                     ; 93 {
 134                     	switch	.text
 135  0024               _ITC_GetSoftIntStatus:
 139                     ; 94     return (uint8_t)(ITC_GetCPUCC() & CPU_CC_I1I0);
 141  0024 adda          	call	_ITC_GetCPUCC
 143  0026 a428          	and	a,#40
 146  0028 81            	ret
 395                     .const:	section	.text
 396  0000               L22:
 397  0000 0051          	dc.w	L14
 398  0002 0051          	dc.w	L14
 399  0004 0051          	dc.w	L14
 400  0006 0051          	dc.w	L14
 401  0008 005a          	dc.w	L34
 402  000a 005a          	dc.w	L34
 403  000c 005a          	dc.w	L34
 404  000e 005a          	dc.w	L34
 405  0010 008e          	dc.w	L302
 406  0012 008e          	dc.w	L302
 407  0014 0063          	dc.w	L54
 408  0016 0063          	dc.w	L54
 409  0018 006c          	dc.w	L74
 410  001a 006c          	dc.w	L74
 411  001c 006c          	dc.w	L74
 412  001e 006c          	dc.w	L74
 413  0020 0075          	dc.w	L15
 414  0022 0075          	dc.w	L15
 415  0024 0075          	dc.w	L15
 416  0026 0075          	dc.w	L15
 417  0028 008e          	dc.w	L302
 418  002a 008e          	dc.w	L302
 419  002c 007e          	dc.w	L35
 420  002e 007e          	dc.w	L35
 421  0030 0087          	dc.w	L55
 422                     ; 102 ITC_PriorityLevel_TypeDef ITC_GetSoftwarePriority(ITC_Irq_TypeDef IrqNum)
 422                     ; 103 {
 423                     	switch	.text
 424  0029               _ITC_GetSoftwarePriority:
 426  0029 88            	push	a
 427  002a 89            	pushw	x
 428       00000002      OFST:	set	2
 431                     ; 105     uint8_t Value = 0;
 433  002b 0f02          	clr	(OFST+0,sp)
 434                     ; 106     uint8_t Mask = 0;
 436  002d 7b01          	ld	a,(OFST-1,sp)
 437  002f 97            	ld	xl,a
 438                     ; 109     assert_param(IS_ITC_IRQ_OK((uint8_t)IrqNum));
 440                     ; 112     Mask = (uint8_t)(0x03U << (((uint8_t)IrqNum % 4U) * 2U));
 442  0030 7b03          	ld	a,(OFST+1,sp)
 443  0032 a403          	and	a,#3
 444  0034 48            	sll	a
 445  0035 5f            	clrw	x
 446  0036 97            	ld	xl,a
 447  0037 a603          	ld	a,#3
 448  0039 5d            	tnzw	x
 449  003a 2704          	jreq	L41
 450  003c               L61:
 451  003c 48            	sll	a
 452  003d 5a            	decw	x
 453  003e 26fc          	jrne	L61
 454  0040               L41:
 455  0040 6b01          	ld	(OFST-1,sp),a
 456                     ; 114     switch (IrqNum)
 458  0042 7b03          	ld	a,(OFST+1,sp)
 460                     ; 184     default:
 460                     ; 185         break;
 461  0044 a119          	cp	a,#25
 462  0046 2407          	jruge	L02
 463  0048 5f            	clrw	x
 464  0049 97            	ld	xl,a
 465  004a 58            	sllw	x
 466  004b de0000        	ldw	x,(L22,x)
 467  004e fc            	jp	(x)
 468  004f               L02:
 469  004f 203d          	jra	L302
 470  0051               L14:
 471                     ; 116     case ITC_IRQ_TLI: /* TLI software priority can be read but has no meaning */
 471                     ; 117     case ITC_IRQ_AWU:
 471                     ; 118     case ITC_IRQ_CLK:
 471                     ; 119     case ITC_IRQ_PORTA:
 471                     ; 120         Value = (uint8_t)(ITC->ISPR1 & Mask); /* Read software priority */
 473  0051 c67f70        	ld	a,32624
 474  0054 1401          	and	a,(OFST-1,sp)
 475  0056 6b02          	ld	(OFST+0,sp),a
 476                     ; 121         break;
 478  0058 2034          	jra	L302
 479  005a               L34:
 480                     ; 122     case ITC_IRQ_PORTB:
 480                     ; 123     case ITC_IRQ_PORTC:
 480                     ; 124     case ITC_IRQ_PORTD:
 480                     ; 125     case ITC_IRQ_PORTE:
 480                     ; 126         Value = (uint8_t)(ITC->ISPR2 & Mask); /* Read software priority */
 482  005a c67f71        	ld	a,32625
 483  005d 1401          	and	a,(OFST-1,sp)
 484  005f 6b02          	ld	(OFST+0,sp),a
 485                     ; 127         break;
 487  0061 202b          	jra	L302
 488  0063               L54:
 489                     ; 137     case ITC_IRQ_SPI:
 489                     ; 138     case ITC_IRQ_TIM1_OVF:
 489                     ; 139         Value = (uint8_t)(ITC->ISPR3 & Mask); /* Read software priority */
 491  0063 c67f72        	ld	a,32626
 492  0066 1401          	and	a,(OFST-1,sp)
 493  0068 6b02          	ld	(OFST+0,sp),a
 494                     ; 140         break;
 496  006a 2022          	jra	L302
 497  006c               L74:
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
 500  006c c67f73        	ld	a,32627
 501  006f 1401          	and	a,(OFST-1,sp)
 502  0071 6b02          	ld	(OFST+0,sp),a
 503                     ; 152         break;
 505  0073 2019          	jra	L302
 506  0075               L15:
 507                     ; 153     case ITC_IRQ_TIM3_CAPCOM:
 507                     ; 154     case ITC_IRQ_UART1_TX:
 507                     ; 155     case ITC_IRQ_UART1_RX:
 507                     ; 156     case ITC_IRQ_I2C:
 507                     ; 157         Value = (uint8_t)(ITC->ISPR5 & Mask); /* Read software priority */
 509  0075 c67f74        	ld	a,32628
 510  0078 1401          	and	a,(OFST-1,sp)
 511  007a 6b02          	ld	(OFST+0,sp),a
 512                     ; 158         break;
 514  007c 2010          	jra	L302
 515  007e               L35:
 516                     ; 171     case ITC_IRQ_ADC1:
 516                     ; 172 #endif /*STM8S105, STM8S103 or STM8S905 or STM8AF626x */
 516                     ; 173 
 516                     ; 174 #ifdef STM8S903
 516                     ; 175     case ITC_IRQ_TIM6_OVFTRI:
 516                     ; 176 #else
 516                     ; 177     case ITC_IRQ_TIM4_OVF:
 516                     ; 178 #endif /*STM8S903*/
 516                     ; 179         Value = (uint8_t)(ITC->ISPR6 & Mask); /* Read software priority */
 518  007e c67f75        	ld	a,32629
 519  0081 1401          	and	a,(OFST-1,sp)
 520  0083 6b02          	ld	(OFST+0,sp),a
 521                     ; 180         break;
 523  0085 2007          	jra	L302
 524  0087               L55:
 525                     ; 181     case ITC_IRQ_EEPROM_EEC:
 525                     ; 182         Value = (uint8_t)(ITC->ISPR7 & Mask); /* Read software priority */
 527  0087 c67f76        	ld	a,32630
 528  008a 1401          	and	a,(OFST-1,sp)
 529  008c 6b02          	ld	(OFST+0,sp),a
 530                     ; 183         break;
 532  008e               L75:
 533                     ; 184     default:
 533                     ; 185         break;
 535  008e               L302:
 536                     ; 188     Value >>= (uint8_t)(((uint8_t)IrqNum % 4u) * 2u);
 538  008e 7b03          	ld	a,(OFST+1,sp)
 539  0090 a403          	and	a,#3
 540  0092 48            	sll	a
 541  0093 5f            	clrw	x
 542  0094 97            	ld	xl,a
 543  0095 7b02          	ld	a,(OFST+0,sp)
 544  0097 5d            	tnzw	x
 545  0098 2704          	jreq	L42
 546  009a               L62:
 547  009a 44            	srl	a
 548  009b 5a            	decw	x
 549  009c 26fc          	jrne	L62
 550  009e               L42:
 551  009e 6b02          	ld	(OFST+0,sp),a
 552                     ; 190     return((ITC_PriorityLevel_TypeDef)Value);
 554  00a0 7b02          	ld	a,(OFST+0,sp)
 557  00a2 5b03          	addw	sp,#3
 558  00a4 81            	ret
 622                     	switch	.const
 623  0032               L44:
 624  0032 00e1          	dc.w	L502
 625  0034 00e1          	dc.w	L502
 626  0036 00e1          	dc.w	L502
 627  0038 00e1          	dc.w	L502
 628  003a 00f3          	dc.w	L702
 629  003c 00f3          	dc.w	L702
 630  003e 00f3          	dc.w	L702
 631  0040 00f3          	dc.w	L702
 632  0042 015d          	dc.w	L162
 633  0044 015d          	dc.w	L162
 634  0046 0105          	dc.w	L112
 635  0048 0105          	dc.w	L112
 636  004a 0117          	dc.w	L312
 637  004c 0117          	dc.w	L312
 638  004e 0117          	dc.w	L312
 639  0050 0117          	dc.w	L312
 640  0052 0129          	dc.w	L512
 641  0054 0129          	dc.w	L512
 642  0056 0129          	dc.w	L512
 643  0058 0129          	dc.w	L512
 644  005a 015d          	dc.w	L162
 645  005c 015d          	dc.w	L162
 646  005e 013b          	dc.w	L712
 647  0060 013b          	dc.w	L712
 648  0062 014d          	dc.w	L122
 649                     ; 207 void ITC_SetSoftwarePriority(ITC_Irq_TypeDef IrqNum, ITC_PriorityLevel_TypeDef PriorityValue)
 649                     ; 208 {
 650                     	switch	.text
 651  00a5               _ITC_SetSoftwarePriority:
 653  00a5 89            	pushw	x
 654  00a6 89            	pushw	x
 655       00000002      OFST:	set	2
 658                     ; 210     uint8_t Mask = 0;
 660  00a7 7b01          	ld	a,(OFST-1,sp)
 661  00a9 97            	ld	xl,a
 662                     ; 211     uint8_t NewPriority = 0;
 664  00aa 7b02          	ld	a,(OFST+0,sp)
 665  00ac 97            	ld	xl,a
 666                     ; 214     assert_param(IS_ITC_IRQ_OK((uint8_t)IrqNum));
 668                     ; 215     assert_param(IS_ITC_PRIORITY_OK(PriorityValue));
 670                     ; 218     assert_param(IS_ITC_INTERRUPTS_DISABLED);
 672                     ; 222     Mask = (uint8_t)(~(uint8_t)(0x03U << (((uint8_t)IrqNum % 4U) * 2U)));
 674  00ad 7b03          	ld	a,(OFST+1,sp)
 675  00af a403          	and	a,#3
 676  00b1 48            	sll	a
 677  00b2 5f            	clrw	x
 678  00b3 97            	ld	xl,a
 679  00b4 a603          	ld	a,#3
 680  00b6 5d            	tnzw	x
 681  00b7 2704          	jreq	L23
 682  00b9               L43:
 683  00b9 48            	sll	a
 684  00ba 5a            	decw	x
 685  00bb 26fc          	jrne	L43
 686  00bd               L23:
 687  00bd 43            	cpl	a
 688  00be 6b01          	ld	(OFST-1,sp),a
 689                     ; 225     NewPriority = (uint8_t)((uint8_t)(PriorityValue) << (((uint8_t)IrqNum % 4U) * 2U));
 691  00c0 7b03          	ld	a,(OFST+1,sp)
 692  00c2 a403          	and	a,#3
 693  00c4 48            	sll	a
 694  00c5 5f            	clrw	x
 695  00c6 97            	ld	xl,a
 696  00c7 7b04          	ld	a,(OFST+2,sp)
 697  00c9 5d            	tnzw	x
 698  00ca 2704          	jreq	L63
 699  00cc               L04:
 700  00cc 48            	sll	a
 701  00cd 5a            	decw	x
 702  00ce 26fc          	jrne	L04
 703  00d0               L63:
 704  00d0 6b02          	ld	(OFST+0,sp),a
 705                     ; 227     switch (IrqNum)
 707  00d2 7b03          	ld	a,(OFST+1,sp)
 709                     ; 311     default:
 709                     ; 312         break;
 710  00d4 a119          	cp	a,#25
 711  00d6 2407          	jruge	L24
 712  00d8 5f            	clrw	x
 713  00d9 97            	ld	xl,a
 714  00da 58            	sllw	x
 715  00db de0032        	ldw	x,(L44,x)
 716  00de fc            	jp	(x)
 717  00df               L24:
 718  00df 207c          	jra	L162
 719  00e1               L502:
 720                     ; 230     case ITC_IRQ_TLI: /* TLI software priority can be written but has no meaning */
 720                     ; 231     case ITC_IRQ_AWU:
 720                     ; 232     case ITC_IRQ_CLK:
 720                     ; 233     case ITC_IRQ_PORTA:
 720                     ; 234         ITC->ISPR1 &= Mask;
 722  00e1 c67f70        	ld	a,32624
 723  00e4 1401          	and	a,(OFST-1,sp)
 724  00e6 c77f70        	ld	32624,a
 725                     ; 235         ITC->ISPR1 |= NewPriority;
 727  00e9 c67f70        	ld	a,32624
 728  00ec 1a02          	or	a,(OFST+0,sp)
 729  00ee c77f70        	ld	32624,a
 730                     ; 236         break;
 732  00f1 206a          	jra	L162
 733  00f3               L702:
 734                     ; 238     case ITC_IRQ_PORTB:
 734                     ; 239     case ITC_IRQ_PORTC:
 734                     ; 240     case ITC_IRQ_PORTD:
 734                     ; 241     case ITC_IRQ_PORTE:
 734                     ; 242         ITC->ISPR2 &= Mask;
 736  00f3 c67f71        	ld	a,32625
 737  00f6 1401          	and	a,(OFST-1,sp)
 738  00f8 c77f71        	ld	32625,a
 739                     ; 243         ITC->ISPR2 |= NewPriority;
 741  00fb c67f71        	ld	a,32625
 742  00fe 1a02          	or	a,(OFST+0,sp)
 743  0100 c77f71        	ld	32625,a
 744                     ; 244         break;
 746  0103 2058          	jra	L162
 747  0105               L112:
 748                     ; 254     case ITC_IRQ_SPI:
 748                     ; 255     case ITC_IRQ_TIM1_OVF:
 748                     ; 256         ITC->ISPR3 &= Mask;
 750  0105 c67f72        	ld	a,32626
 751  0108 1401          	and	a,(OFST-1,sp)
 752  010a c77f72        	ld	32626,a
 753                     ; 257         ITC->ISPR3 |= NewPriority;
 755  010d c67f72        	ld	a,32626
 756  0110 1a02          	or	a,(OFST+0,sp)
 757  0112 c77f72        	ld	32626,a
 758                     ; 258         break;
 760  0115 2046          	jra	L162
 761  0117               L312:
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
 764  0117 c67f73        	ld	a,32627
 765  011a 1401          	and	a,(OFST-1,sp)
 766  011c c77f73        	ld	32627,a
 767                     ; 271         ITC->ISPR4 |= NewPriority;
 769  011f c67f73        	ld	a,32627
 770  0122 1a02          	or	a,(OFST+0,sp)
 771  0124 c77f73        	ld	32627,a
 772                     ; 272         break;
 774  0127 2034          	jra	L162
 775  0129               L512:
 776                     ; 274     case ITC_IRQ_TIM3_CAPCOM:
 776                     ; 275     case ITC_IRQ_UART1_TX:
 776                     ; 276     case ITC_IRQ_UART1_RX:
 776                     ; 277     case ITC_IRQ_I2C:
 776                     ; 278         ITC->ISPR5 &= Mask;
 778  0129 c67f74        	ld	a,32628
 779  012c 1401          	and	a,(OFST-1,sp)
 780  012e c77f74        	ld	32628,a
 781                     ; 279         ITC->ISPR5 |= NewPriority;
 783  0131 c67f74        	ld	a,32628
 784  0134 1a02          	or	a,(OFST+0,sp)
 785  0136 c77f74        	ld	32628,a
 786                     ; 280         break;
 788  0139 2022          	jra	L162
 789  013b               L712:
 790                     ; 294     case ITC_IRQ_ADC1:
 790                     ; 295 #endif /*STM8S105, STM8S103 or STM8S905 or STM8AF626x */
 790                     ; 296 
 790                     ; 297 #ifdef STM8S903
 790                     ; 298     case ITC_IRQ_TIM6_OVFTRI:
 790                     ; 299 #else
 790                     ; 300     case ITC_IRQ_TIM4_OVF:
 790                     ; 301 #endif /*STM8S903*/
 790                     ; 302         ITC->ISPR6 &= Mask;
 792  013b c67f75        	ld	a,32629
 793  013e 1401          	and	a,(OFST-1,sp)
 794  0140 c77f75        	ld	32629,a
 795                     ; 303         ITC->ISPR6 |= NewPriority;
 797  0143 c67f75        	ld	a,32629
 798  0146 1a02          	or	a,(OFST+0,sp)
 799  0148 c77f75        	ld	32629,a
 800                     ; 304         break;
 802  014b 2010          	jra	L162
 803  014d               L122:
 804                     ; 306     case ITC_IRQ_EEPROM_EEC:
 804                     ; 307         ITC->ISPR7 &= Mask;
 806  014d c67f76        	ld	a,32630
 807  0150 1401          	and	a,(OFST-1,sp)
 808  0152 c77f76        	ld	32630,a
 809                     ; 308         ITC->ISPR7 |= NewPriority;
 811  0155 c67f76        	ld	a,32630
 812  0158 1a02          	or	a,(OFST+0,sp)
 813  015a c77f76        	ld	32630,a
 814                     ; 309         break;
 816  015d               L322:
 817                     ; 311     default:
 817                     ; 312         break;
 819  015d               L162:
 820                     ; 316 }
 823  015d 5b04          	addw	sp,#4
 824  015f 81            	ret
 837                     	xdef	_ITC_GetSoftwarePriority
 838                     	xdef	_ITC_SetSoftwarePriority
 839                     	xdef	_ITC_GetSoftIntStatus
 840                     	xdef	_ITC_DeInit
 841                     	xdef	_ITC_GetCPUCC
 860                     	end
