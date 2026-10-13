#include "lsm6ds3.h"
#include <stdint.h>



int Imu_Init(I2C_HandleTypeDef* i2c_data_bus) {
    uint8_t check;
    uint8_t ctrl_val = 0x40;
    HAL_I2C_Mem_Read(i2c_data_bus, IMU_ADDRESS,WHO_AM_I,I2C_MEMADD_SIZE_8BIT,&check,1,100);
    if(check == 0x69){
    	HAL_I2C_Mem_Write(i2c_data_bus, IMU_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &ctrl_val, 1, 100);
    	HAL_I2C_Mem_Write(i2c_data_bus, IMU_ADDRESS, CTRL2_G , I2C_MEMADD_SIZE_8BIT, &ctrl_val, 1, 100);

    	return 0;
    }
    else{
    	return -1;
    }

}


void Imu_Read(I2C_HandleTypeDef* i2c_data_bus, LSM6DS3_Data_t* imu_data) {
    uint8_t buffer[12];
    HAL_I2C_Mem_Read(i2c_data_bus, IMU_ADDRESS,OUTX_L_G,I2C_MEMADD_SIZE_8BIT,buffer,12,100);
    imu_data->gx = (int16_t)(buffer[1] << 8 | buffer[0]);
    imu_data->gy = (int16_t)(buffer[3] << 8 | buffer[2]);
    imu_data->gz = (int16_t)(buffer[5] << 8 | buffer[4]);
    imu_data->ax = (int16_t)(buffer[7] << 8 | buffer[6]);
    imu_data->ay = (int16_t)(buffer[9] << 8 | buffer[8]);
    imu_data->az = (int16_t)(buffer[11] << 8 | buffer[10]);

}
