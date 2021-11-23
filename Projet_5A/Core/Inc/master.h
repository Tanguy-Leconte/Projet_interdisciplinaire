/*
 * master.h
 *
 *  Created on: 19 nov. 2021
 *      Author: rapha
 */

#ifndef INC_MASTER_H_
#define INC_MASTER_H_


// ###########		INCLUDE		###############
#include "Boost.h"

// ########### 		DEFINE		###############

// ########### 		STRUCTURE	###############

// ########### 		GLOBAL VARS	###############

// ###########		CLASS		###############
class Master{
	private:
	// OBJECT
		Coulomb_meter Sensor_charge;
		Coulomb_meter Sensor_discharge;
		Boost boost;

		UART_HandleTypeDef serial_com;
	// VARS
		float soc_max = 0.0; 	// in mAh
	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS

	// FUNCTIONS
		void Set_max_SOC (float val);

		void Write_log(string mes);
};

#endif /* INC_MASTER_H_ */
