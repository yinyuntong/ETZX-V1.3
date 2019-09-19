#ifndef _IN_UART_
#define _IN_UART_

#define UART0_BUF_SIZE		(MESG_BUF_LEN+7+12)//���ڻ�������С����
typedef struct
{
	INT8U 	byState;
	INT8U	byRxHead;
	INT8U	byDealHead;										//һ֡���������ݵ�ͷ��
	INT8U 	byRxTail;
	INT8U   byCrc;
	INT8U   byTimerout;		
	INT8U 	uiLen;
} STUartControl, *pSTUartControl;



typedef struct
{
	INT8U byDataType;										//�������ͣ�0x01��������
	INT8U byInfoType;										//������������,0x01:��ͷ��,0x02:�����嵥,0x03:ҩƷ�嵥
	INT8U byFrameID;										//֡��
	INT8U byEndFlag;										//ҳ������־��0x00:û�н�����0x01:����
} STLcdCtlData, *pSTLcdCtlData;


typedef struct
{
	INT8U	byStartByte1;					//����֡��ʼ�ַ�1
	INT8U 	bySndSecAddr;					//�������ͷ�����ַ
	INT8U 	bySndRoomAddr;					//�������ͷ�����ַ
	INT8U 	bySndBedAddr;					//�������ͷ�����ַ	
	INT8U 	byRecSecAddr;					//�շ�����ַ
	INT8U 	byRecRoomAddr;					//�շ�����ַ
	INT8U 	byRecBedAddr;					//�շ�����ַ
	INT8U 	byCmd;							//����
	//INT16U  uiLen;							//���ݳ��ȣ�2���ֽ�
	INT8U   byLenHigh;						//���ȸ��ֽ�
	INT8U	byLenLow;						//���ȵ��ֽ�
	INT8U   byLcdSec;						//������ʾ������
	INT8U   byLcdRoom;						//������ʾ������
	INT8U   byNull;							//δ��
	STLcdCtlData stLcdCtlData;			//������ʾ��ҳ���������
	INT8U   byData[1];						//��·����	
} STUartFreq, *pSTUartFreq;


#define FREQ_DATA_LEN			19//16						//������·���ݳ���
#define FREQ_HEAD_SIZE			10							//����֡��ͷ����
#define START_BYTE1				0x5a
#define	START_BYTE2				0xa5
#define END_BYTE				0x16  


extern STUartControl stUartControl;								//���ڿ��ƽṹ��

extern void UartInit(INT8U baud);
extern void UartReInit(INT8U baud); 
extern void UartManager(void);
extern void uartSendByte (INT8U ucDat);
extern INT8U CheckPsk(pSTUartFreq pstDataFreq);
#endif

