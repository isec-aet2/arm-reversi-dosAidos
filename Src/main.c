
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

#ifndef _Coord_
#define _Coord_

typedef struct _coord{
	int x;
	int y;
}Coord;

typedef struct _game{
	int number;
	_Bool winner;
	int time;
}Game;

typedef int tcolour;
typedef enum _state {MENU,GAME} State;
typedef enum _content {PL1,PL2,E1,E2,EMPTY} Content;

#endif


tcolour contClr[] = {LCD_COLOR_LIGHTMAGENTA,LCD_COLOR_CYAN,LCD_COLOR_DARKMAGENTA,LCD_COLOR_DARKCYAN};

int timCount = 0;
_Bool timFlag = 0;

_Bool tsFlag = 0;
_Bool dsFlag = 0;
_Bool btnLeft = 0;
_Bool personFlag = 0;
int menuSize = 2;
TS_StateTypeDef TS_State;

State mode = MENU;
_Bool ai;
_Bool printFlag = 1;
Content board[ROWS][COLS];
_Bool player = PL1;
Coord touch;
Coord prev;
int btn;
Coord allEnemies[8];
_Bool end = 0;

char menuOpt[][STRSIZE] = {"Play against AI","Play against NI","Resume game"};

uint32_t ConvertedValue;
long int JTemp;
char desc[STRSIZE];




void debug(char * text){
	BSP_LCD_SetBackColor(BCKGND);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, LCDYMAX/2, (uint8_t *)text, RIGHT_MODE);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
	if(htim -> Instance == TIM6){
		timCount++;
		timFlag = 1;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		BSP_TS_GetState(&TS_State);
		if(TS_State.touchDetected){
			tsFlag = 1;
		}
	}
	if(GPIO_Pin == GPIO_PIN_0){
		mode = MENU;
	}
}

void analogClock(int colour){
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillCircle(CLCKCNTRX, CLCKCNTRY, CLCKRAD);
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_DrawCircle(CLCKCNTRX, CLCKCNTRY, CLCKRAD);
	BSP_LCD_FillCircle(CLCKCNTRX, CLCKCNTRY, CLCKNOSE);
}

void printCountdown(int sec, int colour){
	int angle = 360*sec/TIMEOUTSEC;
	int catX = CLCKRAD*(int)sin(angle);
	int catY = CLCKRAD*(int)cos(angle);
	if(angle>180){
		catX *= -1;
	}
	if(angle>90 && angle<270){
		catY *= -1;
	}
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_DrawLine(CLCKCNTRX, CLCKCNTRY, CLCKCNTRX+catX, CLCKCNTRY+catY);
}

int toPos(int index){
	return SQSIZE*index+BORDER;
}

int toIndex(int pos){
	return (pos-BORDER)/SQSIZE;
}

int toButton(int posY){
	return posY*menuSize/LCDYMAX;
}

void resetBoard(){
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			if((i==ROWS/2-1 && j==COLS/2-1) || (i==ROWS/2 && j==COLS/2)){
				board[i][j] = PL1;
			}else if((i==ROWS/2 && j==COLS/2-1) || (i==ROWS/2-1 && j==COLS/2)){
				board[i][j] = PL2;
			}else{
				board[i][j] = EMPTY;
			}
		}
	}
}

void printFrame(){
	BSP_LCD_SetTextColor(BCKGND);
	BSP_LCD_FillRect(0, 0, SQSIZE*ROWS+2*BORDER, BORDER);
	BSP_LCD_FillRect(0, 0, BORDER, SQSIZE*COLS+2*BORDER);
	BSP_LCD_FillRect(0, SQSIZE*COLS+BORDER+1, SQSIZE*ROWS+2*BORDER, BORDER);
	BSP_LCD_FillRect(SQSIZE*ROWS+BORDER+1, 0, BORDER, SQSIZE*COLS+2*BORDER);
}

