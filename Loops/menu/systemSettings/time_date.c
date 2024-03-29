/*
 * time_date.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Lennon S
 */

/* Includes */
#include "main.h"
#include "header.h"
#include "buttons.h"
#include <stdbool.h>
#include <stdio.h>

/* Variables */
uint8_t month_index 	= 0;			/* Current index of 2-digit month value		*/
uint8_t day_index 		= 0;			/* Current index of 2-digit day value	 	*/
uint8_t year_index		= 0;			/* Current index of 4-digit year value		*/
uint8_t hour_index		= 0;			/* Current index of 2-digit hour value		*/
uint8_t minute_index	= 0;			/* Current index of 2-digit minute value 	*/

uint8_t month_val_tens;					/* Integer representing the ten's place for the month		*/
uint8_t month_val_ones;					/* Integer representing the one's place for the month		*/
uint8_t day_val_tens;					/* Integer representing the ten's place for the day			*/
uint8_t day_val_ones;					/* Integer representing the one's place for the day			*/
uint8_t year_val_thous;  				/* Integer representing the thousand's place for the year	*/
uint8_t year_val_hunds;					/* Integer representing the hundred's place for the year	*/
uint8_t year_val_tens;					/* Integer representing the tens's place for the year		*/
uint8_t year_val_ones;					/* Integer representing the one's place for the year		*/
uint8_t hour_val_tens;					/* Integer representing the ten's place for the hour		*/
uint8_t hour_val_ones;					/* Integer representing the one's place for the hour		*/
uint8_t minute_val_tens;				/* Integer representing the ten's place for the minute		*/
uint8_t minute_val_ones;				/* Integer representing the one's place for the minute		*/

uint16_t accum 			= 0;			/* Temporary integer to keep track of a change in time or date	*/
uint16_t year_check 	= 0;			/* Temporary integer used to check the user inputed year		*/

RTC_TimeTypeDef RTC_time;				/* Instantiation of the RTC time	*/
RTC_DateTypeDef RTC_date;				/* Instantiation of the RTC date	*/

uint8_t minutes;						/* Current minute of RTC clock		*/
uint8_t hours;							/* Current hour of RTC clock		*/
uint8_t day;							/* Current day of RTC clock			*/
uint8_t month;							/* Current month of RTC clock		*/
uint8_t year;							/* Current year of RTC clock		*/

bool month_changed	= false;			/* Status of month, changed or not	*/
bool day_changed 	= false;			/* Status of day, changed or not	*/
bool year_changed 	= false;			/* Status of year, changed or not	*/
bool minute_changed = false;			/* Status of minute, changed or not	*/
bool hour_changed 	= false;			/* Status of hour, changed or not	*/

uint8_t new_month;						/* New month value inputed by user	*/
uint8_t new_day;						/* New day value inputed by user	*/
uint8_t new_year;						/* New year value inputed by user	*/
uint8_t new_hour;						/* New hour value inputed by user	*/
uint8_t new_minute;						/* New minute value inputed by user	*/

char min[2];							/* Stores 2 byte value of minute	*/
char hr[2];								/* Stores 2 byte value of hour		*/
char mon[2];							/* Stores 2 byte value of month		*/
char dy[2];								/* Stores 2 byte value of day		*/
char yr[2];								/* Stores 2 byte value of year		*/

char year_full[4];						/* Stores 4 byte value of year to be checked */

char minute_buff[2];					/* Buffer for minute to be printed	*/
char hour_buff[2];						/* Buffer for hour to be printed	*/
char month_buff[2];						/* Buffer for month to be printed	*/
char day_buff[2];						/* Buffer for day to be printed		*/
char year_buff[2];						/* Buffer for year to be printed	*/

int num;								/* Temporary value used for mathematical operators on date and time	*/

/* Buttons that will be initialized and used on this page 	*/
struct button next_btn;
struct button back_btn;
struct button month_block ;
struct button day_block ;
struct button year_block;
struct button hour_block;
struct button minute_block;
struct button btn1;
struct button btn4;
struct button btn7;
struct button btnp;
struct button btn2;
struct button btn5;
struct button btn8;
struct button btn0;
struct button btn3;
struct button btn6;
struct button btn9;
struct button btnc;


struct numpad pinpad;					/*Pin pad that will be initialized and used on this page	*/




/*Function Definitions*/
/************************************************************************************************/


/**
  * @brief  Loop handler
  * @param	None
  * @retval None
  */
void (*handle_page_loop)(void);

/**
 * @brief Initializes all buttons that will be used for the time_date page
 * @param 	None
 * @retval 	None
 */
