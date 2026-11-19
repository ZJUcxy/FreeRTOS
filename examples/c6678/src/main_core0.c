#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "shared_ipc.h"
#include "app_types.h"
#include <string.h>

QueueHandle_t g_cmdQueue;
AppStats_t g_stats;

extern void WorkerTask(void *arg);
extern void IpcTask(void *arg);
extern void CmdTask(void *arg);

/* port 层提供：初始化 Tick 定时器与中断 */
extern void vPortSetupTimerInterrupt(void);
extern void prvSetupHardware(void);

static void create_app_objects(void)
{
    memset(&g_stats, 0, sizeof(g_stats));
    Ipc_InitMailbox();

    g_cmdQueue = xQueueCreate(8, sizeof(AppCmd_t));
    configASSERT(g_cmdQueue != NULL);

    configASSERT(xTaskCreate(WorkerTask, "worker", 512, NULL, 4, NULL) == pdPASS);
    configASSERT(xTaskCreate(IpcTask, "ipc", 512, NULL, 5, NULL) == pdPASS);
    configASSERT(xTaskCreate(CmdTask, "cmd", 512, NULL, 3, NULL) == pdPASS);
}

int main(void)
{
    prvSetupHardware();
    create_app_objects();
    vTaskStartScheduler();

    for (;;) {
        /* 调度器启动失败才会到这里 */
    }
}

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}
