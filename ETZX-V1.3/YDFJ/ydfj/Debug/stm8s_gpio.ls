   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  98                     ; 47 void GPIO_DeInit(GPIO_TypeDef* GPIOx)
  98                     ; 48 {
 100                     	switch	.text
 101  0000               _GPIO_DeInit:
 105                     ; 49     GPIOx->ODR = GPIO_ODR_RESET_VALUE; /* Reset Output Data Register */
 107  0000 7f            	clr	(x)
 108                     ; 50     GPIOx->DDR = GPIO_DDR_RESET_VALUE; /* Reset Data Direction Register */
 110  0001 6f02          	clr	(2,x)
 111                     ; 51     GPIOx->CR1 = GPIO_CR1_RESET_VALUE; /* Reset Control Register 1 */
 113  0003 6f03          	clr	(3,x)
 114                     ; 52     GPIOx->CR2 = GPIO_CR2_RESET_VALUE; /* Reset Control Register 2 */
 116  0005 6f04          	clr	(4,x)
 117                     ; 53 }
 120  0007 81            	ret
 360                     ; 65 void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, GPIO_Mode_TypeDef GPIO_Mode)
 360                     ; 66 {
 361                     	switch	.text
 362  0008               _GPIO_Init:
 364  0008 89            	pushw	x
 365       00000000      OFST:	set	0
 368                     ; 71     assert_param(IS_GPIO_MODE_OK(GPIO_Mode));
 370                     ; 72     assert_param(IS_GPIO_PIN_OK(GPIO_Pin));
 372                     ; 75   GPIOx->CR2 &= (uint8_t)(~(GPIO_Pin));
 374  0009 7b05          	ld	a,(OFST+5,sp)
 375  000b 43            	cpl	a
 376  000c e404          	and	a,(4,x)
 377  000e e704          	ld	(4,x),a
 378                     ; 81     if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x80) != (uint8_t)0x00) /* Output mode */
 380  0010 7b06          	ld	a,(OFST+6,sp)
 381  0012 a580          	bcp	a,#128
 382  0014 271d          	jreq	L771
 383                     ; 83         if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x10) != (uint8_t)0x00) /* High level */
 385  0016 7b06          	ld	a,(OFST+6,sp)
 386  0018 a510          	bcp	a,#16
 387  001a 2706          	jreq	L102
 388                     ; 85             GPIOx->ODR |= (uint8_t)GPIO_Pin;
 390  001c f6            	ld	a,(x)
 391  001d 1a05          	or	a,(OFST+5,sp)
 392  001f f7            	ld	(x),a
 394  0020 2007          	jra	L302
 395  0022               L102:
 396                     ; 89             GPIOx->ODR &= (uint8_t)(~(GPIO_Pin));
 398  0022 1e01          	ldw	x,(OFST+1,sp)
 399  0024 7b05          	ld	a,(OFST+5,sp)
 400  0026 43            	cpl	a
 401  0027 f4            	and	a,(x)
 402  0028 f7            	ld	(x),a
 403  0029               L302:
 404                     ; 92         GPIOx->DDR |= (uint8_t)GPIO_Pin;
 406  0029 1e01          	ldw	x,(OFST+1,sp)
 407  002b e602          	ld	a,(2,x)
 408  002d 1a05          	or	a,(OFST+5,sp)
 409  002f e702          	ld	(2,x),a
 411  0031 2009          	jra	L502
 412  0033               L771:
 413                     ; 97         GPIOx->DDR &= (uint8_t)(~(GPIO_Pin));
 415  0033 1e01          	ldw	x,(OFST+1,sp)
 416  0035 7b05          	ld	a,(OFST+5,sp)
 417  0037 43            	cpl	a
 418  0038 e402          	and	a,(2,x)
 419  003a e702          	ld	(2,x),a
 420  003c               L502:
 421                     ; 104     if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x40) != (uint8_t)0x00) /* Pull-Up or Push-Pull */
 423  003c 7b06          	ld	a,(OFST+6,sp)
 424  003e a540          	bcp	a,#64
 425  0040 270a          	jreq	L702
 426                     ; 106         GPIOx->CR1 |= (uint8_t)GPIO_Pin;
 428  0042 1e01          	ldw	x,(OFST+1,sp)
 429  0044 e603          	ld	a,(3,x)
 430  0046 1a05          	or	a,(OFST+5,sp)
 431  0048 e703          	ld	(3,x),a
 433  004a 2009          	jra	L112
 434  004c               L702:
 435                     ; 110         GPIOx->CR1 &= (uint8_t)(~(GPIO_Pin));
 437  004c 1e01          	ldw	x,(OFST+1,sp)
 438  004e 7b05          	ld	a,(OFST+5,sp)
 439  0050 43            	cpl	a
 440  0051 e403          	and	a,(3,x)
 441  0053 e703          	ld	(3,x),a
 442  0055               L112:
 443                     ; 117     if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x20) != (uint8_t)0x00) /* Interrupt or Slow slope */
 445  0055 7b06          	ld	a,(OFST+6,sp)
 446  0057 a520          	bcp	a,#32
 447  0059 270a          	jreq	L312
 448                     ; 119         GPIOx->CR2 |= (uint8_t)GPIO_Pin;
 450  005b 1e01          	ldw	x,(OFST+1,sp)
 451  005d e604          	ld	a,(4,x)
 452  005f 1a05          	or	a,(OFST+5,sp)
 453  0061 e704          	ld	(4,x),a
 455  0063 2009          	jra	L512
 456  0065               L312:
 457                     ; 123         GPIOx->CR2 &= (uint8_t)(~(GPIO_Pin));
 459  0065 1e01          	ldw	x,(OFST+1,sp)
 460  0067 7b05          	ld	a,(OFST+5,sp)
 461  0069 43            	cpl	a
 462  006a e404          	and	a,(4,x)
 463  006c e704          	ld	(4,x),a
 464  006e               L512:
 465                     ; 125 }
 468  006e 85            	popw	x
 469  006f 81            	ret
 515                     ; 135 void GPIO_Write(GPIO_TypeDef* GPIOx, uint8_t PortVal)
 515                     ; 136 {
 516                     	switch	.text
 517  0070               _GPIO_Write:
 519  0070 89            	pushw	x
 520       00000000      OFST:	set	0
 523                     ; 137     GPIOx->ODR = PortVal;
 525  0071 7b05          	ld	a,(OFST+5,sp)
 526  0073 1e01          	ldw	x,(OFST+1,sp)
 527  0075 f7            	ld	(x),a
 528                     ; 138 }
 531  0076 85            	popw	x
 532  0077 81            	ret
 579                     ; 148 void GPIO_WriteHigh(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
 579                     ; 149 {
 580                     	switch	.text
 581  0078               _GPIO_WriteHigh:
 583  0078 89            	pushw	x
 584       00000000      OFST:	set	0
 587                     ; 150     GPIOx->ODR |= (uint8_t)PortPins;
 589  0079 f6            	ld	a,(x)
 590  007a 1a05          	or	a,(OFST+5,sp)
 591  007c f7            	ld	(x),a
 592                     ; 151 }
 595  007d 85            	popw	x
 596  007e 81            	ret
 643                     ; 161 void GPIO_WriteLow(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
 643                     ; 162 {
 644                     	switch	.text
 645  007f               _GPIO_WriteLow:
 647  007f 89            	pushw	x
 648       00000000      OFST:	set	0
 651                     ; 163     GPIOx->ODR &= (uint8_t)(~PortPins);
 653  0080 7b05          	ld	a,(OFST+5,sp)
 654  0082 43            	cpl	a
 655  0083 f4            	and	a,(x)
 656  0084 f7            	ld	(x),a
 657                     ; 164 }
 660  0085 85            	popw	x
 661  0086 81            	ret
 708                     ; 174 void GPIO_WriteReverse(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
 708                     ; 175 {
 709                     	switch	.text
 710  0087               _GPIO_WriteReverse:
 712  0087 89            	pushw	x
 713       00000000      OFST:	set	0
 716                     ; 176     GPIOx->ODR ^= (uint8_t)PortPins;
 718  0088 f6            	ld	a,(x)
 719  0089 1805          	xor	a,	(OFST+5,sp)
 720  008b f7            	ld	(x),a
 721                     ; 177 }
 724  008c 85            	popw	x
 725  008d 81            	ret
 763                     ; 185 uint8_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
 763                     ; 186 {
 764                     	switch	.text
 765  008e               _GPIO_ReadOutputData:
 769                     ; 187     return ((uint8_t)GPIOx->ODR);
 771  008e f6            	ld	a,(x)
 774  008f 81            	ret
 811                     ; 196 uint8_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
 811                     ; 197 {
 812                     	switch	.text
 813  0090               _GPIO_ReadInputData:
 817                     ; 198     return ((uint8_t)GPIOx->IDR);
 819  0090 e601          	ld	a,(1,x)
 822  0092 81            	ret
 890                     ; 207 BitStatus GPIO_ReadInputPin(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin)
 890                     ; 208 {
 891                     	switch	.text
 892  0093               _GPIO_ReadInputPin:
 894  0093 89            	pushw	x
 895       00000000      OFST:	set	0
 898                     ; 209     return ((BitStatus)(GPIOx->IDR & (uint8_t)GPIO_Pin));
 900  0094 e601          	ld	a,(1,x)
 901  0096 1405          	and	a,(OFST+5,sp)
 904  0098 85            	popw	x
 905  0099 81            	ret
 983                     ; 219 void GPIO_ExternalPullUpConfig(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, FunctionalState NewState)
 983                     ; 220 {
 984                     	switch	.text
 985  009a               _GPIO_ExternalPullUpConfig:
 987  009a 89            	pushw	x
 988       00000000      OFST:	set	0
 991                     ; 222     assert_param(IS_GPIO_PIN_OK(GPIO_Pin));
 993                     ; 223     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 995                     ; 225     if (NewState != DISABLE) /* External Pull-Up Set*/
 997  009b 0d06          	tnz	(OFST+6,sp)
 998  009d 2708          	jreq	L374
 999                     ; 227         GPIOx->CR1 |= (uint8_t)GPIO_Pin;
1001  009f e603          	ld	a,(3,x)
1002  00a1 1a05          	or	a,(OFST+5,sp)
1003  00a3 e703          	ld	(3,x),a
1005  00a5 2009          	jra	L574
1006  00a7               L374:
1007                     ; 230         GPIOx->CR1 &= (uint8_t)(~(GPIO_Pin));
1009  00a7 1e01          	ldw	x,(OFST+1,sp)
1010  00a9 7b05          	ld	a,(OFST+5,sp)
1011  00ab 43            	cpl	a
1012  00ac e403          	and	a,(3,x)
1013  00ae e703          	ld	(3,x),a
1014  00b0               L574:
1015                     ; 232 }
1018  00b0 85            	popw	x
1019  00b1 81            	ret
1032                     	xdef	_GPIO_ExternalPullUpConfig
1033                     	xdef	_GPIO_ReadInputPin
1034                     	xdef	_GPIO_ReadOutputData
1035                     	xdef	_GPIO_ReadInputData
1036                     	xdef	_GPIO_WriteReverse
1037                     	xdef	_GPIO_WriteLow
1038                     	xdef	_GPIO_WriteHigh
1039                     	xdef	_GPIO_Write
1040                     	xdef	_GPIO_Init
1041                     	xdef	_GPIO_DeInit
1060                     	end
