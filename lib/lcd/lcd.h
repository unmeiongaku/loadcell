#ifndef LCD_H
#define LCD_H


#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "user_define.h"

typedef struct{
    float weight;
    float current;
    float voltage;
    float rpm;
    float rads;
    float temp;
    float temp_of_motors;
    float pressure;
}lcd_value_t;

typedef struct{
    bool getweight;
    bool getcurrent;
    bool getvoltage;
    bool getrpm;
    bool getrads;
    bool gettemp;
    bool gettemp_of_motors;
    bool getpressure;
}lcd_data_enable_t;   


typedef enum{
    MENU_LCD,
    SPRINT_DATA_LCD,
    DISPLAY_DATA_LCD,
    CALIB_HX711_LCD,
    SEARCHING_CALIBRATION_LOADCELL_LCD,
    SAVE_CALIB_DATA_LCD,
    POSSITIVE_PROPELLER_LCD,
    NEGATIVE_PROPELLER_LCD,
    SET_SERIAL_BAUDRATE_LCD,
    STOP_LCD,
}lcd_mode_t;

typedef struct{
    lcd_value_t display;
    lcd_mode_t lcd_mode;
    /*Display list*/
    uint8_t select;
    uint8_t maxselect;
    uint16_t signal;
    /*Display data list*/
    uint8_t data_list_select;
    uint8_t data_list_maxselect;
    /*sprint and display data*/
    lcd_data_enable_t sprintdata;
    lcd_data_enable_t displaydata;
    /*arrow*/
    uint8_t arrow;
    uint8_t page;
    /*isFirstDisplay*/
    bool isFistdisplay;
    /*Baudrate*/
    uint32_t baudrate;
    /*Calibration Value*/
    float loadcell_calibration;
    float saving_data;
}lcd_t;


void lcd_init();
void lcd_clear();
void lcd_set_cursor(uint8_t col, uint8_t row);
uint8_t lcd_display(lcd_t *lcd);
uint8_t lcd_mode_init(lcd_t *lcd,lcd_mode_t lcd_mode);
uint16_t lcd_increase_select(lcd_t *lcd);
uint16_t lcd_decrease_select(lcd_t *lcd);
// uint16_t dispatch_internal_signal(lcd_t *lcd);
uint16_t dispatch_internal_signal_sprint_display(lcd_t *lcd);

uint8_t change_enable_or_disable_sprint(lcd_t *lcd,uint16_t arrow);

void display_data(lcd_t lcd);

void display_saving_calibration_data_to_eeprom();

#endif