/*
 * display.c
 *
 *  Created on: Jun 29, 2017
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
  * @brief  Display settings page. From here the user can
  * 		adjust the brightness of the screen using a
  * 		sliding bar. The user can then save the new
  * 		brightness setting by pressing next, or can
  * 		go back to the system settings menu without
  * 		saving the new brightness setting.
  * @param	None
  * @retval None
  */
void display_loop(){

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed buttons for this page */
	struct button back_btn = new_button(8,213,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,22);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected, get x and y values of touch */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, don't save changes and go back to system settings page */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}
