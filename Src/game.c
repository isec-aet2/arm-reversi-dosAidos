
#include "game.h"



void swapPlayer(){
	game.player = !game.player;
	resetClocks();
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
