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
	theEncoder button_main(GPIOEncoder, EncoderButtonPin, TIM_ENC);
	//Display
	Display screen(&hspi1, PIN_LCD_RS, PORT_LCD_RS, PIN_LCD_CS, PORT_LCD_CS);
	// UI
	UI ui(screen,button_main);

// Coulomb meter
	extern I2C_HandleTypeDef hi2c1;
	Coulomb_meter sensor_charge(hi2c1);

// Boost construction
	extern TIM_HandleTypeDef htim2;
	Boost boost(sensor_charge, htim2);
// Master construction
	Master master();

// Test hash
	myHash<Values,float> table(6);


void setup(){
	// We start the encoder
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	TIM_ENC -> CNT = 32768;
	// Start the PWM
	screen.init();
	ui.init_menu();
	ui.print();
}

void My_app(){
	setup();

	Test_UI();
	while (1)
	{

	}
}

#ifdef __cplusplus
}
#endif


