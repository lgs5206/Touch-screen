/*
 *  sd_card_functions.c
 *
 *  Created on: Jun 26, 2017
 *      Author: Lennon S.
 */

/* Includes */
#include "main.h"
#include "header.h"

/* Variables */
FATFS SD_FatFs;  								/* File system object for SD card logical drive */
char SD_Path[4]; 								/* SD card logical drive path 					*/
char* pDirectoryFiles[MAX_BMP_FILES];			/* Max number of bitmap files: 25 				*/
uint8_t  ubNumberOfFiles = 0;					/* Number of bitmaps on SD card 				*/
uint32_t uwBmplen = 0;							/* Length of particular bitmap 					*/
uint32_t transparency = 255;					/* Transparency value: 0 = Fully transparent	*/
uint8_t buffer[30];								/* 30 Byte buffer								*/
uint8_t *uwInternelBuffer;						/* Pointer to internal bitmap buffer			*/
uint32_t counter = 0;							/* Variable to iterate through bitmaps			*/

/*Function Definitions */
/************************************************************************************************/


/**
  * @brief  Initializes and reads SD card
  * @param	None
  * @retval Number of bitmap files on SD card
  */
int read_SD_card(void)
{
	/*Initializes bitmap buffer to location in memory */
	uwInternelBuffer = (uint8_t *)0xC0260000;

	/* Display a message if SD card is not present */
	while(BSP_SD_IsDetected() != SD_PRESENT)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Please insert SD Card                  ");
	}

	/* If SD card is detected, clear the screen */
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	/* Link the SD Card disk I/O driver */
	if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
	{
	/* Initialize the Directory Files pointers */
	for (counter = 0; counter < MAX_BMP_FILES; counter++)
	{
	  pDirectoryFiles[counter] = malloc(MAX_BMP_FILE_NAME);

	  /* If memory cannot be allocated, display message */
	  if(pDirectoryFiles[counter] == NULL)
	  {
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Cannot allocate memory ");

		/* Endless loop */
		while(1)
		{
		}
	  }
	}

	/**
	* If SD card is properly initialized, proceed to read it
	* Get the BMP file names on root directory
	*/
	ubNumberOfFiles = Storage_GetDirectoryBitmapFiles("/Media", pDirectoryFiles);

	/* If SD card does not contain bitmaps, display message */
	if (ubNumberOfFiles == 0)
	{
	  for (counter = 0; counter < MAX_BMP_FILES; counter++)
	  {
		free(pDirectoryFiles[counter]);
	  }
	  BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  No Bitmap files...      ");
	  while(1)
	  {
	  }
	}
	}
	else
	{
	/* FatFs Initialization Error */
	}
	return ubNumberOfFiles;
}

/**
  * @brief  Draws a bitmap from the SD card. Bitmap files must be in folder "Media" on root
  * @param	x: X coordinate of upper left corner of bitmap
  * @param	y: Y coordinate of upper left corner of bitmap
  * @param	buffer: 30 Byte buffer
  * @param	transparency: Transparency value of bitmap, 0 = fully transparent, 255 = fully opaque
  * @param 	counter: Number representing the bitmap's index on SD card, 0 = first file
  * @retval None
  */
void draw(uint32_t x,uint32_t y,uint8_t buffer[30],uint32_t transparency,uint32_t counter)
{
    /* Format the string */
    sprintf ((char*)buffer, "Media/%-11.11s", pDirectoryFiles[counter]);

    /* If bitmap file is properly copied to buffer, display it on LCD frame buffer */
    if (Storage_CheckBitmapFile((const char*)buffer, &uwBmplen) == 0)
    {
        /* Format the string */
        sprintf ((char*)buffer, "Media/%-11.11s", pDirectoryFiles[counter]);

        /* Set LCD foreground Layer */
        BSP_LCD_SelectLayer(1);

        /* Open a file and copy its content to an internal buffer */
        Storage_OpenReadFile(uwInternelBuffer, (const char*)buffer);

        /* Write bmp file on LCD frame buffer */
        BSP_LCD_DrawBitmap(x, y, uwInternelBuffer);

        /* Set transparency to fully opaque */
        BSP_LCD_SetTransparency(1, transparency);
    }
    /* If bitmap is not properly copied, display error message */
    else if(Storage_CheckBitmapFile((const char*)buffer, &uwBmplen) == 1)
    {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);

        BSP_LCD_DisplayStringAtLine(7, (uint8_t *) buffer);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. ");
        while(1)
        {
        }
    }
}
