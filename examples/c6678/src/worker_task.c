#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "app_types.h"
#include "shared_ipc.h"

extern QueueHandle_t g_cmdQueue;
extern AppStats_t g_stats;

void WorkerTask(void *arg)
{
    TickType_t last = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(1);
    AppCmd_t cmd;
    uint32_t rate_hz = 1000U;

    (void)arg;

    for (;;) {
        while (xQueueReceive(g_cmdQueue, &cmd, 0) == pdPASS) {
            switch (cmd.type) {
            case APP_CMD_SET_RATE:
                if (cmd.param0 > 0U) {
                    rate_hz = cmd.param0;
                }
                break;
            case APP_CMD_START_JOB:
                Ipc_SubmitProcess(cmd.param0, cmd.param1, 1024U);
                break;
            case APP_CMD_STOP:
                g_ipc_mailbox.cmd = IPC_CMD_NOP;
                g_ipc_mailbox.status = IPC_STATUS_IDLE;
                break;
            default:
                break;
            }
        }

        /* 占位：本地轻量处理；重负载应 offload 到 Core1 */
        g_stats.worker_ticks++;
        (void)rate_hz;

        vTaskDelayUntil(&last, period);
    }
}
