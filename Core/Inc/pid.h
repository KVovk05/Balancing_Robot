#pragma once
#include <stdint.h>

int16_t PID_Compute(float current_pitch, float target_pitch, float dt);
void clamp(float* output, float min, float max);
