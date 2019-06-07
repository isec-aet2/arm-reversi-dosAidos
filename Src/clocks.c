
#include "clocks.h"


double toDegrees(double rad){
	return rad*M_PI/180;
}

void analogClock(tcolour colour, tside side){
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_FillCircle(clckcntrX[side], CLCKCNTRY, CLCKRAD);
	BSP_LCD_SetTextColor(CLCKBKG);
	BSP_LCD_FillRect(clckcntrX[side]-CLCKRAD, CLCKCNTRY-CLCKRAD, CLCKRAD, CLCKRAD);
	BSP_LCD_SetTextColor(CLCKFRAME);
	BSP_LCD_DrawCircle(clckcntrX[side], CLCKCNTRY, CLCKRAD+1);
	BSP_LCD_DrawCircle(clckcntrX[side], CLCKCNTRY, CLCKRAD+2);
}

void printCountdown(double sec, tcolour colour, tside side){
	double angle = 360*sec/TIMEOUTSEC;
	angle = toDegrees(angle);
	double catX = sin(angle)*CLCKRAD;
	double catY = cos(angle)*CLCKRAD;
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_DrawLine(clckcntrX[side], CLCKCNTRY, clckcntrX[side]+catX, CLCKCNTRY-catY);
}

void printClock(tside side){
	analogClock(CLCKBKG,clckcntrX[side]);
	for(double i=0; i<TIMEOUTSEC*DANGERFR; i+=CLCKSPEED){
		printCountdown(i,pieceClr[!game.player],clckcntrX[side]);
	}
	analogClock(DANGERCLR,clckcntrX[side]);
	for(double i=TIMEOUTSEC*(double)DANGERFR; i<TIMEOUTSEC; i+=CLCKSPEED){
		printCountdown(i,DANGERCLR,clckcntrX[side]);
	}
	analogClock(CLCKBKG,clckcntrX[side]);
}

void resetClocks(){
	clockAn = 0;
	analogClock(CLCKBKG,LEFT);
	analogClock(CLCKBKG,RIGHT);
}
