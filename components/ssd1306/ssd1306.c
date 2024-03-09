#include "ssd1306.h"
#include "font8x8.h"

static void i2c_ssd1306_set_wise(I2C_SSD1306_t* ssd1306, SSD1306_WISE_t wise){
    esp_err_t ret;
    ssd1306->i2c_manager->cmd = i2c_cmd_link_create();
    i2c_master_start(ssd1306->i2c_manager->cmd);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CONTROL_BYTE_CMD, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_DISPLAY_OFF, true);
    if(wise == SSD1306_TOPTOBOTTOM){
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_COM_SCAN_DIRECTION_NORMAL, true);
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SEGMENT_REMAP_0, true);
    } else if(wise == SSD1306_BOTTOMTOTOP){
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_COM_SCAN_DIRECTION_REMAP, true);
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SEGMENT_REMAP_1, true);
    }
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_DISPLAY_ON, true);
    i2c_master_stop(ssd1306->i2c_manager->cmd);
    ret = i2c_master_cmd_begin(ssd1306->i2c_manager->port, ssd1306->i2c_manager->cmd, 10/portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(SSD1306_TAG, "i2c_master_cmd_begin failed, error=%d", ret);
    }
    i2c_cmd_link_delete(ssd1306->i2c_manager->cmd);
    ssd1306->wise = wise;
}

static void i2c_writebyte_ssd1306RAM(I2C_SSD1306_t* ssd1306, short int page, short int segment, uint8_t byte) {
    esp_err_t ret;
    ssd1306->i2c_manager->cmd = i2c_cmd_link_create();
    i2c_master_start(ssd1306->i2c_manager->cmd);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CONTROL_BYTE_CMD, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (0xB0 | page), true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (0x00 | (segment & 0x0F)), true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (0x10 | ((segment >> 4) & 0x0F)), true);
    i2c_master_stop(ssd1306->i2c_manager->cmd);
    ret = i2c_master_cmd_begin(ssd1306->i2c_manager->port, ssd1306->i2c_manager->cmd, 10/portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(SSD1306_TAG, "i2c_master_cmd_begin failed, error=%d", ret);
    }
    i2c_cmd_link_delete(ssd1306->i2c_manager->cmd);

    ssd1306->i2c_manager->cmd = i2c_cmd_link_create();
    i2c_master_start(ssd1306->i2c_manager->cmd);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CONTROL_BYTE_DATA, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, byte, true);
    i2c_master_stop(ssd1306->i2c_manager->cmd);
    ret = i2c_master_cmd_begin(ssd1306->i2c_manager->port, ssd1306->i2c_manager->cmd, 10/portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(SSD1306_TAG, "i2c_master_cmd_begin failed, error=%d", ret);
    }
    i2c_cmd_link_delete(ssd1306->i2c_manager->cmd);
}

static void i2c_writearray_ssd1306RAM(I2C_SSD1306_t* ssd1306, short int page, short int segment, uint8_t *array, short int length) {
    esp_err_t ret;
    ssd1306->i2c_manager->cmd = i2c_cmd_link_create();
    i2c_master_start(ssd1306->i2c_manager->cmd);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CONTROL_BYTE_CMD, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (0xB0 | page), true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (0x00 | (segment & 0x0F)), true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (0x10 | ((segment >> 4) & 0x0F)), true);
    i2c_master_stop(ssd1306->i2c_manager->cmd);
    ret = i2c_master_cmd_begin(ssd1306->i2c_manager->port, ssd1306->i2c_manager->cmd, 10/portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(SSD1306_TAG, "i2c_master_cmd_begin failed, error=%d", ret);
    }
    i2c_cmd_link_delete(ssd1306->i2c_manager->cmd);

    ssd1306->i2c_manager->cmd = i2c_cmd_link_create();
    i2c_master_start(ssd1306->i2c_manager->cmd);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CONTROL_BYTE_DATA, true);
    i2c_master_write(ssd1306->i2c_manager->cmd, array, length, true);
    i2c_master_stop(ssd1306->i2c_manager->cmd);
    ret = i2c_master_cmd_begin(ssd1306->i2c_manager->port, ssd1306->i2c_manager->cmd, 10/portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(SSD1306_TAG, "i2c_master_cmd_begin failed, error=%d", ret);
    }
    i2c_cmd_link_delete(ssd1306->i2c_manager->cmd);
}


