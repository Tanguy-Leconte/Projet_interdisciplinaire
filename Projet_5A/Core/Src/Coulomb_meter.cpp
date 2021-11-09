/*
 * Coulomb_meter.cpp
 *
 *  Created on: 7 nov. 2021
 *      Author: rapha
 */

// ###########		INCLUDE		###############
#include "Coulomb_meter.h"
#include "stm32l4xx_hal_i2c.h"
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

#define TIMEOUT				0xFFFF

extern I2C_HandleTypeDef hi2c1;

// ########### FONCTIONS TEST ################
void Test_coulomb_meter(){
	Coulomb_meter Explorateur(hi2c1);
}

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
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in creating the coulomb_meter object :" << err << "bad value";
		stream >> mes;
		throw (mes);
	}

	// Initialize the communication with the module and right the control register
		// SDA and SLK must be high at the beginning, then SDA => 0 => start signal
		// Then we send the 7bits of the address and the last bit is R/W (1=Read)
		// Then we address the register we want to write or read
		// Finally we send the data (if write mode)
		// Stop signal il SDA=>1 when SCK is at 1
	init();
}

void Coulomb_meter::init(){
	// we want to write so the last bit of address is 0
	uint8_t num_register = B_Control; //Correspond to the control register
	// Construction of the register Control
	uint8_t reg = (ADCmode << OFFSET_ADCmode) | (ALCC << OFFSET_ALCC) | (Prescaler_M << OFFSET_Prescaler) | (PowerDown << OFFSET_PowerDown);
	// Hal I2C handles the ack normally
	if (HAL_I2C_Mem_Write(&hi2c,address,(uint16_t)num_register,1,&reg,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in initializing the device with the I2C bus";
		stream >> mes;
		throw (mes);
	}
}

/**
  * @brief Set the desired initial SOC in mAh
  * @param  SOC value in mAh
  * @retval Effective SOC value (regarding truncature effects)
*/
void Coulomb_meter::Set_SOC_mAh(float SOC_mAh){
	Coulomb_meter::SOC_mAh=SOC_mAh;
}

/**
  * @brief get the actual SOC in the device
  * @param  none
  * @retval Effective SOC value
  */
float Coulomb_meter::Get_SOC_mAh(){
	SOC_mAh=0;
	uint8_t* pData = new uint8_t;
	// Get MSB
	if (HAL_I2C_Mem_Read(&hi2c,address,C_AccumulateChargeMSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting the accumulated charge (MSB) with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		SOC_mAh += (*pData)*(float)STEP_ACCUMULATED_CHARGE*256;
	}
	// Get LSB
	if (HAL_I2C_Mem_Read(&hi2c,address,D_AccumulateChargeLSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting the accumulated charge (LSB) with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		SOC_mAh += (*pData)*STEP_ACCUMULATED_CHARGE;
	}
	return SOC_mAh;
}

/**
  * @brief get the actual analog values
  * La fonction retourne l'ensemble des 3 valeurs analogiques.
  * Le LTC est supposé avoir été lancé auparavant.
  * La conversion dure approximativement 33ms pour le voltage 4.5ms pour le courant
  * et la température.
  * Soit un lancement est opéré toutes les 50ms (pour être tranquille) puis le résultat est mesuré
  * Soit on met le circuit en mode automatique continue.
  * Il n'y  a pas de flag de fin de conversion.

  * @param  none
  * @retval Structure LTC2944_AnalogVal_Typedef
  */
LTC2944_AnalogVal_Typedef Coulomb_meter::Get_AnalogVal(){
	uint8_t* pData = new uint8_t;
	uint16_t result = 0;
	//############# VOLTAGE ##############
	// Get Voltage_MSB
	if (HAL_I2C_Mem_Read(&hi2c,address,I_Voltage_MSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting I_Voltage_MSB with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		result = (*pData)<<8;
	}
	// Get Voltage_LSB
	if (HAL_I2C_Mem_Read(&hi2c,address,J_Voltage_LSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting I_Voltage_MSB with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		result |= (*pData);

	}
	values.Voltage_V = FSR_ADC_VOLTAGE*((float)result/(float)STEP_ADC_VOLTAGE);
	//############# CURRENT ##############
	// Get Voltage_MSB
	if (HAL_I2C_Mem_Read(&hi2c,address,I_Voltage_MSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting I_Voltage_MSB with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		result = (*pData)<<8;
	}
	// Get Voltage_LSB
	if (HAL_I2C_Mem_Read(&hi2c,address,J_Voltage_LSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting J_Voltage_LSB with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		result |= (*pData);
	}
	values.Current_mA = ((float)FSR_ADC_CURRENT/R_sense)*(((float)result-(float)STEP_ADC_CURRENT)/(float)STEP_ADC_CURRENT);

	//############# TEMPERATURE ##############
	// Get Temperature_MSB
	result = 0;
	if (HAL_I2C_Mem_Read(&hi2c,address,U_Temperature_MSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting U_Temperature_MSB with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		result = (*pData)<<8;
	}
	// Get Temperature_LSB
	if (HAL_I2C_Mem_Read(&hi2c,address,V_Temperature_LSB,1,pData,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting V_Temperature_LSB with the I2C bus";
		stream >> mes;
		throw (mes);
	}else{
		result |= (*pData);
	}
	values.Temperature_Celsius = (float)FSR_ADC_TEMP*((float)result/(float)STEP_ADC_TEMP);

	return values;
}
