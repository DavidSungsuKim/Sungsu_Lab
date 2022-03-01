
#include "Common.h"

#include "ListBasedStack.h"

void StackInit(Stack* pstack)
{
	pstack->head = NULL;
}

int SIsEmpty(Stack* pstack)
{
	if(pstack->head == NULL)
	{
		printf("You have reached at the bottom of the stack!\n");
		return TRUE;
	}
	else
		return FALSE;
}

void SPush(Stack* pstack, Data data)
{
	Node*	newNode = (Node*)malloc(sizeof(Node));

	newNode->data	= data;
	newNode->next	= pstack->head;		// newNode is to be added to the left of the current head.

	pstack->head	= newNode;			// update head!
}

Data SPop(Stack* pstack)
{
	Data	data;
	Node*	node;
	
	node	= pstack->head;
	if (SIsEmpty(pstack))
		exit(-1);

	data			= node->data;
	pstack->head	= node->next;

	free(node);

	return data;
}

Data SPeek(Stack* pstack)
{
	if (SIsEmpty(pstack))
		exit(-1);

	return pstack->head->data;
}