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

// ########### 		NOTES		###############
/* This file is the equivalent of our main file. In fact, as we use C++ language and CubeMX only generate C file, we decided to run our main program here.
 * We call My_app() in the main() as it is declared as an extern "C" which create a link between the C program and the C++ one.
 *
 * NB : Don't forget to delete the TIM5 ISR in stm32l4xx_it.c file when you generate code with CubeMX because it is also declared here.
 *	(To be able to use C++ objects)
 *
 *	TODO 	: cf: all the TODO labels to see improvement or things that we put for debug purpose
 *			  (We need to remove some things to test everything for now we have :
 *			  the user interface, the boost (refresh the PWM but we haven't test the MPPT)
 *			  and the charge sensor working in this version of software commit : 79140c6 branch master)
 *			: Improve the management of errors !
 *			: Implement the use of real time (with TIM5) and protect the "table" variable
 *			  (Perhaps use an RT OS such as FREE-RTOS to create threads and mutex)
 *			: Test and adjust the MPPT algorithm
 *			: Implement the reading of the voltage at the solar panel to show it to the user
 *			  (the ADC is already configured)
 *			: Try to define with the starting voltage the value of the initial SOC of the battery.
 *			  (Normally the characteristic is linear must of the time)
 *			: Implement the communication with the discharge Coulomb meter
 *			  (The value of Rsense must be put into the Coulomb_meter class and given through the constructor)
 */

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
	// set up in Fast mode at 400 kHz
	extern I2C_HandleTypeDef hi2c1;
	Coulomb_meter sensor_charge(&hi2c1);

	// set up in Normal mode at 10 kHz
	extern I2C_HandleTypeDef hi2c2;
	Coulomb_meter sensor_discharge(&hi2c2);

// Boost construction
	extern TIM_HandleTypeDef htim2;
	extern ADC_HandleTypeDef hadc1;
	Boost boost(sensor_charge, &htim2, TIM_CHANNEL_1, &hadc1);

// Master construction
	extern UART_HandleTypeDef huart2;
	extern TIM_HandleTypeDef htim5;
	Master master(sensor_charge, sensor_discharge, boost, ui, &huart2, &htim5);

void setup(){
	// Init the master and all the
	try {
		master.init();
	}catch(string mes){
		master.Write_log(mes);
		master.Set_state(S_ERROR);
	}
}

void My_app(){
	setup();

	while (1)
	{
		try {
			// Main program
			master.handler();
			master.handlerUI();
		}catch(string mes){
			master.Write_log(mes);
			master.Set_state(S_ERROR);
		}
	}

}

// Interrupt for the master handler 1 kHz :
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim5);
	//
}

#ifdef __cplusplus
}
#endif


