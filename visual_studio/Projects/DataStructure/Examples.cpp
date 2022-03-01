
#include "Common.h"
#include "Examples.h"

#include <atlstr.h>					// To use CString,

/////////////////////////////////////////////////////////////
// Only one of these can be used at the same time.

//#define TEST_LINKED_LIST					1
//#define TEST_CIRCULAR_LINKED_LIST			1
//#define TEST_BIDIRECTIONAL_LINKED_LIST	1
//#define TEST_LINKED_LIST_BASED_STACK		1
//#define TEST_LIST_BASED_QUEUE				1
//#define TEST_DEQUE						1

#define TEST_USEFUL_HEAP					1

/////////////////////////////////////////////////////////////

#include "RecursionExample.h"

#ifdef TEST_LINKED_LIST
#include "LinkedList.h"

int WhoIsPrecede(LData d1, LData d2)
{
	if (d1.posX > d2.posX)
	{
		return 0;
	}
	else if (d1.posX == d2.posX)
	{
		if (d1.posY > d2.posY)
		{
			return 1;
		}
	}
}
#endif

#ifdef TEST_CIRCULAR_LINKED_LIST
#include "CircularLinkedList.h"
#endif

#ifdef TEST_BIDIRECTIONAL_LINKED_LIST
#include "BidirectionalLinkedList.h"
void	PrintListAscendingOrder(List* list);
void	PrintListDescendingOrder(List* list);

#ifdef TEST_BIDIRECTIONAL_LINKED_LIST

void PrintListAscendingOrder(List* list)
{
	Data	data;
	int		i, n;

	n = LCount(list);

	LFirst(list, &data);
	printf("Data in the list : \n%d\n", data);

	for (i = 0; i < (n - 1); i++)
	{
		if (LNext(list, &data) == TRUE)
		{
			printf("%d\n", data);
		}
	}

	printf("\n");
}

void PrintListDescendingOrder(List* list)
{
	Data	data;
	int		i, n;

	n = LCount(list);

	LEnd(list, &data);
	printf("Data in the list : \n%d\n", data);

	for (i = 0; i < (n - 1); i++)
	{
		if (LPrevious(list, &data) == TRUE)
		{
			printf("%d\n", data);
		}
	}

	printf("\n");
}

#endif
#endif

#include "ArrayBasedStack.h"

#ifdef TEST_LINKED_LIST_BASED_STACK
#include "ListBasedStack.h"
#endif

#include "CircularQueue.h"

#ifdef TEST_LIST_BASED_QUEUE
#include "ListBasedQueue.h"
#endif

#ifdef TEST_DEQUE
#include "Deque.h"
#endif

#include "BinaryTree.h"
#include "ExpressionTree.h"

#ifdef TEST_USEFUL_HEAP
#include "UsefulHeap.h"
#include "PriorityQueue.h"
#else
#include "SimpleHeap.h"
#endif

#include "SortingAlgorithms.h"
#include "BinarySearchTree.h"
#include "Table.h"
#include "ALGraph.h"

void ShowLinkedList(void)
{
#ifdef TEST_LINKED_LIST
	List	list;
	LData	data;
	ListInit(&list);

	SetSortRule(&list, WhoIsPrecede);

	data.posX = 1;	data.posY = 2;	SInsert(&list, data);
	data.posX = 3;	data.posY = 3;	SInsert(&list, data);
	data.posX = 3;	data.posY = 2;	SInsert(&list, data);
	data.posX = 6;	data.posY = 1;	SInsert(&list, data);
	data.posX = 9;	data.posY = 2;	SInsert(&list, data);

	printf("The number of data in the list : %d \n", LCount(&list));

	if (LFirst(&list, &data))
	{
		printf("(%d, %d) ", data.posX, data.posY);
		while (LNext(&list, &data))
			printf("(%d, %d) ", data.posX, data.posY);
	}

	printf("\n\n");

	if (LFirst(&list, &data))
	{
		if (data.posX == 6)
			LRemove(&list);

		while (LNext(&list, &data))
		{
			if (data.posX == 6)
				LRemove(&list);
		}
	}

	printf("The number of data in the list : %d \n", LCount(&list));

	if (LFirst(&list, &data))
	{
		printf("(%d, %d) ", data.posX, data.posY);
		while (LNext(&list, &data))
			printf("(%d, %d) ", data.posX, data.posY);
	}

	printf("\n\n");

#endif
}

