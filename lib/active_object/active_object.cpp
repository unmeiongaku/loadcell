#include "user_define.h"
#include "active_object.h"
#include "lcd.h"
#include "LiquidCrystal_I2C.h"
// #include "Eeprom24Cxx.h"
#include "Adafruit_BMP280.h"
#include "mybmp280.h"
// #include "myhx711.h"
#include "sprint_serial_data.h"

static event_status_t proobject_state_handle_MENU_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_SPINT_DATA_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_DISPLAY_DATA_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_CALIB_HX711_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_PUT_STANDARD_MASS_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_SAVE_LOAD_CELL_CALIBRATION_DATA_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_POSSITIVE_PROPELLER_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_NEGATIVE_PROPELLER_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_SET_SERIAL_BAUDRATE_SM(proobject_t *const mobj, event_t const *const e);
static event_status_t proobject_state_handle_STOP_SM(proobject_t *const mobj, event_t const *const e);

static void dispatch_signals(event_t const * const e);
static void sprint_signal(proobject_t *const mobj, event_t const * const e);


static uint16_t internal_signal;


uint16_t get_internal_signal(){
    return internal_signal;
}

void reset_internal_signal(){
    internal_signal = 0;
}

void buzzer_notification();
void led_notification();

void buzzer_notification(){
    digitalWrite(BUZZER_GPIO,HIGH);
    delay(20);
    digitalWrite(BUZZER_GPIO,LOW);
}

void led_notification(){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN,LOW);
}


static uint8_t get_datas(proobject_t *const mobj);
static void save_data_to_display_lcd(proobject_t *const mobj);
static void save_data_to_sprint_terminal(proobject_t *const mobj);

static uint8_t get_datas(proobject_t *const mobj){
    //mobj->globaldata.weight = hx711_get_weight(&mobj->loadcell_global);
    return 0;
}

static void save_data_to_display_lcd(proobject_t *const mobj){
    mobj->dlcd.display.weight = mobj->globaldata.weight;
    mobj->dlcd.display.current = mobj->globaldata.current;
    mobj->dlcd.display.pressure = mobj->globaldata.pressure;
    mobj->dlcd.display.rads = mobj->globaldata.rads;
    mobj->dlcd.display.rpm = mobj->globaldata.rpm;
    mobj->dlcd.display.temp = mobj->globaldata.temp;
    mobj->dlcd.display.temp_of_motors = mobj->globaldata.temp_of_motors;
    mobj->dlcd.display.voltage = mobj->globaldata.voltage;
}

static void save_data_to_sprint_terminal(proobject_t *const mobj){
    mobj->globalsprint.sprint.weight = mobj->globaldata.weight;
    mobj->globalsprint.sprint.current = mobj->globaldata.current;
    mobj->globalsprint.sprint.pressure = mobj->globaldata.pressure;
    mobj->globalsprint.sprint.rads = mobj->globaldata.rads;
    mobj->globalsprint.sprint.rpm = mobj->globaldata.rpm;
    mobj->globalsprint.sprint.temp = mobj->globaldata.temp;
    mobj->globalsprint.sprint.temp_of_motors = mobj->globaldata.temp_of_motors;
    mobj->globalsprint.sprint.voltage = mobj->globaldata.voltage;
}


void proobject_init(proobject_t *const mobj){
    /*Button Init*/
    pinMode(BUTTON_SETTING_SIG,INPUT);
    pinMode(BUTTON_BACK_SIG,INPUT);
    pinMode(BUTTON_INCREASE_SIG,INPUT);
    pinMode(BUTTON_DECREASE_SIG,INPUT);
    pinMode(BUZZER_GPIO,OUTPUT);
    /*Sensor init*/
    //my_bmp280_init(&mobj->bmp280);
    /*Lcd Init*/
    lcd_init();
    /*Serial Print Init*/
    serial_sprint_data_init(&mobj->globalsprint);
    event_t ee; 
    ee.sig = ENTRY;
    mobj->active_state = MENU_SM;
    proobject_state_machine(mobj,&ee);
}

