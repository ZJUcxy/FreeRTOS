#include "pid_incremental.h"
#include "pid_common.h"

void PidInc_Init(PidIncremental_t *pid,
                 float kp, float ki, float kd,
                 float out_min, float out_max)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->du_min = out_min - out_max; /* 默认允许较大单步变化 */
    pid->du_max = out_max - out_min;
    PidInc_Reset(pid);
}

void PidInc_SetDeltaLimit(PidIncremental_t *pid, float du_min, float du_max)
{
    pid->du_min = du_min;
    pid->du_max = du_max;
}

void PidInc_Reset(PidIncremental_t *pid)
{
    pid->err_k1 = 0.0f;
    pid->err_k2 = 0.0f;
    pid->u = 0.0f;
}

void PidInc_SetOutput(PidIncremental_t *pid, float u)
{
    pid->u = pid_clampf(u, pid->out_min, pid->out_max);
}

float PidInc_UpdateDelta(PidIncremental_t *pid, float setpoint, float measurement, float dt)
{
    float err;
    float du;

    if ((pid == 0) || (dt <= 0.0f)) {
        return 0.0f;
    }

    err = setpoint - measurement;

    /* Δu = Kp*Δe + Ki*e*dt + Kd*Δ²e/dt */
    du = pid->kp * (err - pid->err_k1)
       + pid->ki * err * dt
       + pid->kd * ((err - 2.0f * pid->err_k1 + pid->err_k2) / dt);

    du = pid_clampf(du, pid->du_min, pid->du_max);

    pid->u = pid_clampf(pid->u + du, pid->out_min, pid->out_max);

    pid->err_k2 = pid->err_k1;
    pid->err_k1 = err;

    return du;
}

float PidInc_Update(PidIncremental_t *pid, float setpoint, float measurement, float dt)
{
    (void)PidInc_UpdateDelta(pid, setpoint, measurement, dt);
    return pid->u;
}
