   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 44 void SPI_DeInit(void)
  32                     ; 45 {
  34                     .text:	section	.text,new
  35  0000               _SPI_DeInit:
  39                     ; 46     SPI->CR1    = SPI_CR1_RESET_VALUE;
  41  0000 725f5200      	clr	20992
  42                     ; 47     SPI->CR2    = SPI_CR2_RESET_VALUE;
  44  0004 725f5201      	clr	20993
  45                     ; 48     SPI->ICR    = SPI_ICR_RESET_VALUE;
  47  0008 725f5202      	clr	20994
  48                     ; 49     SPI->SR     = SPI_SR_RESET_VALUE;
  50  000c 35025203      	mov	20995,#2
  51                     ; 50     SPI->CRCPR  = SPI_CRCPR_RESET_VALUE;
  53  0010 35075205      	mov	20997,#7
  54                     ; 51 }
  57  0014 81            	ret
 373                     ; 72 void SPI_Init(SPI_FirstBit_TypeDef FirstBit, SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, SPI_Mode_TypeDef Mode, SPI_ClockPolarity_TypeDef ClockPolarity, SPI_ClockPhase_TypeDef ClockPhase, SPI_DataDirection_TypeDef Data_Direction, SPI_NSS_TypeDef Slave_Management, uint8_t CRCPolynomial)
 373                     ; 73 {
 374                     .text:	section	.text,new
 375  0000               _SPI_Init:
 377  0000 89            	pushw	x
 378  0001 88            	push	a
 379       00000001      OFST:	set	1
 382                     ; 75     assert_param(IS_SPI_FIRSTBIT_OK(FirstBit));
 384                     ; 76     assert_param(IS_SPI_BAUDRATE_PRESCALER_OK(BaudRatePrescaler));
 386                     ; 77     assert_param(IS_SPI_MODE_OK(Mode));
 388                     ; 78     assert_param(IS_SPI_POLARITY_OK(ClockPolarity));
 390                     ; 79     assert_param(IS_SPI_PHASE_OK(ClockPhase));
 392                     ; 80     assert_param(IS_SPI_DATA_DIRECTION_OK(Data_Direction));
 394                     ; 81     assert_param(IS_SPI_SLAVEMANAGEMENT_OK(Slave_Management));
 396                     ; 82     assert_param(IS_SPI_CRC_POLYNOMIAL_OK(CRCPolynomial));
 398                     ; 85     SPI->CR1 = (uint8_t)((uint8_t)((uint8_t)FirstBit | BaudRatePrescaler) |
 398                     ; 86                     (uint8_t)((uint8_t)ClockPolarity | ClockPhase));
 400  0002 7b07          	ld	a,(OFST+6,sp)
 401  0004 1a08          	or	a,(OFST+7,sp)
 402  0006 6b01          	ld	(OFST+0,sp),a
 403  0008 9f            	ld	a,xl
 404  0009 1a02          	or	a,(OFST+1,sp)
 405  000b 1a01          	or	a,(OFST+0,sp)
 406  000d c75200        	ld	20992,a
 407                     ; 89     SPI->CR2 = (uint8_t)((uint8_t)(Data_Direction) | (uint8_t)(Slave_Management));
 409  0010 7b09          	ld	a,(OFST+8,sp)
 410  0012 1a0a          	or	a,(OFST+9,sp)
 411  0014 c75201        	ld	20993,a
 412                     ; 91     if (Mode == SPI_MODE_MASTER)
 414  0017 7b06          	ld	a,(OFST+5,sp)
 415  0019 a104          	cp	a,#4
 416  001b 2606          	jrne	L302
 417                     ; 93         SPI->CR2 |= (uint8_t)SPI_CR2_SSI;
 419  001d 72105201      	bset	20993,#0
 421  0021 2004          	jra	L502
 422  0023               L302:
 423                     ; 97         SPI->CR2 &= (uint8_t)~(SPI_CR2_SSI);
 425  0023 72115201      	bres	20993,#0
 426  0027               L502:
 427                     ; 101     SPI->CR1 |= (uint8_t)(Mode);
 429  0027 c65200        	ld	a,20992
 430  002a 1a06          	or	a,(OFST+5,sp)
 431  002c c75200        	ld	20992,a
 432                     ; 104     SPI->CRCPR = (uint8_t)CRCPolynomial;
 434  002f 7b0b          	ld	a,(OFST+10,sp)
 435  0031 c75205        	ld	20997,a
 436                     ; 105 }
 439  0034 5b03          	addw	sp,#3
 440  0036 81            	ret
 495                     ; 113 void SPI_Cmd(FunctionalState NewState)
 495                     ; 114 {
 496                     .text:	section	.text,new
 497  0000               _SPI_Cmd:
 501                     ; 116     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 503                     ; 118     if (NewState != DISABLE)
 505  0000 4d            	tnz	a
 506  0001 2706          	jreq	L532
 507                     ; 120         SPI->CR1 |= SPI_CR1_SPE; /* Enable the SPI peripheral*/
 509  0003 721c5200      	bset	20992,#6
 511  0007 2004          	jra	L732
 512  0009               L532:
 513                     ; 124         SPI->CR1 &= (uint8_t)(~SPI_CR1_SPE); /* Disable the SPI peripheral*/
 515  0009 721d5200      	bres	20992,#6
 516  000d               L732:
 517                     ; 126 }
 520  000d 81            	ret
 629                     ; 135 void SPI_ITConfig(SPI_IT_TypeDef SPI_IT, FunctionalState NewState)
 629                     ; 136 {
 630                     .text:	section	.text,new
 631  0000               _SPI_ITConfig:
 633  0000 89            	pushw	x
 634  0001 88            	push	a
 635       00000001      OFST:	set	1
 638                     ; 137     uint8_t itpos = 0;
 640  0002 0f01          	clr	(OFST+0,sp)
 641                     ; 139     assert_param(IS_SPI_CONFIG_IT_OK(SPI_IT));
 643                     ; 140     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 645                     ; 143     itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)SPI_IT & (uint8_t)0x0F));
 647  0004 9e            	ld	a,xh
 648  0005 a40f          	and	a,#15
 649  0007 5f            	clrw	x
 650  0008 97            	ld	xl,a
 651  0009 a601          	ld	a,#1
 652  000b 5d            	tnzw	x
 653  000c 2704          	jreq	L41
 654  000e               L61:
 655  000e 48            	sll	a
 656  000f 5a            	decw	x
 657  0010 26fc          	jrne	L61
 658  0012               L41:
 659  0012 6b01          	ld	(OFST+0,sp),a
 660                     ; 145     if (NewState != DISABLE)
 662  0014 0d03          	tnz	(OFST+2,sp)
 663  0016 270a          	jreq	L113
 664                     ; 147         SPI->ICR |= itpos; /* Enable interrupt*/
 666  0018 c65202        	ld	a,20994
 667  001b 1a01          	or	a,(OFST+0,sp)
 668  001d c75202        	ld	20994,a
 670  0020 2009          	jra	L313
 671  0022               L113:
 672                     ; 151         SPI->ICR &= (uint8_t)(~itpos); /* Disable interrupt*/
 674  0022 7b01          	ld	a,(OFST+0,sp)
 675  0024 43            	cpl	a
 676  0025 c45202        	and	a,20994
 677  0028 c75202        	ld	20994,a
 678  002b               L313:
 679                     ; 153 }
 682  002b 5b03          	addw	sp,#3
 683  002d 81            	ret
 717                     ; 159 void SPI_SendData(uint8_t Data)
 717                     ; 160 {
 718                     .text:	section	.text,new
 719  0000               _SPI_SendData:
 723                     ; 161     SPI->DR = Data; /* Write in the DR register the data to be sent*/
 725  0000 c75204        	ld	20996,a
 726                     ; 162 }
 729  0003 81            	ret
 752                     ; 169 uint8_t SPI_ReceiveData(void)
 752                     ; 170 {
 753                     .text:	section	.text,new
 754  0000               _SPI_ReceiveData:
 758                     ; 171     return ((uint8_t)SPI->DR); /* Return the data in the DR register*/
 760  0000 c65204        	ld	a,20996
 763  0003 81            	ret
 799                     ; 180 void SPI_NSSInternalSoftwareCmd(FunctionalState NewState)
 799                     ; 181 {
 800                     .text:	section	.text,new
 801  0000               _SPI_NSSInternalSoftwareCmd:
 805                     ; 183     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 807                     ; 185     if (NewState != DISABLE)
 809  0000 4d            	tnz	a
 810  0001 2706          	jreq	L163
 811                     ; 187         SPI->CR2 |= SPI_CR2_SSI; /* Set NSS pin internally by software*/
 813  0003 72105201      	bset	20993,#0
 815  0007 2004          	jra	L363
 816  0009               L163:
 817                     ; 191         SPI->CR2 &= (uint8_t)(~SPI_CR2_SSI); /* Reset NSS pin internally by software*/
 819  0009 72115201      	bres	20993,#0
 820  000d               L363:
 821                     ; 193 }
 824  000d 81            	ret
 847                     ; 200 void SPI_TransmitCRC(void)
 847                     ; 201 {
 848                     .text:	section	.text,new
 849  0000               _SPI_TransmitCRC:
 853                     ; 202     SPI->CR2 |= SPI_CR2_CRCNEXT; /* Enable the CRC transmission*/
 855  0000 72185201      	bset	20993,#4
 856                     ; 203 }
 859  0004 81            	ret
 895                     ; 211 void SPI_CalculateCRCCmd(FunctionalState NewState)
 895                     ; 212 {
 896                     .text:	section	.text,new
 897  0000               _SPI_CalculateCRCCmd:
 901                     ; 214     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 903                     ; 216     if (NewState != DISABLE)
 905  0000 4d            	tnz	a
 906  0001 2706          	jreq	L314
 907                     ; 218         SPI->CR2 |= SPI_CR2_CRCEN; /* Enable the CRC calculation*/
 909  0003 721a5201      	bset	20993,#5
 911  0007 2004          	jra	L514
 912  0009               L314:
 913                     ; 222         SPI->CR2 &= (uint8_t)(~SPI_CR2_CRCEN); /* Disable the CRC calculation*/
 915  0009 721b5201      	bres	20993,#5
 916  000d               L514:
 917                     ; 224 }
 920  000d 81            	ret
 984                     ; 231 uint8_t SPI_GetCRC(SPI_CRC_TypeDef SPI_CRC)
 984                     ; 232 {
 985                     .text:	section	.text,new
 986  0000               _SPI_GetCRC:
 988  0000 88            	push	a
 989       00000001      OFST:	set	1
 992                     ; 233     uint8_t crcreg = 0;
 994  0001 0f01          	clr	(OFST+0,sp)
 995                     ; 236     assert_param(IS_SPI_CRC_OK(SPI_CRC));
 997                     ; 238     if (SPI_CRC != SPI_CRC_RX)
 999  0003 4d            	tnz	a
1000  0004 2707          	jreq	L154
1001                     ; 240         crcreg = SPI->TXCRCR;  /* Get the Tx CRC register*/
1003  0006 c65207        	ld	a,20999
1004  0009 6b01          	ld	(OFST+0,sp),a
1006  000b 2005          	jra	L354
1007  000d               L154:
1008                     ; 244         crcreg = SPI->RXCRCR; /* Get the Rx CRC register*/
1010  000d c65206        	ld	a,20998
1011  0010 6b01          	ld	(OFST+0,sp),a
1012  0012               L354:
1013                     ; 248     return crcreg;
1015  0012 7b01          	ld	a,(OFST+0,sp)
1018  0014 5b01          	addw	sp,#1
1019  0016 81            	ret
1044                     ; 256 void SPI_ResetCRC(void)
1044                     ; 257 {
1045                     .text:	section	.text,new
1046  0000               _SPI_ResetCRC:
1050                     ; 260     SPI_CalculateCRCCmd(ENABLE);
1052  0000 a601          	ld	a,#1
1053  0002 cd0000        	call	_SPI_CalculateCRCCmd
1055                     ; 263     SPI_Cmd(ENABLE);
1057  0005 a601          	ld	a,#1
1058  0007 cd0000        	call	_SPI_Cmd
1060                     ; 264 }
1063  000a 81            	ret
1087                     ; 271 uint8_t SPI_GetCRCPolynomial(void)
1087                     ; 272 {
1088                     .text:	section	.text,new
1089  0000               _SPI_GetCRCPolynomial:
1093                     ; 273     return SPI->CRCPR; /* Return the CRC polynomial register */
1095  0000 c65205        	ld	a,20997
1098  0003 81            	ret
1154                     ; 281 void SPI_BiDirectionalLineConfig(SPI_Direction_TypeDef SPI_Direction)
1154                     ; 282 {
1155                     .text:	section	.text,new
1156  0000               _SPI_BiDirectionalLineConfig:
1160                     ; 284     assert_param(IS_SPI_DIRECTION_OK(SPI_Direction));
1162                     ; 286     if (SPI_Direction != SPI_DIRECTION_RX)
1164  0000 4d            	tnz	a
1165  0001 2706          	jreq	L325
1166                     ; 288         SPI->CR2 |= SPI_CR2_BDOE; /* Set the Tx only mode*/
1168  0003 721c5201      	bset	20993,#6
1170  0007 2004          	jra	L525
1171  0009               L325:
1172                     ; 292         SPI->CR2 &= (uint8_t)(~SPI_CR2_BDOE); /* Set the Rx only mode*/
1174  0009 721d5201      	bres	20993,#6
1175  000d               L525:
1176                     ; 294 }
1179  000d 81            	ret
1300                     ; 304 FlagStatus SPI_GetFlagStatus(SPI_Flag_TypeDef SPI_FLAG)
1300                     ; 305 {
1301                     .text:	section	.text,new
1302  0000               _SPI_GetFlagStatus:
1304  0000 88            	push	a
1305       00000001      OFST:	set	1
1308                     ; 306     FlagStatus status = RESET;
1310  0001 0f01          	clr	(OFST+0,sp)
1311                     ; 308     assert_param(IS_SPI_FLAGS_OK(SPI_FLAG));
1313                     ; 311     if ((SPI->SR & (uint8_t)SPI_FLAG) != (uint8_t)RESET)
1315  0003 c45203        	and	a,20995
1316  0006 2706          	jreq	L306
1317                     ; 313         status = SET; /* SPI_FLAG is set */
1319  0008 a601          	ld	a,#1
1320  000a 6b01          	ld	(OFST+0,sp),a
1322  000c 2002          	jra	L506
1323  000e               L306:
1324                     ; 317         status = RESET; /* SPI_FLAG is reset*/
1326  000e 0f01          	clr	(OFST+0,sp)
1327  0010               L506:
1328                     ; 321     return status;
1330  0010 7b01          	ld	a,(OFST+0,sp)
1333  0012 5b01          	addw	sp,#1
1334  0014 81            	ret
1369                     ; 339 void SPI_ClearFlag(SPI_Flag_TypeDef SPI_FLAG)
1369                     ; 340 {
1370                     .text:	section	.text,new
1371  0000               _SPI_ClearFlag:
1375                     ; 341     assert_param(IS_SPI_CLEAR_FLAGS_OK(SPI_FLAG));
1377                     ; 343     SPI->SR = (uint8_t)(~SPI_FLAG);
1379  0000 43            	cpl	a
1380  0001 c75203        	ld	20995,a
1381                     ; 344 }
1384  0004 81            	ret
1466                     ; 359 ITStatus SPI_GetITStatus(SPI_IT_TypeDef SPI_IT)
1466                     ; 360 {
1467                     .text:	section	.text,new
1468  0000               _SPI_GetITStatus:
1470  0000 88            	push	a
1471  0001 89            	pushw	x
1472       00000002      OFST:	set	2
1475                     ; 361     ITStatus pendingbitstatus = RESET;
1477  0002 7b02          	ld	a,(OFST+0,sp)
1478  0004 97            	ld	xl,a
1479                     ; 362     uint8_t itpos = 0;
1481  0005 7b01          	ld	a,(OFST-1,sp)
1482  0007 97            	ld	xl,a
1483                     ; 363     uint8_t itmask1 = 0;
1485  0008 7b02          	ld	a,(OFST+0,sp)
1486  000a 97            	ld	xl,a
1487                     ; 364     uint8_t itmask2 = 0;
1489  000b 7b02          	ld	a,(OFST+0,sp)
1490  000d 97            	ld	xl,a
1491                     ; 365     uint8_t enablestatus = 0;
1493  000e 7b02          	ld	a,(OFST+0,sp)
1494  0010 97            	ld	xl,a
1495                     ; 366     assert_param(IS_SPI_GET_IT_OK(SPI_IT));
1497                     ; 368     itpos = (uint8_t)((uint8_t)1 << ((uint8_t)SPI_IT & (uint8_t)0x0F));
1499  0011 7b03          	ld	a,(OFST+1,sp)
1500  0013 a40f          	and	a,#15
1501  0015 5f            	clrw	x
1502  0016 97            	ld	xl,a
1503  0017 a601          	ld	a,#1
1504  0019 5d            	tnzw	x
1505  001a 2704          	jreq	L05
1506  001c               L25:
1507  001c 48            	sll	a
1508  001d 5a            	decw	x
1509  001e 26fc          	jrne	L25
1510  0020               L05:
1511  0020 6b01          	ld	(OFST-1,sp),a
1512                     ; 371     itmask1 = (uint8_t)((uint8_t)SPI_IT >> (uint8_t)4);
1514  0022 7b03          	ld	a,(OFST+1,sp)
1515  0024 4e            	swap	a
1516  0025 a40f          	and	a,#15
1517  0027 6b02          	ld	(OFST+0,sp),a
1518                     ; 373     itmask2 = (uint8_t)((uint8_t)1 << itmask1);
1520  0029 7b02          	ld	a,(OFST+0,sp)
1521  002b 5f            	clrw	x
1522  002c 97            	ld	xl,a
1523  002d a601          	ld	a,#1
1524  002f 5d            	tnzw	x
1525  0030 2704          	jreq	L45
1526  0032               L65:
1527  0032 48            	sll	a
1528  0033 5a            	decw	x
1529  0034 26fc          	jrne	L65
1530  0036               L45:
1531  0036 6b02          	ld	(OFST+0,sp),a
1532                     ; 375     enablestatus = (uint8_t)((uint8_t)SPI->SR & itmask2);
1534  0038 c65203        	ld	a,20995
1535  003b 1402          	and	a,(OFST+0,sp)
1536  003d 6b02          	ld	(OFST+0,sp),a
1537                     ; 377     if (((SPI->ICR & itpos) != RESET) && enablestatus)
1539  003f c65202        	ld	a,20994
1540  0042 1501          	bcp	a,(OFST-1,sp)
1541  0044 270a          	jreq	L766
1543  0046 0d02          	tnz	(OFST+0,sp)
1544  0048 2706          	jreq	L766
1545                     ; 380         pendingbitstatus = SET;
1547  004a a601          	ld	a,#1
1548  004c 6b02          	ld	(OFST+0,sp),a
1550  004e 2002          	jra	L176
1551  0050               L766:
1552                     ; 385         pendingbitstatus = RESET;
1554  0050 0f02          	clr	(OFST+0,sp)
1555  0052               L176:
1556                     ; 388     return  pendingbitstatus;
1558  0052 7b02          	ld	a,(OFST+0,sp)
1561  0054 5b03          	addw	sp,#3
1562  0056 81            	ret
1607                     ; 404 void SPI_ClearITPendingBit(SPI_IT_TypeDef SPI_IT)
1607                     ; 405 {
1608                     .text:	section	.text,new
1609  0000               _SPI_ClearITPendingBit:
1611  0000 88            	push	a
1612       00000001      OFST:	set	1
1615                     ; 406     uint8_t itpos = 0;
1617  0001 0f01          	clr	(OFST+0,sp)
1618                     ; 407     assert_param(IS_SPI_CLEAR_IT_OK(SPI_IT));
1620                     ; 412     itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)(SPI_IT & (uint8_t)0xF0) >> 4));
1622  0003 a4f0          	and	a,#240
1623  0005 4e            	swap	a
1624  0006 a40f          	and	a,#15
1625  0008 5f            	clrw	x
1626  0009 97            	ld	xl,a
1627  000a a601          	ld	a,#1
1628  000c 5d            	tnzw	x
1629  000d 2704          	jreq	L26
1630  000f               L46:
1631  000f 48            	sll	a
1632  0010 5a            	decw	x
1633  0011 26fc          	jrne	L46
1634  0013               L26:
1635  0013 6b01          	ld	(OFST+0,sp),a
1636                     ; 414     SPI->SR = (uint8_t)(~itpos);
1638  0015 7b01          	ld	a,(OFST+0,sp)
1639  0017 43            	cpl	a
1640  0018 c75203        	ld	20995,a
1641                     ; 416 }
1644  001b 84            	pop	a
1645  001c 81            	ret
1658                     	xdef	_SPI_ClearITPendingBit
1659                     	xdef	_SPI_GetITStatus
1660                     	xdef	_SPI_ClearFlag
1661                     	xdef	_SPI_GetFlagStatus
1662                     	xdef	_SPI_BiDirectionalLineConfig
1663                     	xdef	_SPI_GetCRCPolynomial
1664                     	xdef	_SPI_ResetCRC
1665                     	xdef	_SPI_GetCRC
1666                     	xdef	_SPI_CalculateCRCCmd
1667                     	xdef	_SPI_TransmitCRC
1668                     	xdef	_SPI_NSSInternalSoftwareCmd
1669                     	xdef	_SPI_ReceiveData
1670                     	xdef	_SPI_SendData
1671                     	xdef	_SPI_ITConfig
1672                     	xdef	_SPI_Cmd
1673                     	xdef	_SPI_Init
1674                     	xdef	_SPI_DeInit
1693                     	end
