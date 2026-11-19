/**
 * @file pid_incremental.h
 * @brief 增量式 PID
 *
 * Δu(k) = Kp*(e(k)-e(k-1)) + Ki*e(k)*dt + Kd*(e(k)-2e(k-1)+e(k-2))/dt
 * 本模块同时维护累计输出 u，便于直接驱动执行器。
 */
#ifndef PID_INCREMENTAL_H
#define PID_INCREMENTAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float kp;
    float ki;
    float kd;
    float err_k1;   /* e(k-1) */
    float err_k2;   /* e(k-2) */
    float u;        /* 累计输出 */
    float du_min;
    float du_max;
    float out_min;
    float out_max;
} PidIncremental_t;

void PidInc_Init(PidIncremental_t *pid,
                 float kp, float ki, float kd,
                 float out_min, float out_max);

void PidInc_SetDeltaLimit(PidIncremental_t *pid, float du_min, float du_max);
void PidInc_Reset(PidIncremental_t *pid);
void PidInc_SetOutput(PidIncremental_t *pid, float u);

/** 返回累计输出 u(k) = u(k-1) + Δu(k) */
float PidInc_Update(PidIncremental_t *pid, float setpoint, float measurement, float dt);

/** 仅返回本次增量 Δu(k)（同时更新内部累计 u） */
float PidInc_UpdateDelta(PidIncremental_t *pid, float setpoint, float measurement, float dt);

#ifdef __cplusplus
}
#endif

#endif /* PID_INCREMENTAL_H */
