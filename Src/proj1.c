
#include "stdlib.h"
#include "proj1.h"
#include "main.h"


int chooseBest(int possMoves[], int nAvail){
	int max = 0;
	for(int i=0; i<nAvail; i++){
		if(possMoves[i]>max){
			max = possMoves[i];
		}
	}
	int bestMoves[nAvail];
	int ibm = 0;
	for(int i=0; i<nAvail; i++){
		if(possMoves[i]==max){
			bestMoves[ibm] = i;
			ibm++;
		}
	}
	int r = rand()%ibm;
	return 0;
	return possMoves[bestMoves[r]];
}

Coord chooseMove(Coord avail[], int nAvail, Coord targets[], _Bool player){
	int possMoves[nAvail];
	for(int i=0; i<nAvail; i++){
		int nEnemies = 0;
		int nDirec = findTargets(avail[i],player,targets);
		for(int j=0; j<nDirec; j++){
			nEnemies += findEnemies(targets[j],avail[i],player,0);
		}
		possMoves[i] = nEnemies;
	}
	int bestMove = chooseBest(possMoves,nAvail);
//	Coord c;
//	c.x = 1;
//	c.y = 1;
//	return c;
	return avail[bestMove];
}

int checkAllMoves(_Bool player, Coord avail[]){ //checks all possible moves
	int n = 0;
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(board[i][j] > BLUE){//== EMPTY{
            	Coord empty;
            	empty.x = i;
            	empty.y = j;
                if(checkEnemies(player,empty)){
                    board[empty.x][empty.y] = player+AVAILDIF;
                    avail[n] = empty;
                    n++;
                }else{
                	board[empty.x][empty.y] = EMPTY;
                }
            }
        }
    }
    return n;
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
        if (board[diag.x][diag.y] > BLUE) //returns false if after the enemy's pieces the next space is empty
            return FALSE;
        if (board[diag.x][diag.y] == player) //returns true if a player's piece is found
            return TRUE;
    }
}

int findTargets(Coord move, _Bool player, Coord targets[]){ //fills the allEnemies[] array
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
					targets[n] = enemy;
					n++;
				}
			}
		}
    }
    return n;
}

int findEnemies(Coord direction, Coord move, _Bool player, _Bool conv){ //converts the trapped enemy's pieces to player's pieces
    int nEnemies = 0;
    move.x += direction.x;
    move.y += direction.y;
    while(board[move.x][move.y] == !player){
		nEnemies++;
		if(conv){
			board[move.x][move.y] = player; //follows the enemy's direction until it reaches player's piece and convert all the pieces in between
			convertColour(move);
		}
		move.x += direction.x;
        move.y += direction.y;
        if(move.x<0 || move.x>=ROWS || move.y<0 || move.y>=COLS){
            break;
        }
    }
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



