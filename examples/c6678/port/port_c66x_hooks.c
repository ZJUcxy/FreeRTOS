/**
 * @file port_c66x_hooks.c
 * @brief C66x FreeRTOS port 挂钩示意（非完整官方 port）
 *
 * 上板前必须实现：
 * 1) GPTimer 周期中断作为 Tick
 * 2) 上下文切换汇编（或编译器内嵌）
 * 3) 临界区开关 GIE
 * 4) pxPortInitialiseStack 按 C66x ABI 压栈
 */
#include <stdint.h>

void prvSetupHardware(void)
{
    /*
     * 1. 初始化 PLL / PSC
     * 2. 配置 L1/L2 cache
     * 3. 映射 INTC：Timer → CPU INT
     * 4. 映射门铃中断 → Ipc_OnDoorbellIsr
     */
}

void vPortSetupTimerInterrupt(void)
{
    /*
     * 配置 GPTimer：周期 = 1 / configTICK_RATE_HZ
     * ISR 内调用：
     *   if (xTaskIncrementTick()) { ... yield ... }
     */
}

void vPortEnterCritical(void)
{
    /* 保存并清除 CSR.GIE，支持嵌套计数 */
}

void vPortExitCritical(void)
{
    /* 嵌套减一后恢复 GIE */
}

/*
 * 下列符号通常在 portASM.asm / port.c 中实现：
 * - pxPortInitialiseStack
 * - vPortYield
 * - xPortStartScheduler
 */
