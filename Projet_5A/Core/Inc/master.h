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
 *
 * 	We use the TIM6 to run the handler and the state machine
 * 	f = 10 kHz => change it in CubeMX if needed
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

	// Master & Boost
		#define PIN_BOOST_SHUTDOWN		GPIO_PIN_8
		#define PORT_BOOST_SHUTDOWN		GPIOC

		#define PIN_BOOST_BACKTOBACK	GPIO_PIN_9
		#define PORT_BOOST_BACKTOBACK	GPIOC

#define DEFAULT_MAX_SOC_PERCENT			80

// ########### 		STRUCTURE	###############
enum StateSystem{
	S_STOP,			// the system is off
	S_STARTING, 	// initialization phase
	S_RUNNING,		// operating normally
	S_ERROR,		// an error occurred => user action needed
	S_WAIT_SOC,		// Wait until the SOC is under the maximum SOC
};
// ########### 		FUNCTION	###############

// ###########		CLASS		###############
class Master{
	private:
	// OBJECT
		Coulomb_meter sensor_charge;
		Coulomb_meter sensor_discharge;
		Boost boost;
		UI ui;

		UART_HandleTypeDef* serial_com;
		// Timer used for the frequency of the regulation of the boost (MPPT)
		TIM_HandleTypeDef* real_time_timer;

		//GPIO for the shutdown and the activation of the back to back
		// TODO : assign those values in the constructor for more flexibility
		// TODO : Move the shutdown management in the boost class
		//			and write a shutdown() function in that class
		uint16_t PIN_SHUTDOWN			= PIN_BOOST_SHUTDOWN;
		GPIO_TypeDef * PORT_SHUTDOWN 	= PORT_BOOST_SHUTDOWN;

		uint16_t PIN_BACKTOBACK			= PIN_BOOST_BACKTOBACK;
		GPIO_TypeDef * PORT_BACKTOBACK 	= PORT_BOOST_BACKTOBACK;

	// VARS
		// Contain the different values such as the voltage, the current and so on
		myHash<Values,float> table;
		MPPT_val values; // values given by the boost object

		// FREQUENCY
		int f_update;
		int f_regulation;

		// STATE
		string err;
		StateSystem state = S_STOP;
	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS
		Master(Coulomb_meter sensor_charge, Coulomb_meter sensor_discharge, Boost boost, UI ui, UART_HandleTypeDef* serial_com, TIM_HandleTypeDef* real_time_timer);
	// FUNCTIONS
		// This function initialize all the objects and the different periph used by this class
		void init();
		// Write log onto the UART (serial port COM)
		// We can read it with a PC and store it
		void Write_log(string mes);
		// State manually the state of the system (Must be use carefully)
		void Set_state(StateSystem s){state = s;}
		// Get the value from the boost object and update the values in "table"
		void Get_values();
		// Update the value of the UI with the values stored in "table"
		void Update_UI();
		// function that run the state machine and update the value
		void handler();
		// function that run the User interface. Called it in the while(1) loop
		void handlerUI();
};

#endif /* INC_MASTER_H_ */
