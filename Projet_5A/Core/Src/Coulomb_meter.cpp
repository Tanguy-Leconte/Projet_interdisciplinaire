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
#define DEFAULT_ADCmode		0x3	// Sleep
#define DEFAULT_ALCC		0x2		// Alerts enable
#define DEFAULT_Prescaler	0x7		// default prescaler to 1024 (MAX 0X07 = 4096!!)
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

#define TIMEOUT				5000

#define KELVIN_TO_CELCIUS	(273.15)

extern I2C_HandleTypeDef hi2c1;

// ########### FONCTIONS TEST ################
void Test_coulomb_meter(){
	try{
		Coulomb_meter Explorateur(&hi2c1);
		LTC2944_AnalogVal_Typedef values;
		float SOC = 3000;	// in mAh
		Explorateur.Set_SOC_mAh(SOC);

		while(1){
			values = Explorateur.Get_AnalogVal();
			HAL_Delay(1000);
			SOC = Explorateur.Get_SOC_mAh();
			HAL_Delay(1000);
		}
	}catch(string err){
		// printf(err)
		while(1){}
	}
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
Coulomb_meter::Coulomb_meter(I2C_HandleTypeDef* p_hi2c):Coulomb_meter(p_hi2c, DEFAULT_ADCmode, DEFAULT_ALCC, DEFAULT_PowerDown){}
Coulomb_meter::Coulomb_meter(I2C_HandleTypeDef* p_hi2c, uint8_t ADCmode, uint8_t ALCC, uint8_t PowerDown):p_hi2c(p_hi2c),Prescaler_M(DEFAULT_Prescaler){
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
		mes = stream.str();
		throw (mes);
	}
}

// Initialize the communication with the module and right the control register
	// SDA and SLK must be high at the beginning, then SDA => 0 => start signal
	// Then we send the 7bits of the address and the last bit is R/W (1=Read)
	// Then we address the register we want to write or read
	// Finally we send the data (if write mode)
	// Stop signal SDA=>1 when SCK is at 1

void Coulomb_meter::init(){
	// we want to write so the last bit of address is 0
	uint8_t num_register = B_Control; //Correspond to the control register
	// Construction of the register Control
	uint8_t reg = (ADCmode << OFFSET_ADCmode) | (ALCC << OFFSET_ALCC) | (Prescaler_M << OFFSET_Prescaler) | (PowerDown << OFFSET_PowerDown);
	// Hal I2C handles the ack normally
	if (HAL_I2C_Mem_Write(p_hi2c,address_w,(uint16_t)num_register,1,&reg,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in initializing the device with the I2C bus";
		mes = stream.str();
		throw (mes);
	}
}
/**
 * @brief : Get the control register value
 */
uint8_t Coulomb_meter::Get_Control_Register(){
	uint8_t val=0;
	// Get Control register
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,B_Control,1,&val,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting the B_Control with the I2C bus";
		mes = stream.str();
		throw (mes);
	}
	return val;
}

/**
  * @brief Set the desired initial SOC in mAh
  * @param  SOC value in mAh
  * @retval Effective SOC value (regarding truncature effects)
  */
