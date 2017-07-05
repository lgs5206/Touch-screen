/*
 * buttons.c
 *
 *  Created on: Jun 27, 2017
 *      Author: Lennon S.
 */

/* Includes */
#include "buttons.h"
#include "header.h"
#include <stdbool.h>


/*Function Definitions*/
/************************************************************************************************/


/**
  * @brief  Draw a rectangle showing the location of a button
  * @param	btn:	Button structure to draw
  * @param	fill:	True: draw filled rectangle, False: draw rectangle outline
  * @retval None
  */
void draw_button(struct button btn, bool fill) {
	if(fill == true){
		BSP_LCD_FillRect(btn.x, btn.y, btn.width, btn.height);
    }
    else{
    	BSP_LCD_DrawRect(btn.x, btn.y, btn.width, btn.height);
    }
}

/**
 * @brief	Get status of bed light: on or off
 * @param 	None
 * @retval	Bool true if on, false if off
 */
bool get_light_status(){
	return light_on;
}

/**
 * @brief	Get status of bed alarm: on or off
 * @param 	None
 * @retval	Bool true if on, false if off
 */
bool get_alarm_status(){
	return alarm_on;
}

/*Structure Definitions*/
/************************************************************************************************/


/**
  * @brief  Create a new instance of a button
  * @param	x: 		X value of upper left corner of button
  * @param 	y: 		Y value of upper left corner of button
  * @param 	width: 	Width of button
  * @param height:	Height of button
  * @retval None
  */
struct button
new_button(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
  struct button btn;

  btn.x = x;
  btn.y = y;
  btn.width = width;
  btn.height = height;

  return btn;
}


