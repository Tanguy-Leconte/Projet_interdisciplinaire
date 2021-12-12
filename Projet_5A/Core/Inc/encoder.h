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
	int sensRotation; 		//2 = ne bouge pas | 1 = dans un sens | 0 = dans l'autre sens

	// FUNCTIONS
	void computeSensRotation(int newCompteur);

	public:
	// VARS

	//CONSTRUCTORS
	theEncoder();
	theEncoder(int theValInit);

	// FUNCTIONS
	void changeCompteurEncoder();

	bool isButtonPressed();

	int getSensRotation();

};


#endif /* INC_ENCODER_H_ */
