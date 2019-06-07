
#include "main.h"

#ifndef _TC_
#define _TC_
typedef enum _content {PINK,BLUE,PINKAVAIL,BLUEAVAIL,EMPTY} tcontent;
#endif

#ifndef _GAME_
#define _GAME_

void resetBoard();
void playAI(Coord move);
void play();
void swapPlayer();
void initGame();
void endGame(tcontent winner);

#endif
