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
#include "configs.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"
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

typedef uint32_t tcolour;
typedef enum _state {MENU,GAME,NONE} State;
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
void debug(char * text);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void analogClock();
void printCountdown(double sec, tcolour colour);
int toPos(int index);
int toIndex(int pos);
int toButton(int posY);
void resetBoard();
void printFrame();
void printBoard();
void funcao(Coord move);
void play();
void selectSq(Coord sq);
void colourButton(int btn, int btnClr, int txtClr);
void convertColour(Coord enemy);
void printMenu();
pPoint createSkirt(pPoint skirt);
pPoint createTimeLeft(pPoint timeLeft, int polp4x, int polp4y);
void printMale(tcolour colour);
void printFemale(tcolour colour);
void checkMenuTS();
void checkGameTS();
void checkTIM();

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

#define PINK					  LCD_COLOR_MAGENTA
#define BLUE					  LCD_COLOR_CYAN

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
#define AIDELAY				   100
#define CLRDELAY			     1
#define CLRSPEED			     1

#define BCKGND					  LCD_COLOR_WHITE
#define BUTTONCLR				  LCD_COLOR_LIGHTGRAY
#define BUTTONTXTCLR			  LCD_COLOR_BLACK
#define PRESSEDBUTTONCLR		  LCD_COLOR_DARKGRAY
#define PRESSEDBUTTONTXTCLR		  LCD_COLOR_WHITE
#define SELECTEDCLR				  LCD_COLOR_DARKGRAY
#define SELECTEDDIF			 	  SQSIZE/5
#define ORIGOPT					 3

#define LCDXCENTRE				  BSP_LCD_GetXSize()/2
#define LCDYMAX					  BSP_LCD_GetYSize()

#define TIMEOUTSEC				20
#define TIMEOUTMAX				 3
#define CLCKCNTRX				70
#define CLCKCNTRY				70
#define CLCKRAD					50
#define CLCKSPEED			   -10
#define	CLOCKSPEEDBASE			 2
#define DANGERFR			  0.75
#define CLCKBKG					  LCD_COLOR_WHITE
#define CLCKFRAME				  LCD_COLOR_BLACK
#define DANGERCLR				  LCD_COLOR_RED

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
#define PL1CLR					  PINK
#define PL2CLR					  BLUE
#define E1CLR					  LCD_COLOR_DARKMAGENTA
#define E2CLR					  LCD_COLOR_DARKCYAN
#define BOARDCLR				  LCD_COLOR_BLACK
#define GRIDCLR					  LCD_COLOR_WHITE

#define R	 			0x00000100
#define G	 			0x00010000

#define BODYDIST			   285
#define HEADRAD				    30
#define HEADY				   175
#define LHEADX				      LCDXCENTRE-BODYDIST
#define RHEADX				      LCDXCENTRE+BODYDIST
#define BODYHEIGHT			   150
#define BODYWIDTH			    75
#define BODYY					  HEADY+HEADRAD+DECAP
#define RBODYX					  RHEADX-BODYWIDTH/2
#define TRIP1X					  LHEADX
#define TRIP1Y					  BODYY
#define TRIP2X					  LHEADX-BODYWIDTH
#define TRIP2Y					  BODYY+BODYHEIGHT
#define TRIP3X					  LHEADX+BODYWIDTH
#define TRIP3Y					  BODYY+BODYHEIGHT
#define DECAP					10


extern Content board[ROWS][COLS];
/* USER CODE END Private defines */



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
