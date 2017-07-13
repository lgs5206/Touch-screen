/*
 * weight_correction_loop.c
 *
 *  Created on: Jun 30, 2017
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
  * @brief  Weight correction screen. From here the user can
  * 		manually adjust the weight patient, then press
  * 		next to save the weight, or press back to go back
  * 		to the menu page.
  * @param	None
  * @retval None
  */
void weight_correction_loop(){

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed buttons for this page */
	struct button back_btn = new_button(8,213,55,55);
	struct button next_btn = new_button(213,213,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,27);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, go back to the menu page */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = menu_page_loop;
				return;
			}
			/* If next button is pressed, warn user not to touch the scale, then go to weight page with new, adjusted weight */
			else if(is_within_bounds(user_x,user_y, next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				draw(0,0,buffer,transparency,4);
				HAL_Delay(2000);
				handle_page_loop = weight_zeroed_loop;
				return;
			}
		}
	}
}
