
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

TIM_HandleTypeDef htim7;

SDRAM_HandleTypeDef hsdram1;









/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


typedef uint32_t tcolour;
typedef enum _state {MENU,GAME,NONE} State;
typedef enum _content {PINK,BLUE,PINKAVAIL,BLUEAVAIL,EMPTY} Content;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void debug(char * text);
int toPosX(int index);
int toPosY(int index);
int toIndexX(int pos);
int toIndexY(int pos);
int toButton(int posY);
void resetBoard();
void printFrame();
void printBoard();
void playAI(Coord move);
void play();
void swapPlayer();
void selectSq(Coord sq);
void colourButton(int btn, int btnClr, int txtClr);
void convertColour(Coord enemy);
void printMenu();
pPoint createSkirt(pPoint skirt);
pPoint createTimeLeft(pPoint timeLeft, int polp4x, int polp4y);
void printMale(tcolour colour);
void printFemale(tcolour colour);


/* USER CODE BEGIN EFP */


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

#define TEMP_REFRESH_PERIOD    250
#define MAX_CONVERTED_VALUE   4095
#define AMBIENT_TEMP            25
#define VSENS_AT_AMBIENT_TEMP  760
#define AVG_SLOPE               25
#define VREF                  3300

#define PINKCLR					  LCD_COLOR_MAGENTA
#define BLUECLR					  LCD_COLOR_CYAN

#define LCDXCNTR				  (BSP_LCD_GetXSize()/2)
#define LCDYMAX					  BSP_LCD_GetYSize()

#define NCONT					 5
#define SQSIZE					50
#define CIRRAD					20
#define BORDERX   				  (LCDXCNTR-SQSIZE*COLS/2)
#define BORDERY					  ((LCDYMAX-SQSIZE*ROWS)/2)
#define STRSIZE				   100
#define ELIPSEX				   100
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
#define FRAMECLR				  LCD_COLOR_GRAY
#define BUTTONCLR				  LCD_COLOR_LIGHTGRAY
#define BUTTONTXTCLR			  LCD_COLOR_BLACK
#define PRESSEDBUTTONCLR		  LCD_COLOR_DARKGRAY
#define PRESSEDBUTTONTXTCLR		  LCD_COLOR_WHITE
#define SELECTEDCLR				  LCD_COLOR_DARKGRAY
#define SELECTEDDIF			 	  (SQSIZE/5)
#define ORIGOPT					 3

#define TIMEOUTSEC				20
#define TIMEOUTMAX				 3
#define LCLCKCNTRX			   100
#define RCLCKCNTRX				  (LCDXCNTR*2-LCLCKCNTRX)
#define CLCKCNTRY			   100
#define CLCKRAD					50
#define CLCKSPEED			0.0001
#define DANGERFR			  0.75
#define CLCKBKG					  (LCD_COLOR_WHITE-1)
#define CLCKFRAME				  LCD_COLOR_BLACK
#define DANGERCLR				  LCD_COLOR_RED

#define ROWS 					 8
#define COLS 					 8
#define NOCOORD 				-2
//#define EMPTY					-1
//#define PINK					 0
//#define BLUE					 1
//#define PINKAVAIL					 2
//#define BLUEAVAIL					 3
#define EDIF					  PINKAVAIL-PINK
#define TRUE 					 1
#define FALSE 					 0
//#define PINKCLR					  PINKCLR
//#define BLUECLR					  BLUECLR
#define PINKAVAILCLR					  LCD_COLOR_DARKMAGENTA
#define BLUEAVAILCLR					  LCD_COLOR_DARKCYAN
#define BOARDCLR				  LCD_COLOR_BLACK
#define GRIDCLR					  LCD_COLOR_WHITE

#define R	 			0x00000100
#define G	 			0x00010000

#define BODYDIST			   285
#define HEADRAD				    30
#define HEADY				   175
#define LHEADX				      (LCDXCNTR-BODYDIST)
#define RHEADX				      (LCDXCNTR+BODYDIST)
#define BODYHEIGHT			   150
#define BODYWIDTH			    75
#define BODYY					  (HEADY+HEADRAD+DECAP)
#define RBODYX					  (RHEADX-BODYWIDTH/2)
#define TRIP1X					  LHEADX
#define TRIP1Y					  BODYY
#define TRIP2X					  (LHEADX-BODYWIDTH)
#define TRIP2Y					  (BODYY+BODYHEIGHT)
#define TRIP3X					  (LHEADX+BODYWIDTH)
#define TRIP3Y					  (BODYY+BODYHEIGHT)
#define DECAP					10


typedef struct _game{
	//Time totalTime;
	//Time playerTime[2];
	int totalTime;
	int playerTime[2];
	char playerName[2][STRSIZE];
	int score[2];
	int nPossMoves[2];
	int nTimeOut[2];
	Content player;
}Game;



#ifndef _ST_
#define _ST_

typedef struct _coord{
	int x;
	int y;
}Coord;

#endif


extern Content board[ROWS][COLS];
extern State mode;
extern _Bool ai;
extern _Bool ai2;
extern _Bool printFlag;
extern Content board[ROWS][COLS];
extern Coord touch;
extern Coord prev;
extern int btn;
extern Coord avail[ROWS*COLS];
extern Coord allEnemies[8];
extern int remain;
extern _Bool configFlag;
extern _Bool newGame;
extern _Bool clockFlag;
extern double clockAn;
extern _Bool redFlag;
extern int timCount;
extern _Bool timFlag;
extern _Bool pbFlag;
extern _Bool tsFlag;
extern _Bool dsFlag;
extern _Bool btnLeft;
extern _Bool personFlag;
extern int menuSize;
extern TS_StateTypeDef TS_State;
extern tcolour pieceClr[NCONT];
extern char menuOpt[ORIGOPT+1][STRSIZE];
extern uint32_t convertedValue;
extern long int degrees;
extern char temp[STRSIZE];
extern Game game;


/* USER CODE END Private defines */



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
