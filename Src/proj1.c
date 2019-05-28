
#include "proj1.h"


void checkAllMoves(_Bool player, Coord avail[]){ //checks all possible moves
    int n=0;
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(board[i][j] == EMPTY){
            	Coord empty;
            	empty.x = i;
            	empty.y = j;
                if(checkEnemies(player,empty)){
                    avail[n] = empty; //if checkEnemies is true, the move will be stores in avail[]
                    n++;
                }
            }
        }
    }
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

void exposeAllEnemies(Coord move, _Bool player, Coord allEnemies[]){ //fills the allEnemies[] array
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
}

void theConverter(Coord direction, Coord move, _Bool player){ //converts the trapped enemy's pieces to player's pieces
    do{
        board[move.x][move.y] = player; //follows the enemy's direction until it reaches player's piece and convert all the pieces in between
        move.x += direction.x;
        move.y += direction.y;
        if(move.x<0 || move.x>=ROWS || move.y<0 || move.y>=COLS){
            break;
        }
    }while(board[move.x][move.y] == !player);

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
    checkAllMoves(player,avail);//checks all the possible moves and stores them in the avail[] array
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



