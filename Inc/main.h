/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "proj1.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
ADC_HandleTypeDef hadc1;

DMA2D_HandleTypeDef hdma2d;

DSI_HandleTypeDef hdsi;

LTDC_HandleTypeDef hltdc;

TIM_HandleTypeDef htim6;

SDRAM_HandleTypeDef hsdram1;


#ifndef _Coord_
#define _Coord_

typedef struct _coord{
	int x;
	int y;
}Coord;

#endif

typedef int tcolour;
typedef enum _state {MENU,GAME} State;
typedef enum _content {PL1,PL2,E1,E2,EMPTY} Content;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//extern int board[ROWS][COLS];
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

#define TEMP_REFRESH_PERIOD    250
#define MAX_CONVERTED_VALUE   4095
#define AMBIENT_TEMP            25
#define VSENS_AT_AMBIENT_TEMP  760
#define AVG_SLOPE               25
#define VREF                  3300

#define SQSIZE					55
#define CIRRAD					20
#define BORDER					  (LCDYMAX-SQSIZE*ROWS)/2
#define STRSIZE				   100
#define ELIPSEX				   150
#define ELIPSEY				    30

#define TEMPCLR					  LCD_COLOR_GRAY
#define TEMPFONT				  Font8

#define MFONT					  Font24
#define MFONTSIZE				24

#define TOUCHDELAY				12

#define BCKGND					  LCD_COLOR_WHITE
#define BUTTONCLR				  LCD_COLOR_DARKMAGENTA
#define BUTTONTXTCLR			  LCD_COLOR_LIGHTYELLOW
#define PRESSEDBUTTONCLR		  LCD_COLOR_BLACK
#define PRESSEDBUTTONTXTCLR		  LCD_COLOR_GRAY
#define SELECTEDCLR				  LCD_COLOR_DARKGRAY
#define SELECTEDDIF			 	  SQSIZE/5

#define LCDXCENTRE				  BSP_LCD_GetXSize()/2
#define LCDYMAX					  BSP_LCD_GetYSize()

#define TIMEOUTSEC				20
#define TIMEOUTMAX				 3
#define CLCKCNTRX				70
#define CLCKCNTRY				70
#define CLCKRAD					70
#define CLCKNOSE				 3

#define ROWS 					 8
#define COLS 					 8
#define NOCOORD 				-2
//#define EMPTY					-1
//#define PL1					 0
//#define PL2					 1
//#define E1					 2
//#define E2					 3
#define EDIF					  E1-PL1
#define TRUE 					 1
#define FALSE 					 0
#define PL1CLR					  LCD_COLOR_LIGHTMAGENTA
#define PL2CLR					  LCD_COLOR_CYAN
#define E1CLR					  LCD_COLOR_DARKMAGENTA
#define E2CLR					  LCD_COLOR_DARKCYAN
#define BOARDCLR				  LCD_COLOR_BLACK
#define GRIDCLR					  LCD_COLOR_WHITE

#define HEADDIST			   285
#define HEADRAD				    50
#define HEADY				   175
#define LHEADX				      LCDXCENTRE-HEADDIST
#define RHEADX				      LCDXCENTRE+HEADDIST



extern Content board[ROWS][COLS];
/* USER CODE END Private defines */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
