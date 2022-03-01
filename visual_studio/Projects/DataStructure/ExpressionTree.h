#pragma once

#ifndef _EXPRESSION_TREE_
#define _EXPRESSION_TREE_		1

#include	"BinaryTree.h"

BTreeNode*	MakeExpTree			(const char exp[]);
int			EvaluateExpTree		(BTreeNode*	bt);

void		ShowNodeData		(int data);
void		ShowPrefixTypeExp	(BTreeNode* bt);
void		ShowInfixTypeExp	(BTreeNode* bt);
void		ShowPostfixTypeExp	(BTreeNode* bt);

#endif