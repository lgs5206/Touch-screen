/*
 * hold_weight_loop.c
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
  * @brief  Hold weight page. Displays the hold weight check list. Once all criteria have been
  * 		met, user can press next to go to the weight held page, or can press back to go
  * 		back to the weight page.
  * @param	None
  * @retval None
  */
void hold_weight_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed buttons for this page */
	 struct button next_btn = new_button(420,213,55,55);
	 struct button back_btn = new_button(3,213,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,2);


	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, go back to the weight page */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = weight_loop;
				return;
			}
			/* If the next button is pressed, go to the weight held page */
			else if(is_within_bounds(user_x, user_y, next_btn.x, next_btn.y, next_btn.width, next_btn.height)){
				handle_page_loop = weight_held_loop;
				return;
			}
			else{

			}
		}
	}
}
