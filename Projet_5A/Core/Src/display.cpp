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
uint8_t display[1]	={0x0C};
uint8_t clear[1]	={0x01};
uint8_t set[1]		={0x06};
uint8_t mode[1]		={0x38};
uint8_t addr[1]		={0xC5};
uint8_t data_1[1]	={0x59};

// ########### 		CLASS		###############
// Initialisation procedure
Display::Display(SPI_HandleTypeDef hspi):hspi(hspi){
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

void Display::write_data(uint8_t* data){
	HAL_GPIO_WritePin(PORT_INIT, PIN_INIT, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi,data,1,1000);
	HAL_GPIO_WritePin(PORT_INIT, PIN_INIT, GPIO_PIN_SET);
}

// Write a command through SPI
void Display::write_cmd(uint8_t* cmd){
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi,cmd,1,1000);
}

void Display::lcd_print(unsigned char* string){
	unsigned char * ptrChaine;
	ptrChaine=string;
	while(*ptrChaine!=0){
		write_data(ptrChaine);
		ptrChaine++;
	}
}

void Display::set_cursor(int column,int line){
	uint8_t val_addr;
	val_addr = (line * 0x40)+column;
	val_addr = 0x80+(val_addr & 0x7F);
	write_cmd(&val_addr);
}
