#pragma once

#ifndef	_PRIORITY_QUEUE_
#define	_PRIORITY_QUEUE_		1

#include "UsefulHeap.h"

typedef Heap	PQueue;
typedef HData	PQData;

void	PQueueInit		(PQueue* ppq, PriorityComp pc);
int		PQIsEmpty		(PQueue* ppq);

void	PEnqueue		(PQueue* ppq, PQData data);
PQData	PDequeue		(PQueue* ppq);

#endif