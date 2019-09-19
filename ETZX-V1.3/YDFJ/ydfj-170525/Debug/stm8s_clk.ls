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
  47                     ; 66 void CLK_DeInit(void)
  47                     ; 67 {
  49                     	switch	.text
  50  0000               _CLK_DeInit:
  54                     ; 69     CLK->ICKR = CLK_ICKR_RESET_VALUE;
  56  0000 350150c0      	mov	20672,#1
  57                     ; 70     CLK->ECKR = CLK_ECKR_RESET_VALUE;
  59  0004 725f50c1      	clr	20673
  60                     ; 71     CLK->SWR  = CLK_SWR_RESET_VALUE;
  62  0008 35e150c4      	mov	20676,#225
  63                     ; 72     CLK->SWCR = CLK_SWCR_RESET_VALUE;
  65  000c 725f50c5      	clr	20677
  66                     ; 73     CLK->CKDIVR = CLK_CKDIVR_RESET_VALUE;
  68  0010 351850c6      	mov	20678,#24
  69                     ; 74     CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
  71  0014 35ff50c7      	mov	20679,#255
  72                     ; 75     CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
  74  0018 35ff50ca      	mov	20682,#255
  75                     ; 76     CLK->CSSR = CLK_CSSR_RESET_VALUE;
  77  001c 725f50c8      	clr	20680
  78                     ; 77     CLK->CCOR = CLK_CCOR_RESET_VALUE;
  80  0020 725f50c9      	clr	20681
  82  0024               L52:
  83                     ; 78     while ((CLK->CCOR & CLK_CCOR_CCOEN)!= 0)
  85  0024 c650c9        	ld	a,20681
  86  0027 a501          	bcp	a,#1
  87  0029 26f9          	jrne	L52
  88                     ; 80     CLK->CCOR = CLK_CCOR_RESET_VALUE;
  90  002b 725f50c9      	clr	20681
  91                     ; 81     CLK->HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
  93  002f 725f50cc      	clr	20684
  94                     ; 82     CLK->SWIMCCR = CLK_SWIMCCR_RESET_VALUE;
  96  0033 725f50cd      	clr	20685
  97                     ; 84 }
 100  0037 81            	ret
 156                     ; 95 void CLK_FastHaltWakeUpCmd(FunctionalState NewState)
 156                     ; 96 {
 157                     	switch	.text
 158  0038               _CLK_FastHaltWakeUpCmd:
 162                     ; 99     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 164                     ; 101     if (NewState != DISABLE)
 166  0038 4d            	tnz	a
 167  0039 2706          	jreq	L75
 168                     ; 104         CLK->ICKR |= CLK_ICKR_FHWU;
 170  003b 721450c0      	bset	20672,#2
 172  003f 2004          	jra	L16
 173  0041               L75:
 174                     ; 109         CLK->ICKR &= (uint8_t)(~CLK_ICKR_FHWU);
 176  0041 721550c0      	bres	20672,#2
 177  0045               L16:
 178                     ; 112 }
 181  0045 81            	ret
 216                     ; 119 void CLK_HSECmd(FunctionalState NewState)
 216                     ; 120 {
 217                     	switch	.text
 218  0046               _CLK_HSECmd:
 222                     ; 123     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 224                     ; 125     if (NewState != DISABLE)
 226  0046 4d            	tnz	a
 227  0047 2706          	jreq	L101
 228                     ; 128         CLK->ECKR |= CLK_ECKR_HSEEN;
 230  0049 721050c1      	bset	20673,#0
 232  004d 2004          	jra	L301
 233  004f               L101:
 234                     ; 133         CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
 236  004f 721150c1      	bres	20673,#0
 237  0053               L301:
 238                     ; 136 }
 241  0053 81            	ret
 276                     ; 143 void CLK_HSICmd(FunctionalState NewState)
 276                     ; 144 {
 277                     	switch	.text
 278  0054               _CLK_HSICmd:
 282                     ; 147     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 284                     ; 149     if (NewState != DISABLE)
 286  0054 4d            	tnz	a
 287  0055 2706          	jreq	L321
 288                     ; 152         CLK->ICKR |= CLK_ICKR_HSIEN;
 290  0057 721050c0      	bset	20672,#0
 292  005b 2004          	jra	L521
 293  005d               L321:
 294                     ; 157         CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
 296  005d 721150c0      	bres	20672,#0
 297  0061               L521:
 298                     ; 160 }
 301  0061 81            	ret
 336                     ; 167 void CLK_LSICmd(FunctionalState NewState)
 336                     ; 168 {
 337                     	switch	.text
 338  0062               _CLK_LSICmd:
 342                     ; 171     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 344                     ; 173     if (NewState != DISABLE)
 346  0062 4d            	tnz	a
 347  0063 2706          	jreq	L541
 348                     ; 176         CLK->ICKR |= CLK_ICKR_LSIEN;
 350  0065 721650c0      	bset	20672,#3
 352  0069 2004          	jra	L741
 353  006b               L541:
 354                     ; 181         CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
 356  006b 721750c0      	bres	20672,#3
 357  006f               L741:
 358                     ; 184 }
 361  006f 81            	ret
 396                     ; 192 void CLK_CCOCmd(FunctionalState NewState)
 396                     ; 193 {
 397                     	switch	.text
 398  0070               _CLK_CCOCmd:
 402                     ; 196     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 404                     ; 198     if (NewState != DISABLE)
 406  0070 4d            	tnz	a
 407  0071 2706          	jreq	L761
 408                     ; 201         CLK->CCOR |= CLK_CCOR_CCOEN;
 410  0073 721050c9      	bset	20681,#0
 412  0077 2004          	jra	L171
 413  0079               L761:
 414                     ; 206         CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOEN);
 416  0079 721150c9      	bres	20681,#0
 417  007d               L171:
 418                     ; 209 }
 421  007d 81            	ret
 456                     ; 218 void CLK_ClockSwitchCmd(FunctionalState NewState)
 456                     ; 219 {
 457                     	switch	.text
 458  007e               _CLK_ClockSwitchCmd:
 462                     ; 222     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 464                     ; 224     if (NewState != DISABLE )
 466  007e 4d            	tnz	a
 467  007f 2706          	jreq	L112
 468                     ; 227         CLK->SWCR |= CLK_SWCR_SWEN;
 470  0081 721250c5      	bset	20677,#1
 472  0085 2004          	jra	L312
 473  0087               L112:
 474                     ; 232         CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWEN);
 476  0087 721350c5      	bres	20677,#1
 477  008b               L312:
 478                     ; 235 }
 481  008b 81            	ret
 517                     ; 245 void CLK_SlowActiveHaltWakeUpCmd(FunctionalState NewState)
 517                     ; 246 {
 518                     	switch	.text
 519  008c               _CLK_SlowActiveHaltWakeUpCmd:
 523                     ; 249     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 525                     ; 251     if (NewState != DISABLE)
 527  008c 4d            	tnz	a
 528  008d 2706          	jreq	L332
 529                     ; 254         CLK->ICKR |= CLK_ICKR_SWUAH;
 531  008f 721a50c0      	bset	20672,#5
 533  0093 2004          	jra	L532
 534  0095               L332:
 535                     ; 259         CLK->ICKR &= (uint8_t)(~CLK_ICKR_SWUAH);
 537  0095 721b50c0      	bres	20672,#5
 538  0099               L532:
 539                     ; 262 }
 542  0099 81            	ret
 701                     ; 272 void CLK_PeripheralClockConfig(CLK_Peripheral_TypeDef CLK_Peripheral, FunctionalState NewState)
 701                     ; 273 {
 702                     	switch	.text
 703  009a               _CLK_PeripheralClockConfig:
 705  009a 89            	pushw	x
 706       00000000      OFST:	set	0
 709                     ; 276     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 711                     ; 277     assert_param(IS_CLK_PERIPHERAL_OK(CLK_Peripheral));
 713                     ; 279     if (((uint8_t)CLK_Peripheral & (uint8_t)0x10) == 0x00)
 715  009b 9e            	ld	a,xh
 716  009c a510          	bcp	a,#16
 717  009e 2633          	jrne	L123
 718                     ; 281         if (NewState != DISABLE)
 720  00a0 0d02          	tnz	(OFST+2,sp)
 721  00a2 2717          	jreq	L323
 722                     ; 284             CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F));
 724  00a4 7b01          	ld	a,(OFST+1,sp)
 725  00a6 a40f          	and	a,#15
 726  00a8 5f            	clrw	x
 727  00a9 97            	ld	xl,a
 728  00aa a601          	ld	a,#1
 729  00ac 5d            	tnzw	x
 730  00ad 2704          	jreq	L62
 731  00af               L03:
 732  00af 48            	sll	a
 733  00b0 5a            	decw	x
 734  00b1 26fc          	jrne	L03
 735  00b3               L62:
 736  00b3 ca50c7        	or	a,20679
 737  00b6 c750c7        	ld	20679,a
 739  00b9 2049          	jra	L723
 740  00bb               L323:
 741                     ; 289             CLK->PCKENR1 &= (uint8_t)(~(uint8_t)(((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F))));
 743  00bb 7b01          	ld	a,(OFST+1,sp)
 744  00bd a40f          	and	a,#15
 745  00bf 5f            	clrw	x
 746  00c0 97            	ld	xl,a
 747  00c1 a601          	ld	a,#1
 748  00c3 5d            	tnzw	x
 749  00c4 2704          	jreq	L23
 750  00c6               L43:
 751  00c6 48            	sll	a
 752  00c7 5a            	decw	x
 753  00c8 26fc          	jrne	L43
 754  00ca               L23:
 755  00ca 43            	cpl	a
 756  00cb c450c7        	and	a,20679
 757  00ce c750c7        	ld	20679,a
 758  00d1 2031          	jra	L723
 759  00d3               L123:
 760                     ; 294         if (NewState != DISABLE)
 762  00d3 0d02          	tnz	(OFST+2,sp)
 763  00d5 2717          	jreq	L133
 764                     ; 297             CLK->PCKENR2 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F));
 766  00d7 7b01          	ld	a,(OFST+1,sp)
 767  00d9 a40f          	and	a,#15
 768  00db 5f            	clrw	x
 769  00dc 97            	ld	xl,a
 770  00dd a601          	ld	a,#1
 771  00df 5d            	tnzw	x
 772  00e0 2704          	jreq	L63
 773  00e2               L04:
 774  00e2 48            	sll	a
 775  00e3 5a            	decw	x
 776  00e4 26fc          	jrne	L04
 777  00e6               L63:
 778  00e6 ca50ca        	or	a,20682
 779  00e9 c750ca        	ld	20682,a
 781  00ec 2016          	jra	L723
 782  00ee               L133:
 783                     ; 302             CLK->PCKENR2 &= (uint8_t)(~(uint8_t)(((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F))));
 785  00ee 7b01          	ld	a,(OFST+1,sp)
 786  00f0 a40f          	and	a,#15
 787  00f2 5f            	clrw	x
 788  00f3 97            	ld	xl,a
 789  00f4 a601          	ld	a,#1
 790  00f6 5d            	tnzw	x
 791  00f7 2704          	jreq	L24
 792  00f9               L44:
 793  00f9 48            	sll	a
 794  00fa 5a            	decw	x
 795  00fb 26fc          	jrne	L44
 796  00fd               L24:
 797  00fd 43            	cpl	a
 798  00fe c450ca        	and	a,20682
 799  0101 c750ca        	ld	20682,a
 800  0104               L723:
 801                     ; 306 }
 804  0104 85            	popw	x
 805  0105 81            	ret
 993                     ; 319 ErrorStatus CLK_ClockSwitchConfig(CLK_SwitchMode_TypeDef CLK_SwitchMode, CLK_Source_TypeDef CLK_NewClock, FunctionalState ITState, CLK_CurrentClockState_TypeDef CLK_CurrentClockState)
 993                     ; 320 {
 994                     	switch	.text
 995  0106               _CLK_ClockSwitchConfig:
 997  0106 89            	pushw	x
 998  0107 5204          	subw	sp,#4
 999       00000004      OFST:	set	4
1002                     ; 323     uint16_t DownCounter = CLK_TIMEOUT;
1004  0109 ae0491        	ldw	x,#1169
1005  010c 1f03          	ldw	(OFST-1,sp),x
1006                     ; 324     ErrorStatus Swif = ERROR;
1008  010e 7b02          	ld	a,(OFST-2,sp)
1009  0110 97            	ld	xl,a
1010                     ; 327     assert_param(IS_CLK_SOURCE_OK(CLK_NewClock));
1012                     ; 328     assert_param(IS_CLK_SWITCHMODE_OK(CLK_SwitchMode));
1014                     ; 329     assert_param(IS_FUNCTIONALSTATE_OK(ITState));
1016                     ; 330     assert_param(IS_CLK_CURRENTCLOCKSTATE_OK(CLK_CurrentClockState));
1018                     ; 333     clock_master = (CLK_Source_TypeDef)CLK->CMSR;
1020  0111 c650c3        	ld	a,20675
1021  0114 6b01          	ld	(OFST-3,sp),a
1022                     ; 336     if (CLK_SwitchMode == CLK_SWITCHMODE_AUTO)
1024  0116 7b05          	ld	a,(OFST+1,sp)
1025  0118 a101          	cp	a,#1
1026  011a 2639          	jrne	L544
1027                     ; 340         CLK->SWCR |= CLK_SWCR_SWEN;
1029  011c 721250c5      	bset	20677,#1
1030                     ; 343         if (ITState != DISABLE)
1032  0120 0d09          	tnz	(OFST+5,sp)
1033  0122 2706          	jreq	L744
1034                     ; 345             CLK->SWCR |= CLK_SWCR_SWIEN;
1036  0124 721450c5      	bset	20677,#2
1038  0128 2004          	jra	L154
1039  012a               L744:
1040                     ; 349             CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
1042  012a 721550c5      	bres	20677,#2
1043  012e               L154:
1044                     ; 353         CLK->SWR = (uint8_t)CLK_NewClock;
1046  012e 7b06          	ld	a,(OFST+2,sp)
1047  0130 c750c4        	ld	20676,a
1049  0133 2007          	jra	L754
1050  0135               L354:
1051                     ; 357             DownCounter--;
1053  0135 1e03          	ldw	x,(OFST-1,sp)
1054  0137 1d0001        	subw	x,#1
1055  013a 1f03          	ldw	(OFST-1,sp),x
1056  013c               L754:
1057                     ; 355         while ((((CLK->SWCR & CLK_SWCR_SWBSY) != 0 )&& (DownCounter != 0)))
1059  013c c650c5        	ld	a,20677
1060  013f a501          	bcp	a,#1
1061  0141 2704          	jreq	L364
1063  0143 1e03          	ldw	x,(OFST-1,sp)
1064  0145 26ee          	jrne	L354
1065  0147               L364:
1066                     ; 360         if (DownCounter != 0)
1068  0147 1e03          	ldw	x,(OFST-1,sp)
1069  0149 2706          	jreq	L564
1070                     ; 362             Swif = SUCCESS;
1072  014b a601          	ld	a,#1
1073  014d 6b02          	ld	(OFST-2,sp),a
1075  014f 201b          	jra	L174
1076  0151               L564:
1077                     ; 366             Swif = ERROR;
1079  0151 0f02          	clr	(OFST-2,sp)
1080  0153 2017          	jra	L174
1081  0155               L544:
1082                     ; 374         if (ITState != DISABLE)
1084  0155 0d09          	tnz	(OFST+5,sp)
1085  0157 2706          	jreq	L374
1086                     ; 376             CLK->SWCR |= CLK_SWCR_SWIEN;
1088  0159 721450c5      	bset	20677,#2
1090  015d 2004          	jra	L574
1091  015f               L374:
1092                     ; 380             CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
1094  015f 721550c5      	bres	20677,#2
1095  0163               L574:
1096                     ; 384         CLK->SWR = (uint8_t)CLK_NewClock;
1098  0163 7b06          	ld	a,(OFST+2,sp)
1099  0165 c750c4        	ld	20676,a
1100                     ; 388         Swif = SUCCESS;
1102  0168 a601          	ld	a,#1
1103  016a 6b02          	ld	(OFST-2,sp),a
1104  016c               L174:
1105                     ; 393     if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_HSI))
1107  016c 0d0a          	tnz	(OFST+6,sp)
1108  016e 260c          	jrne	L774
1110  0170 7b01          	ld	a,(OFST-3,sp)
1111  0172 a1e1          	cp	a,#225
1112  0174 2606          	jrne	L774
1113                     ; 395         CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
1115  0176 721150c0      	bres	20672,#0
1117  017a 201e          	jra	L105
1118  017c               L774:
1119                     ; 397     else if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_LSI))
1121  017c 0d0a          	tnz	(OFST+6,sp)
1122  017e 260c          	jrne	L305
1124  0180 7b01          	ld	a,(OFST-3,sp)
1125  0182 a1d2          	cp	a,#210
1126  0184 2606          	jrne	L305
1127                     ; 399         CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
1129  0186 721750c0      	bres	20672,#3
1131  018a 200e          	jra	L105
1132  018c               L305:
1133                     ; 401     else if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_HSE))
1135  018c 0d0a          	tnz	(OFST+6,sp)
1136  018e 260a          	jrne	L105
1138  0190 7b01          	ld	a,(OFST-3,sp)
1139  0192 a1b4          	cp	a,#180
1140  0194 2604          	jrne	L105
1141                     ; 403         CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
1143  0196 721150c1      	bres	20673,#0
1144  019a               L105:
1145                     ; 406     return(Swif);
1147  019a 7b02          	ld	a,(OFST-2,sp)
1150  019c 5b06          	addw	sp,#6
1151  019e 81            	ret
1289                     ; 416 void CLK_HSIPrescalerConfig(CLK_Prescaler_TypeDef HSIPrescaler)
1289                     ; 417 {
1290                     	switch	.text
1291  019f               _CLK_HSIPrescalerConfig:
1293  019f 88            	push	a
1294       00000000      OFST:	set	0
1297                     ; 420     assert_param(IS_CLK_HSIPRESCALER_OK(HSIPrescaler));
1299                     ; 423     CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
1301  01a0 c650c6        	ld	a,20678
1302  01a3 a4e7          	and	a,#231
1303  01a5 c750c6        	ld	20678,a
1304                     ; 426     CLK->CKDIVR |= (uint8_t)HSIPrescaler;
1306  01a8 c650c6        	ld	a,20678
1307  01ab 1a01          	or	a,(OFST+1,sp)
1308  01ad c750c6        	ld	20678,a
1309                     ; 428 }
1312  01b0 84            	pop	a
1313  01b1 81            	ret
1448                     ; 439 void CLK_CCOConfig(CLK_Output_TypeDef CLK_CCO)
1448                     ; 440 {
1449                     	switch	.text
1450  01b2               _CLK_CCOConfig:
1452  01b2 88            	push	a
1453       00000000      OFST:	set	0
1456                     ; 443     assert_param(IS_CLK_OUTPUT_OK(CLK_CCO));
1458                     ; 446     CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOSEL);
1460  01b3 c650c9        	ld	a,20681
1461  01b6 a4e1          	and	a,#225
1462  01b8 c750c9        	ld	20681,a
1463                     ; 449     CLK->CCOR |= (uint8_t)CLK_CCO;
1465  01bb c650c9        	ld	a,20681
1466  01be 1a01          	or	a,(OFST+1,sp)
1467  01c0 c750c9        	ld	20681,a
1468                     ; 452     CLK->CCOR |= CLK_CCOR_CCOEN;
1470  01c3 721050c9      	bset	20681,#0
1471                     ; 454 }
1474  01c7 84            	pop	a
1475  01c8 81            	ret
1540                     ; 464 void CLK_ITConfig(CLK_IT_TypeDef CLK_IT, FunctionalState NewState)
1540                     ; 465 {
1541                     	switch	.text
1542  01c9               _CLK_ITConfig:
1544  01c9 89            	pushw	x
1545       00000000      OFST:	set	0
1548                     ; 468     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1550                     ; 469     assert_param(IS_CLK_IT_OK(CLK_IT));
1552                     ; 471     if (NewState != DISABLE)
1554  01ca 9f            	ld	a,xl
1555  01cb 4d            	tnz	a
1556  01cc 2719          	jreq	L507
1557                     ; 473         switch (CLK_IT)
1559  01ce 9e            	ld	a,xh
1561                     ; 481         default:
1561                     ; 482             break;
1562  01cf a00c          	sub	a,#12
1563  01d1 270a          	jreq	L146
1564  01d3 a010          	sub	a,#16
1565  01d5 2624          	jrne	L317
1566                     ; 475         case CLK_IT_SWIF: /* Enable the clock switch interrupt */
1566                     ; 476             CLK->SWCR |= CLK_SWCR_SWIEN;
1568  01d7 721450c5      	bset	20677,#2
1569                     ; 477             break;
1571  01db 201e          	jra	L317
1572  01dd               L146:
1573                     ; 478         case CLK_IT_CSSD: /* Enable the clock security system detection interrupt */
1573                     ; 479             CLK->CSSR |= CLK_CSSR_CSSDIE;
1575  01dd 721450c8      	bset	20680,#2
1576                     ; 480             break;
1578  01e1 2018          	jra	L317
1579  01e3               L346:
1580                     ; 481         default:
1580                     ; 482             break;
1582  01e3 2016          	jra	L317
1583  01e5               L117:
1585  01e5 2014          	jra	L317
1586  01e7               L507:
1587                     ; 487         switch (CLK_IT)
1589  01e7 7b01          	ld	a,(OFST+1,sp)
1591                     ; 495         default:
1591                     ; 496             break;
1592  01e9 a00c          	sub	a,#12
1593  01eb 270a          	jreq	L746
1594  01ed a010          	sub	a,#16
1595  01ef 260a          	jrne	L317
1596                     ; 489         case CLK_IT_SWIF: /* Disable the clock switch interrupt */
1596                     ; 490             CLK->SWCR  &= (uint8_t)(~CLK_SWCR_SWIEN);
1598  01f1 721550c5      	bres	20677,#2
1599                     ; 491             break;
1601  01f5 2004          	jra	L317
1602  01f7               L746:
1603                     ; 492         case CLK_IT_CSSD: /* Disable the clock security system detection interrupt */
1603                     ; 493             CLK->CSSR &= (uint8_t)(~CLK_CSSR_CSSDIE);
1605  01f7 721550c8      	bres	20680,#2
1606                     ; 494             break;
1607  01fb               L317:
1608                     ; 500 }
1611  01fb 85            	popw	x
1612  01fc 81            	ret
1613  01fd               L156:
1614                     ; 495         default:
1614                     ; 496             break;
1616  01fd 20fc          	jra	L317
1617  01ff               L717:
1618  01ff 20fa          	jra	L317
1653                     ; 507 void CLK_SYSCLKConfig(CLK_Prescaler_TypeDef CLK_Prescaler)
1653                     ; 508 {
1654                     	switch	.text
1655  0201               _CLK_SYSCLKConfig:
1657  0201 88            	push	a
1658       00000000      OFST:	set	0
1661                     ; 511     assert_param(IS_CLK_PRESCALER_OK(CLK_Prescaler));
1663                     ; 513     if (((uint8_t)CLK_Prescaler & (uint8_t)0x80) == 0x00) /* Bit7 = 0 means HSI divider */
1665  0202 a580          	bcp	a,#128
1666  0204 2614          	jrne	L737
1667                     ; 515         CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
1669  0206 c650c6        	ld	a,20678
1670  0209 a4e7          	and	a,#231
1671  020b c750c6        	ld	20678,a
1672                     ; 516         CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_Prescaler & (uint8_t)CLK_CKDIVR_HSIDIV);
1674  020e 7b01          	ld	a,(OFST+1,sp)
1675  0210 a418          	and	a,#24
1676  0212 ca50c6        	or	a,20678
1677  0215 c750c6        	ld	20678,a
1679  0218 2012          	jra	L147
1680  021a               L737:
1681                     ; 520         CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_CPUDIV);
1683  021a c650c6        	ld	a,20678
1684  021d a4f8          	and	a,#248
1685  021f c750c6        	ld	20678,a
1686                     ; 521         CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_Prescaler & (uint8_t)CLK_CKDIVR_CPUDIV);
1688  0222 7b01          	ld	a,(OFST+1,sp)
1689  0224 a407          	and	a,#7
1690  0226 ca50c6        	or	a,20678
1691  0229 c750c6        	ld	20678,a
1692  022c               L147:
1693                     ; 524 }
1696  022c 84            	pop	a
1697  022d 81            	ret
1753                     ; 531 void CLK_SWIMConfig(CLK_SWIMDivider_TypeDef CLK_SWIMDivider)
1753                     ; 532 {
1754                     	switch	.text
1755  022e               _CLK_SWIMConfig:
1759                     ; 535     assert_param(IS_CLK_SWIMDIVIDER_OK(CLK_SWIMDivider));
1761                     ; 537     if (CLK_SWIMDivider != CLK_SWIMDIVIDER_2)
1763  022e 4d            	tnz	a
1764  022f 2706          	jreq	L177
1765                     ; 540         CLK->SWIMCCR |= CLK_SWIMCCR_SWIMDIV;
1767  0231 721050cd      	bset	20685,#0
1769  0235 2004          	jra	L377
1770  0237               L177:
1771                     ; 545         CLK->SWIMCCR &= (uint8_t)(~CLK_SWIMCCR_SWIMDIV);
1773  0237 721150cd      	bres	20685,#0
1774  023b               L377:
1775                     ; 548 }
1778  023b 81            	ret
1802                     ; 557 void CLK_ClockSecuritySystemEnable(void)
1802                     ; 558 {
1803                     	switch	.text
1804  023c               _CLK_ClockSecuritySystemEnable:
1808                     ; 560     CLK->CSSR |= CLK_CSSR_CSSEN;
1810  023c 721050c8      	bset	20680,#0
1811                     ; 561 }
1814  0240 81            	ret
1839                     ; 569 CLK_Source_TypeDef CLK_GetSYSCLKSource(void)
1839                     ; 570 {
1840                     	switch	.text
1841  0241               _CLK_GetSYSCLKSource:
1845                     ; 571     return((CLK_Source_TypeDef)CLK->CMSR);
1847  0241 c650c3        	ld	a,20675
1850  0244 81            	ret
1913                     ; 579 uint32_t CLK_GetClockFreq(void)
1913                     ; 580 {
1914                     	switch	.text
1915  0245               _CLK_GetClockFreq:
1917  0245 5209          	subw	sp,#9
1918       00000009      OFST:	set	9
1921                     ; 582     uint32_t clockfrequency = 0;
1923  0247 96            	ldw	x,sp
1924  0248 1c0005        	addw	x,#OFST-4
1925  024b cd0000        	call	c_ltor
1927                     ; 583     CLK_Source_TypeDef clocksource = CLK_SOURCE_HSI;
1929  024e 7b09          	ld	a,(OFST+0,sp)
1930  0250 97            	ld	xl,a
1931                     ; 584     uint8_t tmp = 0, presc = 0;
1933  0251 7b09          	ld	a,(OFST+0,sp)
1934  0253 97            	ld	xl,a
1937  0254 7b09          	ld	a,(OFST+0,sp)
1938  0256 97            	ld	xl,a
1939                     ; 587     clocksource = (CLK_Source_TypeDef)CLK->CMSR;
1941  0257 c650c3        	ld	a,20675
1942  025a 6b09          	ld	(OFST+0,sp),a
1943                     ; 589     if (clocksource == CLK_SOURCE_HSI)
1945  025c 7b09          	ld	a,(OFST+0,sp)
1946  025e a1e1          	cp	a,#225
1947  0260 2642          	jrne	L7401
1948                     ; 591         tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
1950  0262 c650c6        	ld	a,20678
1951  0265 a418          	and	a,#24
1952  0267 6b09          	ld	(OFST+0,sp),a
1953                     ; 592         tmp = (uint8_t)(tmp >> 3);
1955  0269 7b09          	ld	a,(OFST+0,sp)
1956  026b 44            	srl	a
1957  026c 44            	srl	a
1958  026d 44            	srl	a
1959  026e 6b09          	ld	(OFST+0,sp),a
1960                     ; 593         presc = HSIDivFactor[tmp];
1962  0270 7b09          	ld	a,(OFST+0,sp)
1963  0272 5f            	clrw	x
1964  0273 97            	ld	xl,a
1965  0274 d60000        	ld	a,(_HSIDivFactor,x)
1966  0277 6b09          	ld	(OFST+0,sp),a
1967                     ; 594         clockfrequency = HSI_VALUE / presc;
1969  0279 7b09          	ld	a,(OFST+0,sp)
1970  027b b703          	ld	c_lreg+3,a
1971  027d 3f02          	clr	c_lreg+2
1972  027f 3f01          	clr	c_lreg+1
1973  0281 3f00          	clr	c_lreg
1974  0283 96            	ldw	x,sp
1975  0284 1c0001        	addw	x,#OFST-8
1976  0287 cd0000        	call	c_rtol
1978  028a ae2400        	ldw	x,#9216
1979  028d bf02          	ldw	c_lreg+2,x
1980  028f ae00f4        	ldw	x,#244
1981  0292 bf00          	ldw	c_lreg,x
1982  0294 96            	ldw	x,sp
1983  0295 1c0001        	addw	x,#OFST-8
1984  0298 cd0000        	call	c_ludv
1986  029b 96            	ldw	x,sp
1987  029c 1c0005        	addw	x,#OFST-4
1988  029f cd0000        	call	c_rtol
1991  02a2 201c          	jra	L1501
1992  02a4               L7401:
1993                     ; 596     else if ( clocksource == CLK_SOURCE_LSI)
1995  02a4 7b09          	ld	a,(OFST+0,sp)
1996  02a6 a1d2          	cp	a,#210
1997  02a8 260c          	jrne	L3501
1998                     ; 598         clockfrequency = LSI_VALUE;
2000  02aa aef400        	ldw	x,#62464
2001  02ad 1f07          	ldw	(OFST-2,sp),x
2002  02af ae0001        	ldw	x,#1
2003  02b2 1f05          	ldw	(OFST-4,sp),x
2005  02b4 200a          	jra	L1501
2006  02b6               L3501:
2007                     ; 602         clockfrequency = HSE_VALUE;
2009  02b6 ae2400        	ldw	x,#9216
2010  02b9 1f07          	ldw	(OFST-2,sp),x
2011  02bb ae00f4        	ldw	x,#244
2012  02be 1f05          	ldw	(OFST-4,sp),x
2013  02c0               L1501:
2014                     ; 605     return((uint32_t)clockfrequency);
2016  02c0 96            	ldw	x,sp
2017  02c1 1c0005        	addw	x,#OFST-4
2018  02c4 cd0000        	call	c_ltor
2022  02c7 5b09          	addw	sp,#9
2023  02c9 81            	ret
2122                     ; 616 void CLK_AdjustHSICalibrationValue(CLK_HSITrimValue_TypeDef CLK_HSICalibrationValue)
2122                     ; 617 {
2123                     	switch	.text
2124  02ca               _CLK_AdjustHSICalibrationValue:
2126  02ca 88            	push	a
2127       00000000      OFST:	set	0
2130                     ; 620     assert_param(IS_CLK_HSITRIMVALUE_OK(CLK_HSICalibrationValue));
2132                     ; 623     CLK->HSITRIMR = (uint8_t)( (uint8_t)(CLK->HSITRIMR & (uint8_t)(~CLK_HSITRIMR_HSITRIM))|((uint8_t)CLK_HSICalibrationValue));
2134  02cb c650cc        	ld	a,20684
2135  02ce a4f8          	and	a,#248
2136  02d0 1a01          	or	a,(OFST+1,sp)
2137  02d2 c750cc        	ld	20684,a
2138                     ; 625 }
2141  02d5 84            	pop	a
2142  02d6 81            	ret
2166                     ; 636 void CLK_SYSCLKEmergencyClear(void)
2166                     ; 637 {
2167                     	switch	.text
2168  02d7               _CLK_SYSCLKEmergencyClear:
2172                     ; 638     CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWBSY);
2174  02d7 721150c5      	bres	20677,#0
2175                     ; 639 }
2178  02db 81            	ret
2331                     ; 648 FlagStatus CLK_GetFlagStatus(CLK_Flag_TypeDef CLK_FLAG)
2331                     ; 649 {
2332                     	switch	.text
2333  02dc               _CLK_GetFlagStatus:
2335  02dc 89            	pushw	x
2336  02dd 5203          	subw	sp,#3
2337       00000003      OFST:	set	3
2340                     ; 651     uint16_t statusreg = 0;
2342  02df 1e01          	ldw	x,(OFST-2,sp)
2343                     ; 652     uint8_t tmpreg = 0;
2345  02e1 7b03          	ld	a,(OFST+0,sp)
2346  02e3 97            	ld	xl,a
2347                     ; 653     FlagStatus bitstatus = RESET;
2349  02e4 7b03          	ld	a,(OFST+0,sp)
2350  02e6 97            	ld	xl,a
2351                     ; 656     assert_param(IS_CLK_FLAG_OK(CLK_FLAG));
2353                     ; 659     statusreg = (uint16_t)((uint16_t)CLK_FLAG & (uint16_t)0xFF00);
2355  02e7 7b04          	ld	a,(OFST+1,sp)
2356  02e9 97            	ld	xl,a
2357  02ea 7b05          	ld	a,(OFST+2,sp)
2358  02ec 9f            	ld	a,xl
2359  02ed a4ff          	and	a,#255
2360  02ef 97            	ld	xl,a
2361  02f0 4f            	clr	a
2362  02f1 02            	rlwa	x,a
2363  02f2 1f01          	ldw	(OFST-2,sp),x
2364  02f4 01            	rrwa	x,a
2365                     ; 662     if (statusreg == 0x0100) /* The flag to check is in ICKRregister */
2367  02f5 1e01          	ldw	x,(OFST-2,sp)
2368  02f7 a30100        	cpw	x,#256
2369  02fa 2607          	jrne	L1221
2370                     ; 664         tmpreg = CLK->ICKR;
2372  02fc c650c0        	ld	a,20672
2373  02ff 6b03          	ld	(OFST+0,sp),a
2375  0301 202f          	jra	L3221
2376  0303               L1221:
2377                     ; 666     else if (statusreg == 0x0200) /* The flag to check is in ECKRregister */
2379  0303 1e01          	ldw	x,(OFST-2,sp)
2380  0305 a30200        	cpw	x,#512
2381  0308 2607          	jrne	L5221
2382                     ; 668         tmpreg = CLK->ECKR;
2384  030a c650c1        	ld	a,20673
2385  030d 6b03          	ld	(OFST+0,sp),a
2387  030f 2021          	jra	L3221
2388  0311               L5221:
2389                     ; 670     else if (statusreg == 0x0300) /* The flag to check is in SWIC register */
2391  0311 1e01          	ldw	x,(OFST-2,sp)
2392  0313 a30300        	cpw	x,#768
2393  0316 2607          	jrne	L1321
2394                     ; 672         tmpreg = CLK->SWCR;
2396  0318 c650c5        	ld	a,20677
2397  031b 6b03          	ld	(OFST+0,sp),a
2399  031d 2013          	jra	L3221
2400  031f               L1321:
2401                     ; 674     else if (statusreg == 0x0400) /* The flag to check is in CSS register */
2403  031f 1e01          	ldw	x,(OFST-2,sp)
2404  0321 a30400        	cpw	x,#1024
2405  0324 2607          	jrne	L5321
2406                     ; 676         tmpreg = CLK->CSSR;
2408  0326 c650c8        	ld	a,20680
2409  0329 6b03          	ld	(OFST+0,sp),a
2411  032b 2005          	jra	L3221
2412  032d               L5321:
2413                     ; 680         tmpreg = CLK->CCOR;
2415  032d c650c9        	ld	a,20681
2416  0330 6b03          	ld	(OFST+0,sp),a
2417  0332               L3221:
2418                     ; 683     if ((tmpreg & (uint8_t)CLK_FLAG) != (uint8_t)RESET)
2420  0332 7b05          	ld	a,(OFST+2,sp)
2421  0334 1503          	bcp	a,(OFST+0,sp)
2422  0336 2706          	jreq	L1421
2423                     ; 685         bitstatus = SET;
2425  0338 a601          	ld	a,#1
2426  033a 6b03          	ld	(OFST+0,sp),a
2428  033c 2002          	jra	L3421
2429  033e               L1421:
2430                     ; 689         bitstatus = RESET;
2432  033e 0f03          	clr	(OFST+0,sp)
2433  0340               L3421:
2434                     ; 693     return((FlagStatus)bitstatus);
2436  0340 7b03          	ld	a,(OFST+0,sp)
2439  0342 5b05          	addw	sp,#5
2440  0344 81            	ret
2486                     ; 703 ITStatus CLK_GetITStatus(CLK_IT_TypeDef CLK_IT)
2486                     ; 704 {
2487                     	switch	.text
2488  0345               _CLK_GetITStatus:
2490  0345 88            	push	a
2491  0346 88            	push	a
2492       00000001      OFST:	set	1
2495                     ; 706     ITStatus bitstatus = RESET;
2497  0347 0f01          	clr	(OFST+0,sp)
2498                     ; 709     assert_param(IS_CLK_IT_OK(CLK_IT));
2500                     ; 711     if (CLK_IT == CLK_IT_SWIF)
2502  0349 a11c          	cp	a,#28
2503  034b 2611          	jrne	L7621
2504                     ; 714         if ((CLK->SWCR & (uint8_t)CLK_IT) == (uint8_t)0x0C)
2506  034d c450c5        	and	a,20677
2507  0350 a10c          	cp	a,#12
2508  0352 2606          	jrne	L1721
2509                     ; 716             bitstatus = SET;
2511  0354 a601          	ld	a,#1
2512  0356 6b01          	ld	(OFST+0,sp),a
2514  0358 2015          	jra	L5721
2515  035a               L1721:
2516                     ; 720             bitstatus = RESET;
2518  035a 0f01          	clr	(OFST+0,sp)
2519  035c 2011          	jra	L5721
2520  035e               L7621:
2521                     ; 726         if ((CLK->CSSR & (uint8_t)CLK_IT) == (uint8_t)0x0C)
2523  035e c650c8        	ld	a,20680
2524  0361 1402          	and	a,(OFST+1,sp)
2525  0363 a10c          	cp	a,#12
2526  0365 2606          	jrne	L7721
2527                     ; 728             bitstatus = SET;
2529  0367 a601          	ld	a,#1
2530  0369 6b01          	ld	(OFST+0,sp),a
2532  036b 2002          	jra	L5721
2533  036d               L7721:
2534                     ; 732             bitstatus = RESET;
2536  036d 0f01          	clr	(OFST+0,sp)
2537  036f               L5721:
2538                     ; 737     return bitstatus;
2540  036f 7b01          	ld	a,(OFST+0,sp)
2543  0371 85            	popw	x
2544  0372 81            	ret
2580                     ; 747 void CLK_ClearITPendingBit(CLK_IT_TypeDef CLK_IT)
2580                     ; 748 {
2581                     	switch	.text
2582  0373               _CLK_ClearITPendingBit:
2586                     ; 751     assert_param(IS_CLK_IT_OK(CLK_IT));
2588                     ; 753     if (CLK_IT == (uint8_t)CLK_IT_CSSD)
2590  0373 a10c          	cp	a,#12
2591  0375 2606          	jrne	L1231
2592                     ; 756         CLK->CSSR &= (uint8_t)(~CLK_CSSR_CSSD);
2594  0377 721750c8      	bres	20680,#3
2596  037b 2004          	jra	L3231
2597  037d               L1231:
2598                     ; 761         CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIF);
2600  037d 721750c5      	bres	20677,#3
2601  0381               L3231:
2602                     ; 764 }
2605  0381 81            	ret
2640                     	xdef	_CLKPrescTable
2641                     	xdef	_HSIDivFactor
2642                     	xdef	_CLK_ClearITPendingBit
2643                     	xdef	_CLK_GetITStatus
2644                     	xdef	_CLK_GetFlagStatus
2645                     	xdef	_CLK_GetSYSCLKSource
2646                     	xdef	_CLK_GetClockFreq
2647                     	xdef	_CLK_AdjustHSICalibrationValue
2648                     	xdef	_CLK_SYSCLKEmergencyClear
2649                     	xdef	_CLK_ClockSecuritySystemEnable
2650                     	xdef	_CLK_SWIMConfig
2651                     	xdef	_CLK_SYSCLKConfig
2652                     	xdef	_CLK_ITConfig
2653                     	xdef	_CLK_CCOConfig
2654                     	xdef	_CLK_HSIPrescalerConfig
2655                     	xdef	_CLK_ClockSwitchConfig
2656                     	xdef	_CLK_PeripheralClockConfig
2657                     	xdef	_CLK_SlowActiveHaltWakeUpCmd
2658                     	xdef	_CLK_FastHaltWakeUpCmd
2659                     	xdef	_CLK_ClockSwitchCmd
2660                     	xdef	_CLK_CCOCmd
2661                     	xdef	_CLK_LSICmd
2662                     	xdef	_CLK_HSICmd
2663                     	xdef	_CLK_HSECmd
2664                     	xdef	_CLK_DeInit
2665                     	xref.b	c_lreg
2684                     	xref	c_ludv
2685                     	xref	c_rtol
2686                     	xref	c_ltor
2687                     	end
