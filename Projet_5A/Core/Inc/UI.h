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
#define NB_PAGE_TOT		5

// ########### 		STRUCTURE	###############
// PAge strucutre with all the wanted informations
typedef struct {
	uint16_t num 	= 0;
	string title 	= "";
	string text 	= "";
}Page;

// array containing the pages of the menu
Page menu[NB_PAGE_TOT];

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

		// FUNCTIONS
			init_menu();
};


#endif /* INC_UI_H_ */
