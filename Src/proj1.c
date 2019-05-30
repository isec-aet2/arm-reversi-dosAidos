
#include "proj1.h"

#define ROWS 					 8
#define COLS 					 8
#define NOCOORD 				-2
#define EMPTY					-1
#define PL1						 0
#define PL2						 1
#define E1						 2
#define E2						 3
#define TRUE 					 1
#define FALSE 					 0

int board[ROWS][COLS];

_Bool checkAllMoves(_Bool player){ //checks all possible moves
	int noMoves = TRUE;
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(board[i][j] == EMPTY){
            	Coord empty;
            	empty.x = i;
            	empty.y = j;
                if(checkEnemies(player,empty)){
                    board[empty.x][empty.y] = E1; //if checkEnemies is true, the move will be stores in avail[]
                    if(player) board[empty.x][empty.y] = E2;
                    noMoves = FALSE;
                }
            }
        }
    }
    return noMoves;
}

_Bool checkEnemies(_Bool player, Coord empty){ //checks if there are enemies nearby the player's move
   for(int i=-1; i<=1; i++){ //avoids crossing the matrix's borders
       if(empty.x+i>=ROWS || empty.x+i<0){
           continue;
       }
       for(int j=-1; j<=1; j++){
           if(empty.y+j>=COLS || empty.y+j<0){
                continue;
            }
            if(board[empty.x+i][empty.y+j] == !player){
            	Coord enemy;
            	enemy.x = i;
            	enemy.y = j;
                if(checkTrapped(player,empty,enemy)){ //checks if in each direction, if the enemy's pieces are trapped
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

_Bool checkTrapped(_Bool player, Coord empty, Coord enemy){ //checks if the enemy pieces are trapped
    int mult;
    Coord diag;
    for(mult=1; 1 ; mult++) {
    	diag.x = mult*enemy.x+empty.x;
    	diag.y = mult*enemy.y+empty.y;
        if (diag.x<0 || diag.x>=ROWS || diag.y<0 || diag.y>=COLS ) //stops from crossing the matrix's borders
            return FALSE;
        if (board[diag.x][diag.y] == EMPTY) //returns false if after the enemy's pieces the next space is empty
            return FALSE;
        if (board[diag.x][diag.y] == player) //returns true if a player's piece is found
            return TRUE;
    }
}

int exposeAllEnemies(Coord move, _Bool player, Coord allEnemies[]){ //fills the allEnemies[] array
    int n = 0;
    for(int i=-1; i<=1; i++){ //checks if we don't cross the matrix's borders
		if(move.x+i>=ROWS || move.x+i<0){
		   continue;
		}
		for(int j=-1; j<=1; j++){
		   if(move.y+j>=COLS|| move.y+j<0){
				continue;
			}
			if(board[move.x+i][move.y+j] == !player){
				Coord enemy;
				enemy.x = i;
				enemy.y = j;
				if(checkTrapped(player,move,enemy)){ //checks if the enemies are trapped, and, if so, stores the coordinates' differences from the move in the allEnemies array
					allEnemies[n] = enemy;
					n++;
				}
			}
		}
    }
    return n;
}

int theConverter(Coord direction, Coord move, _Bool player){ //converts the trapped enemy's pieces to player's pieces
    int nEnemies = 0;
	do{
		nEnemies++;
        board[move.x][move.y] = player; //follows the enemy's direction until it reaches player's piece and convert all the pieces in between
        move.x += direction.x;
        move.y += direction.y;
        if(move.x<0 || move.x>=ROWS || move.y<0 || move.y>=COLS){
            break;
        }
    }while(board[move.x][move.y] == !player);
	return nEnemies;
}

void resetArray(Coord array[], int size){ //cleans the allEnemies[] array
    for(int i=0; i<size; i++){
        array[i].x = NOCOORD;
    }
}

_Bool checkAvail(Coord move, Coord avail[]){ //checks is the player's move is in the avail[] array
    for(int i=0; avail[i].x!=NOCOORD; i++){
        if(move.x==avail[i].x && move.y==avail[i].y){
            return TRUE;
        }
    }
    return FALSE;
}

void proj1Main(_Bool player){
	Coord move;
    Coord avail[ROWS*COLS];//create array of available moves and set it to all zeros
    resetArray(avail,ROWS*COLS);
    checkAllMoves();//checks all the possible moves and stores them in the avail[] array
	if(avail[0].x==NOCOORD){ // ends the game when there are no move available moves to play
		return;
	}
    /*int move = setMove( board, symbol, name, player, avail); //receives input from user and checks if it is one of the available options
    moveToIndexes(move,board,&row,&col,player); //converts move given by player to board coordinates */
    Coord allEnemies[8]; //creates array with the directions(differences between enemy coordinates and the move's coordinates) of all the enemies nearby
    resetArray(allEnemies,8); //clears all elements from the allEnemies array
    exposeAllEnemies(move,player,allEnemies); //checks all the directions where convertion is needed and stores it in the allEnemies[] array
    for(int i=0; allEnemies[i].x!=NOCOORD; i++){ //converts all the trapped enemies into own's symbols
        theConverter(allEnemies[i],move,player);
    }
    player=!player; //changes turns of players
}



