#include "motorrpm.h"
#include "Arduino.h"
#include "string.h"

float rpm = 0.0; // Tốc độ quay (RPM)
float rad = 0.0; // Tốc độ góc (Rad/s)

// Hàm xử lý chuỗi nhận được
void processUARTData(String data) {
    int commaIndex = data.indexOf(','); // Tìm vị trí dấu phẩy
    if (commaIndex > 0) {
        // Tách chuỗi thành 2 phần
        String rpmStr = data.substring(0, commaIndex);           // Trước dấu phẩy
        String radStr = data.substring(commaIndex + 1);  // Sau dấu phẩy, loại bỏ '\n'
        radStr.trim();

        // Chuyển chuỗi sang số thực
        rpm = rpmStr.toFloat();
        rad = radStr.toFloat();

        // Hiển thị giá trị nhận được (kiểm tra)
        Serial.print("RPM: ");
        Serial.println(rpm);
        Serial.print("Rad/s: ");
        Serial.println(rad);
    } else {
        Serial.println("Invalid data format!"); // Báo lỗi nếu chuỗi không hợp lệ
    }
}

void motor_serial_init(){
    Serial1.begin(57600); // UART với STM32
}

void get_rpm_rads(float *rpmglobal,float *radsglobal){
    if (Serial1.available()) {
        String receivedData = Serial1.readStringUntil('\n'); // Đọc chuỗi từ UART
        processUARTData(receivedData); // Xử lý chuỗi nhận được
    }
    *rpmglobal = rpm;
    *radsglobal = rad;
}