void I2C_SSD1306_display_set_wise(I2C_SSD1306_t* ssd1306, SSD1306_WISE_t wise) {
    i2c_ssd1306_set_wise(ssd1306, wise);
}

void I2C_SSD1306_update_display_segment(I2C_SSD1306_t* ssd1306, short int page, short int segment, uint8_t byte) {
    if(page < 0 || page >= ssd1306->pages_number || segment < 0 || segment >= ssd1306->width){
        ESP_LOGE(SSD1306_TAG, "Invalid page or segment for display segment");
        return;
    }
    i2c_writebyte_ssd1306RAM(ssd1306, page, segment, byte);
}

void I2C_SSD1306_update_display_page(I2C_SSD1306_t* ssd1306, short int page) {
    if(page < 0 || page >= ssd1306->pages_number){
        ESP_LOGE(SSD1306_TAG, "Invalid page for display page");
        return;
    }
    i2c_writearray_ssd1306RAM(ssd1306, page, 0, ssd1306->page[page].segment, ssd1306->width);
}

void I2C_SSD1306_update_display_page_frame(I2C_SSD1306_t* ssd1306, short int page, short int start_segment, short int end_segment) {
    if(page < 0 || page >= ssd1306->pages_number || end_segment <= start_segment || start_segment < 0 || start_segment >= ssd1306->width || end_segment < 0 || end_segment > ssd1306->width){
        ESP_LOGE(SSD1306_TAG, "Invalid page or segment for display frame");
        return;
    }
    uint8_t data[end_segment - start_segment];
    for (int i = 0; i < end_segment - start_segment; i++) {
        data[i] = ssd1306->page[page].segment[start_segment + i];
    }
    i2c_writearray_ssd1306RAM(ssd1306, page, start_segment, data, end_segment - start_segment);
}

void I2C_SSD1306_update_display_frame(I2C_SSD1306_t* ssd1306, short int start_page, short int end_page, short int start_segment, short int end_segment) {
    if(end_page <= start_page || start_page < 0 || start_page >= ssd1306->pages_number || end_page < 0 || end_page >= ssd1306->pages_number || end_segment <= start_segment || start_segment < 0 || start_segment >= ssd1306->width || end_segment < 0 || end_segment >= ssd1306->width){
        ESP_LOGE(SSD1306_TAG, "Invalid page or segment for display frame");
        return;
    }
    uint8_t data[end_segment - start_segment];
    for (int i = start_page; i <= end_page; i++) {
        for (int j = 0; j < end_segment - start_segment; j++) {
            data[j] = ssd1306->page[i].segment[start_segment + j];
        }
        i2c_writearray_ssd1306RAM(ssd1306, i, start_segment, data, end_segment - start_segment);
    }
}

void I2C_SSD1306_update_display(I2C_SSD1306_t* ssd1306) {
    for (int i = 0; i < ssd1306->pages_number; i++) {
        i2c_writearray_ssd1306RAM(ssd1306, i, 0, ssd1306->page[i].segment, ssd1306->width);
    }
}


void I2C_SSD1306_buffer_clear(I2C_SSD1306_t* ssd1306) {
    for (int i = 0; i < ssd1306->pages_number; i++) {
        memset(ssd1306->page[i].segment, 0, ssd1306->width);
    }
}

void I2C_SSD1306_buffer_fill_pixel(I2C_SSD1306_t* ssd1306, short int x, short int y, bool value) {
    if (x < 0 || x >= ssd1306->width || y < 0 || y >= ssd1306->height) {
        ESP_LOGE(SSD1306_TAG, "Invalid coordinates for buffer pixel");
        return;
    }
    if (value) {
        ssd1306->page[y/8].segment[x] |= (1 << (y%8));
    } else {
        ssd1306->page[y/8].segment[x] &= ~(1 << (y%8));
    }
}

