
#include "game.h"


void swapPlayer(){
	game.player = !game.player;
	resetClocks();
	remain = checkAllMoves(game.player,avail);
	fillInfo();
	printInfo(!HEAD,NEWMOVE);
	printBoard();
	if(!remain){
		remain = 0;
		if(game.score[PINK] > game.score[BLUE]){
			endGame(PINK);
		}else if(game.score[BLUE] > game.score[PINK]){
			endGame(!game.player);
		}else if(game.playerTime[PINK] < game.playerTime[BLUE]){
			endGame(PINK);
		}else if(game.playerTime[BLUE] < game.playerTime[PINK]){
			endGame(BLUE);
		}else{
			endGame(!game.player);
		}
	}
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
		checkPB();
		checkTIM6();
		checkTIM7();
		fillInfo();
		printInfo(0,0);
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

void playNI(){
	if(checkGameTS()){
		board[touch.x][touch.y] = game.player;
	}
}

void play(){
	checkTIM6();
	checkTIM7();
	if(checkPB()){
		//return;
	}
	board[touch.x][touch.y] = game.player;
	setMove();
	resetArray(targets,8);
	findTargets(touch,game.player,targets);
	for(int i=0; targets[i].x!=NOCOORD; i++){
		findEnemies(targets[i],touch,game.player,1);
	}
	swapPlayer();
	if((aiFlag && game.player==iAI) || ai2Flag){
		touch = chooseMove(avail,remain,targets,game.player);
		playAI(touch);
		play();
	}
}

void initGame(){
	game.totalTime = 0;
	game.playerTime[PINK] = game.playerTime[BLUE] = 0;
	strcpy(game.playerName[PINK], "Pink");
	strcpy(game.playerName[BLUE], "Blue");
	game.score[PINK] = game.score[BLUE] = 2;
	game.nPossMoves[PINK] = game.nPossMoves[BLUE] = 4;
	game.nTimeOut[PINK] = game.nTimeOut[BLUE] = 0;
	game.player = rand()%2;
}

void endGame(tcontent winner){
	mode = END;
	BSP_LCD_Clear(pieceClr[winner]);
	BSP_LCD_SetBackColor(pieceClr[winner]);
	BSP_LCD_SetFont(&ENDFONT);
	BSP_LCD_SetTextColor(pieceClr[winner+AVAILDIF]);
	BSP_LCD_DisplayStringAt(0, LCDYMAX/2, (uint8_t*)"GAME OVER", CENTER_MODE);
}
