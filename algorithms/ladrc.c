#include "ladrc.h"
#include "pid_common.h"

static void ladrc_update_gains(Ladrc_t *ctrl)
{
    float wo = ctrl->wo;
    float wc = ctrl->wc;

    /* (s+wo)^3 */
    ctrl->beta1 = 3.0f * wo;
    ctrl->beta2 = 3.0f * wo * wo;
    ctrl->beta3 = wo * wo * wo;

    /* (s+wc)^2 => kp=wc^2, kd=2wc */
    ctrl->kp = wc * wc;
    ctrl->kd = 2.0f * wc;
}

void Ladrc_Init(Ladrc_t *ctrl, float wo, float wc, float b0, float u_min, float u_max)
{
    ctrl->wo = wo;
    ctrl->wc = wc;
    ctrl->b0 = (b0 == 0.0f) ? 1.0f : b0;
    ctrl->u_min = u_min;
    ctrl->u_max = u_max;
    ladrc_update_gains(ctrl);
    Ladrc_Reset(ctrl, 0.0f);
}

void Ladrc_SetBandwidth(Ladrc_t *ctrl, float wo, float wc)
{
    ctrl->wo = wo;
    ctrl->wc = wc;
    ladrc_update_gains(ctrl);
}

void Ladrc_Reset(Ladrc_t *ctrl, float y0)
{
    ctrl->z1 = y0;
    ctrl->z2 = 0.0f;
    ctrl->z3 = 0.0f;
    ctrl->last_u = 0.0f;
}

float Ladrc_Update(Ladrc_t *ctrl, float setpoint, float measurement, float dt)
{
    float err_obs;
    float e;
    float u0;
    float u;

    if ((ctrl == 0) || (dt <= 0.0f)) {
        return 0.0f;
    }

    /* LESO，使用上一拍控制量 last_u */
    err_obs = measurement - ctrl->z1;
    ctrl->z1 += dt * (ctrl->z2 + ctrl->beta1 * err_obs);
    ctrl->z2 += dt * (ctrl->z3 + ctrl->b0 * ctrl->last_u + ctrl->beta2 * err_obs);
    ctrl->z3 += dt * (ctrl->beta3 * err_obs);

    /* LSEF + 扰动补偿 */
    e = setpoint - ctrl->z1;
    u0 = ctrl->kp * e - ctrl->kd * ctrl->z2;
    u = (u0 - ctrl->z3) / ctrl->b0;
    u = pid_clampf(u, ctrl->u_min, ctrl->u_max);

    ctrl->last_u = u;
    return u;
}
