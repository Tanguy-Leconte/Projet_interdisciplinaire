/*
 * Boost.h
 *
 *  Created on: 11 nov. 2021
 *      Author: rapha
 */

#ifndef INC_BOOST_H_
#define INC_BOOST_H_
/*
 * This class handles the boost, operates the MPPT algorithm and actualize the PWM
 * The frequency of the PWM is 150 kHz
 */
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
		// OBJ
		Coulomb_meter sensor_charge;

		// PWM
		TIM_HandleTypeDef htim_PWM;
		uint32_t channel_PWM;			// can be from TIM_CHANNEL_1 to TIM_CHANNEL_6
		int frequency_kHz		= 150;  // frequency in kHz

		// PI/MPPT PARAM
		PI pi_val 				= {0,0};		// PI values
		T_Gain gain				= {0,0,0};		// gain for e(k) and e(k-1)
		Corr_in_out corrector	= {0,0,0,0};	// in out of the corrector

		MPPT_val mppt_val		= {0,0,0,0,0,0};

		float setpoint			= 0.0;
		float dutycycle			= 0.0;			// between 0 and 1
	// FUNCTIONS

	public:
	// VARS

	//CONSTRUCTORS
		/*
		 * @brief Initialize the boost
		 * @param  	hi2c a structure that define the I2C module, that part must be configured,
		 * 			Coulomb_meter : an initialize object of the Coulomb meter class
		 * @retval None
		 */
		Boost(Coulomb_meter sensor_charge, TIM_HandleTypeDef htim_PWM, uint32_t channel_PWM);
		Boost(Coulomb_meter sensor_charge, TIM_HandleTypeDef htim_PWM, uint32_t channel_PWM, PI corr_val);
	// FUNCTIONS
		/*
		 * @brief function that initialize the PWM and all the boost instance
		 * 			Call it in the setup function
		 * @param None
		 * @retval None
		 */
		void init();
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
		 * @brief calculate the duty cycle using the setpoint
		 * @param NONE
		 * @retval NONE
		 */
		void ProcessDutycycle();
		/*
		 * @brief actualise the PWM value to reach the setpoint
		 * @param NONE
		 * @retval NONE
		 */
		void ActualisePWM();

		//####### GETTER / SETTER ############
		// Only for debug purpose!!
		//@brief set the setpoint manually
		void Set_setpoint(float order){setpoint = order;};
		//@brief set the dutycycle manually
		void Set_dutycycle(float ratio){dutycycle = ratio;};
		//@brief get the value given by the coulomb meter
		MPPT_val Get_values(){return mppt_val;};


};

// Test function for the boost
void Test_Boost();

#endif /* INC_BOOST_H_ */
