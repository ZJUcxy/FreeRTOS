# 07 移植要点

## 7.1 Port 层职责

`portable/` 需要提供：
- 上下文保存/恢复（任务切换）
- SysTick / 定时器 Tick
- 临界区开关中断原语
- 栈初始化（异常帧/寄存器初值）

Cortex-M 官方 port 成熟；C66x 需自定义或参考社区 port。

## 7.2 FreeRTOSConfig.h 最小清单

```c
#define configCPU_CLOCK_HZ              SystemCoreClock
#define configTICK_RATE_HZ              1000
#define configMAX_PRIORITIES            8
#define configMINIMAL_STACK_SIZE        128
#define configTOTAL_HEAP_SIZE           (64*1024)
#define configUSE_PREEMPTION            1
#define configUSE_MUTEXES               1
#define configUSE_TIMERS                1
#define configUSE_TASK_NOTIFICATIONS    1
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configASSERT(x)                 if((x)==0){taskDISABLE_INTERRUPTS();for(;;);}
```

## 7.3 Tickless Idle（低功耗）

`configUSE_TICKLESS_IDLE = 1` 适合电池设备。  
电机闭环运行期通常保持 Tick 常开，避免唤醒抖动。

## 7.4 多核策略

| 平台 | 建议 |
|------|------|
| STM32H7 单核 M7 | 标准单核 FreeRTOS |
| STM32H7 双核 H745 | 每核独立 OS 或一边裸机，核间用 HSEM/OpenAMP |
| C6678 8 核 | **一核一 OS 实例**；共享内存 + 门铃/IPC 通知，不共享同一调度器 |

本指南 C6678 例程采用：**Core0 跑 FreeRTOS，其他核可跑独立循环或另一 OS 实例**。
