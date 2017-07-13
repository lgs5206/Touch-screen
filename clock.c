/*
 * clock.c
 *
 *  Created on: Jul 6, 2017
 *      Author: Lennon S.
 */


#include "clock.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"
#include "stm32f7xx_hal_rtc.h"
#include "stm32746g_discovery_lcd.h"

#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */

RTC_HandleTypeDef RtcHandle;
RTC_InitTypeDef RTC_init;
RTC_TimeTypeDef RTC_time;
RTC_DateTypeDef RTC_date;

uint8_t seconds,minutes,hours,month,day,year;
char sec[30];
char min[30];
char hr[30];
char mon[30];
char dy[30];
char yr[30];


void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Configure LSE as RTC clock source ##################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return;
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    return;
  }

  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();
}


void clock_SaveParameter(uint32_t address, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&RtcHandle,address,data);
}

uint32_t clock_RestoreParameter(uint32_t address)
{
   return HAL_RTCEx_BKUPRead(&RtcHandle,address);
}


void clock_GetTime(  RTC_TimeTypeDef *Time)
{
   HAL_RTC_GetTime(&RtcHandle, Time, FORMAT_BIN);
}


void clock_SetTime(RTC_TimeTypeDef *Time,uint8_t seconds,uint8_t minutes,uint8_t hours)
{
	Time->StoreOperation = 0;
	Time->SubSeconds = 0;
	Time->DayLightSaving = 0;
	Time->Seconds = seconds;
	Time->Minutes = minutes;
	Time->Hours = hours;
	HAL_RTC_SetTime(&RtcHandle, Time, FORMAT_BIN);
}

void set_hour(RTC_TimeTypeDef *Time,uint8_t hour){

	Time->Hours = hour;
	HAL_RTC_SetTime(&RtcHandle, Time, FORMAT_BIN);
}

void set_minute(RTC_TimeTypeDef *Time,uint8_t minute){

	Time->Minutes = minute;
	HAL_RTC_SetTime(&RtcHandle, Time, FORMAT_BIN);
}

void set_second(RTC_TimeTypeDef *Time,uint8_t second){
	Time->Seconds = second;
	HAL_RTC_SetTime(&RtcHandle, Time, FORMAT_BIN);
}


void clock_GetDate(  RTC_DateTypeDef *Date)
{
   HAL_RTC_GetDate(&RtcHandle, Date, FORMAT_BIN);

   if((Date->Date == 0) || (Date->Month == 0))
   {
     Date->Date = Date->Month = 1;
   }
}

void set_year(RTC_DateTypeDef *Date, uint8_t year){

	Date->Year = year;
	HAL_RTC_SetDate(&RtcHandle, Date, FORMAT_BIN);
}

void set_month(RTC_DateTypeDef *Date, uint8_t month){

	Date->Month = month;
	HAL_RTC_SetDate(&RtcHandle, Date, FORMAT_BIN);
}

void set_day(RTC_DateTypeDef *Date, uint8_t day){

	Date->Date = day;
	HAL_RTC_SetDate(&RtcHandle, Date, FORMAT_BIN);
}


void clock_SetDate(  RTC_DateTypeDef *Date, uint8_t month, uint8_t day, uint8_t year)
{
	Date->Date = day;
	Date->Month = month;
	Date->Year = year;
	HAL_RTC_SetDate(&RtcHandle, Date, FORMAT_BIN);
}


uint8_t get_seconds(RTC_TimeTypeDef *Time){

	return Time->Seconds;
}

uint8_t get_minutes(RTC_TimeTypeDef *Time){

	return Time->Minutes;
}
uint8_t get_hours(RTC_TimeTypeDef *Time){

	return Time->Hours;
}


void clockInit(void){

	RtcHandle.Instance = RTC;
	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_12;
	RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
	  {
	  }
}

void clockDeinit(void){
	HAL_RTC_DeInit(&RtcHandle);
}

void draw_time(){

	clock_GetTime(&RTC_time);
	clock_GetDate(&RTC_date);

	seconds = RTC_time.Seconds;
	minutes = RTC_time.Minutes;
	hours = RTC_time.Hours;

	snprintf(sec,30, "%d",seconds);
	snprintf(min,30,"%d",minutes);
	snprintf(hr,30,"%d",hours);

	day = RTC_date.Date;
	month = RTC_date.Month;
	year = RTC_date.Year;

	snprintf(dy,30,"%d",day);
	snprintf(mon,30,"%d",month);
	snprintf(yr,30,"%d",year);

	char buff[30];
	if(month < 10){
		strcpy(buff,"0");
		strcat(buff,mon);
	}
	else{
		strcpy(buff,mon);
	}
	strcat(buff,"/");
	if(day < 10){
		strcat(buff,"0");
		strcat(buff,dy);
	}
	else{
		strcat(buff,dy);
	}
	strcat(buff,"/");
	strcat(buff,"20");
	if(year < 10){
		strcat(buff,"0");
		strcat(buff,yr);
	}
	else{
		strcat(buff,yr);
	}
	strcat(buff," ");
	if(hours < 10){
		strcat(buff,"0");
		strcat(buff,hr);
	}
	else{
		strcat(buff,hr);
	}
	strcat(buff,":");
	if(minutes < 10){
		strcat(buff,"0");
		strcat(buff,min);
	}
	else{
		strcat(buff,min);
	}
	strcat(buff,":");
	if(seconds < 10){
		strcat(buff,"0");
		strcat(buff,sec);
	}
	else{
		strcat(buff,sec);
	}
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(4,5,(uint8_t *)buff,RIGHT_MODE);
}
