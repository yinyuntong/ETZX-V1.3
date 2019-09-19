   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  64                     ; 81 void FLASH_Unlock(FLASH_MemType_TypeDef FLASH_MemType)
  64                     ; 82 {
  66                     	switch	.text
  67  0000               _FLASH_Unlock:
  71                     ; 84     assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
  73                     ; 87     if (FLASH_MemType == FLASH_MEMTYPE_PROG)
  75  0000 a1fd          	cp	a,#253
  76  0002 260a          	jrne	L73
  77                     ; 89         FLASH->PUKR = FLASH_RASS_KEY1;
  79  0004 35565062      	mov	20578,#86
  80                     ; 90         FLASH->PUKR = FLASH_RASS_KEY2;
  82  0008 35ae5062      	mov	20578,#174
  84  000c 2008          	jra	L14
  85  000e               L73:
  86                     ; 95         FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
  88  000e 35ae5064      	mov	20580,#174
  89                     ; 96         FLASH->DUKR = FLASH_RASS_KEY1;
  91  0012 35565064      	mov	20580,#86
  92  0016               L14:
  93                     ; 98 }
  96  0016 81            	ret
 131                     ; 106 void FLASH_Lock(FLASH_MemType_TypeDef FLASH_MemType)
 131                     ; 107 {
 132                     	switch	.text
 133  0017               _FLASH_Lock:
 137                     ; 109     assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
 139                     ; 112   FLASH->IAPSR &= (uint8_t)FLASH_MemType;
 141  0017 c4505f        	and	a,20575
 142  001a c7505f        	ld	20575,a
 143                     ; 113 }
 146  001d 81            	ret
 169                     ; 120 void FLASH_DeInit(void)
 169                     ; 121 {
 170                     	switch	.text
 171  001e               _FLASH_DeInit:
 175                     ; 122     FLASH->CR1 = FLASH_CR1_RESET_VALUE;
 177  001e 725f505a      	clr	20570
 178                     ; 123     FLASH->CR2 = FLASH_CR2_RESET_VALUE;
 180  0022 725f505b      	clr	20571
 181                     ; 124     FLASH->NCR2 = FLASH_NCR2_RESET_VALUE;
 183  0026 35ff505c      	mov	20572,#255
 184                     ; 125     FLASH->IAPSR &= (uint8_t)(~FLASH_IAPSR_DUL);
 186  002a 7217505f      	bres	20575,#3
 187                     ; 126     FLASH->IAPSR &= (uint8_t)(~FLASH_IAPSR_PUL);
 189  002e 7213505f      	bres	20575,#1
 190                     ; 127     (void) FLASH->IAPSR; /* Reading of this register causes the clearing of status flags */
 192  0032 c6505f        	ld	a,20575
 193  0035 97            	ld	xl,a
 194                     ; 128 }
 197  0036 81            	ret
 252                     ; 136 void FLASH_ITConfig(FunctionalState NewState)
 252                     ; 137 {
 253                     	switch	.text
 254  0037               _FLASH_ITConfig:
 258                     ; 139   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 260                     ; 141     if (NewState != DISABLE)
 262  0037 4d            	tnz	a
 263  0038 2706          	jreq	L711
 264                     ; 143         FLASH->CR1 |= FLASH_CR1_IE; /* Enables the interrupt sources */
 266  003a 7212505a      	bset	20570,#1
 268  003e 2004          	jra	L121
 269  0040               L711:
 270                     ; 147         FLASH->CR1 &= (uint8_t)(~FLASH_CR1_IE); /* Disables the interrupt sources */
 272  0040 7213505a      	bres	20570,#1
 273  0044               L121:
 274                     ; 149 }
 277  0044 81            	ret
 311                     ; 158 void FLASH_EraseByte(uint32_t Address)
 311                     ; 159 {
 312                     	switch	.text
 313  0045               _FLASH_EraseByte:
 315       00000000      OFST:	set	0
 318                     ; 161     assert_param(IS_FLASH_ADDRESS_OK(Address));
 320                     ; 164    *(PointerAttr uint8_t*) (uint16_t)Address = FLASH_CLEAR_BYTE; 
 322  0045 1e05          	ldw	x,(OFST+5,sp)
 323  0047 7f            	clr	(x)
 324                     ; 166 }
 327  0048 81            	ret
 370                     ; 176 void FLASH_ProgramByte(uint32_t Address, uint8_t Data)
 370                     ; 177 {
 371                     	switch	.text
 372  0049               _FLASH_ProgramByte:
 374       00000000      OFST:	set	0
 377                     ; 179     assert_param(IS_FLASH_ADDRESS_OK(Address));
 379                     ; 180     *(PointerAttr uint8_t*) (uint16_t)Address = Data;
 381  0049 7b07          	ld	a,(OFST+7,sp)
 382  004b 1e05          	ldw	x,(OFST+5,sp)
 383  004d f7            	ld	(x),a
 384                     ; 181 }
 387  004e 81            	ret
 421                     ; 190 uint8_t FLASH_ReadByte(uint32_t Address)
 421                     ; 191 {
 422                     	switch	.text
 423  004f               _FLASH_ReadByte:
 425       00000000      OFST:	set	0
 428                     ; 193     assert_param(IS_FLASH_ADDRESS_OK(Address));
 430                     ; 196     return(*(PointerAttr uint8_t *) (uint16_t)Address); 
 432  004f 1e05          	ldw	x,(OFST+5,sp)
 433  0051 f6            	ld	a,(x)
 436  0052 81            	ret
 479                     ; 207 void FLASH_ProgramWord(uint32_t Address, uint32_t Data)
 479                     ; 208 {
 480                     	switch	.text
 481  0053               _FLASH_ProgramWord:
 483       00000000      OFST:	set	0
 486                     ; 210     assert_param(IS_FLASH_ADDRESS_OK(Address));
 488                     ; 213     FLASH->CR2 |= FLASH_CR2_WPRG;
 490  0053 721c505b      	bset	20571,#6
 491                     ; 214     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NWPRG);
 493  0057 721d505c      	bres	20572,#6
 494                     ; 217     *((PointerAttr uint8_t*)(uint16_t)Address)       = *((uint8_t*)(&Data));
 496  005b 7b07          	ld	a,(OFST+7,sp)
 497  005d 1e05          	ldw	x,(OFST+5,sp)
 498  005f f7            	ld	(x),a
 499                     ; 219     *(((PointerAttr uint8_t*)(uint16_t)Address) + 1) = *((uint8_t*)(&Data)+1); 
 501  0060 7b08          	ld	a,(OFST+8,sp)
 502  0062 1e05          	ldw	x,(OFST+5,sp)
 503  0064 e701          	ld	(1,x),a
 504                     ; 221     *(((PointerAttr uint8_t*)(uint16_t)Address) + 2) = *((uint8_t*)(&Data)+2); 
 506  0066 7b09          	ld	a,(OFST+9,sp)
 507  0068 1e05          	ldw	x,(OFST+5,sp)
 508  006a e702          	ld	(2,x),a
 509                     ; 223     *(((PointerAttr uint8_t*)(uint16_t)Address) + 3) = *((uint8_t*)(&Data)+3); 
 511  006c 7b0a          	ld	a,(OFST+10,sp)
 512  006e 1e05          	ldw	x,(OFST+5,sp)
 513  0070 e703          	ld	(3,x),a
 514                     ; 224 }
 517  0072 81            	ret
 562                     ; 232 void FLASH_ProgramOptionByte(uint16_t Address, uint8_t Data)
 562                     ; 233 {
 563                     	switch	.text
 564  0073               _FLASH_ProgramOptionByte:
 566  0073 89            	pushw	x
 567       00000000      OFST:	set	0
 570                     ; 235     assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
 572                     ; 238     FLASH->CR2 |= FLASH_CR2_OPT;
 574  0074 721e505b      	bset	20571,#7
 575                     ; 239     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);
 577  0078 721f505c      	bres	20572,#7
 578                     ; 242     if (Address == 0x4800)
 580  007c a34800        	cpw	x,#18432
 581  007f 2607          	jrne	L542
 582                     ; 245        *((NEAR uint8_t*)Address) = Data;
 584  0081 7b05          	ld	a,(OFST+5,sp)
 585  0083 1e01          	ldw	x,(OFST+1,sp)
 586  0085 f7            	ld	(x),a
 588  0086 200c          	jra	L742
 589  0088               L542:
 590                     ; 250        *((NEAR uint8_t*)Address) = Data;
 592  0088 7b05          	ld	a,(OFST+5,sp)
 593  008a 1e01          	ldw	x,(OFST+1,sp)
 594  008c f7            	ld	(x),a
 595                     ; 251        *((NEAR uint8_t*)((uint16_t)(Address + 1))) = (uint8_t)(~Data);
 597  008d 7b05          	ld	a,(OFST+5,sp)
 598  008f 43            	cpl	a
 599  0090 1e01          	ldw	x,(OFST+1,sp)
 600  0092 e701          	ld	(1,x),a
 601  0094               L742:
 602                     ; 253     FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
 604  0094 a6fd          	ld	a,#253
 605  0096 cd0196        	call	_FLASH_WaitForLastOperation
 607                     ; 256     FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
 609  0099 721f505b      	bres	20571,#7
 610                     ; 257     FLASH->NCR2 |= FLASH_NCR2_NOPT;
 612  009d 721e505c      	bset	20572,#7
 613                     ; 258 }
 616  00a1 85            	popw	x
 617  00a2 81            	ret
 653                     ; 265 void FLASH_EraseOptionByte(uint16_t Address)
 653                     ; 266 {
 654                     	switch	.text
 655  00a3               _FLASH_EraseOptionByte:
 657  00a3 89            	pushw	x
 658       00000000      OFST:	set	0
 661                     ; 268     assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
 663                     ; 271     FLASH->CR2 |= FLASH_CR2_OPT;
 665  00a4 721e505b      	bset	20571,#7
 666                     ; 272     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);
 668  00a8 721f505c      	bres	20572,#7
 669                     ; 275      if (Address == 0x4800)
 671  00ac a34800        	cpw	x,#18432
 672  00af 2603          	jrne	L762
 673                     ; 278        *((NEAR uint8_t*)Address) = FLASH_CLEAR_BYTE;
 675  00b1 7f            	clr	(x)
 677  00b2 2009          	jra	L172
 678  00b4               L762:
 679                     ; 283        *((NEAR uint8_t*)Address) = FLASH_CLEAR_BYTE;
 681  00b4 1e01          	ldw	x,(OFST+1,sp)
 682  00b6 7f            	clr	(x)
 683                     ; 284        *((NEAR uint8_t*)((uint16_t)(Address + (uint16_t)1 ))) = FLASH_SET_BYTE;
 685  00b7 1e01          	ldw	x,(OFST+1,sp)
 686  00b9 a6ff          	ld	a,#255
 687  00bb e701          	ld	(1,x),a
 688  00bd               L172:
 689                     ; 286     FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
 691  00bd a6fd          	ld	a,#253
 692  00bf cd0196        	call	_FLASH_WaitForLastOperation
 694                     ; 289     FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
 696  00c2 721f505b      	bres	20571,#7
 697                     ; 290     FLASH->NCR2 |= FLASH_NCR2_NOPT;
 699  00c6 721e505c      	bset	20572,#7
 700                     ; 291 }
 703  00ca 85            	popw	x
 704  00cb 81            	ret
 767                     ; 297 uint16_t FLASH_ReadOptionByte(uint16_t Address)
 767                     ; 298 {
 768                     	switch	.text
 769  00cc               _FLASH_ReadOptionByte:
 771  00cc 89            	pushw	x
 772  00cd 5204          	subw	sp,#4
 773       00000004      OFST:	set	4
 776                     ; 299     uint8_t value_optbyte, value_optbyte_complement = 0;
 778  00cf 7b01          	ld	a,(OFST-3,sp)
 779  00d1 97            	ld	xl,a
 780                     ; 300     uint16_t res_value = 0;
 782  00d2 1e03          	ldw	x,(OFST-1,sp)
 783                     ; 303     assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
 785                     ; 306     value_optbyte = *((NEAR uint8_t*)Address); /* Read option byte */
 787  00d4 1e05          	ldw	x,(OFST+1,sp)
 788  00d6 f6            	ld	a,(x)
 789  00d7 6b02          	ld	(OFST-2,sp),a
 790                     ; 307     value_optbyte_complement = *(((NEAR uint8_t*)Address) + 1); /* Read option byte complement */
 792  00d9 1e05          	ldw	x,(OFST+1,sp)
 793  00db e601          	ld	a,(1,x)
 794  00dd 6b01          	ld	(OFST-3,sp),a
 795                     ; 310     if (Address == 0x4800)	 
 797  00df 1e05          	ldw	x,(OFST+1,sp)
 798  00e1 a34800        	cpw	x,#18432
 799  00e4 2608          	jrne	L523
 800                     ; 312         res_value =	 value_optbyte;
 802  00e6 7b02          	ld	a,(OFST-2,sp)
 803  00e8 5f            	clrw	x
 804  00e9 97            	ld	xl,a
 805  00ea 1f03          	ldw	(OFST-1,sp),x
 807  00ec 2023          	jra	L723
 808  00ee               L523:
 809                     ; 316         if (value_optbyte == (uint8_t)(~value_optbyte_complement))
 811  00ee 7b01          	ld	a,(OFST-3,sp)
 812  00f0 43            	cpl	a
 813  00f1 1102          	cp	a,(OFST-2,sp)
 814  00f3 2617          	jrne	L133
 815                     ; 318             res_value = (uint16_t)((uint16_t)value_optbyte << 8);
 817  00f5 7b02          	ld	a,(OFST-2,sp)
 818  00f7 5f            	clrw	x
 819  00f8 97            	ld	xl,a
 820  00f9 4f            	clr	a
 821  00fa 02            	rlwa	x,a
 822  00fb 1f03          	ldw	(OFST-1,sp),x
 823                     ; 319             res_value = res_value | (uint16_t)value_optbyte_complement;
 825  00fd 7b01          	ld	a,(OFST-3,sp)
 826  00ff 5f            	clrw	x
 827  0100 97            	ld	xl,a
 828  0101 01            	rrwa	x,a
 829  0102 1a04          	or	a,(OFST+0,sp)
 830  0104 01            	rrwa	x,a
 831  0105 1a03          	or	a,(OFST-1,sp)
 832  0107 01            	rrwa	x,a
 833  0108 1f03          	ldw	(OFST-1,sp),x
 835  010a 2005          	jra	L723
 836  010c               L133:
 837                     ; 323             res_value = FLASH_OPTIONBYTE_ERROR;
 839  010c ae5555        	ldw	x,#21845
 840  010f 1f03          	ldw	(OFST-1,sp),x
 841  0111               L723:
 842                     ; 326     return(res_value);
 844  0111 1e03          	ldw	x,(OFST-1,sp)
 847  0113 5b06          	addw	sp,#6
 848  0115 81            	ret
 922                     ; 335 void FLASH_SetLowPowerMode(FLASH_LPMode_TypeDef FLASH_LPMode)
 922                     ; 336 {
 923                     	switch	.text
 924  0116               _FLASH_SetLowPowerMode:
 926  0116 88            	push	a
 927       00000000      OFST:	set	0
 930                     ; 338     assert_param(IS_FLASH_LOW_POWER_MODE_OK(FLASH_LPMode));
 932                     ; 341     FLASH->CR1 &= (uint8_t)(~(FLASH_CR1_HALT | FLASH_CR1_AHALT)); 
 934  0117 c6505a        	ld	a,20570
 935  011a a4f3          	and	a,#243
 936  011c c7505a        	ld	20570,a
 937                     ; 344     FLASH->CR1 |= (uint8_t)FLASH_LPMode; 
 939  011f c6505a        	ld	a,20570
 940  0122 1a01          	or	a,(OFST+1,sp)
 941  0124 c7505a        	ld	20570,a
 942                     ; 345 }
 945  0127 84            	pop	a
 946  0128 81            	ret
1004                     ; 353 void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef FLASH_ProgTime)
1004                     ; 354 {
1005                     	switch	.text
1006  0129               _FLASH_SetProgrammingTime:
1010                     ; 356     assert_param(IS_FLASH_PROGRAM_TIME_OK(FLASH_ProgTime));
1012                     ; 358     FLASH->CR1 &= (uint8_t)(~FLASH_CR1_FIX);
1014  0129 7211505a      	bres	20570,#0
1015                     ; 359     FLASH->CR1 |= (uint8_t)FLASH_ProgTime;
1017  012d ca505a        	or	a,20570
1018  0130 c7505a        	ld	20570,a
1019                     ; 360 }
1022  0133 81            	ret
1047                     ; 367 FLASH_LPMode_TypeDef FLASH_GetLowPowerMode(void)
1047                     ; 368 {
1048                     	switch	.text
1049  0134               _FLASH_GetLowPowerMode:
1053                     ; 369     return((FLASH_LPMode_TypeDef)(FLASH->CR1 & (uint8_t)(FLASH_CR1_HALT | FLASH_CR1_AHALT)));
1055  0134 c6505a        	ld	a,20570
1056  0137 a40c          	and	a,#12
1059  0139 81            	ret
1084                     ; 377 FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void)
1084                     ; 378 {
1085                     	switch	.text
1086  013a               _FLASH_GetProgrammingTime:
1090                     ; 379     return((FLASH_ProgramTime_TypeDef)(FLASH->CR1 & FLASH_CR1_FIX));
1092  013a c6505a        	ld	a,20570
1093  013d a401          	and	a,#1
1096  013f 81            	ret
1130                     ; 387 uint32_t FLASH_GetBootSize(void)
1130                     ; 388 {
1131                     	switch	.text
1132  0140               _FLASH_GetBootSize:
1134  0140 5204          	subw	sp,#4
1135       00000004      OFST:	set	4
1138                     ; 389     uint32_t temp = 0;
1140  0142 ae0000        	ldw	x,#0
1141  0145 1f03          	ldw	(OFST-1,sp),x
1142  0147 ae0000        	ldw	x,#0
1143  014a 1f01          	ldw	(OFST-3,sp),x
1144                     ; 392     temp = (uint32_t)((uint32_t)FLASH->FPR * (uint32_t)512);
1146  014c c6505d        	ld	a,20573
1147  014f 5f            	clrw	x
1148  0150 97            	ld	xl,a
1149  0151 90ae0200      	ldw	y,#512
1150  0155 cd0000        	call	c_umul
1152  0158 96            	ldw	x,sp
1153  0159 1c0001        	addw	x,#OFST-3
1154  015c cd0000        	call	c_rtol
1156                     ; 395     if (FLASH->FPR == 0xFF)
1158  015f c6505d        	ld	a,20573
1159  0162 a1ff          	cp	a,#255
1160  0164 2611          	jrne	L354
1161                     ; 397         temp += 512;
1163  0166 ae0200        	ldw	x,#512
1164  0169 bf02          	ldw	c_lreg+2,x
1165  016b ae0000        	ldw	x,#0
1166  016e bf00          	ldw	c_lreg,x
1167  0170 96            	ldw	x,sp
1168  0171 1c0001        	addw	x,#OFST-3
1169  0174 cd0000        	call	c_lgadd
1171  0177               L354:
1172                     ; 401     return(temp);
1174  0177 96            	ldw	x,sp
1175  0178 1c0001        	addw	x,#OFST-3
1176  017b cd0000        	call	c_ltor
1180  017e 5b04          	addw	sp,#4
1181  0180 81            	ret
1283                     ; 412 FlagStatus FLASH_GetFlagStatus(FLASH_Flag_TypeDef FLASH_FLAG)
1283                     ; 413 {
1284                     	switch	.text
1285  0181               _FLASH_GetFlagStatus:
1287  0181 88            	push	a
1288       00000001      OFST:	set	1
1291                     ; 414     FlagStatus status = RESET;
1293  0182 0f01          	clr	(OFST+0,sp)
1294                     ; 416     assert_param(IS_FLASH_FLAGS_OK(FLASH_FLAG));
1296                     ; 419     if ((FLASH->IAPSR & (uint8_t)FLASH_FLAG) != (uint8_t)RESET)
1298  0184 c4505f        	and	a,20575
1299  0187 2706          	jreq	L325
1300                     ; 421         status = SET; /* FLASH_FLAG is set */
1302  0189 a601          	ld	a,#1
1303  018b 6b01          	ld	(OFST+0,sp),a
1305  018d 2002          	jra	L525
1306  018f               L325:
1307                     ; 425         status = RESET; /* FLASH_FLAG is reset*/
1309  018f 0f01          	clr	(OFST+0,sp)
1310  0191               L525:
1311                     ; 429     return status;
1313  0191 7b01          	ld	a,(OFST+0,sp)
1316  0193 5b01          	addw	sp,#1
1317  0195 81            	ret
1402                     ; 531 IN_RAM(FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType)) 
1402                     ; 532 {
1403                     	switch	.text
1404  0196               _FLASH_WaitForLastOperation:
1406  0196 5205          	subw	sp,#5
1407       00000005      OFST:	set	5
1410                     ; 533     uint8_t flagstatus = 0x00;
1412  0198 0f05          	clr	(OFST+0,sp)
1413                     ; 534     uint32_t timeout = OPERATION_TIMEOUT;
1415  019a aeffff        	ldw	x,#65535
1416  019d 1f03          	ldw	(OFST-2,sp),x
1417  019f ae000f        	ldw	x,#15
1418  01a2 1f01          	ldw	(OFST-4,sp),x
1420  01a4 2010          	jra	L375
1421  01a6               L765:
1422                     ; 560         flagstatus = (uint8_t)(FLASH->IAPSR & (FLASH_IAPSR_EOP | FLASH_IAPSR_WR_PG_DIS));
1424  01a6 c6505f        	ld	a,20575
1425  01a9 a405          	and	a,#5
1426  01ab 6b05          	ld	(OFST+0,sp),a
1427                     ; 561         timeout--;
1429  01ad 96            	ldw	x,sp
1430  01ae 1c0001        	addw	x,#OFST-4
1431  01b1 a601          	ld	a,#1
1432  01b3 cd0000        	call	c_lgsbc
1434  01b6               L375:
1435                     ; 558     while ((flagstatus == 0x00) && (timeout != 0x00))
1437  01b6 0d05          	tnz	(OFST+0,sp)
1438  01b8 2609          	jrne	L775
1440  01ba 96            	ldw	x,sp
1441  01bb 1c0001        	addw	x,#OFST-4
1442  01be cd0000        	call	c_lzmp
1444  01c1 26e3          	jrne	L765
1445  01c3               L775:
1446                     ; 566     if (timeout == 0x00 )
1448  01c3 96            	ldw	x,sp
1449  01c4 1c0001        	addw	x,#OFST-4
1450  01c7 cd0000        	call	c_lzmp
1452  01ca 2604          	jrne	L106
1453                     ; 568         flagstatus = FLASH_STATUS_TIMEOUT;
1455  01cc a602          	ld	a,#2
1456  01ce 6b05          	ld	(OFST+0,sp),a
1457  01d0               L106:
1458                     ; 571     return((FLASH_Status_TypeDef)flagstatus);
1460  01d0 7b05          	ld	a,(OFST+0,sp)
1463  01d2 5b05          	addw	sp,#5
1464  01d4 81            	ret
1527                     ; 581 IN_RAM(void FLASH_EraseBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType))
1527                     ; 582 {
1528                     	switch	.text
1529  01d5               _FLASH_EraseBlock:
1531  01d5 89            	pushw	x
1532  01d6 5206          	subw	sp,#6
1533       00000006      OFST:	set	6
1536                     ; 583   uint32_t startaddress = 0;
1538  01d8 96            	ldw	x,sp
1539  01d9 1c0003        	addw	x,#OFST-3
1540  01dc cd0000        	call	c_ltor
1542                     ; 592   assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
1544                     ; 593   if (FLASH_MemType == FLASH_MEMTYPE_PROG)
1546  01df 7b0b          	ld	a,(OFST+5,sp)
1547  01e1 a1fd          	cp	a,#253
1548  01e3 260c          	jrne	L536
1549                     ; 595       assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
1551                     ; 596       startaddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
1553  01e5 ae8000        	ldw	x,#32768
1554  01e8 1f05          	ldw	(OFST-1,sp),x
1555  01ea ae0000        	ldw	x,#0
1556  01ed 1f03          	ldw	(OFST-3,sp),x
1558  01ef 200a          	jra	L736
1559  01f1               L536:
1560                     ; 600       assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
1562                     ; 601       startaddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
1564  01f1 ae4000        	ldw	x,#16384
1565  01f4 1f05          	ldw	(OFST-1,sp),x
1566  01f6 ae0000        	ldw	x,#0
1567  01f9 1f03          	ldw	(OFST-3,sp),x
1568  01fb               L736:
1569                     ; 608     pwFlash = (PointerAttr uint32_t *)(uint16_t)(startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE));
1571  01fb 1e07          	ldw	x,(OFST+1,sp)
1572  01fd a640          	ld	a,#64
1573  01ff cd0000        	call	c_cmulx
1575  0202 96            	ldw	x,sp
1576  0203 1c0003        	addw	x,#OFST-3
1577  0206 cd0000        	call	c_ladd
1579  0209 be02          	ldw	x,c_lreg+2
1580  020b 1f01          	ldw	(OFST-5,sp),x
1581                     ; 612     FLASH->CR2 |= FLASH_CR2_ERASE;
1583  020d 721a505b      	bset	20571,#5
1584                     ; 613     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NERASE);
1586  0211 721b505c      	bres	20572,#5
1587                     ; 616     *pwFlash = (uint32_t)0;
1589  0215 1e01          	ldw	x,(OFST-5,sp)
1590  0217 a600          	ld	a,#0
1591  0219 e703          	ld	(3,x),a
1592  021b a600          	ld	a,#0
1593  021d e702          	ld	(2,x),a
1594  021f a600          	ld	a,#0
1595  0221 e701          	ld	(1,x),a
1596  0223 a600          	ld	a,#0
1597  0225 f7            	ld	(x),a
1598                     ; 623 }
1601  0226 5b08          	addw	sp,#8
1602  0228 81            	ret
1706                     ; 634 IN_RAM(void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType, 
1706                     ; 635                         FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer))
1706                     ; 636 {
1707                     	switch	.text
1708  0229               _FLASH_ProgramBlock:
1710  0229 89            	pushw	x
1711  022a 5206          	subw	sp,#6
1712       00000006      OFST:	set	6
1715                     ; 637     uint16_t Count = 0;
1717  022c 1e05          	ldw	x,(OFST-1,sp)
1718                     ; 638     uint32_t startaddress = 0;
1720  022e 96            	ldw	x,sp
1721  022f 1c0001        	addw	x,#OFST-5
1722  0232 cd0000        	call	c_ltor
1724                     ; 641     assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
1726                     ; 642     assert_param(IS_FLASH_PROGRAM_MODE_OK(FLASH_ProgMode));
1728                     ; 643     if (FLASH_MemType == FLASH_MEMTYPE_PROG)
1730  0235 7b0b          	ld	a,(OFST+5,sp)
1731  0237 a1fd          	cp	a,#253
1732  0239 260c          	jrne	L317
1733                     ; 645         assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
1735                     ; 646         startaddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
1737  023b ae8000        	ldw	x,#32768
1738  023e 1f03          	ldw	(OFST-3,sp),x
1739  0240 ae0000        	ldw	x,#0
1740  0243 1f01          	ldw	(OFST-5,sp),x
1742  0245 200a          	jra	L517
1743  0247               L317:
1744                     ; 650         assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
1746                     ; 651         startaddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
1748  0247 ae4000        	ldw	x,#16384
1749  024a 1f03          	ldw	(OFST-3,sp),x
1750  024c ae0000        	ldw	x,#0
1751  024f 1f01          	ldw	(OFST-5,sp),x
1752  0251               L517:
1753                     ; 655     startaddress = startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE);
1755  0251 1e07          	ldw	x,(OFST+1,sp)
1756  0253 a640          	ld	a,#64
1757  0255 cd0000        	call	c_cmulx
1759  0258 96            	ldw	x,sp
1760  0259 1c0001        	addw	x,#OFST-5
1761  025c cd0000        	call	c_lgadd
1763                     ; 658     if (FLASH_ProgMode == FLASH_PROGRAMMODE_STANDARD)
1765  025f 0d0c          	tnz	(OFST+6,sp)
1766  0261 260a          	jrne	L717
1767                     ; 661         FLASH->CR2 |= FLASH_CR2_PRG;
1769  0263 7210505b      	bset	20571,#0
1770                     ; 662         FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NPRG);
1772  0267 7211505c      	bres	20572,#0
1774  026b 2008          	jra	L127
1775  026d               L717:
1776                     ; 667         FLASH->CR2 |= FLASH_CR2_FPRG;
1778  026d 7218505b      	bset	20571,#4
1779                     ; 668         FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NFPRG);
1781  0271 7219505c      	bres	20572,#4
1782  0275               L127:
1783                     ; 672     for (Count = 0; Count < FLASH_BLOCK_SIZE; Count++)
1785  0275 5f            	clrw	x
1786  0276 1f05          	ldw	(OFST-1,sp),x
1787  0278               L327:
1788                     ; 678   *((PointerAttr uint8_t*) (uint16_t)startaddress + Count) = ((uint8_t)(Buffer[Count]));
1790  0278 1e0d          	ldw	x,(OFST+7,sp)
1791  027a 72fb05        	addw	x,(OFST-1,sp)
1792  027d f6            	ld	a,(x)
1793  027e 1e03          	ldw	x,(OFST-3,sp)
1794  0280 72fb05        	addw	x,(OFST-1,sp)
1795  0283 f7            	ld	(x),a
1796                     ; 672     for (Count = 0; Count < FLASH_BLOCK_SIZE; Count++)
1798  0284 1e05          	ldw	x,(OFST-1,sp)
1799  0286 1c0001        	addw	x,#1
1800  0289 1f05          	ldw	(OFST-1,sp),x
1803  028b 1e05          	ldw	x,(OFST-1,sp)
1804  028d a30040        	cpw	x,#64
1805  0290 25e6          	jrult	L327
1806                     ; 681 }
1809  0292 5b08          	addw	sp,#8
1810  0294 81            	ret
1823                     	xdef	_FLASH_WaitForLastOperation
1824                     	xdef	_FLASH_ProgramBlock
1825                     	xdef	_FLASH_EraseBlock
1826                     	xdef	_FLASH_GetFlagStatus
1827                     	xdef	_FLASH_GetBootSize
1828                     	xdef	_FLASH_GetProgrammingTime
1829                     	xdef	_FLASH_GetLowPowerMode
1830                     	xdef	_FLASH_SetProgrammingTime
1831                     	xdef	_FLASH_SetLowPowerMode
1832                     	xdef	_FLASH_EraseOptionByte
1833                     	xdef	_FLASH_ProgramOptionByte
1834                     	xdef	_FLASH_ReadOptionByte
1835                     	xdef	_FLASH_ProgramWord
1836                     	xdef	_FLASH_ReadByte
1837                     	xdef	_FLASH_ProgramByte
1838                     	xdef	_FLASH_EraseByte
1839                     	xdef	_FLASH_ITConfig
1840                     	xdef	_FLASH_DeInit
1841                     	xdef	_FLASH_Lock
1842                     	xdef	_FLASH_Unlock
1843                     	xref.b	c_lreg
1862                     	xref	c_ladd
1863                     	xref	c_cmulx
1864                     	xref	c_lzmp
1865                     	xref	c_lgsbc
1866                     	xref	c_ltor
1867                     	xref	c_lgadd
1868                     	xref	c_rtol
1869                     	xref	c_umul
1870                     	end
