
#include "Common.h"

#include "PriorityQueue.h"

void PQueueInit(PQueue* ppq, PriorityComp pc)
{
	UHeapInit(ppq, pc);
}

int	PQIsEmpty(PQueue* ppq)
{
	return (UHIsEmpty(ppq));
}

void PEnqueue(PQueue* ppq, PQData data)
{
	UHInsert(ppq, data);
}

PQData	PDequeue(PQueue* ppq)
{
	return (UHDelete(ppq));
}