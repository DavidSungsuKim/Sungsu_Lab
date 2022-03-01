
#include "Common.h"

#include "BinaryTree.h"

BTreeNode*	MakeBTreeNode(BTData data)
{
	BTreeNode*	nd	= (BTreeNode*)malloc(sizeof(BTreeNode));

	nd->left	= NULL;
	nd->right	= NULL;

	nd->data	= data;

	return nd;
}

BTData GetData(BTreeNode*	bt)
{
	return bt->data;
}

void SetData(BTreeNode*	bt, BTData	data)
{	
	if(bt == NULL)
		return;

	bt->data = data;
}

BTreeNode* GetLeftSubTree(BTreeNode* bt)
{
	return bt->left;
}

BTreeNode* GetRightSubTree(BTreeNode* bt)
{
	return bt->right;
}

void MakeLeftSubTree(BTreeNode* main, BTreeNode* sub)
{
	if(main->left != NULL)
		free(main->left);

	main->left	= sub;
}

void MakeRightSubTree(BTreeNode* main, BTreeNode* sub)
{
	if (main->right != NULL)
		free(main->right);

	main->right = sub;
}

void TraverseInOrder(BTreeNode* bt, FuncPtr pfAction)
{	
	if (bt == NULL)
		return;
	
	bool	bOperator;
	
	bOperator = (0 <= bt->data && bt->data <= 9) ? false : true;

	if(bOperator)
		printf("(");

	TraverseInOrder(bt->left, pfAction);

	pfAction(bt->data);

	TraverseInOrder(bt->right, pfAction);
	
	if(bOperator)
		printf(")");
}

void TraversePreOrder(BTreeNode* bt, FuncPtr pfAction)
{
	if (bt == NULL)
		return;

	pfAction(bt->data);
	TraversePreOrder(bt->left, pfAction);
	TraversePreOrder(bt->right, pfAction);
}

void TraversePostOrder(BTreeNode* bt, FuncPtr pfAction)
{
	if (bt == NULL)
		return;

	TraversePostOrder(bt->left, pfAction);
	TraversePostOrder(bt->right, pfAction);
	pfAction(bt->data);
}

void DeleteTree(BTreeNode* bt)
{
	if (bt == NULL)
		return;

	DeleteTree(bt->left);
	DeleteTree(bt->right);

	printf("%d is deleted..\n", bt->data);
	free(bt);
	bt = NULL;
}

BTreeNode* RemoveLeftSubTree(BTreeNode* bt)
{
	BTreeNode* delNode = NULL;

	if (bt != NULL)
	{
		delNode		= bt->left;
		bt->left	= NULL;								// we don't free the memory, and just break the connection between them.
	}

	return delNode;
}

BTreeNode* RemoveRightSubTree(BTreeNode* bt)
{
	BTreeNode* delNode = NULL;

	if (bt != NULL)
	{
		delNode = bt->right;
		bt->right = NULL;
	}

	return delNode;
}

void ChangeLeftSubTree(BTreeNode* main, BTreeNode* sub)
{
	main->left = sub;
}

void ChangeRightSubTree(BTreeNode* main, BTreeNode* sub)
{
	main->right = sub;
}
