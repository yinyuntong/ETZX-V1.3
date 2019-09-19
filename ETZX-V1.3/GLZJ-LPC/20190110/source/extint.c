#define _IN_EXTINT_
#include"custom.h"

void eint0Init (void);
void eint1Init (void);
void eint2Init (void);
void eint3Init (void);
int32_t zyIsrSet (unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio);


/*********************************************************************************************************
** Function name:       eint0Init
** Descriptions:        外部中断0初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void eint0Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 20);								//p210
//	LPC_PINCON->PINMODE4 &= (~(0x03<<20));	//复位值为00：接上拉电阻

    //LPC_SC->EXTMODE  &=  0xFE; 		//外部中断0选择为电平触发    
	LPC_SC->EXTMODE  |=  0x01;	// 选择为边沿触发 
	   
    LPC_SC->EXTPOLAR &=  0xFE;  // 选择为低电平或下降沿触发中断          
	//LPC_SC->EXTPOLAR |= 0x01;     // 选择为高电平或上升沿触发中断  
	      
    LPC_SC->EXTINT    |=  0x01;          // 清除外部中断0的中断标志     

	NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_SetPriority(EINT0_IRQn, 8); // 设置外部中断并使能            
}




/*********************************************************************************************************
** Function name:       eint0Init
** Descriptions:        外部中断0初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void eint1Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 22);								//p211
//	LPC_PINCON->PINMODE4 &= (~(0x03<<22));   //复位值为00：接上拉电阻
    //LPC_SC->EXTMODE  &=  0xFD;			// 外部中断方式选择		0电平触发     1边沿触发 
    //LPC_SC->EXTPOLAR &=  0xFD;	        // 0选择为低电平或下降沿触发中断         1选择为高电平或上升沿触发中断      
	LPC_SC->EXTMODE  |=  0x02;        		// 外部中断1选择为边沿触发      
    LPC_SC->EXTPOLAR &=  0xFD;				// 下降沿触发中断  
	             
    LPC_SC->EXTINT    |=  0x02;     		// 清除外部中断1的中断标志      

	NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_SetPriority(EINT1_IRQn, 1); 	// 设置外部中断并使能      
}


/*********************************************************************************************************
** Function name:       eint0Init
** Descriptions:        外部中断0初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void eint2Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 24);								//p212
//	LPC_PINCON->PINMODE4 &= (~(0x03<<24));     //复位值为00：接上拉电阻

    //LPC_SC->EXTMODE  &=  0xFB;	// 外部中断2选择为电平触发  
	LPC_SC->EXTMODE  |=  0x04;	// 外部中断2  1选择为边沿触发 

    //LPC_SC->EXTPOLAR &=  0xFB;	// 0选择为低电平或下降沿触发中断    
	LPC_SC->EXTPOLAR |=  0x04;	// 1选择为高电平或上升沿触发中断   
          
	LPC_SC->EXTINT    |=  0x04; 	// 清除外部中断2的中断标志      

	NVIC_EnableIRQ(EINT2_IRQn);
    NVIC_SetPriority(EINT2_IRQn,9);                                    /* 设置外部中断并使能           */ 
}
/*********************************************************************************************************
** Function name:       eint3Init
** Descriptions:        外部中断0初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void eint3Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 26);		//p213
//	LPC_PINCON->PINMODE4 &= (~(0x03<<26));		//引脚使能片内上拉电阻

	//LPC_SC->EXTMODE  &=	 0xf7;					// 外部中断0选择为电平触发
    LPC_SC->EXTMODE  |=  0x08;                	//1选择为边沿触发 

	LPC_SC->EXTPOLAR &=  0xf7;	             	// 0选择为低电平或下降沿触发中断
    //LPC_SC->EXTPOLAR |=  0x08;	             	// 1选择为高电平或上升沿触发中断 
	                
    LPC_SC->EXTINT    |=  0x08;                	// 清除外部中断3的中断标志     

	NVIC_EnableIRQ(EINT3_IRQn);
    NVIC_SetPriority(EINT3_IRQn, 1);   	// 设置外部中断并使能          
}

/*********************************************************************************************************
** Function name:			zyIsrSet
** Descriptions:			IO系统设置中断服务程序
** input parameters:		uiChannel:	中断通道号
**							ulFunction: 中断服务程序地址
**							uiPrio: 	中断优先级
** output parameters:		none
** Returned value:			zy_OK: 成功
**							负数:  错误,绝对值参考zy_if.h
** Created by:				Chenmingji
** Created Date:			2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
int32_t zyIsrSet (unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio)
{
	unsigned int uiTmp1, uiTmp2, uiTmp3;
	
	if (uiChannel > MAX_VICS) 
	{
		return -ZY_NOT_OK;
	}

	//zyIrqDisable();

	if (uiChannel >= 16) 
	{
		uiTmp3 = uiChannel - 16;
		uiTmp1 = uiTmp3 / 32;
		uiTmp2 = uiTmp3 % 32;


        ((int32_t *)0xE000E100)[uiTmp1] = 1 << uiTmp2;
        ((int8_t *)0xE000E400)[uiTmp3]  = uiPrio;
    } 
	else 
	{
        switch (uiChannel) 
		{
        
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;

        default:
            break;
        }
        if (uiChannel >= MMI) 
		{
            uiTmp3 = uiChannel - MMI;
            ((int8_t *)0xE000ED18)[uiTmp3]  = uiPrio;
        }
    }

//#if VECTOR_TABLE_IN_FLASH == 0
//    ((unsigned long *)VTOR)[uiChannel] = ulFunction;
//#endif
//    zyIrqEnable();
    return ZY_OK;
}


