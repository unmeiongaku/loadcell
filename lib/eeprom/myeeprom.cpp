#include "myeeprom.h"
#include "EEPROM.h"
#include "Arduino.h"


extern EEPROMClass EEPROM;

float convert_binary_to_float_32bit(uint8_t buffer[4]){
	int S_part;
	float Fraction_part = 1.0f;
	int Exponent_part = 0;
	float value_cal = 0.0f;
	//calculation S part
	S_part = (uint8_t)buffer[3]>>7;
	S_part = pow(-1,S_part);
	//calculation Fraction part
	int p,e;
	int multi_value;
	for(int i =0;i<23;i++){
	if(i<7){
		p = (uint8_t)(buffer[2]>>(6-i)) & 0x01;
			if(p==1){
				multi_value = 1;
			}
			else{
				multi_value = 0;
			}
	}
	else if(i>6 && i<15){
		p = (uint8_t)(buffer[1]>>(14-i)) & 0x01; // chua fixx
			if(p==1){
				multi_value = 1;
			}
			else{
				multi_value = 0;
			}
	}
	else{
		p = (uint8_t)(buffer[0]>>(22-i)) & 0x01;
			if(p==1){
				multi_value = 1;
			}
			else{
				multi_value = 0;
			}
	}
	Fraction_part = Fraction_part + (multi_value*(1/pow(2,i+1)));
	//printf("%.10f,%d,%.10f\n",Fraction_part,i);
	}
	//calculation Exponent part
	for(int i = 0;i<8;i++){
		if(i<7){
			e = (uint8_t)(buffer[3]>>(6-i)) & 0x01;
			if(e==1){
				multi_value = 1;
			}
			else{
				multi_value = 0;
			}
		}
		else{
			e = (uint8_t)(buffer[2]>>(14-i)) & 0x01;
			if(e==1){
				multi_value = 1;
			}
			else{
				multi_value = 0;
			}
		}
		Exponent_part = Exponent_part + multi_value*pow(2,7-i);
		//printf("%d,%d:\n",Exponent_part,i);
	}
	value_cal = S_part*Fraction_part*pow(2,(Exponent_part-BIAS_IEEE_754_FLOATING_32_BIT));
	return value_cal;
}

uint8_t eeprom_write_type_float(uint16_t page_start, float data){
    uint8_t buffer[4];
    memcpy(buffer, &data,4); // Sao chép dữ liệu float vào buffer
    
    for (uint8_t i = 0; i < 4; i++) {
        EEPROM.write(page_start + i, buffer[i]); // Sử dụng page_start
		Serial.println("Write");
		Serial.println(buffer[i]);
    }
    return 0; // Trả về 0 để biểu thị thành công
}

uint8_t eeprom_read_type_float(uint16_t page_start, float *data){
	/* To convert bit to float watch this video
	 * https://www.youtube.com/watch?v=8cRQT93Olek
	 * */
	uint8_t buffer[4];
	for(uint8_t i =0; i<4;i++){
        buffer[i] = EEPROM.read(page_start + i);
		Serial.println("Read");
		Serial.println(buffer[i]);
    }
	memcpy(data, buffer, sizeof(float)); // Chuyển buffer thành float
	return 0;
}

uint8_t eeprom_write_type_int(uint16_t page_start, int data){
	uint8_t buffer[4];
	memset(buffer,0, sizeof(int));
	memcpy(buffer,&data,sizeof(int));
	for(uint8_t i =0; i<4;i++){
        EEPROM.write(WEIGHT_CALIBRATION_ADDR + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_int(uint16_t page_start, int *data){
	uint8_t buffer[4];
	for(uint8_t i =0; i<4;i++){
        buffer[i] = EEPROM.read(WEIGHT_CALIBRATION_ADDR + i);
    }
	*data = (uint32_t)(((((uint32_t)buffer[3]  << 24) | (uint32_t)buffer[2]<<16) | (uint16_t)buffer[1]<<8) | buffer[0]);
	return 0;
}

uint8_t eeprom_write_type_1_byte_int8(uint16_t page_start, int8_t data){
	uint8_t buffer[1];
	memset(buffer,0, sizeof(int8_t));
	memcpy(buffer,&data,sizeof(int8_t));
	for(uint8_t i =0; i<sizeof(data);i++){
        EEPROM.write(WEIGHT_CALIBRATION_ADDR + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_1_byte_int8(uint16_t page_start, int8_t *data){
	uint8_t buffer[1];
	for(uint8_t i =0; i<sizeof(data);i++){
        buffer[i] = EEPROM.read(WEIGHT_CALIBRATION_ADDR + i);
    }
	*data = buffer[0];
	return 0;
}


uint8_t eeprom_write_type_2_bytes_uint16(uint16_t page_start, uint16_t data){
	uint8_t buffer[2];
	memset(buffer,0, sizeof(uint16_t));
	memcpy(buffer,&data,sizeof(uint16_t));
	for(uint8_t i =0; i<sizeof(data);i++){
        EEPROM.write(WEIGHT_CALIBRATION_ADDR + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_2_bytes_uint16(uint16_t page_start, uint16_t *data){
	uint8_t buffer[2];
	for(uint8_t i =0; i<sizeof(data);i++){
        buffer[i] = EEPROM.read(WEIGHT_CALIBRATION_ADDR + i);
    }
	*data = (uint32_t)(((uint16_t)buffer[1]<<8) |(uint8_t)buffer[0] );
	return 0;
}

uint8_t eeprom_write_type_2_bytes_int16(uint16_t page_start, int16_t data){
	uint8_t buffer[2];
	memset(buffer,0, sizeof(int16_t));
	memcpy(buffer,&data,sizeof(int16_t));
	for(uint8_t i =0; i<sizeof(data);i++){
        EEPROM.write(WEIGHT_CALIBRATION_ADDR + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_2_bytes_int16(uint16_t page_start, int16_t *data){
	uint8_t buffer[2];
	for(uint8_t i =0; i<sizeof(data);i++){
        buffer[i] = EEPROM.read(page_start + i);
    }
	*data = (uint32_t)(((int16_t)buffer[1]<<8) |(uint8_t)buffer[0] );
	return 0;
}

int compare_floats(float a, float b) {
	float epsilon = 1e-6;
    // So sánh hai số thực với độ chính xác epsilon
    return fabs(a - b) < epsilon;
}