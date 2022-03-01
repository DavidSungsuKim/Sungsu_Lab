// SungsuWin32.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C"
{
	typedef void (*pFunc)(void);
	void Func1	(int x, pFunc fp);
	void Func11	(void);
}

void Func1	(int x, pFunc fp)	{ }
void Func11	(void)				{ }
void Func2	(void)				{ }

int main()
{
	Func1(1, &Func11);
	Func1(1, &Func2);

	return 0;
}