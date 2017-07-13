/*
 *  pin_loop.c
 *
 *  Created on: Jun 26, 2017
 *      Author: Lennon S.
 */

/* Includes */
#include <stdbool.h>
#include "main.h"
#include "header.h"
#include "buttons.h"

/* Variables */
 char correct_pin[4] = "1234";		/* correct pin value				*/
 char user_pin[4]= "";				/* user pin value, empty at start	*/
 int num;

/*Function Definitions */
/************************************************************************************************/

/**
  * @brief  Loop handler
  * @param	None
  * @retval None
  */
void (*handle_page_loop)(void);

/**
  * @brief	Compares user entered pin to correct pin value
  * @param	correct: The correct pin value
  * @param 	user:    The user's pin input
  * @retval True if the values are identical, false otherwise
  */
bool check_pin(char correct[4], char user[4]){
 	if (correct[0]==user[0] &&
 	    correct[1]==user[1] &&
		correct[2]==user[2] &&
		correct[3]==user[3]){
 		return true;
 	}
 	else{
 		return false;
 	}
 }


/**
  * @brief  Pin screen. Prompts user to enter their pin. If the pin
  * 		is correct, the user can proceed to edit scale settings.
  * @param	None
  * @retval None
  */
void pin_loop()
{
	 /* Initialize touch */
	 TS_StateTypeDef TS_State;

	 /* Draw background image */
	 draw(0,0,buffer,transparency,5);

	 /* Create next and back buttons */
	 struct button next_btn = new_button(213,213,55,55);
	 struct button back_btn = new_button(8,213,55,55);

	 /* Create left pin pad buttons */
	 struct button btn1 = new_button(282,10,55,55);
	 struct button btn4 = new_button(282,77,55,55);
	 struct button btn7 = new_button(282,143,55,55);
	 struct button btnp = new_button(282,209,55,55);

	 /* Create middle pin pad buttons */
	 struct button btn2 = new_button(350,10,55,55);
	 struct button btn5 = new_button(350,77,55,55);
	 struct button btn8 = new_button(350,143,55,55);
	 struct button btn0 = new_button(350,209,55,55);

	 /* Create right pin pad buttons */
	 struct button btn3 = new_button(418,10,55,55);
	 struct button btn6 = new_button(418,77,55,55);
	 struct button btn9 = new_button(418,143,55,55);
	 struct button btnc = new_button(418,209,55,55);

	 struct numpad pinpad = numpad_create(
		&btn0,
	 	&btn1,
	 	&btn2,
	 	&btn3,
	 	&btn4,
	 	&btn5,
	 	&btn6,
	 	&btn7,
	 	&btn8,
	 	&btn9,
	 	&btnp,
	 	&btnc
	 );

 	 /* Working index of pin, initialized to 0	*/
 	 uint8_t index = 0;

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);
		/* If touch is detected */
		if (TS_State.touchDetected) {
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If next button is pressed */
			if(is_within_bounds(user_x, user_y, next_btn.x, next_btn.y,
								next_btn.width, next_btn.height)){
				/* If pin is too short or not correct */
				if(index <4 || !check_pin(correct_pin,user_pin)){
					/* Display Invalid Pin message, then re-draw screen and reset the pin */
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"        INVALID PIN");
					HAL_Delay(1500);
					draw(0,0,buffer,transparency,5);
					index = 0;
				}
				/* Else, if pin is correct, go to the weight page */
				else{
					handle_page_loop = weight_loop;
					return;
				}
			}

			/* If the back button is pressed, reset the pin value and go back to the home page */
			else if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				index = 0;
				handle_page_loop = home_screen_loop;
				return;
			}
			/* If the clear button is pressed, clear the pin screen and reset the pin value */
			else if(numpad_poll_clr(pinpad, user_x, user_y)){
				draw(0,0,buffer,transparency,5);
				index = 0;
			}
			/* If a number button is pressed, and less than 4 digits have been entered for the pin,
			* enter that number as a digit for the pin at the current index, then increment the index */
			else if(numpad_poll_num(pinpad,user_x,user_y,&num)&&index<4){
					user_pin[index] = num+48;
					index++;
					HAL_Delay(200);
			}
		}
		/* If nothing is pressed */
		else{
		}
		/* Draw a star for each number pressed, up to 4 */
		for(int i = index; i < index+1; i++){
			if(i == 1){
				draw(37,115,buffer,transparency,7);
			}
			else if(i > 1){
				draw((37+57*(i-1)),115,buffer,transparency,7);
			}
		}
	}
}
