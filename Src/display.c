
#include "display.h"

#ifndef _TIME_
#define _TIME_
typedef struct _time{
	int sec;
	int min;
	int hour;
}Time;
#endif

void resetBoard(){
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			if((i==ROWS/2-1 && j==COLS/2-1) || (i==ROWS/2 && j==COLS/2)){
				board[i][j] = PINK;
			}else if((i==ROWS/2 && j==COLS/2-1) || (i==ROWS/2-1 && j==COLS/2)){
				board[i][j] = BLUE;
			}else{
				board[i][j] = EMPTY;
			}
		}
	}
	checkAllMoves(game.player,avail);
}

void printFrame(){
	BSP_LCD_SetTextColor(FRAMECLR);
	BSP_LCD_FillRect(BORDERX-SELECTEDDIF, BORDERY-SELECTEDDIF, SQSIZE*ROWS+2*SELECTEDDIF, SELECTEDDIF);
	BSP_LCD_FillRect(BORDERX-SELECTEDDIF, BORDERY-SELECTEDDIF, SELECTEDDIF, SQSIZE*COLS+2*SELECTEDDIF);
	BSP_LCD_FillRect(BORDERX-SELECTEDDIF, SQSIZE*COLS+BORDERY+1, SQSIZE*ROWS+2*SELECTEDDIF, SELECTEDDIF);
	BSP_LCD_FillRect(SQSIZE*ROWS+BORDERX+1, BORDERY-SELECTEDDIF, SELECTEDDIF, SQSIZE*COLS+2*SELECTEDDIF);
}

void printBoard(){
	tcontent sq;
	BSP_LCD_SetTextColor(BOARDCLR);
	BSP_LCD_FillRect(BORDERX, BORDERY, SQSIZE*ROWS, SQSIZE*COLS);
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			sq = board[i][j];
			BSP_LCD_SetTextColor(GRIDCLR);
			BSP_LCD_DrawRect(toPosX(i), toPosY(j), SQSIZE, SQSIZE);
			if(sq<=BLUE){
				BSP_LCD_SetTextColor(pieceClr[sq]);
				BSP_LCD_FillCircle(toPosX(i)+SQSIZE/2.0, toPosY(j)+SQSIZE/2.0, CIRRAD);
			}else if(sq==game.player+AVAILDIF){
				BSP_LCD_SetTextColor(pieceClr[sq]);
				BSP_LCD_DrawCircle(toPosX(i)+SQSIZE/2.0, toPosY(j)+SQSIZE/2.0, CIRRAD);
			}
		}
	}
	printFrame();
}

void fillInfo(){
	game.score[PINK] = game.score[BLUE] = game.nPossMoves[PINK] = game.nPossMoves[BLUE] = 0;
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			switch(board[i][j]){
			case PINK:
				game.score[PINK]++;
				break;
			case BLUE:
				game.score[BLUE]++;
				break;
			case PINKAVAIL:
				game.nPossMoves[PINK]++;
				break;
			case BLUEAVAIL:
				game.nPossMoves[BLUE]++;
			case EMPTY:
				break; //to avoid warnings
			}
		}
	}
	Time tTime = toTime(game.totalTime);
	Time pTime[] = {toTime(game.playerTime[PINK]),toTime(game.playerTime[BLUE])};
	sprintf(info1[0], "%s\t %.2d : %.2d : %.2d", templ1[0], tTime.hour, tTime.min, tTime.sec);
	for(int i=PINK; i<=BLUE; i++){
		sprintf(info2[0][i], "%.2d : %.2d", pTime[i].min, pTime[i].sec);
		sprintf(info2[1][i], "%d", game.score[i]);
		sprintf(info2[2][i], "%d", game.nPossMoves[i]);
		sprintf(info2[3][i], "%d", TIMEOUTMAX-game.nTimeOut[i]);
	}
}

