#ifndef _I2C_MANAGER_H_
#define _I2C_MANAGER_H_

#include <esp_log.h>
#include <inttypes.h>
#include <driver/i2c.h>

#define I2C_MANAGER_TAG "I2C_MANAGER"

typedef struct {
    i2c_port_t port;
    i2c_cmd_handle_t cmd;
} I2C_MANAGER_t;

/* ESP32 IC2 RANGE FREQ: 0->400kHz (IN SOME CASES MAX 1MHz)*/
I2C_MANAGER_t* I2C_MANAGER_custom_master_init(i2c_port_t port, uint32_t clk_speed, gpio_num_t sda, gpio_pullup_t sda_pullup, gpio_num_t scl, gpio_pullup_t scl_pullup);
I2C_MANAGER_t* I2C_MANAGER_master_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl);

#endif