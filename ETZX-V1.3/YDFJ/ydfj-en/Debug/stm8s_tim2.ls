   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 46 void TIM2_DeInit(void)
  32                     ; 47 {
  34                     	switch	.text
  35  0000               _TIM2_DeInit:
  39                     ; 49     TIM2->CR1 = (uint8_t)TIM2_CR1_RESET_VALUE;
  41  0000 725f5300      	clr	21248
  42                     ; 50     TIM2->IER = (uint8_t)TIM2_IER_RESET_VALUE;
  44  0004 725f5303      	clr	21251
  45                     ; 51     TIM2->SR2 = (uint8_t)TIM2_SR2_RESET_VALUE;
  47  0008 725f5305      	clr	21253
  48                     ; 54     TIM2->CCER1 = (uint8_t)TIM2_CCER1_RESET_VALUE;
  50  000c 725f530a      	clr	21258
  51                     ; 55     TIM2->CCER2 = (uint8_t)TIM2_CCER2_RESET_VALUE;
  53  0010 725f530b      	clr	21259
  54                     ; 59     TIM2->CCER1 = (uint8_t)TIM2_CCER1_RESET_VALUE;
  56  0014 725f530a      	clr	21258
  57                     ; 60     TIM2->CCER2 = (uint8_t)TIM2_CCER2_RESET_VALUE;
  59  0018 725f530b      	clr	21259
  60                     ; 61     TIM2->CCMR1 = (uint8_t)TIM2_CCMR1_RESET_VALUE;
  62  001c 725f5307      	clr	21255
  63                     ; 62     TIM2->CCMR2 = (uint8_t)TIM2_CCMR2_RESET_VALUE;
  65  0020 725f5308      	clr	21256
  66                     ; 63     TIM2->CCMR3 = (uint8_t)TIM2_CCMR3_RESET_VALUE;
  68  0024 725f5309      	clr	21257
  69                     ; 64     TIM2->CNTRH = (uint8_t)TIM2_CNTRH_RESET_VALUE;
  71  0028 725f530c      	clr	21260
  72                     ; 65     TIM2->CNTRL = (uint8_t)TIM2_CNTRL_RESET_VALUE;
  74  002c 725f530d      	clr	21261
  75                     ; 66     TIM2->PSCR = (uint8_t)TIM2_PSCR_RESET_VALUE;
  77  0030 725f530e      	clr	21262
  78                     ; 67     TIM2->ARRH  = (uint8_t)TIM2_ARRH_RESET_VALUE;
  80  0034 35ff530f      	mov	21263,#255
  81                     ; 68     TIM2->ARRL  = (uint8_t)TIM2_ARRL_RESET_VALUE;
  83  0038 35ff5310      	mov	21264,#255
  84                     ; 69     TIM2->CCR1H = (uint8_t)TIM2_CCR1H_RESET_VALUE;
  86  003c 725f5311      	clr	21265
  87                     ; 70     TIM2->CCR1L = (uint8_t)TIM2_CCR1L_RESET_VALUE;
  89  0040 725f5312      	clr	21266
  90                     ; 71     TIM2->CCR2H = (uint8_t)TIM2_CCR2H_RESET_VALUE;
  92  0044 725f5313      	clr	21267
  93                     ; 72     TIM2->CCR2L = (uint8_t)TIM2_CCR2L_RESET_VALUE;
  95  0048 725f5314      	clr	21268
  96                     ; 73     TIM2->CCR3H = (uint8_t)TIM2_CCR3H_RESET_VALUE;
  98  004c 725f5315      	clr	21269
  99                     ; 74     TIM2->CCR3L = (uint8_t)TIM2_CCR3L_RESET_VALUE;
 101  0050 725f5316      	clr	21270
 102                     ; 75     TIM2->SR1 = (uint8_t)TIM2_SR1_RESET_VALUE;
 104  0054 725f5304      	clr	21252
 105                     ; 76 }
 108  0058 81            	ret
 276                     ; 85 void TIM2_TimeBaseInit( TIM2_Prescaler_TypeDef TIM2_Prescaler,
 276                     ; 86                         uint16_t TIM2_Period)
 276                     ; 87 {
 277                     	switch	.text
 278  0059               _TIM2_TimeBaseInit:
 280  0059 88            	push	a
 281       00000000      OFST:	set	0
 284                     ; 89     TIM2->PSCR = (uint8_t)(TIM2_Prescaler);
 286  005a c7530e        	ld	21262,a
 287                     ; 91     TIM2->ARRH = (uint8_t)(TIM2_Period >> 8);
 289  005d 7b04          	ld	a,(OFST+4,sp)
 290  005f c7530f        	ld	21263,a
 291                     ; 92     TIM2->ARRL = (uint8_t)(TIM2_Period);
 293  0062 7b05          	ld	a,(OFST+5,sp)
 294  0064 c75310        	ld	21264,a
 295                     ; 93 }
 298  0067 84            	pop	a
 299  0068 81            	ret
 456                     ; 104 void TIM2_OC1Init(TIM2_OCMode_TypeDef TIM2_OCMode,
 456                     ; 105                   TIM2_OutputState_TypeDef TIM2_OutputState,
 456                     ; 106                   uint16_t TIM2_Pulse,
 456                     ; 107                   TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
 456                     ; 108 {
 457                     	switch	.text
 458  0069               _TIM2_OC1Init:
 460  0069 89            	pushw	x
 461  006a 88            	push	a
 462       00000001      OFST:	set	1
 465                     ; 110     assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
 467                     ; 111     assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
 469                     ; 112     assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
 471                     ; 115     TIM2->CCER1 &= (uint8_t)(~( TIM2_CCER1_CC1E | TIM2_CCER1_CC1P));
 473  006b c6530a        	ld	a,21258
 474  006e a4fc          	and	a,#252
 475  0070 c7530a        	ld	21258,a
 476                     ; 117     TIM2->CCER1 |= (uint8_t)((uint8_t)(TIM2_OutputState & TIM2_CCER1_CC1E ) | 
 476                     ; 118                              (uint8_t)(TIM2_OCPolarity & TIM2_CCER1_CC1P));
 478  0073 7b08          	ld	a,(OFST+7,sp)
 479  0075 a402          	and	a,#2
 480  0077 6b01          	ld	(OFST+0,sp),a
 481  0079 9f            	ld	a,xl
 482  007a a401          	and	a,#1
 483  007c 1a01          	or	a,(OFST+0,sp)
 484  007e ca530a        	or	a,21258
 485  0081 c7530a        	ld	21258,a
 486                     ; 121     TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM)) |
 486                     ; 122                             (uint8_t)TIM2_OCMode);
 488  0084 c65307        	ld	a,21255
 489  0087 a48f          	and	a,#143
 490  0089 1a02          	or	a,(OFST+1,sp)
 491  008b c75307        	ld	21255,a
 492                     ; 125     TIM2->CCR1H = (uint8_t)(TIM2_Pulse >> 8);
 494  008e 7b06          	ld	a,(OFST+5,sp)
 495  0090 c75311        	ld	21265,a
 496                     ; 126     TIM2->CCR1L = (uint8_t)(TIM2_Pulse);
 498  0093 7b07          	ld	a,(OFST+6,sp)
 499  0095 c75312        	ld	21266,a
 500                     ; 127 }
 503  0098 5b03          	addw	sp,#3
 504  009a 81            	ret
 568                     ; 138 void TIM2_OC2Init(TIM2_OCMode_TypeDef TIM2_OCMode,
 568                     ; 139                   TIM2_OutputState_TypeDef TIM2_OutputState,
 568                     ; 140                   uint16_t TIM2_Pulse,
 568                     ; 141                   TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
 568                     ; 142 {
 569                     	switch	.text
 570  009b               _TIM2_OC2Init:
 572  009b 89            	pushw	x
 573  009c 88            	push	a
 574       00000001      OFST:	set	1
 577                     ; 144     assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
 579                     ; 145     assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
 581                     ; 146     assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
 583                     ; 150     TIM2->CCER1 &= (uint8_t)(~( TIM2_CCER1_CC2E |  TIM2_CCER1_CC2P ));
 585  009d c6530a        	ld	a,21258
 586  00a0 a4cf          	and	a,#207
 587  00a2 c7530a        	ld	21258,a
 588                     ; 152     TIM2->CCER1 |= (uint8_t)((uint8_t)(TIM2_OutputState  & TIM2_CCER1_CC2E ) |
 588                     ; 153                         (uint8_t)(TIM2_OCPolarity & TIM2_CCER1_CC2P));
 590  00a5 7b08          	ld	a,(OFST+7,sp)
 591  00a7 a420          	and	a,#32
 592  00a9 6b01          	ld	(OFST+0,sp),a
 593  00ab 9f            	ld	a,xl
 594  00ac a410          	and	a,#16
 595  00ae 1a01          	or	a,(OFST+0,sp)
 596  00b0 ca530a        	or	a,21258
 597  00b3 c7530a        	ld	21258,a
 598                     ; 157     TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM)) | 
 598                     ; 158                             (uint8_t)TIM2_OCMode);
 600  00b6 c65308        	ld	a,21256
 601  00b9 a48f          	and	a,#143
 602  00bb 1a02          	or	a,(OFST+1,sp)
 603  00bd c75308        	ld	21256,a
 604                     ; 162     TIM2->CCR2H = (uint8_t)(TIM2_Pulse >> 8);
 606  00c0 7b06          	ld	a,(OFST+5,sp)
 607  00c2 c75313        	ld	21267,a
 608                     ; 163     TIM2->CCR2L = (uint8_t)(TIM2_Pulse);
 610  00c5 7b07          	ld	a,(OFST+6,sp)
 611  00c7 c75314        	ld	21268,a
 612                     ; 164 }
 615  00ca 5b03          	addw	sp,#3
 616  00cc 81            	ret
 680                     ; 175 void TIM2_OC3Init(TIM2_OCMode_TypeDef TIM2_OCMode,
 680                     ; 176                   TIM2_OutputState_TypeDef TIM2_OutputState,
 680                     ; 177                   uint16_t TIM2_Pulse,
 680                     ; 178                   TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
 680                     ; 179 {
 681                     	switch	.text
 682  00cd               _TIM2_OC3Init:
 684  00cd 89            	pushw	x
 685  00ce 88            	push	a
 686       00000001      OFST:	set	1
 689                     ; 181     assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
 691                     ; 182     assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
 693                     ; 183     assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
 695                     ; 185     TIM2->CCER2 &= (uint8_t)(~( TIM2_CCER2_CC3E  | TIM2_CCER2_CC3P));
 697  00cf c6530b        	ld	a,21259
 698  00d2 a4fc          	and	a,#252
 699  00d4 c7530b        	ld	21259,a
 700                     ; 187     TIM2->CCER2 |= (uint8_t)((uint8_t)(TIM2_OutputState & TIM2_CCER2_CC3E) |  
 700                     ; 188                              (uint8_t)(TIM2_OCPolarity & TIM2_CCER2_CC3P));
 702  00d7 7b08          	ld	a,(OFST+7,sp)
 703  00d9 a402          	and	a,#2
 704  00db 6b01          	ld	(OFST+0,sp),a
 705  00dd 9f            	ld	a,xl
 706  00de a401          	and	a,#1
 707  00e0 1a01          	or	a,(OFST+0,sp)
 708  00e2 ca530b        	or	a,21259
 709  00e5 c7530b        	ld	21259,a
 710                     ; 191     TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_OCM)) |
 710                     ; 192                             (uint8_t)TIM2_OCMode);
 712  00e8 c65309        	ld	a,21257
 713  00eb a48f          	and	a,#143
 714  00ed 1a02          	or	a,(OFST+1,sp)
 715  00ef c75309        	ld	21257,a
 716                     ; 195     TIM2->CCR3H = (uint8_t)(TIM2_Pulse >> 8);
 718  00f2 7b06          	ld	a,(OFST+5,sp)
 719  00f4 c75315        	ld	21269,a
 720                     ; 196     TIM2->CCR3L = (uint8_t)(TIM2_Pulse);
 722  00f7 7b07          	ld	a,(OFST+6,sp)
 723  00f9 c75316        	ld	21270,a
 724                     ; 198 }
 727  00fc 5b03          	addw	sp,#3
 728  00fe 81            	ret
 921                     ; 210 void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel,
 921                     ; 211                  TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
 921                     ; 212                  TIM2_ICSelection_TypeDef TIM2_ICSelection,
 921                     ; 213                  TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
 921                     ; 214                  uint8_t TIM2_ICFilter)
 921                     ; 215 {
 922                     	switch	.text
 923  00ff               _TIM2_ICInit:
 925  00ff 89            	pushw	x
 926       00000000      OFST:	set	0
 929                     ; 217     assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
 931                     ; 218     assert_param(IS_TIM2_IC_POLARITY_OK(TIM2_ICPolarity));
 933                     ; 219     assert_param(IS_TIM2_IC_SELECTION_OK(TIM2_ICSelection));
 935                     ; 220     assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_ICPrescaler));
 937                     ; 221     assert_param(IS_TIM2_IC_FILTER_OK(TIM2_ICFilter));
 939                     ; 223     if (TIM2_Channel == TIM2_CHANNEL_1)
 941  0100 9e            	ld	a,xh
 942  0101 4d            	tnz	a
 943  0102 2614          	jrne	L104
 944                     ; 226         TI1_Config((uint8_t)TIM2_ICPolarity,
 944                     ; 227                    (uint8_t)TIM2_ICSelection,
 944                     ; 228                    (uint8_t)TIM2_ICFilter);
 946  0104 7b07          	ld	a,(OFST+7,sp)
 947  0106 88            	push	a
 948  0107 7b06          	ld	a,(OFST+6,sp)
 949  0109 97            	ld	xl,a
 950  010a 7b03          	ld	a,(OFST+3,sp)
 951  010c 95            	ld	xh,a
 952  010d cd0481        	call	L3_TI1_Config
 954  0110 84            	pop	a
 955                     ; 231         TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
 957  0111 7b06          	ld	a,(OFST+6,sp)
 958  0113 cd0347        	call	_TIM2_SetIC1Prescaler
 961  0116 202c          	jra	L304
 962  0118               L104:
 963                     ; 233     else if (TIM2_Channel == TIM2_CHANNEL_2)
 965  0118 7b01          	ld	a,(OFST+1,sp)
 966  011a a101          	cp	a,#1
 967  011c 2614          	jrne	L504
 968                     ; 236         TI2_Config((uint8_t)TIM2_ICPolarity,
 968                     ; 237                    (uint8_t)TIM2_ICSelection,
 968                     ; 238                    (uint8_t)TIM2_ICFilter);
 970  011e 7b07          	ld	a,(OFST+7,sp)
 971  0120 88            	push	a
 972  0121 7b06          	ld	a,(OFST+6,sp)
 973  0123 97            	ld	xl,a
 974  0124 7b03          	ld	a,(OFST+3,sp)
 975  0126 95            	ld	xh,a
 976  0127 cd04b1        	call	L5_TI2_Config
 978  012a 84            	pop	a
 979                     ; 241         TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
 981  012b 7b06          	ld	a,(OFST+6,sp)
 982  012d cd0354        	call	_TIM2_SetIC2Prescaler
 985  0130 2012          	jra	L304
 986  0132               L504:
 987                     ; 246         TI3_Config((uint8_t)TIM2_ICPolarity,
 987                     ; 247                    (uint8_t)TIM2_ICSelection,
 987                     ; 248                    (uint8_t)TIM2_ICFilter);
 989  0132 7b07          	ld	a,(OFST+7,sp)
 990  0134 88            	push	a
 991  0135 7b06          	ld	a,(OFST+6,sp)
 992  0137 97            	ld	xl,a
 993  0138 7b03          	ld	a,(OFST+3,sp)
 994  013a 95            	ld	xh,a
 995  013b cd04e1        	call	L7_TI3_Config
 997  013e 84            	pop	a
 998                     ; 251         TIM2_SetIC3Prescaler(TIM2_ICPrescaler);
