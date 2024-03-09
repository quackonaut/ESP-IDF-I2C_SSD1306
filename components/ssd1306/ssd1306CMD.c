/* https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf */


/*  CONTROL BYTE --------------------------------------------------------------------------------------------------------------*/
/*  
    Co:     0 = The transmission of the following information will contain data bytes only.
            1 = NOT DEFINED
    D/C#:   0 = The following data byte is a command
            1 = The following data byte is a data which will be stored at the GDDRAM. The GDDRAM column address pointer will be 
                increased by one automatically after each data write. 
*/
#define OLED_CONTROL_BYTE_CMD               0x00    // AFTER THE ADDRESS BYTE   (Co=0, D/C#=0)
#define OLED_CONTROL_BYTE_DATA              0x40    // AFTER THE ADDRESS BYTE   (Co=0, D/C#=1)
/*  END ------------------------------------------------------------------------------------------------------------------------*/


/*  FUNDAMENTAL COMMAND --------------------------------------------------------------------------------------------------------*/
#define OLED_CMD_SET_CONTRAST_CONTROL       0x81    // SET CONTRAST (VALUES: 0x00-0xFF;0-255) [DURING RESET: 0x7F]
#define OLED_CMD_DISPLAY_RAM                0xA4    // DISPLAY RAM CONTENT [DURING RESET: 0xA4]
#define OLED_CMD_DISPLAY_ALLON              0xA5    // DISPLAY ALL LEDS ON
#define OLED_CMD_NORMAL_DISPLAY             0xA6    // 0 IN RAM: LED OFF & 1 IN RAM = LED ON [DURING RESET: 0xA6]
#define OLED_CMD_INVERTED_DISPLAY           0xA7    // 0 IN RAM: LED ON & 1 IN RAM = LED OFF
#define OLED_CMD_DISPLAY_OFF                0xAE    // TURN OFF OLED PANEL (SLEEP MODE) [DURING RESET: 0xAE]
#define OLED_CMD_DISPLAY_ON                 0xAF    // TURN ON OLED PANEL (NORMAL MODE)
/*  END ------------------------------------------------------------------------------------------------------------------------*/


/*  ADDRESSING SETTING ---------------------------------------------------------------------------------------------------------*/
#define OLED_CMD_SET_MEMORY_ADDR_MODE       0x20    // MEMORY ADDRESSING MODE (VALUES: 0x00,0x01,0x02;HORZ,VERT,PAGE) [DURING RESET: 0x02]
#define OLED_VALUE_PAGE_ADDR_MODE           0x02    // Page Addressing Mode
#define OLED_VALUE_HORIZONTAL_ADDR_MODE     0x00    // Horizontal Addressing Mode
#define OLED_VALUE_VERTICAL_ADDR_MODE       0x01    // Vertical Addressing Mode
#define OLED_CMD_SET_COLUMN_ADDR_RANGE      0x21    // SET COLUMN START&END ADDRESS (VALUES: 0x00-0x7F;0-127)&(VALUES: 0x00-0x7F;0-127) [DURING RESET: 0x00&0x7F | ONLY HORZ/VERT ADDRESS MODE]
#define OLED_CMD_SET_PAGE_ADDR_RANGE        0x22    // SET PAGE START&END ADDRESS (VALUES: 0x00-0x07;0-7)&(VALUES: 0x00-0x07;0-7) [DURING RESET: 0x00&0x07 | ONLY HORZ/VERT ADDRESS MODE]
#define OLED_VALUE_PAGE_ADDR_MODE           0x02    // Page Addressing Mode
#define OLED_CMD_PAGE_LOWER_START_ADDR_0    0x00    // PAGE LOWER COLUMN START ADDRESS 0 (0x00-0x0F;0-15) [DURING RESET: 0x00 | ONLY PAGE ADDRESS MODE]
#define OLED_CMD_PAGE_HIGHER_START_ADDR_0   0X10    // PAGE HIGHER COLUMN START ADDRESS 0 (0x10-0x1F;16-31) [DURING RESET: 0x10 | ONLY PAGE ADDRESS MODE]?
#define OLED_CMD_NO_OPERATION               0xE3    // NO OPERATION COMMAND
/*  END ------------------------------------------------------------------------------------------------------------------------*/


