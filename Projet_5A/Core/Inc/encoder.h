/*
 * encodeur.h
 *
 *  Created on: Nov 23, 2021
 *      Author: tami
 */
#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

// ###########		INCLUDES	###############
#include "stm32l4xx_hal.h"

// ########### 		CLASS		###############

class theEncoder{
	private:
	// VARS
		int compteurEncoder;
		bool buttonPress;		//true = bouton pressé | false = bouton non pressé
		int sensRotation; 		//0 = ne bouge pas | 1 = dans un sens | -1 = dans l'autre sens
		int nbOfTurn;			// Number of turn made between to time

		GPIO_TypeDef* GPIOEncoder;
		uint16_t EncoderButtonPin;
		TIM_TypeDef* TIM_ENC;
		TIM_HandleTypeDef* htim;

	// FUNCTIONS
	void computeSensRotation(int newCompteur);

	public:
	// VARS

	//CONSTRUCTORS
	theEncoder(GPIO_TypeDef* GPIOenc, uint16_t ButtonPin, TIM_TypeDef* TIMER, TIM_HandleTypeDef* htim);
	theEncoder(GPIO_TypeDef* GPIOenc, uint16_t ButtonPin, TIM_TypeDef* TIMER, TIM_HandleTypeDef* htim, int theValInit);

	// FUNCTIONS
	void init();

	int getNbTurnEncoder();

	bool isButtonPressed();

	int getSensRotation();

};


#endif /* INC_ENCODER_H_ */
