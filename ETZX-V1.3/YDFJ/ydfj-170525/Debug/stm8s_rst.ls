   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
 106                     ; 48 FlagStatus RST_GetFlagStatus(RST_Flag_TypeDef RST_Flag)
 106                     ; 49 {
 108                     	switch	.text
 109  0000               _RST_GetFlagStatus:
 113                     ; 51     assert_param(IS_RST_FLAG_OK(RST_Flag));
 115                     ; 55     return ((FlagStatus)((uint8_t)RST->SR & (uint8_t)RST_Flag));
 117  0000 c450b3        	and	a,20659
 120  0003 81            	ret
 155                     ; 64 void RST_ClearFlag(RST_Flag_TypeDef RST_Flag)
 155                     ; 65 {
 156                     	switch	.text
 157  0004               _RST_ClearFlag:
 161                     ; 67     assert_param(IS_RST_FLAG_OK(RST_Flag));
 163                     ; 69     RST->SR = (uint8_t)RST_Flag;
 165  0004 c750b3        	ld	20659,a
 166                     ; 70 }
 169  0007 81            	ret
 182                     	xdef	_RST_ClearFlag
 183                     	xdef	_RST_GetFlagStatus
 202                     	end
