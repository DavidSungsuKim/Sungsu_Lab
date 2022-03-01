
#include "Common.h"

#include "BidirectionalLinkedList.h"

void ListInit(List*	plist)
{
	Node*	dummyFront	= (Node*)malloc(sizeof(Node));
	Node*	dummyEnd	= (Node*)malloc(sizeof(Node));

	dummyFront->data	= 111;
	dummyEnd->data		= 999;
	
	dummyFront->next	= dummyEnd;					// Is this right?
	dummyFront->prev	= NULL;

	dummyEnd->prev		= dummyFront;				// Is this right?
	dummyEnd->next		= NULL;

	plist->head			= dummyFront;
	plist->tail			= dummyEnd;

	plist->numOfData	= 0;
}

void ListTerminate(List*	plist)
{
	if (plist->numOfData)
	{		
		printf("You are not allowed to terminate the list with existing data.\nRemove all data in the list first.");
		return;
	}

	free(plist->head);
	free(plist->tail);

	return;
}

void LInsert(List*	plist, Data data, eOptionBL option)
{
	Node*	newNode	= (Node*)malloc(sizeof(Node));
	newNode->data	= data;

	if (option & eInsertEnd)
	{
		newNode->prev		= plist->tail->prev;
		newNode->next		= plist->tail;

		plist->tail->prev->next	= newNode;
		plist->tail->prev		= newNode;			
	}
	else
	{ 
		newNode->next		= plist->head->next;
		newNode->prev		= plist->head;
		
		plist->head->next->prev	= newNode;
		plist->head->next		= newNode;				
	}

	(plist->numOfData)++;
}

int	LFirst(List*	plist, Data* pdata)
{
/*	if (plist->head == NULL)						// We don't need to have this since we have a dummy node at the front.
		return FALSE;	
		*/

	if(plist->head->next == plist->tail)			
		return FALSE;

	plist->cur	= plist->head->next;				// Considering the front dummy, first data should be head's next.
	*pdata		= plist->cur->data;

	return  TRUE;
}

int	LEnd(List*	plist, Data* pdata)
{
	plist->cur	= plist->tail->prev;
	*pdata		= plist->cur->data;

	return  TRUE;
}

int	LNext(List*	plist, Data* pdata)
{
	if (plist->cur->next == plist->tail)
		return FALSE;

	plist->cur	= plist->cur->next;
	*pdata		= plist->cur->data;

	return TRUE;
}

int	LPrevious(List*	plist, Data* pdata)
{
	if(plist->cur->prev == NULL)
		return FALSE;

	plist->cur	= plist->cur->prev;
	*pdata		= plist->cur->data;

	return TRUE;
}

int	LRemove(List*	plist, Data* pdata)
{
	Node*	newCur;

	if (plist->numOfData == 0)
		return FALSE;

	if (plist->cur == plist->head)
	{
		printf("There is data in the list, but you cannot delete because it's the head!\n");
		return FALSE;
	}

	plist->cur->prev->next	= plist->cur->next;
	plist->cur->next->prev	= plist->cur->prev;

	newCur	= plist->cur->prev;
	*pdata	= plist->cur->data;

	free(plist->cur);

	plist->cur = newCur;

	(plist->numOfData)--;

	return TRUE;
}

int	LCount(List*	plist)
{
	return (plist->numOfData);
}