#ifndef MYBMP280_H
#define MYBMP280_H


#include "Adafruit_BMP280.h"
#include "Arduino.h"
#include "Wire.h"

typedef struct{
    float temp;
    float pressure;
    sensors_event_t temp_event;
    sensors_event_t pressure_event;
}my_bmp280_t;

void my_bmp280_init(my_bmp280_t *my_bmp280);
void my_bmp280_get_data(my_bmp280_t *my_bmp280);

#endif