#include "i2c.h"
#include "esp_log.h"

static const char *TAG = "I2C";

esp_err_t i2c_bus_init(i2c_master_bus_handle_t *bus_handle, gpio_num_t sda_io_num, gpio_num_t scl_io_num, i2c_port_num_t port)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = port,
        .sda_io_num = sda_io_num,
        .scl_io_num = scl_io_num,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    return i2c_new_master_bus(&bus_config, bus_handle);
}

esp_err_t i2c_device_add(i2c_master_bus_handle_t bus_handle, uint8_t device_addr, uint32_t freq_hz, i2c_master_dev_handle_t *dev_handle)
{
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_addr,
        .scl_speed_hz = freq_hz,
    };
    return i2c_master_bus_add_device(bus_handle, &dev_config, dev_handle);
}

esp_err_t i2c_register_read(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, size_t len)
{
    esp_err_t ret = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS);
    if (ret == ESP_ERR_INVALID_RESPONSE) {
        ESP_LOGE(TAG, "Device NACKed! Check address and wiring.");
    }
    return ret;
}

esp_err_t i2c_register_write_byte(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS);
}
