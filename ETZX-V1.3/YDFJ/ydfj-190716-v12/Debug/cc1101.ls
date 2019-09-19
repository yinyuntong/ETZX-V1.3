   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     	bsct
   5  0000               _PaTabel:
   6  0000 c0            	dc.b	192
   7  0001 c0            	dc.b	192
   8  0002 c0            	dc.b	192
   9  0003 c0            	dc.b	192
  10  0004 c0            	dc.b	192
  11  0005 c0            	dc.b	192
  12  0006 c0            	dc.b	192
  13  0007 c0            	dc.b	192
  14                     .const:	section	.text
  15  0000               _rfSettings:
  16  0000 08            	dc.b	8
  17  0001 00            	dc.b	0
  18  0002 10            	dc.b	16
  19  0003 a7            	dc.b	167
  20  0004 62            	dc.b	98
  21  0005 58            	dc.b	88
  22  0006 43            	dc.b	67
  23  0007 03            	dc.b	3
  24  0008 22            	dc.b	34
  25  0009 f8            	dc.b	248
  26  000a 00            	dc.b	0
  27  000b 47            	dc.b	71
  28  000c 56            	dc.b	86
  29  000d 10            	dc.b	16
  30  000e 18            	dc.b	24
  31  000f 1d            	dc.b	29
  32  0010 1c            	dc.b	28
  33  0011 43            	dc.b	67
  34  0012 40            	dc.b	64
  35  0013 91            	dc.b	145
  36  0014 ea            	dc.b	234
  37  0015 2a            	dc.b	42
  38  0016 00            	dc.b	0
  39  0017 11            	dc.b	17
  40  0018 59            	dc.b	89
  41  0019 81            	dc.b	129
  42  001a 35            	dc.b	53
  43  001b 09            	dc.b	9
  44  001c 0b            	dc.b	11
  45  001d 06            	dc.b	6
  46  001e 04            	dc.b	4
  47  001f 05            	dc.b	5
  48  0020 00            	dc.b	0
  49  0021 20            	dc.b	32
  50  0022 00000000      	ds.b	4
  99                     ; 160 void delay(unsigned int s)
  99                     ; 161 {
 101                     .text:	section	.text,new
 102  0000               _delay:
 104  0000 89            	pushw	x
 105  0001 89            	pushw	x
 106       00000002      OFST:	set	2
 109                     ; 163 	for(i=0; i<s; i++);
 111  0002 5f            	clrw	x
 112  0003 1f01          	ldw	(OFST-1,sp),x
 114  0005 2007          	jra	L73
 115  0007               L33:
 119  0007 1e01          	ldw	x,(OFST-1,sp)
 120  0009 1c0001        	addw	x,#1
 121  000c 1f01          	ldw	(OFST-1,sp),x
 122  000e               L73:
 125  000e 1e01          	ldw	x,(OFST-1,sp)
 126  0010 1303          	cpw	x,(OFST+1,sp)
 127  0012 25f3          	jrult	L33
 128                     ; 164 	for(i=0; i<s; i++);
 130  0014 5f            	clrw	x
 131  0015 1f01          	ldw	(OFST-1,sp),x
 133  0017 2007          	jra	L74
 134  0019               L34:
 138  0019 1e01          	ldw	x,(OFST-1,sp)
 139  001b 1c0001        	addw	x,#1
 140  001e 1f01          	ldw	(OFST-1,sp),x
 141  0020               L74:
 144  0020 1e01          	ldw	x,(OFST-1,sp)
 145  0022 1303          	cpw	x,(OFST+1,sp)
 146  0024 25f3          	jrult	L34
 147                     ; 165 }
 150  0026 5b04          	addw	sp,#4
 151  0028 81            	ret
 194                     ; 167 void halWait(INT16U timeout) 
 194                     ; 168 {
 195                     .text:	section	.text,new
 196  0000               _halWait:
 198  0000 89            	pushw	x
 199  0001 88            	push	a
 200       00000001      OFST:	set	1
 203  0002               L57:
 204                     ; 172 		i=10;
 206  0002 a60a          	ld	a,#10
 207  0004 6b01          	ld	(OFST+0,sp),a
 209  0006               L701:
 210                     ; 173 		while(i--);
 212  0006 7b01          	ld	a,(OFST+0,sp)
 213  0008 0a01          	dec	(OFST+0,sp)
 214  000a 4d            	tnz	a
 215  000b 26f9          	jrne	L701
 216                     ; 174     } while (--timeout);
 218  000d 1e02          	ldw	x,(OFST+1,sp)
 219  000f 1d0001        	subw	x,#1
 220  0012 1f02          	ldw	(OFST+1,sp),x
 221  0014 26ec          	jrne	L57
 222                     ; 176 }
 225  0016 5b03          	addw	sp,#3
 226  0018 81            	ret
 279                     ; 178 unsigned char SPITransferByte(unsigned char byData)
 279                     ; 179 {
 280                     .text:	section	.text,new
 281  0000               _SPITransferByte:
 283  0000 88            	push	a
 284  0001 89            	pushw	x
 285       00000002      OFST:	set	2
 288                     ; 181 	uint8 RecData=0;
 290  0002 0f02          	clr	(OFST+0,sp)
 291                     ; 182 	for(i=0; i<8; i++)
 293  0004 0f01          	clr	(OFST-1,sp)
 294  0006               L141:
 295                     ; 184 		SetSCK(0);
 297  0006 721b500a      	bres	20490,#5
 298                     ; 185 		if(byData&0x01){ SetMOSI(1);}
 301  000a 7b03          	ld	a,(OFST+1,sp)
 302  000c a501          	bcp	a,#1
 303  000e 2706          	jreq	L741
 306  0010 721c500a      	bset	20490,#6
 309  0014 2004          	jra	L151
 310  0016               L741:
 311                     ; 186 		else {SetMOSI(0);}
 313  0016 721d500a      	bres	20490,#6
 314  001a               L151:
 315                     ; 187 		Delayus(150);
 317  001a ae0096        	ldw	x,#150
 318  001d cd0000        	call	_Delayus
 320                     ; 188 		SetSCK(1);
 322  0020 721a500a      	bset	20490,#5
 323                     ; 189 		Delayus(150);
 326  0024 ae0096        	ldw	x,#150
 327  0027 cd0000        	call	_Delayus
 329                     ; 190 		if(GetMISO()!=0) RecData |=0x80;
 331  002a c6500b        	ld	a,20491
 332  002d a580          	bcp	a,#128
 333  002f 2708          	jreq	L351
 336  0031 7b02          	ld	a,(OFST+0,sp)
 337  0033 aa80          	or	a,#128
 338  0035 6b02          	ld	(OFST+0,sp),a
 340  0037 2006          	jra	L551
 341  0039               L351:
 342                     ; 191 		else RecData &=0x7f;
 344  0039 7b02          	ld	a,(OFST+0,sp)
 345  003b a47f          	and	a,#127
 346  003d 6b02          	ld	(OFST+0,sp),a
 347  003f               L551:
 348                     ; 192 		byData>>= 1;
 350  003f 0403          	srl	(OFST+1,sp)
 351                     ; 193 		RecData>>=1;
 353  0041 0402          	srl	(OFST+0,sp)
 354                     ; 182 	for(i=0; i<8; i++)
 356  0043 0c01          	inc	(OFST-1,sp)
 359  0045 7b01          	ld	a,(OFST-1,sp)
 360  0047 a108          	cp	a,#8
 361  0049 25bb          	jrult	L141
 362                     ; 195 	return(RecData);
 364  004b 7b02          	ld	a,(OFST+0,sp)
 367  004d 5b03          	addw	sp,#3
 368  004f 81            	ret
 411                     ; 198 unsigned char SPI_SendByte(unsigned char Data)
 411                     ; 199 {
 412                     .text:	section	.text,new
 413  0000               _SPI_SendByte:
 415  0000 89            	pushw	x
 416       00000002      OFST:	set	2
 419                     ; 200 	INT16U x=0;
 421  0001 5f            	clrw	x
 422  0002 1f01          	ldw	(OFST-1,sp),x
 423                     ; 202 	SPI->DR = Data;
 425  0004 c75204        	ld	20996,a
 427  0007 2013          	jra	L502
 428  0009               L102:
 429                     ; 203 	while(SPI->SR&SPI_SR_BSY)  {if(x>1000)break;x++;};
 431  0009 1e01          	ldw	x,(OFST-1,sp)
 432  000b a303e9        	cpw	x,#1001
 433  000e 2505          	jrult	L112
 435  0010               L702:
 436                     ; 204 	return SPI->DR;
 439  0010 c65204        	ld	a,20996
 442  0013 85            	popw	x
 443  0014 81            	ret
 444  0015               L112:
 445                     ; 203 	while(SPI->SR&SPI_SR_BSY)  {if(x>1000)break;x++;};
 447  0015 1e01          	ldw	x,(OFST-1,sp)
 448  0017 1c0001        	addw	x,#1
 449  001a 1f01          	ldw	(OFST-1,sp),x
 450  001c               L502:
 453  001c c65203        	ld	a,20995
 454  001f a580          	bcp	a,#128
 455  0021 26e6          	jrne	L102
 456  0023 20eb          	jra	L702
 481                     ; 209 void Init_WOR(void)
 481                     ; 210 {
 482                     .text:	section	.text,new
 483  0000               _Init_WOR:
 487                     ; 211 	halSpiStrobe(CCxxx0_SIDLE);
 489  0000 a636          	ld	a,#54
 490  0002 cd0000        	call	_halSpiStrobe
 492                     ; 212 	halSpiWriteReg(CCxxx0_WORCTRL, 0x78); // 
 494  0005 ae0078        	ldw	x,#120
 495  0008 a620          	ld	a,#32
 496  000a 95            	ld	xh,a
 497  000b cd0000        	call	_halSpiWriteReg
 499                     ; 213 	halSpiWriteReg(CCxxx0_WOREVT1, 0x02);//43B5 500ms, 28A0 300ms 1B15 200ms
 501  000e ae0002        	ldw	x,#2
 502  0011 a61e          	ld	a,#30
 503  0013 95            	ld	xh,a
 504  0014 cd0000        	call	_halSpiWriteReg
 506                     ; 214 	halSpiWriteReg(CCxxx0_WOREVT0, 0xB5);// 876B 1S
 508  0017 ae00b5        	ldw	x,#181
 509  001a a61f          	ld	a,#31
 510  001c 95            	ld	xh,a
 511  001d cd0000        	call	_halSpiWriteReg
 513                     ; 215 	halSpiWriteReg(CCxxx0_MCSM2, 0x1f); // RX_TIME // RX_TIME_RSSI= 1 RX_TIME_QUAL = 0
 515  0020 ae001f        	ldw	x,#31
 516  0023 a616          	ld	a,#22
 517  0025 95            	ld	xh,a
 518  0026 cd0000        	call	_halSpiWriteReg
 520                     ; 218 }
 523  0029 81            	ret
 547                     ; 219 void StartRxWor(void)
 547                     ; 220 {
 548                     .text:	section	.text,new
 549  0000               _StartRxWor:
 553                     ; 221 	halSpiStrobe(CCxxx0_SIDLE);
 555  0000 a636          	ld	a,#54
 556  0002 cd0000        	call	_halSpiStrobe
 558                     ; 222 	halSpiStrobe(CCxxx0_SWORRST);
 560  0005 a63c          	ld	a,#60
 561  0007 cd0000        	call	_halSpiStrobe
 563                     ; 223 	halSpiStrobe(CCxxx0_SWOR);
 565  000a a638          	ld	a,#56
 566  000c cd0000        	call	_halSpiStrobe
 568                     ; 224 }
 571  000f 81            	ret
 599                     ; 234 unsigned char CC1101_Init(void)
 599                     ; 235 {
 600                     .text:	section	.text,new
 601  0000               _CC1101_Init:
 605                     ; 236 	SetCSN(1);
 607  0000 72125000      	bset	20480,#1
 608                     ; 237 	delay(5000);
 611  0004 ae1388        	ldw	x,#5000
 612  0007 cd0000        	call	_delay
 614                     ; 238 	POWER_UP_RESET_CC1100();
 616  000a cd0000        	call	_POWER_UP_RESET_CC1100
 618                     ; 239 	halRfWriteRfSettings();
 620  000d cd0000        	call	_halRfWriteRfSettings
 622                     ; 240 	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
 624  0010 4b08          	push	#8
 625  0012 ae0000        	ldw	x,#_PaTabel
 626  0015 89            	pushw	x
 627  0016 a63e          	ld	a,#62
 628  0018 cd0000        	call	_halSpiWriteBurstReg
 630  001b 5b03          	addw	sp,#3
 631                     ; 244 	return 0;
 633  001d 4f            	clr	a
 636  001e 81            	ret
 671                     ; 254 void RESET_CC1100(void) 
 671                     ; 255 {
 672                     .text:	section	.text,new
 673  0000               _RESET_CC1100:
 675  0000 89            	pushw	x
 676       00000002      OFST:	set	2
 679                     ; 256 	INT16U x=0;
 681  0001 5f            	clrw	x
 682  0002 1f01          	ldw	(OFST-1,sp),x
 683                     ; 257 	SetCSN(0); 
 685  0004 72135000      	bres	20480,#1
 688  0008 2018          	jra	L362
 689  000a               L162:
 690                     ; 258 	while (GetMISO()){if(x>10000)break;x++;};
 692  000a 1e01          	ldw	x,(OFST-1,sp)
 693  000c a32711        	cpw	x,#10001
 694  000f 250a          	jrult	L762
 696  0011               L562:
 697                     ; 259   	SPI_SendByte(CCxxx0_SRES); 		//写入复位命令
 700  0011 a630          	ld	a,#48
 701  0013 cd0000        	call	_SPI_SendByte
 703                     ; 260 	x=0;
 705  0016 5f            	clrw	x
 706  0017 1f01          	ldw	(OFST-1,sp),x
 708  0019 2024          	jra	L572
 709  001b               L762:
 710                     ; 258 	while (GetMISO()){if(x>10000)break;x++;};
 712  001b 1e01          	ldw	x,(OFST-1,sp)
 713  001d 1c0001        	addw	x,#1
 714  0020 1f01          	ldw	(OFST-1,sp),x
 715  0022               L362:
 718  0022 c6500b        	ld	a,20491
 719  0025 a580          	bcp	a,#128
 720  0027 26e1          	jrne	L162
 721  0029 20e6          	jra	L562
 722  002b               L172:
 723                     ; 261 	while (GetMISO()){if(x>10000)break;x++;}; 
 725  002b 1e01          	ldw	x,(OFST-1,sp)
 726  002d a32711        	cpw	x,#10001
 727  0030 2506          	jrult	L103
 729  0032               L772:
 730                     ; 262   	SetCSN(1); 
 733  0032 72125000      	bset	20480,#1
 734                     ; 263 }
 738  0036 85            	popw	x
 739  0037 81            	ret
 740  0038               L103:
 741                     ; 261 	while (GetMISO()){if(x>10000)break;x++;}; 
 743  0038 1e01          	ldw	x,(OFST-1,sp)
 744  003a 1c0001        	addw	x,#1
 745  003d 1f01          	ldw	(OFST-1,sp),x
 746  003f               L572:
 749  003f c6500b        	ld	a,20491
 750  0042 a580          	bcp	a,#128
 751  0044 26e5          	jrne	L172
 752  0046 20ea          	jra	L772
 778                     ; 271 void POWER_UP_RESET_CC1100(void) 
 778                     ; 272 {
 779                     .text:	section	.text,new
 780  0000               _POWER_UP_RESET_CC1100:
 784                     ; 273 	SetCSN(1); 
 786  0000 72125000      	bset	20480,#1
 787                     ; 274 	halWait(1); 
 790  0004 ae0001        	ldw	x,#1
 791  0007 cd0000        	call	_halWait
 793                     ; 275 	SetCSN(0); 
 795  000a 72135000      	bres	20480,#1
 796                     ; 276 	halWait(1); 
 799  000e ae0001        	ldw	x,#1
 800  0011 cd0000        	call	_halWait
 802                     ; 277 	SetCSN(1); 
 804  0014 72125000      	bset	20480,#1
 805                     ; 278 	halWait(41); 
 808  0018 ae0029        	ldw	x,#41
 809  001b cd0000        	call	_halWait
 811                     ; 279 	RESET_CC1100();   		//复位CC1100
 813  001e cd0000        	call	_RESET_CC1100
 815                     ; 280 }
 818  0021 81            	ret
 871                     ; 288 void halSpiWriteReg(INT8U addr, INT8U value) 
 871                     ; 289 {
 872                     .text:	section	.text,new
 873  0000               _halSpiWriteReg:
 875  0000 89            	pushw	x
 876  0001 89            	pushw	x
 877       00000002      OFST:	set	2
 880                     ; 290 	INT16U x=0;
 882  0002 5f            	clrw	x
 883  0003 1f01          	ldw	(OFST-1,sp),x
 884                     ; 292     SetCSN(0);
 886  0005 72135000      	bres	20480,#1
 889  0009 201f          	jra	L343
 890  000b               L143:
 891                     ; 293     while (GetMISO()){if(x>10000)break;x++;};
 893  000b 1e01          	ldw	x,(OFST-1,sp)
 894  000d a32711        	cpw	x,#10001
 895  0010 2511          	jrult	L743
 897  0012               L543:
 898                     ; 294     SPI_SendByte(addr);		//写地址
 901  0012 7b03          	ld	a,(OFST+1,sp)
 902  0014 cd0000        	call	_SPI_SendByte
 904                     ; 295     SPI_SendByte(value);		//写入配置
 906  0017 7b04          	ld	a,(OFST+2,sp)
 907  0019 cd0000        	call	_SPI_SendByte
 909                     ; 296     SetCSN(1);
 911  001c 72125000      	bset	20480,#1
 912                     ; 297 }
 916  0020 5b04          	addw	sp,#4
 917  0022 81            	ret
 918  0023               L743:
 919                     ; 293     while (GetMISO()){if(x>10000)break;x++;};
 921  0023 1e01          	ldw	x,(OFST-1,sp)
 922  0025 1c0001        	addw	x,#1
 923  0028 1f01          	ldw	(OFST-1,sp),x
 924  002a               L343:
 927  002a c6500b        	ld	a,20491
 928  002d a580          	bcp	a,#128
 929  002f 26da          	jrne	L143
 930  0031 20df          	jra	L543
1013                     	switch	.const
1014  0026               L23:
1015  0026 0000ea61      	dc.l	60001
1016                     ; 305 void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
1016                     ; 306 {
1017                     .text:	section	.text,new
1018  0000               _halSpiWriteBurstReg:
1020  0000 88            	push	a
1021  0001 5203          	subw	sp,#3
1022       00000003      OFST:	set	3
1025                     ; 307 	INT16U x=0;
1027  0003 5f            	clrw	x
1028  0004 1f01          	ldw	(OFST-2,sp),x
1029                     ; 309 	temp = addr | WRITE_BURST;
1031  0006 aa40          	or	a,#64
1032  0008 6b03          	ld	(OFST+0,sp),a
1033                     ; 310     SetCSN(0);
1035  000a 72135000      	bres	20480,#1
1038  000e 201e          	jra	L514
1039  0010               L314:
1040                     ; 311     while (GetMISO()){if(x>60000)break;x++;};;
1042  0010 9c            	rvf
1043  0011 1e01          	ldw	x,(OFST-2,sp)
1044  0013 cd0000        	call	c_uitolx
1046  0016 ae0026        	ldw	x,#L23
1047  0019 cd0000        	call	c_lcmp
1049  001c 2f09          	jrslt	L124
1051  001e               L714:
1052                     ; 312     SPI_SendData(temp);
1056  001e 7b03          	ld	a,(OFST+0,sp)
1057  0020 cd0000        	call	_SPI_SendData
1059                     ; 313     for (i = 0; i < count; i++)
1061  0023 0f03          	clr	(OFST+0,sp)
1063  0025 2021          	jra	L724
1064  0027               L124:
1065                     ; 311     while (GetMISO()){if(x>60000)break;x++;};;
1067  0027 1e01          	ldw	x,(OFST-2,sp)
1068  0029 1c0001        	addw	x,#1
1069  002c 1f01          	ldw	(OFST-2,sp),x
1070  002e               L514:
1073  002e c6500b        	ld	a,20491
1074  0031 a580          	bcp	a,#128
1075  0033 26db          	jrne	L314
1076  0035 20e7          	jra	L714
1077  0037               L324:
1078                     ; 315 		SPI_SendByte(buffer[i]);
1080  0037 7b07          	ld	a,(OFST+4,sp)
1081  0039 97            	ld	xl,a
1082  003a 7b08          	ld	a,(OFST+5,sp)
1083  003c 1b03          	add	a,(OFST+0,sp)
1084  003e 2401          	jrnc	L43
1085  0040 5c            	incw	x
1086  0041               L43:
1087  0041 02            	rlwa	x,a
1088  0042 f6            	ld	a,(x)
1089  0043 cd0000        	call	_SPI_SendByte
1091                     ; 313     for (i = 0; i < count; i++)
1093  0046 0c03          	inc	(OFST+0,sp)
1094  0048               L724:
1097  0048 7b03          	ld	a,(OFST+0,sp)
1098  004a 1109          	cp	a,(OFST+6,sp)
1099  004c 25e9          	jrult	L324
1100                     ; 317     SetCSN(1);
1102  004e 72125000      	bset	20480,#1
1103                     ; 318 }
1107  0052 5b04          	addw	sp,#4
1108  0054 81            	ret
1152                     ; 326 void halSpiStrobe(INT8U strobe) 
1152                     ; 327 {
1153                     .text:	section	.text,new
1154  0000               _halSpiStrobe:
1156  0000 88            	push	a
1157  0001 89            	pushw	x
1158       00000002      OFST:	set	2
1161                     ; 328 	INT16U x=0;
1163  0002 5f            	clrw	x
1164  0003 1f01          	ldw	(OFST-1,sp),x
1165                     ; 329     SetCSN(0);
1167  0005 72135000      	bres	20480,#1
1170  0009 201a          	jra	L754
1171  000b               L554:
1172                     ; 330     while (GetMISO()){if(x>10000)break;x++;};
1174  000b 1e01          	ldw	x,(OFST-1,sp)
1175  000d a32711        	cpw	x,#10001
1176  0010 250c          	jrult	L364
1178  0012               L164:
1179                     ; 331     SPI_SendByte(strobe);		//写入命令
1182  0012 7b03          	ld	a,(OFST+1,sp)
1183  0014 cd0000        	call	_SPI_SendByte
1185                     ; 332     SetCSN(1);
1187  0017 72125000      	bset	20480,#1
1188                     ; 333 }
1192  001b 5b03          	addw	sp,#3
1193  001d 81            	ret
1194  001e               L364:
1195                     ; 330     while (GetMISO()){if(x>10000)break;x++;};
1197  001e 1e01          	ldw	x,(OFST-1,sp)
1198  0020 1c0001        	addw	x,#1
1199  0023 1f01          	ldw	(OFST-1,sp),x
1200  0025               L754:
1203  0025 c6500b        	ld	a,20491
1204  0028 a580          	bcp	a,#128
1205  002a 26df          	jrne	L554
1206  002c 20e4          	jra	L164
1268                     ; 341 INT8U halSpiReadReg(INT8U addr) 
1268                     ; 342 {
1269                     .text:	section	.text,new
1270  0000               _halSpiReadReg:
1272  0000 5203          	subw	sp,#3
1273       00000003      OFST:	set	3
1276                     ; 343 	INT16U x=0;
1278  0002 5f            	clrw	x
1279  0003 1f02          	ldw	(OFST-1,sp),x
1280                     ; 345   	temp = addr|READ_SINGLE;//读寄存器命令
1282  0005 aa80          	or	a,#128
1283  0007 6b01          	ld	(OFST-2,sp),a
1284                     ; 346 	SetCSN(0);
1286  0009 72135000      	bres	20480,#1
1289  000d 2022          	jra	L125
1290  000f               L715:
1291                     ; 347 	while (GetMISO()){if(x>10000)break;x++;};;
1293  000f 1e02          	ldw	x,(OFST-1,sp)
1294  0011 a32711        	cpw	x,#10001
1295  0014 2514          	jrult	L525
1297  0016               L325:
1298                     ; 348 	SPI_SendByte(temp);
1302  0016 7b01          	ld	a,(OFST-2,sp)
1303  0018 cd0000        	call	_SPI_SendByte
1305                     ; 349 	value = SPI_SendByte(0);
1307  001b 4f            	clr	a
1308  001c cd0000        	call	_SPI_SendByte
1310  001f 6b01          	ld	(OFST-2,sp),a
1311                     ; 350 	SetCSN(1);
1313  0021 72125000      	bset	20480,#1
1314                     ; 351 	return value;
1317  0025 7b01          	ld	a,(OFST-2,sp)
1320  0027 5b03          	addw	sp,#3
1321  0029 81            	ret
1322  002a               L525:
1323                     ; 347 	while (GetMISO()){if(x>10000)break;x++;};;
1325  002a 1e02          	ldw	x,(OFST-1,sp)
1326  002c 1c0001        	addw	x,#1
1327  002f 1f02          	ldw	(OFST-1,sp),x
1328  0031               L125:
1331  0031 c6500b        	ld	a,20491
1332  0034 a580          	bcp	a,#128
1333  0036 26d7          	jrne	L715
1334  0038 20dc          	jra	L325
1415                     ; 361 void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
1415                     ; 362 {
1416                     .text:	section	.text,new
1417  0000               _halSpiReadBurstReg:
1419  0000 88            	push	a
1420  0001 5203          	subw	sp,#3
1421       00000003      OFST:	set	3
1424                     ; 363 	INT16U x=0;
1426  0003 5f            	clrw	x
1427  0004 1f01          	ldw	(OFST-2,sp),x
1428                     ; 365 	temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
1430  0006 aac0          	or	a,#192
1431  0008 6b03          	ld	(OFST+0,sp),a
1432                     ; 366     SetCSN(0);
1434  000a 72135000      	bres	20480,#1
1437  000e 2017          	jra	L375
1438  0010               L175:
1439                     ; 367     while (GetMISO()){if(x>10000)break;x++;};;
1441  0010 1e01          	ldw	x,(OFST-2,sp)
1442  0012 a32711        	cpw	x,#10001
1443  0015 2509          	jrult	L775
1445  0017               L575:
1446                     ; 368 	SPI_SendByte(temp);   
1450  0017 7b03          	ld	a,(OFST+0,sp)
1451  0019 cd0000        	call	_SPI_SendByte
1453                     ; 369     for (i = 0; i < count; i++) 
1455  001c 0f03          	clr	(OFST+0,sp)
1457  001e 2024          	jra	L506
1458  0020               L775:
1459                     ; 367     while (GetMISO()){if(x>10000)break;x++;};;
1461  0020 1e01          	ldw	x,(OFST-2,sp)
1462  0022 1c0001        	addw	x,#1
1463  0025 1f01          	ldw	(OFST-2,sp),x
1464  0027               L375:
1467  0027 c6500b        	ld	a,20491
1468  002a a580          	bcp	a,#128
1469  002c 26e2          	jrne	L175
1470  002e 20e7          	jra	L575
1471  0030               L106:
1472                     ; 371         buffer[i] = SPI_SendByte(0);
1474  0030 7b07          	ld	a,(OFST+4,sp)
1475  0032 97            	ld	xl,a
1476  0033 7b08          	ld	a,(OFST+5,sp)
1477  0035 1b03          	add	a,(OFST+0,sp)
1478  0037 2401          	jrnc	L44
1479  0039 5c            	incw	x
1480  003a               L44:
1481  003a 02            	rlwa	x,a
1482  003b 89            	pushw	x
1483  003c 4f            	clr	a
1484  003d cd0000        	call	_SPI_SendByte
1486  0040 85            	popw	x
1487  0041 f7            	ld	(x),a
1488                     ; 369     for (i = 0; i < count; i++) 
1490  0042 0c03          	inc	(OFST+0,sp)
1491  0044               L506:
1494  0044 7b03          	ld	a,(OFST+0,sp)
1495  0046 1109          	cp	a,(OFST+6,sp)
1496  0048 25e6          	jrult	L106
1497                     ; 373     SetCSN(1);
1499  004a 72125000      	bset	20480,#1
1500                     ; 374 }
1504  004e 5b04          	addw	sp,#4
1505  0050 81            	ret
1567                     ; 383 INT8U halSpiReadStatus(INT8U addr) 
1567                     ; 384 {
1568                     .text:	section	.text,new
1569  0000               _halSpiReadStatus:
1571  0000 5203          	subw	sp,#3
1572       00000003      OFST:	set	3
1575                     ; 385 	INT16U x=0;
1577  0002 5f            	clrw	x
1578  0003 1f02          	ldw	(OFST-1,sp),x
1579                     ; 387 	temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
1581  0005 aac0          	or	a,#192
1582  0007 6b01          	ld	(OFST-2,sp),a
1583                     ; 388     SetCSN(0);
1585  0009 72135000      	bres	20480,#1
1588  000d 2022          	jra	L546
1589  000f               L346:
1590                     ; 389     while (GetMISO()){if(x>10000)break;x++;};;
1592  000f 1e02          	ldw	x,(OFST-1,sp)
1593  0011 a32711        	cpw	x,#10001
1594  0014 2514          	jrult	L156
1596  0016               L746:
1597                     ; 390     SPI_SendByte(temp);
1601  0016 7b01          	ld	a,(OFST-2,sp)
1602  0018 cd0000        	call	_SPI_SendByte
1604                     ; 391 	value = SPI_SendByte(0);
1606  001b 4f            	clr	a
1607  001c cd0000        	call	_SPI_SendByte
1609  001f 6b01          	ld	(OFST-2,sp),a
1610                     ; 392 	SetCSN(1);
1612  0021 72125000      	bset	20480,#1
1613                     ; 393 	return value;
1616  0025 7b01          	ld	a,(OFST-2,sp)
1619  0027 5b03          	addw	sp,#3
1620  0029 81            	ret
1621  002a               L156:
1622                     ; 389     while (GetMISO()){if(x>10000)break;x++;};;
1624  002a 1e02          	ldw	x,(OFST-1,sp)
1625  002c 1c0001        	addw	x,#1
1626  002f 1f02          	ldw	(OFST-1,sp),x
1627  0031               L546:
1630  0031 c6500b        	ld	a,20491
1631  0034 a580          	bcp	a,#128
1632  0036 26d7          	jrne	L346
1633  0038 20dc          	jra	L746
1659                     ; 403 void halRfWriteRfSettings(void) 
1659                     ; 404 {
1660                     .text:	section	.text,new
1661  0000               _halRfWriteRfSettings:
1665                     ; 406     halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
1667  0000 ae0008        	ldw	x,#8
1668  0003 a60b          	ld	a,#11
1669  0005 95            	ld	xh,a
1670  0006 cd0000        	call	_halSpiWriteReg
1672                     ; 407     halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
1674  0009 5f            	clrw	x
1675  000a a60c          	ld	a,#12
1676  000c 95            	ld	xh,a
1677  000d cd0000        	call	_halSpiWriteReg
1679                     ; 408     halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
1681  0010 ae0010        	ldw	x,#16
1682  0013 a60d          	ld	a,#13
1683  0015 95            	ld	xh,a
1684  0016 cd0000        	call	_halSpiWriteReg
1686                     ; 409     halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
1688  0019 ae00a7        	ldw	x,#167
1689  001c a60e          	ld	a,#14
1690  001e 95            	ld	xh,a
1691  001f cd0000        	call	_halSpiWriteReg
1693                     ; 410     halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
1695  0022 ae0062        	ldw	x,#98
1696  0025 a60f          	ld	a,#15
1697  0027 95            	ld	xh,a
1698  0028 cd0000        	call	_halSpiWriteReg
1700                     ; 411     halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
1702  002b ae0058        	ldw	x,#88
1703  002e a610          	ld	a,#16
1704  0030 95            	ld	xh,a
1705  0031 cd0000        	call	_halSpiWriteReg
1707                     ; 412     halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
1709  0034 ae0043        	ldw	x,#67
1710  0037 a611          	ld	a,#17
1711  0039 95            	ld	xh,a
1712  003a cd0000        	call	_halSpiWriteReg
1714                     ; 413     halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
1716  003d ae0003        	ldw	x,#3
1717  0040 a612          	ld	a,#18
1718  0042 95            	ld	xh,a
1719  0043 cd0000        	call	_halSpiWriteReg
1721                     ; 414     halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
1723  0046 ae0022        	ldw	x,#34
1724  0049 a613          	ld	a,#19
1725  004b 95            	ld	xh,a
1726  004c cd0000        	call	_halSpiWriteReg
1728                     ; 415     halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
1730  004f ae00f8        	ldw	x,#248
1731  0052 a614          	ld	a,#20
1732  0054 95            	ld	xh,a
1733  0055 cd0000        	call	_halSpiWriteReg
1735                     ; 416     halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
1737  0058 5f            	clrw	x
1738  0059 a60a          	ld	a,#10
1739  005b 95            	ld	xh,a
1740  005c cd0000        	call	_halSpiWriteReg
1742                     ; 417     halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
1744  005f ae0047        	ldw	x,#71
1745  0062 a615          	ld	a,#21
1746  0064 95            	ld	xh,a
1747  0065 cd0000        	call	_halSpiWriteReg
1749                     ; 418     halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
1751  0068 ae0056        	ldw	x,#86
1752  006b a621          	ld	a,#33
1753  006d 95            	ld	xh,a
1754  006e cd0000        	call	_halSpiWriteReg
1756                     ; 419     halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
1758  0071 ae0010        	ldw	x,#16
1759  0074 a622          	ld	a,#34
1760  0076 95            	ld	xh,a
1761  0077 cd0000        	call	_halSpiWriteReg
1763                     ; 421     halSpiWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
1765  007a ae0018        	ldw	x,#24
1766  007d a618          	ld	a,#24
1767  007f 95            	ld	xh,a
1768  0080 cd0000        	call	_halSpiWriteReg
1770                     ; 422     halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
1772  0083 ae001d        	ldw	x,#29
1773  0086 a619          	ld	a,#25
1774  0088 95            	ld	xh,a
1775  0089 cd0000        	call	_halSpiWriteReg
1777                     ; 423     halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
1779  008c ae001c        	ldw	x,#28
1780  008f a61a          	ld	a,#26
1781  0091 95            	ld	xh,a
1782  0092 cd0000        	call	_halSpiWriteReg
1784                     ; 424     halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
1786  0095 ae0043        	ldw	x,#67
1787  0098 a61b          	ld	a,#27
1788  009a 95            	ld	xh,a
1789  009b cd0000        	call	_halSpiWriteReg
1791                     ; 425 	halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
1793  009e ae0040        	ldw	x,#64
1794  00a1 a61c          	ld	a,#28
1795  00a3 95            	ld	xh,a
1796  00a4 cd0000        	call	_halSpiWriteReg
1798                     ; 426     halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
1800  00a7 ae0091        	ldw	x,#145
1801  00aa a61d          	ld	a,#29
1802  00ac 95            	ld	xh,a
1803  00ad cd0000        	call	_halSpiWriteReg
1805                     ; 427     halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
1807  00b0 ae00ea        	ldw	x,#234
1808  00b3 a623          	ld	a,#35
1809  00b5 95            	ld	xh,a
1810  00b6 cd0000        	call	_halSpiWriteReg
1812                     ; 428 	halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
1814  00b9 ae002a        	ldw	x,#42
1815  00bc a624          	ld	a,#36
1816  00be 95            	ld	xh,a
1817  00bf cd0000        	call	_halSpiWriteReg
1819                     ; 429 	halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
1821  00c2 5f            	clrw	x
1822  00c3 a625          	ld	a,#37
1823  00c5 95            	ld	xh,a
1824  00c6 cd0000        	call	_halSpiWriteReg
1826                     ; 430     halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
1828  00c9 ae0011        	ldw	x,#17
1829  00cc a626          	ld	a,#38
1830  00ce 95            	ld	xh,a
1831  00cf cd0000        	call	_halSpiWriteReg
1833                     ; 431     halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
1835  00d2 ae0059        	ldw	x,#89
1836  00d5 a629          	ld	a,#41
1837  00d7 95            	ld	xh,a
1838  00d8 cd0000        	call	_halSpiWriteReg
1840                     ; 432     halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
1842  00db ae0081        	ldw	x,#129
1843  00de a62c          	ld	a,#44
1844  00e0 95            	ld	xh,a
1845  00e1 cd0000        	call	_halSpiWriteReg
1847                     ; 433     halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
1849  00e4 ae0035        	ldw	x,#53
1850  00e7 a62d          	ld	a,#45
1851  00e9 95            	ld	xh,a
1852  00ea cd0000        	call	_halSpiWriteReg
1854                     ; 434     halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
1856  00ed ae0009        	ldw	x,#9
1857  00f0 a62e          	ld	a,#46
1858  00f2 95            	ld	xh,a
1859  00f3 cd0000        	call	_halSpiWriteReg
1861                     ; 435     halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
1863  00f6 ae000b        	ldw	x,#11
1864  00f9 4f            	clr	a
1865  00fa 95            	ld	xh,a
1866  00fb cd0000        	call	_halSpiWriteReg
1868                     ; 436     halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
1870  00fe ae0006        	ldw	x,#6
1871  0101 a602          	ld	a,#2
1872  0103 95            	ld	xh,a
1873  0104 cd0000        	call	_halSpiWriteReg
1875                     ; 437     halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
1877  0107 ae0004        	ldw	x,#4
1878  010a a607          	ld	a,#7
1879  010c 95            	ld	xh,a
1880  010d cd0000        	call	_halSpiWriteReg
1882                     ; 438     halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
1884  0110 ae0005        	ldw	x,#5
1885  0113 a608          	ld	a,#8
1886  0115 95            	ld	xh,a
1887  0116 cd0000        	call	_halSpiWriteReg
1889                     ; 439     halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
1891  0119 5f            	clrw	x
1892  011a a609          	ld	a,#9
1893  011c 95            	ld	xh,a
1894  011d cd0000        	call	_halSpiWriteReg
1896                     ; 440     halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
1898  0120 ae0020        	ldw	x,#32
1899  0123 a606          	ld	a,#6
1900  0125 95            	ld	xh,a
1901  0126 cd0000        	call	_halSpiWriteReg
1903                     ; 450 }
1906  0129 81            	ret
1963                     ; 461 void halRfSendPacket(INT8U *txBuffer, INT8U size) 
1963                     ; 462 {
1964                     .text:	section	.text,new
1965  0000               _halRfSendPacket:
1967  0000 89            	pushw	x
1968  0001 89            	pushw	x
1969       00000002      OFST:	set	2
1972                     ; 463 	INT16U x=0;
1974  0002 5f            	clrw	x
1975  0003 1f01          	ldw	(OFST-1,sp),x
1976                     ; 464 	halSpiStrobe(CCxxx0_SIDLE);
1978  0005 a636          	ld	a,#54
1979  0007 cd0000        	call	_halSpiStrobe
1981                     ; 466 	halSpiWriteReg(CCxxx0_TXFIFO, size);
1983  000a 7b07          	ld	a,(OFST+5,sp)
1984  000c 97            	ld	xl,a
1985  000d a63f          	ld	a,#63
1986  000f 95            	ld	xh,a
1987  0010 cd0000        	call	_halSpiWriteReg
1989                     ; 467   	halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//写入要发送的数据
1991  0013 7b07          	ld	a,(OFST+5,sp)
1992  0015 88            	push	a
1993  0016 1e04          	ldw	x,(OFST+2,sp)
1994  0018 89            	pushw	x
1995  0019 a63f          	ld	a,#63
1996  001b cd0000        	call	_halSpiWriteBurstReg
1998  001e 5b03          	addw	sp,#3
1999                     ; 469   	halSpiStrobe(CCxxx0_STX);		//进入发送模式发送数据
2001  0020 a635          	ld	a,#53
2002  0022 cd0000        	call	_halSpiStrobe
2005  0025 2020          	jra	L317
2006  0027               L117:
2007                     ; 472   	while (GetGDO0()==0) {halWait(1);if(x>60000)break;x++;}
2009  0027 ae0001        	ldw	x,#1
2010  002a cd0000        	call	_halWait
2014  002d 9c            	rvf
2015  002e 1e01          	ldw	x,(OFST-1,sp)
2016  0030 cd0000        	call	c_uitolx
2018  0033 ae0026        	ldw	x,#L23
2019  0036 cd0000        	call	c_lcmp
2021  0039 2f05          	jrslt	L717
2023  003b               L517:
2024                     ; 474 	x=0;
2026  003b 5f            	clrw	x
2027  003c 1f01          	ldw	(OFST-1,sp),x
2029  003e 2033          	jra	L527
2030  0040               L717:
2031                     ; 472   	while (GetGDO0()==0) {halWait(1);if(x>60000)break;x++;}
2033  0040 1e01          	ldw	x,(OFST-1,sp)
2034  0042 1c0001        	addw	x,#1
2035  0045 1f01          	ldw	(OFST-1,sp),x
2036  0047               L317:
2039  0047 c65010        	ld	a,20496
2040  004a a510          	bcp	a,#16
2041  004c 27d9          	jreq	L117
2042  004e 20eb          	jra	L517
2043  0050               L127:
2044                     ; 475 	while (GetGDO0()){halWait(1);if(x>60000)break;x++;}
2046  0050 ae0001        	ldw	x,#1
2047  0053 cd0000        	call	_halWait
2051  0056 9c            	rvf
2052  0057 1e01          	ldw	x,(OFST-1,sp)
2053  0059 cd0000        	call	c_uitolx
2055  005c ae0026        	ldw	x,#L23
2056  005f cd0000        	call	c_lcmp
2058  0062 2f08          	jrslt	L137
2060  0064               L727:
2061                     ; 476 	halSpiStrobe(CCxxx0_SFTX);
2063  0064 a63b          	ld	a,#59
2064  0066 cd0000        	call	_halSpiStrobe
2066                     ; 477 }
2069  0069 5b04          	addw	sp,#4
2070  006b 81            	ret
2071  006c               L137:
2072                     ; 475 	while (GetGDO0()){halWait(1);if(x>60000)break;x++;}
2074  006c 1e01          	ldw	x,(OFST-1,sp)
2075  006e 1c0001        	addw	x,#1
2076  0071 1f01          	ldw	(OFST-1,sp),x
2077  0073               L527:
2080  0073 c65010        	ld	a,20496
2081  0076 a510          	bcp	a,#16
2082  0078 26d6          	jrne	L127
2083  007a 20e8          	jra	L727
2162                     ; 479 INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
2162                     ; 480 {
2163                     .text:	section	.text,new
2164  0000               _halRfReceivePacket:
2166  0000 89            	pushw	x
2167  0001 5204          	subw	sp,#4
2168       00000004      OFST:	set	4
2171                     ; 483 	INT8U i=(*length)*4;  // 具体多少要根据datarate和length来决定
2173  0003 1e09          	ldw	x,(OFST+5,sp)
2174  0005 f6            	ld	a,(x)
2175  0006 48            	sll	a
2176  0007 48            	sll	a
2177  0008 6b01          	ld	(OFST-3,sp),a
2178                     ; 484     if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
2180  000a a63b          	ld	a,#59
2181  000c cd0000        	call	_halSpiReadStatus
2183  000f a57f          	bcp	a,#127
2184  0011 276e          	jreq	L177
2185                     ; 486       	packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
2187  0013 a63f          	ld	a,#63
2188  0015 cd0000        	call	_halSpiReadReg
2190  0018 6b02          	ld	(OFST-2,sp),a
2191                     ; 487       	if (packetLength <= *length) 		//如果所要的有效数据长度小于等于接收到的数据包的长度
2193  001a 1e09          	ldw	x,(OFST+5,sp)
2194  001c f6            	ld	a,(x)
2195  001d 1102          	cp	a,(OFST-2,sp)
2196  001f 2552          	jrult	L377
2197                     ; 489         	halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
2199  0021 7b02          	ld	a,(OFST-2,sp)
2200  0023 88            	push	a
2201  0024 1e06          	ldw	x,(OFST+2,sp)
2202  0026 89            	pushw	x
2203  0027 a63f          	ld	a,#63
2204  0029 cd0000        	call	_halSpiReadBurstReg
2206  002c 5b03          	addw	sp,#3
2207                     ; 490         	*length = packetLength;				//把接收数据长度的修改为当前数据的长度
2209  002e 7b02          	ld	a,(OFST-2,sp)
2210  0030 1e09          	ldw	x,(OFST+5,sp)
2211  0032 f7            	ld	(x),a
2212                     ; 493         	halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
2214  0033 4b02          	push	#2
2215  0035 96            	ldw	x,sp
2216  0036 1c0004        	addw	x,#OFST+0
2217  0039 89            	pushw	x
2218  003a a63f          	ld	a,#63
2219  003c cd0000        	call	_halSpiReadBurstReg
2221  003f 5b03          	addw	sp,#3
2222                     ; 496 			if(status[0] >= 128) 
2224  0041 7b03          	ld	a,(OFST-1,sp)
2225  0043 a180          	cp	a,#128
2226  0045 2515          	jrult	L577
2227                     ; 498 				CC1101SigValue = (status[0] - 256)/2 - 74;
2229  0047 7b03          	ld	a,(OFST-1,sp)
2230  0049 5f            	clrw	x
2231  004a 97            	ld	xl,a
2232  004b 1d0100        	subw	x,#256
2233  004e a602          	ld	a,#2
2234  0050 cd0000        	call	c_sdivx
2236  0053 1d004a        	subw	x,#74
2237  0056 01            	rrwa	x,a
2238  0057 b700          	ld	_CC1101SigValue,a
2239  0059 02            	rlwa	x,a
2241  005a 2007          	jra	L777
2242  005c               L577:
2243                     ; 502 				CC1101SigValue = (status[0])/2 - 74;
2245  005c 7b03          	ld	a,(OFST-1,sp)
2246  005e 44            	srl	a
2247  005f a04a          	sub	a,#74
2248  0061 b700          	ld	_CC1101SigValue,a
2249  0063               L777:
2250                     ; 507 			UART1_SendData8(status[0]); 
2252  0063 7b03          	ld	a,(OFST-1,sp)
2253  0065 cd0000        	call	_UART1_SendData8
2255                     ; 509 			halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
2257  0068 a63a          	ld	a,#58
2258  006a cd0000        	call	_halSpiStrobe
2260                     ; 510         	return (status[1] & CRC_OK);			//如果校验成功返回接收成功
2262  006d 7b04          	ld	a,(OFST+0,sp)
2263  006f a480          	and	a,#128
2265  0071 200b          	jra	L65
2266  0073               L377:
2267                     ; 514         	*length = packetLength;
2269  0073 7b02          	ld	a,(OFST-2,sp)
2270  0075 1e09          	ldw	x,(OFST+5,sp)
2271  0077 f7            	ld	(x),a
2272                     ; 515         	halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
2274  0078 a63a          	ld	a,#58
2275  007a cd0000        	call	_halSpiStrobe
2277                     ; 516         	return 0;
2279  007d 4f            	clr	a
2281  007e               L65:
2283  007e 5b06          	addw	sp,#6
2284  0080 81            	ret
2285  0081               L177:
2286                     ; 519 	else	return 0;
2288  0081 4f            	clr	a
2290  0082 20fa          	jra	L65
2598                     	xdef	_halWait
2599                     	xdef	_rfSettings
2600                     	xdef	_CC1101_Init
2601                     	xdef	_StartRxWor
2602                     	xdef	_Init_WOR
2603                     	xdef	_halRfReceivePacket
2604                     	xdef	_halRfSendPacket
2605                     	xdef	_halRfWriteRfSettings
2606                     	xdef	_halSpiReadStatus
2607                     	xdef	_halSpiReadBurstReg
2608                     	xdef	_halSpiReadReg
2609                     	xdef	_halSpiStrobe
2610                     	xdef	_halSpiWriteBurstReg
2611                     	xdef	_halSpiWriteReg
2612                     	xdef	_POWER_UP_RESET_CC1100
2613                     	xdef	_RESET_CC1100
2614                     	xdef	_delay
2615                     	xdef	_PaTabel
2616                     	xdef	_SPI_SendByte
2617                     	xdef	_SPITransferByte
2618                     	xref	_Delayus
2619                     	xref.b	_CC1101SigValue
2620                     	xref	_UART1_SendData8
2621                     	xref	_SPI_SendData
2640                     	xref	c_sdivx
2641                     	xref	c_lcmp
2642                     	xref	c_uitolx
2643                     	end
