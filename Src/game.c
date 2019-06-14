
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
			printEndMode(PINK);
		}else if(game.score[BLUE] > game.score[PINK]){
			printEndMode(!game.player);
		}else if(game.playerTime[PINK] < game.playerTime[BLUE]){
			printEndMode(PINK);
		}else if(game.playerTime[BLUE] < game.playerTime[PINK]){
			printEndMode(BLUE);
		}else{
			printEndMode(!game.player);
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
				deselectSq(pass);
				pass.x += difX;
			}
		}
		if(pass.y!=move.y){
			if(!(rand()%3)){
				deselectSq(pass);
				pass.y += difY;
			}
		}
		selectSq(pass);
		HAL_Delay(AIDELAY);
	}
	deselectSq(pass);
	touch = pass;
	play();
}

void playNI(){
	if(checkGameTS()){
		play();
	}
}

void play(){
	board[touch.x][touch.y] = game.player;
	setMove(touch);
	resetArray(targets,8);
	findTargets(touch,game.player,targets);
	for(int i=0; targets[i].x!=NOCOORD; i++){
		findEnemies(targets[i],touch,game.player,CONVERT);
	}
	swapPlayer();
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
