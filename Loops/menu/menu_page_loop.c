/*
 * menu_page_loop.c
 *
 *  Created on: Jun 28, 2017
 *      Author: Lennon S.
 */

/* Includes */
#include "main.h"
#include "header.h"
#include "buttons.h"
#include <stdbool.h>

/*Function Definitions*/
/************************************************************************************************/

/**
  * @brief  Loop handler
  * @param	None
  * @retval None
  */
void (*handle_page_loop)(void);


/**
  * @brief  Pause screen loop
  * @param	None
  * @retval None
  */
void menu_page_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed components for this page */
	 struct button back_btn = new_button(8,213,55,55);
	 struct button zero_sys = new_button(140,33,340,30);
	 struct button bed_exit = new_button(140,82,340,30);
	 struct button adj_weit = new_button(140,130,340,30);
	 struct button evnt_log = new_button(140,176,340,30);
	 struct button sys_sett = new_button(140,225,340,30);

	/* Draw background image */
	draw(0,0,buffer,transparency,3);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, re-initialize the pin value and go back to the main menu */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = weight_loop;
				return;
			}
			/* If zero system is pressed */
			else if(is_within_bounds(user_x,user_y,zero_sys.x,zero_sys.y,zero_sys.width,zero_sys.height)){
				handle_page_loop = sys_zero_loop;
				return;
			}
			/* If bed exit button is pressed */
			else if(is_within_bounds(user_x,user_y,bed_exit.x,bed_exit.y,bed_exit.width,bed_exit.height)){
				handle_page_loop = bed_exit_loop;
				return;
			}
			/* If weight adjust button is pressed */
			else if(is_within_bounds(user_x,user_y,adj_weit.x,adj_weit.y,adj_weit.width,adj_weit.height)){
				handle_page_loop = adj_weight_loop;
				return;
			}
			/* If event log button is pressed */
			else if(is_within_bounds(user_x,user_y,evnt_log.x,evnt_log.y,evnt_log.width,evnt_log.height)){
				handle_page_loop = event_log_loop;
				return;
			}
			/* If system settings button is pressed */
			else if(is_within_bounds(user_x,user_y,sys_sett.x,sys_sett.y,sys_sett.width,sys_sett.height)){
				handle_page_loop = system_settings_loop;
				return;
			}
			else{

			}
		}
	}
}
