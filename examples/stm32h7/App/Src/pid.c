#include "pid.h"

static float clampf(float x, float lo, float hi)
{
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

void PID_Init(PID_t *pid, float kp, float ki, float kd, float out_min, float out_max)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->out_min = out_min;
    pid->out_max = out_max;
    PID_Reset(pid);
}

void PID_Reset(PID_t *pid)
{
    pid->integ = 0.0f;
    pid->prev_err = 0.0f;
}

float PID_Update(PID_t *pid, float target, float measure, float dt)
{
    float err = target - measure;
    float deriv;
    float out;

    if (dt <= 0.0f) {
        return 0.0f;
    }

    pid->integ += err * dt;
    deriv = (err - pid->prev_err) / dt;
    out = pid->kp * err + pid->ki * pid->integ + pid->kd * deriv;

    /* 简化抗饱和：输出已顶满且误差仍同向时回退积分 */
    if ((out > pid->out_max) && (err > 0.0f)) {
        pid->integ -= err * dt;
    } else if ((out < pid->out_min) && (err < 0.0f)) {
        pid->integ -= err * dt;
    }

    out = clampf(out, pid->out_min, pid->out_max);
    pid->prev_err = err;
    return out;
}