void ShowCircularLinkedList(void)
{
#ifdef TEST_CIRCULAR_LINKED_LIST
	List	list;
	ListInit(&list);

	Data	data;

	LGetPersonInfo(&data, "SungsuKim", 100);	LInsert(&list, data);
	LGetPersonInfo(&data, "David", 101);	LInsert(&list, data);
	LGetPersonInfo(&data, "Solomon", 102);	LInsert(&list, data);
	LGetPersonInfo(&data, "GraceLee", 103);	LInsert(&list, data);
	LGetPersonInfo(&data, "Grape", 104);	LInsert(&list, data);

	int		days = 1;
	CString nameFrom("SungsuKim");
	CString	nameNext;

	LCheckNext(&list, nameFrom, nameNext, days);

	printf("%s...%d days...%s\n", (TCHAR*)(LPCTSTR)nameFrom, days, nameNext);	// It seems to differ from VS6.0, and I cannot print out string itself using %s.

#endif
}

void ShowBidirectionalLinkedList(void)
{
#ifdef TEST_BIDIRECTIONAL_LINKED_LIST

	List	list;
	Data	data;
	int		i, n;
	eOptionBL	option;

	ListInit(&list);

	option = eInsertFront;//eInsertEnd;	//;

	LInsert(&list, 1, option);
	LInsert(&list, 2, option);
	LInsert(&list, 3, option);
	LInsert(&list, 4, option);
	LInsert(&list, 5, option);
	LInsert(&list, 6, option);
	LInsert(&list, 7, option);

	n = LCount(&list);
	printf("The number of data in the list : %d\n", n);

	PrintListAscendingOrder(&list);
	PrintListDescendingOrder(&list);
	PrintListAscendingOrder(&list);

	//	LFirst(&list, &data);	printf("First	in the list : %d\n", data);
	LEnd(&list, &data);		printf("Last	in the list : %d\n", data);

	while (1)
	{
		if (LRemove(&list, &data) == TRUE)
		{
			printf("Data to be deleted : %d. ", data);
		}
		else
			break;
		printf("The number of data in the list : %d\n", LCount(&list));
	}

	printf("The number of data in the list : %d\n", LCount(&list));

	printf("\n");

	ListTerminate(&list);

#endif
}

void ShowCircularQueue(void)
{
	Queue q;

	QueueInit(&q);

	Enqueue(&q, 1);
	Enqueue(&q, 2);
	Enqueue(&q, 3);
	Enqueue(&q, 4);
	Enqueue(&q, 5);

	while(!QIsEmpty(&q))
		printf("%d ", Dequeue(&q));
}


void ShowListBasedQueue(void)
{
#ifdef TEST_LIST_BASED_QUEUE
	Queue	q;

	InitQueue(&q);

	Enqueue(&q, 1);
	Enqueue(&q, 2);
	Enqueue(&q, 3);
	Enqueue(&q, 4);
	Enqueue(&q, 5);
	Enqueue(&q, 6);

	while (!IsQEmpty(&q))
	{
		printf("%d ", Dequeue(&q));
	}

#endif
}

void ShowArrayBasedStack(void)
{
	Stack	stack;
	StackInit(&stack);

	SPush(&stack, 1);
	SPush(&stack, 2);
	SPush(&stack, 3);
	SPush(&stack, 4);
	SPush(&stack, 5);

	printf("Data on the stack : \n");
	if(!SIsEmpty(&stack))
		printf("%d (top)\n", SPop(&stack));

	while(!SIsEmpty(&stack))
		printf("%d \n", SPop(&stack));

	printf("_____\n");
}

void ShowListBasedStack(void)
{
#ifdef TEST_LINKED_LIST_BASED_STACK
	Stack	stack;
	StackInit(&stack);

	SPush(&stack, 1);
	SPush(&stack, 2);
	SPush(&stack, 3);
	SPush(&stack, 4);
	SPush(&stack, 5);

	while (!SIsEmpty(&stack))
	{
		printf("%d\n", SPop(&stack));
	}
#endif
}