/*  HARDWARE CONFIGURATION (Panel resolution & layout related) -----------------------------------------------------------------*/
#define OLED_CMD_DISPLAY_START_LINE_0       0x40    // RAM DISPLAY START LINE (0x40-0x7F;64-127; 0->64) [DURING RESET: 0x40]
#define OLED_CMD_SEGMENT_REMAP_0            0xA0    // COLUMN ADDRESS 0 IS MAPPED TO SEG0 (LEFT TO RIGHT) [DURING RESET: 0xA0]
#define OLED_CMD_SEGMENT_REMAP_1            0xA1    // COLUMN ADDRESS 127 IS MAPPED TO SEG0 (RIGHT TO LEFT)
#define OLED_CMD_SET_MUX_RATIO              0xA8    // SET MUX RATIO (VALUES: 0x0F-0x3F;15-63) [DURING RESET: 64MUX=0x3F; 48MUX=0x2F, 40MUX=0X27, 32MUX=0x1F]
// OLED_CMD_COM_SCAN_DIRECTION <-(AFFECTS)-> OLED_CMD_SET_COM_PIN_HARDWARE_MAP
#define OLED_CMD_COM_SCAN_DIRECTION_NORMAL  0xC0    // COM SCAN DIRECTION NORMAL COM0 -> COM[MUX-1] (BOTTOM TO TOP)
#define OLED_CMD_COM_SCAN_DIRECTION_REMAP   0xC8    // COM SCAN DIRECTION REMAPPED COM[MUX-1] -> COM0 (TOP TO BOTTOM)
#define OLED_CMD_SET_COM_PIN_HARDWARE_MAP   0xDA    // SET COM PINS HARDWARE (VALUES: 0x02, 0x12, 0x22, 0x32 ; DISABLE[COM]REMAP-SEQUENTIAL[COM], DISABLE[COM]REMAP-ALTERNATIVE[COM], ENABLE[COM]REMAP-SEQUENTIAL[COM], ENABLE[COM]REMAP-ALTERNATIVE[COM]) [DURING RESET: 0x12]
// <-(END AFFECTS)->
#define OLED_CMD_SET_VERT_DISPLAY_OFFSET    0xD3    // SET VERTICAL DISPLAY OFFSET (VALUES: 0x00-0x3F;0-63) [DURING RESET: 0x00]
/*  END ------------------------------------------------------------------------------------------------------------------------*/


/*  CONFIGURE DISPLAY  CLOCK
        DCLOCK = FREQOSC / DIVIDE_RATIO
        FREQFRAME = FREQOSC / (NUMBER_OF_DISPLAY_CLOCKS_PER_ROW * DIVIDE_RATIO * NUMBER_OF_MUX)
        WHERE:
            FREQOSC = 0x00->0x0F (0-15)
            DIVIDE_RATIO = [0x00->0x0F (1-16)] + 1
            NUMBER_OF_DISPLAY_CLOCKS_PER_ROW =  Phase 1 period + Phase 2 period + BANK0 = [DURING RESET: 2 + 2 + 50 = 54] 
            NUMBER_OF_MUX = 0x0F->0x3F (15-63)
*/


/*  TIMING & DRIVING SCHEME SETTING --------------------------------------------------------------------------------------------*/
#define OLED_CMD_SET_DISPLAY_CLK_DIVIDE     0xD5    // SET DISPLAY CLOCK OSCILLATOR FREQUENCY & DIVIDE RATIO (VALUE: (OSCFREQ) << 4 | (DIVIDE RATIO)) [DURING RESET: 0x80]
// OSCFREQ= 0x00->0x0F (0-15 CONFIG NOT MENTION IN DATASHEET), DIVIDE RATIO= [0x00->0x0F (1-16)] + 1
#define OLED_CMD_SET_PRECHARGE_PERIOD       0xD9    // SET PRE-CHARGE PERIOD (VALUE: PHASE2 << 4 | PHASE1) [DURING RESET: 0x22]
// DEFAULT PHASE2=0x02;2 DOTS, PHASE1=0x02;2 DOTS -> (0x02)<<4 | 0x02 = 0X22 [CLOCKS 0 TO 15, CLOCK 0 IS INVALID ENTRY]
#define OLED_CMD_SET_VCOMH_DESELECT_LEVEL   0xDB    // SET VCOMH DESELECT LEVEL (VALUE: 0X00,0X20,0X30;0.65,0.77,0.83 VCC) [DURING RESET: 0x20]
/*  END ------------------------------------------------------------------------------------------------------------------------*/


/*  CHARGE PUMP ----------------------------------------------------------------------------------------------------------------*/
#define OLED_CMD_SET_CHARGE_PUMP            0x8D    // SET CHARGE PUMP (VALUES: 0x10,0x14;OFF,ON) [DURING RESET: 0x14]
#define OLED_VALUE_CHARGE_PUMP_ON           0x14    // Charge Pump ON
#define OLED_VALUE_CHARGE_PUMP_OFF          0x10    // Charge Pump OFF
/*  END ------------------------------------------------------------------------------------------------------------------------*/


// Scrolling Command (Pg.28)
#define OLED_CMD_RIGHT_HORIZONTAL_SCROLL    0x26
#define OLED_CMD_LEFT_HORIZONTAL_SCROLL     0x27 //<---------------------------------------
#define OLED_CMD_CONTINUOUS_SCROLL          0x29
#define OLED_CMD_DEACTIVE_SCROLL            0x2E
#define OLED_CMD_ACTIVE_SCROLL              0x2F
#define OLED_CMD_VERTICAL                   0xA3

