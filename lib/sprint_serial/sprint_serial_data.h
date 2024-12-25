#ifndef SPRINT_SERIAL_DATA_H
#define SPRINT_SERIAL_DATA_H

#include "Arduino.h"
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
}sprint_value_t;

typedef struct{
    bool getweight;
    bool getcurrent;
    bool getvoltage;
    bool getrpm;
    bool getrads;
    bool gettemp;
    bool gettemp_of_motors;
    bool getpressure;
}sprint_data_enable_t;   

typedef struct{
    /*baudrate part*/
    unsigned long baudrate;
    unsigned long default_baudrate;
    uint8_t select_baudrate_number;
    uint8_t max_baudrate_number;
    sprint_value_t sprint;
    sprint_data_enable_t sprint_enable;
    /*sprint list*/
    uint8_t select;
    uint8_t maxselect;
    /*sprint data list*/
    uint8_t sprint_list_select;
    uint8_t sprint_list_maxselect;
}my_sprint_t;


void serial_sprint_data_init(my_sprint_t *sprint);

void serial_sprint_reinit(my_sprint_t *sprint);
void serial_sprint_deinit();

void serial_sprintln_char(const char * c);
void serial_sprint_char(const char * c);

void serial_decrease(my_sprint_t *sprint);
void serial_increase(my_sprint_t *sprint);

void set_baud_rate(my_sprint_t *sprint);

void serial_sprint_data(my_sprint_t sprint);



#endif