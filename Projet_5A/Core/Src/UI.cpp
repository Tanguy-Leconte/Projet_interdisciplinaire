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

//############ 		TEST		###############
void Test_UI(){
	while(1){
		ui.handler();
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
		menu[c_page].nb_sub_page 	= 6;
		// SUB_PAGE for each possible value
			// SOC
			Sub_Page* p_soc				= new Sub_Page[menu[c_page].nb_sub_page];
			p_soc[c_sub_page].num		= c_sub_page;
			p_soc[c_sub_page].num_page	= c_page;
			p_soc[c_sub_page].val_name	= SOC;
			p_soc[c_sub_page].val_txt	= "SOC";
			p_soc[c_sub_page].val		= 0.0;
			c_sub_page++;
			// CURRENT_BAT
			p_soc[c_sub_page].num		= c_sub_page;
			p_soc[c_sub_page].num_page	= c_page;
			p_soc[c_sub_page].val_name	= CURRENT_BAT;
			p_soc[c_sub_page].val_txt	= "I_bat";
			p_soc[c_sub_page].val		= 0.0;
			c_sub_page++;
			// VOLTAGE_BAT
			p_soc[c_sub_page].num		= c_sub_page;
			p_soc[c_sub_page].num_page	= c_page;
			p_soc[c_sub_page].val_name	= VOLTAGE_BAT;
			p_soc[c_sub_page].val_txt	= "V_bat";
			p_soc[c_sub_page].val		= 0.0;
			c_sub_page++;
			// CURRENT_PANNEL
			p_soc[c_sub_page].num		= c_sub_page;
			p_soc[c_sub_page].num_page	= c_page;
			p_soc[c_sub_page].val_name	= CURRENT_PANNEL;
			p_soc[c_sub_page].val_txt	= "I_panel";
			p_soc[c_sub_page].val		= 0.0;
			c_sub_page++;
			// POWER
			p_soc[c_sub_page].num		= c_sub_page;
			p_soc[c_sub_page].num_page	= c_page;
			p_soc[c_sub_page].val_name	= VOLTAGE_PANNEL;
			p_soc[c_sub_page].val_txt	= "V_panel";
			p_soc[c_sub_page].val		= 0.0;
			c_sub_page++;
			// CURRENT_PANNEL
			p_soc[c_sub_page].num		= c_sub_page;
			p_soc[c_sub_page].num_page	= c_page;
			p_soc[c_sub_page].val_name	= POWER;
			p_soc[c_sub_page].val_txt	= "Power";
			p_soc[c_sub_page].val		= 0.0;
		menu[c_page].sub=p_soc;
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
#define THRESHOLD_FAST_TURN		5
#define THRESHOLD_LONG_CLICK	10
unsigned int cp_click = 0;
Action UI::computeButtonAction(){
	if (button.isButtonPressed()){
		cp_click++;
		if (cp_click > THRESHOLD_LONG_CLICK){
			return LONG_CLICK;
		}else{
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
	num_tot_subpage = p_actual_page->nb_sub_page;
	Sub_Page* p_actual_subpage = NULL;
	if (num_tot_subpage > num_on_subpage){
		p_actual_subpage = &(p_actual_page->sub[num_on_subpage]);
	}

	// We print the page
	display.set_cursor(1, 1);
	display.print(p_actual_page->title);

	// We print the number of the page
	int length_nb = 1;
	if (p_actual_page->num > 9){
		length_nb++;
	}
	display.set_cursor(1, MAX_CHAR_PER_LINE - length_nb);
	display.print(p_actual_page->num);

	// print now the subpage
	if (p_actual_subpage != NULL){
		display.set_cursor(2, 1);
		display.print(p_actual_subpage->val_txt);
		// We now print the value
		display.set_cursor(2, (p_actual_subpage->val_txt).length() + 1);
		display.print(p_actual_subpage->val);
	}
}

/* @brief 	: Find the sub_page corresponding to the wanted value and return a pointer onto it
 * This function can be used in other class in order to update the value of the UI
 * @args  	: the wanted value
 * @retval	: a sub_page pointer
 */
Sub_Page* UI::find(Values w_val){
	int i = 0;
	for (i=0; i < menu[DONNEES].nb_sub_page; i++){
		if (w_val == menu[DONNEES].sub->val){
			return menu[DONNEES].sub;
		}
	}
	return NULL;
}


/* @brief 	: This function where all the different task are made to test if there is an interaction with the user
 * 			This function can be called either periodically or in the while(1) loop
 * @args  	: NONE
 * @retval	: NONE
 */
// TODO : Next step is to use Click_level to have more pages
// but it need an implementation of long click to go back to the general menu
void UI::handler(){
	event = computeButtonAction();
	switch (event){
		case (CLICK):
			is_Clicked = !is_Clicked; // we toogle the clicked state (focus or not)
			break;
		case (LONG_CLICK):
			// Not implemented
			break;
	// TODO : check the direction
		case (GO_LEFT):
			// Increment
			if (is_Clicked){
				if (menu[num_on_page].sub->is_val_W){
					menu[num_on_page].sub->val++;
				}else{
					is_Clicked = false;
				}
			}else{
				if(num_on_subpage < menu[num_on_page].nb_sub_page - 1){
					//there is another subpage
					num_on_subpage++;
				}else{
					//we go on the new page
					num_on_page = (num_on_page + 1)%(NB_PAGE_TOT);
					num_on_subpage = 0;
				}
			}
			// we actualise the data on the screen
			print();
			break;
		case (GO_RIGHT):
			// Decrement
			if (is_Clicked){
				if (menu[num_on_page].sub->is_val_W){
					menu[num_on_page].sub->val--;
				}else{
					is_Clicked = false;
				}
			}else{
				if(num_on_subpage > 0){
					//there is another subpage
					num_on_subpage--;
				}else{
					//we go on the new page
					num_on_page = (num_on_page - 1)%(NB_PAGE_TOT);
					num_on_subpage = menu[num_on_page].nb_sub_page;
				}
			}
			// we actualise the data on the screen
			print();
			break;
		case (FAST_LEFT):
			break;
		case (FAST_RIGHT):
			break;
		case (NOTHING):
			break;
		default:
			stringstream stream;
			string mes;
			stream << "File=" << __FILE__ << " | Line=" << __LINE__ << " | Error in the action demanding :" << "bad value";
			stream >> mes;
			throw (mes);
	}

}

