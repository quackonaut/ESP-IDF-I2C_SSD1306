#include <stdio.h>

#include "i2c_manager.h"
#include "ssd1306.h"
#include "ssd1306_images.h"

void app_main(void)
{
    I2C_MANAGER_t* i2c_master = I2C_MANAGER_master_init(I2C_NUM_0, GPIO_NUM_21, GPIO_NUM_22);
    
    I2C_SSD1306_t* ssd1306 = I2C_SSD1306_init(i2c_master, 0x3C, 128, 64, SSD1306_BOTTOMTOTOP);
    I2C_SSD1306_buffer_image_on(ssd1306, 48, 16, (uint8_t *)ssd1306_esp_logo_img, 32, 32, false);
    I2C_SSD1306_update_display(ssd1306);
    vTaskDelay(4000/portTICK_PERIOD_MS);

    I2C_SSD1306_buffer_clear(ssd1306);
    I2C_SSD1306_buffer_text_on(ssd1306, 12, 0, "Hello, world!", true);
    I2C_SSD1306_buffer_fill_space(ssd1306, 0, 9, 127, 10, true);
    I2C_SSD1306_buffer_text_on(ssd1306, 0, 11, "ABCDEFGHIJKLMNOP", false);
    I2C_SSD1306_buffer_text_on(ssd1306, 0, 19, "QRSTUVWXYZabcdfe", false);
    I2C_SSD1306_buffer_text_on(ssd1306, 0, 27, "ghijklmnopqrstuv", false);
    I2C_SSD1306_buffer_text_on(ssd1306, 0, 35, "wxyz1234567890!(", false);
    I2C_SSD1306_buffer_text_on(ssd1306, 0, 43, ")-=+[]{};:'\",.<>", false);
    I2C_SSD1306_buffer_text_on(ssd1306, 0, 51, "?/\\|_`~@#$%^&*", false);
    I2C_SSD1306_update_display(ssd1306);
    vTaskDelay(4000/portTICK_PERIOD_MS);

    I2C_SSD1306_buffer_fill_space(ssd1306, 0, 11, 127, 63, false);
    int x = 123456789;
    I2C_SSD1306_buffer_int_on(ssd1306, 0, 19, x, false);
    I2C_SSD1306_buffer_float_on(ssd1306, 0, 35, x/2, true);
    I2C_SSD1306_update_display(ssd1306);
}