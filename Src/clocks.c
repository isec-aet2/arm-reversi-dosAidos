
#include "clocks.h"


double toDegrees(double rad){
	return rad*M_PI/180;
}

void analogClock(tcolour colour, tside side, _Bool danger){
	BSP_LCD_SetTextColor(colour);
	BSP_LCD_FillCircle(clckcntrX[side], CLCKCNTRY, CLCKRAD);
	if(danger){
		BSP_LCD_SetTextColor(CLCKBKG);
		BSP_LCD_FillRect(clckcntrX[side]-CLCKRAD, CLCKCNTRY-CLCKRAD, CLCKRAD, CLCKRAD);
	}
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

void resetClocks(){
	clockAn = 0;
	analogClock(CLCKBKG,LEFT,!DANGER);
	analogClock(CLCKBKG,RIGHT,!DANGER);
}
