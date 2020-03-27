
#include "Common.h"

#include <string.h>
#include <ctype.h>

#include "ListBasedStack.h"
#include "BinaryTree.h"
#include "ExpressionTree.h"

BTreeNode*	MakeExpTree(const char exp[])
{
	Stack		stack;
	BTreeNode*	pnode	= NULL;

	int		expLen	= strlen(exp);
	int		i;

	StackInit(&stack);

	for (i = 0; i < expLen; i++)
	{
		pnode = MakeBTreeNode(NULL);

		if (isdigit(exp[i]))
		{
			SetData(pnode, exp[i]-'0');
		}
		else
		{
			MakeRightSubTree(pnode, SPop(&stack));
			MakeLeftSubTree(pnode, SPop(&stack));
			SetData(pnode, exp[i]);
		}

		SPush(&stack, pnode);
	}

	return SPop(&stack);
}

int	EvaluateExpTree(BTreeNode*	bt)
{
	int op1, op2;

	if(GetLeftSubTree(bt) == NULL && GetRightSubTree(bt) == NULL)
		return GetData(bt);

	op1 = EvaluateExpTree(GetLeftSubTree(bt));
	op2	= EvaluateExpTree(GetRightSubTree(bt));

	switch (GetData(bt))
	{
		case '+':
			return op1 + op2;
			break;

		case '-':	
			return op1 - op2;
			break;
		
		case '*':
			return op1 * op2;
			break;

		case '/':
			return op1 / op2;
			break;

		default:
			return 0;
	}

	return 0;	// This should never be reached.
}

void ShowNodeData(int data)
{
	if(0 <= data && data <= 9)
		printf("%d ", data);
	else
		printf("%c ", data);
}

void ShowPrefixTypeExp(BTreeNode* bt)
{
	TraversePreOrder(bt, ShowNodeData);
}

void ShowInfixTypeExp(BTreeNode* bt)
{
	TraverseInOrder(bt, ShowNodeData);
}

void ShowPostfixTypeExp(BTreeNode* bt)
{
	TraversePostOrder(bt, ShowNodeData);	
}