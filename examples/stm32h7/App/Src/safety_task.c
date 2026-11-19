#include "safety_task.h"
#include "app_config.h"
#include "app_telemetry.h"
#include "cmsis_os2.h"

void SafetyTask_Entry(void *argument)
{
    AppTelemetry_t snap;
    uint32_t now;
    uint32_t age;

    (void)argument;

    for (;;) {
        AppTelemetry_Snapshot(&snap);
        now = osKernelGetTickCount();
        age = now - snap.last_cmd_tick;

        if (age > COMM_TIMEOUT_MS) {
            AppTelemetry_SetFault(FAULT_COMM_LOSS);
            AppTelemetry_SetTarget(0.0f);
        } else {
            AppTelemetry_ClearFault(FAULT_COMM_LOSS);
        }

        if ((snap.meas_rpm > SPEED_ABS_LIMIT_RPM) ||
            (snap.meas_rpm < -SPEED_ABS_LIMIT_RPM)) {
            AppTelemetry_SetFault(FAULT_OVERSPEED);
            AppTelemetry_SetTarget(0.0f);
        } else {
            AppTelemetry_ClearFault(FAULT_OVERSPEED);
        }

        osDelay(SAFETY_PERIOD_MS);
    }
}
