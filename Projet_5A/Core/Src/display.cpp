/*
 * display.cpp
 *
 *  Created on: 13 déc. 2021
 *      Author: rapha
 */



// ###########		INCLUDE		###############
#include "display.h"
#include <string>
#include <sstream>

using namespace std;

// ########### 		DEFINE		###############

// ########### 		CLASS		###############
Display::Display(SPI_HandleTypeDef hspi){
	HAL_GPIO_WritePin(PORT_INIT, PIN_INIT, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_RESET);
	write_cmd(mode);
	HAL_Delay(100);
	write_cmd(display);
	HAL_Delay(100);
	write_cmd(clear);
	HAL_Delay(100);
	write_cmd(set);
	HAL_Delay(100);
}

// Write a command through SPI
void Display::write_cmd(uint8_t* cmd){
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi,cmd,1,1000);
}
