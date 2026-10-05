#include "motor.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"

extern  TIM_HandleTypedef htim1;

void Motors_Init(void){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}
void Clamp_Motors(int16_t* left_speed, int16_t* right_speed){

}
void Motors_Set_Speed(int16_t left_speed, int16_t right_speed){
	clamp_motors(&left_speed, &right_speed);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);



//abs!
	_HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,left_speed);
	_HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,right_speed);
}
void Motors_Brake(void){
	_HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	_HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

}
