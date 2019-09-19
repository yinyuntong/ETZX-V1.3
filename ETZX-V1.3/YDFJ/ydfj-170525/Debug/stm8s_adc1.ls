   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
  32                     ; 48 void ADC1_DeInit(void)
  32                     ; 49 {
  34                     	switch	.text
  35  0000               _ADC1_DeInit:
  39                     ; 50     ADC1->CSR  = ADC1_CSR_RESET_VALUE;
  41  0000 725f5400      	clr	21504
  42                     ; 51     ADC1->CR1  = ADC1_CR1_RESET_VALUE;
  44  0004 725f5401      	clr	21505
  45                     ; 52     ADC1->CR2  = ADC1_CR2_RESET_VALUE;
  47  0008 725f5402      	clr	21506
  48                     ; 53     ADC1->CR3  = ADC1_CR3_RESET_VALUE;
  50  000c 725f5403      	clr	21507
  51                     ; 54     ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
  53  0010 725f5406      	clr	21510
  54                     ; 55     ADC1->TDRL = ADC1_TDRL_RESET_VALUE;
  56  0014 725f5407      	clr	21511
  57                     ; 56     ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
  59  0018 35ff5408      	mov	21512,#255
  60                     ; 57     ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
  62  001c 35035409      	mov	21513,#3
  63                     ; 58     ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
  65  0020 725f540a      	clr	21514
  66                     ; 59     ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
  68  0024 725f540b      	clr	21515
  69                     ; 60     ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
  71  0028 725f540e      	clr	21518
  72                     ; 61     ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;
  74  002c 725f540f      	clr	21519
  75                     ; 62 }
  78  0030 81            	ret
 529                     ; 85 void ADC1_Init(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_PresSel_TypeDef ADC1_PrescalerSelection, ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState ADC1_ExtTriggerState, ADC1_Align_TypeDef ADC1_Align, ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState ADC1_SchmittTriggerState)
 529                     ; 86 {
 530                     	switch	.text
 531  0031               _ADC1_Init:
 533  0031 89            	pushw	x
 534       00000000      OFST:	set	0
 537                     ; 89     assert_param(IS_ADC1_CONVERSIONMODE_OK(ADC1_ConversionMode));
 539                     ; 90     assert_param(IS_ADC1_CHANNEL_OK(ADC1_Channel));
 541                     ; 91     assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
 543                     ; 92     assert_param(IS_ADC1_EXTTRIG_OK(ADC1_ExtTrigger));
 545                     ; 93     assert_param(IS_FUNCTIONALSTATE_OK(((ADC1_ExtTriggerState))));
 547                     ; 94     assert_param(IS_ADC1_ALIGN_OK(ADC1_Align));
 549                     ; 95     assert_param(IS_ADC1_SCHMITTTRIG_OK(ADC1_SchmittTriggerChannel));
 551                     ; 96     assert_param(IS_FUNCTIONALSTATE_OK(ADC1_SchmittTriggerState));
 553                     ; 101     ADC1_ConversionConfig(ADC1_ConversionMode, ADC1_Channel, ADC1_Align);
 555  0032 7b08          	ld	a,(OFST+8,sp)
 556  0034 88            	push	a
 557  0035 9f            	ld	a,xl
 558  0036 97            	ld	xl,a
 559  0037 7b02          	ld	a,(OFST+2,sp)
 560  0039 95            	ld	xh,a
 561  003a cd013d        	call	_ADC1_ConversionConfig
 563  003d 84            	pop	a
 564                     ; 103     ADC1_PrescalerConfig(ADC1_PrescalerSelection);
 566  003e 7b05          	ld	a,(OFST+5,sp)
 567  0040 ad5a          	call	_ADC1_PrescalerConfig
 569                     ; 108     ADC1_ExternalTriggerConfig(ADC1_ExtTrigger, ADC1_ExtTriggerState);
 571  0042 7b07          	ld	a,(OFST+7,sp)
 572  0044 97            	ld	xl,a
 573  0045 7b06          	ld	a,(OFST+6,sp)
 574  0047 95            	ld	xh,a
 575  0048 cd016b        	call	_ADC1_ExternalTriggerConfig
 577                     ; 113     ADC1_SchmittTriggerConfig(ADC1_SchmittTriggerChannel, ADC1_SchmittTriggerState);
 579  004b 7b0a          	ld	a,(OFST+10,sp)
 580  004d 97            	ld	xl,a
 581  004e 7b09          	ld	a,(OFST+9,sp)
 582  0050 95            	ld	xh,a
 583  0051 ad5c          	call	_ADC1_SchmittTriggerConfig
 585                     ; 116     ADC1->CR1 |= ADC1_CR1_ADON;
 587  0053 72105401      	bset	21505,#0
 588                     ; 118 }
 591  0057 85            	popw	x
 592  0058 81            	ret
 627                     ; 126 void ADC1_Cmd(FunctionalState NewState)
 627                     ; 127 {
 628                     	switch	.text
 629  0059               _ADC1_Cmd:
 633                     ; 130     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 635                     ; 132     if (NewState != DISABLE)
 637  0059 4d            	tnz	a
 638  005a 2706          	jreq	L362
 639                     ; 134         ADC1->CR1 |= ADC1_CR1_ADON;
 641  005c 72105401      	bset	21505,#0
 643  0060 2004          	jra	L562
 644  0062               L362:
 645                     ; 138         ADC1->CR1 &= (uint8_t)(~ADC1_CR1_ADON);
 647  0062 72115401      	bres	21505,#0
 648  0066               L562:
 649                     ; 141 }
 652  0066 81            	ret
 687                     ; 148 void ADC1_ScanModeCmd(FunctionalState NewState)
 687                     ; 149 {
 688                     	switch	.text
 689  0067               _ADC1_ScanModeCmd:
 693                     ; 152     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 695                     ; 154     if (NewState != DISABLE)
 697  0067 4d            	tnz	a
 698  0068 2706          	jreq	L503
 699                     ; 156         ADC1->CR2 |= ADC1_CR2_SCAN;
 701  006a 72125402      	bset	21506,#1
 703  006e 2004          	jra	L703
 704  0070               L503:
 705                     ; 160         ADC1->CR2 &= (uint8_t)(~ADC1_CR2_SCAN);
 707  0070 72135402      	bres	21506,#1
 708  0074               L703:
 709                     ; 163 }
 712  0074 81            	ret
 747                     ; 170 void ADC1_DataBufferCmd(FunctionalState NewState)
 747                     ; 171 {
 748                     	switch	.text
 749  0075               _ADC1_DataBufferCmd:
 753                     ; 174     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 755                     ; 176     if (NewState != DISABLE)
 757  0075 4d            	tnz	a
 758  0076 2706          	jreq	L723
 759                     ; 178         ADC1->CR3 |= ADC1_CR3_DBUF;
 761  0078 721e5403      	bset	21507,#7
 763  007c 2004          	jra	L133
 764  007e               L723:
 765                     ; 182         ADC1->CR3 &= (uint8_t)(~ADC1_CR3_DBUF);
 767  007e 721f5403      	bres	21507,#7
 768  0082               L133:
 769                     ; 185 }
 772  0082 81            	ret
 928                     ; 196 void ADC1_ITConfig(ADC1_IT_TypeDef ADC1_IT, FunctionalState NewState)
 928                     ; 197 {
 929                     	switch	.text
 930  0083               _ADC1_ITConfig:
 932  0083 89            	pushw	x
 933       00000000      OFST:	set	0
 936                     ; 200     assert_param(IS_ADC1_IT_OK(ADC1_IT));
 938                     ; 201     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
 940                     ; 203     if (NewState != DISABLE)
 942  0084 0d05          	tnz	(OFST+5,sp)
 943  0086 2709          	jreq	L714
 944                     ; 206         ADC1->CSR |= (uint8_t)ADC1_IT;
 946  0088 9f            	ld	a,xl
 947  0089 ca5400        	or	a,21504
 948  008c c75400        	ld	21504,a
 950  008f 2009          	jra	L124
 951  0091               L714:
 952                     ; 211         ADC1->CSR &= (uint8_t)((uint16_t)~(uint16_t)ADC1_IT);
 954  0091 7b02          	ld	a,(OFST+2,sp)
 955  0093 43            	cpl	a
 956  0094 c45400        	and	a,21504
 957  0097 c75400        	ld	21504,a
 958  009a               L124:
 959                     ; 214 }
 962  009a 85            	popw	x
 963  009b 81            	ret
 999                     ; 222 void ADC1_PrescalerConfig(ADC1_PresSel_TypeDef ADC1_Prescaler)
 999                     ; 223 {
1000                     	switch	.text
1001  009c               _ADC1_PrescalerConfig:
1003  009c 88            	push	a
1004       00000000      OFST:	set	0
1007                     ; 226     assert_param(IS_ADC1_PRESSEL_OK(ADC1_Prescaler));
1009                     ; 229     ADC1->CR1 &= (uint8_t)(~ADC1_CR1_SPSEL);
1011  009d c65401        	ld	a,21505
1012  00a0 a48f          	and	a,#143
1013  00a2 c75401        	ld	21505,a
1014                     ; 231     ADC1->CR1 |= (uint8_t)(ADC1_Prescaler);
1016  00a5 c65401        	ld	a,21505
1017  00a8 1a01          	or	a,(OFST+1,sp)
1018  00aa c75401        	ld	21505,a
1019                     ; 233 }
1022  00ad 84            	pop	a
1023  00ae 81            	ret
1070                     ; 244 void ADC1_SchmittTriggerConfig(ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState NewState)
1070                     ; 245 {
1071                     	switch	.text
1072  00af               _ADC1_SchmittTriggerConfig:
1074  00af 89            	pushw	x
1075       00000000      OFST:	set	0
1078                     ; 248     assert_param(IS_ADC1_SCHMITTTRIG_OK(ADC1_SchmittTriggerChannel));
1080                     ; 249     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1082                     ; 251     if (ADC1_SchmittTriggerChannel == ADC1_SCHMITTTRIG_ALL)
1084  00b0 9e            	ld	a,xh
1085  00b1 a1ff          	cp	a,#255
1086  00b3 2620          	jrne	L364
1087                     ; 253         if (NewState != DISABLE)
1089  00b5 9f            	ld	a,xl
1090  00b6 4d            	tnz	a
1091  00b7 270a          	jreq	L564
1092                     ; 255             ADC1->TDRL &= (uint8_t)0x0;
1094  00b9 725f5407      	clr	21511
1095                     ; 256             ADC1->TDRH &= (uint8_t)0x0;
1097  00bd 725f5406      	clr	21510
1099  00c1 2078          	jra	L174
1100  00c3               L564:
1101                     ; 260             ADC1->TDRL |= (uint8_t)0xFF;
1103  00c3 c65407        	ld	a,21511
1104  00c6 aaff          	or	a,#255
1105  00c8 c75407        	ld	21511,a
1106                     ; 261             ADC1->TDRH |= (uint8_t)0xFF;
1108  00cb c65406        	ld	a,21510
1109  00ce aaff          	or	a,#255
1110  00d0 c75406        	ld	21510,a
1111  00d3 2066          	jra	L174
1112  00d5               L364:
1113                     ; 264     else if (ADC1_SchmittTriggerChannel < ADC1_SCHMITTTRIG_CHANNEL8)
1115  00d5 7b01          	ld	a,(OFST+1,sp)
1116  00d7 a108          	cp	a,#8
1117  00d9 242f          	jruge	L374
1118                     ; 266         if (NewState != DISABLE)
1120  00db 0d02          	tnz	(OFST+2,sp)
1121  00dd 2716          	jreq	L574
1122                     ; 268             ADC1->TDRL &= (uint8_t)(~(uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SchmittTriggerChannel));
1124  00df 7b01          	ld	a,(OFST+1,sp)
1125  00e1 5f            	clrw	x
1126  00e2 97            	ld	xl,a
1127  00e3 a601          	ld	a,#1
1128  00e5 5d            	tnzw	x
1129  00e6 2704          	jreq	L42
1130  00e8               L62:
1131  00e8 48            	sll	a
1132  00e9 5a            	decw	x
1133  00ea 26fc          	jrne	L62
1134  00ec               L42:
1135  00ec 43            	cpl	a
1136  00ed c45407        	and	a,21511
1137  00f0 c75407        	ld	21511,a
1139  00f3 2046          	jra	L174
1140  00f5               L574:
1141                     ; 272             ADC1->TDRL |= (uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SchmittTriggerChannel);
1143  00f5 7b01          	ld	a,(OFST+1,sp)
1144  00f7 5f            	clrw	x
1145  00f8 97            	ld	xl,a
1146  00f9 a601          	ld	a,#1
1147  00fb 5d            	tnzw	x
1148  00fc 2704          	jreq	L03
1149  00fe               L23:
1150  00fe 48            	sll	a
1151  00ff 5a            	decw	x
1152  0100 26fc          	jrne	L23
1153  0102               L03:
1154  0102 ca5407        	or	a,21511
1155  0105 c75407        	ld	21511,a
1156  0108 2031          	jra	L174
1157  010a               L374:
1158                     ; 277         if (NewState != DISABLE)
1160  010a 0d02          	tnz	(OFST+2,sp)
1161  010c 2718          	jreq	L305
1162                     ; 279             ADC1->TDRH &= (uint8_t)(~(uint8_t)((uint8_t)0x01 << ((uint8_t)ADC1_SchmittTriggerChannel - (uint8_t)8)));
1164  010e 7b01          	ld	a,(OFST+1,sp)
1165  0110 a008          	sub	a,#8
1166  0112 5f            	clrw	x
1167  0113 97            	ld	xl,a
1168  0114 a601          	ld	a,#1
1169  0116 5d            	tnzw	x
1170  0117 2704          	jreq	L43
1171  0119               L63:
1172  0119 48            	sll	a
1173  011a 5a            	decw	x
1174  011b 26fc          	jrne	L63
1175  011d               L43:
1176  011d 43            	cpl	a
1177  011e c45406        	and	a,21510
1178  0121 c75406        	ld	21510,a
1180  0124 2015          	jra	L174
1181  0126               L305:
1182                     ; 283             ADC1->TDRH |= (uint8_t)((uint8_t)0x01 << ((uint8_t)ADC1_SchmittTriggerChannel - (uint8_t)8));
1184  0126 7b01          	ld	a,(OFST+1,sp)
1185  0128 a008          	sub	a,#8
1186  012a 5f            	clrw	x
1187  012b 97            	ld	xl,a
1188  012c a601          	ld	a,#1
1189  012e 5d            	tnzw	x
1190  012f 2704          	jreq	L04
1191  0131               L24:
1192  0131 48            	sll	a
1193  0132 5a            	decw	x
1194  0133 26fc          	jrne	L24
1195  0135               L04:
1196  0135 ca5406        	or	a,21510
1197  0138 c75406        	ld	21510,a
1198  013b               L174:
1199                     ; 287 }
1202  013b 85            	popw	x
1203  013c 81            	ret
1260                     ; 300 void ADC1_ConversionConfig(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_Align_TypeDef ADC1_Align)
1260                     ; 301 {
1261                     	switch	.text
1262  013d               _ADC1_ConversionConfig:
1264  013d 89            	pushw	x
1265       00000000      OFST:	set	0
1268                     ; 304     assert_param(IS_ADC1_CONVERSIONMODE_OK(ADC1_ConversionMode));
1270                     ; 305     assert_param(IS_ADC1_CHANNEL_OK(ADC1_Channel));
1272                     ; 306     assert_param(IS_ADC1_ALIGN_OK(ADC1_Align));
1274                     ; 309     ADC1->CR2 &= (uint8_t)(~ADC1_CR2_ALIGN);
1276  013e 72175402      	bres	21506,#3
1277                     ; 311     ADC1->CR2 |= (uint8_t)(ADC1_Align);
1279  0142 c65402        	ld	a,21506
1280  0145 1a05          	or	a,(OFST+5,sp)
1281  0147 c75402        	ld	21506,a
1282                     ; 313     if (ADC1_ConversionMode == ADC1_CONVERSIONMODE_CONTINUOUS)
1284  014a 9e            	ld	a,xh
1285  014b a101          	cp	a,#1
1286  014d 2606          	jrne	L535
1287                     ; 316         ADC1->CR1 |= ADC1_CR1_CONT;
1289  014f 72125401      	bset	21505,#1
1291  0153 2004          	jra	L735
1292  0155               L535:
1293                     ; 321         ADC1->CR1 &= (uint8_t)(~ADC1_CR1_CONT);
1295  0155 72135401      	bres	21505,#1
1296  0159               L735:
1297                     ; 325     ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);
1299  0159 c65400        	ld	a,21504
1300  015c a4f0          	and	a,#240
1301  015e c75400        	ld	21504,a
1302                     ; 327     ADC1->CSR |= (uint8_t)(ADC1_Channel);
1304  0161 c65400        	ld	a,21504
1305  0164 1a02          	or	a,(OFST+2,sp)
1306  0166 c75400        	ld	21504,a
1307                     ; 329 }
1310  0169 85            	popw	x
1311  016a 81            	ret
1357                     ; 342 void ADC1_ExternalTriggerConfig(ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState NewState)
1357                     ; 343 {
1358                     	switch	.text
1359  016b               _ADC1_ExternalTriggerConfig:
1361  016b 89            	pushw	x
1362       00000000      OFST:	set	0
1365                     ; 346     assert_param(IS_ADC1_EXTTRIG_OK(ADC1_ExtTrigger));
1367                     ; 347     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1369                     ; 350     ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTSEL);
1371  016c c65402        	ld	a,21506
1372  016f a4cf          	and	a,#207
1373  0171 c75402        	ld	21506,a
1374                     ; 352     if (NewState != DISABLE)
1376  0174 9f            	ld	a,xl
1377  0175 4d            	tnz	a
1378  0176 2706          	jreq	L365
1379                     ; 355         ADC1->CR2 |= (uint8_t)(ADC1_CR2_EXTTRIG);
1381  0178 721c5402      	bset	21506,#6
1383  017c 2004          	jra	L565
1384  017e               L365:
1385                     ; 360         ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTTRIG);
1387  017e 721d5402      	bres	21506,#6
1388  0182               L565:
1389                     ; 364     ADC1->CR2 |= (uint8_t)(ADC1_ExtTrigger);
1391  0182 c65402        	ld	a,21506
1392  0185 1a01          	or	a,(OFST+1,sp)
1393  0187 c75402        	ld	21506,a
1394                     ; 366 }
1397  018a 85            	popw	x
1398  018b 81            	ret
1422                     ; 378 void ADC1_StartConversion(void)
1422                     ; 379 {
1423                     	switch	.text
1424  018c               _ADC1_StartConversion:
1428                     ; 380     ADC1->CR1 |= ADC1_CR1_ADON;
1430  018c 72105401      	bset	21505,#0
1431                     ; 381 }
1434  0190 81            	ret
1478                     ; 390 uint16_t ADC1_GetConversionValue(void)
1478                     ; 391 {
1479                     	switch	.text
1480  0191               _ADC1_GetConversionValue:
1482  0191 5205          	subw	sp,#5
1483       00000005      OFST:	set	5
1486                     ; 393     uint16_t temph = 0;
1488  0193 1e04          	ldw	x,(OFST-1,sp)
1489                     ; 394     uint8_t templ = 0;
1491  0195 7b03          	ld	a,(OFST-2,sp)
1492  0197 97            	ld	xl,a
1493                     ; 396     if ((ADC1->CR2 & ADC1_CR2_ALIGN) != 0) /* Right alignment */
1495  0198 c65402        	ld	a,21506
1496  019b a508          	bcp	a,#8
1497  019d 2719          	jreq	L126
1498                     ; 399         templ = ADC1->DRL;
1500  019f c65405        	ld	a,21509
1501  01a2 6b03          	ld	(OFST-2,sp),a
1502                     ; 401         temph = ADC1->DRH;
1504  01a4 c65404        	ld	a,21508
1505  01a7 5f            	clrw	x
1506  01a8 97            	ld	xl,a
1507  01a9 1f04          	ldw	(OFST-1,sp),x
1508                     ; 403         temph = (uint16_t)(templ | (uint16_t)(temph << (uint8_t)8));
1510  01ab 1e04          	ldw	x,(OFST-1,sp)
1511  01ad 4f            	clr	a
1512  01ae 02            	rlwa	x,a
1513  01af 01            	rrwa	x,a
1514  01b0 1a03          	or	a,(OFST-2,sp)
1515  01b2 02            	rlwa	x,a
1516  01b3 1f04          	ldw	(OFST-1,sp),x
1517  01b5 01            	rrwa	x,a
1519  01b6 2021          	jra	L326
1520  01b8               L126:
1521                     ; 408         temph = ADC1->DRH;
1523  01b8 c65404        	ld	a,21508
1524  01bb 5f            	clrw	x
1525  01bc 97            	ld	xl,a
1526  01bd 1f04          	ldw	(OFST-1,sp),x
1527                     ; 410         templ = ADC1->DRL;
1529  01bf c65405        	ld	a,21509
1530  01c2 6b03          	ld	(OFST-2,sp),a
1531                     ; 412         temph = (uint16_t)((uint16_t)((uint16_t)templ << 6) | (uint16_t)((uint16_t)temph << 8));
1533  01c4 1e04          	ldw	x,(OFST-1,sp)
1534  01c6 4f            	clr	a
1535  01c7 02            	rlwa	x,a
1536  01c8 1f01          	ldw	(OFST-4,sp),x
1537  01ca 7b03          	ld	a,(OFST-2,sp)
1538  01cc 97            	ld	xl,a
1539  01cd a640          	ld	a,#64
1540  01cf 42            	mul	x,a
1541  01d0 01            	rrwa	x,a
1542  01d1 1a02          	or	a,(OFST-3,sp)
1543  01d3 01            	rrwa	x,a
1544  01d4 1a01          	or	a,(OFST-4,sp)
1545  01d6 01            	rrwa	x,a
1546  01d7 1f04          	ldw	(OFST-1,sp),x
1547  01d9               L326:
1548                     ; 415     return ((uint16_t)temph);
1550  01d9 1e04          	ldw	x,(OFST-1,sp)
1553  01db 5b05          	addw	sp,#5
1554  01dd 81            	ret
1600                     ; 427 void ADC1_AWDChannelConfig(ADC1_Channel_TypeDef Channel, FunctionalState NewState)
1600                     ; 428 {
1601                     	switch	.text
1602  01de               _ADC1_AWDChannelConfig:
1604  01de 89            	pushw	x
1605       00000000      OFST:	set	0
1608                     ; 430     assert_param(IS_FUNCTIONALSTATE_OK(NewState));
1610                     ; 431     assert_param(IS_ADC1_CHANNEL_OK(Channel));
1612                     ; 433     if (Channel < (uint8_t)8)
1614  01df 9e            	ld	a,xh
1615  01e0 a108          	cp	a,#8
1616  01e2 242e          	jruge	L746
1617                     ; 435         if (NewState != DISABLE)
1619  01e4 9f            	ld	a,xl
1620  01e5 4d            	tnz	a
1621  01e6 2714          	jreq	L156
1622                     ; 437             ADC1->AWCRL |= (uint8_t)((uint8_t)1 << Channel);
1624  01e8 9e            	ld	a,xh
1625  01e9 5f            	clrw	x
1626  01ea 97            	ld	xl,a
1627  01eb a601          	ld	a,#1
1628  01ed 5d            	tnzw	x
1629  01ee 2704          	jreq	L65
1630  01f0               L06:
1631  01f0 48            	sll	a
1632  01f1 5a            	decw	x
1633  01f2 26fc          	jrne	L06
1634  01f4               L65:
1635  01f4 ca540f        	or	a,21519
1636  01f7 c7540f        	ld	21519,a
1638  01fa 2047          	jra	L556
1639  01fc               L156:
1640                     ; 441             ADC1->AWCRL &= (uint8_t)~(uint8_t)((uint8_t)1 << Channel);
1642  01fc 7b01          	ld	a,(OFST+1,sp)
1643  01fe 5f            	clrw	x
1644  01ff 97            	ld	xl,a
1645  0200 a601          	ld	a,#1
1646  0202 5d            	tnzw	x
1647  0203 2704          	jreq	L26
1648  0205               L46:
1649  0205 48            	sll	a
1650  0206 5a            	decw	x
1651  0207 26fc          	jrne	L46
1652  0209               L26:
1653  0209 43            	cpl	a
1654  020a c4540f        	and	a,21519
1655  020d c7540f        	ld	21519,a
1656  0210 2031          	jra	L556
1657  0212               L746:
1658                     ; 446         if (NewState != DISABLE)
1660  0212 0d02          	tnz	(OFST+2,sp)
1661  0214 2717          	jreq	L756
1662                     ; 448             ADC1->AWCRH |= (uint8_t)((uint8_t)1 << (Channel - (uint8_t)8));
1664  0216 7b01          	ld	a,(OFST+1,sp)
1665  0218 a008          	sub	a,#8
1666  021a 5f            	clrw	x
1667  021b 97            	ld	xl,a
1668  021c a601          	ld	a,#1
1669  021e 5d            	tnzw	x
1670  021f 2704          	jreq	L66
1671  0221               L07:
1672  0221 48            	sll	a
1673  0222 5a            	decw	x
1674  0223 26fc          	jrne	L07
1675  0225               L66:
1676  0225 ca540e        	or	a,21518
1677  0228 c7540e        	ld	21518,a
1679  022b 2016          	jra	L556
1680  022d               L756:
1681                     ; 452             ADC1->AWCRH &= (uint8_t)~(uint8_t)((uint8_t)1 << (uint8_t)(Channel - (uint8_t)8));
1683  022d 7b01          	ld	a,(OFST+1,sp)
1684  022f a008          	sub	a,#8
1685  0231 5f            	clrw	x
1686  0232 97            	ld	xl,a
1687  0233 a601          	ld	a,#1
1688  0235 5d            	tnzw	x
1689  0236 2704          	jreq	L27
1690  0238               L47:
1691  0238 48            	sll	a
1692  0239 5a            	decw	x
1693  023a 26fc          	jrne	L47
1694  023c               L27:
1695  023c 43            	cpl	a
1696  023d c4540e        	and	a,21518
1697  0240 c7540e        	ld	21518,a
1698  0243               L556:
1699                     ; 455 }
1702  0243 85            	popw	x
1703  0244 81            	ret
1738                     ; 463 void ADC1_SetHighThreshold(uint16_t Threshold)
1738                     ; 464 {
1739                     	switch	.text
1740  0245               _ADC1_SetHighThreshold:
1742  0245 89            	pushw	x
1743       00000000      OFST:	set	0
1746                     ; 465     ADC1->HTRH = (uint8_t)(Threshold >> (uint8_t)2);
1748  0246 54            	srlw	x
1749  0247 54            	srlw	x
1750  0248 9f            	ld	a,xl
1751  0249 c75408        	ld	21512,a
1752                     ; 466     ADC1->HTRL = (uint8_t)Threshold;
1754  024c 7b02          	ld	a,(OFST+2,sp)
1755  024e c75409        	ld	21513,a
1756                     ; 467 }
1759  0251 85            	popw	x
1760  0252 81            	ret
1795                     ; 475 void ADC1_SetLowThreshold(uint16_t Threshold)
1795                     ; 476 {
1796                     	switch	.text
1797  0253               _ADC1_SetLowThreshold:
1801                     ; 477     ADC1->LTRL = (uint8_t)Threshold;
1803  0253 9f            	ld	a,xl
1804  0254 c7540b        	ld	21515,a
1805                     ; 478     ADC1->LTRH = (uint8_t)(Threshold >> (uint8_t)2);
1807  0257 54            	srlw	x
1808  0258 54            	srlw	x
1809  0259 9f            	ld	a,xl
1810  025a c7540a        	ld	21514,a
1811                     ; 479 }
1814  025d 81            	ret
1867                     ; 488 uint16_t ADC1_GetBufferValue(uint8_t Buffer)
1867                     ; 489 {
1868                     	switch	.text
1869  025e               _ADC1_GetBufferValue:
1871  025e 88            	push	a
1872  025f 5205          	subw	sp,#5
1873       00000005      OFST:	set	5
1876                     ; 491     uint16_t temph = 0;
1878  0261 1e04          	ldw	x,(OFST-1,sp)
1879                     ; 492     uint8_t templ = 0;
1881  0263 7b03          	ld	a,(OFST-2,sp)
1882  0265 97            	ld	xl,a
1883                     ; 495     assert_param(IS_ADC1_BUFFER_OK(Buffer));
1885                     ; 497     if ((ADC1->CR2 & ADC1_CR2_ALIGN) != 0) /* Right alignment */
1887  0266 c65402        	ld	a,21506
1888  0269 a508          	bcp	a,#8
1889  026b 2723          	jreq	L547
1890                     ; 500         templ = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1) + 1);
1892  026d 7b06          	ld	a,(OFST+1,sp)
1893  026f 48            	sll	a
1894  0270 5f            	clrw	x
1895  0271 97            	ld	xl,a
1896  0272 d653e1        	ld	a,(21473,x)
1897  0275 6b03          	ld	(OFST-2,sp),a
1898                     ; 502         temph = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1));
1900  0277 7b06          	ld	a,(OFST+1,sp)
1901  0279 48            	sll	a
1902  027a 5f            	clrw	x
1903  027b 97            	ld	xl,a
1904  027c d653e0        	ld	a,(21472,x)
1905  027f 5f            	clrw	x
1906  0280 97            	ld	xl,a
1907  0281 1f04          	ldw	(OFST-1,sp),x
1908                     ; 504         temph = (uint16_t)(templ | (uint16_t)(temph << (uint8_t)8));
1910  0283 1e04          	ldw	x,(OFST-1,sp)
1911  0285 4f            	clr	a
1912  0286 02            	rlwa	x,a
1913  0287 01            	rrwa	x,a
1914  0288 1a03          	or	a,(OFST-2,sp)
1915  028a 02            	rlwa	x,a
1916  028b 1f04          	ldw	(OFST-1,sp),x
1917  028d 01            	rrwa	x,a
1919  028e 202b          	jra	L747
1920  0290               L547:
1921                     ; 509         temph = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1));
1923  0290 7b06          	ld	a,(OFST+1,sp)
1924  0292 48            	sll	a
1925  0293 5f            	clrw	x
1926  0294 97            	ld	xl,a
1927  0295 d653e0        	ld	a,(21472,x)
1928  0298 5f            	clrw	x
1929  0299 97            	ld	xl,a
1930  029a 1f04          	ldw	(OFST-1,sp),x
1931                     ; 511         templ = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1) + 1);
1933  029c 7b06          	ld	a,(OFST+1,sp)
1934  029e 48            	sll	a
1935  029f 5f            	clrw	x
1936  02a0 97            	ld	xl,a
1937  02a1 d653e1        	ld	a,(21473,x)
1938  02a4 6b03          	ld	(OFST-2,sp),a
1939                     ; 513         temph = (uint16_t)((uint16_t)((uint16_t)templ << 6) | (uint16_t)(temph << 8));
1941  02a6 1e04          	ldw	x,(OFST-1,sp)
1942  02a8 4f            	clr	a
1943  02a9 02            	rlwa	x,a
1944  02aa 1f01          	ldw	(OFST-4,sp),x
1945  02ac 7b03          	ld	a,(OFST-2,sp)
1946  02ae 97            	ld	xl,a
1947  02af a640          	ld	a,#64
1948  02b1 42            	mul	x,a
1949  02b2 01            	rrwa	x,a
1950  02b3 1a02          	or	a,(OFST-3,sp)
1951  02b5 01            	rrwa	x,a
1952  02b6 1a01          	or	a,(OFST-4,sp)
1953  02b8 01            	rrwa	x,a
1954  02b9 1f04          	ldw	(OFST-1,sp),x
1955  02bb               L747:
1956                     ; 516     return ((uint16_t)temph);
1958  02bb 1e04          	ldw	x,(OFST-1,sp)
1961  02bd 5b06          	addw	sp,#6
1962  02bf 81            	ret
2028                     ; 526 FlagStatus ADC1_GetAWDChannelStatus(ADC1_Channel_TypeDef Channel)
2028                     ; 527 {
2029                     	switch	.text
2030  02c0               _ADC1_GetAWDChannelStatus:
2032  02c0 88            	push	a
2033  02c1 88            	push	a
2034       00000001      OFST:	set	1
2037                     ; 528     uint8_t status = 0;
2039  02c2 0f01          	clr	(OFST+0,sp)
2040                     ; 531     assert_param(IS_ADC1_CHANNEL_OK(Channel));
2042                     ; 533     if (Channel < (uint8_t)8)
2044  02c4 a108          	cp	a,#8
2045  02c6 2412          	jruge	L3001
2046                     ; 535         status = (uint8_t)(ADC1->AWSRL & (uint8_t)((uint8_t)1 << Channel));
2048  02c8 5f            	clrw	x
2049  02c9 97            	ld	xl,a
2050  02ca a601          	ld	a,#1
2051  02cc 5d            	tnzw	x
2052  02cd 2704          	jreq	L601
2053  02cf               L011:
2054  02cf 48            	sll	a
2055  02d0 5a            	decw	x
2056  02d1 26fc          	jrne	L011
2057  02d3               L601:
2058  02d3 c4540d        	and	a,21517
2059  02d6 6b01          	ld	(OFST+0,sp),a
2061  02d8 2014          	jra	L5001
2062  02da               L3001:
2063                     ; 539         status = (uint8_t)(ADC1->AWSRH & (uint8_t)((uint8_t)1 << (Channel - (uint8_t)8)));
2065  02da 7b02          	ld	a,(OFST+1,sp)
2066  02dc a008          	sub	a,#8
2067  02de 5f            	clrw	x
2068  02df 97            	ld	xl,a
2069  02e0 a601          	ld	a,#1
2070  02e2 5d            	tnzw	x
2071  02e3 2704          	jreq	L211
2072  02e5               L411:
2073  02e5 48            	sll	a
2074  02e6 5a            	decw	x
2075  02e7 26fc          	jrne	L411
2076  02e9               L211:
2077  02e9 c4540c        	and	a,21516
2078  02ec 6b01          	ld	(OFST+0,sp),a
2079  02ee               L5001:
2080                     ; 542     return ((FlagStatus)status);
2082  02ee 7b01          	ld	a,(OFST+0,sp)
2085  02f0 85            	popw	x
2086  02f1 81            	ret
2244                     ; 551 FlagStatus ADC1_GetFlagStatus(ADC1_Flag_TypeDef Flag)
2244                     ; 552 {
2245                     	switch	.text
2246  02f2               _ADC1_GetFlagStatus:
2248  02f2 88            	push	a
2249  02f3 88            	push	a
2250       00000001      OFST:	set	1
2253                     ; 553     uint8_t flagstatus = 0;
2255  02f4 7b01          	ld	a,(OFST+0,sp)
2256  02f6 97            	ld	xl,a
2257                     ; 554     uint8_t temp = 0;
2259  02f7 7b01          	ld	a,(OFST+0,sp)
2260  02f9 97            	ld	xl,a
2261                     ; 557     assert_param(IS_ADC1_FLAG_OK(Flag));
2263                     ; 559     if ((Flag & 0x0F) == 0x01)
2265  02fa 7b02          	ld	a,(OFST+1,sp)
2266  02fc a40f          	and	a,#15
2267  02fe a101          	cp	a,#1
2268  0300 2609          	jrne	L5701
2269                     ; 562         flagstatus = (uint8_t)(ADC1->CR3 & ADC1_CR3_OVR);
2271  0302 c65403        	ld	a,21507
2272  0305 a440          	and	a,#64
2273  0307 6b01          	ld	(OFST+0,sp),a
2275  0309 2045          	jra	L7701
2276  030b               L5701:
2277                     ; 564     else if ((Flag & 0xF0) == 0x10)
2279  030b 7b02          	ld	a,(OFST+1,sp)
2280  030d a4f0          	and	a,#240
2281  030f a110          	cp	a,#16
2282  0311 2636          	jrne	L1011
2283                     ; 567         temp = (uint8_t)(Flag & (uint8_t)0x0F);
2285  0313 7b02          	ld	a,(OFST+1,sp)
2286  0315 a40f          	and	a,#15
2287  0317 6b01          	ld	(OFST+0,sp),a
2288                     ; 568         if (temp < 8)
2290  0319 7b01          	ld	a,(OFST+0,sp)
2291  031b a108          	cp	a,#8
2292  031d 2414          	jruge	L3011
2293                     ; 570             flagstatus = (uint8_t)(ADC1->AWSRL & (uint8_t)((uint8_t)1 << temp));
2295  031f 7b01          	ld	a,(OFST+0,sp)
2296  0321 5f            	clrw	x
2297  0322 97            	ld	xl,a
2298  0323 a601          	ld	a,#1
2299  0325 5d            	tnzw	x
2300  0326 2704          	jreq	L021
2301  0328               L221:
2302  0328 48            	sll	a
2303  0329 5a            	decw	x
2304  032a 26fc          	jrne	L221
2305  032c               L021:
2306  032c c4540d        	and	a,21517
2307  032f 6b01          	ld	(OFST+0,sp),a
2309  0331 201d          	jra	L7701
2310  0333               L3011:
2311                     ; 574             flagstatus = (uint8_t)(ADC1->AWSRH & (uint8_t)((uint8_t)1 << (temp - 8)));
2313  0333 7b01          	ld	a,(OFST+0,sp)
2314  0335 a008          	sub	a,#8
2315  0337 5f            	clrw	x
2316  0338 97            	ld	xl,a
2317  0339 a601          	ld	a,#1
2318  033b 5d            	tnzw	x
2319  033c 2704          	jreq	L421
2320  033e               L621:
2321  033e 48            	sll	a
2322  033f 5a            	decw	x
2323  0340 26fc          	jrne	L621
2324  0342               L421:
2325  0342 c4540c        	and	a,21516
2326  0345 6b01          	ld	(OFST+0,sp),a
2327  0347 2007          	jra	L7701
2328  0349               L1011:
2329                     ; 579         flagstatus = (uint8_t)(ADC1->CSR & Flag);
2331  0349 c65400        	ld	a,21504
2332  034c 1402          	and	a,(OFST+1,sp)
2333  034e 6b01          	ld	(OFST+0,sp),a
2334  0350               L7701:
2335                     ; 581     return ((FlagStatus)flagstatus);
2337  0350 7b01          	ld	a,(OFST+0,sp)
2340  0352 85            	popw	x
2341  0353 81            	ret
2385                     ; 591 void ADC1_ClearFlag(ADC1_Flag_TypeDef Flag)
2385                     ; 592 {
2386                     	switch	.text
2387  0354               _ADC1_ClearFlag:
2389  0354 88            	push	a
2390  0355 88            	push	a
2391       00000001      OFST:	set	1
2394                     ; 593     uint8_t temp = 0;
2396  0356 0f01          	clr	(OFST+0,sp)
2397                     ; 596     assert_param(IS_ADC1_FLAG_OK(Flag));
2399                     ; 598     if ((Flag & 0x0F) == 0x01)
2401  0358 a40f          	and	a,#15
2402  035a a101          	cp	a,#1
2403  035c 2606          	jrne	L3311
2404                     ; 601         ADC1->CR3 &= (uint8_t)(~ADC1_CR3_OVR);
2406  035e 721d5403      	bres	21507,#6
2408  0362 204b          	jra	L5311
2409  0364               L3311:
2410                     ; 603     else if ((Flag & 0xF0) == 0x10)
2412  0364 7b02          	ld	a,(OFST+1,sp)
2413  0366 a4f0          	and	a,#240
2414  0368 a110          	cp	a,#16
2415  036a 263a          	jrne	L7311
2416                     ; 606         temp = (uint8_t)(Flag & (uint8_t)0x0F);
2418  036c 7b02          	ld	a,(OFST+1,sp)
2419  036e a40f          	and	a,#15
2420  0370 6b01          	ld	(OFST+0,sp),a
2421                     ; 607         if (temp < 8)
2423  0372 7b01          	ld	a,(OFST+0,sp)
2424  0374 a108          	cp	a,#8
2425  0376 2416          	jruge	L1411
2426                     ; 609             ADC1->AWSRL &= (uint8_t)~(uint8_t)((uint8_t)1 << temp);
2428  0378 7b01          	ld	a,(OFST+0,sp)
2429  037a 5f            	clrw	x
2430  037b 97            	ld	xl,a
2431  037c a601          	ld	a,#1
2432  037e 5d            	tnzw	x
2433  037f 2704          	jreq	L231
2434  0381               L431:
2435  0381 48            	sll	a
2436  0382 5a            	decw	x
2437  0383 26fc          	jrne	L431
2438  0385               L231:
2439  0385 43            	cpl	a
2440  0386 c4540d        	and	a,21517
2441  0389 c7540d        	ld	21517,a
2443  038c 2021          	jra	L5311
2444  038e               L1411:
2445                     ; 613             ADC1->AWSRH &= (uint8_t)~(uint8_t)((uint8_t)1 << (temp - 8));
2447  038e 7b01          	ld	a,(OFST+0,sp)
2448  0390 a008          	sub	a,#8
2449  0392 5f            	clrw	x
2450  0393 97            	ld	xl,a
2451  0394 a601          	ld	a,#1
2452  0396 5d            	tnzw	x
2453  0397 2704          	jreq	L631
2454  0399               L041:
2455  0399 48            	sll	a
2456  039a 5a            	decw	x
2457  039b 26fc          	jrne	L041
2458  039d               L631:
2459  039d 43            	cpl	a
2460  039e c4540c        	and	a,21516
2461  03a1 c7540c        	ld	21516,a
2462  03a4 2009          	jra	L5311
2463  03a6               L7311:
2464                     ; 618         ADC1->CSR &= (uint8_t) (~Flag);
2466  03a6 7b02          	ld	a,(OFST+1,sp)
2467  03a8 43            	cpl	a
2468  03a9 c45400        	and	a,21504
2469  03ac c75400        	ld	21504,a
2470  03af               L5311:
2471                     ; 620 }
2474  03af 85            	popw	x
2475  03b0 81            	ret
2530                     ; 640 ITStatus ADC1_GetITStatus(ADC1_IT_TypeDef ITPendingBit)
2530                     ; 641 {
2531                     	switch	.text
2532  03b1               _ADC1_GetITStatus:
2534  03b1 89            	pushw	x
2535  03b2 88            	push	a
2536       00000001      OFST:	set	1
2539                     ; 642     ITStatus itstatus = RESET;
2541  03b3 7b01          	ld	a,(OFST+0,sp)
2542  03b5 97            	ld	xl,a
2543                     ; 643     uint8_t temp = 0;
2545  03b6 7b01          	ld	a,(OFST+0,sp)
2546  03b8 97            	ld	xl,a
2547                     ; 646     assert_param(IS_ADC1_ITPENDINGBIT_OK(ITPendingBit));
2549                     ; 648     if (((uint16_t)ITPendingBit & 0xF0) == 0x10)
2551  03b9 7b02          	ld	a,(OFST+1,sp)
2552  03bb 97            	ld	xl,a
2553  03bc 7b03          	ld	a,(OFST+2,sp)
2554  03be a4f0          	and	a,#240
2555  03c0 5f            	clrw	x
2556  03c1 02            	rlwa	x,a
2557  03c2 a30010        	cpw	x,#16
2558  03c5 2636          	jrne	L5711
2559                     ; 651         temp = (uint8_t)((uint16_t)ITPendingBit & 0x0F);
2561  03c7 7b03          	ld	a,(OFST+2,sp)
2562  03c9 a40f          	and	a,#15
2563  03cb 6b01          	ld	(OFST+0,sp),a
2564                     ; 652         if (temp < 8)
2566  03cd 7b01          	ld	a,(OFST+0,sp)
2567  03cf a108          	cp	a,#8
2568  03d1 2414          	jruge	L7711
2569                     ; 654             itstatus = (ITStatus)(ADC1->AWSRL & (uint8_t)((uint8_t)1 << temp));
2571  03d3 7b01          	ld	a,(OFST+0,sp)
2572  03d5 5f            	clrw	x
2573  03d6 97            	ld	xl,a
2574  03d7 a601          	ld	a,#1
2575  03d9 5d            	tnzw	x
2576  03da 2704          	jreq	L441
2577  03dc               L641:
2578  03dc 48            	sll	a
2579  03dd 5a            	decw	x
2580  03de 26fc          	jrne	L641
2581  03e0               L441:
2582  03e0 c4540d        	and	a,21517
2583  03e3 6b01          	ld	(OFST+0,sp),a
2585  03e5 201d          	jra	L3021
2586  03e7               L7711:
2587                     ; 658             itstatus = (ITStatus)(ADC1->AWSRH & (uint8_t)((uint8_t)1 << (temp - 8)));
2589  03e7 7b01          	ld	a,(OFST+0,sp)
2590  03e9 a008          	sub	a,#8
2591  03eb 5f            	clrw	x
2592  03ec 97            	ld	xl,a
2593  03ed a601          	ld	a,#1
2594  03ef 5d            	tnzw	x
2595  03f0 2704          	jreq	L051
2596  03f2               L251:
2597  03f2 48            	sll	a
2598  03f3 5a            	decw	x
2599  03f4 26fc          	jrne	L251
2600  03f6               L051:
2601  03f6 c4540c        	and	a,21516
2602  03f9 6b01          	ld	(OFST+0,sp),a
2603  03fb 2007          	jra	L3021
2604  03fd               L5711:
2605                     ; 663         itstatus = (ITStatus)(ADC1->CSR & (uint8_t)ITPendingBit);
2607  03fd c65400        	ld	a,21504
2608  0400 1403          	and	a,(OFST+2,sp)
2609  0402 6b01          	ld	(OFST+0,sp),a
2610  0404               L3021:
2611                     ; 665     return ((ITStatus)itstatus);
2613  0404 7b01          	ld	a,(OFST+0,sp)
2616  0406 5b03          	addw	sp,#3
2617  0408 81            	ret
2662                     ; 687 void ADC1_ClearITPendingBit(ADC1_IT_TypeDef ITPendingBit)
2662                     ; 688 {
2663                     	switch	.text
2664  0409               _ADC1_ClearITPendingBit:
2666  0409 89            	pushw	x
2667  040a 88            	push	a
2668       00000001      OFST:	set	1
2671                     ; 689     uint8_t temp = 0;
2673  040b 0f01          	clr	(OFST+0,sp)
2674                     ; 692     assert_param(IS_ADC1_ITPENDINGBIT_OK(ITPendingBit));
2676                     ; 694     if (((uint16_t)ITPendingBit & 0xF0) == 0x10)
2678  040d 01            	rrwa	x,a
2679  040e a4f0          	and	a,#240
2680  0410 5f            	clrw	x
2681  0411 02            	rlwa	x,a
2682  0412 a30010        	cpw	x,#16
2683  0415 263a          	jrne	L7221
2684                     ; 697         temp = (uint8_t)((uint16_t)ITPendingBit & 0x0F);
2686  0417 7b03          	ld	a,(OFST+2,sp)
2687  0419 a40f          	and	a,#15
2688  041b 6b01          	ld	(OFST+0,sp),a
2689                     ; 698         if (temp < 8)
2691  041d 7b01          	ld	a,(OFST+0,sp)
2692  041f a108          	cp	a,#8
2693  0421 2416          	jruge	L1321
2694                     ; 700             ADC1->AWSRL &= (uint8_t)~(uint8_t)((uint8_t)1 << temp);
2696  0423 7b01          	ld	a,(OFST+0,sp)
2697  0425 5f            	clrw	x
2698  0426 97            	ld	xl,a
2699  0427 a601          	ld	a,#1
2700  0429 5d            	tnzw	x
2701  042a 2704          	jreq	L651
2702  042c               L061:
2703  042c 48            	sll	a
2704  042d 5a            	decw	x
2705  042e 26fc          	jrne	L061
2706  0430               L651:
2707  0430 43            	cpl	a
2708  0431 c4540d        	and	a,21517
2709  0434 c7540d        	ld	21517,a
2711  0437 2021          	jra	L5321
2712  0439               L1321:
2713                     ; 704             ADC1->AWSRH &= (uint8_t)~(uint8_t)((uint8_t)1 << (temp - 8));
2715  0439 7b01          	ld	a,(OFST+0,sp)
2716  043b a008          	sub	a,#8
2717  043d 5f            	clrw	x
2718  043e 97            	ld	xl,a
2719  043f a601          	ld	a,#1
2720  0441 5d            	tnzw	x
2721  0442 2704          	jreq	L261
2722  0444               L461:
2723  0444 48            	sll	a
2724  0445 5a            	decw	x
2725  0446 26fc          	jrne	L461
2726  0448               L261:
2727  0448 43            	cpl	a
2728  0449 c4540c        	and	a,21516
2729  044c c7540c        	ld	21516,a
2730  044f 2009          	jra	L5321
2731  0451               L7221:
2732                     ; 709         ADC1->CSR &= (uint8_t)((uint16_t)~(uint16_t)ITPendingBit);
2734  0451 7b03          	ld	a,(OFST+2,sp)
2735  0453 43            	cpl	a
2736  0454 c45400        	and	a,21504
2737  0457 c75400        	ld	21504,a
2738  045a               L5321:
2739                     ; 711 }
2742  045a 5b03          	addw	sp,#3
2743  045c 81            	ret
2756                     	xdef	_ADC1_ClearITPendingBit
2757                     	xdef	_ADC1_GetITStatus
2758                     	xdef	_ADC1_ClearFlag
2759                     	xdef	_ADC1_GetFlagStatus
2760                     	xdef	_ADC1_GetAWDChannelStatus
2761                     	xdef	_ADC1_GetBufferValue
2762                     	xdef	_ADC1_SetLowThreshold
2763                     	xdef	_ADC1_SetHighThreshold
2764                     	xdef	_ADC1_GetConversionValue
2765                     	xdef	_ADC1_StartConversion
2766                     	xdef	_ADC1_AWDChannelConfig
2767                     	xdef	_ADC1_ExternalTriggerConfig
2768                     	xdef	_ADC1_ConversionConfig
2769                     	xdef	_ADC1_SchmittTriggerConfig
2770                     	xdef	_ADC1_PrescalerConfig
2771                     	xdef	_ADC1_ITConfig
2772                     	xdef	_ADC1_DataBufferCmd
2773                     	xdef	_ADC1_ScanModeCmd
2774                     	xdef	_ADC1_Cmd
2775                     	xdef	_ADC1_Init
2776                     	xdef	_ADC1_DeInit
2795                     	end
