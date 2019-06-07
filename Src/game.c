
#include "game.h"



void swapPlayer(){
	game.player = !game.player;
	resetClocks();
	fillInfo();
	printInfo(0);
	printBoard();
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
	resetArray(targets,8);
	findTargets(touch,game.player,targets);
	for(int i=0; targets[i].x!=NOCOORD; i++){
		findEnemies(targets[i],touch,game.player,1);
	}
	swapPlayer();
	remain = checkAllMoves(game.player,avail);
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
	if((ai && game.player==ai) || ai2){
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
	game.player = PINK;
}

void endGame(tcontent winner){
	mode = MENU;
	menuSize = ORIGOPT;
	printFlag = 1;
}