void printBoard(){
	Content sq;
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(BORDER, BORDER, SQSIZE*ROWS, SQSIZE*COLS);
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			sq = board[i][j];
			BSP_LCD_SetTextColor(GRIDCLR);
			BSP_LCD_DrawRect(toPos(i), toPos(j), SQSIZE, SQSIZE);
			if(sq<=PL2){
				BSP_LCD_SetTextColor(contClr[sq]);
				BSP_LCD_FillCircle(toPos(i)+SQSIZE/2.0, toPos(j)+SQSIZE/2.0, CIRRAD);
				//continue;
			}else if(sq<=PL2+EDIF){
				BSP_LCD_SetTextColor(contClr[sq]);
				BSP_LCD_DrawCircle(toPos(i)+SQSIZE/2.0, toPos(j)+SQSIZE/2.0, CIRRAD);
			}
		}
	}
	printFrame();
}

void play(){
	if(player==ai){
		//touch = chooseMove();
	}
	board[touch.x][touch.y] = player;
	resetArray(allEnemies,8);
	exposeAllEnemies(touch,player,allEnemies);
	for(int i=0; allEnemies[i].x!=NOCOORD; i++){ //converts all the trapped enemies into own's symbols
		theConverter(allEnemies[i],touch,player);
	}
	printBoard();
	player = !player;
	end = checkAllMoves(player);
	if(end){
		debug("END");
		mode = MENU;
		printFlag = 1;
		end = 0;
	}
	if(ai){
		play();
	}
}

void selectSq(Coord sq){
	BSP_LCD_SetTextColor(SELECTEDCLR);
	BSP_LCD_FillRect(toPos(sq.x)-SELECTEDDIF/2, toPos(sq.y)-SELECTEDDIF/2, SQSIZE+SELECTEDDIF, SQSIZE+SELECTEDDIF);
	BSP_LCD_SetTextColor(GRIDCLR);
	BSP_LCD_DrawRect(toPos(sq.x)-SELECTEDDIF/2, toPos(sq.y)-SELECTEDDIF/2, SQSIZE+SELECTEDDIF, SQSIZE+SELECTEDDIF);
	if(board[sq.x][sq.y]!=EMPTY){
		switch(board[sq.x][sq.y]){
			case PL1:
				BSP_LCD_SetTextColor(PL1CLR);
				break;
			case PL2:
				BSP_LCD_SetTextColor(PL2CLR);
				break;
			case E1:
				BSP_LCD_SetTextColor(E1CLR);
				break;
			case E2:
				BSP_LCD_SetTextColor(E2CLR);
		}
	BSP_LCD_FillCircle(toPos(sq.x)+SQSIZE/2.0, toPos(sq.y)+SQSIZE/2.0, CIRRAD);
	}
}

void colourButton(int btn, int btnClr, int txtClr){
	BSP_LCD_SetTextColor(btnClr);
	BSP_LCD_FillEllipse(LCDXCENTRE, LCDYMAX/(menuSize+1)*(btn+1), ELIPSEX, ELIPSEY);
	BSP_LCD_SetFont(&MFONT);
	BSP_LCD_SetTextColor(txtClr);
	BSP_LCD_SetBackColor(btnClr);
	BSP_LCD_DisplayStringAt(0,LCDYMAX/(menuSize+1)*(btn+1)-MFONTSIZE/2,(uint8_t *)menuOpt[btn],CENTER_MODE);
}

void printMenu(){
	for(int i=0; i<menuSize; i++){
		colourButton(i, BUTTONCLR, BUTTONTXTCLR);
	}
}

pPoint createSkirt(pPoint skirt){
	Point trip1;
	Point trip2;
	Point trip3;
	trip1.X = TRIP1X;
	trip1.Y = TRIP1Y;
	trip2.X = TRIP2X;
	trip2.Y = TRIP2Y;
	trip3.X = TRIP3X;
	trip3.Y = TRIP3Y;
	skirt[0] = trip1;
	skirt[1] = trip2;
	skirt[2] = trip3;
	return skirt;
}

void printMale(tcolour colour){
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_FillCircle(RHEADX, HEADY, HEADRAD);
	BSP_LCD_FillRect(RBODYX, BODYY, BODYWIDTH, BODYHEIGHT);
}

