
#include "Common.h"

#include "Table.h"

#ifdef USE_CHAINNING
#include "LinkedList.h"
#endif

void TBLInit(Table* pt, HashFunc* f)
{
	int	i;

	for (i = 0; i < MAX_TBL; i++)
	{
#ifdef USE_CHAINNING
		ListInit(&(pt->tbl[i]));								// each of tbl is a List.
#else	
		pt->tbl[i].status = EMPTY;
#endif
	}

	pt->hf	= f;
}

void TBLInsert(Table* pt, Key k, Value v)
{
	int		hv = pt->hf(k);

#ifdef USE_CHAINNING
	Slot	ns = { k,v };

	if(TBLSearch(pt, k) != NULL)
	{
		printf("Error a duplicate key has occurred!.\n");
		return;
	}
	else
	{
		LInsert(&(pt->tbl[hv]), ns);
	}
#else
	pt->tbl[hv].val		= v;
	pt->tbl[hv].key		= k;
	pt->tbl[hv].status	= INUSE;
#endif
}

Value TBLDelete(Table* pt, Key k)
{
	int hv = pt->hf(k);

#ifdef USE_CHAINNING
	Slot	cSlot;

	if( LFirst((&(pt->tbl[hv])), &cSlot))
	{
		if (cSlot.key == k)
		{
			LRemove(&(pt->tbl[hv]));
			return cSlot.val;
		}
		else
		{
			while (LNext(&(pt->tbl[hv]), &cSlot))
			{
				if(cSlot.key == k)
				{
					LRemove(&(pt->tbl[hv]));
					return cSlot.val;
				}
			}
		}
	}

	return NULL;
#else
	if (pt->tbl[hv].status != INUSE)
	{
		return NULL;
	}
	else
	{
		pt->tbl[hv].status = DELETED;
		return (pt->tbl[hv].val);
	}
#endif
}

Value TBLSearch(Table* pt, Key k)
{
	int hv = pt->hf(k);

#ifdef USE_CHAINNING
	Slot cSlot;

	if (LFirst(&(pt->tbl[hv]), &cSlot))
	{
		if (cSlot.key == k)
		{
			return cSlot.val;
		}
		else
		{
			while(LNext(&(pt->tbl[hv]), &cSlot))
			{
				if(cSlot.key == k)
					return cSlot.val;
			}
		}
	}

	return NULL;

#else

	if (pt->tbl[hv].status != INUSE)	
		return NULL;
	else
		return 	(pt->tbl[hv].val);
#endif
}