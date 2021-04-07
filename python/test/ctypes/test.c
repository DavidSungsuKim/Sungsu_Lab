
#include <stdio.h>
#include "test.h"

int add(int a, int b)
{
    int ret = a+b;
 //   printf("%d=add(%d, %d)\n", ret,a,b); // not allowed in ctypes
    return ret;
}