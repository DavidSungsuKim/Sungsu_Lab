#pragma once

#ifndef _BIDIRECTIONAL_LINKED_LIST_
#define _BIDIRECTIONAL_LINKED_LIST_

/**************************************
*
*	1. have dummy nodes at the front and at the end
*	2. have 'head' and 'tail' pointing the front and the end, respectably.
*
**************************************/

#include <stdio.h>
#include <atlstr.h>	

#define	TRUE	1
#define FALSE	0

enum eOptionBL
{
	eInsertFront	= 0,
	eInsertEnd		= 1
};

typedef int Data;

typedef struct _node
{
	Data	data;
	struct _node*	next;
	struct _node*	prev;
}Node;

typedef struct _BLinkedList
{
	Node*	head;
	Node*	tail;
	Node*	cur;
	int		numOfData;
}BLinkedList;

typedef BLinkedList List;

void	ListInit	(List*	plist);
void	ListTerminate	(List*	plist);
void	LInsert		(List*	plist, Data data, eOptionBL option);

int		LFirst		(List*	plist, Data* pdata);
int		LEnd		(List*	plist, Data* pdata);
int		LNext		(List*	plist, Data* pdata);
int		LPrevious	(List*	plist, Data* pdata);
int		LRemove		(List*	plist, Data* pdata);
int		LCount		(List*	plist);

#endif