void I2C_SSD1306_buffer_fill_space(I2C_SSD1306_t* ssd1306, short int x1, short int y1, short int x2, short int y2, bool value) {
    if (x1 < 0 || x1 >= ssd1306->width || x2 < 0 || x2 >= ssd1306->width || y1 < 0 || y1 >= ssd1306->height || y2 < 0 || y2 >= ssd1306->height) {
        ESP_LOGE(SSD1306_TAG, "Invalid coordinates for buffer fill");
        return;
    }
    if (x1 > x2) {
        short int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2) {
        short int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    for (int i = y1; i < y2; i++) {
        for (int j = x1; j < x2; j++) {
            if (value) {
                ssd1306->page[i/8].segment[j] |= (1 << (i%8));
            } else {
                ssd1306->page[i/8].segment[j] &= ~(1 << (i%8));
            }
        }
    }
}

void I2C_SSD1306_buffer_fill_custom_text(I2C_SSD1306_t* ssd1306, short int page, char *text, bool invert) {
    if (page < 0 || page >= ssd1306->pages_number) {
        ESP_LOGE(SSD1306_TAG, "Invalid page for buffer text");
        return;
    }
    short int x = 0;
    short int length = strlen(text);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < 8; j++) {
            if(invert){
                ssd1306->page[page].segment[x+j] = ~font8x8[(int)text[i]][j];
            } else{
                ssd1306->page[page].segment[x+j] = font8x8[(int)text[i]][j];
            }
        }    
        x += 8;
        if(x >= ssd1306->width){
            page++;
            if (page >= ssd1306->pages_number) {
                ESP_LOGE(SSD1306_TAG, "Buffer text overflow");
                return;
            }
            x = 0;
        }
    }
}

void I2C_SSD1306_buffer_fill_text(I2C_SSD1306_t* ssd1306, short int page, char *text){
    I2C_SSD1306_buffer_fill_custom_text(ssd1306, page, text, false);

}

void I2C_SSD1306_buffer_custom_text_on(I2C_SSD1306_t* ssd1306, short int x, short int y, char *text, int font_multiplier, bool invert, bool text_wrap) {
    if (x < 0 || x >= ssd1306->width || y < 0 || y >= ssd1306->height) {
        ESP_LOGE(SSD1306_TAG, "Invalid coordinates for buffer text");
        return;
    }
    short int length = strlen(text);
    short int page = y / 8;
    short int offset_height = y % 8;
    if (offset_height == 0){
        I2C_SSD1306_buffer_fill_space(ssd1306, x, y, (x+length*8*font_multiplier >= ssd1306->width) ? ssd1306->width -1:x+length*8*font_multiplier, y+8, false);
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 8 * font_multiplier; j++) {
                if(invert){
                    ssd1306->page[page].segment[x+j] = ~font8x8[(int)text[i]][j/font_multiplier];
                } else{
                    ssd1306->page[page].segment[x+j] = font8x8[(int)text[i]][j/font_multiplier];
                }
            }    
            x += 8 * font_multiplier;
            if(x > ssd1306->width){
                if(text_wrap){
                    page++;
                    if (page >= ssd1306->pages_number) {
                        ESP_LOGE(SSD1306_TAG, "Buffer text overflow, text_wrap is enabled but no more pages available");
                        return;
                    }
                    x = 0;
                } else{
                    ESP_LOGE(SSD1306_TAG, "Buffer text overflow, text_wrap is disabled and text is too long for the current page");
                    break;
                }
            }
        } 
    } else{
        if(page + 1 >= ssd1306->pages_number){
            ESP_LOGE(SSD1306_TAG, "Buffer text overflow");
            return;
        }

        I2C_SSD1306_buffer_fill_space(ssd1306, x, y, (x+length*8*font_multiplier >= ssd1306->width) ? ssd1306->width -1:x+length*8*font_multiplier, y+8, false);
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 8 * font_multiplier; j++) {
                if(invert){
                    ssd1306->page[page].segment[x+j] |= (~font8x8[(int)text[i]][j/font_multiplier] << offset_height) & (0xFF << offset_height);
                    ssd1306->page[page+1].segment[x+j] |= (~font8x8[(int)text[i]][j/font_multiplier] >> (8-(offset_height))) & (0xFF >> (8-(offset_height)));
                } else{
                    ssd1306->page[page].segment[x+j] |= (font8x8[(int)text[i]][j/font_multiplier] << (offset_height)) & (0xFF << offset_height);
                    ssd1306->page[page+1].segment[x+j] |= font8x8[(int)text[i]][j/font_multiplier] >> (8-(offset_height)) & (0xFF >> (8-(offset_height)));
                }
            }    
            x += 8 * font_multiplier;
            if(x > ssd1306->width){
                if(text_wrap){
                    page++;
                    if ((page + 1) >= ssd1306->pages_number) {
                        ESP_LOGE(SSD1306_TAG, "Buffer text overflow, text_wrap is enabled but no more pages available");
                        return;
                    }
                    x = 0;
                } else{
                    ESP_LOGE(SSD1306_TAG, "Buffer text overflow, text_wrap is disabled and text is too long for the current pages");
                    break;
                }
            }
        }
    }
}

