#pragma once

#ifndef _TABLE_
#define	_TABLE_			1

#include "Slot.h"

#ifdef USE_CHAINNING
#include "LinkedList.h"
#endif

#define MAX_TBL		100

typedef int HashFunc(Key key);

typedef struct _table
{
#ifdef USE_CHAINNING
	List		tbl[MAX_TBL];
#else
	Slot		tbl[MAX_TBL];
#endif	
	HashFunc*	hf;

}Table;

void		TBLInit		(Table* pt, HashFunc* f);
void		TBLInsert	(Table* pt, Key k, Value v);
Value		TBLDelete	(Table* pt, Key k);
Value		TBLSearch	(Table* pt, Key k);

#endif
