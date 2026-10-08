#include "motor.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include <stdlib.h>

extern  TIM_HandleTypeDef htim1;

void Motors_Init(void){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
}
void Clamp_Motors(int16_t* left_speed, int16_t* right_speed){
	if(*left_speed > MOTOR_MAX_PWM){
		*left_speed = MOTOR_MAX_PWM;
	}
	if(*right_speed > MOTOR_MAX_PWM){
		*right_speed = MOTOR_MAX_PWM;
	}
	if(*left_speed < -MOTOR_MAX_PWM){
			*left_speed = -MOTOR_MAX_PWM;
	}
	if(*right_speed < -MOTOR_MAX_PWM){
		*right_speed = -MOTOR_MAX_PWM;
	}


}
void Motors_Set_Speed(int16_t left_speed, int16_t right_speed){
	Clamp_Motors(&left_speed, &right_speed);

	if(left_speed < 0){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
	}
	if(right_speed < 0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	}

	if(left_speed > 0){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
	}
	if(right_speed > 0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
	}

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,abs(left_speed));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,abs(right_speed));
}
void Motors_Brake(void){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

}
