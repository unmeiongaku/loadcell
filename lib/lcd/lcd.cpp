#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "lcd.h"
#include "user_define.h"
#include "stdlib.h"
#include "active_object.h"
#include "sprint_serial_data.h"
#include "mybmp280.h"
#include "myhx711.h"
#include "EEPROM.h"
#include "myeeprom.h"

static bool IsSprintFirstInit = true;
static bool IsDisplayFirstInit = true;

LiquidCrystal_I2C lcdglobal(LCD_I2C_ADDR, LCD_COL, LCD_ROW);

static uint8_t lcd_menu_handle(lcd_t *lcd);
static uint8_t lcd_sprint_handle(lcd_t *lcd);
static uint8_t lcd_display_handle(lcd_t *lcd);
static uint8_t lcd_calib_hx711_handle(lcd_t *lcd);
static uint8_t lcd_searching_calibration_loadcell_handle(lcd_t *lcd);
static uint8_t lcd_possitive_propller_handle(lcd_t *lcd);
static uint8_t lcd_save_calib_data_handle(lcd_t *lcd);
static uint8_t lcd_set_serial_baudrate_handle(lcd_t *lcd);


/*Display data*/
static void display_data_pos_propeller_callback(lcd_t lcd);
static void display_data_neg_propeller_callback(lcd_t lcd);


void lcd_init(){
    lcdglobal.begin();
    lcdglobal.backlight();
    lcdglobal.setCursor(3,0);
    lcdglobal.print("INITIALIZATION");
    lcdglobal.setCursor(1,1);
    lcdglobal.print("WAIT FOR A MOMENT!");
}

void lcd_clear(){
    lcdglobal.clear();
}

void lcd_set_cursor(uint8_t col, uint8_t row){
    lcdglobal.setCursor(col,row);
}

uint8_t lcd_display(lcd_t *lcd){
    switch(lcd->lcd_mode){
    case MENU_LCD:
    {
        lcd_menu_handle(lcd);
    }
        break;
    case SPRINT_DATA_LCD:
    {
        lcd_sprint_handle(lcd);
    }
        break;
    case DISPLAY_DATA_LCD:
    {
        lcd_display_handle(lcd);
    }
        break;
    case CALIB_HX711_LCD:
    {
        lcd_calib_hx711_handle(lcd);
    }
        break;
    case SEARCHING_CALIBRATION_LOADCELL_LCD:
    {
        lcd_searching_calibration_loadcell_handle(lcd);
    }
        break;
    case SAVE_CALIB_DATA_LCD:
    {
        lcd_save_calib_data_handle(lcd);
    }
        break;
    case SET_SERIAL_BAUDRATE_LCD:
    {
        lcd_set_serial_baudrate_handle(lcd);
    }
        break;
    case POSSITIVE_PROPELLER_LCD:
    {
        lcd_possitive_propller_handle(lcd);
    }
        break;
    case NEGATIVE_PROPELLER_LCD:
    {
        
    }
        break;
    default:
        break;
    }  
    return 0;  
}

