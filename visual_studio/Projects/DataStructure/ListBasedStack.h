#pragma once

#ifndef LIST_BASED_STACK	
#define LIST_BASED_STACK	1

#define TRUE		1
#define FALSE		0

#define USED_WITH_BINARY_TREE	1

#ifdef USED_WITH_BINARY_TREE
	#include "BinaryTree.h"
	typedef BTreeNode*	Data;
#else
	typedef int	Data;
#endif

typedef struct _node
{
	Data			data;
	struct _node*	next;
}Node;

typedef struct _listStack
{
	Node*	head;
}Stack;

void	StackInit	(Stack*	pstack);
int		SIsEmpty	(Stack* pstack);

void	SPush		(Stack* pstack, Data data);
Data	SPop		(Stack* pstack);
Data	SPeek		(Stack* pstack);

#endif
