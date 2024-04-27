cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32G031xx 
	STM32_THREAD_SAFE_STRATEGY=4
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Inc
    ../../Drivers/STM32G0xx_HAL_Driver/Inc
    ../../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
    ../../Middlewares/Third_Party/FreeRTOS/Source/include
    ../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0
    ../../Drivers/CMSIS/Device/ST/STM32G0xx/Include
    ../../Drivers/CMSIS/Include
    ../../Drivers/BSP/STM32G0xx_Nucleo_32
)

target_sources(stm32cubemx INTERFACE
    ../../Src/main.c
    ../../Src/app_freertos.c
    ../../Src/stm32g0xx_it.c
    ../../Src/stm32g0xx_hal_msp.c
    ../../Src/stm32g0xx_hal_timebase_tim.c
    ../../Drivers/BSP/STM32G0xx_Nucleo_32/stm32g0xx_nucleo_32.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_dma.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c
    ../../Src/system_stm32g0xx.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/list.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
    ../../Src/sysmem.c
    ../../Src/syscalls.c
    ../../startup_stm32g031xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


