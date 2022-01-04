/*
 * master.cpp
 *
 *  Created on: 19 nov. 2021
 *      Author: rapha
 */
// ###########		INCLUDE		###############
#include "master.h"
#include <string>
#include <sstream>

// ########### 		CLASS		###############
// ---- CONSTRUCTORS ------

Master::Master(Coulomb_meter Sensor_charge, Coulomb_meter Sensor_discharge, Boost boost, UART_HandleTypeDef serial_com):\
		Sensor_charge(Sensor_charge), Sensor_discharge(Sensor_discharge), boost(boost), serial_com(serial_com), table(NB_OF_DISPLAYED_VALUES)\
{}

// ---- FUNCTIONS ------

/* @brief 	: This function initialize all the objects and the different periph used by tthis class
 * @param 	: The value of the Max SOC in float between 0 and 100
 * @retval 	: NONE
 */
void Master::init(){
	// Coulomb meter
	Sensor_charge.init();
	Sensor_discharge.init();

	// Boost
	boost.init();

	// UI


}

/* @brief 	: Setter for the max value of the soc
 * @param 	: The value of the Max SOC in float between 0 and 100
 * @retval 	: NONE
 */
void Master::Set_max_SOC (float val){
	if (val < 0 || val > 100){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in the action demanding :" << "bad value";
		stream >> mes;
		throw (mes);
	}else{
		soc_max = val;
	}
}
