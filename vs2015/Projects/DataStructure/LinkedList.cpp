
#include "Common.h"

#include "LinkedList.h"

void ListInit(List* plist)
{
	plist->head			= (Node*)malloc(sizeof(Node));
	plist->head->next	= NULL;
	plist->comp			= NULL;						// the pointer of a function to be used in sorting.
	plist->numOfData	= 0;						// Q : don't I need to initialize plist->before ?
}

void LInsert(List* plist, LData data)
{
	Node*	newNode	= (Node*)malloc(sizeof(Node));
	newNode->data	= data;
	newNode->next	= plist->head->next;			// Q : what is this? 
	plist->head->next	= newNode;					// A : I can think like this : I am inserting a new data between head and its next! Simple!

	(plist->numOfData++);
}

void SInsert(List* plist, LData data)
{
	Node*	newNode = (Node*)malloc(sizeof(Node));
	Node*	pred	= plist->head;

	newNode->data = data;

	while (pred->next != NULL && plist->comp(data, pred->next->data) != 0)
	{
		pred = pred->next;
	}

	newNode->next	= pred->next;
	pred->next		= newNode;

	(plist->numOfData)++;
}

int LFirst(List* plist, LData* pdata)				// to be used in searching for the first data in the list.
{
	if( plist->head->next == NULL )
		return FALSE;

#if 0
	plist->before	= plist->head;					// ..... (1) // Q : why do we need this process?
	plist->cur		= plist->head->next;			// ..... (2)	
#else
	plist->cur		= plist->head->next;			// ..... (2)	
	plist->before	= plist->head;					// ..... (1) // Q : why do we need this process?
#endif													// It doesn't matter to change calling order for 1 and 2.

	*pdata	= plist->cur->data;						// we aim at returning the data of head.

	return TRUE;
}

int	LNext(List* plist, LData* pdata)				// This function is clear enough to understand.
{
	if(plist->cur->next == NULL)						
		return FALSE;

	plist->before	= plist->cur;
	plist->cur		= plist->cur->next;

	*pdata			= plist->cur->data;

	return TRUE;
}

LData LRemove(List* plist)
{
	Node*	rpos	= plist->cur;
	LData	rdata	= rpos->data;					// I must acquire the data before deleting it.
	
	plist->before->next	= plist->cur->next;			// .... (1)
	plist->cur			= plist->before;			// .... (2)	The order is unchangable.

	free(rpos);

	(plist->numOfData)--;

	return rdata;
}

int	LCount(List* plist)
{
	return (plist->numOfData);
}

void SetSortRule(List* plist, int(*comp)(LData d1, LData d2))
{
	plist->comp	= comp;
}