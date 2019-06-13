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
//#include "fatfs.h"


#include "configs.h"
#include "proj1.h"
#include "interrupts.h"
#include "clocks.h"
#include "converters.h"
#include "display.h"
#include "game.h"

#ifndef _ST_
#define _ST_
typedef struct _coord{
	int x;
	int y;
}Coord;
#endif

int timCount = 0;
_Bool timFlag = 0;
_Bool pbFlag = 0;
_Bool tsFlag = 0;
_Bool dsFlag = 0;
_Bool btnLeft = 0;
int personFlag = 0;
int menuSize = ORIGOPT;
TS_StateTypeDef TS_State;
tmode mode = MENU;
int iAI;
_Bool aiFlag;
_Bool ai2Flag;
_Bool printFlag = 1;
tcontent board[ROWS][COLS];
Coord touch;
Coord prev;
int btn;
Coord avail[ROWS*COLS];
Coord targets[8];
int remain = 0;
_Bool configFlag = 0;
_Bool newGame = 1;
_Bool clockFlag = 0;
double clockAn = 0;
_Bool redFlag = 0;
tcolour touchClr;
int iClr = 0;
tbody bodyDisp[] = {FEMALE,MALE};
tside thisSide = 0;

Game game;
//game.totalTime = 0;

tcolour pieceClr[] = {PINKCLR,BLUECLR,PINKAVAILCLR,BLUEAVAILCLR};
char menuOpt[][STRSIZE] = {"NI vs AI","NI vs NI","AI vs AI","Resume game"};

uint32_t convertedValue;
long int degrees;
char temp[STRSIZE];

int clckcntrX[] = {LCLCKCNTRX,RCLCKCNTRX};
int headX[] = {LHEADX,RHEADX};
int bodyX[] = {LBODYX,RBODYX};
int infotX[] = {LINFOT,RINFOT};
int infoX[] = {LINFO,RINFO};

char info1[NINFO1][STRSIZE];
char info2[NINFO2][2][STRSIZE];
char head1[NINFO1][STRSIZE] = {"Game's total time:"};
char head2[NINFO2][STRSIZE] = {"Player:","Total time:","Score:","Possible moves:","Timeouts left:"};

Point ltrip1;
Point ltrip2;
Point ltrip3;
Point rtrip1;
Point rtrip2;
Point rtrip3;
Point skirt[2][3];



void debug(char * text){
	BSP_LCD_SetBackColor(BCKGND);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, LCDYMAX/2, (uint8_t *)text, RIGHT_MODE);
}

//void writeSD (){
//	fillInfo();
//	unsigned int nBytes;
//	char info[STRSIZE*(NINFO1*2+NINFO2*2*2)];
//	info[0] = 0;
//	char endl13[] = {13,0};
//	char endl10[] = {10,0};
//	for(int i=0; i<NINFO1; i++){
//		strcat(info,head1[i]);
//		strcat(info,info1[i]);
//		strcat(info,endl13);
//		strcat(info,endl10);
//	}
//	for(int i=0; i<NINFO2; i++){
//		for(int j=PINK; i<=BLUE; j++){
//			strcat(info,head2[i]);
//			strcat(info,info2[j][i]);
//			strcat(info,endl13);
//			strcat(info,endl10);
//		}
//	}
//	if(f_mount (&SDFatFS, SDPath, 0)!=FR_OK){
//	   Error_Handler();
//	}
//	HAL_Delay(100);
//	if(f_open (&SDFile, "end.txt", FA_OPEN_APPEND | FA_WRITE )!=FR_OK){
//	   Error_Handler();
//	}
//	HAL_Delay(100);
//	if(f_write (&SDFile, info, strlen(info), &nBytes)!=FR_OK){
//	   Error_Handler();
//	}
//	f_close (&SDFile);
//}

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
	//MX_FATFS_Init();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND,LCD_FB_START_ADDRESS);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_PB_Init(0,1);

	BSP_TS_Init(LCDXCNTR*2,LCDYMAX);
	BSP_TS_ITConfig();

	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_ADC_Start_IT(&hadc1);

	BSP_LED_Init(LED_RED);
	BSP_LED_Init(LED_GREEN);

	initSkirt();

	configFlag = 1;
}


int main(){

	configs();

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
				printBody(PINKCLR, LEFT, FEMALE);
				printBody(BLUECLR, RIGHT, MALE);
				printFlag = 0;
			}
			checkMenuTS();
			break;
		case GAME:
			if(printFlag){
				BSP_LCD_Clear(BCKGND);
				initGame();
				resetBoard();
				printBoard();
				resetClocks();
				fillInfo();
				printInfo(HEAD,NEWMOVE);
				printFlag = 0;
			}
			if(game.player==iAI || ai2Flag){
				touch = chooseMove(avail,remain,targets,game.player);
				playAI(touch);
			}else{
				playNI();
			}
			break;
		case END:
			checkEndTS();
		case NONE:
			break;
			//included to avoid warnings
		}
	}
}
