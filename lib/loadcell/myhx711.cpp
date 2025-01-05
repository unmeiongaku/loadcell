#include "myhx711.h"
#include "HX711_ADC.h"
#include "stdint.h"
#include "user_define.h"
#include "lcd.h"
#include "LiquidCrystal_I2C.h"

//pins:
const int HX711_dout = HX711_DOUT_DEFINE; //mcu > HX711 dout pin
const int HX711_sck = HX711_SCK_DEFINE; //mcu > HX711 sck pin

static hx711_global_t hx711_global;

static HX711_ADC LoadCell(HX711_dout,HX711_sck);
static HX711_ADC CalibLoadCell(HX711_dout,HX711_sck);


int8_t hx711_init(hx711_global_t *hx711_global){
    LoadCell.begin();
    //hx711_global->calibration_data = 0.0f;
    hx711_global->newDataReady = 0;
    hx711_global->stabilizingtime = 2000;
    hx711_global->_tare = true;
    LoadCell.start(hx711_global->stabilizingtime, hx711_global->_tare);
    if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    }
    else {
    LoadCell.setCalFactor(hx711_global->calibration_data); // set calibration factor (float)
    Serial.println("Startup is complete");
    }
    while (!LoadCell.update());
    Serial.print("Calibration value: ");
    Serial.println(LoadCell.getCalFactor());
    Serial.print("HX711 measured conversion time ms: ");
    Serial.println(LoadCell.getConversionTime());
    Serial.print("HX711 measured sampling rate HZ: ");
    Serial.println(LoadCell.getSPS());
    Serial.print("HX711 measured settlingtime ms: ");
    Serial.println(LoadCell.getSettlingTime());
    Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
    if (LoadCell.getSPS() < 7) {
        Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
    }
    else if (LoadCell.getSPS() > 100) {
        Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
    }
    return 0;
}

void save_calibration_data_to_global_loadcell(hx711_global_t *hx711_global,hx711_global_t hx711_calib){
        hx711_global->calibration_data = hx711_calib.calibration_data;
}

float hx711_get_weight(hx711_global_t *hx711_global){
    // check for new data/start next conversion:
    if (LoadCell.update()) hx711_global->newDataReady = true;
    // get smoothed value from the dataset:
    if (hx711_global->newDataReady == true) {
        hx711_global->weight = LoadCell.getData();
        hx711_global->newDataReady = false;
    }
    return hx711_global->weight;
}


float hx711_calib_init(hx711_global_t *hx711_calib){
    hx711_calib->isCalibrationDone = false;
    CalibLoadCell.begin();
    hx711_calib->calibration_data = 0.0f;
    hx711_calib->_tare = true;
    LoadCell.start(hx711_calib->stabilizingtime, hx711_calib->_tare);
    if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
    }
    else {
        LoadCell.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
        Serial.println("Startup is complete");
    }
    while (!LoadCell.update());
    return hx711_calibration(hx711_calib); 
}

float hx711_calibration(hx711_global_t *hx711_calib){
    Serial.println("***");
    Serial.println("Start calibration:");
    Serial.println("Place the load cell an a level stable surface.");
    Serial.println("Remove any load applied to the load cell.");
    Serial.println("Send 't' from serial monitor to set the tare offset."); 
    boolean _resume = false;
    while (_resume == false) {
        LoadCell.update();
        if (Serial.available() > 0) {
        if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte == 't') LoadCell.tareNoDelay();
        }
        }
        if (LoadCell.getTareStatus() == true) {
        Serial.println("Tare complete");
        _resume = true;
        }
    }
    Serial.println("Now, place your known mass on the loadcell.");
    Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");


    float known_mass = 0;
    _resume = false;
    while (_resume == false) {
        LoadCell.update();
        if (Serial.available() > 0) {
        known_mass = Serial.parseFloat();
        if (known_mass != 0) {
            Serial.print("Known mass is: ");
            Serial.println(known_mass);
            _resume = true;
        }
        }
    }

    LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
    float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

    hx711_calib->calibration_data = newCalibrationValue;

    Serial.print("New calibration value has been set to: ");
    Serial.print(newCalibrationValue);
    Serial.println(", use this as calibration value (calFactor) in your project sketch.");
    hx711_calib->isCalibrationDone = true;
    display_saving_calibration_data_to_eeprom();
    return newCalibrationValue;
}
