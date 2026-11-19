#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "app_types.h"

#include <string.h>

extern QueueHandle_t g_cmdQueue;
extern AppStats_t g_stats;

/*
 * 主机命令入口占位：
 * 实际可接 SRIO/PCIe/UART/仿真器脚本。
 * 演示：周期性注入 START_JOB。
 */
void CmdTask(void *arg)
{
    AppCmd_t cmd;
    TickType_t last = xTaskGetTickCount();

    (void)arg;

    /* 启动后设置处理节拍 */
    cmd.type = APP_CMD_SET_RATE;
    cmd.param0 = 1000U;
    cmd.param1 = 0U;
    (void)xQueueSend(g_cmdQueue, &cmd, 0);

    for (;;) {
        cmd.type = APP_CMD_START_JOB;
        cmd.param0 = 0x0C001000U; /* 示例输入缓冲地址 */
        cmd.param1 = 0x0C002000U; /* 示例输出缓冲地址 */
        (void)xQueueSend(g_cmdQueue, &cmd, 0);

        /* 每 100ms 投递一次作业（演示） */
        vTaskDelayUntil(&last, pdMS_TO_TICKS(100));

        /* 可在此打印 g_stats（通过 CCS CIO 或自定义日志） */
        (void)g_stats;
    }
}
