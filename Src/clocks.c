
#include "clocks.h"

double toDegrees(double rad){
	return rad*M_PI/180;
}

void analogClock(tcolour colour, int centreX){
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_FillCircle(centreX, CLCKCNTRY, CLCKRAD);
	BSP_LCD_SetTextColor(CLCKBKG);
	BSP_LCD_FillRect(centreX-CLCKRAD, CLCKCNTRY-CLCKRAD, CLCKRAD, CLCKRAD);
	BSP_LCD_SetTextColor(CLCKFRAME);
	BSP_LCD_DrawCircle(centreX, CLCKCNTRY, CLCKRAD+1);
	BSP_LCD_DrawCircle(centreX, CLCKCNTRY, CLCKRAD+2);
}

void printCountdown(double sec, tcolour colour, int centreX){
	double angle = 360*sec/TIMEOUTSEC;
	angle = toDegrees(angle);
	double catX = sin(angle)*CLCKRAD;
	double catY = cos(angle)*CLCKRAD;
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_DrawLine(centreX, CLCKCNTRY, centreX+catX, CLCKCNTRY-catY);
}

void printClock(int centreX){
	analogClock(CLCKBKG,centreX);
	for(double i=0; i<TIMEOUTSEC*DANGERFR; i+=CLCKSPEED){
		printCountdown(i,pieceClr[!game.player],centreX);
	}
	analogClock(DANGERCLR,centreX);
	for(double i=TIMEOUTSEC*(double)DANGERFR; i<TIMEOUTSEC; i+=CLCKSPEED){
		printCountdown(i,DANGERCLR,centreX);
	}
	analogClock(CLCKBKG,centreX);
}

void resetClock(){
	clockAn = 0;
	analogClock(CLCKBKG,LCLCKCNTRX);
	analogClock(CLCKBKG,RCLCKCNTRX);
}
