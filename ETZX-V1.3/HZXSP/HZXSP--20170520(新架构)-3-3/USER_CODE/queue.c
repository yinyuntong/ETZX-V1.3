


#include <stdio.h>
#include <malloc.h>

#include "queue.h"

Queue *InitQu()
{
	Queue *q;
	q= (Queue *)malloc(sizeof(Queue));		//分配空间
	q->front =0;
	q->rear =0;
	q->tag =0;
	return q;
}

INT8U EnQu(Queue *q,INT8U x)
{
	if(q->rear== q->front && q->tag==1) //表示队列满
		return 0;
	else
	{
		q->qu[q->rear]=x;
		q->rear= (q->rear+1)%MAX_SIZE;	//尾指针向后走一步
		if(q->rear == q->front)	//若有元素入队以后出现q->rear ==q->front,则表示队列满
			q->tag =1;
		return 1;
	}
}



INT8U DeQu(Queue *q,INT8U *dat)
{
	if(q->rear==q->front &&q->tag ==0)//表示队列空
		return 0;
	else
	{
		*dat = q->qu[q->front];		//取得数据
		q->qu[q->front]=0;			//将队头元素的值赋值为0
		q->front = (q->front+1)%MAX_SIZE;	//头指针向后走一步
		if(q->rear == q->front)   //苦有元素出队以后出现q->rear==q->front,则表示队空
			q->tag =0;
		return 1;
	}
}

