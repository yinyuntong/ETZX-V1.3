   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     .const:	section	.text
   5  0000               _HSIDivFactor:
   6  0000 01            	dc.b	1
   7  0001 02            	dc.b	2
   8  0002 04            	dc.b	4
   9  0003 08            	dc.b	8
  10  0004               _CLKPrescTable:
  11  0004 01            	dc.b	1
  12  0005 02            	dc.b	2
  13  0006 04            	dc.b	4
  14  0007 08            	dc.b	8
  15  0008 0a            	dc.b	10
  16  0009 10            	dc.b	16
  17  000a 14            	dc.b	20
  18  000b 28            	dc.b	40
  59                     ; 70 void CLK_DeInit(void)
  59                     ; 71 {
  61                     .text:	section	.text,new
  62  0000               _CLK_DeInit:
  64  0000 89            	pushw	x
  65       00000002      OFST:	set	2
  68                     ; 72 	uint16_t x=0;
  70  0001 5f            	clrw	x
  71  0002 1f01          	ldw	(OFST-1,sp),x
  72                     ; 73     CLK->ICKR = CLK_ICKR_RESET_VALUE;
  74  0004 350150c0      	mov	20672,#1
  75                     ; 74     CLK->ECKR = CLK_ECKR_RESET_VALUE;
  77  0008 725f50c1      	clr	20673
  78                     ; 75     CLK->SWR  = CLK_SWR_RESET_VALUE;
  80  000c 35e150c4      	mov	20676,#225
  81                     ; 76     CLK->SWCR = CLK_SWCR_RESET_VALUE;
  83  0010 725f50c5      	clr	20677
  84                     ; 77     CLK->CKDIVR = CLK_CKDIVR_RESET_VALUE;
  86  0014 351850c6      	mov	20678,#24
  87                     ; 78     CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
  89  0018 35ff50c7      	mov	20679,#255
  90                     ; 79     CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
  92  001c 35ff50ca      	mov	20682,#255
  93                     ; 80     CLK->CSSR = CLK_CSSR_RESET_VALUE;
  95  0020 725f50c8      	clr	20680
  96                     ; 81     CLK->CCOR = CLK_CCOR_RESET_VALUE;
  98  0024 725f50c9      	clr	20681
 100  0028 2022          	jra	L33
 101  002a               L72:
 102                     ; 84     	Delayms(20);
 104  002a ae0014        	ldw	x,#20
 105  002d cd0000        	call	_Delayms
 107                     ; 85 		x++;
 109  0030 1e01          	ldw	x,(OFST-1,sp)
 110  0032 1c0001        	addw	x,#1
 111  0035 1f01          	ldw	(OFST-1,sp),x
 112                     ; 86 		if(x>=500) break;
 114  0037 1e01          	ldw	x,(OFST-1,sp)
 115  0039 a301f4        	cpw	x,#500
 116  003c 250e          	jrult	L33
 118  003e               L53:
 119                     ; 88     CLK->CCOR = CLK_CCOR_RESET_VALUE;
 121  003e 725f50c9      	clr	20681
 122                     ; 89     CLK->HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
 124  0042 725f50cc      	clr	20684
 125                     ; 90     CLK->SWIMCCR = CLK_SWIMCCR_RESET_VALUE;
 127  0046 725f50cd      	clr	20685
 128                     ; 92 }
 131  004a 85            	popw	x
 132  004b 81            	ret
 133  004c               L33:
 134                     ; 82     while ((CLK->CCOR & CLK_CCOR_CCOEN)!= 0)
 136  004c c650c9        	ld	a,20681
 137  004f a501          	bcp	a,#1
 138  0051 26d7          	jrne	L72
 139  0053 20e9          	jra	L53
 195                     ; 103 void CLK_FastHaltWakeUpCmd(FunctionalState NewState)
 195                     ; 104 {
 196                     .text:	section	.text,new
 197  0000               _CLK_FastHaltWakeUpCmd:
 201                     ; 107     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 203                     ; 109     if (NewState != DISABLE)
 205  0000 4d            	tnz	a
 206  0001 2706          	jreq	L76
 207                     ; 112         CLK->ICKR |= CLK_ICKR_FHWU;
 209  0003 721450c0      	bset	20672,#2
 211  0007 2004          	jra	L17
 212  0009               L76:
 213                     ; 117         CLK->ICKR &= (uint8_t)(~CLK_ICKR_FHWU);
 215  0009 721550c0      	bres	20672,#2
 216  000d               L17:
 217                     ; 120 }
 220  000d 81            	ret
 255                     ; 127 void CLK_HSECmd(FunctionalState NewState)
 255                     ; 128 {
 256                     .text:	section	.text,new
 257  0000               _CLK_HSECmd:
 261                     ; 131     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 263                     ; 133     if (NewState != DISABLE)
 265  0000 4d            	tnz	a
 266  0001 2706          	jreq	L111
 267                     ; 136         CLK->ECKR |= CLK_ECKR_HSEEN;
 269  0003 721050c1      	bset	20673,#0
 271  0007 2004          	jra	L311
 272  0009               L111:
 273                     ; 141         CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
 275  0009 721150c1      	bres	20673,#0
 276  000d               L311:
 277                     ; 144 }
 280  000d 81            	ret
 315                     ; 151 void CLK_HSICmd(FunctionalState NewState)
 315                     ; 152 {
 316                     .text:	section	.text,new
 317  0000               _CLK_HSICmd:
 321                     ; 155     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 323                     ; 157     if (NewState != DISABLE)
 325  0000 4d            	tnz	a
 326  0001 2706          	jreq	L331
 327                     ; 160         CLK->ICKR |= CLK_ICKR_HSIEN;
 329  0003 721050c0      	bset	20672,#0
 331  0007 2004          	jra	L531
 332  0009               L331:
 333                     ; 165         CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
 335  0009 721150c0      	bres	20672,#0
 336  000d               L531:
 337                     ; 168 }
 340  000d 81            	ret
 375                     ; 175 void CLK_LSICmd(FunctionalState NewState)
 375                     ; 176 {
 376                     .text:	section	.text,new
 377  0000               _CLK_LSICmd:
 381                     ; 179     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 383                     ; 181     if (NewState != DISABLE)
 385  0000 4d            	tnz	a
 386  0001 2706          	jreq	L551
 387                     ; 184         CLK->ICKR |= CLK_ICKR_LSIEN;
 389  0003 721650c0      	bset	20672,#3
 391  0007 2004          	jra	L751
 392  0009               L551:
 393                     ; 189         CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
 395  0009 721750c0      	bres	20672,#3
 396  000d               L751:
 397                     ; 192 }
 400  000d 81            	ret
 435                     ; 200 void CLK_CCOCmd(FunctionalState NewState)
 435                     ; 201 {
 436                     .text:	section	.text,new
 437  0000               _CLK_CCOCmd:
 441                     ; 204     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 443                     ; 206     if (NewState != DISABLE)
 445  0000 4d            	tnz	a
 446  0001 2706          	jreq	L771
 447                     ; 209         CLK->CCOR |= CLK_CCOR_CCOEN;
 449  0003 721050c9      	bset	20681,#0
 451  0007 2004          	jra	L102
 452  0009               L771:
 453                     ; 214         CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOEN);
 455  0009 721150c9      	bres	20681,#0
 456  000d               L102:
 457                     ; 217 }
 460  000d 81            	ret
 495                     ; 226 void CLK_ClockSwitchCmd(FunctionalState NewState)
 495                     ; 227 {
 496                     .text:	section	.text,new
 497  0000               _CLK_ClockSwitchCmd:
 501                     ; 230     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 503                     ; 232     if (NewState != DISABLE )
 505  0000 4d            	tnz	a
 506  0001 2706          	jreq	L122
 507                     ; 235         CLK->SWCR |= CLK_SWCR_SWEN;
 509  0003 721250c5      	bset	20677,#1
 511  0007 2004          	jra	L322
 512  0009               L122:
 513                     ; 240         CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWEN);
 515  0009 721350c5      	bres	20677,#1
 516  000d               L322:
 517                     ; 243 }
 520  000d 81            	ret
 556                     ; 253 void CLK_SlowActiveHaltWakeUpCmd(FunctionalState NewState)
 556                     ; 254 {
 557                     .text:	section	.text,new
 558  0000               _CLK_SlowActiveHaltWakeUpCmd:
 562                     ; 257     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 564                     ; 259     if (NewState != DISABLE)
 566  0000 4d            	tnz	a
 567  0001 2706          	jreq	L342
 568                     ; 262         CLK->ICKR |= CLK_ICKR_SWUAH;
 570  0003 721a50c0      	bset	20672,#5
 572  0007 2004          	jra	L542
 573  0009               L342:
 574                     ; 267         CLK->ICKR &= (uint8_t)(~CLK_ICKR_SWUAH);
 576  0009 721b50c0      	bres	20672,#5
 577  000d               L542:
 578                     ; 270 }
 581  000d 81            	ret
 740                     ; 280 void CLK_PeripheralClockConfig(CLK_Peripheral_TypeDef CLK_Peripheral, FunctionalState NewState)
 740                     ; 281 {
 741                     .text:	section	.text,new
 742  0000               _CLK_PeripheralClockConfig:
 744  0000 89            	pushw	x
 745       00000000      OFST:	set	0
 748                     ; 284     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 750                     ; 285     assert_param(IS_CLK_PERIPHERAL_OK(CLK_Peripheral));
 752                     ; 287     if (((uint8_t)CLK_Peripheral & (uint8_t)0x10) == 0x00)
 754  0001 9e            	ld	a,xh
 755  0002 a510          	bcp	a,#16
 756  0004 2633          	jrne	L133
 757                     ; 289         if (NewState != DISABLE)
 759  0006 0d02          	tnz	(OFST+2,sp)
 760  0008 2717          	jreq	L333
 761                     ; 292             CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F));
 763  000a 7b01          	ld	a,(OFST+1,sp)
 764  000c a40f          	and	a,#15
 765  000e 5f            	clrw	x
 766  000f 97            	ld	xl,a
 767  0010 a601          	ld	a,#1
 768  0012 5d            	tnzw	x
 769  0013 2704          	jreq	L62
 770  0015               L03:
 771  0015 48            	sll	a
 772  0016 5a            	decw	x
 773  0017 26fc          	jrne	L03
 774  0019               L62:
 775  0019 ca50c7        	or	a,20679
 776  001c c750c7        	ld	20679,a
 778  001f 2049          	jra	L733
 779  0021               L333:
 780                     ; 297             CLK->PCKENR1 &= (uint8_t)(~(uint8_t)(((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F))));
 782  0021 7b01          	ld	a,(OFST+1,sp)
 783  0023 a40f          	and	a,#15
 784  0025 5f            	clrw	x
 785  0026 97            	ld	xl,a
 786  0027 a601          	ld	a,#1
 787  0029 5d            	tnzw	x
 788  002a 2704          	jreq	L23
 789  002c               L43:
 790  002c 48            	sll	a
 791  002d 5a            	decw	x
 792  002e 26fc          	jrne	L43
 793  0030               L23:
 794  0030 43            	cpl	a
 795  0031 c450c7        	and	a,20679
 796  0034 c750c7        	ld	20679,a
 797  0037 2031          	jra	L733
 798  0039               L133:
 799                     ; 302         if (NewState != DISABLE)
 801  0039 0d02          	tnz	(OFST+2,sp)
 802  003b 2717          	jreq	L143
 803                     ; 305             CLK->PCKENR2 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F));
 805  003d 7b01          	ld	a,(OFST+1,sp)
 806  003f a40f          	and	a,#15
 807  0041 5f            	clrw	x
 808  0042 97            	ld	xl,a
 809  0043 a601          	ld	a,#1
 810  0045 5d            	tnzw	x
 811  0046 2704          	jreq	L63
 812  0048               L04:
 813  0048 48            	sll	a
 814  0049 5a            	decw	x
 815  004a 26fc          	jrne	L04
 816  004c               L63:
 817  004c ca50ca        	or	a,20682
 818  004f c750ca        	ld	20682,a
 820  0052 2016          	jra	L733
 821  0054               L143:
 822                     ; 310             CLK->PCKENR2 &= (uint8_t)(~(uint8_t)(((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F))));
 824  0054 7b01          	ld	a,(OFST+1,sp)
 825  0056 a40f          	and	a,#15
 826  0058 5f            	clrw	x
 827  0059 97            	ld	xl,a
 828  005a a601          	ld	a,#1
 829  005c 5d            	tnzw	x
 830  005d 2704          	jreq	L24
 831  005f               L44:
 832  005f 48            	sll	a
 833  0060 5a            	decw	x
 834  0061 26fc          	jrne	L44
 835  0063               L24:
 836  0063 43            	cpl	a
 837  0064 c450ca        	and	a,20682
 838  0067 c750ca        	ld	20682,a
 839  006a               L733:
 840                     ; 314 }
 843  006a 85            	popw	x
 844  006b 81            	ret
