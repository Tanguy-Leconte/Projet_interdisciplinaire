/*
 * UI.cpp
 *
 *  Created on: 7 déc. 2021
 *      Author: rapha
 */

// ###########		INCLUDE		###############
#include "UI.h"
#include <string>
#include <sstream>

using namespace std;

// ########### 		VARS		###############
extern UI ui;
// ########### 		DEFINE		###############
#define FAST_INC_VAL		10	// nb for the fast changes in the values
// define the time for each type of click
#define THRESHOLD_FAST_TURN		4 // must be under 2
#define THRESHOLD_CLICK			5
#define THRESHOLD_LONG_CLICK	50
//############ 		TEST		###############
void Test_UI(){
	while(1){
		ui.handler();
		HAL_Delay(20);
	}
}

// ########### 		CLASS		###############
// ---- CONSTRUCTORS ------
UI::UI(Display display,theEncoder button):display(display),button(button){
}

/* @brief 	: This function is called at the initialization to construct the different pages of the menu
 * @args  	: NONE
 * @retval	: NONE
 */
void UI::init_menu(){
	// We init the display and the button
	display.init();
	button.init();

	// TODO : change this array for a vector (if we have enough space in FLASH) to handle more easily the sub pages
	int c_page = 0;
	int c_sub_page = 0;
	// PAGE MENU
		menu[c_page].num 			= MENU;
		menu[c_page].title 			= "Menu";
		menu[c_page].text 			= "";
		c_page++;

	// PAGE VALEURS
		c_sub_page = 0;
		menu[c_page].num 			= DONNEES;
		menu[c_page].title 			= "Valeurs";
		menu[c_page].text 			= "";
		// SUB_PAGE for each possible value
			// SOC
			Sub_Page* p_sub_page = new Sub_Page();
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= SOC;
			p_sub_page->val_txt		= "SOC";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= false;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
			// MAXIMUM SOC
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= SOC_MAX;
			p_sub_page->val_txt		= "MAX SOC SET";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= true;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
			// CURRENT_BAT
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= CURRENT_BAT;
			p_sub_page->val_txt		= "I_bat";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= false;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
			// VOLTAGE_BAT
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= VOLTAGE_BAT;
			p_sub_page->val_txt		= "V_bat";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= false;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
			// CURRENT_PANNEL
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= CURRENT_PANNEL;
			p_sub_page->val_txt		= "I_panel";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= false;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
			// POWER
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= VOLTAGE_PANNEL;
			p_sub_page->val_txt		= "V_panel";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= false;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
			// CURRENT_PANNEL
			p_sub_page->num			= c_sub_page;
			p_sub_page->num_page	= c_page;
			p_sub_page->val_name	= POWER;
			p_sub_page->val_txt		= "Power";
			p_sub_page->val			= 0.0;
			p_sub_page->is_val_W	= false;
			menu[c_page].sub.push_back(*p_sub_page);
			c_sub_page++;
		c_page++;

	// PAGE ERROR
		c_sub_page = 0;
		menu[c_page].num 			= ERREUR;
		menu[c_page].title 			= "Erreurs";
		menu[c_page].text 			= "";
		// TODO: do the sub pages with the different error
		c_page++;


}

/* @brief 	: This function the action made by the user on the button. Hypth: every action are made one by one and one page after another
 * @args  	: NONE
 * @retval	: NONE
 */
unsigned int cp_click = 0;
Action UI::computeButtonAction(){
	if (button.isButtonPressed() && (event == LONG_CLICK || event == CLICK)){
		return LONG_CLICK;
	}
	if (button.isButtonPressed() && cp_click < (THRESHOLD_LONG_CLICK + 10) && event != LONG_CLICK){
		cp_click++;
	}else if (cp_click > 0){
		if (cp_click > THRESHOLD_LONG_CLICK){
			cp_click = 0;
			return LONG_CLICK;
		}else if (cp_click > THRESHOLD_CLICK){
			cp_click = 0;
			return CLICK;
		}
	}

	int nb_turn = button.getNbTurnEncoder();
	if (nb_turn < -THRESHOLD_FAST_TURN){
		return FAST_RIGHT;
	}else if (nb_turn < 0){
		return GO_RIGHT;
	}else if (nb_turn > THRESHOLD_FAST_TURN){
		return FAST_LEFT;
	}else if (nb_turn > 0){
		return GO_LEFT;
	}else{
		return NOTHING;
	}
}

/* @brief 	: This print the actual page and subpage
 * @args  	: NONE
 * @retval	: Values on the screen
 */