event_status_t proobject_state_machine(proobject_t *const mobj, event_t const * const e){
    sprint_signal(mobj,e);
    switch(mobj->active_state){
        case MENU_SM:
        {
            return proobject_state_handle_MENU_SM(mobj,e);
        }
            break;
        case SPRINT_DATA_SM:
        {
            return proobject_state_handle_SPINT_DATA_SM(mobj,e);
        }
            break;
        case DISPLAY_DATA_SM:
        {
            return proobject_state_handle_DISPLAY_DATA_SM(mobj,e);
        }
            break;
        case CALIB_HX711_SM:
        {
            return proobject_state_handle_CALIB_HX711_SM(mobj,e);
        }
            break;
        case PUT_STANDARD_MASS_SM:
        {
            return proobject_state_handle_PUT_STANDARD_MASS_SM(mobj,e);
        }
            break;
        case SAVE_LOAD_CELL_CALIBRATION_DATA_SM:
        {
            return proobject_state_handle_SAVE_LOAD_CELL_CALIBRATION_DATA_SM(mobj,e);
        }
            break;
        case POSITIVE_PROPELLER_SM:
        {
            return proobject_state_handle_POSSITIVE_PROPELLER_SM(mobj,e);
        }
            break;
        case NEGATIVE_PROPELLER_SM:
        {
            return proobject_state_handle_NEGATIVE_PROPELLER_SM(mobj,e);
        }
            break;
        case SET_SERIAL_BAUDRATE_SM:
        {
            return proobject_state_handle_SET_SERIAL_BAUDRATE_SM(mobj,e);
        }
        case STOP_SM:
        {
            return proobject_state_handle_STOP_SM(mobj,e);
        }
            break;
        case MAX_SM:
            break;
    }
    return EVENT_IGNORED;
}


