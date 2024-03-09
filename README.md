# _I2C SSD1306 OLED Driver_

(See the README.md file in the upper level 'examples' directory for more information about examples.)
This is the simplest buildable example. The example is used by command `idf.py create-project`
that copies the project to user specified path and set it's name. For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project)

This is the first version of a driver developed to be used as a component for implementing an OLED using the SSD1306. In theory, the code should work for different versions of the OLED SSD1306 module (128x64, 128x32, 72x40, 64x48). However, its functionality has only been verified on an ESP32 DEV MODULE board with 38 pins and 30 pins, paired with a 128x64 OLED (0.96"). Therefore, if you plan to use it on other ESP boards or with different OLED configurations, you must be very cautious. Alternatively, you can use other drivers that have been tested on multiple boards, such as the ones mentioned below. These were used as a basis for the development of this project.

Repositories used as a foundation for the development of this project:

https://github.com/nopnop2002/esp-idf-ssd1306

https://github.com/yanbe/ssd1306-esp-idf-i2c

Datasheets used:
https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

## Important

This component is under development, so its functionality has only been verified on an ESP32 DEV MOD board using an OLED SSD1306 0.96" (128x64). For now, I recommend its implementation using these components.

## How to use this component

STEP 1:

To implement this component (library) in your ESP-IDF project, you just need to copy the files located in the components directory "i2c_manager" and "ssd1306" to your components folder and include them in your main code. The first component "i2c_manager" is a component that creates a structure (simulating an object) which sets up and configures a means to use the ESP32's I2C. This is done so that you can implement the I2C protocol as a master in other modules that require it, not only in the OLED SSD1306. The second component "ssd1306" is the controller for the OLED SSD1306, where the methods and functions used by the component are located.

STEP 2:

Once the components are included in your project and added to your main program, you just need to use the functions provided in this version, which allow you to print texts, integers, floats, and load images. In the main program of this repository, "main.c", you will find the main methods and functions that you can use. The operation, in broad terms, consists of internally creating an array to store the buffer of the OLED screen, which will later be transferred to the RAM of the SSD1306. As can be seen in "main.c", the methods involving buffer editing are executed first, and then, using the function I2C_SSD1306_update_display(ssd1306), they are sent to the OLED's RAM.

## Do you have any questions, suggestions, or have you found any errors?

If you have any questions or suggestions about the operation of the component, or if you encountered any errors while compiling it on your ESP32 board, please don't hesitate to leave your comment.
