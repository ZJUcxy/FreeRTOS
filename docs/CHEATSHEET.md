# FreeRTOS 速查表

## 任务

```c
xTaskCreate(fn, "name", stackWords, arg, prio, &handle);
vTaskDelay(pdMS_TO_TICKS(10));
vTaskDelayUntil(&last, pdMS_TO_TICKS(1));
```

## 队列

```c
q = xQueueCreate(len, sizeof(msg));
xQueueSend(q, &msg, ticks);
xQueueOverwrite(q, &msg);          /* 长度须为 1 */
xQueueReceive(q, &msg, ticks);
xQueueSendFromISR(q, &msg, &hpw);
```

## 通知

```c
xTaskNotifyGive(handle);
ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
vTaskNotifyGiveFromISR(handle, &hpw);
portYIELD_FROM_ISR(hpw);
```

## 互斥

```c
mtx = xSemaphoreCreateMutex();
xSemaphoreTake(mtx, ticks);
xSemaphoreGive(mtx);
```

## 调试

```c
uxTaskGetStackHighWaterMark(NULL);
configASSERT(x);
```

## STM32H7 优先级口诀

能调用 FreeRTOS API 的中断，优先级数值必须 **≥** `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY`。

## C6678 口诀

一核一调度器；跨核只传描述符 + 门铃；共享缓冲维护 Cache。
