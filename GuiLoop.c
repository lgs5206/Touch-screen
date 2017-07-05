/*
 *  GuiLoop.c
 *
 *  Created on: Jun 26, 2017
 *      Author: Lennon S.
 */

/* Includes */
#include <stdbool.h>
#include "main.h"
#include "header.h"

/* Variables */
uint8_t buffer[30];					/* 30 Byte buffer */



/*Function Definitions */
/************************************************************************************************/


/**
  * @brief  Loop handler
  * @param	None
  * @retval None
  */
void (*handle_page_loop)(void);

/**
  * @brief  Checks if touch is within the bounds of a button
  * @param	user_x: 	X value of user's touch
  * @param 	user_y: 	Y value of user's touch
  * @param	obj_x: 		X value of upper left corner of object
  * @param 	obj_y: 		Y value of upper left corner of object
  * @param	obj_width: 	Width of object
  * @param 	obj_height: Height of object
  * @retval Result of boolean expression. True if user's touch is within bounds of object
  */
char is_within_bounds(uint16_t user_x,uint16_t user_y,uint16_t obj_x,uint16_t obj_y,uint16_t obj_width,uint16_t obj_height){

	return user_x >= obj_x && user_x <= (obj_x + obj_width) && user_y >= obj_y && user_y <= (obj_y + obj_height);
}

/**
  * @brief  Calls function to handle the page loop
  * @param	None
  * @retval None
  */
void gui_loop(){
    handle_page_loop();
}

/**
  * @brief  Initializes loop handler to first page
  * @param	None
  * @retval None
  */
void gui_setup() {
  handle_page_loop = home_screen_loop;
}


