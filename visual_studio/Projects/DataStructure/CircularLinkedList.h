#pragma once

#ifndef _CIRCULAR_LINKED_LIST_
#define _CIRCULAR_LINKED_LIST_

#include <stdio.h>
#include <atlstr.h>	

typedef struct _data
{
	char	name[20];
	int		id;
}Data;

typedef struct _node
{
	Data			data;
	struct _node*	next;
}Node;

typedef struct _circularlinkedList
{
	Node*	tail;
	Node*	cur;
	Node*	before;
	int		numOfData;
}CList;

typedef CList List;

void	ListInit		(List* plist);
void	LInsert			(List* plist, Data data);
void	LInsertFront	(List* plist, Data data);
int		LFirst			(List* plist, Data* pdata);
int		LNext			(List* plist, Data* pdata);
Data	LRemove			(List* plist);
int		LCount			(List* plist);

int		LGetPersonInfo	(Data* pdata, CString name, int id);
int		LCheckNext		(List* plist, CString name, CString& nameNext, int days);

bool	IsSameString	(CString str1,	CString str2);


#endif