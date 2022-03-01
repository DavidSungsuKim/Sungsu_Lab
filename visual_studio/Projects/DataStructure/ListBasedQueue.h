#pragma once

#ifndef LIST_BASED_QUEUE
#define	LIST_BASED_QUEUE	1

#define TRUE	1
#define FALSE	0

typedef	int	Data;
typedef struct _node
{
	Data	data;
	struct _node*	next;
}Node;

typedef struct _lQueue
{
	Node*	front;
	Node*	rear;
}Queue;

void	InitQueue	(Queue*	pq);
int		IsQEmpty	(Queue*	pq);
void	Enqueue		(Queue*	pq, Data data);
Data	Dequeue		(Queue*	pq);
Data	PeekQ		(Queue*	pq);

#endif