
#include "Common.h"

#include "Deque.h"

void InitDeque(Deque*	pdeq)
{
	pdeq->head	= NULL;
	pdeq->tail	= NULL;
}

int	IsDequeEmpty(Deque*	pdeq)
{
	if(pdeq->head == NULL)									
		return TRUE;										
	else													
		return FALSE;
}

void AddDequeFront(Deque*	pdeq, Data data)
{
	Node*	newNode	= (Node*)malloc(sizeof(Node));
	newNode->data	= data;

	if (pdeq->head == NULL)
	{
		newNode->next	= NULL;
		newNode->prev	= NULL;

		pdeq->head		= newNode;
		pdeq->tail		= newNode;

		printf("Data is added at the front for the first time = %d\n", data);
	}
	else
	{
		newNode->next	= pdeq->head;
		newNode->prev	= NULL;

		pdeq->head->prev	= newNode;
		pdeq->head			= newNode;						// change head.

		printf("Data is added at the front = %d\n", data);
	}
}

void AddDequeEnd(Deque* pdeq, Data data)
{
	Node*	newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;

	if (pdeq->head == NULL)									// Is it reasonable to go by checking 'head'? Should I replace it with 'tail'?
	{
		newNode->next = NULL;
		newNode->prev = NULL;

		pdeq->head = newNode;
		pdeq->tail = newNode;

		printf("Data is added at the end for the first time = %d\n", data);
	}
	else
	{
		newNode->prev	= pdeq->tail;
		newNode->next	= NULL;

		pdeq->tail->next	= newNode;
		pdeq->tail			= newNode;

		printf("Data is added at the front = %d\n", data);
	}
}

Data RemoveDequeFront(Deque* pdeq)
{
	if (pdeq->head == NULL)
	{
		printf("There is no data to be deleted..\n");
		exit(-1);
	}

	Data	data;
	Node*	prevHead	= pdeq->head;

	data	= prevHead->data;								// copy the current head's data.

	pdeq->head = pdeq->head->next;							// shift

	free(prevHead);											// remove previous head first.

	if(pdeq->head == NULL)									// Having this code might not be necessary, but obviously helpful.
		pdeq->tail = NULL;
	else
		pdeq->head->prev = NULL;

	printf("Data is removed at the front = %d\n", data);

	return data;
}

Data RemoveDequeEnd(Deque* pdeq)
{
	if (pdeq->head == NULL)									// To be sure of any data in a deque, I just check 'head'.
	{
		printf("There is no data to be deleted..\n");
		exit(-1);
	}

	Data	data;
	Node*	prevTail = pdeq->tail;

	data	= prevTail->data;

	pdeq->tail = pdeq->tail->prev;

	free(prevTail);

	if (pdeq->tail == NULL)									// Having this code might not be necessary, but obviously helpful.
		pdeq->head = NULL;
	else
		pdeq->tail->next = NULL;

	printf("Data is removed at the end = %d\n", data);

	return data;
}

Data GetDequeFirst(Deque* pdeq)
{
	Data data = NULL;

	if (pdeq->head == NULL)									// To be sure of any data in a deque, I just check 'head'.
	{
		printf("There is no data to be deleted..\n");
		exit(-1);
	}

	return pdeq->head->data;
}

Data GetDequeEnd(Deque* pdeq)
{
	Data data = NULL;

	if (pdeq->head == NULL)									// To be sure of any data in a deque, I just check 'head'.
	{
		printf("There is no data to be deleted..\n");
		exit(-1);
	}

	return pdeq->tail->data;
}

