
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
	return avail[bestMove];
}

int checkAllMoves(_Bool player, Coord avail[]){
	int n = 0;
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(board[i][j] > BLUE){
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

_Bool checkEnemies(_Bool player, Coord empty){
   for(int i=-1; i<=1; i++){
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
                if(checkTrapped(player,empty,enemy)){
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

_Bool checkTrapped(_Bool player, Coord empty, Coord enemy){
    int mult;
    Coord diag;
    for(mult=1; 1 ; mult++) {
    	diag.x = mult*enemy.x+empty.x;
    	diag.y = mult*enemy.y+empty.y;
        if (diag.x<0 || diag.x>=ROWS || diag.y<0 || diag.y>=COLS )
            return FALSE;
        if (board[diag.x][diag.y] > BLUE)
            return FALSE;
        if (board[diag.x][diag.y] == player)
            return TRUE;
    }
}

int findTargets(Coord move, _Bool player, Coord targets[]){
    int n = 0;
    for(int i=-1; i<=1; i++){
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
				if(checkTrapped(player,move,enemy)){
					targets[n] = enemy;
					n++;
				}
			}
		}
    }
    return n;
}

int findEnemies(Coord direction, Coord move, _Bool player, _Bool conv){
    int nEnemies = 0;
    move.x += direction.x;
    move.y += direction.y;
    while(board[move.x][move.y] == !player){
		nEnemies++;
		if(conv){
			board[move.x][move.y] = player;
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

void resetArray(Coord array[], int size){
    for(int i=0; i<size; i++){
        array[i].x = NOCOORD;
    }
}

_Bool checkAvail(Coord move, Coord avail[]){
    for(int i=0; avail[i].x!=NOCOORD; i++){
        if(move.x==avail[i].x && move.y==avail[i].y){
            return TRUE;
        }
    }
    return FALSE;
}



