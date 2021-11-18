/*
 * Boost.h
 *
 *  Created on: 11 nov. 2021
 *      Author: rapha
 */

#ifndef INC_BOOST_H_
#define INC_BOOST_H_

// ###########		INCLUDE		###############
#include "Coulomb_meter.h"

// ########### 		DEFINE		###############
#define FREQUENCY_KHZ		150								// Frequency of the regulation loop
#define PERIOD				(1.0/(1000.0*FREQUENCY_KHZ))

#define BOOST_K				1.0 	//
#define BOOST_Ki			1.0 	//
// ########### 		STRUCTURE	###############
typedef struct{
	float K; 		// proportionnal gain
	float Ki; 		// integral gain
}PI;

typedef struct{
	float in_actual; 		// gain for actual input of the corr
	float in_previous; 		// gain for previous input of the corr
	float out_previous; 	// gain for previous output of the corr
}T_Gain;

typedef struct{
	float in_actual; 		// actual input of the corr
	float in_previous; 		// previous input of the corr
	float out_actual; 		// previous output of the corr
	float out_previous; 	// previous output of the corr
}Corr_in_out;

typedef struct{
	float panel_voltage; 	// voltage at the solar panel
	float panel_voltage_prev; // previous voltage at the solar panel
	float bat_voltage; 		// voltage at the battery panel
	float current_mA; 		// current
	float previous_power;	// previous calculated power
	float actual_power;		// actual power
}MPPT_val;

// ########### 		GLOBAL VARS	###############

// ###########		CLASS		###############
class Boost{
	private:
	// VARS
		Coulomb_meter Sensor_charge;
		PI pi_val 				= {0,0};	// PI values
		T_Gain gain				= {0,0,0};	// gain for e(k) and e(k-1)
		Corr_in_out corrector	= {0,0,0,0};	// in out of the corrector

		MPPT_val mppt_val		= {0,0,0,0,0,0};

		float setpoint			= 0;
	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS
		/*
		 * @brief Initialize the boost
		 * @param  	hi2c a structure that define the I2C module, that part must be configured,
		 * 			ADCmode : 0x0-0x3,
		 * 			ALCC : 0x0-0x3 (define the alert configuration),
		 * 			PowerDown : 0x0-0x1 (define the shutdown of the analog circuit to reduce the current consumption)
		 * @retval None
		 */
		Boost(I2C_HandleTypeDef hi2c_charge);
		Boost(I2C_HandleTypeDef hi2c_charge, PI corr_val);
	// FUNCTIONS
		/*
		 * @brief Process the regulation of the boost
		 * @param
		 * @retval the value on the output of the corrector
		 * TODO : saturate the output | calculate K and Ki | convert the output in a pwm ratio
		 */
		float Regulate();
		/*
		 * @brief Porcess the MPPT algo
		 * @param
		 * @retval None
		 */
		void MPPT();
		/*
		 * @brief set the setpoint
		 * @param
		 * @retval None
		 */
		void Set_setpoint(float order){setpoint = order;};

};



#endif /* INC_BOOST_H_ */
