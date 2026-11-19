/**
 * @file example_usage.c
 * @brief 三种算法在 1ms 控制环中的调用示例（不链接，仅作参考）
 */
#include "pid_positional.h"
#include "pid_incremental.h"
#include "ladrc.h"

void demo_control_step(float target, float meas, float dt)
{
    static PidPositional_t pid_abs;
    static PidIncremental_t pid_inc;
    static Ladrc_t ladrc;
    static int inited = 0;
    float u_abs;
    float u_inc;
    float u_adrc;

    if (!inited) {
        PidPos_Init(&pid_abs, 0.8f, 2.0f, 0.01f, -1.0f, 1.0f);
        PidInc_Init(&pid_inc, 0.8f, 2.0f, 0.01f, -1.0f, 1.0f);
        PidInc_SetDeltaLimit(&pid_inc, -0.05f, 0.05f);
        Ladrc_Init(&ladrc, 100.0f /*wo*/, 30.0f /*wc*/, 1.0f /*b0*/, -1.0f, 1.0f);
        inited = 1;
    }

    u_abs = PidPos_Update(&pid_abs, target, meas, dt);
    u_inc = PidInc_Update(&pid_inc, target, meas, dt);
    u_adrc = Ladrc_Update(&ladrc, target, meas, dt);

    (void)u_abs;
    (void)u_inc;
    (void)u_adrc;
}
