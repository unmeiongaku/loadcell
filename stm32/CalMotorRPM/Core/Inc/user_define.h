/*
 * user_define.h
 *
 *  Created on: Jun 16, 2024
 *      Author: nguye
 */

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#define DELAY_MS_FOR_LCD_FUNCTION 	5

#define MAX_CALLBACK_FUNC 					10
#define UWB_PERIOD_CALLBACK 				5
#define PRESCALER_TIME_TICK_PERIOD		5

#define BMP280_PERIOD_CALLBACK 			15

#define LCD_PERIOD_CALLBACK 					50


#define SSD1306_I2C			hi2c1
#define SSD1306_STATUS 									0
//0X78 				0
//0X7A 				1
/*SSD1306_DEFINE_FUNC*/
#if SSD1306_STATUS == 0
#define SSD1306_STATUS_I2C_ADDR			(0x78 << 1)
#elif SSD1306_STATUS == 1
#define SSD1306_STATUS_ADDR			(0x7A << 1)
#endif

#define TIM_DELAY_US											htim5
#define TIM_DELAY_MS											htim5

#ifndef INC_USER_DEFINE_H_
#define INC_USER_DEFINE_H_

#define WAITSYS_NOT_PASSED						0
#define PASS_FIRST_WAITSYS							1
#define PASS_SECOND_WAITSYS						2
#define PASS_THIRD_WAITSYS							3

#define NUMBER_OF_TAG						1
#define NUMBER_OF_ANCHORS				4

#define UWB_SPI hspi1

#define UWB_DEBUG_SM			1
/*
 * ENABLE										1
 * DISABLE										0
 *
 * */

#define UWB_SELECT_LIB		0
/*	CUSTOM LIB							0
 * MANUFACTURER LIB				1
 *
 * */

#define DEFAULT_MODE_SELECT 	1
/*	MODE	ANCHOR								0
 * MODE 	TAG										1
 *
 * */

#define ANCHOR_SELECT 		0
/*	ANCHOR TYPE A					0
 *  ANCHOR TYPE B					1
 *  ANCHOR TYPE C					2
 *  ANCHOR TYPE D					3
 * */
#if ANCHOR_SELECT == 0
#define ANCHOR_TYPE	'A'
#elif ANCHOR_SELECT == 1
#define ANCHOR_TYPE	'B'
#elif ANCHOR_SELECT == 2
#define ANCHOR_TYPE	'C'
#elif ANCHOR_SELECT == 3
#define ANCHOR_TYPE	'D'
#endif

#define TAG_SELECT     0
/*	 TAG TYPE A				0
 *  TAG TYPE B					1
 *  TAG TYPE C					2
 *  TAG TYPE D				3
 * */
#if TAG_SELECT == 0
#define TAG_TYPE	'TA'
#elif TAG_SELECT == 1
#define TAG_TYPE	'TB'
#elif TAG_SELECT == 2
#define TAG_TYPE	'TC'
#elif TAG_SELECT == 3
#define TAG_TYPE	'TD'
#endif

/*UWB State Machine Sig Define*/
#define NEXT_SIG_DEFINE 							1
#define FALSE_SIG_DEFINE 						2
#define OVERLOAD_RX_BUFFER_DEFINE	3
#define RX_POLL_MSG_DEFINE					4
#define RX_FINAL_MSG_DEFINE					5
#define RX_NO_MSG_DEFINE						6
#define RETURN_LOOP_SIG_DEFINE			7
#endif /* INC_USER_DEFINE_H_ */

/*BME_STATE_MACHINE_SIG_DEFINE*/
#define BME_NEXT_SIG_DEFINE 						31
