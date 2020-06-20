
#include <stdio.h>
#include <stdlib.h>

#include "..//include/LinkedList.h"

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

int main()
{
/*	{
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
	}*/

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

	return 0;
}