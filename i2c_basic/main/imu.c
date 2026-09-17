#include "imu.h"
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#define ACCEL_XOUT_H                0x3B
#define GYRO_XOUT_H                 0x43
#define INT_ENABLE_REG              0x38
#define LP_ACCEL_ODR_REG            0x1E
#define PWR_MGMT_2_REG              0x6C
#define ACCEL_CONFIG2_REG           0x1D
#define MPU9250_PWR_MGMT_1_REG_ADDR 0x6B
#define WOM_THR_REG                 0x1F
#define ACCEL_INTR_CTRL_REG         0x69

#define DATA_RATE                   0x08
#define WOM_INT                     0x40
#define DISABLE_GYRO                0x07
#define ENABLE_GYRO                 0x00
#define DISABLE_LPF                 0x08
#define LOW_POWER_CYCLE             0x20
#define ACCEL_WOM                   0xC0
#define WOM_THRESHOLD               0x7D
#define NOT_USED                    0x00
#define AUTO_SELECT_CLK             0x01

typedef struct {
    uint8_t data_rate;
    uint8_t accel_conf;
    uint8_t interrupt;
    uint8_t gyro;
    uint8_t power;
    uint8_t threshold;
    uint8_t accel_int;
} imu_config_t;

// static void process_accel(uint8_t *data, imu_data_t *imu)
// {
//     imu->x_accel = data[0] << 8 | data[1];
//     imu->y_accel = data[2] << 8 | data[3];
//     imu->z_accel = data[4] << 8 | data[5];
// }

// static void process_gyro(uint8_t *data, imu_data_t *imu)
// {
//     imu->x_gyro = data[0] << 8 | data[1];
//     imu->y_gyro = data[2] << 8 | data[3];
//     imu->z_gyro = data[4] << 8 | data[5];
// }

static void imu_i2c_init_helper(i2c_master_dev_handle_t dev_handle, imu_config_t *imu_config)
{
    i2c_register_write_byte(dev_handle, LP_ACCEL_ODR_REG, imu_config->data_rate);
    i2c_register_write_byte(dev_handle, INT_ENABLE_REG, imu_config->interrupt);
    i2c_register_write_byte(dev_handle, PWR_MGMT_2_REG, imu_config->gyro);
    i2c_register_write_byte(dev_handle, ACCEL_CONFIG2_REG, imu_config->accel_conf);
    i2c_register_write_byte(dev_handle, MPU9250_PWR_MGMT_1_REG_ADDR, imu_config->power);
    i2c_register_write_byte(dev_handle, WOM_THR_REG, imu_config->threshold);
    i2c_register_write_byte(dev_handle, ACCEL_INTR_CTRL_REG, imu_config->accel_int);
}

esp_err_t imu_who_am_i(i2c_master_dev_handle_t dev_handle, uint8_t *who_am_i)
{
    return i2c_register_read(dev_handle, MPU9250_WHO_AM_I_REG_ADDR, who_am_i, 1);
}

void imu_init(i2c_master_dev_handle_t dev_handle)
{
    imu_config_t imu_config = {
        .data_rate = DATA_RATE,
        .interrupt = NOT_USED,
        .gyro = ENABLE_GYRO,
        .accel_conf = DISABLE_LPF,
        .power = AUTO_SELECT_CLK,
        .threshold = NOT_USED,
        .accel_int = NOT_USED,
    };
    imu_i2c_init_helper(dev_handle, &imu_config);
}

esp_err_t imu_read(i2c_master_dev_handle_t dev_handle, uint8_t *read_buffer, size_t len)
{
    uint8_t buffer_accel[6];
    uint8_t buffer_gyro[6];
    esp_err_t ret = i2c_register_read(dev_handle, ACCEL_XOUT_H, buffer_accel, sizeof(buffer_accel));

    if (ret != ESP_OK) {
        return ret;
    }

    ret = i2c_register_read(dev_handle, GYRO_XOUT_H, buffer_gyro, sizeof(buffer_gyro));

    if (ret != ESP_OK) {
        return ret;
    }

    memcpy(read_buffer, buffer_accel, sizeof(buffer_accel));
    memcpy(read_buffer + sizeof(buffer_accel), buffer_gyro, sizeof(buffer_gyro));
    return ESP_OK;
}
