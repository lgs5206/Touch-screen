/*
 * event_log_loop.c
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
  * @brief  Event log page. Displays the three buttons for the logs
  * 		available. From here the user can go to the weight
  * 		event log, the bed exit event log, the surface angle
  * 		log, or can go back to the menu page.
  * @param	None
  * @retval None
  */
void event_log_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed components for this page */
	struct button back_btn = new_button(3,213,55,55);
	struct button weight_btn = new_button(90,49,55,55);
	struct button bed_btn = new_button(90,110,55,55);
	struct button angle_btn = new_button(90,177,55,55);


	/* Draw background image */
	draw(0,0,buffer,transparency,14);

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
			/* If the weight button is pressed, go to the weight event log */
			else if(is_within_bounds(user_x, user_y, weight_btn.x, weight_btn.y,weight_btn.width, weight_btn.height)){
				handle_page_loop = weight_log_loop;
				return;
			}
			/* If the bed button is pressed, go to the bed exit event log */
			else if(is_within_bounds(user_x,user_y,bed_btn.x,bed_btn.y,bed_btn.width,bed_btn.height)){
				handle_page_loop = bed_exit_log;
				return;
			}
			/* If the angle button is pressed, go to the surface angle event log */
			else if(is_within_bounds(user_x,user_y,angle_btn.x,angle_btn.y,angle_btn.width,angle_btn.height)){
				handle_page_loop = surface_angle_log;
				return;
			}
			else{
			}
		}
	}
}