uint16_t dispatch_internal_signal_sprint_display(lcd_t *lcd){
    uint16_t signal;
    if(lcd->lcd_mode == MENU_LCD){
        if(lcd->select == 0){
            signal = SPINT_DATA_SIG_DEFINE;
        }
        else if(lcd->select == 1){
            signal = DISPLAY_DATA_SIG_DEFINE;
        }
        else if(lcd->select == 2){
            signal = CALIB_HX711_SIG_DEFINE;
        }
        else if(lcd->select == 3){
            signal = POSITIVE_PROPELLER_SIG_DEFINE; 
        }
        else if(lcd->select == 4){
            signal = NEGATIVE_PROPELLER_SIG_DEFINE;
        }
        else if(lcd->select == 5){
            signal = SET_SERIAL_BAUDRATE_SIG_DEFINE;
        }
        lcd->signal = signal;
        Serial.print("SIGNAL NUMBER: ");
        Serial.println(lcd->signal-1);
    }
    else if(lcd->lcd_mode == SPRINT_DATA_LCD || lcd->lcd_mode == DISPLAY_DATA_LCD){
        if(lcd->select == 0){
            signal = WEIGHT_DATA_DEFINE;
        }
        else if(lcd->select == 1){
            signal = RPM_DATA_DEFINE;
        }
        else if(lcd->select == 2){
            signal = RADS_DATA_DEFINE;
        }
        else if(lcd->select == 3){
            signal = CURRENT_DATA_DEFINE; 
        }
        else if(lcd->select == 4){
            signal = VOLTAGE_DATA_DEFINE;
        }
        else if(lcd->select == 5){
            signal = TEMP_DATA_DEFINE;
        }
        else if(lcd->select == 6){
            signal = TEMP_MOTOR_DATA_DEFINE;
        }
        else if(lcd->select == 7){
            signal = PRESSURE_MOTOR_DATA_DEFINE;
        }
        lcd->signal = signal;
    }
    else if(lcd->lcd_mode ==POSSITIVE_PROPELLER_LCD){
        if(lcd->select == 0){
            signal = POSSITIVE_PROPLLER_LIST_1;
        }
        else if(lcd->select == 1){
            signal = POSSITIVE_PROPLLER_LIST_2;
        }
        lcd->signal = signal;
    }
    else if(lcd->lcd_mode == NEGATIVE_PROPELLER_LCD){
        if(lcd->select == 0){
            signal = POSSITIVE_PROPLLER_LIST_1;
        }
        else if(lcd->select == 1){
            signal = POSSITIVE_PROPLLER_LIST_2;
        }
        lcd->signal = signal;
    }
    else{
        signal = 0;
    }

    return signal;
}

uint16_t lcd_increase_select(lcd_t *lcd){
    if(lcd->select == lcd->maxselect){
        lcd->select = 0;
    }
    else{
        lcd->select++;
    }
    return dispatch_internal_signal_sprint_display(lcd);
}

uint16_t lcd_decrease_select(lcd_t *lcd){
    if(lcd->select == 0){
        lcd->select = lcd->maxselect;
    }
    else{
        lcd->select--;
    }
    return dispatch_internal_signal_sprint_display(lcd);
}


uint8_t lcd_mode_init(lcd_t *lcd,lcd_mode_t lcd_mode){
    lcd_clear();
    switch(lcd_mode){
        case MENU_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->maxselect = LCD_MENU_LIST_DEFINE-1;
            lcd->select = 0;
            lcd->isFistdisplay = true;
            lcd->page = 0;
            lcd->arrow = 0;
            lcd->signal = SPINT_DATA_SIG_DEFINE;
        }
            break;
        case SPRINT_DATA_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->maxselect = LCD_SPRINT_LIST_DEFINE-1;
            lcd->select = 0;
            lcd->page = 0;
            lcd->arrow = 0;
        }
            break;
        case DISPLAY_DATA_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->maxselect = LCD_DISPLAY_LIST_DEFINE-1;
            lcd->select = 0;
            lcd->page = 0;
            lcd->arrow = 0;
        }
            break;
        case CALIB_HX711_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->page = 0;
            lcd->arrow = 0;
            lcd->select = 0;
        }
            break;
        case SEARCHING_CALIBRATION_LOADCELL_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->page = 0;
            lcd->arrow = 0;
            lcd->select = 0;
        }
            break;
        case SAVE_CALIB_DATA_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->page = 0;
            lcd->arrow = 0;
            lcd->select = 0;
        }
            break;
        case POSSITIVE_PROPELLER_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->page = 0;
            lcd->arrow = 0;
            lcd->select = 0;
            lcd->maxselect = LCD_POSITIVE_PROPELLER_LIST_DEFINE-1;
        }
            break;
        case NEGATIVE_PROPELLER_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->page = 0;
            lcd->arrow = 0;
            lcd->select = 0;
            lcd->maxselect = LCD_POSITIVE_PROPELLER_LIST_DEFINE-1;
        }
            break;
        case SET_SERIAL_BAUDRATE_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->maxselect = LCD_SET_SERIAL_BAUDRATE-1;
            lcd->select = 0;
            lcd->page = 0;
            lcd->arrow = 0;
        }
            break;
        case STOP_LCD:
        {
            lcd->lcd_mode = lcd_mode;
            lcd->page = 0;
            lcd->arrow = 0;
        }
            break;
        default:
            break;
    }
    return 0;
}


