
#include "Common.h"

#include "AVLRebalance.h"

BTreeNode* RotateLL(BTreeNode* bst)
{
	BTreeNode*	pNode;									// parent node
	BTreeNode*	cNode;									// child node

	pNode	= bst;
	cNode	= GetLeftSubTree(pNode);

	ChangeLeftSubTree(pNode, GetRightSubTree(cNode));	// we suppose cNode has a sub tree on its right side, and this is a generalization of LL rotation.
	ChangeRightSubTree(cNode, pNode);					// this represents the core concept for LL rotation.

	return cNode;
}

BTreeNode* RotateRR(BTreeNode* bst)						// this function is exactly mirrored to RotateLL's
{
	BTreeNode*	pNode;									// parent node
	BTreeNode*	cNode;									// child node

	pNode = bst;
	cNode = GetRightSubTree(pNode);

	ChangeRightSubTree(pNode, GetLeftSubTree(cNode));
	ChangeLeftSubTree(cNode, pNode);

	return cNode;
}

BTreeNode* RotateRL(BTreeNode* bst)
{
	BTreeNode*	pNode;									// parent node
	BTreeNode*	cNode;									// child node

	pNode = bst;
	cNode = GetRightSubTree(pNode);

	ChangeRightSubTree(pNode, RotateLL(cNode));

	return RotateRR(pNode);
}

BTreeNode* RotateLR(BTreeNode* bst)
{
	BTreeNode*	pNode;									// parent node
	BTreeNode*	cNode;									// child node

	pNode = bst;
	cNode = GetLeftSubTree(pNode);

	ChangeLeftSubTree(pNode, RotateRR(cNode));

	return RotateLL(pNode);
}

int GetHeight(BTreeNode* bst)
{
	int		leftH;
	int		rightH;

	if(bst == NULL)
		return 0;

	leftH	= GetHeight(GetLeftSubTree(bst));
	rightH	= GetHeight(GetRightSubTree(bst));

	if(leftH > rightH)
		return (leftH + 1);
	else
		return (rightH + 1);
}

int GetHeightDiff(BTreeNode* bst)
{
	int		lsh;
	int		rsh;

	if(bst == NULL)
		return 0;

	lsh	= GetHeight(GetLeftSubTree(bst));
	rsh	= GetHeight(GetRightSubTree(bst));

	return lsh - rsh;
}

BTreeNode*	Rebalance(BTreeNode**	pRoot)
{
	int		hDiff = GetHeightDiff(*pRoot);

	if (hDiff > 1)
	{
		if(GetHeightDiff(GetLeftSubTree(*pRoot)) > 0)
			*pRoot = RotateLL(*pRoot);
		else
			*pRoot = RotateLR(*pRoot);
	}
	else if (hDiff < -1)
	{
		if(GetHeightDiff(GetRightSubTree(*pRoot)) < 0)
			*pRoot = RotateRR(*pRoot);
		else
			*pRoot = RotateRL(*pRoot);
	}

	return *pRoot;											// the adress for pRoot can be changed.
}