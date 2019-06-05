#ifndef _TYPES_
#define _TYPES_
typedef enum _side {LEFT, RIGHT} tside;
typedef enum _body {FEMALE,MALE} tbody;
#endif

#include "main.h"

#ifndef _CLCK_
#define _CLCK_

typedef uint32_t tcolour;


double toDegrees(double rad);
void analogClock(tcolour colour, tside side);
void printCountdown(double sec, tcolour colour, tside side);
void printClock(tside side);
void resetClocks();


#endif
