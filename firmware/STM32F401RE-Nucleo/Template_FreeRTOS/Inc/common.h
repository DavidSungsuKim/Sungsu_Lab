/**************************************************************************
 * @file    common.h
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

#define	ASSERT(x)					if(!x) \
										while(1);

#define _printf(f_, ...)			PrintfUART((f_), ##__VA_ARGS__);
#define _printfEx(f_, ...)			PrintfUARTEx( __FILE__, __LINE__, (f_), ##__VA_ARGS__);

void	PrintfUART		(const char *str, ...);
void	PrintfUARTEx	(const char *file, int line, const char* str, ...);

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__ */
