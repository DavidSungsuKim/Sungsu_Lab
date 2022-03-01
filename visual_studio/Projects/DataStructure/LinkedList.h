#pragma once

#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#define TRUE	1
#define FALSE	0

#include "slot.h"

#ifdef USE_CHAINNING
typedef Slot LData;
#else

typedef struct _point
{
	int	posX;
	int	posY;
}LData;

#endif

typedef struct _node
{
	LData			data;
	struct _node*	next;
}Node;

typedef struct _linkedList
{
	Node*	head;
	Node*	cur;
	Node*	before;
	int		numOfData;
	int		(*comp)(LData d1, LData d2);
}List;

//typedef LinkedList	List;

void	ListInit	(List* plist);
void	LInsert		(List* plist, LData data);
void	SInsert		(List* plist, LData data);
int		LFirst		(List* plist, LData* pdata);
int		LNext		(List* plist, LData* pdata);

LData	LRemove		(List* plist);
int		LCount		(List* plist);

void	SetSortRule	(List* plist, int (*comp)(LData d1, LData d2));

#endif