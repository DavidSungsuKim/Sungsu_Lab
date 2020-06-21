
#include <stdio.h>

#include "..//include/DoublyLinkedList.h"

#define _DEBUG_LIST_	

CDoublyList::CDoublyList()
{
	Init();
}

CDoublyList::~CDoublyList()
{
	Terminate();
}

void
CDoublyList::Init()
{
	if ( m_bInit )
		Terminate();

	// dummy node
	m_pHead = GetNewNode();
	m_pTail = GetNewNode();
	
	m_pHead->data = -1;
	m_pTail->data = -1;

	// initial connections
	m_pHead->pNext = m_pTail;
	m_pHead->pPrev = NULL;

	m_pTail->pPrev = m_pHead;
	m_pTail->pNext = NULL;

	m_pCur	= NULL;

	m_bInit = true;
}

int
CDoublyList::Terminate()
{
	if ( !m_bInit )
		return 0;

	RemoveAll();

	free ( m_pHead );
	free ( m_pTail );

	m_bInit = false;
	return 0;
}

int		
CDoublyList::PushBack(Data& aData)
{
	StNode* node = GetNewNode();

	// Establish relationships around the new node first.
	// This helps me more.
	node->data = aData;
	node->pNext = m_pTail;
	node->pPrev = m_pTail->pPrev;

	// And then update the rest.
	// NOTE : the order below mustn't be changed!
	m_pTail->pPrev->pNext = node;
	m_pTail->pPrev = node;

	m_num++;

#ifdef _DEBUG_LIST_
	printf("PushBack, n=%d\n", m_num );
#endif
	return 0;
}

int		
CDoublyList::PushFront(Data& aData)
{
	StNode* node = GetNewNode();

	node->data = aData;
	node->pNext = m_pHead->pNext;
	node->pPrev = m_pHead;

	m_pHead->pNext->pPrev = node;
	m_pHead->pNext = node;

	m_num++;

#ifdef _DEBUG_LIST_
	printf("PushFront, n=%d\n", m_num);
#endif
	return 0;
}

int
CDoublyList::PopBack(Data& aData)
{
	if ( !m_num )
		return -1;

	StNode *node = m_pTail->pPrev;
	aData = node->data;

	StNode *prevNew = m_pTail->pPrev->pPrev;

	m_pTail->pPrev->pPrev->pNext = m_pTail;
	m_pTail->pPrev = prevNew;

	free (node);
	m_num--;

#ifdef _DEBUG_LIST_
	printf("PopBack, n=%d\n", m_num);
#endif
	return 0;
}

int		
CDoublyList::PopFront(Data& aData)
{
	if ( !m_num )
		return -1;

	StNode* node = m_pHead->pNext;
	aData = node->data;

	StNode* nextNew = m_pHead->pNext->pNext;

	m_pHead->pNext->pNext->pPrev = m_pHead;
	m_pHead->pNext = nextNew;

	free (node);
	m_num--;

#ifdef _DEBUG_LIST_
	printf("PopFront, n=%d\n", m_num);
#endif
	return 0;
}

int		
CDoublyList::First(Data& aData)
{
	aData = m_pHead->pNext->data;
	m_pCur = m_pHead->pNext;

	return 0;
}

int		
CDoublyList::Next(Data& aData)
{
	if ( m_pCur->pNext == m_pTail )
		return -1;

	aData = m_pCur->pNext->data;
	m_pCur = m_pCur->pNext;

	return 0;
}

int	
CDoublyList::Prev(Data& aData)
{	
	if ( m_pCur->pPrev == m_pHead )
		return -1;

	aData = m_pCur->pPrev->data;
	m_pCur = m_pCur->pPrev;

	return 0;
}

int		
CDoublyList::Find(Data& aData)
{
	StNode *pNode = m_pHead->pNext;

	while ( m_pCur->pNext != m_pTail )
	{
		m_pCur = pNode;
		if ( pNode->data == aData )
			return 0;
		pNode = pNode->pNext;
	}

	return -1;
}

int		
CDoublyList::RemoveAll()
{
	Data data;
	while ( !PopFront(data) );

	return 0;
}