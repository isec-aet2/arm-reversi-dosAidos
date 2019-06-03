
#include "main.h"

#ifndef _CLCK_
#define _CLCK_

typedef uint32_t tcolour;

double toDegrees(double rad);
void analogClock(tcolour colour, int centreX);
void printCountdown(double sec, tcolour colour, int centreX);
void printClock(int centreX);
void resetClock();


#endif