void I2C_SSD1306_buffer_text_on(I2C_SSD1306_t* ssd1306, short int x, short int y, char *text, bool invert) {
    I2C_SSD1306_buffer_custom_text_on(ssd1306, x, y, text, 1, invert, false);
}

void I2C_SSD1306_buffer_int_on(I2C_SSD1306_t* ssd1306, short int x, short int y, int number, bool invert) {
    char text[12];
    sprintf(text, "%d", number);
    I2C_SSD1306_buffer_custom_text_on(ssd1306, x, y, text, 1, invert, false);
}

void I2C_SSD1306_buffer_custom_float_on(I2C_SSD1306_t* ssd1306, short int x, short int y, float number, short int precision, bool invert) {
    char text[12];
    sprintf(text, "%.*f", precision, number);
    I2C_SSD1306_buffer_custom_text_on(ssd1306, x, y, text, 1, invert, false);
}

void I2C_SSD1306_buffer_float_on(I2C_SSD1306_t* ssd1306, short int x, short int y, float number, bool invert) {
    I2C_SSD1306_buffer_custom_float_on(ssd1306, x, y, number, 2, invert);
}

void I2C_SSD1306_buffer_image_on(I2C_SSD1306_t* ssd1306, short int x, short int y, uint8_t *image, short int width, short int height, bool invert) {
    if (x < 0 || x >= ssd1306->width || y < 0 || y >= ssd1306->height || width < 0 || width > ssd1306->width || height < 0 || height > ssd1306->height) {
        ESP_LOGE(SSD1306_TAG, "Invalid coordinates for buffer image");
        return;
    }
    if (x + width > ssd1306->width || y + height > ssd1306->height) {
        ESP_LOGE(SSD1306_TAG, "Buffer image overflow");
        return;
    }
    short int initial_page = y / 8;
    short int final_page = (y + height - 1) / 8;
    short int range = final_page - initial_page;
    if(final_page >= ssd1306->pages_number){
        ESP_LOGE(SSD1306_TAG, "Buffer image overflow");
        return;
    }
    short int offset_height = y % 8;
    if (offset_height == 0){
        for (int page = 0; page <= range; page++) {
            for (int j = 0; j < width; j++) {
                if(invert){
                    ssd1306->page[initial_page+page].segment[x+j] = ~image[page * width + j];
                } else{
                    ssd1306->page[initial_page+page].segment[x+j] = image[page * width + j];
                }
            } 
        } 
    } else{
        I2C_SSD1306_buffer_fill_space(ssd1306, x, y, x+width, y+height, false);
        for (int page = 0; page < range; page++) {
            for (int j = 0; j < width; j++) {
                if(invert){
                    ssd1306->page[initial_page+page].segment[x+j] |= (~image[page * width + j] << offset_height) & (0xFF << offset_height);
                    ssd1306->page[initial_page+page+1].segment[x+j] |= (~image[page * width + j] >> (8-(offset_height))) & (0xFF >> (8-(offset_height)));
                } else{
                    ssd1306->page[initial_page+page].segment[x+j] |= (image[page * width + j] << (offset_height)) & (0xFF << offset_height);
                    ssd1306->page[initial_page+page+1].segment[x+j] |= image[page * width + j] >> (8-(offset_height)) & (0xFF >> (8-(offset_height)));
                }
            } 
        }
    }
}

