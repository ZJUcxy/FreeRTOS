# 01 概述与选型

## 1.1 FreeRTOS 适合什么场景

- 需要确定性响应：控制环、采集、通信协议栈并存
- 多功能并行：UI/日志/网络不应阻塞控制
- MCU/DSP 资源有限，但希望比“超级循环 + 标志位”更可维护

不适合：硬实时纳秒级闭环（应放 ISR/硬件加速器，任务只做毫秒级协调）。

## 1.2 版本与发行形态

| 形态 | 说明 |
|------|------|
| FreeRTOS Kernel | 纯内核：任务、队列、定时器、事件 |
| FreeRTOS-Plus | TCP/CLI/FAT/Trace 等组件 |
| Amazon FreeRTOS / FreeRTOS LTS | 带云连接与长期支持分支 |

工程建议：内核固定到一个 LTS/稳定小版本，再按需引入 Plus 组件。

## 1.3 与裸机超级循环对比

| 维度 | 裸机 while(1) | FreeRTOS |
|------|---------------|----------|
| 并行表达 | 标志位/状态机交织 | 任务边界清晰 |
| 阻塞等待 | 忙等或手工轮询 | `vTaskDelay` / Queue 阻塞 |
| 优先级 | 手工拼凑 | 内核抢占调度 |
| 调试成本 | 初期低、后期高 | 初期略高、规模化后更稳 |

## 1.4 推荐工程分层

```text
Board/          时钟、引脚、外设初始化（CubeMX / CSL）
Drivers/        PWM、UART、ADC、编码器等驱动
Middleware/     FreeRTOS、协议栈、文件系统
App/            业务任务：控制、通信、安全
Config/         FreeRTOSConfig.h、板级参数
```

原则：**驱动不依赖任务；任务不直接操作寄存器。**

## 1.5 本指南目标产物

- 统一的任务/IPC 设计模板
- STM32H7：CubeHAL + CMSIS-RTOS v2 电机控制骨架
- C6678：单核 FreeRTOS 启动 + 核间通知示例
