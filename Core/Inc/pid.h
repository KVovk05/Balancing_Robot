#ifndef PID_H_
#define PID_H_
#include <stdint.h>

extern float Kp;
extern float Ki;
extern float Kd;

void PID_Reset(void);
int16_t PID_Compute(float current_pitch, float target_pitch, float dt);
void clamp(float* output, float min, float max);
#endif
