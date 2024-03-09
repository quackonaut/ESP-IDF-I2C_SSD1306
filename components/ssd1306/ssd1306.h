#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "i2c_manager.h"
#include "ssd1306CMD.c"
#include <stdlib.h>
#include <string.h>

#define SSD1306_TAG "SSD1306"

typedef enum {
    SSD1306_TOPTOBOTTOM,
    SSD1306_BOTTOMTOTOP
} SSD1306_WISE_t;

typedef struct {
    uint8_t *segment;
} SSD1306_PAGE_t;

typedef struct {
    I2C_MANAGER_t* i2c_manager;
    short int address;
    short int width;
    short int height;
    SSD1306_WISE_t wise;
    short int pages_number;
    SSD1306_PAGE_t *page;
} I2C_SSD1306_t;

I2C_SSD1306_t* I2C_SSD1306_init(I2C_MANAGER_t* i2c_manager, short int address, short int width, short int height, SSD1306_WISE_t wise);

void I2C_SSD1306_display_set_wise(I2C_SSD1306_t* ssd1306, SSD1306_WISE_t wise);
void I2C_SSD1306_update_display_segment(I2C_SSD1306_t* ssd1306, short int page, short int segment, uint8_t byte);
void I2C_SSD1306_update_display_page(I2C_SSD1306_t* ssd1306, short int page);
void I2C_SSD1306_update_display_page_frame(I2C_SSD1306_t* ssd1306, short int page, short int start_segment, short int end_segment);
void I2C_SSD1306_update_display_frame(I2C_SSD1306_t* ssd1306, short int start_page, short int end_page, short int start_segment, short int end_segment);
void I2C_SSD1306_update_display(I2C_SSD1306_t* ssd1306);

void I2C_SSD1306_buffer_clear(I2C_SSD1306_t* ssd1306);
void I2C_SSD1306_buffer_fill_pixel(I2C_SSD1306_t* ssd1306, short int x, short int y, bool value);
void I2C_SSD1306_buffer_fill_space(I2C_SSD1306_t* ssd1306, short int x1, short int y1, short int x2, short int y2, bool value);
void I2C_SSD1306_buffer_fill_custom_text(I2C_SSD1306_t* ssd1306, short int page, char *text, bool invert);
void I2C_SSD1306_buffer_fill_text(I2C_SSD1306_t* ssd1306, short int page, char *text);
void I2C_SSD1306_buffer_custom_text_on(I2C_SSD1306_t* ssd1306, short int x, short int y, char *text, int font_multiplier, bool invert, bool text_wrap);
void I2C_SSD1306_buffer_text_on(I2C_SSD1306_t* ssd1306, short int x, short int y, char *text, bool invert);
void I2C_SSD1306_buffer_int_on(I2C_SSD1306_t* ssd1306, short int x, short int y, int number, bool invert);
void I2C_SSD1306_buffer_custom_float_on(I2C_SSD1306_t* ssd1306, short int x, short int y, float number, short int precision, bool invert);
void I2C_SSD1306_buffer_float_on(I2C_SSD1306_t* ssd1306, short int x, short int y, float number, bool invert);
void I2C_SSD1306_buffer_image_on(I2C_SSD1306_t* ssd1306, short int x, short int y, uint8_t *image, short int width, short int height, bool invert);

#endif