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
  84                     ; 52 void Delayms(uint16 ms)	  
  84                     ; 53 {
  86                     .text:	section	.text,new
  87  0000               _Delayms:
  89  0000 89            	pushw	x
  90  0001 89            	pushw	x
  91       00000002      OFST:	set	2
  94  0002 2010          	jra	L53
  95  0004               L33:
  96                     ; 56 		for(i=300;i>0;i--);
  98  0004 ae012c        	ldw	x,#300
  99  0007 1f01          	ldw	(OFST-1,sp),x
 100  0009               L14:
 104  0009 1e01          	ldw	x,(OFST-1,sp)
 105  000b 1d0001        	subw	x,#1
 106  000e 1f01          	ldw	(OFST-1,sp),x
 109  0010 1e01          	ldw	x,(OFST-1,sp)
 110  0012 26f5          	jrne	L14
 111  0014               L53:
 112                     ; 55    while(ms--)
 114  0014 1e03          	ldw	x,(OFST+1,sp)
 115  0016 1d0001        	subw	x,#1
 116  0019 1f03          	ldw	(OFST+1,sp),x
 117  001b 1c0001        	addw	x,#1
 118  001e a30000        	cpw	x,#0
 119  0021 26e1          	jrne	L33
 120                     ; 57 }
 123  0023 5b04          	addw	sp,#4
 124  0025 81            	ret
 161                     ; 59 void Delayus(uint16 us)	  
 161                     ; 60 {
 162                     .text:	section	.text,new
 163  0000               _Delayus:
 165  0000 89            	pushw	x
 166       00000000      OFST:	set	0
 169  0001 2003          	jra	L76
 170  0003               L56:
 171                     ; 61  while(us--){nop();nop();nop();};
 174  0003 9d            nop
 180  0004 9d            nop
 186  0005 9d            nop
 189  0006               L76:
 192  0006 1e01          	ldw	x,(OFST+1,sp)
 193  0008 1d0001        	subw	x,#1
 194  000b 1f01          	ldw	(OFST+1,sp),x
 195  000d 1c0001        	addw	x,#1
 196  0010 a30000        	cpw	x,#0
 197  0013 26ee          	jrne	L56
 198                     ; 62 }
 202  0015 85            	popw	x
 203  0016 81            	ret
 238                     ; 64 void WTVxxxInit(unsigned char voml)
 238                     ; 65 {		
 239                     .text:	section	.text,new
 240  0000               _WTVxxxInit:
 244                     ; 67 	SetAUDRST(0);
 246  0000 7219500a      	bres	20490,#4
 247                     ; 68 	Delayms(5); 		
 250  0004 ae0005        	ldw	x,#5
 251  0007 cd0000        	call	_Delayms
 253                     ; 69 	SetAUDRST(1);
 255  000a 7218500a      	bset	20490,#4
 256                     ; 70 	Delayms(10);
 259  000e ae000a        	ldw	x,#10
 260  0011 cd0000        	call	_Delayms
 262                     ; 71 }
 265  0014 81            	ret
 294                     ; 72 void StopPlay(void)
 294                     ; 73 {
 295                     .text:	section	.text,new
 296  0000               _StopPlay:
 300                     ; 74 	SPI_Cmd(DISABLE);
 302  0000 4f            	clr	a
 303  0001 cd0000        	call	_SPI_Cmd
 305                     ; 75 	Delayms(2);
 307  0004 ae0002        	ldw	x,#2
 308  0007 cd0000        	call	_Delayms
 310                     ; 76 	SetSCK(1);
 312  000a 721a500a      	bset	20490,#5
 313                     ; 77   SetAUDCS(0);
 316  000e 7217500a      	bres	20490,#3
 317                     ; 78 	Delayms(2);
 320  0012 ae0002        	ldw	x,#2
 321  0015 cd0000        	call	_Delayms
 323                     ; 79 	SPITransferByte(CMD_CLOSE_PA);
 325  0018 a6f0          	ld	a,#240
 326  001a cd0000        	call	_SPITransferByte
 328                     ; 80 	SetAUDCS(1);
 330  001d 7216500a      	bset	20490,#3
 331                     ; 81 	SPI_Cmd(ENABLE);
 334  0021 a601          	ld	a,#1
 335  0023 cd0000        	call	_SPI_Cmd
 337                     ; 82 	play_end=play_start;
 339  0026 450102        	mov	_play_end,_play_start
 340                     ; 83 	audio_play=0;
 342  0029 3f00          	clr	_audio_play
 343                     ; 84 }
 346  002b 81            	ret
 377                     ; 88 void StartPlay(void)
 377                     ; 89 {   
 378                     .text:	section	.text,new
 379  0000               _StartPlay:
 383                     ; 90 	if (audio_play) return;
 385  0000 3d00          	tnz	_audio_play
 386  0002 2701          	jreq	L131
 390  0004 81            	ret
 391  0005               L131:
 392                     ; 91 	SPI_Cmd(DISABLE);
 394  0005 4f            	clr	a
 395  0006 cd0000        	call	_SPI_Cmd
 397                     ; 92 	Delayms(1);
 399  0009 ae0001        	ldw	x,#1
 400  000c cd0000        	call	_Delayms
 402                     ; 93 	SetSCK(1);
 404  000f 721a500a      	bset	20490,#5
 405                     ; 94   SetAUDCS(0);
 408  0013 7217500a      	bres	20490,#3
 409                     ; 95 	Delayms(1);
 412  0017 ae0001        	ldw	x,#1
 413  001a cd0000        	call	_Delayms
 415                     ; 96 	SPITransferByte(CMD_OPEN_PA);
 417  001d a6f1          	ld	a,#241
 418  001f cd0000        	call	_SPITransferByte
 420                     ; 97 	SPITransferByte(CMD_VOL_SET+stEepromCfgData.byVol); 
 422  0022 b605          	ld	a,_stEepromCfgData+5
 423  0024 abe0          	add	a,#224
 424  0026 cd0000        	call	_SPITransferByte
 426                     ; 98 	SPITransferByte(CMD_PLAY+play_buf[play_start]);
 428  0029 b601          	ld	a,_play_start
 429  002b 5f            	clrw	x
 430  002c 97            	ld	xl,a
 431  002d e600          	ld	a,(_play_buf,x)
 432  002f cd0000        	call	_SPITransferByte
 434                     ; 99 	play_start = (play_start+1)%30;
 436  0032 b601          	ld	a,_play_start
 437  0034 5f            	clrw	x
 438  0035 97            	ld	xl,a
 439  0036 5c            	incw	x
 440  0037 a61e          	ld	a,#30
 441  0039 cd0000        	call	c_smodx
 443  003c 9f            	ld	a,xl
 444  003d b701          	ld	_play_start,a
 445                     ; 101 	SetAUDCS(1);
 447  003f 7216500a      	bset	20490,#3
 448                     ; 102 	SPI_Cmd(ENABLE);
 451  0043 a601          	ld	a,#1
 452  0045 cd0000        	call	_SPI_Cmd
 454                     ; 103 	Delayms(1);
 456  0048 ae0001        	ldw	x,#1
 457  004b cd0000        	call	_Delayms
 459                     ; 104 	audio_irq=0;
 461  004e 3f00          	clr	_audio_irq
 462                     ; 105 	audio_play=1;
 464  0050 35010000      	mov	_audio_play,#1
 465                     ; 106 }
 468  0054 81            	ret
 495                     ; 108 void CheckPlay(void)
 495                     ; 109 {
 496                     .text:	section	.text,new
 497  0000               _CheckPlay:
 501                     ; 111 	if (play_end!=play_start) StartPlay();
 503  0000 b602          	ld	a,_play_end
 504  0002 b101          	cp	a,_play_start
 505  0004 2705          	jreq	L341
 508  0006 cd0000        	call	_StartPlay
 511  0009 2003          	jra	L541
 512  000b               L341:
 513                     ; 112 	else StopPlay();	
 515  000b cd0000        	call	_StopPlay
 517  000e               L541:
 518                     ; 113 }
 521  000e 81            	ret
 559                     ; 115 void PlaySound(INT8U pos)
 559                     ; 116 {
 560                     .text:	section	.text,new
 561  0000               _PlaySound:
 563  0000 88            	push	a
 564       00000000      OFST:	set	0
 567                     ; 117 	play_buf[play_end] = pos;
 569  0001 b602          	ld	a,_play_end
 570  0003 5f            	clrw	x
 571  0004 97            	ld	xl,a
 572  0005 7b01          	ld	a,(OFST+1,sp)
 573  0007 e700          	ld	(_play_buf,x),a
 574                     ; 118 	if (play_start==play_end) StartPlay();
 576  0009 b601          	ld	a,_play_start
 577  000b b102          	cp	a,_play_end
 578  000d 2603          	jrne	L561
 581  000f cd0000        	call	_StartPlay
 583  0012               L561:
 584                     ; 119 	play_end = (play_end+1)%30;
 586  0012 b602          	ld	a,_play_end
 587  0014 5f            	clrw	x
 588  0015 97            	ld	xl,a
 589  0016 5c            	incw	x
 590  0017 a61e          	ld	a,#30
 591  0019 cd0000        	call	c_smodx
 593  001c 9f            	ld	a,xl
 594  001d b702          	ld	_play_end,a
 595                     ; 120 }
 598  001f 84            	pop	a
 599  0020 81            	ret
 652                     ; 122 void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
 652                     ; 123 { 
 653                     .text:	section	.text,new
 654  0000               _PlaySpeak:
 656  0000 89            	pushw	x
 657       00000000      OFST:	set	0
 660                     ; 124 	if((dat2==6)||(dat2==7))  //¥≤Õ∑∫ÙΩ–ªÚ ‰“∫ÕÍ±œ
 662  0001 7b05          	ld	a,(OFST+5,sp)
 663  0003 a106          	cp	a,#6
 664  0005 2709          	jreq	L712
 666  0007 7b05          	ld	a,(OFST+5,sp)
 667  0009 a107          	cp	a,#7
 668  000b 2703          	jreq	L42
 669  000d cc00a8        	jp	L512
 670  0010               L42:
 671  0010               L712:
 672                     ; 126 		if (dat==250) PlaySound(21);//º”¥≤
 674  0010 7b01          	ld	a,(OFST+1,sp)
 675  0012 a1fa          	cp	a,#250
 676  0014 2605          	jrne	L122
 679  0016 a615          	ld	a,#21
 680  0018 cd0000        	call	_PlaySound
 682  001b               L122:
 683                     ; 127 		dat = dat1;
 685  001b 7b02          	ld	a,(OFST+2,sp)
 686  001d 6b01          	ld	(OFST+1,sp),a
 687                     ; 128 		if (dat1>=100)   //???°•°¿°ßo?
 689  001f 7b02          	ld	a,(OFST+2,sp)
 690  0021 a164          	cp	a,#100
 691  0023 251f          	jrult	L322
 692                     ; 130 			PlaySound(dat1/100);
 694  0025 7b02          	ld	a,(OFST+2,sp)
 695  0027 ae0064        	ldw	x,#100
 696  002a 51            	exgw	x,y
 697  002b 5f            	clrw	x
 698  002c 97            	ld	xl,a
 699  002d 65            	divw	x,y
 700  002e 9f            	ld	a,xl
 701  002f cd0000        	call	_PlaySound
 703                     ; 131 			PlaySound(11);
 705  0032 a60b          	ld	a,#11
 706  0034 cd0000        	call	_PlaySound
 708                     ; 132 			dat1 %= 100;
 710  0037 7b02          	ld	a,(OFST+2,sp)
 711  0039 ae0064        	ldw	x,#100
 712  003c 51            	exgw	x,y
 713  003d 5f            	clrw	x
 714  003e 97            	ld	xl,a
 715  003f 65            	divw	x,y
 716  0040 909f          	ld	a,yl
 717  0042 6b02          	ld	(OFST+2,sp),a
 718  0044               L322:
 719                     ; 134 		if (dat1>=10)
 721  0044 7b02          	ld	a,(OFST+2,sp)
 722  0046 a10a          	cp	a,#10
 723  0048 252d          	jrult	L522
 724                     ; 136 			if (dat1>19||dat>100) PlaySound(dat1/10);
 726  004a 7b02          	ld	a,(OFST+2,sp)
 727  004c a114          	cp	a,#20
 728  004e 2406          	jruge	L132
 730  0050 7b01          	ld	a,(OFST+1,sp)
 731  0052 a165          	cp	a,#101
 732  0054 250d          	jrult	L722
 733  0056               L132:
 736  0056 7b02          	ld	a,(OFST+2,sp)
 737  0058 ae000a        	ldw	x,#10
 738  005b 51            	exgw	x,y
 739  005c 5f            	clrw	x
 740  005d 97            	ld	xl,a
 741  005e 65            	divw	x,y
 742  005f 9f            	ld	a,xl
 743  0060 cd0000        	call	_PlaySound
 745  0063               L722:
 746                     ; 137 			PlaySound(10);
 748  0063 a60a          	ld	a,#10
 749  0065 cd0000        	call	_PlaySound
 751                     ; 138 			dat1 %= 10;
 753  0068 7b02          	ld	a,(OFST+2,sp)
 754  006a ae000a        	ldw	x,#10
 755  006d 51            	exgw	x,y
 756  006e 5f            	clrw	x
 757  006f 97            	ld	xl,a
 758  0070 65            	divw	x,y
 759  0071 909f          	ld	a,yl
 760  0073 6b02          	ld	(OFST+2,sp),a
 762  0075 200a          	jra	L332
 763  0077               L522:
 764                     ; 139 		} else if(dat>100) PlaySound(0);
 766  0077 7b01          	ld	a,(OFST+1,sp)
 767  0079 a165          	cp	a,#101
 768  007b 2504          	jrult	L332
 771  007d 4f            	clr	a
 772  007e cd0000        	call	_PlaySound
 774  0081               L332:
 775                     ; 140 		if (dat1>0)	PlaySound(dat1);
 777  0081 0d02          	tnz	(OFST+2,sp)
 778  0083 2705          	jreq	L732
 781  0085 7b02          	ld	a,(OFST+2,sp)
 782  0087 cd0000        	call	_PlaySound
 784  008a               L732:
 785                     ; 141 		PlaySound(15);//¥≤
 787  008a a60f          	ld	a,#15
 788  008c cd0000        	call	_PlaySound
 790                     ; 142 		if (dat2==6) PlaySound(12);//∫ÙΩ–
 792  008f 7b05          	ld	a,(OFST+5,sp)
 793  0091 a106          	cp	a,#6
 794  0093 2607          	jrne	L142
 797  0095 a60c          	ld	a,#12
 798  0097 cd0000        	call	_PlaySound
 801  009a 200a          	jra	L542
 802  009c               L142:
 803                     ; 144 			PlaySound(13);
 805  009c a60d          	ld	a,#13
 806  009e cd0000        	call	_PlaySound
 808                     ; 145 			PlaySound(14);// ‰“∫ÕÍ±œ
 810  00a1 a60e          	ld	a,#14
 811  00a3 cd0000        	call	_PlaySound
 813  00a6               L542:
 814                     ; 161 }
 817  00a6 85            	popw	x
 818  00a7 81            	ret
 819  00a8               L512:
 820                     ; 150 		if (dat>=10) //???°•°¿°ßo?
 822  00a8 7b01          	ld	a,(OFST+1,sp)
 823  00aa a10a          	cp	a,#10
 824  00ac 2525          	jrult	L742
 825                     ; 152 			if (dat>19) PlaySound(dat/10);
 827  00ae 7b01          	ld	a,(OFST+1,sp)
 828  00b0 a114          	cp	a,#20
 829  00b2 250d          	jrult	L152
 832  00b4 7b01          	ld	a,(OFST+1,sp)
 833  00b6 ae000a        	ldw	x,#10
 834  00b9 51            	exgw	x,y
 835  00ba 5f            	clrw	x
 836  00bb 97            	ld	xl,a
 837  00bc 65            	divw	x,y
 838  00bd 9f            	ld	a,xl
 839  00be cd0000        	call	_PlaySound
 841  00c1               L152:
 842                     ; 153 			PlaySound(10);
 844  00c1 a60a          	ld	a,#10
 845  00c3 cd0000        	call	_PlaySound
 847                     ; 154 			dat %= 10;
 849  00c6 7b01          	ld	a,(OFST+1,sp)
 850  00c8 ae000a        	ldw	x,#10
 851  00cb 51            	exgw	x,y
 852  00cc 5f            	clrw	x
 853  00cd 97            	ld	xl,a
 854  00ce 65            	divw	x,y
 855  00cf 909f          	ld	a,yl
 856  00d1 6b01          	ld	(OFST+1,sp),a
 857  00d3               L742:
 858                     ; 156 		if (dat>0) PlaySound(dat);
 860  00d3 0d01          	tnz	(OFST+1,sp)
 861  00d5 2705          	jreq	L352
 864  00d7 7b01          	ld	a,(OFST+1,sp)
 865  00d9 cd0000        	call	_PlaySound
 867  00dc               L352:
 868                     ; 157 		PlaySound(17); //∑ø
 870  00dc a611          	ld	a,#17
 871  00de cd0000        	call	_PlaySound
 873                     ; 158 		PlaySound(18);//Œ¿…˙º‰
 875  00e1 a612          	ld	a,#18
 876  00e3 cd0000        	call	_PlaySound
 878                     ; 159 		PlaySound(12);//∫ÙΩ–
 880  00e6 a60c          	ld	a,#12
 881  00e8 cd0000        	call	_PlaySound
 883  00eb 20b9          	jra	L542
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
 976                     	xdef	_StopPlay
 977                     	xdef	_CheckPlay
 978                     	xdef	_PlaySound
 979                     	xdef	_WTVxxxInit
 980                     	xdef	_PlaySpeak
 981                     	xref	_SPITransferByte
 982                     	xdef	_Delayus
 983                     	xdef	_Delayms
 984                     	xref.b	_stEepromCfgData
 985                     	xref	_SPI_Cmd
1005                     	xref	c_smodx
1006                     	end
