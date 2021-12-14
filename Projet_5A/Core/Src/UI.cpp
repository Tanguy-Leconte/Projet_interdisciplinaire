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
extern SPI_HandleTypeDef hspi1;
// ########### 		DEFINE		###############

//############ 		TEST		###############
void Test_UI(){

}

// ########### 		CLASS		###############
// ---- CONSTRUCTORS ------
UI::UI(SPI_HandleTypeDef hspi):display(hspi){
	init_menu();
}

/* @brief 	: This function is called at the initialization to construct the different pages of the menu
 * @args  	: NONE
 * @retval	: NONE
 */
void UI::init_menu(){
	int c_page = 0;
	int c_sub_page = 0;
	// PAGE MENU
		menu[c_page].num 			= c_page;
		menu[c_page].title 			= "Menu";
		menu[c_page].text 			= "";
		c_page++;

	// PAGE VALEURS
		c_sub_page = 0;
		menu[c_page].num 			= c_page;
		menu[c_page].title 			= "Valeurs";
		menu[c_page].text 			= "";
		menu[c_page].nb_sub_page 	= 6;
		c_page++;
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
		menu[c_page].num 			= c_page;
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
Action UI::computeButtonAction(){
	if (button.isButtonPressed()){
		return CLICK;
	}

	int nb_turn = button.getNbTurnEncoder();
	if (nb_turn < -THRESHOLD_FAST_TURN){
		return FAST_LEFT;
	}else if (nb_turn < 0){
		return GO_LEFT;
	}else if (nb_turn > THRESHOLD_FAST_TURN){
		return FAST_RIGHT;
	}else if (nb_turn > 0){
		return GO_RIGHT;
	}else{
		return NOTHING;
	}
}

/* @brief 	: This print the actual page and subpage
 * @args  	: NONE
 * @retval	: Values on the screen
 */
void UI::print(){
	// Select the right page with the given number in num_on_page and nb_sub_page
	Page* p_actual_page = &(menu[num_on_page]);
	num_tot_subpage = p_actual_page->nb_sub_page;
	Sub_Page* p_actual_subpage = NULL;
	if (num_tot_subpage > num_on_subpage){
		p_actual_subpage = &(p_actual_page->sub[num_on_subpage]);
	}

	// We print the page
	display.set_cursor(1, 1);
	// string to char *
	display.print(p_actual_page->title);
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
					num_on_page = (num_on_page + 1)%(num_tot_subpage);
					num_on_subpage = 0;
				}
			}
			break;
		case (GO_RIGHT):
			// Increment
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
					num_on_page = (num_on_page - 1)%(num_tot_subpage);
					num_on_subpage = menu[num_on_page].nb_sub_page;
				}
			}
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

	// we actualise the data on the screen
	print();
}

/* @brief 	: This function where all the different task are made to test if there is an interaction with the user
 * 			This function can be called either periodically or in the while(1) loop
 * @args  	: NONE
 * @retval	: NONE
 */
