/*
************************Copyright(c)************************
*	  			   湖南熙旺达科技有限公司
*			       	All Rights Reserved
*			     		 
*
*-----------------------文件信息---------------------------
*文件名称     		:IAP.h
*文件描述    		:写Flash驱动头文件
*创建人     		:陈卫国
*创建日期   		:2008-9-22
*版本号       		:V1.0
*注释	     		:采用STC系列CPU,该类型CPU将内部Flash作
				     EEProm使用,擦除需以扇区为单位,在进行
				     IAP操作过程中,CPU按照设定的时间挂起,
				     此时CPU不能够进行任何工作
*----------------------------------------------------------
*修改人  			:
*修改日期  			:
*版本号        		:
*注释	     		:
***********************************************************
*/
#define		IAP_READ			0x01						//IAP读命令
#define		IAP_WRITE			0x02						//IAP写命令
#define		IAP_ERASE			0x03						//IAP擦除命令
#define		IAP_WAIT_TIME		0x02						//IAP等待时间

#ifndef _IN_IAP_
#define _IN_IAP_
void EnableIAP(uint8 byCmd);
void DisableIAP(void);
uint8 IapReadByte(uint16 uiAddr);
void IapWriteByte(uint16 uiAddr, uint8 byData);
void IapErase(uint16 uiAddr);
#endif