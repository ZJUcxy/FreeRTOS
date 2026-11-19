/**
 * @file FreeRTOSConfig_ref.h
 * @brief STM32H7 参考配置片段（请合并进 CubeMX 生成的 FreeRTOSConfig.h）
 */
#ifndef FREERTOS_CONFIG_REF_H
#define FREERTOS_CONFIG_REF_H

#include <stdint.h>

extern uint32_t SystemCoreClock;

#define configCPU_CLOCK_HZ                       (SystemCoreClock)
#define configTICK_RATE_HZ                       ((TickType_t)1000)
#define configMAX_PRIORITIES                     (8)
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
#define configTOTAL_HEAP_SIZE                    ((size_t)(48 * 1024))
#define configUSE_PREEMPTION                     1
#define configUSE_TIME_SLICING                   1
#define configUSE_MUTEXES                        1
#define configUSE_COUNTING_SEMAPHORES            1
#define configUSE_TASK_NOTIFICATIONS             1
#define configUSE_TIMERS                         1
#define configTIMER_TASK_PRIORITY                (2)
#define configTIMER_QUEUE_LENGTH                 10
#define configTIMER_TASK_STACK_DEPTH             256
#define configCHECK_FOR_STACK_OVERFLOW           2
#define configUSE_MALLOC_FAILED_HOOK             1
#define configSUPPORT_STATIC_ALLOCATION          1
#define configSUPPORT_DYNAMIC_ALLOCATION         1

/* CMSIS-RTOS2 / Cortex-M 常用 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for(;;); }

#endif
