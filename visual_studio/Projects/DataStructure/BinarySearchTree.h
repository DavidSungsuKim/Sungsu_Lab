#pragma once

#ifndef _BINARY_SEARCH_TREE_
#define _BINARY_SEARCH_TREE_		1

#include "BinaryTree.h"

typedef BTData	BSTData;

void		BSTMakeAndInit	(BTreeNode**	pRoot);
BSTData		BSTGetNodeData	(BTreeNode*		pBst);
BTreeNode*	BSTInsert		(BTreeNode**	pRoot,	BSTData	data);
BTreeNode*	BSTSearch		(BTreeNode*		pBst,	BSTData target);

BTreeNode*	BSTRemove		(BTreeNode**	pRoot,	BSTData	target);
void		BSTShowAll		(BTreeNode*		pBst,	FuncPtr pFunc);

#endif