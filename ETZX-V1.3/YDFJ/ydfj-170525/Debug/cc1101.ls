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
  16  0000 00            	dc.b	0
  17  0001 08            	dc.b	8
  18  0002 00            	dc.b	0
  19  0003 10            	dc.b	16
  20  0004 a7            	dc.b	167
  21  0005 62            	dc.b	98
  22  0006 58            	dc.b	88
  23  0007 43            	dc.b	67
  24  0008 03            	dc.b	3
  25  0009 22            	dc.b	34
  26  000a f8            	dc.b	248
  27  000b 00            	dc.b	0
  28  000c 47            	dc.b	71
  29  000d 56            	dc.b	86
  30  000e 10            	dc.b	16
  31  000f 18            	dc.b	24
  32  0010 1d            	dc.b	29
  33  0011 1c            	dc.b	28
  34  0012 43            	dc.b	67
  35  0013 40            	dc.b	64
  36  0014 91            	dc.b	145
  37  0015 ea            	dc.b	234
  38  0016 2a            	dc.b	42
  39  0017 00            	dc.b	0
  40  0018 11            	dc.b	17
  41  0019 59            	dc.b	89
  42  001a 81            	dc.b	129
  43  001b 35            	dc.b	53
  44  001c 09            	dc.b	9
  45  001d 0e            	dc.b	14
  46  001e 06            	dc.b	6
  47  001f 04            	dc.b	4
  48  0020 05            	dc.b	5
  49  0021 00            	dc.b	0
  50  0022 20            	dc.b	32
  51  0023 00000000      	ds.b	4
 100                     ; 151 void delay(unsigned int s)
 100                     ; 152 {
 102                     	switch	.text
 103  0000               _delay:
 105  0000 89            	pushw	x
 106  0001 89            	pushw	x
 107       00000002      OFST:	set	2
 110                     ; 154 	for(i=0; i<s; i++);
 112  0002 5f            	clrw	x
 113  0003 1f01          	ldw	(OFST-1,sp),x
 115  0005 2007          	jra	L73
 116  0007               L33:
 120  0007 1e01          	ldw	x,(OFST-1,sp)
 121  0009 1c0001        	addw	x,#1
 122  000c 1f01          	ldw	(OFST-1,sp),x
 123  000e               L73:
 126  000e 1e01          	ldw	x,(OFST-1,sp)
 127  0010 1303          	cpw	x,(OFST+1,sp)
 128  0012 25f3          	jrult	L33
 129                     ; 155 	for(i=0; i<s; i++);
 131  0014 5f            	clrw	x
 132  0015 1f01          	ldw	(OFST-1,sp),x
 134  0017 2007          	jra	L74
 135  0019               L34:
 139  0019 1e01          	ldw	x,(OFST-1,sp)
 140  001b 1c0001        	addw	x,#1
 141  001e 1f01          	ldw	(OFST-1,sp),x
 142  0020               L74:
 145  0020 1e01          	ldw	x,(OFST-1,sp)
 146  0022 1303          	cpw	x,(OFST+1,sp)
 147  0024 25f3          	jrult	L34
 148                     ; 156 }
 151  0026 5b04          	addw	sp,#4
 152  0028 81            	ret
 188                     ; 158 void halWait(INT16U timeout) {
 189                     	switch	.text
 190  0029               _halWait:
 192  0029 89            	pushw	x
 193       00000000      OFST:	set	0
 196  002a               L17:
 197                     ; 160 		nop();
 200  002a 9d            nop
 202                     ; 161 		nop(); 
 206  002b 9d            nop
 208                     ; 162     } while (--timeout);
 210  002c 1e01          	ldw	x,(OFST+1,sp)
 211  002e 1d0001        	subw	x,#1
 212  0031 1f01          	ldw	(OFST+1,sp),x
 213  0033 26f5          	jrne	L17
 214                     ; 163 }
 217  0035 85            	popw	x
 218  0036 81            	ret
 271                     ; 165 unsigned char SPITransferByte(unsigned char byData)
 271                     ; 166 {
 272                     	switch	.text
 273  0037               _SPITransferByte:
 275  0037 88            	push	a
 276  0038 89            	pushw	x
 277       00000002      OFST:	set	2
 280                     ; 168 	uint8 RecData=0;
 282  0039 0f02          	clr	(OFST+0,sp)
 283                     ; 169 	for(i=0; i<8; i++)
 285  003b 0f01          	clr	(OFST-1,sp)
 286  003d               L521:
 287                     ; 171 		SetSCK(0);
 289  003d 721b500a      	bres	20490,#5
 290                     ; 172 		if(byData&0x01){ SetMOSI(1);}
 293  0041 7b03          	ld	a,(OFST+1,sp)
 294  0043 a501          	bcp	a,#1
 295  0045 2706          	jreq	L331
 298  0047 721c500a      	bset	20490,#6
 301  004b 2004          	jra	L531
 302  004d               L331:
 303                     ; 173 		else {SetMOSI(0);}
 305  004d 721d500a      	bres	20490,#6
 306  0051               L531:
 307                     ; 174 		Delayus(150);
 309  0051 ae0096        	ldw	x,#150
 310  0054 cd0000        	call	_Delayus
 312                     ; 175 		SetSCK(1);
 314  0057 721a500a      	bset	20490,#5
 315                     ; 176 		Delayus(150);
 318  005b ae0096        	ldw	x,#150
 319  005e cd0000        	call	_Delayus
 321                     ; 177 		if(GetMISO()!=0) RecData |=0x80;
 323  0061 c6500b        	ld	a,20491
 324  0064 a580          	bcp	a,#128
 325  0066 2708          	jreq	L731
 328  0068 7b02          	ld	a,(OFST+0,sp)
 329  006a aa80          	or	a,#128
 330  006c 6b02          	ld	(OFST+0,sp),a
 332  006e 2006          	jra	L141
 333  0070               L731:
 334                     ; 178 		else RecData &=0x7f;
 336  0070 7b02          	ld	a,(OFST+0,sp)
 337  0072 a47f          	and	a,#127
 338  0074 6b02          	ld	(OFST+0,sp),a
 339  0076               L141:
 340                     ; 179 		byData>>= 1;
 342  0076 0403          	srl	(OFST+1,sp)
 343                     ; 180 		RecData>>=1;
 345  0078 0402          	srl	(OFST+0,sp)
 346                     ; 169 	for(i=0; i<8; i++)
 348  007a 0c01          	inc	(OFST-1,sp)
 351  007c 7b01          	ld	a,(OFST-1,sp)
 352  007e a108          	cp	a,#8
 353  0080 25bb          	jrult	L521
 354                     ; 182 	return(RecData);
 356  0082 7b02          	ld	a,(OFST+0,sp)
 359  0084 5b03          	addw	sp,#3
 360  0086 81            	ret
 394                     ; 185 unsigned char SPI_SendByte(unsigned char Data)
 394                     ; 186 {
 395                     	switch	.text
 396  0087               _SPI_SendByte:
 400                     ; 187 	SPI->DR = Data;
 402  0087 c75204        	ld	20996,a
 404  008a               L561:
 405                     ; 188 	while(SPI->SR&SPI_SR_BSY);
 407  008a c65203        	ld	a,20995
 408  008d a580          	bcp	a,#128
 409  008f 26f9          	jrne	L561
 410                     ; 189 	return SPI->DR;
 412  0091 c65204        	ld	a,20996
 415  0094 81            	ret
 440                     ; 194 void Init_WOR(void)
 440                     ; 195 {
 441                     	switch	.text
 442  0095               _Init_WOR:
 446                     ; 196 	halSpiStrobe(CCxxx0_SIDLE);
 448  0095 a636          	ld	a,#54
 449  0097 cd018d        	call	_halSpiStrobe
 451                     ; 197 	halSpiWriteReg(CCxxx0_WORCTRL, 0x78); // 
 453  009a ae0078        	ldw	x,#120
 454  009d a620          	ld	a,#32
 455  009f 95            	ld	xh,a
 456  00a0 cd013a        	call	_halSpiWriteReg
 458                     ; 198 	halSpiWriteReg(CCxxx0_WOREVT1, 0x02);//43B5 500ms, 28A0 300ms 1B15 200ms
 460  00a3 ae0002        	ldw	x,#2
 461  00a6 a61e          	ld	a,#30
 462  00a8 95            	ld	xh,a
 463  00a9 cd013a        	call	_halSpiWriteReg
 465                     ; 199 	halSpiWriteReg(CCxxx0_WOREVT0, 0xB5);// 876B 1S
 467  00ac ae00b5        	ldw	x,#181
 468  00af a61f          	ld	a,#31
 469  00b1 95            	ld	xh,a
 470  00b2 cd013a        	call	_halSpiWriteReg
 472                     ; 200 	halSpiWriteReg(CCxxx0_MCSM2, 0x1f); // RX_TIME // RX_TIME_RSSI= 1 RX_TIME_QUAL = 0
 474  00b5 ae001f        	ldw	x,#31
 475  00b8 a616          	ld	a,#22
 476  00ba 95            	ld	xh,a
 477  00bb ad7d          	call	_halSpiWriteReg
 479                     ; 203 }
 482  00bd 81            	ret
 506                     ; 204 void StartRxWor(void)
 506                     ; 205 {
 507                     	switch	.text
 508  00be               _StartRxWor:
 512                     ; 206 	halSpiStrobe(CCxxx0_SIDLE);
 514  00be a636          	ld	a,#54
 515  00c0 cd018d        	call	_halSpiStrobe
 517                     ; 207 	halSpiStrobe(CCxxx0_SWORRST);
 519  00c3 a63c          	ld	a,#60
 520  00c5 cd018d        	call	_halSpiStrobe
 522                     ; 208 	halSpiStrobe(CCxxx0_SWOR);
 524  00c8 a638          	ld	a,#56
 525  00ca cd018d        	call	_halSpiStrobe
 527                     ; 209 }
 530  00cd 81            	ret
 557                     ; 219 unsigned char CC1101_Init(void)
 557                     ; 220 {
 558                     	switch	.text
 559  00ce               _CC1101_Init:
 563                     ; 221 	if (POWER_UP_RESET_CC1100()==1) return 1;
 565  00ce ad49          	call	_POWER_UP_RESET_CC1100
 567  00d0 a101          	cp	a,#1
 568  00d2 2603          	jrne	L122
 571  00d4 a601          	ld	a,#1
 574  00d6 81            	ret
 575  00d7               L122:
 576                     ; 222 	halRfWriteRfSettings();
 578  00d7 cd0226        	call	_halRfWriteRfSettings
 580                     ; 223 	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
 582  00da 4b08          	push	#8
 583  00dc ae0000        	ldw	x,#_PaTabel
 584  00df 89            	pushw	x
 585  00e0 a63e          	ld	a,#62
 586  00e2 ad72          	call	_halSpiWriteBurstReg
 588  00e4 5b03          	addw	sp,#3
 589                     ; 226 	return 0;
 591  00e6 4f            	clr	a
 594  00e7 81            	ret
 629                     ; 236 unsigned char RESET_CC1100(void) 
 629                     ; 237 {
 630                     	switch	.text
 631  00e8               _RESET_CC1100:
 633  00e8 88            	push	a
 634       00000001      OFST:	set	1
 637                     ; 238 	unsigned char i=0;
 639  00e9 0f01          	clr	(OFST+0,sp)
 640                     ; 239 	SetCSN(0); 
 642  00eb 72135000      	bres	20480,#1
 645  00ef 200d          	jra	L342
 646  00f1               L142:
 647                     ; 240 	while (GetMISO()){i++;if (i>250) return 1;};
 649  00f1 0c01          	inc	(OFST+0,sp)
 652  00f3 7b01          	ld	a,(OFST+0,sp)
 653  00f5 a1fb          	cp	a,#251
 654  00f7 2505          	jrult	L342
 657  00f9 a601          	ld	a,#1
 660  00fb 5b01          	addw	sp,#1
 661  00fd 81            	ret
 662  00fe               L342:
 665  00fe c6500b        	ld	a,20491
 666  0101 a580          	bcp	a,#128
 667  0103 26ec          	jrne	L142
 668                     ; 241   SPI_SendByte(CCxxx0_SRES); 		//写入复位命令
 671  0105 a630          	ld	a,#48
 672  0107 cd0087        	call	_SPI_SendByte
 675  010a               L352:
 676                     ; 242 	while (GetMISO()); 
 678  010a c6500b        	ld	a,20491
 679  010d a580          	bcp	a,#128
 680  010f 26f9          	jrne	L352
 681                     ; 243   SetCSN(1); 
 683  0111 72125000      	bset	20480,#1
 684                     ; 244 	return 0;
 687  0115 4f            	clr	a
 690  0116 5b01          	addw	sp,#1
 691  0118 81            	ret
 717                     ; 253 unsigned char POWER_UP_RESET_CC1100(void) 
 717                     ; 254 {
 718                     	switch	.text
 719  0119               _POWER_UP_RESET_CC1100:
 723                     ; 255 	SetCSN(1); 
 725  0119 72125000      	bset	20480,#1
 726                     ; 256 	halWait(1); 
 729  011d ae0001        	ldw	x,#1
 730  0120 cd0029        	call	_halWait
 732                     ; 257 	SetCSN(0); 
 734  0123 72135000      	bres	20480,#1
 735                     ; 258 	halWait(1); 
 738  0127 ae0001        	ldw	x,#1
 739  012a cd0029        	call	_halWait
 741                     ; 259 	SetCSN(1); 
 743  012d 72125000      	bset	20480,#1
 744                     ; 260 	halWait(41); 
 747  0131 ae0029        	ldw	x,#41
 748  0134 cd0029        	call	_halWait
 750                     ; 261 	return RESET_CC1100();   		//复位CC1100
 752  0137 adaf          	call	_RESET_CC1100
 756  0139 81            	ret
 800                     ; 270 void halSpiWriteReg(INT8U addr, INT8U value) 
 800                     ; 271 {
 801                     	switch	.text
 802  013a               _halSpiWriteReg:
 804  013a 89            	pushw	x
 805       00000000      OFST:	set	0
 808                     ; 272     SetCSN(0);
 810  013b 72135000      	bres	20480,#1
 813  013f               L313:
 814                     ; 273     while (GetMISO());
 816  013f c6500b        	ld	a,20491
 817  0142 a580          	bcp	a,#128
 818  0144 26f9          	jrne	L313
 819                     ; 274     SPI_SendByte(addr);		//写地址
 821  0146 7b01          	ld	a,(OFST+1,sp)
 822  0148 cd0087        	call	_SPI_SendByte
 824                     ; 275     SPI_SendByte(value);		//写入配置
 826  014b 7b02          	ld	a,(OFST+2,sp)
 827  014d cd0087        	call	_SPI_SendByte
 829                     ; 276     SetCSN(1);
 831  0150 72125000      	bset	20480,#1
 832                     ; 277 }
 836  0154 85            	popw	x
 837  0155 81            	ret
 911                     ; 285 void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
 911                     ; 286 {
 912                     	switch	.text
 913  0156               _halSpiWriteBurstReg:
 915  0156 88            	push	a
 916  0157 88            	push	a
 917       00000001      OFST:	set	1
 920                     ; 288 		temp = addr | WRITE_BURST;
 922  0158 aa40          	or	a,#64
 923  015a 6b01          	ld	(OFST+0,sp),a
 924                     ; 289     SetCSN(0);
 926  015c 72135000      	bres	20480,#1
 929  0160               L753:
 930                     ; 290     while (GetMISO());
 932  0160 c6500b        	ld	a,20491
 933  0163 a580          	bcp	a,#128
 934  0165 26f9          	jrne	L753
 935                     ; 291     SPI_SendData(temp);
 937  0167 7b01          	ld	a,(OFST+0,sp)
 938  0169 cd0000        	call	_SPI_SendData
 940                     ; 292     for (i = 0; i < count; i++)
 942  016c 0f01          	clr	(OFST+0,sp)
 944  016e 2011          	jra	L763
 945  0170               L363:
 946                     ; 294 			SPI_SendByte(buffer[i]);
 948  0170 7b05          	ld	a,(OFST+4,sp)
 949  0172 97            	ld	xl,a
 950  0173 7b06          	ld	a,(OFST+5,sp)
 951  0175 1b01          	add	a,(OFST+0,sp)
 952  0177 2401          	jrnc	L23
 953  0179 5c            	incw	x
 954  017a               L23:
 955  017a 02            	rlwa	x,a
 956  017b f6            	ld	a,(x)
 957  017c cd0087        	call	_SPI_SendByte
 959                     ; 292     for (i = 0; i < count; i++)
 961  017f 0c01          	inc	(OFST+0,sp)
 962  0181               L763:
 965  0181 7b01          	ld	a,(OFST+0,sp)
 966  0183 1107          	cp	a,(OFST+6,sp)
 967  0185 25e9          	jrult	L363
 968                     ; 296     SetCSN(1);
 970  0187 72125000      	bset	20480,#1
 971                     ; 297 }
 975  018b 85            	popw	x
 976  018c 81            	ret
1011                     ; 305 void halSpiStrobe(INT8U strobe) 
1011                     ; 306 {
1012                     	switch	.text
1013  018d               _halSpiStrobe:
1015  018d 88            	push	a
1016       00000000      OFST:	set	0
1019                     ; 307     SetCSN(0);
1021  018e 72135000      	bres	20480,#1
1024  0192               L314:
1025                     ; 308     while (GetMISO());
1027  0192 c6500b        	ld	a,20491
1028  0195 a580          	bcp	a,#128
1029  0197 26f9          	jrne	L314
1030                     ; 309     SPI_SendByte(strobe);		//写入命令
1032  0199 7b01          	ld	a,(OFST+1,sp)
1033  019b cd0087        	call	_SPI_SendByte
1035                     ; 310     SetCSN(1);
1037  019e 72125000      	bset	20480,#1
1038                     ; 311 }
1042  01a2 84            	pop	a
1043  01a3 81            	ret
1096                     ; 319 INT8U halSpiReadReg(INT8U addr) 
1096                     ; 320 {
1097                     	switch	.text
1098  01a4               _halSpiReadReg:
1100  01a4 88            	push	a
1101       00000001      OFST:	set	1
1104                     ; 322   temp = addr|READ_SINGLE;//读寄存器命令
1106  01a5 aa80          	or	a,#128
1107  01a7 6b01          	ld	(OFST+0,sp),a
1108                     ; 323 	SetCSN(0);
1110  01a9 72135000      	bres	20480,#1
1113  01ad               L744:
1114                     ; 324 	while (GetMISO());
1116  01ad c6500b        	ld	a,20491
1117  01b0 a580          	bcp	a,#128
1118  01b2 26f9          	jrne	L744
1119                     ; 325 	SPI_SendByte(temp);
1121  01b4 7b01          	ld	a,(OFST+0,sp)
1122  01b6 cd0087        	call	_SPI_SendByte
1124                     ; 326 	value = SPI_SendByte(0);
1126  01b9 4f            	clr	a
1127  01ba cd0087        	call	_SPI_SendByte
1129  01bd 6b01          	ld	(OFST+0,sp),a
1130                     ; 327 	SetCSN(1);
1132  01bf 72125000      	bset	20480,#1
1133                     ; 328 	return value;
1136  01c3 7b01          	ld	a,(OFST+0,sp)
1139  01c5 5b01          	addw	sp,#1
1140  01c7 81            	ret
1212                     ; 338 void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
1212                     ; 339 {
1213                     	switch	.text
1214  01c8               _halSpiReadBurstReg:
1216  01c8 88            	push	a
1217  01c9 88            	push	a
1218       00000001      OFST:	set	1
1221                     ; 341 		temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
1223  01ca aac0          	or	a,#192
1224  01cc 6b01          	ld	(OFST+0,sp),a
1225                     ; 342     SetCSN(0);
1227  01ce 72135000      	bres	20480,#1
1230  01d2               L315:
1231                     ; 343     while (GetMISO());
1233  01d2 c6500b        	ld	a,20491
1234  01d5 a580          	bcp	a,#128
1235  01d7 26f9          	jrne	L315
1236                     ; 344 		SPI_SendByte(temp);   
1238  01d9 7b01          	ld	a,(OFST+0,sp)
1239  01db cd0087        	call	_SPI_SendByte
1241                     ; 345     for (i = 0; i < count; i++) 
1243  01de 0f01          	clr	(OFST+0,sp)
1245  01e0 2014          	jra	L325
1246  01e2               L715:
1247                     ; 347         buffer[i] = SPI_SendByte(0);
1249  01e2 7b05          	ld	a,(OFST+4,sp)
1250  01e4 97            	ld	xl,a
1251  01e5 7b06          	ld	a,(OFST+5,sp)
1252  01e7 1b01          	add	a,(OFST+0,sp)
1253  01e9 2401          	jrnc	L24
1254  01eb 5c            	incw	x
1255  01ec               L24:
1256  01ec 02            	rlwa	x,a
1257  01ed 89            	pushw	x
1258  01ee 4f            	clr	a
1259  01ef cd0087        	call	_SPI_SendByte
1261  01f2 85            	popw	x
1262  01f3 f7            	ld	(x),a
1263                     ; 345     for (i = 0; i < count; i++) 
1265  01f4 0c01          	inc	(OFST+0,sp)
1266  01f6               L325:
1269  01f6 7b01          	ld	a,(OFST+0,sp)
1270  01f8 1107          	cp	a,(OFST+6,sp)
1271  01fa 25e6          	jrult	L715
1272                     ; 349     SetCSN(1);
1274  01fc 72125000      	bset	20480,#1
1275                     ; 350 }
1279  0200 85            	popw	x
1280  0201 81            	ret
1333                     ; 359 INT8U halSpiReadStatus(INT8U addr) 
1333                     ; 360 {
1334                     	switch	.text
1335  0202               _halSpiReadStatus:
1337  0202 88            	push	a
1338       00000001      OFST:	set	1
1341                     ; 362 		temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
1343  0203 aac0          	or	a,#192
1344  0205 6b01          	ld	(OFST+0,sp),a
1345                     ; 363     SetCSN(0);
1347  0207 72135000      	bres	20480,#1
1350  020b               L755:
1351                     ; 364     while (GetMISO());
1353  020b c6500b        	ld	a,20491
1354  020e a580          	bcp	a,#128
1355  0210 26f9          	jrne	L755
1356                     ; 365     SPI_SendByte(temp);
1358  0212 7b01          	ld	a,(OFST+0,sp)
1359  0214 cd0087        	call	_SPI_SendByte
1361                     ; 366 		value = SPI_SendByte(0);
1363  0217 4f            	clr	a
1364  0218 cd0087        	call	_SPI_SendByte
1366  021b 6b01          	ld	(OFST+0,sp),a
1367                     ; 367 		SetCSN(1);
1369  021d 72125000      	bset	20480,#1
1370                     ; 368 		return value;
1373  0221 7b01          	ld	a,(OFST+0,sp)
1376  0223 5b01          	addw	sp,#1
1377  0225 81            	ret
1403                     ; 376 void halRfWriteRfSettings(void) 
1403                     ; 377 {
1404                     	switch	.text
1405  0226               _halRfWriteRfSettings:
1409                     ; 379 	halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//自已加的
1411  0226 5f            	clrw	x
1412  0227 a60c          	ld	a,#12
1413  0229 95            	ld	xh,a
1414  022a cd013a        	call	_halSpiWriteReg
1416                     ; 381     halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
1418  022d ae0008        	ldw	x,#8
1419  0230 a60b          	ld	a,#11
1420  0232 95            	ld	xh,a
1421  0233 cd013a        	call	_halSpiWriteReg
1423                     ; 382     halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
1425  0236 5f            	clrw	x
1426  0237 a60c          	ld	a,#12
1427  0239 95            	ld	xh,a
1428  023a cd013a        	call	_halSpiWriteReg
1430                     ; 383     halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
1432  023d ae0010        	ldw	x,#16
1433  0240 a60d          	ld	a,#13
1434  0242 95            	ld	xh,a
1435  0243 cd013a        	call	_halSpiWriteReg
1437                     ; 384     halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
1439  0246 ae00a7        	ldw	x,#167
1440  0249 a60e          	ld	a,#14
1441  024b 95            	ld	xh,a
1442  024c cd013a        	call	_halSpiWriteReg
1444                     ; 385     halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
1446  024f ae0062        	ldw	x,#98
1447  0252 a60f          	ld	a,#15
1448  0254 95            	ld	xh,a
1449  0255 cd013a        	call	_halSpiWriteReg
1451                     ; 386     halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
1453  0258 ae0058        	ldw	x,#88
1454  025b a610          	ld	a,#16
1455  025d 95            	ld	xh,a
1456  025e cd013a        	call	_halSpiWriteReg
1458                     ; 387     halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
1460  0261 ae0043        	ldw	x,#67
1461  0264 a611          	ld	a,#17
1462  0266 95            	ld	xh,a
1463  0267 cd013a        	call	_halSpiWriteReg
1465                     ; 388     halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
1467  026a ae0003        	ldw	x,#3
1468  026d a612          	ld	a,#18
1469  026f 95            	ld	xh,a
1470  0270 cd013a        	call	_halSpiWriteReg
1472                     ; 389     halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
1474  0273 ae0022        	ldw	x,#34
1475  0276 a613          	ld	a,#19
1476  0278 95            	ld	xh,a
1477  0279 cd013a        	call	_halSpiWriteReg
1479                     ; 390     halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
1481  027c ae00f8        	ldw	x,#248
1482  027f a614          	ld	a,#20
1483  0281 95            	ld	xh,a
1484  0282 cd013a        	call	_halSpiWriteReg
1486                     ; 391     halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
1488  0285 5f            	clrw	x
1489  0286 a60a          	ld	a,#10
1490  0288 95            	ld	xh,a
1491  0289 cd013a        	call	_halSpiWriteReg
1493                     ; 392     halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
1495  028c ae0047        	ldw	x,#71
1496  028f a615          	ld	a,#21
1497  0291 95            	ld	xh,a
1498  0292 cd013a        	call	_halSpiWriteReg
1500                     ; 393     halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
1502  0295 ae0056        	ldw	x,#86
1503  0298 a621          	ld	a,#33
1504  029a 95            	ld	xh,a
1505  029b cd013a        	call	_halSpiWriteReg
1507                     ; 394     halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
1509  029e ae0010        	ldw	x,#16
1510  02a1 a622          	ld	a,#34
1511  02a3 95            	ld	xh,a
1512  02a4 cd013a        	call	_halSpiWriteReg
1514                     ; 395     halSpiWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
1516  02a7 ae0018        	ldw	x,#24
1517  02aa a618          	ld	a,#24
1518  02ac 95            	ld	xh,a
1519  02ad cd013a        	call	_halSpiWriteReg
1521                     ; 396     halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
1523  02b0 ae001d        	ldw	x,#29
1524  02b3 a619          	ld	a,#25
1525  02b5 95            	ld	xh,a
1526  02b6 cd013a        	call	_halSpiWriteReg
1528                     ; 397     halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
1530  02b9 ae001c        	ldw	x,#28
1531  02bc a61a          	ld	a,#26
1532  02be 95            	ld	xh,a
1533  02bf cd013a        	call	_halSpiWriteReg
1535                     ; 398     halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
1537  02c2 ae0043        	ldw	x,#67
1538  02c5 a61b          	ld	a,#27
1539  02c7 95            	ld	xh,a
1540  02c8 cd013a        	call	_halSpiWriteReg
1542                     ; 399 		halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
1544  02cb ae0040        	ldw	x,#64
1545  02ce a61c          	ld	a,#28
1546  02d0 95            	ld	xh,a
1547  02d1 cd013a        	call	_halSpiWriteReg
1549                     ; 400     halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
1551  02d4 ae0091        	ldw	x,#145
1552  02d7 a61d          	ld	a,#29
1553  02d9 95            	ld	xh,a
1554  02da cd013a        	call	_halSpiWriteReg
1556                     ; 401     halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
1558  02dd ae00ea        	ldw	x,#234
1559  02e0 a623          	ld	a,#35
1560  02e2 95            	ld	xh,a
1561  02e3 cd013a        	call	_halSpiWriteReg
1563                     ; 402 		halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
1565  02e6 ae002a        	ldw	x,#42
1566  02e9 a624          	ld	a,#36
1567  02eb 95            	ld	xh,a
1568  02ec cd013a        	call	_halSpiWriteReg
1570                     ; 403 		halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
1572  02ef 5f            	clrw	x
1573  02f0 a625          	ld	a,#37
1574  02f2 95            	ld	xh,a
1575  02f3 cd013a        	call	_halSpiWriteReg
1577                     ; 404     halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
1579  02f6 ae0011        	ldw	x,#17
1580  02f9 a626          	ld	a,#38
1581  02fb 95            	ld	xh,a
1582  02fc cd013a        	call	_halSpiWriteReg
1584                     ; 405     halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
1586  02ff ae0059        	ldw	x,#89
1587  0302 a629          	ld	a,#41
1588  0304 95            	ld	xh,a
1589  0305 cd013a        	call	_halSpiWriteReg
1591                     ; 406     halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
1593  0308 ae0081        	ldw	x,#129
1594  030b a62c          	ld	a,#44
1595  030d 95            	ld	xh,a
1596  030e cd013a        	call	_halSpiWriteReg
1598                     ; 407     halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
1600  0311 ae0035        	ldw	x,#53
1601  0314 a62d          	ld	a,#45
1602  0316 95            	ld	xh,a
1603  0317 cd013a        	call	_halSpiWriteReg
1605                     ; 408     halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
1607  031a ae0009        	ldw	x,#9
1608  031d a62e          	ld	a,#46
1609  031f 95            	ld	xh,a
1610  0320 cd013a        	call	_halSpiWriteReg
1612                     ; 409     halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
1614  0323 ae000e        	ldw	x,#14
1615  0326 4f            	clr	a
1616  0327 95            	ld	xh,a
1617  0328 cd013a        	call	_halSpiWriteReg
1619                     ; 410     halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
1621  032b ae0006        	ldw	x,#6
1622  032e a602          	ld	a,#2
1623  0330 95            	ld	xh,a
1624  0331 cd013a        	call	_halSpiWriteReg
1626                     ; 411     halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
1628  0334 ae0004        	ldw	x,#4
1629  0337 a607          	ld	a,#7
1630  0339 95            	ld	xh,a
1631  033a cd013a        	call	_halSpiWriteReg
1633                     ; 412     halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
1635  033d ae0005        	ldw	x,#5
1636  0340 a608          	ld	a,#8
1637  0342 95            	ld	xh,a
1638  0343 cd013a        	call	_halSpiWriteReg
1640                     ; 413     halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
1642  0346 5f            	clrw	x
1643  0347 a609          	ld	a,#9
1644  0349 95            	ld	xh,a
1645  034a cd013a        	call	_halSpiWriteReg
1647                     ; 414     halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
1649  034d ae0020        	ldw	x,#32
1650  0350 a606          	ld	a,#6
1651  0352 95            	ld	xh,a
1652  0353 cd013a        	call	_halSpiWriteReg
1654                     ; 419 		halSpiWriteReg(CCxxx0_WOREVT1,  rfSettings.WOREVT1);
1656  0356 5f            	clrw	x
1657  0357 a61e          	ld	a,#30
1658  0359 95            	ld	xh,a
1659  035a cd013a        	call	_halSpiWriteReg
1661                     ; 420     halSpiWriteReg(CCxxx0_WOREVT0,  rfSettings.WOREVT0);
1663  035d 5f            	clrw	x
1664  035e a61f          	ld	a,#31
1665  0360 95            	ld	xh,a
1666  0361 cd013a        	call	_halSpiWriteReg
1668                     ; 421     halSpiWriteReg(CCxxx0_WORCTRL,  rfSettings.WORCTRL);
1670  0364 5f            	clrw	x
1671  0365 a620          	ld	a,#32
1672  0367 95            	ld	xh,a
1673  0368 cd013a        	call	_halSpiWriteReg
1675                     ; 422 }
1678  036b 81            	ret
1725                     ; 431 void halRfSendPacket(INT8U *txBuffer, INT8U size) 
1725                     ; 432 {
1726                     	switch	.text
1727  036c               _halRfSendPacket:
1729  036c 89            	pushw	x
1730       00000000      OFST:	set	0
1733                     ; 433 	halSpiWriteReg(CCxxx0_TXFIFO, size);
1735  036d 7b05          	ld	a,(OFST+5,sp)
1736  036f 97            	ld	xl,a
1737  0370 a63f          	ld	a,#63
1738  0372 95            	ld	xh,a
1739  0373 cd013a        	call	_halSpiWriteReg
1741                     ; 434   halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//写入要发送的数据
1743  0376 7b05          	ld	a,(OFST+5,sp)
1744  0378 88            	push	a
1745  0379 1e02          	ldw	x,(OFST+2,sp)
1746  037b 89            	pushw	x
1747  037c a63f          	ld	a,#63
1748  037e cd0156        	call	_halSpiWriteBurstReg
1750  0381 5b03          	addw	sp,#3
1751                     ; 436   halSpiStrobe(CCxxx0_STX);		//进入发送模式发送数据
1753  0383 a635          	ld	a,#53
1754  0385 cd018d        	call	_halSpiStrobe
1757  0388               L716:
1758                     ; 439   while (GetGDO0()==0);
1760  0388 c65010        	ld	a,20496
1761  038b a510          	bcp	a,#16
1762  038d 27f9          	jreq	L716
1764  038f               L526:
1765                     ; 441   while (GetGDO0());
1767  038f c65010        	ld	a,20496
1768  0392 a510          	bcp	a,#16
1769  0394 26f9          	jrne	L526
1770                     ; 442 	halSpiStrobe(CCxxx0_SFTX);
1772  0396 a63b          	ld	a,#59
1773  0398 cd018d        	call	_halSpiStrobe
1775                     ; 443 }
1778  039b 85            	popw	x
1779  039c 81            	ret
1856                     ; 445 INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
1856                     ; 446 {
1857                     	switch	.text
1858  039d               _halRfReceivePacket:
1860  039d 89            	pushw	x
1861  039e 5204          	subw	sp,#4
1862       00000004      OFST:	set	4
1865                     ; 449 		INT8U i=(*length)*4;  // 具体多少要根据datarate和length来决定
1867  03a0 1e09          	ldw	x,(OFST+5,sp)
1868  03a2 f6            	ld	a,(x)
1869  03a3 48            	sll	a
1870  03a4 48            	sll	a
1871  03a5 6b01          	ld	(OFST-3,sp),a
1872                     ; 450     if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
1874  03a7 a63b          	ld	a,#59
1875  03a9 cd0202        	call	_halSpiReadStatus
1877  03ac a57f          	bcp	a,#127
1878  03ae 2747          	jreq	L766
1879                     ; 452       packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
1881  03b0 a63f          	ld	a,#63
1882  03b2 cd01a4        	call	_halSpiReadReg
1884  03b5 6b04          	ld	(OFST+0,sp),a
1885                     ; 453       if (packetLength <= *length) 		//如果所要的有效数据长度小于等于接收到的数据包的长度
1887  03b7 1e09          	ldw	x,(OFST+5,sp)
1888  03b9 f6            	ld	a,(x)
1889  03ba 1104          	cp	a,(OFST+0,sp)
1890  03bc 252b          	jrult	L176
1891                     ; 455         halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
1893  03be 7b04          	ld	a,(OFST+0,sp)
1894  03c0 88            	push	a
1895  03c1 1e06          	ldw	x,(OFST+2,sp)
1896  03c3 89            	pushw	x
1897  03c4 a63f          	ld	a,#63
1898  03c6 cd01c8        	call	_halSpiReadBurstReg
1900  03c9 5b03          	addw	sp,#3
1901                     ; 456         *length = packetLength;				//把接收数据长度的修改为当前数据的长度
1903  03cb 7b04          	ld	a,(OFST+0,sp)
1904  03cd 1e09          	ldw	x,(OFST+5,sp)
1905  03cf f7            	ld	(x),a
1906                     ; 459         halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
1908  03d0 4b02          	push	#2
1909  03d2 96            	ldw	x,sp
1910  03d3 1c0003        	addw	x,#OFST-1
1911  03d6 89            	pushw	x
1912  03d7 a63f          	ld	a,#63
1913  03d9 cd01c8        	call	_halSpiReadBurstReg
1915  03dc 5b03          	addw	sp,#3
1916                     ; 460 				halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
1918  03de a63a          	ld	a,#58
1919  03e0 cd018d        	call	_halSpiStrobe
1921                     ; 461         return (status[1] & CRC_OK);			//如果校验成功返回接收成功
1923  03e3 7b03          	ld	a,(OFST-1,sp)
1924  03e5 a480          	and	a,#128
1926  03e7 200b          	jra	L45
1927  03e9               L176:
1928                     ; 465         *length = packetLength;
1930  03e9 7b04          	ld	a,(OFST+0,sp)
1931  03eb 1e09          	ldw	x,(OFST+5,sp)
1932  03ed f7            	ld	(x),a
1933                     ; 466         halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
1935  03ee a63a          	ld	a,#58
1936  03f0 cd018d        	call	_halSpiStrobe
1938                     ; 467         return 0;
1940  03f3 4f            	clr	a
1942  03f4               L45:
1944  03f4 5b06          	addw	sp,#6
1945  03f6 81            	ret
1946  03f7               L766:
1947                     ; 471 		return 0;
1949  03f7 4f            	clr	a
1951  03f8 20fa          	jra	L45
2266                     	xdef	_halWait
2267                     	xdef	_rfSettings
2268                     	xdef	_CC1101_Init
2269                     	xdef	_StartRxWor
2270                     	xdef	_Init_WOR
2271                     	xdef	_halRfReceivePacket
2272                     	xdef	_halRfSendPacket
2273                     	xdef	_halRfWriteRfSettings
2274                     	xdef	_halSpiReadStatus
2275                     	xdef	_halSpiReadBurstReg
2276                     	xdef	_halSpiReadReg
2277                     	xdef	_halSpiStrobe
2278                     	xdef	_halSpiWriteBurstReg
2279                     	xdef	_halSpiWriteReg
2280                     	xdef	_POWER_UP_RESET_CC1100
2281                     	xdef	_RESET_CC1100
2282                     	xdef	_delay
2283                     	xdef	_PaTabel
2284                     	xdef	_SPI_SendByte
2285                     	xdef	_SPITransferByte
2286                     	xref	_Delayus
2287                     	xref	_SPI_SendData
2306                     	end
