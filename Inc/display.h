
#include "main.h"

#ifndef _TYPES_
#define _TYPES_
typedef enum _side {LEFT, RIGHT} tside;
typedef enum _body {FEMALE,MALE} tbody;
#endif
#ifndef _TC_
#define _TC_
typedef enum _content {PINK,BLUE,PINKAVAIL,BLUEAVAIL,EMPTY} tcontent;
#endif

#ifndef _DISP_
#define _DISP_

void printFrame();
void printBoard();
void selectSq(Coord sq);
void deselectSq(Coord sq);
void colourButton(int btn, int btnClr, int txtClr);
void convertColour(Coord enemy);
void setMove(Coord move);
void printMenu();
void fillInfo();
void printInfo(_Bool headFlag, _Bool newMoveFlag);
pPoint createTimeLeft(pPoint timeLeft, int polp4x, int polp4y);
void printBody(tcolour colour, tside side, tbody body);
void initSkirt();
void printSnowBall();
void printEndMode(tcontent winner);
void printGameMode();
void printMenuMode();

#endif

