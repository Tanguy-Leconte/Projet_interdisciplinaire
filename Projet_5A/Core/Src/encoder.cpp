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
#define TIM_ENC				TIM3
// ########### FONCTIONS TEST ################

// ########### 		CLASS		###############

theEncoder::theEncoder(){
	compteurEncoder = 0;
	TIM_ENC -> CNT = 32768;
}

theEncoder::theEncoder(int theValInit){
	compteurEncoder = theValInit;
	TIM_ENC -> CNT = 32768;
}

void theEncoder::computeSensRotation(int newCompteur){

	if(newCompteur > compteurEncoder){
		sensRotation = 1;
	} else if(newCompteur < compteurEncoder){
		sensRotation = -1;
	} else {
		sensRotation = 0;
	}

}

int theEncoder::getNbTurnEncoder(){
	int newCompteur = TIM_ENC -> CNT;

	nbOfTurn = newCompteur - compteurEncoder;
	computeSensRotation(newCompteur);

	compteurEncoder = newCompteur;

	return nbOfTurn*sensRotation;
}

bool theEncoder::isButtonPressed(){
	return HAL_GPIO_ReadPin(GPIOEncoder, EncoderButtonPin);
}

int theEncoder::getSensRotation(){
	return sensRotation;
}


