#include "Adafruit_BMP280.h"
#include "mybmp280.h"

static Adafruit_BMP280 bmp; // use I2C interface
static Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
static Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();


void my_bmp280_init(my_bmp280_t *my_bmp280){
    my_bmp280->pressure = 0.f;
    my_bmp280->temp = 0.f;
    Serial.println(F("BMP280 Sensor event test"));
    unsigned status;
    status = bmp.begin(); 
    if (!status) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                          "try a different address!"));
        Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    bmp_temp->printSensorDetails();
}


void my_bmp280_get_data(my_bmp280_t *my_bmp280){
    bmp_temp->getEvent(&my_bmp280->temp_event);
    bmp_pressure->getEvent(&my_bmp280->pressure_event);
    my_bmp280->temp = my_bmp280->temp_event.temperature;
    my_bmp280->pressure = my_bmp280->pressure_event.pressure;
}

