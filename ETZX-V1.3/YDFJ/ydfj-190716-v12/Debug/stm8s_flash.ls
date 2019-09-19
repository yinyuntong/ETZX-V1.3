   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  64                     ; 81 void FLASH_Unlock(FLASH_MemType_TypeDef FLASH_MemType)
  64                     ; 82 {
  66                     .text:	section	.text,new
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
 132                     .text:	section	.text,new
 133  0000               _FLASH_Lock:
 137                     ; 109     assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
 139                     ; 112   FLASH->IAPSR &= (uint8_t)FLASH_MemType;
 141  0000 c4505f        	and	a,20575
 142  0003 c7505f        	ld	20575,a
 143                     ; 113 }
 146  0006 81            	ret
 169                     ; 120 void FLASH_DeInit(void)
 169                     ; 121 {
 170                     .text:	section	.text,new
 171  0000               _FLASH_DeInit:
 175                     ; 122     FLASH->CR1 = FLASH_CR1_RESET_VALUE;
 177  0000 725f505a      	clr	20570
 178                     ; 123     FLASH->CR2 = FLASH_CR2_RESET_VALUE;
 180  0004 725f505b      	clr	20571
 181                     ; 124     FLASH->NCR2 = FLASH_NCR2_RESET_VALUE;
 183  0008 35ff505c      	mov	20572,#255
 184                     ; 125     FLASH->IAPSR &= (uint8_t)(~FLASH_IAPSR_DUL);
 186  000c 7217505f      	bres	20575,#3
 187                     ; 126     FLASH->IAPSR &= (uint8_t)(~FLASH_IAPSR_PUL);
 189  0010 7213505f      	bres	20575,#1
 190                     ; 127     (void) FLASH->IAPSR; /* Reading of this register causes the clearing of status flags */
 192  0014 c6505f        	ld	a,20575
 193  0017 97            	ld	xl,a
 194                     ; 128 }
 197  0018 81            	ret
 252                     ; 136 void FLASH_ITConfig(FunctionalState NewState)
 252                     ; 137 {
 253                     .text:	section	.text,new
 254  0000               _FLASH_ITConfig:
 258                     ; 139   assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 260                     ; 141     if (NewState != DISABLE)
 262  0000 4d            	tnz	a
 263  0001 2706          	jreq	L711
 264                     ; 143         FLASH->CR1 |= FLASH_CR1_IE; /* Enables the interrupt sources */
 266  0003 7212505a      	bset	20570,#1
 268  0007 2004          	jra	L121
 269  0009               L711:
 270                     ; 147         FLASH->CR1 &= (uint8_t)(~FLASH_CR1_IE); /* Disables the interrupt sources */
 272  0009 7213505a      	bres	20570,#1
 273  000d               L121:
 274                     ; 149 }
 277  000d 81            	ret
 311                     ; 158 void FLASH_EraseByte(uint32_t Address)
 311                     ; 159 {
 312                     .text:	section	.text,new
 313  0000               _FLASH_EraseByte:
 315       00000000      OFST:	set	0
 318                     ; 161     assert_param(IS_FLASH_ADDRESS_OK(Address));
 320                     ; 164    *(PointerAttr uint8_t*) (uint16_t)Address = FLASH_CLEAR_BYTE; 
 322  0000 1e05          	ldw	x,(OFST+5,sp)
 323  0002 7f            	clr	(x)
 324                     ; 166 }
 327  0003 81            	ret
 370                     ; 176 void FLASH_ProgramByte(uint32_t Address, uint8_t Data)
 370                     ; 177 {
 371                     .text:	section	.text,new
 372  0000               _FLASH_ProgramByte:
 374       00000000      OFST:	set	0
 377                     ; 179     assert_param(IS_FLASH_ADDRESS_OK(Address));
 379                     ; 180     *(PointerAttr uint8_t*) (uint16_t)Address = Data;
 381  0000 7b07          	ld	a,(OFST+7,sp)
 382  0002 1e05          	ldw	x,(OFST+5,sp)
 383  0004 f7            	ld	(x),a
 384                     ; 181 }
 387  0005 81            	ret
 421                     ; 190 uint8_t FLASH_ReadByte(uint32_t Address)
 421                     ; 191 {
 422                     .text:	section	.text,new
 423  0000               _FLASH_ReadByte:
 425       00000000      OFST:	set	0
 428                     ; 193     assert_param(IS_FLASH_ADDRESS_OK(Address));
 430                     ; 196     return(*(PointerAttr uint8_t *) (uint16_t)Address); 
 432  0000 1e05          	ldw	x,(OFST+5,sp)
 433  0002 f6            	ld	a,(x)
 436  0003 81            	ret
 479                     ; 207 void FLASH_ProgramWord(uint32_t Address, uint32_t Data)
 479                     ; 208 {
 480                     .text:	section	.text,new
 481  0000               _FLASH_ProgramWord:
 483       00000000      OFST:	set	0
 486                     ; 210     assert_param(IS_FLASH_ADDRESS_OK(Address));
 488                     ; 213     FLASH->CR2 |= FLASH_CR2_WPRG;
 490  0000 721c505b      	bset	20571,#6
 491                     ; 214     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NWPRG);
 493  0004 721d505c      	bres	20572,#6
 494                     ; 217     *((PointerAttr uint8_t*)(uint16_t)Address)       = *((uint8_t*)(&Data));
 496  0008 7b07          	ld	a,(OFST+7,sp)
 497  000a 1e05          	ldw	x,(OFST+5,sp)
 498  000c f7            	ld	(x),a
 499                     ; 219     *(((PointerAttr uint8_t*)(uint16_t)Address) + 1) = *((uint8_t*)(&Data)+1); 
 501  000d 7b08          	ld	a,(OFST+8,sp)
 502  000f 1e05          	ldw	x,(OFST+5,sp)
 503  0011 e701          	ld	(1,x),a
 504                     ; 221     *(((PointerAttr uint8_t*)(uint16_t)Address) + 2) = *((uint8_t*)(&Data)+2); 
 506  0013 7b09          	ld	a,(OFST+9,sp)
 507  0015 1e05          	ldw	x,(OFST+5,sp)
 508  0017 e702          	ld	(2,x),a
 509                     ; 223     *(((PointerAttr uint8_t*)(uint16_t)Address) + 3) = *((uint8_t*)(&Data)+3); 
 511  0019 7b0a          	ld	a,(OFST+10,sp)
 512  001b 1e05          	ldw	x,(OFST+5,sp)
 513  001d e703          	ld	(3,x),a
 514                     ; 224 }
 517  001f 81            	ret
 562                     ; 232 void FLASH_ProgramOptionByte(uint16_t Address, uint8_t Data)
 562                     ; 233 {
 563                     .text:	section	.text,new
 564  0000               _FLASH_ProgramOptionByte:
 566  0000 89            	pushw	x
 567       00000000      OFST:	set	0
 570                     ; 235     assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
 572                     ; 238     FLASH->CR2 |= FLASH_CR2_OPT;
 574  0001 721e505b      	bset	20571,#7
 575                     ; 239     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);
 577  0005 721f505c      	bres	20572,#7
 578                     ; 242     if (Address == 0x4800)
 580  0009 a34800        	cpw	x,#18432
 581  000c 2607          	jrne	L542
 582                     ; 245        *((NEAR uint8_t*)Address) = Data;
 584  000e 7b05          	ld	a,(OFST+5,sp)
 585  0010 1e01          	ldw	x,(OFST+1,sp)
 586  0012 f7            	ld	(x),a
 588  0013 200c          	jra	L742
 589  0015               L542:
 590                     ; 250        *((NEAR uint8_t*)Address) = Data;
 592  0015 7b05          	ld	a,(OFST+5,sp)
 593  0017 1e01          	ldw	x,(OFST+1,sp)
 594  0019 f7            	ld	(x),a
 595                     ; 251        *((NEAR uint8_t*)((uint16_t)(Address + 1))) = (uint8_t)(~Data);
 597  001a 7b05          	ld	a,(OFST+5,sp)
 598  001c 43            	cpl	a
 599  001d 1e01          	ldw	x,(OFST+1,sp)
 600  001f e701          	ld	(1,x),a
 601  0021               L742:
 602                     ; 253     FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
 604  0021 a6fd          	ld	a,#253
 605  0023 cd0000        	call	_FLASH_WaitForLastOperation
 607                     ; 256     FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
 609  0026 721f505b      	bres	20571,#7
 610                     ; 257     FLASH->NCR2 |= FLASH_NCR2_NOPT;
 612  002a 721e505c      	bset	20572,#7
 613                     ; 258 }
 616  002e 85            	popw	x
 617  002f 81            	ret
 653                     ; 265 void FLASH_EraseOptionByte(uint16_t Address)
 653                     ; 266 {
 654                     .text:	section	.text,new
 655  0000               _FLASH_EraseOptionByte:
 657  0000 89            	pushw	x
 658       00000000      OFST:	set	0
 661                     ; 268     assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
 663                     ; 271     FLASH->CR2 |= FLASH_CR2_OPT;
 665  0001 721e505b      	bset	20571,#7
 666                     ; 272     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);
 668  0005 721f505c      	bres	20572,#7
 669                     ; 275      if (Address == 0x4800)
 671  0009 a34800        	cpw	x,#18432
 672  000c 2603          	jrne	L762
 673                     ; 278        *((NEAR uint8_t*)Address) = FLASH_CLEAR_BYTE;
 675  000e 7f            	clr	(x)
 677  000f 2009          	jra	L172
 678  0011               L762:
 679                     ; 283        *((NEAR uint8_t*)Address) = FLASH_CLEAR_BYTE;
 681  0011 1e01          	ldw	x,(OFST+1,sp)
 682  0013 7f            	clr	(x)
 683                     ; 284        *((NEAR uint8_t*)((uint16_t)(Address + (uint16_t)1 ))) = FLASH_SET_BYTE;
 685  0014 1e01          	ldw	x,(OFST+1,sp)
 686  0016 a6ff          	ld	a,#255
 687  0018 e701          	ld	(1,x),a
 688  001a               L172:
 689                     ; 286     FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
 691  001a a6fd          	ld	a,#253
 692  001c cd0000        	call	_FLASH_WaitForLastOperation
 694                     ; 289     FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
 696  001f 721f505b      	bres	20571,#7
 697                     ; 290     FLASH->NCR2 |= FLASH_NCR2_NOPT;
 699  0023 721e505c      	bset	20572,#7
 700                     ; 291 }
 703  0027 85            	popw	x
 704  0028 81            	ret
 767                     ; 297 uint16_t FLASH_ReadOptionByte(uint16_t Address)
 767                     ; 298 {
 768                     .text:	section	.text,new
 769  0000               _FLASH_ReadOptionByte:
 771  0000 89            	pushw	x
 772  0001 5204          	subw	sp,#4
 773       00000004      OFST:	set	4
 776                     ; 299     uint8_t value_optbyte, value_optbyte_complement = 0;
 778  0003 7b01          	ld	a,(OFST-3,sp)
 779  0005 97            	ld	xl,a
 780                     ; 300     uint16_t res_value = 0;
 782  0006 1e03          	ldw	x,(OFST-1,sp)
 783                     ; 303     assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
 785                     ; 306     value_optbyte = *((NEAR uint8_t*)Address); /* Read option byte */
 787  0008 1e05          	ldw	x,(OFST+1,sp)
 788  000a f6            	ld	a,(x)
 789  000b 6b02          	ld	(OFST-2,sp),a
 790                     ; 307     value_optbyte_complement = *(((NEAR uint8_t*)Address) + 1); /* Read option byte complement */
 792  000d 1e05          	ldw	x,(OFST+1,sp)
 793  000f e601          	ld	a,(1,x)
 794  0011 6b01          	ld	(OFST-3,sp),a
 795                     ; 310     if (Address == 0x4800)	 
 797  0013 1e05          	ldw	x,(OFST+1,sp)
 798  0015 a34800        	cpw	x,#18432
 799  0018 2608          	jrne	L523
 800                     ; 312         res_value =	 value_optbyte;
 802  001a 7b02          	ld	a,(OFST-2,sp)
 803  001c 5f            	clrw	x
 804  001d 97            	ld	xl,a
 805  001e 1f03          	ldw	(OFST-1,sp),x
 807  0020 2023          	jra	L723
 808  0022               L523:
 809                     ; 316         if (value_optbyte == (uint8_t)(~value_optbyte_complement))
 811  0022 7b01          	ld	a,(OFST-3,sp)
 812  0024 43            	cpl	a
 813  0025 1102          	cp	a,(OFST-2,sp)
 814  0027 2617          	jrne	L133
 815                     ; 318             res_value = (uint16_t)((uint16_t)value_optbyte << 8);
 817  0029 7b02          	ld	a,(OFST-2,sp)
 818  002b 5f            	clrw	x
 819  002c 97            	ld	xl,a
 820  002d 4f            	clr	a
 821  002e 02            	rlwa	x,a
 822  002f 1f03          	ldw	(OFST-1,sp),x
 823                     ; 319             res_value = res_value | (uint16_t)value_optbyte_complement;
 825  0031 7b01          	ld	a,(OFST-3,sp)
 826  0033 5f            	clrw	x
 827  0034 97            	ld	xl,a
 828  0035 01            	rrwa	x,a
 829  0036 1a04          	or	a,(OFST+0,sp)
 830  0038 01            	rrwa	x,a
 831  0039 1a03          	or	a,(OFST-1,sp)
 832  003b 01            	rrwa	x,a
 833  003c 1f03          	ldw	(OFST-1,sp),x
 835  003e 2005          	jra	L723
 836  0040               L133:
 837                     ; 323             res_value = FLASH_OPTIONBYTE_ERROR;
 839  0040 ae5555        	ldw	x,#21845
 840  0043 1f03          	ldw	(OFST-1,sp),x
 841  0045               L723:
 842                     ; 326     return(res_value);
 844  0045 1e03          	ldw	x,(OFST-1,sp)
 847  0047 5b06          	addw	sp,#6
 848  0049 81            	ret
 922                     ; 335 void FLASH_SetLowPowerMode(FLASH_LPMode_TypeDef FLASH_LPMode)
 922                     ; 336 {
 923                     .text:	section	.text,new
 924  0000               _FLASH_SetLowPowerMode:
 926  0000 88            	push	a
 927       00000000      OFST:	set	0
 930                     ; 338     assert_param(IS_FLASH_LOW_POWER_MODE_OK(FLASH_LPMode));
 932                     ; 341     FLASH->CR1 &= (uint8_t)(~(FLASH_CR1_HALT | FLASH_CR1_AHALT)); 
 934  0001 c6505a        	ld	a,20570
 935  0004 a4f3          	and	a,#243
 936  0006 c7505a        	ld	20570,a
 937                     ; 344     FLASH->CR1 |= (uint8_t)FLASH_LPMode; 
 939  0009 c6505a        	ld	a,20570
 940  000c 1a01          	or	a,(OFST+1,sp)
 941  000e c7505a        	ld	20570,a
 942                     ; 345 }
 945  0011 84            	pop	a
 946  0012 81            	ret
1004                     ; 353 void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef FLASH_ProgTime)
1004                     ; 354 {
1005                     .text:	section	.text,new
1006  0000               _FLASH_SetProgrammingTime:
1010                     ; 356     assert_param(IS_FLASH_PROGRAM_TIME_OK(FLASH_ProgTime));
1012                     ; 358     FLASH->CR1 &= (uint8_t)(~FLASH_CR1_FIX);
1014  0000 7211505a      	bres	20570,#0
1015                     ; 359     FLASH->CR1 |= (uint8_t)FLASH_ProgTime;
1017  0004 ca505a        	or	a,20570
1018  0007 c7505a        	ld	20570,a
1019                     ; 360 }
1022  000a 81            	ret
1047                     ; 367 FLASH_LPMode_TypeDef FLASH_GetLowPowerMode(void)
1047                     ; 368 {
1048                     .text:	section	.text,new
1049  0000               _FLASH_GetLowPowerMode:
1053                     ; 369     return((FLASH_LPMode_TypeDef)(FLASH->CR1 & (uint8_t)(FLASH_CR1_HALT | FLASH_CR1_AHALT)));
1055  0000 c6505a        	ld	a,20570
1056  0003 a40c          	and	a,#12
1059  0005 81            	ret
1084                     ; 377 FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void)
1084                     ; 378 {
1085                     .text:	section	.text,new
1086  0000               _FLASH_GetProgrammingTime:
1090                     ; 379     return((FLASH_ProgramTime_TypeDef)(FLASH->CR1 & FLASH_CR1_FIX));
1092  0000 c6505a        	ld	a,20570
1093  0003 a401          	and	a,#1
1096  0005 81            	ret
1130                     ; 387 uint32_t FLASH_GetBootSize(void)
1130                     ; 388 {
1131                     .text:	section	.text,new
1132  0000               _FLASH_GetBootSize:
1134  0000 5204          	subw	sp,#4
1135       00000004      OFST:	set	4
1138                     ; 389     uint32_t temp = 0;
1140  0002 ae0000        	ldw	x,#0
1141  0005 1f03          	ldw	(OFST-1,sp),x
1142  0007 ae0000        	ldw	x,#0
1143  000a 1f01          	ldw	(OFST-3,sp),x
1144                     ; 392     temp = (uint32_t)((uint32_t)FLASH->FPR * (uint32_t)512);
1146  000c c6505d        	ld	a,20573
1147  000f 5f            	clrw	x
1148  0010 97            	ld	xl,a
1149  0011 90ae0200      	ldw	y,#512
1150  0015 cd0000        	call	c_umul
1152  0018 96            	ldw	x,sp
1153  0019 1c0001        	addw	x,#OFST-3
1154  001c cd0000        	call	c_rtol
1156                     ; 395     if (FLASH->FPR == 0xFF)
1158  001f c6505d        	ld	a,20573
1159  0022 a1ff          	cp	a,#255
1160  0024 2611          	jrne	L354
1161                     ; 397         temp += 512;
1163  0026 ae0200        	ldw	x,#512
1164  0029 bf02          	ldw	c_lreg+2,x
1165  002b ae0000        	ldw	x,#0
1166  002e bf00          	ldw	c_lreg,x
1167  0030 96            	ldw	x,sp
1168  0031 1c0001        	addw	x,#OFST-3
1169  0034 cd0000        	call	c_lgadd
1171  0037               L354:
1172                     ; 401     return(temp);
1174  0037 96            	ldw	x,sp
1175  0038 1c0001        	addw	x,#OFST-3
1176  003b cd0000        	call	c_ltor
1180  003e 5b04          	addw	sp,#4
1181  0040 81            	ret
1283                     ; 412 FlagStatus FLASH_GetFlagStatus(FLASH_Flag_TypeDef FLASH_FLAG)
1283                     ; 413 {
1284                     .text:	section	.text,new
1285  0000               _FLASH_GetFlagStatus:
1287  0000 88            	push	a
1288       00000001      OFST:	set	1
1291                     ; 414     FlagStatus status = RESET;
1293  0001 0f01          	clr	(OFST+0,sp)
1294                     ; 416     assert_param(IS_FLASH_FLAGS_OK(FLASH_FLAG));
1296                     ; 419     if ((FLASH->IAPSR & (uint8_t)FLASH_FLAG) != (uint8_t)RESET)
1298  0003 c4505f        	and	a,20575
1299  0006 2706          	jreq	L325
1300                     ; 421         status = SET; /* FLASH_FLAG is set */
1302  0008 a601          	ld	a,#1
1303  000a 6b01          	ld	(OFST+0,sp),a
1305  000c 2002          	jra	L525
1306  000e               L325:
1307                     ; 425         status = RESET; /* FLASH_FLAG is reset*/
1309  000e 0f01          	clr	(OFST+0,sp)
1310  0010               L525:
1311                     ; 429     return status;
1313  0010 7b01          	ld	a,(OFST+0,sp)
1316  0012 5b01          	addw	sp,#1
1317  0014 81            	ret
1402                     ; 531 IN_RAM(FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType)) 
1402                     ; 532 {
1403                     .text:	section	.text,new
1404  0000               _FLASH_WaitForLastOperation:
1406  0000 5205          	subw	sp,#5
1407       00000005      OFST:	set	5
1410                     ; 533     uint8_t flagstatus = 0x00;
1412  0002 0f05          	clr	(OFST+0,sp)
1413                     ; 534     uint32_t timeout = OPERATION_TIMEOUT;
1415  0004 aeffff        	ldw	x,#65535
1416  0007 1f03          	ldw	(OFST-2,sp),x
1417  0009 ae000f        	ldw	x,#15
1418  000c 1f01          	ldw	(OFST-4,sp),x
1420  000e 2010          	jra	L375
1421  0010               L765:
1422                     ; 560         flagstatus = (uint8_t)(FLASH->IAPSR & (FLASH_IAPSR_EOP | FLASH_IAPSR_WR_PG_DIS));
1424  0010 c6505f        	ld	a,20575
1425  0013 a405          	and	a,#5
1426  0015 6b05          	ld	(OFST+0,sp),a
1427                     ; 561         timeout--;
1429  0017 96            	ldw	x,sp
1430  0018 1c0001        	addw	x,#OFST-4
1431  001b a601          	ld	a,#1
1432  001d cd0000        	call	c_lgsbc
1434  0020               L375:
1435                     ; 558     while ((flagstatus == 0x00) && (timeout != 0x00))
1437  0020 0d05          	tnz	(OFST+0,sp)
1438  0022 2609          	jrne	L775
1440  0024 96            	ldw	x,sp
1441  0025 1c0001        	addw	x,#OFST-4
1442  0028 cd0000        	call	c_lzmp
1444  002b 26e3          	jrne	L765
1445  002d               L775:
1446                     ; 566     if (timeout == 0x00 )
1448  002d 96            	ldw	x,sp
1449  002e 1c0001        	addw	x,#OFST-4
1450  0031 cd0000        	call	c_lzmp
1452  0034 2604          	jrne	L106
1453                     ; 568         flagstatus = FLASH_STATUS_TIMEOUT;
1455  0036 a602          	ld	a,#2
1456  0038 6b05          	ld	(OFST+0,sp),a
1457  003a               L106:
1458                     ; 571     return((FLASH_Status_TypeDef)flagstatus);
1460  003a 7b05          	ld	a,(OFST+0,sp)
1463  003c 5b05          	addw	sp,#5
1464  003e 81            	ret
1527                     ; 581 IN_RAM(void FLASH_EraseBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType))
1527                     ; 582 {
1528                     .text:	section	.text,new
1529  0000               _FLASH_EraseBlock:
1531  0000 89            	pushw	x
1532  0001 5206          	subw	sp,#6
1533       00000006      OFST:	set	6
1536                     ; 583   uint32_t startaddress = 0;
1538  0003 96            	ldw	x,sp
1539  0004 1c0003        	addw	x,#OFST-3
1540  0007 cd0000        	call	c_ltor
1542                     ; 592   assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
1544                     ; 593   if (FLASH_MemType == FLASH_MEMTYPE_PROG)
1546  000a 7b0b          	ld	a,(OFST+5,sp)
1547  000c a1fd          	cp	a,#253
1548  000e 260c          	jrne	L536
1549                     ; 595       assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
1551                     ; 596       startaddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
1553  0010 ae8000        	ldw	x,#32768
1554  0013 1f05          	ldw	(OFST-1,sp),x
1555  0015 ae0000        	ldw	x,#0
1556  0018 1f03          	ldw	(OFST-3,sp),x
1558  001a 200a          	jra	L736
1559  001c               L536:
1560                     ; 600       assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
1562                     ; 601       startaddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
1564  001c ae4000        	ldw	x,#16384
1565  001f 1f05          	ldw	(OFST-1,sp),x
1566  0021 ae0000        	ldw	x,#0
1567  0024 1f03          	ldw	(OFST-3,sp),x
1568  0026               L736:
1569                     ; 608     pwFlash = (PointerAttr uint32_t *)(uint16_t)(startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE));
1571  0026 1e07          	ldw	x,(OFST+1,sp)
1572  0028 a640          	ld	a,#64
1573  002a cd0000        	call	c_cmulx
1575  002d 96            	ldw	x,sp
1576  002e 1c0003        	addw	x,#OFST-3
1577  0031 cd0000        	call	c_ladd
1579  0034 be02          	ldw	x,c_lreg+2
1580  0036 1f01          	ldw	(OFST-5,sp),x
1581                     ; 612     FLASH->CR2 |= FLASH_CR2_ERASE;
1583  0038 721a505b      	bset	20571,#5
1584                     ; 613     FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NERASE);
1586  003c 721b505c      	bres	20572,#5
1587                     ; 616     *pwFlash = (uint32_t)0;
1589  0040 1e01          	ldw	x,(OFST-5,sp)
1590  0042 a600          	ld	a,#0
1591  0044 e703          	ld	(3,x),a
1592  0046 a600          	ld	a,#0
1593  0048 e702          	ld	(2,x),a
1594  004a a600          	ld	a,#0
1595  004c e701          	ld	(1,x),a
1596  004e a600          	ld	a,#0
1597  0050 f7            	ld	(x),a
1598                     ; 623 }
1601  0051 5b08          	addw	sp,#8
1602  0053 81            	ret
1706                     ; 634 IN_RAM(void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType, 
1706                     ; 635                         FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer))
1706                     ; 636 {
1707                     .text:	section	.text,new
1708  0000               _FLASH_ProgramBlock:
1710  0000 89            	pushw	x
1711  0001 5206          	subw	sp,#6
1712       00000006      OFST:	set	6
1715                     ; 637     uint16_t Count = 0;
1717  0003 1e05          	ldw	x,(OFST-1,sp)
1718                     ; 638     uint32_t startaddress = 0;
1720  0005 96            	ldw	x,sp
1721  0006 1c0001        	addw	x,#OFST-5
1722  0009 cd0000        	call	c_ltor
1724                     ; 641     assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
1726                     ; 642     assert_param(IS_FLASH_PROGRAM_MODE_OK(FLASH_ProgMode));
1728                     ; 643     if (FLASH_MemType == FLASH_MEMTYPE_PROG)
1730  000c 7b0b          	ld	a,(OFST+5,sp)
1731  000e a1fd          	cp	a,#253
1732  0010 260c          	jrne	L317
1733                     ; 645         assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
1735                     ; 646         startaddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
1737  0012 ae8000        	ldw	x,#32768
1738  0015 1f03          	ldw	(OFST-3,sp),x
1739  0017 ae0000        	ldw	x,#0
1740  001a 1f01          	ldw	(OFST-5,sp),x
1742  001c 200a          	jra	L517
1743  001e               L317:
1744                     ; 650         assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
1746                     ; 651         startaddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
1748  001e ae4000        	ldw	x,#16384
1749  0021 1f03          	ldw	(OFST-3,sp),x
1750  0023 ae0000        	ldw	x,#0
1751  0026 1f01          	ldw	(OFST-5,sp),x
1752  0028               L517:
1753                     ; 655     startaddress = startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE);
1755  0028 1e07          	ldw	x,(OFST+1,sp)
1756  002a a640          	ld	a,#64
1757  002c cd0000        	call	c_cmulx
1759  002f 96            	ldw	x,sp
1760  0030 1c0001        	addw	x,#OFST-5
1761  0033 cd0000        	call	c_lgadd
1763                     ; 658     if (FLASH_ProgMode == FLASH_PROGRAMMODE_STANDARD)
1765  0036 0d0c          	tnz	(OFST+6,sp)
1766  0038 260a          	jrne	L717
1767                     ; 661         FLASH->CR2 |= FLASH_CR2_PRG;
1769  003a 7210505b      	bset	20571,#0
1770                     ; 662         FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NPRG);
1772  003e 7211505c      	bres	20572,#0
1774  0042 2008          	jra	L127
1775  0044               L717:
1776                     ; 667         FLASH->CR2 |= FLASH_CR2_FPRG;
1778  0044 7218505b      	bset	20571,#4
1779                     ; 668         FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NFPRG);
1781  0048 7219505c      	bres	20572,#4
1782  004c               L127:
1783                     ; 672     for (Count = 0; Count < FLASH_BLOCK_SIZE; Count++)
1785  004c 5f            	clrw	x
1786  004d 1f05          	ldw	(OFST-1,sp),x
1787  004f               L327:
1788                     ; 678   *((PointerAttr uint8_t*) (uint16_t)startaddress + Count) = ((uint8_t)(Buffer[Count]));
1790  004f 1e0d          	ldw	x,(OFST+7,sp)
1791  0051 72fb05        	addw	x,(OFST-1,sp)
1792  0054 f6            	ld	a,(x)
1793  0055 1e03          	ldw	x,(OFST-3,sp)
1794  0057 72fb05        	addw	x,(OFST-1,sp)
1795  005a f7            	ld	(x),a
1796                     ; 672     for (Count = 0; Count < FLASH_BLOCK_SIZE; Count++)
1798  005b 1e05          	ldw	x,(OFST-1,sp)
1799  005d 1c0001        	addw	x,#1
1800  0060 1f05          	ldw	(OFST-1,sp),x
1803  0062 1e05          	ldw	x,(OFST-1,sp)
1804  0064 a30040        	cpw	x,#64
1805  0067 25e6          	jrult	L327
1806                     ; 681 }
1809  0069 5b08          	addw	sp,#8
1810  006b 81            	ret
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
