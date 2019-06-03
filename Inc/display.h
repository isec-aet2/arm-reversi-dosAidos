
#include "main.h"

#ifndef _DISP_
#define _DISP_

void printFrame();
void printBoard();
void selectSq(Coord sq);
void colourButton(int btn, int btnClr, int txtClr);
void convertColour(Coord enemy);
void printMenu();
pPoint createSkirt(pPoint skirt);
pPoint createTimeLeft(pPoint timeLeft, int polp4x, int polp4y);
void printMale(tcolour colour);
void printFemale(tcolour colour);

#endif
