
#include <stdio.h>
#include <stdlib.h>

#include "..//include/LinkedList.h"
#include "..//include/DoublyLinkedList.h"

void TestLinkedList			();
void TestDoublyLinkedList	();

int main()
{
//	TestLinkedList();
	TestDoublyLinkedList();

	return 0;
}

void TestLinkedList()
{
	int i;

	StList list;
	ListInit(&list);

	for (i = 0; i < 10; i++)
	ListInsert(&list, i);

	int data;
	ListFirst(&list, &data);
	printf("1st data=%d\n", data);

	while (!ListNext(&list, &data))
	printf("data=%d\n", data);

	while (!ListRemove(&list, &data))
	printf("data(removed)=%d, count=%d\n", data, ListCount(&list));

	ListTerminate(&list);
}

void TestDoublyLinkedList()
{
	CDoublyList list;

	int i;
	Data a;

	for (i = 0; i < 10; i++)
	{
		a = i;
		//	list.PushBack(a);
		list.PushFront(a);
	}

	list.First(a);
	printf("first=%d\n\n", a);
	for (i = 0; i < 10; i++)
	{
		list.Next(a);
		printf("next=%d\n", a);
	}
	printf("\n");

	for (i = 0; i < 10; i++)
	{
		list.Prev(a);
		printf("prev=%d\n", a);
	}

	//	for (i = 0; i < 10; i++)
	//		list.PopBack(a);

	a = 3;
	printf("find %d == %s\n", a, list.Find(a) == 0 ? "found" : "not found");

	list.RemoveAll();
}