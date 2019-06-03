
#include "converters.h"


int toPosX(int index){
	return SQSIZE*index+BORDERX;
}

int toPosY(int index){
	return SQSIZE*index+BORDERY;
}

int toIndexX(int pos){
	return (pos-BORDERX)/SQSIZE;
}

int toIndexY(int pos){
	return (pos-BORDERY)/SQSIZE;
}

int toButton(int posY){
	return posY*menuSize/LCDYMAX;
}
