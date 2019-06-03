
#include "display.h"

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
	Content sq;
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
			}else if(sq==game.player+EDIF){
				BSP_LCD_SetTextColor(pieceClr[sq]);
				BSP_LCD_DrawCircle(toPosX(i)+SQSIZE/2.0, toPosY(j)+SQSIZE/2.0, CIRRAD);
			}
		}
	}
	printFrame();
}

//void printInfo(){
//	sprintf(game.)
//}

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
	int inc = sign*(G-R)*CLRSPEED;
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

