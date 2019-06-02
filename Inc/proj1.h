
#include "main.h"

#ifndef _Coord_
#define _Coord_

typedef struct _coord{
	int x;
	int y;
}Coord;

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

