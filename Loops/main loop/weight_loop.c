/*
 * weight_loop.c
 *
 *  Created on: Jun 27, 2017
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
  * @brief  Weight screen. Displays current weight and current delta weight,
  * 		as well as the current angle of the bed.From here, user can turn
  * 		on/off the bed light, turn on/off the bed alarm, go back to the
  * 		home page, save the weight to the log, hold the weight to make
  * 		bed adjustments, zero the weight, or go on to adjust the menu settings.
  * @param	None
  * @retval None
  */
void weight_loop() {

	/* Initialize touch */
	TS_StateTypeDef TS_State;

	/* Build needed buttons for this page */
	struct button home_btn = new_button(0,0,60,55);
	struct button pause_btn = new_button(345,35,110,65);
	struct button zero_btn = new_button(345,113,110,65);
	struct button menu_btn = new_button(411,211,55,55);
	struct button alarm_btn = new_button(13,211,55,55);
	struct button light_btn = new_button(337,211,55,55);

	/* Draw background image */
	draw(0,0,buffer,transparency,8);

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

		/* If touch is detected, save x and y values of touch */
		if (TS_State.touchDetected) {
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If home button is pressed, go to home page */
			if(is_within_bounds(user_x, user_y, home_btn.x, home_btn.y,home_btn.width, home_btn.height)){
				handle_page_loop = home_screen_loop;
				return;
			}
			/* If pause button is pressed, go to hold weight page */
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
			/* If menu button is pressed, go to menu page */
			else if(is_within_bounds(user_x,user_y,menu_btn.x,menu_btn.y,menu_btn.width,menu_btn.height)){
				handle_page_loop = menu_page_loop;
				return;
			}
			/* If alarm button is pressed, toggle it on/off and display icon accordingly */
			else if(is_within_bounds(user_x,user_y,alarm_btn.x,alarm_btn.y,alarm_btn.width,alarm_btn.height)){
				if(get_alarm_status()){
					alarm_on = !alarm_on;
					draw(10,201,buffer,transparency,10);
					HAL_Delay(300);
				}
				else{
					alarm_on = !alarm_on;
					draw(10,201,buffer,transparency,11);
					HAL_Delay(300);
				}
			}
			/* If bed light button is pressed, toggle it on/off and display icon accordingly */
			else if(is_within_bounds(user_x,user_y,light_btn.x,light_btn.y,light_btn.width,light_btn.height)){
				if(get_light_status()){
					light_on = !light_on;
					draw(337,208,buffer,transparency,1);
					HAL_Delay(300);
				}
				else{
					light_on = !light_on;
					draw(337,208,buffer,transparency,9);
					HAL_Delay(300);
				}
			}
			else{

			}
		}
	}
}

