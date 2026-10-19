#include <stdint.h>

float Kp = 0;
float Ki = 0;
float Kd = 0;
float target_pitch;
void clamp(float* output, float min, float max){
	if(*output > max){
		*output = max;
	}
	if(*output < min){
		*output = min;
	}
}


int16_t PID_Compute(float current_pitch, float target_pitch, float dt){

	static float error_previous = 0;
	static float I = 0;
	float error = target_pitch - current_pitch;
	float P = Kp * error;
	I = I + (Ki * error * dt);
	float D = Kd * (error - error_previous) / dt;
	error_previous = error;
	clamp(&I, -1500, 1500);
	float output = P + I + D;
	clamp(&output, -4199, 4199);

	return (int16_t)output;

}
