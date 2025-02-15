﻿// ModernCplusplus.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "ModernCpp.h"

void TestBasics();
void TestModernCpp();
void TestModernTemplate();

int main()
{
	TestBasics();
	TestModernCpp();
	TestModernTemplate();
	
	return 0;
}

void TestBasics()
{
	int array[10];

	int size = sizeof(array);

	printf("size=%d\r\n", size);
}

void TestModernCpp()
{
	printf("test modern C++\r\n");

	CModernCpp	modern;
	modern.TestRandom();
	modern.TypeDeduction();
	modern.UniversalReference();
	modern.Auto();
	modern.Initialization();
	modern.DeclType();
	modern.StdFunction();
	modern.LamdaExpression();
	modern.LamdaExpressionForDisassembly();
	modern.ParentheseBraces();
	modern.InitializerList();
	modern.NullPtr();
	modern.ScopedEnums();
	modern.Tuple();
	modern.Delete();
	modern.DeleteForNonMembers();
	modern.Override();
	modern.ConstIterator();
	modern.NoExcept();
	modern.ConstExpr();
	modern.StdMutex();
	modern.StdAtomic();
	modern.UniquePointer();
	modern.DesignatedInitializer();
	modern.SharedPointer();
	modern.WeakPointer();
	modern.MakeShared();
	modern.MoveAndForward();
	modern.PerfectForwarding();
	modern.UtilizeTemplate();
	modern.FunctionObj();
	modern.ConstIterator2();
}

void TestModernTemplate()
{
	int a = 1234;
	CModernTemplate<int> test(a);

	int b = test.Return();
	printf("a=%d, b=%d\r\n", a, b);

	test.Func1(b);

	printf("b=%d\r\n", b);
	test.Func3(b);
	printf("b=%d\r\n", b);
}