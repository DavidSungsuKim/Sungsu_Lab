
#ifndef __DOUBLYLINKED_LIST_H__
#define __DOUBLYLINKED_LIST_H__

#include <stdlib.h>

typedef int Data;

class CDoublyList
{
public:
	CDoublyList();
	~CDoublyList();

protected:
	typedef struct stNode
	{
		Data	data;
		stNode *pNext;
		stNode *pPrev;
	}StNode;

	StNode *m_pHead;
	StNode *m_pTail;
	StNode *m_pCur;
	int		m_num;

	bool	m_bInit;

public:
	void	Init		();
	int		Terminate	();

	int		PushBack	(Data& aData);
	int		PushFront	(Data& aData);
	int		PopBack		(Data& aData);
	int		PopFront	(Data& aData);

	int		First		(Data& aData);
	int		Next		(Data& aData);
	int		Prev		(Data& aData);
	int		RemoveAll	();
	int		Find		(Data& aData);

	int		Count		() { return m_num; }

protected:
	StNode*	GetNewNode	() { return (StNode*)malloc(sizeof(StNode)); }
};

#endif