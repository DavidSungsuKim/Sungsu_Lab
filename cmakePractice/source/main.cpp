
#include "app.hpp"
#include <stdio.h>

int main(int arg, char *args[])
{
	int x = AppFunctionOne();	
	printf("AppFunctionOne()=%d\r\n", x);
	
	int i;
	for ( i = 0; i < arg; i++ )
	{
		printf("arg[%d], %s\r\n", i, args[i]);
	}
	
	return 0;
}