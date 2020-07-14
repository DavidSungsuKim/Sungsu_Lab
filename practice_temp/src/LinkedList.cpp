
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

//////////////////////////////////////////////////////////////////////////

CLinkedList::CLinkedList()
{
	Init();
}

CLinkedList::~CLinkedList()
{
	RemoveAll();
	if ( !m_head )
		free (m_head);
}

void	
CLinkedList::Init()
{
	// Assign dummy node
	m_head = (StNode*)malloc(sizeof(StNode));
	m_head->pNext = NULL;
	m_head->data = 9999;

	m_cur = m_head->pNext;

#ifdef _USE_TAIL_
	m_tail = m_head;
#endif
}

int
CLinkedList::First(Data& arData)
{
	m_cur = m_head->pNext;
	return 0;
}

int		
CLinkedList::Insert(Data& arData)
{
	// Insert from the head.
	
	StNode* node = (StNode*)malloc(sizeof(StNode));
	node->data = arData;
	
	node->pNext = m_head->pNext;
	m_head->pNext = node;

	m_num++;

	return 0;
}

int		
CLinkedList::Next(Data& arData)
{
	if (!m_num)
		return -1;

	if (!m_cur)
		return -2;

	arData = m_cur->data;
	m_cur = m_cur->pNext;

	return 0;
}

int
CLinkedList::Remove(Data& arData)
{
/*	if (!m_num)
		return -1;

	StNode* node = m_cur;

	m_cur->pNext

	free(node);
	m_num--;
	*/

	return -1;


	return 0;
}

int
CLinkedList::RemoveAll()
{
	return -1;
}

int		
CLinkedList::Count()
{
	return m_num;
}
