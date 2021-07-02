/**************************************************************************
 * @file    config.h
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Include *******************************/

/********************************* Const *********************************/
// MODES
//#define MODE_DEBUG
#define MODE_TEST
#define TBD

// CONFIGURATIONS
#define CONFIG_USE_RTOS
#define CONFIG_USE_UART_DEBUG
#define CONFIG_USE_SYSTICK_TIMER

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/

/************************* Function Definition ***************************/

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H__ */
