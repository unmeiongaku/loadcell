#ifndef USER_STRUCT_H
#define USER_STRUCT_H

#include "Arduino.h"

#define ARDUINO_DEBUG 1


#define LED_LOADCELL_SM_NOTIFICATION LED_BUILTIN


#define LED_TICK_CALLBACK                   4
#define LED_RPM_INTERRUPT                   5
#define LED_SERIAL_PRINT                    6

#define ENABLE_TICK_LED                     0

/*DEINFE RPM INTERRUPT*/
#define RPM_INTERRUPT_GPIO                  3       

/*define Button*/
#define BUTTON_SETTING_SIG                    46                                              
#define BUTTON_BACK_SIG                       50
#define BUTTON_INCREASE_SIG                   26                                              
#define BUTTON_DECREASE_SIG                   30

/*define Buzzer*/   
#define BUZZER_GPIO                           2        

/*Define Your LCD */
#define LCD_I2C_SELECT     1
/*    0x27                 0
*     0x3F                 1 
*/

#if LCD_I2C_SELECT == 0
#define LCD_I2C_ADDR      0x27
#elif LCD_I2C_SELECT == 1
#define LCD_I2C_ADDR      0x3F
#endif

#define LCD_TYPE       0
/*    2004                 0
*     1602                 1 
*/

#if LCD_TYPE == 0
#define    LCD_COL     20
#define    LCD_ROW     4
#elif LCD_TYPE == 1
#define    LCD_COL     16
#define    LCD_ROW     2
#endif


/*define Your isr*/
//ESP worked with 80mhz
//prescaler 80 
//CYCLE = 80MHZ/80 = 1MHZ

#define ARDUINO_NANO_PRESCALAR       16

/*Select your ISR CYCLE*/
/*  1ms                     0
*   10ms                    1        
*   100ms                   2
*   1s                      3    
*/

#define TIME_TICK_CYCLE_DEFINE  100

#define SELECT_YOUR_ISR     0

#if  SELECT_YOUR_ISR == 0
#define ISR_CYCLE           1000
#define MS_TO_S             1000   
#elif   SELECT_YOUR_ISR == 1
#define ISR_CYCLE           10000
#define MS_TO_S             100 
#elif   SELECT_YOUR_ISR == 2
#define ISR_CYCLE           100000
#define MS_TO_S             10 
#elif   SELECT_YOUR_ISR == 3
#define ISR_CYCLE           1000000
#define MS_TO_S             1
#endif

/*hx711 define pins number*/
#define HX711_DOUT_DEFINE           8
#define HX711_SCK_DEFINE            7


/*LCD_LIST_DEFINE*/
#define LCD_MENU_LIST_DEFINE        6

/*SPRINT_DATA_LCD AND DISPLAY_DATA_LCD*/
/*LIST 1*/
    //WEIGHT
    //VOLTAGE
    //RPM
    //RAD/S
/*LIST 2*/
    //CURRENT
    //TEMP OF MOTOR
    //TEMP
    //PRESSURE
#define LCD_SPRINT_LIST_DEFINE       8
#define LCD_DISPLAY_LIST_DEFINE      8
#define LCD_POSITIVE_PROPELLER_LIST_DEFINE       2

/*SERIAL SPRINT BAUDRATE LIST*/
#define DEFAULT_SERIAL_SPRINT_NUMBER        5
//4800         0
//9600         1
//14400        2
//38400        3
//56000        4
//57600        5
//115200       6
#if DEFAULT_SERIAL_SPRINT_NUMBER == 0
#define DEFAULT_SERIAL_SPRINT        4800
#elif DEFAULT_SERIAL_SPRINT_NUMBER == 1
#define DEFAULT_SERIAL_SPRINT        9600
#elif DEFAULT_SERIAL_SPRINT_NUMBER == 2
#define DEFAULT_SERIAL_SPRINT        14400
#elif DEFAULT_SERIAL_SPRINT_NUMBER == 3
#define DEFAULT_SERIAL_SPRINT        38400
#elif DEFAULT_SERIAL_SPRINT_NUMBER == 4
#define DEFAULT_SERIAL_SPRINT        56000
#elif DEFAULT_SERIAL_SPRINT_NUMBER == 5
#define DEFAULT_SERIAL_SPRINT        57600
#elif DEFAULT_SERIAL_SPRINT_NUMBER == 6
#define DEFAULT_SERIAL_SPRINT        115200
#endif

#define LCD_SET_SERIAL_BAUDRATE      7

/*LCD_DISPLAY_DATA_LIST*/
#define LCD_DISPLAY_DATA_POS_PROPELLER_LIST      2
#define LCD_DISPLAY_DATA_NEG_PROPELLER_LIST      2



/*State Machine Signal Define*/
// total signals are saving in 16 bits uint16_t

//4bit for button signals ( buttonsig)
//BIT     3           2           1           0
//SIG     CCW_SIG     CW-SIG      BACK_SIG    SETTING_SIG

#define CCW_SIG_DEFINE             8
#define CW_SIG_DEFINE              4
#define BACK_SIG_DEFINE            2
#define SETTING_SIG_DEFINE         1                             



/*Define internal signal*/
//BIT     9                         8                       7                       6                       5                   4
//SIG     SET_SERIAL_BAUDRATE_SIG   NEGATIVE_PROPELLER_SIG  POSITIVE_PROPELLER_SIG CALIB_HX711_SIG          DISPLAY_DATA_SIG    SPINT_DATA_SIG

#define SET_SERIAL_BAUDRATE_SIG_DEFINE          512+SETTING_SIG_DEFINE   
#define NEGATIVE_PROPELLER_SIG_DEFINE           256+SETTING_SIG_DEFINE
#define POSITIVE_PROPELLER_SIG_DEFINE           128+SETTING_SIG_DEFINE
#define CALIB_HX711_SIG_DEFINE                  64+SETTING_SIG_DEFINE
#define DISPLAY_DATA_SIG_DEFINE                 32+SETTING_SIG_DEFINE    
#define SPINT_DATA_SIG_DEFINE                   16+SETTING_SIG_DEFINE  


/*SPINT & DISPLAY DATA */

#define DATA_ENA_DISABLE_ROW                9

#define WEIGHT_DATA_DEFINE                  0
#define RPM_DATA_DEFINE                     1
#define RADS_DATA_DEFINE                    2
#define CURRENT_DATA_DEFINE                 3
#define VOLTAGE_DATA_DEFINE                 4
#define TEMP_DATA_DEFINE                    5
#define TEMP_MOTOR_DATA_DEFINE              6
#define PRESSURE_MOTOR_DATA_DEFINE          7


/*POSSITIVE_PROPLLER_LIST*/
#define POSSITIVE_PROPLLER_LIST_1           0
#define POSSITIVE_PROPLLER_LIST_2           1

/*INTERRUPT RPM GPIO*/
#define RPM_INTERRUPT_DEFINE                1

/*TIME_TICK_SIG*/
#define TIME_TICK_SIG_DEFINE          1024

/*EEPROM*/
#define EEPROM_SAVING_WEIGHT_CALIBRATION_ADDR       50


#endif 