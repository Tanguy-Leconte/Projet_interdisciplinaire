/*
 * codeur.cpp
 *
 *  Created on: Nov 23, 2021
 *      Author: tami
 */

// ###########		INCLUDE		###############
#include "stm32l4xx_hal.h"
#include "encoder.h"
#include <string>
#include <sstream>

using namespace std;

// ########### 		DEFINE		###############
#define GPIOEncoder 		GPIOC
#define EncoderButtonPin 	GPIO_PIN_1
// ########### FONCTIONS TEST ################

// ########### 		CLASS		###############

theEncoder::theEncoder(){
	compteurEncoder = 0;
	TIM3 -> CNT = 32768;
}

theEncoder::theEncoder(int theValInit){
	compteurEncoder = theValInit;
	TIM3 -> CNT = 32768;
}

void theEncoder::computeSensRotation(int newCompteur){

	if(newCompteur > compteurEncoder){
		sensRotation = 1;
	} else if(newCompteur < compteurEncoder){
		sensRotation = 0;
	} else {
		sensRotation = 2;
	}

}

void theEncoder::changeCompteurEncoder(){
	int newCompteur = TIM3 -> CNT;

	computeSensRotation(newCompteur);

	compteurEncoder = newCompteur;
}

bool theEncoder::isButtonPressed(){
	return HAL_GPIO_ReadPin(GPIOEncoder, EncoderButtonPin);
}

int theEncoder::getSensRotation(){
	return sensRotation;
}

