#include "i2c_manager.h"


I2C_MANAGER_t* I2C_MANAGER_custom_master_init(i2c_port_t port, uint32_t clk_speed, gpio_num_t sda, gpio_pullup_t sda_pullup, gpio_num_t scl, gpio_pullup_t scl_pullup) {
    esp_err_t ret;
    I2C_MANAGER_t* i2c_manager = (I2C_MANAGER_t*)malloc(sizeof(I2C_MANAGER_t));
    i2c_manager->port = port;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = sda_pullup,
        .scl_pullup_en = scl_pullup,
        .master.clk_speed = clk_speed
    };

    ret = i2c_param_config(i2c_manager->port, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(I2C_MANAGER_TAG, "i2c_param_config failed, error=%d", ret);
        free(i2c_manager);
        exit(EXIT_FAILURE);
    }
    ret = i2c_driver_install(i2c_manager->port, I2C_MODE_MASTER, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(I2C_MANAGER_TAG, "i2c_driver_install failed, error=%d", ret);
        free(i2c_manager);
        exit(EXIT_FAILURE);
    }
    ESP_LOGI(I2C_MANAGER_TAG, "I2C master initialized on port %d at %dHz using gpios SDA: %d & SCL: %d", i2c_manager->port, (int)clk_speed, (int)sda, (int)scl);
    return i2c_manager;
}

I2C_MANAGER_t* I2C_MANAGER_master_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl) {
    return I2C_MANAGER_custom_master_init(port, 400000, sda, GPIO_PULLUP_ENABLE, scl, GPIO_PULLUP_ENABLE);
}
