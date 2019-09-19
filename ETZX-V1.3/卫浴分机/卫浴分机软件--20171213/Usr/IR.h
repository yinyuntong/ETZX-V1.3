/*
*-----------------------�ļ���Ϣ---------------------------
*�ļ�����     		:IR.h
*�ļ�����    		:������ճ���ͷ�ļ�
*������     		:�ܼ�ǿ
*��������   		:2011-2-16
*�汾��       		:
*ע��	     		:
*----------------------------------------------------------
*�޸���  			:
*�޸�����  			:
*�汾��        		:
*ע��	     		:
***********************************************************
*/
#ifndef _IR_H_
#define _IR_H_

#define IrDA_CMD_NURSE_IN   0x01							//����ӿڻ�ʿ��λ����
#define IrDA_CMD_INSPECTION	0x02							//����ӿڲ鷿����


#define NURSE_IN_TIME	350								//��ʿ��λ�ź���Чʱ��
#define MOVE_FJ				251								//�ƶ��ֻ�



#ifndef _IN_IR_

extern uint8 xdata MoveFjNum;		//�ƶ��ֻ�����

extern uint8 xdata UartRecData;
extern uint8 xdata IrDataPosit;

extern uint16  uiNurseInTime;						//��⻤ʿ��λ��ʱʱ��
extern uint8 	byIrDataIntervalTimer;				//�������ݽ��չ������ֽ�֮����ʱ��
extern uint8 	byNumSetTime;						//���ʱ��

extern uint8 xdata byIrDARecBuff[], byIrDADealBuff[];		//�������������������

extern bit bFourByteRec;					//���ڽ��յ�4�ֽ�����
extern bit bSixByteRec;					//���ڽ��յ�6�ֽ����


extern bit	bSetKeyDown ;				//���ü�����״̬
extern bit	bNurseIn;	 			//��ʿ��λ��־


extern void IRInit(void);
extern void IrDATreat(void);	//���ݴ�����
extern void IrDANumberSet(void);	//���ݴ�����

#endif



#endif
