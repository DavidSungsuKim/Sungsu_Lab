
#if ( __SIZEOF_LONG__ == 8 )
    #define _LONG_8BYTE__
#endif

#include <stdlib.h>
#include <stdio.h>

#define printf_error(x) printf("error:%s, file=%s, line=%d\n", x, __FILE__, __LINE__ );