1000  013f 7b06          	ld	a,(OFST+6,sp)
1001  0141 cd0361        	call	_TIM2_SetIC3Prescaler
1003  0144               L304:
1004                     ; 253 }
1007  0144 85            	popw	x
1008  0145 81            	ret
1104                     ; 265 void TIM2_PWMIConfig(TIM2_Channel_TypeDef TIM2_Channel,
1104                     ; 266                      TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
1104                     ; 267                      TIM2_ICSelection_TypeDef TIM2_ICSelection,
1104                     ; 268                      TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
1104                     ; 269                      uint8_t TIM2_ICFilter)
1104                     ; 270 {
1105                     	switch	.text
1106  0146               _TIM2_PWMIConfig:
1108  0146 89            	pushw	x
1109  0147 89            	pushw	x
1110       00000002      OFST:	set	2
1113                     ; 271     uint8_t icpolarity = (uint8_t)TIM2_ICPOLARITY_RISING;
1115  0148 7b01          	ld	a,(OFST-1,sp)
1116  014a 97            	ld	xl,a
1117                     ; 272     uint8_t icselection = (uint8_t)TIM2_ICSELECTION_DIRECTTI;
1119  014b 7b02          	ld	a,(OFST+0,sp)
1120  014d 97            	ld	xl,a
1121                     ; 275     assert_param(IS_TIM2_PWMI_CHANNEL_OK(TIM2_Channel));
1123                     ; 276     assert_param(IS_TIM2_IC_POLARITY_OK(TIM2_ICPolarity));
1125                     ; 277     assert_param(IS_TIM2_IC_SELECTION_OK(TIM2_ICSelection));
1127                     ; 278     assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_ICPrescaler));
1129                     ; 281     if (TIM2_ICPolarity != TIM2_ICPOLARITY_FALLING)
1131  014e 7b04          	ld	a,(OFST+2,sp)
1132  0150 a144          	cp	a,#68
1133  0152 2706          	jreq	L754
1134                     ; 283         icpolarity = (uint8_t)TIM2_ICPOLARITY_FALLING;
1136  0154 a644          	ld	a,#68
1137  0156 6b01          	ld	(OFST-1,sp),a
1139  0158 2002          	jra	L164
1140  015a               L754:
1141                     ; 287         icpolarity = (uint8_t)TIM2_ICPOLARITY_RISING;
1143  015a 0f01          	clr	(OFST-1,sp)
1144  015c               L164:
1145                     ; 291     if (TIM2_ICSelection == TIM2_ICSELECTION_DIRECTTI)
1147  015c 7b07          	ld	a,(OFST+5,sp)
1148  015e a101          	cp	a,#1
1149  0160 2606          	jrne	L364
1150                     ; 293         icselection = (uint8_t)TIM2_ICSELECTION_INDIRECTTI;
1152  0162 a602          	ld	a,#2
1153  0164 6b02          	ld	(OFST+0,sp),a
1155  0166 2004          	jra	L564
1156  0168               L364:
1157                     ; 297         icselection = (uint8_t)TIM2_ICSELECTION_DIRECTTI;
1159  0168 a601          	ld	a,#1
1160  016a 6b02          	ld	(OFST+0,sp),a
1161  016c               L564:
1162                     ; 300     if (TIM2_Channel == TIM2_CHANNEL_1)
1164  016c 0d03          	tnz	(OFST+1,sp)
1165  016e 2626          	jrne	L764
1166                     ; 303         TI1_Config((uint8_t)TIM2_ICPolarity, (uint8_t)TIM2_ICSelection,
1166                     ; 304                    (uint8_t)TIM2_ICFilter);
1168  0170 7b09          	ld	a,(OFST+7,sp)
1169  0172 88            	push	a
1170  0173 7b08          	ld	a,(OFST+6,sp)
1171  0175 97            	ld	xl,a
1172  0176 7b05          	ld	a,(OFST+3,sp)
1173  0178 95            	ld	xh,a
1174  0179 cd0481        	call	L3_TI1_Config
1176  017c 84            	pop	a
1177                     ; 307         TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
1179  017d 7b08          	ld	a,(OFST+6,sp)
1180  017f cd0347        	call	_TIM2_SetIC1Prescaler
1182                     ; 310         TI2_Config(icpolarity, icselection, TIM2_ICFilter);
1184  0182 7b09          	ld	a,(OFST+7,sp)
1185  0184 88            	push	a
1186  0185 7b03          	ld	a,(OFST+1,sp)
1187  0187 97            	ld	xl,a
1188  0188 7b02          	ld	a,(OFST+0,sp)
1189  018a 95            	ld	xh,a
1190  018b cd04b1        	call	L5_TI2_Config
1192  018e 84            	pop	a
1193                     ; 313         TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
1195  018f 7b08          	ld	a,(OFST+6,sp)
1196  0191 cd0354        	call	_TIM2_SetIC2Prescaler
1199  0194 2024          	jra	L174
1200  0196               L764:
1201                     ; 318         TI2_Config((uint8_t)TIM2_ICPolarity, (uint8_t)TIM2_ICSelection,
1201                     ; 319                    (uint8_t)TIM2_ICFilter);
1203  0196 7b09          	ld	a,(OFST+7,sp)
1204  0198 88            	push	a
1205  0199 7b08          	ld	a,(OFST+6,sp)
1206  019b 97            	ld	xl,a
1207  019c 7b05          	ld	a,(OFST+3,sp)
1208  019e 95            	ld	xh,a
1209  019f cd04b1        	call	L5_TI2_Config
1211  01a2 84            	pop	a
1212                     ; 322         TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
1214  01a3 7b08          	ld	a,(OFST+6,sp)
1215  01a5 cd0354        	call	_TIM2_SetIC2Prescaler
1217                     ; 325         TI1_Config((uint8_t)icpolarity, icselection, (uint8_t)TIM2_ICFilter);
1219  01a8 7b09          	ld	a,(OFST+7,sp)
1220  01aa 88            	push	a
1221  01ab 7b03          	ld	a,(OFST+1,sp)
1222  01ad 97            	ld	xl,a
1223  01ae 7b02          	ld	a,(OFST+0,sp)
1224  01b0 95            	ld	xh,a
1225  01b1 cd0481        	call	L3_TI1_Config
1227  01b4 84            	pop	a
1228                     ; 328         TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
1230  01b5 7b08          	ld	a,(OFST+6,sp)
1231  01b7 cd0347        	call	_TIM2_SetIC1Prescaler
1233  01ba               L174:
1234                     ; 330 }
1237  01ba 5b04          	addw	sp,#4
1238  01bc 81            	ret
1293                     ; 339 void TIM2_Cmd(FunctionalState NewState)
1293                     ; 340 {
1294                     	switch	.text
1295  01bd               _TIM2_Cmd:
1299                     ; 342     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1301                     ; 345     if (NewState != DISABLE)
1303  01bd 4d            	tnz	a
1304  01be 2706          	jreq	L125
1305                     ; 347         TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;
1307  01c0 72105300      	bset	21248,#0
1309  01c4 2004          	jra	L325
1310  01c6               L125:
1311                     ; 351         TIM2->CR1 &= (uint8_t)(~TIM2_CR1_CEN);
1313  01c6 72115300      	bres	21248,#0
1314  01ca               L325:
1315                     ; 353 }
1318  01ca 81            	ret
1397                     ; 369 void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState)
1397                     ; 370 {
1398                     	switch	.text
1399  01cb               _TIM2_ITConfig:
1401  01cb 89            	pushw	x
1402       00000000      OFST:	set	0
1405                     ; 372     assert_param(IS_TIM2_IT_OK(TIM2_IT));
1407                     ; 373     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1409                     ; 375     if (NewState != DISABLE)
1411  01cc 9f            	ld	a,xl
1412  01cd 4d            	tnz	a
1413  01ce 2709          	jreq	L365
1414                     ; 378         TIM2->IER |= (uint8_t)TIM2_IT;
1416  01d0 9e            	ld	a,xh
1417  01d1 ca5303        	or	a,21251
1418  01d4 c75303        	ld	21251,a
1420  01d7 2009          	jra	L565
1421  01d9               L365:
1422                     ; 383         TIM2->IER &= (uint8_t)(~TIM2_IT);
1424  01d9 7b01          	ld	a,(OFST+1,sp)
1425  01db 43            	cpl	a
1426  01dc c45303        	and	a,21251
1427  01df c75303        	ld	21251,a
1428  01e2               L565:
1429                     ; 385 }
1432  01e2 85            	popw	x
1433  01e3 81            	ret
1469                     ; 394 void TIM2_UpdateDisableConfig(FunctionalState NewState)
1469                     ; 395 {
1470                     	switch	.text
1471  01e4               _TIM2_UpdateDisableConfig:
1475                     ; 397     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1477                     ; 400     if (NewState != DISABLE)
1479  01e4 4d            	tnz	a
1480  01e5 2706          	jreq	L506
1481                     ; 402         TIM2->CR1 |= (uint8_t)TIM2_CR1_UDIS;
1483  01e7 72125300      	bset	21248,#1
1485  01eb 2004          	jra	L706
1486  01ed               L506:
1487                     ; 406         TIM2->CR1 &= (uint8_t)(~TIM2_CR1_UDIS);
1489  01ed 72135300      	bres	21248,#1
1490  01f1               L706:
1491                     ; 408 }
1494  01f1 81            	ret
1552                     ; 418 void TIM2_UpdateRequestConfig(TIM2_UpdateSource_TypeDef TIM2_UpdateSource)
1552                     ; 419 {
1553                     	switch	.text
1554  01f2               _TIM2_UpdateRequestConfig:
1558                     ; 421     assert_param(IS_TIM2_UPDATE_SOURCE_OK(TIM2_UpdateSource));
1560                     ; 424     if (TIM2_UpdateSource != TIM2_UPDATESOURCE_GLOBAL)
1562  01f2 4d            	tnz	a
1563  01f3 2706          	jreq	L736
1564                     ; 426         TIM2->CR1 |= (uint8_t)TIM2_CR1_URS;
1566  01f5 72145300      	bset	21248,#2
1568  01f9 2004          	jra	L146
1569  01fb               L736:
1570                     ; 430         TIM2->CR1 &= (uint8_t)(~TIM2_CR1_URS);
1572  01fb 72155300      	bres	21248,#2
1573  01ff               L146:
1574                     ; 432 }
1577  01ff 81            	ret
1634                     ; 443 void TIM2_SelectOnePulseMode(TIM2_OPMode_TypeDef TIM2_OPMode)
1634                     ; 444 {
1635                     	switch	.text
1636  0200               _TIM2_SelectOnePulseMode:
1640                     ; 446     assert_param(IS_TIM2_OPM_MODE_OK(TIM2_OPMode));
1642                     ; 449     if (TIM2_OPMode != TIM2_OPMODE_REPETITIVE)
1644  0200 4d            	tnz	a
1645  0201 2706          	jreq	L176
1646                     ; 451         TIM2->CR1 |= (uint8_t)TIM2_CR1_OPM;
1648  0203 72165300      	bset	21248,#3
1650  0207 2004          	jra	L376
1651  0209               L176:
1652                     ; 455         TIM2->CR1 &= (uint8_t)(~TIM2_CR1_OPM);
1654  0209 72175300      	bres	21248,#3
1655  020d               L376:
1656                     ; 458 }
1659  020d 81            	ret
1727                     ; 489 void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler,
1727                     ; 490                           TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode)
1727                     ; 491 {
1728                     	switch	.text
1729  020e               _TIM2_PrescalerConfig:
1733                     ; 493     assert_param(IS_TIM2_PRESCALER_RELOAD_OK(TIM2_PSCReloadMode));
1735                     ; 494     assert_param(IS_TIM2_PRESCALER_OK(Prescaler));
1737                     ; 497     TIM2->PSCR = (uint8_t)Prescaler;
1739  020e 9e            	ld	a,xh
1740  020f c7530e        	ld	21262,a
1741                     ; 500     TIM2->EGR = (uint8_t)TIM2_PSCReloadMode;
1743  0212 9f            	ld	a,xl
1744  0213 c75306        	ld	21254,a
1745                     ; 501 }
1748  0216 81            	ret
1806                     ; 512 void TIM2_ForcedOC1Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
1806                     ; 513 {
1807                     	switch	.text
1808  0217               _TIM2_ForcedOC1Config:
1810  0217 88            	push	a
1811       00000000      OFST:	set	0
1814                     ; 515     assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));
1816                     ; 518     TIM2->CCMR1  =  (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM))  
1816                     ; 519                               | (uint8_t)TIM2_ForcedAction);
1818  0218 c65307        	ld	a,21255
1819  021b a48f          	and	a,#143
1820  021d 1a01          	or	a,(OFST+1,sp)
1821  021f c75307        	ld	21255,a
1822                     ; 520 }
1825  0222 84            	pop	a
1826  0223 81            	ret
1862                     ; 531 void TIM2_ForcedOC2Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
1862                     ; 532 {
1863                     	switch	.text
1864  0224               _TIM2_ForcedOC2Config:
1866  0224 88            	push	a
1867       00000000      OFST:	set	0
1870                     ; 534     assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));
1872                     ; 537     TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM))  
1872                     ; 538                             | (uint8_t)TIM2_ForcedAction);
1874  0225 c65308        	ld	a,21256
1875  0228 a48f          	and	a,#143
1876  022a 1a01          	or	a,(OFST+1,sp)
1877  022c c75308        	ld	21256,a
1878                     ; 539 }
1881  022f 84            	pop	a
1882  0230 81            	ret
1918                     ; 550 void TIM2_ForcedOC3Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
1918                     ; 551 {
1919                     	switch	.text
1920  0231               _TIM2_ForcedOC3Config:
1922  0231 88            	push	a
1923       00000000      OFST:	set	0
1926                     ; 553     assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));
1928                     ; 556     TIM2->CCMR3  =  (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_OCM))
1928                     ; 557                               | (uint8_t)TIM2_ForcedAction);
1930  0232 c65309        	ld	a,21257
1931  0235 a48f          	and	a,#143
1932  0237 1a01          	or	a,(OFST+1,sp)
1933  0239 c75309        	ld	21257,a
1934                     ; 558 }
1937  023c 84            	pop	a
1938  023d 81            	ret
1974                     ; 567 void TIM2_ARRPreloadConfig(FunctionalState NewState)
1974                     ; 568 {
1975                     	switch	.text
1976  023e               _TIM2_ARRPreloadConfig:
1980                     ; 570     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1982                     ; 573     if (NewState != DISABLE)
1984  023e 4d            	tnz	a
1985  023f 2706          	jreq	L7201
1986                     ; 575         TIM2->CR1 |= (uint8_t)TIM2_CR1_ARPE;
1988  0241 721e5300      	bset	21248,#7
1990  0245 2004          	jra	L1301
1991  0247               L7201:
1992                     ; 579         TIM2->CR1 &= (uint8_t)(~TIM2_CR1_ARPE);
1994  0247 721f5300      	bres	21248,#7
1995  024b               L1301:
1996                     ; 581 }
1999  024b 81            	ret
2035                     ; 590 void TIM2_OC1PreloadConfig(FunctionalState NewState)
2035                     ; 591 {
2036                     	switch	.text
2037  024c               _TIM2_OC1PreloadConfig:
2041                     ; 593     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2043                     ; 596     if (NewState != DISABLE)
2045  024c 4d            	tnz	a
2046  024d 2706          	jreq	L1501
2047                     ; 598         TIM2->CCMR1 |= (uint8_t)TIM2_CCMR_OCxPE;
2049  024f 72165307      	bset	21255,#3
2051  0253 2004          	jra	L3501
2052  0255               L1501:
2053                     ; 602         TIM2->CCMR1 &= (uint8_t)(~TIM2_CCMR_OCxPE);
2055  0255 72175307      	bres	21255,#3
2056  0259               L3501:
2057                     ; 604 }
2060  0259 81            	ret
2096                     ; 613 void TIM2_OC2PreloadConfig(FunctionalState NewState)
2096                     ; 614 {
2097                     	switch	.text
2098  025a               _TIM2_OC2PreloadConfig:
2102                     ; 616     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2104                     ; 619     if (NewState != DISABLE)
2106  025a 4d            	tnz	a
2107  025b 2706          	jreq	L3701
2108                     ; 621         TIM2->CCMR2 |= (uint8_t)TIM2_CCMR_OCxPE;
2110  025d 72165308      	bset	21256,#3
2112  0261 2004          	jra	L5701
2113  0263               L3701:
2114                     ; 625         TIM2->CCMR2 &= (uint8_t)(~TIM2_CCMR_OCxPE);
2116  0263 72175308      	bres	21256,#3
2117  0267               L5701:
2118                     ; 627 }
2121  0267 81            	ret
2157                     ; 636 void TIM2_OC3PreloadConfig(FunctionalState NewState)
2157                     ; 637 {
2158                     	switch	.text
2159  0268               _TIM2_OC3PreloadConfig:
2163                     ; 639     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2165                     ; 642     if (NewState != DISABLE)
2167  0268 4d            	tnz	a
2168  0269 2706          	jreq	L5111
2169                     ; 644         TIM2->CCMR3 |= (uint8_t)TIM2_CCMR_OCxPE;
2171  026b 72165309      	bset	21257,#3
2173  026f 2004          	jra	L7111
2174  0271               L5111:
2175                     ; 648         TIM2->CCMR3 &= (uint8_t)(~TIM2_CCMR_OCxPE);
2177  0271 72175309      	bres	21257,#3
2178  0275               L7111:
2179                     ; 650 }
2182  0275 81            	ret
2255                     ; 663 void TIM2_GenerateEvent(TIM2_EventSource_TypeDef TIM2_EventSource)
2255                     ; 664 {
2256                     	switch	.text
2257  0276               _TIM2_GenerateEvent:
2261                     ; 666     assert_param(IS_TIM2_EVENT_SOURCE_OK(TIM2_EventSource));
2263                     ; 669     TIM2->EGR = (uint8_t)TIM2_EventSource;
2265  0276 c75306        	ld	21254,a
2266                     ; 670 }
2269  0279 81            	ret
2305                     ; 681 void TIM2_OC1PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
2305                     ; 682 {
2306                     	switch	.text
2307  027a               _TIM2_OC1PolarityConfig:
2311                     ; 684     assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
2313                     ; 687     if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
2315  027a 4d            	tnz	a
2316  027b 2706          	jreq	L1711
2317                     ; 689         TIM2->CCER1 |= (uint8_t)TIM2_CCER1_CC1P;
2319  027d 7212530a      	bset	21258,#1
2321  0281 2004          	jra	L3711
2322  0283               L1711:
2323                     ; 693         TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1P);
2325  0283 7213530a      	bres	21258,#1
2326  0287               L3711:
2327                     ; 695 }
2330  0287 81            	ret
2366                     ; 706 void TIM2_OC2PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
2366                     ; 707 {
2367                     	switch	.text
2368  0288               _TIM2_OC2PolarityConfig:
2372                     ; 709     assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
2374                     ; 712     if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
2376  0288 4d            	tnz	a
2377  0289 2706          	jreq	L3121
2378                     ; 714         TIM2->CCER1 |= TIM2_CCER1_CC2P;
2380  028b 721a530a      	bset	21258,#5
2382  028f 2004          	jra	L5121
2383  0291               L3121:
2384                     ; 718         TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2P);
2386  0291 721b530a      	bres	21258,#5
2387  0295               L5121:
2388                     ; 720 }
2391  0295 81            	ret
2427                     ; 731 void TIM2_OC3PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
2427                     ; 732 {
2428                     	switch	.text
2429  0296               _TIM2_OC3PolarityConfig:
2433                     ; 734     assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
2435                     ; 737     if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
2437  0296 4d            	tnz	a
2438  0297 2706          	jreq	L5321
2439                     ; 739         TIM2->CCER2 |= (uint8_t)TIM2_CCER2_CC3P;
2441  0299 7212530b      	bset	21259,#1
2443  029d 2004          	jra	L7321
2444  029f               L5321:
2445                     ; 743         TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3P);
2447  029f 7213530b      	bres	21259,#1
2448  02a3               L7321:
2449                     ; 745 }
2452  02a3 81            	ret
2497                     ; 759 void TIM2_CCxCmd(TIM2_Channel_TypeDef TIM2_Channel, FunctionalState NewState)
2497                     ; 760 {
2498                     	switch	.text
2499  02a4               _TIM2_CCxCmd:
2501  02a4 89            	pushw	x
2502       00000000      OFST:	set	0
2505                     ; 762     assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
2507                     ; 763     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2509                     ; 765     if (TIM2_Channel == TIM2_CHANNEL_1)
2511  02a5 9e            	ld	a,xh
2512  02a6 4d            	tnz	a
2513  02a7 2610          	jrne	L3621
2514                     ; 768         if (NewState != DISABLE)
2516  02a9 9f            	ld	a,xl
2517  02aa 4d            	tnz	a
2518  02ab 2706          	jreq	L5621
2519                     ; 770             TIM2->CCER1 |= (uint8_t)TIM2_CCER1_CC1E;
2521  02ad 7210530a      	bset	21258,#0
2523  02b1 202a          	jra	L1721
2524  02b3               L5621:
2525                     ; 774             TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);
2527  02b3 7211530a      	bres	21258,#0
2528  02b7 2024          	jra	L1721
2529  02b9               L3621:
2530                     ; 778     else if (TIM2_Channel == TIM2_CHANNEL_2)
2532  02b9 7b01          	ld	a,(OFST+1,sp)
2533  02bb a101          	cp	a,#1
2534  02bd 2610          	jrne	L3721
2535                     ; 781         if (NewState != DISABLE)
2537  02bf 0d02          	tnz	(OFST+2,sp)
2538  02c1 2706          	jreq	L5721
2539                     ; 783             TIM2->CCER1 |= (uint8_t)TIM2_CCER1_CC2E;
2541  02c3 7218530a      	bset	21258,#4
2543  02c7 2014          	jra	L1721
2544  02c9               L5721:
2545                     ; 787             TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);
2547  02c9 7219530a      	bres	21258,#4
2548  02cd 200e          	jra	L1721
2549  02cf               L3721:
2550                     ; 793         if (NewState != DISABLE)
2552  02cf 0d02          	tnz	(OFST+2,sp)
2553  02d1 2706          	jreq	L3031
2554                     ; 795             TIM2->CCER2 |= (uint8_t)TIM2_CCER2_CC3E;
2556  02d3 7210530b      	bset	21259,#0
2558  02d7 2004          	jra	L1721
2559  02d9               L3031:
2560                     ; 799             TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3E);
2562  02d9 7211530b      	bres	21259,#0
2563  02dd               L1721:
2564                     ; 802 }
2567  02dd 85            	popw	x
2568  02de 81            	ret
2613                     ; 824 void TIM2_SelectOCxM(TIM2_Channel_TypeDef TIM2_Channel, TIM2_OCMode_TypeDef TIM2_OCMode)
2613                     ; 825 {
2614                     	switch	.text
2615  02df               _TIM2_SelectOCxM:
2617  02df 89            	pushw	x
2618       00000000      OFST:	set	0
2621                     ; 827     assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
2623                     ; 828     assert_param(IS_TIM2_OCM_OK(TIM2_OCMode));
2625                     ; 830     if (TIM2_Channel == TIM2_CHANNEL_1)
2627  02e0 9e            	ld	a,xh
2628  02e1 4d            	tnz	a
2629  02e2 2610          	jrne	L1331
2630                     ; 833         TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);
2632  02e4 7211530a      	bres	21258,#0
2633                     ; 836         TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM))
2633                     ; 837                                | (uint8_t)TIM2_OCMode);
2635  02e8 c65307        	ld	a,21255
2636  02eb a48f          	and	a,#143
2637  02ed 1a02          	or	a,(OFST+2,sp)
2638  02ef c75307        	ld	21255,a
2640  02f2 2024          	jra	L3331
2641  02f4               L1331:
2642                     ; 839     else if (TIM2_Channel == TIM2_CHANNEL_2)
2644  02f4 7b01          	ld	a,(OFST+1,sp)
2645  02f6 a101          	cp	a,#1
2646  02f8 2610          	jrne	L5331
2647                     ; 842         TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);
2649  02fa 7219530a      	bres	21258,#4
2650                     ; 845         TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM))
2650                     ; 846                                 | (uint8_t)TIM2_OCMode);
2652  02fe c65308        	ld	a,21256
2653  0301 a48f          	and	a,#143
2654  0303 1a02          	or	a,(OFST+2,sp)
2655  0305 c75308        	ld	21256,a
2657  0308 200e          	jra	L3331
2658  030a               L5331:
2659                     ; 851         TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3E);
2661  030a 7211530b      	bres	21259,#0
2662                     ; 854         TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_OCM))
2662                     ; 855                                 | (uint8_t)TIM2_OCMode);
2664  030e c65309        	ld	a,21257
2665  0311 a48f          	and	a,#143
2666  0313 1a02          	or	a,(OFST+2,sp)
2667  0315 c75309        	ld	21257,a
2668  0318               L3331:
2669                     ; 857 }
2672  0318 85            	popw	x
2673  0319 81            	ret
2707                     ; 866 void TIM2_SetCounter(uint16_t Counter)
2707                     ; 867 {
2708                     	switch	.text
2709  031a               _TIM2_SetCounter:
2713                     ; 869     TIM2->CNTRH = (uint8_t)(Counter >> 8);
2715  031a 9e            	ld	a,xh
2716  031b c7530c        	ld	21260,a
2717                     ; 870     TIM2->CNTRL = (uint8_t)(Counter);
2719  031e 9f            	ld	a,xl
2720  031f c7530d        	ld	21261,a
2721                     ; 872 }
2724  0322 81            	ret
2758                     ; 881 void TIM2_SetAutoreload(uint16_t Autoreload)
2758                     ; 882 {
2759                     	switch	.text
2760  0323               _TIM2_SetAutoreload:
2764                     ; 885     TIM2->ARRH = (uint8_t)(Autoreload >> 8);
2766  0323 9e            	ld	a,xh
2767  0324 c7530f        	ld	21263,a
2768                     ; 886     TIM2->ARRL = (uint8_t)(Autoreload);
2770  0327 9f            	ld	a,xl
2771  0328 c75310        	ld	21264,a
2772                     ; 888 }
2775  032b 81            	ret
2809                     ; 897 void TIM2_SetCompare1(uint16_t Compare1)
2809                     ; 898 {
2810                     	switch	.text
2811  032c               _TIM2_SetCompare1:
2815                     ; 900     TIM2->CCR1H = (uint8_t)(Compare1 >> 8);
2817  032c 9e            	ld	a,xh
2818  032d c75311        	ld	21265,a
2819                     ; 901     TIM2->CCR1L = (uint8_t)(Compare1);
2821  0330 9f            	ld	a,xl
2822  0331 c75312        	ld	21266,a
2823                     ; 903 }
2826  0334 81            	ret
2860                     ; 912 void TIM2_SetCompare2(uint16_t Compare2)
2860                     ; 913 {
2861                     	switch	.text
2862  0335               _TIM2_SetCompare2:
2866                     ; 915     TIM2->CCR2H = (uint8_t)(Compare2 >> 8);
2868  0335 9e            	ld	a,xh
2869  0336 c75313        	ld	21267,a
2870                     ; 916     TIM2->CCR2L = (uint8_t)(Compare2);
2872  0339 9f            	ld	a,xl
2873  033a c75314        	ld	21268,a
2874                     ; 918 }
2877  033d 81            	ret
2911                     ; 927 void TIM2_SetCompare3(uint16_t Compare3)
2911                     ; 928 {
2912                     	switch	.text
2913  033e               _TIM2_SetCompare3:
2917                     ; 930     TIM2->CCR3H = (uint8_t)(Compare3 >> 8);
2919  033e 9e            	ld	a,xh
2920  033f c75315        	ld	21269,a
2921                     ; 931     TIM2->CCR3L = (uint8_t)(Compare3);
2923  0342 9f            	ld	a,xl
2924  0343 c75316        	ld	21270,a
2925                     ; 933 }
2928  0346 81            	ret
2964                     ; 946 void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler)
2964                     ; 947 {
2965                     	switch	.text
2966  0347               _TIM2_SetIC1Prescaler:
2968  0347 88            	push	a
2969       00000000      OFST:	set	0
2972                     ; 949     assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC1Prescaler));
2974                     ; 952     TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_ICxPSC))
2974                     ; 953                             | (uint8_t)TIM2_IC1Prescaler);
2976  0348 c65307        	ld	a,21255
2977  034b a4f3          	and	a,#243
2978  034d 1a01          	or	a,(OFST+1,sp)
2979  034f c75307        	ld	21255,a
2980                     ; 954 }
2983  0352 84            	pop	a
2984  0353 81            	ret
3020                     ; 966 void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler)
3020                     ; 967 {
3021                     	switch	.text
3022  0354               _TIM2_SetIC2Prescaler:
3024  0354 88            	push	a
3025       00000000      OFST:	set	0
3028                     ; 969     assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC2Prescaler));
3030                     ; 972     TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_ICxPSC))
3030                     ; 973                             | (uint8_t)TIM2_IC2Prescaler);
3032  0355 c65308        	ld	a,21256
3033  0358 a4f3          	and	a,#243
3034  035a 1a01          	or	a,(OFST+1,sp)
3035  035c c75308        	ld	21256,a
3036                     ; 974 }
3039  035f 84            	pop	a
3040  0360 81            	ret
3076                     ; 986 void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler)
3076                     ; 987 {
3077                     	switch	.text
3078  0361               _TIM2_SetIC3Prescaler:
3080  0361 88            	push	a
3081       00000000      OFST:	set	0
3084                     ; 990     assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC3Prescaler));
3086                     ; 992     TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_ICxPSC))
3086                     ; 993                             | (uint8_t)TIM2_IC3Prescaler);
3088  0362 c65309        	ld	a,21257
3089  0365 a4f3          	and	a,#243
3090  0367 1a01          	or	a,(OFST+1,sp)
3091  0369 c75309        	ld	21257,a
3092                     ; 994 }
3095  036c 84            	pop	a
3096  036d 81            	ret
3148                     ; 1001 uint16_t TIM2_GetCapture1(void)
3148                     ; 1002 {
3149                     	switch	.text
3150  036e               _TIM2_GetCapture1:
3152  036e 5204          	subw	sp,#4
3153       00000004      OFST:	set	4
3156                     ; 1004     uint16_t tmpccr1 = 0;
3158  0370 1e03          	ldw	x,(OFST-1,sp)
3159                     ; 1005     uint8_t tmpccr1l=0, tmpccr1h=0;
3161  0372 7b01          	ld	a,(OFST-3,sp)
3162  0374 97            	ld	xl,a
3165  0375 7b02          	ld	a,(OFST-2,sp)
3166  0377 97            	ld	xl,a
3167                     ; 1007     tmpccr1h = TIM2->CCR1H;
3169  0378 c65311        	ld	a,21265
3170  037b 6b02          	ld	(OFST-2,sp),a
3171                     ; 1008     tmpccr1l = TIM2->CCR1L;
3173  037d c65312        	ld	a,21266
3174  0380 6b01          	ld	(OFST-3,sp),a
3175                     ; 1010     tmpccr1 = (uint16_t)(tmpccr1l);
3177  0382 7b01          	ld	a,(OFST-3,sp)
3178  0384 5f            	clrw	x
3179  0385 97            	ld	xl,a
3180  0386 1f03          	ldw	(OFST-1,sp),x
3181                     ; 1011     tmpccr1 |= (uint16_t)((uint16_t)tmpccr1h << 8);
3183  0388 7b02          	ld	a,(OFST-2,sp)
3184  038a 5f            	clrw	x
3185  038b 97            	ld	xl,a
3186  038c 4f            	clr	a
3187  038d 02            	rlwa	x,a
3188  038e 01            	rrwa	x,a
3189  038f 1a04          	or	a,(OFST+0,sp)
3190  0391 01            	rrwa	x,a
3191  0392 1a03          	or	a,(OFST-1,sp)
3192  0394 01            	rrwa	x,a
3193  0395 1f03          	ldw	(OFST-1,sp),x
3194                     ; 1013     return (uint16_t)tmpccr1;
3196  0397 1e03          	ldw	x,(OFST-1,sp)
3199  0399 5b04          	addw	sp,#4
3200  039b 81            	ret
3252                     ; 1021 uint16_t TIM2_GetCapture2(void)
3252                     ; 1022 {
3253                     	switch	.text
3254  039c               _TIM2_GetCapture2:
3256  039c 5204          	subw	sp,#4
3257       00000004      OFST:	set	4
3260                     ; 1024     uint16_t tmpccr2 = 0;
3262  039e 1e03          	ldw	x,(OFST-1,sp)
3263                     ; 1025     uint8_t tmpccr2l=0, tmpccr2h=0;
3265  03a0 7b01          	ld	a,(OFST-3,sp)
3266  03a2 97            	ld	xl,a
3269  03a3 7b02          	ld	a,(OFST-2,sp)
3270  03a5 97            	ld	xl,a
3271                     ; 1027     tmpccr2h = TIM2->CCR2H;
3273  03a6 c65313        	ld	a,21267
3274  03a9 6b02          	ld	(OFST-2,sp),a
3275                     ; 1028     tmpccr2l = TIM2->CCR2L;
3277  03ab c65314        	ld	a,21268
3278  03ae 6b01          	ld	(OFST-3,sp),a
3279                     ; 1030     tmpccr2 = (uint16_t)(tmpccr2l);
3281  03b0 7b01          	ld	a,(OFST-3,sp)
3282  03b2 5f            	clrw	x
3283  03b3 97            	ld	xl,a
3284  03b4 1f03          	ldw	(OFST-1,sp),x
3285                     ; 1031     tmpccr2 |= (uint16_t)((uint16_t)tmpccr2h << 8);
3287  03b6 7b02          	ld	a,(OFST-2,sp)
3288  03b8 5f            	clrw	x
3289  03b9 97            	ld	xl,a
3290  03ba 4f            	clr	a
3291  03bb 02            	rlwa	x,a
3292  03bc 01            	rrwa	x,a
3293  03bd 1a04          	or	a,(OFST+0,sp)
3294  03bf 01            	rrwa	x,a
3295  03c0 1a03          	or	a,(OFST-1,sp)
3296  03c2 01            	rrwa	x,a
3297  03c3 1f03          	ldw	(OFST-1,sp),x
3298                     ; 1033     return (uint16_t)tmpccr2;
3300  03c5 1e03          	ldw	x,(OFST-1,sp)
3303  03c7 5b04          	addw	sp,#4
3304  03c9 81            	ret
3356                     ; 1041 uint16_t TIM2_GetCapture3(void)
3356                     ; 1042 {
3357                     	switch	.text
3358  03ca               _TIM2_GetCapture3:
3360  03ca 5204          	subw	sp,#4
3361       00000004      OFST:	set	4
3364                     ; 1044     uint16_t tmpccr3 = 0;
3366  03cc 1e03          	ldw	x,(OFST-1,sp)
3367                     ; 1045     uint8_t tmpccr3l=0, tmpccr3h=0;
3369  03ce 7b01          	ld	a,(OFST-3,sp)
3370  03d0 97            	ld	xl,a
3373  03d1 7b02          	ld	a,(OFST-2,sp)
3374  03d3 97            	ld	xl,a
3375                     ; 1047     tmpccr3h = TIM2->CCR3H;
3377  03d4 c65315        	ld	a,21269
3378  03d7 6b02          	ld	(OFST-2,sp),a
3379                     ; 1048     tmpccr3l = TIM2->CCR3L;
3381  03d9 c65316        	ld	a,21270
3382  03dc 6b01          	ld	(OFST-3,sp),a
3383                     ; 1050     tmpccr3 = (uint16_t)(tmpccr3l);
3385  03de 7b01          	ld	a,(OFST-3,sp)
3386  03e0 5f            	clrw	x
3387  03e1 97            	ld	xl,a
3388  03e2 1f03          	ldw	(OFST-1,sp),x
3389                     ; 1051     tmpccr3 |= (uint16_t)((uint16_t)tmpccr3h << 8);
3391  03e4 7b02          	ld	a,(OFST-2,sp)
3392  03e6 5f            	clrw	x
3393  03e7 97            	ld	xl,a
3394  03e8 4f            	clr	a
3395  03e9 02            	rlwa	x,a
3396  03ea 01            	rrwa	x,a
3397  03eb 1a04          	or	a,(OFST+0,sp)
3398  03ed 01            	rrwa	x,a
3399  03ee 1a03          	or	a,(OFST-1,sp)
3400  03f0 01            	rrwa	x,a
3401  03f1 1f03          	ldw	(OFST-1,sp),x
3402                     ; 1053     return (uint16_t)tmpccr3;
3404  03f3 1e03          	ldw	x,(OFST-1,sp)
3407  03f5 5b04          	addw	sp,#4
3408  03f7 81            	ret
3442                     ; 1061 uint16_t TIM2_GetCounter(void)
3442                     ; 1062 {
3443                     	switch	.text
3444  03f8               _TIM2_GetCounter:
3446  03f8 89            	pushw	x
3447       00000002      OFST:	set	2
3450                     ; 1063      uint16_t tmpcntr = 0;
3452  03f9 5f            	clrw	x
3453  03fa 1f01          	ldw	(OFST-1,sp),x
3454                     ; 1065     tmpcntr =  ((uint16_t)TIM2->CNTRH << 8);
3456  03fc c6530c        	ld	a,21260
3457  03ff 5f            	clrw	x
3458  0400 97            	ld	xl,a
3459  0401 4f            	clr	a
3460  0402 02            	rlwa	x,a
3461  0403 1f01          	ldw	(OFST-1,sp),x
3462                     ; 1067     return (uint16_t)( tmpcntr| (uint16_t)(TIM2->CNTRL));
3464  0405 c6530d        	ld	a,21261
3465  0408 5f            	clrw	x
3466  0409 97            	ld	xl,a
3467  040a 01            	rrwa	x,a
3468  040b 1a02          	or	a,(OFST+0,sp)
3469  040d 01            	rrwa	x,a
3470  040e 1a01          	or	a,(OFST-1,sp)
3471  0410 01            	rrwa	x,a
3474  0411 5b02          	addw	sp,#2
3475  0413 81            	ret
3499                     ; 1076 TIM2_Prescaler_TypeDef TIM2_GetPrescaler(void)
3499                     ; 1077 {
3500                     	switch	.text
3501  0414               _TIM2_GetPrescaler:
3505                     ; 1079     return (TIM2_Prescaler_TypeDef)(TIM2->PSCR);
3507  0414 c6530e        	ld	a,21262
3510  0417 81            	ret
3649                     ; 1096 FlagStatus TIM2_GetFlagStatus(TIM2_FLAG_TypeDef TIM2_FLAG)
3649                     ; 1097 {
3650                     	switch	.text
3651  0418               _TIM2_GetFlagStatus:
3653  0418 89            	pushw	x
3654  0419 89            	pushw	x
3655       00000002      OFST:	set	2
3658                     ; 1098     FlagStatus bitstatus = RESET;
3660  041a 7b02          	ld	a,(OFST+0,sp)
3661  041c 97            	ld	xl,a
3662                     ; 1099     uint8_t tim2_flag_l = 0, tim2_flag_h = 0;
3664  041d 7b01          	ld	a,(OFST-1,sp)
3665  041f 97            	ld	xl,a
3668  0420 7b02          	ld	a,(OFST+0,sp)
3669  0422 97            	ld	xl,a
3670                     ; 1102     assert_param(IS_TIM2_GET_FLAG_OK(TIM2_FLAG));
3672                     ; 1104     tim2_flag_l = (uint8_t)(TIM2->SR1 & (uint8_t)TIM2_FLAG);
3674  0423 c65304        	ld	a,21252
3675  0426 1404          	and	a,(OFST+2,sp)
3676  0428 6b01          	ld	(OFST-1,sp),a
3677                     ; 1105     tim2_flag_h = (uint8_t)((uint16_t)TIM2_FLAG >> 8);
3679  042a 7b03          	ld	a,(OFST+1,sp)
3680  042c 6b02          	ld	(OFST+0,sp),a
3681                     ; 1107     if ((tim2_flag_l | (uint8_t)(TIM2->SR2 & tim2_flag_h)) != (uint8_t)RESET )
3683  042e c65305        	ld	a,21253
3684  0431 1402          	and	a,(OFST+0,sp)
3685  0433 1a01          	or	a,(OFST-1,sp)
3686  0435 2706          	jreq	L5371
3687                     ; 1109         bitstatus = SET;
3689  0437 a601          	ld	a,#1
3690  0439 6b02          	ld	(OFST+0,sp),a
3692  043b 2002          	jra	L7371
3693  043d               L5371:
3694                     ; 1113         bitstatus = RESET;
3696  043d 0f02          	clr	(OFST+0,sp)
3697  043f               L7371:
3698                     ; 1115     return (FlagStatus)bitstatus;
3700  043f 7b02          	ld	a,(OFST+0,sp)
3703  0441 5b04          	addw	sp,#4
3704  0443 81            	ret
3739                     ; 1132 void TIM2_ClearFlag(TIM2_FLAG_TypeDef TIM2_FLAG)
3739                     ; 1133 {
3740                     	switch	.text
3741  0444               _TIM2_ClearFlag:
3745                     ; 1135     assert_param(IS_TIM2_CLEAR_FLAG_OK(TIM2_FLAG));
3747                     ; 1138     TIM2->SR1 = (uint8_t)(~((uint8_t)(TIM2_FLAG)));
3749  0444 9f            	ld	a,xl
3750  0445 43            	cpl	a
3751  0446 c75304        	ld	21252,a
3752                     ; 1139     TIM2->SR2 = (uint8_t)(~((uint8_t)((uint8_t)TIM2_FLAG >> 8)));
3754  0449 35ff5305      	mov	21253,#255
3755                     ; 1140 }
3758  044d 81            	ret
3822                     ; 1154 ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT)
3822                     ; 1155 {
3823                     	switch	.text
3824  044e               _TIM2_GetITStatus:
3826  044e 88            	push	a
3827  044f 89            	pushw	x
3828       00000002      OFST:	set	2
3831                     ; 1156     ITStatus bitstatus = RESET;
3833  0450 7b02          	ld	a,(OFST+0,sp)
3834  0452 97            	ld	xl,a
3835                     ; 1157     uint8_t TIM2_itStatus = 0, TIM2_itEnable = 0;
3837  0453 7b01          	ld	a,(OFST-1,sp)
3838  0455 97            	ld	xl,a
3841  0456 7b02          	ld	a,(OFST+0,sp)
3842  0458 97            	ld	xl,a
3843                     ; 1160     assert_param(IS_TIM2_GET_IT_OK(TIM2_IT));
3845                     ; 1162     TIM2_itStatus = (uint8_t)(TIM2->SR1 & TIM2_IT);
3847  0459 c65304        	ld	a,21252
3848  045c 1403          	and	a,(OFST+1,sp)
3849  045e 6b01          	ld	(OFST-1,sp),a
3850                     ; 1164     TIM2_itEnable = (uint8_t)(TIM2->IER & TIM2_IT);
3852  0460 c65303        	ld	a,21251
3853  0463 1403          	and	a,(OFST+1,sp)
3854  0465 6b02          	ld	(OFST+0,sp),a
3855                     ; 1166     if ((TIM2_itStatus != (uint8_t)RESET ) && (TIM2_itEnable != (uint8_t)RESET ))
3857  0467 0d01          	tnz	(OFST-1,sp)
3858  0469 270a          	jreq	L1102
3860  046b 0d02          	tnz	(OFST+0,sp)
3861  046d 2706          	jreq	L1102
3862                     ; 1168         bitstatus = SET;
3864  046f a601          	ld	a,#1
3865  0471 6b02          	ld	(OFST+0,sp),a
3867  0473 2002          	jra	L3102
3868  0475               L1102:
3869                     ; 1172         bitstatus = RESET;
3871  0475 0f02          	clr	(OFST+0,sp)
3872  0477               L3102:
3873                     ; 1174     return (ITStatus)(bitstatus);
3875  0477 7b02          	ld	a,(OFST+0,sp)
3878  0479 5b03          	addw	sp,#3
3879  047b 81            	ret
3915                     ; 1188 void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT)
3915                     ; 1189 {
3916                     	switch	.text
3917  047c               _TIM2_ClearITPendingBit:
3921                     ; 1191     assert_param(IS_TIM2_IT_OK(TIM2_IT));
3923                     ; 1194     TIM2->SR1 = (uint8_t)(~TIM2_IT);
3925  047c 43            	cpl	a
3926  047d c75304        	ld	21252,a
3927                     ; 1195 }
3930  0480 81            	ret
3982                     ; 1214 static void TI1_Config(uint8_t TIM2_ICPolarity,
3982                     ; 1215                        uint8_t TIM2_ICSelection,
3982                     ; 1216                        uint8_t TIM2_ICFilter)
3982                     ; 1217 {
3983                     	switch	.text
3984  0481               L3_TI1_Config:
3986  0481 89            	pushw	x
3987  0482 88            	push	a
3988       00000001      OFST:	set	1
3991                     ; 1219     TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);
3993  0483 7211530a      	bres	21258,#0
3994                     ; 1222     TIM2->CCMR1  = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~(uint8_t)( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF )))
3994                     ; 1223                              | (uint8_t)(((TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));
3996  0487 7b06          	ld	a,(OFST+5,sp)
3997  0489 97            	ld	xl,a
3998  048a a610          	ld	a,#16
3999  048c 42            	mul	x,a
4000  048d 9f            	ld	a,xl
4001  048e 1a03          	or	a,(OFST+2,sp)
4002  0490 6b01          	ld	(OFST+0,sp),a
4003  0492 c65307        	ld	a,21255
4004  0495 a40c          	and	a,#12
4005  0497 1a01          	or	a,(OFST+0,sp)
4006  0499 c75307        	ld	21255,a
4007                     ; 1226     if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
4009  049c 0d02          	tnz	(OFST+1,sp)
4010  049e 2706          	jreq	L1602
4011                     ; 1228         TIM2->CCER1 |= TIM2_CCER1_CC1P;
4013  04a0 7212530a      	bset	21258,#1
4015  04a4 2004          	jra	L3602
4016  04a6               L1602:
4017                     ; 1232         TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1P);
4019  04a6 7213530a      	bres	21258,#1
4020  04aa               L3602:
4021                     ; 1235     TIM2->CCER1 |= TIM2_CCER1_CC1E;
4023  04aa 7210530a      	bset	21258,#0
4024                     ; 1236 }
4027  04ae 5b03          	addw	sp,#3
4028  04b0 81            	ret
4080                     ; 1255 static void TI2_Config(uint8_t TIM2_ICPolarity,
4080                     ; 1256                        uint8_t TIM2_ICSelection,
4080                     ; 1257                        uint8_t TIM2_ICFilter)
4080                     ; 1258 {
4081                     	switch	.text
4082  04b1               L5_TI2_Config:
4084  04b1 89            	pushw	x
4085  04b2 88            	push	a
4086       00000001      OFST:	set	1
4089                     ; 1260     TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);
4091  04b3 7219530a      	bres	21258,#4
4092                     ; 1263     TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~(uint8_t)( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF ))) 
4092                     ; 1264                             | (uint8_t)(( (TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));
4094  04b7 7b06          	ld	a,(OFST+5,sp)
4095  04b9 97            	ld	xl,a
4096  04ba a610          	ld	a,#16
4097  04bc 42            	mul	x,a
4098  04bd 9f            	ld	a,xl
4099  04be 1a03          	or	a,(OFST+2,sp)
4100  04c0 6b01          	ld	(OFST+0,sp),a
4101  04c2 c65308        	ld	a,21256
4102  04c5 a40c          	and	a,#12
4103  04c7 1a01          	or	a,(OFST+0,sp)
4104  04c9 c75308        	ld	21256,a
4105                     ; 1268     if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
4107  04cc 0d02          	tnz	(OFST+1,sp)
4108  04ce 2706          	jreq	L3112
4109                     ; 1270         TIM2->CCER1 |= TIM2_CCER1_CC2P;
4111  04d0 721a530a      	bset	21258,#5
4113  04d4 2004          	jra	L5112
4114  04d6               L3112:
4115                     ; 1274         TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2P);
4117  04d6 721b530a      	bres	21258,#5
4118  04da               L5112:
4119                     ; 1278     TIM2->CCER1 |= TIM2_CCER1_CC2E;
4121  04da 7218530a      	bset	21258,#4
4122                     ; 1280 }
4125  04de 5b03          	addw	sp,#3
4126  04e0 81            	ret
4178                     ; 1296 static void TI3_Config(uint8_t TIM2_ICPolarity, uint8_t TIM2_ICSelection,
4178                     ; 1297                        uint8_t TIM2_ICFilter)
4178                     ; 1298 {
4179                     	switch	.text
4180  04e1               L7_TI3_Config:
4182  04e1 89            	pushw	x
4183  04e2 88            	push	a
4184       00000001      OFST:	set	1
4187                     ; 1300     TIM2->CCER2 &=  (uint8_t)(~TIM2_CCER2_CC3E);
4189  04e3 7211530b      	bres	21259,#0
4190                     ; 1303     TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF))) 
4190                     ; 1304                             | (uint8_t)(( (TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));
4192  04e7 7b06          	ld	a,(OFST+5,sp)
4193  04e9 97            	ld	xl,a
4194  04ea a610          	ld	a,#16
4195  04ec 42            	mul	x,a
4196  04ed 9f            	ld	a,xl
4197  04ee 1a03          	or	a,(OFST+2,sp)
4198  04f0 6b01          	ld	(OFST+0,sp),a
4199  04f2 c65309        	ld	a,21257
4200  04f5 a40c          	and	a,#12
4201  04f7 1a01          	or	a,(OFST+0,sp)
4202  04f9 c75309        	ld	21257,a
4203                     ; 1308     if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
4205  04fc 0d02          	tnz	(OFST+1,sp)
4206  04fe 2706          	jreq	L5412
4207                     ; 1310         TIM2->CCER2 |= TIM2_CCER2_CC3P;
4209  0500 7212530b      	bset	21259,#1
4211  0504 2004          	jra	L7412
4212  0506               L5412:
4213                     ; 1314         TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3P);
4215  0506 7213530b      	bres	21259,#1
4216  050a               L7412:
4217                     ; 1317     TIM2->CCER2 |= TIM2_CCER2_CC3E;
4219  050a 7210530b      	bset	21259,#0
4220                     ; 1318 }
4223  050e 5b03          	addw	sp,#3
4224  0510 81            	ret
4237                     	xdef	_TIM2_ClearITPendingBit
4238                     	xdef	_TIM2_GetITStatus
4239                     	xdef	_TIM2_ClearFlag
4240                     	xdef	_TIM2_GetFlagStatus
4241                     	xdef	_TIM2_GetPrescaler
4242                     	xdef	_TIM2_GetCounter
4243                     	xdef	_TIM2_GetCapture3
4244                     	xdef	_TIM2_GetCapture2
4245                     	xdef	_TIM2_GetCapture1
4246                     	xdef	_TIM2_SetIC3Prescaler
4247                     	xdef	_TIM2_SetIC2Prescaler
4248                     	xdef	_TIM2_SetIC1Prescaler
4249                     	xdef	_TIM2_SetCompare3
4250                     	xdef	_TIM2_SetCompare2
4251                     	xdef	_TIM2_SetCompare1
4252                     	xdef	_TIM2_SetAutoreload
4253                     	xdef	_TIM2_SetCounter
4254                     	xdef	_TIM2_SelectOCxM
4255                     	xdef	_TIM2_CCxCmd
4256                     	xdef	_TIM2_OC3PolarityConfig
4257                     	xdef	_TIM2_OC2PolarityConfig
4258                     	xdef	_TIM2_OC1PolarityConfig
4259                     	xdef	_TIM2_GenerateEvent
4260                     	xdef	_TIM2_OC3PreloadConfig
4261                     	xdef	_TIM2_OC2PreloadConfig
4262                     	xdef	_TIM2_OC1PreloadConfig
4263                     	xdef	_TIM2_ARRPreloadConfig
4264                     	xdef	_TIM2_ForcedOC3Config
4265                     	xdef	_TIM2_ForcedOC2Config
4266                     	xdef	_TIM2_ForcedOC1Config
4267                     	xdef	_TIM2_PrescalerConfig
4268                     	xdef	_TIM2_SelectOnePulseMode
4269                     	xdef	_TIM2_UpdateRequestConfig
4270                     	xdef	_TIM2_UpdateDisableConfig
4271                     	xdef	_TIM2_ITConfig
4272                     	xdef	_TIM2_Cmd
4273                     	xdef	_TIM2_PWMIConfig
4274                     	xdef	_TIM2_ICInit
4275                     	xdef	_TIM2_OC3Init
4276                     	xdef	_TIM2_OC2Init
4277                     	xdef	_TIM2_OC1Init
4278                     	xdef	_TIM2_TimeBaseInit
4279                     	xdef	_TIM2_DeInit
4298                     	end
