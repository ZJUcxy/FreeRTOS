#include "control_task.h"
#include "app_config.h"
#include "app_telemetry.h"
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "cmsis_os2.h"

void ControlTask_Entry(void *argument)
{
    PID_t speed_pid;
    const float dt = (float)CTRL_PERIOD_MS / 1000.0f;
    uint32_t last_wake;
    float cmd = 0.0f;
    float meas;
    float out;
    AppTelemetry_t snap;

    (void)argument;

    PID_Init(&speed_pid, SPEED_KP, SPEED_KI, SPEED_KD,
             -MOTOR_MAX_ABS_OUTPUT, MOTOR_MAX_ABS_OUTPUT);
    Motor_Init();
    Encoder_Init();

    last_wake = osKernelGetTickCount();

    for (;;) {
        if (osMessageQueueGet(g_targetSpeedQueue, &cmd, NULL, 0U) == osOK) {
            AppTelemetry_SetTarget(cmd);
        }

        AppTelemetry_Snapshot(&snap);

        if (snap.fault_flags != 0U) {
            Motor_Stop();
            PID_Reset(&speed_pid);
            AppTelemetry_UpdateMeas(Encoder_GetSpeedRPM(dt), 0.0f);
        } else {
            meas = Encoder_GetSpeedRPM(dt);
            out = PID_Update(&speed_pid, snap.target_rpm, meas, dt);
            Motor_SetOutput(out);
            AppTelemetry_UpdateMeas(meas, out);
        }

        last_wake += CTRL_PERIOD_MS;
        osDelayUntil(last_wake);
    }
}
