#include "encoder.h"
#include "app_config.h"
#include "tim.h"

static int32_t s_last_cnt;

void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    s_last_cnt = (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
}

float Encoder_GetSpeedRPM(float dt_s)
{
    int32_t cnt;
    int32_t diff;
    float rev;
    float rps;

    if (dt_s <= 0.0f) {
        return 0.0f;
    }

    cnt = (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
    diff = cnt - s_last_cnt;
    s_last_cnt = cnt;

    /* 16-bit 计数器回绕处理；若 ARR 更大请相应调整 */
    if (diff > 32767) {
        diff -= 65536;
    } else if (diff < -32768) {
        diff += 65536;
    }

    rev = (float)diff / (ENCODER_CPR * GEAR_RATIO);
    rps = rev / dt_s;
    return rps * 60.0f;
}
