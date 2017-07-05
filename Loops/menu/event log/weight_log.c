/*
 * weight_log.c
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
  * @brief  Weight event log. Displays all recorded events when
  * 		the weight was adjusted. From here the user can
  * 		scroll up and down through these events, can clear
  * 		the entire log, or can go back to the event log page.
  * @param	None
  * @retval None
  */
void weight_log_loop(){
	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed components for this page */
	struct button back_btn = new_button(3,213,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,20);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected, get x and y values of touch */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, go back to event log menu */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = event_log_loop;
				return;
			}
			else{

			}
		}
	}
}


