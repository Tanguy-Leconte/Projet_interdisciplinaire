/*
 * master.h
 *
 *  Created on: 19 nov. 2021
 *      Author: rapha
 */

#ifndef INC_MASTER_H_
#define INC_MASTER_H_


// ###########		INCLUDE		###############
#include "Boost.h"
#include "UI.h"
#include "hash.h"

// ########### 		DEFINE/VARS	###############
	// Main button
		#define GPIOEncoder 		GPIOC
		#define EncoderButtonPin 	GPIO_PIN_1
		#define TIM_ENC				TIM3

	// LCD screen
		extern SPI_HandleTypeDef hspi1;
		#define PIN_LCD_RS			GPIO_PIN_6
  	  	#define PORT_LCD_RS			GPIOA
		#define PIN_LCD_CS			GPIO_PIN_9
		#define PORT_LCD_CS			GPIOA

// ########### 		STRUCTURE	###############

// ########### 		FUNCTION	###############
void setup();

// ###########		CLASS		###############
class Master{
	private:
	// OBJECT
		Coulomb_meter Sensor_charge;
		Coulomb_meter Sensor_discharge;
		Boost boost;

		UART_HandleTypeDef serial_com;
	// VARS
		float soc_max = 0.0; 	// in mAh

		myHash<Values,float> table;
	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS
		Master(Coulomb_meter Sensor_charge, Coulomb_meter Sensor_discharge, Boost boost, UART_HandleTypeDef serial_com);
	// FUNCTIONS
		void Set_max_SOC (float val);

		void Write_log(string mes);
};

#endif /* INC_MASTER_H_ */
