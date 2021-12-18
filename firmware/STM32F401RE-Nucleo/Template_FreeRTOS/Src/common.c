/**************************************************************************
 * @file    common.c
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

#include "common.h"
#include "HALinterface.h"

#define SIZE_BUFF_TIMESTAMP		9
#define SIZE_BUFF_PRINTF		128
#define SIZE_BUFF_PRINTF_EX		256
#define SIZE_MAX_SENTENCE		(SIZE_BUFF_PRINTF - SIZE_BUFF_TIMESTAMP)
#define	PRINT_ONLY_FILENAME

void PrintfUART(const char* str, ...)
{
    uint32_t tick = HALIF_GetSysTick();

	int length = strlen(str);
	if ( length >= SIZE_MAX_SENTENCE )
	{
		HALIF_UART2SendSync("PrintfUART: 'str' exceeded the buffer.\r\n");
		return;
	}

	// To make use of variable-length argument lists
    char buf[SIZE_BUFF_PRINTF] = {0,};
    sprintf(buf, "%8ld ", tick);

    va_list ap;
    va_start(ap, str);
    vsprintf(&buf[SIZE_BUFF_TIMESTAMP], str, ap);
    va_end(ap);

    puts(buf);

    HALIF_UART2SendSync(buf);
}

void PrintfUARTEx(const char *file, int line, const char* str, ...)
{
	uint32_t tick = HALIF_GetSysTick();

	int length = strlen(str);
	if ( length >= SIZE_MAX_SENTENCE )
	{
		HALIF_UART2SendSync("PrintfUARTEx: 'str' exceeded the buffer.\r\n");
		return;
	}

	// To make use of variable-length argument lists
	char buf[SIZE_BUFF_PRINTF] = {0,};
	sprintf(buf, "%8ld ", tick);

    va_list ap;
    va_start(ap, str);
    vsprintf(&buf[SIZE_BUFF_TIMESTAMP], str, ap);
    va_end(ap);

    puts(buf);

    int index = 0;

#ifdef PRINT_ONLY_FILENAME
    int i = strlen(file);
    while(i--)
    {
    	if ( file[i] == '\\')
    	{
    		index = i+1;
    		break;
    	}
    }
#endif

	length = strlen(file) - index;
	if ( length >= SIZE_BUFF_PRINTF )
	{
		HALIF_UART2SendSync("PrintfUARTEx: 'file' exceeded the buffer.\r\n");
		return;
	}

	char bufEx[SIZE_BUFF_PRINTF_EX] = {0,};
	sprintf(bufEx, "Ex %s L=%d: %s", &file[index], line, buf);

	HALIF_UART2SendSync(bufEx);
}