void display_data(lcd_t lcd){
    switch(lcd.lcd_mode){
        case POSSITIVE_PROPELLER_LCD:
        {
            display_data_pos_propeller_callback(lcd);
        }
            break;
        case NEGATIVE_PROPELLER_LCD:
        {
            display_data_neg_propeller_callback(lcd);
        }
            break; 
        default:
            break;
    }
}

void dispatch_arrow(lcd_t *lcd) {
    for(uint8_t i = 0; i<4;i++){
        lcdglobal.setCursor(19, i);
        if(i==lcd->arrow){
            lcdglobal.print((char)200);  // Hiển thị mũi tên
        }
        else{
            lcdglobal.setCursor(19, i);
            lcdglobal.print(" ");  // Hiển thị mũi tên
        }
    }
}

static uint8_t lcd_menu_handle(lcd_t *lcd){
    if(lcd->signal == SPINT_DATA_SIG_DEFINE){
        lcd->arrow = 0;
        lcd->page = 0;
    }
    else if(lcd->signal == DISPLAY_DATA_SIG_DEFINE){
        lcd->arrow = 1;
        lcd->page = 0;
    }
    else if(lcd->signal == CALIB_HX711_SIG_DEFINE){
        lcd->arrow = 2;
        lcd->page = 0;
    }
    else if(lcd->signal == POSITIVE_PROPELLER_SIG_DEFINE){
        lcd->arrow = 3;
        lcd->page = 0;
    }
    else if(lcd->signal == NEGATIVE_PROPELLER_SIG_DEFINE){
        lcd->arrow = 0;
        lcd->page = 1;
    }
    else if(lcd->signal == SET_SERIAL_BAUDRATE_SIG_DEFINE){
        lcd->arrow = 1;
        lcd->page = 1;
    }
    if(lcd->page == 0){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("PRINT DATA");      // select = 0
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("DISPLAY DATA");   // select = 1
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("LOADCELL CALIB"); // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("posPROPELLERS");  // select = 3
    }
    else if(lcd->page==1){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("negPROPELLERS");  // select = 4
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("BAUDRATE");      // select = 5
    }
    dispatch_arrow(lcd);
    return 0;
}

uint8_t change_enable_or_disable_sprint(lcd_t *lcd,uint16_t arrow){
    switch(arrow){
        case WEIGHT_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.getweight == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.getweight = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.getweight = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.getweight == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.getweight = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.getweight = true;
                    }
                }
            }
        break;
        case RPM_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.getrpm == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.getrpm = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.getrpm = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.getrpm == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.getrpm = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.getrpm = true;
                    }
                }
            }
        break;
        case RADS_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.getrads == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.getrads = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.getrads = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.getrads == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.getrads = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.getrads = true;
                    }
                }
            }
        break;
        case CURRENT_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.getcurrent == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.getcurrent = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.getcurrent = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.getcurrent == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.getcurrent = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.getcurrent = true;
                    }
                }
            }
        break;
        case VOLTAGE_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.getvoltage == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.getvoltage = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.getvoltage = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.getvoltage == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.getvoltage = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.getvoltage = true;
                    }
                }
            }
        break;
        case TEMP_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.gettemp == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.gettemp = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.gettemp = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.gettemp == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.gettemp = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.gettemp = true;
                    }
                }
            }
        break;
        case TEMP_MOTOR_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.gettemp_of_motors == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.gettemp_of_motors = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.gettemp_of_motors = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.gettemp_of_motors == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.gettemp_of_motors = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.gettemp_of_motors = true;
                    }
                }
            }
        break;
        case PRESSURE_MOTOR_DATA_DEFINE:
            {
                lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                lcdglobal.print("       ");
                if(lcd->lcd_mode == SPRINT_DATA_LCD){
                    if(lcd->sprintdata.getpressure == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Disable");
                        lcd->sprintdata.getpressure = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Enable");
                        lcd->sprintdata.getpressure = true;
                    }
                }
                else if(lcd->lcd_mode == DISPLAY_DATA_LCD){
                    if(lcd->displaydata.getpressure == true){
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Disable");
                        lcd->displaydata.getpressure = false;
                    }
                    else{
                        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
                        lcdglobal.print("Enable");
                        lcd->displaydata.getpressure = true;
                    }
                }
            }
        break;
    }
    return 0;
}


