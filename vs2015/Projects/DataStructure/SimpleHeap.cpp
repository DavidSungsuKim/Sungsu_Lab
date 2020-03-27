
#include "Common.h"

#include "SimpleHeap.h"

void HeapInit(Heap*	ph)
{
	ph->numOfData = 0;
}

int	HIsEmpty(Heap*	ph)
{
	if(ph->numOfData == 0)
		return TRUE;
	else
		return FALSE;
}

int	GetParentIDX(int idx)
{
	return (idx / 2);								// Since it's a full binary tree.
}

int	GetLChildIDX(int idx)
{
	return (idx * 2);								// Since it's a full binary tree.
}

int GetRChildIDX(int idx)
{
	return (GetLChildIDX(idx) + 1);					// Since it's a full binary tree.
}

int	GetHiPriChildIDX(Heap* ph, int idx)
{
	int			numOfData;
	HeapElem*	pArr;
	int			idxLeft, idxRight;

	numOfData	= ph->numOfData;
	pArr		= ph->headArr;

	idxLeft		= GetLChildIDX(idx);
	idxRight	= GetRChildIDX(idx);

	if(idxLeft > numOfData)
	{
		return 0;
	}
	else if(idxLeft == numOfData)					// since it's a full binary tree we can make sure that there is no right child in this case.
	{
		return idxLeft;
	}
	else
	{
		if(pArr[idxLeft].pr > pArr[idxRight].pr)
		{
			return idxRight;
		}
		else
		{
			return idxLeft;
		}
	}
}

void HInsert(Heap*	ph, HData data, Priority pr)
{
	HeapElem	nelem = { pr, data };				// data to be added to the heap.
	int			numOfData;
	HeapElem*	pArr;

	int			idx;
	int			idxParent;

	idx		= ph->numOfData + 1;					// the index to be the last.
	pArr	= ph->headArr;

	// the location for a new data to be inserted begins at the place which has lowest priority.
	while (idx != 1)								// if the index reaches to the highest, break.
	{
		idxParent = GetParentIDX(idx);				// get the index of its parent by assuming the heap is a fully binaray tree.

		// search for the locatioin for data to be added, based on priorities.
		if (pr < pArr[idxParent].pr)				// if the priority of the parent is 'lower' than that of the current data,			
		{											// exchange the data in it with the data of its child.
			pArr[idx]	= pArr[idxParent];			// Note) The bigger pr is, the lower the priority is.
			idx			= idxParent;				
		}
		else
			break;									// if the priority has precedence over that of its parent, break exchanging.
	}

	pArr[idx] = nelem;								// this is the final destination.
	(ph->numOfData)++;								// increase numOfData by 1.
}

HData HDelete(Heap*	ph)
{
	HeapElem*	pArr;
	HeapElem	lastElem;
	HData		retData;
	int			numOfData;

	int			idx, idxParent, idxChild;
	
	pArr		= ph->headArr;
	numOfData	= ph->numOfData;

	retData		= pArr[1].data;						// always data on top is deleted.
	lastElem	= pArr[numOfData];					// why do we need to have this?	Ah, this is because at the end of this function we need to delete one element,
													// and this is to get the last element in advance.
	idxParent	= 1;
	while (idxChild = GetHiPriChildIDX(ph, idxParent))	// if 'idxChild' exists, it means 'idxParent' is not the last element.
	{
		if (lastElem.pr <= pArr[idxChild].pr)		// this is the condition to stop searching.
			break;

		pArr[idxParent]	= pArr[idxChild];			// the child that has higher priority come in the place of current parent.
		idxParent		= idxChild;
	}

	pArr[idxParent]	= lastElem;						// comes in the place of its parent.
	(ph->numOfData)--;

	return retData;
}