void ShowDeque(void)
{
#ifdef TEST_DEQUE
	Deque	deq;
	Data	data;

	InitDeque(&deq);

	printf("First phase...\n");

	AddDequeFront(&deq, 1);
	AddDequeFront(&deq, 2);
	AddDequeFront(&deq, 3);
	AddDequeFront(&deq, 4);
	printf("\n");

	while (!IsDequeEmpty(&deq))
	{
		RemoveDequeFront(&deq);
	}

	printf("\n\n");
	printf("Second phase....\n");

	AddDequeEnd(&deq, 5);
	AddDequeEnd(&deq, 6);
	AddDequeEnd(&deq, 7);
	AddDequeEnd(&deq, 8);
	printf("\n");

	while (!IsDequeEmpty(&deq))
	{
		RemoveDequeEnd(&deq);
	}

	AddDequeEnd(&deq, 10);
	RemoveDequeFront(&deq);

#endif
}

void PrintData(BTData data);
void PrintData(BTData data)
{
	printf("%d ", data);
}

void ShowBinaryTree(void)
{
	BTreeNode*	bt1	= MakeBTreeNode(1);
	BTreeNode*	bt2 = MakeBTreeNode(2);
	BTreeNode*	bt3 = MakeBTreeNode(3);
	BTreeNode*	bt4 = MakeBTreeNode(4);

	MakeLeftSubTree(bt1, bt2);
	MakeRightSubTree(bt1, bt3);
	MakeLeftSubTree(bt2, bt4);

	printf("%d \n", GetData(GetLeftSubTree(bt1)));
	printf("%d \n\n", GetData(GetLeftSubTree(GetLeftSubTree(bt1))));

	printf("\n\nStart TraverseInOrder...\n");
	TraverseInOrder(bt1, PrintData);

	printf("\n\nStart TraversePreOrder...\n");
	TraversePreOrder(bt1, PrintData);

	printf("\n\nStart TraversePostOrder...\n");
	TraversePostOrder(bt1, PrintData);

	printf("\n\nDeleteTreeUnder...\n");
	DeleteTree(bt1);

	return;
}

void ShowExpressionTree(void)
{
	char exp[] = "12+7*";
	BTreeNode* eTree = MakeExpTree(exp);

	ShowPrefixTypeExp(eTree);	printf("\n");

	ShowInfixTypeExp(eTree);	printf("\n");

	ShowPostfixTypeExp(eTree);	printf("\n");

	printf("Result: %d \n\n", EvaluateExpTree(eTree));
}

void ShowSimpleHeap(void)
{
#ifndef	TEST_USEFUL_HEAP
	Heap	heap;
	HeapInit(&heap);

	HInsert(&heap, 'A', 1);
	HInsert(&heap, 'B', 2);
	HInsert(&heap, 'C', 3);
	printf("%c \n", HDelete(&heap));

	HInsert(&heap, 'A', 1);
	HInsert(&heap, 'B', 2);
	HInsert(&heap, 'C', 3);
	printf("%c \n", HDelete(&heap));

	while(!HIsEmpty(&heap))
		printf("%c \n", HDelete(&heap));
#endif
	return;
}

int DataPriorityComp(HData ch1, HData ch2)
{
	return ch2.weight - ch1.weight;
//	return ch1.weight - ch2.weight;
};

void ShowUsefulHeap(void)
{
#ifdef	TEST_USEFUL_HEAP
	Heap	heap;
	HData	data;

	UHeapInit(&heap, DataPriorityComp);
		
	data.weight	= 'A';	UHInsert(&heap, data);
	data.weight = 'B';	UHInsert(&heap, data);
	data.weight = 'C';	UHInsert(&heap, data);
	data.weight = 'D';	UHInsert(&heap, data);

	data	= UHDelete(&heap);
	printf("%c \n", data.weight);

	data.weight = 'A';	UHInsert(&heap, data);
	data.weight = 'B';	UHInsert(&heap, data);
	data.weight = 'C';	UHInsert(&heap, data);
	data.weight = 'D';	UHInsert(&heap, data);

	data = UHDelete(&heap);
	printf("%c \n", data.weight);

	while (!UHIsEmpty(&heap))
	{
		data = UHDelete(&heap);
		printf("%c \n", data.weight);
	}

	return;

#endif
}