void printFemale(tcolour colour){
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_FillCircle(LHEADX, HEADY, HEADRAD);
	Point skirt[3];
	BSP_LCD_FillPolygon(createSkirt(skirt), 3);
}

void checkMenuTS(){
	tcolour touchClr;
	if(tsFlag){
		tsFlag = 0;
		touch.x = TS_State.touchX[0];
		touch.y = TS_State.touchY[0];
		touchClr = BSP_LCD_ReadPixel(touch.x, touch.y);
		if(touchClr==BUTTONCLR || touchClr==BUTTONTXTCLR){
			tsFlag = 0;
			btn = toButton(touch.y);
			colourButton(btn, PRESSEDBUTTONCLR, PRESSEDBUTTONTXTCLR);
			personFlag = 0;
			btnLeft = 1;
			dsFlag = 1;
		}else if(touchClr==BLUE || touchClr==PINK){
			personFlag = 1;
			btnLeft = 1;
			dsFlag = 1;
		}else if(btnLeft && touchClr==BCKGND){
			personFlag = 0;
			btnLeft = 0;
			colourButton(btn, BUTTONCLR, BUTTONTXTCLR);
			dsFlag = 0;
		}
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		dsFlag = 0;
		if(personFlag){
			if(touchClr==BLUE){
				if(touch.x<LCDXCENTRE){
					printFemale(PINK);
				}else{
					printMale(PINK);
				}
			}else if(touchClr==PINK){
				if(touch.x<LCDXCENTRE){
					printFemale(BLUE);
				}else{
					printMale(BLUE);
				}
			}
		}else{
			colourButton(btn, BUTTONCLR, BUTTONTXTCLR);
			mode = GAME;
			printFlag = 1;
		}
	}
}

void checkGameTS(){
	if(tsFlag){
		tsFlag = 0;
		touch.x = toIndex(TS_State.touchX[0]);
		touch.y = toIndex(TS_State.touchY[0]);
		if(touch.x>=0 && touch.y>=0 && touch.x<ROWS && touch.y<COLS){
			if(touch.x!=prev.x || touch.y!=prev.y){
				printBoard();
				selectSq(touch);
				dsFlag = 1;
				prev.x = touch.x;
				prev.y = touch.y;
			}
		}
		HAL_Delay(TOUCHDELAY);
	}else if(dsFlag){
		dsFlag = 0;
		play();
	}
}

void checkTIM(){
	if(timFlag){
		timFlag = 0;
		HAL_StatusTypeDef status=HAL_ADC_PollForConversion(&hadc1,TEMP_REFRESH_PERIOD);
		if(status==HAL_OK)
		{
			ConvertedValue=HAL_ADC_GetValue(&hadc1);
			JTemp = ((((ConvertedValue * VREF)/MAX_CONVERTED_VALUE) - VSENS_AT_AMBIENT_TEMP) * 10 / AVG_SLOPE) + AMBIENT_TEMP;
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf(desc, "\t%ld degrees Celsius", JTemp);
			BSP_LCD_SetTextColor(TEMPCLR);
			BSP_LCD_SetBackColor(BCKGND);
			BSP_LCD_SetFont(&TEMPFONT);
			BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)desc, RIGHT_MODE);
		}
	}
}





int main(void)
{
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
	MX_ADC1_Init();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND,LCD_FB_START_ADDRESS);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_TS_Init(800,480);
	BSP_TS_ITConfig();

	HAL_TIM_Base_Start_IT(&htim6);
	HAL_ADC_Start_IT(&hadc1);

	resetBoard();
	checkAllMoves(player);


	while (1)
	{
		checkTIM();
		switch(mode){
		case MENU:
			if(printFlag){
				BSP_LCD_Clear(BCKGND);
				printMenu();
				printMale(BLUE);
				printFemale(PINK);
				printFlag = 0;
			}
			checkMenuTS();
			break;
		case GAME:
			if(printFlag){
				BSP_LCD_Clear(BCKGND);
				printBoard();
				printFlag = 0;
			}
			checkGameTS();
		}
	}
}
