
#include <stdio.h>
#include <iostream>
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