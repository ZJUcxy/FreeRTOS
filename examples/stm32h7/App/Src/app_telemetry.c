#include "app_telemetry.h"

AppTelemetry_t g_telem;
osMutexId_t g_telemMutex;
osMessageQueueId_t g_targetSpeedQueue;

void AppTelemetry_Init(void)
{
    const osMutexAttr_t attr = {
        .name = "telemMtx"
    };

    g_telemMutex = osMutexNew(&attr);
    g_telem.target_rpm = 0.0f;
    g_telem.meas_rpm = 0.0f;
    g_telem.ctrl_out = 0.0f;
    g_telem.fault_flags = 0U;
    g_telem.last_cmd_tick = osKernelGetTickCount();
}

void AppTelemetry_SetTarget(float rpm)
{
    if (osMutexAcquire(g_telemMutex, 10U) == osOK) {
        g_telem.target_rpm = rpm;
        g_telem.last_cmd_tick = osKernelGetTickCount();
        osMutexRelease(g_telemMutex);
    }
}

void AppTelemetry_UpdateMeas(float meas, float out)
{
    if (osMutexAcquire(g_telemMutex, 0U) == osOK) {
        g_telem.meas_rpm = meas;
        g_telem.ctrl_out = out;
        osMutexRelease(g_telemMutex);
    }
}

void AppTelemetry_SetFault(uint32_t bits)
{
    if (osMutexAcquire(g_telemMutex, 10U) == osOK) {
        g_telem.fault_flags |= bits;
        osMutexRelease(g_telemMutex);
    }
}

void AppTelemetry_ClearFault(uint32_t bits)
{
    if (osMutexAcquire(g_telemMutex, 10U) == osOK) {
        g_telem.fault_flags &= ~bits;
        osMutexRelease(g_telemMutex);
    }
}

void AppTelemetry_Snapshot(AppTelemetry_t *out)
{
    if (out == NULL) {
        return;
    }
    if (osMutexAcquire(g_telemMutex, 10U) == osOK) {
        *out = g_telem;
        osMutexRelease(g_telemMutex);
    }
}
