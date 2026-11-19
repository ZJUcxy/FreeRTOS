#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

void Encoder_Init(void);
float Encoder_GetSpeedRPM(float dt_s);

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_H */
