/*
 * bed_exit_loop.c
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
  * @brief  Bed exit page. Displays the current sensitivity and alarm volume of the bed.
  * 		User can adjust both of these settings using a slide bard. User can then
  * 		press next to save the settings, or back to go back to the menu page
  * 		without saving any changes.
  * @param	None
  * @retval None
  */
void bed_exit_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed buttons for this page */
	struct button next_btn = new_button(420,213,55,55);
	struct button back_btn = new_button(3,213,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,12);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, go back to the menu */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = menu_page_loop;
				return;
			}
			/* If the next button is pressed, save the changes and warn the user
			 * not to touch the weighing surface, then go to the weight page */
			else if(is_within_bounds(user_x, user_y, next_btn.x, next_btn.y,next_btn.width, next_btn.height)){
				draw(0,0,buffer, transparency,4);
				HAL_Delay(2000);
				handle_page_loop = weight_loop;
				return;
			}
			else{

			}
		}
	}
}
