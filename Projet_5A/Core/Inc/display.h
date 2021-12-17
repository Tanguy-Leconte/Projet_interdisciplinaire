/*
 * display.h
 *
 *  Created on: 13 déc. 2021
 *      Author: rapha
 *
 *      Sccren: 16*2 char
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


// ###########		INCLUDE		###############
#include "stm32l4xx_hal.h"
#include <string>

using namespace std;
// ########### 		DEFINE		###############
#define MAX_CHAR_PER_LINE		16
#define MAX_LINE				2
// ########### 		STRUCTURE	###############

// ########### 		GLOBAL VARS	###############

// ###########		CLASS		###############
class Display{
	private:
	// VARS
		SPI_HandleTypeDef hspi;
	// CST
		// TODO: assign those values in the constructor
		uint16_t PIN_RS;
		GPIO_TypeDef * PORT_RS;
		uint16_t PIN_CS;
		GPIO_TypeDef * PORT_CS;
	// FUNCTIONS
		void write_cmd(uint8_t* cmd);
		void write_data(uint8_t* data);
	public:
	// VARS

	//CONSTRUCTORS
		//TODO : more powerful constructor
		Display(SPI_HandleTypeDef hspi, uint16_t PIN_RS, GPIO_TypeDef * PORT_RS, uint16_t PIN_CS, GPIO_TypeDef * PORT_CS);
	// FUNCTIONS
		void init();
		// clear the screen
		void clear();
		// Use set_cursor before print to select a location
		void print(uint8_t* s);
		void print(string s);
		void print(int number);
		void set_cursor(int column,int line);

};




#endif /* INC_DISPLAY_H_ */
