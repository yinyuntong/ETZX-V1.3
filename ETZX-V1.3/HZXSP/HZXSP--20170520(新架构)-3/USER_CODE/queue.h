
#ifndef _QUEUE_H_
#define _QUEUE_H_

#define MAX_SIZE 100

typedef struct queue
{
	INT8U qu[MAX_SIZE];
	INT8U front;
	INT8U rear;
	INT8U tag;// front == rearʱ��ʾ��������յı�־:tag==1��ʾ����tag=0��ʾ��
	
}Queue;





#endif




