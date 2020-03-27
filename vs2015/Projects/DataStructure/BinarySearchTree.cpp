
#include "Common.h"

#include "BinarySearchTree.h"

#define _USE_REBALANCING_	1
#ifdef _USE_REBALANCING_
#include "AVLRebalance.h"
#endif

void BSTMakeAndInit(BTreeNode**	pRoot)
{
	*pRoot = NULL;							// should we have a form of double pointer?
											// YES. in BSTInsert() pRoot gets memory address, 
											// and if we don't have double poninter, pRoot lost its address after executing the function for the caller to see.
}

BSTData	BSTGetNodeData(BTreeNode* pBst)
{
	return GetData(pBst);
}

BTreeNode* BSTInsert(BTreeNode**	pRoot, BSTData	data)	// we must use pRoot in the form of double pointer to allocate its address in the function.
{
	if (*pRoot == NULL)
	{
		*pRoot = MakeBTreeNode(NULL);
		SetData(*pRoot, data);
	}
	else if (data < GetData(*pRoot))
	{
		BSTInsert(&((*pRoot)->left), data);
#ifdef _USE_REBALANCING_
		*pRoot = Rebalance(pRoot);
#endif
	}
	else if (data > GetData(*pRoot))
	{
		BSTInsert(&((*pRoot)->right), data);
#ifdef _USE_REBALANCING_
		*pRoot = Rebalance(pRoot);
#endif
	}
	else
		return NULL;

	return *pRoot;
}

BTreeNode*	BSTSearch(BTreeNode* pBst, BSTData target)
{
	BTreeNode*	cNode	= pBst;
	BSTData		cd;

	while (cNode != NULL)
	{
		cd = GetData(cNode);

		if(target == cd)
			return cNode;
		else if(target < cd)
			cNode = GetLeftSubTree(cNode);
		else
			cNode = GetRightSubTree(cNode);
	}

	return NULL;
}

BTreeNode* BSTRemove(BTreeNode** pRoot, BSTData	target)
{
	BTreeNode*	pVRoot	= MakeBTreeNode(NULL);									// virtual root node		
	BTreeNode*	pNode	= pVRoot;												// parent node
	BTreeNode*	cNode	= *pRoot;												// current node
	BTreeNode*	dNode	= NULL;													// node to be deleted

	ChangeRightSubTree(pVRoot, *pRoot);											// this pVRoot will be used almost at the end of this function.

	while (cNode != NULL && GetData(cNode) != target)							// search for the node to be deleted.
	{
		pNode = cNode;

		if(target < GetData(cNode))												// we suppose data is arranged in order.
			cNode = GetLeftSubTree(cNode);
		else
			cNode = GetRightSubTree(cNode);
	}

	if(cNode == NULL)
		return NULL;

	dNode = cNode;																// we found the node to be deleted! 

	if (GetLeftSubTree(dNode) == NULL && GetRightSubTree(dNode) == NULL)		// case 1 : terminal node
	{																			//			deleting is quite simple : just break the connection!
		if(GetLeftSubTree(pNode) == dNode)
			RemoveLeftSubTree(pNode);
		else
			RemoveRightSubTree(pNode);
	}
	else if (GetLeftSubTree(dNode) == NULL || GetRightSubTree(dNode) == NULL)	// case 2 : has one of the two branches
	{																			//			deleting is relatively simpler than case 3.
		BTreeNode*	dcNode;														// child node of the node deleted

		if(GetLeftSubTree(dNode) != NULL)
			dcNode = GetLeftSubTree(dNode);
		else
			dcNode = GetRightSubTree(dNode);

		if(GetLeftSubTree(pNode) == dNode)										// just adopt the child according to whether the node deleted is the left sub tree or not.
			ChangeLeftSubTree(pNode, dcNode);
		else
			ChangeRightSubTree(pNode, dcNode);
	}
	else																		// case 3 : has both of the two branches
	{																			//			most difficult than any of two cases
		BTreeNode*	mNode	= GetRightSubTree(dNode);							// we are going to find the node in the right sub tree that has the smallest value
		BTreeNode*	mpNode	= dNode;											// , and the smallest value must be located in the lowest left.
		int			delData;

		while (GetLeftSubTree(mNode) != NULL)
		{
			mpNode	= mNode;
			mNode	= GetLeftSubTree(mNode);
		}

		delData	= GetData(dNode);												
		SetData(dNode, GetData(mNode));											// mNode's value is smallest of dNode's right sub tree. We copy this to dNode.
																				// Q: do we have to do this ? since we copied by data, I wonder it's unneccesary.
																				// A : yes. since mNode will be deleted from the tree in the process of adopting its child to its grandparent,
																				//		we need this.
		if(GetLeftSubTree(mpNode) == mNode)
			ChangeLeftSubTree(mpNode, GetRightSubTree(mNode));
		else																	// this can only happen when mNode is right under the dNode, which is the first and the last right sub tree.
			ChangeRightSubTree(mpNode, GetRightSubTree(mNode));

		dNode = mNode;															 
		SetData(dNode, delData);												 
	}															

	if(GetRightSubTree(pVRoot) != *pRoot)										// Q : do we need these codes?
		*pRoot = GetRightSubTree(pVRoot);										// A : yes. since root node can be replaced with one that had existed its sub tree.

	free(pVRoot);																// pVRoot as a node itself will be freed.
	
#ifdef _AVL_REBALANCE_
	Rebalance(pRoot);
#endif
	return dNode;
}

void BSTShowAll(BTreeNode*	pBst, FuncPtr pFunc)
{
	TraverseInOrder(pBst, pFunc);
}