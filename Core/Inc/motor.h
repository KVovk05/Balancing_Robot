#pragma once
#include <stdint.h>


#define MOTOR_MAX_PWM 4199

void Motors_Init(void);
void Motors_Set_Speed(int16_t left_speed, int16_t right_speed);
void Motors_Brake(void);
