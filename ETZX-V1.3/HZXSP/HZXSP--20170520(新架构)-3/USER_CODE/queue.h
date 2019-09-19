
#ifndef _QUEUE_H_
#define _QUEUE_H_

#define MAX_SIZE 100

typedef struct queue
{
	INT8U qu[MAX_SIZE];
	INT8U front;
	INT8U rear;
	INT8U tag;// front == rear时表示队列满或空的标志:tag==1表示满，tag=0表示空
	
}Queue;





#endif




