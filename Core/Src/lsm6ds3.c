#include "lsm6ds3.h"
#include <stdint.h>
#include <math.h>

#ifndef CTRL3_C
#define CTRL3_C 0x12
#endif

static float gyro_bias_x = 0.0f;
static float gyro_bias_y = 0.0f;
static float gyro_bias_z = 0.0f;

int Imu_Init(I2C_HandleTypeDef* i2c_data_bus) {
    uint8_t check;
    uint8_t ctrl1_xl = 0x60; // 416 Гц, ±2g
    uint8_t ctrl2_g  = 0x64; // 416 Гц, ±500 dps
    uint8_t ctrl3_c  = 0x44; // BDU = 1, IF_INC = 1

    HAL_I2C_Mem_Read(i2c_data_bus, IMU_ADDRESS, WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &check, 1, 100);
    if (check == 0x69) {
        HAL_I2C_Mem_Write(i2c_data_bus, IMU_ADDRESS, CTRL3_C,  I2C_MEMADD_SIZE_8BIT, &ctrl3_c,  1, 100);
        HAL_I2C_Mem_Write(i2c_data_bus, IMU_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &ctrl1_xl, 1, 100);
        HAL_I2C_Mem_Write(i2c_data_bus, IMU_ADDRESS, CTRL2_G,  I2C_MEMADD_SIZE_8BIT, &ctrl2_g,  1, 100);
        return 0;
    }
    return -1;
}

void Imu_Read(I2C_HandleTypeDef* i2c_data_bus, LSM6DS3_Data_t* imu_data) {
    uint8_t buffer[12];
    HAL_I2C_Mem_Read(i2c_data_bus, IMU_ADDRESS, OUTX_L_G, I2C_MEMADD_SIZE_8BIT, buffer, 12, 100);
    imu_data->gx = (int16_t)(buffer[1]  << 8 | buffer[0]);
    imu_data->gy = (int16_t)(buffer[3]  << 8 | buffer[2]);
    imu_data->gz = (int16_t)(buffer[5]  << 8 | buffer[4]);
    imu_data->ax = (int16_t)(buffer[7]  << 8 | buffer[6]);
    imu_data->ay = (int16_t)(buffer[9]  << 8 | buffer[8]);
    imu_data->az = (int16_t)(buffer[11] << 8 | buffer[10]);
}

void Imu_Process(LSM6DS3_Data_t* raw_data, Vector3D* accel_g, Vector3D* gyro_dps) {
    accel_g->x = raw_data->ax * 0.000061f;   // ±2g
    accel_g->y = raw_data->ay * 0.000061f;
    accel_g->z = raw_data->az * 0.000061f;

    gyro_dps->x = raw_data->gx * 0.0175f - gyro_bias_x;  // ±500 dps
    gyro_dps->y = raw_data->gy * 0.0175f - gyro_bias_y;
    gyro_dps->z = raw_data->gz * 0.0175f - gyro_bias_z;
}

// Викликати один раз після Imu_Init, поки робот лежить нерухомо
void Imu_CalibrateGyro(I2C_HandleTypeDef* i2c_data_bus, uint16_t samples) {
    LSM6DS3_Data_t raw;
    float sx = 0.0f, sy = 0.0f, sz = 0.0f;

    gyro_bias_x = gyro_bias_y = gyro_bias_z = 0.0f;
    HAL_Delay(200); // дати датчику стабілізуватися

    for (uint16_t i = 0; i < samples; i++) {
        Imu_Read(i2c_data_bus, &raw);
        sx += raw.gx * 0.0175f;
        sy += raw.gy * 0.0175f;
        sz += raw.gz * 0.0175f;
        HAL_Delay(3);
    }
    gyro_bias_x = sx / samples;
    gyro_bias_y = sy / samples;
    gyro_bias_z = sz / samples;
}

float Imu_GetPitch(Vector3D* accel, Vector3D* gyro, float dt) {
    static float pitch_angle = 0.0f;
    static uint8_t initialized = 0;
    const float alpha = 0.98f;

    float acc_angle = atan2f(accel->x, accel->z) * 57.29578f;

    if (!initialized) {
        pitch_angle = acc_angle;
        initialized = 1;
    }


    pitch_angle = alpha * (pitch_angle - gyro->y * dt) + (1.0f - alpha) * acc_angle;

    return pitch_angle;
}
