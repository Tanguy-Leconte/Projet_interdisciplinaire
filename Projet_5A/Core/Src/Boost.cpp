/*
 * Boost.cpp
 *
 *  Created on: 11 nov. 2021
 *      Author: rapha
 */


// ###########		INCLUDE		###############
#include "Boost.h"
#include "Coulomb_meter.h"
#include <string>
#include <sstream>

using namespace std;

// ########### 		DEFINE		###############
#define MPPT_THRESHOLD			50	// in mW
// ########### 		CLASS		###############
Boost::Boost(I2C_HandleTypeDef hi2c_charge) : Boost(hi2c_charge, {BOOST_K,BOOST_Ki}) {}

// using the Bilinear transform : p = (2/T)*(z-1)/(z+1)
Boost::Boost(I2C_HandleTypeDef hi2c_charge, PI corr_val) : Sensor_charge(hi2c_charge),pi_val(corr_val) {
	gain.in_actual = ((PERIOD*pi_val.K)+(2*pi_val.Ki))/(PERIOD);
	gain.in_previous = ((PERIOD*pi_val.K)-(2*pi_val.Ki))/(PERIOD);
	gain.out_previous = -1.0;
}

/*
 * @brief Process the regulation of the boost
 * @param
 * @retval None
 */
float Boost::Regulate(){
	// Mesure the voltage
	corrector.in_actual 	= setpoint - Sensor_charge.Get_AnalogVal().Voltage_V;
	corrector.out_actual 	= gain.in_actual * corrector.in_actual + gain.in_previous * corrector.in_previous + gain.out_previous;
	// actualise the old values
	corrector.in_previous = corrector.in_actual;
	corrector.out_previous = corrector.out_actual;

	return corrector.out_actual;
}

/*
 * @brief Porcess the MPPT algo
 * @param
 * @retval None
 */
void Boost::MPPT(){
	LTC2944_AnalogVal_Typedef val = Sensor_charge.Get_AnalogVal();
	mppt_val.bat_voltage = val.Voltage_V;
	mppt_val.current_mA = val.Current_mA;
	// Calculate the power in mW
	mppt_val.actual_power = mppt_val.bat_voltage * mppt_val.current_mA;
	float variation = 0.0;
	if ((mppt_val.panel_voltage_prev - mppt_val.panel_voltage) != 0){
		variation = (mppt_val.actual_power - mppt_val.previous_power) / (mppt_val.panel_voltage - mppt_val.panel_voltage_prev);
	}

	// Set the new point of working (variation is the derivative of : f(Voltage) = Power
	if (abs(mppt_val.actual_power - mppt_val.previous_power) > MPPT_THRESHOLD){
		Set_setpoint(mppt_val.panel_voltage + variation);
	}

	mppt_val.previous_power = mppt_val.actual_power;
	mppt_val.panel_voltage_prev = mppt_val.panel_voltage;
}
