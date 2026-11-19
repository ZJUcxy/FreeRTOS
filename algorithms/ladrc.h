/**
 * @file ladrc.h
 * @brief 线性自抗扰控制（LADRC）——二阶对象常用形式
 *
 * LESO 估计 [z1,z2,z3] ≈ [y, dy, 总扰动]
 * LSEF：u0 = kp*(r - z1) - kd*z2
 * 扰动补偿：u = (u0 - z3) / b0
 *
 * 调参：先定 b0，再定 wc，最后 wo ≈ (3~5)*wc
 */
#ifndef LADRC_H
#define LADRC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float wo;
    float wc;
    float b0;

    float z1;
    float z2;
    float z3;
    float last_u;

    float u_min;
    float u_max;

    float beta1;
    float beta2;
    float beta3;
    float kp;
    float kd;
} Ladrc_t;

void Ladrc_Init(Ladrc_t *ctrl, float wo, float wc, float b0, float u_min, float u_max);
void Ladrc_SetBandwidth(Ladrc_t *ctrl, float wo, float wc);
void Ladrc_Reset(Ladrc_t *ctrl, float y0);
float Ladrc_Update(Ladrc_t *ctrl, float setpoint, float measurement, float dt);

#ifdef __cplusplus
}
#endif

#endif /* LADRC_H */
