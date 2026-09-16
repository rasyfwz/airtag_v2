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
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "i2c.h"
#include "imu.h"

static const char *TAG = "example";

#define I2C_MASTER_SCL_IO           9                           /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8                           /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          400000                      /*!< I2C master clock frequency */

#define GPIO_INPUT_INT              7
#define GPIO_PIN_SEL                (1U << GPIO_INPUT_INT)

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void motion_interrupt_init(void)
{
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_set_intr_type(GPIO_INPUT_INT, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_INPUT_INT, gpio_isr_handler, (void*) GPIO_INPUT_INT);
}

void app_main(void)
{
    imu_data_t imu;
    uint8_t who_am_i;
    uint32_t io_num;

    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;

    motion_interrupt_init();

    ESP_ERROR_CHECK(i2c_bus_init(&bus_handle, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, I2C_MASTER_NUM));
    ESP_ERROR_CHECK(i2c_device_add(bus_handle, MPU9250_SENSOR_ADDR, I2C_MASTER_FREQ_HZ, &dev_handle));
    ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read the MPU9250 WHO_AM_I register, on power up the register should have the value 0x71 */
    ESP_ERROR_CHECK(imu_who_am_i(dev_handle, &who_am_i));
    ESP_LOGI(TAG, "WHO_AM_I = %X", who_am_i);

    imu_init(dev_handle);

    while (1) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_ERROR_CHECK(imu_read_accel(dev_handle, &imu));
            ESP_ERROR_CHECK(imu_read_gyro(dev_handle, &imu));
            printf("Acceleration (x, y, z): (%hd, %hd, %hd)\n", imu.x_accel, imu.y_accel, imu.z_accel);
            printf("Angular Velocity (x, y, z): (%hd, %hd, %hd)\n", imu.x_gyro, imu.y_gyro, imu.z_gyro);
        }
    }
}
