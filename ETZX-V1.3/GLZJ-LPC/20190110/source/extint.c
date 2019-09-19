#define _IN_EXTINT_
#include"custom.h"

void eint0Init (void);
void eint1Init (void);
void eint2Init (void);
void eint3Init (void);
int32_t zyIsrSet (unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio);


/*********************************************************************************************************
** Function name:       eint0Init
** Descriptions:        �ⲿ�ж�0��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void eint0Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 20);								//p210
//	LPC_PINCON->PINMODE4 &= (~(0x03<<20));	//��λֵΪ00������������

    //LPC_SC->EXTMODE  &=  0xFE; 		//�ⲿ�ж�0ѡ��Ϊ��ƽ����    
	LPC_SC->EXTMODE  |=  0x01;	// ѡ��Ϊ���ش��� 
	   
    LPC_SC->EXTPOLAR &=  0xFE;  // ѡ��Ϊ�͵�ƽ���½��ش����ж�          
	//LPC_SC->EXTPOLAR |= 0x01;     // ѡ��Ϊ�ߵ�ƽ�������ش����ж�  
	      
    LPC_SC->EXTINT    |=  0x01;          // ����ⲿ�ж�0���жϱ�־     

	NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_SetPriority(EINT0_IRQn, 8); // �����ⲿ�жϲ�ʹ��            
}




/*********************************************************************************************************
** Function name:       eint0Init
** Descriptions:        �ⲿ�ж�0��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void eint1Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 22);								//p211
//	LPC_PINCON->PINMODE4 &= (~(0x03<<22));   //��λֵΪ00������������
    //LPC_SC->EXTMODE  &=  0xFD;			// �ⲿ�жϷ�ʽѡ��		0��ƽ����     1���ش��� 
    //LPC_SC->EXTPOLAR &=  0xFD;	        // 0ѡ��Ϊ�͵�ƽ���½��ش����ж�         1ѡ��Ϊ�ߵ�ƽ�������ش����ж�      
	LPC_SC->EXTMODE  |=  0x02;        		// �ⲿ�ж�1ѡ��Ϊ���ش���      
    LPC_SC->EXTPOLAR &=  0xFD;				// �½��ش����ж�  
	             
    LPC_SC->EXTINT    |=  0x02;     		// ����ⲿ�ж�1���жϱ�־      

	NVIC_EnableIRQ(EINT1_IRQn);
    NVIC_SetPriority(EINT1_IRQn, 1); 	// �����ⲿ�жϲ�ʹ��      
}


/*********************************************************************************************************
** Function name:       eint0Init
** Descriptions:        �ⲿ�ж�0��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void eint2Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 24);								//p212
//	LPC_PINCON->PINMODE4 &= (~(0x03<<24));     //��λֵΪ00������������

    //LPC_SC->EXTMODE  &=  0xFB;	// �ⲿ�ж�2ѡ��Ϊ��ƽ����  
	LPC_SC->EXTMODE  |=  0x04;	// �ⲿ�ж�2  1ѡ��Ϊ���ش��� 

    //LPC_SC->EXTPOLAR &=  0xFB;	// 0ѡ��Ϊ�͵�ƽ���½��ش����ж�    
	LPC_SC->EXTPOLAR |=  0x04;	// 1ѡ��Ϊ�ߵ�ƽ�������ش����ж�   
          
	LPC_SC->EXTINT    |=  0x04; 	// ����ⲿ�ж�2���жϱ�־      

	NVIC_EnableIRQ(EINT2_IRQn);
    NVIC_SetPriority(EINT2_IRQn,9);                                    /* �����ⲿ�жϲ�ʹ��           */ 
}
/*********************************************************************************************************
** Function name:       eint3Init
** Descriptions:        �ⲿ�ж�0��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void eint3Init ()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 26);		//p213
//	LPC_PINCON->PINMODE4 &= (~(0x03<<26));		//����ʹ��Ƭ����������

	//LPC_SC->EXTMODE  &=	 0xf7;					// �ⲿ�ж�0ѡ��Ϊ��ƽ����
    LPC_SC->EXTMODE  |=  0x08;                	//1ѡ��Ϊ���ش��� 

	LPC_SC->EXTPOLAR &=  0xf7;	             	// 0ѡ��Ϊ�͵�ƽ���½��ش����ж�
    //LPC_SC->EXTPOLAR |=  0x08;	             	// 1ѡ��Ϊ�ߵ�ƽ�������ش����ж� 
	                
    LPC_SC->EXTINT    |=  0x08;                	// ����ⲿ�ж�3���жϱ�־     

	NVIC_EnableIRQ(EINT3_IRQn);
    NVIC_SetPriority(EINT3_IRQn, 1);   	// �����ⲿ�жϲ�ʹ��          
}

/*********************************************************************************************************
** Function name:			zyIsrSet
** Descriptions:			IOϵͳ�����жϷ������
** input parameters:		uiChannel:	�ж�ͨ����
**							ulFunction: �жϷ�������ַ
**							uiPrio: 	�ж����ȼ�
** output parameters:		none
** Returned value:			zy_OK: �ɹ�
**							����:  ����,����ֵ�ο�zy_if.h
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