static event_status_t proobject_state_handle_MENU_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_clear();
            lcd_mode_init(&mobj->dlcd,MENU_LCD);
            lcd_display(&mobj->dlcd);
            internal_signal = dispatch_internal_signal_sprint_display(&mobj->dlcd);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case CW_SIG:
        {
            led_notification();
            buzzer_notification();
            internal_signal = lcd_increase_select(&mobj->dlcd);
            /*Display_lcd*/
            lcd_display(&mobj->dlcd);
            return EVENT_HANDLED; 
        } 
        case CCW_SIG:
        {
            led_notification();
            buzzer_notification();
            internal_signal = lcd_decrease_select(&mobj->dlcd);
            /*Display_lcd*/
            lcd_display(&mobj->dlcd);
            return EVENT_HANDLED; 
        } 
        case SPRINT_DATA_SIG:
        {
            mobj->active_state = SPRINT_DATA_SM;
            return EVENT_TRANSITION;
        }
        case DISPLAY_DATA_SIG:
        {
            mobj->active_state = DISPLAY_DATA_SM;
            return EVENT_TRANSITION;
        }
        case CALIB_HX711_SIG:
        {
            mobj->active_state = CALIB_HX711_SM;
            return EVENT_TRANSITION;
        }
        case POSITIVE_PROPELLER_SIG:
        {
            mobj->active_state = POSITIVE_PROPELLER_SM;
            return EVENT_TRANSITION;
        }
        case NEGATIVE_PROPELLER_SIG:
        { 
            mobj->active_state = NEGATIVE_PROPELLER_SM;
            return EVENT_TRANSITION;
        }
        case SET_SERIAL_BAUDRATE_SIG:
        { 
            mobj->active_state = SET_SERIAL_BAUDRATE_SM;
            return EVENT_TRANSITION;
        }             
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_SPINT_DATA_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            mobj->arrow_sprint = 0;
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,SPRINT_DATA_LCD);
            lcd_display(&mobj->dlcd);
            dispatch_internal_signal_sprint_display(&mobj->dlcd);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case CW_SIG:
        {
            buzzer_notification();
            lcd_increase_select(&mobj->dlcd);
            mobj->arrow_sprint = dispatch_internal_signal_sprint_display(&mobj->dlcd);
            /*Display_lcd*/
            lcd_display(&mobj->dlcd);
            /*dispatch_enable_disable*/
            return EVENT_HANDLED; 
        } 
        case CCW_SIG:
        {
            buzzer_notification();
            lcd_decrease_select(&mobj->dlcd);
            mobj->arrow_sprint = dispatch_internal_signal_sprint_display(&mobj->dlcd);
            /*Display_lcd*/
            lcd_display(&mobj->dlcd);
            return EVENT_HANDLED; 
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }    
        case SETTING_SIG:
        {
            buzzer_notification();
            change_enable_or_disable_sprint(&mobj->dlcd,mobj->arrow_sprint);
            return EVENT_HANDLED;
        }
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_DISPLAY_DATA_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,DISPLAY_DATA_LCD);
            lcd_display(&mobj->dlcd);
            dispatch_internal_signal_sprint_display(&mobj->dlcd);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case CW_SIG:
        {
            buzzer_notification();
            lcd_increase_select(&mobj->dlcd);
            mobj->arrow_display = dispatch_internal_signal_sprint_display(&mobj->dlcd);
            /*Display_lcd*/
            lcd_display(&mobj->dlcd);
            /*dispatch_enable_disable*/
            return EVENT_HANDLED; 
        } 
        case CCW_SIG:
        {
            buzzer_notification();
            lcd_decrease_select(&mobj->dlcd);
            mobj->arrow_display = dispatch_internal_signal_sprint_display(&mobj->dlcd);
            /*Display_lcd*/
            lcd_display(&mobj->dlcd);
            return EVENT_HANDLED; 
        }
        case SETTING_SIG:
        {
            buzzer_notification();
            change_enable_or_disable_sprint(&mobj->dlcd,mobj->arrow_display);
            return EVENT_HANDLED;
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }  
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_CALIB_HX711_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,CALIB_HX711_LCD);
            //hx711_calib_init(&mobj->loadcell_global);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case CW_SIG:
        {
            return EVENT_HANDLED;
        }
        case SETTING_SIG:
        {
            mobj->active_state = SAVE_LOAD_CELL_CALIBRATION_DATA_SM;
            return EVENT_TRANSITION; 
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }  
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_PUT_STANDARD_MASS_SM(proobject_t *const mobj, event_t const *const e){
       switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,PUT_STANDARD_MASS_LCD);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case CCW_SIG:
        {
            mobj->active_state = SAVE_LOAD_CELL_CALIBRATION_DATA_SM;
            return EVENT_TRANSITION; 
        }
        case CW_SIG:
        {
            mobj->active_state = SAVE_LOAD_CELL_CALIBRATION_DATA_SM;
            return EVENT_TRANSITION; 
        }
        case SETTING_SIG:
        {
            mobj->active_state = CALIB_HX711_SM;
            return EVENT_TRANSITION; 
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }  
    }
    return EVENT_IGNORED; 
}


static event_status_t proobject_state_handle_SAVE_LOAD_CELL_CALIBRATION_DATA_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,SAVE_CALIB_DATA_LCD);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case SETTING_SIG:
        {
            mobj->active_state = CALIB_HX711_SM;
            return EVENT_TRANSITION;
        }  
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }  
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_POSSITIVE_PROPELLER_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,POSSITIVE_PROPELLER_LCD);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case TIME_TICK_SIG:
        {
            return EVENT_HANDLED; 
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }    
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_NEGATIVE_PROPELLER_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,NEGATIVE_PROPELLER_LCD);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case TIME_TICK_SIG:
        {
            return EVENT_HANDLED; 
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }     
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_SET_SERIAL_BAUDRATE_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,SET_SERIAL_BAUDRATE_LCD);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }  
    }
    return EVENT_IGNORED;
}

