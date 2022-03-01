
#include "Common.h"

#include "CircularLinkedList.h"

#define TRUE	1
#define FALSE	0

void ListInit(List* plist)
{
	plist->tail			= NULL;
	plist->cur			= NULL;
	plist->before		= NULL;
	plist->numOfData	= 0;
}

void LInsert(List* plist, Data data)
{
	Node*	newNode	= (Node*)malloc(sizeof(Node));
	newNode->data	= data;

	if (plist->tail == NULL)
	{
		plist->tail		= newNode;
		newNode->next	= newNode;
	}
	else
	{
		newNode->next		= plist->tail->next;
		plist->tail->next	= newNode;					// Q : Why is this needed?	
		plist->tail			= newNode;					
	}

	/*
	A : Follow these steps, and you might remeber what I meant.
		i)	There are two data in the list.
			pointers			:	tail's next			tail
			physical elements	:	(1)			->		(2)		
	
		2)	You to add new data in the list.
			pointers			:						tail			to be tail's next
			physical elements	:	(1)			->		(2)				(3)

		3)	You added new data in the list.
			pointers			:	tail's next							tail
			physical elements	:	(1)			->		(2)		->		(3)
	*/

	(plist->numOfData)++;
}

void LInsertFront(List* plist, Data data)
{
	Node*	newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;

	if (plist->tail == NULL)
	{
		plist->tail		= newNode;
		newNode->next	= newNode;
	}
	else
	{
		newNode->next		= plist->tail->next;
		plist->tail->next	= newNode;
	}

	(plist->numOfData)++;
}

int	LFirst(List* plist, Data* pdata)
{
	if (plist->tail == NULL)
		return FALSE;

	plist->before	= plist->tail;
	plist->cur		= plist->tail->next;

	*pdata	= plist->cur->data;
	return TRUE;
}

int	LNext(List* plist, Data* pdata)
{
	if (plist->tail == NULL)
		return FALSE;

	plist->before	= plist->cur;
	plist->cur		= plist->cur->next;

	*pdata			= plist->cur->data;
	
	return TRUE;
}

Data LRemove(List* plist)
{
	Node*	rpos	= plist->cur;					// 'cur' is depending on LNext(), so it can differ whenever called.
	Data	rdata	= rpos->data;

	if (rpos == plist->tail)
	{
		if (plist->tail == plist->tail->next)		// If the tail is itself, make the pointer NULL, and the memory is to be freed.
			plist->tail = NULL;
		else
			plist->tail = plist->before;			
	}

	plist->before->next	= plist->cur->next;			// These are common process for removing any data in the list.
	plist->cur			= plist->before;

	free(rpos);
	(plist->numOfData)++;
	return	rdata;
}

int	LCount(List* plist)
{
	return plist->numOfData;
}

int	LGetPersonInfo(Data* pdata, CString str, int id)
{
	int i;
	int	size;

	memset(pdata->name, 0, sizeof(pdata->name));

	size = min(sizeof(pdata->name), str.GetLength());

	for (i = 0; i < size; i++)
	{
		pdata->name[i] = str.GetAt(i);
	}

	pdata->id = id;

	return TRUE;
}

int	LCheckNext(List* plist, CString name, CString& nameNext, int days)
{
	Data	data;
	int		i;
	int		nSizeList;

	CString nameCur;

	if ( !LFirst(plist, &data) )
		return FALSE;

	nSizeList = LCount(plist);

	while (nSizeList--)
	{
		LNext(plist, &data);
		
		nameCur = data.name;

		if ( IsSameString(nameCur, name) )
		{
			while (days--)
			{
				LNext(plist, &data);
			}

			nameNext = data.name;
			return TRUE;
		}
	}

	nameNext = "JohnDoe";
	return FALSE;
}

bool IsSameString(CString str1, CString str2)
{
	bool bSame;

	if(str1.GetLength() != str2.GetLength())
		return false;

	bSame = true;
	for (int i = 0; i < str1.GetLength(); i++)
	{
		if ( str1.GetAt(i) != str2.GetAt(i) )
			bSame = false;
	}

	return bSame;
}