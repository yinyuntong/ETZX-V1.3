   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 52 void TIM1_DeInit(void)
  32                     ; 53 {
  34                     .text:	section	.text,new
  35  0000               _TIM1_DeInit:
  39                     ; 54     TIM1->CR1  = TIM1_CR1_RESET_VALUE;
  41  0000 725f5250      	clr	21072
  42                     ; 55     TIM1->CR2  = TIM1_CR2_RESET_VALUE;
  44  0004 725f5251      	clr	21073
  45                     ; 56     TIM1->SMCR = TIM1_SMCR_RESET_VALUE;
  47  0008 725f5252      	clr	21074
  48                     ; 57     TIM1->ETR  = TIM1_ETR_RESET_VALUE;
  50  000c 725f5253      	clr	21075
  51                     ; 58     TIM1->IER  = TIM1_IER_RESET_VALUE;
  53  0010 725f5254      	clr	21076
  54                     ; 59     TIM1->SR2  = TIM1_SR2_RESET_VALUE;
  56  0014 725f5256      	clr	21078
  57                     ; 61     TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
  59  0018 725f525c      	clr	21084
  60                     ; 62     TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
  62  001c 725f525d      	clr	21085
  63                     ; 64     TIM1->CCMR1 = 0x01;
  65  0020 35015258      	mov	21080,#1
  66                     ; 65     TIM1->CCMR2 = 0x01;
  68  0024 35015259      	mov	21081,#1
  69                     ; 66     TIM1->CCMR3 = 0x01;
  71  0028 3501525a      	mov	21082,#1
  72                     ; 67     TIM1->CCMR4 = 0x01;
  74  002c 3501525b      	mov	21083,#1
  75                     ; 69     TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
  77  0030 725f525c      	clr	21084
  78                     ; 70     TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
  80  0034 725f525d      	clr	21085
  81                     ; 71     TIM1->CCMR1 = TIM1_CCMR1_RESET_VALUE;
  83  0038 725f5258      	clr	21080
  84                     ; 72     TIM1->CCMR2 = TIM1_CCMR2_RESET_VALUE;
  86  003c 725f5259      	clr	21081
  87                     ; 73     TIM1->CCMR3 = TIM1_CCMR3_RESET_VALUE;
  89  0040 725f525a      	clr	21082
  90                     ; 74     TIM1->CCMR4 = TIM1_CCMR4_RESET_VALUE;
  92  0044 725f525b      	clr	21083
  93                     ; 75     TIM1->CNTRH = TIM1_CNTRH_RESET_VALUE;
  95  0048 725f525e      	clr	21086
  96                     ; 76     TIM1->CNTRL = TIM1_CNTRL_RESET_VALUE;
  98  004c 725f525f      	clr	21087
  99                     ; 77     TIM1->PSCRH = TIM1_PSCRH_RESET_VALUE;
 101  0050 725f5260      	clr	21088
 102                     ; 78     TIM1->PSCRL = TIM1_PSCRL_RESET_VALUE;
 104  0054 725f5261      	clr	21089
 105                     ; 79     TIM1->ARRH  = TIM1_ARRH_RESET_VALUE;
 107  0058 35ff5262      	mov	21090,#255
 108                     ; 80     TIM1->ARRL  = TIM1_ARRL_RESET_VALUE;
 110  005c 35ff5263      	mov	21091,#255
 111                     ; 81     TIM1->CCR1H = TIM1_CCR1H_RESET_VALUE;
 113  0060 725f5265      	clr	21093
 114                     ; 82     TIM1->CCR1L = TIM1_CCR1L_RESET_VALUE;
 116  0064 725f5266      	clr	21094
 117                     ; 83     TIM1->CCR2H = TIM1_CCR2H_RESET_VALUE;
 119  0068 725f5267      	clr	21095
 120                     ; 84     TIM1->CCR2L = TIM1_CCR2L_RESET_VALUE;
 122  006c 725f5268      	clr	21096
 123                     ; 85     TIM1->CCR3H = TIM1_CCR3H_RESET_VALUE;
 125  0070 725f5269      	clr	21097
 126                     ; 86     TIM1->CCR3L = TIM1_CCR3L_RESET_VALUE;
 128  0074 725f526a      	clr	21098
 129                     ; 87     TIM1->CCR4H = TIM1_CCR4H_RESET_VALUE;
 131  0078 725f526b      	clr	21099
 132                     ; 88     TIM1->CCR4L = TIM1_CCR4L_RESET_VALUE;
 134  007c 725f526c      	clr	21100
 135                     ; 89     TIM1->OISR  = TIM1_OISR_RESET_VALUE;
 137  0080 725f526f      	clr	21103
 138                     ; 90     TIM1->EGR   = 0x01; /* TIM1_EGR_UG */
 140  0084 35015257      	mov	21079,#1
 141                     ; 91     TIM1->DTR   = TIM1_DTR_RESET_VALUE;
 143  0088 725f526e      	clr	21102
 144                     ; 92     TIM1->BKR   = TIM1_BKR_RESET_VALUE;
 146  008c 725f526d      	clr	21101
 147                     ; 93     TIM1->RCR   = TIM1_RCR_RESET_VALUE;
 149  0090 725f5264      	clr	21092
 150                     ; 94     TIM1->SR1   = TIM1_SR1_RESET_VALUE;
 152  0094 725f5255      	clr	21077
 153                     ; 95 }
 156  0098 81            	ret
 265                     ; 105 void TIM1_TimeBaseInit(uint16_t TIM1_Prescaler,
 265                     ; 106                        TIM1_CounterMode_TypeDef TIM1_CounterMode,
 265                     ; 107                        uint16_t TIM1_Period,
 265                     ; 108                        uint8_t TIM1_RepetitionCounter)
 265                     ; 109 {
 266                     .text:	section	.text,new
 267  0000               _TIM1_TimeBaseInit:
 269  0000 89            	pushw	x
 270       00000000      OFST:	set	0
 273                     ; 112     assert_param(IS_TIM1_COUNTER_MODE_OK(TIM1_CounterMode));
 275                     ; 115     TIM1->ARRH = (uint8_t)(TIM1_Period >> 8);
 277  0001 7b06          	ld	a,(OFST+6,sp)
 278  0003 c75262        	ld	21090,a
 279                     ; 116     TIM1->ARRL = (uint8_t)(TIM1_Period);
 281  0006 7b07          	ld	a,(OFST+7,sp)
 282  0008 c75263        	ld	21091,a
 283                     ; 119     TIM1->PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
 285  000b 9e            	ld	a,xh
 286  000c c75260        	ld	21088,a
 287                     ; 120     TIM1->PSCRL = (uint8_t)(TIM1_Prescaler);
 289  000f 9f            	ld	a,xl
 290  0010 c75261        	ld	21089,a
 291                     ; 123     TIM1->CR1 = (uint8_t)((uint8_t)(TIM1->CR1 & (uint8_t)(~(TIM1_CR1_CMS | TIM1_CR1_DIR)))
 291                     ; 124                            | (uint8_t)(TIM1_CounterMode));
 293  0013 c65250        	ld	a,21072
 294  0016 a48f          	and	a,#143
 295  0018 1a05          	or	a,(OFST+5,sp)
 296  001a c75250        	ld	21072,a
 297                     ; 127     TIM1->RCR = TIM1_RepetitionCounter;
 299  001d 7b08          	ld	a,(OFST+8,sp)
 300  001f c75264        	ld	21092,a
 301                     ; 129 }
 304  0022 85            	popw	x
 305  0023 81            	ret
 590                     ; 150 void TIM1_OC1Init(TIM1_OCMode_TypeDef TIM1_OCMode,
 590                     ; 151                   TIM1_OutputState_TypeDef TIM1_OutputState,
 590                     ; 152                   TIM1_OutputNState_TypeDef TIM1_OutputNState,
 590                     ; 153                   uint16_t TIM1_Pulse,
 590                     ; 154                   TIM1_OCPolarity_TypeDef TIM1_OCPolarity,
 590                     ; 155                   TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity,
 590                     ; 156                   TIM1_OCIdleState_TypeDef TIM1_OCIdleState,
 590                     ; 157                   TIM1_OCNIdleState_TypeDef TIM1_OCNIdleState)
 590                     ; 158 {
 591                     .text:	section	.text,new
 592  0000               _TIM1_OC1Init:
 594  0000 89            	pushw	x
 595  0001 5203          	subw	sp,#3
 596       00000003      OFST:	set	3
 599                     ; 160     assert_param(IS_TIM1_OC_MODE_OK(TIM1_OCMode));
 601                     ; 161     assert_param(IS_TIM1_OUTPUT_STATE_OK(TIM1_OutputState));
 603                     ; 162     assert_param(IS_TIM1_OUTPUTN_STATE_OK(TIM1_OutputNState));
 605                     ; 163     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
 607                     ; 164     assert_param(IS_TIM1_OCN_POLARITY_OK(TIM1_OCNPolarity));
 609                     ; 165     assert_param(IS_TIM1_OCIDLE_STATE_OK(TIM1_OCIdleState));
 611                     ; 166     assert_param(IS_TIM1_OCNIDLE_STATE_OK(TIM1_OCNIdleState));
 613                     ; 170     TIM1->CCER1 &= (uint8_t)(~( TIM1_CCER1_CC1E | TIM1_CCER1_CC1NE 
 613                     ; 171                                | TIM1_CCER1_CC1P | TIM1_CCER1_CC1NP));
 615  0003 c6525c        	ld	a,21084
 616  0006 a4f0          	and	a,#240
 617  0008 c7525c        	ld	21084,a
 618                     ; 174   TIM1->CCER1 |= (uint8_t)((uint8_t)((uint8_t)(TIM1_OutputState & TIM1_CCER1_CC1E)
 618                     ; 175                                      | (uint8_t)(TIM1_OutputNState & TIM1_CCER1_CC1NE))
 618                     ; 176                            | (uint8_t)( (uint8_t)(TIM1_OCPolarity  & TIM1_CCER1_CC1P)
 618                     ; 177                                         | (uint8_t)(TIM1_OCNPolarity & TIM1_CCER1_CC1NP)));
 620  000b 7b0c          	ld	a,(OFST+9,sp)
 621  000d a408          	and	a,#8
 622  000f 6b03          	ld	(OFST+0,sp),a
 623  0011 7b0b          	ld	a,(OFST+8,sp)
 624  0013 a402          	and	a,#2
 625  0015 1a03          	or	a,(OFST+0,sp)
 626  0017 6b02          	ld	(OFST-1,sp),a
 627  0019 7b08          	ld	a,(OFST+5,sp)
 628  001b a404          	and	a,#4
 629  001d 6b01          	ld	(OFST-2,sp),a
 630  001f 9f            	ld	a,xl
 631  0020 a401          	and	a,#1
 632  0022 1a01          	or	a,(OFST-2,sp)
 633  0024 1a02          	or	a,(OFST-1,sp)
 634  0026 ca525c        	or	a,21084
 635  0029 c7525c        	ld	21084,a
 636                     ; 180     TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_OCM)) | 
 636                     ; 181                             (uint8_t)TIM1_OCMode);
 638  002c c65258        	ld	a,21080
 639  002f a48f          	and	a,#143
 640  0031 1a04          	or	a,(OFST+1,sp)
 641  0033 c75258        	ld	21080,a
 642                     ; 184     TIM1->OISR &= (uint8_t)(~(TIM1_OISR_OIS1 | TIM1_OISR_OIS1N));
 644  0036 c6526f        	ld	a,21103
 645  0039 a4fc          	and	a,#252
 646  003b c7526f        	ld	21103,a
 647                     ; 186     TIM1->OISR |= (uint8_t)((uint8_t)( TIM1_OCIdleState & TIM1_OISR_OIS1 ) | 
 647                     ; 187                             (uint8_t)( TIM1_OCNIdleState & TIM1_OISR_OIS1N ));
 649  003e 7b0e          	ld	a,(OFST+11,sp)
 650  0040 a402          	and	a,#2
 651  0042 6b03          	ld	(OFST+0,sp),a
 652  0044 7b0d          	ld	a,(OFST+10,sp)
 653  0046 a401          	and	a,#1
 654  0048 1a03          	or	a,(OFST+0,sp)
 655  004a ca526f        	or	a,21103
 656  004d c7526f        	ld	21103,a
 657                     ; 190     TIM1->CCR1H = (uint8_t)(TIM1_Pulse >> 8);
 659  0050 7b09          	ld	a,(OFST+6,sp)
 660  0052 c75265        	ld	21093,a
 661                     ; 191     TIM1->CCR1L = (uint8_t)(TIM1_Pulse);
 663  0055 7b0a          	ld	a,(OFST+7,sp)
 664  0057 c75266        	ld	21094,a
 665                     ; 192 }
 668  005a 5b05          	addw	sp,#5
 669  005c 81            	ret
 773                     ; 213 void TIM1_OC2Init(TIM1_OCMode_TypeDef TIM1_OCMode,
 773                     ; 214                   TIM1_OutputState_TypeDef TIM1_OutputState,
 773                     ; 215                   TIM1_OutputNState_TypeDef TIM1_OutputNState,
 773                     ; 216                   uint16_t TIM1_Pulse,
 773                     ; 217                   TIM1_OCPolarity_TypeDef TIM1_OCPolarity,
 773                     ; 218                   TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity,
 773                     ; 219                   TIM1_OCIdleState_TypeDef TIM1_OCIdleState,
 773                     ; 220                   TIM1_OCNIdleState_TypeDef TIM1_OCNIdleState)
 773                     ; 221 {
 774                     .text:	section	.text,new
 775  0000               _TIM1_OC2Init:
 777  0000 89            	pushw	x
 778  0001 5203          	subw	sp,#3
 779       00000003      OFST:	set	3
 782                     ; 223     assert_param(IS_TIM1_OC_MODE_OK(TIM1_OCMode));
 784                     ; 224     assert_param(IS_TIM1_OUTPUT_STATE_OK(TIM1_OutputState));
 786                     ; 225     assert_param(IS_TIM1_OUTPUTN_STATE_OK(TIM1_OutputNState));
 788                     ; 226     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
 790                     ; 227     assert_param(IS_TIM1_OCN_POLARITY_OK(TIM1_OCNPolarity));
 792                     ; 228     assert_param(IS_TIM1_OCIDLE_STATE_OK(TIM1_OCIdleState));
 794                     ; 229     assert_param(IS_TIM1_OCNIDLE_STATE_OK(TIM1_OCNIdleState));
 796                     ; 233     TIM1->CCER1 &= (uint8_t)(~( TIM1_CCER1_CC2E | TIM1_CCER1_CC2NE | 
 796                     ; 234                                 TIM1_CCER1_CC2P | TIM1_CCER1_CC2NP));
 798  0003 c6525c        	ld	a,21084
 799  0006 a40f          	and	a,#15
 800  0008 c7525c        	ld	21084,a
 801                     ; 238     TIM1->CCER1 |= (uint8_t)((uint8_t)((uint8_t)(TIM1_OutputState & TIM1_CCER1_CC2E  ) | 
 801                     ; 239                              (uint8_t)(TIM1_OutputNState & TIM1_CCER1_CC2NE )) | 
 801                     ; 240                              (uint8_t)((uint8_t)(TIM1_OCPolarity  & TIM1_CCER1_CC2P  ) | 
 801                     ; 241                              (uint8_t)(TIM1_OCNPolarity & TIM1_CCER1_CC2NP )));
 803  000b 7b0c          	ld	a,(OFST+9,sp)
 804  000d a480          	and	a,#128
 805  000f 6b03          	ld	(OFST+0,sp),a
 806  0011 7b0b          	ld	a,(OFST+8,sp)
 807  0013 a420          	and	a,#32
 808  0015 1a03          	or	a,(OFST+0,sp)
 809  0017 6b02          	ld	(OFST-1,sp),a
 810  0019 7b08          	ld	a,(OFST+5,sp)
 811  001b a440          	and	a,#64
 812  001d 6b01          	ld	(OFST-2,sp),a
 813  001f 9f            	ld	a,xl
 814  0020 a410          	and	a,#16
 815  0022 1a01          	or	a,(OFST-2,sp)
 816  0024 1a02          	or	a,(OFST-1,sp)
 817  0026 ca525c        	or	a,21084
 818  0029 c7525c        	ld	21084,a
 819                     ; 244     TIM1->CCMR2 = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~TIM1_CCMR_OCM)) | 
 819                     ; 245                              (uint8_t)TIM1_OCMode);
 821  002c c65259        	ld	a,21081
 822  002f a48f          	and	a,#143
 823  0031 1a04          	or	a,(OFST+1,sp)
 824  0033 c75259        	ld	21081,a
 825                     ; 248     TIM1->OISR &= (uint8_t)(~(TIM1_OISR_OIS2 | TIM1_OISR_OIS2N));
 827  0036 c6526f        	ld	a,21103
 828  0039 a4f3          	and	a,#243
 829  003b c7526f        	ld	21103,a
 830                     ; 250     TIM1->OISR |= (uint8_t)((uint8_t)(TIM1_OISR_OIS2 & TIM1_OCIdleState) | 
 830                     ; 251                             (uint8_t)(TIM1_OISR_OIS2N & TIM1_OCNIdleState));
 832  003e 7b0e          	ld	a,(OFST+11,sp)
 833  0040 a408          	and	a,#8
 834  0042 6b03          	ld	(OFST+0,sp),a
 835  0044 7b0d          	ld	a,(OFST+10,sp)
 836  0046 a404          	and	a,#4
 837  0048 1a03          	or	a,(OFST+0,sp)
 838  004a ca526f        	or	a,21103
 839  004d c7526f        	ld	21103,a
 840                     ; 254     TIM1->CCR2H = (uint8_t)(TIM1_Pulse >> 8);
 842  0050 7b09          	ld	a,(OFST+6,sp)
 843  0052 c75267        	ld	21095,a
 844                     ; 255     TIM1->CCR2L = (uint8_t)(TIM1_Pulse);
 846  0055 7b0a          	ld	a,(OFST+7,sp)
 847  0057 c75268        	ld	21096,a
 848                     ; 257 }
 851  005a 5b05          	addw	sp,#5
 852  005c 81            	ret
 956                     ; 278 void TIM1_OC3Init(TIM1_OCMode_TypeDef TIM1_OCMode,
 956                     ; 279                   TIM1_OutputState_TypeDef TIM1_OutputState,
 956                     ; 280                   TIM1_OutputNState_TypeDef TIM1_OutputNState,
 956                     ; 281                   uint16_t TIM1_Pulse,
 956                     ; 282                   TIM1_OCPolarity_TypeDef TIM1_OCPolarity,
 956                     ; 283                   TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity,
 956                     ; 284                   TIM1_OCIdleState_TypeDef TIM1_OCIdleState,
 956                     ; 285                   TIM1_OCNIdleState_TypeDef TIM1_OCNIdleState)
 956                     ; 286 {
 957                     .text:	section	.text,new
 958  0000               _TIM1_OC3Init:
 960  0000 89            	pushw	x
 961  0001 5203          	subw	sp,#3
 962       00000003      OFST:	set	3
 965                     ; 288     assert_param(IS_TIM1_OC_MODE_OK(TIM1_OCMode));
 967                     ; 289     assert_param(IS_TIM1_OUTPUT_STATE_OK(TIM1_OutputState));
 969                     ; 290     assert_param(IS_TIM1_OUTPUTN_STATE_OK(TIM1_OutputNState));
 971                     ; 291     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
 973                     ; 292     assert_param(IS_TIM1_OCN_POLARITY_OK(TIM1_OCNPolarity));
 975                     ; 293     assert_param(IS_TIM1_OCIDLE_STATE_OK(TIM1_OCIdleState));
 977                     ; 294     assert_param(IS_TIM1_OCNIDLE_STATE_OK(TIM1_OCNIdleState));
 979                     ; 298     TIM1->CCER2 &= (uint8_t)(~( TIM1_CCER2_CC3E | TIM1_CCER2_CC3NE | 
 979                     ; 299                                 TIM1_CCER2_CC3P | TIM1_CCER2_CC3NP));
 981  0003 c6525d        	ld	a,21085
 982  0006 a4f0          	and	a,#240
 983  0008 c7525d        	ld	21085,a
 984                     ; 302     TIM1->CCER2 |= (uint8_t)((uint8_t)((uint8_t)(TIM1_OutputState  & TIM1_CCER2_CC3E   ) |
 984                     ; 303                              (uint8_t)(TIM1_OutputNState & TIM1_CCER2_CC3NE  )) | 
 984                     ; 304                              (uint8_t)((uint8_t)(TIM1_OCPolarity   & TIM1_CCER2_CC3P   ) | 
 984                     ; 305                              (uint8_t)(TIM1_OCNPolarity  & TIM1_CCER2_CC3NP  )));
 986  000b 7b0c          	ld	a,(OFST+9,sp)
 987  000d a408          	and	a,#8
 988  000f 6b03          	ld	(OFST+0,sp),a
 989  0011 7b0b          	ld	a,(OFST+8,sp)
 990  0013 a402          	and	a,#2
 991  0015 1a03          	or	a,(OFST+0,sp)
 992  0017 6b02          	ld	(OFST-1,sp),a
 993  0019 7b08          	ld	a,(OFST+5,sp)
 994  001b a404          	and	a,#4
 995  001d 6b01          	ld	(OFST-2,sp),a
 996  001f 9f            	ld	a,xl
 997  0020 a401          	and	a,#1
 998  0022 1a01          	or	a,(OFST-2,sp)
 999  0024 1a02          	or	a,(OFST-1,sp)
1000  0026 ca525d        	or	a,21085
1001  0029 c7525d        	ld	21085,a
1002                     ; 308     TIM1->CCMR3 = (uint8_t)((uint8_t)(TIM1->CCMR3 & (uint8_t)(~TIM1_CCMR_OCM)) | 
1002                     ; 309                              (uint8_t)TIM1_OCMode);
1004  002c c6525a        	ld	a,21082
1005  002f a48f          	and	a,#143
1006  0031 1a04          	or	a,(OFST+1,sp)
1007  0033 c7525a        	ld	21082,a
1008                     ; 312     TIM1->OISR &= (uint8_t)(~(TIM1_OISR_OIS3 | TIM1_OISR_OIS3N));
1010  0036 c6526f        	ld	a,21103
1011  0039 a4cf          	and	a,#207
1012  003b c7526f        	ld	21103,a
1013                     ; 314     TIM1->OISR |= (uint8_t)((uint8_t)(TIM1_OISR_OIS3 & TIM1_OCIdleState) | 
1013                     ; 315                             (uint8_t)(TIM1_OISR_OIS3N & TIM1_OCNIdleState));
1015  003e 7b0e          	ld	a,(OFST+11,sp)
1016  0040 a420          	and	a,#32
1017  0042 6b03          	ld	(OFST+0,sp),a
1018  0044 7b0d          	ld	a,(OFST+10,sp)
1019  0046 a410          	and	a,#16
1020  0048 1a03          	or	a,(OFST+0,sp)
1021  004a ca526f        	or	a,21103
1022  004d c7526f        	ld	21103,a
1023                     ; 318     TIM1->CCR3H = (uint8_t)(TIM1_Pulse >> 8);
1025  0050 7b09          	ld	a,(OFST+6,sp)
1026  0052 c75269        	ld	21097,a
1027                     ; 319     TIM1->CCR3L = (uint8_t)(TIM1_Pulse);
1029  0055 7b0a          	ld	a,(OFST+7,sp)
1030  0057 c7526a        	ld	21098,a
1031                     ; 321 }
1034  005a 5b05          	addw	sp,#5
1035  005c 81            	ret
1109                     ; 336 void TIM1_OC4Init(TIM1_OCMode_TypeDef TIM1_OCMode,
1109                     ; 337                   TIM1_OutputState_TypeDef TIM1_OutputState,
1109                     ; 338                   uint16_t TIM1_Pulse,
1109                     ; 339                   TIM1_OCPolarity_TypeDef TIM1_OCPolarity,
1109                     ; 340                   TIM1_OCIdleState_TypeDef TIM1_OCIdleState)
1109                     ; 341 {
1110                     .text:	section	.text,new
1111  0000               _TIM1_OC4Init:
1113  0000 89            	pushw	x
1114  0001 88            	push	a
1115       00000001      OFST:	set	1
1118                     ; 343     assert_param(IS_TIM1_OC_MODE_OK(TIM1_OCMode));
1120                     ; 344     assert_param(IS_TIM1_OUTPUT_STATE_OK(TIM1_OutputState));
1122                     ; 345     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
1124                     ; 346     assert_param(IS_TIM1_OCIDLE_STATE_OK(TIM1_OCIdleState));
1126                     ; 349     TIM1->CCER2 &= (uint8_t)(~(TIM1_CCER2_CC4E | TIM1_CCER2_CC4P));
1128  0002 c6525d        	ld	a,21085
1129  0005 a4cf          	and	a,#207
1130  0007 c7525d        	ld	21085,a
1131                     ; 351     TIM1->CCER2 |= (uint8_t)((uint8_t)(TIM1_OutputState & TIM1_CCER2_CC4E ) |  
1131                     ; 352                              (uint8_t)(TIM1_OCPolarity  & TIM1_CCER2_CC4P ));
1133  000a 7b08          	ld	a,(OFST+7,sp)
1134  000c a420          	and	a,#32
1135  000e 6b01          	ld	(OFST+0,sp),a
1136  0010 9f            	ld	a,xl
1137  0011 a410          	and	a,#16
1138  0013 1a01          	or	a,(OFST+0,sp)
1139  0015 ca525d        	or	a,21085
1140  0018 c7525d        	ld	21085,a
1141                     ; 355     TIM1->CCMR4 = (uint8_t)((uint8_t)(TIM1->CCMR4 & (uint8_t)(~TIM1_CCMR_OCM)) | 
1141                     ; 356                             TIM1_OCMode);
1143  001b c6525b        	ld	a,21083
1144  001e a48f          	and	a,#143
1145  0020 1a02          	or	a,(OFST+1,sp)
1146  0022 c7525b        	ld	21083,a
1147                     ; 359     if (TIM1_OCIdleState != TIM1_OCIDLESTATE_RESET)
1149  0025 0d09          	tnz	(OFST+8,sp)
1150  0027 270a          	jreq	L534
1151                     ; 361         TIM1->OISR |= (uint8_t)(~TIM1_CCER2_CC4P);
1153  0029 c6526f        	ld	a,21103
1154  002c aadf          	or	a,#223
1155  002e c7526f        	ld	21103,a
1157  0031 2004          	jra	L734
1158  0033               L534:
1159                     ; 365         TIM1->OISR &= (uint8_t)(~TIM1_OISR_OIS4);
1161  0033 721d526f      	bres	21103,#6
1162  0037               L734:
1163                     ; 369     TIM1->CCR4H = (uint8_t)(TIM1_Pulse >> 8);
1165  0037 7b06          	ld	a,(OFST+5,sp)
1166  0039 c7526b        	ld	21099,a
1167                     ; 370     TIM1->CCR4L = (uint8_t)(TIM1_Pulse);
1169  003c 7b07          	ld	a,(OFST+6,sp)
1170  003e c7526c        	ld	21100,a
1171                     ; 372 }
1174  0041 5b03          	addw	sp,#3
1175  0043 81            	ret
1380                     ; 387 void TIM1_BDTRConfig(TIM1_OSSIState_TypeDef TIM1_OSSIState,
1380                     ; 388                      TIM1_LockLevel_TypeDef TIM1_LockLevel,
1380                     ; 389                      uint8_t TIM1_DeadTime,
1380                     ; 390                      TIM1_BreakState_TypeDef TIM1_Break,
1380                     ; 391                      TIM1_BreakPolarity_TypeDef TIM1_BreakPolarity,
1380                     ; 392                      TIM1_AutomaticOutput_TypeDef TIM1_AutomaticOutput)
1380                     ; 393 {
1381                     .text:	section	.text,new
1382  0000               _TIM1_BDTRConfig:
1384  0000 89            	pushw	x
1385  0001 88            	push	a
1386       00000001      OFST:	set	1
1389                     ; 395     assert_param(IS_TIM1_OSSI_STATE_OK(TIM1_OSSIState));
1391                     ; 396     assert_param(IS_TIM1_LOCK_LEVEL_OK(TIM1_LockLevel));
1393                     ; 397     assert_param(IS_TIM1_BREAK_STATE_OK(TIM1_Break));
1395                     ; 398     assert_param(IS_TIM1_BREAK_POLARITY_OK(TIM1_BreakPolarity));
1397                     ; 399     assert_param(IS_TIM1_AUTOMATIC_OUTPUT_STATE_OK(TIM1_AutomaticOutput));
1399                     ; 401     TIM1->DTR = (uint8_t)(TIM1_DeadTime);
1401  0002 7b06          	ld	a,(OFST+5,sp)
1402  0004 c7526e        	ld	21102,a
1403                     ; 405     TIM1->BKR  =  (uint8_t)((uint8_t)(TIM1_OSSIState | (uint8_t)TIM1_LockLevel)  | 
1403                     ; 406                             (uint8_t)((uint8_t)(TIM1_Break | (uint8_t)TIM1_BreakPolarity)  | 
1403                     ; 407                             (uint8_t)TIM1_AutomaticOutput));
1405  0007 7b07          	ld	a,(OFST+6,sp)
1406  0009 1a08          	or	a,(OFST+7,sp)
1407  000b 1a09          	or	a,(OFST+8,sp)
1408  000d 6b01          	ld	(OFST+0,sp),a
1409  000f 9f            	ld	a,xl
1410  0010 1a02          	or	a,(OFST+1,sp)
1411  0012 1a01          	or	a,(OFST+0,sp)
1412  0014 c7526d        	ld	21101,a
1413                     ; 409 }
1416  0017 5b03          	addw	sp,#3
1417  0019 81            	ret
1619                     ; 423 void TIM1_ICInit(TIM1_Channel_TypeDef TIM1_Channel,
1619                     ; 424                  TIM1_ICPolarity_TypeDef TIM1_ICPolarity,
1619                     ; 425                  TIM1_ICSelection_TypeDef TIM1_ICSelection,
1619                     ; 426                  TIM1_ICPSC_TypeDef TIM1_ICPrescaler,
1619                     ; 427                  uint8_t TIM1_ICFilter)
1619                     ; 428 {
1620                     .text:	section	.text,new
1621  0000               _TIM1_ICInit:
1623  0000 89            	pushw	x
1624       00000000      OFST:	set	0
1627                     ; 431     assert_param(IS_TIM1_CHANNEL_OK(TIM1_Channel));
1629                     ; 432     assert_param(IS_TIM1_IC_POLARITY_OK(TIM1_ICPolarity));
1631                     ; 433     assert_param(IS_TIM1_IC_SELECTION_OK(TIM1_ICSelection));
1633                     ; 434     assert_param(IS_TIM1_IC_PRESCALER_OK(TIM1_ICPrescaler));
1635                     ; 435     assert_param(IS_TIM1_IC_FILTER_OK(TIM1_ICFilter));
1637                     ; 437     if (TIM1_Channel == TIM1_CHANNEL_1)
1639  0001 9e            	ld	a,xh
1640  0002 4d            	tnz	a
1641  0003 2614          	jrne	L766
1642                     ; 440         TI1_Config((uint8_t)TIM1_ICPolarity,
1642                     ; 441                    (uint8_t)TIM1_ICSelection,
1642                     ; 442                    (uint8_t)TIM1_ICFilter);
1644  0005 7b07          	ld	a,(OFST+7,sp)
1645  0007 88            	push	a
1646  0008 7b06          	ld	a,(OFST+6,sp)
1647  000a 97            	ld	xl,a
1648  000b 7b03          	ld	a,(OFST+3,sp)
1649  000d 95            	ld	xh,a
1650  000e cd0000        	call	L3_TI1_Config
1652  0011 84            	pop	a
1653                     ; 444         TIM1_SetIC1Prescaler(TIM1_ICPrescaler);
1655  0012 7b06          	ld	a,(OFST+6,sp)
1656  0014 cd0000        	call	_TIM1_SetIC1Prescaler
1659  0017 2046          	jra	L176
1660  0019               L766:
1661                     ; 446     else if (TIM1_Channel == TIM1_CHANNEL_2)
1663  0019 7b01          	ld	a,(OFST+1,sp)
1664  001b a101          	cp	a,#1
1665  001d 2614          	jrne	L376
1666                     ; 449         TI2_Config((uint8_t)TIM1_ICPolarity,
1666                     ; 450                    (uint8_t)TIM1_ICSelection,
1666                     ; 451                    (uint8_t)TIM1_ICFilter);
1668  001f 7b07          	ld	a,(OFST+7,sp)
1669  0021 88            	push	a
1670  0022 7b06          	ld	a,(OFST+6,sp)
1671  0024 97            	ld	xl,a
1672  0025 7b03          	ld	a,(OFST+3,sp)
1673  0027 95            	ld	xh,a
1674  0028 cd0000        	call	L5_TI2_Config
1676  002b 84            	pop	a
1677                     ; 453         TIM1_SetIC2Prescaler(TIM1_ICPrescaler);
1679  002c 7b06          	ld	a,(OFST+6,sp)
1680  002e cd0000        	call	_TIM1_SetIC2Prescaler
1683  0031 202c          	jra	L176
1684  0033               L376:
1685                     ; 455     else if (TIM1_Channel == TIM1_CHANNEL_3)
1687  0033 7b01          	ld	a,(OFST+1,sp)
1688  0035 a102          	cp	a,#2
1689  0037 2614          	jrne	L776
1690                     ; 458         TI3_Config((uint8_t)TIM1_ICPolarity,
1690                     ; 459                    (uint8_t)TIM1_ICSelection,
1690                     ; 460                    (uint8_t)TIM1_ICFilter);
1692  0039 7b07          	ld	a,(OFST+7,sp)
1693  003b 88            	push	a
1694  003c 7b06          	ld	a,(OFST+6,sp)
1695  003e 97            	ld	xl,a
1696  003f 7b03          	ld	a,(OFST+3,sp)
1697  0041 95            	ld	xh,a
1698  0042 cd0000        	call	L7_TI3_Config
1700  0045 84            	pop	a
1701                     ; 462         TIM1_SetIC3Prescaler(TIM1_ICPrescaler);
1703  0046 7b06          	ld	a,(OFST+6,sp)
1704  0048 cd0000        	call	_TIM1_SetIC3Prescaler
1707  004b 2012          	jra	L176
1708  004d               L776:
1709                     ; 467         TI4_Config((uint8_t)TIM1_ICPolarity,
1709                     ; 468                    (uint8_t)TIM1_ICSelection,
1709                     ; 469                    (uint8_t)TIM1_ICFilter);
1711  004d 7b07          	ld	a,(OFST+7,sp)
1712  004f 88            	push	a
1713  0050 7b06          	ld	a,(OFST+6,sp)
1714  0052 97            	ld	xl,a
1715  0053 7b03          	ld	a,(OFST+3,sp)
1716  0055 95            	ld	xh,a
1717  0056 cd0000        	call	L11_TI4_Config
1719  0059 84            	pop	a
1720                     ; 471         TIM1_SetIC4Prescaler(TIM1_ICPrescaler);
1722  005a 7b06          	ld	a,(OFST+6,sp)
1723  005c cd0000        	call	_TIM1_SetIC4Prescaler
1725  005f               L176:
1726                     ; 474 }
1729  005f 85            	popw	x
1730  0060 81            	ret
1826                     ; 490 void TIM1_PWMIConfig(TIM1_Channel_TypeDef TIM1_Channel,
1826                     ; 491                      TIM1_ICPolarity_TypeDef TIM1_ICPolarity,
1826                     ; 492                      TIM1_ICSelection_TypeDef TIM1_ICSelection,
1826                     ; 493                      TIM1_ICPSC_TypeDef TIM1_ICPrescaler,
1826                     ; 494                      uint8_t TIM1_ICFilter)
1826                     ; 495 {
1827                     .text:	section	.text,new
1828  0000               _TIM1_PWMIConfig:
1830  0000 89            	pushw	x
1831  0001 89            	pushw	x
1832       00000002      OFST:	set	2
1835                     ; 496     uint8_t icpolarity = TIM1_ICPOLARITY_RISING;
1837  0002 7b01          	ld	a,(OFST-1,sp)
1838  0004 97            	ld	xl,a
1839                     ; 497     uint8_t icselection = TIM1_ICSELECTION_DIRECTTI;
1841  0005 7b02          	ld	a,(OFST+0,sp)
1842  0007 97            	ld	xl,a
1843                     ; 500     assert_param(IS_TIM1_PWMI_CHANNEL_OK(TIM1_Channel));
1845                     ; 501     assert_param(IS_TIM1_IC_POLARITY_OK(TIM1_ICPolarity));
1847                     ; 502     assert_param(IS_TIM1_IC_SELECTION_OK(TIM1_ICSelection));
1849                     ; 503     assert_param(IS_TIM1_IC_PRESCALER_OK(TIM1_ICPrescaler));
1851                     ; 506     if (TIM1_ICPolarity != TIM1_ICPOLARITY_FALLING)
1853  0008 7b04          	ld	a,(OFST+2,sp)
1854  000a a101          	cp	a,#1
1855  000c 2706          	jreq	L157
1856                     ; 508         icpolarity = TIM1_ICPOLARITY_FALLING;
1858  000e a601          	ld	a,#1
1859  0010 6b01          	ld	(OFST-1,sp),a
1861  0012 2002          	jra	L357
1862  0014               L157:
1863                     ; 512         icpolarity = TIM1_ICPOLARITY_RISING;
1865  0014 0f01          	clr	(OFST-1,sp)
1866  0016               L357:
1867                     ; 516     if (TIM1_ICSelection == TIM1_ICSELECTION_DIRECTTI)
1869  0016 7b07          	ld	a,(OFST+5,sp)
1870  0018 a101          	cp	a,#1
1871  001a 2606          	jrne	L557
1872                     ; 518         icselection = TIM1_ICSELECTION_INDIRECTTI;
1874  001c a602          	ld	a,#2
1875  001e 6b02          	ld	(OFST+0,sp),a
1877  0020 2004          	jra	L757
1878  0022               L557:
1879                     ; 522         icselection = TIM1_ICSELECTION_DIRECTTI;
1881  0022 a601          	ld	a,#1
1882  0024 6b02          	ld	(OFST+0,sp),a
1883  0026               L757:
1884                     ; 525     if (TIM1_Channel == TIM1_CHANNEL_1)
1886  0026 0d03          	tnz	(OFST+1,sp)
1887  0028 2626          	jrne	L167
1888                     ; 528         TI1_Config((uint8_t)TIM1_ICPolarity, (uint8_t)TIM1_ICSelection,
1888                     ; 529                    (uint8_t)TIM1_ICFilter);
1890  002a 7b09          	ld	a,(OFST+7,sp)
1891  002c 88            	push	a
1892  002d 7b08          	ld	a,(OFST+6,sp)
1893  002f 97            	ld	xl,a
1894  0030 7b05          	ld	a,(OFST+3,sp)
1895  0032 95            	ld	xh,a
1896  0033 cd0000        	call	L3_TI1_Config
1898  0036 84            	pop	a
1899                     ; 532         TIM1_SetIC1Prescaler(TIM1_ICPrescaler);
1901  0037 7b08          	ld	a,(OFST+6,sp)
1902  0039 cd0000        	call	_TIM1_SetIC1Prescaler
1904                     ; 535         TI2_Config(icpolarity, icselection, TIM1_ICFilter);
1906  003c 7b09          	ld	a,(OFST+7,sp)
1907  003e 88            	push	a
1908  003f 7b03          	ld	a,(OFST+1,sp)
1909  0041 97            	ld	xl,a
1910  0042 7b02          	ld	a,(OFST+0,sp)
1911  0044 95            	ld	xh,a
1912  0045 cd0000        	call	L5_TI2_Config
1914  0048 84            	pop	a
1915                     ; 538         TIM1_SetIC2Prescaler(TIM1_ICPrescaler);
1917  0049 7b08          	ld	a,(OFST+6,sp)
1918  004b cd0000        	call	_TIM1_SetIC2Prescaler
1921  004e 2024          	jra	L367
1922  0050               L167:
1923                     ; 543         TI2_Config((uint8_t)TIM1_ICPolarity, (uint8_t)TIM1_ICSelection,
1923                     ; 544                    (uint8_t)TIM1_ICFilter);
1925  0050 7b09          	ld	a,(OFST+7,sp)
1926  0052 88            	push	a
1927  0053 7b08          	ld	a,(OFST+6,sp)
1928  0055 97            	ld	xl,a
1929  0056 7b05          	ld	a,(OFST+3,sp)
1930  0058 95            	ld	xh,a
1931  0059 cd0000        	call	L5_TI2_Config
1933  005c 84            	pop	a
1934                     ; 547         TIM1_SetIC2Prescaler(TIM1_ICPrescaler);
1936  005d 7b08          	ld	a,(OFST+6,sp)
1937  005f cd0000        	call	_TIM1_SetIC2Prescaler
1939                     ; 550         TI1_Config(icpolarity, icselection, TIM1_ICFilter);
1941  0062 7b09          	ld	a,(OFST+7,sp)
1942  0064 88            	push	a
1943  0065 7b03          	ld	a,(OFST+1,sp)
1944  0067 97            	ld	xl,a
1945  0068 7b02          	ld	a,(OFST+0,sp)
1946  006a 95            	ld	xh,a
1947  006b cd0000        	call	L3_TI1_Config
1949  006e 84            	pop	a
1950                     ; 553         TIM1_SetIC1Prescaler(TIM1_ICPrescaler);
1952  006f 7b08          	ld	a,(OFST+6,sp)
1953  0071 cd0000        	call	_TIM1_SetIC1Prescaler
1955  0074               L367:
1956                     ; 555 }
1959  0074 5b04          	addw	sp,#4
1960  0076 81            	ret
2015                     ; 563 void TIM1_Cmd(FunctionalState NewState)
2015                     ; 564 {
2016                     .text:	section	.text,new
2017  0000               _TIM1_Cmd:
2021                     ; 566     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2023                     ; 569     if (NewState != DISABLE)
2025  0000 4d            	tnz	a
2026  0001 2706          	jreq	L3101
2027                     ; 571         TIM1->CR1 |= TIM1_CR1_CEN;
2029  0003 72105250      	bset	21072,#0
2031  0007 2004          	jra	L5101
2032  0009               L3101:
2033                     ; 575         TIM1->CR1 &= (uint8_t)(~TIM1_CR1_CEN);
2035  0009 72115250      	bres	21072,#0
2036  000d               L5101:
2037                     ; 577 }
2040  000d 81            	ret
2076                     ; 585 void TIM1_CtrlPWMOutputs(FunctionalState NewState)
2076                     ; 586 {
2077                     .text:	section	.text,new
2078  0000               _TIM1_CtrlPWMOutputs:
2082                     ; 588     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2084                     ; 592     if (NewState != DISABLE)
2086  0000 4d            	tnz	a
2087  0001 2706          	jreq	L5301
2088                     ; 594         TIM1->BKR |= TIM1_BKR_MOE;
2090  0003 721e526d      	bset	21101,#7
2092  0007 2004          	jra	L7301
2093  0009               L5301:
2094                     ; 598         TIM1->BKR &= (uint8_t)(~TIM1_BKR_MOE);
2096  0009 721f526d      	bres	21101,#7
2097  000d               L7301:
2098                     ; 600 }
2101  000d 81            	ret
2208                     ; 619 void TIM1_ITConfig(TIM1_IT_TypeDef  TIM1_IT, FunctionalState NewState)
2208                     ; 620 {
2209                     .text:	section	.text,new
2210  0000               _TIM1_ITConfig:
2212  0000 89            	pushw	x
2213       00000000      OFST:	set	0
2216                     ; 622     assert_param(IS_TIM1_IT_OK(TIM1_IT));
2218                     ; 623     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2220                     ; 625     if (NewState != DISABLE)
2222  0001 9f            	ld	a,xl
2223  0002 4d            	tnz	a
2224  0003 2709          	jreq	L7011
2225                     ; 628         TIM1->IER |= (uint8_t)TIM1_IT;
2227  0005 9e            	ld	a,xh
2228  0006 ca5254        	or	a,21076
2229  0009 c75254        	ld	21076,a
2231  000c 2009          	jra	L1111
2232  000e               L7011:
2233                     ; 633         TIM1->IER &= (uint8_t)(~(uint8_t)TIM1_IT);
2235  000e 7b01          	ld	a,(OFST+1,sp)
2236  0010 43            	cpl	a
2237  0011 c45254        	and	a,21076
2238  0014 c75254        	ld	21076,a
2239  0017               L1111:
2240                     ; 635 }
2243  0017 85            	popw	x
2244  0018 81            	ret
2268                     ; 642 void TIM1_InternalClockConfig(void)
2268                     ; 643 {
2269                     .text:	section	.text,new
2270  0000               _TIM1_InternalClockConfig:
2274                     ; 645     TIM1->SMCR &= (uint8_t)(~TIM1_SMCR_SMS);
2276  0000 c65252        	ld	a,21074
2277  0003 a4f8          	and	a,#248
2278  0005 c75252        	ld	21074,a
2279                     ; 646 }
2282  0008 81            	ret
2399                     ; 664 void TIM1_ETRClockMode1Config(TIM1_ExtTRGPSC_TypeDef TIM1_ExtTRGPrescaler,
2399                     ; 665                               TIM1_ExtTRGPolarity_TypeDef TIM1_ExtTRGPolarity,
2399                     ; 666                               uint8_t ExtTRGFilter)
2399                     ; 667 {
2400                     .text:	section	.text,new
2401  0000               _TIM1_ETRClockMode1Config:
2403  0000 89            	pushw	x
2404       00000000      OFST:	set	0
2407                     ; 669     assert_param(IS_TIM1_EXT_PRESCALER_OK(TIM1_ExtTRGPrescaler));
2409                     ; 670     assert_param(IS_TIM1_EXT_POLARITY_OK(TIM1_ExtTRGPolarity));
2411                     ; 673     TIM1_ETRConfig(TIM1_ExtTRGPrescaler, TIM1_ExtTRGPolarity, ExtTRGFilter);
2413  0001 7b05          	ld	a,(OFST+5,sp)
2414  0003 88            	push	a
2415  0004 9f            	ld	a,xl
2416  0005 97            	ld	xl,a
2417  0006 7b02          	ld	a,(OFST+2,sp)
2418  0008 95            	ld	xh,a
2419  0009 cd0000        	call	_TIM1_ETRConfig
2421  000c 84            	pop	a
2422                     ; 676     TIM1->SMCR = (uint8_t)((uint8_t)(TIM1->SMCR & (uint8_t)(~(uint8_t)(TIM1_SMCR_SMS | TIM1_SMCR_TS )))
2422                     ; 677                            | (uint8_t)((uint8_t)TIM1_SLAVEMODE_EXTERNAL1 | TIM1_TS_ETRF ));
2424  000d c65252        	ld	a,21074
2425  0010 a488          	and	a,#136
2426  0012 aa77          	or	a,#119
2427  0014 c75252        	ld	21074,a
2428                     ; 678 }
2431  0017 85            	popw	x
2432  0018 81            	ret
2490                     ; 696 void TIM1_ETRClockMode2Config(TIM1_ExtTRGPSC_TypeDef TIM1_ExtTRGPrescaler,
2490                     ; 697                               TIM1_ExtTRGPolarity_TypeDef TIM1_ExtTRGPolarity,
2490                     ; 698                               uint8_t ExtTRGFilter)
2490                     ; 699 {
2491                     .text:	section	.text,new
2492  0000               _TIM1_ETRClockMode2Config:
2494  0000 89            	pushw	x
2495       00000000      OFST:	set	0
2498                     ; 701     assert_param(IS_TIM1_EXT_PRESCALER_OK(TIM1_ExtTRGPrescaler));
2500                     ; 702     assert_param(IS_TIM1_EXT_POLARITY_OK(TIM1_ExtTRGPolarity));
2502                     ; 705     TIM1_ETRConfig(TIM1_ExtTRGPrescaler, TIM1_ExtTRGPolarity, ExtTRGFilter);
2504  0001 7b05          	ld	a,(OFST+5,sp)
2505  0003 88            	push	a
2506  0004 9f            	ld	a,xl
2507  0005 97            	ld	xl,a
2508  0006 7b02          	ld	a,(OFST+2,sp)
2509  0008 95            	ld	xh,a
2510  0009 cd0000        	call	_TIM1_ETRConfig
2512  000c 84            	pop	a
2513                     ; 708     TIM1->ETR |= TIM1_ETR_ECE;
2515  000d 721c5253      	bset	21075,#6
2516                     ; 709 }
2519  0011 85            	popw	x
2520  0012 81            	ret
2576                     ; 727 void TIM1_ETRConfig(TIM1_ExtTRGPSC_TypeDef TIM1_ExtTRGPrescaler,
2576                     ; 728                     TIM1_ExtTRGPolarity_TypeDef TIM1_ExtTRGPolarity,
2576                     ; 729                     uint8_t ExtTRGFilter)
2576                     ; 730 {
2577                     .text:	section	.text,new
2578  0000               _TIM1_ETRConfig:
2580  0000 89            	pushw	x
2581       00000000      OFST:	set	0
2584                     ; 732     assert_param(IS_TIM1_EXT_TRG_FILTER_OK(ExtTRGFilter));
2586                     ; 734     TIM1->ETR |= (uint8_t)((uint8_t)(TIM1_ExtTRGPrescaler | (uint8_t)TIM1_ExtTRGPolarity )|
2586                     ; 735                       (uint8_t)ExtTRGFilter );
2588  0001 9f            	ld	a,xl
2589  0002 1a01          	or	a,(OFST+1,sp)
2590  0004 1a05          	or	a,(OFST+5,sp)
2591  0006 ca5253        	or	a,21075
2592  0009 c75253        	ld	21075,a
2593                     ; 736 }
2596  000c 85            	popw	x
2597  000d 81            	ret
2686                     ; 753 void TIM1_TIxExternalClockConfig(TIM1_TIxExternalCLK1Source_TypeDef TIM1_TIxExternalCLKSource,
2686                     ; 754                                  TIM1_ICPolarity_TypeDef TIM1_ICPolarity,
2686                     ; 755                                  uint8_t ICFilter)
2686                     ; 756 {
2687                     .text:	section	.text,new
2688  0000               _TIM1_TIxExternalClockConfig:
2690  0000 89            	pushw	x
2691       00000000      OFST:	set	0
2694                     ; 758     assert_param(IS_TIM1_TIXCLK_SOURCE_OK(TIM1_TIxExternalCLKSource));
2696                     ; 759     assert_param(IS_TIM1_IC_POLARITY_OK(TIM1_ICPolarity));
2698                     ; 760     assert_param(IS_TIM1_IC_FILTER_OK(ICFilter));
2700                     ; 763     if (TIM1_TIxExternalCLKSource == TIM1_TIXEXTERNALCLK1SOURCE_TI2)
2702  0001 9e            	ld	a,xh
2703  0002 a160          	cp	a,#96
2704  0004 260f          	jrne	L1131
2705                     ; 765         TI2_Config((uint8_t)TIM1_ICPolarity, (uint8_t)TIM1_ICSELECTION_DIRECTTI, (uint8_t)ICFilter);
2707  0006 7b05          	ld	a,(OFST+5,sp)
2708  0008 88            	push	a
2709  0009 ae0001        	ldw	x,#1
2710  000c 7b03          	ld	a,(OFST+3,sp)
2711  000e 95            	ld	xh,a
2712  000f cd0000        	call	L5_TI2_Config
2714  0012 84            	pop	a
2716  0013 200d          	jra	L3131
2717  0015               L1131:
2718                     ; 769         TI1_Config((uint8_t)TIM1_ICPolarity, (uint8_t)TIM1_ICSELECTION_DIRECTTI, (uint8_t)ICFilter);
2720  0015 7b05          	ld	a,(OFST+5,sp)
2721  0017 88            	push	a
2722  0018 ae0001        	ldw	x,#1
2723  001b 7b03          	ld	a,(OFST+3,sp)
2724  001d 95            	ld	xh,a
2725  001e cd0000        	call	L3_TI1_Config
2727  0021 84            	pop	a
2728  0022               L3131:
2729                     ; 773     TIM1_SelectInputTrigger((TIM1_TS_TypeDef)TIM1_TIxExternalCLKSource);
2731  0022 7b01          	ld	a,(OFST+1,sp)
2732  0024 cd0000        	call	_TIM1_SelectInputTrigger
2734                     ; 776     TIM1->SMCR |= (uint8_t)(TIM1_SLAVEMODE_EXTERNAL1);
2736  0027 c65252        	ld	a,21074
2737  002a aa07          	or	a,#7
2738  002c c75252        	ld	21074,a
2739                     ; 777 }
2742  002f 85            	popw	x
2743  0030 81            	ret
2828                     ; 789 void TIM1_SelectInputTrigger(TIM1_TS_TypeDef TIM1_InputTriggerSource)
2828                     ; 790 {
2829                     .text:	section	.text,new
2830  0000               _TIM1_SelectInputTrigger:
2832  0000 88            	push	a
2833       00000000      OFST:	set	0
2836                     ; 792     assert_param(IS_TIM1_TRIGGER_SELECTION_OK(TIM1_InputTriggerSource));
2838                     ; 795     TIM1->SMCR = (uint8_t)((uint8_t)(TIM1->SMCR & (uint8_t)(~TIM1_SMCR_TS)) | (uint8_t)TIM1_InputTriggerSource);
2840  0001 c65252        	ld	a,21074
2841  0004 a48f          	and	a,#143
2842  0006 1a01          	or	a,(OFST+1,sp)
2843  0008 c75252        	ld	21074,a
2844                     ; 796 }
2847  000b 84            	pop	a
2848  000c 81            	ret
2884                     ; 806 void TIM1_UpdateDisableConfig(FunctionalState NewState)
2884                     ; 807 {
2885                     .text:	section	.text,new
2886  0000               _TIM1_UpdateDisableConfig:
2890                     ; 809     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
2892                     ; 812     if (NewState != DISABLE)
2894  0000 4d            	tnz	a
2895  0001 2706          	jreq	L1731
2896                     ; 814         TIM1->CR1 |= TIM1_CR1_UDIS;
2898  0003 72125250      	bset	21072,#1
2900  0007 2004          	jra	L3731
2901  0009               L1731:
2902                     ; 818         TIM1->CR1 &= (uint8_t)(~TIM1_CR1_UDIS);
2904  0009 72135250      	bres	21072,#1
2905  000d               L3731:
2906                     ; 820 }
2909  000d 81            	ret
2967                     ; 830 void TIM1_UpdateRequestConfig(TIM1_UpdateSource_TypeDef TIM1_UpdateSource)
2967                     ; 831 {
2968                     .text:	section	.text,new
2969  0000               _TIM1_UpdateRequestConfig:
2973                     ; 833     assert_param(IS_TIM1_UPDATE_SOURCE_OK(TIM1_UpdateSource));
2975                     ; 836     if (TIM1_UpdateSource != TIM1_UPDATESOURCE_GLOBAL)
2977  0000 4d            	tnz	a
2978  0001 2706          	jreq	L3241
2979                     ; 838         TIM1->CR1 |= TIM1_CR1_URS;
2981  0003 72145250      	bset	21072,#2
2983  0007 2004          	jra	L5241
2984  0009               L3241:
2985                     ; 842         TIM1->CR1 &= (uint8_t)(~TIM1_CR1_URS);
2987  0009 72155250      	bres	21072,#2
2988  000d               L5241:
2989                     ; 844 }
2992  000d 81            	ret
3028                     ; 853 void TIM1_SelectHallSensor(FunctionalState NewState)
3028                     ; 854 {
3029                     .text:	section	.text,new
3030  0000               _TIM1_SelectHallSensor:
3034                     ; 856     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
3036                     ; 859     if (NewState != DISABLE)
3038  0000 4d            	tnz	a
3039  0001 2706          	jreq	L5441
3040                     ; 861         TIM1->CR2 |= TIM1_CR2_TI1S;
3042  0003 721e5251      	bset	21073,#7
3044  0007 2004          	jra	L7441
3045  0009               L5441:
3046                     ; 865         TIM1->CR2 &= (uint8_t)(~TIM1_CR2_TI1S);
3048  0009 721f5251      	bres	21073,#7
3049  000d               L7441:
3050                     ; 867 }
3053  000d 81            	ret
3110                     ; 878 void TIM1_SelectOnePulseMode(TIM1_OPMode_TypeDef TIM1_OPMode)
3110                     ; 879 {
3111                     .text:	section	.text,new
3112  0000               _TIM1_SelectOnePulseMode:
3116                     ; 881     assert_param(IS_TIM1_OPM_MODE_OK(TIM1_OPMode));
3118                     ; 884     if (TIM1_OPMode != TIM1_OPMODE_REPETITIVE)
3120  0000 4d            	tnz	a
3121  0001 2706          	jreq	L7741
3122                     ; 886         TIM1->CR1 |= TIM1_CR1_OPM;
3124  0003 72165250      	bset	21072,#3
3126  0007 2004          	jra	L1051
3127  0009               L7741:
3128                     ; 890         TIM1->CR1 &= (uint8_t)(~TIM1_CR1_OPM);
3130  0009 72175250      	bres	21072,#3
3131  000d               L1051:
3132                     ; 893 }
3135  000d 81            	ret
3233                     ; 909 void TIM1_SelectOutputTrigger(TIM1_TRGOSource_TypeDef TIM1_TRGOSource)
3233                     ; 910 {
3234                     .text:	section	.text,new
3235  0000               _TIM1_SelectOutputTrigger:
3237  0000 88            	push	a
3238       00000000      OFST:	set	0
3241                     ; 912     assert_param(IS_TIM1_TRGO_SOURCE_OK(TIM1_TRGOSource));
3243                     ; 915     TIM1->CR2 = (uint8_t)((uint8_t)(TIM1->CR2 & (uint8_t)(~TIM1_CR2_MMS)) | 
3243                     ; 916                           (uint8_t) TIM1_TRGOSource);
3245  0001 c65251        	ld	a,21073
3246  0004 a48f          	and	a,#143
3247  0006 1a01          	or	a,(OFST+1,sp)
3248  0008 c75251        	ld	21073,a
3249                     ; 917 }
3252  000b 84            	pop	a
3253  000c 81            	ret
3327                     ; 929 void TIM1_SelectSlaveMode(TIM1_SlaveMode_TypeDef TIM1_SlaveMode)
3327                     ; 930 {
3328                     .text:	section	.text,new
3329  0000               _TIM1_SelectSlaveMode:
3331  0000 88            	push	a
3332       00000000      OFST:	set	0
3335                     ; 933     assert_param(IS_TIM1_SLAVE_MODE_OK(TIM1_SlaveMode));
3337                     ; 936     TIM1->SMCR = (uint8_t)((uint8_t)(TIM1->SMCR & (uint8_t)(~TIM1_SMCR_SMS)) |
3337                     ; 937                            (uint8_t)TIM1_SlaveMode);
3339  0001 c65252        	ld	a,21074
3340  0004 a4f8          	and	a,#248
3341  0006 1a01          	or	a,(OFST+1,sp)
3342  0008 c75252        	ld	21074,a
3343                     ; 939 }
3346  000b 84            	pop	a
3347  000c 81            	ret
3383                     ; 947 void TIM1_SelectMasterSlaveMode(FunctionalState NewState)
3383                     ; 948 {
3384                     .text:	section	.text,new
3385  0000               _TIM1_SelectMasterSlaveMode:
3389                     ; 950     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
3391                     ; 953     if (NewState != DISABLE)
3393  0000 4d            	tnz	a
3394  0001 2706          	jreq	L3161
3395                     ; 955         TIM1->SMCR |= TIM1_SMCR_MSM;
3397  0003 721e5252      	bset	21074,#7
3399  0007 2004          	jra	L5161
3400  0009               L3161:
3401                     ; 959         TIM1->SMCR &= (uint8_t)(~TIM1_SMCR_MSM);
3403  0009 721f5252      	bres	21074,#7
3404  000d               L5161:
3405                     ; 961 }
3408  000d 81            	ret
3494                     ; 983 void TIM1_EncoderInterfaceConfig(TIM1_EncoderMode_TypeDef TIM1_EncoderMode,
3494                     ; 984                                  TIM1_ICPolarity_TypeDef TIM1_IC1Polarity,
3494                     ; 985                                  TIM1_ICPolarity_TypeDef TIM1_IC2Polarity)
3494                     ; 986 {
3495                     .text:	section	.text,new
3496  0000               _TIM1_EncoderInterfaceConfig:
3498  0000 89            	pushw	x
3499       00000000      OFST:	set	0
3502                     ; 990     assert_param(IS_TIM1_ENCODER_MODE_OK(TIM1_EncoderMode));
3504                     ; 991     assert_param(IS_TIM1_IC_POLARITY_OK(TIM1_IC1Polarity));
3506                     ; 992     assert_param(IS_TIM1_IC_POLARITY_OK(TIM1_IC2Polarity));
3508                     ; 995     if (TIM1_IC1Polarity != TIM1_ICPOLARITY_RISING)
3510  0001 9f            	ld	a,xl
3511  0002 4d            	tnz	a
3512  0003 2706          	jreq	L7561
3513                     ; 997         TIM1->CCER1 |= TIM1_CCER1_CC1P;
3515  0005 7212525c      	bset	21084,#1
3517  0009 2004          	jra	L1661
3518  000b               L7561:
3519                     ; 1001         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1P);
3521  000b 7213525c      	bres	21084,#1
3522  000f               L1661:
3523                     ; 1004     if (TIM1_IC2Polarity != TIM1_ICPOLARITY_RISING)
3525  000f 0d05          	tnz	(OFST+5,sp)
3526  0011 2706          	jreq	L3661
3527                     ; 1006         TIM1->CCER1 |= TIM1_CCER1_CC2P;
3529  0013 721a525c      	bset	21084,#5
3531  0017 2004          	jra	L5661
3532  0019               L3661:
3533                     ; 1010         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2P);
3535  0019 721b525c      	bres	21084,#5
3536  001d               L5661:
3537                     ; 1013     TIM1->SMCR = (uint8_t)((uint8_t)(TIM1->SMCR & (uint8_t)(TIM1_SMCR_MSM | TIM1_SMCR_TS))
3537                     ; 1014                            | (uint8_t) TIM1_EncoderMode);
3539  001d c65252        	ld	a,21074
3540  0020 a4f0          	and	a,#240
3541  0022 1a01          	or	a,(OFST+1,sp)
3542  0024 c75252        	ld	21074,a
3543                     ; 1017     TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_CCxS)) 
3543                     ; 1018                             | (uint8_t) CCMR_TIxDirect_Set);
3545  0027 c65258        	ld	a,21080
3546  002a a4fc          	and	a,#252
3547  002c aa01          	or	a,#1
3548  002e c75258        	ld	21080,a
3549                     ; 1019     TIM1->CCMR2 = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~TIM1_CCMR_CCxS))
3549                     ; 1020                             | (uint8_t) CCMR_TIxDirect_Set);
3551  0031 c65259        	ld	a,21081
3552  0034 a4fc          	and	a,#252
3553  0036 aa01          	or	a,#1
3554  0038 c75259        	ld	21081,a
3555                     ; 1022 }
3558  003b 85            	popw	x
3559  003c 81            	ret
3626                     ; 1035 void TIM1_PrescalerConfig(uint16_t Prescaler,
3626                     ; 1036                           TIM1_PSCReloadMode_TypeDef TIM1_PSCReloadMode)
3626                     ; 1037 {
3627                     .text:	section	.text,new
3628  0000               _TIM1_PrescalerConfig:
3630  0000 89            	pushw	x
3631       00000000      OFST:	set	0
3634                     ; 1039     assert_param(IS_TIM1_PRESCALER_RELOAD_OK(TIM1_PSCReloadMode));
3636                     ; 1042     TIM1->PSCRH = (uint8_t)(Prescaler >> 8);
3638  0001 9e            	ld	a,xh
3639  0002 c75260        	ld	21088,a
3640                     ; 1043     TIM1->PSCRL = (uint8_t)(Prescaler);
3642  0005 9f            	ld	a,xl
3643  0006 c75261        	ld	21089,a
3644                     ; 1046     TIM1->EGR = (uint8_t)TIM1_PSCReloadMode;
3646  0009 7b05          	ld	a,(OFST+5,sp)
3647  000b c75257        	ld	21079,a
3648                     ; 1048 }
3651  000e 85            	popw	x
3652  000f 81            	ret
3688                     ; 1061 void TIM1_CounterModeConfig(TIM1_CounterMode_TypeDef TIM1_CounterMode)
3688                     ; 1062 {
3689                     .text:	section	.text,new
3690  0000               _TIM1_CounterModeConfig:
3692  0000 88            	push	a
3693       00000000      OFST:	set	0
3696                     ; 1064     assert_param(IS_TIM1_COUNTER_MODE_OK(TIM1_CounterMode));
3698                     ; 1068     TIM1->CR1 = (uint8_t)((uint8_t)(TIM1->CR1 & (uint8_t)((uint8_t)(~TIM1_CR1_CMS) & (uint8_t)(~TIM1_CR1_DIR)))
3698                     ; 1069                           | (uint8_t)TIM1_CounterMode);
3700  0001 c65250        	ld	a,21072
3701  0004 a48f          	and	a,#143
3702  0006 1a01          	or	a,(OFST+1,sp)
3703  0008 c75250        	ld	21072,a
3704                     ; 1070 }
3707  000b 84            	pop	a
3708  000c 81            	ret
3766                     ; 1081 void TIM1_ForcedOC1Config(TIM1_ForcedAction_TypeDef TIM1_ForcedAction)
3766                     ; 1082 {
3767                     .text:	section	.text,new
3768  0000               _TIM1_ForcedOC1Config:
3770  0000 88            	push	a
3771       00000000      OFST:	set	0
3774                     ; 1084     assert_param(IS_TIM1_FORCED_ACTION_OK(TIM1_ForcedAction));
3776                     ; 1087     TIM1->CCMR1 =  (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_OCM))|
3776                     ; 1088                              (uint8_t)TIM1_ForcedAction);
3778  0001 c65258        	ld	a,21080
3779  0004 a48f          	and	a,#143
3780  0006 1a01          	or	a,(OFST+1,sp)
3781  0008 c75258        	ld	21080,a
3782                     ; 1089 }
3785  000b 84            	pop	a
3786  000c 81            	ret
3822                     ; 1100 void TIM1_ForcedOC2Config(TIM1_ForcedAction_TypeDef TIM1_ForcedAction)
3822                     ; 1101 {
3823                     .text:	section	.text,new
3824  0000               _TIM1_ForcedOC2Config:
3826  0000 88            	push	a
3827       00000000      OFST:	set	0
3830                     ; 1103     assert_param(IS_TIM1_FORCED_ACTION_OK(TIM1_ForcedAction));
3832                     ; 1106     TIM1->CCMR2  =  (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~TIM1_CCMR_OCM))
3832                     ; 1107                               | (uint8_t)TIM1_ForcedAction);
3834  0001 c65259        	ld	a,21081
3835  0004 a48f          	and	a,#143
3836  0006 1a01          	or	a,(OFST+1,sp)
3837  0008 c75259        	ld	21081,a
3838                     ; 1108 }
3841  000b 84            	pop	a
3842  000c 81            	ret
3878                     ; 1120 void TIM1_ForcedOC3Config(TIM1_ForcedAction_TypeDef TIM1_ForcedAction)
3878                     ; 1121 {
3879                     .text:	section	.text,new
3880  0000               _TIM1_ForcedOC3Config:
3882  0000 88            	push	a
3883       00000000      OFST:	set	0
3886                     ; 1123     assert_param(IS_TIM1_FORCED_ACTION_OK(TIM1_ForcedAction));
3888                     ; 1126     TIM1->CCMR3  =  (uint8_t)((uint8_t)(TIM1->CCMR3 & (uint8_t)(~TIM1_CCMR_OCM))  
3888                     ; 1127                               | (uint8_t)TIM1_ForcedAction);
3890  0001 c6525a        	ld	a,21082
3891  0004 a48f          	and	a,#143
3892  0006 1a01          	or	a,(OFST+1,sp)
3893  0008 c7525a        	ld	21082,a
3894                     ; 1128 }
3897  000b 84            	pop	a
3898  000c 81            	ret
3934                     ; 1140 void TIM1_ForcedOC4Config(TIM1_ForcedAction_TypeDef TIM1_ForcedAction)
3934                     ; 1141 {
3935                     .text:	section	.text,new
3936  0000               _TIM1_ForcedOC4Config:
3938  0000 88            	push	a
3939       00000000      OFST:	set	0
3942                     ; 1143     assert_param(IS_TIM1_FORCED_ACTION_OK(TIM1_ForcedAction));
3944                     ; 1146     TIM1->CCMR4  =  (uint8_t)((uint8_t)(TIM1->CCMR4 & (uint8_t)(~TIM1_CCMR_OCM)) 
3944                     ; 1147                               | (uint8_t)TIM1_ForcedAction);
3946  0001 c6525b        	ld	a,21083
3947  0004 a48f          	and	a,#143
3948  0006 1a01          	or	a,(OFST+1,sp)
3949  0008 c7525b        	ld	21083,a
3950                     ; 1148 }
3953  000b 84            	pop	a
3954  000c 81            	ret
3990                     ; 1157 void TIM1_ARRPreloadConfig(FunctionalState NewState)
3990                     ; 1158 {
3991                     .text:	section	.text,new
3992  0000               _TIM1_ARRPreloadConfig:
3996                     ; 1160     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
3998                     ; 1163     if (NewState != DISABLE)
4000  0000 4d            	tnz	a
4001  0001 2706          	jreq	L5502
4002                     ; 1165         TIM1->CR1 |= TIM1_CR1_ARPE;
4004  0003 721e5250      	bset	21072,#7
4006  0007 2004          	jra	L7502
4007  0009               L5502:
4008                     ; 1169         TIM1->CR1 &= (uint8_t)(~TIM1_CR1_ARPE);
4010  0009 721f5250      	bres	21072,#7
4011  000d               L7502:
4012                     ; 1171 }
4015  000d 81            	ret
4050                     ; 1180 void TIM1_SelectCOM(FunctionalState NewState)
4050                     ; 1181 {
4051                     .text:	section	.text,new
4052  0000               _TIM1_SelectCOM:
4056                     ; 1183     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4058                     ; 1186     if (NewState != DISABLE)
4060  0000 4d            	tnz	a
4061  0001 2706          	jreq	L7702
4062                     ; 1188         TIM1->CR2 |= TIM1_CR2_COMS;
4064  0003 72145251      	bset	21073,#2
4066  0007 2004          	jra	L1012
4067  0009               L7702:
4068                     ; 1192         TIM1->CR2 &= (uint8_t)(~TIM1_CR2_COMS);
4070  0009 72155251      	bres	21073,#2
4071  000d               L1012:
4072                     ; 1194 }
4075  000d 81            	ret
4111                     ; 1202 void TIM1_CCPreloadControl(FunctionalState NewState)
4111                     ; 1203 {
4112                     .text:	section	.text,new
4113  0000               _TIM1_CCPreloadControl:
4117                     ; 1205     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4119                     ; 1208     if (NewState != DISABLE)
4121  0000 4d            	tnz	a
4122  0001 2706          	jreq	L1212
4123                     ; 1210         TIM1->CR2 |= TIM1_CR2_CCPC;
4125  0003 72105251      	bset	21073,#0
4127  0007 2004          	jra	L3212
4128  0009               L1212:
4129                     ; 1214         TIM1->CR2 &= (uint8_t)(~TIM1_CR2_CCPC);
4131  0009 72115251      	bres	21073,#0
4132  000d               L3212:
4133                     ; 1216 }
4136  000d 81            	ret
4172                     ; 1225 void TIM1_OC1PreloadConfig(FunctionalState NewState)
4172                     ; 1226 {
4173                     .text:	section	.text,new
4174  0000               _TIM1_OC1PreloadConfig:
4178                     ; 1228     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4180                     ; 1231     if (NewState != DISABLE)
4182  0000 4d            	tnz	a
4183  0001 2706          	jreq	L3412
4184                     ; 1233         TIM1->CCMR1 |= TIM1_CCMR_OCxPE;
4186  0003 72165258      	bset	21080,#3
4188  0007 2004          	jra	L5412
4189  0009               L3412:
4190                     ; 1237         TIM1->CCMR1 &= (uint8_t)(~TIM1_CCMR_OCxPE);
4192  0009 72175258      	bres	21080,#3
4193  000d               L5412:
4194                     ; 1239 }
4197  000d 81            	ret
4233                     ; 1248 void TIM1_OC2PreloadConfig(FunctionalState NewState)
4233                     ; 1249 {
4234                     .text:	section	.text,new
4235  0000               _TIM1_OC2PreloadConfig:
4239                     ; 1251     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4241                     ; 1254     if (NewState != DISABLE)
4243  0000 4d            	tnz	a
4244  0001 2706          	jreq	L5612
4245                     ; 1256         TIM1->CCMR2 |= TIM1_CCMR_OCxPE;
4247  0003 72165259      	bset	21081,#3
4249  0007 2004          	jra	L7612
4250  0009               L5612:
4251                     ; 1260         TIM1->CCMR2 &= (uint8_t)(~TIM1_CCMR_OCxPE);
4253  0009 72175259      	bres	21081,#3
4254  000d               L7612:
4255                     ; 1262 }
4258  000d 81            	ret
4294                     ; 1271 void TIM1_OC3PreloadConfig(FunctionalState NewState)
4294                     ; 1272 {
4295                     .text:	section	.text,new
4296  0000               _TIM1_OC3PreloadConfig:
4300                     ; 1274     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4302                     ; 1277     if (NewState != DISABLE)
4304  0000 4d            	tnz	a
4305  0001 2706          	jreq	L7022
4306                     ; 1279         TIM1->CCMR3 |= TIM1_CCMR_OCxPE;
4308  0003 7216525a      	bset	21082,#3
4310  0007 2004          	jra	L1122
4311  0009               L7022:
4312                     ; 1283         TIM1->CCMR3 &= (uint8_t)(~TIM1_CCMR_OCxPE);
4314  0009 7217525a      	bres	21082,#3
4315  000d               L1122:
4316                     ; 1285 }
4319  000d 81            	ret
4355                     ; 1295 void TIM1_OC4PreloadConfig(FunctionalState NewState)
4355                     ; 1296 {
4356                     .text:	section	.text,new
4357  0000               _TIM1_OC4PreloadConfig:
4361                     ; 1298     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4363                     ; 1301     if (NewState != DISABLE)
4365  0000 4d            	tnz	a
4366  0001 2706          	jreq	L1322
4367                     ; 1303         TIM1->CCMR4 |= TIM1_CCMR_OCxPE;
4369  0003 7216525b      	bset	21083,#3
4371  0007 2004          	jra	L3322
4372  0009               L1322:
4373                     ; 1307         TIM1->CCMR4 &= (uint8_t)(~TIM1_CCMR_OCxPE);
4375  0009 7217525b      	bres	21083,#3
4376  000d               L3322:
4377                     ; 1309 }
4380  000d 81            	ret
4415                     ; 1317 void TIM1_OC1FastConfig(FunctionalState NewState)
4415                     ; 1318 {
4416                     .text:	section	.text,new
4417  0000               _TIM1_OC1FastConfig:
4421                     ; 1320     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4423                     ; 1323     if (NewState != DISABLE)
4425  0000 4d            	tnz	a
4426  0001 2706          	jreq	L3522
4427                     ; 1325         TIM1->CCMR1 |= TIM1_CCMR_OCxFE;
4429  0003 72145258      	bset	21080,#2
4431  0007 2004          	jra	L5522
4432  0009               L3522:
4433                     ; 1329         TIM1->CCMR1 &= (uint8_t)(~TIM1_CCMR_OCxFE);
4435  0009 72155258      	bres	21080,#2
4436  000d               L5522:
4437                     ; 1331 }
4440  000d 81            	ret
4475                     ; 1341 void TIM1_OC2FastConfig(FunctionalState NewState)
4475                     ; 1342 {
4476                     .text:	section	.text,new
4477  0000               _TIM1_OC2FastConfig:
4481                     ; 1344     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4483                     ; 1347     if (NewState != DISABLE)
4485  0000 4d            	tnz	a
4486  0001 2706          	jreq	L5722
4487                     ; 1349         TIM1->CCMR2 |= TIM1_CCMR_OCxFE;
4489  0003 72145259      	bset	21081,#2
4491  0007 2004          	jra	L7722
4492  0009               L5722:
4493                     ; 1353         TIM1->CCMR2 &= (uint8_t)(~TIM1_CCMR_OCxFE);
4495  0009 72155259      	bres	21081,#2
4496  000d               L7722:
4497                     ; 1355 }
4500  000d 81            	ret
4535                     ; 1364 void TIM1_OC3FastConfig(FunctionalState NewState)
4535                     ; 1365 {
4536                     .text:	section	.text,new
4537  0000               _TIM1_OC3FastConfig:
4541                     ; 1367     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4543                     ; 1370     if (NewState != DISABLE)
4545  0000 4d            	tnz	a
4546  0001 2706          	jreq	L7132
4547                     ; 1372         TIM1->CCMR3 |= TIM1_CCMR_OCxFE;
4549  0003 7214525a      	bset	21082,#2
4551  0007 2004          	jra	L1232
4552  0009               L7132:
4553                     ; 1376         TIM1->CCMR3 &= (uint8_t)(~TIM1_CCMR_OCxFE);
4555  0009 7215525a      	bres	21082,#2
4556  000d               L1232:
4557                     ; 1378 }
4560  000d 81            	ret
4595                     ; 1387 void TIM1_OC4FastConfig(FunctionalState NewState)
4595                     ; 1388 {
4596                     .text:	section	.text,new
4597  0000               _TIM1_OC4FastConfig:
4601                     ; 1390     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
4603                     ; 1393     if (NewState != DISABLE)
4605  0000 4d            	tnz	a
4606  0001 2706          	jreq	L1432
4607                     ; 1395         TIM1->CCMR4 |= TIM1_CCMR_OCxFE;
4609  0003 7214525b      	bset	21083,#2
4611  0007 2004          	jra	L3432
4612  0009               L1432:
4613                     ; 1399         TIM1->CCMR4 &= (uint8_t)(~TIM1_CCMR_OCxFE);
4615  0009 7215525b      	bres	21083,#2
4616  000d               L3432:
4617                     ; 1401 }
4620  000d 81            	ret
4725                     ; 1418 void TIM1_GenerateEvent(TIM1_EventSource_TypeDef TIM1_EventSource)
4725                     ; 1419 {
4726                     .text:	section	.text,new
4727  0000               _TIM1_GenerateEvent:
4731                     ; 1421     assert_param(IS_TIM1_EVENT_SOURCE_OK(TIM1_EventSource));
4733                     ; 1424     TIM1->EGR = (uint8_t)TIM1_EventSource;
4735  0000 c75257        	ld	21079,a
4736                     ; 1425 }
4739  0003 81            	ret
4775                     ; 1436 void TIM1_OC1PolarityConfig(TIM1_OCPolarity_TypeDef TIM1_OCPolarity)
4775                     ; 1437 {
4776                     .text:	section	.text,new
4777  0000               _TIM1_OC1PolarityConfig:
4781                     ; 1439     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
4783                     ; 1442     if (TIM1_OCPolarity != TIM1_OCPOLARITY_HIGH)
4785  0000 4d            	tnz	a
4786  0001 2706          	jreq	L5242
4787                     ; 1444         TIM1->CCER1 |= TIM1_CCER1_CC1P;
4789  0003 7212525c      	bset	21084,#1
4791  0007 2004          	jra	L7242
4792  0009               L5242:
4793                     ; 1448         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1P);
4795  0009 7213525c      	bres	21084,#1
4796  000d               L7242:
4797                     ; 1450 }
4800  000d 81            	ret
4836                     ; 1461 void TIM1_OC1NPolarityConfig(TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity)
4836                     ; 1462 {
4837                     .text:	section	.text,new
4838  0000               _TIM1_OC1NPolarityConfig:
4842                     ; 1464     assert_param(IS_TIM1_OCN_POLARITY_OK(TIM1_OCNPolarity));
4844                     ; 1467     if (TIM1_OCNPolarity != TIM1_OCNPOLARITY_HIGH)
4846  0000 4d            	tnz	a
4847  0001 2706          	jreq	L7442
4848                     ; 1469         TIM1->CCER1 |= TIM1_CCER1_CC1NP;
4850  0003 7216525c      	bset	21084,#3
4852  0007 2004          	jra	L1542
4853  0009               L7442:
4854                     ; 1473         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1NP);
4856  0009 7217525c      	bres	21084,#3
4857  000d               L1542:
4858                     ; 1475 }
4861  000d 81            	ret
4897                     ; 1486 void TIM1_OC2PolarityConfig(TIM1_OCPolarity_TypeDef TIM1_OCPolarity)
4897                     ; 1487 {
4898                     .text:	section	.text,new
4899  0000               _TIM1_OC2PolarityConfig:
4903                     ; 1489     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
4905                     ; 1492     if (TIM1_OCPolarity != TIM1_OCPOLARITY_HIGH)
4907  0000 4d            	tnz	a
4908  0001 2706          	jreq	L1742
4909                     ; 1494         TIM1->CCER1 |= TIM1_CCER1_CC2P;
4911  0003 721a525c      	bset	21084,#5
4913  0007 2004          	jra	L3742
4914  0009               L1742:
4915                     ; 1498         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2P);
4917  0009 721b525c      	bres	21084,#5
4918  000d               L3742:
4919                     ; 1500 }
4922  000d 81            	ret
4958                     ; 1510 void TIM1_OC2NPolarityConfig(TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity)
4958                     ; 1511 {
4959                     .text:	section	.text,new
4960  0000               _TIM1_OC2NPolarityConfig:
4964                     ; 1513     assert_param(IS_TIM1_OCN_POLARITY_OK(TIM1_OCNPolarity));
4966                     ; 1516     if (TIM1_OCNPolarity != TIM1_OCNPOLARITY_HIGH)
4968  0000 4d            	tnz	a
4969  0001 2706          	jreq	L3152
4970                     ; 1518         TIM1->CCER1 |= TIM1_CCER1_CC2NP;
4972  0003 721e525c      	bset	21084,#7
4974  0007 2004          	jra	L5152
4975  0009               L3152:
4976                     ; 1522         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2NP);
4978  0009 721f525c      	bres	21084,#7
4979  000d               L5152:
4980                     ; 1524 }
4983  000d 81            	ret
5019                     ; 1535 void TIM1_OC3PolarityConfig(TIM1_OCPolarity_TypeDef TIM1_OCPolarity)
5019                     ; 1536 {
5020                     .text:	section	.text,new
5021  0000               _TIM1_OC3PolarityConfig:
5025                     ; 1538     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
5027                     ; 1541     if (TIM1_OCPolarity != TIM1_OCPOLARITY_HIGH)
5029  0000 4d            	tnz	a
5030  0001 2706          	jreq	L5352
5031                     ; 1543         TIM1->CCER2 |= TIM1_CCER2_CC3P;
5033  0003 7212525d      	bset	21085,#1
5035  0007 2004          	jra	L7352
5036  0009               L5352:
5037                     ; 1547         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3P);
5039  0009 7213525d      	bres	21085,#1
5040  000d               L7352:
5041                     ; 1549 }
5044  000d 81            	ret
5080                     ; 1560 void TIM1_OC3NPolarityConfig(TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity)
5080                     ; 1561 {
5081                     .text:	section	.text,new
5082  0000               _TIM1_OC3NPolarityConfig:
5086                     ; 1563     assert_param(IS_TIM1_OCN_POLARITY_OK(TIM1_OCNPolarity));
5088                     ; 1566     if (TIM1_OCNPolarity != TIM1_OCNPOLARITY_HIGH)
5090  0000 4d            	tnz	a
5091  0001 2706          	jreq	L7552
5092                     ; 1568         TIM1->CCER2 |= TIM1_CCER2_CC3NP;
5094  0003 7216525d      	bset	21085,#3
5096  0007 2004          	jra	L1652
5097  0009               L7552:
5098                     ; 1572         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3NP);
5100  0009 7217525d      	bres	21085,#3
5101  000d               L1652:
5102                     ; 1574 }
5105  000d 81            	ret
5141                     ; 1584 void TIM1_OC4PolarityConfig(TIM1_OCPolarity_TypeDef TIM1_OCPolarity)
5141                     ; 1585 {
5142                     .text:	section	.text,new
5143  0000               _TIM1_OC4PolarityConfig:
5147                     ; 1587     assert_param(IS_TIM1_OC_POLARITY_OK(TIM1_OCPolarity));
5149                     ; 1590     if (TIM1_OCPolarity != TIM1_OCPOLARITY_HIGH)
5151  0000 4d            	tnz	a
5152  0001 2706          	jreq	L1062
5153                     ; 1592         TIM1->CCER2 |= TIM1_CCER2_CC4P;
5155  0003 721a525d      	bset	21085,#5
5157  0007 2004          	jra	L3062
5158  0009               L1062:
5159                     ; 1596         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC4P);
5161  0009 721b525d      	bres	21085,#5
5162  000d               L3062:
5163                     ; 1598 }
5166  000d 81            	ret
5211                     ; 1613 void TIM1_CCxCmd(TIM1_Channel_TypeDef TIM1_Channel, FunctionalState NewState)
5211                     ; 1614 {
5212                     .text:	section	.text,new
5213  0000               _TIM1_CCxCmd:
5215  0000 89            	pushw	x
5216       00000000      OFST:	set	0
5219                     ; 1616     assert_param(IS_TIM1_CHANNEL_OK(TIM1_Channel));
5221                     ; 1617     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
5223                     ; 1619     if (TIM1_Channel == TIM1_CHANNEL_1)
5225  0001 9e            	ld	a,xh
5226  0002 4d            	tnz	a
5227  0003 2610          	jrne	L7262
5228                     ; 1622         if (NewState != DISABLE)
5230  0005 9f            	ld	a,xl
5231  0006 4d            	tnz	a
5232  0007 2706          	jreq	L1362
5233                     ; 1624             TIM1->CCER1 |= TIM1_CCER1_CC1E;
5235  0009 7210525c      	bset	21084,#0
5237  000d 2040          	jra	L5362
5238  000f               L1362:
5239                     ; 1628             TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1E);
5241  000f 7211525c      	bres	21084,#0
5242  0013 203a          	jra	L5362
5243  0015               L7262:
5244                     ; 1632     else if (TIM1_Channel == TIM1_CHANNEL_2)
5246  0015 7b01          	ld	a,(OFST+1,sp)
5247  0017 a101          	cp	a,#1
5248  0019 2610          	jrne	L7362
5249                     ; 1635         if (NewState != DISABLE)
5251  001b 0d02          	tnz	(OFST+2,sp)
5252  001d 2706          	jreq	L1462
5253                     ; 1637             TIM1->CCER1 |= TIM1_CCER1_CC2E;
5255  001f 7218525c      	bset	21084,#4
5257  0023 202a          	jra	L5362
5258  0025               L1462:
5259                     ; 1641             TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2E);
5261  0025 7219525c      	bres	21084,#4
5262  0029 2024          	jra	L5362
5263  002b               L7362:
5264                     ; 1644     else if (TIM1_Channel == TIM1_CHANNEL_3)
5266  002b 7b01          	ld	a,(OFST+1,sp)
5267  002d a102          	cp	a,#2
5268  002f 2610          	jrne	L7462
5269                     ; 1647         if (NewState != DISABLE)
5271  0031 0d02          	tnz	(OFST+2,sp)
5272  0033 2706          	jreq	L1562
5273                     ; 1649             TIM1->CCER2 |= TIM1_CCER2_CC3E;
5275  0035 7210525d      	bset	21085,#0
5277  0039 2014          	jra	L5362
5278  003b               L1562:
5279                     ; 1653             TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3E);
5281  003b 7211525d      	bres	21085,#0
5282  003f 200e          	jra	L5362
5283  0041               L7462:
5284                     ; 1659         if (NewState != DISABLE)
5286  0041 0d02          	tnz	(OFST+2,sp)
5287  0043 2706          	jreq	L7562
5288                     ; 1661             TIM1->CCER2 |= TIM1_CCER2_CC4E;
5290  0045 7218525d      	bset	21085,#4
5292  0049 2004          	jra	L5362
5293  004b               L7562:
5294                     ; 1665             TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC4E);
5296  004b 7219525d      	bres	21085,#4
5297  004f               L5362:
5298                     ; 1668 }
5301  004f 85            	popw	x
5302  0050 81            	ret
5347                     ; 1681 void TIM1_CCxNCmd(TIM1_Channel_TypeDef TIM1_Channel, FunctionalState NewState)
5347                     ; 1682 {
5348                     .text:	section	.text,new
5349  0000               _TIM1_CCxNCmd:
5351  0000 89            	pushw	x
5352       00000000      OFST:	set	0
5355                     ; 1684     assert_param(IS_TIM1_COMPLEMENTARY_CHANNEL_OK(TIM1_Channel));
5357                     ; 1685     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
5359                     ; 1687     if (TIM1_Channel == TIM1_CHANNEL_1)
5361  0001 9e            	ld	a,xh
5362  0002 4d            	tnz	a
5363  0003 2610          	jrne	L5072
5364                     ; 1690         if (NewState != DISABLE)
5366  0005 9f            	ld	a,xl
5367  0006 4d            	tnz	a
5368  0007 2706          	jreq	L7072
5369                     ; 1692             TIM1->CCER1 |= TIM1_CCER1_CC1NE;
5371  0009 7214525c      	bset	21084,#2
5373  000d 202a          	jra	L3172
5374  000f               L7072:
5375                     ; 1696             TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1NE);
5377  000f 7215525c      	bres	21084,#2
5378  0013 2024          	jra	L3172
5379  0015               L5072:
5380                     ; 1699     else if (TIM1_Channel == TIM1_CHANNEL_2)
5382  0015 7b01          	ld	a,(OFST+1,sp)
5383  0017 a101          	cp	a,#1
5384  0019 2610          	jrne	L5172
5385                     ; 1702         if (NewState != DISABLE)
5387  001b 0d02          	tnz	(OFST+2,sp)
5388  001d 2706          	jreq	L7172
5389                     ; 1704             TIM1->CCER1 |= TIM1_CCER1_CC2NE;
5391  001f 721c525c      	bset	21084,#6
5393  0023 2014          	jra	L3172
5394  0025               L7172:
5395                     ; 1708             TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2NE);
5397  0025 721d525c      	bres	21084,#6
5398  0029 200e          	jra	L3172
5399  002b               L5172:
5400                     ; 1714         if (NewState != DISABLE)
5402  002b 0d02          	tnz	(OFST+2,sp)
5403  002d 2706          	jreq	L5272
5404                     ; 1716             TIM1->CCER2 |= TIM1_CCER2_CC3NE;
5406  002f 7214525d      	bset	21085,#2
5408  0033 2004          	jra	L3172
5409  0035               L5272:
5410                     ; 1720             TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3NE);
5412  0035 7215525d      	bres	21085,#2
5413  0039               L3172:
5414                     ; 1723 }
5417  0039 85            	popw	x
5418  003a 81            	ret
5463                     ; 1747 void TIM1_SelectOCxM(TIM1_Channel_TypeDef TIM1_Channel, TIM1_OCMode_TypeDef TIM1_OCMode)
5463                     ; 1748 {
5464                     .text:	section	.text,new
5465  0000               _TIM1_SelectOCxM:
5467  0000 89            	pushw	x
5468       00000000      OFST:	set	0
5471                     ; 1750     assert_param(IS_TIM1_CHANNEL_OK(TIM1_Channel));
5473                     ; 1751     assert_param(IS_TIM1_OCM_OK(TIM1_OCMode));
5475                     ; 1753     if (TIM1_Channel == TIM1_CHANNEL_1)
5477  0001 9e            	ld	a,xh
5478  0002 4d            	tnz	a
5479  0003 2610          	jrne	L3572
5480                     ; 1756         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1E);
5482  0005 7211525c      	bres	21084,#0
5483                     ; 1759         TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_OCM)) 
5483                     ; 1760                                 | (uint8_t)TIM1_OCMode);
5485  0009 c65258        	ld	a,21080
5486  000c a48f          	and	a,#143
5487  000e 1a02          	or	a,(OFST+2,sp)
5488  0010 c75258        	ld	21080,a
5490  0013 203a          	jra	L5572
5491  0015               L3572:
5492                     ; 1762     else if (TIM1_Channel == TIM1_CHANNEL_2)
5494  0015 7b01          	ld	a,(OFST+1,sp)
5495  0017 a101          	cp	a,#1
5496  0019 2610          	jrne	L7572
5497                     ; 1765         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2E);
5499  001b 7219525c      	bres	21084,#4
5500                     ; 1768         TIM1->CCMR2 = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~TIM1_CCMR_OCM))
5500                     ; 1769                                 | (uint8_t)TIM1_OCMode);
5502  001f c65259        	ld	a,21081
5503  0022 a48f          	and	a,#143
5504  0024 1a02          	or	a,(OFST+2,sp)
5505  0026 c75259        	ld	21081,a
5507  0029 2024          	jra	L5572
5508  002b               L7572:
5509                     ; 1771     else if (TIM1_Channel == TIM1_CHANNEL_3)
5511  002b 7b01          	ld	a,(OFST+1,sp)
5512  002d a102          	cp	a,#2
5513  002f 2610          	jrne	L3672
5514                     ; 1774         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3E);
5516  0031 7211525d      	bres	21085,#0
5517                     ; 1777         TIM1->CCMR3 = (uint8_t)((uint8_t)(TIM1->CCMR3 & (uint8_t)(~TIM1_CCMR_OCM)) 
5517                     ; 1778                                 | (uint8_t)TIM1_OCMode);
5519  0035 c6525a        	ld	a,21082
5520  0038 a48f          	and	a,#143
5521  003a 1a02          	or	a,(OFST+2,sp)
5522  003c c7525a        	ld	21082,a
5524  003f 200e          	jra	L5572
5525  0041               L3672:
5526                     ; 1783         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC4E);
5528  0041 7219525d      	bres	21085,#4
5529                     ; 1786         TIM1->CCMR4 = (uint8_t)((uint8_t)(TIM1->CCMR4 & (uint8_t)(~TIM1_CCMR_OCM)) 
5529                     ; 1787                                 | (uint8_t)TIM1_OCMode);
5531  0045 c6525b        	ld	a,21083
5532  0048 a48f          	and	a,#143
5533  004a 1a02          	or	a,(OFST+2,sp)
5534  004c c7525b        	ld	21083,a
5535  004f               L5572:
5536                     ; 1789 }
5539  004f 85            	popw	x
5540  0050 81            	ret
5574                     ; 1798 void TIM1_SetCounter(uint16_t Counter)
5574                     ; 1799 {
5575                     .text:	section	.text,new
5576  0000               _TIM1_SetCounter:
5580                     ; 1801     TIM1->CNTRH = (uint8_t)(Counter >> 8);
5582  0000 9e            	ld	a,xh
5583  0001 c7525e        	ld	21086,a
5584                     ; 1802     TIM1->CNTRL = (uint8_t)(Counter);
5586  0004 9f            	ld	a,xl
5587  0005 c7525f        	ld	21087,a
5588                     ; 1804 }
5591  0008 81            	ret
5625                     ; 1813 void TIM1_SetAutoreload(uint16_t Autoreload)
5625                     ; 1814 {
5626                     .text:	section	.text,new
5627  0000               _TIM1_SetAutoreload:
5631                     ; 1817     TIM1->ARRH = (uint8_t)(Autoreload >> 8);
5633  0000 9e            	ld	a,xh
5634  0001 c75262        	ld	21090,a
5635                     ; 1818     TIM1->ARRL = (uint8_t)(Autoreload);
5637  0004 9f            	ld	a,xl
5638  0005 c75263        	ld	21091,a
5639                     ; 1820 }
5642  0008 81            	ret
5676                     ; 1829 void TIM1_SetCompare1(uint16_t Compare1)
5676                     ; 1830 {
5677                     .text:	section	.text,new
5678  0000               _TIM1_SetCompare1:
5682                     ; 1832     TIM1->CCR1H = (uint8_t)(Compare1 >> 8);
5684  0000 9e            	ld	a,xh
5685  0001 c75265        	ld	21093,a
5686                     ; 1833     TIM1->CCR1L = (uint8_t)(Compare1);
5688  0004 9f            	ld	a,xl
5689  0005 c75266        	ld	21094,a
5690                     ; 1835 }
5693  0008 81            	ret
5727                     ; 1844 void TIM1_SetCompare2(uint16_t Compare2)
5727                     ; 1845 {
5728                     .text:	section	.text,new
5729  0000               _TIM1_SetCompare2:
5733                     ; 1847     TIM1->CCR2H = (uint8_t)(Compare2 >> 8);
5735  0000 9e            	ld	a,xh
5736  0001 c75267        	ld	21095,a
5737                     ; 1848     TIM1->CCR2L = (uint8_t)(Compare2);
5739  0004 9f            	ld	a,xl
5740  0005 c75268        	ld	21096,a
5741                     ; 1850 }
5744  0008 81            	ret
5778                     ; 1859 void TIM1_SetCompare3(uint16_t Compare3)
5778                     ; 1860 {
5779                     .text:	section	.text,new
5780  0000               _TIM1_SetCompare3:
5784                     ; 1862     TIM1->CCR3H = (uint8_t)(Compare3 >> 8);
5786  0000 9e            	ld	a,xh
5787  0001 c75269        	ld	21097,a
5788                     ; 1863     TIM1->CCR3L = (uint8_t)(Compare3);
5790  0004 9f            	ld	a,xl
5791  0005 c7526a        	ld	21098,a
5792                     ; 1865 }
5795  0008 81            	ret
5829                     ; 1874 void TIM1_SetCompare4(uint16_t Compare4)
5829                     ; 1875 {
5830                     .text:	section	.text,new
5831  0000               _TIM1_SetCompare4:
5835                     ; 1877     TIM1->CCR4H = (uint8_t)(Compare4 >> 8);
5837  0000 9e            	ld	a,xh
5838  0001 c7526b        	ld	21099,a
5839                     ; 1878     TIM1->CCR4L = (uint8_t)(Compare4);
5841  0004 9f            	ld	a,xl
5842  0005 c7526c        	ld	21100,a
5843                     ; 1879 }
5846  0008 81            	ret
5882                     ; 1892 void TIM1_SetIC1Prescaler(TIM1_ICPSC_TypeDef TIM1_IC1Prescaler)
5882                     ; 1893 {
5883                     .text:	section	.text,new
5884  0000               _TIM1_SetIC1Prescaler:
5886  0000 88            	push	a
5887       00000000      OFST:	set	0
5890                     ; 1895     assert_param(IS_TIM1_IC_PRESCALER_OK(TIM1_IC1Prescaler));
5892                     ; 1898     TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_ICxPSC)) 
5892                     ; 1899                             | (uint8_t)TIM1_IC1Prescaler);
5894  0001 c65258        	ld	a,21080
5895  0004 a4f3          	and	a,#243
5896  0006 1a01          	or	a,(OFST+1,sp)
5897  0008 c75258        	ld	21080,a
5898                     ; 1901 }
5901  000b 84            	pop	a
5902  000c 81            	ret
5938                     ; 1913 void TIM1_SetIC2Prescaler(TIM1_ICPSC_TypeDef TIM1_IC2Prescaler)
5938                     ; 1914 {
5939                     .text:	section	.text,new
5940  0000               _TIM1_SetIC2Prescaler:
5942  0000 88            	push	a
5943       00000000      OFST:	set	0
5946                     ; 1917     assert_param(IS_TIM1_IC_PRESCALER_OK(TIM1_IC2Prescaler));
5948                     ; 1920     TIM1->CCMR2 = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~TIM1_CCMR_ICxPSC))
5948                     ; 1921                             | (uint8_t)TIM1_IC2Prescaler);
5950  0001 c65259        	ld	a,21081
5951  0004 a4f3          	and	a,#243
5952  0006 1a01          	or	a,(OFST+1,sp)
5953  0008 c75259        	ld	21081,a
5954                     ; 1922 }
5957  000b 84            	pop	a
5958  000c 81            	ret
5994                     ; 1935 void TIM1_SetIC3Prescaler(TIM1_ICPSC_TypeDef TIM1_IC3Prescaler)
5994                     ; 1936 {
5995                     .text:	section	.text,new
5996  0000               _TIM1_SetIC3Prescaler:
5998  0000 88            	push	a
5999       00000000      OFST:	set	0
6002                     ; 1939     assert_param(IS_TIM1_IC_PRESCALER_OK(TIM1_IC3Prescaler));
6004                     ; 1942     TIM1->CCMR3 = (uint8_t)((uint8_t)(TIM1->CCMR3 & (uint8_t)(~TIM1_CCMR_ICxPSC)) | 
6004                     ; 1943                             (uint8_t)TIM1_IC3Prescaler);
6006  0001 c6525a        	ld	a,21082
6007  0004 a4f3          	and	a,#243
6008  0006 1a01          	or	a,(OFST+1,sp)
6009  0008 c7525a        	ld	21082,a
6010                     ; 1944 }
6013  000b 84            	pop	a
6014  000c 81            	ret
6050                     ; 1956 void TIM1_SetIC4Prescaler(TIM1_ICPSC_TypeDef TIM1_IC4Prescaler)
6050                     ; 1957 {
6051                     .text:	section	.text,new
6052  0000               _TIM1_SetIC4Prescaler:
6054  0000 88            	push	a
6055       00000000      OFST:	set	0
6058                     ; 1960     assert_param(IS_TIM1_IC_PRESCALER_OK(TIM1_IC4Prescaler));
6060                     ; 1963     TIM1->CCMR4 = (uint8_t)((uint8_t)(TIM1->CCMR4 & (uint8_t)(~TIM1_CCMR_ICxPSC)) |
6060                     ; 1964                             (uint8_t)TIM1_IC4Prescaler);
6062  0001 c6525b        	ld	a,21083
6063  0004 a4f3          	and	a,#243
6064  0006 1a01          	or	a,(OFST+1,sp)
6065  0008 c7525b        	ld	21083,a
6066                     ; 1965 }
6069  000b 84            	pop	a
6070  000c 81            	ret
6122                     ; 1972 uint16_t TIM1_GetCapture1(void)
6122                     ; 1973 {
6123                     .text:	section	.text,new
6124  0000               _TIM1_GetCapture1:
6126  0000 5204          	subw	sp,#4
6127       00000004      OFST:	set	4
6130                     ; 1976     uint16_t tmpccr1 = 0;
6132  0002 1e03          	ldw	x,(OFST-1,sp)
6133                     ; 1977     uint8_t tmpccr1l=0, tmpccr1h=0;
6135  0004 7b01          	ld	a,(OFST-3,sp)
6136  0006 97            	ld	xl,a
6139  0007 7b02          	ld	a,(OFST-2,sp)
6140  0009 97            	ld	xl,a
6141                     ; 1979     tmpccr1h = TIM1->CCR1H;
6143  000a c65265        	ld	a,21093
6144  000d 6b02          	ld	(OFST-2,sp),a
6145                     ; 1980     tmpccr1l = TIM1->CCR1L;
6147  000f c65266        	ld	a,21094
6148  0012 6b01          	ld	(OFST-3,sp),a
6149                     ; 1982     tmpccr1 = (uint16_t)(tmpccr1l);
6151  0014 7b01          	ld	a,(OFST-3,sp)
6152  0016 5f            	clrw	x
6153  0017 97            	ld	xl,a
6154  0018 1f03          	ldw	(OFST-1,sp),x
6155                     ; 1983     tmpccr1 |= (uint16_t)((uint16_t)tmpccr1h << 8);
6157  001a 7b02          	ld	a,(OFST-2,sp)
6158  001c 5f            	clrw	x
6159  001d 97            	ld	xl,a
6160  001e 4f            	clr	a
6161  001f 02            	rlwa	x,a
6162  0020 01            	rrwa	x,a
6163  0021 1a04          	or	a,(OFST+0,sp)
6164  0023 01            	rrwa	x,a
6165  0024 1a03          	or	a,(OFST-1,sp)
6166  0026 01            	rrwa	x,a
6167  0027 1f03          	ldw	(OFST-1,sp),x
6168                     ; 1985     return (uint16_t)tmpccr1;
6170  0029 1e03          	ldw	x,(OFST-1,sp)
6173  002b 5b04          	addw	sp,#4
6174  002d 81            	ret
6226                     ; 1993 uint16_t TIM1_GetCapture2(void)
6226                     ; 1994 {
6227                     .text:	section	.text,new
6228  0000               _TIM1_GetCapture2:
6230  0000 5204          	subw	sp,#4
6231       00000004      OFST:	set	4
6234                     ; 1997     uint16_t tmpccr2 = 0;
6236  0002 1e03          	ldw	x,(OFST-1,sp)
6237                     ; 1998     uint8_t tmpccr2l=0, tmpccr2h=0;
6239  0004 7b01          	ld	a,(OFST-3,sp)
6240  0006 97            	ld	xl,a
6243  0007 7b02          	ld	a,(OFST-2,sp)
6244  0009 97            	ld	xl,a
6245                     ; 2000     tmpccr2h = TIM1->CCR2H;
6247  000a c65267        	ld	a,21095
6248  000d 6b02          	ld	(OFST-2,sp),a
6249                     ; 2001     tmpccr2l = TIM1->CCR2L;
6251  000f c65268        	ld	a,21096
6252  0012 6b01          	ld	(OFST-3,sp),a
6253                     ; 2003     tmpccr2 = (uint16_t)(tmpccr2l);
6255  0014 7b01          	ld	a,(OFST-3,sp)
6256  0016 5f            	clrw	x
6257  0017 97            	ld	xl,a
6258  0018 1f03          	ldw	(OFST-1,sp),x
6259                     ; 2004     tmpccr2 |= (uint16_t)((uint16_t)tmpccr2h << 8);
6261  001a 7b02          	ld	a,(OFST-2,sp)
6262  001c 5f            	clrw	x
6263  001d 97            	ld	xl,a
6264  001e 4f            	clr	a
6265  001f 02            	rlwa	x,a
6266  0020 01            	rrwa	x,a
6267  0021 1a04          	or	a,(OFST+0,sp)
6268  0023 01            	rrwa	x,a
6269  0024 1a03          	or	a,(OFST-1,sp)
6270  0026 01            	rrwa	x,a
6271  0027 1f03          	ldw	(OFST-1,sp),x
6272                     ; 2006     return (uint16_t)tmpccr2;
6274  0029 1e03          	ldw	x,(OFST-1,sp)
6277  002b 5b04          	addw	sp,#4
6278  002d 81            	ret
6330                     ; 2014 uint16_t TIM1_GetCapture3(void)
6330                     ; 2015 {
6331                     .text:	section	.text,new
6332  0000               _TIM1_GetCapture3:
6334  0000 5204          	subw	sp,#4
6335       00000004      OFST:	set	4
6338                     ; 2017     uint16_t tmpccr3 = 0;
6340  0002 1e03          	ldw	x,(OFST-1,sp)
6341                     ; 2018     uint8_t tmpccr3l=0, tmpccr3h=0;
6343  0004 7b01          	ld	a,(OFST-3,sp)
6344  0006 97            	ld	xl,a
6347  0007 7b02          	ld	a,(OFST-2,sp)
6348  0009 97            	ld	xl,a
6349                     ; 2020     tmpccr3h = TIM1->CCR3H;
6351  000a c65269        	ld	a,21097
6352  000d 6b02          	ld	(OFST-2,sp),a
6353                     ; 2021     tmpccr3l = TIM1->CCR3L;
6355  000f c6526a        	ld	a,21098
6356  0012 6b01          	ld	(OFST-3,sp),a
6357                     ; 2023     tmpccr3 = (uint16_t)(tmpccr3l);
6359  0014 7b01          	ld	a,(OFST-3,sp)
6360  0016 5f            	clrw	x
6361  0017 97            	ld	xl,a
6362  0018 1f03          	ldw	(OFST-1,sp),x
6363                     ; 2024     tmpccr3 |= (uint16_t)((uint16_t)tmpccr3h << 8);
6365  001a 7b02          	ld	a,(OFST-2,sp)
6366  001c 5f            	clrw	x
6367  001d 97            	ld	xl,a
6368  001e 4f            	clr	a
6369  001f 02            	rlwa	x,a
6370  0020 01            	rrwa	x,a
6371  0021 1a04          	or	a,(OFST+0,sp)
6372  0023 01            	rrwa	x,a
6373  0024 1a03          	or	a,(OFST-1,sp)
6374  0026 01            	rrwa	x,a
6375  0027 1f03          	ldw	(OFST-1,sp),x
6376                     ; 2026     return (uint16_t)tmpccr3;
6378  0029 1e03          	ldw	x,(OFST-1,sp)
6381  002b 5b04          	addw	sp,#4
6382  002d 81            	ret
6434                     ; 2034 uint16_t TIM1_GetCapture4(void)
6434                     ; 2035 {
6435                     .text:	section	.text,new
6436  0000               _TIM1_GetCapture4:
6438  0000 5204          	subw	sp,#4
6439       00000004      OFST:	set	4
6442                     ; 2037     uint16_t tmpccr4 = 0;
6444  0002 1e03          	ldw	x,(OFST-1,sp)
6445                     ; 2038     uint8_t tmpccr4l=0, tmpccr4h=0;
6447  0004 7b01          	ld	a,(OFST-3,sp)
6448  0006 97            	ld	xl,a
6451  0007 7b02          	ld	a,(OFST-2,sp)
6452  0009 97            	ld	xl,a
6453                     ; 2040     tmpccr4h = TIM1->CCR4H;
6455  000a c6526b        	ld	a,21099
6456  000d 6b02          	ld	(OFST-2,sp),a
6457                     ; 2041     tmpccr4l = TIM1->CCR4L;
6459  000f c6526c        	ld	a,21100
6460  0012 6b01          	ld	(OFST-3,sp),a
6461                     ; 2043     tmpccr4 = (uint16_t)(tmpccr4l);
6463  0014 7b01          	ld	a,(OFST-3,sp)
6464  0016 5f            	clrw	x
6465  0017 97            	ld	xl,a
6466  0018 1f03          	ldw	(OFST-1,sp),x
6467                     ; 2044     tmpccr4 |= (uint16_t)((uint16_t)tmpccr4h << 8);
6469  001a 7b02          	ld	a,(OFST-2,sp)
6470  001c 5f            	clrw	x
6471  001d 97            	ld	xl,a
6472  001e 4f            	clr	a
6473  001f 02            	rlwa	x,a
6474  0020 01            	rrwa	x,a
6475  0021 1a04          	or	a,(OFST+0,sp)
6476  0023 01            	rrwa	x,a
6477  0024 1a03          	or	a,(OFST-1,sp)
6478  0026 01            	rrwa	x,a
6479  0027 1f03          	ldw	(OFST-1,sp),x
6480                     ; 2046     return (uint16_t)tmpccr4;
6482  0029 1e03          	ldw	x,(OFST-1,sp)
6485  002b 5b04          	addw	sp,#4
6486  002d 81            	ret
6520                     ; 2054 uint16_t TIM1_GetCounter(void)
6520                     ; 2055 {
6521                     .text:	section	.text,new
6522  0000               _TIM1_GetCounter:
6524  0000 89            	pushw	x
6525       00000002      OFST:	set	2
6528                     ; 2056   uint16_t tmpcntr = 0;
6530  0001 5f            	clrw	x
6531  0002 1f01          	ldw	(OFST-1,sp),x
6532                     ; 2058   tmpcntr = ((uint16_t)TIM1->CNTRH << 8);
6534  0004 c6525e        	ld	a,21086
6535  0007 5f            	clrw	x
6536  0008 97            	ld	xl,a
6537  0009 4f            	clr	a
6538  000a 02            	rlwa	x,a
6539  000b 1f01          	ldw	(OFST-1,sp),x
6540                     ; 2061     return (uint16_t)(tmpcntr | (uint16_t)(TIM1->CNTRL));
6542  000d c6525f        	ld	a,21087
6543  0010 5f            	clrw	x
6544  0011 97            	ld	xl,a
6545  0012 01            	rrwa	x,a
6546  0013 1a02          	or	a,(OFST+0,sp)
6547  0015 01            	rrwa	x,a
6548  0016 1a01          	or	a,(OFST-1,sp)
6549  0018 01            	rrwa	x,a
6552  0019 5b02          	addw	sp,#2
6553  001b 81            	ret
6587                     ; 2069 uint16_t TIM1_GetPrescaler(void)
6587                     ; 2070 {
6588                     .text:	section	.text,new
6589  0000               _TIM1_GetPrescaler:
6591  0000 89            	pushw	x
6592       00000002      OFST:	set	2
6595                     ; 2071    uint16_t temp = 0;
6597  0001 5f            	clrw	x
6598  0002 1f01          	ldw	(OFST-1,sp),x
6599                     ; 2073    temp = ((uint16_t)TIM1->PSCRH << 8);
6601  0004 c65260        	ld	a,21088
6602  0007 5f            	clrw	x
6603  0008 97            	ld	xl,a
6604  0009 4f            	clr	a
6605  000a 02            	rlwa	x,a
6606  000b 1f01          	ldw	(OFST-1,sp),x
6607                     ; 2076     return (uint16_t)( temp | (uint16_t)(TIM1->PSCRL));
6609  000d c65261        	ld	a,21089
6610  0010 5f            	clrw	x
6611  0011 97            	ld	xl,a
6612  0012 01            	rrwa	x,a
6613  0013 1a02          	or	a,(OFST+0,sp)
6614  0015 01            	rrwa	x,a
6615  0016 1a01          	or	a,(OFST-1,sp)
6616  0018 01            	rrwa	x,a
6619  0019 5b02          	addw	sp,#2
6620  001b 81            	ret
6794                     ; 2097 FlagStatus TIM1_GetFlagStatus(TIM1_FLAG_TypeDef TIM1_FLAG)
6794                     ; 2098 {
6795                     .text:	section	.text,new
6796  0000               _TIM1_GetFlagStatus:
6798  0000 89            	pushw	x
6799  0001 89            	pushw	x
6800       00000002      OFST:	set	2
6803                     ; 2099     FlagStatus bitstatus = RESET;
6805  0002 7b02          	ld	a,(OFST+0,sp)
6806  0004 97            	ld	xl,a
6807                     ; 2100     uint8_t tim1_flag_l = 0, tim1_flag_h = 0;
6809  0005 7b01          	ld	a,(OFST-1,sp)
6810  0007 97            	ld	xl,a
6813  0008 7b02          	ld	a,(OFST+0,sp)
6814  000a 97            	ld	xl,a
6815                     ; 2103     assert_param(IS_TIM1_GET_FLAG_OK(TIM1_FLAG));
6817                     ; 2105     tim1_flag_l = (uint8_t)(TIM1->SR1 & (uint8_t)TIM1_FLAG);
6819  000b c65255        	ld	a,21077
6820  000e 1404          	and	a,(OFST+2,sp)
6821  0010 6b01          	ld	(OFST-1,sp),a
6822                     ; 2106     tim1_flag_h = (uint8_t)((uint16_t)TIM1_FLAG >> 8);
6824  0012 7b03          	ld	a,(OFST+1,sp)
6825  0014 6b02          	ld	(OFST+0,sp),a
6826                     ; 2108     if ((tim1_flag_l | (uint8_t)(TIM1->SR2 & tim1_flag_h)) != 0)
6828  0016 c65256        	ld	a,21078
6829  0019 1402          	and	a,(OFST+0,sp)
6830  001b 1a01          	or	a,(OFST-1,sp)
6831  001d 2706          	jreq	L5643
6832                     ; 2110         bitstatus = SET;
6834  001f a601          	ld	a,#1
6835  0021 6b02          	ld	(OFST+0,sp),a
6837  0023 2002          	jra	L7643
6838  0025               L5643:
6839                     ; 2114         bitstatus = RESET;
6841  0025 0f02          	clr	(OFST+0,sp)
6842  0027               L7643:
6843                     ; 2116     return (FlagStatus)(bitstatus);
6845  0027 7b02          	ld	a,(OFST+0,sp)
6848  0029 5b04          	addw	sp,#4
6849  002b 81            	ret
6884                     ; 2137 void TIM1_ClearFlag(TIM1_FLAG_TypeDef TIM1_FLAG)
6884                     ; 2138 {
6885                     .text:	section	.text,new
6886  0000               _TIM1_ClearFlag:
6888  0000 89            	pushw	x
6889       00000000      OFST:	set	0
6892                     ; 2140     assert_param(IS_TIM1_CLEAR_FLAG_OK(TIM1_FLAG));
6894                     ; 2143     TIM1->SR1 = (uint8_t)(~(uint8_t)(TIM1_FLAG));
6896  0001 9f            	ld	a,xl
6897  0002 43            	cpl	a
6898  0003 c75255        	ld	21077,a
6899                     ; 2144     TIM1->SR2 = (uint8_t)((uint8_t)(~((uint8_t)((uint16_t)TIM1_FLAG >> 8))) & 
6899                     ; 2145                           (uint8_t)0x1E);
6901  0006 7b01          	ld	a,(OFST+1,sp)
6902  0008 43            	cpl	a
6903  0009 a41e          	and	a,#30
6904  000b c75256        	ld	21078,a
6905                     ; 2146 }
6908  000e 85            	popw	x
6909  000f 81            	ret
6973                     ; 2162 ITStatus TIM1_GetITStatus(TIM1_IT_TypeDef TIM1_IT)
6973                     ; 2163 {
6974                     .text:	section	.text,new
6975  0000               _TIM1_GetITStatus:
6977  0000 88            	push	a
6978  0001 89            	pushw	x
6979       00000002      OFST:	set	2
6982                     ; 2164     ITStatus bitstatus = RESET;
6984  0002 7b02          	ld	a,(OFST+0,sp)
6985  0004 97            	ld	xl,a
6986                     ; 2165     uint8_t TIM1_itStatus = 0, TIM1_itEnable = 0;
6988  0005 7b01          	ld	a,(OFST-1,sp)
6989  0007 97            	ld	xl,a
6992  0008 7b02          	ld	a,(OFST+0,sp)
6993  000a 97            	ld	xl,a
6994                     ; 2168     assert_param(IS_TIM1_GET_IT_OK(TIM1_IT));
6996                     ; 2170     TIM1_itStatus = (uint8_t)(TIM1->SR1 & (uint8_t)TIM1_IT);
6998  000b c65255        	ld	a,21077
6999  000e 1403          	and	a,(OFST+1,sp)
7000  0010 6b01          	ld	(OFST-1,sp),a
7001                     ; 2172     TIM1_itEnable = (uint8_t)(TIM1->IER & (uint8_t)TIM1_IT);
7003  0012 c65254        	ld	a,21076
7004  0015 1403          	and	a,(OFST+1,sp)
7005  0017 6b02          	ld	(OFST+0,sp),a
7006                     ; 2174     if ((TIM1_itStatus != (uint8_t)RESET ) && (TIM1_itEnable != (uint8_t)RESET ))
7008  0019 0d01          	tnz	(OFST-1,sp)
7009  001b 270a          	jreq	L1453
7011  001d 0d02          	tnz	(OFST+0,sp)
7012  001f 2706          	jreq	L1453
7013                     ; 2176         bitstatus = SET;
7015  0021 a601          	ld	a,#1
7016  0023 6b02          	ld	(OFST+0,sp),a
7018  0025 2002          	jra	L3453
7019  0027               L1453:
7020                     ; 2180         bitstatus = RESET;
7022  0027 0f02          	clr	(OFST+0,sp)
7023  0029               L3453:
7024                     ; 2182     return (ITStatus)(bitstatus);
7026  0029 7b02          	ld	a,(OFST+0,sp)
7029  002b 5b03          	addw	sp,#3
7030  002d 81            	ret
7066                     ; 2199 void TIM1_ClearITPendingBit(TIM1_IT_TypeDef TIM1_IT)
7066                     ; 2200 {
7067                     .text:	section	.text,new
7068  0000               _TIM1_ClearITPendingBit:
7072                     ; 2202     assert_param(IS_TIM1_IT_OK(TIM1_IT));
7074                     ; 2205     TIM1->SR1 = (uint8_t)(~(uint8_t)TIM1_IT);
7076  0000 43            	cpl	a
7077  0001 c75255        	ld	21077,a
7078                     ; 2206 }
7081  0004 81            	ret
7133                     ; 2224 static void TI1_Config(uint8_t TIM1_ICPolarity,
7133                     ; 2225                        uint8_t TIM1_ICSelection,
7133                     ; 2226                        uint8_t TIM1_ICFilter)
7133                     ; 2227 {
7134                     .text:	section	.text,new
7135  0000               L3_TI1_Config:
7137  0000 89            	pushw	x
7138  0001 88            	push	a
7139       00000001      OFST:	set	1
7142                     ; 2230     TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1E);
7144  0002 7211525c      	bres	21084,#0
7145                     ; 2233     TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~(uint8_t)( TIM1_CCMR_CCxS | TIM1_CCMR_ICxF ))) | 
7145                     ; 2234                             (uint8_t)(( (TIM1_ICSelection)) | ((uint8_t)( TIM1_ICFilter << 4))));
7147  0006 7b06          	ld	a,(OFST+5,sp)
7148  0008 97            	ld	xl,a
7149  0009 a610          	ld	a,#16
7150  000b 42            	mul	x,a
7151  000c 9f            	ld	a,xl
7152  000d 1a03          	or	a,(OFST+2,sp)
7153  000f 6b01          	ld	(OFST+0,sp),a
7154  0011 c65258        	ld	a,21080
7155  0014 a40c          	and	a,#12
7156  0016 1a01          	or	a,(OFST+0,sp)
7157  0018 c75258        	ld	21080,a
7158                     ; 2237     if (TIM1_ICPolarity != TIM1_ICPOLARITY_RISING)
7160  001b 0d02          	tnz	(OFST+1,sp)
7161  001d 2706          	jreq	L1163
7162                     ; 2239         TIM1->CCER1 |= TIM1_CCER1_CC1P;
7164  001f 7212525c      	bset	21084,#1
7166  0023 2004          	jra	L3163
7167  0025               L1163:
7168                     ; 2243         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1P);
7170  0025 7213525c      	bres	21084,#1
7171  0029               L3163:
7172                     ; 2247     TIM1->CCER1 |=  TIM1_CCER1_CC1E;
7174  0029 7210525c      	bset	21084,#0
7175                     ; 2248 }
7178  002d 5b03          	addw	sp,#3
7179  002f 81            	ret
7231                     ; 2266 static void TI2_Config(uint8_t TIM1_ICPolarity,
7231                     ; 2267                        uint8_t TIM1_ICSelection,
7231                     ; 2268                        uint8_t TIM1_ICFilter)
7231                     ; 2269 {
7232                     .text:	section	.text,new
7233  0000               L5_TI2_Config:
7235  0000 89            	pushw	x
7236  0001 88            	push	a
7237       00000001      OFST:	set	1
7240                     ; 2271     TIM1->CCER1 &=  (uint8_t)(~TIM1_CCER1_CC2E);
7242  0002 7219525c      	bres	21084,#4
7243                     ; 2274     TIM1->CCMR2  = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~(uint8_t)( TIM1_CCMR_CCxS | TIM1_CCMR_ICxF ))) 
7243                     ; 2275                             | (uint8_t)(( (TIM1_ICSelection)) | ((uint8_t)( TIM1_ICFilter << 4))));
7245  0006 7b06          	ld	a,(OFST+5,sp)
7246  0008 97            	ld	xl,a
7247  0009 a610          	ld	a,#16
7248  000b 42            	mul	x,a
7249  000c 9f            	ld	a,xl
7250  000d 1a03          	or	a,(OFST+2,sp)
7251  000f 6b01          	ld	(OFST+0,sp),a
7252  0011 c65259        	ld	a,21081
7253  0014 a40c          	and	a,#12
7254  0016 1a01          	or	a,(OFST+0,sp)
7255  0018 c75259        	ld	21081,a
7256                     ; 2277     if (TIM1_ICPolarity != TIM1_ICPOLARITY_RISING)
7258  001b 0d02          	tnz	(OFST+1,sp)
7259  001d 2706          	jreq	L3463
7260                     ; 2279         TIM1->CCER1 |= TIM1_CCER1_CC2P;
7262  001f 721a525c      	bset	21084,#5
7264  0023 2004          	jra	L5463
7265  0025               L3463:
7266                     ; 2283         TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC2P);
7268  0025 721b525c      	bres	21084,#5
7269  0029               L5463:
7270                     ; 2286     TIM1->CCER1 |=  TIM1_CCER1_CC2E;
7272  0029 7218525c      	bset	21084,#4
7273                     ; 2287 }
7276  002d 5b03          	addw	sp,#3
7277  002f 81            	ret
7329                     ; 2305 static void TI3_Config(uint8_t TIM1_ICPolarity,
7329                     ; 2306                        uint8_t TIM1_ICSelection,
7329                     ; 2307                        uint8_t TIM1_ICFilter)
7329                     ; 2308 {
7330                     .text:	section	.text,new
7331  0000               L7_TI3_Config:
7333  0000 89            	pushw	x
7334  0001 88            	push	a
7335       00000001      OFST:	set	1
7338                     ; 2310     TIM1->CCER2 &=  (uint8_t)(~TIM1_CCER2_CC3E);
7340  0002 7211525d      	bres	21085,#0
7341                     ; 2313     TIM1->CCMR3 = (uint8_t)((uint8_t)(TIM1->CCMR3 & (uint8_t)(~(uint8_t)( TIM1_CCMR_CCxS | TIM1_CCMR_ICxF))) 
7341                     ; 2314                             | (uint8_t)(( (TIM1_ICSelection)) | ((uint8_t)( TIM1_ICFilter << 4))));
7343  0006 7b06          	ld	a,(OFST+5,sp)
7344  0008 97            	ld	xl,a
7345  0009 a610          	ld	a,#16
7346  000b 42            	mul	x,a
7347  000c 9f            	ld	a,xl
7348  000d 1a03          	or	a,(OFST+2,sp)
7349  000f 6b01          	ld	(OFST+0,sp),a
7350  0011 c6525a        	ld	a,21082
7351  0014 a40c          	and	a,#12
7352  0016 1a01          	or	a,(OFST+0,sp)
7353  0018 c7525a        	ld	21082,a
7354                     ; 2317     if (TIM1_ICPolarity != TIM1_ICPOLARITY_RISING)
7356  001b 0d02          	tnz	(OFST+1,sp)
7357  001d 2706          	jreq	L5763
7358                     ; 2319         TIM1->CCER2 |= TIM1_CCER2_CC3P;
7360  001f 7212525d      	bset	21085,#1
7362  0023 2004          	jra	L7763
7363  0025               L5763:
7364                     ; 2323         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3P);
7366  0025 7213525d      	bres	21085,#1
7367  0029               L7763:
7368                     ; 2326     TIM1->CCER2 |=  TIM1_CCER2_CC3E;
7370  0029 7210525d      	bset	21085,#0
7371                     ; 2327 }
7374  002d 5b03          	addw	sp,#3
7375  002f 81            	ret
7427                     ; 2346 static void TI4_Config(uint8_t TIM1_ICPolarity,
7427                     ; 2347                        uint8_t TIM1_ICSelection,
7427                     ; 2348                        uint8_t TIM1_ICFilter)
7427                     ; 2349 {
7428                     .text:	section	.text,new
7429  0000               L11_TI4_Config:
7431  0000 89            	pushw	x
7432  0001 88            	push	a
7433       00000001      OFST:	set	1
7436                     ; 2352     TIM1->CCER2 &=  (uint8_t)(~TIM1_CCER2_CC4E);
7438  0002 7219525d      	bres	21085,#4
7439                     ; 2355     TIM1->CCMR4 = (uint8_t)((uint8_t)(TIM1->CCMR4 & (uint8_t)(~(uint8_t)( TIM1_CCMR_CCxS | TIM1_CCMR_ICxF )))
7439                     ; 2356                             | (uint8_t)(( (TIM1_ICSelection)) | ((uint8_t)( TIM1_ICFilter << 4))));
7441  0006 7b06          	ld	a,(OFST+5,sp)
7442  0008 97            	ld	xl,a
7443  0009 a610          	ld	a,#16
7444  000b 42            	mul	x,a
7445  000c 9f            	ld	a,xl
7446  000d 1a03          	or	a,(OFST+2,sp)
7447  000f 6b01          	ld	(OFST+0,sp),a
7448  0011 c6525b        	ld	a,21083
7449  0014 a40c          	and	a,#12
7450  0016 1a01          	or	a,(OFST+0,sp)
7451  0018 c7525b        	ld	21083,a
7452                     ; 2359     if (TIM1_ICPolarity != TIM1_ICPOLARITY_RISING)
7454  001b 0d02          	tnz	(OFST+1,sp)
7455  001d 2706          	jreq	L7273
7456                     ; 2361         TIM1->CCER2 |= TIM1_CCER2_CC4P;
7458  001f 721a525d      	bset	21085,#5
7460  0023 2004          	jra	L1373
7461  0025               L7273:
7462                     ; 2365         TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC4P);
7464  0025 721b525d      	bres	21085,#5
7465  0029               L1373:
7466                     ; 2369     TIM1->CCER2 |=  TIM1_CCER2_CC4E;
7468  0029 7218525d      	bset	21085,#4
7469                     ; 2370 }
7472  002d 5b03          	addw	sp,#3
7473  002f 81            	ret
7486                     	xdef	_TIM1_ClearITPendingBit
7487                     	xdef	_TIM1_GetITStatus
7488                     	xdef	_TIM1_ClearFlag
7489                     	xdef	_TIM1_GetFlagStatus
7490                     	xdef	_TIM1_GetPrescaler
7491                     	xdef	_TIM1_GetCounter
7492                     	xdef	_TIM1_GetCapture4
7493                     	xdef	_TIM1_GetCapture3
7494                     	xdef	_TIM1_GetCapture2
7495                     	xdef	_TIM1_GetCapture1
7496                     	xdef	_TIM1_SetIC4Prescaler
7497                     	xdef	_TIM1_SetIC3Prescaler
7498                     	xdef	_TIM1_SetIC2Prescaler
7499                     	xdef	_TIM1_SetIC1Prescaler
7500                     	xdef	_TIM1_SetCompare4
7501                     	xdef	_TIM1_SetCompare3
7502                     	xdef	_TIM1_SetCompare2
7503                     	xdef	_TIM1_SetCompare1
7504                     	xdef	_TIM1_SetAutoreload
7505                     	xdef	_TIM1_SetCounter
7506                     	xdef	_TIM1_SelectOCxM
7507                     	xdef	_TIM1_CCxNCmd
7508                     	xdef	_TIM1_CCxCmd
7509                     	xdef	_TIM1_OC4PolarityConfig
7510                     	xdef	_TIM1_OC3NPolarityConfig
7511                     	xdef	_TIM1_OC3PolarityConfig
7512                     	xdef	_TIM1_OC2NPolarityConfig
7513                     	xdef	_TIM1_OC2PolarityConfig
7514                     	xdef	_TIM1_OC1NPolarityConfig
7515                     	xdef	_TIM1_OC1PolarityConfig
7516                     	xdef	_TIM1_GenerateEvent
7517                     	xdef	_TIM1_OC4FastConfig
7518                     	xdef	_TIM1_OC3FastConfig
7519                     	xdef	_TIM1_OC2FastConfig
7520                     	xdef	_TIM1_OC1FastConfig
7521                     	xdef	_TIM1_OC4PreloadConfig
7522                     	xdef	_TIM1_OC3PreloadConfig
7523                     	xdef	_TIM1_OC2PreloadConfig
7524                     	xdef	_TIM1_OC1PreloadConfig
7525                     	xdef	_TIM1_CCPreloadControl
7526                     	xdef	_TIM1_SelectCOM
7527                     	xdef	_TIM1_ARRPreloadConfig
7528                     	xdef	_TIM1_ForcedOC4Config
7529                     	xdef	_TIM1_ForcedOC3Config
7530                     	xdef	_TIM1_ForcedOC2Config
7531                     	xdef	_TIM1_ForcedOC1Config
7532                     	xdef	_TIM1_CounterModeConfig
7533                     	xdef	_TIM1_PrescalerConfig
7534                     	xdef	_TIM1_EncoderInterfaceConfig
7535                     	xdef	_TIM1_SelectMasterSlaveMode
7536                     	xdef	_TIM1_SelectSlaveMode
7537                     	xdef	_TIM1_SelectOutputTrigger
7538                     	xdef	_TIM1_SelectOnePulseMode
7539                     	xdef	_TIM1_SelectHallSensor
7540                     	xdef	_TIM1_UpdateRequestConfig
7541                     	xdef	_TIM1_UpdateDisableConfig
7542                     	xdef	_TIM1_SelectInputTrigger
7543                     	xdef	_TIM1_TIxExternalClockConfig
7544                     	xdef	_TIM1_ETRConfig
7545                     	xdef	_TIM1_ETRClockMode2Config
7546                     	xdef	_TIM1_ETRClockMode1Config
7547                     	xdef	_TIM1_InternalClockConfig
7548                     	xdef	_TIM1_ITConfig
7549                     	xdef	_TIM1_CtrlPWMOutputs
7550                     	xdef	_TIM1_Cmd
7551                     	xdef	_TIM1_PWMIConfig
7552                     	xdef	_TIM1_ICInit
7553                     	xdef	_TIM1_BDTRConfig
7554                     	xdef	_TIM1_OC4Init
7555                     	xdef	_TIM1_OC3Init
7556                     	xdef	_TIM1_OC2Init
7557                     	xdef	_TIM1_OC1Init
7558                     	xdef	_TIM1_TimeBaseInit
7559                     	xdef	_TIM1_DeInit
7578                     	end