void UI::print(){
	// We clear the screen
	display.clear();
	// Select the right page with the given number in num_on_page and nb_sub_page
	Page* p_actual_page = &(menu[num_on_page]);
	num_tot_subpage = p_actual_page->sub.size();
	Sub_Page* p_actual_subpage = NULL;
	if (num_tot_subpage > num_on_subpage){
		p_actual_subpage = &(p_actual_page->sub[num_on_subpage]);
	}

	// We print the page
	display.set_cursor(0, 0);
	display.print(p_actual_page->title);

	// We print the number of the page
	int length_nb = 1;
	if (p_actual_page->num > 9){
		length_nb++;
	}
	display.set_cursor(0, (MAX_CHAR_PER_LINE - length_nb));
	display.print(p_actual_page->num);

	// print now the subpage
	if (p_actual_subpage != NULL){
		display.set_cursor(1, 0);
		display.print(p_actual_subpage->val_txt);
		// We now print the value
		display.set_cursor(1, (p_actual_subpage->val_txt).length() + 1);
		if (num_on_page == (int) DONNEES){
			display.print(p_actual_subpage->val);
		}
		// We notice the user that the value is writtable
		if (p_actual_subpage->is_val_W){
			display.set_cursor(1, (MAX_CHAR_PER_LINE - 1));
			display.print(CHAR_EDITABLE);
		}
	}

	// Where are we ? page, subpage or value ?
	switch (array_level[click_level]){
	case(SUBPAGE):
		display.set_cursor(0, (MAX_CHAR_PER_LINE - 1));
		display.print(CHAR_SELECTION);
		break;
	case(VALUE):
		display.set_cursor(0, (MAX_CHAR_PER_LINE - 1));
		display.print(CHAR_SELECTION);
		display.set_cursor(1, (MAX_CHAR_PER_LINE - 1));
		display.print(CHAR_SELECTION);
		break;
	default:
		break;
	}
}

/* @brief 	: Find the sub_page corresponding to the wanted value and return a pointer onto it
 * This function can be used in other class in order to update the value of the UI
 * @args  	: the wanted value
 * @retval	: a sub_page pointer
 */
Sub_Page* UI::find(Values w_val){
	size_t i = 0;
	Sub_Page subpage;
	for (i=0; i < menu[DONNEES].sub.size(); i++){
		subpage = menu[DONNEES].sub[i];
		if (w_val == subpage.val_name){
			return &(menu[DONNEES].sub[i]);
		}
	}
	return NULL;
}

/* @brief 	: Add a subpage to a given page
 * @args  	: n_page 	: the number of the page in which we want to add the subpage (cf Page_name enum strucutre)
 * 			  sub		: the sub page that we want to add
 */
void UI::add_subpage(Page_name n_page, Sub_Page sub){
	menu[n_page].sub.push_back(sub);
}

/* @brief 	: Wait for a specific action from the user
 * @args  	: action	: The wanted action to unlock the system
 */
void UI::wait_for_user_action(Action action){
	Action last_event;
	do{
		last_event = event;
		event = computeButtonAction();
		HAL_Delay(DELAY_UI_MS);
	}while(event == last_event || event != action);
}

/* @brief 	: Display an error, tell if the error is blocing or not
 * @args  	: err : the error message
 * 			action	: The wanted action to unlock the system
 * 			If Action == NOTHING, the function is not blocking
 */
void UI::print_error(string err, Action action){
	num_on_page = (int) ERREUR;
	Sub_Page sub;
	sub.num = menu[ERREUR].sub.size();
	sub.num_page = (int) ERREUR;
	sub.is_val_W = true;
	sub.val_txt = err;
	add_subpage(ERREUR, sub);
	// We print the message and fix the screen if we wait for an action
	print();
	if (NOTHING != action){
		wait_for_user_action(action);
		num_on_page = (int) MENU;
		print();
	}
}

/* @brief 	: Display an error, tell if the error is blocing or not
 * @args  	: err : the error message
 */
void UI::print_error(string err){
	print_error(err, NOTHING);
}


/* @brief 	: This function where all the different task are made to test if there is an interaction with the user
 * 			This function can be called either periodically or in the while(1) loop
 * @args  	: NONE
 * @retval	: NONE
 */
// TODO : Next step is to use Click_level to have more pages
// but it need an implementation of long click to go back to the general menu
// global var to count and update the screen every ten times if we stay on the same page
int cp = 0;
void UI::handler(){
	Action last_event = event;
	if((event = computeButtonAction()) == last_event && last_event != NOTHING ){
		return;
	}
	switch (event){
		case (CLICK):
			goClick(false);
			// we actualise the data on the screen
			print();
			break;
		case (LONG_CLICK):
			// Go back to the high page
			goClick(true);
			// we actualise the data on the screen
			print();
			break;
	// TODO : check the direction
		case (GO_RIGHT):
			// Increment
			goRight(false);
			// we actualise the data on the screen
			print();
			break;
		case (GO_LEFT):
			//Decrement
			goLeft(false);
			// we actualise the data on the screen
			print();
			break;
		case (FAST_RIGHT):
			// fast Increment
			goRight(true);
			// we actualise the data on the screen
			print();
			break;
		case (FAST_LEFT):
			//fast Decrement
			goLeft(true);
			// we actualise the data on the screen
			print();
			break;
		case (NOTHING):
			cp++;
			if (cp > CST_REFRESH_SCREEN){
				cp = 0;
				print();
			}
			break;
		default:
			stringstream stream;
			string mes;
			stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in the action demanding :" << "bad value";
			mes = stream.str();
			throw (mes);
	}

}