void printInfo(_Bool templFlag){
	BSP_LCD_SetFont(&INFOFONT);
	BSP_LCD_SetBackColor(BCKGND);
	BSP_LCD_SetTextColor(INFOCLR);
	for(int i=NINFO1; i>0; i--){
		BSP_LCD_DisplayStringAt(0, LCDYMAX-INFOYBORDER*i, (uint8_t *)info1[i-1], CENTER_MODE);
	}
	for(int i=0; i<NINFO2; i++){
		for(int j=LEFT; j<=RIGHT; j++){
			if(templFlag){
				BSP_LCD_DisplayStringAt(infotX[j], YINFO+(i+1)*(LCDYMAX-YINFO)/(NINFO2+1), (uint8_t *)templ2[i], LEFT_MODE);
			}
			BSP_LCD_DisplayStringAt(infoX[j], YINFO+(i+1)*(LCDYMAX-YINFO)/(NINFO2+1), (uint8_t *)info2[i][j], RIGHT_MODE);
		}
	}
	templFlag = 0;
}

void selectSq(Coord sq){
	BSP_LCD_SetTextColor(SELECTEDCLR);
	BSP_LCD_FillRect(toPosX(sq.x)-SELECTEDDIF/2, toPosY(sq.y)-SELECTEDDIF/2, SQSIZE+SELECTEDDIF, SQSIZE+SELECTEDDIF);
	BSP_LCD_SetTextColor(GRIDCLR);
	BSP_LCD_DrawRect(toPosX(sq.x)-SELECTEDDIF/2, toPosY(sq.y)-SELECTEDDIF/2, SQSIZE+SELECTEDDIF, SQSIZE+SELECTEDDIF);
	if(board[sq.x][sq.y]!=EMPTY){
		BSP_LCD_SetTextColor(pieceClr[board[sq.x][sq.y]]);
	BSP_LCD_FillCircle(toPosX(sq.x)+SQSIZE/2.0, toPosY(sq.y)+SQSIZE/2.0, CIRRAD);
	}
}


void convertColour(Coord enemy){
	int sign;
	if(pieceClr[game.player]==PINKCLR){
		sign = 1;
	}
	if(pieceClr[game.player]==BLUECLR){
		sign = -1;
	}
	int inc = sign*CLRSPEED*(G-R);
	for(tcolour c=pieceClr[!game.player]; c-inc!=pieceClr[game.player]; c+=inc){
		if(sign*(signed int)pieceClr[game.player]+inc<(signed int)sign*c){
			c = pieceClr[game.player];
		}
		BSP_LCD_SetTextColor(c);
		BSP_LCD_FillCircle(toPosX(enemy.x)+SQSIZE/2, toPosY(enemy.y)+SQSIZE/2, CIRRAD);
		HAL_Delay(CLRDELAY);
	}
}

void colourButton(int btn, int btnClr, int txtClr){
	BSP_LCD_SetTextColor(btnClr);
	BSP_LCD_FillEllipse(LCDXCNTR, LCDYMAX/(menuSize+1)*(btn+1), ELIPSEX, ELIPSEY);
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

void printBody(tcolour colour, tside side, tbody body){
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_FillCircle(headX[side], HEADY, HEADRAD);
	if(body==FEMALE){
		BSP_LCD_FillPolygon(skirt[side], 3);
	}else{
		BSP_LCD_FillRect(bodyX[side], BODYY, BODYWIDTH, BODYHEIGHT);
	}
}

void initSkirt(){
	ltrip1.X = LTRIP1X;
	ltrip2.X = LTRIP2X;
	ltrip3.X = LTRIP3X;
	rtrip1.X = RTRIP1X;
	rtrip2.X = RTRIP2X;
	rtrip3.X = RTRIP3X;
	ltrip1.Y = rtrip1.Y = TRIP1Y;
	ltrip2.Y = rtrip2.Y = TRIP2Y;
	ltrip3.Y = rtrip3.Y = TRIP3Y;
	skirt[0][0] = ltrip1;
	skirt[0][1] = ltrip2;
	skirt[0][2] = ltrip3;
	skirt[1][0] = rtrip1;
	skirt[1][1] = rtrip2;
	skirt[1][2] = rtrip3;
}

