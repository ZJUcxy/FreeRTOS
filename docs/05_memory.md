# 05 内存与堆

## 5.1 heap 方案

| 实现 | 特点 | 适用 |
|------|------|------|
| heap_1 | 只分配不释放 | 静态系统 |
| heap_2 | 可释放，易碎片 | 少用 |
| heap_4 | 合并相邻空闲块 | **通用首选** |
| heap_5 | 多内存区 | 外扩 SDRAM/多段 RAM |

STM32H7 常把 DMA 缓冲放在特定 RAM（D2/AXI），可用 heap_5 或手动静态放置。

## 5.2 静态创建（推荐量产）

```c
StaticTask_t tcb;
StackType_t stack[512];
TaskHandle_t h = xTaskCreateStatic(fn, "t", 512, NULL, prio, stack, &tcb);
```

优点：启动失败可预期、无堆碎片、便于认证。

## 5.3 常见坑

- `pvPortMalloc` 失败未检查 → 空指针崩溃
- 在 ISR 里 malloc/free
- 大数组放任务栈导致溢出
- H7 Cache：DMA 缓冲需非 cache 或做 Clean/Invalidate

## 5.4 FreeRTOSConfig 相关项

```c
#define configSUPPORT_DYNAMIC_ALLOCATION  1
#define configSUPPORT_STATIC_ALLOCATION   1
#define configTOTAL_HEAP_SIZE             ((size_t)(64*1024))
#define configAPPLICATION_ALLOCATED_HEAP  0
```
