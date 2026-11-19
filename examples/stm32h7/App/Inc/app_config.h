#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define CTRL_PERIOD_MS           1U
#define COMM_PERIOD_MS           20U
#define SAFETY_PERIOD_MS         10U

#define COMM_TIMEOUT_MS          500U

#define PWM_MAX_DUTY             999U
#define MOTOR_MAX_ABS_OUTPUT     0.95f

#define ENCODER_CPR              1024.0f
#define GEAR_RATIO               1.0f

#define SPEED_KP                 0.08f
#define SPEED_KI                 0.02f
#define SPEED_KD                 0.0f

#define FAULT_COMM_LOSS          (1U << 0)
#define FAULT_OVERSPEED          (1U << 1)

#define SPEED_ABS_LIMIT_RPM      6000.0f

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
