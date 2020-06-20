
#include <stdio.h>
#include <stdlib.h>

#include "..//include/LinkedList.h"

#define _USE_DUMMY_HEAD_

void
ListInit(StList *apList)
{
#ifdef _USE_DUMMY_HEAD_
	apList->head = (StNode*)malloc(sizeof(StNode));
	apList->head->pNext = NULL;
	apList->head->data = 9999;

	// Now head is pointing to the dummy node.
	apList->cur = apList->head->pNext;
#elif
	apList->head = NULL;
	apList->cur = NULL;
#endif

	apList->num = 0;
}

int
ListFirst(StList *apList, int *apData)
{
	if (!apList->num)
		return -1;

#ifdef _USE_DUMMY_HEAD_
	*apData = apList->head->pNext->data;
	apList->cur = apList->head->pNext->pNext;
#elif
	*apData = apList->head->data;
	apList->cur = apList->head->pNext;
#endif

	return 0;
}

void
ListInsert(StList *apList, int aData)
{
	StNode* node = (StNode*)malloc(sizeof(StNode));
	node->data = aData;

#ifdef _USE_DUMMY_HEAD_
	node->pNext = apList->head->pNext;
	apList->head->pNext = node;
#elif
	if (apList->head == NULL)
		node->pNext = NULL;
	else
		node->pNext = apList->head;
	apList->head = node;
#endif
	apList->num++;
}

int
ListNext(StList *apList, int *apData)
{
	if (!apList->num)
		return -1;

	if (!apList->cur)
		return -2;

	*apData = apList->cur->data;
	apList->cur = apList->cur->pNext;

	return 0;
}

int
ListRemove(StList *apList, int *apData)
{
	if (!apList->num)
		return -1;

	StNode* node = apList->head;

	*apData = node->data;
	apList->head = apList->head->pNext;

	free(node);
	apList->num--;

	return 0;
}

void
ListTerminate(StList *apList)
{
#ifdef _USE_DUMMY_HEAD_
	if ( !apList->num )
	{
		if ( apList->head != NULL )
		{
			free(apList->head);
			apList->head = NULL;
		}
		return;
	}

	StNode *node = apList->head->pNext;
	free( apList->head );
	apList->num--;

	apList->head = node;

	ListTerminate( apList );
#endif
}

int
ListCount(StList *apList)
{
	return apList->num;
}
