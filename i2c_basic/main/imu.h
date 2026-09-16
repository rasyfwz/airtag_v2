#pragma once

#include "driver/i2c_master.h"

#define MPU9250_SENSOR_ADDR         0x68        /*!< Address of the MPU9250 sensor */
#define MPU9250_WHO_AM_I_REG_ADDR   0x75        /*!< Register address of the "who am I" register */


typedef struct {
    uint16_t x_accel;
    uint16_t y_accel;
    uint16_t z_accel;
} imu_data_t;

/**
 * @brief Read the WHO_AM_I register (expected value 0x71 on power-up)
 */
esp_err_t imu_who_am_i(i2c_master_dev_handle_t dev_handle, uint8_t *who_am_i);

/**
 * @brief Configure the IMU for low-power wake-on-motion operation
 */
void imu_init(i2c_master_dev_handle_t dev_handle);

/**
 * @brief Read and decode the latest accelerometer sample
 */
esp_err_t imu_read_accel(i2c_master_dev_handle_t dev_handle, imu_data_t *imu);

