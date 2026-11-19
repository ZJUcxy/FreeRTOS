# 06 中断与 ISR

## 6.1 黄金法则

1. ISR 尽量短：清标志 → 取数据 → 通知任务
2. 只用 `xxxFromISR` API
3. 若 `xxxFromISR` 返回需要切换，调用 `portYIELD_FROM_ISR`
4. 不要在 ISR 里做 PID、浮点大算、printf、阻塞

## 6.2 Cortex-M（STM32H7）优先级

- 设置 `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY`
- **允许调用 FreeRTOS API 的中断**，其逻辑优先级数值必须 ≥ 该阈值（数值越大优先级越低）
- 更高优先级中断可抢占内核，但不得调用 FreeRTOS API

CubeMX 中注意 NVIC 优先级分组与 FreeRTOS 配置一致。

## 6.3 延迟处理模式（推荐）

```text
硬件中断 → FromISR 通知 → Worker Task 处理协议/控制
```

编码器高速计数用定时器硬件；速度解算与 PID 放在 1ms 任务。

## 6.4 C66x 中断注意

- 通过 INTC/CIC 路由事件到 CPU 中断
- Tick 一般绑 GPTimer / TSCL 方案需自行 port
- ISR 与任务共享数据时注意 cache 一致性（写回/无效）
