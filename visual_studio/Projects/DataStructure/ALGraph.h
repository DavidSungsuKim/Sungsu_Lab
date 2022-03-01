
#pragma once

#ifndef _ALGRAPH_
#define _ALGRAPH_		1

#include "LinkedList.h"

#include "PriorityQueue.h"
#include "ArrayBasedStack.h"
#include "ALEdge.h"

#define DEC_CHAR(x) (x + 65)

enum
{
	A, B, C, D, E, F, G, H, I, J
};

typedef struct _ual
{
	int		numV;
	int		numE;
	List*	adjList;
	int*	visitInfo;
	PQueue	pqueue;					// edge information is inserted into queue and sorted along the weight defined by users.
}ALGraph;

void		GraphInit		(ALGraph* pg, int nv);
void		GraphDestroy	(ALGraph* pg);
void		AddEdge			(ALGraph* pg, int fromV, int toV, char weight);
void		RemoveEdge		(ALGraph* pg, int fromV, int toV);
int			IsConnVertex	(ALGraph* pg, int v1, int v2);
void		RecoverEdge		(ALGraph* pg, int fromV, int toV, int weight);
void		ShowGraphEdge	(ALGraph* pg);

void		ShowDFSGraphVertex	(ALGraph* pg, int startV);
void		ShowBFSGraphVertex	(ALGraph* pg, int startV);

void		ConKruskalMST	(ALGraph* pg);
void		ShowGraphEdgeWeightInfo	(ALGraph* pg);

#endif