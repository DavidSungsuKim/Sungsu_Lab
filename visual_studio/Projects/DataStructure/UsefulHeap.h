#pragma once

#ifndef _USEFUL_HEAP_
#define _USEFUL_HEAP_	1

#include "ALEdge.h"

#define TRUE			1
#define FALSE			0

#define INDEX_LOWEST	1

#define HEAP_LEN		100

typedef Edge	HData;
typedef int		PriorityComp(HData d1, HData d2);		// a type of function to be used in our heap.

typedef struct _heap									// Note) Compared to the case of SimpleHeap, it has no explicit pr, since it is dynamically determined
{														//			along the outcome of comp.
	PriorityComp*	comp;								// to be used as a function pointer.
	int				numOfData;
	HData			heapArr[HEAP_LEN];
}Heap;

// public 
void	UHeapInit	(Heap* ph, PriorityComp pc);
int		UHIsEmpty	(Heap* ph);

void	UHInsert	(Heap* ph, HData data);
HData	UHDelete	(Heap* ph);

// private
int		UGetParentIDX(int idx);							// all the same as 'SimpleHeap.h'
int		UGetLChildIDX(int idx);							// all the same as 'SimpleHeap.h'
int		UGetRChildIDX(int idx);							// all the same as 'SimpleHeap.h'

int		UGetHiPriChildIDX(Heap* ph, int idx);


#endif