void ShowPriorityQueue(void)
{
#ifdef	TEST_USEFUL_HEAP

	PQueue		pq;
	PQData		data;

	PQueueInit	(&pq, DataPriorityComp);

	data.weight = 'A';	PEnqueue(&pq, data);
	data.weight = 'B';	PEnqueue(&pq, data);
	data.weight = 'C';	PEnqueue(&pq, data);
	data.weight = 'D';	PEnqueue(&pq, data);
	data.weight = 'E';	PEnqueue(&pq, data);

	data = PDequeue(&pq);
	printf("%c \n", data.weight);

	data.weight = 'A';	PEnqueue(&pq, data);
	data.weight = 'B';	PEnqueue(&pq, data);
	data.weight = 'C';	PEnqueue(&pq, data);
	data.weight = 'D';	PEnqueue(&pq, data);
	data.weight = 'E';	PEnqueue(&pq, data);

	data = PDequeue(&pq);
	printf("%c \n", data.weight);
	
	while (!PQIsEmpty(&pq))
	{
		data = PDequeue(&pq);
		printf("%c \n", data.weight);
	}

#endif
}

#define SIZE_ARRAY	10
int	g_array[SIZE_ARRAY] = { 8,10,3,2,9,6,7,4,5,1 };

void ShowSorting(void)
{
	int*	array	= g_array;	// to monitor as a global array.
	int		i;

/*	Before sorting...*/
	i = SIZE_ARRAY;
	while(i--)
		printf("array[%d]	=  [%d]\n", i, array[i]);
	printf("\n");

/*	Sorting..*/
	int	n;
	printf("Choose a sorting algorithm(1~7) : ");
	scanf_s("%d", &n);
	switch (n)
	{
		case 1:	BubbleSort		(array, SIZE_ARRAY);					break;
		case 2:	SelectionSort	(array, SIZE_ARRAY);					break;
		case 3: InsertionSort	(array, SIZE_ARRAY);					break;
		case 4: HeapSort		(array, SIZE_ARRAY, DataPriorityComp);	break;
		case 5: MergeSort		(array, SIZE_ARRAY);					break;
		case 6: QuickSort		(array, SIZE_ARRAY);					break;
		case 7: RadixSort		(array, SIZE_ARRAY);					break;

		default:	
			break;
	}
	
/*	After sorting...*/
	i = SIZE_ARRAY;
	while (i--)
		printf("array[%d]	=  [%d]\n", i, array[i]);

	printf("\n");

	return;

#undef SIZE_ARRAY
}

void ShowRecursion(void)
{
	int	n = 5;

	Recursive(n);

	printf("%d! = %d\n", n, Factorial(n));
	printf("Fibonacci of %dth = %d\n", n, FibonacciSeq(n));

	{
		int arr[]	= {1,3,5,7,9};
		int	nSearch	= 2;
		int index;

		index = BinarySearch(arr, 0, 4, nSearch);
		printf("BinarySearch result=%d\n\n", index);
	}

	{
		int arr[]	= { 1,3,5,7,9 };
		int	nSearch = 2;
		int index;

		index = InterpolationSearch(arr, 1, 4, nSearch);
		printf("InterpolationSearch result=%d\n\n", index);
	}

	HanoiTower(3);

	n = 0;
	return;
}

void SearchBTree(BTreeNode* pNode, BTData target)
{
	BTreeNode*	pSNode;

	pSNode = BSTSearch(pNode, target);
	if (pSNode == NULL)	
	{
		printf("Failed to search data %d in the tree.\n", target);
	}
	else
	{
		printf("Succeeded to find data %d in the list.\n", target);
	}
}

void ShowBinarySearchTree(void)
{
	BTreeNode*	pRootNode;
	BTreeNode*	pSNode;

	BSTMakeAndInit(&pRootNode);					// since pRootNode is a poninter, &pRootNode will represent the memory address pRootNode as a variable is located.

	BSTInsert(&pRootNode, 9);
	BSTInsert(&pRootNode, 1);
	BSTInsert(&pRootNode, 6);
	BSTInsert(&pRootNode, 2);
	BSTInsert(&pRootNode, 8);
	BSTInsert(&pRootNode, 3);
	BSTInsert(&pRootNode, 5);

	SearchBTree(pRootNode, 1);
	SearchBTree(pRootNode, 2);
	SearchBTree(pRootNode, 3);
	SearchBTree(pRootNode, 4);
	SearchBTree(pRootNode, 5);

	BSTShowAll(pRootNode, PrintData);	printf("\n");
	pSNode = BSTRemove(&pRootNode, 1);
	free(pSNode);

	BSTShowAll(pRootNode, PrintData);	printf("\n");
	pSNode = BSTRemove(&pRootNode, 2);
	free(pSNode);

	BSTShowAll(pRootNode, PrintData);	printf("\n");
	pSNode = BSTRemove(&pRootNode, 3);
	free(pSNode);

	BSTShowAll(pRootNode, PrintData);	printf("\n");
	pSNode = BSTRemove(&pRootNode, 4);
	free(pSNode);

	BSTShowAll(pRootNode, PrintData);	printf("\n");
	pSNode = BSTRemove(&pRootNode, 5);
	free(pSNode);

	BSTShowAll(pRootNode, PrintData);	printf("\n");
}

