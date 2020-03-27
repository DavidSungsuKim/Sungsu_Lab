
#include "Common.h"

#include "ListBasedQueue.h"

void InitQueue(Queue*	pq)
{
	pq->front	= NULL;
	pq->rear	= NULL;
}

int	IsQEmpty(Queue*	pq)
{
	if (pq->front == NULL)
		return TRUE;
	else
		return FALSE;
}

void Enqueue(Queue*	pq, Data data)
{
	Node*	newNode	= (Node*)malloc(sizeof(Node));
	newNode->next	= NULL;
	newNode->data	= data;

	if (IsQEmpty(pq))
	{
		pq->front	= newNode;
		pq->rear	= newNode;
	}
	else
	{
		pq->rear->next	= newNode;
		pq->rear		= newNode;		// change rear.
	}
}

Data Dequeue(Queue*	pq)
{
	Node*	delNode;
	Data	retData;

	if (IsQEmpty(pq))
	{
		printf("There is no data to dequeue..\n");
		exit(-1);
	}

	delNode		= pq->front;			// copy the pointer of the front to be deleted afterwards.
	retData		= delNode->data;		// copy the data from the front.
	pq->front	= pq->front->next;		// change the front moving to the right.

	free(delNode);
	return retData;
}

Data PeekQ(Queue*	pq)
{
	if (IsQEmpty(pq))
	{
		printf("Queue memory error...?\n");
		exit(-1);
	}

	return pq->front->data;
}