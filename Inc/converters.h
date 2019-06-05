
#include "main.h"

#ifndef _TIME_
#define _TIME_
typedef struct _time{
	int sec;
	int min;
	int hour;
}Time;
#endif

#ifndef _CONV_
#define _CONV_

int toPosX(int index);
int toPosY(int index);
int toIndexX(int pos);
int toIndexY(int pos);
int toButton(int posY);
Time toTime(int total);


#endif