int  MyHashFunc(int k)
{
	return (k % 100);
}

void ShowHashTable(void)
{
	Table	myTbl;
	Person*	np;
	Person*	sp;
	Person* rp;

	TBLInit(&myTbl, MyHashFunc);

	np = MakePersonData(19830001, "KimSungSu", "Seoul");
	TBLInsert(&myTbl, GetSSN(np), np);

	np = MakePersonData(19860002, "LeeEunHye", "Anyang");
	TBLInsert(&myTbl, GetSSN(np), np);

	np = MakePersonData(20170003, "KimDaEl", "Seongnam");
	TBLInsert(&myTbl, GetSSN(np), np);

	sp = TBLSearch(&myTbl, 19830001);
	if(sp != NULL)
		ShowPerInfo(sp);

	sp = TBLSearch(&myTbl, 19860002);
	if (sp != NULL)
		ShowPerInfo(sp);

	sp = TBLSearch(&myTbl, 20170003);
	if (sp != NULL)
		ShowPerInfo(sp);

	rp = TBLDelete(&myTbl, 19830001);
	if (rp != NULL)
		free(rp);

	rp = TBLDelete(&myTbl, 19860002);
	if (rp != NULL)
		free(rp);

	rp = TBLDelete(&myTbl, 20170003);
	if (rp != NULL)
		free(rp);
}

void ShowGraph(void)
{
	ALGraph		graph;
	int			bUseKruskal = false;

#if (1)
	GraphInit(&graph, 6);

	AddEdge(&graph, A, B, 9);
	AddEdge(&graph, B, C, 2);
	AddEdge(&graph, A, C, 12);
	AddEdge(&graph, A, D, 8);
	AddEdge(&graph, D, C, 6);
	AddEdge(&graph, A, F, 11);
	AddEdge(&graph, F, D, 4);
	AddEdge(&graph, D, E, 3);
	AddEdge(&graph, E, C, 7);
	AddEdge(&graph, F, E, 13);
		
	printf("Do you want to apply Kruscal Algorithm? (more than 0 to Yes): ");
	scanf_s("%d", &bUseKruskal);

	if(bUseKruskal)
	{
		printf("Kruskal Algorithm executes...\n");
		ConKruskalMST(&graph);
		printf("end..\n\n");
	}

	ShowGraphEdge(&graph);
	printf("\n");

	if(bUseKruskal)
	{
		printf("Minimum Spanning Tree has been achieved! \n");
		ShowGraphEdgeWeightInfo(&graph);
	}

#else	
	GraphInit(&graph, 7);

	AddEdge(&graph, A, B, NULL);
	AddEdge(&graph, A, D, NULL);
	AddEdge(&graph, B, C, NULL);
	AddEdge(&graph, D, C, NULL);
	AddEdge(&graph, D, E, NULL);
	AddEdge(&graph, E, F, NULL);
	AddEdge(&graph, E, G, NULL);

	ShowGraphEdge(&graph);
	printf("\n");

	printf("DFS : Depth First Search..\n");
	ShowDFSGraphVertex(&graph, A);	printf("\n");
	ShowDFSGraphVertex(&graph, C);	printf("\n");
	ShowDFSGraphVertex(&graph, E);	printf("\n");
	ShowDFSGraphVertex(&graph, G);	printf("\n");
	printf("\n");

	printf("BFS : Breadth First Search..\n");
	ShowBFSGraphVertex(&graph, A);	printf("\n");
	ShowBFSGraphVertex(&graph, C);	printf("\n");
	ShowBFSGraphVertex(&graph, E);	printf("\n");
	ShowBFSGraphVertex(&graph, G);	printf("\n");
#endif

	GraphDestroy(&graph);
}