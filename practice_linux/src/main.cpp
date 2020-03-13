#include <stdio.h>
#include "class.h"

int main() 
{
	printf("Hello Sungsu! I am RP 4B!\r\n");
	CTestChild testChild;
	CTest* pTest = &testChild;

	int ret = 0;
	ret = pTest->Func();

	printf("%d=pTest->Func()\r\n", ret);
	return 0;
}
