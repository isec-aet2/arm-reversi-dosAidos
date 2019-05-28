
#ifndef _R_
#define _R_

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

#include "main.h"


#ifndef _Coord_
#define _Coord_

typedef struct _coord{
	int x;
	int y;
}Coord;

#endif

int checkAllMoves(_Bool player, Coord avail[]);
_Bool checkEnemies(_Bool player, Coord empty);
_Bool checkTrapped(_Bool player, Coord empty, Coord enemy);
int exposeAllEnemies(Coord move, _Bool player, Coord allEnemies[]);
int theConverter(Coord direction, Coord move, _Bool player);
void resetArray(Coord array[], int size);
_Bool checkAvail(Coord move, Coord avail[]);
void proj1Main(_Bool player);

#endif
