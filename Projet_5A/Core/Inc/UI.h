/*
 * UI.h
 *
 *  Created on: 7 déc. 2021
 *      Author: rapha
 */

#ifndef INC_UI_H_
#define INC_UI_H_

// ###########		INCLUDE		###############
#include "encoder.h"
#include "display.h"
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

typedef enum{
	PAGE,
	SUBPAGE,
	VALUE
}Click_level;

// Type enum representing the action
typedef enum{
	NOTHING,
	CLICK,
	LONG_CLICK,
	GO_LEFT,
	FAST_LEFT,
	GO_RIGHT,
	FAST_RIGHT
}Action;

// Type Enum with the different values that we can change dynamically
// We want to display : SOC | voltages | error | power in the battery
typedef enum {
	NONE,
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
	Values val_name = NONE;
	bool is_val_W = false;		// is the value changeable?
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

// ########### 		CLASS		###############
class UI{
	private:
		// VARS
			// /!\ start to 0 like C array
			int num_on_page = 0;		// number of the actual page
			int num_on_subpage = 0; 	// number of the actual subpage
			int num_tot_subpage = 0;	// number of total of subpages of the actual page
			bool is_Clicked = false;	// inform if we have clicked before
			Click_level click_level = PAGE;
			Action event = NOTHING;

			// array containing the pages of the menu
			Page* menu = new Page[NB_PAGE_TOT];

		// OBJECT
			Display display;
			theEncoder button;
		// FUNCTIONS
			Action computeButtonAction();
	public:
		// VARS

		//CONSTRUCTORS
			// TODO : DO a generic constructor with all the pin for the encoder and the spi of the display
			UI(\
					//Display
					Display display,\
					//button
					theEncoder button\
				);
		// FUNCTIONS
			void init_menu();

			void print();

			void handler();
};

void Test_UI();

#endif /* INC_UI_H_ */
