
#include <stdio.h>
#include <stdlib.h>

#include "..//include/Common.h"

#ifdef _OS_WINDOWS_
#include <process.h>
#include <windows.h>
#include <time.h>
#endif

#include <string>

#include "..//include/LinkedList.h"
#include "..//include/DoublyLinkedList.h"
#include "..//include/StructTest.h"
#include "..//include/ThreadTest.h"
#include "..//include/DesignPattern.h"
#include "..//include/Stack.h"
#include "..//include/Queue.h"
#include "..//include/ClassTest.h"

//////////////////////////////////////////////////////////////////////////
void TestLinkedList			();
void TestDoublyLinkedList	();
void TestStruct				();
void TestDesignPattern		();
void TestStack				();
void TestQueue				();
void TestPrintLog			(char* str, ...);

//////////////////////////////////////////////////////////////////////////
HANDLE	g_hThread	= NULL;
StMotor g_motor;

void			TestFPointerThread();
void			MakeThread	(HANDLE& arHandle, void* apContext);
static	void	ThreadProc	(LPVOID* pParam);
void			Task		(StMotor* apMotor);
//////////////////////////////////////////////////////////////////////////

typedef void pVoid(void);
void Test (void) { printf("void function\n"); }

namespace std
{
	int Test1() { return 1; }
};

int Test1() { return 2; }

char* my_itoa(int val, char *buf, int radix);
char* my_itoa(int val, char *buf, int radix)
{
	// Suppose buf is big enough

	enum { OFFSET_ASCII = 48 };

	// Handling exception
	if ( radix <= 0 )
		abort();

	int i			= 0;
	int quotient	= abs(val);
	int quotientNew;
	int rem;

	// Calculating into string
	do
	{
		quotientNew = quotient / radix;
		rem			= quotient - quotientNew * radix;

		quotient	= quotientNew;
		buf[i++]	= rem + OFFSET_ASCII;
	}
	while ( quotient >= radix );

	if ( quotient )
		buf[i++] = quotient + OFFSET_ASCII;
	if ( val < 0 )
		buf[i++] = '-';

	// Reversing
	int count = i;
	int loop  = count / 2;

	count--;
	for (i = 0; i < loop; i++)
	{
		char temp		= buf[i];
		buf[i]			= buf[count-i];
		buf[count-i]	= temp;
	}
	count++;

	// Appending EOS
	buf[count] = '\0';
	
	return buf;
}

int main()
{
// 	int i;
// 	printf("aaaaaaaaaaaaaaaaaaaaaaaaa");
// 	
// 	printf("\b");
// 	printf("b");
// 
// 	for(i=0; i<100; i++)
// 	{
// 		printf("Percent=%d\r",i);
// 		Sleep(100);
// 	}
// 
// 	printf("ok?\n");

// 	char *const string1 = "Hello\n";
// 	char *const string2 = "Hello\n";
// 	
// 	//string1 = string2;
// 	string1[0] = 1;
// 
// 	printf("%s", string1);

// 	char	buff[256];
// 	char	*pString;
// 	int		radix = 2;
// 
// 	pString = my_itoa( -1, buff, 10 );
// 
// 	printf("%s\n", pString);

//	TestLinkedList();
//	TestDoublyLinkedList();
//	TestStruct();
//	TestFPointerThread();
//	TestDesignPattern();
//	TestStack();
//	TestQueue();

//	StatePatternC1();
//	StatePatternC2();

// 	long	a = 1;
// 	long*	pa = &a;
// 	long	b = 2;
// 
// 	_asm	mov	ecx, pa;
// 	_asm	mov	eax, b;
// 	_asm	or [ecx], eax;
// 
// 	long	a2	= 1;
// 	long	b2	= 2;
// 
// 	a2 |= b2;

// 	char ch;	
// 	ch = getchar();
// 	putchar(ch);

//	TestPrintLog("test %d %d %d\n", 1,2,3);

//	char buf[256] = {0,};
//	int day = 1;
//	scanf_s("%d", &day);

// 	int *pm = (int*)malloc(sizeof(int));
// 	*pm = 1;
// 	int *pc = (int*)calloc(10, sizeof(int));
// 
// 	ldiv_t l = ldiv(5,3);
// 
// 
// 	char buff[10];
// 
// 	strcpy_s(buff, 10, "123456789");
// 
// 	#pragma pack(1)
// 	typedef struct
// 	{
// 		int  x1;
// 		char x2;
// 	}StSizeTest;
// 	#pragma pack()
// 
// 	typedef struct
// 	{
// 		int  x1;
// 		char x2;
// 	}StSizeTest2;
// 
// 	size_t size = sizeof(StSizeTest);
// 	size_t size2 = sizeof(StSizeTest2);

//	StatePatternC3();

// 	char xx [10];
// 
// 	int x = 1;
// 	x = (int)&xx;
// 	div_t t = div(x,4);

// 	COperator opA(1,1);
// 	COperator opB(2,2);
// 	COperator opC;
// 
// 	opA = opB;
// 	opA += opB;
// 
// 	opC = opA + opB;
// 
// 	opC = opB + 2;
// 	opC = 2 + opB; // non member function ÀÎ operator+ »ç¿ë!

// 	std::string str;
// 	int x1;
// 
// 	x1 = std::Test1();
// 	x1 = Test1();

// 	CParent1 *pp;
// 	CChild1 cc;
// 
// 	pp = &cc;
// 
// 	CChild1 *pc = dynamic_cast<CChild1*>(pp);
// 	int x = pc->GetVal();
// 
// 	CChild1 *pc2 = (CChild1*)pp;
// 	x = pc->GetVal();

// 	CChild2 c2;
// 	int x = c2.Func1();
// 
// 	x = c2.Func2();
// 	x = c2.Func1(3);

// 	CParent2 *pp2;
// 	CChild2 cc2;
// 
// 	pp2 = &cc2;
// 	int x;
// 	x = pp2->Func1(10); 
// 
// 	CChild2 *pc2 = &cc2;
// 	x = pc2->Func1(10);

// 	long tickStart;
// 	long tick;
// 	int	i;
// 	int	loop = 100000000;
// 
// 	volatile double* pDoubleOri = new double[10];
// 	volatile double* pDouble = pDoubleOri;
// 	ldiv_t l;
// 
// 	tickStart = GetTickCount();
// 	for (i = 0; i < loop; i++)
// 	{
// 		pDouble[1] = i;
// 	}
// 	tick = GetTickCount() - tickStart;
// 
// 	l = ldiv((long)pDouble, 8);
// 
// 	char* pChar = (char*)pDouble;
// 	pChar++;
// 	pChar++;
// 	pChar++;
// 	
// 	pDouble = (double*)pChar;
// 
// 	tickStart = GetTickCount();
// 	for (i = 0; i < loop; i++)
// 	{
// 		pDouble[1] = i;
// 	}
// 	tick = GetTickCount() - tickStart;
// 
// 	l = ldiv((long)pDouble, 8);
// 
// 	delete [] pDoubleOri;


	return 0;
}