static uint8_t lcd_sprint_handle(lcd_t *lcd){
    if(lcd->signal == WEIGHT_DATA_DEFINE){
        lcd->arrow = 0;
        lcd->page = 0;
    }
    else if(lcd->signal == RPM_DATA_DEFINE){
        lcd->arrow = 1;
        lcd->page = 0;
    }
    else if(lcd->signal == RADS_DATA_DEFINE){
        lcd->arrow = 2;
        lcd->page = 0;
    }
    else if(lcd->signal == CURRENT_DATA_DEFINE){
        lcd->arrow = 3;
        lcd->page = 0;
    }
    else if(lcd->signal == VOLTAGE_DATA_DEFINE){
        lcd->arrow = 0;
        lcd->page = 1;
    }
    else if(lcd->signal == TEMP_DATA_DEFINE){
        lcd->arrow = 1;
        lcd->page = 1;
    }
    else if(lcd->signal == TEMP_MOTOR_DATA_DEFINE){
        lcd->arrow = 2;
        lcd->page = 1;
    }
    else if(lcd->signal == PRESSURE_MOTOR_DATA_DEFINE){
        lcd->arrow = 3;
        lcd->page = 1;
    }
    if(lcd->page == 0){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("WEIGHT");      // select = 0
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("RPM");   // select = 1
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("RADS"); // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("CUR");  // select = 3



        /*ENABLE - DISABLE*/
        if(lcd->sprintdata.getweight == true){
            lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
            lcdglobal.print("Enable");
        }
        else{
            lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
            lcdglobal.print("Disable");
        }

        if(lcd->sprintdata.getrpm == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Disable");
        }

        if(lcd->sprintdata.getrads == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Disable");
        }

        if(lcd->sprintdata.getcurrent == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Disable");
        }
    }
    else if(lcd->page==1){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("VOL");  // select = 4
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("TEMP");      // select = 5
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("mTEMP"); // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("PRES");  // select = 3

        /*ENABLE - DISABLE*/
        if(lcd->sprintdata.getvoltage == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
        lcdglobal.print("Disable");
        }

        if(lcd->sprintdata.gettemp == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Disable");
        }

        if(lcd->sprintdata.gettemp_of_motors == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Disable");
        }

        if(lcd->sprintdata.getpressure == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Disable");
        }
    }
    dispatch_arrow(lcd);
    return 0;
}

static uint8_t lcd_display_handle(lcd_t *lcd){
    if(lcd->signal == WEIGHT_DATA_DEFINE){
        lcd->arrow = 0;
        lcd->page = 0;
    }
    else if(lcd->signal == RPM_DATA_DEFINE){
        lcd->arrow = 1;
        lcd->page = 0;
    }
    else if(lcd->signal == RADS_DATA_DEFINE){
        lcd->arrow = 2;
        lcd->page = 0;
    }
    else if(lcd->signal == CURRENT_DATA_DEFINE){
        lcd->arrow = 3;
        lcd->page = 0;
    }
    else if(lcd->signal == VOLTAGE_DATA_DEFINE){
        lcd->arrow = 0;
        lcd->page = 1;
    }
    else if(lcd->signal == TEMP_DATA_DEFINE){
        lcd->arrow = 1;
        lcd->page = 1;
    }
    else if(lcd->signal == TEMP_MOTOR_DATA_DEFINE){
        lcd->arrow = 2;
        lcd->page = 1;
    }
    else if(lcd->signal == PRESSURE_MOTOR_DATA_DEFINE){
        lcd->arrow = 3;
        lcd->page = 1;
    }
    if(lcd->page == 0){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("WEIGHT");      // select = 0
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("RPM");   // select = 1
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("RADS"); // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("CUR");  // select = 3

        /*ENABLE - DISABLE*/
        if(lcd->displaydata.getweight == true){
            lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
            lcdglobal.print("Enable");
        }
        else{
            lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
            lcdglobal.print("Disable");
        }

        if(lcd->displaydata.getrpm == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Disable");
        }

        if(lcd->displaydata.getrads == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Disable");
        }

        if(lcd->displaydata.getcurrent == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Disable");
        }
    }
    else if(lcd->page==1){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("VOL");  // select = 4
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("TEMP");      // select = 5
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("mTEMP"); // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("PRES");  // select = 3

        /*ENABLE - DISABLE*/
        if(lcd->displaydata.getvoltage == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 0); 
        lcdglobal.print("Disable");
        }

        if(lcd->displaydata.gettemp == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 1); 
        lcdglobal.print("Disable");
        }

        if(lcd->displaydata.gettemp_of_motors == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 2); 
        lcdglobal.print("Disable");
        }

        if(lcd->displaydata.getpressure == true){
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Enable");
        }
        else{
        lcdglobal.setCursor(DATA_ENA_DISABLE_ROW, 3); 
        lcdglobal.print("Disable");
        }
    }
    dispatch_arrow(lcd);
    return 0;
}

