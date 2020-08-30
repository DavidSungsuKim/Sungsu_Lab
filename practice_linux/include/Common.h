
#if ( __SIZEOF_LONG__ == 8 )
    #define _LONG_8BYTE__
#endif

#include <stdlib.h>
#include <stdio.h>

#define _printf_error(x) 		printf("error:%s, file=%s(L=%d)\n", x, __FILE__, __LINE__ );
#define _printf_ret_error(ret, x) 	printf("error:%s, ret=%d file=%s(L=%d)\n", x, ret, __FILE__, __LINE__ );
