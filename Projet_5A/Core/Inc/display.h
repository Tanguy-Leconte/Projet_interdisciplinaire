/*
 * display.h
 *
 *  Created on: 13 déc. 2021
 *      Author: rapha
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


// ###########		INCLUDE		###############
#include "main.h"
// ########### 		DEFINE		###############

// ########### 		STRUCTURE	###############

// ########### 		GLOBAL VARS	###############

// ###########		CLASS		###############
class Display{
	private:
	// VARS
		uint16_t PIN_CS = GPIO_PIN_6;
		GPIO_TypeDef * PORT_CS = GPIOA;
		uint16_t PIN_INIT = GPIO_PIN_9;
		GPIO_TypeDef * PORT_INIT = GPIOA;
	// FUNCTIONS
		void write_cmd(uint8_t* cmd)
	public:
	// VARS

	//CONSTRUCTORS
		Display(SPI_HandleTypeDef hspi);
	// FUNCTIONS

};




#endif /* INC_DISPLAY_H_ */
