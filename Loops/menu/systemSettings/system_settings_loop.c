/*
 * system_settings_loop.c
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
  * @brief  System settings page. Displays a list of settings that are available to change.
  * 		From here the user can chose to adjust the on time, the units that weight will
  * 		be displayed in, the weight change threshold, the brightness of the display,
  * 		and the time and date. The user can also go back to the menu.
  * @param	None
  * @retval None
  */
void system_settings_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed buttons for this page */
	 struct button back_btn = new_button(8,213,55,55);
	 struct button on_time = new_button(138,33,340,30);
	 struct button units_btn = new_button(138,82,340,30);
	 struct button delta_weight = new_button(138,130,340,30);
	 struct button display_btn = new_button(138,176,340,30);
	 struct button time_date = new_button(138,225,340,30);

	/* Draw background image */
	draw(0,0,buffer,transparency,15);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected, get x and y values of touch */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, go back to the menu */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = menu_page_loop;
				return;
			}
			/* If the on time button is pressed, go to the on time page */
			else if(is_within_bounds(user_x,user_y,on_time.x,on_time.y,on_time.width,on_time.height)){
				handle_page_loop = on_time_loop;
				return;
			}
			/* If the units button is pressed, go to the units page */
			else if(is_within_bounds(user_x,user_y,units_btn.x,units_btn.y,units_btn.width,units_btn.height)){
					handle_page_loop = units_loop;
					return;
			}
			/* If the delta weight button is pressed, go to the delta weight page */
			else if(is_within_bounds(user_x,user_y,delta_weight.x,delta_weight.y,delta_weight.width,delta_weight.height)){
					handle_page_loop = delta_weight_loop;
					return;
			}
			/* If the display button is pressed, go to the display page */
			else if(is_within_bounds(user_x,user_y,display_btn.x,display_btn.y,display_btn.width,display_btn.height)){
					handle_page_loop = display_loop;
					return;
			}
			/* If the time and date button is pressed, go to the time and date page */
			else if(is_within_bounds(user_x,user_y,time_date.x,time_date.y,time_date.width,time_date.height)){
					handle_page_loop = time_date_loop;
					return;
			}
			else{
			}
		}
	}
}
