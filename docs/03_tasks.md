# 03 任务设计

## 3.1 任务原型

```c
void AppTask(void *arg)
{
    /* 初始化：只做一次 */
    for (;;) {
        /* 等待事件 / 延时 / 取队列 */
        /* 处理 */
        /* 输出 / 置状态 */
    }
}
```

每个任务只做一件事；跨域协作走 IPC，不要互相直接调用内部函数改状态。

## 3.2 周期任务模板

```c
const TickType_t period = pdMS_TO_TICKS(1);
TickType_t last = xTaskGetTickCount();
for (;;) {
    /* work */
    vTaskDelayUntil(&last, period);
}
```

电机控制、采样整理优先此模板。

## 3.3 事件驱动任务模板

```c
for (;;) {
    if (xQueueReceive(q, &msg, portMAX_DELAY) == pdPASS) {
        handle(&msg);
    }
}
```

串口命令、按键、网络收包适合此模型。

## 3.4 栈估算

1. 先给 1~2KB（Cortex-M 按字再换算），开启 `configCHECK_FOR_STACK_OVERFLOW`
2. 用 `uxTaskGetStackHighWaterMark` 看余量
3. 递归、`printf`、大局部数组是栈杀手——改静态/堆缓冲

## 3.5 任务拆分建议（电机控制场景）

| 任务 | 周期/触发 | 职责 |
|------|-----------|------|
| ControlTask | 1ms | 编码器→PID→PWM |
| CommTask | 队列/20ms | 命令与遥测 |
| SafetyTask | 10ms | 失联、过流、急停 |
| MonitorTask | 100ms | CPU/栈/心跳 |

## 3.6 反模式

- 在任务里长时间关中断做浮点大算
- 一个“上帝任务”处理所有业务
- 用全局变量互相抢写且不加同步
- 在任务里忙等硬件标志而不设超时
