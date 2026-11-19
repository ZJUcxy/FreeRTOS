/**
 * @file pid_positional.h
 * @brief 绝对式（位置式）PID
 *
 * u(k) = Kp*e(k) + Ki*Σe + Kd*(e(k)-e(k-1))/dt
 */
#ifndef PID_POSITIONAL_H
#define PID_POSITIONAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float kp;
    float ki;
    float kd;
    float integ;
    float prev_err;
    float out_min;
    float out_max;
    float integ_min;
    float integ_max;
} PidPositional_t;

void PidPos_Init(PidPositional_t *pid,
                 float kp, float ki, float kd,
                 float out_min, float out_max);

void PidPos_SetIntegralLimit(PidPositional_t *pid, float integ_min, float integ_max);
void PidPos_Reset(PidPositional_t *pid);
float PidPos_Update(PidPositional_t *pid, float setpoint, float measurement, float dt);

#ifdef __cplusplus
}
#endif

#endif /* PID_POSITIONAL_H */
