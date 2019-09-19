   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  34                     ; 14 @far @interrupt void NonHandledInterrupt (void)
  34                     ; 15 {
  35                     	switch	.text
  36  0000               f_NonHandledInterrupt:
  41                     ; 19 	return;
  44  0000 80            	iret
  66                     ; 24 @far @interrupt void AWU_IT (void){
  67                     	switch	.text
  68  0001               f_AWU_IT:
  73                     ; 26   (void) AWU->CSR;
  75  0001 c650f0        	ld	a,20720
  76  0004 97            	ld	xl,a
  77                     ; 27   return;
  80  0005 80            	iret
 102                     ; 30 @far @interrupt void PortAInterrupt (void){
 103                     	switch	.text
 104  0006               f_PortAInterrupt:
 109                     ; 31 	return;
 112  0006 80            	iret
 134                     ; 34 @far @interrupt void PortCInterrupt (void){
 135                     	switch	.text
 136  0007               f_PortCInterrupt:
 141                     ; 35 }
 144  0007 80            	iret
 167                     ; 36 @far @interrupt void PortDInterrupt (void){
 168                     	switch	.text
 169  0008               f_PortDInterrupt:
 174                     ; 37 	rfirq=1; //无线中断
 176  0008 35010000      	mov	_rfirq,#1
 177                     ; 38 	return;
 180  000c 80            	iret
 204                     ; 42 @far @interrupt void PortBInterrupt (void){
 205                     	switch	.text
 206  000d               f_PortBInterrupt:
 211                     ; 43 	audio_irq=1;
 213  000d 35010000      	mov	_audio_irq,#1
 214                     ; 44 	audio_play=0;//声音播放结束
 216  0011 3f00          	clr	_audio_play
 217                     ; 45 }
 220  0013 80            	iret
 243                     ; 48 @far @interrupt void TIM2UpdateInterrupt(void)
 243                     ; 49 {
 244                     	switch	.text
 245  0014               f_TIM2UpdateInterrupt:
 250                     ; 50 }
 253  0014 80            	iret
 255                     .const:	section	.text
 256  0000               __vectab:
 257  0000 82            	dc.b	130
 259  0001 00            	dc.b	page(__stext)
 260  0002 0000          	dc.w	__stext
 261  0004 82            	dc.b	130
 263  0005 00            	dc.b	page(f_NonHandledInterrupt)
 264  0006 0000          	dc.w	f_NonHandledInterrupt
 265  0008 82            	dc.b	130
 267  0009 00            	dc.b	page(f_NonHandledInterrupt)
 268  000a 0000          	dc.w	f_NonHandledInterrupt
 269  000c 82            	dc.b	130
 271  000d 01            	dc.b	page(f_AWU_IT)
 272  000e 0001          	dc.w	f_AWU_IT
 273  0010 82            	dc.b	130
 275  0011 00            	dc.b	page(f_NonHandledInterrupt)
 276  0012 0000          	dc.w	f_NonHandledInterrupt
 277  0014 82            	dc.b	130
 279  0015 06            	dc.b	page(f_PortAInterrupt)
 280  0016 0006          	dc.w	f_PortAInterrupt
 281  0018 82            	dc.b	130
 283  0019 0d            	dc.b	page(f_PortBInterrupt)
 284  001a 000d          	dc.w	f_PortBInterrupt
 285  001c 82            	dc.b	130
 287  001d 07            	dc.b	page(f_PortCInterrupt)
 288  001e 0007          	dc.w	f_PortCInterrupt
 289  0020 82            	dc.b	130
 291  0021 08            	dc.b	page(f_PortDInterrupt)
 292  0022 0008          	dc.w	f_PortDInterrupt
 293  0024 82            	dc.b	130
 295  0025 00            	dc.b	page(f_NonHandledInterrupt)
 296  0026 0000          	dc.w	f_NonHandledInterrupt
 297  0028 82            	dc.b	130
 299  0029 00            	dc.b	page(f_NonHandledInterrupt)
 300  002a 0000          	dc.w	f_NonHandledInterrupt
 301  002c 82            	dc.b	130
 303  002d 00            	dc.b	page(f_NonHandledInterrupt)
 304  002e 0000          	dc.w	f_NonHandledInterrupt
 305  0030 82            	dc.b	130
 307  0031 00            	dc.b	page(f_NonHandledInterrupt)
 308  0032 0000          	dc.w	f_NonHandledInterrupt
 309  0034 82            	dc.b	130
 311  0035 00            	dc.b	page(f_NonHandledInterrupt)
 312  0036 0000          	dc.w	f_NonHandledInterrupt
 313  0038 82            	dc.b	130
 315  0039 00            	dc.b	page(f_NonHandledInterrupt)
 316  003a 0000          	dc.w	f_NonHandledInterrupt
 317  003c 82            	dc.b	130
 319  003d 14            	dc.b	page(f_TIM2UpdateInterrupt)
 320  003e 0014          	dc.w	f_TIM2UpdateInterrupt
 321  0040 82            	dc.b	130
 323  0041 00            	dc.b	page(f_NonHandledInterrupt)
 324  0042 0000          	dc.w	f_NonHandledInterrupt
 325  0044 82            	dc.b	130
 327  0045 00            	dc.b	page(f_NonHandledInterrupt)
 328  0046 0000          	dc.w	f_NonHandledInterrupt
 329  0048 82            	dc.b	130
 331  0049 00            	dc.b	page(f_NonHandledInterrupt)
 332  004a 0000          	dc.w	f_NonHandledInterrupt
 333  004c 82            	dc.b	130
 335  004d 00            	dc.b	page(f_NonHandledInterrupt)
 336  004e 0000          	dc.w	f_NonHandledInterrupt
 337  0050 82            	dc.b	130
 339  0051 00            	dc.b	page(f_NonHandledInterrupt)
 340  0052 0000          	dc.w	f_NonHandledInterrupt
 341  0054 82            	dc.b	130
 343  0055 00            	dc.b	page(f_NonHandledInterrupt)
 344  0056 0000          	dc.w	f_NonHandledInterrupt
 345  0058 82            	dc.b	130
 347  0059 00            	dc.b	page(f_NonHandledInterrupt)
 348  005a 0000          	dc.w	f_NonHandledInterrupt
 349  005c 82            	dc.b	130
 351  005d 00            	dc.b	page(f_NonHandledInterrupt)
 352  005e 0000          	dc.w	f_NonHandledInterrupt
 353  0060 82            	dc.b	130
 355  0061 00            	dc.b	page(f_NonHandledInterrupt)
 356  0062 0000          	dc.w	f_NonHandledInterrupt
 357  0064 82            	dc.b	130
 359  0065 00            	dc.b	page(f_NonHandledInterrupt)
 360  0066 0000          	dc.w	f_NonHandledInterrupt
 361  0068 82            	dc.b	130
 363  0069 00            	dc.b	page(f_NonHandledInterrupt)
 364  006a 0000          	dc.w	f_NonHandledInterrupt
 365  006c 82            	dc.b	130
 367  006d 00            	dc.b	page(f_NonHandledInterrupt)
 368  006e 0000          	dc.w	f_NonHandledInterrupt
 369  0070 82            	dc.b	130
 371  0071 00            	dc.b	page(f_NonHandledInterrupt)
 372  0072 0000          	dc.w	f_NonHandledInterrupt
 373  0074 82            	dc.b	130
 375  0075 00            	dc.b	page(f_NonHandledInterrupt)
 376  0076 0000          	dc.w	f_NonHandledInterrupt
 377  0078 82            	dc.b	130
 379  0079 00            	dc.b	page(f_NonHandledInterrupt)
 380  007a 0000          	dc.w	f_NonHandledInterrupt
 381  007c 82            	dc.b	130
 383  007d 00            	dc.b	page(f_NonHandledInterrupt)
 384  007e 0000          	dc.w	f_NonHandledInterrupt
 435                     	xdef	__vectab
 436                     	xref	__stext
 437                     	xdef	f_TIM2UpdateInterrupt
 438                     	xdef	f_PortBInterrupt
 439                     	xref.b	_audio_irq
 440                     	xref.b	_audio_play
 441                     	xdef	f_PortDInterrupt
 442                     	xdef	f_PortCInterrupt
 443                     	xdef	f_PortAInterrupt
 444                     	xdef	f_AWU_IT
 445                     	xdef	f_NonHandledInterrupt
 446                     	xref.b	_rfirq
 465                     	end
