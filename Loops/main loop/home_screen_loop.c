/*
 * home_screen_loop.c
 *
 *  Created on: Jun 27, 2017
 *      Author: Lennon S.
 */

/* Includes */
#include "main.h"
#include "header.h"
#include "buttons.h"
#include "clock.h"
#include <stdbool.h>

/* Variables */
bool alarm_on = true;		/* Alarm is set to on by default */
bool light_on = true; 		/* Light is set to on by default */
RTC_TimeTypeDef RTC_time;
RTC_DateTypeDef RTC_date;

/*Function Definitions*/
/************************************************************************************************/

/**
  * @brief  Loop handler
  * @param	None
  * @retval None
  */
void (*handle_page_loop)(void);


/**
  * @brief  Displays the home screen. From here the user can turn on/off the
  * 		bed alarm, turn on/off the bed light, and go to weight screen.
  * @param	None
  * @retval None
  */
void home_screen_loop() {

	/* Initialize Touch */
	TS_StateTypeDef TS_State;

	/* Draw background image */
	draw(0,0,buffer,transparency,0);


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


	/* Build needed buttons for this page */
	struct button menu_btn = new_button(410,211,55,55);
	struct button alarm_btn = new_button(13,211,55,55);
	struct button light_btn = new_button(337,211,55,55);

	//clock_SetTime(&RTC_time,0,6,4);
	//clock_SetDate(&RTC_date,6, 15, 12);

	while(1){
		draw_time();
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);
		/* If touch is detected */
		if (TS_State.touchDetected) {
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If menu button is pressed, go to the pin screen for user to input their pin */
			if(is_within_bounds(user_x, user_y, menu_btn.x, menu_btn.y,menu_btn.width, menu_btn.height)){
				handle_page_loop = pin_loop;
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
