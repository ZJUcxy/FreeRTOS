#include "motor.h"
#include "app_config.h"

/* CubeMX 生成：按实际工程包含路径调整 */
#include "tim.h"
#include "gpio.h"

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    Motor_Stop();
}

void Motor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);
}

void Motor_SetOutput(float norm)
{
    uint32_t cmp;

    if (norm > 1.0f) {
        norm = 1.0f;
    } else if (norm < -1.0f) {
        norm = -1.0f;
    }

    if (norm >= 0.0f) {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_RESET);
        norm = -norm;
    }

    cmp = (uint32_t)(norm * (float)PWM_MAX_DUTY);
    if (cmp > PWM_MAX_DUTY) {
        cmp = PWM_MAX_DUTY;
    }
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, cmp);
}