static uint8_t lcd_calib_hx711_handle(lcd_t *lcd){
    switch(lcd->select){
        case 0:
        {
            lcd_clear();
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Calibrating Loadcell");
            lcdglobal.setCursor(0, 1); 
            lcdglobal.print("Please Open Hercules");
            lcdglobal.setCursor(0, 2); 
            lcdglobal.print("Baudrate:");
            lcdglobal.setCursor(10, 2); 
            lcdglobal.print(lcd->baudrate);
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("BACK");
            lcdglobal.setCursor(6, 3); 
            lcdglobal.print("NEXT");
            lcdglobal.setCursor(11, 3); 
            lcdglobal.print("SRCH/CHCK");
        }
        break;
        case 1:
        {
            lcd_clear();
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Loadcell Had Been");
            lcdglobal.setCursor(0, 1); 
            lcdglobal.print("Calibrated. Wanna");
            lcdglobal.setCursor(0, 2); 
            lcdglobal.print("Calibrate Again? ");
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("NO");
            lcdglobal.setCursor(8, 3); 
            lcdglobal.print("YES");
            lcdglobal.setCursor(14, 3); 
            lcdglobal.print("CW/CCW");
        }
        break;
        case 2:
        {
            lcd_clear();
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Lastest Value Is:");
            lcdglobal.setCursor(6, 1); 
            lcdglobal.print(lcd->loadcell_calibration);
            lcdglobal.setCursor(2, 2); 
            lcdglobal.print("Calibrate Again ?");
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("No");
            lcdglobal.setCursor(16, 3); 
            lcdglobal.print("Yes");
        }
        break;
        case 3:
        {
            lcd_clear();
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Calibrating...");
            lcdglobal.setCursor(0, 1); 
            lcdglobal.print("Following step");
            lcdglobal.setCursor(0, 2); 
            lcdglobal.print("On the Terminal");
        }
        break;
    }
    return 0;
}

void display_saving_calibration_data_to_eeprom(){
        lcdglobal.setCursor(0, 3); 
        lcdglobal.print("BACK");
        lcdglobal.setCursor(6, 3); 
        lcdglobal.print("SAVING EEPROM");
}

static uint8_t lcd_searching_calibration_loadcell_handle(lcd_t *lcd){
    lcd_clear();
    switch(lcd->select){
        case 0:
        {
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("SEARCHING..."); 
        }
            break;
        case 1:
        {
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Can't Found A"); 
            lcdglobal.setCursor(0, 1); 
            lcdglobal.print("Valid Data, Please"); 
            lcdglobal.setCursor(0, 2); 
            lcdglobal.print("Calibration Again"); 
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("Menu"); 
            lcdglobal.setCursor(10, 3); 
            lcdglobal.print("Calibrate"); 
        }
            break;
        case 2:
        {
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Lastest Value Is:");
            lcdglobal.setCursor(6, 1); 
            lcdglobal.print(lcd->loadcell_calibration);
            lcdglobal.setCursor(2, 2); 
            lcdglobal.print("Calibrate Again ?");
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("No");
            lcdglobal.setCursor(16, 3); 
            lcdglobal.print("Yes");
        }
            break;
    }
}

