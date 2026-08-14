/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* i2c - Simple Example

   Simple I2C example that shows how to initialize I2C
   as well as reading and writing from and to registers for a sensor connected over I2C.

   The sensor used in this example is a MPU9250 inertial measurement unit.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

static const char *TAG = "example";

#define I2C_MASTER_SCL_IO           9                           /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8                           /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          400000                      /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define MPU9250_SENSOR_ADDR         0x68        /*!< Address of the MPU9250 sensor */
#define MPU9250_WHO_AM_I_REG_ADDR   0x75        /*!< Register addresses of the "who am I" register */
#define MPU9250_PWR_MGMT_1_REG_ADDR 0x6B        /*!< Register addresses of the power management register */
#define MPU9250_RESET_BIT           7

#define ACCEL_XOUT_H                0x3B
#define INT_ENABLE_REG              0x38
#define LP_ACCEL_ODR_REG            0x1E
#define FIFO_EN_REG                 0x23

#define FIFO_CONFIG                 0x08
#define DATA_RATE                   0x08
#define INTERRUPT_CONFIG            0x01

//RAW_RDY_EN
//LP_ACCEL_ODR
typedef struct {
    uint16_t x_accel;
    uint16_t y_accel;
    uint16_t z_accel;
} imu_data_t;

typedef struct {
    uint8_t data_rate;
    uint8_t interrupt;
    uint8_t fifo;

} imu_config_t;

static void i2c_write(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr,)

/**
 * @brief Read a sequence of bytes from a MPU9250 sensor registers
 */
static esp_err_t imu6500_register_read(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, size_t len)
{   
    esp_err_t ret = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS);
    if (ret == ESP_ERR_INVALID_RESPONSE) {
        ESP_LOGE("I2C", "Device NACKed! Check address and wiring.");
    }
    return ret;
}
/**
* @brief process the acceleration buffer
*/
static void process_buffer(uint8_t * data, uint8_t len, imu_t * imu) {
    imu->x_accel = data[0] << 8 | data[1];
    imu->y_accel = data[2] << 8 | data[3];
    imu->z_accel = data[4] << 8 | data[5];
}

/**
 * @brief Write a byte to a MPU9250 sensor register
 */
static esp_err_t imu6500_register_write_byte(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS);
}

static void imu_i2c_init_helper(i2c_master_dev_handle_t dev_handle, imu_config_t * imu_config)
{
    imu6500_register_write_byte(dev_handle, LP_ACCEL_ODR_REG, imu_config->data_rate);
    imu6500_register_write_byte(dev_handle, INT_ENABLE_REG, imu_config->interrupt);
    imu6500_register_write_byte(dev_handle, FIFO_EN_REG, imu_config->fifo);
}

static void imu_init(i2c_master_dev_handle_t dev_handle) {
    imu_config_t imu_config = {
        .data_rate = DATA_RATE,
        .interrupt = INTERRUPT_CONFIG,
        .fifo = FIFO_CONFIG,
    };

    imu_i2c_init_helper(dev_handle, &imu_config);
}

static void read_from_fifo()
{
    
}

/**
 * @brief i2c master initialization
 */
static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MPU9250_SENSOR_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
}

void app_main(void)
{
    uint8_t data[2];
    uint8_t accel_data[6];
    imu_data_t imu;
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    i2c_master_init(&bus_handle, &dev_handle);
    ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read the MPU9250 WHO_AM_I register, on power up the register should have the value 0x71 */
    ESP_ERROR_CHECK(imu6500_register_read(dev_handle, MPU9250_WHO_AM_I_REG_ADDR, data, 1));
    ESP_LOGI(TAG, "WHO_AM_I = %X", data[0]);

    while (1) {
        ESP_ERROR_CHECK(imu6500_register_read(dev_handle, ACCEL_XOUT_H, accel_data, sizeof(accel_data)));
        process_buffer(accel_data, sizeof(accel_data), &imu);

        printf("Acceleration (x, y, z): (%hu, %hu, %hu)\n", imu.x_accel, imu.y_accel, imu.z_accel);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
