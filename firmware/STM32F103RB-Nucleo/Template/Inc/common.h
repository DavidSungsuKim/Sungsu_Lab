
#ifndef _COMMON_H_
#define _COMMON_H_

#include "config.h"

#define	FIRMWARE_NAME		"F103_NUCLEO_TEMPLATE"
#define	FIRMWARE_VERSION	"V00.00.01"

#define _printf(f_, ...)			PrintfUART((f_), ##__VA_ARGS__);
#define _printfEx(f_, ...)			PrintfUARTEx( __FILE__, __LINE__, (f_), ##__VA_ARGS__);

void	PrintfUART		(const char *str, ...);
void	PrintfUARTEx	(const char *file, int line, const char* str, ...);

#endif /* _COMMON_H_ */
