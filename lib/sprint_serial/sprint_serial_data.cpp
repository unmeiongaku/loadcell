#include "sprint_serial_data.h"
#include "Arduino.h"
#include "lcd.h"
#include "LiquidCrystal_I2C.h"
#include "mybmp280.h"

void set_baud_rate(my_sprint_t *sprint){
    switch(sprint->select_baudrate_number){
        case 0:
        {
            sprint->baudrate = 4800;
        }
            break;
        case 1:
        {
            sprint->baudrate = 9600;
        }
            break;
        case 2:
        {
            sprint->baudrate = 14400;
        }
            break;
        case 3:
        {
            sprint->baudrate = 38400;
        }
            break;
        case 4:
        {
            sprint->baudrate = 56000;
        }
            break;
        case 5:
        {
            sprint->baudrate = 57600;
        }
            break;
        case 6:
        {
            sprint->baudrate = 115200;
        }
            break;
        default:
            break;
    }
}

void serial_sprint_char(const char * c){
    Serial.print(c); 
}

void serial_sprintln_char(const char * c){
    Serial.println(c); 
}

void serial_sprint_data_init(my_sprint_t *sprint){
    sprint->select_baudrate_number = DEFAULT_SERIAL_SPRINT_NUMBER;
    sprint->max_baudrate_number = LCD_SET_SERIAL_BAUDRATE-1;
    set_baud_rate(sprint);
    Serial.begin(sprint->baudrate);
    Serial.println("Init Success\n");
    delay(10);
}

void serial_sprint_deinit(){
    Serial.end();
}


void serial_increase(my_sprint_t *sprint){
    if(sprint->select_baudrate_number == sprint->max_baudrate_number){
        sprint->select_baudrate_number = 0; 
    }
    else{
       sprint->select_baudrate_number++; 
    } 
}

void serial_decrease(my_sprint_t *sprint){
    if(sprint->select_baudrate_number == 0){
        sprint->select_baudrate_number = sprint->max_baudrate_number; 
    }
    else{
       sprint->select_baudrate_number--; 
    } 
}

void serial_sprint_reinit(my_sprint_t *sprint){
    set_baud_rate(sprint);
    Serial.begin(sprint->baudrate);
    delay(10);
}

void serial_sprint_data(my_sprint_t sprint){
    if(sprint.sprint_enable.getcurrent == true) Serial.println(sprint.sprint.current);
    Serial.print(" ");
    if(sprint.sprint_enable.getpressure == true) Serial.println(sprint.sprint.pressure);
    Serial.print(" ");
    if(sprint.sprint_enable.getrads == true) Serial.println(sprint.sprint.rads);
    Serial.print(" ");
    if(sprint.sprint_enable.getrpm == true) Serial.println(sprint.sprint.rpm);
    Serial.print(" ");
    if(sprint.sprint_enable.gettemp == true) Serial.println(sprint.sprint.temp);
    Serial.print(" ");
    if(sprint.sprint_enable.gettemp_of_motors == true) Serial.println(sprint.sprint.temp_of_motors);
    Serial.print(" ");
    if(sprint.sprint_enable.getvoltage == true) Serial.println(sprint.sprint.voltage);
    Serial.print(" ");
    if(sprint.sprint_enable.getweight == true) Serial.println(sprint.sprint.weight);
    Serial.print(" ");
}   

