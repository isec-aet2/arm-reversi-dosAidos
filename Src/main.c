
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"
#include "stm32f7xx_hal.h"


#include "configs.h"
#include "proj1.h"
#include "interrupts.h"
#include "clocks.h"
#include "converters.h"
#include "display.h"

#ifndef _ST_
#define _ST_

//typedef int tcolour;
//typedef enum _state {MENU,GAME} State;
//typedef enum _content {PINK,BLUE,PINKAVAIL,BLUEAVAIL,EMPTY} Content;

typedef struct _coord{
	int x;
	int y;
}Coord;

//typedef struct _time{
//	int sec;
//	int min;
//	int hour;
//}Time;

//typedef struct __game{
	//Time game.totalTime;
	//Time game.playerTime[2];
//	char game.playerName[2][STRSIZE];
//	int score[2];
//	int nPossMoves[2];
//	int nTimeOut[2];
//	_Bool game.player;
//}GameInfo;



#endif

//typedef struct _game{
//	//Time game.totalTime;
//	//Time game.playerTime[2];
//	//char game.playerName[2][STRSIZE];
//	int score[2];
//	int nPossMoves[2];
//	int nTimeOut[2];
//	int game.player;
//}Game;

tcolour pieceClr[] = {PINKCLR,BLUECLR,LCD_COLOR_DARKMAGENTA,LCD_COLOR_DARKCYAN};

int timCount = 0;
_Bool timFlag = 0;
_Bool pbFlag = 0;
_Bool tsFlag = 0;
_Bool dsFlag = 0;
_Bool btnLeft = 0;
_Bool personFlag = 0;
int menuSize = ORIGOPT;
TS_StateTypeDef TS_State;

State mode = MENU;
_Bool ai;
_Bool ai2;
_Bool printFlag = 1;
Content board[ROWS][COLS];
Coord touch;
Coord prev;
int btn;
Coord avail[ROWS*COLS];
Coord allEnemies[8];
int remain = 0;
_Bool configFlag = 0;
_Bool newGame = 1;
_Bool clockFlag = 0;
double clockAn = 0;
_Bool redFlag = 0;

Game game;
//game.totalTime = 0;
//game.playerTime[] = {0,0};
//game.score[] = {2,2};
//game.nPossMoves[] = {4,4};
//game.nTimeOut[] = {0,0};


#ifdef _ST_

//struct _game game;
//game.totalTime = 0;
//game.playerTime = {0,0};
//game.playerName = {"PINKCLR","BLUECLR"};
//char game.playerName[2][STRSIZE];
//int score[2];
//int nPossMoves[2];
//int nTimeOut[2];
//_Bool game.player;



#endif


char menuOpt[][STRSIZE] = {"NI vs AI","NI vs NI","AI vs AI","Resume game"};

uint32_t convertedValue;
long int degrees;
char temp[STRSIZE];





void debug(char * text){
	BSP_LCD_SetBackColor(BCKGND);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, LCDYMAX/2, (uint8_t *)text, RIGHT_MODE);
}



//Time toTime(int total){
//	Time time;
//	time.hour = (total%60)%60;
//	total /= 60;
//	time.min = total%60;
//	time.sec = total/60;
//	return time;
//}




void swapPlayer(){
	game.player = !game.player;
	resetClock();

}

void playAI(Coord move){
	Coord pass;
	pass.x = rand()%ROWS;
	pass.y = rand()%COLS;
	selectSq(pass);
	HAL_Delay(AIDELAY);
	int difX = (move.x>pass.x)*2-1;
	int difY = (move.y>pass.y)*2-1;
	while(pass.x!=move.x || pass.y!=move.y){
		if(pass.x!=move.x){
			if(!(rand()%3)){
				pass.x += difX;
			}
		}
		if(pass.y!=move.y){
			if(!(rand()%3)){
				pass.y += difY;
			}
		}
		printBoard();
		selectSq(pass);
		HAL_Delay(AIDELAY);
	}
}

void play(){
	checkTIM6();
	checkTIM7();
	if(checkPB()){
		//return;
	}
	board[touch.x][touch.y] = game.player;
	resetArray(allEnemies,8);
	exposeAllEnemies(touch,game.player,allEnemies);
	for(int i=0; allEnemies[i].x!=NOCOORD; i++){ //converts all the trapped enemies into own's symbols
		theConverter(allEnemies[i],touch,game.player,1);
	}
	//game.player = !game.player;
	//clockAn = 0;
	swapPlayer();
	remain = checkAllMoves(game.player,avail);
	printBoard();
	if(!remain){
		resetBoard();
		mode = MENU;
		menuSize = ORIGOPT;
		printFlag = 1;
		remain = 0;
	}
	if((ai && game.player==ai) || ai2){
		touch = chooseMove(avail,remain,allEnemies,game.player);
		playAI(touch);
		play();
	}
}


void configs(){
	srand(time(NULL));

	SCB_EnableICache();

	SCB_EnableDCache();

	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA2D_Init();
	MX_DSIHOST_DSI_Init();
	MX_FMC_Init();
	MX_LTDC_Init();
	MX_TIM6_Init();
	MX_TIM7_Init();
	MX_ADC1_Init();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND,LCD_FB_START_ADDRESS);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_PB_Init(0,1);

	BSP_TS_Init(800,480);
	BSP_TS_ITConfig();

	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_ADC_Start_IT(&hadc1);

	BSP_LED_Init(LED_RED);
	BSP_LED_Init(LED_GREEN);

	configFlag = 1;
}

void initGame(){
	game.totalTime = 0;
	game.playerTime[PINK] = game.playerTime[BLUE] = 0;
	strcpy(game.playerName[PINK], "Pink");
	strcpy(game.playerName[BLUE], "Blue");
	game.score[PINK] = game.score[BLUE] = 2;
	game.nPossMoves[PINK] = game.nPossMoves[BLUE] = 4;
	game.nTimeOut[PINK] = game.nTimeOut[BLUE] = 0;
	game.player = PINK;
}


int main(void)
{

	if(!configFlag){
		configs();
	}

	game.player = PINK;

	while (1)
	{
		checkPB();
		checkTIM6();
		checkTIM7();
		switch(mode){
		case MENU:
			if(printFlag){
				BSP_LCD_Clear(BCKGND);
				printMenu();
				printMale(BLUECLR);
				printFemale(PINKCLR);
				printFlag = 0;
			}
			checkMenuTS();
			break;
		case GAME:
			if(printFlag){
				BSP_LCD_Clear(BCKGND);
				printBoard();
				resetClock();
				printFlag = 0;
			}
			checkGameTS();
//			analogClock(CLCKBKG,RCLCKCNTRX);
//			printClock(LCLCKCNTRX);
//			game.player = !game.player;
//			//analogClock(CLCKBKG,RCLCKCNTRX);
//			printClock(RCLCKCNTRX);
//			game.player = !game.player;
		}
	}
}
