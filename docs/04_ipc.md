# 04 任务间通信（IPC）

## 4.1 选型速查

| 需求 | 推荐对象 |
|------|----------|
| 传递命令/结构体 | Queue |
| 最新设定值覆盖 | 长度 1 的 Queue + `xQueueOverwrite` |
| 资源互斥 | Mutex（支持优先级继承） |
| 事件发生通知 | Binary Semaphore / Task Notification |
| 多条件组合 | EventGroup |
| 字节流 | StreamBuffer / MessageBuffer |
| ISR→任务 | `xxxFromISR` + Task Notification（最快） |

## 4.2 Queue 注意点

- 传递**值拷贝**，大块数据传指针并明确所有权
- 控制指令优先“最新有效”，用 overwrite 避免堆积过期命令
- ISR 中只能用 `xQueueSendFromISR`

## 4.3 Mutex vs Semaphore

- Mutex：保护共享资源，同一任务 acquire/release
- Binary Semaphore：同步“发生了某事”，常由 ISR give、任务 take
- Counting Semaphore：资源池/多实例事件计数

## 4.4 Task Notification（推荐）

轻量、快，适合 1:1 通知：

```c
/* ISR */
vTaskNotifyGiveFromISR(ctrlTaskHandle, &hpw);
portYIELD_FROM_ISR(hpw);

/* Task */
ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
```

## 4.5 电机控制典型数据流

```text
UART ISR / CommTask  --(Queue overwrite)-->  target_rpm
Encoder DMA/定时      --(本地采样)-------->  ControlTask
ControlTask           --(共享遥测+Mutex)-->  CommTask 上报
SafetyTask            --(EventGroup)------>  FAULT 停机
```
