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
#include "main.h"
#include <string>
#include <sstream>

using namespace std;
// ########### 		DEFINE		###############

// ########### 		STRUCTURE	###############

// ########### 		GLOBAL VARS	###############

// ###########		CLASS		###############
class Display{
	private:
	// VARS
		SPI_HandleTypeDef hspi;
	// CST
		// TODO: assign those values in the constructor
		uint16_t PIN_RS = GPIO_PIN_6;
		GPIO_TypeDef * PORT_RS = GPIOA;
		uint16_t PIN_CS = GPIO_PIN_9;
		GPIO_TypeDef * PORT_CS = GPIOA;
	// FUNCTIONS
		void write_cmd(uint8_t* cmd);
		void write_data(uint8_t* data);
	public:
	// VARS

	//CONSTRUCTORS
		//TODO : more powerful constructor
		Display(SPI_HandleTypeDef hspi);
	// FUNCTIONS
		// Use set_cursor before print to select a location
		void print(uint8_t* s);
		void print(string s);
		void print(int number);
		void set_cursor(int column,int line);

};




#endif /* INC_DISPLAY_H_ */
