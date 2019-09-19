   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 44 void SPI_DeInit(void)
  32                     ; 45 {
  34                     	switch	.text
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
 374                     	switch	.text
 375  0015               _SPI_Init:
 377  0015 89            	pushw	x
 378  0016 88            	push	a
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
 400  0017 7b07          	ld	a,(OFST+6,sp)
 401  0019 1a08          	or	a,(OFST+7,sp)
 402  001b 6b01          	ld	(OFST+0,sp),a
 403  001d 9f            	ld	a,xl
 404  001e 1a02          	or	a,(OFST+1,sp)
 405  0020 1a01          	or	a,(OFST+0,sp)
 406  0022 c75200        	ld	20992,a
 407                     ; 89     SPI->CR2 = (uint8_t)((uint8_t)(Data_Direction) | (uint8_t)(Slave_Management));
 409  0025 7b09          	ld	a,(OFST+8,sp)
 410  0027 1a0a          	or	a,(OFST+9,sp)
 411  0029 c75201        	ld	20993,a
 412                     ; 91     if (Mode == SPI_MODE_MASTER)
 414  002c 7b06          	ld	a,(OFST+5,sp)
 415  002e a104          	cp	a,#4
 416  0030 2606          	jrne	L302
 417                     ; 93         SPI->CR2 |= (uint8_t)SPI_CR2_SSI;
 419  0032 72105201      	bset	20993,#0
 421  0036 2004          	jra	L502
 422  0038               L302:
 423                     ; 97         SPI->CR2 &= (uint8_t)~(SPI_CR2_SSI);
 425  0038 72115201      	bres	20993,#0
 426  003c               L502:
 427                     ; 101     SPI->CR1 |= (uint8_t)(Mode);
 429  003c c65200        	ld	a,20992
 430  003f 1a06          	or	a,(OFST+5,sp)
 431  0041 c75200        	ld	20992,a
 432                     ; 104     SPI->CRCPR = (uint8_t)CRCPolynomial;
 434  0044 7b0b          	ld	a,(OFST+10,sp)
 435  0046 c75205        	ld	20997,a
 436                     ; 105 }
 439  0049 5b03          	addw	sp,#3
 440  004b 81            	ret
 495                     ; 113 void SPI_Cmd(FunctionalState NewState)
 495                     ; 114 {
 496                     	switch	.text
 497  004c               _SPI_Cmd:
 501                     ; 116     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 503                     ; 118     if (NewState != DISABLE)
 505  004c 4d            	tnz	a
 506  004d 2706          	jreq	L532
 507                     ; 120         SPI->CR1 |= SPI_CR1_SPE; /* Enable the SPI peripheral*/
 509  004f 721c5200      	bset	20992,#6
 511  0053 2004          	jra	L732
 512  0055               L532:
 513                     ; 124         SPI->CR1 &= (uint8_t)(~SPI_CR1_SPE); /* Disable the SPI peripheral*/
 515  0055 721d5200      	bres	20992,#6
 516  0059               L732:
 517                     ; 126 }
 520  0059 81            	ret
 629                     ; 135 void SPI_ITConfig(SPI_IT_TypeDef SPI_IT, FunctionalState NewState)
 629                     ; 136 {
 630                     	switch	.text
 631  005a               _SPI_ITConfig:
 633  005a 89            	pushw	x
 634  005b 88            	push	a
 635       00000001      OFST:	set	1
 638                     ; 137     uint8_t itpos = 0;
 640  005c 0f01          	clr	(OFST+0,sp)
 641                     ; 139     assert_param(IS_SPI_CONFIG_IT_OK(SPI_IT));
 643                     ; 140     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 645                     ; 143     itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)SPI_IT & (uint8_t)0x0F));
 647  005e 9e            	ld	a,xh
 648  005f a40f          	and	a,#15
 649  0061 5f            	clrw	x
 650  0062 97            	ld	xl,a
 651  0063 a601          	ld	a,#1
 652  0065 5d            	tnzw	x
 653  0066 2704          	jreq	L41
 654  0068               L61:
 655  0068 48            	sll	a
 656  0069 5a            	decw	x
 657  006a 26fc          	jrne	L61
 658  006c               L41:
 659  006c 6b01          	ld	(OFST+0,sp),a
 660                     ; 145     if (NewState != DISABLE)
 662  006e 0d03          	tnz	(OFST+2,sp)
 663  0070 270a          	jreq	L113
 664                     ; 147         SPI->ICR |= itpos; /* Enable interrupt*/
 666  0072 c65202        	ld	a,20994
 667  0075 1a01          	or	a,(OFST+0,sp)
 668  0077 c75202        	ld	20994,a
 670  007a 2009          	jra	L313
 671  007c               L113:
 672                     ; 151         SPI->ICR &= (uint8_t)(~itpos); /* Disable interrupt*/
 674  007c 7b01          	ld	a,(OFST+0,sp)
 675  007e 43            	cpl	a
 676  007f c45202        	and	a,20994
 677  0082 c75202        	ld	20994,a
 678  0085               L313:
 679                     ; 153 }
 682  0085 5b03          	addw	sp,#3
 683  0087 81            	ret
 717                     ; 159 void SPI_SendData(uint8_t Data)
 717                     ; 160 {
 718                     	switch	.text
 719  0088               _SPI_SendData:
 723                     ; 161     SPI->DR = Data; /* Write in the DR register the data to be sent*/
 725  0088 c75204        	ld	20996,a
 726                     ; 162 }
 729  008b 81            	ret
 752                     ; 169 uint8_t SPI_ReceiveData(void)
 752                     ; 170 {
 753                     	switch	.text
 754  008c               _SPI_ReceiveData:
 758                     ; 171     return ((uint8_t)SPI->DR); /* Return the data in the DR register*/
 760  008c c65204        	ld	a,20996
 763  008f 81            	ret
 799                     ; 180 void SPI_NSSInternalSoftwareCmd(FunctionalState NewState)
 799                     ; 181 {
 800                     	switch	.text
 801  0090               _SPI_NSSInternalSoftwareCmd:
 805                     ; 183     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 807                     ; 185     if (NewState != DISABLE)
 809  0090 4d            	tnz	a
 810  0091 2706          	jreq	L163
 811                     ; 187         SPI->CR2 |= SPI_CR2_SSI; /* Set NSS pin internally by software*/
 813  0093 72105201      	bset	20993,#0
 815  0097 2004          	jra	L363
 816  0099               L163:
 817                     ; 191         SPI->CR2 &= (uint8_t)(~SPI_CR2_SSI); /* Reset NSS pin internally by software*/
 819  0099 72115201      	bres	20993,#0
 820  009d               L363:
 821                     ; 193 }
 824  009d 81            	ret
 847                     ; 200 void SPI_TransmitCRC(void)
 847                     ; 201 {
 848                     	switch	.text
 849  009e               _SPI_TransmitCRC:
 853                     ; 202     SPI->CR2 |= SPI_CR2_CRCNEXT; /* Enable the CRC transmission*/
 855  009e 72185201      	bset	20993,#4
 856                     ; 203 }
 859  00a2 81            	ret
 895                     ; 211 void SPI_CalculateCRCCmd(FunctionalState NewState)
 895                     ; 212 {
 896                     	switch	.text
 897  00a3               _SPI_CalculateCRCCmd:
 901                     ; 214     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 903                     ; 216     if (NewState != DISABLE)
 905  00a3 4d            	tnz	a
 906  00a4 2706          	jreq	L314
 907                     ; 218         SPI->CR2 |= SPI_CR2_CRCEN; /* Enable the CRC calculation*/
 909  00a6 721a5201      	bset	20993,#5
 911  00aa 2004          	jra	L514
 912  00ac               L314:
 913                     ; 222         SPI->CR2 &= (uint8_t)(~SPI_CR2_CRCEN); /* Disable the CRC calculation*/
 915  00ac 721b5201      	bres	20993,#5
 916  00b0               L514:
 917                     ; 224 }
 920  00b0 81            	ret
 984                     ; 231 uint8_t SPI_GetCRC(SPI_CRC_TypeDef SPI_CRC)
 984                     ; 232 {
 985                     	switch	.text
 986  00b1               _SPI_GetCRC:
 988  00b1 88            	push	a
 989       00000001      OFST:	set	1
 992                     ; 233     uint8_t crcreg = 0;
 994  00b2 0f01          	clr	(OFST+0,sp)
 995                     ; 236     assert_param(IS_SPI_CRC_OK(SPI_CRC));
 997                     ; 238     if (SPI_CRC != SPI_CRC_RX)
 999  00b4 4d            	tnz	a
1000  00b5 2707          	jreq	L154
1001                     ; 240         crcreg = SPI->TXCRCR;  /* Get the Tx CRC register*/
1003  00b7 c65207        	ld	a,20999
1004  00ba 6b01          	ld	(OFST+0,sp),a
1006  00bc 2005          	jra	L354
1007  00be               L154:
1008                     ; 244         crcreg = SPI->RXCRCR; /* Get the Rx CRC register*/
1010  00be c65206        	ld	a,20998
1011  00c1 6b01          	ld	(OFST+0,sp),a
1012  00c3               L354:
1013                     ; 248     return crcreg;
1015  00c3 7b01          	ld	a,(OFST+0,sp)
1018  00c5 5b01          	addw	sp,#1
1019  00c7 81            	ret
1044                     ; 256 void SPI_ResetCRC(void)
1044                     ; 257 {
1045                     	switch	.text
1046  00c8               _SPI_ResetCRC:
1050                     ; 260     SPI_CalculateCRCCmd(ENABLE);
1052  00c8 a601          	ld	a,#1
1053  00ca add7          	call	_SPI_CalculateCRCCmd
1055                     ; 263     SPI_Cmd(ENABLE);
1057  00cc a601          	ld	a,#1
1058  00ce cd004c        	call	_SPI_Cmd
1060                     ; 264 }
1063  00d1 81            	ret
1087                     ; 271 uint8_t SPI_GetCRCPolynomial(void)
1087                     ; 272 {
1088                     	switch	.text
1089  00d2               _SPI_GetCRCPolynomial:
1093                     ; 273     return SPI->CRCPR; /* Return the CRC polynomial register */
1095  00d2 c65205        	ld	a,20997
1098  00d5 81            	ret
1154                     ; 281 void SPI_BiDirectionalLineConfig(SPI_Direction_TypeDef SPI_Direction)
1154                     ; 282 {
1155                     	switch	.text
1156  00d6               _SPI_BiDirectionalLineConfig:
1160                     ; 284     assert_param(IS_SPI_DIRECTION_OK(SPI_Direction));
1162                     ; 286     if (SPI_Direction != SPI_DIRECTION_RX)
1164  00d6 4d            	tnz	a
1165  00d7 2706          	jreq	L325
1166                     ; 288         SPI->CR2 |= SPI_CR2_BDOE; /* Set the Tx only mode*/
1168  00d9 721c5201      	bset	20993,#6
1170  00dd 2004          	jra	L525
1171  00df               L325:
1172                     ; 292         SPI->CR2 &= (uint8_t)(~SPI_CR2_BDOE); /* Set the Rx only mode*/
1174  00df 721d5201      	bres	20993,#6
1175  00e3               L525:
1176                     ; 294 }
1179  00e3 81            	ret
1300                     ; 304 FlagStatus SPI_GetFlagStatus(SPI_Flag_TypeDef SPI_FLAG)
1300                     ; 305 {
1301                     	switch	.text
1302  00e4               _SPI_GetFlagStatus:
1304  00e4 88            	push	a
1305       00000001      OFST:	set	1
1308                     ; 306     FlagStatus status = RESET;
1310  00e5 0f01          	clr	(OFST+0,sp)
1311                     ; 308     assert_param(IS_SPI_FLAGS_OK(SPI_FLAG));
1313                     ; 311     if ((SPI->SR & (uint8_t)SPI_FLAG) != (uint8_t)RESET)
1315  00e7 c45203        	and	a,20995
1316  00ea 2706          	jreq	L306
1317                     ; 313         status = SET; /* SPI_FLAG is set */
1319  00ec a601          	ld	a,#1
1320  00ee 6b01          	ld	(OFST+0,sp),a
1322  00f0 2002          	jra	L506
1323  00f2               L306:
1324                     ; 317         status = RESET; /* SPI_FLAG is reset*/
1326  00f2 0f01          	clr	(OFST+0,sp)
1327  00f4               L506:
1328                     ; 321     return status;
1330  00f4 7b01          	ld	a,(OFST+0,sp)
1333  00f6 5b01          	addw	sp,#1
1334  00f8 81            	ret
1369                     ; 339 void SPI_ClearFlag(SPI_Flag_TypeDef SPI_FLAG)
1369                     ; 340 {
1370                     	switch	.text
1371  00f9               _SPI_ClearFlag:
1375                     ; 341     assert_param(IS_SPI_CLEAR_FLAGS_OK(SPI_FLAG));
1377                     ; 343     SPI->SR = (uint8_t)(~SPI_FLAG);
1379  00f9 43            	cpl	a
1380  00fa c75203        	ld	20995,a
1381                     ; 344 }
1384  00fd 81            	ret
1466                     ; 359 ITStatus SPI_GetITStatus(SPI_IT_TypeDef SPI_IT)
1466                     ; 360 {
1467                     	switch	.text
1468  00fe               _SPI_GetITStatus:
1470  00fe 88            	push	a
1471  00ff 89            	pushw	x
1472       00000002      OFST:	set	2
1475                     ; 361     ITStatus pendingbitstatus = RESET;
1477  0100 7b02          	ld	a,(OFST+0,sp)
1478  0102 97            	ld	xl,a
1479                     ; 362     uint8_t itpos = 0;
1481  0103 7b01          	ld	a,(OFST-1,sp)
1482  0105 97            	ld	xl,a
1483                     ; 363     uint8_t itmask1 = 0;
1485  0106 7b02          	ld	a,(OFST+0,sp)
1486  0108 97            	ld	xl,a
1487                     ; 364     uint8_t itmask2 = 0;
1489  0109 7b02          	ld	a,(OFST+0,sp)
1490  010b 97            	ld	xl,a
1491                     ; 365     uint8_t enablestatus = 0;
1493  010c 7b02          	ld	a,(OFST+0,sp)
1494  010e 97            	ld	xl,a
1495                     ; 366     assert_param(IS_SPI_GET_IT_OK(SPI_IT));
1497                     ; 368     itpos = (uint8_t)((uint8_t)1 << ((uint8_t)SPI_IT & (uint8_t)0x0F));
1499  010f 7b03          	ld	a,(OFST+1,sp)
1500  0111 a40f          	and	a,#15
1501  0113 5f            	clrw	x
1502  0114 97            	ld	xl,a
1503  0115 a601          	ld	a,#1
1504  0117 5d            	tnzw	x
1505  0118 2704          	jreq	L05
1506  011a               L25:
1507  011a 48            	sll	a
1508  011b 5a            	decw	x
1509  011c 26fc          	jrne	L25
1510  011e               L05:
1511  011e 6b01          	ld	(OFST-1,sp),a
1512                     ; 371     itmask1 = (uint8_t)((uint8_t)SPI_IT >> (uint8_t)4);
1514  0120 7b03          	ld	a,(OFST+1,sp)
1515  0122 4e            	swap	a
1516  0123 a40f          	and	a,#15
1517  0125 6b02          	ld	(OFST+0,sp),a
1518                     ; 373     itmask2 = (uint8_t)((uint8_t)1 << itmask1);
1520  0127 7b02          	ld	a,(OFST+0,sp)
1521  0129 5f            	clrw	x
1522  012a 97            	ld	xl,a
1523  012b a601          	ld	a,#1
1524  012d 5d            	tnzw	x
1525  012e 2704          	jreq	L45
1526  0130               L65:
1527  0130 48            	sll	a
1528  0131 5a            	decw	x
1529  0132 26fc          	jrne	L65
1530  0134               L45:
1531  0134 6b02          	ld	(OFST+0,sp),a
1532                     ; 375     enablestatus = (uint8_t)((uint8_t)SPI->SR & itmask2);
1534  0136 c65203        	ld	a,20995
1535  0139 1402          	and	a,(OFST+0,sp)
1536  013b 6b02          	ld	(OFST+0,sp),a
1537                     ; 377     if (((SPI->ICR & itpos) != RESET) && enablestatus)
1539  013d c65202        	ld	a,20994
1540  0140 1501          	bcp	a,(OFST-1,sp)
1541  0142 270a          	jreq	L766
1543  0144 0d02          	tnz	(OFST+0,sp)
1544  0146 2706          	jreq	L766
1545                     ; 380         pendingbitstatus = SET;
1547  0148 a601          	ld	a,#1
1548  014a 6b02          	ld	(OFST+0,sp),a
1550  014c 2002          	jra	L176
1551  014e               L766:
1552                     ; 385         pendingbitstatus = RESET;
1554  014e 0f02          	clr	(OFST+0,sp)
1555  0150               L176:
1556                     ; 388     return  pendingbitstatus;
1558  0150 7b02          	ld	a,(OFST+0,sp)
1561  0152 5b03          	addw	sp,#3
1562  0154 81            	ret
1607                     ; 404 void SPI_ClearITPendingBit(SPI_IT_TypeDef SPI_IT)
1607                     ; 405 {
1608                     	switch	.text
1609  0155               _SPI_ClearITPendingBit:
1611  0155 88            	push	a
1612       00000001      OFST:	set	1
1615                     ; 406     uint8_t itpos = 0;
1617  0156 0f01          	clr	(OFST+0,sp)
1618                     ; 407     assert_param(IS_SPI_CLEAR_IT_OK(SPI_IT));
1620                     ; 412     itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)(SPI_IT & (uint8_t)0xF0) >> 4));
1622  0158 a4f0          	and	a,#240
1623  015a 4e            	swap	a
1624  015b a40f          	and	a,#15
1625  015d 5f            	clrw	x
1626  015e 97            	ld	xl,a
1627  015f a601          	ld	a,#1
1628  0161 5d            	tnzw	x
1629  0162 2704          	jreq	L26
1630  0164               L46:
1631  0164 48            	sll	a
1632  0165 5a            	decw	x
1633  0166 26fc          	jrne	L46
1634  0168               L26:
1635  0168 6b01          	ld	(OFST+0,sp),a
1636                     ; 414     SPI->SR = (uint8_t)(~itpos);
1638  016a 7b01          	ld	a,(OFST+0,sp)
1639  016c 43            	cpl	a
1640  016d c75203        	ld	20995,a
1641                     ; 416 }
1644  0170 84            	pop	a
1645  0171 81            	ret
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
