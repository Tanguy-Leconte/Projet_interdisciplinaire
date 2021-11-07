/*
 * Coulomb_meter.cpp
 *
 *  Created on: 7 nov. 2021
 *      Author: rapha
 */

// ###########		INCLUDE		###############
#include "Coulomb_meter.h"
#include <string>
#include <sstream>

using namespace std;

// ########### 		DEFINE		###############
// Prescaler bits => the value
const uint16_t TAB_PRESCALER_M[8] {1,4,16,64,256,1024,4096,4096};
// INIT OF THE DEVICE
#define DEFAULT_ADCmode		0x0		// Sleep
#define DEFAULT_ALCC		0x2		// Alerts enable
#define DEFAULT_Prescaler	0x5		// default prescaler to 1024 (MAX 0X07!!)
#define DEFAULT_PowerDown	0x0		// The analog part is working
// Offset in the control register
#define OFFSET_ADCmode		6
#define OFFSET_ALCC			1
#define OFFSET_Prescaler	3
#define OFFSET_PowerDown	0

#define MAX_VAL_ADCmode		0x03
#define MAX_VAL_ALCC		0x03
// #define MAX_VAL_Prescaler	0x07 //not used because set in the code, see DEFAULT_Prescaler
#define MAX_VAL_PowerDown	0x01

// ########### 		CLASS		###############
//CONSTRUCTORS
	/*
	 * @brief configure the LTC2944 at the starting and
	 * @param  	hi2c a structure that define the I2C module, that part must be configured,
	 * 			ADCmode : 0x0-0x3,
	 * 			ALCC : 0x0-0x3 (define the alert configuration),
	 * 			PowerDown : 0x0-0x1 (define the shutdown of the analog circuit to reduce the current consumption)
	 * @retval None
	 */
Coulomb_meter::Coulomb_meter(I2C_HandleTypeDef hi2c):Coulomb_meter(hi2c, DEFAULT_ADCmode, DEFAULT_ALCC, DEFAULT_PowerDown){}
Coulomb_meter::Coulomb_meter(I2C_HandleTypeDef hi2c, uint8_t ADCmode, uint8_t ALCC, uint8_t PowerDown):hi2c(hi2c),Prescaler_M(DEFAULT_Prescaler){
	// Verification that the value are in the right interval.
	string err = "";
	if (ADCmode <= MAX_VAL_ADCmode){
		Coulomb_meter::ADCmode=ADCmode;
	}else{
		err = "ADCmode";
	}
	if (ALCC <= MAX_VAL_ALCC){
		Coulomb_meter::ALCC=ALCC;
	}else{
		err = "ALCC";
	}

	Prescaler_M=DEFAULT_Prescaler;
	M_value= TAB_PRESCALER_M[Prescaler_M];

	if (PowerDown <= MAX_VAL_PowerDown){
		Coulomb_meter::PowerDown=PowerDown;
	}else{
		err = "PowerDown";
	}

	if (err != ""){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Erreur in creating the coulomb_meter object :" << err << "bad value";
		stream >> mes;
		throw (mes);
	}

	// Initialize the communication with the module and right the control register
}
