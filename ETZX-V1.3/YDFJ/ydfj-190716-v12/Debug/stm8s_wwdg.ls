   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  52                     ; 47 void WWDG_Init(uint8_t Counter, uint8_t WindowValue)
  52                     ; 48 {
  54                     .text:	section	.text,new
  55  0000               _WWDG_Init:
  57  0000 89            	pushw	x
  58       00000000      OFST:	set	0
  61                     ; 50     assert_param(IS_WWDG_WINDOWLIMITVALUE_OK(WindowValue));
  63                     ; 52     WWDG->WR = WWDG_WR_RESET_VALUE;
  65  0001 357f50d2      	mov	20690,#127
  66                     ; 53     WWDG->CR = (uint8_t)((uint8_t)(WWDG_CR_WDGA | WWDG_CR_T6) | (uint8_t)Counter);
  68  0005 9e            	ld	a,xh
  69  0006 aac0          	or	a,#192
  70  0008 c750d1        	ld	20689,a
  71                     ; 54     WWDG->WR = (uint8_t)((uint8_t)(~WWDG_CR_WDGA) & (uint8_t)(WWDG_CR_T6 | WindowValue));
  73  000b 7b02          	ld	a,(OFST+2,sp)
  74  000d aa40          	or	a,#64
  75  000f a47f          	and	a,#127
  76  0011 c750d2        	ld	20690,a
  77                     ; 55 }
  80  0014 85            	popw	x
  81  0015 81            	ret
 115                     ; 63 void WWDG_SetCounter(uint8_t Counter)
 115                     ; 64 {
 116                     .text:	section	.text,new
 117  0000               _WWDG_SetCounter:
 121                     ; 67     assert_param(IS_WWDG_COUNTERVALUE_OK(Counter));
 123                     ; 71   WWDG->CR = (uint8_t)(Counter & (uint8_t)BIT_MASK);
 125  0000 a47f          	and	a,#127
 126  0002 c750d1        	ld	20689,a
 127                     ; 73 }
 130  0005 81            	ret
 153                     ; 82 uint8_t WWDG_GetCounter(void)
 153                     ; 83 {
 154                     .text:	section	.text,new
 155  0000               _WWDG_GetCounter:
 159                     ; 84     return(WWDG->CR);
 161  0000 c650d1        	ld	a,20689
 164  0003 81            	ret
 187                     ; 92 void WWDG_SWReset(void)
 187                     ; 93 {
 188                     .text:	section	.text,new
 189  0000               _WWDG_SWReset:
 193                     ; 94     WWDG->CR = WWDG_CR_WDGA; /* Activate WWDG, with clearing T6 */
 195  0000 358050d1      	mov	20689,#128
 196                     ; 95 }
 199  0004 81            	ret
 234                     ; 104 void WWDG_SetWindowValue(uint8_t WindowValue)
 234                     ; 105 {
 235                     .text:	section	.text,new
 236  0000               _WWDG_SetWindowValue:
 240                     ; 107     assert_param(IS_WWDG_WINDOWLIMITVALUE_OK(WindowValue));
 242                     ; 109     WWDG->WR = (uint8_t)((uint8_t)(~WWDG_CR_WDGA) & (uint8_t)(WWDG_CR_T6 | WindowValue));
 244  0000 aa40          	or	a,#64
 245  0002 a47f          	and	a,#127
 246  0004 c750d2        	ld	20690,a
 247                     ; 110 }
 250  0007 81            	ret
 263                     	xdef	_WWDG_SetWindowValue
 264                     	xdef	_WWDG_SWReset
 265                     	xdef	_WWDG_GetCounter
 266                     	xdef	_WWDG_SetCounter
 267                     	xdef	_WWDG_Init
 286                     	end
