#include"custom.h"

//定义函数指针  
void (*IAP_Entry) (uint32_t ulParam_tab[], uint32_t ulPesult_tab[]);
//*/

uint32_t  gul_para_in[8];                                                  /* IAP入口参数缓冲区            */
uint32_t  gul_para_out[8];                                                 /* IAP出口参数缓冲区            */
/*********************************************************************************************************
** Function name:       sector_prepare
** Descriptions:        IAP操作扇区选择，命令代码50
** input parameters:    ucSec1:           起始扇区
**                      ucSec2:           终止扇区
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值     
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  sector_prepare (uint8_t ucSec1, uint8_t ucSec2)
{  
    gul_para_in[0] = IAP_Prepare;                                        /* 设置命令字                   */
    gul_para_in[1] = ucSec1;                                             /* 设置参数                     */
    gul_para_in[2] = ucSec2;                            
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */
   
    return (gul_para_out[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       ram_copy
** Descriptions:        复制RAM的数据到FLASH，命令代码51
** input parameters:    ulDst:             目标地址，即FLASH起始地址。以512字节为分界
**                      ulSrc:             源地址，即RAM地址。地址必须字对齐
**                      ulNo:              复制字节个数，为512/1024/4096/8192
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值     
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  ram_copy (uint32_t ulDst, uint32_t ulSrc, uint32_t ulNo)
{  
    gul_para_in[0] = IAP_RAMTOFLASH;                                     /* 设置命令字                   */
    gul_para_in[1] = ulDst;                                              /* 设置参数                     */
    gul_para_in[2] = ulSrc;
    gul_para_in[3] = ulNo;
    gul_para_in[4] = IAP_FCCLK;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */
    
    return (gul_para_out[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       sector_erase
** Descriptions:        扇区擦除，命令代码52
** input parameters:    ucSec1             起始扇区
**                      ucSec2             终止扇区92
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  sector_erase (uint8_t ucSec1, uint8_t ucSec2)
{  
    gul_para_in[0] = IAP_ERASESECTOR;                                    /* 设置命令字                   */
    gul_para_in[1] = ucSec1;                                             /* 设置参数                     */
    gul_para_in[2] = ucSec2;
    gul_para_in[3] = IAP_FCCLK;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */
   
    return (gul_para_out[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       bland_chk
** Descriptions:        扇区查空，命令代码53
** input parameters:    ucSec1:              起始扇区
**                      ucSec2:              终止扇区92
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  bland_chk (uint8_t ucSec1, uint8_t ucSec2)
{  
    gul_para_in[0] = IAP_BLAND_CHK;                                       /* 设置命令字                   */
    gul_para_in[1] = ucSec1;                                             /* 设置参数                     */
    gul_para_in[2] = ucSec2;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */

    return (gul_para_out[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       parId_read
** Descriptions:        扇区查空，命令代码54
** input parameters:    无
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  parId_read (void)
{  
    gul_para_in[0] = IAP_READPARTID;                                     /* 设置命令字                   */
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */

    return (gul_para_out[0]);                                            /* 返回状态码                   */
}
/*********************************************************************************************************
** Function name:      
** Descriptions:        读取序列号，命令代码58
** input parameters:    无
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  read_serial_code (void)
{  
    gul_para_in[0] = Read_chip_serial;                                     /* 设置命令字                   */
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */

	return (gul_para_out[0]);  
}
/*********************************************************************************************************
** Function name:       codeId_boot
** Descriptions:        扇区查空，命令代码55
** input parameters:    无
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  codeId_boot (void)
{  
    gul_para_in[0] = IAP_BOOTCODEID;                                     /* 设置命令字                   */
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */

    return (gul_para_out[0]);                                            /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       data_compare
** Descriptions:        校验数据，命令代码56
** input parameters:    ulDst:             目标地址，即RAM/FLASH起始地址。地址必须字对齐
**                      ulSrc:             源地址，即FLASH/RAM地址。地址必须字对齐
**                      ulNo:              复制字节个数，必须能被4整除
** output parameters:   gul_para_out[0]:    IAP操作状态码,IAP返回值
** Returned value:      gul_para_out[0]:    IAP操作状态码,IAP返回值                     
*********************************************************************************************************/
uint32_t  data_compare (uint32_t ulDst, uint32_t ulSrc, uint32_t ulNo)
{  
    gul_para_in[0] = IAP_COMPARE;                                        /* 设置命令字                   */
    gul_para_in[1] = ulDst;                                              /* 设置参数                     */
    gul_para_in[2] = ulSrc;
    gul_para_in[3] = ulNo;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* 调用IAP服务程序              */

    return (gul_para_out[0]);                                            /* 返回状态码                   */
}

