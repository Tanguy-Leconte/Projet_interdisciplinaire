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
	theEncoder button_main(GPIOEncoder, EncoderButtonPin, TIM_ENC);
	//Display
	Display screen(hspi1, PIN_LCD_RS, PORT_LCD_RS, PIN_LCD_CS, PORT_LCD_CS);
	// UI
	UI ui(screen,button_main);

// Boost construction

// Master construction
	Master master();

// Test hash
	myHash<Values,float> table(6);


void setup(){
	screen.init();
	ui.init_menu();
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


