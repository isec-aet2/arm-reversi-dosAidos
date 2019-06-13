
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"
#include "stm32f7xx_hal.h"

#include "main.h"
#include "configs.h"
#include "proj1.h"
#include "interrupts.h"
#include "clocks.h"
#include "converters.h"
#include "display.h"
#include "game.h"

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
typedef enum _mode {MENU,GAME,END,NONE} tmode;
#ifndef _TC_
#define _TC_
typedef enum _content {PINK,BLUE,PINKAVAIL,BLUEAVAIL,EMPTY} tcontent;
#endif
#ifndef _TYPES_
#define _TYPES_
typedef enum _side {LEFT, RIGHT} tside;
typedef enum _body {FEMALE,MALE} tbody;
#endif

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

void debug(char * text);
void configs();


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

#define LCDXCNTR				  (800/2)
#define LCDYMAX					  480

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
#define ENDFONT				      Font24
#define INFOFONT				  Font12
#define INFOCLR					  LCD_COLOR_BLACK
#define INFOXBORDER				30
#define INFOYBORDER				20
#define NINFO1					 1
#define NINFO2					 5
#define YINFO					  (CLCKCNTRY+CLCKRAD)
#define LINFOT					  INFOXBORDER
#define RINFOT					  (LCDXCNTR+SQSIZE*COLS/2+RINFO)
#define LINFO					  RINFOT
#define RINFO					  LINFOT

#define TOUCHDELAY				12
#define AIDELAY				   100
#define CLRDELAY			     1
#define CLRSPEED			     1

#define BCKGND					  LCD_COLOR_WHITE
#define FRAMECLR				  BCKGND
#define BUTTONCLR				  LCD_COLOR_LIGHTGRAY
#define BUTTONTXTCLR			  LCD_COLOR_BLACK
#define PRESSEDBUTTONCLR		  LCD_COLOR_DARKGRAY
#define PRESSEDBUTTONTXTCLR		  LCD_COLOR_WHITE
#define SELECTEDCLR				  LCD_COLOR_DARKGRAY
#define SELECTEDDIF			 	  (SQSIZE/5)
#define ORIGOPT					 3
#define TRANSSEC				 2

#define TIMEOUTSEC				20
#define TIMEOUTMAX				 3
#define LCLCKCNTRX			   	  (BORDERX/2)
#define RCLCKCNTRX				  (LCDXCNTR*2-LCLCKCNTRX)
#define CLCKCNTRY			   100
#define CLCKRAD					55
#define CLCKSPEED			0.0001
#define DANGERFR			  0.75
#define CLCKBKG					  (LCD_COLOR_WHITE-1)
#define CLCKFRAME				  (LCD_COLOR_BLACK+1)
#define DANGERCLR				  LCD_COLOR_RED

#define ROWS 					 8
#define COLS 					 8
#define NOCOORD 				-2
#define AVAILDIF		    	  (PINKAVAIL-PINK)
#define TRUE 					 1
#define FALSE 					 0
#define PINKCLR					  LCD_COLOR_MAGENTA
#define BLUECLR					  LCD_COLOR_CYAN
#define PINKAVAILCLR			  LCD_COLOR_DARKMAGENTA
#define BLUEAVAILCLR			  LCD_COLOR_DARKCYAN
#define BOARDCLR				  LCD_COLOR_BLACK
#define GRIDCLR					  LCD_COLOR_WHITE
#define SBCLR					  LCD_COLOR_WHITE

#define R	 			0x00000100
#define G	 			0x00010000

#define BODYDIST			   250
#define HEADRAD				    30
#define HEADY				   175
#define LHEADX				      (LCDXCNTR-BODYDIST)
#define RHEADX				      (LCDXCNTR+BODYDIST)
#define BODYHEIGHT			   150
#define BODYWIDTH			    75
#define BODYY					  (HEADY+HEADRAD+DECAP)
#define LBODYX					  (LHEADX-BODYWIDTH/2)
#define RBODYX					  (RHEADX-BODYWIDTH/2)
#define LTRIP1X					  LHEADX
#define LTRIP2X					  (LHEADX-BODYWIDTH)
#define LTRIP3X					  (LHEADX+BODYWIDTH)
#define RTRIP1X					  RHEADX
#define RTRIP2X					  (RHEADX-BODYWIDTH)
#define RTRIP3X					  (RHEADX+BODYWIDTH)
#define TRIP1Y					  BODYY
#define TRIP2Y					  (BODYY+BODYHEIGHT)
#define TRIP3Y					  (BODYY+BODYHEIGHT)
#define DECAP					10

#define CONVERT					 1
#define HEAD					 1
#define NEWMOVE					 1
#define TIMEOUT					 1
#define DANGER					 1


typedef struct _game{
	//Time totalTime;
	//Time playerTime[2];
	int totalTime;
	int playerTime[2];
	char playerName[2][STRSIZE];
	int score[2];
	int nPossMoves[2];
	int nTimeOut[2];
	tcontent player;
}Game;

#ifndef _TIME_
#define _TIME_
typedef struct _time{
	int sec;
	int min;
	int hour;
}Time;
#endif

#ifndef _ST_
#define _ST_

typedef struct _coord{
	int x;
	int y;
}Coord;

#endif

extern uint32_t convertedValue;
extern long int degrees;
extern char temp[STRSIZE];
extern tmode mode;
extern _Bool printFlag;
extern Coord touch;
extern Coord prev;
extern tside thisSide;
extern tbody thisBody;
extern int iClr;
extern int iAI;
extern _Bool aiFlag;
extern _Bool ai2Flag;
extern int btn;
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
extern int personFlag;
extern tcolour touchClr;
extern int menuSize;
extern TS_StateTypeDef TS_State;
extern tcolour pieceClr[NCONT];
extern char menuOpt[ORIGOPT+1][STRSIZE];
extern tcontent board[ROWS][COLS];
extern Coord avail[ROWS*COLS];
extern Coord targets[8];
extern int clckcntrX[2];
extern int headX[2];
extern int bodyX[2];
extern int infotX[2];;
extern int infoX[2];
extern Point ltrip1;
extern Point ltrip2;
extern Point ltrip3;
extern Point rtrip1;
extern Point rtrip2;
extern Point rtrip3;
extern Point skirt[2][3];
extern tbody bodyDisp[2];
extern Game game;
extern char head1[NINFO1][STRSIZE];
extern char head2[NINFO2][STRSIZE];
extern char info1[NINFO1][STRSIZE];
extern char info2[NINFO2][2][STRSIZE];
extern int sizeSB;

/* USER CODE END Private defines */



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
