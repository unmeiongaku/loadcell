#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include <stdint.h>
#include "myhx711.h"
#include "lcd.h"
#include "sprint_serial_data.h"
#include "mybmp280.h"
#include "Adafruit_BMP280.h"
#include "EEPROM.h"


typedef struct{
    uint32_t totalcnt;
    uint32_t currentcnt;
    uint32_t lastcounter;
}motor_counter_t;

typedef enum{
    NOT_PRESSED,
    BOUNCE,
    PRESSED,
}button_state_t;

typedef enum {
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION
}event_status_t;


/* Signals of the application*/
typedef enum{
    BACK_SIG = 1,
    SETTING_SIG,
    CW_SIG,
    CCW_SIG,
/*Internal transition sig*/
    TIME_TICK_SIG,
/*button sm transition sig*/
    SPRINT_DATA_SIG, 
    DISPLAY_DATA_SIG, 
    CALIB_HX711_SIG, 
    POSITIVE_PROPELLER_SIG, 
    NEGATIVE_PROPELLER_SIG,
    SET_SERIAL_BAUDRATE_SIG,
/* Internal activity signals */
    ENTRY,
    EXIT
}proobject_signal_t;

typedef enum{
    MENU_SM,
    SPRINT_DATA_SM,
    DISPLAY_DATA_SM,
    CALIB_HX711_SM,
    SEARCHING_CALIBRATION_LOADCELL_SM,
    SAVE_LOAD_CELL_CALIBRATION_DATA_SM,
    POSITIVE_PROPELLER_SM,
    NEGATIVE_PROPELLER_SM,
    SET_SERIAL_BAUDRATE_SM,
    STOP_SM,
    MAX_SM,
}proobject_state_t;


typedef struct{
    float weight;
    float current;
    float voltage;
    float rpm;
    float rads;
    float temp;
    float temp_of_motors;
    float pressure;
}data_struct_t;

//forward declarations
struct proobject_tag;
struct event_tag;

/* Main application structure */
typedef struct proobject_tag {
    /*global data*/
    data_struct_t globaldata;
    /*lcd*/
    lcd_t   dlcd;
    uint16_t arrow_display;
    /*Serail sprint*/
    my_sprint_t globalsprint;
    uint16_t arrow_sprint;
    /*loadcell*/
    hx711_global_t loadcell_global;
    hx711_global_t loadcell_calibration;
    bool isLoadCellGlobalInitDone;
    /*bmp280*/
    my_bmp280_t bmp280;
    /*state*/
    proobject_state_t active_state;
}proobject_t;

/*Generic(Super) event structure */
typedef struct event_tag{
    uint8_t sig;
}event_t;

/* For user generated events */
typedef struct{
    event_t super;
    uint8_t ss;
}proobject_user_event_t;

/* For tick event */
typedef struct{
    event_t super;
    uint8_t ss;
}proobject_tick_event_t;


void proobject_init(proobject_t *const mobj);
event_status_t proobject_state_machine(proobject_t *const mobj, event_t const * const e);

uint16_t get_internal_signal();
void reset_internal_signal();

#endif //ACTIVE_OBJECT_H

