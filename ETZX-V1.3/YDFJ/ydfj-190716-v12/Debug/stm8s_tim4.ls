   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 43 void TIM4_DeInit(void)
  32                     ; 44 {
  34                     .text:	section	.text,new
  35  0000               _TIM4_DeInit:
  39                     ; 45     TIM4->CR1 = TIM4_CR1_RESET_VALUE;
  41  0000 725f5340      	clr	21312
  42                     ; 46     TIM4->IER = TIM4_IER_RESET_VALUE;
  44  0004 725f5343      	clr	21315
  45                     ; 47     TIM4->CNTR = TIM4_CNTR_RESET_VALUE;
  47  0008 725f5346      	clr	21318
  48                     ; 48     TIM4->PSCR = TIM4_PSCR_RESET_VALUE;
  50  000c 725f5347      	clr	21319
  51                     ; 49     TIM4->ARR = TIM4_ARR_RESET_VALUE;
  53  0010 35ff5348      	mov	21320,#255
  54                     ; 50     TIM4->SR1 = TIM4_SR1_RESET_VALUE;
  56  0014 725f5344      	clr	21316
  57                     ; 51 }
  60  0018 81            	ret
 166                     ; 59 void TIM4_TimeBaseInit(TIM4_Prescaler_TypeDef TIM4_Prescaler, uint8_t TIM4_Period)
 166                     ; 60 {
 167                     .text:	section	.text,new
 168  0000               _TIM4_TimeBaseInit:
 172                     ; 62     assert_param(IS_TIM4_PRESCALER_OK(TIM4_Prescaler));
 174                     ; 64     TIM4->PSCR = (uint8_t)(TIM4_Prescaler);
 176  0000 9e            	ld	a,xh
 177  0001 c75347        	ld	21319,a
 178                     ; 66     TIM4->ARR = (uint8_t)(TIM4_Period);
 180  0004 9f            	ld	a,xl
 181  0005 c75348        	ld	21320,a
 182                     ; 67 }
 185  0008 81            	ret
 240                     ; 77 void TIM4_Cmd(FunctionalState NewState)
 240                     ; 78 {
 241                     .text:	section	.text,new
 242  0000               _TIM4_Cmd:
 246                     ; 80     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 248                     ; 83     if (NewState != DISABLE)
 250  0000 4d            	tnz	a
 251  0001 2706          	jreq	L511
 252                     ; 85         TIM4->CR1 |= TIM4_CR1_CEN;
 254  0003 72105340      	bset	21312,#0
 256  0007 2004          	jra	L711
 257  0009               L511:
 258                     ; 89         TIM4->CR1 &= (uint8_t)(~TIM4_CR1_CEN);
 260  0009 72115340      	bres	21312,#0
 261  000d               L711:
 262                     ; 91 }
 265  000d 81            	ret
 323                     ; 103 void TIM4_ITConfig(TIM4_IT_TypeDef TIM4_IT, FunctionalState NewState)
 323                     ; 104 {
 324                     .text:	section	.text,new
 325  0000               _TIM4_ITConfig:
 327  0000 89            	pushw	x
 328       00000000      OFST:	set	0
 331                     ; 106     assert_param(IS_TIM4_IT_OK(TIM4_IT));
 333                     ; 107     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 335                     ; 109     if (NewState != DISABLE)
 337  0001 9f            	ld	a,xl
 338  0002 4d            	tnz	a
 339  0003 2709          	jreq	L151
 340                     ; 112         TIM4->IER |= (uint8_t)TIM4_IT;
 342  0005 9e            	ld	a,xh
 343  0006 ca5343        	or	a,21315
 344  0009 c75343        	ld	21315,a
 346  000c 2009          	jra	L351
 347  000e               L151:
 348                     ; 117         TIM4->IER &= (uint8_t)(~TIM4_IT);
 350  000e 7b01          	ld	a,(OFST+1,sp)
 351  0010 43            	cpl	a
 352  0011 c45343        	and	a,21315
 353  0014 c75343        	ld	21315,a
 354  0017               L351:
 355                     ; 119 }
 358  0017 85            	popw	x
 359  0018 81            	ret
 395                     ; 127 void TIM4_UpdateDisableConfig(FunctionalState NewState)
 395                     ; 128 {
 396                     .text:	section	.text,new
 397  0000               _TIM4_UpdateDisableConfig:
 401                     ; 130     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 403                     ; 133     if (NewState != DISABLE)
 405  0000 4d            	tnz	a
 406  0001 2706          	jreq	L371
 407                     ; 135         TIM4->CR1 |= TIM4_CR1_UDIS;
 409  0003 72125340      	bset	21312,#1
 411  0007 2004          	jra	L571
 412  0009               L371:
 413                     ; 139         TIM4->CR1 &= (uint8_t)(~TIM4_CR1_UDIS);
 415  0009 72135340      	bres	21312,#1
 416  000d               L571:
 417                     ; 141 }
 420  000d 81            	ret
 478                     ; 151 void TIM4_UpdateRequestConfig(TIM4_UpdateSource_TypeDef TIM4_UpdateSource)
 478                     ; 152 {
 479                     .text:	section	.text,new
 480  0000               _TIM4_UpdateRequestConfig:
 484                     ; 154     assert_param(IS_TIM4_UPDATE_SOURCE_OK(TIM4_UpdateSource));
 486                     ; 157     if (TIM4_UpdateSource != TIM4_UPDATESOURCE_GLOBAL)
 488  0000 4d            	tnz	a
 489  0001 2706          	jreq	L522
 490                     ; 159         TIM4->CR1 |= TIM4_CR1_URS;
 492  0003 72145340      	bset	21312,#2
 494  0007 2004          	jra	L722
 495  0009               L522:
 496                     ; 163         TIM4->CR1 &= (uint8_t)(~TIM4_CR1_URS);
 498  0009 72155340      	bres	21312,#2
 499  000d               L722:
 500                     ; 165 }
 503  000d 81            	ret
 560                     ; 175 void TIM4_SelectOnePulseMode(TIM4_OPMode_TypeDef TIM4_OPMode)
 560                     ; 176 {
 561                     .text:	section	.text,new
 562  0000               _TIM4_SelectOnePulseMode:
 566                     ; 178     assert_param(IS_TIM4_OPM_MODE_OK(TIM4_OPMode));
 568                     ; 181     if (TIM4_OPMode != TIM4_OPMODE_REPETITIVE)
 570  0000 4d            	tnz	a
 571  0001 2706          	jreq	L752
 572                     ; 183         TIM4->CR1 |= TIM4_CR1_OPM;
 574  0003 72165340      	bset	21312,#3
 576  0007 2004          	jra	L162
 577  0009               L752:
 578                     ; 187         TIM4->CR1 &= (uint8_t)(~TIM4_CR1_OPM);
 580  0009 72175340      	bres	21312,#3
 581  000d               L162:
 582                     ; 190 }
 585  000d 81            	ret
 653                     ; 212 void TIM4_PrescalerConfig(TIM4_Prescaler_TypeDef Prescaler, TIM4_PSCReloadMode_TypeDef TIM4_PSCReloadMode)
 653                     ; 213 {
 654                     .text:	section	.text,new
 655  0000               _TIM4_PrescalerConfig:
 659                     ; 215     assert_param(IS_TIM4_PRESCALER_RELOAD_OK(TIM4_PSCReloadMode));
 661                     ; 216     assert_param(IS_TIM4_PRESCALER_OK(Prescaler));
 663                     ; 219     TIM4->PSCR = (uint8_t)Prescaler;
 665  0000 9e            	ld	a,xh
 666  0001 c75347        	ld	21319,a
 667                     ; 222     TIM4->EGR = (uint8_t)TIM4_PSCReloadMode;
 669  0004 9f            	ld	a,xl
 670  0005 c75345        	ld	21317,a
 671                     ; 223 }
 674  0008 81            	ret
 710                     ; 231 void TIM4_ARRPreloadConfig(FunctionalState NewState)
 710                     ; 232 {
 711                     .text:	section	.text,new
 712  0000               _TIM4_ARRPreloadConfig:
 716                     ; 234     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 718                     ; 237     if (NewState != DISABLE)
 720  0000 4d            	tnz	a
 721  0001 2706          	jreq	L333
 722                     ; 239         TIM4->CR1 |= TIM4_CR1_ARPE;
 724  0003 721e5340      	bset	21312,#7
 726  0007 2004          	jra	L533
 727  0009               L333:
 728                     ; 243         TIM4->CR1 &= (uint8_t)(~TIM4_CR1_ARPE);
 730  0009 721f5340      	bres	21312,#7
 731  000d               L533:
 732                     ; 245 }
 735  000d 81            	ret
 784                     ; 254 void TIM4_GenerateEvent(TIM4_EventSource_TypeDef TIM4_EventSource)
 784                     ; 255 {
 785                     .text:	section	.text,new
 786  0000               _TIM4_GenerateEvent:
 790                     ; 257     assert_param(IS_TIM4_EVENT_SOURCE_OK(TIM4_EventSource));
 792                     ; 260     TIM4->EGR = (uint8_t)(TIM4_EventSource);
 794  0000 c75345        	ld	21317,a
 795                     ; 261 }
 798  0003 81            	ret
 832                     ; 270 void TIM4_SetCounter(uint8_t Counter)
 832                     ; 271 {
 833                     .text:	section	.text,new
 834  0000               _TIM4_SetCounter:
 838                     ; 273     TIM4->CNTR = (uint8_t)(Counter);
 840  0000 c75346        	ld	21318,a
 841                     ; 274 }
 844  0003 81            	ret
 878                     ; 283 void TIM4_SetAutoreload(uint8_t Autoreload)
 878                     ; 284 {
 879                     .text:	section	.text,new
 880  0000               _TIM4_SetAutoreload:
 884                     ; 286     TIM4->ARR = (uint8_t)(Autoreload);
 886  0000 c75348        	ld	21320,a
 887                     ; 287 }
 890  0003 81            	ret
 903                     	xdef	_TIM4_SetAutoreload
 904                     	xdef	_TIM4_SetCounter
 905                     	xdef	_TIM4_GenerateEvent
 906                     	xdef	_TIM4_ARRPreloadConfig
 907                     	xdef	_TIM4_PrescalerConfig
 908                     	xdef	_TIM4_SelectOnePulseMode
 909                     	xdef	_TIM4_UpdateRequestConfig
 910                     	xdef	_TIM4_UpdateDisableConfig
 911                     	xdef	_TIM4_ITConfig
 912                     	xdef	_TIM4_Cmd
 913                     	xdef	_TIM4_TimeBaseInit
 914                     	xdef	_TIM4_DeInit
 933                     	end
