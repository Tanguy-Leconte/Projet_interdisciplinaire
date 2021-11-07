/*
 * Coulomb_meter.cpp
 *
 *  Created on: 7 nov. 2021
 *      Author: rapha
 */

// ###########		INCLUDE		###############
#include "Coulomb_meter.h"

// ########### 		DEFINE		###############
// INIT OF THE DEVICE
#define DEFAULT_ADCmode		0x0		// Sleep
#define DEFAULT_ALCC		0x2		// Alerts enable
#define DEFAULT_PowerDown	0x0		// The analog part is working
// Offset in the control register
#define OFFSET_ADCmode		6
#define OFFSET_ALCC			1
#define OFFSET_PowerDown	0

// ########### 		CLASS		###############



