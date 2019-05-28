
#include "ai.h"
#include "main.h"

int chooseBest(){
	int r = rand();
}

Coord chooseMove(Coord avail[], int nAvail, Coord allEnemies[], _Bool player){
	int nEnemies;
	int bestMoves[nAvail];
	for(int i=0; i<nAvail; i++){
		nEnemies = exposeAllEnemies(avail[i],player,allEnemies);
		for(int j=0; j<nEnemies; j++){
			nEnemies *= theConverter(allEnemies[j],avail[i],player);
		}
		bestMoves[i] = nEnemies;
	}
}
