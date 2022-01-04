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

Master::Master(Coulomb_meter Sensor_charge, Coulomb_meter Sensor_discharge, Boost boost, UI ui, UART_HandleTypeDef* serial_com, TIM_HandleTypeDef* real_time_timer):\
		Sensor_charge(Sensor_charge), Sensor_discharge(Sensor_discharge), boost(boost), ui(ui), serial_com(serial_com), real_time_timer(real_time_timer), table(NB_OF_DISPLAYED_VALUES)\
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
	ui.init_menu();
	ui.print();

	// Init the table
	int el = SOC;
	do{
		Values val = static_cast<Values>(el);
		if (!(table.add(val, 0.0, 0))){
			stringstream stream;
			string mes;
			stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in the initialization of the table";
			stream >> mes;
			throw (mes);
		}
	}while(el < POWER);
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

/* @brief 	: Update the value of the UI with the values stored in "table"
 * @param 	: NONE
 * @retval 	: NONE
 */
void Master::Update_UI(){
	Sub_Page* p_aux; // pointer over the different sub page to update the value
	int el = SOC;
	do{
		Values val = static_cast<Values>(el);
		p_aux = ui.find(val);
		p_aux->val = table[val];
	}while(el < POWER);
}