static uint8_t lcd_save_calib_data_handle(lcd_t *lcd){
    lcd_clear();
    switch(lcd->select){
        case 0:
        {
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Saving...");
        }
            break;
        case 1:
        {
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Data Had Been Saved");
            lcdglobal.setCursor(6, 1); 
            lcdglobal.print(lcd->saving_data);
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("BACK");
        }
            break;
        case 2:
        {
            lcdglobal.setCursor(0, 0); 
            lcdglobal.print("Saving Failt");
            lcdglobal.setCursor(6, 1); 
            lcdglobal.print(lcd->saving_data);
            lcdglobal.setCursor(0, 3); 
            lcdglobal.print("BACK");
            lcdglobal.setCursor(10, 3); 
            lcdglobal.print("TRY AGAIN");
        }
            break;
    }
}


static uint8_t lcd_possitive_propller_handle(lcd_t *lcd){
    if(lcd->signal == POSSITIVE_PROPLLER_LIST_1){
        lcd->page = 0;
    }
    else if(lcd->signal == POSSITIVE_PROPLLER_LIST_2){
        lcd->page = 1;
    }
    if(lcd->page == 0){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("WEIGHT:");      // select = 0
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("ROLL/MIN:");   // select = 1
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("RAD/SEC:"); // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("CURRENT:");  // select = 3
    }
    else if(lcd->page == 1){
        lcd_clear();
        lcdglobal.setCursor(0, 0);
        lcdglobal.print("VOLTAGE:");         // select = 4
        lcdglobal.setCursor(0, 1);
        lcdglobal.print("TEMPERATURE:");        // select = 5
        lcdglobal.setCursor(0, 2);
        lcdglobal.print("mTEMPERATURE:");       // select = 2
        lcdglobal.setCursor(0, 3);
        lcdglobal.print("PRESSURE:");        // select = 3
    }
}

static uint8_t lcd_set_serial_baudrate_handle(lcd_t *lcd){
    return 0;
}


static void display_data_pos_propeller_callback(lcd_t lcd){
    if(lcd.page == 0){
        if(lcd.displaydata.getweight==true){
            lcdglobal.setCursor(11, 0);
            lcdglobal.print(lcd.display.weight);
        }
        if(lcd.displaydata.getrpm==true){
            lcdglobal.setCursor(11, 1);
            lcdglobal.print(lcd.display.rpm);
        }
        if(lcd.displaydata.getrads==true){
            lcdglobal.setCursor(11, 2);
            lcdglobal.print(lcd.display.rads);
        }
        if(lcd.displaydata.getcurrent==true){
            
        }
    }
    else if(lcd.page == 1){
        if(lcd.displaydata.gettemp==true){
            lcdglobal.setCursor(14, 1);
            lcdglobal.print(lcd.display.temp);
        }
        if(lcd.displaydata.getpressure==true){
            lcdglobal.setCursor(14, 3);
            lcdglobal.print(lcd.display.pressure);
        }
        if(lcd.displaydata.getvoltage==true){
            
        }
        if(lcd.displaydata.gettemp_of_motors==true){
            
        }
    
    }
}

static void display_data_neg_propeller_callback(lcd_t lcd){
    switch(lcd.data_list_select){
        case 0:
        {  
            if(lcd.displaydata.getrads == true){

            }
            else{
                
            }
            if(lcd.displaydata.getrpm == true){

            }
            else{
                
            }
            if(lcd.displaydata.getvoltage == true){

            }
            else{
                
            }
            if(lcd.displaydata.getweight == true){

            }
            else{
                
            }
        }
        break;
        case 1:
        {
            if(lcd.displaydata.getpressure == true){

            }
            else{
                
            }
            if(lcd.displaydata.gettemp == true){

            }
            else{
                
            }
            if(lcd.displaydata.gettemp_of_motors == true){

            }
            else{
                
            }
            if(lcd.displaydata.getcurrent == true){

            }
            else{

            }
        }
        break;
        default:
            break;
    }
}