1032                     ; 327 ErrorStatus CLK_ClockSwitchConfig(CLK_SwitchMode_TypeDef CLK_SwitchMode, CLK_Source_TypeDef CLK_NewClock, FunctionalState ITState, CLK_CurrentClockState_TypeDef CLK_CurrentClockState)
1032                     ; 328 {
1033                     .text:	section	.text,new
1034  0000               _CLK_ClockSwitchConfig:
1036  0000 89            	pushw	x
1037  0001 5204          	subw	sp,#4
1038       00000004      OFST:	set	4
1041                     ; 331     uint16_t DownCounter = CLK_TIMEOUT;
1043  0003 ae0491        	ldw	x,#1169
1044  0006 1f03          	ldw	(OFST-1,sp),x
1045                     ; 332     ErrorStatus Swif = ERROR;
1047  0008 7b02          	ld	a,(OFST-2,sp)
1048  000a 97            	ld	xl,a
1049                     ; 335     assert_param(IS_CLK_SOURCE_OK(CLK_NewClock));
1051                     ; 336     assert_param(IS_CLK_SWITCHMODE_OK(CLK_SwitchMode));
1053                     ; 337     assert_param(IS_FUNCTIONALSTATE_OK(ITState));
1055                     ; 338     assert_param(IS_CLK_CURRENTCLOCKSTATE_OK(CLK_CurrentClockState));
1057                     ; 341     clock_master = (CLK_Source_TypeDef)CLK->CMSR;
1059  000b c650c3        	ld	a,20675
1060  000e 6b01          	ld	(OFST-3,sp),a
1061                     ; 344     if (CLK_SwitchMode == CLK_SWITCHMODE_AUTO)
1063  0010 7b05          	ld	a,(OFST+1,sp)
1064  0012 a101          	cp	a,#1
1065  0014 2639          	jrne	L554
1066                     ; 348         CLK->SWCR |= CLK_SWCR_SWEN;
1068  0016 721250c5      	bset	20677,#1
1069                     ; 351         if (ITState != DISABLE)
1071  001a 0d09          	tnz	(OFST+5,sp)
1072  001c 2706          	jreq	L754
1073                     ; 353             CLK->SWCR |= CLK_SWCR_SWIEN;
1075  001e 721450c5      	bset	20677,#2
1077  0022 2004          	jra	L164
1078  0024               L754:
1079                     ; 357             CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
1081  0024 721550c5      	bres	20677,#2
1082  0028               L164:
1083                     ; 361         CLK->SWR = (uint8_t)CLK_NewClock;
1085  0028 7b06          	ld	a,(OFST+2,sp)
1086  002a c750c4        	ld	20676,a
1088  002d 2007          	jra	L764
1089  002f               L364:
1090                     ; 365             DownCounter--;
1092  002f 1e03          	ldw	x,(OFST-1,sp)
1093  0031 1d0001        	subw	x,#1
1094  0034 1f03          	ldw	(OFST-1,sp),x
1095  0036               L764:
1096                     ; 363         while ((((CLK->SWCR & CLK_SWCR_SWBSY) != 0 )&& (DownCounter != 0)))
1098  0036 c650c5        	ld	a,20677
1099  0039 a501          	bcp	a,#1
1100  003b 2704          	jreq	L374
1102  003d 1e03          	ldw	x,(OFST-1,sp)
1103  003f 26ee          	jrne	L364
1104  0041               L374:
1105                     ; 368         if (DownCounter != 0)
1107  0041 1e03          	ldw	x,(OFST-1,sp)
1108  0043 2706          	jreq	L574
1109                     ; 370             Swif = SUCCESS;
1111  0045 a601          	ld	a,#1
1112  0047 6b02          	ld	(OFST-2,sp),a
1114  0049 201b          	jra	L105
1115  004b               L574:
1116                     ; 374             Swif = ERROR;
1118  004b 0f02          	clr	(OFST-2,sp)
1119  004d 2017          	jra	L105
1120  004f               L554:
1121                     ; 382         if (ITState != DISABLE)
1123  004f 0d09          	tnz	(OFST+5,sp)
1124  0051 2706          	jreq	L305
1125                     ; 384             CLK->SWCR |= CLK_SWCR_SWIEN;
1127  0053 721450c5      	bset	20677,#2
1129  0057 2004          	jra	L505
1130  0059               L305:
1131                     ; 388             CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
1133  0059 721550c5      	bres	20677,#2
1134  005d               L505:
1135                     ; 392         CLK->SWR = (uint8_t)CLK_NewClock;
1137  005d 7b06          	ld	a,(OFST+2,sp)
1138  005f c750c4        	ld	20676,a
1139                     ; 396         Swif = SUCCESS;
1141  0062 a601          	ld	a,#1
1142  0064 6b02          	ld	(OFST-2,sp),a
1143  0066               L105:
1144                     ; 401     if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_HSI))
1146  0066 0d0a          	tnz	(OFST+6,sp)
1147  0068 260c          	jrne	L705
1149  006a 7b01          	ld	a,(OFST-3,sp)
1150  006c a1e1          	cp	a,#225
1151  006e 2606          	jrne	L705
1152                     ; 403         CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
1154  0070 721150c0      	bres	20672,#0
1156  0074 201e          	jra	L115
1157  0076               L705:
1158                     ; 405     else if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_LSI))
1160  0076 0d0a          	tnz	(OFST+6,sp)
1161  0078 260c          	jrne	L315
1163  007a 7b01          	ld	a,(OFST-3,sp)
1164  007c a1d2          	cp	a,#210
1165  007e 2606          	jrne	L315
1166                     ; 407         CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
1168  0080 721750c0      	bres	20672,#3
1170  0084 200e          	jra	L115
1171  0086               L315:
1172                     ; 409     else if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_HSE))
1174  0086 0d0a          	tnz	(OFST+6,sp)
1175  0088 260a          	jrne	L115
1177  008a 7b01          	ld	a,(OFST-3,sp)
1178  008c a1b4          	cp	a,#180
1179  008e 2604          	jrne	L115
1180                     ; 411         CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
1182  0090 721150c1      	bres	20673,#0
1183  0094               L115:
1184                     ; 414     return(Swif);
1186  0094 7b02          	ld	a,(OFST-2,sp)
1189  0096 5b06          	addw	sp,#6
1190  0098 81            	ret
1328                     ; 424 void CLK_HSIPrescalerConfig(CLK_Prescaler_TypeDef HSIPrescaler)
1328                     ; 425 {
1329                     .text:	section	.text,new
1330  0000               _CLK_HSIPrescalerConfig:
1332  0000 88            	push	a
1333       00000000      OFST:	set	0
1336                     ; 428     assert_param(IS_CLK_HSIPRESCALER_OK(HSIPrescaler));
1338                     ; 431     CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
1340  0001 c650c6        	ld	a,20678
1341  0004 a4e7          	and	a,#231
1342  0006 c750c6        	ld	20678,a
1343                     ; 434     CLK->CKDIVR |= (uint8_t)HSIPrescaler;
1345  0009 c650c6        	ld	a,20678
1346  000c 1a01          	or	a,(OFST+1,sp)
1347  000e c750c6        	ld	20678,a
1348                     ; 436 }
1351  0011 84            	pop	a
1352  0012 81            	ret
1487                     ; 447 void CLK_CCOConfig(CLK_Output_TypeDef CLK_CCO)
1487                     ; 448 {
1488                     .text:	section	.text,new
1489  0000               _CLK_CCOConfig:
1491  0000 88            	push	a
1492       00000000      OFST:	set	0
1495                     ; 451     assert_param(IS_CLK_OUTPUT_OK(CLK_CCO));
1497                     ; 454     CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOSEL);
1499  0001 c650c9        	ld	a,20681
1500  0004 a4e1          	and	a,#225
1501  0006 c750c9        	ld	20681,a
1502                     ; 457     CLK->CCOR |= (uint8_t)CLK_CCO;
1504  0009 c650c9        	ld	a,20681
1505  000c 1a01          	or	a,(OFST+1,sp)
1506  000e c750c9        	ld	20681,a
1507                     ; 460     CLK->CCOR |= CLK_CCOR_CCOEN;
1509  0011 721050c9      	bset	20681,#0
1510                     ; 462 }
1513  0015 84            	pop	a
1514  0016 81            	ret
1579                     ; 472 void CLK_ITConfig(CLK_IT_TypeDef CLK_IT, FunctionalState NewState)
1579                     ; 473 {
1580                     .text:	section	.text,new
1581  0000               _CLK_ITConfig:
1583  0000 89            	pushw	x
1584       00000000      OFST:	set	0
1587                     ; 476     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1589                     ; 477     assert_param(IS_CLK_IT_OK(CLK_IT));
1591                     ; 479     if (NewState != DISABLE)
1593  0001 9f            	ld	a,xl
1594  0002 4d            	tnz	a
1595  0003 2719          	jreq	L517
1596                     ; 481         switch (CLK_IT)
1598  0005 9e            	ld	a,xh
1600                     ; 489         default:
1600                     ; 490             break;
1601  0006 a00c          	sub	a,#12
1602  0008 270a          	jreq	L156
1603  000a a010          	sub	a,#16
1604  000c 2624          	jrne	L327
1605                     ; 483         case CLK_IT_SWIF: /* Enable the clock switch interrupt */
1605                     ; 484             CLK->SWCR |= CLK_SWCR_SWIEN;
1607  000e 721450c5      	bset	20677,#2
1608                     ; 485             break;
1610  0012 201e          	jra	L327
1611  0014               L156:
1612                     ; 486         case CLK_IT_CSSD: /* Enable the clock security system detection interrupt */
1612                     ; 487             CLK->CSSR |= CLK_CSSR_CSSDIE;
1614  0014 721450c8      	bset	20680,#2
1615                     ; 488             break;
1617  0018 2018          	jra	L327
1618  001a               L356:
1619                     ; 489         default:
1619                     ; 490             break;
1621  001a 2016          	jra	L327
1622  001c               L127:
1624  001c 2014          	jra	L327
1625  001e               L517:
1626                     ; 495         switch (CLK_IT)
1628  001e 7b01          	ld	a,(OFST+1,sp)
1630                     ; 503         default:
1630                     ; 504             break;
1631  0020 a00c          	sub	a,#12
1632  0022 270a          	jreq	L756
1633  0024 a010          	sub	a,#16
1634  0026 260a          	jrne	L327
1635                     ; 497         case CLK_IT_SWIF: /* Disable the clock switch interrupt */
1635                     ; 498             CLK->SWCR  &= (uint8_t)(~CLK_SWCR_SWIEN);
1637  0028 721550c5      	bres	20677,#2
1638                     ; 499             break;
1640  002c 2004          	jra	L327
1641  002e               L756:
1642                     ; 500         case CLK_IT_CSSD: /* Disable the clock security system detection interrupt */
1642                     ; 501             CLK->CSSR &= (uint8_t)(~CLK_CSSR_CSSDIE);
1644  002e 721550c8      	bres	20680,#2
1645                     ; 502             break;
1646  0032               L327:
1647                     ; 508 }
1650  0032 85            	popw	x
1651  0033 81            	ret
1652  0034               L166:
1653                     ; 503         default:
1653                     ; 504             break;
1655  0034 20fc          	jra	L327
1656  0036               L727:
1657  0036 20fa          	jra	L327
1692                     ; 515 void CLK_SYSCLKConfig(CLK_Prescaler_TypeDef CLK_Prescaler)
1692                     ; 516 {
1693                     .text:	section	.text,new
1694  0000               _CLK_SYSCLKConfig:
1696  0000 88            	push	a
1697       00000000      OFST:	set	0
1700                     ; 519     assert_param(IS_CLK_PRESCALER_OK(CLK_Prescaler));
1702                     ; 521     if (((uint8_t)CLK_Prescaler & (uint8_t)0x80) == 0x00) /* Bit7 = 0 means HSI divider */
1704  0001 a580          	bcp	a,#128
1705  0003 2614          	jrne	L747
1706                     ; 523         CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
1708  0005 c650c6        	ld	a,20678
1709  0008 a4e7          	and	a,#231
1710  000a c750c6        	ld	20678,a
1711                     ; 524         CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_Prescaler & (uint8_t)CLK_CKDIVR_HSIDIV);
1713  000d 7b01          	ld	a,(OFST+1,sp)
1714  000f a418          	and	a,#24
1715  0011 ca50c6        	or	a,20678
1716  0014 c750c6        	ld	20678,a
1718  0017 2012          	jra	L157
1719  0019               L747:
1720                     ; 528         CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_CPUDIV);
1722  0019 c650c6        	ld	a,20678
1723  001c a4f8          	and	a,#248
1724  001e c750c6        	ld	20678,a
1725                     ; 529         CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_Prescaler & (uint8_t)CLK_CKDIVR_CPUDIV);
1727  0021 7b01          	ld	a,(OFST+1,sp)
1728  0023 a407          	and	a,#7
1729  0025 ca50c6        	or	a,20678
1730  0028 c750c6        	ld	20678,a
1731  002b               L157:
1732                     ; 532 }
1735  002b 84            	pop	a
1736  002c 81            	ret
1792                     ; 539 void CLK_SWIMConfig(CLK_SWIMDivider_TypeDef CLK_SWIMDivider)
1792                     ; 540 {
1793                     .text:	section	.text,new
1794  0000               _CLK_SWIMConfig:
1798                     ; 543     assert_param(IS_CLK_SWIMDIVIDER_OK(CLK_SWIMDivider));
1800                     ; 545     if (CLK_SWIMDivider != CLK_SWIMDIVIDER_2)
1802  0000 4d            	tnz	a
1803  0001 2706          	jreq	L1001
1804                     ; 548         CLK->SWIMCCR |= CLK_SWIMCCR_SWIMDIV;
1806  0003 721050cd      	bset	20685,#0
1808  0007 2004          	jra	L3001
1809  0009               L1001:
1810                     ; 553         CLK->SWIMCCR &= (uint8_t)(~CLK_SWIMCCR_SWIMDIV);
1812  0009 721150cd      	bres	20685,#0
1813  000d               L3001:
1814                     ; 556 }
1817  000d 81            	ret
1841                     ; 565 void CLK_ClockSecuritySystemEnable(void)
1841                     ; 566 {
1842                     .text:	section	.text,new
1843  0000               _CLK_ClockSecuritySystemEnable:
1847                     ; 568     CLK->CSSR |= CLK_CSSR_CSSEN;
1849  0000 721050c8      	bset	20680,#0
1850                     ; 569 }
1853  0004 81            	ret
1878                     ; 577 CLK_Source_TypeDef CLK_GetSYSCLKSource(void)
1878                     ; 578 {
1879                     .text:	section	.text,new
1880  0000               _CLK_GetSYSCLKSource:
1884                     ; 579     return((CLK_Source_TypeDef)CLK->CMSR);
1886  0000 c650c3        	ld	a,20675
1889  0003 81            	ret
1952                     ; 587 uint32_t CLK_GetClockFreq(void)
1952                     ; 588 {
1953                     .text:	section	.text,new
1954  0000               _CLK_GetClockFreq:
1956  0000 5209          	subw	sp,#9
1957       00000009      OFST:	set	9
1960                     ; 590     uint32_t clockfrequency = 0;
1962  0002 96            	ldw	x,sp
1963  0003 1c0005        	addw	x,#OFST-4
1964  0006 cd0000        	call	c_ltor
1966                     ; 591     CLK_Source_TypeDef clocksource = CLK_SOURCE_HSI;
1968  0009 7b09          	ld	a,(OFST+0,sp)
1969  000b 97            	ld	xl,a
1970                     ; 592     uint8_t tmp = 0, presc = 0;
1972  000c 7b09          	ld	a,(OFST+0,sp)
1973  000e 97            	ld	xl,a
1976  000f 7b09          	ld	a,(OFST+0,sp)
1977  0011 97            	ld	xl,a
1978                     ; 595     clocksource = (CLK_Source_TypeDef)CLK->CMSR;
1980  0012 c650c3        	ld	a,20675
1981  0015 6b09          	ld	(OFST+0,sp),a
1982                     ; 597     if (clocksource == CLK_SOURCE_HSI)
1984  0017 7b09          	ld	a,(OFST+0,sp)
1985  0019 a1e1          	cp	a,#225
1986  001b 2642          	jrne	L7501
1987                     ; 599         tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
1989  001d c650c6        	ld	a,20678
1990  0020 a418          	and	a,#24
1991  0022 6b09          	ld	(OFST+0,sp),a
1992                     ; 600         tmp = (uint8_t)(tmp >> 3);
1994  0024 7b09          	ld	a,(OFST+0,sp)
1995  0026 44            	srl	a
1996  0027 44            	srl	a
1997  0028 44            	srl	a
1998  0029 6b09          	ld	(OFST+0,sp),a
1999                     ; 601         presc = HSIDivFactor[tmp];
2001  002b 7b09          	ld	a,(OFST+0,sp)
2002  002d 5f            	clrw	x
2003  002e 97            	ld	xl,a
2004  002f d60000        	ld	a,(_HSIDivFactor,x)
2005  0032 6b09          	ld	(OFST+0,sp),a
2006                     ; 602         clockfrequency = HSI_VALUE / presc;
2008  0034 7b09          	ld	a,(OFST+0,sp)
2009  0036 b703          	ld	c_lreg+3,a
2010  0038 3f02          	clr	c_lreg+2
2011  003a 3f01          	clr	c_lreg+1
2012  003c 3f00          	clr	c_lreg
2013  003e 96            	ldw	x,sp
2014  003f 1c0001        	addw	x,#OFST-8
2015  0042 cd0000        	call	c_rtol
2017  0045 ae2400        	ldw	x,#9216
2018  0048 bf02          	ldw	c_lreg+2,x
2019  004a ae00f4        	ldw	x,#244
2020  004d bf00          	ldw	c_lreg,x
2021  004f 96            	ldw	x,sp
2022  0050 1c0001        	addw	x,#OFST-8
2023  0053 cd0000        	call	c_ludv
2025  0056 96            	ldw	x,sp
2026  0057 1c0005        	addw	x,#OFST-4
2027  005a cd0000        	call	c_rtol
2030  005d 201c          	jra	L1601
2031  005f               L7501:
2032                     ; 604     else if ( clocksource == CLK_SOURCE_LSI)
2034  005f 7b09          	ld	a,(OFST+0,sp)
2035  0061 a1d2          	cp	a,#210
2036  0063 260c          	jrne	L3601
2037                     ; 606         clockfrequency = LSI_VALUE;
2039  0065 aef400        	ldw	x,#62464
2040  0068 1f07          	ldw	(OFST-2,sp),x
2041  006a ae0001        	ldw	x,#1
2042  006d 1f05          	ldw	(OFST-4,sp),x
2044  006f 200a          	jra	L1601
2045  0071               L3601:
2046                     ; 610         clockfrequency = HSE_VALUE;
2048  0071 ae2400        	ldw	x,#9216
2049  0074 1f07          	ldw	(OFST-2,sp),x
2050  0076 ae00f4        	ldw	x,#244
2051  0079 1f05          	ldw	(OFST-4,sp),x
2052  007b               L1601:
2053                     ; 613     return((uint32_t)clockfrequency);
2055  007b 96            	ldw	x,sp
2056  007c 1c0005        	addw	x,#OFST-4
2057  007f cd0000        	call	c_ltor
2061  0082 5b09          	addw	sp,#9
2062  0084 81            	ret
2161                     ; 624 void CLK_AdjustHSICalibrationValue(CLK_HSITrimValue_TypeDef CLK_HSICalibrationValue)
2161                     ; 625 {
2162                     .text:	section	.text,new
2163  0000               _CLK_AdjustHSICalibrationValue:
2165  0000 88            	push	a
2166       00000000      OFST:	set	0
2169                     ; 628     assert_param(IS_CLK_HSITRIMVALUE_OK(CLK_HSICalibrationValue));
2171                     ; 631     CLK->HSITRIMR = (uint8_t)( (uint8_t)(CLK->HSITRIMR & (uint8_t)(~CLK_HSITRIMR_HSITRIM))|((uint8_t)CLK_HSICalibrationValue));
2173  0001 c650cc        	ld	a,20684
2174  0004 a4f8          	and	a,#248
2175  0006 1a01          	or	a,(OFST+1,sp)
2176  0008 c750cc        	ld	20684,a
2177                     ; 633 }
2180  000b 84            	pop	a
2181  000c 81            	ret
2205                     ; 644 void CLK_SYSCLKEmergencyClear(void)
2205                     ; 645 {
2206                     .text:	section	.text,new
2207  0000               _CLK_SYSCLKEmergencyClear:
2211                     ; 646     CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWBSY);
2213  0000 721150c5      	bres	20677,#0
2214                     ; 647 }
2217  0004 81            	ret
2370                     ; 656 FlagStatus CLK_GetFlagStatus(CLK_Flag_TypeDef CLK_FLAG)
2370                     ; 657 {
2371                     .text:	section	.text,new
2372  0000               _CLK_GetFlagStatus:
2374  0000 89            	pushw	x
2375  0001 5203          	subw	sp,#3
2376       00000003      OFST:	set	3
2379                     ; 659     uint16_t statusreg = 0;
2381  0003 1e01          	ldw	x,(OFST-2,sp)
2382                     ; 660     uint8_t tmpreg = 0;
2384  0005 7b03          	ld	a,(OFST+0,sp)
2385  0007 97            	ld	xl,a
2386                     ; 661     FlagStatus bitstatus = RESET;
2388  0008 7b03          	ld	a,(OFST+0,sp)
2389  000a 97            	ld	xl,a
2390                     ; 664     assert_param(IS_CLK_FLAG_OK(CLK_FLAG));
2392                     ; 667     statusreg = (uint16_t)((uint16_t)CLK_FLAG & (uint16_t)0xFF00);
2394  000b 7b04          	ld	a,(OFST+1,sp)
2395  000d 97            	ld	xl,a
2396  000e 7b05          	ld	a,(OFST+2,sp)
2397  0010 9f            	ld	a,xl
2398  0011 a4ff          	and	a,#255
2399  0013 97            	ld	xl,a
2400  0014 4f            	clr	a
2401  0015 02            	rlwa	x,a
2402  0016 1f01          	ldw	(OFST-2,sp),x
2403  0018 01            	rrwa	x,a
2404                     ; 670     if (statusreg == 0x0100) /* The flag to check is in ICKRregister */
2406  0019 1e01          	ldw	x,(OFST-2,sp)
2407  001b a30100        	cpw	x,#256
2408  001e 2607          	jrne	L1321
2409                     ; 672         tmpreg = CLK->ICKR;
2411  0020 c650c0        	ld	a,20672
2412  0023 6b03          	ld	(OFST+0,sp),a
2414  0025 202f          	jra	L3321
2415  0027               L1321:
2416                     ; 674     else if (statusreg == 0x0200) /* The flag to check is in ECKRregister */
2418  0027 1e01          	ldw	x,(OFST-2,sp)
2419  0029 a30200        	cpw	x,#512
2420  002c 2607          	jrne	L5321
2421                     ; 676         tmpreg = CLK->ECKR;
2423  002e c650c1        	ld	a,20673
2424  0031 6b03          	ld	(OFST+0,sp),a
2426  0033 2021          	jra	L3321
2427  0035               L5321:
2428                     ; 678     else if (statusreg == 0x0300) /* The flag to check is in SWIC register */
2430  0035 1e01          	ldw	x,(OFST-2,sp)
2431  0037 a30300        	cpw	x,#768
2432  003a 2607          	jrne	L1421
2433                     ; 680         tmpreg = CLK->SWCR;
2435  003c c650c5        	ld	a,20677
2436  003f 6b03          	ld	(OFST+0,sp),a
2438  0041 2013          	jra	L3321
2439  0043               L1421:
2440                     ; 682     else if (statusreg == 0x0400) /* The flag to check is in CSS register */
2442  0043 1e01          	ldw	x,(OFST-2,sp)
2443  0045 a30400        	cpw	x,#1024
2444  0048 2607          	jrne	L5421
2445                     ; 684         tmpreg = CLK->CSSR;
2447  004a c650c8        	ld	a,20680
2448  004d 6b03          	ld	(OFST+0,sp),a
2450  004f 2005          	jra	L3321
2451  0051               L5421:
2452                     ; 688         tmpreg = CLK->CCOR;
2454  0051 c650c9        	ld	a,20681
2455  0054 6b03          	ld	(OFST+0,sp),a
2456  0056               L3321:
2457                     ; 691     if ((tmpreg & (uint8_t)CLK_FLAG) != (uint8_t)RESET)
2459  0056 7b05          	ld	a,(OFST+2,sp)
2460  0058 1503          	bcp	a,(OFST+0,sp)
2461  005a 2706          	jreq	L1521
2462                     ; 693         bitstatus = SET;
2464  005c a601          	ld	a,#1
2465  005e 6b03          	ld	(OFST+0,sp),a
2467  0060 2002          	jra	L3521
2468  0062               L1521:
2469                     ; 697         bitstatus = RESET;
2471  0062 0f03          	clr	(OFST+0,sp)
2472  0064               L3521:
2473                     ; 701     return((FlagStatus)bitstatus);
2475  0064 7b03          	ld	a,(OFST+0,sp)
2478  0066 5b05          	addw	sp,#5
2479  0068 81            	ret
2525                     ; 711 ITStatus CLK_GetITStatus(CLK_IT_TypeDef CLK_IT)
2525                     ; 712 {
2526                     .text:	section	.text,new
2527  0000               _CLK_GetITStatus:
2529  0000 88            	push	a
2530  0001 88            	push	a
2531       00000001      OFST:	set	1
2534                     ; 714     ITStatus bitstatus = RESET;
2536  0002 0f01          	clr	(OFST+0,sp)
2537                     ; 717     assert_param(IS_CLK_IT_OK(CLK_IT));
2539                     ; 719     if (CLK_IT == CLK_IT_SWIF)
2541  0004 a11c          	cp	a,#28
2542  0006 2611          	jrne	L7721
2543                     ; 722         if ((CLK->SWCR & (uint8_t)CLK_IT) == (uint8_t)0x0C)
2545  0008 c450c5        	and	a,20677
2546  000b a10c          	cp	a,#12
2547  000d 2606          	jrne	L1031
2548                     ; 724             bitstatus = SET;
2550  000f a601          	ld	a,#1
2551  0011 6b01          	ld	(OFST+0,sp),a
2553  0013 2015          	jra	L5031
2554  0015               L1031:
2555                     ; 728             bitstatus = RESET;
2557  0015 0f01          	clr	(OFST+0,sp)
2558  0017 2011          	jra	L5031
2559  0019               L7721:
2560                     ; 734         if ((CLK->CSSR & (uint8_t)CLK_IT) == (uint8_t)0x0C)
2562  0019 c650c8        	ld	a,20680
2563  001c 1402          	and	a,(OFST+1,sp)
2564  001e a10c          	cp	a,#12
2565  0020 2606          	jrne	L7031
2566                     ; 736             bitstatus = SET;
2568  0022 a601          	ld	a,#1
2569  0024 6b01          	ld	(OFST+0,sp),a
2571  0026 2002          	jra	L5031
2572  0028               L7031:
2573                     ; 740             bitstatus = RESET;
2575  0028 0f01          	clr	(OFST+0,sp)
2576  002a               L5031:
2577                     ; 745     return bitstatus;
2579  002a 7b01          	ld	a,(OFST+0,sp)
2582  002c 85            	popw	x
2583  002d 81            	ret
2619                     ; 755 void CLK_ClearITPendingBit(CLK_IT_TypeDef CLK_IT)
2619                     ; 756 {
2620                     .text:	section	.text,new
2621  0000               _CLK_ClearITPendingBit:
2625                     ; 759     assert_param(IS_CLK_IT_OK(CLK_IT));
2627                     ; 761     if (CLK_IT == (uint8_t)CLK_IT_CSSD)
2629  0000 a10c          	cp	a,#12
2630  0002 2606          	jrne	L1331
2631                     ; 764         CLK->CSSR &= (uint8_t)(~CLK_CSSR_CSSD);
2633  0004 721750c8      	bres	20680,#3
2635  0008 2004          	jra	L3331
2636  000a               L1331:
2637                     ; 769         CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIF);
2639  000a 721750c5      	bres	20677,#3
2640  000e               L3331:
2641                     ; 772 }
2644  000e 81            	ret
2679                     	xref	_Delayms
2680                     	xdef	_CLKPrescTable
2681                     	xdef	_HSIDivFactor
2682                     	xdef	_CLK_ClearITPendingBit
2683                     	xdef	_CLK_GetITStatus
2684                     	xdef	_CLK_GetFlagStatus
2685                     	xdef	_CLK_GetSYSCLKSource
2686                     	xdef	_CLK_GetClockFreq
2687                     	xdef	_CLK_AdjustHSICalibrationValue
2688                     	xdef	_CLK_SYSCLKEmergencyClear
2689                     	xdef	_CLK_ClockSecuritySystemEnable
2690                     	xdef	_CLK_SWIMConfig
2691                     	xdef	_CLK_SYSCLKConfig
2692                     	xdef	_CLK_ITConfig
2693                     	xdef	_CLK_CCOConfig
2694                     	xdef	_CLK_HSIPrescalerConfig
2695                     	xdef	_CLK_ClockSwitchConfig
2696                     	xdef	_CLK_PeripheralClockConfig
2697                     	xdef	_CLK_SlowActiveHaltWakeUpCmd
2698                     	xdef	_CLK_FastHaltWakeUpCmd
2699                     	xdef	_CLK_ClockSwitchCmd
2700                     	xdef	_CLK_CCOCmd
2701                     	xdef	_CLK_LSICmd
2702                     	xdef	_CLK_HSICmd
2703                     	xdef	_CLK_HSECmd
2704                     	xdef	_CLK_DeInit
2705                     	xref.b	c_lreg
2724                     	xref	c_ludv
2725                     	xref	c_rtol
2726                     	xref	c_ltor
2727                     	end
