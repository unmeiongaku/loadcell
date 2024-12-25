#ifndef MYEEPROM_H
#define MYEEPROM_H

#include "Arduino.h"
#include "EEPROM.h"
#include "user_define.h"

#define BIAS_IEEE_754_FLOATING_32_BIT 127

typedef enum{
    WEIGHT_CALIBRATION_ADDR = EEPROM_SAVING_WEIGHT_CALIBRATION_ADDR,
}eeprom_saving_addr_t;

float convert_binary_to_float_32bit(uint8_t buffer[4]);
uint8_t eeprom_write_type_float(uint16_t page_start, float data);
uint8_t eeprom_read_type_float(uint16_t page_start, uint16_t offset, float *data);

uint8_t eeprom_write_type_int(uint16_t page_start, int data);
uint8_t eeprom_read_type_int(uint16_t page_start, int *data);

uint8_t eeprom_write_type_1_byte_int8(uint16_t page_start, int8_t data);
uint8_t eeprom_read_type_1_byte_int8(uint16_t page_start, int8_t *data);

uint8_t eeprom_write_type_2_bytes_uint16(uint16_t page_start, uint16_t data);
uint8_t eeprom_read_type_2_bytes_uint16(uint16_t page_start, uint16_t *data);

uint8_t eeprom_write_type_2_bytes_int16(uint16_t page_start, int16_t data);
uint8_t eeprom_read_type_2_bytes_int16(uint16_t page_start, int16_t *data);

#endif //MYEEPROM_H