I2C_SSD1306_t* I2C_SSD1306_init(I2C_MANAGER_t* i2c_manager, short int address, short int width, short int height, SSD1306_WISE_t wise) {
    esp_err_t ret;
    I2C_SSD1306_t* ssd1306 = (I2C_SSD1306_t*)malloc(sizeof(I2C_SSD1306_t));
    ssd1306->i2c_manager = i2c_manager;
    ssd1306->address = address;
    ssd1306->width = width;
    ssd1306->height = height;
    ssd1306->wise = wise;
    ssd1306->pages_number = height / 8;
    /*  CREATE EMPTY BUFFER PAGES*/
    ssd1306->page = (SSD1306_PAGE_t*)malloc(sizeof(SSD1306_PAGE_t)*ssd1306->pages_number);
    if (ssd1306->page == NULL) {
        ESP_LOGE(SSD1306_TAG, "Memory allocation failed creating SSD1306 PAGE structure");
        free(ssd1306);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ssd1306->pages_number; i++) {
        ssd1306->page[i].segment = (uint8_t*)malloc(sizeof(uint8_t)*ssd1306->width);
        if (ssd1306->page[i].segment == NULL) {
            ESP_LOGE(SSD1306_TAG, "Memory allocation failed creating SSD1306 PAGE segment");
            for (int j = 0; j < i; j++) {
                free(ssd1306->page[j].segment);
            }
            free(ssd1306->page);
            free(ssd1306);
            exit(EXIT_FAILURE);
        }
        memset(ssd1306->page[i].segment, 0, ssd1306->width);
    }
    /*  SETUP INITIAL SSD1306 SETTINGS */
    ssd1306->i2c_manager->cmd = i2c_cmd_link_create();
    i2c_master_start(ssd1306->i2c_manager->cmd);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CONTROL_BYTE_CMD, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_DISPLAY_OFF, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_MUX_RATIO, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, (ssd1306->height - 1), true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_VERT_DISPLAY_OFFSET, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_DISPLAY_START_LINE_0, true);
    if(ssd1306->wise == SSD1306_TOPTOBOTTOM){
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_COM_SCAN_DIRECTION_NORMAL, true);
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SEGMENT_REMAP_0, true);
    } else if(ssd1306->wise == SSD1306_BOTTOMTOTOP){
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_COM_SCAN_DIRECTION_REMAP, true);
        i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SEGMENT_REMAP_1, true);
    }
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_COM_PIN_HARDWARE_MAP, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, 0x12, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_MEMORY_ADDR_MODE, true); 
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_VALUE_PAGE_ADDR_MODE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_CONTRAST_CONTROL, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, 0xFF, true);
    
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_DISPLAY_CLK_DIVIDE, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, 0x80, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_DISPLAY_RAM, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_NORMAL_DISPLAY, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_SET_CHARGE_PUMP, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_VALUE_CHARGE_PUMP_ON, true);
    i2c_master_write_byte(ssd1306->i2c_manager->cmd, OLED_CMD_DISPLAY_ON, true);

    i2c_master_stop(ssd1306->i2c_manager->cmd);

    ret = i2c_master_cmd_begin(ssd1306->i2c_manager->port, ssd1306->i2c_manager->cmd, 10/portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(SSD1306_TAG, "i2c_master_cmd_begin failed, error=%d", ret);
        free(ssd1306);
        exit(EXIT_FAILURE);
    }
    i2c_cmd_link_delete(ssd1306->i2c_manager->cmd);

    ESP_LOGI(SSD1306_TAG, "Initialized at address 0x%02X with %dpx width and %dpx height", ssd1306->address, ssd1306->width, ssd1306->height);
    return ssd1306;
}