void Coulomb_meter::Set_SOC_mAh(float SOC_mAh){
	Coulomb_meter::SOC_mAh=SOC_mAh;
	int aux_SOC = (int)(SOC_mAh/STEP_ACCUMULATED_CHARGE);
	uint8_t* pSOC = new (uint8_t);
	*pSOC = (aux_SOC & 0xff00)>>8;
	// Set the MSB for the SOC value
	if (HAL_I2C_Mem_Write(p_hi2c,address_w,C_AccumulateChargeMSB,1,pSOC,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in writing in the C_AccumulateChargeMSB register with the I2C bus";
		mes = stream.str();
		throw (mes);
	}
	*pSOC = (aux_SOC & 0xff);
	// Set the LSB for the SOC value
	if (HAL_I2C_Mem_Write(p_hi2c,address_w,D_AccumulateChargeLSB,1,pSOC,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in writing in the D_AccumulateChargeLSB register with the I2C bus";
		mes = stream.str();
		throw (mes);
	}
}

/**
  * @brief get the actual SOC in the device
  * @param  none
  * @retval Effective SOC value
  */
float Coulomb_meter::Get_SOC_mAh(){
	SOC_mAh=0;
	uint8_t Data;
	// Get MSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,C_AccumulateChargeMSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting the accumulated charge (MSB) with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		SOC_mAh += (Data)*(float)STEP_ACCUMULATED_CHARGE*256;
	}
	// Get LSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,D_AccumulateChargeLSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting the accumulated charge (LSB) with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		SOC_mAh += (Data)*STEP_ACCUMULATED_CHARGE;
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
	uint8_t Data;
	uint16_t result = 0;
	//############# VOLTAGE ##############
	// Get Voltage_MSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,I_Voltage_MSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting I_Voltage_MSB with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		result = (Data)<<8;
	}
	// Get Voltage_LSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,J_Voltage_LSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting I_Voltage_MSB with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		result |= (Data);

	}
	values.Voltage_V = FSR_ADC_VOLTAGE*((float)result/(float)STEP_ADC_VOLTAGE);
	//############# CURRENT ##############
	// Get Voltage_MSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,O_Current_MSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting I_Voltage_MSB with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		result = (Data)<<8;
	}
	// Get Voltage_LSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,P_Current_MSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting J_Voltage_LSB with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		result |= (Data);
	}
	values.Current_mA = ((float)FSR_ADC_CURRENT/R_sense)*(((float)result-(float)STEP_ADC_CURRENT)/(float)STEP_ADC_CURRENT);

	//############# TEMPERATURE ##############
	// Get Temperature_MSB
	result = 0;
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,U_Temperature_MSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting U_Temperature_MSB with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		result = (Data)<<8;
	}
	// Get Temperature_LSB
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,V_Temperature_LSB,1,&Data,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting V_Temperature_LSB with the I2C bus";
		mes = stream.str();
		throw (mes);
	}else{
		result |= (Data);
	}
	values.Temperature_Celsius = ((float)FSR_ADC_TEMP*((float)result/(float)STEP_ADC_TEMP)) - KELVIN_TO_CELCIUS;

	return values;
}

/**
  * @brief set the device in lowpower mode (no measure), analog part disable
  * @param  none
  * @retval none
  */
void Coulomb_meter::Set_LowPowerMode(){
	uint8_t reg = 0;
	// we read the actual value of the control register to change the Power shutdown bit
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,B_Control,1,&reg,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting B_Control with the I2C bus";
		mes = stream.str();
		throw (mes);
	}

	// We change the power shutdown bit to 1 => shuts down the analog part of the device
	if ((reg & 0x1) == 1){
		// The analog part is already off
		return;
	}else{
		reg |= 0x1;
	}

	// Hal I2C handles the ack normally
	if (HAL_I2C_Mem_Write(p_hi2c,address_w,B_Control,1,&reg,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in writing the control register of the device with the I2C bus";
		mes = stream.str();
		throw (mes);
	}
}

/**
  * @brief set device in normal mode (analog part enable)
  * @param  none
  * @retval none
  */
void Coulomb_meter::Set_NormalMode(){
	uint8_t reg = 0;
	// we read the actual value of the control register to change the Power shutdown bit
	if (HAL_I2C_Mem_Read(p_hi2c,address_r,B_Control,1,&reg,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in getting B_Control with the I2C bus";
		mes = stream.str();
		throw (mes);
	}

	// We change the power shutdown bit to 0 => enables the analog part of the device
	if ((reg & 0x1) == 0){
		// The analog part is already on
		return;
	}else{
		reg &= 0xfe;
	}

	// Hal I2C handles the ack normally
	if (HAL_I2C_Mem_Write(p_hi2c,address_w,B_Control,1,&reg,1,TIMEOUT) != HAL_OK){
		stringstream stream;
		string mes;
		stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in writing the control register of the device with the I2C bus";
		mes = stream.str();
		throw (mes);
	}
}
