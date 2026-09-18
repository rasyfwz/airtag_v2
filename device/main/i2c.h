#pragma once

#include "driver/i2c_master.h"
#include "driver/gpio.h"

#define I2C_MASTER_TIMEOUT_MS       1000        /*!< I2C transaction timeout */

/**
 * @brief Create an I2C master bus
 */
esp_err_t i2c_bus_init(i2c_master_bus_handle_t *bus_handle, gpio_num_t sda_io_num, gpio_num_t scl_io_num, i2c_port_num_t port);

/**
 * @brief Attach a device to an existing I2C master bus
 */
esp_err_t i2c_device_add(i2c_master_bus_handle_t bus_handle, uint8_t device_addr, uint32_t freq_hz, i2c_master_dev_handle_t *dev_handle);

/**
 * @brief Read a sequence of bytes from a device register
 */
esp_err_t i2c_register_read(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Write a byte to a device register
 */
esp_err_t i2c_register_write_byte(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data);
