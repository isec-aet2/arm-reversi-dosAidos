
#include "ai.h"
#include "main.h"

int chooseBest(int possMoves[], int nAvail){
	int max = 0;
	for(int i=0; i<nAvail; i++){
		if(possMoves[i]>max){
			max = possMoves[i];
		}
	}
	int bestMoves[nAvail];
	int ibm = 0;
	for(int i=0; i<nAvail; i++){
		if(possMoves[i]==max){
			bestMoves[ibm] = i;
			ibm++;
		}
	}
	int r = rand()%ibm;
	return possMoves[bestMoves[r]];
}

Coord chooseMove(Coord avail[], int nAvail, Coord allEnemies[], _Bool player){
	int nEnemies;
	int possMoves[nAvail];
	int bestMove;
	for(int i=0; i<nAvail; i++){
		nEnemies = exposeAllEnemies(avail[i],player,allEnemies);
		for(int j=0; j<nEnemies; j++){
			nEnemies *= theConverter(allEnemies[j],avail[i],player);
		}
		possMoves[i] = nEnemies;
	}
	bestMove = chooseBest(possMoves,nAvail);
	return allEnemies[bestMove];
}
