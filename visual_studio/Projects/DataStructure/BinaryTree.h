#pragma once

#ifndef	_BINARY_TREE_
#define	_BINARY_TREE_		1

typedef int	BTData;
typedef struct _bTreeNode
{
	BTData	data;
	struct	_bTreeNode*	left;
	struct	_bTreeNode*	right;
}BTreeNode;

typedef void FuncPtr(BTData data);

BTreeNode*	MakeBTreeNode		(BTData	data);
BTData		GetData				(BTreeNode*	bt);
void		SetData				(BTreeNode*	bt, BTData	data);

BTreeNode*	GetLeftSubTree		(BTreeNode* bt);
BTreeNode*	GetRightSubTree		(BTreeNode* bt);

void		MakeLeftSubTree		(BTreeNode* main, BTreeNode* sub);
void		MakeRightSubTree	(BTreeNode* main, BTreeNode* sub);

void		TraverseInOrder		(BTreeNode* bt, FuncPtr pfAction);
void		TraversePreOrder	(BTreeNode* bt, FuncPtr pfAction);
void		TraversePostOrder	(BTreeNode* bt, FuncPtr pfAction);

void		DeleteTree			(BTreeNode* bt);

BTreeNode*	RemoveLeftSubTree	(BTreeNode* bt);
BTreeNode*	RemoveRightSubTree	(BTreeNode* bt);
void		ChangeLeftSubTree	(BTreeNode* main, BTreeNode* sub);
void		ChangeRightSubTree	(BTreeNode* main, BTreeNode* sub);

#endif