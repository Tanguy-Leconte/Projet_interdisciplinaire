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

// ########### 		DEFINE		###############

// ########### 		CLASS		###############
// ---- CONSTRUCTORS ------
UI::UI(){
	init_menu();
}

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

