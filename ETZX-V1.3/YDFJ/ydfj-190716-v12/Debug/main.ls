   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     	bsct
   5  0000               _ir_inc:
   6  0000 0000          	dc.w	0
   7  0002               _check_bat_cnt:
   8  0002 0000          	dc.w	0
   9  0004               _rfirq:
  10  0004 00            	dc.b	0
  11  0005               _audio_irq:
  12  0005 00            	dc.b	0
  13  0006               _RFStatusCnt:
  14  0006 0000          	dc.w	0
  15  0008               _RFClearState:
  16  0008 00            	dc.b	0
  17  0009               _RFStatus:
  18  0009 00            	dc.b	0
  19  000a               _CC1101SigValue:
  20  000a 00            	dc.b	0
  21  000b               _low_bat:
  22  000b 00            	dc.b	0
  23  000c               _para_error:
  24  000c 00            	dc.b	0
  53                     ; 58 void ConfigIOPort(void)
  53                     ; 59 {
  55                     .text:	section	.text,new
  56  0000               _ConfigIOPort:
  60                     ; 60 	GPIOA->ODR &= ~(IO_SW3); //置低	默认为输入口，当改为输出口时，蓝灯亮
  62  0000 72155000      	bres	20480,#2
  63                     ; 61 	GPIOA->ODR &= ~IO_IRCNT; //置低　当改变方向时，直接关闭一个红外管
  65  0004 72175000      	bres	20480,#3
  66                     ; 62 	GPIOA->DDR |= IO_RFCS|IO_IRCNT; //输出模式
  68  0008 c65002        	ld	a,20482
  69  000b aa0a          	or	a,#10
  70  000d c75002        	ld	20482,a
  71                     ; 63 	GPIOA->CR1 |= (IO_RFCS|IO_SW3); //上拉
  73  0010 c65003        	ld	a,20483
  74  0013 aa06          	or	a,#6
  75  0015 c75003        	ld	20483,a
  76                     ; 64 	GPIOA->CR2 |= (IO_SW3); //允许按键中断
  78  0018 72145004      	bset	20484,#2
  79                     ; 65 	GPIOB->ODR |= IO_LED; //置高
  81  001c 72185005      	bset	20485,#4
  82                     ; 66 	GPIOB->DDR |= IO_LED; //输出模式
  84  0020 72185007      	bset	20487,#4
  85                     ; 67 	GPIOB->CR2 |= (IO_INTAUD); 
  87  0024 721a5009      	bset	20489,#5
  88                     ; 68 	GPIOC->ODR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
  90  0028 c6500a        	ld	a,20490
  91  002b aa78          	or	a,#120
  92  002d c7500a        	ld	20490,a
  93                     ; 69 	GPIOC->DDR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
  95  0030 c6500c        	ld	a,20492
  96  0033 aa78          	or	a,#120
  97  0035 c7500c        	ld	20492,a
  98                     ; 70 	GPIOC->CR1 |= IO_SPICLK|IO_SPIMOSI|IO_SPIMISO|IO_AUDRST|IO_AUDCS;
 100  0038 c6500d        	ld	a,20493
 101  003b aaf8          	or	a,#248
 102  003d c7500d        	ld	20493,a
 103                     ; 71 	GPIOD->ODR |= IO_AUDCS;
 105  0040 7216500f      	bset	20495,#3
 106                     ; 72 	GPIOD->DDR |= IO_AUDCS|IO_RFCS|IO_IR;
 108  0044 c65011        	ld	a,20497
 109  0047 aa0a          	or	a,#10
 110  0049 c75011        	ld	20497,a
 111                     ; 73 	GPIOD->CR1 |= IO_SW4|IO_SW2|IO_IR;
 113  004c c65012        	ld	a,20498
 114  004f aa4c          	or	a,#76
 115  0051 c75012        	ld	20498,a
 116                     ; 74 	GPIOD->CR2 |= (IO_SW4|IO_SW2); //允许按键中断
 118  0054 c65013        	ld	a,20499
 119  0057 aa44          	or	a,#68
 120  0059 c75013        	ld	20499,a
 121                     ; 80 }
 124  005c 81            	ret
 177                     ; 103 INT8U EEPROM_write(INT16U uiAddress, INT8U ucData)
 177                     ; 104 {
 178                     .text:	section	.text,new
 179  0000               _EEPROM_write:
 181  0000 89            	pushw	x
 182  0001 89            	pushw	x
 183       00000002      OFST:	set	2
 186                     ; 105     INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress; 
 188  0002 1c4000        	addw	x,#16384
 189  0005 1f01          	ldw	(OFST-1,sp),x
 190                     ; 106     if (eeprom_addr1 > EEPROM_MAX_ADDRESS)                              // 地址错误
 192  0007 1e01          	ldw	x,(OFST-1,sp)
 193  0009 a34281        	cpw	x,#17025
 194  000c 2504          	jrult	L74
 195                     ; 108         return 1;                                            // 
 197  000e a601          	ld	a,#1
 199  0010 2006          	jra	L01
 200  0012               L74:
 201                     ; 113 	if(low_bat )
 203  0012 3d0b          	tnz	_low_bat
 204  0014 2705          	jreq	L35
 205                     ; 115 		return 1;
 207  0016 a601          	ld	a,#1
 209  0018               L01:
 211  0018 5b04          	addw	sp,#4
 212  001a 81            	ret
 213  001b               L35:
 214                     ; 119     EEPROM_PROTECT_DISABLE();                                           // 关闭写保护
 216  001b 35ae5064      	mov	20580,#174
 219  001f 35565064      	mov	20580,#86
 220                     ; 120     *((@far INT8U*) eeprom_addr1) = ucData;                             // 
 222  0023 7b01          	ld	a,(OFST-1,sp)
 223  0025 97            	ld	xl,a
 224  0026 7b02          	ld	a,(OFST+0,sp)
 225  0028 3f00          	clr	c_x
 226  002a 02            	rlwa	x,a
 227  002b 9093          	ldw	y,x
 228  002d 7b07          	ld	a,(OFST+5,sp)
 229  002f bf01          	ldw	c_x+1,x
 230  0031 92bd0000      	ldf	[c_x.e],a
 231                     ; 121     EEPROM_PROTECT_ENABLE();                                            // 开启写保护
 233  0035 7217505f      	bres	20575,#3
 234                     ; 122     return 0;
 236  0039 4f            	clr	a
 238  003a 20dc          	jra	L01
 281                     ; 124 INT8U EEPROM_read(INT16U uiAddress)
 281                     ; 125 {
 282                     .text:	section	.text,new
 283  0000               _EEPROM_read:
 285  0000 89            	pushw	x
 286       00000002      OFST:	set	2
 289                     ; 126     INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress;             //
 291  0001 1c4000        	addw	x,#16384
 292  0004 1f01          	ldw	(OFST-1,sp),x
 293                     ; 127     return  *((@far INT8U*) eeprom_addr1);                              // 读取数据 
 295  0006 7b01          	ld	a,(OFST-1,sp)
 296  0008 97            	ld	xl,a
 297  0009 7b02          	ld	a,(OFST+0,sp)
 298  000b 3f00          	clr	c_x
 299  000d 02            	rlwa	x,a
 300  000e 9093          	ldw	y,x
 301  0010 bf01          	ldw	c_x+1,x
 302  0012 92bc0000      	ldf	a,[c_x.e]
 305  0016 85            	popw	x
 306  0017 81            	ret
 359                     ; 135 INT16U GetADCResult(INT8U ch)
 359                     ; 136 {
 360                     .text:	section	.text,new
 361  0000               _GetADCResult:
 363  0000 88            	push	a
 364  0001 89            	pushw	x
 365       00000002      OFST:	set	2
 368                     ; 138 	ADC1_DeInit();
 370  0002 cd0000        	call	_ADC1_DeInit
 372                     ; 139 	ADC1->CSR|=ch; //选择通道
 374  0005 c65400        	ld	a,21504
 375  0008 1a03          	or	a,(OFST+1,sp)
 376  000a c75400        	ld	21504,a
 377                     ; 140   	ADC1->CR2|=0x08; // 数据排列,右对齐
 379  000d 72165402      	bset	21506,#3
 380                     ; 141 	ADC1->CR2&=~0x02; //SCAN DISABLE
 382  0011 72135402      	bres	21506,#1
 383                     ; 142 	ADC1->CR3&=~0x80;  //DATE BUFFER DISABLE
 385  0015 721f5403      	bres	21507,#7
 386                     ; 143 	ADC1->TDRL=(1<<ch);
 388  0019 7b03          	ld	a,(OFST+1,sp)
 389  001b 5f            	clrw	x
 390  001c 97            	ld	xl,a
 391  001d a601          	ld	a,#1
 392  001f 5d            	tnzw	x
 393  0020 2704          	jreq	L61
 394  0022               L02:
 395  0022 48            	sll	a
 396  0023 5a            	decw	x
 397  0024 26fc          	jrne	L02
 398  0026               L61:
 399  0026 c75407        	ld	21511,a
 400                     ; 144 	ADC1->CR1&=~0x02;
 402  0029 72135401      	bres	21505,#1
 403                     ; 145 	ADC1->CR1|=0x01;
 405  002d 72105401      	bset	21505,#0
 406                     ; 146 	for(i=0;i<200;i++); // 延时一段时间，至少100uS，保证ADC模块的上电完成
 408  0031 5f            	clrw	x
 409  0032 1f01          	ldw	(OFST-1,sp),x
 410  0034               L331:
 414  0034 1e01          	ldw	x,(OFST-1,sp)
 415  0036 1c0001        	addw	x,#1
 416  0039 1f01          	ldw	(OFST-1,sp),x
 419  003b 1e01          	ldw	x,(OFST-1,sp)
 420  003d a300c8        	cpw	x,#200
 421  0040 25f2          	jrult	L331
 422                     ; 147   	ADC1->CSR&=(~0x80);
 424  0042 721f5400      	bres	21504,#7
 425                     ; 148 	ADC1->CR1|=0x01; // 再次将CR1寄存器的最低位置1,开启AD转换
 427  0046 72105401      	bset	21505,#0
 429  004a               L341:
 430                     ; 149   	while((ADC1->CSR & 0x80)==0); // 等待转换结束
 432  004a c65400        	ld	a,21504
 433  004d a580          	bcp	a,#128
 434  004f 27f9          	jreq	L341
 435                     ; 150   	value=ADC1->DRH;
 437  0051 c65404        	ld	a,21508
 438  0054 5f            	clrw	x
 439  0055 97            	ld	xl,a
 440  0056 1f01          	ldw	(OFST-1,sp),x
 441                     ; 151 	value<<=8;
 443  0058 7b02          	ld	a,(OFST+0,sp)
 444  005a 6b01          	ld	(OFST-1,sp),a
 445  005c 0f02          	clr	(OFST+0,sp)
 446                     ; 152   	value|=ADC1->DRL;
 448  005e c65405        	ld	a,21509
 449  0061 5f            	clrw	x
 450  0062 97            	ld	xl,a
 451  0063 01            	rrwa	x,a
 452  0064 1a02          	or	a,(OFST+0,sp)
 453  0066 01            	rrwa	x,a
 454  0067 1a01          	or	a,(OFST-1,sp)
 455  0069 01            	rrwa	x,a
 456  006a 1f01          	ldw	(OFST-1,sp),x
 457                     ; 153 	ADC1->CSR&=(~0x80);
 459  006c 721f5400      	bres	21504,#7
 460                     ; 154 	ADC1->CR1&=~0x01;	
 462  0070 72115401      	bres	21505,#0
 463                     ; 155 	ADC1_DeInit();
 465  0074 cd0000        	call	_ADC1_DeInit
 467                     ; 156   	return value;                 //Return ADC result
 469  0077 1e01          	ldw	x,(OFST-1,sp)
 472  0079 5b03          	addw	sp,#3
 473  007b 81            	ret
 544                     ; 161 uint8 ParaCrcCheck(uint16 addr)
 544                     ; 162 {
 545                     .text:	section	.text,new
 546  0000               _ParaCrcCheck:
 548  0000 89            	pushw	x
 549  0001 5203          	subw	sp,#3
 550       00000003      OFST:	set	3
 553                     ; 163 	uint8 crcResult=0;
 555  0003 0f01          	clr	(OFST-2,sp)
 556                     ; 164 	uint8 crcData =0;
 558  0005 7b03          	ld	a,(OFST+0,sp)
 559  0007 97            	ld	xl,a
 560                     ; 170 	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
 562  0008 0f02          	clr	(OFST-1,sp)
 563  000a               L502:
 564                     ; 172 		paraData = EEPROM_read(addr);
 566  000a 1e04          	ldw	x,(OFST+1,sp)
 567  000c cd0000        	call	_EEPROM_read
 569  000f 6b03          	ld	(OFST+0,sp),a
 570                     ; 173 		crcResult +=paraData; 
 572  0011 7b01          	ld	a,(OFST-2,sp)
 573  0013 1b03          	add	a,(OFST+0,sp)
 574  0015 6b01          	ld	(OFST-2,sp),a
 575                     ; 174 		addr++;
 577  0017 1e04          	ldw	x,(OFST+1,sp)
 578  0019 1c0001        	addw	x,#1
 579  001c 1f04          	ldw	(OFST+1,sp),x
 580                     ; 170 	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
 582  001e 0c02          	inc	(OFST-1,sp)
 585  0020 7b02          	ld	a,(OFST-1,sp)
 586  0022 a106          	cp	a,#6
 587  0024 25e4          	jrult	L502
 588                     ; 176 	crcData = EEPROM_read(addr);
 590  0026 1e04          	ldw	x,(OFST+1,sp)
 591  0028 cd0000        	call	_EEPROM_read
 593  002b 6b03          	ld	(OFST+0,sp),a
 594                     ; 179 	if(crcData == crcResult) return 1;
 596  002d 7b03          	ld	a,(OFST+0,sp)
 597  002f 1101          	cp	a,(OFST-2,sp)
 598  0031 2604          	jrne	L312
 601  0033 a601          	ld	a,#1
 603  0035 2001          	jra	L42
 604  0037               L312:
 605                     ; 180 	else return 0;
 607  0037 4f            	clr	a
 609  0038               L42:
 611  0038 5b05          	addw	sp,#5
 612  003a 81            	ret
 666                     ; 186 uint8 CalcParaCrc(void)
 666                     ; 187 {
 667                     .text:	section	.text,new
 668  0000               _CalcParaCrc:
 670  0000 5204          	subw	sp,#4
 671       00000004      OFST:	set	4
 674                     ; 188 	uint8 crcResult=0;
 676  0002 0f01          	clr	(OFST-3,sp)
 677                     ; 190 	uint8 *addr= &(stEepromCfgData.byInitFlag);
 679  0004 ae002d        	ldw	x,#_stEepromCfgData
 680  0007 1f02          	ldw	(OFST-2,sp),x
 681                     ; 193 	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
 683  0009 0f04          	clr	(OFST+0,sp)
 684  000b               L542:
 685                     ; 195 		crcResult += *addr;
 687  000b 1e02          	ldw	x,(OFST-2,sp)
 688  000d 7b01          	ld	a,(OFST-3,sp)
 689  000f fb            	add	a,(x)
 690  0010 6b01          	ld	(OFST-3,sp),a
 691                     ; 196 		addr++;
 693  0012 1e02          	ldw	x,(OFST-2,sp)
 694  0014 1c0001        	addw	x,#1
 695  0017 1f02          	ldw	(OFST-2,sp),x
 696                     ; 193 	for(i=0;i<sizeof(STEepromCfgData)-1;i++)
 698  0019 0c04          	inc	(OFST+0,sp)
 701  001b 7b04          	ld	a,(OFST+0,sp)
 702  001d a106          	cp	a,#6
 703  001f 25ea          	jrult	L542
 704                     ; 199 	return crcResult;
 706  0021 7b01          	ld	a,(OFST-3,sp)
 709  0023 5b04          	addw	sp,#4
 710  0025 81            	ret
 765                     ; 203 void ReadParameter(uint16 addr)
 765                     ; 204 {
 766                     .text:	section	.text,new
 767  0000               _ReadParameter:
 769  0000 89            	pushw	x
 770  0001 5203          	subw	sp,#3
 771       00000003      OFST:	set	3
 774                     ; 206 	uint8 i=0;
 776  0003 7b03          	ld	a,(OFST+0,sp)
 777  0005 97            	ld	xl,a
 778                     ; 207 	uint8 *pbyData = &(stEepromCfgData.byInitFlag);
 780  0006 ae002d        	ldw	x,#_stEepromCfgData
 781  0009 1f01          	ldw	(OFST-2,sp),x
 782                     ; 210 	for(i=0;i<sizeof(STEepromCfgData);i++)
 784  000b 0f03          	clr	(OFST+0,sp)
 785  000d               L103:
 786                     ; 212 		*pbyData = EEPROM_read(addr);
 788  000d 1e04          	ldw	x,(OFST+1,sp)
 789  000f cd0000        	call	_EEPROM_read
 791  0012 1e01          	ldw	x,(OFST-2,sp)
 792  0014 f7            	ld	(x),a
 793                     ; 213 		pbyData++;
 795  0015 1e01          	ldw	x,(OFST-2,sp)
 796  0017 1c0001        	addw	x,#1
 797  001a 1f01          	ldw	(OFST-2,sp),x
 798                     ; 214 		addr++;
 800  001c 1e04          	ldw	x,(OFST+1,sp)
 801  001e 1c0001        	addw	x,#1
 802  0021 1f04          	ldw	(OFST+1,sp),x
 803                     ; 210 	for(i=0;i<sizeof(STEepromCfgData);i++)
 805  0023 0c03          	inc	(OFST+0,sp)
 808  0025 7b03          	ld	a,(OFST+0,sp)
 809  0027 a107          	cp	a,#7
 810  0029 25e2          	jrult	L103
 811                     ; 216 }
 814  002b 5b05          	addw	sp,#5
 815  002d 81            	ret
 919                     ; 219 bool SaveParameter(uint16 addr)
 919                     ; 220 {  
 920                     .text:	section	.text,new
 921  0000               _SaveParameter:
 923  0000 89            	pushw	x
 924  0001 5208          	subw	sp,#8
 925       00000008      OFST:	set	8
 928                     ; 222 	uint16 addr_temp=addr;
 930  0003 1f03          	ldw	(OFST-5,sp),x
 931                     ; 228 	writetimes =3;
 933  0005 a603          	ld	a,#3
 934  0007 6b02          	ld	(OFST-6,sp),a
 935  0009               L163:
 936                     ; 231 		addr = addr_temp;
 938  0009 1e03          	ldw	x,(OFST-5,sp)
 939  000b 1f09          	ldw	(OFST+1,sp),x
 940                     ; 233 		pbyData=&(stEepromCfgData.byInitFlag);
 942  000d ae002d        	ldw	x,#_stEepromCfgData
 943  0010 1f06          	ldw	(OFST-2,sp),x
 944                     ; 234 		for(i=0;i<sizeof(STEepromCfgData);i++)
 946  0012 0f08          	clr	(OFST+0,sp)
 947  0014               L763:
 948                     ; 236 			EEPROM_write(addr,*pbyData);
 950  0014 1e06          	ldw	x,(OFST-2,sp)
 951  0016 f6            	ld	a,(x)
 952  0017 88            	push	a
 953  0018 1e0a          	ldw	x,(OFST+2,sp)
 954  001a cd0000        	call	_EEPROM_write
 956  001d 84            	pop	a
 957                     ; 237 			pbyData++;
 959  001e 1e06          	ldw	x,(OFST-2,sp)
 960  0020 1c0001        	addw	x,#1
 961  0023 1f06          	ldw	(OFST-2,sp),x
 962                     ; 238 			addr++;
 964  0025 1e09          	ldw	x,(OFST+1,sp)
 965  0027 1c0001        	addw	x,#1
 966  002a 1f09          	ldw	(OFST+1,sp),x
 967                     ; 234 		for(i=0;i<sizeof(STEepromCfgData);i++)
 969  002c 0c08          	inc	(OFST+0,sp)
 972  002e 7b08          	ld	a,(OFST+0,sp)
 973  0030 a107          	cp	a,#7
 974  0032 25e0          	jrult	L763
 975                     ; 242 		readtimes=3;
 977  0034 a603          	ld	a,#3
 978  0036 6b05          	ld	(OFST-3,sp),a
 979  0038               L573:
 980                     ; 245 			pbyData=&(stEepromCfgData.byInitFlag);
 982  0038 ae002d        	ldw	x,#_stEepromCfgData
 983  003b 1f06          	ldw	(OFST-2,sp),x
 984                     ; 246 			addr = addr_temp;
 986  003d 1e03          	ldw	x,(OFST-5,sp)
 987  003f 1f09          	ldw	(OFST+1,sp),x
 988                     ; 249 			for(i=0;i<sizeof(STEepromCfgData);i++)
 990  0041 0f08          	clr	(OFST+0,sp)
 991  0043               L304:
 992                     ; 251 				if(*pbyData != EEPROM_read(addr)) break;
 994  0043 1e09          	ldw	x,(OFST+1,sp)
 995  0045 cd0000        	call	_EEPROM_read
 997  0048 6b01          	ld	(OFST-7,sp),a
 998  004a 1e06          	ldw	x,(OFST-2,sp)
 999  004c f6            	ld	a,(x)
1000  004d 1101          	cp	a,(OFST-7,sp)
1001  004f 2616          	jrne	L704
1004                     ; 253 				pbyData++;
1006  0051 1e06          	ldw	x,(OFST-2,sp)
1007  0053 1c0001        	addw	x,#1
1008  0056 1f06          	ldw	(OFST-2,sp),x
1009                     ; 254 				addr++;
1011  0058 1e09          	ldw	x,(OFST+1,sp)
1012  005a 1c0001        	addw	x,#1
1013  005d 1f09          	ldw	(OFST+1,sp),x
1014                     ; 249 			for(i=0;i<sizeof(STEepromCfgData);i++)
1016  005f 0c08          	inc	(OFST+0,sp)
1019  0061 7b08          	ld	a,(OFST+0,sp)
1020  0063 a107          	cp	a,#7
1021  0065 25dc          	jrult	L304
1022  0067               L704:
1023                     ; 257 			if(i>= sizeof(STEepromCfgData))
1025  0067 7b08          	ld	a,(OFST+0,sp)
1026  0069 a107          	cp	a,#7
1027  006b 2504          	jrult	L773
1028                     ; 261 				return 1;
1030  006d a601          	ld	a,#1
1032  006f 200a          	jra	L43
1033  0071               L773:
1034                     ; 264 		}while(--readtimes);
1036  0071 0a05          	dec	(OFST-3,sp)
1037  0073 26c3          	jrne	L573
1038                     ; 266 		if(readtimes)
1040  0075 0d05          	tnz	(OFST-3,sp)
1041  0077 2705          	jreq	L363
1042                     ; 269 			return 1;
1044  0079 a601          	ld	a,#1
1046  007b               L43:
1048  007b 5b0a          	addw	sp,#10
1049  007d 81            	ret
1050  007e               L363:
1051                     ; 271 	}while(--writetimes);
1053  007e 0a02          	dec	(OFST-6,sp)
1054  0080 2687          	jrne	L163
1055                     ; 273 	return 0;
1057  0082 4f            	clr	a
1059  0083 20f6          	jra	L43
1110                     ; 278 void InitParameter(void)
1110                     ; 279 {
1111                     .text:	section	.text,new
1112  0000               _InitParameter:
1114  0000 89            	pushw	x
1115       00000002      OFST:	set	2
1118                     ; 281 	uint8 readtimes=3;
1120  0001 a603          	ld	a,#3
1121  0003 6b02          	ld	(OFST+0,sp),a
1122                     ; 283 	uint8 result=0;
1124  0005 7b01          	ld	a,(OFST-1,sp)
1125  0007 97            	ld	xl,a
1126  0008               L754:
1127                     ; 289 		if(ParaCrcCheck(IAP0_ADDR) ==1)	 break;
1129  0008 ae0002        	ldw	x,#2
1130  000b cd0000        	call	_ParaCrcCheck
1132  000e a101          	cp	a,#1
1133  0010 2704          	jreq	L364
1136  0012               L164:
1137                     ; 292 	}while(--readtimes);
1139  0012 0a02          	dec	(OFST+0,sp)
1140  0014 26f2          	jrne	L754
1141  0016               L364:
1142                     ; 295 	if(readtimes)
1144  0016 0d02          	tnz	(OFST+0,sp)
1145  0018 271a          	jreq	L764
1146                     ; 297 		if(EEPROM_read(IAP0_ADDR+INIT_INDEX)== 0X5A)
1148  001a ae0002        	ldw	x,#2
1149  001d cd0000        	call	_EEPROM_read
1151  0020 a15a          	cp	a,#90
1152  0022 260c          	jrne	L174
1153                     ; 299 			ReadParameter(IAP0_ADDR);
1155  0024 ae0002        	ldw	x,#2
1156  0027 cd0000        	call	_ReadParameter
1158                     ; 301 			result=1;
1160  002a a601          	ld	a,#1
1161  002c 6b01          	ld	(OFST-1,sp),a
1163  002e 2006          	jra	L574
1164  0030               L174:
1165                     ; 303 		else result =0;
1167  0030 0f01          	clr	(OFST-1,sp)
1168  0032 2002          	jra	L574
1169  0034               L764:
1170                     ; 306 	else result =0;
1172  0034 0f01          	clr	(OFST-1,sp)
1173  0036               L574:
1174                     ; 309 	readtimes=3;
1176  0036 a603          	ld	a,#3
1177  0038 6b02          	ld	(OFST+0,sp),a
1178  003a               L774:
1179                     ; 314 		if(ParaCrcCheck(IAP1_ADDR) ==1)  break;
1181  003a ae0022        	ldw	x,#34
1182  003d cd0000        	call	_ParaCrcCheck
1184  0040 a101          	cp	a,#1
1185  0042 2704          	jreq	L305
1188  0044               L105:
1189                     ; 317 	}while(--readtimes);
1191  0044 0a02          	dec	(OFST+0,sp)
1192  0046 26f2          	jrne	L774
1193  0048               L305:
1194                     ; 320 	if(readtimes)
1196  0048 0d02          	tnz	(OFST+0,sp)
1197  004a 2724          	jreq	L705
1198                     ; 322 		if(EEPROM_read(IAP1_ADDR+INIT_INDEX)== 0X5A)
1200  004c ae0022        	ldw	x,#34
1201  004f cd0000        	call	_EEPROM_read
1203  0052 a15a          	cp	a,#90
1204  0054 2612          	jrne	L115
1205                     ; 324 			if(result==0)
1207  0056 0d01          	tnz	(OFST-1,sp)
1208  0058 2606          	jrne	L315
1209                     ; 326 				ReadParameter(IAP1_ADDR);
1211  005a ae0022        	ldw	x,#34
1212  005d cd0000        	call	_ReadParameter
1214  0060               L315:
1215                     ; 329 			result |= 0x02;
1217  0060 7b01          	ld	a,(OFST-1,sp)
1218  0062 aa02          	or	a,#2
1219  0064 6b01          	ld	(OFST-1,sp),a
1221  0066 200e          	jra	L715
1222  0068               L115:
1223                     ; 331 		else result &= (~0x02);
1225  0068 7b01          	ld	a,(OFST-1,sp)
1226  006a a4fd          	and	a,#253
1227  006c 6b01          	ld	(OFST-1,sp),a
1228  006e 2006          	jra	L715
1229  0070               L705:
1230                     ; 334 	else result &= (~0x02);
1232  0070 7b01          	ld	a,(OFST-1,sp)
1233  0072 a4fd          	and	a,#253
1234  0074 6b01          	ld	(OFST-1,sp),a
1235  0076               L715:
1236                     ; 339 	switch(result)
1238  0076 7b01          	ld	a,(OFST-1,sp)
1240                     ; 367 		default:
1240                     ; 368 			break;
1241  0078 4d            	tnz	a
1242  0079 2708          	jreq	L324
1243  007b 4a            	dec	a
1244  007c 2719          	jreq	L524
1245  007e 4a            	dec	a
1246  007f 271e          	jreq	L724
1247  0081 2022          	jra	L325
1248  0083               L324:
1249                     ; 341 		case 0:
1249                     ; 342 /*			stEepromCfgData.byInitFlag = 0x5a;
1249                     ; 343 			stEepromCfgData.bySec = 254;
1249                     ; 344 			stEepromCfgData.byNum = 0x02;
1249                     ; 345 			stEepromCfgData.bySBed =0x00;
1249                     ; 346 			stEepromCfgData.byEBed =255;
1249                     ; 347 			stEepromCfgData.byVol =8;
1249                     ; 348 			stEepromCfgData.byCrc = CalcParaCrc();
1249                     ; 349 */
1249                     ; 350 			para_error =1;
1251  0083 3501000c      	mov	_para_error,#1
1252                     ; 351 			stEepromCfgData.byVol =8;
1254  0087 35080032      	mov	_stEepromCfgData+5,#8
1255                     ; 352 			WTVxxxInit(15);      //语音芯片初始化
1257  008b a60f          	ld	a,#15
1258  008d cd0000        	call	_WTVxxxInit
1260                     ; 353 			PlaySound(20);		 //失败			
1262  0090 a614          	ld	a,#20
1263  0092 cd0000        	call	_PlaySound
1265                     ; 354 			break;
1267  0095 200e          	jra	L325
1268  0097               L524:
1269                     ; 356 		case 1://0区数据正确，1区数据不正确
1269                     ; 357 			SaveParameter(IAP1_ADDR);
1271  0097 ae0022        	ldw	x,#34
1272  009a cd0000        	call	_SaveParameter
1274                     ; 358 			break;
1276  009d 2006          	jra	L325
1277  009f               L724:
1278                     ; 360 		case 2://1区数据正确，0区数据不正确	
1278                     ; 361 			SaveParameter(IAP0_ADDR);
1280  009f ae0002        	ldw	x,#2
1281  00a2 cd0000        	call	_SaveParameter
1283                     ; 362 			break;
1285  00a5               L134:
1286                     ; 364 		case 3:	//两区数据都正确
1286                     ; 365 			break;
1288  00a5               L334:
1289                     ; 367 		default:
1289                     ; 368 			break;
1291  00a5               L325:
1292                     ; 373 }
1295  00a5 85            	popw	x
1296  00a6 81            	ret
1389                     ; 376 void CheckRecv(void)
1389                     ; 377 {
1390                     .text:	section	.text,new
1391  0000               _CheckRecv:
1393  0000 5206          	subw	sp,#6
1394       00000006      OFST:	set	6
1397                     ; 379 	INT16U cont=0,cont1=0;
1399  0002 1e04          	ldw	x,(OFST-2,sp)
1402  0004 1e01          	ldw	x,(OFST-5,sp)
1403                     ; 380 	DisableRFIrq();
1405  0006 72195013      	bres	20499,#4
1406                     ; 381   	leng=24;
1408  000a a618          	ld	a,#24
1409  000c 6b03          	ld	(OFST-3,sp),a
1410                     ; 382   	if(halRfReceivePacket(RxBuf,&leng))     //读取接收到的无线数据
1412  000e 96            	ldw	x,sp
1413  000f 1c0003        	addw	x,#OFST-3
1414  0012 89            	pushw	x
1415  0013 ae000d        	ldw	x,#_RxBuf
1416  0016 cd0000        	call	_halRfReceivePacket
1418  0019 85            	popw	x
1419  001a 4d            	tnz	a
1420  001b 2603          	jrne	L24
1421  001d cc027c        	jp	L775
1422  0020               L24:
1423                     ; 384 		UART1_SendData8(leng);
1425  0020 7b03          	ld	a,(OFST-3,sp)
1426  0022 cd0000        	call	_UART1_SendData8
1428                     ; 385 		for(i=0;i<leng;i++)
1430  0025 0f06          	clr	(OFST+0,sp)
1432  0027 200b          	jra	L506
1433  0029               L106:
1434                     ; 387 			UART1_SendData8(RxBuf[i]);
1436  0029 7b06          	ld	a,(OFST+0,sp)
1437  002b 5f            	clrw	x
1438  002c 97            	ld	xl,a
1439  002d e60d          	ld	a,(_RxBuf,x)
1440  002f cd0000        	call	_UART1_SendData8
1442                     ; 385 		for(i=0;i<leng;i++)
1444  0032 0c06          	inc	(OFST+0,sp)
1445  0034               L506:
1448  0034 7b06          	ld	a,(OFST+0,sp)
1449  0036 1103          	cp	a,(OFST-3,sp)
1450  0038 25ef          	jrult	L106
1451                     ; 392 		if(leng ==5 )
1453  003a 7b03          	ld	a,(OFST-3,sp)
1454  003c a105          	cp	a,#5
1455  003e 267a          	jrne	L116
1456                     ; 394 			if(stEepromCfgData.bySec ==254)
1458  0040 b62e          	ld	a,_stEepromCfgData+1
1459  0042 a1fe          	cp	a,#254
1460  0044 2703          	jreq	L44
1461  0046 cc027c        	jp	L775
1462  0049               L44:
1463                     ; 397 				if((RxBuf[0]==0xff) && (RxBuf[1]==0xff) && (RxBuf[2]==0x01) &&  (RxBuf[3]==0x32) && (RxBuf[4]==0x00))
1465  0049 b60d          	ld	a,_RxBuf
1466  004b a1ff          	cp	a,#255
1467  004d 2703          	jreq	L64
1468  004f cc027c        	jp	L775
1469  0052               L64:
1471  0052 b60e          	ld	a,_RxBuf+1
1472  0054 a1ff          	cp	a,#255
1473  0056 2703          	jreq	L05
1474  0058 cc027c        	jp	L775
1475  005b               L05:
1477  005b b60f          	ld	a,_RxBuf+2
1478  005d a101          	cp	a,#1
1479  005f 2703          	jreq	L25
1480  0061 cc027c        	jp	L775
1481  0064               L25:
1483  0064 b610          	ld	a,_RxBuf+3
1484  0066 a132          	cp	a,#50
1485  0068 2703          	jreq	L45
1486  006a cc027c        	jp	L775
1487  006d               L45:
1489  006d 3d11          	tnz	_RxBuf+4
1490  006f 2703          	jreq	L65
1491  0071 cc027c        	jp	L775
1492  0074               L65:
1493                     ; 399 	  				SetLEDMode(1);			//输出模式，蓝灯亮
1495  0074 72145002      	bset	20482,#2
1496                     ; 402 					TxBuf[0]=0xff;
1499  0078 35ff0025      	mov	_TxBuf,#255
1500                     ; 403 					TxBuf[1]=0xff;  		 
1502  007c 35ff0026      	mov	_TxBuf+1,#255
1503                     ; 404 					TxBuf[2]=0x01;
1505  0080 35010027      	mov	_TxBuf+2,#1
1506                     ; 405 					TxBuf[3]=0x32;
1508  0084 35320028      	mov	_TxBuf+3,#50
1509                     ; 406 					TxBuf[4]= CC1101SigValue;
1511  0088 450a29        	mov	_TxBuf+4,_CC1101SigValue
1512                     ; 408 					halSpiStrobe(CCxxx0_SIDLE);
1514  008b a636          	ld	a,#54
1515  008d cd0000        	call	_halSpiStrobe
1517                     ; 409 					halRfSendPacket(TxBuf,5);//发送回复命令数据
1519  0090 4b05          	push	#5
1520  0092 ae0025        	ldw	x,#_TxBuf
1521  0095 cd0000        	call	_halRfSendPacket
1523  0098 84            	pop	a
1524                     ; 410 					Delayms(100);
1526  0099 ae0064        	ldw	x,#100
1527  009c cd0000        	call	_Delayms
1529                     ; 411 					halRfSendPacket(TxBuf,5);//发送回复命令数据
1531  009f 4b05          	push	#5
1532  00a1 ae0025        	ldw	x,#_TxBuf
1533  00a4 cd0000        	call	_halRfSendPacket
1535  00a7 84            	pop	a
1536                     ; 413 					WTVxxxInit(stEepromCfgData.byVol);	   //语音芯片初始化
1538  00a8 b632          	ld	a,_stEepromCfgData+5
1539  00aa cd0000        	call	_WTVxxxInit
1541                     ; 414 					PlaySound(0x0e);
1543  00ad a60e          	ld	a,#14
1544  00af cd0000        	call	_PlaySound
1546                     ; 416 					SetLEDMode(0);			//输入模式，蓝灯灭					
1548  00b2 72155002      	bres	20482,#2
1550  00b6 ac7c027c      	jpf	L775
1551  00ba               L116:
1552                     ; 421 		else if (leng>7)
1554  00ba 7b03          	ld	a,(OFST-3,sp)
1555  00bc a108          	cp	a,#8
1556  00be 2403          	jruge	L06
1557  00c0 cc027c        	jp	L775
1558  00c3               L06:
1559                     ; 423 			halSpiStrobe(CCxxx0_SPWD);	//进入掉电状态
1561  00c3 a639          	ld	a,#57
1562  00c5 cd0000        	call	_halSpiStrobe
1564                     ; 425   			SetLEDMode(1);			//输出模式，蓝灯亮
1566  00c8 72145002      	bset	20482,#2
1567                     ; 427 			i=0;
1570  00cc 7b06          	ld	a,(OFST+0,sp)
1571  00ce 97            	ld	xl,a
1572                     ; 428 	  		if(RxBuf[7]==0xaa)
1574  00cf b614          	ld	a,_RxBuf+7
1575  00d1 a1aa          	cp	a,#170
1576  00d3 2703          	jreq	L26
1577  00d5 cc0278        	jp	L326
1578  00d8               L26:
1579                     ; 430 	  			switch(RxBuf[3])                    //判别接收命令
1581  00d8 b610          	ld	a,_RxBuf+3
1583                     ; 522 				default: 
1583                     ; 523 					break;
1584  00da a006          	sub	a,#6
1585  00dc 2603          	jrne	L46
1586  00de cc01dd        	jp	L135
1587  00e1               L46:
1588  00e1 4a            	dec	a
1589  00e2 2603          	jrne	L66
1590  00e4 cc01f2        	jp	L335
1591  00e7               L66:
1592  00e7 4a            	dec	a
1593  00e8 2603          	jrne	L07
1594  00ea cc0204        	jp	L535
1595  00ed               L07:
1596  00ed a052          	sub	a,#82
1597  00ef 270b          	jreq	L525
1598  00f1 a003          	sub	a,#3
1599  00f3 2603          	jrne	L27
1600  00f5 cc01d6        	jp	L725
1601  00f8               L27:
1602  00f8 ac780278      	jpf	L326
1603  00fc               L525:
1604                     ; 432 					case 0x5a:                       // 编号
1604                     ; 433 		     		if(RxBuf[1]==251)
1606  00fc b60e          	ld	a,_RxBuf+1
1607  00fe a1fb          	cp	a,#251
1608  0100 2703          	jreq	L47
1609  0102 cc0278        	jp	L326
1610  0105               L47:
1611                     ; 435 						SetLEDMode(0);			//sw3输入模式
1613  0105 72155002      	bres	20482,#2
1614                     ; 436 						Delayms(1);
1617  0109 ae0001        	ldw	x,#1
1618  010c cd0000        	call	_Delayms
1620                     ; 437 						if(GetSW2()==0 || GetSW3()==0)
1622  010f c65010        	ld	a,20496
1623  0112 a540          	bcp	a,#64
1624  0114 270a          	jreq	L536
1626  0116 c65001        	ld	a,20481
1627  0119 a504          	bcp	a,#4
1628  011b 2703          	jreq	L67
1629  011d cc0278        	jp	L326
1630  0120               L67:
1631  0120               L536:
1632                     ; 440 							SetLEDMode(1);			//输出模式，蓝灯亮
1634  0120 72145002      	bset	20482,#2
1635                     ; 442 				      		cont=0;cont1=0;
1638  0124 5f            	clrw	x
1639  0125 1f04          	ldw	(OFST-2,sp),x
1642  0127 5f            	clrw	x
1643  0128 1f01          	ldw	(OFST-5,sp),x
1644  012a               L736:
1645                     ; 445 								cont++;
1647  012a 1e04          	ldw	x,(OFST-2,sp)
1648  012c 1c0001        	addw	x,#1
1649  012f 1f04          	ldw	(OFST-2,sp),x
1650                     ; 446 								if (cont==500)		//0.5秒
1652  0131 1e04          	ldw	x,(OFST-2,sp)
1653  0133 a301f4        	cpw	x,#500
1654  0136 2604          	jrne	L346
1655                     ; 448 									SetLEDMode(0);	//输入模式，蓝灯灭
1657  0138 72155002      	bres	20482,#2
1659  013c               L346:
1660                     ; 450 	              				if(cont>1000)
1662  013c 1e04          	ldw	x,(OFST-2,sp)
1663  013e a303e9        	cpw	x,#1001
1664  0141 250e          	jrult	L546
1665                     ; 452 									cont=0;
1667  0143 5f            	clrw	x
1668  0144 1f04          	ldw	(OFST-2,sp),x
1669                     ; 453 									SetLEDMode(1);	//输出模式，蓝灯亮	
1671  0146 72145002      	bset	20482,#2
1672                     ; 454 									cont1++;
1675  014a 1e01          	ldw	x,(OFST-5,sp)
1676  014c 1c0001        	addw	x,#1
1677  014f 1f01          	ldw	(OFST-5,sp),x
1678  0151               L546:
1679                     ; 456 							  	if(cont1>10){break;}	//5秒时间到，退出编号状态
1681  0151 1e01          	ldw	x,(OFST-5,sp)
1682  0153 a3000b        	cpw	x,#11
1683  0156 2503          	jrult	L001
1684  0158 cc0278        	jp	L326
1685  015b               L001:
1688                     ; 457 							  	if(GetSW4()==0)
1690  015b c65010        	ld	a,20496
1691  015e a504          	bcp	a,#4
1692  0160 266a          	jrne	L156
1693                     ; 459 									stEepromCfgData.byInitFlag =0x5a;
1695  0162 355a002d      	mov	_stEepromCfgData,#90
1696                     ; 460 									stEepromCfgData.bySec = RxBuf[0];	//区号
1698  0166 450d2e        	mov	_stEepromCfgData+1,_RxBuf
1699                     ; 461 									stEepromCfgData.byNum = RxBuf[2];	//移动分机号
1701  0169 450f2f        	mov	_stEepromCfgData+2,_RxBuf+2
1702                     ; 462 									stEepromCfgData.bySBed = RxBuf[4];	//起始床号
1704  016c 451130        	mov	_stEepromCfgData+3,_RxBuf+4
1705                     ; 463 									stEepromCfgData.byEBed = RxBuf[5];	//结束床号
1707  016f 451231        	mov	_stEepromCfgData+4,_RxBuf+5
1708                     ; 464 									stEepromCfgData.byCrc = CalcParaCrc();
1710  0172 cd0000        	call	_CalcParaCrc
1712  0175 b733          	ld	_stEepromCfgData+6,a
1713                     ; 466 									i=SaveParameter(IAP0_ADDR);
1715  0177 7b06          	ld	a,(OFST+0,sp)
1716  0179 97            	ld	xl,a
1717  017a ae0002        	ldw	x,#2
1718  017d cd0000        	call	_SaveParameter
1720                     ; 467 									i=SaveParameter(IAP1_ADDR);
1722  0180 ae0022        	ldw	x,#34
1723  0183 cd0000        	call	_SaveParameter
1725  0186 6b06          	ld	(OFST+0,sp),a
1726                     ; 468 									WTVxxxInit(stEepromCfgData.byVol);      //语音芯片初始化
1728  0188 b632          	ld	a,_stEepromCfgData+5
1729  018a cd0000        	call	_WTVxxxInit
1731                     ; 470 									if(i)
1733  018d 0d06          	tnz	(OFST+0,sp)
1734  018f 2716          	jreq	L356
1735                     ; 472 				    					PlaySound(19);//成功 
1737  0191 a613          	ld	a,#19
1738  0193 cd0000        	call	_PlaySound
1740                     ; 474 										seng_tx_answer(0x5a);
1742  0196 a65a          	ld	a,#90
1743  0198 cd0000        	call	_seng_tx_answer
1745                     ; 475 										halSpiStrobe(CCxxx0_SIDLE);
1747  019b a636          	ld	a,#54
1748  019d cd0000        	call	_halSpiStrobe
1750                     ; 476 										halSpiStrobe(CCxxx0_SFTX);										
1752  01a0 a63b          	ld	a,#59
1753  01a2 cd0000        	call	_halSpiStrobe
1756  01a5 2005          	jra	L556
1757  01a7               L356:
1758                     ; 481 										PlaySound(20);
1760  01a7 a614          	ld	a,#20
1761  01a9 cd0000        	call	_PlaySound
1763  01ac               L556:
1764                     ; 483 									i=0;
1766  01ac 0f06          	clr	(OFST+0,sp)
1768  01ae 2011          	jra	L366
1769  01b0               L756:
1770                     ; 484 								  	while(GetSW4()==0) {Delayms(20); i++; if(i>=200) break;};	//等待按键松开
1772  01b0 ae0014        	ldw	x,#20
1773  01b3 cd0000        	call	_Delayms
1777  01b6 0c06          	inc	(OFST+0,sp)
1780  01b8 7b06          	ld	a,(OFST+0,sp)
1781  01ba a1c8          	cp	a,#200
1782  01bc 2503          	jrult	L201
1783  01be cc0278        	jp	L326
1784  01c1               L201:
1787  01c1               L366:
1790  01c1 c65010        	ld	a,20496
1791  01c4 a504          	bcp	a,#4
1792  01c6 27e8          	jreq	L756
1793  01c8 ac780278      	jpf	L326
1794  01cc               L156:
1795                     ; 487 								Delayms(1);
1797  01cc ae0001        	ldw	x,#1
1798  01cf cd0000        	call	_Delayms
1801  01d2 ac2a012a      	jpf	L736
1802  01d6               L725:
1803                     ; 492 				case 0x5d:
1803                     ; 493 					seng_tx_para();
1805  01d6 cd0000        	call	_seng_tx_para
1807                     ; 494 					break;
1809  01d9 ac780278      	jpf	L326
1810  01dd               L135:
1811                     ; 496 				case 0x06:                 	//床头呼叫
1811                     ; 497 					if (((RxBuf[2]<stEepromCfgData.bySBed)||(RxBuf[2]>stEepromCfgData.byEBed))&&RxBuf[1]!=250) break;		//加床分机没有界限全部处理
1813  01dd b60f          	ld	a,_RxBuf+2
1814  01df b130          	cp	a,_stEepromCfgData+3
1815  01e1 2506          	jrult	L376
1817  01e3 b60f          	ld	a,_RxBuf+2
1818  01e5 b131          	cp	a,_stEepromCfgData+4
1819  01e7 2309          	jrule	L335
1820  01e9               L376:
1822  01e9 b60e          	ld	a,_RxBuf+1
1823  01eb a1fa          	cp	a,#250
1824  01ed 2703          	jreq	L401
1825  01ef cc0278        	jp	L326
1826  01f2               L401:
1829  01f2               L335:
1830                     ; 498 				case 0x07:					//输液
1830                     ; 499 					if (((RxBuf[2]<stEepromCfgData.bySBed)||(RxBuf[2]>stEepromCfgData.byEBed))&&RxBuf[1]!=250) break;
1832  01f2 b60f          	ld	a,_RxBuf+2
1833  01f4 b130          	cp	a,_stEepromCfgData+3
1834  01f6 2506          	jrult	L776
1836  01f8 b60f          	ld	a,_RxBuf+2
1837  01fa b131          	cp	a,_stEepromCfgData+4
1838  01fc 2306          	jrule	L535
1839  01fe               L776:
1841  01fe b60e          	ld	a,_RxBuf+1
1842  0200 a1fa          	cp	a,#250
1843  0202 2674          	jrne	L326
1846  0204               L535:
1847                     ; 500 				case 0x08:                 	//卫生间呼叫
1847                     ; 501 					if (RxBuf[1]==251) break;	//移动分机不处理
1849  0204 b60e          	ld	a,_RxBuf+1
1850  0206 a1fb          	cp	a,#251
1851  0208 276e          	jreq	L326
1854                     ; 502 			  		if(stEepromCfgData.bySec==RxBuf[0])
1856  020a b62e          	ld	a,_stEepromCfgData+1
1857  020c b10d          	cp	a,_RxBuf
1858  020e 2668          	jrne	L326
1859                     ; 504 						for (i=0;i<4;i++)
1861  0210 0f06          	clr	(OFST+0,sp)
1862  0212               L507:
1863                     ; 506 							if (RxBufBak[i]!=RxBuf[i]) break;
1865  0212 7b06          	ld	a,(OFST+0,sp)
1866  0214 5f            	clrw	x
1867  0215 97            	ld	xl,a
1868  0216 7b06          	ld	a,(OFST+0,sp)
1869  0218 905f          	clrw	y
1870  021a 9097          	ld	yl,a
1871  021c 90e609        	ld	a,(_RxBufBak,y)
1872  021f e10d          	cp	a,(_RxBuf,x)
1873  0221 2608          	jrne	L117
1876                     ; 504 						for (i=0;i<4;i++)
1878  0223 0c06          	inc	(OFST+0,sp)
1881  0225 7b06          	ld	a,(OFST+0,sp)
1882  0227 a104          	cp	a,#4
1883  0229 25e7          	jrult	L507
1884  022b               L117:
1885                     ; 508 						if (play_end==play_start||i<4)
1887  022b b600          	ld	a,_play_end
1888  022d b100          	cp	a,_play_start
1889  022f 2706          	jreq	L717
1891  0231 7b06          	ld	a,(OFST+0,sp)
1892  0233 a104          	cp	a,#4
1893  0235 2441          	jruge	L326
1894  0237               L717:
1895                     ; 510 							calling=1;
1897  0237 35010008      	mov	_calling,#1
1898                     ; 511 							if (play_end==play_start) WTVxxxInit(stEepromCfgData.byVol);      //语音芯片初始化
1900  023b b600          	ld	a,_play_end
1901  023d b100          	cp	a,_play_start
1902  023f 2607          	jrne	L127
1905  0241 b632          	ld	a,_stEepromCfgData+5
1906  0243 cd0000        	call	_WTVxxxInit
1909  0246 2003          	jra	L327
1910  0248               L127:
1911                     ; 512 							else StopPlay();
1913  0248 cd0000        	call	_StopPlay
1915  024b               L327:
1916                     ; 513 							for (i=0;i<4;i++) RxBufBak[i] = RxBuf[i];		//将数据备份
1918  024b 0f06          	clr	(OFST+0,sp)
1919  024d               L527:
1922  024d 7b06          	ld	a,(OFST+0,sp)
1923  024f 5f            	clrw	x
1924  0250 97            	ld	xl,a
1925  0251 e60d          	ld	a,(_RxBuf,x)
1926  0253 e709          	ld	(_RxBufBak,x),a
1929  0255 0c06          	inc	(OFST+0,sp)
1932  0257 7b06          	ld	a,(OFST+0,sp)
1933  0259 a104          	cp	a,#4
1934  025b 25f0          	jrult	L527
1935                     ; 514 							for(k=0;k<3;k++)
1937  025d 0f06          	clr	(OFST+0,sp)
1938  025f               L337:
1939                     ; 516 								PlaySpeak(RxBuf[1],RxBuf[2],RxBuf[3]);
1941  025f 3b0010        	push	_RxBuf+3
1942  0262 b60f          	ld	a,_RxBuf+2
1943  0264 97            	ld	xl,a
1944  0265 b60e          	ld	a,_RxBuf+1
1945  0267 95            	ld	xh,a
1946  0268 cd0000        	call	_PlaySpeak
1948  026b 84            	pop	a
1949                     ; 517 								SetLEDMode(0);
1951  026c 72155002      	bres	20482,#2
1952                     ; 514 							for(k=0;k<3;k++)
1955  0270 0c06          	inc	(OFST+0,sp)
1958  0272 7b06          	ld	a,(OFST+0,sp)
1959  0274 a103          	cp	a,#3
1960  0276 25e7          	jrult	L337
1961  0278               L735:
1962                     ; 522 				default: 
1962                     ; 523 					break;
1964  0278               L726:
1965  0278               L326:
1966                     ; 526 			SetLEDMode(0);//输入模式，蓝灯灭
1968  0278 72155002      	bres	20482,#2
1970  027c               L775:
1971                     ; 536 	StartRxWor();
1973  027c cd0000        	call	_StartRxWor
1975                     ; 540 	rfirq=0;
1977  027f 3f04          	clr	_rfirq
1978                     ; 541 	EnableRFIrq();
1980  0281 72185013      	bset	20499,#4
1981                     ; 542 }
1984  0285 5b06          	addw	sp,#6
1985  0287 81            	ret
2014                     ; 547 void timer4_init(void)
2014                     ; 548 {
2015                     .text:	section	.text,new
2016  0000               _timer4_init:
2020                     ; 549 	TIM4_DeInit();		//恢复相关寄存器默认值
2022  0000 cd0000        	call	_TIM4_DeInit
2024                     ; 557 	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 69);//初始化预分频器及自动装载值
2026  0003 ae0045        	ldw	x,#69
2027  0006 a607          	ld	a,#7
2028  0008 95            	ld	xh,a
2029  0009 cd0000        	call	_TIM4_TimeBaseInit
2031                     ; 564 	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	//中断配置:允许
2033  000c ae0001        	ldw	x,#1
2034  000f a601          	ld	a,#1
2035  0011 95            	ld	xh,a
2036  0012 cd0000        	call	_TIM4_ITConfig
2038                     ; 565 	TIM4_UpdateDisableConfig(DISABLE);		//允许更新事件
2040  0015 4f            	clr	a
2041  0016 cd0000        	call	_TIM4_UpdateDisableConfig
2043                     ; 566 	TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);	//溢出产生更新中断
2045  0019 a601          	ld	a,#1
2046  001b cd0000        	call	_TIM4_UpdateRequestConfig
2048                     ; 567 	TIM4->EGR=0x01;//允许产生更新事件
2050  001e 35015345      	mov	21317,#1
2051                     ; 569 	TIM4_Cmd(ENABLE);		//启用定时器4
2053  0022 a601          	ld	a,#1
2054  0024 cd0000        	call	_TIM4_Cmd
2056                     ; 573 }
2059  0027 81            	ret
2152                     ; 579 void main(void)
2152                     ; 580 {
2153                     .text:	section	.text,new
2154  0000               _main:
2156  0000 5203          	subw	sp,#3
2157       00000003      OFST:	set	3
2160                     ; 582   	unsigned char i=0,k=0;
2162  0002 0f01          	clr	(OFST-2,sp)
2165  0004 7b02          	ld	a,(OFST-1,sp)
2166  0006 97            	ld	xl,a
2167                     ; 583 	unsigned char index=0;
2169  0007 7b03          	ld	a,(OFST+0,sp)
2170  0009 97            	ld	xl,a
2171                     ; 586 	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);	//Fhsi = Fhsi rc/1=16M
2173  000a 4f            	clr	a
2174  000b cd0000        	call	_CLK_SYSCLKConfig
2176                     ; 587 	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);	//Fcpu = Fhsi/4 =4M
2178  000e a682          	ld	a,#130
2179  0010 cd0000        	call	_CLK_SYSCLKConfig
2181                     ; 589 	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
2183  0013 5f            	clrw	x
2184  0014 4f            	clr	a
2185  0015 95            	ld	xh,a
2186  0016 cd0000        	call	_CLK_PeripheralClockConfig
2188                     ; 590 	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
2190  0019 5f            	clrw	x
2191  001a a607          	ld	a,#7
2192  001c 95            	ld	xh,a
2193  001d cd0000        	call	_CLK_PeripheralClockConfig
2195                     ; 593 	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
2197  0020 5f            	clrw	x
2198  0021 a603          	ld	a,#3
2199  0023 95            	ld	xh,a
2200  0024 cd0000        	call	_CLK_PeripheralClockConfig
2202                     ; 600 	ConfigIOPort();
2204  0027 cd0000        	call	_ConfigIOPort
2206                     ; 610 	ADC1_ITConfig(ADC1_IT_EOC,DISABLE);
2208  002a 4b00          	push	#0
2209  002c ae0080        	ldw	x,#128
2210  002f cd0000        	call	_ADC1_ITConfig
2212  0032 84            	pop	a
2213                     ; 613 	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD,EXTI_SENSITIVITY_FALL_ONLY);
2215  0033 ae0002        	ldw	x,#2
2216  0036 a603          	ld	a,#3
2217  0038 95            	ld	xh,a
2218  0039 cd0000        	call	_EXTI_SetExtIntSensitivity
2220                     ; 614 	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);
2222  003c ae0002        	ldw	x,#2
2223  003f 4f            	clr	a
2224  0040 95            	ld	xh,a
2225  0041 cd0000        	call	_EXTI_SetExtIntSensitivity
2227                     ; 615 	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_RISE_ONLY);
2229  0044 ae0001        	ldw	x,#1
2230  0047 a601          	ld	a,#1
2231  0049 95            	ld	xh,a
2232  004a cd0000        	call	_EXTI_SetExtIntSensitivity
2234                     ; 616 	SPI_Init(SPI_FIRSTBIT_MSB,SPI_BAUDRATEPRESCALER_8,SPI_MODE_MASTER,SPI_CLOCKPOLARITY_LOW,SPI_CLOCKPHASE_1EDGE,SPI_DATADIRECTION_2LINES_FULLDUPLEX,SPI_NSS_SOFT,0);
2236  004d 4b00          	push	#0
2237  004f 4b02          	push	#2
2238  0051 4b00          	push	#0
2239  0053 4b00          	push	#0
2240  0055 4b00          	push	#0
2241  0057 4b04          	push	#4
2242  0059 ae0010        	ldw	x,#16
2243  005c 4f            	clr	a
2244  005d 95            	ld	xh,a
2245  005e cd0000        	call	_SPI_Init
2247  0061 5b06          	addw	sp,#6
2248                     ; 617 	SPI_Cmd(ENABLE);
2250  0063 a601          	ld	a,#1
2251  0065 cd0000        	call	_SPI_Cmd
2253                     ; 638 	IrDi_init();
2255  0068 cd0000        	call	_IrDi_init
2257                     ; 642 	enableInterrupts();
2260  006b 9a            rim
2262                     ; 647 	power_rec[0] = GetADCResult(5);
2265  006c a605          	ld	a,#5
2266  006e cd0000        	call	_GetADCResult
2268  0071 bf00          	ldw	_power_rec,x
2269                     ; 652 	SetLEDMode(1);				//输出模式，蓝灯亮
2271  0073 72145002      	bset	20482,#2
2272                     ; 653 	CC1101_Init();              //cc1101初始化
2275  0077 cd0000        	call	_CC1101_Init
2277                     ; 655 	Init_WOR();					//可唤醒
2279  007a cd0000        	call	_Init_WOR
2281                     ; 657 	halSpiStrobe(CCxxx0_SPWD);	//进入掉电状态
2283  007d a639          	ld	a,#57
2284  007f cd0000        	call	_halSpiStrobe
2286                     ; 659 	InitParameter();
2288  0082 cd0000        	call	_InitParameter
2290                     ; 670 	StartRxWor();
2292  0085 cd0000        	call	_StartRxWor
2294                     ; 675 	SetLEDMode(0);				//输入模式，蓝灯亮
2296  0088 72155002      	bres	20482,#2
2297                     ; 676 	EnableRFIrq();
2300  008c 72185013      	bset	20499,#4
2301                     ; 679 	index=0;
2303  0090 0f03          	clr	(OFST+0,sp)
2304                     ; 680 	power_rec[index] = GetADCResult(5);            
2306  0092 a605          	ld	a,#5
2307  0094 cd0000        	call	_GetADCResult
2309  0097 bf00          	ldw	_power_rec,x
2310                     ; 681 	index++;
2312  0099 0c03          	inc	(OFST+0,sp)
2313                     ; 682 	power_rec[3]=power_rec[2]=power_rec[1]=power_rec[0];
2315  009b be00          	ldw	x,_power_rec
2316  009d bf02          	ldw	_power_rec+2,x
2317  009f be02          	ldw	x,_power_rec+2
2318  00a1 bf04          	ldw	_power_rec+4,x
2319  00a3 be04          	ldw	x,_power_rec+4
2320  00a5 bf06          	ldw	_power_rec+6,x
2321                     ; 686 	seng_tx_para();
2323  00a7 cd0000        	call	_seng_tx_para
2325  00aa               L5001:
2326                     ; 692 		if(GetIrState(IR_REPEAT))
2328  00aa b600          	ld	a,_IrState
2329  00ac a508          	bcp	a,#8
2330  00ae 2707          	jreq	L1101
2331                     ; 694 			ClrIrState(IR_REPEAT);
2333  00b0 72170000      	bres	_IrState,#3
2334                     ; 695 			Ir_send_repeat();
2336  00b4 cd0000        	call	_Ir_send_repeat
2338  00b7               L1101:
2339                     ; 699 		if (audio_irq)
2341  00b7 3d05          	tnz	_audio_irq
2342  00b9 2705          	jreq	L3101
2343                     ; 701 			CheckPlay();
2345  00bb cd0000        	call	_CheckPlay
2347                     ; 702 			audio_irq=0;
2349  00be 3f05          	clr	_audio_irq
2350  00c0               L3101:
2351                     ; 707 		if(++check_bat_cnt == 2000)
2353  00c0 be02          	ldw	x,_check_bat_cnt
2354  00c2 1c0001        	addw	x,#1
2355  00c5 bf02          	ldw	_check_bat_cnt,x
2356  00c7 a307d0        	cpw	x,#2000
2357  00ca 263e          	jrne	L5101
2358                     ; 709 			power_rec[index] = GetADCResult(5);
2360  00cc a605          	ld	a,#5
2361  00ce cd0000        	call	_GetADCResult
2363  00d1 7b03          	ld	a,(OFST+0,sp)
2364  00d3 905f          	clrw	y
2365  00d5 9097          	ld	yl,a
2366  00d7 9058          	sllw	y
2367  00d9 90ef00        	ldw	(_power_rec,y),x
2368                     ; 710 			index = (index+1)%3;
2370  00dc 7b03          	ld	a,(OFST+0,sp)
2371  00de 5f            	clrw	x
2372  00df 97            	ld	xl,a
2373  00e0 5c            	incw	x
2374  00e1 a603          	ld	a,#3
2375  00e3 cd0000        	call	c_smodx
2377  00e6 9f            	ld	a,xl
2378  00e7 6b03          	ld	(OFST+0,sp),a
2379                     ; 711 			power_rec[3] = (power_rec[2]+power_rec[1]+power_rec[0])/3;		// 3次电压的平均值
2381  00e9 be04          	ldw	x,_power_rec+4
2382  00eb 72bb0002      	addw	x,_power_rec+2
2383  00ef 72bb0000      	addw	x,_power_rec
2384  00f3 a603          	ld	a,#3
2385  00f5 62            	div	x,a
2386  00f6 bf06          	ldw	_power_rec+6,x
2387                     ; 731 			if (power_rec[3]<0x24e) //<3.6v	
2389  00f8 be06          	ldw	x,_power_rec+6
2390  00fa a3024e        	cpw	x,#590
2391  00fd 2406          	jruge	L7101
2392                     ; 733 				low_bat=1;
2394  00ff 3501000b      	mov	_low_bat,#1
2396  0103 2002          	jra	L1201
2397  0105               L7101:
2398                     ; 737 				low_bat=0;
2400  0105 3f0b          	clr	_low_bat
2401  0107               L1201:
2402                     ; 739 			check_bat_cnt =0;
2404  0107 5f            	clrw	x
2405  0108 bf02          	ldw	_check_bat_cnt,x
2406  010a               L5101:
2407                     ; 744 		if (rfirq!=0)
2409  010a 3d04          	tnz	_rfirq
2410  010c 2703          	jreq	L3201
2411                     ; 746 			CheckRecv();
2413  010e cd0000        	call	_CheckRecv
2415  0111               L3201:
2416                     ; 749 		if ((i=key_in())!=0)
2418  0111 cd0000        	call	_key_in
2420  0114 6b01          	ld	(OFST-2,sp),a
2421  0116 2603          	jrne	L211
2422  0118 cc0224        	jp	L5201
2423  011b               L211:
2424                     ; 751 			switch(i)
2426  011b 7b01          	ld	a,(OFST-2,sp)
2428                     ; 842 				break;
2429  011d a002          	sub	a,#2
2430  011f 275a          	jreq	L357
2431  0121 4a            	dec	a
2432  0122 270a          	jreq	L157
2433  0124 4a            	dec	a
2434  0125 2603          	jrne	L411
2435  0127 cc01c8        	jp	L557
2436  012a               L411:
2437  012a ac240224      	jpf	L5201
2438  012e               L157:
2439                     ; 753 			case 3: 
2439                     ; 754 				if(stEepromCfgData.byVol>2)               //音量-
2441  012e b632          	ld	a,_stEepromCfgData+5
2442  0130 a103          	cp	a,#3
2443  0132 2539          	jrult	L3301
2444                     ; 756 					stEepromCfgData.byVol-=2;
2446  0134 3a32          	dec	_stEepromCfgData+5
2447  0136 3a32          	dec	_stEepromCfgData+5
2448                     ; 757 					if (stEepromCfgData.byVol>15) stEepromCfgData.byVol=0;
2450  0138 b632          	ld	a,_stEepromCfgData+5
2451  013a a110          	cp	a,#16
2452  013c 2502          	jrult	L5301
2455  013e 3f32          	clr	_stEepromCfgData+5
2456  0140               L5301:
2457                     ; 758 					stEepromCfgData.byCrc = CalcParaCrc();
2459  0140 cd0000        	call	_CalcParaCrc
2461  0143 b733          	ld	_stEepromCfgData+6,a
2462                     ; 760 					EEPROM_write(IAP0_ADDR+VOL_INDEX,stEepromCfgData.byVol);
2464  0145 3b0032        	push	_stEepromCfgData+5
2465  0148 ae0007        	ldw	x,#7
2466  014b cd0000        	call	_EEPROM_write
2468  014e 84            	pop	a
2469                     ; 761 					EEPROM_write(IAP1_ADDR+VOL_INDEX,stEepromCfgData.byVol);
2471  014f 3b0032        	push	_stEepromCfgData+5
2472  0152 ae0027        	ldw	x,#39
2473  0155 cd0000        	call	_EEPROM_write
2475  0158 84            	pop	a
2476                     ; 762 					EEPROM_write(IAP0_ADDR+CRC_INDEX,stEepromCfgData.byCrc);
2478  0159 3b0033        	push	_stEepromCfgData+6
2479  015c ae0008        	ldw	x,#8
2480  015f cd0000        	call	_EEPROM_write
2482  0162 84            	pop	a
2483                     ; 763 					EEPROM_write(IAP1_ADDR+CRC_INDEX,stEepromCfgData.byCrc);					
2485  0163 3b0033        	push	_stEepromCfgData+6
2486  0166 ae0028        	ldw	x,#40
2487  0169 cd0000        	call	_EEPROM_write
2489  016c 84            	pop	a
2490  016d               L3301:
2491                     ; 766 				WTVxxxInit(stEepromCfgData.byVol);	   //语音芯片初始化
2493  016d b632          	ld	a,_stEepromCfgData+5
2494  016f cd0000        	call	_WTVxxxInit
2496                     ; 767 				PlaySound(22);//音量调节成功
2498  0172 a616          	ld	a,#22
2499  0174 cd0000        	call	_PlaySound
2501                     ; 768 				break;
2503  0177 ac240224      	jpf	L5201
2504  017b               L357:
2505                     ; 769 			case 2: //音量+
2505                     ; 770 				if(stEepromCfgData.byVol<15)
2507  017b b632          	ld	a,_stEepromCfgData+5
2508  017d a10f          	cp	a,#15
2509  017f 243b          	jruge	L7301
2510                     ; 772 					stEepromCfgData.byVol+=2;
2512  0181 3c32          	inc	_stEepromCfgData+5
2513  0183 3c32          	inc	_stEepromCfgData+5
2514                     ; 773 					if (stEepromCfgData.byVol>15) stEepromCfgData.byVol=15;
2516  0185 b632          	ld	a,_stEepromCfgData+5
2517  0187 a110          	cp	a,#16
2518  0189 2504          	jrult	L1401
2521  018b 350f0032      	mov	_stEepromCfgData+5,#15
2522  018f               L1401:
2523                     ; 774 					stEepromCfgData.byCrc = CalcParaCrc();					
2525  018f cd0000        	call	_CalcParaCrc
2527  0192 b733          	ld	_stEepromCfgData+6,a
2528                     ; 776 					EEPROM_write(IAP0_ADDR+VOL_INDEX,stEepromCfgData.byVol);
2530  0194 3b0032        	push	_stEepromCfgData+5
2531  0197 ae0007        	ldw	x,#7
2532  019a cd0000        	call	_EEPROM_write
2534  019d 84            	pop	a
2535                     ; 777 					EEPROM_write(IAP1_ADDR+VOL_INDEX,stEepromCfgData.byVol);
2537  019e 3b0032        	push	_stEepromCfgData+5
2538  01a1 ae0027        	ldw	x,#39
2539  01a4 cd0000        	call	_EEPROM_write
2541  01a7 84            	pop	a
2542                     ; 778 					EEPROM_write(IAP0_ADDR+CRC_INDEX,stEepromCfgData.byCrc);
2544  01a8 3b0033        	push	_stEepromCfgData+6
2545  01ab ae0008        	ldw	x,#8
2546  01ae cd0000        	call	_EEPROM_write
2548  01b1 84            	pop	a
2549                     ; 779 					EEPROM_write(IAP1_ADDR+CRC_INDEX,stEepromCfgData.byCrc);					
2551  01b2 3b0033        	push	_stEepromCfgData+6
2552  01b5 ae0028        	ldw	x,#40
2553  01b8 cd0000        	call	_EEPROM_write
2555  01bb 84            	pop	a
2556  01bc               L7301:
2557                     ; 783 				WTVxxxInit(stEepromCfgData.byVol);	   //语音芯片初始化
2559  01bc b632          	ld	a,_stEepromCfgData+5
2560  01be cd0000        	call	_WTVxxxInit
2562                     ; 784 				PlaySound(22);
2564  01c1 a616          	ld	a,#22
2565  01c3 cd0000        	call	_PlaySound
2567                     ; 785 				break;
2569  01c6 205c          	jra	L5201
2570  01c8               L557:
2571                     ; 786 			case 4: 						//挂断键/查房
2571                     ; 787 				if (calling)
2573  01c8 3d08          	tnz	_calling
2574  01ca 2715          	jreq	L3401
2575                     ; 789 					StopPlay();
2577  01cc cd0000        	call	_StopPlay
2579                     ; 790 					seng_tx();
2581  01cf cd0000        	call	_seng_tx
2583                     ; 791 					halSpiStrobe(CCxxx0_SIDLE);
2585  01d2 a636          	ld	a,#54
2586  01d4 cd0000        	call	_halSpiStrobe
2588                     ; 792 					halSpiStrobe(CCxxx0_SFTX);						
2590  01d7 a63b          	ld	a,#59
2591  01d9 cd0000        	call	_halSpiStrobe
2593                     ; 794 					StartRxWor();					
2595  01dc cd0000        	call	_StartRxWor
2598  01df 2041          	jra	L5401
2599  01e1               L3401:
2600                     ; 801 					seng_tx();
2602  01e1 cd0000        	call	_seng_tx
2604                     ; 802 					halSpiStrobe(CCxxx0_SIDLE);
2606  01e4 a636          	ld	a,#54
2607  01e6 cd0000        	call	_halSpiStrobe
2609                     ; 803 					halSpiStrobe(CCxxx0_SFTX);						
2611  01e9 a63b          	ld	a,#59
2612  01eb cd0000        	call	_halSpiStrobe
2614                     ; 805 					StartRxWor();					
2616  01ee cd0000        	call	_StartRxWor
2618                     ; 810 					TxBuf[0]=0x05;
2620  01f1 35050025      	mov	_TxBuf,#5
2621                     ; 811 					TxBuf[1]=stEepromCfgData.byNum;
2623  01f5 452f26        	mov	_TxBuf+1,_stEepromCfgData+2
2624                     ; 812 					TxBuf[2]=0x02;
2626  01f8 35020027      	mov	_TxBuf+2,#2
2627                     ; 813 					k=0x05+stEepromCfgData.byNum+0x02;
2629  01fc b62f          	ld	a,_stEepromCfgData+2
2630  01fe ab07          	add	a,#7
2631  0200 6b02          	ld	(OFST-1,sp),a
2632                     ; 814 					TxBuf[3]=k;
2634  0202 7b02          	ld	a,(OFST-1,sp)
2635  0204 b728          	ld	_TxBuf+3,a
2636                     ; 818 					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
2638  0206 4b00          	push	#0
2639  0208 4b04          	push	#4
2640  020a ae0025        	ldw	x,#_TxBuf
2641  020d cd0000        	call	_IrDi_send_data
2643  0210 85            	popw	x
2644                     ; 819 					Delayms(100);
2646  0211 ae0064        	ldw	x,#100
2647  0214 cd0000        	call	_Delayms
2649                     ; 820 					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
2651  0217 4b00          	push	#0
2652  0219 4b04          	push	#4
2653  021b ae0025        	ldw	x,#_TxBuf
2654  021e cd0000        	call	_IrDi_send_data
2656  0221 85            	popw	x
2657  0222               L5401:
2658                     ; 841 				rfirq=0;
2660  0222 3f04          	clr	_rfirq
2661                     ; 842 				break;
2663  0224               L1301:
2664  0224               L5201:
2665                     ; 848 		ir_inc++;
2667  0224 be00          	ldw	x,_ir_inc
2668  0226 1c0001        	addw	x,#1
2669  0229 bf00          	ldw	_ir_inc,x
2670                     ; 851 		if (low_bat)
2672  022b 3d0b          	tnz	_low_bat
2673  022d 2726          	jreq	L7401
2674                     ; 853 			if ((ir_inc%1000)==50)	//灭的时间长，主要也是为了省电 
2676  022f be00          	ldw	x,_ir_inc
2677  0231 90ae03e8      	ldw	y,#1000
2678  0235 65            	divw	x,y
2679  0236 51            	exgw	x,y
2680  0237 a30032        	cpw	x,#50
2681  023a 2606          	jrne	L1501
2682                     ; 855 				SetLed(1);		//灭
2684  023c 72185005      	bset	20485,#4
2687  0240 2017          	jra	L7501
2688  0242               L1501:
2689                     ; 857 			else if ((ir_inc%1000)==999) 
2691  0242 be00          	ldw	x,_ir_inc
2692  0244 90ae03e8      	ldw	y,#1000
2693  0248 65            	divw	x,y
2694  0249 51            	exgw	x,y
2695  024a a303e7        	cpw	x,#999
2696  024d 260a          	jrne	L7501
2697                     ; 859 				SetLed(0);		//亮
2699  024f 72195005      	bres	20485,#4
2701  0253 2004          	jra	L7501
2702  0255               L7401:
2703                     ; 862 		else SetLed(1);	//电量正常，红灯灭
2705  0255 72185005      	bset	20485,#4
2706  0259               L7501:
2707                     ; 866 		if(ir_inc %2000 == 0)	//	2S1次
2710  0259 be00          	ldw	x,_ir_inc
2711  025b 90ae07d0      	ldw	y,#2000
2712  025f 65            	divw	x,y
2713  0260 51            	exgw	x,y
2714  0261 a30000        	cpw	x,#0
2715  0264 2620          	jrne	L1601
2716                     ; 870 			TxBuf[0]=0x05;
2718  0266 35050025      	mov	_TxBuf,#5
2719                     ; 871 			TxBuf[1]=stEepromCfgData.byNum;
2721  026a 452f26        	mov	_TxBuf+1,_stEepromCfgData+2
2722                     ; 872 			TxBuf[2]=0x01;
2724  026d 35010027      	mov	_TxBuf+2,#1
2725                     ; 873 			k=0x05+stEepromCfgData.byNum+0x01;
2727  0271 b62f          	ld	a,_stEepromCfgData+2
2728  0273 ab06          	add	a,#6
2729  0275 6b02          	ld	(OFST-1,sp),a
2730                     ; 874 			TxBuf[3]=k;
2732  0277 7b02          	ld	a,(OFST-1,sp)
2733  0279 b728          	ld	_TxBuf+3,a
2734                     ; 877 			IrDi_send_data(TxBuf,4,IR_MODE_NORMAL);
2736  027b 4b01          	push	#1
2737  027d 4b04          	push	#4
2738  027f ae0025        	ldw	x,#_TxBuf
2739  0282 cd0000        	call	_IrDi_send_data
2741  0285 85            	popw	x
2742  0286               L1601:
2743                     ; 898 		if(ir_inc %4000 == 0)	//	4S   1次
2745  0286 be00          	ldw	x,_ir_inc
2746  0288 90ae0fa0      	ldw	y,#4000
2747  028c 65            	divw	x,y
2748  028d 51            	exgw	x,y
2749  028e a30000        	cpw	x,#0
2750  0291 260a          	jrne	L3601
2751                     ; 901 			ir_inc =0;			//轮到最大数据时将数据清0
2753  0293 5f            	clrw	x
2754  0294 bf00          	ldw	_ir_inc,x
2755                     ; 907 			if (rfirq==0)
2757  0296 3d04          	tnz	_rfirq
2758  0298 2603          	jrne	L3601
2759                     ; 909 				StartRxWor();
2761  029a cd0000        	call	_StartRxWor
2763  029d               L3601:
2764                     ; 917 		if(RFStatusCnt)
2766  029d be06          	ldw	x,_RFStatusCnt
2767  029f 2720          	jreq	L7601
2768                     ; 919 			if(--RFStatusCnt ==0)
2770  02a1 be06          	ldw	x,_RFStatusCnt
2771  02a3 1d0001        	subw	x,#1
2772  02a6 bf06          	ldw	_RFStatusCnt,x
2773  02a8 2617          	jrne	L7601
2774                     ; 921 				UART1_SendData8(0x11);
2776  02aa a611          	ld	a,#17
2777  02ac cd0000        	call	_UART1_SendData8
2779                     ; 922 				halSpiStrobe(CCxxx0_SIDLE);
2781  02af a636          	ld	a,#54
2782  02b1 cd0000        	call	_halSpiStrobe
2784                     ; 923 				halSpiStrobe(CCxxx0_SFRX);
2786  02b4 a63a          	ld	a,#58
2787  02b6 cd0000        	call	_halSpiStrobe
2789                     ; 924 				halSpiStrobe(CCxxx0_SFTX);
2791  02b9 a63b          	ld	a,#59
2792  02bb cd0000        	call	_halSpiStrobe
2794                     ; 928 				StartRxWor();
2796  02be cd0000        	call	_StartRxWor
2798  02c1               L7601:
2799                     ; 936 		SetLEDMode(0);
2801  02c1 72155002      	bres	20482,#2
2802                     ; 940 		if (GetGDO0())
2805  02c5 c65010        	ld	a,20496
2806  02c8 a510          	bcp	a,#16
2807  02ca 270f          	jreq	L3701
2808                     ; 942 			if(RFStatus==0) 
2810  02cc 3d09          	tnz	_RFStatus
2811  02ce 2610          	jrne	L7701
2812                     ; 944 				RFStatus = 1;  
2814  02d0 35010009      	mov	_RFStatus,#1
2815                     ; 945 				RFStatusCnt = 6000; 	//6秒				
2817  02d4 ae1770        	ldw	x,#6000
2818  02d7 bf06          	ldw	_RFStatusCnt,x
2819  02d9 2005          	jra	L7701
2820  02db               L3701:
2821                     ; 950 			RFStatus = 0;
2823  02db 3f09          	clr	_RFStatus
2824                     ; 951 			RFStatusCnt = 0;
2826  02dd 5f            	clrw	x
2827  02de bf06          	ldw	_RFStatusCnt,x
2828  02e0               L7701:
2829                     ; 954 		Delayms(1);
2831  02e0 ae0001        	ldw	x,#1
2832  02e3 cd0000        	call	_Delayms
2835  02e6 acaa00aa      	jpf	L5001
2879                     ; 960 unsigned char key_in(void)
2879                     ; 961 {
2880                     .text:	section	.text,new
2881  0000               _key_in:
2883  0000 5203          	subw	sp,#3
2884       00000003      OFST:	set	3
2887                     ; 962   	unsigned char i=0;
2889  0002 0f01          	clr	(OFST-2,sp)
2890                     ; 963 	unsigned int x=0;
2892  0004 5f            	clrw	x
2893  0005 1f02          	ldw	(OFST-1,sp),x
2894                     ; 965   	if(GetSW2()==0)
2896  0007 c65010        	ld	a,20496
2897  000a a540          	bcp	a,#64
2898  000c 2633          	jrne	L3211
2899                     ; 967   		SetLEDMode(1);	//面板按键输出模式，用于灯指示
2901  000e 72145002      	bset	20482,#2
2904  0012 201f          	jra	L7211
2905  0014               L5211:
2906                     ; 968   		while(GetSW2()==0) {Delayms(20); x++; if(x>=120) break;}
2908  0014 ae0014        	ldw	x,#20
2909  0017 cd0000        	call	_Delayms
2913  001a 1e02          	ldw	x,(OFST-1,sp)
2914  001c 1c0001        	addw	x,#1
2915  001f 1f02          	ldw	(OFST-1,sp),x
2918  0021 1e02          	ldw	x,(OFST-1,sp)
2919  0023 a30078        	cpw	x,#120
2920  0026 250b          	jrult	L7211
2922  0028               L1311:
2923                     ; 970 		if(x>=120) return 5;	//未用
2925  0028 1e02          	ldw	x,(OFST-1,sp)
2926  002a a30078        	cpw	x,#120
2927  002d 250d          	jrult	L5311
2930  002f a605          	ld	a,#5
2932  0031 200b          	jra	L021
2933  0033               L7211:
2934                     ; 968   		while(GetSW2()==0) {Delayms(20); x++; if(x>=120) break;}
2936  0033 c65010        	ld	a,20496
2937  0036 a540          	bcp	a,#64
2938  0038 27da          	jreq	L5211
2939  003a 20ec          	jra	L1311
2940  003c               L5311:
2941                     ; 971 		else return 2;//音量+
2943  003c a602          	ld	a,#2
2945  003e               L021:
2947  003e 5b03          	addw	sp,#3
2948  0040 81            	ret
2949  0041               L3211:
2950                     ; 975   	if(GetSW3()==0)
2952  0041 c65001        	ld	a,20481
2953  0044 a504          	bcp	a,#4
2954  0046 2639          	jrne	L1411
2955                     ; 977   		x=0;
2957  0048 5f            	clrw	x
2958  0049 1f02          	ldw	(OFST-1,sp),x
2959                     ; 978 		SetLEDMode(0);
2961  004b 72155002      	bres	20482,#2
2964  004f 2023          	jra	L5411
2965  0051               L3411:
2966                     ; 979   		while(GetSW3()==0) {Delayms(20); x++; if(x>=120) break;}
2968  0051 ae0014        	ldw	x,#20
2969  0054 cd0000        	call	_Delayms
2973  0057 1e02          	ldw	x,(OFST-1,sp)
2974  0059 1c0001        	addw	x,#1
2975  005c 1f02          	ldw	(OFST-1,sp),x
2978  005e 1e02          	ldw	x,(OFST-1,sp)
2979  0060 a30078        	cpw	x,#120
2980  0063 250f          	jrult	L5411
2982  0065               L7411:
2983                     ; 980 		SetLEDMode(1);
2985  0065 72145002      	bset	20482,#2
2986                     ; 982 		if(x>=120) return 6;	//未用
2989  0069 1e02          	ldw	x,(OFST-1,sp)
2990  006b a30078        	cpw	x,#120
2991  006e 250d          	jrult	L3511
2994  0070 a606          	ld	a,#6
2996  0072 20ca          	jra	L021
2997  0074               L5411:
2998                     ; 979   		while(GetSW3()==0) {Delayms(20); x++; if(x>=120) break;}
3000  0074 c65001        	ld	a,20481
3001  0077 a504          	bcp	a,#4
3002  0079 27d6          	jreq	L3411
3003  007b 20e8          	jra	L7411
3004  007d               L3511:
3005                     ; 983 		else return 3;//音量-
3007  007d a603          	ld	a,#3
3009  007f 20bd          	jra	L021
3010  0081               L1411:
3011                     ; 986   	if(GetSW4()==0)
3013  0081 c65010        	ld	a,20496
3014  0084 a504          	bcp	a,#4
3015  0086 2635          	jrne	L7511
3016                     ; 988   		x=0;
3018  0088 5f            	clrw	x
3019  0089 1f02          	ldw	(OFST-1,sp),x
3020                     ; 989   		SetLEDMode(1);
3022  008b 72145002      	bset	20482,#2
3025  008f 201f          	jra	L3611
3026  0091               L1611:
3027                     ; 990 		while(GetSW4()==0){Delayms(20); x++; if(x>=120) break;} 
3029  0091 ae0014        	ldw	x,#20
3030  0094 cd0000        	call	_Delayms
3034  0097 1e02          	ldw	x,(OFST-1,sp)
3035  0099 1c0001        	addw	x,#1
3036  009c 1f02          	ldw	(OFST-1,sp),x
3039  009e 1e02          	ldw	x,(OFST-1,sp)
3040  00a0 a30078        	cpw	x,#120
3041  00a3 250b          	jrult	L3611
3043  00a5               L5611:
3044                     ; 992 		if(x>=120) return 7;	//未用
3046  00a5 1e02          	ldw	x,(OFST-1,sp)
3047  00a7 a30078        	cpw	x,#120
3048  00aa 250d          	jrult	L1711
3051  00ac a607          	ld	a,#7
3053  00ae 208e          	jra	L021
3054  00b0               L3611:
3055                     ; 990 		while(GetSW4()==0){Delayms(20); x++; if(x>=120) break;} 
3057  00b0 c65010        	ld	a,20496
3058  00b3 a504          	bcp	a,#4
3059  00b5 27da          	jreq	L1611
3060  00b7 20ec          	jra	L5611
3061  00b9               L1711:
3062                     ; 993 		else return 4;
3064  00b9 a604          	ld	a,#4
3066  00bb 2081          	jra	L021
3067  00bd               L7511:
3068                     ; 996   	return 0;
3070  00bd 4f            	clr	a
3072  00be cc003e        	jra	L021
3114                     ; 999 void seng_tx(void)
3114                     ; 1000 {  
3115                     .text:	section	.text,new
3116  0000               _seng_tx:
3118  0000 88            	push	a
3119       00000001      OFST:	set	1
3122                     ; 1002  	calling=0;
3124  0001 3f08          	clr	_calling
3125                     ; 1004  	TxBuf[0]=stEepromCfgData.bySec;
3127  0003 452e25        	mov	_TxBuf,_stEepromCfgData+1
3128                     ; 1005     if(TxBuf[0]==RxBufBak[0])
3130  0006 b625          	ld	a,_TxBuf
3131  0008 b109          	cp	a,_RxBufBak
3132  000a 2605          	jrne	L3121
3133                     ; 1007 		TxBuf[4]= RxBufBak[0];
3135  000c 450929        	mov	_TxBuf+4,_RxBufBak
3137  000f 2002          	jra	L5121
3138  0011               L3121:
3139                     ; 1011 		TxBuf[4]=0;		//否则发送区号为0
3141  0011 3f29          	clr	_TxBuf+4
3142  0013               L5121:
3143                     ; 1013 	TxBuf[1]=251;  //移动分机的房号固定251           
3145  0013 35fb0026      	mov	_TxBuf+1,#251
3146                     ; 1014 	TxBuf[2]=stEepromCfgData.byNum;
3148  0017 452f27        	mov	_TxBuf+2,_stEepromCfgData+2
3149                     ; 1015 	TxBuf[3]=0x10;
3151  001a 35100028      	mov	_TxBuf+3,#16
3152                     ; 1016 	if (RxBufBak[3]==7) TxBuf[3]=0x11;
3154  001e b60c          	ld	a,_RxBufBak+3
3155  0020 a107          	cp	a,#7
3156  0022 2606          	jrne	L7121
3159  0024 35110028      	mov	_TxBuf+3,#17
3161  0028 200a          	jra	L1221
3162  002a               L7121:
3163                     ; 1017 	else if(RxBufBak[3]==8){TxBuf[3]=0x12;}
3165  002a b60c          	ld	a,_RxBufBak+3
3166  002c a108          	cp	a,#8
3167  002e 2604          	jrne	L1221
3170  0030 35120028      	mov	_TxBuf+3,#18
3171  0034               L1221:
3172                     ; 1018 	TxBuf[5]= RxBufBak[1];
3174  0034 450a2a        	mov	_TxBuf+5,_RxBufBak+1
3175                     ; 1019 	TxBuf[6]= RxBufBak[2];
3177  0037 450b2b        	mov	_TxBuf+6,_RxBufBak+2
3178                     ; 1020 	TxBuf[7]= 0x55;
3180  003a 3555002c      	mov	_TxBuf+7,#85
3181                     ; 1023 	for(i=0;i<8;i++)
3183  003e 0f01          	clr	(OFST+0,sp)
3184  0040               L5221:
3185                     ; 1025 		UART1_SendData8(TxBuf[i]);
3187  0040 7b01          	ld	a,(OFST+0,sp)
3188  0042 5f            	clrw	x
3189  0043 97            	ld	xl,a
3190  0044 e625          	ld	a,(_TxBuf,x)
3191  0046 cd0000        	call	_UART1_SendData8
3193                     ; 1023 	for(i=0;i<8;i++)
3195  0049 0c01          	inc	(OFST+0,sp)
3198  004b 7b01          	ld	a,(OFST+0,sp)
3199  004d a108          	cp	a,#8
3200  004f 25ef          	jrult	L5221
3201                     ; 1028 	halSpiStrobe(CCxxx0_SIDLE);
3203  0051 a636          	ld	a,#54
3204  0053 cd0000        	call	_halSpiStrobe
3206                     ; 1029 	halRfSendPacket(TxBuf,8);//发送挂断命令数据
3208  0056 4b08          	push	#8
3209  0058 ae0025        	ldw	x,#_TxBuf
3210  005b cd0000        	call	_halRfSendPacket
3212  005e 84            	pop	a
3213                     ; 1030 	Delayms(100);
3215  005f ae0064        	ldw	x,#100
3216  0062 cd0000        	call	_Delayms
3218                     ; 1031 	halRfSendPacket(TxBuf,8);//发送挂断命令数据
3220  0065 4b08          	push	#8
3221  0067 ae0025        	ldw	x,#_TxBuf
3222  006a cd0000        	call	_halRfSendPacket
3224  006d 84            	pop	a
3225                     ; 1032 }
3228  006e 84            	pop	a
3229  006f 81            	ret
3268                     ; 1035 void seng_tx_answer(INT8U cmd_type)
3268                     ; 1036 {  
3269                     .text:	section	.text,new
3270  0000               _seng_tx_answer:
3274                     ; 1038  	TxBuf[0]=stEepromCfgData.bySec;	//区号
3276  0000 452e25        	mov	_TxBuf,_stEepromCfgData+1
3277                     ; 1040 	TxBuf[1]=251;  //移动分机的房号固定251           
3279  0003 35fb0026      	mov	_TxBuf+1,#251
3280                     ; 1041 	TxBuf[2]=stEepromCfgData.byNum;	//分机号
3282  0007 452f27        	mov	_TxBuf+2,_stEepromCfgData+2
3283                     ; 1042 	TxBuf[3]=0x02;
3285  000a 35020028      	mov	_TxBuf+3,#2
3286                     ; 1044 	TxBuf[4]=cmd_type;	//原命令
3288  000e b729          	ld	_TxBuf+4,a
3289                     ; 1046 	TxBuf[5]= 0x01;		//成功
3291  0010 3501002a      	mov	_TxBuf+5,#1
3292                     ; 1047 	TxBuf[6]= 0x00;
3294  0014 3f2b          	clr	_TxBuf+6
3295                     ; 1048 	TxBuf[7]= 0x55;
3297  0016 3555002c      	mov	_TxBuf+7,#85
3298                     ; 1049 	halSpiStrobe(CCxxx0_SIDLE);
3300  001a a636          	ld	a,#54
3301  001c cd0000        	call	_halSpiStrobe
3303                     ; 1050 	halRfSendPacket(TxBuf,8);//发送应答命令数据
3305  001f 4b08          	push	#8
3306  0021 ae0025        	ldw	x,#_TxBuf
3307  0024 cd0000        	call	_halRfSendPacket
3309  0027 84            	pop	a
3310                     ; 1051 	Delayms(100);
3312  0028 ae0064        	ldw	x,#100
3313  002b cd0000        	call	_Delayms
3315                     ; 1052 	halRfSendPacket(TxBuf,8);//发送应答命令数据
3317  002e 4b08          	push	#8
3318  0030 ae0025        	ldw	x,#_TxBuf
3319  0033 cd0000        	call	_halRfSendPacket
3321  0036 84            	pop	a
3322                     ; 1053 }
3325  0037 81            	ret
3366                     ; 1056 void seng_tx_para(void)
3366                     ; 1057 {  
3367                     .text:	section	.text,new
3368  0000               _seng_tx_para:
3370  0000 88            	push	a
3371       00000001      OFST:	set	1
3374                     ; 1062 	if(para_error ==0)
3376  0001 3d0c          	tnz	_para_error
3377  0003 2649          	jrne	L7621
3378                     ; 1076 		Delayms(10);
3380  0005 ae000a        	ldw	x,#10
3381  0008 cd0000        	call	_Delayms
3383                     ; 1080 		RxBuf[0]= VERSION_HI;
3385  000b 3501000d      	mov	_RxBuf,#1
3386                     ; 1081 		RxBuf[1]= VERSION_LO;
3388  000f 3502000e      	mov	_RxBuf+1,#2
3389                     ; 1083 		RxBuf[2]= stEepromCfgData.byInitFlag;
3391  0013 452d0f        	mov	_RxBuf+2,_stEepromCfgData
3392                     ; 1084 	 	RxBuf[3]=stEepromCfgData.bySec;	//区号      
3394  0016 452e10        	mov	_RxBuf+3,_stEepromCfgData+1
3395                     ; 1085 		RxBuf[4]=stEepromCfgData.byNum;	//分机号
3397  0019 452f11        	mov	_RxBuf+4,_stEepromCfgData+2
3398                     ; 1086 		RxBuf[5]= stEepromCfgData.bySBed;	
3400  001c 453012        	mov	_RxBuf+5,_stEepromCfgData+3
3401                     ; 1087 		RxBuf[6]= stEepromCfgData.byEBed;		
3403  001f 453113        	mov	_RxBuf+6,_stEepromCfgData+4
3404                     ; 1088 		RxBuf[7]= stEepromCfgData.byVol;
3406  0022 453214        	mov	_RxBuf+7,_stEepromCfgData+5
3407                     ; 1089 		RxBuf[8]= stEepromCfgData.byCrc;
3409  0025 453315        	mov	_RxBuf+8,_stEepromCfgData+6
3410                     ; 1090 		RxBuf[9]= 0x55;
3412  0028 35550016      	mov	_RxBuf+9,#85
3413                     ; 1091 		halSpiStrobe(CCxxx0_SIDLE);
3415  002c a636          	ld	a,#54
3416  002e cd0000        	call	_halSpiStrobe
3418                     ; 1092 		halRfSendPacket(RxBuf,10);//发送应答命令数据
3420  0031 4b0a          	push	#10
3421  0033 ae000d        	ldw	x,#_RxBuf
3422  0036 cd0000        	call	_halRfSendPacket
3424  0039 84            	pop	a
3425                     ; 1096 		for(i=0;i<10;i++) RxBuf[i] =0;
3427  003a 0f01          	clr	(OFST+0,sp)
3428  003c               L1721:
3431  003c 7b01          	ld	a,(OFST+0,sp)
3432  003e 5f            	clrw	x
3433  003f 97            	ld	xl,a
3434  0040 6f0d          	clr	(_RxBuf,x)
3437  0042 0c01          	inc	(OFST+0,sp)
3440  0044 7b01          	ld	a,(OFST+0,sp)
3441  0046 a10a          	cp	a,#10
3442  0048 25f2          	jrult	L1721
3444  004a acf000f0      	jpf	L7721
3445  004e               L7621:
3446                     ; 1105 		Delayms(10);
3448  004e ae000a        	ldw	x,#10
3449  0051 cd0000        	call	_Delayms
3451                     ; 1107 		RxBuf[0]= VERSION_HI;
3453  0054 3501000d      	mov	_RxBuf,#1
3454                     ; 1108 		RxBuf[1]= VERSION_LO;
3456  0058 3502000e      	mov	_RxBuf+1,#2
3457                     ; 1109 	 	RxBuf[2]=EEPROM_read(IAP0_ADDR+INIT_INDEX);
3459  005c ae0002        	ldw	x,#2
3460  005f cd0000        	call	_EEPROM_read
3462  0062 b70f          	ld	_RxBuf+2,a
3463                     ; 1110 	 	RxBuf[3]=EEPROM_read(IAP0_ADDR+SEC_INDEX);
3465  0064 ae0003        	ldw	x,#3
3466  0067 cd0000        	call	_EEPROM_read
3468  006a b710          	ld	_RxBuf+3,a
3469                     ; 1111 		RxBuf[4]=EEPROM_read(IAP0_ADDR+NUM_INDEX);
3471  006c ae0004        	ldw	x,#4
3472  006f cd0000        	call	_EEPROM_read
3474  0072 b711          	ld	_RxBuf+4,a
3475                     ; 1112 	 	RxBuf[5]=EEPROM_read(IAP0_ADDR+SBED_INDEX);
3477  0074 ae0005        	ldw	x,#5
3478  0077 cd0000        	call	_EEPROM_read
3480  007a b712          	ld	_RxBuf+5,a
3481                     ; 1113 	 	RxBuf[6]=EEPROM_read(IAP0_ADDR+EBED_INDEX);
3483  007c ae0006        	ldw	x,#6
3484  007f cd0000        	call	_EEPROM_read
3486  0082 b713          	ld	_RxBuf+6,a
3487                     ; 1114 		RxBuf[7]=EEPROM_read(IAP0_ADDR+VOL_INDEX);	
3489  0084 ae0007        	ldw	x,#7
3490  0087 cd0000        	call	_EEPROM_read
3492  008a b714          	ld	_RxBuf+7,a
3493                     ; 1115 		RxBuf[8]=EEPROM_read(IAP0_ADDR+CRC_INDEX);	
3495  008c ae0008        	ldw	x,#8
3496  008f cd0000        	call	_EEPROM_read
3498  0092 b715          	ld	_RxBuf+8,a
3499                     ; 1117 	 	RxBuf[9] =EEPROM_read(IAP1_ADDR+INIT_INDEX);
3501  0094 ae0022        	ldw	x,#34
3502  0097 cd0000        	call	_EEPROM_read
3504  009a b716          	ld	_RxBuf+9,a
3505                     ; 1118 	 	RxBuf[10]=EEPROM_read(IAP1_ADDR+SEC_INDEX);
3507  009c ae0023        	ldw	x,#35
3508  009f cd0000        	call	_EEPROM_read
3510  00a2 b717          	ld	_RxBuf+10,a
3511                     ; 1119 		RxBuf[11]=EEPROM_read(IAP1_ADDR+NUM_INDEX);
3513  00a4 ae0024        	ldw	x,#36
3514  00a7 cd0000        	call	_EEPROM_read
3516  00aa b718          	ld	_RxBuf+11,a
3517                     ; 1120 	 	RxBuf[12]=EEPROM_read(IAP1_ADDR+SBED_INDEX);
3519  00ac ae0025        	ldw	x,#37
3520  00af cd0000        	call	_EEPROM_read
3522  00b2 b719          	ld	_RxBuf+12,a
3523                     ; 1121 	 	RxBuf[13]=EEPROM_read(IAP1_ADDR+EBED_INDEX);
3525  00b4 ae0026        	ldw	x,#38
3526  00b7 cd0000        	call	_EEPROM_read
3528  00ba b71a          	ld	_RxBuf+13,a
3529                     ; 1122 		RxBuf[14]=EEPROM_read(IAP1_ADDR+VOL_INDEX);	
3531  00bc ae0027        	ldw	x,#39
3532  00bf cd0000        	call	_EEPROM_read
3534  00c2 b71b          	ld	_RxBuf+14,a
3535                     ; 1123 		RxBuf[15]=EEPROM_read(IAP1_ADDR+CRC_INDEX);	
3537  00c4 ae0028        	ldw	x,#40
3538  00c7 cd0000        	call	_EEPROM_read
3540  00ca b71c          	ld	_RxBuf+15,a
3541                     ; 1125 		RxBuf[16]= 0x55;
3543  00cc 3555001d      	mov	_RxBuf+16,#85
3544                     ; 1126 		halSpiStrobe(CCxxx0_SIDLE);
3546  00d0 a636          	ld	a,#54
3547  00d2 cd0000        	call	_halSpiStrobe
3549                     ; 1127 		halRfSendPacket(RxBuf,17);//发送应答命令数据
3551  00d5 4b11          	push	#17
3552  00d7 ae000d        	ldw	x,#_RxBuf
3553  00da cd0000        	call	_halRfSendPacket
3555  00dd 84            	pop	a
3556                     ; 1140 		for(i=0;i<17;i++) RxBuf[i] =0;
3558  00de 0f01          	clr	(OFST+0,sp)
3559  00e0               L1031:
3562  00e0 7b01          	ld	a,(OFST+0,sp)
3563  00e2 5f            	clrw	x
3564  00e3 97            	ld	xl,a
3565  00e4 6f0d          	clr	(_RxBuf,x)
3568  00e6 0c01          	inc	(OFST+0,sp)
3571  00e8 7b01          	ld	a,(OFST+0,sp)
3572  00ea a111          	cp	a,#17
3573  00ec 25f2          	jrult	L1031
3574                     ; 1143 		para_error =0;
3576  00ee 3f0c          	clr	_para_error
3577  00f0               L7721:
3578                     ; 1145 }
3581  00f0 84            	pop	a
3582  00f1 81            	ret
3806                     	xdef	_main
3807                     	xdef	_timer4_init
3808                     	xref	_IrDi_init
3809                     	xdef	_CheckRecv
3810                     	xdef	_InitParameter
3811                     	xdef	_SaveParameter
3812                     	xdef	_ReadParameter
3813                     	xdef	_CalcParaCrc
3814                     	xdef	_ParaCrcCheck
3815                     	xdef	_GetADCResult
3816                     	xdef	_EEPROM_read
3817                     	xdef	_EEPROM_write
3818                     	xdef	_ConfigIOPort
3819                     	xdef	_seng_tx_para
3820                     	xref.b	_play_end
3821                     	xref.b	_play_start
3822                     	xdef	_para_error
3823                     	xdef	_low_bat
3824                     	xdef	_RFStatus
3825                     	xdef	_RFClearState
3826                     	xdef	_RFStatusCnt
3827                     	xdef	_audio_irq
3828                     	switch	.ubsct
3829  0000               _power_rec:
3830  0000 000000000000  	ds.b	8
3831                     	xdef	_power_rec
3832                     	xdef	_check_bat_cnt
3833                     	xdef	_ir_inc
3834  0008               _calling:
3835  0008 00            	ds.b	1
3836                     	xdef	_calling
3837  0009               _RxBufBak:
3838  0009 00000000      	ds.b	4
3839                     	xdef	_RxBufBak
3840  000d               _RxBuf:
3841  000d 000000000000  	ds.b	24
3842                     	xdef	_RxBuf
3843  0025               _TxBuf:
3844  0025 000000000000  	ds.b	8
3845                     	xdef	_TxBuf
3846                     	xref	_CC1101_Init
3847                     	xref	_StartRxWor
3848                     	xref	_Init_WOR
3849                     	xref	_halRfReceivePacket
3850                     	xref	_halRfSendPacket
3851                     	xref	_halSpiStrobe
3852                     	xdef	_seng_tx_answer
3853                     	xref	_Ir_send_repeat
3854                     	xref	_IrDi_send_data
3855                     	xdef	_key_in
3856                     	xdef	_seng_tx
3857                     	xref	_StopPlay
3858                     	xref	_CheckPlay
3859                     	xref	_PlaySound
3860                     	xref	_WTVxxxInit
3861                     	xref	_PlaySpeak
3862                     	xref	_Delayms
3863  002d               _stEepromCfgData:
3864  002d 000000000000  	ds.b	7
3865                     	xdef	_stEepromCfgData
3866                     	xdef	_CC1101SigValue
3867                     	xref.b	_IrState
3868                     	xdef	_rfirq
3869                     	xref	_UART1_SendData8
3870                     	xref	_TIM4_UpdateRequestConfig
3871                     	xref	_TIM4_UpdateDisableConfig
3872                     	xref	_TIM4_ITConfig
3873                     	xref	_TIM4_Cmd
3874                     	xref	_TIM4_TimeBaseInit
3875                     	xref	_TIM4_DeInit
3876                     	xref	_SPI_Cmd
3877                     	xref	_SPI_Init
3878                     	xref	_EXTI_SetExtIntSensitivity
3879                     	xref	_CLK_SYSCLKConfig
3880                     	xref	_CLK_PeripheralClockConfig
3881                     	xref	_ADC1_ITConfig
3882                     	xref	_ADC1_DeInit
3883                     	xref.b	c_x
3903                     	xref	c_smodx
3904                     	end
