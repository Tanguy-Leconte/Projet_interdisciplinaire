/*
 * UI.h
 *
 *  Created on: 7 déc. 2021
 *      Author: rapha
 */

#ifndef INC_UI_H_
#define INC_UI_H_

// ###########		INCLUDE		###############
#include "UI.h"
#include <string>
#include <sstream>

using namespace std;


// ########### 		DEFINE		###############
// Number total of pages in the menu
#define NB_PAGE_TOT		3

// ########### 		ENUM		###############
enum Page_name{
	MENU 		= 0,
	DONNEES 	= 1,
	ERREUR		= 2
};

// Type enum representing the action
enum Action{
	CLICK,
	LONG_CLICK,
	GO_LEFT,
	FAST_LEFT,
	GO_RIGHT,
	FAST_RIGHT
};

// Type Enum with the different values that we can change dynamically
// We want to display : SOC | voltages | error | power in the battery
typedef enum {
	SOC,
	CURRENT_BAT,
	VOLTAGE_BAT,
	CURRENT_PANNEL,
	VOLTAGE_PANNEL,
	POWER
}Values;

// ########### 		STRUCTURE	###############
// Subpage with the value that we want to show
typedef struct {
	int num_page	= 0;
	int num 		= -1;
	Values val_name;
	string val_txt;
	float val		= 0.0;
}Sub_Page;

// Page strucutre with all the wanted informations => 1st row
typedef struct {
	int num 		= -1;
	string title 	= "";
	string text 	= "";
	int nb_sub_page	= 0;
	Sub_Page* sub;
}Page;

// array containing the pages of the menu
Page* menu = new Page[NB_PAGE_TOT];

// ########### 		CLASS		###############
class UI{
	private:
		// VARS
			// number of the actual page
			int num_on_page = 0;
		// FUNCTIONS

	public:
		// VARS

		//CONSTRUCTORS
			UI();
		// FUNCTIONS
			void init_menu();
};


#endif /* INC_UI_H_ */
