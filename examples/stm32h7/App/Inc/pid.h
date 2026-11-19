#ifndef PID_H
#define PID_H

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
} PID_t;

void PID_Init(PID_t *pid, float kp, float ki, float kd, float out_min, float out_max);
void PID_Reset(PID_t *pid);
float PID_Update(PID_t *pid, float target, float measure, float dt);

#ifdef __cplusplus
}
#endif

#endif /* PID_H */
