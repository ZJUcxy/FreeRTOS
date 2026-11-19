#include "pid_positional.h"
#include "pid_common.h"

void PidPos_Init(PidPositional_t *pid,
                 float kp, float ki, float kd,
                 float out_min, float out_max)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->integ_min = out_min;
    pid->integ_max = out_max;
    PidPos_Reset(pid);
}

void PidPos_SetIntegralLimit(PidPositional_t *pid, float integ_min, float integ_max)
{
    pid->integ_min = integ_min;
    pid->integ_max = integ_max;
}

void PidPos_Reset(PidPositional_t *pid)
{
    pid->integ = 0.0f;
    pid->prev_err = 0.0f;
}

float PidPos_Update(PidPositional_t *pid, float setpoint, float measurement, float dt)
{
    float err;
    float deriv;
    float p_term;
    float i_term;
    float d_term;
    float out;

    if ((pid == 0) || (dt <= 0.0f)) {
        return 0.0f;
    }

    err = setpoint - measurement;
    p_term = pid->kp * err;

    pid->integ += err * dt;
    pid->integ = pid_clampf(pid->integ, pid->integ_min, pid->integ_max);
    i_term = pid->ki * pid->integ;

    deriv = (err - pid->prev_err) / dt;
    d_term = pid->kd * deriv;

    out = p_term + i_term + d_term;

    /* 条件积分抗饱和：输出饱和且误差同向时回退本次积分 */
    if ((out > pid->out_max) && (err > 0.0f)) {
        pid->integ -= err * dt;
        i_term = pid->ki * pid->integ;
        out = p_term + i_term + d_term;
    } else if ((out < pid->out_min) && (err < 0.0f)) {
        pid->integ -= err * dt;
        i_term = pid->ki * pid->integ;
        out = p_term + i_term + d_term;
    }

    out = pid_clampf(out, pid->out_min, pid->out_max);
    pid->prev_err = err;
    return out;
}
