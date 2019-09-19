#include"custom.h"

//���庯��ָ��  
void (*IAP_Entry) (uint32_t ulParam_tab[], uint32_t ulPesult_tab[]);
//*/

uint32_t  gul_para_in[8];                                                  /* IAP��ڲ���������            */
uint32_t  gul_para_out[8];                                                 /* IAP���ڲ���������            */
/*********************************************************************************************************
** Function name:       sector_prepare
** Descriptions:        IAP��������ѡ���������50
** input parameters:    ucSec1:           ��ʼ����
**                      ucSec2:           ��ֹ����
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ     
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  sector_prepare (uint8_t ucSec1, uint8_t ucSec2)
{  
    gul_para_in[0] = IAP_Prepare;                                        /* ����������                   */
    gul_para_in[1] = ucSec1;                                             /* ���ò���                     */
    gul_para_in[2] = ucSec2;                            
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */
   
    return (gul_para_out[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       ram_copy
** Descriptions:        ����RAM�����ݵ�FLASH���������51
** input parameters:    ulDst:             Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**                      ulSrc:             Դ��ַ����RAM��ַ����ַ�����ֶ���
**                      ulNo:              �����ֽڸ�����Ϊ512/1024/4096/8192
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ     
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  ram_copy (uint32_t ulDst, uint32_t ulSrc, uint32_t ulNo)
{  
    gul_para_in[0] = IAP_RAMTOFLASH;                                     /* ����������                   */
    gul_para_in[1] = ulDst;                                              /* ���ò���                     */
    gul_para_in[2] = ulSrc;
    gul_para_in[3] = ulNo;
    gul_para_in[4] = IAP_FCCLK;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */
    
    return (gul_para_out[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       sector_erase
** Descriptions:        �����������������52
** input parameters:    ucSec1             ��ʼ����
**                      ucSec2             ��ֹ����92
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  sector_erase (uint8_t ucSec1, uint8_t ucSec2)
{  
    gul_para_in[0] = IAP_ERASESECTOR;                                    /* ����������                   */
    gul_para_in[1] = ucSec1;                                             /* ���ò���                     */
    gul_para_in[2] = ucSec2;
    gul_para_in[3] = IAP_FCCLK;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */
   
    return (gul_para_out[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       bland_chk
** Descriptions:        ������գ��������53
** input parameters:    ucSec1:              ��ʼ����
**                      ucSec2:              ��ֹ����92
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  bland_chk (uint8_t ucSec1, uint8_t ucSec2)
{  
    gul_para_in[0] = IAP_BLAND_CHK;                                       /* ����������                   */
    gul_para_in[1] = ucSec1;                                             /* ���ò���                     */
    gul_para_in[2] = ucSec2;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */

    return (gul_para_out[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       parId_read
** Descriptions:        ������գ��������54
** input parameters:    ��
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  parId_read (void)
{  
    gul_para_in[0] = IAP_READPARTID;                                     /* ����������                   */
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */

    return (gul_para_out[0]);                                            /* ����״̬��                   */
}
/*********************************************************************************************************
** Function name:      
** Descriptions:        ��ȡ���кţ��������58
** input parameters:    ��
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  read_serial_code (void)
{  
    gul_para_in[0] = Read_chip_serial;                                     /* ����������                   */
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */

	return (gul_para_out[0]);  
}
/*********************************************************************************************************
** Function name:       codeId_boot
** Descriptions:        ������գ��������55
** input parameters:    ��
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  codeId_boot (void)
{  
    gul_para_in[0] = IAP_BOOTCODEID;                                     /* ����������                   */
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */

    return (gul_para_out[0]);                                            /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       data_compare
** Descriptions:        У�����ݣ��������56
** input parameters:    ulDst:             Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���
**                      ulSrc:             Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
**                      ulNo:              �����ֽڸ����������ܱ�4����
** output parameters:   gul_para_out[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      gul_para_out[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
uint32_t  data_compare (uint32_t ulDst, uint32_t ulSrc, uint32_t ulNo)
{  
    gul_para_in[0] = IAP_COMPARE;                                        /* ����������                   */
    gul_para_in[1] = ulDst;                                              /* ���ò���                     */
    gul_para_in[2] = ulSrc;
    gul_para_in[3] = ulNo;
    (*IAP_Entry)(gul_para_in, gul_para_out);                              /* ����IAP�������              */

    return (gul_para_out[0]);                                            /* ����״̬��                   */
}

