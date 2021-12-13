/*
 * display.h
 *
 *  Created on: 13 déc. 2021
 *      Author: rapha
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


// ###########		INCLUDE		###############
#include "stm32l4xx_hal_spi.h"
// ########### 		DEFINE		###############

// ########### 		STRUCTURE	###############

// ########### 		GLOBAL VARS	###############

// ###########		CLASS		###############
class Display{
	private:
	// VARS

	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS
		Display(SPI_HandleTypeDef spi);
	// FUNCTIONS

};




#endif /* INC_DISPLAY_H_ */
