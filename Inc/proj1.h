
#include "main.h"


#ifndef _ST_
#define _ST_

typedef struct _coord{
	int x;
	int y;
}Coord;

//typedef struct _time{
//	int sec;
//	int min;
//	int hour;
//}Time;
//
//typedef struct _game{
//	Time totalTime;
//	Time playerTime[2];
//	char playerName[2][STRSIZE];
//	int score[2];
//	int nPossMoves[2];
//	int nTimeOut[2];
//	Content player;
//}Game;

#endif

#ifndef _R_
#define _R_

int checkAllMoves(_Bool player, Coord avail[]);
_Bool checkEnemies(_Bool player, Coord empty);
_Bool checkTrapped(_Bool player, Coord empty, Coord enemy);
int exposeAllEnemies(Coord move, _Bool player, Coord allEnemies[]);
int theConverter(Coord direction, Coord move, _Bool player, _Bool ai);
void resetArray(Coord array[], int size);
_Bool checkAvail(Coord move, Coord avail[]);


int chooseBest(int possMoves[], int nAvail);
Coord chooseMove(Coord avail[], int nAvail, Coord allEnemies[], _Bool player);

#endif