void initialize_buttons(){

	/* Build needed buttons for this page 		*/
	next_btn 		= new_button(213,213,55,55);
	back_btn 		= new_button(8,213,55,55);
	month_block 	= new_button(20,62,58,38);
	day_block 		= new_button(88,62,60,38);
	year_block 		= new_button(155,62,103,38);
	hour_block 		= new_button(20,137,58,38);
	minute_block	= new_button(99,137,60,38);

	/* Create left pin pad buttons 				*/
	btn1 			= new_button(282,10,55,55);
	btn4 			= new_button(282,77,55,55);
	btn7 			= new_button(282,143,55,55);
	btnp 			= new_button(282,209,55,55);

	/* Create middle pin pad buttons 			*/
	btn2 			= new_button(350,10,55,55);
	btn5 			= new_button(350,77,55,55);
	btn8 			= new_button(350,143,55,55);
	btn0 			= new_button(350,209,55,55);

	/* Create right pin pad buttons 			*/
	btn3 			= new_button(418,10,55,55);
	btn6 			= new_button(418,77,55,55);
	btn9			= new_button(418,143,55,55);
	btnc 			= new_button(418,209,55,55);

	/* Create numberpad consisting of buttons	*/
	pinpad = numpad_create(
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
}

/**
 * @brief 	Checks for changes in the date/time. If a change was made,
 * 		  	update the respective value on the RTC.
 * @param	None
 * @retval	None
 */
void update_time(){

	if(month_changed){
		set_month(&RTC_date,new_month);
	}
	if(day_changed){
		set_day(&RTC_date,new_day);
	}
	if(year_changed){
		set_year(&RTC_date,new_year);
	}
	if(hour_changed){
		set_hour(&RTC_time,new_hour);
	}
	if(minute_changed){
		set_minute(&RTC_time,new_minute);
		set_second(&RTC_time,0);
	}
}
/**
 * @brief	Gets the current time and date from RTC. Draws background image
 * 			then displays month, day, year, hour, and minute as separate
 * 			values on the screen.
 * @param	None
 * @retval	None
 */
void draw_date_time(){

	/* Get current time and date from RTC */
	clock_GetTime(&RTC_time);
	clock_GetDate(&RTC_date);

	/* Save each part of time as an integer */
	minutes = RTC_time.Minutes;
	hours = RTC_time.Hours;
	day = RTC_date.Date;
	month = RTC_date.Month;
	year = RTC_date.Year;

	/* Convert and format each integer into a character array */
	sprintf(min,"%02d",minutes);
	sprintf(hr,"%02d",hours);
	sprintf(dy,"%02d",day);
	sprintf(mon,"%02d",month);
	sprintf(yr,"%02d",year);

	/* The full 4-digit year value cannot be saved as a uint8_t
	 * '20' is appended to the front of the 2-digit year. Only
	 * years between 2000 and 2099 will be accepted */
	strcpy(year_full,"20");
	strcat(year_full,yr);


	/* Draw background image and set the font and color*/
	draw(0,0,buffer,transparency,21);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	/* Display each part of time in the correct location on the screen */
	BSP_LCD_DisplayStringAt(35,70,(uint8_t *)mon,LEFT_MODE);
	BSP_LCD_DisplayStringAt(105,70,(uint8_t *)dy,LEFT_MODE);
	BSP_LCD_DisplayStringAt(178,70,(uint8_t *)year_full,LEFT_MODE);
	BSP_LCD_DisplayStringAt(35,145,(uint8_t *)hr,LEFT_MODE);
	BSP_LCD_DisplayStringAt(112,145,(uint8_t *)min,LEFT_MODE);
}


/**
  * @brief  Time/date page. Displays the current time and date. From here the user can
  * 		manually adjust the time and date. The user can then press next to save
  * 		the new time and date, or press back to go back to the system settings
  * 		page without saving any changes.
  * @param	None
  * @retval None
  */
void time_date_loop(){

	/* Initialize touch and buttons, then display the current time and date */
	TS_StateTypeDef TS_State;
	initialize_buttons();
	draw_date_time();

	while (1) {
		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if (TS_State.touchDetected) {
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];

			/* If the back button is pressed, don't save any changes, then go back to the menu */
			if (is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Nothing was changed");
				HAL_Delay(500);
				handle_page_loop = system_settings_loop;
				return;
			}

			/* If the month block is selected, user can input value for month */
			else if(is_within_bounds(user_x,user_y,month_block.x,month_block.y,month_block.width,month_block.height)){
				month_change_loop();
				return;
			}

			/* If the day block is selected, user can input value for day */
			else if(is_within_bounds(user_x,user_y,day_block.x,day_block.y,day_block.width,day_block.height)){
				day_change_loop();
				return;
			}

			/* If the year block is selected, user can input value for year */
			else if(is_within_bounds(user_x,user_y,year_block.x,year_block.y,year_block.width,year_block.height)){
				year_change_loop();
				return;
			}

			/* If the minute block is selected, user can input value for minute */
			else if(is_within_bounds(user_x,user_y,minute_block.x,minute_block.y,minute_block.width,minute_block.height)){
				minute_change_loop();
				return;
			}

			/* If the hour block is selected, user can input value for hour */
			else if(is_within_bounds(user_x,user_y,hour_block.x,hour_block.y,hour_block.width,hour_block.height)){
				hour_change_loop();
				return;
			}

			/* If the next button is pressed when the date or time has not been changed,
			 * warn the user that nothing was changed, then go back to the menu */
			else if(is_within_bounds(user_x,user_y,next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Nothing was changed");
				HAL_Delay(1000);
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}

/**
 * @brief	Allows user to edit the day of the RTC date
 * @param	None
 * @retval	None
 */
void day_change_loop(){

	/* Initialize touch and buttons for the screen */
	TS_StateTypeDef TS_State;
	initialize_buttons();

	/* Set the index and accumulator to zero.
	 * Set day_block.selected to true, and all other button blocks to false */
	accum = 0;
	day_index = 0;
	day_block.selected = true;
	month_block.selected = year_block.selected = hour_block.selected = minute_block.selected = false;

	/* Set the font then clear the text over the day block to show that it's selected */
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	draw_button(day_block,true);

	/* While the day block is selected */
	while(day_block.selected){

		/* Get status of touch screen */
		BSP_TS_GetState(&TS_State);

		/* If touch is detected */
		if(TS_State.touchDetected){
			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];
			/* If clear button is pressed, clear the day block, reset the day_index and accumulator */
			if(numpad_poll_clr(pinpad, user_x, user_y)){
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				draw_button(day_block,true);
				day_index = 0;
				accum = 0;
			}

			/* If a number button is pressed store the integer value, display the
			 * value on the screen, then increment the index and update the accumulator */
			if(numpad_poll_num(pinpad,user_x,user_y,&num) && day_index<2){
				HAL_Delay(150);
				if(day_index == 0){
					day_val_tens = num;
					sprintf(day_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(105,70,(uint8_t *)day_buff,LEFT_MODE);
				}
				else if(day_index == 1){
					day_val_ones = num;
					sprintf(day_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(120,70,(uint8_t *)day_buff,LEFT_MODE);
				}
				day_index++;
				accum = (day_val_tens*10 + day_val_ones);

			}

			/* If the month block is pressed while the day block is selected,
			 * save new_day in storage and enter month_change loop */
			else if(is_within_bounds(user_x,user_y,month_block.x,month_block.y,month_block.width,month_block.height)){

				/* If accumulator is 0, that means nothing has been edited so
				 * go to the month change loop without saving anything */
				if(accum == 0){
					day_index = 0;
					day_changed = false;
					draw_date_time();
					month_change_loop();
				}

				/* If the day inputed by the user is greater than 31, tell the
				 * user it is an invalid day value and don't save that value */
				else if(!(accum<32)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid day value");
					HAL_Delay(1000);
					day_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				/* If the day inputed by the user is a valid number,
				 * save that day then enter the month loop */
				else{
					new_day = accum;
					day_changed = true;
					month_change_loop();
				}
			}

			/* If the year block is pressed while the day block is selected,
			 * save new_day in storage and enter year_change loop */
			else if(is_within_bounds(user_x,user_y,year_block.x,year_block.y,year_block.width,year_block.height)){

				if(accum == 0){
					day_index = 0;
					day_changed = false;
					draw_date_time();
					year_change_loop();
				}

				else if(!(accum<32)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid day value");
					HAL_Delay(1000);
					day_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				else{
					new_day = accum;
					day_changed = true;
					year_change_loop();
				}
			}

			/* If the hour block is pressed while the day block is selected,
			 * save new_day in storage and enter hour_change loop */
			else if(is_within_bounds(user_x,user_y,hour_block.x,hour_block.y,hour_block.width,hour_block.height)){
				if(accum == 0){
					day_changed = false;
					day_index = 0;
					draw_date_time();
					hour_change_loop();
				}

				else if(!(accum<32)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid day value");
					HAL_Delay(1000);
					day_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}

				else{
					new_day = accum;
					day_changed = true;
					hour_change_loop();
				}
			}

			/* If the minute block is pressed while the day block is selected,
			 * save new_day in storage and enter minute_change loop */
			else if(is_within_bounds(user_x,user_y,minute_block.x,minute_block.y,minute_block.width,minute_block.height)){

				if(accum ==0){
					day_index = 0;
					day_changed = false;
					draw_date_time();
					minute_change_loop();
				}
				else if(!(accum<32)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid day value");
					HAL_Delay(1000);
					day_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				else{
					new_day = accum;
					day_changed = true;
					minute_change_loop();
				}
			}

			/* If the next button is pressed while the day block is selected,
			 * check if the value inputed is valid. If it is, save it and update
			 * the time, then go back to the system settings page. Otherwise,
			 * tell the user it is an invalid day value and don't save it. */
			if(is_within_bounds(user_x,user_y,next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				if(!((0<accum)&&(accum<32))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid day value");
					HAL_Delay(1000);
					day_block.selected = false;
					day_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}
				else{
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Date/Time updated");
					new_day = accum;
					day_changed = true;
					update_time();
					handle_page_loop = system_settings_loop;
					HAL_Delay(1000);
					return;
				}
			day_block.selected = false;
			}

			/* If the back button is pressed, go back to system
			 * settings page without saving any changes made. */
			if(is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				day_block.selected = false;
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Changes not saved");
				HAL_Delay(1000);
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}

/**
 * @brief	Allows user to edit the month of the RTC date
 * @param	None
 * @retval	None
 */
void month_change_loop(){

	/* Initialize touch and buttons that will be used on the screen */
	TS_StateTypeDef TS_State;
	initialize_buttons();

	/* Set the index and accumulator to zero.
	 * Set month_block.selected to true, and all other button blocks to false */
	month_block.selected = true;
	day_block.selected = year_block.selected = hour_block.selected = minute_block.selected = false;
	accum = 0;
	month_index = 0;

	/* Set the font then clear the text over the day block to show that it's selected */
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	draw_button(month_block,true);

	/* While the month block is selected */
	while(month_block.selected){

		/* Get status of touch */
		BSP_TS_GetState(&TS_State);
		if(TS_State.touchDetected){

			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];

			/* If clear button is pressed, clear the month block, reset the month_index and accumulator */
			if(numpad_poll_clr(pinpad, user_x, user_y)){
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				draw_button(month_block,true);
				month_index = 0;
				accum = 0;
			}
			/* If a number button is pressed, store and display the integer
			 * value, update the accumulator, and increment the index. */
			if(numpad_poll_num(pinpad,user_x,user_y,&num)&&month_index<2){
				HAL_Delay(150);
				if(month_index == 0){
					month_val_tens = num;
					sprintf(month_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(35,70,(uint8_t *)month_buff,LEFT_MODE);
				}
				else if(month_index == 1){
					month_val_ones = num;
					sprintf(month_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(50,70,(uint8_t *)month_buff,LEFT_MODE);
				}
				month_index++;
				accum = (month_val_tens*10 + month_val_ones);

			}

			/* If the next button is pressed while the month block is selected,
			 * check if the value inputed is valid. If it is, save it and update
			 * the time, then go back to the system settings page. Otherwise,
			 * tell the user it is an invalid day value and don't save it. */
			if(is_within_bounds(user_x,user_y,next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				if(!((0<accum)&&(accum<13))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid month value");
					HAL_Delay(1000);
					month_block.selected = false;
					month_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Date/Time updated");
					HAL_Delay(1000);
					new_month = accum;
					month_changed = true;
					update_time();
					handle_page_loop = system_settings_loop;
					return;
				}
			}
			/* If the day block is pressed while the month block is selected,
			 * save new_month in storage and enter day_change block */
			else if(is_within_bounds(user_x,user_y,day_block.x,day_block.y,day_block.width,day_block.height)){

				/* If accumulator is 0, that means nothing has been edited so
				 * go to the month change loop without saving anything */
				if(accum == 0){
					month_index = 0;
					month_changed = false;
					draw_date_time();
					day_change_loop();
				}
				/* If the month inputed by the user is greater than 12, tell the
				 * user it is an invalid month value and don't save that value */
				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid month value");
					HAL_Delay(1000);
					month_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				/* If the month entered is valid, save that
				 * value and enter the day change loop */
				else{
					new_month = accum;
					month_changed = true;
					day_change_loop();
				}
			}

			/* If the hour block is pressed while the month block is selected,
			 * save new_month in storage and enter hour_change loop */
			else if(is_within_bounds(user_x,user_y,hour_block.x,hour_block.y,hour_block.width,hour_block.height)){
				if(accum == 0){
					month_changed = false;
					month_index = 0;
					draw_date_time();
					hour_change_loop();
				}
				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid month value");
					HAL_Delay(1000);
					month_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					new_month = accum;
					month_changed = true;
					hour_change_loop();
				}
			}

			/* If the year block is pressed while the month block is selected,
			 * save new_month in storage and enter year_change loop */
			else if(is_within_bounds(user_x,user_y,year_block.x,year_block.y,year_block.width,year_block.height)){

				if(accum == 0){
					month_index = 0;
					month_changed = false;
					draw_date_time();
					year_change_loop();
				}
				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid month value");
					HAL_Delay(1000);
					month_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				else{
					new_month = accum;
					month_changed = true;
					year_change_loop();
				}
			}

			/* If the minute block is pressed while the month block is selected,
			 * save new_month in storage and enter minute_change loop */
			else if(is_within_bounds(user_x,user_y,minute_block.x,minute_block.y,minute_block.width,minute_block.height)){

				if(accum == 0){
					month_index = 0;
					month_changed = false;
					draw_date_time();
					minute_change_loop();
				}
				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid month value");
					HAL_Delay(1000);
					month_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				else{
					new_month = accum;
					month_changed = true;
					minute_change_loop();
				}
			}

			/* If the back button is pressed, go back to system
			 * settings page without saving any changes made. */
			if(is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				day_block.selected = false;
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Changes not saved");
				HAL_Delay(1000);
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}

/**
 * @brief	Allows user to edit the day of the RTC date
 * @param	None
 * @retval	None
 */
void year_change_loop(){

    /* Initialize touch and buttons that will be used on the screen*/
    TS_StateTypeDef TS_State;
    initialize_buttons();

    /* Set the accumulator and year index to 0. Set year_block.selected to true,
     * and all other blocks.selected to false.  */
    accum = 0;
    year_index = 0;
    year_block.selected = true;
    month_block.selected = day_block.selected = hour_block.selected = minute_block.selected = false;

    /* Set the font and clear the text from the year block to
     * show that it is currently selected */
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    draw_button(year_block,true);

    /* While the year block is selected */
    while(year_block.selected){

    	/* Get status of touch */
        BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected){
        	/* Get x and y values of touch */
            uint16_t user_x = TS_State.touchX[0];
            uint16_t user_y = TS_State.touchY[0];

            /* If clear button is pressed, clear the year block
             * then reset the month_index and accumulator */
            if(numpad_poll_clr(pinpad, user_x, user_y)){
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                draw_button(year_block,true);
                year_index = 0;
                accum = 0;
            }

			/* If a number button is pressed, store and display the integer
			 * value, update the accumulator, and increment the index. */
            if(numpad_poll_num(pinpad,user_x,user_y,&num) && year_index < 4){
                HAL_Delay(200);
                if(year_index == 0){
                    year_val_thous = num;
                    sprintf(year_buff,"%i",num);
                    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                    BSP_LCD_DisplayStringAt(178,70,(uint8_t *)year_buff,LEFT_MODE);
                }
                else if(year_index == 1){
                    year_val_hunds = num;
                    sprintf(year_buff,"%i",num);
                    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                    BSP_LCD_DisplayStringAt(193,70,(uint8_t *)year_buff,LEFT_MODE);
                }
                else if(year_index == 2){
                    year_val_tens = num;
                    sprintf(year_buff,"%i",num);
                    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                    BSP_LCD_DisplayStringAt(208,70,(uint8_t *)year_buff,LEFT_MODE);
                }
                else if(year_index == 3){
                    year_val_ones = num;
                    sprintf(year_buff,"%i",num);
                    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                    BSP_LCD_DisplayStringAt(223,70,(uint8_t *)year_buff,LEFT_MODE);
                }
                year_index++;
                accum = (year_val_tens*10 + year_val_ones);
                year_check = (year_val_thous*1000 + year_val_hunds*100+accum);
            }

			/* If the month block is pressed while the year block is selected,
			 * save new_year in storage and enter month_change loop */
            else if(is_within_bounds(user_x,user_y,month_block.x,month_block.y,month_block.width,month_block.height)){

				/* If accumulator is 0, that means nothing has been edited so
				 * go to the month change loop without saving anything */
            	if(accum == 0){
					year_index = 0;
					year_changed = false;
					draw_date_time();
					month_change_loop();
				}

				/* If the year inputed by the user is less than 200 or greater
				 * than 2099, tell the user it is an invalid year value and
				 * don't save that value */
            	else if(!((1999<year_check)&&(year_check<2100))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid year value");
					HAL_Delay(1000);
					year_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;

				}

            	/* If the year inputed by the user is valid, save and
            	 * display that value, then go to the month loop */
				else{
					new_year = accum;
					year_changed = true;
					month_change_loop();
				}
            }

			/* If the day block is pressed while the year block is selected,
			 * save new_year in storage and enter day_change loop */
            else if(is_within_bounds(user_x,user_y,day_block.x,day_block.y,day_block.width,day_block.height)){

				if(accum == 0){
					year_changed = false;
					year_index = 0;
					draw_date_time();
					day_change_loop();
				}
				else if(!((1999<year_check)&&(year_check<2100))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid year value");
					HAL_Delay(1000);
					year_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}

				else{
					new_year = accum;
					year_changed = true;
					day_change_loop();
				}
			}

			/* If the minute block is pressed while the year block is selected,
			 * save new_year in storage and enter minute_change loop */
			else if(is_within_bounds(user_x,user_y,minute_block.x,minute_block.y,minute_block.width,minute_block.height)){

				if(accum == 0){
					year_index = 0;
					year_changed = false;
					draw_date_time();
					minute_change_loop();

				}
				else if(!((1999<year_check)&&(year_check<2100))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid year value");
					HAL_Delay(1000);
					year_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}

				else{
					new_year = accum;
					year_changed = true;
					minute_change_loop();
				}
			}

			/* If the hour block is pressed while the year block is selected,
			 * save new_year in storage and enter hour_change loop */
			else if(is_within_bounds(user_x,user_y,hour_block.x,hour_block.y,hour_block.width,hour_block.height)){
				if(accum == 0){
					year_changed = false;
					year_index = 0;
					draw_date_time();
					hour_change_loop();
				}
				else if(!((1999<year_check)&&(year_check<2100))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid year value");
					HAL_Delay(1000);
					year_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					new_year = accum;
					year_changed = true;
					hour_change_loop();
				}
			}

			/* If the next button is pressed while the year block is selected,
			 * check if the value inputed is valid. If it is, save it and update
			 * the time, then go back to the system settings page. Otherwise,
			 * tell the user it is an invalid day value and don't save it. */
            if(is_within_bounds(user_x,user_y,next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				if(!((1999<year_check)&&(year_check<2100))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid year value");
					HAL_Delay(1000);
					year_block.selected = false;
					year_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Date/Time updated");
					new_year = accum;
					year_changed = true;
					update_time();
					handle_page_loop = system_settings_loop;
					HAL_Delay(1000);
					return;
				}
			year_block.selected = false;
			}

            /* If the back button is pressed, go back to the system
             * settings loop without saving any changes */
            if(is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				year_block.selected = false;
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Changes not saved");
				HAL_Delay(1000);
				handle_page_loop = system_settings_loop;
				return;
			}
        }
    }
}

/**
 * @brief	Allows user to edit the day of the RTC date
 * @param	None
 * @retval	None
 */
void minute_change_loop(){

	/* Initialize touch and buttons for the screen*/
	TS_StateTypeDef TS_State;
	initialize_buttons();

	/* Set the accumulator and minute index to 0. Set minute_block.selected
	 * to true, and all other blocks.selected to false */
	accum = 0;
	minute_index = 0;
	minute_block.selected = true;
	month_block.selected = year_block.selected = hour_block.selected = day_block.selected = false;

	/* Set the font, and clear the text from the minute block
	 * to show that the block is currently selected */
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	draw_button(minute_block,true);

	/* While the minute block is selected */
	while(minute_block.selected){

		/* Get status of touch */
		BSP_TS_GetState(&TS_State);
		if(TS_State.touchDetected){

			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];

			/* If clear button is pressed, clear the block, reset the minute index and accumulator */
			if(numpad_poll_clr(pinpad, user_x, user_y)){
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				draw_button(minute_block,true);
				minute_index = 0;
				accum = 0;
			}
			/* If a number button is pressed, store and display the integer
			 * value, then update the accumulator and increment the index. */
			if(numpad_poll_num(pinpad,user_x,user_y,&num) && minute_index<2){
				HAL_Delay(150);
				if(minute_index == 0){
					minute_val_tens = num;
					sprintf(minute_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(112,145,(uint8_t *)minute_buff,LEFT_MODE);
				}
				else if(minute_index == 1){
					minute_val_ones = num;
					sprintf(minute_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(127,145,(uint8_t *)minute_buff,LEFT_MODE);
				}
				minute_index++;
				accum = (minute_val_tens*10 + minute_val_ones);

			}
			/* If the month block is pressed while the minute block is selected,
			 * save new_minute in storage and enter month_change loop */
			else if(is_within_bounds(user_x,user_y,month_block.x,month_block.y,month_block.width,month_block.height)){

				/* If accumulator is 0, that means nothing has been edited so
				 * go to the month change loop without saving anything */
				if(accum == 0){
					minute_changed = false;
					minute_index = 0;
					draw_date_time();
					month_change_loop();
				}

				/* If the minute inputed by the user is greater than 59, tell the
				 * user it is an invalid minute value and don't save that value */
				else if(!(accum<60)){
						BSP_LCD_Clear(LCD_COLOR_WHITE);
						BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
						BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid minute value");
						HAL_Delay(1000);
						minute_index = 0;
						accum = 0;
						draw(0,0,buffer,transparency,21);
						draw_date_time();
						return;

				}

				/* If the minute inputed is valid, save and display that
				 * number, then enter the month change loop */
				else{
					new_minute = accum;
					minute_changed = true;
					month_change_loop();
				}
			}
			/* If the hour block is pressed while the minute block is selected,
			 * save new_minute in storage and enter hour_change loop */
			else if(is_within_bounds(user_x,user_y,hour_block.x,hour_block.y,hour_block.width,hour_block.height)){

				if(accum == 0){
					minute_changed = false;
					minute_index = 0;
					draw_date_time();
					hour_change_loop();
				}
				else if(!(accum<60)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid minute value");
					HAL_Delay(1000);
					minute_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					new_minute = accum;
					minute_changed = true;
					hour_change_loop();
				}
			}
			/* If the year block is pressed while the minute block is selected,
			 * save new_minute in storage and enter year_change loop */
			else if(is_within_bounds(user_x,user_y,year_block.x,year_block.y,year_block.width,year_block.height)){

				if(accum == 0){
					minute_index = 0;
					minute_changed = false;
					draw_date_time();
					year_change_loop();
				}

				else if(!(accum<60)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid minute value");
					HAL_Delay(1000);
					minute_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;

				}

				else{
					new_minute = accum;
					minute_changed = true;
					year_change_loop();
				}
			}

			/* If the day block is pressed while the minute block is selected,
			 * save new_minute in storage and enter day_change loop */
            else if(is_within_bounds(user_x,user_y,day_block.x,day_block.y,day_block.width,day_block.height)){

				if(accum == 0){
					minute_index = 0;
					minute_changed = false;
					draw_date_time();
					day_change_loop();
				}

				else if(!(accum<60)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid minute value");
					HAL_Delay(1000);
					minute_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;

				}

				else{
					new_minute = accum;
					minute_changed = true;
					day_change_loop();
				}
			}

			/* If the next button is pressed while the minute block is selected,
			 * check if the value inputed is valid. If it is, save it and update
			 * the time, then go back to the system settings page. Otherwise,
			 * tell the user it is an invalid day value and don't save it. */
			if(is_within_bounds(user_x,user_y,next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				if(!((0<accum)&&(accum<60))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid minute value");
					HAL_Delay(1000);
					minute_block.selected = false;
					minute_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Date/Time updated");
					new_minute = accum;
					minute_changed = true;
					update_time();
					handle_page_loop = system_settings_loop;
					HAL_Delay(1000);
					return;
				}
			minute_block.selected = false;
			}

            /* If the back button is pressed, go back to the system
             * settings loop without saving any changes */
			if(is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				minute_block.selected = false;
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Changes not saved");
				HAL_Delay(1000);
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}

/**
 * @brief	Allows user to edit the day of the RTC date
 * @param	None
 * @retval	None
 */
void hour_change_loop(){

	/* Initialize touch and buttons that will be used for this page */
	TS_StateTypeDef TS_State;
	initialize_buttons();

	/* Set the accumulator and minute index to 0. Set hour_block.selected
	 * to true, and all other blocks.selected to false */
	accum = 0;
	hour_index = 0;
	hour_block.selected = true;
	month_block.selected = year_block.selected = minute_block.selected = day_block.selected = false;

	/* Set the font, and clear the text from the year block
	 * to show that the block is currently selected */
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	draw_button(hour_block,true);

	/* While the hour block is selected */
	while(hour_block.selected){

		/* Get touch status */
		BSP_TS_GetState(&TS_State);
		if(TS_State.touchDetected){

			/* Get x and y values of touch */
			uint16_t user_x = TS_State.touchX[0];
			uint16_t user_y = TS_State.touchY[0];

			/* If clear button is pressed, clear the hour block, reset the hour_index and accumulator */
			if(numpad_poll_clr(pinpad, user_x, user_y)){
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				draw_button(hour_block,true);
				hour_index = 0;
				accum = 0;
			}

			/* If a number button is pressed, store and display the integer
			 * value, update the accumulator, and increment the index. */
			if(numpad_poll_num(pinpad,user_x,user_y,&num) && hour_index<2){
				HAL_Delay(150);
				if(hour_index == 0){
					hour_val_tens = num;
					sprintf(hour_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(35,145,(uint8_t *)hour_buff,LEFT_MODE);
				}
				else if(hour_index == 1){
					hour_val_ones = num;
					sprintf(hour_buff,"%i",num);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(50,145,(uint8_t *)hour_buff,LEFT_MODE);
				}
				hour_index++;
				accum = (hour_val_tens*10 + hour_val_ones);

			}

			/* If the month block is pressed while the hour block is selected,
			 * save new_hour in storage and enter month_change loop */
			else if(is_within_bounds(user_x,user_y,month_block.x,month_block.y,month_block.width,month_block.height)){

				/* If accumulator is 0, that means nothing has been edited so
				 * go to the month change loop without saving anything */
				if(accum == 0){
					hour_changed = false;
					hour_index = 0;
					draw_date_time();
					month_change_loop();
				}

				/* If the hour inputed by the user is greater than 12, tell the
				 * user it is an invalid hour value and don't save that value */
				else if(!(accum<13)){
						BSP_LCD_Clear(LCD_COLOR_WHITE);
						BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
						BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid hour value");
						HAL_Delay(1000);
						hour_index = 0;
						accum = 0;
						draw(0,0,buffer,transparency,21);
						draw_date_time();
						return;

				}

				/* If the hour inputed is valid, save and display that
				 * number, then enter the month change loop */
				else{
					new_hour = accum;
					hour_changed = true;
					month_change_loop();
				}
			}

			/* If the year block is pressed while the hour block is selected,
			 * save new_hour in storage and enter year_change loop */
			else if(is_within_bounds(user_x,user_y,year_block.x,year_block.y,year_block.width,year_block.height)){

				if(accum == 0){
					hour_index = 0;
					hour_changed = false;
					draw_date_time();
					year_change_loop();
				}

				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid hour value");
					HAL_Delay(1000);
					hour_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;

				}

				else{
					new_hour = accum;
					hour_changed = true;
					year_change_loop();
				}
			}

			/* If the day block is pressed while the hour block is selected,
			 * save new_hour in storage and enter day_change loop */
            else if(is_within_bounds(user_x,user_y,day_block.x,day_block.y,day_block.width,day_block.height)){

				if(accum == 0){
					hour_index = 0;
					hour_changed = false;
					draw_date_time();
					day_change_loop();
				}

				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid hour value");
					HAL_Delay(1000);
					hour_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;

				}

				else{
					new_hour = accum;
					hour_changed = true;
					day_change_loop();
				}
			}

			/* If the minute block is pressed while the hour block is selected,
			 * save new_hour in storage and enter minute_change loop */
			else if(is_within_bounds(user_x,user_y,minute_block.x,minute_block.y,minute_block.width,minute_block.height)){

				if(accum == 0){
					hour_index = 0;
					hour_changed = false;
					draw_date_time();
					minute_change_loop();
				}
				else if(!(accum<13)){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid hour value");
					HAL_Delay(1000);
					hour_index = 0;
					accum = 0;
					draw_date_time();
					return;
				}
				else{
					new_hour = accum;
					hour_changed = true;
					minute_change_loop();
				}
			}

			/* If the next button is pressed while the hour block is selected,
			 * check if the value inputed is valid. If it is, save it and update
			 * the time, then go back to the system settings page. Otherwise,
			 * tell the user it is an invalid day value and don't save it. */
			if(is_within_bounds(user_x,user_y,next_btn.x,next_btn.y,next_btn.width,next_btn.height)){
				if(!((0<accum)&&(accum<13))){
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Invalid hour value");
					HAL_Delay(1000);
					hour_block.selected = false;
					hour_index = 0;
					accum = 0;
					draw(0,0,buffer,transparency,21);
					draw_date_time();
					return;
				}
				else{
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Date/Time updated");
					new_hour = accum;
					hour_changed = true;
					update_time();
					HAL_Delay(1000);
					handle_page_loop = system_settings_loop;
					return;
				}
			hour_block.selected = false;
			}

            /* If the back button is pressed, go back to the system
             * settings loop without saving any changes */
			if(is_within_bounds(user_x,user_y,back_btn.x,back_btn.y,back_btn.width,back_btn.height)){
				hour_block.selected = false;
				BSP_LCD_Clear(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAtLine(5,(uint8_t *)"Changes not saved");
				HAL_Delay(1000);
				handle_page_loop = system_settings_loop;
				return;
			}
		}
	}
}








