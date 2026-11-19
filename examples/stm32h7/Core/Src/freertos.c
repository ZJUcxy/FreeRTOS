/**
 * @file freertos.c
 * @brief 任务与 IPC 创建示意（可粘贴到 CubeMX 生成的 freertos.c）
 */
#include "cmsis_os.h"
#include "app_telemetry.h"
#include "control_task.h"
#include "comm_task.h"
#include "safety_task.h"

osThreadId_t controlTaskHandle;
osThreadId_t commTaskHandle;
osThreadId_t safetyTaskHandle;

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t control_attr = {
        .name = "control",
        .stack_size = 512 * 4,
        .priority = (osPriority_t)osPriorityHigh,
    };
    const osThreadAttr_t comm_attr = {
        .name = "comm",
        .stack_size = 512 * 4,
        .priority = (osPriority_t)osPriorityNormal,
    };
    const osThreadAttr_t safety_attr = {
        .name = "safety",
        .stack_size = 256 * 4,
        .priority = (osPriority_t)osPriorityAboveNormal,
    };
    const osMessageQueueAttr_t q_attr = {
        .name = "targetSpd"
    };

    AppTelemetry_Init();
    g_targetSpeedQueue = osMessageQueueNew(1U, sizeof(float), &q_attr);

    controlTaskHandle = osThreadNew(ControlTask_Entry, NULL, &control_attr);
    safetyTaskHandle  = osThreadNew(SafetyTask_Entry, NULL, &safety_attr);
    commTaskHandle    = osThreadNew(CommTask_Entry, NULL, &comm_attr);
}
