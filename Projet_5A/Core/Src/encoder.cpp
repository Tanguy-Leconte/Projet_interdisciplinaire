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
#define THRESHOLD_RESET_VAL			1000
#define DEFAULT_VAL					32768
// ########### FONCTIONS TEST ################

// ########### 		CLASS		###############

theEncoder::theEncoder(GPIO_TypeDef* GPIOenc, uint16_t ButtonPin, TIM_TypeDef* TIMER, TIM_HandleTypeDef htim):\
		theEncoder(GPIOenc, ButtonPin, TIMER, htim, 32768){}

theEncoder::theEncoder(GPIO_TypeDef* GPIOenc, uint16_t ButtonPin, TIM_TypeDef* TIMER, TIM_HandleTypeDef htim, int theValInit):\
		compteurEncoder(theValInit), GPIOEncoder(GPIOenc), EncoderButtonPin(ButtonPin), TIM_ENC(TIMER), htim(htim)\
{
	TIM_ENC -> CNT = DEFAULT_VAL;
}

void theEncoder::init(){
	// We start the encoder
	HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_1 | TIM_CHANNEL_2);
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

	nbOfTurn = abs(newCompteur - compteurEncoder);
	computeSensRotation(newCompteur);

	// We reset the value of the encoder
	if ((TIM_ENC -> CNT < THRESHOLD_RESET_VAL) || (TIM_ENC -> CNT > (DEFAULT_VAL*2 - THRESHOLD_RESET_VAL))){
		TIM_ENC -> CNT = DEFAULT_VAL;
		compteurEncoder = DEFAULT_VAL;
	}else{
		compteurEncoder = newCompteur;
	}

	return nbOfTurn*sensRotation;
}

bool theEncoder::isButtonPressed(){
	return HAL_GPIO_ReadPin(GPIOEncoder, EncoderButtonPin);
}

int theEncoder::getSensRotation(){
	return sensRotation;
}


