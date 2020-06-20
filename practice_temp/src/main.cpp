
#pragma once

#include <stdio.h>
#include <stdlib.h>

extern "C"
{
	typedef void(*pFunc)(void);
	void Func1(int x, pFunc fp);
	void Func11(void);
}

void Func1(int x, pFunc fp) { }
void Func11(void) { }
void Func2(void) { }

class A
{
public:
	A() {}
	~A() {}

protected:
	int a = 1;

	virtual void Func1(void) {};
};

class B : protected A
{
public:
	B() {}
	~B() {}
protected:
	B(int i) { /* do something */ }

public:
	int Test() { A::a = 2; return A::a; }

	void Func1(void) {};
};

class C : public B
{
public:
	C() : B(4) {}
	~C() {}

public:
	int Test() { /*B::A::a = 3; */return B::A::a; }

};

class D
{
private:
	D(int a) {};

	int a;
};

static int s_1 = 0;

void TestStatic()
{
	static int s_2 = 0;
}


typedef struct stNode
{
	int data;
	stNode *pNext;
}StNode;

typedef struct stList
{
	StNode *head;
	StNode *tail;
	StNode *cur;
	int		num;
}StList;

void	ListInit(StList *apList);
int		ListFirst(StList *apList, int *apData);
void	ListInsert(StList *apList, int aData);
int		ListNext(StList *apList, int *apData);
int		ListRemove(StList *apList, int *apData);
int		ListCount(StList *apList);

int main()
{
	{
		Func1(1, &Func11);
		Func1(1, &Func2);

		s_1 = 0;

		B b;
		int ret;
		ret = b.Test();

		C c;
		ret = c.Test();

		int i;
		int a[5];
		int *p = a;
		for (i = 0; i < 5; i++)
		{
			*p = i;
			printf("a[%d]=%d, addr=0x%d\n", i, i, p);
			p++;
		}

		i = 0;
	}

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

	return 0;
}

void
ListInit(StList *apList)
{
	apList->head = NULL;
	apList->cur = NULL;
	apList->num = 0;
}

int
ListFirst(StList *apList, int *apData)
{
	if (!apList->num)
		return -1;

	*apData = apList->head->data;
	apList->cur = apList->head->pNext;
	return 0;
}

void
ListInsert(StList *apList, int aData)
{
	StNode* node = (StNode*)malloc(sizeof(StNode));

	node->data = aData;

	if (apList->head == NULL)
	{
		node->pNext = NULL;
		apList->head = node;
	}
	else
	{
		node->pNext = apList->head;
		apList->head = node;
	}

	apList->num++;
}

int
ListNext(StList *apList, int *apData)
{
	if (!apList->num)
		return -1;

	if (!apList->cur)
		return -2;

	*apData = apList->cur->data;
	apList->cur = apList->cur->pNext;

	return 0;
}

int
ListRemove(StList *apList, int *apData)
{
	if (!apList->num)
		return -1;

	*apData = apList->head->data;

	StNode* node = apList->head;
	apList->head = apList->head->pNext;

	free(node);

	apList->num--;

	return 0;
}

int
ListCount(StList *apList)
{
	return apList->num;
}