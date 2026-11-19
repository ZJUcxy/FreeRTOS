/**
 * @file main_sketch.c
 * @brief main 流程示意（勿直接替换 CubeMX main.c，按段落合并）
 */
#include "main.h"
#include "cmsis_os.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_USART1_UART_Init(void);
void MX_FREERTOS_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();

    osKernelInitialize();
    MX_FREERTOS_Init();
    osKernelStart();

    for (;;) {
        /* 不会执行到这里 */
    }
}
