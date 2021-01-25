
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Configs */
#define CONFIG_USE_UART
//#define CONFIG_USE_UART_DEBUG
#define CONFIG_USE_SYSTICK_TIMER
#define CONFIG_USE_TICK_TIMER

/* Tick Timer specific config */
#ifdef CONFIG_USE_TICK_TIMER
//#define CONFIG_TICK_TIMER_IRQ
#endif

#endif /* _CONFIG_H_ */
