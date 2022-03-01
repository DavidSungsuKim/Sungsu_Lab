#pragma once

#ifndef _DEQUE_
#define _DEQUE_	1

#define TRUE	1
#define FALSE	0

typedef int		Data;
typedef struct _node
{
	Data	data;
	struct _node*	next;
	struct _node*	prev;
}Node;

typedef struct _dlDeque
{
	Node*	head;
	Node*	tail;
}Deque;

void	InitDeque			(Deque*	pdeq);
int		IsDequeEmpty		(Deque*	pdeq);
void	AddDequeFront		(Deque*	pdeq, Data data);
void	AddDequeEnd			(Deque* pdeq, Data data);

Data	RemoveDequeFront	(Deque* pdeq);
Data	RemoveDequeEnd		(Deque* pdeq);

Data	GetDequeFirst		(Deque* pdeq);
Data	GetDequeEnd			(Deque* pdeq);

#endif