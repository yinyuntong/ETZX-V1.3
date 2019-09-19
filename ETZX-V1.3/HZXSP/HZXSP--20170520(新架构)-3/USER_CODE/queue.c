


#include <stdio.h>
#include <malloc.h>

#include "queue.h"

Queue *InitQu()
{
	Queue *q;
	q= (Queue *)malloc(sizeof(Queue));		//����ռ�
	q->front =0;
	q->rear =0;
	q->tag =0;
	return q;
}

INT8U EnQu(Queue *q,INT8U x)
{
	if(q->rear== q->front && q->tag==1) //��ʾ������
		return 0;
	else
	{
		q->qu[q->rear]=x;
		q->rear= (q->rear+1)%MAX_SIZE;	//βָ�������һ��
		if(q->rear == q->front)	//����Ԫ������Ժ����q->rear ==q->front,���ʾ������
			q->tag =1;
		return 1;
	}
}



INT8U DeQu(Queue *q,INT8U *dat)
{
	if(q->rear==q->front &&q->tag ==0)//��ʾ���п�
		return 0;
	else
	{
		*dat = q->qu[q->front];		//ȡ������
		q->qu[q->front]=0;			//����ͷԪ�ص�ֵ��ֵΪ0
		q->front = (q->front+1)%MAX_SIZE;	//ͷָ�������һ��
		if(q->rear == q->front)   //����Ԫ�س����Ժ����q->rear==q->front,���ʾ�ӿ�
			q->tag =0;
		return 1;
	}
}

