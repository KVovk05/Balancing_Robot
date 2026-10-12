#pragma once
#include <stdint.h>
#include "i2c.h"

#define IMU_ADDRESS 0xD4
#define WHO_AM_I 	0x0F
#define CTRL1_XL 	0x10
#define CTRL2_G 	0x11
#define OUTX_L_G    0x22

typedef struct {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;

}LSM6DS3_Data_t;
typedef struct  {
    float x;
    float y;
    float z;
}Vector3D;

int Imu_Init(I2C_HandleTypeDef* i2c_data_bus);
void Imu_Read(I2C_HandleTypeDef* i2c_data_bus,LSM6DS3_Data_t* imu_data);
