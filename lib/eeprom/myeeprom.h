#ifndef MYEEPROM_H
#define MYEEPROM_H

#include "Arduino.h"
#include "EEPROM.h"
#include "user_define.h"
#include "lcd.h"
#include "myhx711.h"
#include "sprint_serial_data.h"

#define BIAS_IEEE_754_FLOATING_32_BIT 127

typedef enum{
    CALIBRATION_LOADCELL_CHECKING_ADDR = 0,
    DISPLAY_WEIGHT_STATUS_CHECKING_ADDR = 1,
    DISPLAY_RPM_STATUS_CHECKING_ADDR = 2,
    DISPLAY_RADS_STATUS_CHECKING_ADDR = 3,
    DISPLAY_CUR_STATUS_CHECKING_ADDR = 4,
    DISPLAY_VOL_STATUS_CHECKING_ADDR = 5,
    DISPLAY_TEMP_STATUS_CHECKING_ADDR = 6,
    DISPLAY_MTEMP_STATUS_CHECKING_ADDR = 7,
    DISPLAY_PRESSURE_STATUS_CHECKING_ADDR = 8,
    SRPINT_WEIGHT_STATUS_CHECKING_ADDR = 9,
    SRPINT_RPM_STATUS_CHECKING_ADDR = 10,
    SRPINT_RADS_STATUS_CHECKING_ADDR = 11,
    SRPINT_CUR_STATUS_CHECKING_ADDR = 12,
    SRPINT_VOL_STATUS_CHECKING_ADDR = 13,
    SRPINT_TEMP_STATUS_CHECKING_ADDR = 14,
    SRPINT_MTEMP_STATUS_CHECKING_ADDR = 15,
    SRPINT_PRESSURE_STATUS_CHECKING_ADDR = 16,
    WEIGHT_CALIBRATION_ADDR = EEPROM_SAVING_WEIGHT_CALIBRATION_ADDR,
}eeprom_saving_addr_t;


void load_eeprom_systerm(sprint_data_enable_t *sprint_enable,lcd_data_enable_t *displaydata);
void save_eeprom_systerm(sprint_data_enable_t *sprint_enable,lcd_data_enable_t *displaydata);

float convert_binary_to_float_32bit(uint8_t buffer[4]);
uint8_t eeprom_write_type_float(uint16_t page_start, float data);
uint8_t eeprom_read_type_float(uint16_t page_start, float *data);

uint8_t eeprom_write_type_int(uint16_t page_start, int data);
uint8_t eeprom_read_type_int(uint16_t page_start, int *data);

uint8_t eeprom_write_type_1_byte_int8(uint16_t page_start, int8_t data);
uint8_t eeprom_read_type_1_byte_int8(uint16_t page_start, int8_t *data);

uint8_t eeprom_write_type_2_bytes_uint16(uint16_t page_start, uint16_t data);
uint8_t eeprom_read_type_2_bytes_uint16(uint16_t page_start, uint16_t *data);

uint8_t eeprom_write_type_2_bytes_int16(uint16_t page_start, int16_t data);
uint8_t eeprom_read_type_2_bytes_int16(uint16_t page_start, int16_t *data);

int compare_floats(float a, float b);

#endif //MYEEPROM_H