// change the state of the menu
void UI::goRight(bool fast){
	if (fast){
		// Increment
		if (array_level[click_level] == PAGE){
			//we go on the new page
			num_on_page = (num_on_page + 1)%(NB_PAGE_TOT);
			num_on_subpage = 0;
		}else if (array_level[click_level] == SUBPAGE){
			//we go on the next sub page
			num_on_subpage = (num_on_subpage + 1)%(menu[num_on_page].sub.size());
		}else if (array_level[click_level] == VALUE){
			if (menu[num_on_page].sub[num_on_subpage].is_val_W){
				// Are we in the range of the value?
				if (menu[num_on_page].sub[num_on_subpage].val + FAST_INC_VAL < menu[num_on_page].sub[num_on_subpage].max_val){
					menu[num_on_page].sub[num_on_subpage].val+= FAST_INC_VAL;
				}else{
					menu[num_on_page].sub[num_on_subpage].val = menu[num_on_page].sub[num_on_subpage].max_val;
				}
			}
		}
	}else{
		// Increment
		if (array_level[click_level] == PAGE){
			//we go on the new page
			num_on_page = (num_on_page + 1)%(NB_PAGE_TOT);
			num_on_subpage = 0;
		}else if (array_level[click_level] == SUBPAGE){
			//we go on the next sub page
			num_on_subpage = (num_on_subpage + 1)%(menu[num_on_page].sub.size());
		}else if (array_level[click_level] == VALUE){
			if (menu[num_on_page].sub[num_on_subpage].is_val_W){
				// Are we in the range of the value?
				if (menu[num_on_page].sub[num_on_subpage].val + 1 < menu[num_on_page].sub[num_on_subpage].max_val){
					menu[num_on_page].sub[num_on_subpage].val++;
				}else{
					menu[num_on_page].sub[num_on_subpage].val = menu[num_on_page].sub[num_on_subpage].max_val;
				}
			}
		}
	}
}
void UI::goLeft(bool fast){
	// Decrement
	if (fast){
		if (array_level[click_level] == PAGE){
			//we go on the new page
			if ((num_on_page - 1) < 0){
				num_on_page = NB_PAGE_TOT - 1;
			}else{
				num_on_page = (num_on_page - 1)%(NB_PAGE_TOT);
			}
			num_on_subpage = 0;
		}else if (array_level[click_level] == SUBPAGE){
			//we go on the previous sub page
			if ((num_on_subpage - 1) < 0){
				num_on_subpage = menu[num_on_page].sub.size() - 1;
			}else{
				num_on_subpage--;
			}
		}else if (array_level[click_level] == VALUE){
			if (menu[num_on_page].sub[num_on_subpage].is_val_W){
				// Are we in the range of the value?
				if (menu[num_on_page].sub[num_on_subpage].val - FAST_INC_VAL > menu[num_on_page].sub[num_on_subpage].min_val){
					menu[num_on_page].sub[num_on_subpage].val-= FAST_INC_VAL;
				}else{
					menu[num_on_page].sub[num_on_subpage].val = menu[num_on_page].sub[num_on_subpage].min_val;
				}
			}
		}
	}else{
		if (array_level[click_level] == PAGE){
			//we go on the new page
			if ((num_on_page - 1) < 0){
				num_on_page = NB_PAGE_TOT - 1;
			}else{
				num_on_page = (num_on_page - 1)%(NB_PAGE_TOT);
			}
			num_on_subpage = 0;
		}else if (array_level[click_level] == SUBPAGE){
			//we go on the previous sub page
			if ((num_on_subpage - 1) < 0){
				num_on_subpage = menu[num_on_page].sub.size() - 1;
			}else{
				num_on_subpage--;
			}
		}else if (array_level[click_level] == VALUE){
			if (menu[num_on_page].sub[num_on_subpage].is_val_W){
				// Are we in the range of the value?
				if (menu[num_on_page].sub[num_on_subpage].val - 1 > menu[num_on_page].sub[num_on_subpage].min_val){
					menu[num_on_page].sub[num_on_subpage].val--;
				}else{
					menu[num_on_page].sub[num_on_subpage].val = menu[num_on_page].sub[num_on_subpage].min_val;
				}
			}
		}
	}
}
void UI::goClick(bool v_long){
	if (v_long){
		click_level = 0;
	}else {
		bool next_level_ok = false;
		switch(array_level[click_level]){
		case(PAGE):
			if (menu[num_on_page].sub.size() != 0){
				next_level_ok = true;
			}
			break;
		case(SUBPAGE):
			if (menu[num_on_page].sub[num_on_subpage].is_val_W == true){
				// We implement a different behaviour depending on the page
				if (num_on_page == (int) DONNEES){
					next_level_ok = true;
				}else if (num_on_page == (int) ERREUR){
					vector<Sub_Page>::iterator it;
					it = menu[num_on_page].sub.begin() + num_on_subpage;
					menu[num_on_page].sub.erase(it);
				}
			}
			break;
		case(VALUE):
			break;
		}
		if (next_level_ok){
			click_level = (click_level + 1)%(sizeof(array_level)); // we change the click level focus
		}else{
			click_level--;
			if (click_level < 0){
				click_level = 0;
			}
		}
	}

}

