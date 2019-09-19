   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
   4                     	bsct
   5  0000               _audio_play:
   6  0000 00            	dc.b	0
   7  0001               _play_start:
   8  0001 00            	dc.b	0
   9  0002               _play_end:
  10  0002 00            	dc.b	0
  11                     .const:	section	.text
  12  0000               _byVoiceDataAddr:
  13  0000 00            	dc.b	0
  14  0001 01            	dc.b	1
  15  0002 02            	dc.b	2
  16  0003 03            	dc.b	3
  17  0004 04            	dc.b	4
  18  0005 05            	dc.b	5
  19  0006 06            	dc.b	6
  20  0007 07            	dc.b	7
  21  0008 08            	dc.b	8
  22  0009 09            	dc.b	9
  23  000a 0a            	dc.b	10
  24  000b 0b            	dc.b	11
  25  000c 0c            	dc.b	12
  26  000d 0d            	dc.b	13
  27  000e 0e            	dc.b	14
  28  000f 0f            	dc.b	15
  29  0010 10            	dc.b	16
  30  0011 11            	dc.b	17
  31  0012 12            	dc.b	18
  32  0013 13            	dc.b	19
  33  0014 14            	dc.b	20
  34  0015 15            	dc.b	21
  35  0016 16            	dc.b	22
  84                     ; 53 void Delayms(uint16 ms)	  
  84                     ; 54 {
  86                     	switch	.text
  87  0000               _Delayms:
  89  0000 89            	pushw	x
  90  0001 89            	pushw	x
  91       00000002      OFST:	set	2
  94  0002 2010          	jra	L53
  95  0004               L33:
  96                     ; 57 		for(i=300;i>0;i--);
  98  0004 ae012c        	ldw	x,#300
  99  0007 1f01          	ldw	(OFST-1,sp),x
 100  0009               L14:
 104  0009 1e01          	ldw	x,(OFST-1,sp)
 105  000b 1d0001        	subw	x,#1
 106  000e 1f01          	ldw	(OFST-1,sp),x
 109  0010 1e01          	ldw	x,(OFST-1,sp)
 110  0012 26f5          	jrne	L14
 111  0014               L53:
 112                     ; 56    while(ms--)
 114  0014 1e03          	ldw	x,(OFST+1,sp)
 115  0016 1d0001        	subw	x,#1
 116  0019 1f03          	ldw	(OFST+1,sp),x
 117  001b 1c0001        	addw	x,#1
 118  001e a30000        	cpw	x,#0
 119  0021 26e1          	jrne	L33
 120                     ; 58 }
 123  0023 5b04          	addw	sp,#4
 124  0025 81            	ret
 161                     ; 60 void Delayus(uint16 us)	  
 161                     ; 61 {
 162                     	switch	.text
 163  0026               _Delayus:
 165  0026 89            	pushw	x
 166       00000000      OFST:	set	0
 169  0027 2003          	jra	L76
 170  0029               L56:
 171                     ; 62  while(us--){nop();nop();nop();};
 174  0029 9d            nop
 180  002a 9d            nop
 186  002b 9d            nop
 189  002c               L76:
 192  002c 1e01          	ldw	x,(OFST+1,sp)
 193  002e 1d0001        	subw	x,#1
 194  0031 1f01          	ldw	(OFST+1,sp),x
 195  0033 1c0001        	addw	x,#1
 196  0036 a30000        	cpw	x,#0
 197  0039 26ee          	jrne	L56
 198                     ; 63 }
 202  003b 85            	popw	x
 203  003c 81            	ret
 238                     ; 65 void WTVxxxInit(unsigned char voml)
 238                     ; 66 {		
 239                     	switch	.text
 240  003d               _WTVxxxInit:
 244                     ; 68 	SetAUDRST(0);
 246  003d 7219500a      	bres	20490,#4
 247                     ; 69 	Delayms(5); 		
 250  0041 ae0005        	ldw	x,#5
 251  0044 adba          	call	_Delayms
 253                     ; 70 	SetAUDRST(1);
 255  0046 7218500a      	bset	20490,#4
 256                     ; 71 	Delayms(10);
 259  004a ae000a        	ldw	x,#10
 260  004d adb1          	call	_Delayms
 262                     ; 72 }
 265  004f 81            	ret
 294                     ; 73 void StopPlay(void)
 294                     ; 74 {
 295                     	switch	.text
 296  0050               _StopPlay:
 300                     ; 75 	SPI_Cmd(DISABLE);
 302  0050 4f            	clr	a
 303  0051 cd0000        	call	_SPI_Cmd
 305                     ; 76 	Delayms(2);
 307  0054 ae0002        	ldw	x,#2
 308  0057 ada7          	call	_Delayms
 310                     ; 77 	SetSCK(1);
 312  0059 721a500a      	bset	20490,#5
 313                     ; 78   SetAUDCS(0);
 316  005d 7217500a      	bres	20490,#3
 317                     ; 79 	Delayms(2);
 320  0061 ae0002        	ldw	x,#2
 321  0064 ad9a          	call	_Delayms
 323                     ; 80 	SPITransferByte(CMD_CLOSE_PA);
 325  0066 a6f0          	ld	a,#240
 326  0068 cd0000        	call	_SPITransferByte
 328                     ; 81 	SetAUDCS(1);
 330  006b 7216500a      	bset	20490,#3
 331                     ; 82 	SPI_Cmd(ENABLE);
 334  006f a601          	ld	a,#1
 335  0071 cd0000        	call	_SPI_Cmd
 337                     ; 83 	play_end=play_start;
 339  0074 450102        	mov	_play_end,_play_start
 340                     ; 84 	audio_play=0;
 342  0077 3f00          	clr	_audio_play
 343                     ; 85 }
 346  0079 81            	ret
 377                     ; 89 void StartPlay(void)
 377                     ; 90 {   
 378                     	switch	.text
 379  007a               _StartPlay:
 383                     ; 91 	if (audio_play) return;
 385  007a 3d00          	tnz	_audio_play
 386  007c 2701          	jreq	L131
 390  007e 81            	ret
 391  007f               L131:
 392                     ; 92 	SPI_Cmd(DISABLE);
 394  007f 4f            	clr	a
 395  0080 cd0000        	call	_SPI_Cmd
 397                     ; 93 	Delayms(1);
 399  0083 ae0001        	ldw	x,#1
 400  0086 cd0000        	call	_Delayms
 402                     ; 94 	SetSCK(1);
 404  0089 721a500a      	bset	20490,#5
 405                     ; 95   SetAUDCS(0);
 408  008d 7217500a      	bres	20490,#3
 409                     ; 96 	Delayms(1);
 412  0091 ae0001        	ldw	x,#1
 413  0094 cd0000        	call	_Delayms
 415                     ; 97 	SPITransferByte(CMD_OPEN_PA);
 417  0097 a6f1          	ld	a,#241
 418  0099 cd0000        	call	_SPITransferByte
 420                     ; 98 	SPITransferByte(CMD_VOL_SET+voml); 
 422  009c b600          	ld	a,_voml
 423  009e abe0          	add	a,#224
 424  00a0 cd0000        	call	_SPITransferByte
 426                     ; 99 	SPITransferByte(CMD_PLAY+play_buf[play_start]);
 428  00a3 b601          	ld	a,_play_start
 429  00a5 5f            	clrw	x
 430  00a6 97            	ld	xl,a
 431  00a7 e600          	ld	a,(_play_buf,x)
 432  00a9 cd0000        	call	_SPITransferByte
 434                     ; 100 	play_start = (play_start+1)%30;
 436  00ac b601          	ld	a,_play_start
 437  00ae 5f            	clrw	x
 438  00af 97            	ld	xl,a
 439  00b0 5c            	incw	x
 440  00b1 a61e          	ld	a,#30
 441  00b3 cd0000        	call	c_smodx
 443  00b6 9f            	ld	a,xl
 444  00b7 b701          	ld	_play_start,a
 445                     ; 102 	SetAUDCS(1);
 447  00b9 7216500a      	bset	20490,#3
 448                     ; 103 	SPI_Cmd(ENABLE);
 451  00bd a601          	ld	a,#1
 452  00bf cd0000        	call	_SPI_Cmd
 454                     ; 104 	Delayms(1);
 456  00c2 ae0001        	ldw	x,#1
 457  00c5 cd0000        	call	_Delayms
 459                     ; 105 	audio_irq=0;
 461  00c8 3f00          	clr	_audio_irq
 462                     ; 106 	audio_play=1;
 464  00ca 35010000      	mov	_audio_play,#1
 465                     ; 107 }
 468  00ce 81            	ret
 495                     ; 109 void CheckPlay(void)
 495                     ; 110 {
 496                     	switch	.text
 497  00cf               _CheckPlay:
 501                     ; 112 	if (play_end!=play_start) StartPlay();
 503  00cf b602          	ld	a,_play_end
 504  00d1 b101          	cp	a,_play_start
 505  00d3 2704          	jreq	L341
 508  00d5 ada3          	call	_StartPlay
 511  00d7 2003          	jra	L541
 512  00d9               L341:
 513                     ; 113 	else StopPlay();	
 515  00d9 cd0050        	call	_StopPlay
 517  00dc               L541:
 518                     ; 114 }
 521  00dc 81            	ret
 559                     ; 116 void PlaySound(INT8U pos)
 559                     ; 117 {
 560                     	switch	.text
 561  00dd               _PlaySound:
 563  00dd 88            	push	a
 564       00000000      OFST:	set	0
 567                     ; 118 	play_buf[play_end] = pos;
 569  00de b602          	ld	a,_play_end
 570  00e0 5f            	clrw	x
 571  00e1 97            	ld	xl,a
 572  00e2 7b01          	ld	a,(OFST+1,sp)
 573  00e4 e700          	ld	(_play_buf,x),a
 574                     ; 119 	if (play_start==play_end) StartPlay();
 576  00e6 b601          	ld	a,_play_start
 577  00e8 b102          	cp	a,_play_end
 578  00ea 2602          	jrne	L561
 581  00ec ad8c          	call	_StartPlay
 583  00ee               L561:
 584                     ; 120 	play_end = (play_end+1)%30;
 586  00ee b602          	ld	a,_play_end
 587  00f0 5f            	clrw	x
 588  00f1 97            	ld	xl,a
 589  00f2 5c            	incw	x
 590  00f3 a61e          	ld	a,#30
 591  00f5 cd0000        	call	c_smodx
 593  00f8 9f            	ld	a,xl
 594  00f9 b702          	ld	_play_end,a
 595                     ; 121 }
 598  00fb 84            	pop	a
 599  00fc 81            	ret
 652                     ; 123 void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
 652                     ; 124 { 
 653                     	switch	.text
 654  00fd               _PlaySpeak:
 656  00fd 89            	pushw	x
 657       00000000      OFST:	set	0
 660                     ; 125 	if((dat2==6)||(dat2==7))  //¥≤Õ∑∫ÙΩ–ªÚ ‰“∫ÕÍ±œ
 662  00fe 7b05          	ld	a,(OFST+5,sp)
 663  0100 a106          	cp	a,#6
 664  0102 2709          	jreq	L712
 666  0104 7b05          	ld	a,(OFST+5,sp)
 667  0106 a107          	cp	a,#7
 668  0108 2703          	jreq	L42
 669  010a cc01a1        	jp	L512
 670  010d               L42:
 671  010d               L712:
 672                     ; 127 		if (dat==250) PlaySound(21);//º”¥≤
 674  010d 7b01          	ld	a,(OFST+1,sp)
 675  010f a1fa          	cp	a,#250
 676  0111 2604          	jrne	L122
 679  0113 a615          	ld	a,#21
 680  0115 adc6          	call	_PlaySound
 682  0117               L122:
 683                     ; 128 		dat = dat1;
 685  0117 7b02          	ld	a,(OFST+2,sp)
 686  0119 6b01          	ld	(OFST+1,sp),a
 687                     ; 129 		if (dat1>=100)   //???°•°¿°ßo?
 689  011b 7b02          	ld	a,(OFST+2,sp)
 690  011d a164          	cp	a,#100
 691  011f 251d          	jrult	L322
 692                     ; 131 			PlaySound(dat1/100);
 694  0121 7b02          	ld	a,(OFST+2,sp)
 695  0123 ae0064        	ldw	x,#100
 696  0126 51            	exgw	x,y
 697  0127 5f            	clrw	x
 698  0128 97            	ld	xl,a
 699  0129 65            	divw	x,y
 700  012a 9f            	ld	a,xl
 701  012b adb0          	call	_PlaySound
 703                     ; 132 			PlaySound(11);
 705  012d a60b          	ld	a,#11
 706  012f adac          	call	_PlaySound
 708                     ; 133 			dat1 %= 100;
 710  0131 7b02          	ld	a,(OFST+2,sp)
 711  0133 ae0064        	ldw	x,#100
 712  0136 51            	exgw	x,y
 713  0137 5f            	clrw	x
 714  0138 97            	ld	xl,a
 715  0139 65            	divw	x,y
 716  013a 909f          	ld	a,yl
 717  013c 6b02          	ld	(OFST+2,sp),a
 718  013e               L322:
 719                     ; 135 		if (dat1>=10)
 721  013e 7b02          	ld	a,(OFST+2,sp)
 722  0140 a10a          	cp	a,#10
 723  0142 252c          	jrult	L522
 724                     ; 137 			if (dat1>19||dat>100) PlaySound(dat1/10);
 726  0144 7b02          	ld	a,(OFST+2,sp)
 727  0146 a114          	cp	a,#20
 728  0148 2406          	jruge	L132
 730  014a 7b01          	ld	a,(OFST+1,sp)
 731  014c a165          	cp	a,#101
 732  014e 250c          	jrult	L722
 733  0150               L132:
 736  0150 7b02          	ld	a,(OFST+2,sp)
 737  0152 ae000a        	ldw	x,#10
 738  0155 51            	exgw	x,y
 739  0156 5f            	clrw	x
 740  0157 97            	ld	xl,a
 741  0158 65            	divw	x,y
 742  0159 9f            	ld	a,xl
 743  015a ad81          	call	_PlaySound
 745  015c               L722:
 746                     ; 138 			PlaySound(10);
 748  015c a60a          	ld	a,#10
 749  015e cd00dd        	call	_PlaySound
 751                     ; 139 			dat1 %= 10;
 753  0161 7b02          	ld	a,(OFST+2,sp)
 754  0163 ae000a        	ldw	x,#10
 755  0166 51            	exgw	x,y
 756  0167 5f            	clrw	x
 757  0168 97            	ld	xl,a
 758  0169 65            	divw	x,y
 759  016a 909f          	ld	a,yl
 760  016c 6b02          	ld	(OFST+2,sp),a
 762  016e 200a          	jra	L332
 763  0170               L522:
 764                     ; 140 		} else if(dat>100) PlaySound(0);
 766  0170 7b01          	ld	a,(OFST+1,sp)
 767  0172 a165          	cp	a,#101
 768  0174 2504          	jrult	L332
 771  0176 4f            	clr	a
 772  0177 cd00dd        	call	_PlaySound
 774  017a               L332:
 775                     ; 141 		if (dat1>0)	PlaySound(dat1);
 777  017a 0d02          	tnz	(OFST+2,sp)
 778  017c 2705          	jreq	L732
 781  017e 7b02          	ld	a,(OFST+2,sp)
 782  0180 cd00dd        	call	_PlaySound
 784  0183               L732:
 785                     ; 142 		PlaySound(15);//¥≤
 787  0183 a60f          	ld	a,#15
 788  0185 cd00dd        	call	_PlaySound
 790                     ; 143 		if (dat2==6) PlaySound(12);//∫ÙΩ–
 792  0188 7b05          	ld	a,(OFST+5,sp)
 793  018a a106          	cp	a,#6
 794  018c 2607          	jrne	L142
 797  018e a60c          	ld	a,#12
 798  0190 cd00dd        	call	_PlaySound
 801  0193 200a          	jra	L542
 802  0195               L142:
 803                     ; 145 			PlaySound(13);
 805  0195 a60d          	ld	a,#13
 806  0197 cd00dd        	call	_PlaySound
 808                     ; 146 			PlaySound(14);// ‰“∫ÕÍ±œ
 810  019a a60e          	ld	a,#14
 811  019c cd00dd        	call	_PlaySound
 813  019f               L542:
 814                     ; 162 }
 817  019f 85            	popw	x
 818  01a0 81            	ret
 819  01a1               L512:
 820                     ; 151 		if (dat>=10) //???°•°¿°ßo?
 822  01a1 7b01          	ld	a,(OFST+1,sp)
 823  01a3 a10a          	cp	a,#10
 824  01a5 2525          	jrult	L742
 825                     ; 153 			if (dat>19) PlaySound(dat/10);
 827  01a7 7b01          	ld	a,(OFST+1,sp)
 828  01a9 a114          	cp	a,#20
 829  01ab 250d          	jrult	L152
 832  01ad 7b01          	ld	a,(OFST+1,sp)
 833  01af ae000a        	ldw	x,#10
 834  01b2 51            	exgw	x,y
 835  01b3 5f            	clrw	x
 836  01b4 97            	ld	xl,a
 837  01b5 65            	divw	x,y
 838  01b6 9f            	ld	a,xl
 839  01b7 cd00dd        	call	_PlaySound
 841  01ba               L152:
 842                     ; 154 			PlaySound(10);
 844  01ba a60a          	ld	a,#10
 845  01bc cd00dd        	call	_PlaySound
 847                     ; 155 			dat %= 10;
 849  01bf 7b01          	ld	a,(OFST+1,sp)
 850  01c1 ae000a        	ldw	x,#10
 851  01c4 51            	exgw	x,y
 852  01c5 5f            	clrw	x
 853  01c6 97            	ld	xl,a
 854  01c7 65            	divw	x,y
 855  01c8 909f          	ld	a,yl
 856  01ca 6b01          	ld	(OFST+1,sp),a
 857  01cc               L742:
 858                     ; 157 		if (dat>0) PlaySound(dat);
 860  01cc 0d01          	tnz	(OFST+1,sp)
 861  01ce 2705          	jreq	L352
 864  01d0 7b01          	ld	a,(OFST+1,sp)
 865  01d2 cd00dd        	call	_PlaySound
 867  01d5               L352:
 868                     ; 158 		PlaySound(17); //∑ø
 870  01d5 a611          	ld	a,#17
 871  01d7 cd00dd        	call	_PlaySound
 873                     ; 159 		PlaySound(18);//Œ¿…˙º‰
 875  01da a612          	ld	a,#18
 876  01dc cd00dd        	call	_PlaySound
 878                     ; 160 		PlaySound(12);//∫ÙΩ–
 880  01df a60c          	ld	a,#12
 881  01e1 cd00dd        	call	_PlaySound
 883  01e4 20b9          	jra	L542
 966                     	xdef	_StartPlay
 967                     	xdef	_byVoiceDataAddr
 968                     	xdef	_play_end
 969                     	xdef	_play_start
 970                     	switch	.ubsct
 971  0000               _play_buf:
 972  0000 000000000000  	ds.b	30
 973                     	xdef	_play_buf
 974                     	xdef	_audio_play
 975                     	xref.b	_audio_irq
 976                     	xref.b	_voml
 977                     	xdef	_StopPlay
 978                     	xdef	_CheckPlay
 979                     	xdef	_PlaySound
 980                     	xdef	_WTVxxxInit
 981                     	xdef	_PlaySpeak
 982                     	xref	_SPITransferByte
 983                     	xdef	_Delayus
 984                     	xdef	_Delayms
 985                     	xref	_SPI_Cmd
1005                     	xref	c_smodx
1006                     	end
