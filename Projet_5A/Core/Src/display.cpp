/*
 * display.cpp
 *
 *  Created on: 13 déc. 2021
 *      Author: rapha
 */



// ###########		INCLUDE		###############
#include "display.h"
#include <cstring>
#include <sstream>

using namespace std;

// ########### 		DEFINE		###############
uint8_t cmd_display[1]	={0x0C};
uint8_t cmd_clear[1]	={0x01};
uint8_t cmd_set[1]		={0x06};
uint8_t cmd_mode[1]		={0x38};
uint8_t cmd_addr[1]		={0xC5};
uint8_t cmd_data_1[1]	={0x59};

// ########### 		CLASS		###############
// Initialisation procedure
Display::Display(SPI_HandleTypeDef hspi, uint16_t PIN_RS, GPIO_TypeDef * PORT_RS, uint16_t PIN_CS, GPIO_TypeDef * PORT_CS):\
		hspi(hspi), PIN_RS(PIN_RS), PORT_RS(PORT_RS), PIN_CS(PIN_CS), PORT_CS(PORT_CS)\
{

}

void Display::init(){
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT_RS, PIN_RS, GPIO_PIN_RESET);
	write_cmd(cmd_mode);
	HAL_Delay(100);
	write_cmd(cmd_display);
	HAL_Delay(100);
	write_cmd(cmd_clear);
	HAL_Delay(100);
	write_cmd(cmd_set);
	HAL_Delay(100);
}

void Display::write_data(uint8_t* data){
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT_RS, PIN_RS, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi,data,1,1000);
	HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_SET);
}

// Write a command through SPI
void Display::write_cmd(uint8_t* cmd){
	HAL_GPIO_WritePin(PORT_RS, PIN_RS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi,cmd,1,1000);
}

void Display::clear(){
	write_cmd(cmd_clear);
}

void Display::print(uint8_t* s){
	uint8_t* ptrChaine;
	ptrChaine=s;
	while(*ptrChaine!=0){
		write_data(ptrChaine);
		ptrChaine++;
	}
}

void Display::print(string s){
	//string to char*!!
    int n = s.length();
    // declaring character array
    char char_array[n + 1];
    // copying the contents of the
    // string to char array
    strcpy(char_array, s.c_str());
    // we print the char*
    print(char_array);
}

void Display::print(int number){
	// int to string
	stringstream strs;
	strs << number;
	string temp_str = strs.str();
	print(temp_str);
}

void Display::set_cursor(int column,int line){
	uint8_t val_addr;
	val_addr = (line * 0x40)+column;
	val_addr = 0x80+(val_addr & 0x7F);
	write_cmd(&val_addr);
}
