
#include <stdio.h>
#include <iostream>
#include <vector>
#include "ModernCpp.h"

void CModernCpp::TypeDeduction()
{
	int x = 0;

	int* y = &x;
	int z = (int)&y;

	printf("x=0x%x, y=0x%x, z=0x%x\r\n", x, (int)y, z);
}

void func(int x) { /* don't do anything */ }

void CModernCpp::Auto()
{
#if 0
	auto a = 0;
	auto b = 0.0;

	int sizeA = sizeof(a);
	int sizeB = sizeof(b);
#endif

#if 0
	auto i = 0.1235;
	int size = sizeof(i);
	std::cout <<"auto(i)="<< i <<"  size="<< size << std::endl;
#endif

#if 0
	unsigned char x = 1;
	auto xAuto = x;
	int size = sizeof(xAuto);

	xAuto = 1.3;

	printf("size=%d, xAuto=%.f\r\n", size, xAuto);
#endif

#if 0
	CModernCpp x;

	auto* a = &x;
	a->Auto();
#endif

#if 0
	auto *a = func;
	a(1);
#endif
}

void CModernCpp::UniversalReference()
{
	int i = 0;
	auto& r = i;
	r = 1;        

	auto& r2 = r;
	r2 = 2;        

	auto&& r3 = r;
	r3 = 2;         

//	auto& r4 = 1;   // Compile Error!!! 
	auto&& rr2 = 1;
}

void CModernCpp::Initialization()
{
	auto a = 0;
	auto b = { 0 }; // std::initializer_list<int>
	auto c = { 1, 2 };

//	b = a; // compile rror
	b = c;
}

auto CModernCpp::ReturnAuto( int a )
{	
	auto ret = a;
	return ret;
}

void CModernCpp::DeclType()
{
#if 0
	int x = 0;
	decltype(x)	y = 1;

	decltype(0.1) z = 1.2;
#endif

#if 0
	std::vector<int> testList;
	testList.push_back(1);

	int index = 0;
	int value = AuthAndAccess(testList, 0);

	AuthAndAccess(testList, 0) = 3;
	value = testList.at(0);
#endif 

#if 0
	std::vector<int> testList;z
	testList.push_back(1);

	AuthAndAccess2(testList, 0) = 3;

	int value = testList.at(0);
#endif

#if 0
	int x;
	decltype(x) y = 1;
	decltype((x)) z = y;
#endif

	const int x = 0;
	printf("x's name=%s\r\n", typeid(x).name());
}

#include <functional>
void PrintWhatYouHave(int a) { printf("a=%d\r\n", a); }
void CModernCpp::StdFunction()
{
	std::function<void(int)> func;

	func = PrintWhatYouHave;
	func(1);
}

void CModernCpp::LamdaExpression()
{
	int x1 = 1;
	int x2 = 2;

	auto lamdaTest1 = [ ] (int a)		{ printf("lamdaTest1\r\n"); return (a *= 2); };
	
	auto lamdaTest2 = [=](int a) { printf("lamdaTest2\r\n"); return (x1 * 2); };
	
	auto lamdaTest3 = [&] (int a)		{ printf("lamdaTest3\r\n"); return (x1 *= 2); };

	auto lamdaTest4 = [=, &x1](int a)	{ printf("lamdaTest4\r\n"); return (x1 *= (2 * x2)); };

	int ret;
	
	ret = lamdaTest1(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);

	ret = lamdaTest2(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);

	ret = lamdaTest3(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);

	ret = lamdaTest4(2);
	printf("ret=%d, x1=%d, x2=%d\r\n", ret, x1, x2);
}

void CModernCpp::LamdaExpressionForDisassembly()
{
	int x1 = 1;

	auto lamdaTest1 = [&](int a)
	{
		return (x1 *= 2);
	};

	int ret = lamdaTest1(2);
	printf("ret=%d, x1=%d\r\n", ret, x1);
}

void CModernCpp::ParentheseBraces()
{
	int x{ 0 }; // braces, allowed only in modern C++
	int y = 0; 
	int z(0); // parentheses

	std::vector<int> test = { 1,2,3 };

	int a = { 1 + 2 + 3 }; // It works!
	int b(1 + 2 + 3);	   // It works either!

	int value;
	CTestClass a1;
	CTestClass a2(10);
	value = a2.getValue();

//	CTestClass a3();	// It's actually for declaring a function!
//	value = a3.getValue();

	CTestClass a4{};
}

void CModernCpp::InitializerList()
{
	int num;
	CTestClass a1{ 1,2 };
	num = a1.getNumElement();

	CTestClass a2{ 1,2,3,4,5,6 };
	num = a2.getNumElement();

	CTestClass a3{ 1, 2.0 };
	num = a3.getNumElement();

	CTestClass a4{};
	num = a4.getNumElement();
}

void CModernCpp::NullPtr()
{
	CTestClass  testNull;
	testNull.func(0);
}