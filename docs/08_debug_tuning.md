# 08 调试与调优

## 8.1 必开检查

```c
#define configASSERT(x) ...
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_MALLOC_FAILED_HOOK    1
```

实现 `vApplicationStackOverflowHook` / `vApplicationMallocFailedHook` 里拉低安全输出并死循环或复位。

## 8.2 常见故障图谱

| 现象 | 可能原因 |
|------|----------|
| 跑飞到 HardFault | 栈溢出、ISR 调了非 FromISR API、优先级配错 |
| 任务饿死 | 高优先级任务忙循环未阻塞 |
| 队列一直空 | 发送方失败未检查、ISR 未 yield |
| PID 抖动变大 | 周期任务用了 `vTaskDelay` 而非 `DelayUntil` |
| H7 DMA 数据错乱 | Cache 未维护 / 缓冲地址不在合适 RAM |

## 8.3 运行时观测

- `uxTaskGetSystemState` / Tracealyzer / SystemView
- 自定义：每 100ms 打印 CPU%、栈水印、队列深度
- 控制任务加 GPIO 翻转测实际周期抖动

## 8.4 性能建议

1. 热路径避免堆分配与格式化字符串
2. 浮点：M7 开 FPU；C66x 注意 ABI 与上下文是否保存向量寄存器
3. 大数据用零拷贝 + 所有权转移
4. 先正确后优化：先保证 1ms 控制不丢拍
