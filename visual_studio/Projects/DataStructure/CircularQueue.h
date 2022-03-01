#pragma once

#ifndef _CIRCULAR_QUEUE_
#define _CIRCULAR_QUEUE_		1

#define	TRUE		1
#define FALSE		0

#define QUE_LEN		100

typedef int Data;
typedef struct _cQueue
{
	int		front;
	int		rear;
	Data	queArr[QUE_LEN];
}Queue;

void		QueueInit		(Queue* pq);
int			QIsEmpty		(Queue* pq);

void		Enqueue			(Queue* pq, Data data);
Data		Dequeue			(Queue* pq);
Data		QPeek			(Queue* pq);

#endif