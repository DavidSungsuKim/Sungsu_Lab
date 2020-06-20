
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

#endif