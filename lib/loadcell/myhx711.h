#ifndef MYHX711_H
#define MYHX711_H

#include "HX711_ADC.h"
#include "user_define.h"
#include "stdint.h"
#include "stdbool.h"
#include "lcd.h"

typedef struct{
    //Checking Ready
    boolean newDataReady = 0;
    boolean _tare;
    //data
    float weight;
    //using for calibration
    float calibration_data;
    unsigned long stabilizingtime;
    boolean _tare_calib;
    boolean calib_newDataReady;
    /*Calibration Process*/
    bool isCalibrationDone;
    bool isSavingSuccess;
}hx711_global_t;


int8_t hx711_init(hx711_global_t *hx711_global);
float hx711_get_weight(hx711_global_t *hx711_global);

float hx711_calib_init(hx711_global_t *hx711_global);
float hx711_calibration(hx711_global_t *hx711_global);


void save_calibration_data_to_global_loadcell(hx711_global_t *hx711_global,hx711_global_t hx711_calib);

#endif