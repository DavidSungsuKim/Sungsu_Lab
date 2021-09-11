
#include <stdio.h>
#include "ModernCpp.h"

void CModernCpp::TypeDeduction()
{
	int x = 0;

	int* y = &x;
	int z = (int)&y;

	printf("x=0x%x, y=0x%x, z=0x%x\r\n", x, y, z);
}
