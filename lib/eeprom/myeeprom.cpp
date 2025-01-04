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
	memset(buffer,0,4);
	memcpy(buffer,&data,4);
	for(uint8_t i =0; i<4;i++){
        EEPROM.write(page_start + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_int(uint16_t page_start, int *data){
	uint8_t buffer[4];
	for(uint8_t i =0; i<4;i++){
        buffer[i] = EEPROM.read(page_start + i);
    }
	*data = (uint32_t)(((((uint32_t)buffer[3]  << 24) | (uint32_t)buffer[2]<<16) | (uint16_t)buffer[1]<<8) | buffer[0]);
	return 0;
}

uint8_t eeprom_write_type_1_byte_int8(uint16_t page_start, int8_t data){
	uint8_t buffer[1];
	memset(buffer,0, 1);
	memcpy(buffer,&data,1);
	for(uint8_t i =0; i<1;i++){
        EEPROM.write(page_start + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_1_byte_int8(uint16_t page_start, int8_t *data){
	uint8_t buffer[1];
	for(uint8_t i =0; i<1;i++){
        buffer[i] = EEPROM.read(page_start + i);
    }
	*data = buffer[0];
	return 0;
}


uint8_t eeprom_write_type_2_bytes_uint16(uint16_t page_start, uint16_t data){
	uint8_t buffer[2];
	memset(buffer,0, sizeof(uint16_t));
	memcpy(buffer,&data,sizeof(uint16_t));
	for(uint8_t i =0; i<sizeof(data);i++){
        EEPROM.write(page_start + i, buffer[i]);
    }
	return 0;
}

uint8_t eeprom_read_type_2_bytes_uint16(uint16_t page_start, uint16_t *data){
	uint8_t buffer[2];
	for(uint8_t i =0; i<sizeof(data);i++){
        buffer[i] = EEPROM.read(page_start + i);
    }
	*data = (uint32_t)(((uint16_t)buffer[1]<<8) |(uint8_t)buffer[0] );
	return 0;
}

uint8_t eeprom_write_type_2_bytes_int16(uint16_t page_start, int16_t data){
	uint8_t buffer[2];
	memset(buffer,0, sizeof(int16_t));
	memcpy(buffer,&data,sizeof(int16_t));
	for(uint8_t i =0; i<sizeof(data);i++){
        EEPROM.write(page_start + i, buffer[i]);
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

void load_eeprom_systerm(sprint_data_enable_t *sprint_enable,lcd_data_enable_t *displaydata){
	int8_t buffer[1];
	/* SPRINT */
	eeprom_read_type_1_byte_int8(SRPINT_WEIGHT_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->getweight = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_RPM_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->getrpm = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_RADS_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->getrads = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_CUR_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->getcurrent = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_VOL_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->getvoltage = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_TEMP_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->gettemp = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_MTEMP_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->gettemp_of_motors = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(SRPINT_PRESSURE_STATUS_CHECKING_ADDR, buffer);
	sprint_enable->getpressure = (buffer[0] == 1);

	/* LCD DISPLAY */
	eeprom_read_type_1_byte_int8(DISPLAY_WEIGHT_STATUS_CHECKING_ADDR, buffer);
	displaydata->getweight = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_RPM_STATUS_CHECKING_ADDR, buffer);
	displaydata->getrpm = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_RADS_STATUS_CHECKING_ADDR, buffer);
	displaydata->getrads = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_CUR_STATUS_CHECKING_ADDR, buffer);
	displaydata->getcurrent = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_VOL_STATUS_CHECKING_ADDR, buffer);
	displaydata->getvoltage = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_TEMP_STATUS_CHECKING_ADDR, buffer);
	displaydata->gettemp = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_MTEMP_STATUS_CHECKING_ADDR, buffer);
	displaydata->gettemp_of_motors = (buffer[0] == 1);

	eeprom_read_type_1_byte_int8(DISPLAY_PRESSURE_STATUS_CHECKING_ADDR, buffer);
	displaydata->getpressure = (buffer[0] == 1);

#if ARDUINO_DEBUG == 1
	Serial.print("sprint_enable: ");
	Serial.print(sprint_enable->getweight);
	Serial.print(sprint_enable->getrpm);
	Serial.print(sprint_enable->getrads);
	Serial.print(sprint_enable->getcurrent);
	Serial.print(sprint_enable->getvoltage);
	Serial.print(sprint_enable->gettemp);
	Serial.print(sprint_enable->gettemp_of_motors);
	Serial.println(sprint_enable->getpressure);
	Serial.print("displaydata: ");
	Serial.print(displaydata->getweight);
	Serial.print(displaydata->getrpm);
	Serial.print(displaydata->getrads);
	Serial.print(displaydata->getcurrent);
	Serial.print(displaydata->getvoltage);
	Serial.print(displaydata->gettemp);
	Serial.print(displaydata->gettemp_of_motors);
	Serial.println(displaydata->getpressure);
#endif
}

void save_eeprom_systerm(sprint_data_enable_t *sprint_enable,lcd_data_enable_t *displaydata){
	/* SPRINT */
	eeprom_write_type_1_byte_int8(SRPINT_WEIGHT_STATUS_CHECKING_ADDR, sprint_enable->getweight ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_RPM_STATUS_CHECKING_ADDR, sprint_enable->getrpm ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_RADS_STATUS_CHECKING_ADDR, sprint_enable->getrads ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_CUR_STATUS_CHECKING_ADDR, sprint_enable->getcurrent ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_VOL_STATUS_CHECKING_ADDR, sprint_enable->getvoltage ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_TEMP_STATUS_CHECKING_ADDR, sprint_enable->gettemp ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_MTEMP_STATUS_CHECKING_ADDR, sprint_enable->gettemp_of_motors ? 1 : 0);
	eeprom_write_type_1_byte_int8(SRPINT_PRESSURE_STATUS_CHECKING_ADDR, sprint_enable->getpressure ? 1 : 0);

	/* LCD DISPLAY */
	eeprom_write_type_1_byte_int8(DISPLAY_WEIGHT_STATUS_CHECKING_ADDR, displaydata->getweight ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_RPM_STATUS_CHECKING_ADDR, displaydata->getrpm ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_RADS_STATUS_CHECKING_ADDR, displaydata->getrads ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_CUR_STATUS_CHECKING_ADDR, displaydata->getcurrent ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_VOL_STATUS_CHECKING_ADDR, displaydata->getvoltage ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_TEMP_STATUS_CHECKING_ADDR, displaydata->gettemp ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_MTEMP_STATUS_CHECKING_ADDR, displaydata->gettemp_of_motors ? 1 : 0);
	eeprom_write_type_1_byte_int8(DISPLAY_PRESSURE_STATUS_CHECKING_ADDR, displaydata->getpressure ? 1 : 0);

#if ARDUINO_DEBUG == 1
	Serial.println("EEPROM SAVING");
#endif
}