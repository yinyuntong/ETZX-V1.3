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
  84                     ; 55 void Delayms(uint16 ms)	  
  84                     ; 56 {
  86                     	switch	.text
  87  0000               _Delayms:
  89  0000 89            	pushw	x
  90  0001 89            	pushw	x
  91       00000002      OFST:	set	2
  94  0002 2010          	jra	L53
  95  0004               L33:
  96                     ; 59 		for(i=300;i>0;i--);
  98  0004 ae012c        	ldw	x,#300
  99  0007 1f01          	ldw	(OFST-1,sp),x
 100  0009               L14:
 104  0009 1e01          	ldw	x,(OFST-1,sp)
 105  000b 1d0001        	subw	x,#1
 106  000e 1f01          	ldw	(OFST-1,sp),x
 109  0010 1e01          	ldw	x,(OFST-1,sp)
 110  0012 26f5          	jrne	L14
 111  0014               L53:
 112                     ; 58    while(ms--)
 114  0014 1e03          	ldw	x,(OFST+1,sp)
 115  0016 1d0001        	subw	x,#1
 116  0019 1f03          	ldw	(OFST+1,sp),x
 117  001b 1c0001        	addw	x,#1
 118  001e a30000        	cpw	x,#0
 119  0021 26e1          	jrne	L33
 120                     ; 60 }
 123  0023 5b04          	addw	sp,#4
 124  0025 81            	ret
 161                     ; 62 void Delayus(uint16 us)	  
 161                     ; 63 {
 162                     	switch	.text
 163  0026               _Delayus:
 165  0026 89            	pushw	x
 166       00000000      OFST:	set	0
 169  0027 2003          	jra	L76
 170  0029               L56:
 171                     ; 64  while(us--){nop();nop();nop();};
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
 198                     ; 65 }
 202  003b 85            	popw	x
 203  003c 81            	ret
 238                     ; 67 void WTVxxxInit(unsigned char voml)
 238                     ; 68 {		
 239                     	switch	.text
 240  003d               _WTVxxxInit:
 244                     ; 70 	SetAUDRST(0);
 246  003d 7219500a      	bres	20490,#4
 247                     ; 71 	Delayms(5); 		
 250  0041 ae0005        	ldw	x,#5
 251  0044 adba          	call	_Delayms
 253                     ; 72 	SetAUDRST(1);
 255  0046 7218500a      	bset	20490,#4
 256                     ; 73 	Delayms(10);
 259  004a ae000a        	ldw	x,#10
 260  004d adb1          	call	_Delayms
 262                     ; 74 }
 265  004f 81            	ret
 294                     ; 75 void StopPlay(void)
 294                     ; 76 {
 295                     	switch	.text
 296  0050               _StopPlay:
 300                     ; 77 	SPI_Cmd(DISABLE);
 302  0050 4f            	clr	a
 303  0051 cd0000        	call	_SPI_Cmd
 305                     ; 78 	Delayms(2);
 307  0054 ae0002        	ldw	x,#2
 308  0057 ada7          	call	_Delayms
 310                     ; 79 	SetSCK(1);
 312  0059 721a500a      	bset	20490,#5
 313                     ; 80   SetAUDCS(0);
 316  005d 7217500a      	bres	20490,#3
 317                     ; 81 	Delayms(2);
 320  0061 ae0002        	ldw	x,#2
 321  0064 ad9a          	call	_Delayms
 323                     ; 82 	SPITransferByte(CMD_CLOSE_PA);
 325  0066 a6f0          	ld	a,#240
 326  0068 cd0000        	call	_SPITransferByte
 328                     ; 83 	SetAUDCS(1);
 330  006b 7216500a      	bset	20490,#3
 331                     ; 84 	SPI_Cmd(ENABLE);
 334  006f a601          	ld	a,#1
 335  0071 cd0000        	call	_SPI_Cmd
 337                     ; 85 	play_end=play_start;
 339  0074 450102        	mov	_play_end,_play_start
 340                     ; 86 	audio_play=0;
 342  0077 3f00          	clr	_audio_play
 343                     ; 87 }
 346  0079 81            	ret
 377                     ; 91 void StartPlay(void)
 377                     ; 92 {   
 378                     	switch	.text
 379  007a               _StartPlay:
 383                     ; 93 	if (audio_play) return;
 385  007a 3d00          	tnz	_audio_play
 386  007c 2701          	jreq	L131
 390  007e 81            	ret
 391  007f               L131:
 392                     ; 94 	SPI_Cmd(DISABLE);
 394  007f 4f            	clr	a
 395  0080 cd0000        	call	_SPI_Cmd
 397                     ; 95 	Delayms(1);
 399  0083 ae0001        	ldw	x,#1
 400  0086 cd0000        	call	_Delayms
 402                     ; 96 	SetSCK(1);
 404  0089 721a500a      	bset	20490,#5
 405                     ; 97   SetAUDCS(0);
 408  008d 7217500a      	bres	20490,#3
 409                     ; 98 	Delayms(1);
 412  0091 ae0001        	ldw	x,#1
 413  0094 cd0000        	call	_Delayms
 415                     ; 99 	SPITransferByte(CMD_OPEN_PA);
 417  0097 a6f1          	ld	a,#241
 418  0099 cd0000        	call	_SPITransferByte
 420                     ; 100 	SPITransferByte(CMD_VOL_SET+voml); 
 422  009c b600          	ld	a,_voml
 423  009e abe0          	add	a,#224
 424  00a0 cd0000        	call	_SPITransferByte
 426                     ; 101 	SPITransferByte(CMD_PLAY+play_buf[play_start]);
 428  00a3 b601          	ld	a,_play_start
 429  00a5 5f            	clrw	x
 430  00a6 97            	ld	xl,a
 431  00a7 e600          	ld	a,(_play_buf,x)
 432  00a9 cd0000        	call	_SPITransferByte
 434                     ; 102 	play_start = (play_start+1)%30;
 436  00ac b601          	ld	a,_play_start
 437  00ae 5f            	clrw	x
 438  00af 97            	ld	xl,a
 439  00b0 5c            	incw	x
 440  00b1 a61e          	ld	a,#30
 441  00b3 cd0000        	call	c_smodx
 443  00b6 9f            	ld	a,xl
 444  00b7 b701          	ld	_play_start,a
 445                     ; 104 	SetAUDCS(1);
 447  00b9 7216500a      	bset	20490,#3
 448                     ; 105 	SPI_Cmd(ENABLE);
 451  00bd a601          	ld	a,#1
 452  00bf cd0000        	call	_SPI_Cmd
 454                     ; 106 	Delayms(1);
 456  00c2 ae0001        	ldw	x,#1
 457  00c5 cd0000        	call	_Delayms
 459                     ; 107 	audio_irq=0;
 461  00c8 3f00          	clr	_audio_irq
 462                     ; 108 	audio_play=1;
 464  00ca 35010000      	mov	_audio_play,#1
 465                     ; 109 }
 468  00ce 81            	ret
 495                     ; 111 void CheckPlay(void)
 495                     ; 112 {
 496                     	switch	.text
 497  00cf               _CheckPlay:
 501                     ; 114 	if (play_end!=play_start) StartPlay();
 503  00cf b602          	ld	a,_play_end
 504  00d1 b101          	cp	a,_play_start
 505  00d3 2704          	jreq	L341
 508  00d5 ada3          	call	_StartPlay
 511  00d7 2003          	jra	L541
 512  00d9               L341:
 513                     ; 115 	else StopPlay();	
 515  00d9 cd0050        	call	_StopPlay
 517  00dc               L541:
 518                     ; 116 }
 521  00dc 81            	ret
 559                     ; 118 void PlaySound(INT8U pos)
 559                     ; 119 {
 560                     	switch	.text
 561  00dd               _PlaySound:
 563  00dd 88            	push	a
 564       00000000      OFST:	set	0
 567                     ; 120 	play_buf[play_end] = pos;
 569  00de b602          	ld	a,_play_end
 570  00e0 5f            	clrw	x
 571  00e1 97            	ld	xl,a
 572  00e2 7b01          	ld	a,(OFST+1,sp)
 573  00e4 e700          	ld	(_play_buf,x),a
 574                     ; 121 	if (play_start==play_end) StartPlay();
 576  00e6 b601          	ld	a,_play_start
 577  00e8 b102          	cp	a,_play_end
 578  00ea 2602          	jrne	L561
 581  00ec ad8c          	call	_StartPlay
 583  00ee               L561:
 584                     ; 122 	play_end = (play_end+1)%30;
 586  00ee b602          	ld	a,_play_end
 587  00f0 5f            	clrw	x
 588  00f1 97            	ld	xl,a
 589  00f2 5c            	incw	x
 590  00f3 a61e          	ld	a,#30
 591  00f5 cd0000        	call	c_smodx
 593  00f8 9f            	ld	a,xl
 594  00f9 b702          	ld	_play_end,a
 595                     ; 123 }
 598  00fb 84            	pop	a
 599  00fc 81            	ret
 652                     ; 171 void PlaySpeak(uint8 dat,uint8 dat1,uint8 dat2)
 652                     ; 172 { 
 653                     	switch	.text
 654  00fd               _PlaySpeak:
 656  00fd 89            	pushw	x
 657       00000000      OFST:	set	0
 660                     ; 173 	if((dat2==BED_CALL_TYPE) ||(dat2==BED_INFUSION_TYPE))//床头呼叫或输液完毕,都报"xx床呼叫"
 662  00fe 7b05          	ld	a,(OFST+5,sp)
 663  0100 a106          	cp	a,#6
 664  0102 2706          	jreq	L142
 666  0104 7b05          	ld	a,(OFST+5,sp)
 667  0106 a107          	cp	a,#7
 668  0108 264d          	jrne	L732
 669  010a               L142:
 670                     ; 175 		if(dat == ADD_FJ) PlaySound(EXTRA_VOC_ADDR);	//加床
 672  010a 7b01          	ld	a,(OFST+1,sp)
 673  010c a1fa          	cp	a,#250
 674  010e 2604          	jrne	L342
 677  0110 a626          	ld	a,#38
 678  0112 adc9          	call	_PlaySound
 680  0114               L342:
 681                     ; 177 		PlaySound(BED_VOC_ADDR);
 683  0114 a621          	ld	a,#33
 684  0116 adc5          	call	_PlaySound
 686                     ; 178 		PlaySound(EXTENSION_VOC_ADDR);
 688  0118 a625          	ld	a,#37
 689  011a adc1          	call	_PlaySound
 691                     ; 179 		PlaySound(NUMBER_VOC_ADDR);
 693  011c a622          	ld	a,#34
 694  011e adbd          	call	_PlaySound
 697  0120               L542:
 698                     ; 192 	if(dat1>=100)
 700  0120 7b02          	ld	a,(OFST+2,sp)
 701  0122 a164          	cp	a,#100
 702  0124 254e          	jrult	L352
 703                     ; 194 		PlaySound((dat1/100)/*-1*/);
 705  0126 7b02          	ld	a,(OFST+2,sp)
 706  0128 ae0064        	ldw	x,#100
 707  012b 51            	exgw	x,y
 708  012c 5f            	clrw	x
 709  012d 97            	ld	xl,a
 710  012e 65            	divw	x,y
 711  012f 9f            	ld	a,xl
 712  0130 adab          	call	_PlaySound
 714                     ; 195 		PlaySound(HUNDRED_VOC_ADDR);
 716  0132 a61c          	ld	a,#28
 717  0134 ada7          	call	_PlaySound
 719                     ; 196 		if(dat1%100 )
 721  0136 7b02          	ld	a,(OFST+2,sp)
 722  0138 5f            	clrw	x
 723  0139 97            	ld	xl,a
 724  013a a664          	ld	a,#100
 725  013c cd0000        	call	c_smodx
 727  013f a30000        	cpw	x,#0
 728  0142 2730          	jreq	L352
 729                     ; 198 			PlaySound(AND_VOC_ADDR);
 731  0144 a61d          	ld	a,#29
 732  0146 ad95          	call	_PlaySound
 734                     ; 199 			dat1%=100;
 736  0148 7b02          	ld	a,(OFST+2,sp)
 737  014a ae0064        	ldw	x,#100
 738  014d 51            	exgw	x,y
 739  014e 5f            	clrw	x
 740  014f 97            	ld	xl,a
 741  0150 65            	divw	x,y
 742  0151 909f          	ld	a,yl
 743  0153 6b02          	ld	(OFST+2,sp),a
 744  0155 201d          	jra	L352
 745  0157               L732:
 746                     ; 182 	else if(dat2==WC_CALL_TYPE)
 748  0157 7b05          	ld	a,(OFST+5,sp)
 749  0159 a108          	cp	a,#8
 750  015b 2615          	jrne	L742
 751                     ; 184 		PlaySound(BATHROOM_VOC_ADDR);
 753  015d a624          	ld	a,#36
 754  015f cd00dd        	call	_PlaySound
 756                     ; 185 		PlaySound(EXTENSION_VOC_ADDR);
 758  0162 a625          	ld	a,#37
 759  0164 cd00dd        	call	_PlaySound
 761                     ; 186 		PlaySound(NUMBER_VOC_ADDR);
 763  0167 a622          	ld	a,#34
 764  0169 cd00dd        	call	_PlaySound
 766                     ; 188 		dat1 = dat;		//卫生间数据在房号	
 768  016c 7b01          	ld	a,(OFST+1,sp)
 769  016e 6b02          	ld	(OFST+2,sp),a
 771  0170 20ae          	jra	L542
 772  0172               L742:
 773                     ; 190 	else return;
 776  0172 85            	popw	x
 777  0173 81            	ret
 778  0174               L352:
 779                     ; 205 	if(dat1>=20)
 781  0174 7b02          	ld	a,(OFST+2,sp)
 782  0176 a114          	cp	a,#20
 783  0178 257c          	jrult	L752
 784                     ; 207 		switch(dat1/10)
 786  017a 7b02          	ld	a,(OFST+2,sp)
 787  017c 5f            	clrw	x
 788  017d 97            	ld	xl,a
 789  017e a60a          	ld	a,#10
 790  0180 cd0000        	call	c_sdivx
 793                     ; 233 			default:
 793                     ; 234 				break;
 794  0183 1d0002        	subw	x,#2
 795  0186 2717          	jreq	L761
 796  0188 5a            	decw	x
 797  0189 271b          	jreq	L171
 798  018b 5a            	decw	x
 799  018c 271f          	jreq	L371
 800  018e 5a            	decw	x
 801  018f 2723          	jreq	L571
 802  0191 5a            	decw	x
 803  0192 2727          	jreq	L771
 804  0194 5a            	decw	x
 805  0195 272b          	jreq	L102
 806  0197 5a            	decw	x
 807  0198 272f          	jreq	L302
 808  019a 5a            	decw	x
 809  019b 2733          	jreq	L502
 810  019d 2036          	jra	L362
 811  019f               L761:
 812                     ; 209 			case 2:
 812                     ; 210 				PlaySound(TWENTY_VOC_ADDR);
 814  019f a614          	ld	a,#20
 815  01a1 cd00dd        	call	_PlaySound
 817                     ; 211 				break;
 819  01a4 202f          	jra	L362
 820  01a6               L171:
 821                     ; 212 			case 3:
 821                     ; 213 				PlaySound(THIRTY_VOC_ADDR);
 823  01a6 a615          	ld	a,#21
 824  01a8 cd00dd        	call	_PlaySound
 826                     ; 214 				break;	
 828  01ab 2028          	jra	L362
 829  01ad               L371:
 830                     ; 215 			case 4:
 830                     ; 216 				PlaySound(FOURTY_VOC_ADDR);
 832  01ad a616          	ld	a,#22
 833  01af cd00dd        	call	_PlaySound
 835                     ; 217 				break;	
 837  01b2 2021          	jra	L362
 838  01b4               L571:
 839                     ; 218 			case 5:
 839                     ; 219 				PlaySound(FIFTY_VOC_ADDR);
 841  01b4 a617          	ld	a,#23
 842  01b6 cd00dd        	call	_PlaySound
 844                     ; 220 				break;
 846  01b9 201a          	jra	L362
 847  01bb               L771:
 848                     ; 221 			case 6:
 848                     ; 222 				PlaySound(SIXTY_VOC_ADDR);
 850  01bb a618          	ld	a,#24
 851  01bd cd00dd        	call	_PlaySound
 853                     ; 223 				break;	
 855  01c0 2013          	jra	L362
 856  01c2               L102:
 857                     ; 224 			case 7:
 857                     ; 225 				PlaySound(SEVENTY_VOC_ADDR);
 859  01c2 a619          	ld	a,#25
 860  01c4 cd00dd        	call	_PlaySound
 862                     ; 226 				break;	
 864  01c7 200c          	jra	L362
 865  01c9               L302:
 866                     ; 227 			case 8:
 866                     ; 228 				PlaySound(EIGHTY_VOC_ADDR);
 868  01c9 a61a          	ld	a,#26
 869  01cb cd00dd        	call	_PlaySound
 871                     ; 229 				break;	
 873  01ce 2005          	jra	L362
 874  01d0               L502:
 875                     ; 230 			case 9:
 875                     ; 231 				PlaySound(NINETY_VOC_ADDR);
 877  01d0 a61b          	ld	a,#27
 878  01d2 cd00dd        	call	_PlaySound
 880                     ; 232 				break;
 882  01d5               L702:
 883                     ; 233 			default:
 883                     ; 234 				break;
 885  01d5               L362:
 886                     ; 237 		dat1 %=10;
 888  01d5 7b02          	ld	a,(OFST+2,sp)
 889  01d7 ae000a        	ldw	x,#10
 890  01da 51            	exgw	x,y
 891  01db 5f            	clrw	x
 892  01dc 97            	ld	xl,a
 893  01dd 65            	divw	x,y
 894  01de 909f          	ld	a,yl
 895  01e0 6b02          	ld	(OFST+2,sp),a
 896                     ; 238 		if(dat1!=0)
 898  01e2 0d02          	tnz	(OFST+2,sp)
 899  01e4 2719          	jreq	L762
 900                     ; 240 			PlaySound((dat1%10)/*-1*/);
 902  01e6 7b02          	ld	a,(OFST+2,sp)
 903  01e8 ae000a        	ldw	x,#10
 904  01eb 51            	exgw	x,y
 905  01ec 5f            	clrw	x
 906  01ed 97            	ld	xl,a
 907  01ee 65            	divw	x,y
 908  01ef 909f          	ld	a,yl
 909  01f1 cd00dd        	call	_PlaySound
 911  01f4 2009          	jra	L762
 912  01f6               L752:
 913                     ; 246 		if(dat1!=0)	PlaySound(dat1/*-1*/);
 915  01f6 0d02          	tnz	(OFST+2,sp)
 916  01f8 2705          	jreq	L762
 919  01fa 7b02          	ld	a,(OFST+2,sp)
 920  01fc cd00dd        	call	_PlaySound
 922  01ff               L762:
 923                     ; 250 	PlaySound(CALLING_VOC_ADDR);
 925  01ff a61f          	ld	a,#31
 926  0201 cd00dd        	call	_PlaySound
 928                     ; 253 }
 931  0204 85            	popw	x
 932  0205 81            	ret
 985                     ; 259 void playtest(INT8U start_seg,INT8U end_seg)
 985                     ; 260 {
 986                     	switch	.text
 987  0206               _playtest:
 989  0206 89            	pushw	x
 990  0207 88            	push	a
 991       00000001      OFST:	set	1
 994                     ; 262 	for(i=start_seg;i<=end_seg;i++)
 996  0208 9e            	ld	a,xh
 997  0209 6b01          	ld	(OFST+0,sp),a
 999  020b 2007          	jra	L523
1000  020d               L123:
1001                     ; 264 		PlaySound(i);
1003  020d 7b01          	ld	a,(OFST+0,sp)
1004  020f cd00dd        	call	_PlaySound
1006                     ; 262 	for(i=start_seg;i<=end_seg;i++)
1008  0212 0c01          	inc	(OFST+0,sp)
1009  0214               L523:
1012  0214 7b01          	ld	a,(OFST+0,sp)
1013  0216 1103          	cp	a,(OFST+2,sp)
1014  0218 23f3          	jrule	L123
1015                     ; 266 }
1018  021a 5b03          	addw	sp,#3
1019  021c 81            	ret
1102                     	xdef	_StartPlay
1103                     	xdef	_byVoiceDataAddr
1104                     	xdef	_play_end
1105                     	xdef	_play_start
1106                     	switch	.ubsct
1107  0000               _play_buf:
1108  0000 000000000000  	ds.b	30
1109                     	xdef	_play_buf
1110                     	xdef	_audio_play
1111                     	xref.b	_audio_irq
1112                     	xref.b	_voml
1113                     	xdef	_playtest
1114                     	xdef	_StopPlay
1115                     	xdef	_CheckPlay
1116                     	xdef	_PlaySound
1117                     	xdef	_WTVxxxInit
1118                     	xdef	_PlaySpeak
1119                     	xref	_SPITransferByte
1120                     	xdef	_Delayus
1121                     	xdef	_Delayms
1122                     	xref	_SPI_Cmd
1142                     	xref	c_sdivx
1143                     	xref	c_smodx
1144                     	end
