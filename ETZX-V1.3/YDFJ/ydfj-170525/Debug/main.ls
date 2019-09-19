   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     	bsct
   5  0000               _rfirq:
   6  0000 00            	dc.b	0
   7  0001               _audio_irq:
   8  0001 00            	dc.b	0
  37                     ; 42 void ConfigIOPort(void)
  37                     ; 43 {
  39                     	switch	.text
  40  0000               _ConfigIOPort:
  44                     ; 44 	GPIOA->ODR &= ~(IO_SW3); //置低
  46  0000 72155000      	bres	20480,#2
  47                     ; 45 	GPIOA->ODR &= ~IO_IRCNT; //置高
  49  0004 72175000      	bres	20480,#3
  50                     ; 46 	GPIOA->DDR |= IO_RFCS|IO_IRCNT; //输出模式
  52  0008 c65002        	ld	a,20482
  53  000b aa0a          	or	a,#10
  54  000d c75002        	ld	20482,a
  55                     ; 47 	GPIOA->CR1 |= (IO_RFCS|IO_SW3); //上拉
  57  0010 c65003        	ld	a,20483
  58  0013 aa06          	or	a,#6
  59  0015 c75003        	ld	20483,a
  60                     ; 48 	GPIOA->CR2 |= (IO_SW3); //允许按键中断
  62  0018 72145004      	bset	20484,#2
  63                     ; 49 	GPIOB->ODR |= IO_LED; //置高
  65  001c 72185005      	bset	20485,#4
  66                     ; 50 	GPIOB->DDR |= IO_LED; //输出模式
  68  0020 72185007      	bset	20487,#4
  69                     ; 51 	GPIOB->CR2 |= (IO_INTAUD); 
  71  0024 721a5009      	bset	20489,#5
  72                     ; 52 	GPIOC->ODR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
  74  0028 c6500a        	ld	a,20490
  75  002b aa78          	or	a,#120
  76  002d c7500a        	ld	20490,a
  77                     ; 53 	GPIOC->DDR |= IO_AUDCS|IO_AUDRST|IO_SPICLK|IO_SPIMOSI;
  79  0030 c6500c        	ld	a,20492
  80  0033 aa78          	or	a,#120
  81  0035 c7500c        	ld	20492,a
  82                     ; 54 	GPIOC->CR1 |= IO_SPICLK|IO_SPIMOSI|IO_SPIMISO|IO_AUDRST|IO_AUDCS;
  84  0038 c6500d        	ld	a,20493
  85  003b aaf8          	or	a,#248
  86  003d c7500d        	ld	20493,a
  87                     ; 55 	GPIOD->ODR |= IO_AUDCS;
  89  0040 7216500f      	bset	20495,#3
  90                     ; 56 	GPIOD->DDR |= IO_AUDCS|IO_RFCS|IO_IR;
  92  0044 c65011        	ld	a,20497
  93  0047 aa0a          	or	a,#10
  94  0049 c75011        	ld	20497,a
  95                     ; 57 	GPIOD->CR1 |= IO_SW4|IO_SW2|IO_IR;
  97  004c c65012        	ld	a,20498
  98  004f aa4c          	or	a,#76
  99  0051 c75012        	ld	20498,a
 100                     ; 58 	GPIOD->CR2 |= (IO_SW4|IO_SW2); //允许按键中断
 102  0054 c65013        	ld	a,20499
 103  0057 aa44          	or	a,#68
 104  0059 c75013        	ld	20499,a
 105                     ; 59 }
 108  005c 81            	ret
 160                     ; 69 INT8U EEPROM_write(INT16U uiAddress, INT8U ucData)
 160                     ; 70 {
 161                     	switch	.text
 162  005d               _EEPROM_write:
 164  005d 89            	pushw	x
 165  005e 89            	pushw	x
 166       00000002      OFST:	set	2
 169                     ; 71     INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress; 
 171  005f 1c4000        	addw	x,#16384
 172  0062 1f01          	ldw	(OFST-1,sp),x
 173                     ; 72     if (eeprom_addr1 > EEPROM_MAX_ADDRESS)                              // 地址错误
 175  0064 1e01          	ldw	x,(OFST-1,sp)
 176  0066 a34281        	cpw	x,#17025
 177  0069 2504          	jrult	L15
 178                     ; 74         return 1;                                            // 
 180  006b a601          	ld	a,#1
 182  006d 201f          	jra	L01
 183  006f               L15:
 184                     ; 76     EEPROM_PROTECT_DISABLE();                                           // 关闭写保护
 186  006f 35ae5064      	mov	20580,#174
 189  0073 35565064      	mov	20580,#86
 190                     ; 77     *((@far INT8U*) eeprom_addr1) = ucData;                             // 
 192  0077 7b01          	ld	a,(OFST-1,sp)
 193  0079 97            	ld	xl,a
 194  007a 7b02          	ld	a,(OFST+0,sp)
 195  007c 3f00          	clr	c_x
 196  007e 02            	rlwa	x,a
 197  007f 9093          	ldw	y,x
 198  0081 7b07          	ld	a,(OFST+5,sp)
 199  0083 bf01          	ldw	c_x+1,x
 200  0085 92bd0000      	ldf	[c_x.e],a
 201                     ; 78     EEPROM_PROTECT_ENABLE();                                            // 开启写保护
 203  0089 7217505f      	bres	20575,#3
 204                     ; 79     return 0;
 206  008d 4f            	clr	a
 208  008e               L01:
 210  008e 5b04          	addw	sp,#4
 211  0090 81            	ret
 254                     ; 81 INT8U EEPROM_read(INT16U uiAddress)
 254                     ; 82 {
 255                     	switch	.text
 256  0091               _EEPROM_read:
 258  0091 89            	pushw	x
 259       00000002      OFST:	set	2
 262                     ; 83     INT16U  eeprom_addr1 = EEPROM_BASE_ADDRESS + uiAddress;             //
 264  0092 1c4000        	addw	x,#16384
 265  0095 1f01          	ldw	(OFST-1,sp),x
 266                     ; 84     return  *((@far INT8U*) eeprom_addr1);                              // 读取数据 
 268  0097 7b01          	ld	a,(OFST-1,sp)
 269  0099 97            	ld	xl,a
 270  009a 7b02          	ld	a,(OFST+0,sp)
 271  009c 3f00          	clr	c_x
 272  009e 02            	rlwa	x,a
 273  009f 9093          	ldw	y,x
 274  00a1 bf01          	ldw	c_x+1,x
 275  00a3 92bc0000      	ldf	a,[c_x.e]
 278  00a7 85            	popw	x
 279  00a8 81            	ret
 332                     ; 92 INT16U GetADCResult(INT8U ch)
 332                     ; 93 {
 333                     	switch	.text
 334  00a9               _GetADCResult:
 336  00a9 88            	push	a
 337  00aa 89            	pushw	x
 338       00000002      OFST:	set	2
 341                     ; 95 	ADC1_DeInit();
 343  00ab cd0000        	call	_ADC1_DeInit
 345                     ; 96 	ADC1->CSR|=ch; //选择通道
 347  00ae c65400        	ld	a,21504
 348  00b1 1a03          	or	a,(OFST+1,sp)
 349  00b3 c75400        	ld	21504,a
 350                     ; 97   ADC1->CR2|=0x08; // 数据排列,右对齐
 352  00b6 72165402      	bset	21506,#3
 353                     ; 98 	ADC1->CR2&=~0x02; //SCAN DISABLE
 355  00ba 72135402      	bres	21506,#1
 356                     ; 99 	ADC1->CR3&=~0x80;  //DATE BUFFER DISABLE
 358  00be 721f5403      	bres	21507,#7
 359                     ; 100 	ADC1->TDRL=(1<<ch);
 361  00c2 7b03          	ld	a,(OFST+1,sp)
 362  00c4 5f            	clrw	x
 363  00c5 97            	ld	xl,a
 364  00c6 a601          	ld	a,#1
 365  00c8 5d            	tnzw	x
 366  00c9 2704          	jreq	L61
 367  00cb               L02:
 368  00cb 48            	sll	a
 369  00cc 5a            	decw	x
 370  00cd 26fc          	jrne	L02
 371  00cf               L61:
 372  00cf c75407        	ld	21511,a
 373                     ; 101 	ADC1->CR1&=~0x02;
 375  00d2 72135401      	bres	21505,#1
 376                     ; 102 	ADC1->CR1|=0x01;
 378  00d6 72105401      	bset	21505,#0
 379                     ; 103 	for(i=0;i<200;i++); // 延时一段时间，至少100uS，保证ADC模块的上电完成
 381  00da 5f            	clrw	x
 382  00db 1f01          	ldw	(OFST-1,sp),x
 383  00dd               L131:
 387  00dd 1e01          	ldw	x,(OFST-1,sp)
 388  00df 1c0001        	addw	x,#1
 389  00e2 1f01          	ldw	(OFST-1,sp),x
 392  00e4 1e01          	ldw	x,(OFST-1,sp)
 393  00e6 a300c8        	cpw	x,#200
 394  00e9 25f2          	jrult	L131
 395                     ; 104   ADC1->CSR&=(~0x80);
 397  00eb 721f5400      	bres	21504,#7
 398                     ; 105 	ADC1->CR1|=0x01; // 再次将CR1寄存器的最低位置1,开启AD转换
 400  00ef 72105401      	bset	21505,#0
 402  00f3               L141:
 403                     ; 106   while((ADC1->CSR & 0x80)==0); // 等待转换结束
 405  00f3 c65400        	ld	a,21504
 406  00f6 a580          	bcp	a,#128
 407  00f8 27f9          	jreq	L141
 408                     ; 107   value=ADC1->DRH;
 410  00fa c65404        	ld	a,21508
 411  00fd 5f            	clrw	x
 412  00fe 97            	ld	xl,a
 413  00ff 1f01          	ldw	(OFST-1,sp),x
 414                     ; 108 	value<<=8;
 416  0101 7b02          	ld	a,(OFST+0,sp)
 417  0103 6b01          	ld	(OFST-1,sp),a
 418  0105 0f02          	clr	(OFST+0,sp)
 419                     ; 109   value|=ADC1->DRL;
 421  0107 c65405        	ld	a,21509
 422  010a 5f            	clrw	x
 423  010b 97            	ld	xl,a
 424  010c 01            	rrwa	x,a
 425  010d 1a02          	or	a,(OFST+0,sp)
 426  010f 01            	rrwa	x,a
 427  0110 1a01          	or	a,(OFST-1,sp)
 428  0112 01            	rrwa	x,a
 429  0113 1f01          	ldw	(OFST-1,sp),x
 430                     ; 110 	ADC1->CSR&=(~0x80);
 432  0115 721f5400      	bres	21504,#7
 433                     ; 111 	ADC1->CR1&=~0x01;	
 435  0119 72115401      	bres	21505,#0
 436                     ; 112 	ADC1_DeInit();
 438  011d cd0000        	call	_ADC1_DeInit
 440                     ; 113   return value;                 //Return ADC result
 442  0120 1e01          	ldw	x,(OFST-1,sp)
 445  0122 5b03          	addw	sp,#3
 446  0124 81            	ret
 473                     ; 116 void SaveConfig(void)
 473                     ; 117 {
 474                     	switch	.text
 475  0125               _SaveConfig:
 479                     ; 118 	EEPROM_write(0,voml);
 481  0125 3b0000        	push	_voml
 482  0128 5f            	clrw	x
 483  0129 cd005d        	call	_EEPROM_write
 485  012c 84            	pop	a
 486                     ; 119 	EEPROM_write(1,mode);
 488  012d 3b0001        	push	_mode
 489  0130 ae0001        	ldw	x,#1
 490  0133 cd005d        	call	_EEPROM_write
 492  0136 84            	pop	a
 493                     ; 120 	EEPROM_write(2,pa_sel);
 495  0137 3b0002        	push	_pa_sel
 496  013a ae0002        	ldw	x,#2
 497  013d cd005d        	call	_EEPROM_write
 499  0140 84            	pop	a
 500                     ; 121 }
 503  0141 81            	ret
 530                     ; 122 void ReadConfig(void)
 530                     ; 123 {
 531                     	switch	.text
 532  0142               _ReadConfig:
 536                     ; 124 	voml = EEPROM_read(0);
 538  0142 5f            	clrw	x
 539  0143 cd0091        	call	_EEPROM_read
 541  0146 b700          	ld	_voml,a
 542                     ; 125 	mode = EEPROM_read(1);
 544  0148 ae0001        	ldw	x,#1
 545  014b cd0091        	call	_EEPROM_read
 547  014e b701          	ld	_mode,a
 548                     ; 126 	pa_sel = EEPROM_read(2);
 550  0150 ae0002        	ldw	x,#2
 551  0153 cd0091        	call	_EEPROM_read
 553  0156 b702          	ld	_pa_sel,a
 554                     ; 127 }
 557  0158 81            	ret
 651                     ; 129 void CheckRecv(void)
 651                     ; 130 {
 652                     	switch	.text
 653  0159               _CheckRecv:
 655  0159 5206          	subw	sp,#6
 656       00000006      OFST:	set	6
 659                     ; 132 	INT16U cont=0,cont1=0;
 661  015b 1e04          	ldw	x,(OFST-2,sp)
 664  015d 1e02          	ldw	x,(OFST-4,sp)
 665                     ; 133 	DisableRFIrq();
 667  015f 72195013      	bres	20499,#4
 668                     ; 134   leng=24;
 670  0163 a618          	ld	a,#24
 671  0165 6b01          	ld	(OFST-5,sp),a
 672                     ; 135   if(halRfReceivePacket(RxBuf,&leng))     //读取接收到的无线数据
 674  0167 96            	ldw	x,sp
 675  0168 1c0001        	addw	x,#OFST-5
 676  016b 89            	pushw	x
 677  016c ae0015        	ldw	x,#_RxBuf
 678  016f cd0000        	call	_halRfReceivePacket
 680  0172 85            	popw	x
 681  0173 4d            	tnz	a
 682  0174 2603          	jrne	L03
 683  0176 cc0313        	jp	L532
 684  0179               L03:
 685                     ; 137 		if (leng>7)
 687  0179 7b01          	ld	a,(OFST-5,sp)
 688  017b a108          	cp	a,#8
 689  017d 2403          	jruge	L23
 690  017f cc0306        	jp	L732
 691  0182               L23:
 692                     ; 139 			halSpiStrobe(CCxxx0_SPWD);//进入掉电状态
 694  0182 a639          	ld	a,#57
 695  0184 cd0000        	call	_halSpiStrobe
 697                     ; 140   		SetLEDMode(1);
 699  0187 72145002      	bset	20482,#2
 700                     ; 141 	  	if(RxBuf[7]==0xaa)
 703  018b b61c          	ld	a,_RxBuf+7
 704  018d a1aa          	cp	a,#170
 705  018f 2703          	jreq	L43
 706  0191 cc0300        	jp	L142
 707  0194               L43:
 708                     ; 143 	  		switch(RxBuf[3])                    //判别接收命令
 710  0194 b618          	ld	a,_RxBuf+3
 712                     ; 211 				default: 
 712                     ; 212 					break;
 713  0196 a006          	sub	a,#6
 714  0198 2603          	jrne	L63
 715  019a cc0265        	jp	L761
 716  019d               L63:
 717  019d 4a            	dec	a
 718  019e 2603          	jrne	L04
 719  01a0 cc027a        	jp	L171
 720  01a3               L04:
 721  01a3 4a            	dec	a
 722  01a4 2603          	jrne	L24
 723  01a6 cc028c        	jp	L371
 724  01a9               L24:
 725  01a9 a052          	sub	a,#82
 726  01ab 2703          	jreq	L44
 727  01ad cc0300        	jp	L142
 728  01b0               L44:
 729                     ; 145 				case 0x5a:                       // 编号
 729                     ; 146 		     	if(RxBuf[1]==251)
 731  01b0 b616          	ld	a,_RxBuf+1
 732  01b2 a1fb          	cp	a,#251
 733  01b4 2703          	jreq	L64
 734  01b6 cc0300        	jp	L142
 735  01b9               L64:
 736                     ; 148 			      cont=0;cont1=0;
 738  01b9 5f            	clrw	x
 739  01ba 1f04          	ldw	(OFST-2,sp),x
 742  01bc 5f            	clrw	x
 743  01bd 1f02          	ldw	(OFST-4,sp),x
 744  01bf               L152:
 745                     ; 151 							cont++;
 747  01bf 1e04          	ldw	x,(OFST-2,sp)
 748  01c1 1c0001        	addw	x,#1
 749  01c4 1f04          	ldw	(OFST-2,sp),x
 750                     ; 152 							if (cont==500)
 752  01c6 1e04          	ldw	x,(OFST-2,sp)
 753  01c8 a301f4        	cpw	x,#500
 754  01cb 2604          	jrne	L552
 755                     ; 154 								SetLEDMode(0);
 757  01cd 72155002      	bres	20482,#2
 759  01d1               L552:
 760                     ; 156               if(cont>1000)
 762  01d1 1e04          	ldw	x,(OFST-2,sp)
 763  01d3 a303e9        	cpw	x,#1001
 764  01d6 250e          	jrult	L752
 765                     ; 158 								cont=0;
 767  01d8 5f            	clrw	x
 768  01d9 1f04          	ldw	(OFST-2,sp),x
 769                     ; 159 								SetLEDMode(1);
 771  01db 72145002      	bset	20482,#2
 772                     ; 160 								cont1++;
 775  01df 1e02          	ldw	x,(OFST-4,sp)
 776  01e1 1c0001        	addw	x,#1
 777  01e4 1f02          	ldw	(OFST-4,sp),x
 778  01e6               L752:
 779                     ; 162 						  if(cont1>10){break;}
 781  01e6 1e02          	ldw	x,(OFST-4,sp)
 782  01e8 a3000b        	cpw	x,#11
 783  01eb 2503          	jrult	L05
 784  01ed cc0300        	jp	L142
 785  01f0               L05:
 788                     ; 163 						  if(GetSW4()==0)
 790  01f0 c65010        	ld	a,20496
 791  01f3 a504          	bcp	a,#4
 792  01f5 2664          	jrne	L362
 793                     ; 165 								EEPROM_write(3,RxBuf[0]);
 795  01f7 3b0015        	push	_RxBuf
 796  01fa ae0003        	ldw	x,#3
 797  01fd cd005d        	call	_EEPROM_write
 799  0200 84            	pop	a
 800                     ; 166 								EEPROM_write(4,RxBuf[2]);
 802  0201 3b0017        	push	_RxBuf+2
 803  0204 ae0004        	ldw	x,#4
 804  0207 cd005d        	call	_EEPROM_write
 806  020a 84            	pop	a
 807                     ; 167 								EEPROM_write(5,RxBuf[4]);
 809  020b 3b0019        	push	_RxBuf+4
 810  020e ae0005        	ldw	x,#5
 811  0211 cd005d        	call	_EEPROM_write
 813  0214 84            	pop	a
 814                     ; 168 								EEPROM_write(6,RxBuf[5]);
 816  0215 3b001a        	push	_RxBuf+5
 817  0218 ae0006        	ldw	x,#6
 818  021b cd005d        	call	_EEPROM_write
 820  021e 84            	pop	a
 821                     ; 169 							  WTVxxxInit(voml);      //语音芯片初始化
 823  021f b600          	ld	a,_voml
 824  0221 cd0000        	call	_WTVxxxInit
 826                     ; 170 							  if((EEPROM_read(3)==RxBuf[0])&&(EEPROM_read(4)==RxBuf[2]))
 828  0224 ae0003        	ldw	x,#3
 829  0227 cd0091        	call	_EEPROM_read
 831  022a b115          	cp	a,_RxBuf
 832  022c 261d          	jrne	L562
 834  022e ae0004        	ldw	x,#4
 835  0231 cd0091        	call	_EEPROM_read
 837  0234 b117          	cp	a,_RxBuf+2
 838  0236 2613          	jrne	L562
 839                     ; 172 							  	num_1=RxBuf[0]; num_2=RxBuf[2];
 841  0238 451510        	mov	_num_1,_RxBuf
 844  023b 45170f        	mov	_num_2,_RxBuf+2
 845                     ; 173 									sbed = RxBuf[4];
 847  023e 451904        	mov	_sbed,_RxBuf+4
 848                     ; 174 									ebed = RxBuf[5];
 850  0241 451a03        	mov	_ebed,_RxBuf+5
 851                     ; 175 							    PlaySound(19);
 853  0244 a613          	ld	a,#19
 854  0246 cd0000        	call	_PlaySound
 857  0249 2005          	jra	L372
 858  024b               L562:
 859                     ; 177 							  else{PlaySound(20);}
 861  024b a614          	ld	a,#20
 862  024d cd0000        	call	_PlaySound
 864  0250               L372:
 865                     ; 178 							  while(GetSW4()==0);
 867  0250 c65010        	ld	a,20496
 868  0253 a504          	bcp	a,#4
 869  0255 27f9          	jreq	L372
 870                     ; 179 							  break;
 872  0257 ac000300      	jpf	L142
 873  025b               L362:
 874                     ; 181 							Delayms(1);
 876  025b ae0001        	ldw	x,#1
 877  025e cd0000        	call	_Delayms
 880  0261 acbf01bf      	jpf	L152
 881  0265               L761:
 882                     ; 185 				case 0x06:                 	//床头呼叫
 882                     ; 186 					if (((RxBuf[2]<sbed)||(RxBuf[2]>ebed))&&RxBuf[1]!=250) break;
 884  0265 b617          	ld	a,_RxBuf+2
 885  0267 b104          	cp	a,_sbed
 886  0269 2506          	jrult	L103
 888  026b b617          	ld	a,_RxBuf+2
 889  026d b103          	cp	a,_ebed
 890  026f 2309          	jrule	L171
 891  0271               L103:
 893  0271 b616          	ld	a,_RxBuf+1
 894  0273 a1fa          	cp	a,#250
 895  0275 2703          	jreq	L25
 896  0277 cc0300        	jp	L142
 897  027a               L25:
 900  027a               L171:
 901                     ; 187 				case 0x07:					//输液
 901                     ; 188 					if (((RxBuf[2]<sbed)||(RxBuf[2]>ebed))&&RxBuf[1]!=250) break;
 903  027a b617          	ld	a,_RxBuf+2
 904  027c b104          	cp	a,_sbed
 905  027e 2506          	jrult	L503
 907  0280 b617          	ld	a,_RxBuf+2
 908  0282 b103          	cp	a,_ebed
 909  0284 2306          	jrule	L371
 910  0286               L503:
 912  0286 b616          	ld	a,_RxBuf+1
 913  0288 a1fa          	cp	a,#250
 914  028a 2674          	jrne	L142
 917  028c               L371:
 918                     ; 189 				case 0x08:                 	//卫生间呼叫
 918                     ; 190 					if (RxBuf[1]==251) break;
 920  028c b616          	ld	a,_RxBuf+1
 921  028e a1fb          	cp	a,#251
 922  0290 276e          	jreq	L142
 925                     ; 191 			  	if(num_1==RxBuf[0])
 927  0292 b610          	ld	a,_num_1
 928  0294 b115          	cp	a,_RxBuf
 929  0296 2668          	jrne	L142
 930                     ; 193 						for (i=0;i<4;i++)
 932  0298 0f06          	clr	(OFST+0,sp)
 933  029a               L313:
 934                     ; 195 							if (RxBufBak[i]!=RxBuf[i]) break;
 936  029a 7b06          	ld	a,(OFST+0,sp)
 937  029c 5f            	clrw	x
 938  029d 97            	ld	xl,a
 939  029e 7b06          	ld	a,(OFST+0,sp)
 940  02a0 905f          	clrw	y
 941  02a2 9097          	ld	yl,a
 942  02a4 90e611        	ld	a,(_RxBufBak,y)
 943  02a7 e115          	cp	a,(_RxBuf,x)
 944  02a9 2608          	jrne	L713
 947                     ; 193 						for (i=0;i<4;i++)
 949  02ab 0c06          	inc	(OFST+0,sp)
 952  02ad 7b06          	ld	a,(OFST+0,sp)
 953  02af a104          	cp	a,#4
 954  02b1 25e7          	jrult	L313
 955  02b3               L713:
 956                     ; 197 						if (play_end==play_start||i<4)
 958  02b3 b600          	ld	a,_play_end
 959  02b5 b100          	cp	a,_play_start
 960  02b7 2706          	jreq	L523
 962  02b9 7b06          	ld	a,(OFST+0,sp)
 963  02bb a104          	cp	a,#4
 964  02bd 2441          	jruge	L142
 965  02bf               L523:
 966                     ; 199 							calling=1;
 968  02bf 3501000e      	mov	_calling,#1
 969                     ; 200 							if (play_end==play_start) WTVxxxInit(voml);      //语音芯片初始化
 971  02c3 b600          	ld	a,_play_end
 972  02c5 b100          	cp	a,_play_start
 973  02c7 2607          	jrne	L723
 976  02c9 b600          	ld	a,_voml
 977  02cb cd0000        	call	_WTVxxxInit
 980  02ce 2003          	jra	L133
 981  02d0               L723:
 982                     ; 201 							else StopPlay();
 984  02d0 cd0000        	call	_StopPlay
 986  02d3               L133:
 987                     ; 202 							for (i=0;i<4;i++) RxBufBak[i] = RxBuf[i];
 989  02d3 0f06          	clr	(OFST+0,sp)
 990  02d5               L333:
 993  02d5 7b06          	ld	a,(OFST+0,sp)
 994  02d7 5f            	clrw	x
 995  02d8 97            	ld	xl,a
 996  02d9 e615          	ld	a,(_RxBuf,x)
 997  02db e711          	ld	(_RxBufBak,x),a
1000  02dd 0c06          	inc	(OFST+0,sp)
1003  02df 7b06          	ld	a,(OFST+0,sp)
1004  02e1 a104          	cp	a,#4
1005  02e3 25f0          	jrult	L333
1006                     ; 203 							for(k=0;k<3;k++)
1008  02e5 0f06          	clr	(OFST+0,sp)
1009  02e7               L143:
1010                     ; 205 								PlaySpeak(RxBuf[1],RxBuf[2],RxBuf[3]);
1012  02e7 3b0018        	push	_RxBuf+3
1013  02ea b617          	ld	a,_RxBuf+2
1014  02ec 97            	ld	xl,a
1015  02ed b616          	ld	a,_RxBuf+1
1016  02ef 95            	ld	xh,a
1017  02f0 cd0000        	call	_PlaySpeak
1019  02f3 84            	pop	a
1020                     ; 206 								SetLEDMode(0);
1022  02f4 72155002      	bres	20482,#2
1023                     ; 203 							for(k=0;k<3;k++)
1026  02f8 0c06          	inc	(OFST+0,sp)
1029  02fa 7b06          	ld	a,(OFST+0,sp)
1030  02fc a103          	cp	a,#3
1031  02fe 25e7          	jrult	L143
1032  0300               L571:
1033                     ; 211 				default: 
1033                     ; 212 					break;
1035  0300               L542:
1036  0300               L142:
1037                     ; 215 			SetLEDMode(0);
1039  0300 72155002      	bres	20482,#2
1042  0304 2008          	jra	L743
1043  0306               L732:
1044                     ; 219 			CC1101_Init();              //cc1101初始化
1046  0306 cd0000        	call	_CC1101_Init
1048                     ; 221 			Init_WOR();
1050  0309 cd0000        	call	_Init_WOR
1052                     ; 223 			inc=0;
1054  030c 3f0d          	clr	_inc
1055  030e               L743:
1056                     ; 225 		halSpiStrobe(CCxxx0_SIDLE);
1058  030e a636          	ld	a,#54
1059  0310 cd0000        	call	_halSpiStrobe
1061  0313               L532:
1062                     ; 230 	StartRxWor();
1064  0313 cd0000        	call	_StartRxWor
1066                     ; 232 	rfirq=0;
1068  0316 3f00          	clr	_rfirq
1069                     ; 233 	EnableRFIrq();
1071  0318 72185013      	bset	20499,#4
1072                     ; 234 }
1075  031c 5b06          	addw	sp,#6
1076  031e 81            	ret
1210                     ; 237 void main(void)
1210                     ; 238 {
1211                     	switch	.text
1212  031f               _main:
1214  031f 5204          	subw	sp,#4
1215       00000004      OFST:	set	4
1218                     ; 244 	bool low_bat=0,send_ir;
1220  0321 7b02          	ld	a,(OFST-2,sp)
1221  0323 97            	ld	xl,a
1222                     ; 246 	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
1224  0324 4f            	clr	a
1225  0325 cd0000        	call	_CLK_SYSCLKConfig
1227                     ; 247 	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);
1229  0328 a682          	ld	a,#130
1230  032a cd0000        	call	_CLK_SYSCLKConfig
1232                     ; 248 	ConfigIOPort();
1234  032d cd0000        	call	_ConfigIOPort
1236                     ; 251 	ADC1_ITConfig(ADC1_IT_EOC,DISABLE);
1238  0330 4b00          	push	#0
1239  0332 ae0080        	ldw	x,#128
1240  0335 cd0000        	call	_ADC1_ITConfig
1242  0338 84            	pop	a
1243                     ; 253 	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD,EXTI_SENSITIVITY_FALL_ONLY);
1245  0339 ae0002        	ldw	x,#2
1246  033c a603          	ld	a,#3
1247  033e 95            	ld	xh,a
1248  033f cd0000        	call	_EXTI_SetExtIntSensitivity
1250                     ; 254 	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);
1252  0342 ae0002        	ldw	x,#2
1253  0345 4f            	clr	a
1254  0346 95            	ld	xh,a
1255  0347 cd0000        	call	_EXTI_SetExtIntSensitivity
1257                     ; 255 	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_RISE_ONLY);
1259  034a ae0001        	ldw	x,#1
1260  034d a601          	ld	a,#1
1261  034f 95            	ld	xh,a
1262  0350 cd0000        	call	_EXTI_SetExtIntSensitivity
1264                     ; 256 	SPI_Init(SPI_FIRSTBIT_MSB,SPI_BAUDRATEPRESCALER_8,SPI_MODE_MASTER,SPI_CLOCKPOLARITY_LOW,SPI_CLOCKPHASE_1EDGE,SPI_DATADIRECTION_2LINES_FULLDUPLEX,SPI_NSS_SOFT,0);
1266  0353 4b00          	push	#0
1267  0355 4b02          	push	#2
1268  0357 4b00          	push	#0
1269  0359 4b00          	push	#0
1270  035b 4b00          	push	#0
1271  035d 4b04          	push	#4
1272  035f ae0010        	ldw	x,#16
1273  0362 4f            	clr	a
1274  0363 95            	ld	xh,a
1275  0364 cd0000        	call	_SPI_Init
1277  0367 5b06          	addw	sp,#6
1278                     ; 257 	SPI_Cmd(ENABLE);
1280  0369 a601          	ld	a,#1
1281  036b cd0000        	call	_SPI_Cmd
1283                     ; 258 	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, ENABLE);
1285  036e ae0001        	ldw	x,#1
1286  0371 a612          	ld	a,#18
1287  0373 95            	ld	xh,a
1288  0374 cd0000        	call	_CLK_PeripheralClockConfig
1290                     ; 259 	AWU_Init(AWU_TIMEBASE_2S);  //1秒唤醒一次
1292  0377 a60e          	ld	a,#14
1293  0379 cd0000        	call	_AWU_Init
1295                     ; 261   FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN); //停止时FLASH 掉电
1297  037c a604          	ld	a,#4
1298  037e cd0000        	call	_FLASH_SetLowPowerMode
1300                     ; 263   CLK_SlowActiveHaltWakeUpCmd(ENABLE);  //停止时主电源关闭
1302  0381 a601          	ld	a,#1
1303  0383 cd0000        	call	_CLK_SlowActiveHaltWakeUpCmd
1305                     ; 266 	enableInterrupts();
1308  0386 9a            rim
1310                     ; 268 	power_rec[0] = GetADCResult(5);
1313  0387 a605          	ld	a,#5
1314  0389 cd00a9        	call	_GetADCResult
1316  038c bf05          	ldw	_power_rec,x
1317                     ; 270 	SetLEDMode(1);
1319  038e 72145002      	bset	20482,#2
1320                     ; 271 	CC1101_Init();              //cc1101初始化
1323  0392 cd0000        	call	_CC1101_Init
1325                     ; 273 	Init_WOR();
1327  0395 cd0000        	call	_Init_WOR
1329                     ; 275 	halSpiStrobe(CCxxx0_SPWD);//进入掉电状态
1331  0398 a639          	ld	a,#57
1332  039a cd0000        	call	_halSpiStrobe
1334                     ; 277 	ReadConfig();
1336  039d cd0142        	call	_ReadConfig
1338                     ; 278 	if(voml>15)              //如果音量没有被设置，默认为最大
1340  03a0 b600          	ld	a,_voml
1341  03a2 a110          	cp	a,#16
1342  03a4 250f          	jrult	L524
1343                     ; 280 		voml=15;
1345  03a6 350f0000      	mov	_voml,#15
1346                     ; 281 		mode=1; //默认为红外
1348  03aa 35010001      	mov	_mode,#1
1349                     ; 282 		pa_sel=4;//默认无线查房功率为4
1351  03ae 35040002      	mov	_pa_sel,#4
1352                     ; 283 		SaveConfig();
1354  03b2 cd0125        	call	_SaveConfig
1356  03b5               L524:
1357                     ; 286 	num_1= EEPROM_read(3);
1359  03b5 ae0003        	ldw	x,#3
1360  03b8 cd0091        	call	_EEPROM_read
1362  03bb b710          	ld	_num_1,a
1363                     ; 287 	num_2= EEPROM_read(4);
1365  03bd ae0004        	ldw	x,#4
1366  03c0 cd0091        	call	_EEPROM_read
1368  03c3 b70f          	ld	_num_2,a
1369                     ; 288 	sbed= EEPROM_read(5);
1371  03c5 ae0005        	ldw	x,#5
1372  03c8 cd0091        	call	_EEPROM_read
1374  03cb b704          	ld	_sbed,a
1375                     ; 289 	ebed= EEPROM_read(6);
1377  03cd ae0006        	ldw	x,#6
1378  03d0 cd0091        	call	_EEPROM_read
1380  03d3 b703          	ld	_ebed,a
1381                     ; 290 	if ((sbed==0&&ebed==0)||(sbed==255&&ebed==255)) {sbed=0;ebed=255;}
1383  03d5 3d04          	tnz	_sbed
1384  03d7 2604          	jrne	L334
1386  03d9 3d03          	tnz	_ebed
1387  03db 270c          	jreq	L134
1388  03dd               L334:
1390  03dd b604          	ld	a,_sbed
1391  03df a1ff          	cp	a,#255
1392  03e1 260c          	jrne	L724
1394  03e3 b603          	ld	a,_ebed
1395  03e5 a1ff          	cp	a,#255
1396  03e7 2606          	jrne	L724
1397  03e9               L134:
1400  03e9 3f04          	clr	_sbed
1403  03eb 35ff0003      	mov	_ebed,#255
1404  03ef               L724:
1405                     ; 291 	WTVxxxInit(voml);      //语音芯片初始化
1407  03ef b600          	ld	a,_voml
1408  03f1 cd0000        	call	_WTVxxxInit
1410                     ; 296 	StartRxWor();
1412  03f4 cd0000        	call	_StartRxWor
1414                     ; 299 	SetLEDMode(0);
1416  03f7 72155002      	bres	20482,#2
1417                     ; 300 	EnableRFIrq();
1420  03fb 72185013      	bset	20499,#4
1421                     ; 302 	index=0;
1423  03ff 0f03          	clr	(OFST-1,sp)
1424                     ; 303 	power_rec[index] = GetADCResult(5);            
1426  0401 a605          	ld	a,#5
1427  0403 cd00a9        	call	_GetADCResult
1429  0406 bf05          	ldw	_power_rec,x
1430                     ; 304 	index++;
1432  0408 0c03          	inc	(OFST-1,sp)
1433                     ; 305 	power_rec[3]=power_rec[2]=power_rec[1]=power_rec[0];
1435  040a be05          	ldw	x,_power_rec
1436  040c bf07          	ldw	_power_rec+2,x
1437  040e be07          	ldw	x,_power_rec+2
1438  0410 bf09          	ldw	_power_rec+4,x
1439  0412 be09          	ldw	x,_power_rec+4
1440  0414 bf0b          	ldw	_power_rec+6,x
1441  0416               L534:
1442                     ; 308 		if (audio_irq)
1444  0416 3d01          	tnz	_audio_irq
1445  0418 2705          	jreq	L144
1446                     ; 310 			CheckPlay();
1448  041a cd0000        	call	_CheckPlay
1450                     ; 311 			audio_irq=0;
1452  041d 3f01          	clr	_audio_irq
1453  041f               L144:
1454                     ; 313 		power_rec[index] = GetADCResult(5);
1456  041f a605          	ld	a,#5
1457  0421 cd00a9        	call	_GetADCResult
1459  0424 7b03          	ld	a,(OFST-1,sp)
1460  0426 905f          	clrw	y
1461  0428 9097          	ld	yl,a
1462  042a 9058          	sllw	y
1463  042c 90ef05        	ldw	(_power_rec,y),x
1464                     ; 314 		index = (index+1)%3;
1466  042f 7b03          	ld	a,(OFST-1,sp)
1467  0431 5f            	clrw	x
1468  0432 97            	ld	xl,a
1469  0433 5c            	incw	x
1470  0434 a603          	ld	a,#3
1471  0436 cd0000        	call	c_smodx
1473  0439 9f            	ld	a,xl
1474  043a 6b03          	ld	(OFST-1,sp),a
1475                     ; 315 		power_rec[3] = (power_rec[2]+power_rec[1]+power_rec[0])/3;
1477  043c be09          	ldw	x,_power_rec+4
1478  043e 72bb0007      	addw	x,_power_rec+2
1479  0442 72bb0005      	addw	x,_power_rec
1480  0446 a603          	ld	a,#3
1481  0448 62            	div	x,a
1482  0449 bf0b          	ldw	_power_rec+6,x
1483                     ; 316 		if (power_rec[3]<563) //<3.3v
1485  044b be0b          	ldw	x,_power_rec+6
1486  044d a30233        	cpw	x,#563
1487  0450 2406          	jruge	L344
1488                     ; 318 			low_bat=1;
1490  0452 a601          	ld	a,#1
1491  0454 6b02          	ld	(OFST-2,sp),a
1493  0456 2002          	jra	L544
1494  0458               L344:
1495                     ; 319 		}else low_bat=0;
1497  0458 0f02          	clr	(OFST-2,sp)
1498  045a               L544:
1499                     ; 320 		send_ir=1;
1501  045a a601          	ld	a,#1
1502  045c 6b04          	ld	(OFST+0,sp),a
1503                     ; 321 	if (rfirq!=0){CheckRecv();send_ir=0;}//无线数据接收 
1505  045e 3d00          	tnz	_rfirq
1506  0460 2705          	jreq	L744
1509  0462 cd0159        	call	_CheckRecv
1513  0465 0f04          	clr	(OFST+0,sp)
1514  0467               L744:
1515                     ; 322 		if ((i=key_in())!=0)
1517  0467 cd0558        	call	_key_in
1519  046a 6b01          	ld	(OFST-3,sp),a
1520  046c 2603          	jrne	L65
1521  046e cc050c        	jp	L154
1522  0471               L65:
1523                     ; 324 			switch(i)
1525  0471 7b01          	ld	a,(OFST-3,sp)
1527                     ; 377 				break;
1528  0473 a002          	sub	a,#2
1529  0475 272b          	jreq	L353
1530  0477 4a            	dec	a
1531  0478 2707          	jreq	L153
1532  047a 4a            	dec	a
1533  047b 2749          	jreq	L553
1534  047d ac4c054c      	jpf	L374
1535  0481               L153:
1536                     ; 326 			case 3: 
1536                     ; 327 				if(voml>0)               //音量-
1538  0481 3d00          	tnz	_voml
1539  0483 270f          	jreq	L754
1540                     ; 329 					voml-=2;
1542  0485 3a00          	dec	_voml
1543  0487 3a00          	dec	_voml
1544                     ; 330 					if (voml>15) voml=0;
1546  0489 b600          	ld	a,_voml
1547  048b a110          	cp	a,#16
1548  048d 2502          	jrult	L164
1551  048f 3f00          	clr	_voml
1552  0491               L164:
1553                     ; 331 					SaveConfig();
1555  0491 cd0125        	call	_SaveConfig
1557  0494               L754:
1558                     ; 333 				WTVxxxInit(voml);      //语音芯片初始化
1560  0494 b600          	ld	a,_voml
1561  0496 cd0000        	call	_WTVxxxInit
1563                     ; 334 				PlaySound(22);//音量调节成功
1565  0499 a616          	ld	a,#22
1566  049b cd0000        	call	_PlaySound
1568                     ; 335 				break;
1570  049e ac4c054c      	jpf	L374
1571  04a2               L353:
1572                     ; 336 			case 2: //音量+
1572                     ; 337 				if(voml<15)
1574  04a2 b600          	ld	a,_voml
1575  04a4 a10f          	cp	a,#15
1576  04a6 2411          	jruge	L364
1577                     ; 339 					voml+=2;
1579  04a8 3c00          	inc	_voml
1580  04aa 3c00          	inc	_voml
1581                     ; 340 					if (voml>15) voml=15;
1583  04ac b600          	ld	a,_voml
1584  04ae a110          	cp	a,#16
1585  04b0 2504          	jrult	L564
1588  04b2 350f0000      	mov	_voml,#15
1589  04b6               L564:
1590                     ; 341 					SaveConfig();
1592  04b6 cd0125        	call	_SaveConfig
1594  04b9               L364:
1595                     ; 343 				WTVxxxInit(voml);      //语音芯片初始化
1597  04b9 b600          	ld	a,_voml
1598  04bb cd0000        	call	_WTVxxxInit
1600                     ; 344 				PlaySound(22);
1602  04be a616          	ld	a,#22
1603  04c0 cd0000        	call	_PlaySound
1605                     ; 345 				break;
1607  04c3 cc054c        	jra	L374
1608  04c6               L553:
1609                     ; 346 			case 4:                         //挂断键/查房
1609                     ; 347 				if (calling)
1611  04c6 3d0e          	tnz	_calling
1612  04c8 2705          	jreq	L764
1613                     ; 349 					StopPlay();
1615  04ca cd0000        	call	_StopPlay
1618  04cd 2037          	jra	L174
1619  04cf               L764:
1620                     ; 360 					seng_tx();
1622  04cf cd05c5        	call	_seng_tx
1624                     ; 364 					StartRxWor();
1626  04d2 cd0000        	call	_StartRxWor
1628                     ; 367 					TxBuf[0]=0x05;
1630  04d5 3505002d      	mov	_TxBuf,#5
1631                     ; 368 					TxBuf[1]=num_2;
1633  04d9 450f2e        	mov	_TxBuf+1,_num_2
1634                     ; 369 					TxBuf[2]=0x02;
1636  04dc 3502002f      	mov	_TxBuf+2,#2
1637                     ; 370 					k=0x05+num_2+0x02;
1639  04e0 b60f          	ld	a,_num_2
1640  04e2 ab07          	add	a,#7
1641  04e4 6b04          	ld	(OFST+0,sp),a
1642                     ; 371 					TxBuf[3]=k;
1644  04e6 7b04          	ld	a,(OFST+0,sp)
1645  04e8 b730          	ld	_TxBuf+3,a
1646                     ; 372 					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
1648  04ea 4b00          	push	#0
1649  04ec 4b04          	push	#4
1650  04ee ae002d        	ldw	x,#_TxBuf
1651  04f1 cd0000        	call	_IrDi_send_data
1653  04f4 85            	popw	x
1654                     ; 373 					Delayms(100);
1656  04f5 ae0064        	ldw	x,#100
1657  04f8 cd0000        	call	_Delayms
1659                     ; 374 					IrDi_send_data(TxBuf,4,IR_MODE_LITTLE);
1661  04fb 4b00          	push	#0
1662  04fd 4b04          	push	#4
1663  04ff ae002d        	ldw	x,#_TxBuf
1664  0502 cd0000        	call	_IrDi_send_data
1666  0505 85            	popw	x
1667  0506               L174:
1668                     ; 376 				rfirq=0;
1670  0506 3f00          	clr	_rfirq
1671                     ; 377 				break;
1673  0508 2042          	jra	L374
1674  050a               L554:
1676  050a 2040          	jra	L374
1677  050c               L154:
1678                     ; 380 		else if (send_ir)
1680  050c 0d04          	tnz	(OFST+0,sp)
1681  050e 273c          	jreq	L374
1682                     ; 382 			inc++;
1684  0510 3c0d          	inc	_inc
1685                     ; 383 			TxBuf[0]=0x05;
1687  0512 3505002d      	mov	_TxBuf,#5
1688                     ; 384 			TxBuf[1]=num_2;
1690  0516 450f2e        	mov	_TxBuf+1,_num_2
1691                     ; 385 			TxBuf[2]=0x01;
1693  0519 3501002f      	mov	_TxBuf+2,#1
1694                     ; 386 			k=0x05+num_2+0x01;
1696  051d b60f          	ld	a,_num_2
1697  051f ab06          	add	a,#6
1698  0521 6b04          	ld	(OFST+0,sp),a
1699                     ; 387 			TxBuf[3]=k;
1701  0523 7b04          	ld	a,(OFST+0,sp)
1702  0525 b730          	ld	_TxBuf+3,a
1703                     ; 388 			IrDi_send_data(TxBuf,4,IR_MODE_NORMAL);
1705  0527 4b01          	push	#1
1706  0529 4b04          	push	#4
1707  052b ae002d        	ldw	x,#_TxBuf
1708  052e cd0000        	call	_IrDi_send_data
1710  0531 85            	popw	x
1711                     ; 389 			if (low_bat)
1713  0532 0d02          	tnz	(OFST-2,sp)
1714  0534 2712          	jreq	L774
1715                     ; 391 			if ((inc%2)==0) {SetLed(1);} else {SetLed(0);}
1717  0536 b60d          	ld	a,_inc
1718  0538 a501          	bcp	a,#1
1719  053a 2606          	jrne	L105
1722  053c 72185005      	bset	20485,#4
1725  0540 200a          	jra	L374
1726  0542               L105:
1729  0542 72195005      	bres	20485,#4
1731  0546 2004          	jra	L374
1732  0548               L774:
1733                     ; 392 			}else SetLed(1);
1735  0548 72185005      	bset	20485,#4
1736  054c               L374:
1737                     ; 398 		StartRxWor();
1739  054c cd0000        	call	_StartRxWor
1741                     ; 400 		SetLEDMode(0);
1743  054f 72155002      	bres	20482,#2
1744                     ; 401 		halt();	
1748  0553 8e            halt
1752  0554 ac160416      	jpf	L534
1787                     ; 405 unsigned char key_in(void)
1787                     ; 406 {
1788                     	switch	.text
1789  0558               _key_in:
1791  0558 88            	push	a
1792       00000001      OFST:	set	1
1795                     ; 407   unsigned char i=0;
1797  0559 0f01          	clr	(OFST+0,sp)
1798                     ; 408   if(GetSW2()==0)
1800  055b c65010        	ld	a,20496
1801  055e a540          	bcp	a,#64
1802  0560 2631          	jrne	L525
1803                     ; 410   	SetLEDMode(1);
1805  0562 72145002      	bset	20482,#2
1808  0566 2014          	jra	L135
1809  0568               L725:
1810                     ; 413 			Delayms(20);
1812  0568 ae0014        	ldw	x,#20
1813  056b cd0000        	call	_Delayms
1815                     ; 414 			i++;
1817  056e 0c01          	inc	(OFST+0,sp)
1818                     ; 415 		if (i>120) {i--;SetLEDMode(0);}
1820  0570 7b01          	ld	a,(OFST+0,sp)
1821  0572 a179          	cp	a,#121
1822  0574 2506          	jrult	L135
1825  0576 0a01          	dec	(OFST+0,sp)
1828  0578 72155002      	bres	20482,#2
1830  057c               L135:
1831                     ; 411 		while(GetSW2()==0)
1833  057c c65010        	ld	a,20496
1834  057f a540          	bcp	a,#64
1835  0581 27e5          	jreq	L725
1836                     ; 417 		if (i==120) return 5;//PA设置
1838  0583 7b01          	ld	a,(OFST+0,sp)
1839  0585 a178          	cp	a,#120
1840  0587 2605          	jrne	L735
1843  0589 a605          	ld	a,#5
1846  058b 5b01          	addw	sp,#1
1847  058d 81            	ret
1848  058e               L735:
1849                     ; 418 		else return 2;   //音量+
1851  058e a602          	ld	a,#2
1854  0590 5b01          	addw	sp,#1
1855  0592 81            	ret
1856  0593               L525:
1857                     ; 420   if(GetSW3()==0)
1859  0593 c65001        	ld	a,20481
1860  0596 a504          	bcp	a,#4
1861  0598 2610          	jrne	L345
1863  059a               L745:
1864                     ; 422   	while(GetSW3()==0);
1866  059a c65001        	ld	a,20481
1867  059d a504          	bcp	a,#4
1868  059f 27f9          	jreq	L745
1869                     ; 423 		SetLEDMode(1);
1871  05a1 72145002      	bset	20482,#2
1872                     ; 424 		return 3;//音量-
1875  05a5 a603          	ld	a,#3
1878  05a7 5b01          	addw	sp,#1
1879  05a9 81            	ret
1880  05aa               L345:
1881                     ; 427   if(GetSW4()==0)
1883  05aa c65010        	ld	a,20496
1884  05ad a504          	bcp	a,#4
1885  05af 2610          	jrne	L355
1886                     ; 429   	SetLEDMode(1);
1888  05b1 72145002      	bset	20482,#2
1891  05b5               L755:
1892                     ; 430 	while(GetSW4()==0); 
1894  05b5 c65010        	ld	a,20496
1895  05b8 a504          	bcp	a,#4
1896  05ba 27f9          	jreq	L755
1897                     ; 431 	return 4;
1899  05bc a604          	ld	a,#4
1902  05be 5b01          	addw	sp,#1
1903  05c0 81            	ret
1904  05c1               L355:
1905                     ; 435   return 0;
1907  05c1 4f            	clr	a
1910  05c2 5b01          	addw	sp,#1
1911  05c4 81            	ret
1942                     ; 438 void seng_tx(void)
1942                     ; 439 {  
1943                     	switch	.text
1944  05c5               _seng_tx:
1948                     ; 440  	calling=0;
1950  05c5 3f0e          	clr	_calling
1951                     ; 441  	TxBuf[0]=num_1;
1953  05c7 45102d        	mov	_TxBuf,_num_1
1954                     ; 442     if(TxBuf[0]==RxBufBak[0])
1956  05ca b62d          	ld	a,_TxBuf
1957  05cc b111          	cp	a,_RxBufBak
1958  05ce 2605          	jrne	L375
1959                     ; 444 		TxBuf[4]= RxBufBak[0];
1961  05d0 451131        	mov	_TxBuf+4,_RxBufBak
1963  05d3 2002          	jra	L575
1964  05d5               L375:
1965                     ; 448 		TxBuf[4]=0;
1967  05d5 3f31          	clr	_TxBuf+4
1968  05d7               L575:
1969                     ; 450 	TxBuf[1]=251;  //移动分机的房号固定251           
1971  05d7 35fb002e      	mov	_TxBuf+1,#251
1972                     ; 451 	TxBuf[2]=num_2;
1974  05db 450f2f        	mov	_TxBuf+2,_num_2
1975                     ; 452 	TxBuf[3]=0x10;
1977  05de 35100030      	mov	_TxBuf+3,#16
1978                     ; 453 	if (RxBufBak[3]==7) TxBuf[3]=0x11;
1980  05e2 b614          	ld	a,_RxBufBak+3
1981  05e4 a107          	cp	a,#7
1982  05e6 2606          	jrne	L775
1985  05e8 35110030      	mov	_TxBuf+3,#17
1987  05ec 200a          	jra	L106
1988  05ee               L775:
1989                     ; 454 	else if(RxBufBak[3]==8){TxBuf[3]=0x12;}
1991  05ee b614          	ld	a,_RxBufBak+3
1992  05f0 a108          	cp	a,#8
1993  05f2 2604          	jrne	L106
1996  05f4 35120030      	mov	_TxBuf+3,#18
1997  05f8               L106:
1998                     ; 455 	TxBuf[5]= RxBufBak[1];
2000  05f8 451232        	mov	_TxBuf+5,_RxBufBak+1
2001                     ; 456 	TxBuf[6]= RxBufBak[2];
2003  05fb 451333        	mov	_TxBuf+6,_RxBufBak+2
2004                     ; 457 	TxBuf[7]= 0x55;
2006  05fe 35550034      	mov	_TxBuf+7,#85
2007                     ; 458 	halSpiStrobe(CCxxx0_SIDLE);
2009  0602 a636          	ld	a,#54
2010  0604 cd0000        	call	_halSpiStrobe
2012                     ; 459 	halRfSendPacket(TxBuf,8);//发送挂断命令数据
2014  0607 4b08          	push	#8
2015  0609 ae002d        	ldw	x,#_TxBuf
2016  060c cd0000        	call	_halRfSendPacket
2018  060f 84            	pop	a
2019                     ; 460 	Delayms(100);
2021  0610 ae0064        	ldw	x,#100
2022  0613 cd0000        	call	_Delayms
2024                     ; 461 	halRfSendPacket(TxBuf,8);//发送挂断命令数据
2026  0616 4b08          	push	#8
2027  0618 ae002d        	ldw	x,#_TxBuf
2028  061b cd0000        	call	_halRfSendPacket
2030  061e 84            	pop	a
2031                     ; 462 }
2034  061f 81            	ret
2190                     	xdef	_main
2191                     	xdef	_CheckRecv
2192                     	xdef	_ReadConfig
2193                     	xdef	_SaveConfig
2194                     	switch	.ubsct
2195  0000               _voml:
2196  0000 00            	ds.b	1
2197                     	xdef	_voml
2198  0001               _mode:
2199  0001 00            	ds.b	1
2200                     	xdef	_mode
2201  0002               _pa_sel:
2202  0002 00            	ds.b	1
2203                     	xdef	_pa_sel
2204                     	xdef	_GetADCResult
2205                     	xdef	_EEPROM_read
2206                     	xdef	_EEPROM_write
2207                     	xdef	_ConfigIOPort
2208                     	xref.b	_play_end
2209                     	xref.b	_play_start
2210                     	xdef	_audio_irq
2211  0003               _ebed:
2212  0003 00            	ds.b	1
2213                     	xdef	_ebed
2214  0004               _sbed:
2215  0004 00            	ds.b	1
2216                     	xdef	_sbed
2217  0005               _power_rec:
2218  0005 000000000000  	ds.b	8
2219                     	xdef	_power_rec
2220  000d               _inc:
2221  000d 00            	ds.b	1
2222                     	xdef	_inc
2223  000e               _calling:
2224  000e 00            	ds.b	1
2225                     	xdef	_calling
2226  000f               _num_2:
2227  000f 00            	ds.b	1
2228                     	xdef	_num_2
2229  0010               _num_1:
2230  0010 00            	ds.b	1
2231                     	xdef	_num_1
2232  0011               _RxBufBak:
2233  0011 00000000      	ds.b	4
2234                     	xdef	_RxBufBak
2235  0015               _RxBuf:
2236  0015 000000000000  	ds.b	24
2237                     	xdef	_RxBuf
2238  002d               _TxBuf:
2239  002d 000000000000  	ds.b	8
2240                     	xdef	_TxBuf
2241                     	xref	_CC1101_Init
2242                     	xref	_StartRxWor
2243                     	xref	_Init_WOR
2244                     	xref	_halRfReceivePacket
2245                     	xref	_halRfSendPacket
2246                     	xref	_halSpiStrobe
2247                     	xref	_IrDi_send_data
2248                     	xdef	_key_in
2249                     	xdef	_seng_tx
2250                     	xref	_StopPlay
2251                     	xref	_CheckPlay
2252                     	xref	_PlaySound
2253                     	xref	_WTVxxxInit
2254                     	xref	_PlaySpeak
2255                     	xref	_Delayms
2256                     	xdef	_rfirq
2257                     	xref	_SPI_Cmd
2258                     	xref	_SPI_Init
2259                     	xref	_FLASH_SetLowPowerMode
2260                     	xref	_EXTI_SetExtIntSensitivity
2261                     	xref	_CLK_SYSCLKConfig
2262                     	xref	_CLK_PeripheralClockConfig
2263                     	xref	_CLK_SlowActiveHaltWakeUpCmd
2264                     	xref	_AWU_Init
2265                     	xref	_ADC1_ITConfig
2266                     	xref	_ADC1_DeInit
2267                     	xref.b	c_x
2287                     	xref	c_smodx
2288                     	end
