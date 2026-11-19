#ifndef PID_COMMON_H
#define PID_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

static inline float pid_clampf(float x, float lo, float hi)
{
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

#ifdef __cplusplus
}
#endif

#endif /* PID_COMMON_H */
