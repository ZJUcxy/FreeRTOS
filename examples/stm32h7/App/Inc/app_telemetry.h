#ifndef APP_TELEMETRY_H
#define APP_TELEMETRY_H

#include <stdint.h>
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float target_rpm;
    float meas_rpm;
    float ctrl_out;
    uint32_t fault_flags;
    uint32_t last_cmd_tick;
} AppTelemetry_t;

extern AppTelemetry_t g_telem;
extern osMutexId_t g_telemMutex;
extern osMessageQueueId_t g_targetSpeedQueue;

void AppTelemetry_Init(void);
void AppTelemetry_SetTarget(float rpm);
void AppTelemetry_UpdateMeas(float meas, float out);
void AppTelemetry_SetFault(uint32_t bits);
void AppTelemetry_ClearFault(uint32_t bits);
void AppTelemetry_Snapshot(AppTelemetry_t *out);

#ifdef __cplusplus
}
#endif

#endif /* APP_TELEMETRY_H */
