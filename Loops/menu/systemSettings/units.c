/*
 * units.c
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
  * @brief  Units loop
  * @param	None
  * @retval None
  */
void units_loop(){

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed components for this page */
	struct button back_btn = new_button(8,213,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,24);

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If the back button is pressed, re-initialize the pin value and go back to the main menu */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}
