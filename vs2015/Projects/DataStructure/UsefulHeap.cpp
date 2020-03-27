
#include "Common.h"

#include "UsefulHeap.h"

void UHeapInit(Heap* ph, PriorityComp pc)
{
	ph->numOfData	= 0;
	ph->comp		= pc;
}

int	UHIsEmpty(Heap* ph)
{
	return (ph->numOfData) ? FALSE : TRUE;
}

void UHInsert(Heap* ph, HData data)
{
	int idx, idxParent;

	PriorityComp*	pComp;
	HData*			pArr;

	idx		= ph->numOfData + 1;
	pComp	= ph->comp;
	pArr	= ph->heapArr;

	while (idx != 1)
	{
		idxParent = UGetParentIDX(idx);						// 'idx' begins at the lowest point of the heap (ph->numOfData + 1)

		if (pComp(data, pArr[idxParent]) > 0)				// The outcome of pComp() determines which one has higher priority between two.
		{													
			pArr[idx]	= pArr[idxParent];					// If data's priority is higher, it takes the place over parent's.
			idx			= idxParent;
		}
		else
			break;
	}

	pArr[idx]	= data;
	(ph->numOfData)++;
}

HData UHDelete(Heap* ph)
{
	HData	retData;
	HData	lastElem;
	HData*	pArr;
	PriorityComp*	pComp;

	int		idxParent, idxChild;

	pArr		= ph->heapArr;
	pComp		= ph->comp;

	retData		= pArr[INDEX_LOWEST];						// get the data in the lowest in advance since it is going to be taken out.
	lastElem	= pArr[ph->numOfData];						

	idxParent	= INDEX_LOWEST;

	while (idxChild = UGetHiPriChildIDX(ph, idxParent))		// get the index of the child which has higher priority.
	{
		if (pComp(lastElem, pArr[idxChild]) >= 0)			// if it has reached to the last, break.
			break;

		pArr[idxParent]	= pArr[idxChild];					// exchange positions to go lower level.
		idxParent		= idxChild;
	}

	pArr[idxParent]	= lastElem;
	(ph->numOfData)--;

	return retData;
}

int	UGetParentIDX(int idx)
{
	return (idx / 2);										// Since it's a full binary tree.
}

int	UGetLChildIDX(int idx)
{
	return (idx * 2);										// Since it's a full binary tree.
}

int UGetRChildIDX(int idx)
{
	return (UGetLChildIDX(idx) + 1);						// Since it's a full binary tree.
}

int	UGetHiPriChildIDX(Heap* ph, int idx)
{
	int			numOfData;
	HData*		pArr;
	int			idxLeft, idxRight;

	PriorityComp*	pComp;

	numOfData	= ph->numOfData;
	pArr		= ph->heapArr;
	pComp		= ph->comp;

	idxLeft		= UGetLChildIDX(idx);
	idxRight	= UGetRChildIDX(idx);

	if (idxLeft > numOfData)
	{
		return 0;
	}
	else if (idxLeft == numOfData)							// since it's a full binary tree we can make sure that there is no right child in this case.
	{
		return idxLeft;
	}
	else
	{
		if(pComp(pArr[idxLeft], pArr[idxRight]) < 0)
		{
			return idxRight;
		}
		else
		{
			return idxLeft;
		}
	}
}