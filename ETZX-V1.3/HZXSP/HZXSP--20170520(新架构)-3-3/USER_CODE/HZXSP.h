#ifndef _KTKZQ_H_
#define _KTKZQ_H_

#define DISPLAY_STATE_CALL 0x01 //�к�����Ϣ
#define DISPLAY_STATE_NURSE 0x02  //�е�λ��Ϣ
#define DISPLAY_STATE_MESSAGE 0x04 //����Ϣ����
#define DISPLAY_STATE_TIME 0x08 //������ʾʱ��
#define DISPLAY_STATE_CHANGEING 0x10 //�л���ʾ	//ʵ���Ͼ���ʱ�����ӵı仯,����ʱ����ʾ
#define DISPLAY_STATE_CHANGED 0x20 //�б任��ʾ����
#define DISPLAY_STATE_TIME_CHANGED 0x40 //��ʾ�仯


#define MESG_BUF_LEN 252


typedef struct {
	INT8U MessageBuf[MESG_BUF_LEN];
	INT8U color;
	INT8U  byCrc;	//�������ݱ�����EEPROM�й�256�ֽ�

	
	INT16U MessageIndex;
	INT16U MessageShowLen;
	INT8U state;
	INT8U CallBuf[6];	//���浥��������Ϣ
	INT8U  CallMessageBuf[MESG_BUF_LEN];	//������ʾ��Ϣ������
	INT16U CallShowLen;
	INT16U CallIndex;
	INT8U NurseBuf[20];
	INT16U NurseShowLen;
	INT16U NurseIndex;
	INT16U ErrorShowLen;
	INT16U ErrorIndex;
	INT8U ChangeDelay; //�л���ʱ
}DISPLAYINFO,*pDISPLAYINFO;


typedef struct {
	INT8U CallType;
	INT8U CallValue;
}SLAVECALL;


#define BEDTYPE	1
#define WCTYPE	2
#define ADDBEDTYPE	3

#define MESSAGE_CHANGED_DELAY 30 //30S

//ϵͳ״̬λ����

#define SYSTEM_STATE_LOCAL_ERROR 0x01	   //��������
#define SYSTEM_STATE_DISPINFO 0x02  //�Ƿ���ʾ��Ϣ
#define SYSTEM_STATE_ONLINE 0x04 //�Ƿ�������״̬
#define SYSTEM_STATE_MOVING 0x10 //����ִ���ƶ�
#define SYSTEM_STATE_TESTMODE 0x20 //ִ�в���ģʽ
#define SYSTEM_STATE_BUS_ERROR 0x40 //���ߴ���


#define SYSTEM_SAVE_SIZE 256 //ϵͳ��Ϣ�洢����
typedef struct {
	INT8U year;
	INT8U month;
	INT8U day;
	INT8U hour;
	INT8U minute;
	INT8U second;
}DATETIME, *pDATETIME; 

typedef struct system_info
{
	INT8U UartBaud;/*ͨ������*/
	INT8U LOCAL_ADDR;   //��������
	DISPLAYINFO DisplayInfo; //��ʾ��Ϣ
	INT16U state; //ϵͳ״̬
	INT32U error; //ϵͳ�����ʱ
	DATETIME sTime; //ϵͳʱ��
}SYSTEM_INFO;

extern SYSTEM_INFO SystemInfo;	 //ϵͳ��Ϣ
#endif

