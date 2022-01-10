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
#define DEFAULT_VOLTAGE_BAT				50
#define DEFAULT_VOLTAGE_BAT_GAP			5

// ########### 		CLASS		###############
// ---- CONSTRUCTORS ------

Master::Master(Coulomb_meter sensor_charge, Coulomb_meter sensor_discharge, Boost boost, UI ui, UART_HandleTypeDef* serial_com, TIM_HandleTypeDef* real_time_timer):\
		sensor_charge(sensor_charge), sensor_discharge(sensor_discharge), boost(boost), ui(ui), serial_com(serial_com), real_time_timer(real_time_timer), table(NB_OF_DISPLAYED_VALUES)\
{}

// ---- FUNCTIONS ------

/* @brief 	: This function initialize all the objects and the different periph used by tthis class
 * @param 	: The value of the Max SOC in float between 0 and 100
 * @retval 	: NONE
 */
void Master::init(){
	// Coulomb meter
	//try{
		// TODO : REMOVE COMMENT
		//sensor_discharge.init();
	//}catch(string mes){
		// The system can work even if the discharge sensor doesn't work
	//}

	// Boost
	boost.init();

	// UI
	ui.init_menu();
	ui.print();

	// Init the table
	int el = SOC;
	unsigned int pos = 0;
	do{
		Values val = static_cast<Values>(el);
		if (!(table.add(val, 0.0, pos))){
			stringstream stream;
			string mes;
			stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in the initialization of the table";
			stream >> mes;
			throw (mes);
		}
		el ++;
		pos ++;
	}while(el <= POWER);

	// Start the timer
	if (HAL_OK != HAL_TIM_Base_Start_IT(real_time_timer)){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in the starting the real time Timer";
		stream >> mes;
		throw (mes);
	}

	//We start the system
	boost.Set_dutycycle(DEFAULT_VAL_DUTYCYCLE);
	boost.ActualisePWM();
	// We start the system
	state = S_STARTING;
}

/* @brief 	: Get the value from the boost object and update the values in "table"
 * @param 	: NONE
 * @retval 	: NONE
 */
void Master::Get_values(){
	values = boost.Get_values();
	table.modify(SOC, boost.sensor_charge.Get_SOC_mAh());
	table.modify(SOC_MAX, ui.find(SOC_MAX)->val);
	table.modify(CURRENT_BAT, values.current_mA);
	table.modify(VOLTAGE_BAT, values.bat_voltage);
	table.modify(CURRENT_PANNEL, values.actual_power / values.panel_voltage);
	table.modify(VOLTAGE_PANNEL, values.panel_voltage);
	table.modify(POWER, values.actual_power);
}

void Master::Write_log(string mes){
	asm("nop");
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
		el ++;
	}while(el <= POWER);
}

/* @brief 	: function that run the state machine and update the value
 * @param 	: NONE
 * @retval 	: NONE
 */
void Master::handler(){
	try {
		switch(state){
		// ##################### 	STOP SEQUENCE 		#########################
			case S_STOP:
				HAL_GPIO_WritePin(PORT_BACKTOBACK, PIN_BACKTOBACK, GPIO_PIN_RESET);
				// We shutdown the MOS driver
				HAL_GPIO_WritePin(PORT_SHUTDOWN, PIN_SHUTDOWN, GPIO_PIN_RESET);
				boost.Set_dutycycle(DEFAULT_VAL_DUTYCYCLE);
				boost.ActualisePWM();
				break;

		// ##################### 	START SEQUENCE 		#########################
			case S_STARTING:
				Get_values();
				// Are the capacities charged ?
				if ((table[VOLTAGE_BAT] > (DEFAULT_VOLTAGE_BAT - DEFAULT_VOLTAGE_BAT_GAP)) && \
						(table[VOLTAGE_BAT] < (DEFAULT_VOLTAGE_BAT + DEFAULT_VOLTAGE_BAT_GAP))){
					state = S_RUNNING;
					HAL_GPIO_WritePin(PORT_BACKTOBACK, PIN_BACKTOBACK, GPIO_PIN_SET);
				}
				break;

		// ##################### 	NORMAL SEQUENCE 	#########################
			case S_RUNNING:
				// Do the measurements
				Get_values();
				// is there an issue in the battery voltage ?
				if ((table[VOLTAGE_BAT] < (DEFAULT_VOLTAGE_BAT - DEFAULT_VOLTAGE_BAT_GAP)) && \
						(table[VOLTAGE_BAT] > (DEFAULT_VOLTAGE_BAT + DEFAULT_VOLTAGE_BAT_GAP))){
					state = S_ERROR;
					// We shutdown the MOS driver
					HAL_GPIO_WritePin(PORT_SHUTDOWN, PIN_SHUTDOWN, GPIO_PIN_RESET);
				// Do we have reached the maximum SOC
				}else if (table[SOC] > table[SOC_MAX]){
					// We shutdown the MOS driver
					HAL_GPIO_WritePin(PORT_SHUTDOWN, PIN_SHUTDOWN, GPIO_PIN_RESET);
					state = S_WAIT_SOC;
				}else{
					// Normal sequence
					boost.MPPT();
					boost.Process_dutycycle();
					boost.ActualisePWM();
				}
				break;

		// ##################### 	ERROR  SEQUENCE 	#########################
			case S_ERROR:
				// We shutdown the MOS driver
				HAL_GPIO_WritePin(PORT_SHUTDOWN, PIN_SHUTDOWN, GPIO_PIN_RESET);
				// TODO : send message to the UI
				HAL_Delay(10*1000);
				state = S_STOP;
				break;

		// ##################### 	WAIT SOC SEQUENCE 	#########################
			case S_WAIT_SOC:
				// TODO : implement the reading of the sensor_discharge here and update the soc !
				// We stop the timer while reading the sensor discharge values because the communication is very slow
				// Stop  the timer
				HAL_TIM_Base_Stop(real_time_timer);
				// Do the measurements
				Get_values();
				// is there an issue in the battery voltage ?
				if ((table[VOLTAGE_BAT] < (DEFAULT_VOLTAGE_BAT - DEFAULT_VOLTAGE_BAT_GAP)) && \
						(table[VOLTAGE_BAT] > (DEFAULT_VOLTAGE_BAT + DEFAULT_VOLTAGE_BAT_GAP))){
					state = S_ERROR;
					// We shutdown the MOS driver
					HAL_GPIO_WritePin(PORT_SHUTDOWN, PIN_SHUTDOWN, GPIO_PIN_RESET);
				// Do we need to charge the battery ?
				}else if (table[SOC] < table[SOC_MAX]){
					// We go in the start sequence
					state = S_STARTING;
				}
				// Restart  the timer
				HAL_TIM_Base_Start(real_time_timer);
				break;

			default:
				break;
		}
	}catch (string mes){
		state = S_ERROR;
	}
}

/* @brief 	: function that run the UI
 * @param 	: NONE
 * @retval 	: NONE
 */
void Master::handlerUI(){
	Update_UI();
	ui.handler();
	HAL_Delay(20);
}
