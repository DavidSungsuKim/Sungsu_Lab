#pragma once

#ifndef	_SIMPLE_HEAP_
#define	_SIMPLE_HEAP_		1

#define	TRUE	1
#define FALSE	0

#define HEAP_LEN	100

typedef char	HData;
typedef int		Priority;

typedef struct _heapElem
{
	Priority	pr;
	HData		data;
}HeapElem;

typedef struct _heap
{
	int			numOfData;
	HeapElem	headArr[HEAP_LEN];
}Heap;

// public
void	HeapInit	(Heap*	ph);
int		HIsEmpty	(Heap*	ph);

void	HInsert(Heap*	ph, HData data, Priority pr);
HData	HDelete(Heap*	ph);

// private	
int		GetParentIDX	(int idx);
int		GetLChildIDX	(int idx);
int		GetRChildIDX	(int idx);
int		GetHiPriChildIDX(Heap* ph, int idx);



#endif