void TestPrintLog(char* str, ...)
{
	va_list ap;

	va_start(ap, str);

	for( int i = 0; i < 3; i++)
	{
		int x = va_arg(ap, int);
		printf("%d", x);
	}

	va_end(ap);
}
// void*
// CWinTest::ThreadConsumer(void* arg)
// {
// 	CWinTest* 	pThis = (CWinTest*)arg;
// 	StCondition*	pCond = &pThis->m_Cond;
// 	if (!pCond)
// 		return NULL;
// 
// 	long timeOut = 1000;
// 
// 	for (;;)
// 	{
// 		WaitForSingleObject(pCond->mutex, INFINITE);
// 		while (pCond->msg[0] == '\0')         
// 		{
// 			ReleaseMutex(pCond->mutex);
// 			WaitForSingleObject(pCond->event, timeOut);
// 			WaitForSingleObject(pCond->mutex, INFINITE);
// 		}
// 
// 		if (!strcmp(pCond->msg, "quit"))                 
// 		{
// 			ReleaseMutex(pCond->mutex);
// 			break;
// 		}
// 
// 		memset(pCond->msg, 0, sizeof(pCond->msg));  
// 		ReleaseMutex(pCond->mutex);
// 	}
// 	return NULL;
// }

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

void TestStruct()
{
	StMotor motor;

	int ret;
	ret = motor.Move(1);

	motor.EventHandler();

	motor.EventHandler = HandleEventServo;
	
	motor.EventHandler();
}

void TestFPointerThread()
{
	ULONGLONG tickStart, tickEnd;

	tickStart = GetTickCount();
	MakeThread(g_hThread, &g_motor);	// The thread will last during 10 seconds.

	Sleep(3000);
	g_motor.EventHandler = HandleEventServo;

	WaitForSingleObject(g_hThread, INFINITE);

	tickEnd = GetTickCount();
	printf("Tick diff = %lld\n", tickEnd - tickStart);
}

void TestStack()
{
	CStack stack;

	Data a;
	
	a = 1;	stack.Push(a, true);
	a = 2;	stack.Push(a, true);
	a = 3;	stack.Push(a, true);
	a = 4;	stack.Push(a, true);

	stack.Peek(a, true);

	stack.Pop(a, true);
	stack.Pop(a, true);
	stack.Pop(a, true);
	stack.Pop(a, true);
}

void TestQueue()
{
	CQueue queue;

	Data a;
	a = 1; queue.Enqueue(a, true);
	a = 2; queue.Enqueue(a, true);
	a = 3; queue.Enqueue(a, true);
	a = 4; queue.Enqueue(a, true);

	queue.Peek(a, true);

	queue.Dequeue(a, true);
	queue.Dequeue(a, true);
	queue.Dequeue(a, true);
	queue.Dequeue(a, true);

	queue.Peek(a, true);
}

void TestDesignPattern()
{
	Singleton* p = Singleton::Get();
	p->Test();
}

void MakeThread(HANDLE& arHandle, void* apContext)
{
#ifdef _OS_WINDOWS_
	arHandle = (HANDLE)_beginthreadex(NULL,
									0,										// 0 : use default stack size (1024KB)
									(_beginthreadex_proc_type)ThreadProc,	// I need to use type-casting on VS2015
									apContext,								// To designate the address of this class instance
									0,										// 0 : Execute right away, 1 : CREATE_SUSPENDED
									NULL);

//	SetThreadPriority( arHandle, THREAD_PRIORITY_TIME_CRITICAL );
#endif
}

void ThreadProc(LPVOID* pParam)
{
	StMotor* pMotor = (StMotor*)pParam;
	Task( pMotor );

#ifdef _OS_WINDOWS_
	_endthreadex(0);
#endif
}

void Task(StMotor* apMotor)
{
	int count = 0;

	for(;;)
	{
		Sleep(1000);
		count++;

		apMotor->EventHandler();

		printf("Task...%d[sec]\n", count);

		if ( count > 10 )
			break;
	}
}