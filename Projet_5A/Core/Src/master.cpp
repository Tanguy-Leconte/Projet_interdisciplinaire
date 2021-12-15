/*
 * master.cpp
 *
 *  Created on: 19 nov. 2021
 *      Author: rapha
 */
#include "master.h"

void setup(){

}


Master::Master(Coulomb_meter Sensor_charge, Coulomb_meter Sensor_discharge, Boost boost, UART_HandleTypeDef serial_com):\
		Sensor_charge(Sensor_charge), Sensor_discharge(Sensor_discharge), boost(boost), serial_com(serial_com), table(NB_OF_DISPLAYED_VALUES)\
{

}