static event_status_t proobject_state_handle_STOP_SM(proobject_t *const mobj, event_t const *const e){
    switch(e->sig){
        case ENTRY:
        {
            buzzer_notification();
            lcd_mode_init(&mobj->dlcd,STOP_LCD);
            return EVENT_HANDLED;
        }
        case EXIT:
        {
            return EVENT_HANDLED;
        }
        case BACK_SIG:
        {
            mobj->active_state = MENU_SM;
            return EVENT_TRANSITION;
        }  
    }
    return EVENT_IGNORED;
}



static void dispatch_signals(event_t const * const e){
    switch(e->sig){
        case ENTRY:
            serial_sprintln_char("ENTRY_SIG");
        break;
        case EXIT:
            serial_sprintln_char("EXIT_SIG");
        break;
        case BACK_SIG:
            serial_sprintln_char("BACK_SIG");
        break;
        case SETTING_SIG:
            serial_sprintln_char("SETTING_SIG");
        break;
        case CW_SIG:
            serial_sprintln_char("CW_SIG");
        break;
        case CCW_SIG:
            serial_sprintln_char("CCW_SIG");
        break;
        // case TIME_TICK_SIG:
        //     serial_sprintln_char("TIME_TICK_SIG");
        // break;
        case SPRINT_DATA_SIG:
            serial_sprintln_char("SPRINT_DATA_SIG");
        break;
        case DISPLAY_DATA_SIG:
            serial_sprintln_char("DISPLAY_DATA_SIG");
        break;
        case CALIB_HX711_SIG:
            serial_sprintln_char("CALIB_HX711_SIG");
        break;
        case POSITIVE_PROPELLER_SIG:
            serial_sprintln_char("POSITIVE_PROPELLER_SIG");
        break;
        case NEGATIVE_PROPELLER_SIG:
            serial_sprintln_char("NEGATIVE_PROPELLER_SIG");
        break;
        case SET_SERIAL_BAUDRATE_SIG:
            serial_sprintln_char("SET_SERIAL_BAUDRATE_SIG");
        break;
    }
    serial_sprintln_char("-----------------\n");
}

static void sprint_signal(proobject_t *const mobj, event_t const * const e){
    if(e->sig != TIME_TICK_SIG){
        switch(mobj->active_state){
            case MENU_SM:
                serial_sprintln_char("MENU_SM\n");
                serial_sprint_char("MENU_");

            break;
            case SPRINT_DATA_SM:
                serial_sprintln_char("SPRINT_DATA_SM\n");
                serial_sprint_char("SPRINT_DATA_");
            break;
            case DISPLAY_DATA_SM:
                serial_sprintln_char("DISPLAY_DATA_SM\n");
                serial_sprint_char("DISPLAY_DATA_");
            break;
            case CALIB_HX711_SM:
                serial_sprintln_char("CALIB_HX711_SM\n");
                serial_sprint_char("CALIB_HX711_");
            break;
            case PUT_STANDARD_MASS_SM:
                serial_sprintln_char("PUT_STANDARD_MASS_SM\n");
                serial_sprint_char("SET_STANDARD_MASS_");
            break;
            case SAVE_LOAD_CELL_CALIBRATION_DATA_SM:
                serial_sprintln_char("SAVE_LOAD_CELL_CALIBRATION_DATA_SM\n");
                serial_sprint_char("SAVE_LOAD_CELL_CALIBRATION_DATA_");
            break;
            case POSITIVE_PROPELLER_SM:
                serial_sprintln_char("POSITIVE_PROPELLER_SM\n");
                serial_sprint_char("POSITIVE_PROPELLER_");
            break;
            case NEGATIVE_PROPELLER_SM:
                serial_sprintln_char("NEGATIVE_PROPELLER_SM\n");
                serial_sprint_char("NEGATIVE_PROPELLER_");
            break;
            case SET_SERIAL_BAUDRATE_SM:
                serial_sprintln_char("SET_SERIAL_BAUDRATE_SM\n");
                serial_sprint_char("SET_SERIAL_BAUDRATE_");
            break;
            case STOP_SM:
                serial_sprintln_char("STOP_SM\n");
                serial_sprint_char("STOP_");
            break;
            default:
                break;
        }
        dispatch_signals(e);
    } 
}