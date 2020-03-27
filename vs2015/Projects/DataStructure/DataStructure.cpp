
#include "Common.h"

#include "Examples.h"

int main(void)
{
	printf("//////////// Recursion Example /////////////\n");
	ShowRecursion();
	printf("\n\n");

	printf("//////////// Linked List /////////////\n");
	ShowLinkedList();
	ShowCircularLinkedList();
	ShowBidirectionalLinkedList();
	printf("\n\n");

	printf("//////////// Stack /////////////\n");
	ShowArrayBasedStack();
	ShowListBasedStack();
	printf("\n\n");

	printf("//////////// Queue and Deque /////////////\n");
	ShowCircularQueue();
	ShowListBasedQueue();
	ShowDeque();
	printf("\n\n");

	printf("//////////// Tree /////////////\n");
	ShowBinaryTree();
	ShowExpressionTree();
	printf("\n\n");

	printf("//////////// Priority Queue /////////////\n");
	ShowSimpleHeap();
	ShowUsefulHeap();
	ShowPriorityQueue();
	printf("\n\n");

	printf("//////////// Sorting Algorithms /////////////\n");
	ShowSorting();
	printf("\n\n");

	printf("//////////// Binary Search Tree /////////////\n");
	ShowBinarySearchTree();
	printf("\n\n");

	printf("//////////// Hash Table /////////////\n");
	ShowHashTable();
	printf("\n\n");

	printf("//////////// Graph /////////////\n");
	ShowGraph();
	printf("\n\n");

	return 0;
}




