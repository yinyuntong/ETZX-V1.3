//////////////////////
//����0��������

#define 	BUS0_FREQ_SIZE		7						   	//������0����֡����
typedef     unsigned char   uint8;
extern uint8 bdata byBus0State1 ;//	= 0;  							//������0״̬��2
extern bit  bBus0RecFinish;//		= byBus0State1^0;				//������0������ɱ�־
//sbit  bBus0RecBit9			= byBus0State1^1;				//������0�����ֽ����ݵĵ�9λ
extern bit  bBus0SendFinish;		//= byBus0State1^2;				//������0������ɱ�־
//sbit  bBus0Disable			= byBus0State1^3;				//������0��ֹ���ͱ�־
//sbit  bBus0SendBit			= byBus0State1^4;				//������0���ڷ��͵�����λ
//sbit  bBus0RecBit			= byBus0State1^5; 				//������0���ڽ��յ�����λ
extern uint8 xdata byBus0RecData[BUS0_FREQ_SIZE];					//����0���ջ�����
extern void SingleBusInit(void);
extern uint8 Bus0OutputData(uint8* pbyData);
//////////////////////