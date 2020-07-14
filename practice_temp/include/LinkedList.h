
#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct stNode
{
	int data;
	stNode *pNext;
}StNode;

typedef struct stList
{
	StNode *head;
	StNode *cur;
//	StNode *before;
	int		num;
}StList;

void	ListInit(StList *apList);
int		ListFirst(StList *apList, int *apData);
void	ListInsert(StList *apList, int aData);
int		ListNext(StList *apList, int *apData);
int		ListRemove(StList *apList, int *apData);
void	ListTerminate(StList *apList);
int		ListCount(StList *apList);

#define _USE_TAIL_
typedef int	Data;

class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();

	typedef struct stNode
	{
		Data data;
		stNode *pNext;
	}StNode;

protected:

	StNode *m_head;
#ifdef _USE_TAIL_
	StNode *m_tail;
#endif
	StNode *m_cur;
	int		m_num;

public:
	void	Init		();
	int		First		(Data& arData);
	int		Insert		(Data& arData);
	int		Next		(Data& arData);
	int		Remove		(Data& arData);
	int		RemoveAll	();
	int		Count		();
};

#endif