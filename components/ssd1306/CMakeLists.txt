idf_component_register(
    SRCS "ssd1306CMD.c" "ssd1306.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES driver i2c_manager
)