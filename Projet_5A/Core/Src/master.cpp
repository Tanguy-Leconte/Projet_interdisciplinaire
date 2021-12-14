/*
 * master.cpp
 *
 *  Created on: 19 nov. 2021
 *      Author: rapha
 */
#include "master.h"

void setup(){
	// User interface construction
		// The button
		theEncoder button_main(GPIOEncoder, EncoderButtonPin, TIM_ENC);
		//Display
		Display screen(hspi1, PIN_LCD_RS, PORT_LCD_RS, PIN_LCD_CS, PORT_LCD_CS);
		// UI
		UI ui(screen,button_main);
}


