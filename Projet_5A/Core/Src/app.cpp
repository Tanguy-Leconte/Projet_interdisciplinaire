/*
 * app.cpp
 *
 *  Created on: Dec 7, 2021
 *      Author: rapha
 */

// ########### 		INCLUDE		###############
#include "app.h"
#include "master.h"

#ifdef __cplusplus
extern "C" {
#endif

// ########### 	APPLICATION		###############

// User interface construction
	// The button
	extern TIM_HandleTypeDef htim3;
	theEncoder button_main(GPIOEncoder, EncoderButtonPin, TIM_ENC, &htim3);
	//Display
	Display screen(&hspi1, PIN_LCD_RS, PORT_LCD_RS, PIN_LCD_CS, PORT_LCD_CS);
	// UI
	UI ui(screen,button_main);

// Coulomb meter
	extern I2C_HandleTypeDef hi2c1;
	Coulomb_meter sensor_charge(&hi2c1);

	extern I2C_HandleTypeDef hi2c2;
	Coulomb_meter sensor_discharge(&hi2c2);

// Boost construction
	extern TIM_HandleTypeDef htim2;
	extern ADC_HandleTypeDef hadc1;
	Boost boost(sensor_charge, &htim2, TIM_CHANNEL_1, &hadc1);

// Master construction
	extern UART_HandleTypeDef huart2;
	extern TIM_HandleTypeDef htim6;
	Master master(sensor_charge, sensor_discharge, boost, ui, &huart2, &htim6);

// Test hash
	myHash<Values,float> table(6);


void setup(){
	// Init the master
	master.init();
}

void My_app(){
	try {
		setup();

		Test_UI();
		//Test_Boost();
		while (1)
		{
			master.handlerUI();
		}
	}catch(string mes){
		master.Write_log(mes);
	}
}

// Interrupt for the master handler 10 kHz :
void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);
  master.handler();
}

#ifdef __cplusplus
}
#endif


