/*
 * master.h
 *
 *  Created on: 19 nov. 2021
 *      Author: rapha
 */

#ifndef INC_MASTER_H_
#define INC_MASTER_H_

/*
 * This object is the head of our program. It :
 * 	-	decides the frequency of the regulation of the boost, thanks to an interrupt,
 * 	-	updates the data contained in the UI object
 * 	-	monitored the security such as over voltage ...
 * 	- 	processes the start up sequence
 */


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
enum StateSystem{
	S_STOP,		// the system is off
	S_STARTING, 	// initialization phase
	S_RUNNING,	// operating normally
	S_ERROR,		// an error occurred => user action needed
};
// ########### 		FUNCTION	###############

// ###########		CLASS		###############
class Master{
	private:
	// OBJECT
		Coulomb_meter Sensor_charge;
		Coulomb_meter Sensor_discharge;
		Boost boost;
		UI ui;

		UART_HandleTypeDef* serial_com;
		// Timer used for the frequency of the regulation of the boost (MPPT)
		TIM_HandleTypeDef* real_time_timer;
	// VARS
		float soc_max = 0.0; 	// in mAh
		// Contain the different values such as the voltage, the current and so on
		myHash<Values,float> table;

		// FREQUENCY
		int f_update;
		int f_regulation;

		// STATE
		StateSystem state = S_STOP;
	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS
		Master(Coulomb_meter Sensor_charge, Coulomb_meter Sensor_discharge, Boost boost, UI ui, UART_HandleTypeDef* serial_com, TIM_HandleTypeDef* real_time_timer);
	// FUNCTIONS
		// This function initialize all the objects and the different periph used by this class
		void init();
		// Set the value of the maximum SOC
		void Set_max_SOC (float val);
		// Write log onto the UART (serial port COM)
		// We can read it with a PC and store it
		void Write_log(string mes);
		// Update the value of the UI with the values stored in "table"
		void Update_UI();
		// function that run the state machine
		void handler();
};

#endif /* INC_MASTER_H_ */
