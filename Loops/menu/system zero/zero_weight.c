/*
 * zero_weight.c
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
  * @brief  Zero weight loop
  * @param	None
  * @retval None
  */
void zero_weight_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed components for this page */
	struct button home_btn = new_button(0,0,60,55);
	struct button pause_btn = new_button(345,35,110,65);
	struct button zero_btn = new_button(345,113,110,65);
	struct button menu_btn = new_button(411,211,55,55);


	/* Draw background image */
	draw(0,0,buffer,transparency,26);
	/* Draw alarm icon on or off */
	if(get_alarm_status()){
		draw(10,201,buffer,transparency,11);
	}
	else{
		draw(10,201,buffer,transparency,10);
	}
	/* Draw light icon on or off */
	if(get_light_status()){
		draw(337,208,buffer,transparency,9);
	}
	else{
		draw(337,208,buffer,transparency,1);
	}

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];

			/* If home button is pressed, go to home page */
			if(is_within_bounds(user_x, user_y, home_btn.x, home_btn.y,home_btn.width, home_btn.height)){
				handle_page_loop = home_screen_loop;
				return;
			}
			/* If pause button is pressed, go to pause screen */
			else if(is_within_bounds(user_x,user_y,pause_btn.x,pause_btn.y,pause_btn.width,pause_btn.height)){
				handle_page_loop = hold_weight_loop;
				return;
			}
			/* If zero button is pressed, warn user not to touch weighing surface, then zero the scale */
			else if(is_within_bounds(user_x,user_y,zero_btn.x,zero_btn.y,zero_btn.width,zero_btn.height)){
				draw(0,0,buffer, transparency,4);
				HAL_Delay(2000);
				handle_page_loop = weight_zeroed_loop;
				return;
			}
			/* If menu button is pressed, go back to menu page */
			else if(is_within_bounds(user_x,user_y,menu_btn.x,menu_btn.y,menu_btn.width,menu_btn.height)){
				handle_page_loop = menu_page_loop;
				return;
			}
			else{

			}
		}
	}
}

