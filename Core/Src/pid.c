#include "pid.h"

float Kp = 80.0f;
float Ki = 0.0f;
float Kd = 0.0f;

static float pid_i = 0.0f;
static float pid_error_prev = 0.0f;

void PID_Reset(void) {
    pid_i = 0.0f;
    pid_error_prev = 0.0f;
}

void clamp(float* output, float min, float max) {
    if (*output > max) {
        *output = max;
    }
    if (*output < min) {
        *output = min;
    }
}

int16_t PID_Compute(float current_pitch, float target_pitch, float dt) {
    float error = target_pitch - current_pitch;


    float P = Kp * error;
    pid_i += (Ki * error * dt);
    clamp(&pid_i, -1500.0f, 1500.0f);
    float D = Kd * (error - pid_error_prev) / dt;
    pid_error_prev = error;

    float output = P + pid_i + D;
    clamp(&output, -4199.0f, 4199.0f);

    return (int16_t)output;
}
