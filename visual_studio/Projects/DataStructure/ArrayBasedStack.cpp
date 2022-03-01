
#include "Common.h"

#include "ArrayBasedStack.h"

void StackInit(Stack* pstack)
{
	pstack->topIndex = -1;
}

int	SIsEmpty(Stack* pstack)
{
	if(pstack->topIndex < 0)
		return TRUE;
	else
		return FALSE;
}

void SPush(Stack* pstack, Data data)
{
	if (pstack->topIndex == (STACK_LEN - 1))
	{
		printf("Error : Stack is full.\n");
		return;
	}

	pstack->topIndex += 1;
	pstack->stackArr[pstack->topIndex] = data;
}

Data SPop(Stack* pstack)
{
	Data data;
	data = 0;

	if (SIsEmpty(pstack))
	{
		printf("Error : Stack is empty.\n");
		exit(-1);
	}

	data = pstack->stackArr[pstack->topIndex];

	pstack->topIndex -= 1;

	return data;
}

Data SPeek(Stack* pstack)
{
	if (SIsEmpty(pstack))
	{
		printf("Error : Stack is empty.\n");
		exit(-1);
	}

	return pstack->stackArr[pstack->topIndex];
}