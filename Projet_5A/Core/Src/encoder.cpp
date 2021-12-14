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

// ########### FONCTIONS TEST ################

// ########### 		CLASS		###############

theEncoder::theEncoder(GPIO_TypeDef* GPIOenc, uint16_t ButtonPin, TIM_TypeDef* TIMER):\
		theEncoder(GPIOenc, ButtonPin, TIMER, 32768){}

theEncoder::theEncoder(GPIO_TypeDef* GPIOenc, uint16_t ButtonPin, TIM_TypeDef* TIMER, int theValInit):\
		compteurEncoder(theValInit), GPIOEncoder(GPIOenc), EncoderButtonPin(ButtonPin), TIM_ENC(TIMER)\
{
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


