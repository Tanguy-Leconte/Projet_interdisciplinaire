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
//#include "hash.h";
#include <string>
#include <vector>
#include <sstream>

using namespace std;


// ########### 		DEFINE		###############
// Number total of pages in the menu
#define NB_PAGE_TOT			3
#define MAX_SOC_BATTERY 	17500	// in mAh
#define DELAY_UI_MS			20
#define CST_REFRESH_SCREEN	600	// correspond to CST_REFRESH_SCREEN*DELAY_UI_MS in ms
#define CHAR_SELECTION		"<"
#define CHAR_EDITABLE		"#"

// ########### 		ENUM		###############
enum Page_name{
	MENU 		= 0,
	DONNEES 	= 1,
	ERREUR		= 2
};

typedef enum{
	PAGE		= 0,
	SUBPAGE,
	VALUE
}Click_level;
const Click_level array_level[3] = {PAGE, SUBPAGE, VALUE};

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
#define NB_OF_DISPLAYED_VALUES		7
typedef enum {
	NONE,
	SOC,
	SOC_MAX,
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
	bool is_val_W 	= false;		// is the value changeable?
	string val_txt;
	float val		= 0.0;
	float min_val	= 0.0;
	float max_val	= 100.0;
}Sub_Page;

// Page strucutre with all the wanted informations => 1st row
typedef struct {
	int num 		= -1;
	string title 	= "";
	string text 	= "";
	vector<Sub_Page> sub;
}Page;

// ########### 		CLASS		###############
class UI{
	private:
		// VARS
			// /!\ start to 0 like C array
			int num_on_page = 0;		// number of the actual page
			int num_on_subpage = 0; 	// number of the actual subpage
			int num_tot_subpage = 0;	// number of total of subpages of the actual page
			int click_level = 0;
			Action event = NOTHING;

			// array containing the pages of the menu
			Page* menu = new Page[NB_PAGE_TOT];

		// OBJECT
			Display display;
			theEncoder button;
		// FUNCTIONS
			Action computeButtonAction();
			// change the state of the menu
			void goRight(bool fast);
			void goLeft(bool fast);
			void goClick(bool v_long);
	public:
		// VARS

		//CONSTRUCTORS
			UI(\
					//Display
					Display display,\
					//button
					theEncoder button\
				);
		// FUNCTIONS
			/* @brief 	: This function is called at the initialization to construct the different pages of the menu
			 */
			void init_menu();

			/* @brief 	: This print the actual page and subpage
			 * @retval	: Values on the screen
			 */
			void print();


			/* @brief 	: Find the sub_page corresponding to the wanted value and return a pointer onto it
			 * This function can be used in other class in order to update the value of the UI
			 * @args  	: the wanted value
			 * @retval	: a sub_page pointer
			 */
			Sub_Page* find(Values w_val);

			/* @brief 	: Add a subpage to a given page
			 * @args  	: n_page 	: the number of the page in which we want to add the subpage (cf Page_name enum strucutre)
			 * 			  sub		: the sub page that we want to add
			 */
			void add_subpage(Page_name n_page, Sub_Page sub);

			/* @brief 	: Wait for a specific action from the user
			 * @args  	: action	: The wanted action to unlock the system
			 */
			void wait_for_user_action(Action action);

			/* @brief 	: Wait for a specific action from the user before start the system
			 * @args  	: action	: The wanted action to unlock the system
			 */
			void wait_for_starting(Action action);

			/* @brief 	: Display an error, tell if the error is blocing or not
			 * @args  	: err 	: the error message
			 * 			action	: The wanted action to unlock the system
			 * 			If Action == NOTHING, the function is not blocking
			 */
			void print_error(string err, Action action);
			/* @brief 	: Display an error, tell if the error is blocing or not
			 * @args  	: err : the error message
			 */
			void print_error(string err);


			/* @brief 	: Manage the user interface
			*/
			void handler();
};

void Test_UI();

#endif /* INC_UI_H_ */
