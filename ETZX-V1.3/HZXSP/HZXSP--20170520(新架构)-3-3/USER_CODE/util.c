/*********************************************************************************
公共函数
*********************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx Peripheral Registers */
#include "dataType.h"

/*********************************************************************************************************
** Function name:       Delay_Us
** Descriptions:        软件延时(Us)
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Delay_Us (uint16_t ulTime)
{
    while (ulTime--){__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}
}												   

/*********************************************************************************************************
** Function name:       Delay_Ms
** Descriptions:        软件延时(Ms)
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Delay_Ms (uint16_t ulTime)
{
    while (ulTime--) {
        Delay_Us(1000);
	    LPC_WDT->FEED  = 0xAA;                                        //喂狗                      
	    LPC_WDT->FEED  = 0x55; 
    }
}
